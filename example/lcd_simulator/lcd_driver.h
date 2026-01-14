#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// LCD screen size configuration
#define LCD_WIDTH  128
#define LCD_HEIGHT 64

// Color definitions (32-bit RGBA format)
#define COLOR_BLACK     0xFF000000  // Black
#define COLOR_WHITE     0xFFFFFFFF  // White
#define COLOR_RED       0xFFFF0000  // Red
#define COLOR_GREEN     0xFF00FF00  // Green
#define COLOR_BLUE      0xFF0000FF  // Blue
#define COLOR_YELLOW    0xFFFFFF00  // Yellow
#define COLOR_GRAY      0xFFAAAAAA  // Gray
#define COLOR_LIGHT_BLUE 0xFF8888FF // Light blue

// Original pixel state definitions (kept for compatibility, color constants above are recommended)
typedef enum {
    LCD_PIXEL_OFF = COLOR_BLACK,  // Black (off)
    LCD_PIXEL_ON = COLOR_WHITE,   // White (on)
    LCD_PIXEL_RED = COLOR_RED,    // Red
    LCD_PIXEL_GREEN = COLOR_GREEN, // Green
    LCD_PIXEL_BLUE = COLOR_BLUE,  // Blue
    LCD_PIXEL_YELLOW = COLOR_YELLOW, // Yellow
    LCD_PIXEL_GRAY = COLOR_GRAY,  // Gray
} lcd_pixel_state_t;

// Key state definitions
typedef enum {
    KEY_NONE = 0,
    KEY_UP_PRESSED,
    KEY_DOWN_PRESSED,
    KEY_ENTER_PRESSED
} lcd_key_state_t;

// LCD initialization
int lcd_init(void);

// LCD clear screen
void lcd_clear(void);

// Draw single pixel - use 32-bit color value
void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

// Refresh display - draw buffer content to screen
void lcd_update(void);

// Close LCD and release resources
void lcd_deinit(void);

// Get events, return 0 means no event, non-zero means event (can be used to detect window close)
int lcd_get_event(void);

// Get current key state
lcd_key_state_t lcd_get_key_state(void);

// LCD simulator related functions (only for simulation environment)
void lcd_simulator_process_events(void);
void lcd_simulator_wait_for_events(int timeout_ms);
bool lcd_simulator_button_pressed(int button_id);

#endif /* LCD_DRIVER_H */ 