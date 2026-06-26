
#include <stdarg.h>
#include <sys/time.h>

#include <gtest/gtest.h>

#include "panglos/debug.h"
#include "panglos/thread.h"

using namespace panglos;

    /*
     *
     */

extern "C" {

void Error_Handler()
{
    assert(0);
}

    /*
     *  Thread name for logging
     *
     *  TODO : move to panglos/linux code
     */

const char *get_task()
{
    Thread *thread = Thread::get_current();
    return thread ? thread->get_name() : "main";
}

uint32_t get_time(void)
{
    struct timeval  tv;
    gettimeofday(&tv, NULL);

    uint32_t ms = (uint32_t) ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
    return ms;
}

void po_log(Severity s, const char *fmt, ...)
{
    IGNORE(s);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

}

//  FIN
