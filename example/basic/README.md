# 基础任务示例

本示例展示了TinyTask最基本的功能：创建和调度任务。

## 功能特点

- 创建两个独立的周期性任务
- 使用任务延时函数控制任务执行频率
- 演示任务调度器的基本运行机制

## 代码说明

示例中创建了两个任务：
- `task1`: 每555毫秒执行一次，打印当前时间
- `task2`: 每1000毫秒执行一次，打印当前时间

核心代码：
```c
// 创建任务结构
tt_task_t task1, task2;

// 任务1函数
void task1_func(void *arg)
{
  TT_TASK_START;
  while (1) {
    printf("task1 running1,%d\n",tt_task_get_system_tick_ms());
    TT_TASK_DELAY_MS(555);
  }
  TT_TASK_END;
}

// 任务2函数
void task2_func(void *arg)
{
  TT_TASK_START;
  while (1) {
    printf("task2 running2,%d\n",tt_task_get_system_tick_ms());
    TT_TASK_DELAY_MS(1000);
  }
  TT_TASK_END;
}

// 主函数
int main()
{
  printf("db task test\n");
  tt_task_init();        // 初始化任务系统
  init_systick();        // 初始化系统时钟

  // 创建任务
  tt_task_create(&task1, "task1", task1_func);
  tt_task_create(&task2, "task2", task2_func);

  // 启动调度器
  tt_task_start_schedule();

  return 0;
}
```

## 编译与运行

```bash
# 在TinyTask根目录中
mkdir -p build && cd build
cmake ..
make
./example/basic/basic
```

## 预期输出

程序运行后，将看到两个任务交替执行，大致如下：

```
db task test
task1 running1,0
task2 running2,0
task1 running1,555
task1 running1,1110
task2 running2,1000
task1 running1,1665
...
```

通过这个示例，你可以了解TinyTask最基本的任务创建和调度机制。 