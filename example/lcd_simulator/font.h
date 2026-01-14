#ifndef FONT_H
#define FONT_H

#include "lcd_driver.h"
#include <stdint.h>

// Font size definition
#define FONT_WIDTH  16
#define FONT_HEIGHT 16

// Draw single character
void draw_char(int x, int y, char ch, uint32_t color);

// Draw string
void draw_string(int x, int y, const char *str, uint32_t color);

#endif /* FONT_H */ 