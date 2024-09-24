---
# FreeRTOS Custom Examples and Modifications

## Overview
This repository contains examples and custom modifications for FreeRTOS to demonstrate different scheduling mechanisms, including preemption, round-robin scheduling, and process aging with critical task flags. Follow the instructions below to run the provided examples.
---

## Setup Instructions

### Prerequisites

- **Compiler:** arm-none-eabi-gcc
- **FreeRTOS kernel version:** 202212.01
- **Dependencies:** GNU make utility, QEMU

### Cloning the Repository

Clone the repository and navigate to the FreeRTOS project root directory:

```bash
git clone [Repository URL]
cd [Project Directory]
```

### Building and Running FreeRTOS

Navigate to the directory where FreeRTOS and the examples are located:

```bash
cd FreeRTOSv202212.01/FreeRTOS/Demo/0Test/build/gcc
```

Use the following command to build and run:

```bash
make
cd output
qemu-system-arm -machine mps2-an385 -cpu cortex-m3 -kernel ./RTOSDemo.out -monitor none -nographic -serial stdio
```

This qemu command runs a simulation with QEMU on an ARM Cortex-M3 machine. Here's a detailed explanation of the parameters:

- **qemu-system-arm**: Launches QEMU in emulation mode for ARM-based systems.

- **machine mps2-an385**: Specifies the type of machine to emulate. In this case, it emulates an ARM mps2-an385 platform, which represents a development board for Cortex-M3 processors.

- **cpu cortex-m3**: Defines the CPU to emulate as an ARM Cortex-M3 processor.

- **kernel ./RTOSDemo.out**: Loads the binary file of the kernel to be executed (in this case, a FreeRTOS implementation, as suggested by the name RTOSDemo.out).

- **monitor none**: Disables QEMU’s monitoring interface, which usually allows you to send commands to manage the emulation.

- **nographic**: Disables the graphical interface, running QEMU entirely in terminal mode.

- **serial stdio**: Redirects the QEMU serial output to standard input/output (terminal), allowing you to see the output of the simulated program directly in the terminal.

In summary, this command runs a program (kernel) on a simulated Cortex-M3 architecture without a graphical interface, and the output of the emulated system is displayed in the terminal.

---

## Example Instructions

### Example 1: Non-Preemption

#### Overview:

This example highlights **non-preemptive scheduler**.

#### Steps to Run:

1. Modify `FreeRTOSConfig.h`:
   - Set `configUSE_PREEMPTION` to `0` to disable preemption
   - Set `INCLUDE_xTaskAbortDelay` to `0`
   ```c
   #define configUSE_PREEMPTION 0
   #define INCLUDE_xTaskAbortDelay 0
   ```
2. Modify the `main.c` file:

   ```c
   #define NOPREEMPTION	1
   #define PREEMPTION  	0
   #define ROUNDROBIN	    0
   ```

3. Build and run the example:
   ```bash
   make
   cd output
   qemu-system-arm -machine mps2-an385 -cpu cortex-m3 -kernel ./RTOSDemo.out -monitor none -nographic -serial stdio
   ```

### Example 2: Preemption

#### Overview:

This example highlights **preemptive scheduler**.

#### Steps to Run:

1. Modify `FreeRTOSConfig.h`:
   - Set `configUSE_PREEMPTION` to `1` to enable preemption
   - Set `INCLUDE_xTaskAbortDelay` to `1`
   ```c
   #define configUSE_PREEMPTION 1
   #define INCLUDE_xTaskAbortDelay 1
   ```
2. Update the `main.c` file:

   - enable Preemption:

   ```c
   #define NOPREEMPTION	0
   #define PREEMPTION  	1
   #define ROUNDROBIN	    0
   ```

   - uncomment only main_preemption():

   ```c
   #if ( PREEMPTION == 1 )
   {
   	main_preemption();
      // main_point3_final();
      // main_mutex();
    }
   ```

3. Build and run the example:
   ```bash
   make
   cd output
   qemu-system-arm -machine mps2-an385 -cpu cortex-m3 -kernel ./RTOSDemo.out -monitor none -nographic -serial stdio
   ```

### Example 3: Round Robin

#### Overview:

This example highlights **Round Robin scheduler**.

#### Steps to Run:

1. Modify `FreeRTOSConfig.h`:

   - Set `configUSE_PREEMPTION` to `1` to enable preemption
   - Enable time slicing for round-robin scheduling:

   ```c
   #define configUSE_PREEMPTION 1
   #define configUSE_TIME_SLICING 1
   ```

2. Update the `main.c` file:

   - enable round robin:

   ```c
   #define NOPREEMPTION	0
   #define PREEMPTION  	0
   #define ROUNDROBIN	    1
   ```

3. Build and run the example:
   ```bash
   make
   cd output
   qemu-system-arm -machine mps2-an385 -cpu cortex-m3 -kernel ./RTOSDemo.out -monitor none -nographic -serial stdio
   ```

### Example 4: Mutex

#### Overview:

This example highlights **mutex** usage.

#### Steps to Run:

1. Modify `FreeRTOSConfig.h`:

   - disable time slicing for round-robin scheduling:

   ```c
   #define configUSE_TIME_SLICING 0
   ```

2. Update the `main.c` file:

   - enable Preemption:

   ```c
   #define NOPREEMPTION	0
   #define PREEMPTION  	1
   #define ROUNDROBIN	    0
   ```

   - uncomment only main_mutex():

   ```c
   #if ( PREEMPTION == 1 )
   {
      // main_preemption();
      // main_point3_final();
      main_mutex();
    }
   ```

3. Build and run the example:
   ```bash
   make
   cd output
   qemu-system-arm -machine mps2-an385 -cpu cortex-m3 -kernel ./RTOSDemo.out -monitor none -nographic -serial stdio
   ```

---

## Custom Scheduler: Aging and Critical Task Flag

### Overview

This section explains the custom scheduler designed to include aging and critical task flag mechanisms. The modifications ensure that tasks are not stuck in starvation and that critical tasks are given priority when necessary.

**To see source files modification see appendix.**

#### Steps to Run:

1. Modify `FreeRTOSConfig.h`:

   - Add the following configuration for aging and critical task behavior:

   ```c
   #define configUSE_PREEMPTION			1
   #define configUSE_TIME_SLICING          1
   #define configUSE_AGING                 1
   #define configMAX_AGING_COUNTER         3  //MAX AGING COUNTER
   #define configTICK_RATE_HZ				( ( TickType_t ) 1 )  //CHANGE THIS TO CHANGE THE TIME SLICE.
   ```

2. Update the `main.c` file:

   - enable Preemption:

   ```c
   #define NOPREEMPTION	0
   #define PREEMPTION  	1
   #define ROUNDROBIN	    0
   ```

   - uncomment only main_point3_final():

   ```c
   #if ( PREEMPTION == 1 )
   {
      // main_preemption();
      main_point3_final();
      // main_mutex();
    }
   ```

3. Build and run the example:
   ```bash
   make
   cd output
   qemu-system-arm -machine mps2-an385 -cpu cortex-m3 -kernel ./RTOSDemo.out -monitor none -nographic -serial stdio
   ```

## Modifications to the Build Process (Makefile)

to include additional files and correctly compile the code:

1. Navigate to **FreeRTOSv202212.01/FreeRTOS/Demo/0Test/build/gcc** and open the `Makefile`.
2. Ensure that the following changes are present:

   - correct project folder name
   - additional sources file are present

3. Save the changes and rebuild the project as instructed in each example.

```Makefile
   DEMO_PROJECT = $(DEMO_ROOT)/0Test
   VPATH += $(DEMO_PROJECT)
   INCLUDE_DIRS += -I$(DEMO_PROJECT) -I$(DEMO_PROJECT)/CMSIS
   SOURCE_FILES += (DEMO_PROJECT)/main.c
   SOURCE_FILES += (DEMO_PROJECT)/main_blinky.c
   SOURCE_FILES += (DEMO_PROJECT)/main_full.c
   SOURCE_FILES += (DEMO_PROJECT)/main_preemption.c
   SOURCE_FILES += (DEMO_PROJECT)/main_noPreemption.c
   SOURCE_FILES += (DEMO_PROJECT)/main_roundRobin.c
   SOURCE_FILES += (DEMO_PROJECT)/main_point3_final.c
   SOURCE_FILES += (DEMO_PROJECT)/mutex_example.c
```

## Appendix: Code Modifications

### Modifications in `tasks.c`

- **`prvAddTaskToReadyList()`**: Added logic to increment the counter tracking the number of critical tasks in a list when a task is added to the ready list.
- **`TCB_t` structure**: Introduced the `xCritical` flag and `uxAging` counter.
- **`prvInitialiseNewTask()`**: Modified to include the `xCritical` parameter in its argument list, initializing the new flags in the task control block (TCB).
- **`isListItemCritical()`**: A helper function created to determine if a task in the list is critical.
- **`getListItemTaskName()`**: Another helper function created to retrieve the task name from a list item.
- **`xTaskCreateModified()`**: Renamed from `xTaskCreate()`, with the `xCritical` parameter added to initialize the corresponding field in the TCB.
- **`vTaskPrioritySetCustom()`**: An alternative version of `vTaskPrioritySet()` that has been streamlined by removing context switch logic.
- **`vTaskSwitchContext()`**: Added aging management and preprocessor directives to allow the aging feature to be easily disabled via a configuration parameter in _FreeRTOSConfig.h_.

### Modifications in `tasks.h`

- **`xTaskCreate()` and `cxTaskCreate()`**: Created macros for task creation, as described in point 3 of the report.
- **`isListItemCritical()` and `getListItemTaskName()`**: Prototypes added for the helper functions defined in _tasks.c_.
- **`xTaskCreateModified()`**: Updated to match changes made in _tasks.c_.

### Modifications in `list.h`

- **`xList` structure**: Added the `xCriticalCounter` field to track the number of critical tasks within the list.
- **`listGET_OWNER_OF_NEXT_ENTRY`**: Modified the macro to handle scheduling logic for critical tasks.

### Modifications in `list.c`

- **`vListInitialise()`**: Modified to initialize the new `xCriticalCounter` field in the `xList` structure.
- **`uxListRemove()`**: Updated to decrease the critical task counter when a critical task is removed from the process list.
- **`vDecreaseCriticalCounter()`**: A helper function created to manage the reduction of the critical task counter.
