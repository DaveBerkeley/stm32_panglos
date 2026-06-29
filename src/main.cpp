
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

#include "panglos/debug.h"

#include "panglos/mutex.h"
#include "panglos/time.h"
#include "panglos/thread.h"
#include "panglos/logger.h"
#include "panglos/object.h"

#include "panglos/stm32/uart.h"
#include "panglos/stm32/gpio_arm.h"

#include "printf/printf.h"

#include "../test/gtest/gtest.h"

#include "rtos.h"
#include "app_main.h"

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

static void tx_uart(const char *text)
{
    if (!log_uart) return;
    log_uart->tx(text, strlen(text));
}

extern "C" void Error_HandlerX(const char *file, int line)
{
    tx_uart("Error: ");
    tx_uart(file);

    char buff[16];
    snprintf(buff, sizeof(buff), "%d", line);
    tx_uart(" +");
    tx_uart(buff);

    tx_uart("\r\n");
    assert(0);
}

extern "C" void Error_Handler()
{
    Error_HandlerX(__FILE__, __LINE__);
}

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
     *  Platform specific system initialisation
     */

//  Nucleo SMT32F446 Board

#define UART_RX_PORT    GPIOA
#define UART_RX_PIN     GPIO_PIN_3
#define UART_RX_ALT     GPIO_AF7_USART2
#define UART_TX_PORT    GPIOA
#define UART_TX_PIN     GPIO_PIN_2
#define UART_TX_ALT     GPIO_AF7_USART2
#define UART_DEV        USART2
#define UART_BAUD       115200

#define LED_PORT        GPIOA
#define LED_PIN         GPIO_PIN_5

    /*
     *  To get round a linker issue, explicity include unit test modules.
     *
     *  Just by referencing the functions it forces the linker to inspect the module.
     *  Although the show_tests() module doesn't get called, and is removed from the binary,
     *  as are the referenced functions, it does cause the unit tests to be included in the build.
     */

extern void force_test_thread();

void show_tests()
{
    typedef void (*ForceFn)();
    const ForceFn fns[] = {
        force_test_thread,
        0,
    };
    for (const ForceFn *fn = fns; fn; fn++)
    {
        PO_DEBUG("%p", fn);
    }
}

    /*
     *
     */

static void idle_fn(void *arg)
{
    ASSERT(arg);
    uint32_t *count = (uint32_t*) arg;
    while (true)
    {
        const uint32_t now = get_idle_count();
        if (now == *count)
        {
            // Wait for idle
            Time::msleep(2);
            continue;
        }
        *count = now;
        return;
    }
}

void run(void *arg)
{
    // FreeRTOS doesn't delete thread storage until it has an idle cycle
    // so wait for idle between tests so that any threads can be tidied.
    uint32_t count = get_idle_count();
    Test::run(0, 0, idle_fn, & count);

    // Run the main app
    app_main(arg);
}

    /*
     *
     */

extern "C" int main()
{
    HAL_Init();
    //SystemClock_Config();
    heap_init();

    Objects::objects = Objects::create(true);
    ASSERT(Objects::objects);

    // init logging / io
    STM32_UART::Config config = {
        .rx = {
            .port = UART_RX_PORT,
            .pin = UART_RX_PIN,
            .alt = UART_RX_ALT,
        },
        .tx = {
            .port = UART_TX_PORT,
            .pin = UART_TX_PIN,
            .alt = UART_TX_ALT,
        },
        .uart = UART_DEV,
        .baud = UART_BAUD,
    };
    log_uart = STM32_UART::create(& config);
    ASSERT(log_uart);
    Objects::objects->add("uart", log_uart);

    panglos::Mutex *mutex = panglos::Mutex::create();
    logger = new Logging(S_DEBUG, mutex);
    logger->add(log_uart, S_DEBUG, 0);

    // show config, heap / memory data etc.
    PO_DEBUG("PanglOs  STM32");
    PO_DEBUG("FreeRTOS %s", tskKERNEL_VERSION_NUMBER);
    PO_DEBUG("CMSIS    v%d.%d", __CM_CMSIS_VERSION_MAIN, __CM_CMSIS_VERSION_SUB);
    PO_DEBUG("NewLib   v%d.%d", __NEWLIB__, __NEWLIB_MINOR__);

    const int err = SysTick_Config(SystemCoreClock / 1000);
    ASSERT(!err);
 
    GPIO *led = gpio_create(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT_PP, 0); 
    Objects::objects->add("led", led);

    //verbose_init();

    Thread *thread = Thread::create("main", 0);
    thread->start(run, 0);

    // This function should never return
    vTaskStartScheduler();
    return 0;
}

//  FIN
