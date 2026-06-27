
#include <stdlib.h>
#include <string.h>

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

#include "printf/printf.h"

#include "rtos.h"

using namespace panglos;

extern "C" {

    //  Move to rtos app layer

static void out_fn(char c, void* arg)
{
    if (!arg) return;
    UART *uart = (UART*) arg;
    uart->tx(& c, 1);
}

UART *log_uart = 0;

extern "C" void po_log(Severity s, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    if (logger)
    {
        logger->log(s, fmt, ap);
    }
    else
    {
        vfctprintf(out_fn, log_uart, fmt, ap);
        // TODO : if no uart available, do what?
    }
    va_end(ap);
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
    log_uart = STM32_UART::create(& config);
    ASSERT(log_uart);
    //const char *start = "starting logger\r\n";
    //uart->tx(start, strlen(start));

    // must be a real mutex here. The TASK_LOCK Mutex will block eg network, timers
    panglos::Mutex *mutex = panglos::Mutex::create();
    logger = new Logging(S_DEBUG, mutex);

    logger->add(log_uart, S_DEBUG, 0);
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
