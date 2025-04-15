/* Genode includes */
#include <base/component.h>
#include <base/log.h>
#include <util/string.h>

extern "C" void wait_for_continue(void);


namespace Gdb_test {
    using namespace Genode;
    struct Main;
}


struct Gdb_test::Main
{
    Env &_env;


    Main(Env &env) : _env(env)
    {
        Genode::log("Wait");
        wait_for_continue();
        _env.parent().exit(1);
    }

    private:
        Main(const Main &) = delete;

        Main &operator=(const Main &) = delete;
};


void Component::construct(Genode::Env &env) { static Gdb_test::Main main(env); }
