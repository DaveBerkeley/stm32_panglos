
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "cmsis_version.h"

#include "panglos/debug.h"
#include "panglos/stm32/hal.h"

#include "rtos.h"

//  variables defined in linker *.ld file
extern char _estack;
extern char _end;

// interrupts run in the system stack

#if defined(STM32F446xx) 
static const uint32_t SYS_STACK_SIZE = 0xC00;
#endif

    /*
     * See https://nadler.com/embedded/newlibAndFreeRTOS.html
     * and https://github.com/DRNadler/FreeRTOS_helpers/blob/master/heap_useNewlib_ST.c
     * for a discussion of the issues of using NewLib with FreeRTOS.
     */

#if defined(USE_NEWLIB_MALLOC)
#error "not implemented"
#endif

void *__wrap_malloc(size_t s)
{
    return pvPortMalloc(s);
}

void __wrap_free(void *v)
{
    vPortFree(v);
}

void *__wrap__malloc_r(struct _reent *r, size_t s)
{
    UNUSED(r);
    return pvPortMalloc(s);
}

void __wrap__free_r(struct _reent *r, void *v)
{
    UNUSED(r);
    vPortFree(v);
}

void __wrap_realloc(struct _reent *r, void *v, size_t s)
{
    UNUSED(r);
    UNUSED(v);
    UNUSED(s);
    FATAL("");
}

void *_sbrk(int incr)
{
    // Ensure that sbrk() is not being called
    UNUSED(incr);
    FATAL("");
    return (void*) -1;
}

    /*
     *  FreeRTOS Heap Initialisation
     */

static HeapRegion_t heap_regions [2];
size_t heap_size;

void heap_init()
{
    size_t s = (size_t)((& _estack) - (& _end));
    s -= SYS_STACK_SIZE;
    heap_regions[0].pucStartAddress = (uint8_t*) & _end;
    heap_regions[0].xSizeInBytes = s;
    heap_size = s;
    vPortDefineHeapRegions(heap_regions);
}

    /*
     *  FreeRTOS error handlers
     */

void FATAL(const char *text)
{
    Error_HandlerX(text, 0);
}

    /*
     *
     */

extern void xPortSysTickHandler();

void SysTick_Handler()
{
    // SysTick interrupt handler
    HAL_IncTick();
    xPortSysTickHandler();
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char * pcTaskName)
{
    FATAL("Stack Overflow");
}

#if configUSE_MALLOC_FAILED_HOOK == 1
void vApplicationMallocFailedHook()
{
    FATAL("Memory Allocation Failed");
}
#endif

void configureTimerForRunTimeStats(void)
{
    // empty
}

unsigned long getRunTimeCounterValue(void)
{
    return HAL_GetTick();
}

    /*
     *
     */

void __attribute__((weak)) vApplicationIdleHook()
{
    //  TODO : sleep?
}

//  FIN
