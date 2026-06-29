
#include "panglos/debug.h"

#include "panglos/time.h"
#include "panglos/object.h"

#include "panglos/drivers/gpio.h"
#include "panglos/drivers/uart.h"

using namespace panglos;

    /*
     *  Application main task
     */

extern "C" void app_main(void *)
{
    PO_DEBUG("");

    GPIO *led = (GPIO*) Objects::objects->get("led");

    //UART *uart = (UART*) Objects::objects->get("uart");
    //if (uart) PO_DEBUG("found uart");

    Time::tick_t now = Time::get();

    // infinite loop
    while (true)
    {
        if (!Time::elapsed_update(& now, 1000))
        {
            continue;
        }

        PO_DEBUG("");
        if (led) led->toggle();
    }
}

//  FIN
