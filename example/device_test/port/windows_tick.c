#include <stdio.h>
#include <windows.h>
#include "tt_task_port.h"

static HANDLE hThread;

// 线程函数
DWORD WINAPI ThreadFunction(LPVOID lpParam)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
    double elapsedTime;

    // 获取高精度计时器的频率
    QueryPerformanceFrequency(&frequency);

    while (1) {
        // 获取开始时间
        QueryPerformanceCounter(&start);

        // 执行任务
        tt_task_increment_system_tick();

        // 等待直到1ms过去
        do {
            QueryPerformanceCounter(&end);
            elapsedTime = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
        } while (elapsedTime < 1.0);
    }
    return 0;
}

int init_systick()
{
    // 创建线程
    hThread = CreateThread(NULL,           // 默认安全属性
                         0,                // 默认栈大小
                         ThreadFunction,   // 线程函数
                         NULL,             // 线程函数参数
                         0,                // 默认的创建标志
                         NULL              // 线程标识符
    );

    if (hThread == NULL) {
        printf("系统滴答线程创建失败。\n");
        return 1;
    } else {
        printf("系统滴答初始化成功。\n");
    }

    return 0;
}

void deinit_systick()
{
    // 关闭线程句柄
    CloseHandle(hThread);
} 