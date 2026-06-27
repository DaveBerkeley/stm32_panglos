
#include "panglos/debug.h"

#include "panglos/mutex.h"
#include "panglos/time.h"

#include "rtos.h"

using namespace panglos;

extern "C" {

    //  Move to rtos app layer

//extern "C" void po_log(Severity s, const char *fmt, ...)
extern "C" void po_log(Severity , const char *, ...)
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
    heap_init();

    // need to setup heap, init clocks etc.
    // init logging / io
    return app_main();
}

//  FIN
