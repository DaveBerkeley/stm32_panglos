
#include <gtest/gtest.h>

#include "panglos/debug.h"
#include "panglos/logger.h"
#include "panglos/time.h"

static Test *tests = 0;

#if 0
void Test::add(Test *test)
{
    test->next = tests;
    tests = test;
}
#endif

Test::Test(const char *_group, const char *_name, void (*_fn)())
:   group(_group),
    name(_name),
    fn(_fn),
    next(0)
{
    // Link this Test into the global list
    next = tests;
    tests = this;
}

void Test::visit(const char *group, const char *name, visitor fn, void *arg)
{
    ASSERT(fn);
    for (Test *test = tests; test; test = test->next)
    {
        if (group && strcmp(group, test->group))
        {
            // group set but no match
            continue;
        }
        if (name && strcmp(name, test->name))
        {
            // name set but no match
            continue;
        }
        fn(test, arg);
        // FreeRTOS free() gets done in the idle thread,
        // force idle?
    }
}

void v_run(Test *test, void *arg)
{
    UNUSED(arg);
    PO_DEBUG("%s", test->name);
 
    static const char * disabledPrefix = "DISABLED_";

    if (strncmp(disabledPrefix, test->name, strlen(disabledPrefix)) == 0)
    {
        PO_INFO("test %s.%s DISABLED", test->group, test->name);
    }
    else
    {
        PO_INFO("run test %s.%s", test->group, test->name);
        test->fn();
        PO_INFO("run test %s.%s OKAY", test->group, test->name);
    }
}

void Test::run(const char *group, const char *name)
{
    PO_INFO("Running tests ...");
    visit(group, name, v_run, 0);
    PO_INFO("Tests completed");
}

    /*
     *
     */

extern "C" void test_run(const char *group, const char *name)
{
    Test::run(group, name);
}

    /*
     *
     */

struct v_list_t {
    void *arg;
    test_visitor fn;
};

void v_list(Test *test, void *arg)
{
    ASSERT(test);
    ASSERT(arg);
    struct v_list_t *info = (struct v_list_t *) arg;

    info->fn(test->group, test->name, info->arg);
}

extern "C" void test_list(const char *group, const char *name, test_visitor fn, void *arg)
{
    ASSERT(fn);
    struct v_list_t info = { .arg = arg, .fn = fn };
    Test::visit(group, name, v_list, & info);
}

//  FIN
