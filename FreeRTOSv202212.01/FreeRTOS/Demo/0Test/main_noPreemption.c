/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

/* Priorities at which the tasks are created. */
#define mainFIRST_TASK_PRIORITY		( tskIDLE_PRIORITY + 4 )
#define	mainSECOND_TASK_PRIORITY	( tskIDLE_PRIORITY + 1)


/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvFirstTask( void *pvParameters );
static void prvSecondTask( void *pvParameters );




/*-----------------------------------------------------------*/


static TaskHandle_t task1;
static TaskHandle_t task2;

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_noPreemption( void )
{


	
	
		/* Start the two tasks as described in the comments at the top of this
		file. */
		xTaskCreate( prvFirstTask,			/* The function that implements the task. */
					"Task 1", 							/* The text name assigned to the task - for debug only as it is not used by the kernel. */
					configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
					NULL, 							/* The parameter passed to the task - not used in this simple case. */
					mainFIRST_TASK_PRIORITY,/* The priority assigned to the task. */
					&task1 );							/* The task handle is not required, so NULL is passed. */

		xTaskCreate( prvSecondTask, "Task 2", configMINIMAL_STACK_SIZE, NULL, mainSECOND_TASK_PRIORITY, &task2 );

	
                
                
                
                
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

static void prvFirstTask( void *pvParameters )
{
TickType_t xNextWakeTime;
const TickType_t xBlockTime = 200UL;

	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;
        

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();
       
        
        
        printf("Hello from Task 1, my priority is: %d\n", uxTaskPriorityGet(task1));
        printf("Now I go to sleep\n\n");
        vTaskDelayUntil( &xNextWakeTime, xBlockTime );
        
        printf("This should not be printed \n");

	for( ;; )
	{
         
	}
}
/*-----------------------------------------------------------*/


static void prvSecondTask( void *pvParameters )
{

	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;
  
        printf("It's task 2's turn, my priority is: %d\n", uxTaskPriorityGet(task2));
        printf("Even if I have low priority, task 1 will never execute again thanks to no preemptive scheduler \n");
      
  
	for( ;; )
	{
          
	}
}
/*-----------------------------------------------------------*/


