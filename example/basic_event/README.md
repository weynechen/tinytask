# 事件驱动示例

本示例展示了TinyTask的事件机制，通过事件实现任务间的通信和同步。

## 功能特点

- 演示事件的注册和使用
- 展示如何等待和发送事件
- 实现任务间的通信与同步
- 演示事件超时处理

## 代码说明

示例中创建了两个任务：
- `task1`: 注册并等待事件，处理接收到的事件或超时
- `task2`: 定期发送事件给task1

核心代码：
```c
// 创建任务结构
tt_task_t task1, task2;

// 任务1函数 - 等待事件
void task1_func(void *arg)
{
  TT_TASK_START;
  TT_EVENT_REGISTER(TT_EVENT_2|TT_EVENT_1);  // 注册感兴趣的事件
  while (1) {
    printf("task1 running1,%d\n",tt_task_get_system_tick_ms());
    // 等待事件1，超时时间5000ms
    TT_TASK_WAIT_EVENT(TT_EVENT_1,5000);
    if((task->event & TT_EVENT_1) != TT_EVENT_1){
      printf("wait event 1 timeout\n");  // 超时处理
    }else{
      printf("wait event 1 success\n");  // 成功接收事件
    }
    TT_TASK_DELAY_MS(555);
  }
  TT_TASK_END;
}

// 任务2函数 - 发送事件
void task2_func(void *arg)
{
  TT_TASK_START;
  while (1) {
    printf("task2 running2,%d\n",tt_task_get_system_tick_ms());
    TT_TASK_DELAY_MS(1000);
    // 每1000ms发送一次事件1
    tt_event_notify(TT_EVENT_1);
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
```

## 事件机制说明

TinyTask提供以下事件相关的函数和宏：
- `TT_EVENT_REGISTER()`: 注册任务关注的事件
- `TT_TASK_WAIT_EVENT()`: 等待事件，可设置超时时间
- `tt_event_notify()`: 向系统发送事件通知
- `TT_TASK_CLEAR_EVENT()`: 清除已处理的事件标志

## 编译与运行

```bash
# 在TinyTask根目录中
mkdir -p build && cd build
cmake ..
make
./example/basic_event/basic_event
```

## 预期输出

程序运行后，将看到两个任务交替执行，task2定期发送事件，task1接收并处理事件：

```
db task test
task1 running1,0
task2 running2,0
task2 running2,1000
wait event 1 success
task1 running1,1000
task1 running1,1555
task2 running2,2000
wait event 1 success
task1 running1,2110
...
```

通过这个示例，你可以了解TinyTask的事件机制如何实现任务间的通信和同步。 