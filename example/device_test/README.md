# 设备驱动测试示例

本示例展示了TinyTask的设备驱动接口和按键事件处理机制，演示了如何结合任务调度和事件系统实现设备驱动。

## 功能特点

- 演示设备驱动的注册和使用
- 实现按键设备的事件处理
- 区分按键的短按和长按事件
- 通过事件机制处理不同类型的按键操作

## 代码说明

示例创建了三个任务：
- `key_task`: 负责按键状态的扫描和按键事件的触发
- `short_press_task`: 处理按键短按事件
- `long_press_task`: 处理按键长按事件

核心代码（摘自device_test.c）：
```c
// 定义任务结构体
static tt_task_t key_task;
static tt_task_t short_press_task;
static tt_task_t long_press_task;

// 定义按键设备
static key_device_t key_dev;

int main()
{
    printf("按键设备测试程序\n");
    printf("请使用键盘上的's,l'键来模拟按钮\n");
    printf("短按 's': 触发短按事件\n");
    printf("长按 'l': 触发长按事件\n");
    
    // 初始化任务系统
    tt_task_init();
    init_systick();
    
    // 注册按键设备
    register_key_device(&key_dev);
    
    // 创建任务
    tt_task_create(&key_task, "key_task", key_task_func);
    tt_task_create(&short_press_task, "short_press_task", short_press_task_func);
    tt_task_create(&long_press_task, "long_press_task", long_press_task_func);
    
    // 开始任务调度
    tt_task_start_schedule();
    
    return 0;
}
```

按键任务实现（摘自key_task.c）：
```c
void key_task_func(void *arg)
{
    TT_TASK_START;
    
    while (1) {
        // 扫描按键状态
        key_scan();
        
        // 短暂延时，降低CPU占用
        TT_TASK_DELAY_MS(20);
    }
    
    TT_TASK_END;
}
```

事件处理任务实现（摘自event_tasks.c）：
```c
void short_press_task_func(void *arg)
{
    TT_TASK_START;
    
    // 注册感兴趣的事件
    TT_EVENT_REGISTER(KEY_SHORT_PRESS_EVENT);
    
    while (1) {
        // 等待短按事件
        TT_TASK_WAIT_EVENT(KEY_SHORT_PRESS_EVENT, TT_WAIT_FOREVER);
        
        printf("检测到按键短按！\n");
        
        // 清除事件标志
        TT_TASK_CLEAR_EVENT(KEY_SHORT_PRESS_EVENT);
    }
    
    TT_TASK_END;
}

void long_press_task_func(void *arg)
{
    TT_TASK_START;
    
    // 注册感兴趣的事件
    TT_EVENT_REGISTER(KEY_LONG_PRESS_EVENT);
    
    while (1) {
        // 等待长按事件
        TT_TASK_WAIT_EVENT(KEY_LONG_PRESS_EVENT, TT_WAIT_FOREVER);
        
        printf("检测到按键长按！\n");
        
        // 清除事件标志
        TT_TASK_CLEAR_EVENT(KEY_LONG_PRESS_EVENT);
    }
    
    TT_TASK_END;
}
```

## 编译与运行

```bash
# 在TinyTask根目录中
mkdir -p build && cd build
cmake ..
make
./example/device_test/device_test
```

## 使用说明

运行程序后，可以通过键盘上的按键来模拟按钮操作：
- 短按's'键（按下后立即释放）：触发短按事件
- 长按'l'键（按下并保持至少1秒）：触发长按事件

## 预期输出

```
按键设备测试程序
请使用键盘上的's,l'键来模拟按钮
短按 's': 触发短按事件
长按 'l': 触发长按事件
[按下并快速释放's'键]
检测到按键短按！
[长按'l'键1秒以上]
检测到按键长按！
```

通过这个示例，你可以了解TinyTask如何结合任务调度和事件系统实现设备驱动和事件处理。 