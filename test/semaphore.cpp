
#include "gtest/gtest.h"

#include "panglos/debug.h"
#include "panglos/semaphore.h"
#include "panglos/thread.h"

using namespace panglos;

TEST(Semaphore, Basic)
{
    Semaphore *s = Semaphore::create();

    s->post();
    s->wait();

    delete s;
}

class SemWait
{
    Semaphore *semaphore1;
    Semaphore *semaphore2;
    bool dead;
public:
    SemWait()
    :   semaphore1(0),
        semaphore2(0),
        dead(false)
    {
        semaphore1 = Semaphore::create();
        semaphore2 = Semaphore::create();
    }

    ~SemWait()
    {
        delete semaphore1;
        delete semaphore2;
    }

    void kill()
    {
        dead = true;
    }

    int rx_data;
    int tx_data;

    void run()
    {
        while (!dead)
        {
            semaphore1->wait();
            tx_data = rx_data * 2;
            semaphore2->post();
        }
    }

    static void run(void *arg)
    {
        ASSERT(arg);
        SemWait *sw = (SemWait *) arg;
        sw->run();
    }

    void tx(int d)
    {
        rx_data = d;
        semaphore1->post();
    }

    int rx()
    {
        semaphore2->wait();
        return tx_data;
    }
};

TEST(Semaphore, Waiter)
{
    Thread *thread = Thread::create("sema");
    SemWait sw;

    thread->start(SemWait::run, & sw);

    for (int i = 0; i < 100; i++)
    {
        sw.tx(i);
        int j = sw.rx();
        EXPECT_EQ(i*2, j);
    }

    sw.kill();
    sw.tx(-1);

    thread->join();

    delete thread;
}

//  Linker trick
void force_test_semaphore() { }

//  FIN
