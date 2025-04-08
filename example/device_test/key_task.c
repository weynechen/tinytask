#include "key_task.h"
#include "tt_device.h"
#include "tt_task.h"
#include "key.h"
#include <stdio.h>

void key_task_func(void *arg)
{
    TT_TASK_START;
    
    printf("按键任务已启动，请按's'键模拟短按，按'l'键模拟长按\n");
    printf("短按: 触发TT_EVENT_1事件\n");
    printf("长按: 触发TT_EVENT_2事件\n");
    
    while (1) {
        uint8_t key_status = key_scan();
        if (key_status == KEY_SHORT_PRESS) {
            printf("检测到短按\n");
            tt_event_notify(TT_EVENT_1);
        } else if (key_status == KEY_LONG_PRESS) {
            printf("检测到长按\n");
            tt_event_notify(TT_EVENT_2);
        }
        TT_TASK_DELAY_MS(10);
    }
    
    TT_TASK_END;
} 