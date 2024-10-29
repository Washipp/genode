/* Genode includes */
#include <base/attached_rom_dataspace.h>
#include <base/attached_ram_dataspace.h>
#include <base/component.h>
#include <base/heap.h>
#include <block/request_stream.h>
#include <block_session/connection.h>
#include <os/session_policy.h>
#include <util/string.h>

/**
 * Main Idea: Once we receive a submit signal vom the incoming request, we receive a pointer to the payload.
 * We encrypt the data in the payload and add it to the submit queue for the VFS.
 * When we Receive data from the VFS we do the steps above in reverse.
 *
 */
namespace Simple_encryption {
    struct Task;
    struct Block_session_component;

    struct Main;
    using namespace Genode;
    using Block_connection = Block::Connection<Task>;
}


/*
 * This struct holds the necessary request infos (for example the point to the data block).
 * Further it uses the Job interface of the block connection to connect to the block session that handles FS access.
 */
struct Simple_encryption::Task : Block_connection::Job,
                                 Fifo<Task>::Element
{
    struct Unsupported_Operation : Exception {};

    private:
        Block_connection &_connection;
        Block::Request _request;
        Signal_context_capability _finished_sig;
        char *const _data;
        size_t _size;
        int _key;

        const char *_xor_with_int(char *input, size_t size, int key) {
            for (Genode::size_t i = 0; i < size; ++i) {
                char tmp = static_cast<char>(input[i] ^ key);
                input[i] = tmp;
            }
            return input;
        }

    public:
        int id;
        bool acked = false;

        Task(int id, Block_connection &connection, Block::Request request, Signal_context_capability finished_sig,
             void *data, size_t size, int key)
            : Job(connection, {
                      .type = request.operation.type,
                      .block_number = request.operation.block_number,
                      .count = request.operation.count
                  }),
              _connection(connection),
              _request(request),
              _finished_sig(finished_sig),
              _data(static_cast<char *>(data)),
              _size(size),
              _key(key),
              id(id) {}

        void handle_block_io() {
            _connection.update_jobs(*this);
        }

        void produce_write_content(Task &task, Block::off_t offset, char *dst, size_t length) {
            _xor_with_int(task._data, length, _key);
            memcpy(dst + offset, task._data, length);
        }

        void consume_read_result(Task &task, Block::off_t offset, char const *src, size_t length) {
            // TODO: Check if the read job is correctly answered.
            memcpy(task._data, src + offset, length);
            _xor_with_int(task._data, length, _key);
        }

        void completed(Task &task, bool success) {
            if (_finished_sig.valid()) {
                Genode::Signal_transmitter(_finished_sig).submit();
            }

            if (!success)
                error("processing ", task.operation(), " failed");

            task._set_success(success);
        }

        void print(Genode::Output &out) const {
            Genode::print(out, " size: ", _size);
        }

        Block::Request get_request() {
            return _request;
        }

        void _set_success(bool success) {
            _request.success = success;
        }

    private:
        Task(const Simple_encryption::Task &other) = delete;

        Task &operator=(const Task &other) = delete;
};

/*
 * Object that is accessible from remote protection domains.
 * This enables the component to provide a Block Session.
 *
 * The Block::Request_stream interface provides a way to stream of block-operations.
 * (Genode Foundation; 3.6.6. Asynchronous bulk transfer - packet streams)
 */
struct Simple_encryption::Block_session_component : Rpc_object<Block::Session>,
                                                    Block::Request_stream
{
    Entrypoint &_ep;

    Block_session_component(Region_map &rm,
                            Dataspace_capability ds,
                            Entrypoint &ep,
                            Signal_context_capability sigh,
                            Info info)
        : Request_stream(rm, ds, ep, sigh, info),
          _ep(ep) {
        _ep.manage(*this);
    }

    ~Block_session_component() { _ep.dissolve(*this); }

    Info info() const override { return Request_stream::info(); }

    Capability<Tx> tx_cap() override { return Request_stream::tx_cap(); }
};

struct Simple_encryption::Main : Rpc_object<Typed_root<Block::Session> >
{
    int _id_counter = 0;
    Env &_env;

    /**
     * These variables are needed to handle "incoming" requests
     * from components where the Block Service is provided to
     */
    Constructible<Attached_ram_dataspace> _block_ds {};
    Constructible<Block_session_component> _block_session {};
    Signal_handler<Main> _request_handler {_env.ep(), *this, &Main::_handle_requests};

    Block::Session::Info _info {};
    Genode::Fifo<Task> _task_queue {};
    /** To access the config a ROM dataspace is needed (Genode Foundation; 4.6. Component configuration)  */
    Genode::Attached_rom_dataspace _config_rom {_env, "config"};
    int _key;

    /**
     * These three variables are necessary to connect to the block-session
     * that actually read/writes from the fat.img.
     */
    Genode::Heap _heap {_env.ram(), _env.rm()};
    Allocator_avl _block_alloc {&_heap};
    Constructible<Block_connection> _block {};

    Task *_current {nullptr};

    void _handle_requests() {
        if (!_block_session.constructed())
            return;
        Block_session_component &block_session = *_block_session;

        bool progress = true;
        while (progress) {
            progress = false;
            block_session.with_requests([&](Block::Request request) {
                bool const payload = Block::Operation::has_payload(request.operation.type);

                try {
                    if (payload) {
                        block_session.with_content(request, [&](void *ptr, size_t size) {
                            /* ptr points to the block that we need to encrypt and hand to the VFS.*/
                            auto *t = new(&_heap) Task(_id_counter, *_block, request, _request_handler,
                                                       ptr, size, _key);
                            _task_queue.enqueue(*t);
                            progress |= true;
                            _id_counter++;
                        });

                        return Block::Request_stream::Response::ACCEPTED;
                    }
                    auto *t = new(&_heap) Task(_id_counter, *_block, request, _request_handler,
                                               nullptr, 0, _key);
                    _task_queue.enqueue(*t);
                    progress |= true;
                    _id_counter++;

                    return Block::Request_stream::Response::ACCEPTED;
                } catch (Task::Unsupported_Operation) {
                    progress = false;
                    return Block::Request_stream::Response::REJECTED;
                }

                return Block::Request_stream::Response::RETRY;
            });


            if (_current) {
                block_session.try_acknowledge([&](Block_session_component::Ack &ack) {
                    if (_current->acked) {
                        log("(II) Task already Acknowledged. ID ", _current->id);
                    } else if (_current->get_request().success) {
                        ack.submit(_current->get_request());
                        progress |= true;
                        _current->acked = true;
                        destroy(&_heap, _current);
                        _task_queue.dequeue([&](Task &head) { _current = &head; });
                    } else {
                        _current->handle_block_io();
                    }
                });
            }
            if (!_current) {
                _task_queue.dequeue([&](Task &head) { _current = &head; });
                progress |= true;
            }
        }
        block_session.wakeup_client_if_needed();
    }

    void _handle_block_io() {
        _task_queue.for_each([&](Task &task) { task.handle_block_io(); });
    }

    Signal_handler<Main> _block_io_sigh {_env.ep(), *this, &Main::_handle_block_io};


    /* ============= Root interface =============== */

    /* Creates a new Block session */
    Session_capability session(Root::Session_args const &args, Affinity const &) override {
        if (_block_session.constructed()) {
            error("already in use");
            throw Service_denied();
        }

        size_t const ds_size =
                Arg_string::find_arg(args.string(), "tx_buf_size").ulong_value(0);

        Ram_quota const ram_quota = ram_quota_from_args(args.string());

        if (ds_size >= ram_quota.value) {
            warning("communication buffer size exceeds session quota");
            throw Insufficient_ram_quota();
        }

        //TODO: check number of bytes. Is this correct?
        _block.construct(_env, &_block_alloc, Number_of_bytes(4 * 1024 * 1024));
        _block->sigh(_block_io_sigh);
        _info = _block->info();

        _block_ds.construct(_env.ram(), _env.rm(), ds_size);
        _block_session.construct(_env.rm(), _block_ds->cap(), _env.ep(), _request_handler, _info);
        return _block_session->cap();
    }

    /* Handle session upgrade request. */
    void upgrade(Session_capability, Root::Upgrade_args const &) override {}

    /* Closes Block session. */
    void close(Session_capability cap) override {
        if (!_block_session.constructed() || !(_block_session->cap() == cap))
            return;
        _block.destruct();
        _block_session.destruct();
        _block_ds.destruct();
    }

    Main(Env &env) : _env(env),
                     _key(_config_rom.xml().attribute_value("key", 123456)) {
        log("Key found: ", _key);
        /* Announce "Block::Session" to the parent. */
        _env.parent().announce(_env.ep().manage(*this));
    }

    private:
        Main(const Main &) = delete;

        Main &operator=(const Main &) = delete;
};


void Component::construct(Genode::Env &env) { static Simple_encryption::Main main(env); }
