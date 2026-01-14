#include "event_tasks.h"
#include "tt_task.h"
#include <stdio.h>

// Short press response task
void short_press_task_func(void *arg)
{
    TT_TASK_START;
    
    // Register to listen for TT_EVENT_1 (short press)
    TT_EVENT_REGISTER(TT_EVENT_1);
    printf("Short press response task started\n");
    
    while (1) {
        // Wait for short press event
        TT_TASK_WAIT_EVENT(TT_EVENT_1, TT_TASK_WAIT_FOREVER);
        
        // Handle short press event
        printf("====> Handling short press event <====\n");
        
        // Clear event flag
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
    printf("Long press response task started\n");
    
    while (1) {
        // Wait for long press event
        TT_TASK_WAIT_EVENT(TT_EVENT_2, TT_TASK_WAIT_FOREVER);
        
        // Handle long press event
        printf("====> Handling long press event <====\n");
        
        // Clear event flag
        TT_TASK_CLEAR_EVENT(TT_EVENT_2);
    }
    
    TT_TASK_END;
} 