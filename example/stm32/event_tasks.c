#include "event_tasks.h"
#include "tt_task.h"
#include "main.h"
#include "stm32_def.h"
#include <stdio.h>

// 短按响应任务
void short_press_task_func(void *arg)
{
    TT_TASK_START;
    
    // 注册关注TT_EVENT_1事件（短按）
    TT_EVENT_REGISTER(TT_EVENT_1);
    printf("short press response task started\n");
    
    while (1) {
        // 等待短按事件
        TT_TASK_WAIT_EVENT(TT_EVENT_1, TT_TASK_WAIT_FOREVER);
        
        // 处理短按事件 - LED闪烁一次
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        printf("====> short press event: led on <====\n");
        TT_TASK_DELAY_MS(200);
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        printf("====> short press event: led off <====\n");
        
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
    printf("long press response task started\n");
    
    while (1) {
        // 等待长按事件
        TT_TASK_WAIT_EVENT(TT_EVENT_2, TT_TASK_WAIT_FOREVER);
        
        // 处理长按事件 - LED快速闪烁5次
        printf("====> long press event: led flash <====\n");
        for (int i = 0; i < 5; i++) {
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
            TT_TASK_DELAY_MS(100);
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
            TT_TASK_DELAY_MS(100);
        }
        printf("====> long press event completed <====\n");
        
        // 清除事件标志
        TT_TASK_CLEAR_EVENT(TT_EVENT_2);
    }
    
    TT_TASK_END;
} 