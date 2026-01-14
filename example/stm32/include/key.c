#include "key.h"
#include "tt_task.h"
#include "main.h"
#include "stm32_def.h"
#include <stdio.h>

static key_device_t *key_dev = NULL;

// Initialize key device
static int key_init(tt_device_t *dev)
{
    key_device_t *key = (key_device_t *)dev;
    key->last_state = KEY_NO_PRESS;
    key->press_time = 0;
    printf("key device init success\n");
    return 0;
}

// Suspend key device
static int key_suspend(tt_device_t *dev)
{
    UNUSED(dev);
    printf("key device suspend\n");
    return 0;
}

// Resume key device
static int key_resume(tt_device_t *dev)
{
    UNUSED(dev);
    printf("key device resume\n");
    return 0;
}

// Deinitialize key device
static int key_deinit(tt_device_t *dev)
{
    UNUSED(dev);
    printf("key device deinit\n");
    return 0;
}

// Register key device
void register_key_device(key_device_t *key)
{
    if (key == NULL) {
        return;
    }
    
    key->base.name = "key";
    key->base.init = key_init;
    key->base.deinit = key_deinit;
    key->base.suspend = key_suspend;
    key->base.resume = key_resume;
    key->scan = key_scan;
    
    tt_device_add(&key->base);
    key_dev = key;
    
    key->base.init(&key->base);
}

// Key scan function
uint8_t key_scan(void)
{
    uint8_t key_status = KEY_NO_PRESS;
    static uint8_t key_last_state = 1; // Last key state, default to released state
    uint8_t key_current_state;
    uint32_t current_time;
    
    // Read current key state (STM32 keys are typically active low)
    key_current_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
    current_time = tt_task_get_system_tick_ms();
    
    // Key state change
    if (key_current_state != key_last_state) {
        if (key_current_state == 0) { // Key pressed
            // Record press time
            if (key_dev != NULL) {
                key_dev->press_time = current_time;
                key_dev->last_state = KEY_NO_PRESS;
            }
        } else { // Key released
            if (key_dev != NULL && key_dev->press_time > 0) {
                // Judge press time duration, distinguish short and long press
                if (current_time - key_dev->press_time >= KEY_LONG_PRESS_THRESHOLD) {
                    key_status = KEY_LONG_PRESS;
                } else {
                    key_status = KEY_SHORT_PRESS;
                }
                key_dev->press_time = 0;
            }
        }
        key_last_state = key_current_state;
    } else if (key_current_state == 0 && key_dev != NULL && key_dev->press_time > 0) {
        // Key continuously pressed, check if long press threshold reached
        if (current_time - key_dev->press_time >= KEY_LONG_PRESS_THRESHOLD && 
            key_dev->last_state != KEY_LONG_PRESS) {
            key_status = KEY_LONG_PRESS;
            key_dev->last_state = KEY_LONG_PRESS;
        }
    }
    
    return key_status;
} 