#include "event_tasks.h"
#include "tt_task.h"
#include "main.h"
#include "stm32_def.h"
#include <stdio.h>

// Short press response task
void short_press_task_func(void *arg)
{
    TT_TASK_START;
    
    // Register to listen for TT_EVENT_1 (short press)
    TT_EVENT_REGISTER(TT_EVENT_1);
    printf("short press response task started\n");
    
    while (1) {
        // Wait for short press event
        TT_TASK_WAIT_EVENT(TT_EVENT_1, TT_TASK_WAIT_FOREVER);
        
        // Handle short press event - LED blinks once
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        printf("====> short press event: led on <====\n");
        TT_TASK_DELAY_MS(200);
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        printf("====> short press event: led off <====\n");
        
        TT_TASK_CLEAR_EVENT(TT_EVENT_1);
    }
    
    TT_TASK_END;
}

// Long press response task
void long_press_task_func(void *arg)
{
    TT_TASK_START;
    
    // Register to listen for TT_EVENT_2 (long press)
    TT_EVENT_REGISTER(TT_EVENT_2);
    printf("long press response task started\n");
    
    while (1) {
        // Wait for long press event
        TT_TASK_WAIT_EVENT(TT_EVENT_2, TT_TASK_WAIT_FOREVER);
        
        // Handle long press event - LED blinks quickly 5 times
        printf("====> long press event: led flash <====\n");
        for (int i = 0; i < 5; i++) {
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
            TT_TASK_DELAY_MS(100);
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
            TT_TASK_DELAY_MS(100);
        }
        printf("====> long press event completed <====\n");
        
        // Clear event flag
        TT_TASK_CLEAR_EVENT(TT_EVENT_2);
    }
    
    TT_TASK_END;
} 
