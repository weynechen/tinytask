#include "tt_task_port.h"

static uint32_t system_tick = 0;

void tt_task_increment_system_tick(void)
{
    system_tick++;
}

uint32_t tt_task_get_system_tick(void)
{
    return system_tick;
}

uint32_t tt_task_get_system_tick_ms(void)
{
    return system_tick;
} 