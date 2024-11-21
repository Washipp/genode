/* Genode includes */
#include <base/component.h>
#include <base/log.h>
#include <util/string.h>


namespace Fuzzer {
    using namespace Genode;
    struct Main;
}


struct Fuzzer::Main
{
    Env &_env;

    String<256> input = "input_string";


    Main(Env &env) : _env(env) {}

    public:
        void fuzz(auto &fn)
        {
            fn(input);

            // if KERNEL == Linux --> new thread that executes gdb
            // else use gdb monitor.


        }

    private:
        Main(const Main &) = delete;

        Main &operator=(const Main &) = delete;
};


void Component::construct(Genode::Env &env) { static Fuzzer::Main main(env); }
