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

    // Get high-precision timer frequency
    QueryPerformanceFrequency(&frequency);

    while (1) {
        // Get start time
        QueryPerformanceCounter(&start);

        // Execute task
        tt_task_increment_system_tick();

        // Wait until 1ms passes
        do {
            QueryPerformanceCounter(&end);
            elapsedTime = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
        } while (elapsedTime < 1.0);
    }
    return 0;
}

int init_systick()
{
    // Create thread
    hThread = CreateThread(NULL,           // Default security attributes
                         0,                // Default stack size
                         ThreadFunction,   // Thread function
                         NULL,             // Thread function parameter
                         0,                // Default creation flags
                         NULL              // Thread identifier
    );

    if (hThread == NULL) {
        printf("System tick thread creation failed.\n");
        return 1;
    } else {
        printf("System tick init success.\n");
    }

    return 0;
}

void deinit_systick()
{
    // Close thread handle
    CloseHandle(hThread);
} 