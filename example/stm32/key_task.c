#include "key_task.h"
#include "tt_device.h"
#include "tt_task.h"
#include "key.h"
#include <stdio.h>

void key_task_func(void *arg)
{
    TT_TASK_START;
    
    printf("key task started\n");
    printf("short press: trigger TT_EVENT_1 event\n");
    printf("long press: trigger TT_EVENT_2 event\n");
    
    while (1) {
        uint8_t key_status = key_scan();
        if (key_status == KEY_SHORT_PRESS) {
            printf("short press detected\n");
            tt_event_notify(TT_EVENT_1);
        } else if (key_status == KEY_LONG_PRESS) {
            printf("long press detected\n");
            tt_event_notify(TT_EVENT_2);
        }
        
        TT_TASK_DELAY_MS(10);
    }
    
    TT_TASK_END;
} 