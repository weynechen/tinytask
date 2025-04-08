#include <stdio.h>
#include "tt_task_port.h"
#include "tt_task.h"
#ifdef PLATFORM_WINDOWS
#include "windows_tick.h"
#elif defined(PLATFORM_LINUX)
#include "linux_tick.h"
#endif

tt_task_t task1, task2;

void task1_func(void *arg)
{
  TT_TASK_START;
  while (1) {
    printf("task1 running1,%d\n",tt_task_get_system_tick_ms());
    TT_TASK_DELAY_MS(555);
  }
  TT_TASK_END;
}


void task2_func(void *arg)
{
  TT_TASK_START;
  while (1) {
    printf("task2 running2,%d\n",tt_task_get_system_tick_ms());
    TT_TASK_DELAY_MS(1000);
  }
  TT_TASK_END;
}

int main()
{
  printf("db task test\n");
  tt_task_init();
  init_systick();

  tt_task_create(&task1, "task1", task1_func);
  tt_task_create(&task2, "task2", task2_func);

  tt_task_start_schedule();

  return 0;
}