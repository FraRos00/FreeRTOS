
/* Basic includes */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

/* Priorities at which the tasks are created. */
#define mainTASK_HIGH_PRIORITY		( tskIDLE_PRIORITY + 4 )
#define	mainTASK_LOW_PRIORITY	( tskIDLE_PRIORITY + 2)


/*-----------------------------------------------------------*/
/* Tasks functions */

static void prvTask1( void *pvParameters );
static void prvTask2( void *pvParameters );
static void prvTask3( void *pvParameters );
static void prvTask4( void *pvParameters );
static void prvTask5( void *pvParameters );

/*
 * The callback function executed when the software timer expires.
 */
static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle );

/* Helpers */
static void resetPrint(int index);

/*-----------------------------------------------------------*/

static TaskHandle_t task1;
static TaskHandle_t task2;
static TaskHandle_t task3;
static TaskHandle_t task4;
static TaskHandle_t task5;
static TimerHandle_t xTimer = NULL;

int printed[] = {pdTRUE,pdTRUE, pdTRUE, pdTRUE, pdTRUE,};
int counter = 0;
int timerStop = pdFALSE;
/*-----------------------------------------------------------*/

void main_point3_final( void )
{
        /* ACTIVATE ROUND ROBIN SCHEDULER IN FreeRTOSConfig.h */

		cxTaskCreate( prvTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, mainTASK_HIGH_PRIORITY, &task1, pdFALSE );
		// cxTaskCreate( prvTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, mainTASK_HIGH_PRIORITY, &task2, pdFALSE );
		cxTaskCreate( prvTask3, "Task 3", configMINIMAL_STACK_SIZE, NULL, mainTASK_LOW_PRIORITY, &task3, pdFALSE );
		cxTaskCreate( prvTask4, "Task 4", configMINIMAL_STACK_SIZE, NULL, mainTASK_LOW_PRIORITY, &task4, pdFALSE );
		cxTaskCreate( prvTask5, "Task 5", configMINIMAL_STACK_SIZE, NULL, mainTASK_LOW_PRIORITY, &task5, pdTRUE );


		xTimer = xTimerCreate( "Timer",				/* The text name assigned to the software timer - for debug only as it is not used by the kernel. */
								2UL,		/* The period of the software timer in ticks. */
								pdTRUE,				/* xAutoReload is set to pdTRUE, so this is an auto-reload timer. */
								NULL,				/* The timer's ID is not used. */
								prvQueueSendTimerCallback );/* The function executed when the timer expires. */


		xTimerStart( xTimer, 0 ); /* The scheduler has not started so use a block time of 0. */
		/* Start the tasks and timer running. */
		vTaskStartScheduler();


	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details.  NOTE: This demo uses static allocation
	for the idle and timer tasks so this line should never execute. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static void prvTask1( void *pvParameters )
{

	( void ) pvParameters;

    TickType_t ticks;
    ticks = xTaskGetTickCount();
    printf("Task 1. Ticks since start: %u\n", (unsigned int) ticks);

	for( ;; )
    {
        if(printed[0] == pdTRUE)
        {
            printf("Hello from task1 with priority %d\n",uxTaskPriorityGet(task1));
            resetPrint(0);
        }
    }

}

/*-----------------------------------------------------------*/

static void prvTask2( void *pvParameters )
{

	( void ) pvParameters;


    TickType_t ticks;
    ticks = xTaskGetTickCount();
    printf("Task 2. Ticks since start: %u\n", (unsigned int) ticks);

	for( ;; )
    {
        if(printed[1] == pdTRUE)
        {
            printf("Hello from task2 with priority %d\n",uxTaskPriorityGet(task2));
            resetPrint(1);
        }
    }

}

/*-----------------------------------------------------------*/

static void prvTask3( void *pvParameters )
{

	( void ) pvParameters;

    TickType_t ticks;
    ticks = xTaskGetTickCount();
    printf("Task 3. Ticks since start: %u\n", (unsigned int) ticks);

	for( ;; )
    {
        if(printed[2] == pdTRUE)
        {
            printf("Hello from task3 with priority %d\n",uxTaskPriorityGet(task3));
            resetPrint(2);
        }
    }

}
static void prvTask4( void *pvParameters )
{

	( void ) pvParameters;

    TickType_t ticks;
    ticks = xTaskGetTickCount();
    printf("Task 4. Ticks since start: %u\n", (unsigned int) ticks);

	for( ;; )
    {
        if(printed[3] == pdTRUE)
        {
            printf("Hello from task4 with priority %d\n",uxTaskPriorityGet(task4));
            resetPrint(3);
        }
    }

}

static void prvTask5( void *pvParameters )
{

	( void ) pvParameters;

    timerStop = pdTRUE;

    TickType_t ticks;
    ticks = xTaskGetTickCount();
    printf("Task 5. Ticks since start: %u\n", (unsigned int) ticks);

	for( ;; )
    {
        if(counter == 5)
            vTaskDelete(NULL);
        counter++;
        printf("Hello from task5 with priority %d\n",uxTaskPriorityGet(task5));

        // if(printed[4] == pdTRUE)
        // {
        //     resetPrint(4);
        // }
    }

}


static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle )
{


	/* This is the software timer callback function.  The software timer has a
	period of two seconds and is reset each time a key is pressed.  This
	callback function will execute if the timer expires, which will only happen
	if a key is not pressed for two seconds. */

	/* Avoid compiler warnings resulting from the unused parameter. */
	( void ) xTimerHandle;
    
    if(timerStop == pdTRUE)
        xTimerStop(xTimer, 0);

	printf("Hello from timer interrupt\n");
    resetPrint(-1);
}


static void resetPrint(int index)
{
    printed[index] = pdFALSE;
    for(int i=0;i<5;i++)
        if(i != index)
            printed[i] = pdTRUE;
}

