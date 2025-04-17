# TinyTask - Coroutine-like Lightweight Task Scheduling System and Bare Metal Framework

TinyTask is a coroutine-like task scheduling system and bare metal framework designed for resource-constrained microcontrollers. It adopts coroutine-like design principles, implementing efficient task switching in resource-limited environments through non-preemptive cooperative scheduling. It provides simple API interfaces and event mechanisms, making embedded system development simpler and more efficient.

## What is TinyTask

TinyTask is a bare metal framework (non-real-time system) with the following features:
- Support for multiple tasks
- Support for many-to-many event synchronization mechanisms
- Provides multiple common components

It has the following advantages:
- **Easy to port**. The entire system only needs to provide a system clock to run.
- **Extremely small resource footprint**. Using cooperative task switching, each task only consumes an additional 40 bytes of RAM and 100 bytes of ROM.
- **RTOS-like interface**. Usage form is similar to RTOS tasks.

Suitable for scenarios with low real-time requirements but needing to complete multiple logical operations.

## Core Features

1. **Coroutine-like task scheduling**: Uses a coroutine-like cooperative scheduling mechanism, without complex context switching
2. **Extremely low memory usage**: Each task requires minimal additional RAM overhead
3. **No RTOS needed**: Runs on bare metal, avoiding the complexity and resource consumption of traditional RTOS
4. **Simple and intuitive API**: Writing style similar to sequential code, reducing development difficulty
5. **Event-driven mechanism**: Implements module decoupling through event mechanisms
6. **Cross-platform compatibility**: Uses device inheritance mechanism, unified driver interface, can be simulated on PC, easy to port to different platforms

## TinyTask VS Bare Metal

In bare metal programming, there is no unified abstraction paradigm. If there are multiple tasks in the software, such as motor control and LCD display, global condition variables, state machines, and other methods are usually used for implementation. Tasks are tightly coupled, and portability is poor.

TinyTask attempts to provide an abstraction paradigm and offers basic task management capabilities, event mechanisms, and device abstraction. This provides better code organization and maintainability, reduces coupling between tasks, and improves code portability.

## TinyTask VS RTOS

In resource-constrained embedded systems, running a complete real-time operating system (RTOS) often consumes a large amount of system resources, including:

- **RAM**: Using FreeRTOS as an example, each task requires independent stack space, typically 256~512 bytes, so 10 tasks would require 2.5K~5K RAM. If tasks are more complex, space usage could double.
- **ROM**: The complete FreeRTOS kernel code occupies about 8~10K of program space
- **CPU**: Task switching and system calls bring 5~10% performance overhead

These overheads cannot be further reduced in real-time operating systems, as each task requires independent stack space and context environment. This overhead process involves:

- Saving the current task's register state (general registers, program counter, stack pointer, etc.)
- Loading the next task's context from memory
- Restoring the next task's register state

These resource overheads are a significant burden for small microcontrollers with only a few KB of RAM and a few tens of KB of Flash.

TinyTask, as a lighter task scheduling solution, is just a bare metal framework that only solves basic task management, task communication, and is friendly to upper-layer applications. It doesn't require independent task stack space, has no context switching overhead, no complex scheduling algorithms, and is an ideal choice for resource-constrained environments.

TinyTask can also work as a coroutine within an RTOS task.

## Hardware Requirements

### Main Controller
- RAM: >1KB or so
- ROM: >1KB or so

## Directory Structure

- `core/`: Core functionality implementation, including task scheduling, event mechanisms, etc.
- `component/`: Component library, containing various functional components
- `example/`: Example programs
- `tools/`: Auxiliary tools
- `build/`: Build output directory

## Usage

### Basic Task Creation

```c
#include "tt_task.h"

tt_task_t task1;

void task1_func(void *arg)
{
  TT_TASK_START;
  while (1) {
    // Task code
    TT_TASK_DELAY_MS(1000);  // Delay 1000 milliseconds
  }
  TT_TASK_END;
}

int main()
{
  tt_task_init();
  init_systick();  // Initialize system tick timer
  
  tt_task_create(&task1, "task1", task1_func);
  
  tt_task_start_schedule();  // Start the scheduler
  
  return 0;
}
```
As you can see, its usage method is basically similar to RTOS.

### Event Usage

```c
// Register event
TT_EVENT_REGISTER(EVENT_MASK);

// Wait for event
TT_TASK_WAIT_EVENT(EVENT_MASK, timeout);

// Send event notification
tt_event_notify(EVENT_MASK);

// Clear event
TT_TASK_CLEAR_EVENT(EVENT_MASK);
```

## Compilation and Running

Project example programs use the CMake build system

```bash
mkdir build && cd build
cmake ..
make
```
Note: Porting to microcontrollers requires using the microcontroller's IDE. I have only developed and tested on Linux.
For detailed explanations, please refer to the Zhihu article: https://zhuanlan.zhihu.com/p/1893061295797953982, or the WeChat public account article: https://mp.weixin.qq.com/s/Ut9G7X7YOTZWkntSjPxmOQ

## Example Programs

TinyTask provides multiple example programs to help developers quickly understand and use this system:

### Basic Example (basic)

Demonstrates TinyTask's most basic task creation and scheduling functions, including two independent periodic tasks.

[View Basic Example](./example/basic/README.md)

### Event-Driven Example (basic_event)

Demonstrates TinyTask's event mechanism, implementing communication and synchronization between tasks through events.

[View Event-Driven Example](./example/basic_event/README.md)

### Device Driver Test (device_test)

Demonstrates TinyTask's device driver interface and key event handling mechanism, showing how to combine task scheduling and event systems to implement device drivers.

[View Device Driver Test](./example/device_test/README.md)

### LCD Simulator (lcd_simulator)

Based on SDL2 LCD display simulator, demonstrates how TinyTask controls graphic display and animation effects.

[View LCD Simulator Example](./example/lcd_simulator/README.md)

All of the above cases have been tested on PC (currently only tested on Ubuntu 22.04)

@TODO 
Provide microcontroller examples

## License

TinyTask uses the [MIT](./LICENSE) license for open source code.
