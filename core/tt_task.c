#include "tt_task.h"
#include <stdint.h>
#include <string.h>

static tt_list_t *tt_task_list = NULL;
static tt_list_t *head = NULL;
static tt_list_t *event_map[TT_TASK_MAX_EVENT_NUM];

#define GET_TASK_FROM_NODE(ptr,member) \
  ((tt_task_t *)((char *)(ptr) - (uintptr_t)(&((tt_task_t *)0)->member)))

void tt_task_init()
{
  tt_task_list = NULL;
  head = NULL;
  memset(event_map, 0, sizeof(event_map));
}

void tt_task_create(tt_task_t *task, char *name, void (*func)(void *arg))
{
  if (tt_task_list == NULL) {
    tt_task_list = &task->node;
    // 记录下第一个节点
    head = &task->node;
  } else {
    tt_task_list->next = &task->node;
    tt_task_list = &task->node;
  }
  // 最新节点永远指向第一个节点，形成循环链表结构
  task->node.next = head;
  task->name = name;
  task->func = func;
  task->arg = task;
  task->state = 0;
  task->event = 0;
  task->event_node.next = NULL;
}

void tt_task_suspend(tt_task_t *task)
{
  task->state = -1;
}

void tt_task_resume(tt_task_t *task)
{
  task->state = 0;
}

void tt_task_start_schedule()
{
  tt_list_t *list = head;
  while(tt_task_list != NULL) {
    // 根据地址偏移，取出task结构体
    tt_task_t *task = GET_TASK_FROM_NODE(list,node);
    if ((task->func != NULL) && (task->state != -1)){
      task->func(task->arg);
    }
    list = task->node.next;
  }
}

void tt_task_stop_schedule()
{
    tt_task_list = NULL;
    head = NULL;
    memset(event_map, 0, sizeof(event_map));
}

void tt_event_register(tt_task_t *task, int event_mask)
{
  int i;
  for (i = 0; i < TT_TASK_MAX_EVENT_NUM; i++) {
    if (event_mask & (1 << i)) {
      if (event_map[i] == NULL) {
        event_map[i] = &task->event_node;
      } else {
        tt_list_t *node = event_map[i];
        while (node != NULL) {
          if (node == &task->event_node) {
            return;
          }
          node = node->next;
        }
          task->event_node.next = event_map[i];
          event_map[i] = &task->event_node;
      }
    }
  }
}

void tt_event_notify(int event_mask)
{
  int i;
  for (i = 0; i < TT_TASK_MAX_EVENT_NUM; i++) {
    if (event_mask & (1 << i)) {
      tt_list_t *list = event_map[i];
      while (list != NULL) {
        tt_task_t *task = GET_TASK_FROM_NODE(list,event_node);
        task->event |= (1 << i);
        list = task->event_node.next;
      }
    }
  }
}
