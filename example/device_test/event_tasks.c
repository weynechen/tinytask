#include "event_tasks.h"
#include "tt_task.h"
#include <stdio.h>

// 短按响应任务
void short_press_task_func(void *arg)
{
    TT_TASK_START;
    
    // 注册关注TT_EVENT_1事件（短按）
    TT_EVENT_REGISTER(TT_EVENT_1);
    printf("Short press response task started\n");
    
    while (1) {
        // Wait for short press event
        TT_TASK_WAIT_EVENT(TT_EVENT_1, TT_TASK_WAIT_FOREVER);
        
        // Handle short press event
        printf("====> Handling short press event <====\n");
        
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
    printf("Long press response task started\n");
    
    while (1) {
        // Wait for long press event
        TT_TASK_WAIT_EVENT(TT_EVENT_2, TT_TASK_WAIT_FOREVER);
        
        // Handle long press event
        printf("====> Handling long press event <====\n");
        
        // 清除事件标志
        TT_TASK_CLEAR_EVENT(TT_EVENT_2);
    }
    
    TT_TASK_END;
} 