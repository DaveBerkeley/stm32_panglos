
#include <gtest/gtest.h>

#include "panglos/thread.h"
#include "panglos/time.h"

using namespace panglos;

static void runner(void *)
{
    Time::sleep(1);
}

TEST(Thread, Test)
{
    Thread *thread = Thread::create("test");

    thread->start(runner, 0);
    thread->join();

    delete thread;
}

void __attribute__((weak)) force_library_link(void) {
    // This function does nothing
}

//  FIN
