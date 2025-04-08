#include <stdio.h>
#include "tt_task_port.h"
#include "tt_task.h"
#include "key.h"
#include "key_task.h"
#include "event_tasks.h"
#include "tt_device.h"

#ifdef PLATFORM_WINDOWS
#include "windows_tick.h"
#elif defined(PLATFORM_LINUX)
#include "linux_tick.h"
#endif

// 定义任务结构体
static tt_task_t key_task;
static tt_task_t short_press_task;
static tt_task_t long_press_task;

// 定义按键设备
static key_device_t key_dev;

int main()
{
    printf("按键设备测试程序\n");
    printf("请使用键盘上的'k'键来模拟按钮\n");
    printf("短按 'k': 触发短按事件\n");
    printf("长按 'k': 触发长按事件\n");
    
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