#ifndef TT_TASK_H
#define TT_TASK_H
#include "tt_list.h"
#include "tt_task_port.h"
#include <stdint.h>
#include <stddef.h>

#define TT_TASK_WAIT_FOREVER 0xffffffff

typedef struct {
  char *name;
  void (*func)(void *arg);
  void *arg;
  int32_t state;
  uint32_t time;
  tt_event_t event;
  tt_list_node_t node;
  tt_list_node_t event_node;
} tt_task_t;

void tt_task_init(void);
void tt_task_create(tt_task_t *task, char *name, void (*func)(void *arg));
void tt_task_start_schedule(void);
void tt_task_stop_schedule(void);
void tt_event_register(tt_task_t *task, int event_mask);
void tt_event_notify(int event_mask);

#define TT_TASK_START                                                          \
  tt_task_t *task = (tt_task_t *)arg;                                          \
  switch (task->state) {                                                       \
  case 0:

#define TT_TASK_END }

#define TT_TASK_DELAY_MS(ms)                                                   \
  do {                                                                         \
    task->time = tt_task_get_system_tick_ms();                                 \
    task->state = __LINE__;                                                    \
  case __LINE__:;                                                              \
    if (tt_task_get_system_tick_ms() - task->time < ms)                        \
      return;                                                                  \
  } while (0)

#define TT_TASK_DELAY_TICK(tick)                                               \
  do {                                                                         \
    task->time = tt_task_get_system_tick();                                    \
    task->state = __LINE__;                                                    \
  case __LINE__:;                                                              \
    if (tt_task_get_system_tick() - task->time < tick)                         \
      return;                                                                  \
  } while (0)

#define TT_TASK_WAIT_EVENT(event_mask, timeout)                                                   \
  do {                                                                         \
    task->state = __LINE__;                                                    \
    task->time = tt_task_get_system_tick_ms();                                 \
  case __LINE__:;                                                              \
    if ((task->event & (event_mask)) != (event_mask)) {\
      if (timeout == TT_TASK_WAIT_FOREVER)                                        \
        return;                                                                  \
      if (tt_task_get_system_tick_ms() - task->time < (timeout))                \
        return;                                                                  \
    }                                                                          \
  } while (0)

#define TT_TASK_CLEAR_EVENT(event_mask)                                                  \
  do {                                                                         \
    task->event &= (~(event_mask));                                                           \
  } while (0)


#define TT_EVENT_REGISTER(event) tt_event_register(task, event)

#endif

