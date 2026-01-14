#include "lcd_driver.h"
#include "tt_task.h"
#include "tt_task_port.h"
#include "font.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#ifdef PLATFORM_WINDOWS
#include "windows_tick.h"
#elif defined(PLATFORM_LINUX)
#include "linux_tick.h"
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global variables for controlling program execution
volatile bool g_app_running = true;

// Button event definitions
#define BUTTON1_EVENT TT_EVENT_1  // Button 1 pressed event
#define BUTTON2_EVENT TT_EVENT_2  // Button 2 pressed event

// Button structure definition
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    char *text;
    bool selected;
    int animation_frame;
} button_t;

// Button animation frame count
#define MAX_ANIMATION_FRAMES 10

// Global buttons
button_t buttons[2];
int current_selected_button = 0;

// Draw rectangle - completely filled
void draw_rect_filled(int x, int y, int width, int height, uint32_t color) {
    // 确保坐标在有效范围内
    if (x < 0) { width += x; x = 0; }
    if (y < 0) { height += y; y = 0; }
    if (x + width > LCD_WIDTH) width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT) height = LCD_HEIGHT - y;
    if (width <= 0 || height <= 0) return;
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            lcd_draw_pixel(x + i, y + j, color);
        }
    }
}

// Draw border - outline only
void draw_rect_outline(int x, int y, int width, int height, uint32_t color) {
    // 确保坐标在有效范围内
    if (x < 0) { width += x; x = 0; }
    if (y < 0) { height += y; y = 0; }
    if (x + width > LCD_WIDTH) width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT) height = LCD_HEIGHT - y;
    if (width <= 0 || height <= 0) return;
    
    // Draw horizontal lines
    for (int i = 0; i < width; i++) {
        lcd_draw_pixel(x + i, y, color);
        if (y + height - 1 < LCD_HEIGHT)
            lcd_draw_pixel(x + i, y + height - 1, color);
    }
    
    // Draw vertical lines
    for (int j = 1; j < height - 1; j++) {
        lcd_draw_pixel(x, y + j, color);
        if (x + width - 1 < LCD_WIDTH)
            lcd_draw_pixel(x + width - 1, y + j, color);
    }
}

// Simplified Windows-style button
void draw_button(button_t *button) {
    // Draw button background - use gray background
    draw_rect_filled(button->x, button->y, button->width, button->height, COLOR_GRAY);
    
    // Draw black border
    draw_rect_outline(button->x, button->y, button->width, button->height, COLOR_BLACK);
    
    // If button is selected, draw blue border
    if (button->selected) {
        // Draw selected border - light blue
            int offset = 2;  // Border offset
        draw_rect_outline(button->x - offset, button->y - offset, 
                         button->width + 2*offset, button->height + 2*offset, 
                         COLOR_LIGHT_BLUE);
        
            // Add animation effect - second blue border
        int anim_offset = button->animation_frame % MAX_ANIMATION_FRAMES;
        if (anim_offset < MAX_ANIMATION_FRAMES / 2) {
                offset = 3;  // Second layer border offset
            draw_rect_outline(button->x - offset, button->y - offset, 
                             button->width + 2*offset, button->height + 2*offset, 
                             COLOR_LIGHT_BLUE);
        }
    }
    
    // Draw button text - black text
    int text_len = strlen(button->text);
    int text_width = text_len * FONT_WIDTH;
    int text_x = button->x + (button->width - text_width) / 2;
    int text_y = button->y + (button->height - FONT_HEIGHT) / 2;
    
    // Ensure text is within button
    if (text_x < button->x) text_x = button->x + 2;
    
    draw_string(text_x, text_y, button->text, COLOR_GREEN);
}

// Initialize buttons
void init_buttons() {
    // Initialize button 1
    buttons[0].x = 20;
    buttons[0].y = 10;
    buttons[0].width = 90;
    buttons[0].height = 22;
    buttons[0].text = "button1";
    buttons[0].selected = true;
    buttons[0].animation_frame = 0;
    
    // Initialize button 2
    buttons[1].x = 20;
    buttons[1].y = 40;
    buttons[1].width = 90;
    buttons[1].height = 22;
    buttons[1].text = "button2";
    buttons[1].selected = false;
    buttons[1].animation_frame = 0;
    
    // Default select first button
    current_selected_button = 0;
}

// Handle button selection
void handle_button_selection(lcd_key_state_t key) {
    if (key == KEY_UP_PRESSED) {
        // Deselect current button
        buttons[current_selected_button].selected = false;
        
        // Move to next button
        current_selected_button = (current_selected_button == 0) ? 1 : 0;
        
        // Select new button
        buttons[current_selected_button].selected = true;
        
        printf("Selected button %d\n", current_selected_button + 1);
    }
    else if (key == KEY_DOWN_PRESSED) {
        // Deselect current button
        buttons[current_selected_button].selected = false;
        
        // Move to previous button
        current_selected_button = (current_selected_button == 0) ? 1 : 0;
        
        // Select new button
        buttons[current_selected_button].selected = true;
        
        printf("Selected button %d\n", current_selected_button + 1);
    }
    else if (key == KEY_ENTER_PRESSED) {
        // Press enter key, trigger button event
        if (current_selected_button == 0) {
            tt_event_notify(BUTTON1_EVENT);
        } else {
            tt_event_notify(BUTTON2_EVENT);
        }
    }
}

// Display task
void display_task(void* arg) {
    TT_TASK_START;
    
    while (g_app_running) {
        // Clear screen - set to all black
        lcd_clear();
        
        // Update button animation frames
        for (int i = 0; i < 2; i++) {
            if (buttons[i].selected) {
                buttons[i].animation_frame = (buttons[i].animation_frame + 1) % MAX_ANIMATION_FRAMES;
            }
        }
        
        // Draw two buttons
        draw_button(&buttons[0]);
        draw_button(&buttons[1]);
        
        // Update display
        lcd_update();
        
        // Delay 50ms
        TT_TASK_DELAY_MS(50);
    }
    
    TT_TASK_END;
}

// Key handling task
void key_task(void* arg) {
    TT_TASK_START;
    
    printf("Key task started, press 'u' to select up, 'd' to select down, Enter to confirm\n");
    
    while (g_app_running) {
        // Get key state
        lcd_key_state_t key_state = lcd_get_key_state();
        
        // Handle key events
        if (key_state != KEY_NONE) {
            handle_button_selection(key_state);
        }
        
        // Check exit event
        if (lcd_get_event() != 0) {
            g_app_running = false;
            
            // Signal end of task
            printf("Exit event detected, stopping program...\n");
            
            // Exit task scheduling loop
            tt_task_stop_schedule();
            break;
        }
        
        // Brief sleep
        TT_TASK_DELAY_MS(10);
    }
    
    TT_TASK_END;
}

// Button 1 event handling task
void button1_event_task(void* arg) {
    TT_TASK_START;
    
    // Register to listen for BUTTON1_EVENT
    TT_EVENT_REGISTER(BUTTON1_EVENT);
    printf("Button1 event task started\n");
    
    while (g_app_running) {
        // Wait for button 1 event
        TT_TASK_WAIT_EVENT(BUTTON1_EVENT, TT_TASK_WAIT_FOREVER);
        
        // Handle button 1 event
        printf("====> Button1 pressed! Handling button1 event <====\n");
        
        // 清除事件标志
        TT_TASK_CLEAR_EVENT(BUTTON1_EVENT);
    }
    
    TT_TASK_END;
}

// Button 2 event handling task
void button2_event_task(void* arg) {
    TT_TASK_START;
    
    // Register to listen for BUTTON2_EVENT
    TT_EVENT_REGISTER(BUTTON2_EVENT);
    printf("Button2 event task started\n");
    
    while (g_app_running) {
        // Wait for button 2 event
        TT_TASK_WAIT_EVENT(BUTTON2_EVENT, TT_TASK_WAIT_FOREVER);
        
        // Handle button 2 event
        printf("====> Button2 pressed! Handling button2 event <====\n");
        
        // Clear event flag
        TT_TASK_CLEAR_EVENT(BUTTON2_EVENT);
    }
    
    TT_TASK_END;
}

int main() {
    // Initialize LCD
    if (lcd_init() != 0) {
        fprintf(stderr, "LCD initialization failed\n");
        return -1;
    }
    
    printf("LCD simulator started, screen size: %dx%d\n", LCD_WIDTH, LCD_HEIGHT);
    
    // Initialize TinyTask
    tt_task_init();
    
    // Initialize system clock
    init_systick();
    
    // Initialize buttons
    init_buttons();
    
    // Define task structures
    tt_task_t display_task_struct;
    tt_task_t key_task_struct;
    tt_task_t button1_event_task_struct;
    tt_task_t button2_event_task_struct;
    
    // Create display task
    display_task_struct.arg = NULL;
    tt_task_create(&display_task_struct, "display_task", display_task);
    
    // Create key task
    key_task_struct.arg = NULL;
    tt_task_create(&key_task_struct, "key_task", key_task);
    
    // Create button event handling task
    button1_event_task_struct.arg = NULL;
    tt_task_create(&button1_event_task_struct, "button1_event_task", button1_event_task);
    
    button2_event_task_struct.arg = NULL;
    tt_task_create(&button2_event_task_struct, "button2_event_task", button2_event_task);
    
    // Start task scheduling (this will enter the task scheduling loop)
    tt_task_start_schedule();
    
    // Cleanup after scheduler exits
    deinit_systick();
    lcd_deinit();
    
    printf("LCD simulator closed\n");
    return 0;
} 