/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

/* Priorities at which the tasks are created. */
#define mainFIRST_TASK_PRIORITY		( tskIDLE_PRIORITY + 4 )
#define	mainSECOND_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )


/*-----------------------------------------------------------*/

static void prvFirstTask( void *pvParameters );
static void prvSecondTask( void *pvParameters );

/*-----------------------------------------------------------*/


static TaskHandle_t task1;
static TaskHandle_t task2;

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_preemption( void )
{
	
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
	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;
        

        for(int i = 1; i < mainFIRST_TASK_PRIORITY+1; i++){
          printf("Task 1 priority : %d, Task 2 current priority: %d \n", uxTaskPriorityGet(task1), uxTaskPriorityGet(task2));

          vTaskPrioritySet(task2, mainSECOND_TASK_PRIORITY+i);
        //Task priority set controlla la nuova priorita' del task che viene settata e se e' maggiore O UGUALE al task correntemente in esecuzione 
        //effettua un context switch. Per modificare questo comportamento basta cambiare il >= nel controllo 
        //if( uxNewPriority >= pxCurrentTCB->uxPriority )
        // {
        //   xYieldRequired = pdTRUE;
        // }
        }
        
        printf("Hello again from Task 1, Task 2 is sleeping!\n");

	for( ;; )
	{
         
	}
}

/*-----------------------------------------------------------*/

static void prvSecondTask( void *pvParameters )
{
  const TickType_t xBlockTime = (2000UL);
  TickType_t xNextWakeTime;

	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;
        xNextWakeTime = xTaskGetTickCount();
        printf("It's task 2's turn, my priority is: %d\n",uxTaskPriorityGet(task2));
        printf("Let's go to sleep!\n");
        vTaskDelayUntil( &xNextWakeTime, xBlockTime ); 
        // una volta sospeso il task 2 ha la stessa priorita' del task 1. Se le cose rimanessero cosi il task 2 non eseguirebbe piu.
        // il task 1 pero' aumenta nuovamente la priorita' del task2 cosi che quando quest ultimo si sveglia fa preemption sul task1 
       printf("Hello again from task 2 that waked up and preempted task 1\n");
	for( ;; )
	{
          
	}
}
/*-----------------------------------------------------------*/


