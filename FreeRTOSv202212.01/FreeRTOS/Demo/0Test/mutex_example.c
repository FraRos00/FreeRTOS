#include <stdlib.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "semtest.h"
#define mtx_test_STACK_SIZE configMINIMAL_STACK_SIZE

/* Structure used to pass parameters to each task. */

typedef struct MUTEX_PARAMETERS
{
    SemaphoreHandle_t xMutex;
    volatile uint32_t *sharedVariable;
} xMutexParameters;

static void testTaskMutex(void *pvParameters);

/*-----------------------------------------------------------*/

void vStartMutexTasks(UBaseType_t uxPriority)
{
    xMutexParameters *ptMutexParameters;
    ptMutexParameters = (xMutexParameters *)pvPortMalloc(sizeof(xMutexParameters));
    if (ptMutexParameters != NULL)
    {
        ptMutexParameters->xMutex = xSemaphoreCreateMutex();
        if (ptMutexParameters->xMutex != NULL)
        {
            ptMutexParameters->sharedVariable = (uint32_t *)pvPortMalloc(sizeof(uint32_t));
            *(ptMutexParameters->sharedVariable) = 0;

            xTaskCreate(testTaskMutex, "Task1", mtx_test_STACK_SIZE, (void *)ptMutexParameters, uxPriority, (TaskHandle_t *)NULL);
            xTaskCreate(testTaskMutex, "Task2", mtx_test_STACK_SIZE, (void *)ptMutexParameters, uxPriority, (TaskHandle_t *)NULL);
            xTaskCreate(testTaskMutex, "Task3", mtx_test_STACK_SIZE, (void *)ptMutexParameters, uxPriority, (TaskHandle_t *)NULL);
            xTaskCreate(testTaskMutex, "Task4", mtx_test_STACK_SIZE, (void *)ptMutexParameters, uxPriority, (TaskHandle_t *)NULL);

            vQueueAddToRegistry((QueueHandle_t)ptMutexParameters->xMutex, "Mutex");
        }
    }
}

static void testTaskMutex(void *pvParameters)
{
    TaskHandle_t taskHandle = xTaskGetCurrentTaskHandle();
    const char *taskName = pcTaskGetName(taskHandle);

    xMutexParameters *ptMutexParameters = (xMutexParameters *)pvParameters;

    for (;;)
    {
        if (xSemaphoreTake(ptMutexParameters->xMutex, pdMS_TO_TICKS(3000)) == pdPASS)
        {
            printf("TaskName: %s, Previous Shared variable value: %u\n", taskName, *(ptMutexParameters->sharedVariable));

            *(ptMutexParameters->sharedVariable) += 1;

            printf("TaskName: %s, Shared variable value: %u\n", taskName, *(ptMutexParameters->sharedVariable));

            vTaskDelay(pdMS_TO_TICKS(2000));

            xSemaphoreGive(ptMutexParameters->xMutex);

            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else
        {
            printf("TaskName: %s, Status: Failed to access the mutex\n", taskName);
        }
    }
}

/*-----------------------------------------------------------*/

void main_mutex(void)
{
    vStartMutexTasks(1);
    vTaskStartScheduler();

    for (;;)
        ;
}
