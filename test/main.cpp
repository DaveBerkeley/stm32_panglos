
#include <gtest/gtest.h>

#include "panglos/object.h"

#include "app_main.h"

using namespace panglos;

TEST(Main, App)
{
    Objects::objects = Objects::create(true);
    EXPECT_TRUE(Objects::objects);

    app_main(0);

    delete Objects::objects;
    Objects::objects = 0;
}

//  FIN
