#ifndef FONT_H
#define FONT_H

#include "lcd_driver.h"
#include <stdint.h>

// 字体大小定义
#define FONT_WIDTH  16
#define FONT_HEIGHT 16

// 绘制单个字符
void draw_char(int x, int y, char ch, uint32_t color);

// 绘制字符串
void draw_string(int x, int y, const char *str, uint32_t color);

#endif /* FONT_H */ 