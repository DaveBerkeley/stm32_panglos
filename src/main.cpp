
#include <stdlib.h>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

#include "panglos/debug.h"

#include "panglos/mutex.h"
#include "panglos/time.h"
#include "panglos/thread.h"
#include "panglos/logger.h"

#include "panglos/stm32/uart.h"

#include "rtos.h"

using namespace panglos;

extern "C" {

    //  Move to rtos app layer

//extern "C" void po_log(Severity s, const char *fmt, ...)
extern "C" void po_log(Severity , const char *, ...)
{
    Error_Handler();
#if 0
    va_list ap;
    va_start(ap, fmt);
    if (logger)
    {
        logger->log(s, fmt, ap);
    }
    else
    {
        vprintf(fmt, ap);
    }
    va_end(ap);
#endif
}

}

    /*
     *  Application main task
     */

extern "C" void app_main(void *)
{
    PO_DEBUG("");

    // infinite loop
    while (true)
    {
        Time::sleep(1);
        PO_DEBUG("");
    }
}

    /*
     *  Platform specific system initialisation
     */

extern "C" int main()
{
    HAL_Init();
//    SystemClock_Config();
    heap_init();

    // init logging / io
    STM32_UART::Config config = {
        .rx = {
            .port = GPIOA,
            .pin = GPIO_PIN_3,
            .alt = GPIO_AF7_USART2,
        },
        .tx = {
            .port = GPIOA,
            .pin = GPIO_PIN_2,
            .alt = GPIO_AF7_USART2,
        },

        .uart = USART2,
        .baud = 115200,
    };
    UART *uart = STM32_UART::create(& config);

    // must be a real mutex here. The TASK_LOCK Mutex will block eg network, timers
    panglos::Mutex *mutex = panglos::Mutex::create();
    logger = new Logging(S_DEBUG, mutex);

    logger->add(uart, S_DEBUG, 0);
    PO_DEBUG("PanglOs STM32 System");

    // show config, heap / memory data etc.
    // show banner

    //verbose_init();

    //Objects::objects = Objects::create(true);
    //ASSERT(Objects::objects);

    Thread *thread = Thread::create("main", 0);
    thread->start(app_main, 0);

    // This function should never return
    vTaskStartScheduler();
    return 0;
}

//  FIN
