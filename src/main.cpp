
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

#include "gtest/gtest.h"

#include "rtos.h"
#include "app_main.h"

using namespace panglos;

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
     *  Low level diagnostic / logging output
     */

extern "C" {

static void out_fn(char c, void* arg)
{
    if (!arg) return;
    UART *uart = (UART*) arg;
    uart->tx(& c, 1);
}

UART *log_uart = 0;

void tx_uart(const char *text)
{
    if (!log_uart) return;
    log_uart->tx(text, strlen(text));
}

void po_log(Severity s, const char *fmt, ...)
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

static void show_free()
{
    const size_t left = xPortGetFreeHeapSize();
    PO_DEBUG("free_mem=%d", left);
}

static void _idle_fn(uint32_t *count)
{
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
        break;
    }
}

static void idle_fn(void *arg)
{
    ASSERT(arg);
    uint32_t *count = (uint32_t*) arg;
    _idle_fn(count);
    // FreeRTOS cleans up old tasks after it calls the user idle fn
    // so make a second call so we can see the memory after it does this.
    _idle_fn(count);
    show_free();
}

static void run_tests(const char *group, const char *test)
{
    uint32_t count = 0;
    idle_fn(& count);
    Test::run(group, test, idle_fn, & count);
}

void run(void *arg)
{
    run_tests(0, 0);
    // Run the main app
    app_main(arg);
}

    /*
     *
     */

static void show_config(Out *out, const char *end="\r\n")
{
    FmtOut fmt(out);

    fmt.printf("PanglOs   STM32%s", end);
    fmt.printf("FreeRTOS  %s%s", tskKERNEL_VERSION_NUMBER, end);
    fmt.printf("CMSIS     v%d.%d%s", __CM_CMSIS_VERSION_MAIN, __CM_CMSIS_VERSION_SUB, end);
    fmt.printf("NewLib    v%d.%d%s", __NEWLIB__, __NEWLIB_MINOR__, end);
    fmt.printf("free_heap %d%s", xPortGetFreeHeapSize(), end);
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
    show_config(log_uart);

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
