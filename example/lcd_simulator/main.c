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

// 全局变量，用于控制程序运行
volatile bool g_app_running = true;

// 按钮事件定义
#define BUTTON1_EVENT TT_EVENT_1  // 按钮1被按下事件
#define BUTTON2_EVENT TT_EVENT_2  // 按钮2被按下事件

// 按钮结构体定义
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    char *text;
    bool selected;
    int animation_frame;
} button_t;

// 按钮动画帧数
#define MAX_ANIMATION_FRAMES 10

// 全局按钮
button_t buttons[2];
int current_selected_button = 0;

// 绘制矩形 - 完全填充
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

// 绘制边框 - 只有轮廓
void draw_rect_outline(int x, int y, int width, int height, uint32_t color) {
    // 确保坐标在有效范围内
    if (x < 0) { width += x; x = 0; }
    if (y < 0) { height += y; y = 0; }
    if (x + width > LCD_WIDTH) width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT) height = LCD_HEIGHT - y;
    if (width <= 0 || height <= 0) return;
    
    // 绘制水平线
    for (int i = 0; i < width; i++) {
        lcd_draw_pixel(x + i, y, color);
        if (y + height - 1 < LCD_HEIGHT)
            lcd_draw_pixel(x + i, y + height - 1, color);
    }
    
    // 绘制垂直线
    for (int j = 1; j < height - 1; j++) {
        lcd_draw_pixel(x, y + j, color);
        if (x + width - 1 < LCD_WIDTH)
            lcd_draw_pixel(x + width - 1, y + j, color);
    }
}

// 简化的Windows风格按钮
void draw_button(button_t *button) {
    // 绘制按钮背景 - 使用灰色背景
    draw_rect_filled(button->x, button->y, button->width, button->height, COLOR_GRAY);
    
    // 绘制黑色边框
    draw_rect_outline(button->x, button->y, button->width, button->height, COLOR_BLACK);
    
    // 如果按钮被选中，绘制蓝色边框
    if (button->selected) {
        // 绘制选中边框 - 淡蓝色
        int offset = 2;  // 边框偏移量
        draw_rect_outline(button->x - offset, button->y - offset, 
                         button->width + 2*offset, button->height + 2*offset, 
                         COLOR_LIGHT_BLUE);
        
        // 增加动画效果 - 第二个蓝色边框
        int anim_offset = button->animation_frame % MAX_ANIMATION_FRAMES;
        if (anim_offset < MAX_ANIMATION_FRAMES / 2) {
            offset = 3;  // 第二层边框偏移量
            draw_rect_outline(button->x - offset, button->y - offset, 
                             button->width + 2*offset, button->height + 2*offset, 
                             COLOR_LIGHT_BLUE);
        }
    }
    
    // 绘制按钮文本 - 黑色文字
    int text_len = strlen(button->text);
    int text_width = text_len * FONT_WIDTH;
    int text_x = button->x + (button->width - text_width) / 2;
    int text_y = button->y + (button->height - FONT_HEIGHT) / 2;
    
    // 确保文字在按钮内
    if (text_x < button->x) text_x = button->x + 2;
    
    draw_string(text_x, text_y, button->text, COLOR_GREEN);
}

// 初始化按钮
void init_buttons() {
    // 初始化按钮1
    buttons[0].x = 20;
    buttons[0].y = 10;
    buttons[0].width = 90;
    buttons[0].height = 22;
    buttons[0].text = "button1";
    buttons[0].selected = true;
    buttons[0].animation_frame = 0;
    
    // 初始化按钮2
    buttons[1].x = 20;
    buttons[1].y = 40;
    buttons[1].width = 90;
    buttons[1].height = 22;
    buttons[1].text = "button2";
    buttons[1].selected = false;
    buttons[1].animation_frame = 0;
    
    // 默认选中第一个按钮
    current_selected_button = 0;
}

// 处理按钮选择
void handle_button_selection(lcd_key_state_t key) {
    if (key == KEY_UP_PRESSED) {
        // 取消当前按钮选中状态
        buttons[current_selected_button].selected = false;
        
        // 移动到上一个按钮
        current_selected_button = (current_selected_button == 0) ? 1 : 0;
        
        // Select new button
        buttons[current_selected_button].selected = true;
        
        printf("Selected button %d\n", current_selected_button + 1);
    }
    else if (key == KEY_DOWN_PRESSED) {
        // 取消当前按钮选中状态
        buttons[current_selected_button].selected = false;
        
        // 移动到下一个按钮
        current_selected_button = (current_selected_button == 0) ? 1 : 0;
        
        // Select new button
        buttons[current_selected_button].selected = true;
        
        printf("Selected button %d\n", current_selected_button + 1);
    }
    else if (key == KEY_ENTER_PRESSED) {
        // 按下回车键，触发按钮事件
        if (current_selected_button == 0) {
            tt_event_notify(BUTTON1_EVENT);
        } else {
            tt_event_notify(BUTTON2_EVENT);
        }
    }
}

// 显示任务
void display_task(void* arg) {
    TT_TASK_START;
    
    while (g_app_running) {
        // 清屏 - 设为全黑
        lcd_clear();
        
        // 更新按钮动画帧
        for (int i = 0; i < 2; i++) {
            if (buttons[i].selected) {
                buttons[i].animation_frame = (buttons[i].animation_frame + 1) % MAX_ANIMATION_FRAMES;
            }
        }
        
        // 绘制两个按钮
        draw_button(&buttons[0]);
        draw_button(&buttons[1]);
        
        // 更新显示
        lcd_update();
        
        // 延迟50ms
        TT_TASK_DELAY_MS(50);
    }
    
    TT_TASK_END;
}

// 按键处理任务
void key_task(void* arg) {
    TT_TASK_START;
    
    printf("Key task started, press 'u' to select up, 'd' to select down, Enter to confirm\n");
    
    while (g_app_running) {
        // 获取按键状态
        lcd_key_state_t key_state = lcd_get_key_state();
        
        // 处理按键事件
        if (key_state != KEY_NONE) {
            handle_button_selection(key_state);
        }
        
        // 检查退出事件
        if (lcd_get_event() != 0) {
            g_app_running = false;
            
            // 发出结束任务的信号
            printf("Exit event detected, stopping program...\n");
            
            // 退出任务调度循环
            tt_task_stop_schedule();
            break;
        }
        
        // 短暂休眠
        TT_TASK_DELAY_MS(10);
    }
    
    TT_TASK_END;
}

// 按钮1事件处理任务
void button1_event_task(void* arg) {
    TT_TASK_START;
    
    // 注册关注BUTTON1_EVENT事件
    TT_EVENT_REGISTER(BUTTON1_EVENT);
    printf("Button1 event task started\n");
    
    while (g_app_running) {
        // 等待按钮1事件
        TT_TASK_WAIT_EVENT(BUTTON1_EVENT, TT_TASK_WAIT_FOREVER);
        
        // 处理按钮1事件
        printf("====> Button1 pressed! Handling button1 event <====\n");
        
        // 清除事件标志
        TT_TASK_CLEAR_EVENT(BUTTON1_EVENT);
    }
    
    TT_TASK_END;
}

// 按钮2事件处理任务
void button2_event_task(void* arg) {
    TT_TASK_START;
    
    // 注册关注BUTTON2_EVENT事件
    TT_EVENT_REGISTER(BUTTON2_EVENT);
    printf("Button2 event task started\n");
    
    while (g_app_running) {
        // 等待按钮2事件
        TT_TASK_WAIT_EVENT(BUTTON2_EVENT, TT_TASK_WAIT_FOREVER);
        
        // 处理按钮2事件
        printf("====> Button2 pressed! Handling button2 event <====\n");
        
        // 清除事件标志
        TT_TASK_CLEAR_EVENT(BUTTON2_EVENT);
    }
    
    TT_TASK_END;
}

int main() {
    // 初始化LCD
    if (lcd_init() != 0) {
        fprintf(stderr, "LCD initialization failed\n");
        return -1;
    }
    
    printf("LCD simulator started, screen size: %dx%d\n", LCD_WIDTH, LCD_HEIGHT);
    
    // 初始化TinyTask
    tt_task_init();
    
    // 初始化系统时钟
    init_systick();
    
    // 初始化按钮
    init_buttons();
    
    // 定义任务结构体
    tt_task_t display_task_struct;
    tt_task_t key_task_struct;
    tt_task_t button1_event_task_struct;
    tt_task_t button2_event_task_struct;
    
    // 创建显示任务
    display_task_struct.arg = NULL;
    tt_task_create(&display_task_struct, "display_task", display_task);
    
    // 创建按键任务
    key_task_struct.arg = NULL;
    tt_task_create(&key_task_struct, "key_task", key_task);
    
    // 创建按钮事件处理任务
    button1_event_task_struct.arg = NULL;
    tt_task_create(&button1_event_task_struct, "button1_event_task", button1_event_task);
    
    button2_event_task_struct.arg = NULL;
    tt_task_create(&button2_event_task_struct, "button2_event_task", button2_event_task);
    
    // 启动任务调度（这里会进入任务调度循环）
    tt_task_start_schedule();
    
    // 调度器退出后执行清理工作
    deinit_systick();
    lcd_deinit();
    
    printf("LCD simulator closed\n");
    return 0;
} 