
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

    UART *uart = (UART*) Objects::objects->get("uart");
    if (uart) PO_DEBUG("found uart");

//    char buff[32];

    // infinite loop
    while (true)
    {
        Time::sleep(1);
        PO_DEBUG("");
        if (led) led->toggle();

#if 0
        if (uart)
        {
            int n = uart->rx(buff, sizeof(buff));
        }
#endif
    }
}

//  FIN
