#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// LCD屏幕尺寸配置
#define LCD_WIDTH  128
#define LCD_HEIGHT 64

// 颜色定义 (32位RGBA格式)
#define COLOR_BLACK     0xFF000000  // 黑色
#define COLOR_WHITE     0xFFFFFFFF  // 白色
#define COLOR_RED       0xFFFF0000  // 红色
#define COLOR_GREEN     0xFF00FF00  // 绿色
#define COLOR_BLUE      0xFF0000FF  // 蓝色
#define COLOR_YELLOW    0xFFFFFF00  // 黄色
#define COLOR_GRAY      0xFFAAAAAA  // 灰色
#define COLOR_LIGHT_BLUE 0xFF8888FF // 淡蓝色

// 原始像素状态定义 (为了兼容性保留，但推荐使用上面的颜色常量)
typedef enum {
    LCD_PIXEL_OFF = COLOR_BLACK,  // 黑色（关闭）
    LCD_PIXEL_ON = COLOR_WHITE,   // 白色（开启）
    LCD_PIXEL_RED = COLOR_RED,    // 红色
    LCD_PIXEL_GREEN = COLOR_GREEN, // 绿色
    LCD_PIXEL_BLUE = COLOR_BLUE,  // 蓝色
    LCD_PIXEL_YELLOW = COLOR_YELLOW, // 黄色
    LCD_PIXEL_GRAY = COLOR_GRAY,  // 灰色
} lcd_pixel_state_t;

// 按键状态定义
typedef enum {
    KEY_NONE = 0,
    KEY_UP_PRESSED,
    KEY_DOWN_PRESSED,
    KEY_ENTER_PRESSED
} lcd_key_state_t;

// LCD初始化
int lcd_init(void);

// LCD清屏
void lcd_clear(void);

// 绘制单个像素 - 使用32位颜色值
void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

// 刷新显示 - 将缓冲区内容绘制到屏幕
void lcd_update(void);

// 关闭LCD并释放资源
void lcd_deinit(void);

// 获取事件，返回0表示没有事件，非0表示有事件（可用于检测窗口关闭）
int lcd_get_event(void);

// 获取当前按键状态
lcd_key_state_t lcd_get_key_state(void);

// LCD 模拟器相关函数 (仅用于模拟环境)
void lcd_simulator_process_events(void);
void lcd_simulator_wait_for_events(int timeout_ms);
bool lcd_simulator_button_pressed(int button_id);

#endif /* LCD_DRIVER_H */ 