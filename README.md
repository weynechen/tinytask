[[English]](README_en.md)
# TinyTask - 类似协程的轻量级任务调度系统、裸机框架

TinyTask是一个为资源受限的单片机设计的类协程任务调度系统、裸机框架。它采用类似协程的设计理念，通过非抢占式的协作调度方式，在资源受限的环境中实现高效的任务切换。提供简洁的API接口和事件机制，使嵌入式系统开发更加简单高效。

## TinyTask 是什么

TinyTask 是一个裸机框架（非实时系统），具有以下特点：
- 支持多任务
- 支持多对多事件同步机制
- 提供多个常用组件

它有如下优点：
- **移植方便**。整个系统，只需要提供系统时钟即可运行。
- **资源占用极小**。采用协作式任务切换，每个任务仅需额外消耗RAM 40 Byte，ROM 100 Byte。
- **类RTOS接口**。使用形式和RTOS的任务使用类似。

适用于实时性要求不高，又需要完成较多逻辑的场合。

## 核心特性

1. **类协程任务调度**：采用类似协程的协作式调度机制，无需复杂的上下文切换
2. **极低的内存占用**：每个任务仅需极少的额外RAM开销
3. **无需RTOS**：裸机运行，避免了传统RTOS的复杂性和资源消耗
4. **简单直观的API**：类似顺序代码的编写方式，降低开发难度
5. **事件驱动机制**：通过事件机制实现模块间的解耦
6. **跨平台兼容**：使用device继承机制，统一的驱动接口，可在PC上进行仿真，易于移植到不同平台

## TinyTask VS 裸机

在裸机编程中，没有统一的抽象范式。如果软件中存在多个任务，比如电机控制和LCD显示。通常会采用全局条件变量、状态机等方式去实现。任务间耦合严重，可移植性差。

TinyTask 试图提供一种抽象范式，并提供基本的任务管理能力、事件机制和设备抽象。以此提供更好的代码组织和可维护性，降低了任务间的耦合度，提高了代码的可移植性。

## TinyTask VS RTOS

在资源受限的嵌入式系统中，运行完整的实时操作系统(RTOS)往往会占用大量的系统资源，包括:

- **RAM**: 以FreeRTOS为例，每个任务都需要独立的栈空间，通常为256~512字节，10个任务就需要2.5K~5K的RAM。任务稍微复杂点，空间占用就可能翻倍增长。
- **ROM**: FreeRTOS完整内核代码需要占用约8~10K的程序空间
- **CPU**: 任务切换和系统调用会带来5~10%的性能开销

这些开销在实时操作系统中是无法被进一步缩减的，每个任务都需要独立的栈空间和上下文环境，这个开销过程在于:

- 保存当前任务的寄存器状态(通用寄存器、程序计数器、栈指针等)
- 从内存加载下一个任务的上下文
- 恢复下一个任务的寄存器状态

这些资源开销对于RAM只有几K、Flash只有几十K的小型单片机来说是一个不小的负担。

TinyTask作为一个更轻量的任务调度方案，它只是一个裸机框架，只解决基本的任务管理，任务通讯，并且对上层应用友好。它不需要独立的任务栈空间，没有上下文切换的开销，没有复杂的调度算法，是资源受限环境下的理想选择。

TinyTask也可以作为协程工作在RTOS的task中。

## 硬件要求

### 主控
- RAM：>1KB左右
- ROM：>1KB左右

## 目录结构

- `core/`: 核心功能实现，包括任务调度、事件机制等
- `component/`: 组件库，包含各种功能组件
- `example/`: 示例程序
- `tools/`: 辅助工具
- `build/`: 构建输出目录

## 使用方法

### 基本任务创建

```c
#include "tt_task.h"

tt_task_t task1;

void task1_func(void *arg)
{
  TT_TASK_START;
  while (1) {
    // 任务代码
    TT_TASK_DELAY_MS(1000);  // 延时1000毫秒
  }
  TT_TASK_END;
}

int main()
{
  tt_task_init();
  init_systick();  // 初始化系统滴答计时器
  
  tt_task_create(&task1, "task1", task1_func);
  
  tt_task_start_schedule();  // 启动调度器
  
  return 0;
}
```
可以看到，其使用方法和rtos基本接近。

### 事件使用

```c
// 注册事件
TT_EVENT_REGISTER(EVENT_MASK);

// 等待事件
TT_TASK_WAIT_EVENT(EVENT_MASK, timeout);

// 发送事件通知
tt_event_notify(EVENT_MASK);

// 清除事件
TT_TASK_CLEAR_EVENT(EVENT_MASK);
```

## 编译与运行

项目示例程序使用CMake构建系统

```bash
mkdir build && cd build
cmake ..
make
```
注：移植到单片机上需使用单片机的IDE。我仅在Linux下进行了开发和测试。
详细的讲解，请参考知乎文章：https://zhuanlan.zhihu.com/p/1893061295797953982 ，或者微信公众号文章：https://mp.weixin.qq.com/s/Ut9G7X7YOTZWkntSjPxmOQ 
## 示例程序

TinyTask提供了多个示例程序，帮助开发者快速理解和使用本系统：

### 基础示例 (basic)

展示TinyTask最基本的任务创建和调度功能，包括两个独立的周期性任务。

[查看基础示例](./example/basic/README.md)

### 事件驱动示例 (basic_event)

演示TinyTask的事件机制，通过事件实现任务间的通信与同步。

[查看事件驱动示例](./example/basic_event/README.md)

### 设备驱动测试 (device_test)

展示TinyTask的设备驱动接口和按键事件处理机制，演示如何结合任务调度和事件系统实现设备驱动。

[查看设备驱动测试](./example/device_test/README.md)

### LCD模拟器 (lcd_simulator)

基于SDL2的LCD显示模拟器，演示TinyTask如何控制图形显示和动画效果。

[查看LCD模拟器示例](./example/lcd_simulator/README.md)

上述所有的案例在PC上测试（当前仅在Ubuntu 22.04上测试）

[查看STM32示例](./example/stm32/README.md)
基于STM32平台上运行TinyTask的示例项目，演示按键和LED的组合。

## 许可证

TinyTask 采用[MIT](./LICENSE)许可证开放源代码。