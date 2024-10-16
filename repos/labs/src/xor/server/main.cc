/* Genode includes */
#include <base/allocator_avl.h>
#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <base/heap.h>
#include <base/log.h>
#include <block_session/connection.h>
#include <os/reporter.h>
#include <timer_session/connection.h>


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

        bool const _verbose;
        size_t const _io_buffer;
        uint64_t const _progress_interval;
        bool const _copy;
        size_t const _batch;

        Constructible<Timer::Connection> _timer{};

        Constructible<Timer::Periodic_timeout<Read_write> > _progress_timeout{};

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
            _end_time = _timer->elapsed_ms();

            _finished = true;
            if (_finished_sig.valid()) {
                Genode::Signal_transmitter(_finished_sig).submit();
            }

            _timer.destruct();
        }

        Block::Session::Info _info{};

        size_t _length_in_blocks{0};
        size_t _size_in_blocks{0};

        uint64_t _start_time{0};
        uint64_t _end_time{0};
        size_t _bytes{0};
        uint64_t _rx{0};
        uint64_t _tx{0};
        size_t _triggered{0}; /* number of I/O signals */
        unsigned _job_cnt{0};
        unsigned _completed{0};

        bool _stop_on_error{true};
        bool _finished{false};
        bool _success{false};

        Scratch_buffer &_scratch_buffer;

        void _memcpy(char *dst, char const *src, size_t length) {
            if (length > _scratch_buffer.size) {
                warning("scratch buffer too small for copying");
                return;
            }

            Genode::memcpy(dst, src, length);
        }

        bool _alternate_access{false};

        Util::Xoroshiro _random;

        size_t const _size = _node.attribute_value("size", Number_of_bytes());
        uint64_t const _length = _node.attribute_value("length", Number_of_bytes());

        Block::Operation::Type _op_type = Block::Operation::Type::READ;

    public:
        /**
         * Block::Connection::Update_jobs_policy
         */
        void produce_write_content(Job &job, Block::seek_off_t offset, char *dst, size_t length) {
            _tx += length / _info.block_size;
            _bytes += length;

            if (_verbose)
                log("job ", job.id, ": writing ", length, " bytes at ", offset);

            if (_copy)
                _memcpy(dst, _scratch_buffer.base, length);
        }

        /**
         * Block::Connection::Update_jobs_policy
         */
        void consume_read_result(Job &job, Block::seek_off_t offset,
                                 char const *src, size_t length) {
            _rx += length / _info.block_size;
            _bytes += length;

            if (_verbose)
                log("job ", job.id, ": got ", length, " bytes at ", offset);

            if (_copy)
                _memcpy(_scratch_buffer.base, src, length);
        }

        /**
         * Block_connection::Update_jobs_policy
         */
        void completed(Job &job, bool success) {
            _completed++;

            if (_verbose)
                log("job ", job.id, ": ", job.operation(), ", completed");

            if (!success)
                error("processing ", job.operation(), " failed");

            destroy(_alloc, &job);

            if (!success && _stop_on_error)
                throw Test_failed();

            /* replace completed job by new one */
            _spawn_job();

            bool const jobs_active = (_job_cnt != _completed);

            _success = !jobs_active && success;

            if (!jobs_active || !success)
                finish();
        }

    protected:
        void _handle_progress_timeout(Duration) {
            log("progress: rx:", _rx, " tx:", _tx);
        }

        void _handle_block_io() {
            _triggered++;
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
              _verbose(node.attribute_value("verbose", false)),
              _io_buffer(_node.attribute_value("io_buffer",
                                               Number_of_bytes(4 * 1024 * 1024))),
              _progress_interval(_node.attribute_value("progress", (uint64_t) 0)),
              _copy(_node.attribute_value("copy", true)),
              _batch(_node.attribute_value("batch", 1u)),
              _finished_sig(finished_sig),
              _scratch_buffer(scratch_buffer),
              _random(_node.attribute_value("seed", 42UL)) {
            if (_progress_interval)
                _progress_timeout.construct(*_timer, *this,
                                            &Read_write::_handle_progress_timeout,
                                            Microseconds(_progress_interval * 1000));
        }

        virtual ~Read_write() {};

        void start(bool stop_on_error) {
            _stop_on_error = stop_on_error;

            _block.construct(_env, &_block_alloc, _io_buffer);
            _block->sigh(_block_io_sigh);
            _info = _block->info();

            _init();

            for (unsigned i = 0; i < _batch; i++)
                _spawn_job();

            _timer.construct(_env);
            _start_time = _timer->elapsed_ms();

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

        void _init()  {
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

            bool const r = _node.attribute_value("read", false);
            if (r) { _op_type = Block::Operation::Type::READ; }

            bool const w = _node.attribute_value("write", false);
            if (w) { _op_type = Block::Operation::Type::WRITE; }

            _alternate_access = w && r;

            _size_in_blocks = _size / _info.block_size;
            _length_in_blocks = (size_t) (_length / _info.block_size);
        }

        void _spawn_job()  {
            if (_bytes >= _length)
                return;

            _job_cnt++;

            block_number_t const lba = _next_block();

            Block::Operation::Type const op_type =
                    _alternate_access
                        ? (lba & 0x1)
                              ? Block::Operation::Type::WRITE
                              : Block::Operation::Type::READ
                        : _op_type;

            Block::Operation const operation{
                .type = op_type,
                .block_number = lba,
                .count = _size_in_blocks
            };

            new(_alloc) Job(*_block, operation, _job_cnt);
        }

        char const *name() const  { return "random"; }

        void print(Output &out) const  {
            Genode::print(out, name(), " "
                          "size:", _size, " "
                          "length:", _length, " "
                          "copy:", _copy, " "
                          "batch:", _batch);
        }
};

struct Block_connection_test::Main
{
    Genode::Env &_env;
    Genode::Heap _heap{_env.ram(), _env.rm()};

    Genode::Attached_rom_dataspace _config_rom{_env, "config"};

    bool const _log{
        _config_rom.xml().attribute_value("log", false)
    };

    bool const _report{
        _config_rom.xml().attribute_value("report", false)
    };

    bool const _calculate{
        _config_rom.xml().attribute_value("calculate", true)
    };

    bool const _stop_on_error{
        _config_rom.xml().attribute_value("stop_on_error", true)
    };

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
            if (_log) {
                Genode::log("finished ", _current->name());
            }
            Genode::destroy(&_heap, _current);
            _current = nullptr;
        }

        /* execute next test */
        if (!_current) {
            _tests.dequeue([&](Read_write &head) {
                if (_log) { Genode::log("start ", head); }
                try {
                    head.start(_stop_on_error);
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
                    Read_write *t = new(&_heap)
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

    ~Main() {
    }

    private:
        Main(const Main &) = delete;

        Main &operator=(const Main &) = delete;
};


void Component::construct(Genode::Env &env) {
    static Block_connection_test::Main main(env);
}
