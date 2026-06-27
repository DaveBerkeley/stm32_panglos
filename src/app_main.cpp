
#include "panglos/debug.h"

#include "panglos/time.h"
#include "panglos/object.h"

#include "panglos/drivers/gpio.h"

using namespace panglos;

    /*
     *  Application main task
     */

extern "C" void app_main(void *)
{
    PO_DEBUG("");

    GPIO *led = (GPIO*) Objects::objects->get("led");

    // infinite loop
    while (true)
    {
        Time::sleep(1);
        PO_DEBUG("");
        if (led) led->toggle();
    }
}

//  FIN
