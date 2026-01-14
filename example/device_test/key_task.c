#include "key_task.h"
#include "tt_device.h"
#include "tt_task.h"
#include "key.h"
#include <stdio.h>

void key_task_func(void *arg)
{
    TT_TASK_START;
    
    printf("Key task started, press 's' for short press, 'l' for long press\n");
    printf("Short press: trigger TT_EVENT_1 event\n");
    printf("Long press: trigger TT_EVENT_2 event\n");
    
    while (1) {
        uint8_t key_status = key_scan();
        if (key_status == KEY_SHORT_PRESS) {
            printf("Short press detected\n");
        } else if (key_status == KEY_LONG_PRESS) {
            printf("Long press detected\n");
            tt_event_notify(TT_EVENT_2);
        }
        TT_TASK_DELAY_MS(10);
    }
    
    TT_TASK_END;
} 