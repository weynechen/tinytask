#include <stdio.h>
#include "tt_task_port.h"
#include "tt_task.h"
#include "key.h"
#include "key_task.h"
#include "event_tasks.h"
#include "tt_device.h"

#ifdef PLATFORM_WINDOWS
#include "windows_tick.h"
#elif defined(PLATFORM_LINUX)
#include "linux_tick.h"
#endif

// Define task structures
static tt_task_t key_task;
static tt_task_t short_press_task;
static tt_task_t long_press_task;

// Define key device
static key_device_t key_dev;

int main()
{
    printf("Key device test program\n");
    printf("Please use 's,l' keys on keyboard to simulate buttons\n");
    printf("Short press 's': trigger short press event\n");
    printf("Long press 'l': trigger long press event\n");
    
    // Initialize task system
    tt_task_init();
    init_systick();
    
    // Register key device
    register_key_device(&key_dev);
    
    // Create tasks
    tt_task_create(&key_task, "key_task", key_task_func);
    tt_task_create(&short_press_task, "short_press_task", short_press_task_func);
    tt_task_create(&long_press_task, "long_press_task", long_press_task_func);
    
    // Start task scheduling
    tt_task_start_schedule();
    
    return 0;
} 