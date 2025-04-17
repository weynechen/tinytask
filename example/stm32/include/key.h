#ifndef __KEY_H__
#define __KEY_H__

#include "tt_device.h"
#include <stdint.h>

// 按键状态定义
#define KEY_NO_PRESS    0
#define KEY_SHORT_PRESS 1
#define KEY_LONG_PRESS  2

// 长按阈值（毫秒）
#define KEY_LONG_PRESS_THRESHOLD 500

// 按键设备结构体
typedef struct {
    tt_device_t base;
    uint8_t (*scan)(void);
    uint8_t last_state;
    uint32_t press_time;
} key_device_t;

// 函数声明
uint8_t key_scan(void);
void register_key_device(key_device_t *key);

#endif // __KEY_H__ 