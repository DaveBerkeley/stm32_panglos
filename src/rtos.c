
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "cmsis_version.h"

    /*
     *  FreeRTOS error handlers
     */

void FATAL(const char *text)
{
    while (1)
        ;
}

void Error_Handler()
{
    FATAL("RTOS Error");
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
    FATAL("todo");
    return 0;
//    return HAL_GetTick();
}

//  FIN
