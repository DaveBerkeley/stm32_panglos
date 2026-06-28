
#include <stdio.h>

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
    int num = 5;

    Thread *threads[num] = { 0 };
    char names[num][10];
   
    for (int i = 0; i < num; i++)
    {
        snprintf(names[i], sizeof(names[i]), "test_%d", i);
        threads[i] = Thread::create(names[i]);
        threads[i]->start(runner, 0);
    }

    for (int i = 0; i < num; i++)
    {
        threads[i]->join();
        delete threads[i];
    }
}

// Allows linker command to force inclusion of the tests in this module
void force_test_thread(void){}

//  FIN
