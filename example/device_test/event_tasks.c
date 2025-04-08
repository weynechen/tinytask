#include "event_tasks.h"
#include "tt_task.h"
#include <stdio.h>

// 短按响应任务
void short_press_task_func(void *arg)
{
    TT_TASK_START;
    
    // 注册关注TT_EVENT_1事件（短按）
    TT_EVENT_REGISTER(TT_EVENT_1);
    printf("短按响应任务已启动\n");
    
    while (1) {
        // 等待短按事件
        TT_TASK_WAIT_EVENT(TT_EVENT_1, TT_TASK_WAIT_FOREVER);
        
        // 处理短按事件
        printf("====> 处理短按事件 <====\n");
        
        // 清除事件标志
        TT_TASK_CLEAR_EVENT(TT_EVENT_1);
    }
    
    TT_TASK_END;
}

// 长按响应任务
void long_press_task_func(void *arg)
{
    TT_TASK_START;
    
    // 注册关注TT_EVENT_2事件（长按）
    TT_EVENT_REGISTER(TT_EVENT_2);
    printf("长按响应任务已启动\n");
    
    while (1) {
        // 等待长按事件
        TT_TASK_WAIT_EVENT(TT_EVENT_2, TT_TASK_WAIT_FOREVER);
        
        // 处理长按事件
        printf("====> 处理长按事件 <====\n");
        
        // 清除事件标志
        TT_TASK_CLEAR_EVENT(TT_EVENT_2);
    }
    
    TT_TASK_END;
} 