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

// 支持32个事件和64个事件，默认使用32个事件
#define TT_TASK_MAX_EVENT_NUM 64

// Event definitions as macros to avoid enum range warnings
#define TT_NO_EVENT  ((tt_event_t)0)
#define TT_EVENT_1   ((tt_event_t)1u<<0)
#define TT_EVENT_2   ((tt_event_t)1u<<1)
#define TT_EVENT_3   ((tt_event_t)1u<<2)
#define TT_EVENT_4   ((tt_event_t)1u<<3)
#define TT_EVENT_5   ((tt_event_t)1u<<4)
#define TT_EVENT_6   ((tt_event_t)1u<<5)
#define TT_EVENT_7   ((tt_event_t)1u<<6)
#define TT_EVENT_8   ((tt_event_t)1u<<7)
#define TT_EVENT_9   ((tt_event_t)1u<<8)
#define TT_EVENT_10  ((tt_event_t)1u<<9)
#define TT_EVENT_11  ((tt_event_t)1u<<10)
#define TT_EVENT_12  ((tt_event_t)1u<<11)
#define TT_EVENT_13  ((tt_event_t)1u<<12)
#define TT_EVENT_14  ((tt_event_t)1u<<13)
#define TT_EVENT_15  ((tt_event_t)1u<<14)
#define TT_EVENT_16  ((tt_event_t)1u<<15)
#define TT_EVENT_17  ((tt_event_t)1u<<16)
#define TT_EVENT_18  ((tt_event_t)1u<<17)
#define TT_EVENT_19  ((tt_event_t)1u<<18)
#define TT_EVENT_20  ((tt_event_t)1u<<19)
#define TT_EVENT_21  ((tt_event_t)1u<<20)
#define TT_EVENT_22  ((tt_event_t)1u<<21)
#define TT_EVENT_23  ((tt_event_t)1u<<22)
#define TT_EVENT_24  ((tt_event_t)1u<<23)
#define TT_EVENT_25  ((tt_event_t)1u<<24)
#define TT_EVENT_26  ((tt_event_t)1u<<25)
#define TT_EVENT_27  ((tt_event_t)1u<<26)
#define TT_EVENT_28  ((tt_event_t)1u<<27)
#define TT_EVENT_29  ((tt_event_t)1u<<28)
#define TT_EVENT_30  ((tt_event_t)1u<<29)
#define TT_EVENT_31  ((tt_event_t)1u<<30)
#define TT_EVENT_32  ((tt_event_t)1u<<31)
#if TT_TASK_MAX_EVENT_NUM > 32
#define TT_EVENT_33  ((tt_event_t)1ull<<32)
#define TT_EVENT_34  ((tt_event_t)1ull<<33)
#define TT_EVENT_35  ((tt_event_t)1ull<<34)
#define TT_EVENT_36  ((tt_event_t)1ull<<35)
#define TT_EVENT_37  ((tt_event_t)1ull<<36)
#define TT_EVENT_38  ((tt_event_t)1ull<<37)
#define TT_EVENT_39  ((tt_event_t)1ull<<38)
#define TT_EVENT_40  ((tt_event_t)1ull<<39)
#define TT_EVENT_41  ((tt_event_t)1ull<<40)
#define TT_EVENT_42  ((tt_event_t)1ull<<41)
#define TT_EVENT_43  ((tt_event_t)1ull<<42)
#define TT_EVENT_44  ((tt_event_t)1ull<<43)
#define TT_EVENT_45  ((tt_event_t)1ull<<44)
#define TT_EVENT_46  ((tt_event_t)1ull<<45)
#define TT_EVENT_47  ((tt_event_t)1ull<<46)
#define TT_EVENT_48  ((tt_event_t)1ull<<47)
#define TT_EVENT_49  ((tt_event_t)1ull<<48)
#define TT_EVENT_50  ((tt_event_t)1ull<<49)
#define TT_EVENT_51  ((tt_event_t)1ull<<50)
#define TT_EVENT_52  ((tt_event_t)1ull<<51)
#define TT_EVENT_53  ((tt_event_t)1ull<<52)
#define TT_EVENT_54  ((tt_event_t)1ull<<53)
#define TT_EVENT_55  ((tt_event_t)1ull<<54)
#define TT_EVENT_56  ((tt_event_t)1ull<<55)
#define TT_EVENT_57  ((tt_event_t)1ull<<56)
#define TT_EVENT_58  ((tt_event_t)1ull<<57)
#define TT_EVENT_59  ((tt_event_t)1ull<<58)
#define TT_EVENT_60  ((tt_event_t)1ull<<59)
#define TT_EVENT_61  ((tt_event_t)1ull<<60)
#define TT_EVENT_62  ((tt_event_t)1ull<<61)
#endif

#if TT_TASK_MAX_EVENT_NUM <= 32
typedef uint32_t tt_event_t;
#elif TT_TASK_MAX_EVENT_NUM <= 64
typedef uint64_t tt_event_t;
#else
#error "TT_TASK_MAX_EVENT_NUM is too large"
#endif

// 以下函数必须由移植层实现
void tt_task_increment_system_tick(void);
uint32_t tt_task_get_system_tick(void);
uint32_t tt_task_get_system_tick_ms(void);

#endif // TT_TASK_PORT_H 