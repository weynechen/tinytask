#ifndef TT_TASK_PORT_H
#define TT_TASK_PORT_H
#include <stdint.h>

/*
 * TinyTask移植层接口
 * 任何使用TinyTask的项目必须提供以下函数的实现：
 * - tt_task_increment_system_tick
 * - tt_task_get_system_tick
 * - tt_task_get_system_tick_ms
 */

#define TT_TASK_USE_SOFTWARE_TIMER 1
#define TT_TASK_USE_IDLE_TASK 1

#define TT_TASK_MAX_EVENT_NUM 32
typedef enum {
    TT_NO_EVENT = 0,
    TT_EVENT_1 = 1u<<0,  // 按键短按事件
    TT_EVENT_2 = 1u<<1,  // 按键长按事件
    TT_EVENT_3 = 1u<<2,
    TT_EVENT_4 = 1u<<3,
    TT_EVENT_5 = 1u<<4,
    TT_EVENT_6 = 1u<<5,
    TT_EVENT_7 = 1u<<6,
    TT_EVENT_8 = 1u<<7,
    TT_EVENT_9 = 1u<<8,
    TT_EVENT_10 = 1u<<9,
    TT_EVENT_11 = 1u<<10,
    TT_EVENT_12 = 1u<<11,
    TT_EVENT_13 = 1u<<12,
    TT_EVENT_14 = 1u<<13,
    TT_EVENT_15 = 1u<<14,
    TT_EVENT_16 = 1u<<15,
    TT_EVENT_17 = 1u<<16,
    TT_EVENT_18 = 1u<<17,
    TT_EVENT_19 = 1u<<18,
    TT_EVENT_20 = 1u<<19,
    TT_EVENT_21 = 1u<<20,
    TT_EVENT_22 = 1u<<21,
    TT_EVENT_23 = 1u<<22,
    TT_EVENT_24 = 1u<<23,
    TT_EVENT_25 = 1u<<24,
    TT_EVENT_26 = 1u<<25,
    TT_EVENT_27 = 1u<<26,
    TT_EVENT_28 = 1u<<27,
    TT_EVENT_29 = 1u<<28,
    TT_EVENT_30 = 1u<<29,
    TT_EVENT_31 = 1u<<30,
    TT_EVENT_32 = 1u<<31,
} tt_task_event_e;

// 以下函数必须由移植层实现
uint32_t tt_task_get_system_tick(void);
uint32_t tt_task_get_system_tick_ms(void);

#endif // TT_TASK_PORT_H 