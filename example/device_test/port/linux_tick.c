#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "tt_task_port.h"

static pthread_t tick_thread;
static int running = 1;

// 线程函数
void* thread_function(void* arg)
{
    struct timespec start, end;
    long elapsed_ns;
    const long target_ns = 1000000; // 1ms = 1000000ns

    while (running) {
        // 获取开始时间
        clock_gettime(CLOCK_MONOTONIC, &start);

        // 执行任务
        tt_task_increment_system_tick();

        // 等待直到1ms过去
        do {
            clock_gettime(CLOCK_MONOTONIC, &end);
            elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        } while (elapsed_ns < target_ns);
    }
    return NULL;
}

int init_systick()
{
    // 创建线程
    int ret = pthread_create(&tick_thread, NULL, thread_function, NULL);
    
    if (ret != 0) {
        printf("thread create failed!\n");
        return 1;
    } else {
        printf("systick init success!\n");
    }

    return 0;
}

void deinit_systick()
{
    // 停止线程
    running = 0;
    pthread_join(tick_thread, NULL);
} 
