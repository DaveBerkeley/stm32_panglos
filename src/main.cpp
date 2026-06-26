
#include "panglos/debug.h"

#include "panglos/mutex.h"
#include "panglos/time.h"

using namespace panglos;

extern "C" {

extern "C" void po_log(Severity s, const char *fmt, ...)
{
    Error_Handler();
#if 0
    va_list ap;
    va_start(ap, fmt);
    if (logger)
    {
        logger->log(s, fmt, ap);
    }
    else
    {
        vprintf(fmt, ap);
    }
    va_end(ap);
#endif
}

}

    /*
     *  Application initialisation
     */

int app_main()
{
    PO_DEBUG("");

    Mutex *mutex = Mutex::create();

    delete mutex;
    return 0;
}

    /*
     *  Platform specific system initialisation
     */

extern "C" int main()
{
    // need to setup heap, init clocks etc.
    // init logging / io
    return app_main();
}

//  FIN
