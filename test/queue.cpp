
#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"

#include "panglos/debug.h"
#include "panglos/queue.h"
#include "panglos/thread.h"
#include "panglos/time.h"

using namespace panglos;

struct TestData
{
    int i;
    int id;
};

class QueueEnd
{
public:
    Queue *queue;
    Thread *thread;
    bool reader;
    int n;
    char *name;
    int id;
    uint32_t found;

    QueueEnd(Queue *q, bool rd, int _n, int idx)
    :   queue(q),
        thread(0),
        reader(rd),
        n(_n),
        name(0),
        id(idx),
        found(0)
    {
        char buff[16];
        snprintf(buff, sizeof(buff), "q_%d", id);
        name = strdup(buff);
        thread = Thread::create(name);
    }

    ~QueueEnd()
    {
        delete thread;
        free(name);
    }

    void run()
    {
        if (reader)
        {
            for (int i = 0; i < n; i++)
            {
                struct TestData td;
                queue->get((Queue::Message*) & td, -1);
                found |= (1 << td.i);
            }
        }
        else
        {
            for (int i = 0; i < n; i++)
            {
                struct TestData td = { .i = i, .id = id, };
                queue->put((Queue::Message*) & td);
            }
        }
    }

    static void run(void *arg)
    {
        ASSERT(arg);
        QueueEnd *eq = (QueueEnd *) arg;
        eq->run();
    }

    void start()
    {
        thread->start(run, this);
    }

    void join()
    {
        thread->join();
    }
};

    /*
     *
     */

TEST(Queue, Test)
{
    Queue *queue = Queue::create(sizeof(TestData), 5, 0);

    struct TestData test = { .i = 1234 };
    queue->put((const Queue::Message*) & test);

    struct TestData result;
    queue->get((Queue::Message*) & result, -1);

    EXPECT_EQ(test.i, result.i);

    delete queue;
}

TEST(Queue, Reader)
{
    Queue *queue = Queue::create(sizeof(TestData), 5, 0);

    const int num = 4;
    const int n = 8;
    ASSERT((num * n) <= 32);
    QueueEnd *qes[num];

    for (int i = 0; i < num; i++)
    {
        qes[i] = new QueueEnd(queue, true, n, i);
    }

    for (int i = 0; i < num; i++)
    {
        qes[i]->start();
    }

    TestData td = { 0 };
    for (int i = 0; i < (n * num); i++)
    {
        td.i = i;
        queue->put((Queue::Message*) & td);
    }

    // results are the O of all the thead's found data
    uint32_t found = 0;

    for (int i = 0; i < num; i++)
    {
        qes[i]->join();
        found |= qes[i]->found;
    }

    for (int i = 0; i < num; i++)
    {
        delete qes[i];
    }

    delete queue;

    // All the bits should be set
    EXPECT_EQ(found, (1LL << (num * n)) - 1);
}

TEST(Queue, Writer)
{
    Queue *queue = Queue::create(sizeof(TestData), 5, 0);

    const int num = 4;
    const int n = 8;
    ASSERT((num * n) <= 32);
    QueueEnd *qes[num];

    for (int i = 0; i < num; i++)
    {
        qes[i] = new QueueEnd(queue, false, n, i);
    }

    for (int i = 0; i < num; i++)
    {
        qes[i]->start();
    }

    for (int i = 0; i < (n * num); i++)
    {
        TestData td = { 0 };
        queue->get((Queue::Message*) & td, -1);
        qes[td.id]->found |= (1 << td.i);

    }

    for (int i = 0; i < num; i++)
    {
        qes[i]->join();

        EXPECT_EQ(qes[i]->found, (1 << n) - 1);
    }

    for (int i = 0; i < num; i++)
    {
        delete qes[i];
    }

    delete queue;
}

//  Linker trick
void force_test_queue() { }

//  FIN
