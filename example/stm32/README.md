# TinyTask STM32示例

这是一个在STM32平台上运行TinyTask的示例项目，演示了如何使用TinyTask实现按键检测和LED控制的异步任务处理。工程通过STM32CubeMX生成。

## 功能介绍

本示例实现了以下功能：
- 按键检测任务：检测按键的短按和长按事件
- 短按响应任务：接收短按事件并控制LED闪烁一次
- 长按响应任务：接收长按事件并控制LED快速闪烁5次

## 目录结构

- `event_tasks.c/h`：事件响应任务实现
- `key_task.c/h`：按键检测任务实现
- `include/`：应用层头文件
- `port/`：TinyTask移植层
- `Drivers/`：STM32 HAL驱动
- `Core/`：STM32核心文件
- `cmake/`：CMake构建配置
- `build/`：构建输出目录
- `flash_and_monitor.py`：烧录和监控工具

## 硬件要求

- STM32F103系列微控制器开发板
- 一个按键（连接到指定GPIO）
- 一个LED（连接到指定GPIO）
- USB转串口模块（用于烧录和监控）

## 构建与烧录

### 依赖项

- CMake (>= 3.22)
- ARM GCC 工具链 (arm-none-eabi-gcc)
- Python 3
- pyserial 库
- stm32flash 工具 `sudo apt-get install stm32flash`

### 构建项目

```bash
# 创建并进入构建目录
mkdir -p build && cd build

# 配置并构建
cmake ..
make -j4

# 或使用提供的Python脚本
cd ..
python3 flash_and_monitor.py build
```

### 烧录程序

```bash
# 使用提供的Python脚本
python3 flash_and_monitor.py flash

# 监控串口输出
python3 flash_and_monitor.py monitor

# 一次性执行所有操作
python3 flash_and_monitor.py build flash monitor
```

## TinyTask使用示例

### 任务创建

```c
// 在主函数中创建任务
tt_task_create(key_task_func, "key", 128, NULL, 1);
tt_task_create(short_press_task_func, "short", 128, NULL, 2);
tt_task_create(long_press_task_func, "long", 128, NULL, 2);
```

### 事件处理

```c
// 发送事件
tt_event_notify(TT_EVENT_1);  // 发送短按事件

// 等待事件
TT_TASK_WAIT_EVENT(TT_EVENT_1, TT_TASK_WAIT_FOREVER);  // 等待短按事件
```

## 串口输出

烧录后可通过串口监控程序运行状态，示例输出：

```
key task started
short press: trigger TT_EVENT_1 event
long press: trigger TT_EVENT_2 event
short press response task started
long press response task started
short press detected
====> short press event: led on <====
====> short press event: led off <====
long press detected
====> long press event: led flash <====
====> long press event completed <====
```
