/* Genode includes */
#include <base/allocator_avl.h>
#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <base/heap.h>
#include <base/log.h>
#include <block_session/connection.h>
#include <os/reporter.h>
#include <timer_session/connection.h>
#include <util/string.h>


namespace Block_connection_test {
    using namespace Genode;

    struct Scratch_buffer;
    struct Main;
    struct Read_write;
    struct Random;

    struct Test_failed : Exception {};

    struct Constructing_test_failed : Exception {};
}

namespace Util {
    using namespace Genode;

    /*
     * Xoroshiro128+ written in 2014-2016 by Sebastiano Vigna (vigna@acm.org)
     *
     * (see http://xoroshiro.di.unimi.it/xorshift128plus.c and
     *      http://xoroshiro.di.unimi.it/splitmix64.c)
     */

    struct Xoroshiro
    {
        uint64_t seed;

        uint64_t splitmix64() {
            uint64_t z = (seed += 0x9E3779B97F4A7C15ULL);
            z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
            z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
            return z ^ (z >> 31);
        }

        Xoroshiro(uint64_t seed) : seed(seed) {
            s[0] = splitmix64();
            s[1] = splitmix64();
        }

        uint64_t s[2];

        static uint64_t rotl(uint64_t const x, int k) {
            return (x << k) | (x >> (64 - k));
        }

        uint64_t get() {
            uint64_t const s0 = s[0];
            uint64_t s1 = s[1];
            uint64_t const result = s0 + s1;

            s1 ^= s0;

            s[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14);
            s[1] = rotl(s1, 36);

            return result;
        }
    };
}

class Block_connection_test::Scratch_buffer
{
    private:
        Allocator &_alloc;

        Scratch_buffer(Scratch_buffer const &);

        Scratch_buffer &operator =(Scratch_buffer const &);

    public:
        char *const base;
        size_t const size;

        Scratch_buffer(Allocator &alloc, size_t size)
            : _alloc(alloc), base((char *) alloc.alloc(size)), size(size) {}

        ~Scratch_buffer() { destroy(&_alloc, base); }
};

struct Block_connection_test::Read_write : private Genode::Fifo<Read_write>::Element
{
    protected:
        Env &_env;
        Allocator &_alloc;

        Xml_node const _node;

        using block_number_t = Block::block_number_t;

        size_t const _io_buffer;
        size_t const _batch;

        Allocator_avl _block_alloc{&_alloc};

        struct Job;
        using Block_connection = Block::Connection<Job>;

        Constructible<Block_connection> _block{};

        struct Job : Block_connection::Job
        {
            unsigned const id;

            Job(Block_connection &connection, Block::Operation operation, unsigned id)
                : Block_connection::Job(connection, operation), id(id) {}
        };

        /*
         * Must be called by every test when it has finished
         */
        Genode::Signal_context_capability _finished_sig;

        void finish() {
            if (_finished_sig.valid()) {
                Genode::Signal_transmitter(_finished_sig).submit();
            }
        }

        Block::Session::Info _info{};

        size_t _size_in_blocks{0};

        size_t _bytes{0};
        uint64_t _rx{0};
        uint64_t _tx{0};
        unsigned _job_cnt{0};
        unsigned _completed{0};

        Scratch_buffer &_scratch_buffer;

        void _memcpy(char *dst, char const *src, size_t length) {
            if (length > _scratch_buffer.size) {
                warning("scratch buffer too small for copying");
                return;
            }

            Genode::memcpy(dst, src, length);
        }

        /* Used to initialise the Random component */
        Util::Xoroshiro _random;
        size_t const _size = _node.attribute_value("size", Number_of_bytes());
        uint64_t const _length = _node.attribute_value("length", Number_of_bytes());

        const char * _xor_with_int(Genode::String<256> input, char * result, int xor_value) {
            for (Genode::size_t i = 0; i < input.length(); ++i) {
                result[i] = static_cast<char>(input.string()[i] ^ xor_value);
            }

            return result;
        }

    public:
        /**
         * Block::Connection::Update_jobs_policy
         */
        void produce_write_content(Job &job, Block::seek_off_t offset, char *dst, size_t length) {
            _tx += length / _info.block_size;
            _bytes += length;

            log("job ", job.id, ": writing ", length, " bytes at ", offset);

            Genode::String<256> text = "ethz";
            char encrypted_message[256] = "";
            _xor_with_int(text, encrypted_message, 1234);

            log("encrypted: '", Cstring(encrypted_message), "'");
            _memcpy(dst, encrypted_message, text.length());
            // _memcpy(dst, _scratch_buffer.base, length);
        }

        /**
         * Block::Connection::Update_jobs_policy
         */
        void consume_read_result(Job &job, Block::seek_off_t offset,
                                 char const *src, size_t length) {
            _rx += length / _info.block_size;
            _bytes += length;
            log("job ", job.id, ": got ", length, " bytes at ", offset);

            char result[256] = "";
            _xor_with_int(Cstring(src), result, 1234);
            log("decrypted: '", Cstring(result), "'");
        }

        /**
         * Block_connection::Update_jobs_policy
         */
        void completed(Job &job, bool success) {
            _completed++;
            log("job ", job.id, ": ", job.operation(), ", completed");

            if (!success)
                error("processing ", job.operation(), " failed");

            destroy(_alloc, &job);

            /* replace completed job by new one */
            //_spawn_job();

            bool const jobs_active = (_job_cnt != _completed);

            if (!jobs_active || !success)
                finish();
        }

    protected:

        void _handle_block_io() {
            _block->update_jobs(*this);
        }

        Signal_handler<Read_write> _block_io_sigh{
            _env.ep(), *this, &Read_write::_handle_block_io
        };

    public:
        friend class Genode::Fifo<Read_write>;

        Read_write(Env &env, Allocator &alloc, Xml_node node,
                   Signal_context_capability finished_sig,
                   Scratch_buffer &scratch_buffer)
            : _env(env), _alloc(alloc), _node(node),
              _io_buffer(_node.attribute_value("io_buffer",
                                               Number_of_bytes(4 * 1024 * 1024))),
              _batch(_node.attribute_value("batch", 1u)),
              _finished_sig(finished_sig),
              _scratch_buffer(scratch_buffer),
              _random(_node.attribute_value("seed", 42UL)) {

        }

        virtual ~Read_write() {};

        void start() {
            _block.construct(_env, &_block_alloc, _io_buffer);
            _block->sigh(_block_io_sigh);
            _info = _block->info();

            _init();

            for (unsigned i = 0; i < _batch; i++)
                _spawn_job();

            _handle_block_io();
        }

        block_number_t _next_block() {
            uint64_t r = 0;
            block_number_t max = _info.block_count;
            if (max >= _size_in_blocks + 1)
                max -= _size_in_blocks + 1;
            do {
                r = _random.get() % max;
            } while (r + _size_in_blocks > _info.block_count);

            return r;
        }

        void _init() {
            if (_size > _scratch_buffer.size) {
                error("request size exceeds scratch buffer size");
                throw Constructing_test_failed();
            }

            if (!_size || !_length) {
                error("request size or length invalid");
                throw Constructing_test_failed();
            }

            if (_info.block_size > _size || (_size % _info.block_size) != 0) {
                error("request size invalid ", _info.block_size, " ", _size);
                throw Constructing_test_failed();
            }

            _size_in_blocks = _size / _info.block_size;
        }

        void _spawn_job() {
            if (_bytes >= _length)
                return;

            _job_cnt++;

            block_number_t const lba = _next_block();

            Block::Operation const write_op {
                .type = Block::Operation::Type::WRITE,
                .block_number = lba,
                .count = _size_in_blocks
            };

            new(_alloc) Job(*_block, write_op, _job_cnt);

            _job_cnt++;

            Block::Operation const read_op {
                .type = Block::Operation::Type::READ,
                .block_number = lba,
                .count = _size_in_blocks
            };
            new(_alloc) Job(*_block, read_op, _job_cnt);
        }

        char const *name() const { return "random"; }

        void print(Output &out) const {
            Genode::print(out, name(), " "
                          "size:", _size, " "
                          "length:", _length, " "
                          "batch:", _batch);
        }
};

struct Block_connection_test::Main
{
    Genode::Env &_env;
    Genode::Heap _heap{_env.ram(), _env.rm()};

    Genode::Attached_rom_dataspace _config_rom{_env, "config"};

    Genode::Number_of_bytes const _scratch_buffer_size{
        _config_rom.xml().attribute_value("scratch_buffer_size",
                                          Genode::Number_of_bytes(1U << 20))
    };

    Genode::Fifo<Read_write> _tests{};

    Read_write *_current{nullptr};

    bool _success{true};

    void _handle_finished() {
        /* clean up current test */
        if (_current) {
            Genode::log("finished ", _current->name());

            Genode::destroy(&_heap, _current);
            _current = nullptr;
        }

        /* execute next test */
        if (!_current) {
            _tests.dequeue([&](Read_write &head) {
                Genode::log("start ", head);
                try {
                    head.start();
                    _current = &head;
                } catch (...) {
                    Genode::log("Could not start ", head);
                    Genode::destroy(&_heap, &head);
                    throw;
                }
            });
        }

        if (!_current) {
            /* execution is finished */
            Genode::log("--- all tests finished ---");
            _env.parent().exit(_success ? 0 : 1);
        }
    }

    Genode::Signal_handler<Main> _finished_sigh{
        _env.ep(), *this, &Main::_handle_finished
    };

    Scratch_buffer _scratch_buffer{_heap, _scratch_buffer_size};

    void _construct_tests(Genode::Xml_node config) {
        try {
            Genode::Xml_node tests = config.sub_node("tests");
            tests.for_each_sub_node([&](Genode::Xml_node node) {
                if (node.has_type("random")) {
                    auto *t = new(&_heap)
                            Read_write(_env, _heap, node, _finished_sigh, _scratch_buffer);
                    _tests.enqueue(*t);
                }
            });
        } catch (...) { Genode::error("invalid tests"); }
    }

    /**
     * Constructor
     */
    Main(Genode::Env &env) : _env(env) {
        try {
            _construct_tests(_config_rom.xml());
        } catch (...) { throw; }

        Genode::log("--- start tests ---");

        /* initial kick-off */
        _handle_finished();
    }

    ~Main() {}

    private:
        Main(const Main &) = delete;

        Main &operator=(const Main &) = delete;
};

void Component::construct(Genode::Env &env) {
    static Block_connection_test::Main main(env);
}
