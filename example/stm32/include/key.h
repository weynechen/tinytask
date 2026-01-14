#ifndef __KEY_H__
#define __KEY_H__

#include "tt_device.h"
#include <stdint.h>

// Key state definition
#define KEY_NO_PRESS    0
#define KEY_SHORT_PRESS 1
#define KEY_LONG_PRESS  2

// Long press threshold (milliseconds)
#define KEY_LONG_PRESS_THRESHOLD 500

// Key device structure
typedef struct {
    tt_device_t base;
    uint8_t (*scan)(void);
    uint8_t last_state;
    uint32_t press_time;
} key_device_t;

// Function declarations
uint8_t key_scan(void);
void register_key_device(key_device_t *key);

#endif // __KEY_H__ 