#include "lcd_driver.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// SDL相关变量
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static uint32_t* pixel_buffer = NULL;

// 像素颜色定义 - 扩展为支持Windows样式
#define PIXEL_OFF_COLOR  0xFF000000 // 黑色（背景）
#define PIXEL_ON_COLOR   0xFFFFFFFF // 白色（前景）
#define PIXEL_GRAY_COLOR 0xFFD0D0D0 // 灰色（按钮背景）
#define PIXEL_BLUE_COLOR 0xFF4B9EFF // 淡蓝色（选中边框）

// 当前按键状态
static lcd_key_state_t current_key_state = KEY_NONE;

// 像素大小（放大倍数，使显示更清晰）
#define PIXEL_SCALE 4

int lcd_init(void) {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL初始化失败: %s\n", SDL_GetError());
        return -1;
    }

    // 创建窗口
    window = SDL_CreateWindow(
        "LCD模拟器",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LCD_WIDTH * PIXEL_SCALE,
        LCD_HEIGHT * PIXEL_SCALE,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "窗口创建失败: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "渲染器创建失败: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 创建纹理
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        LCD_WIDTH,
        LCD_HEIGHT
    );

    if (!texture) {
        fprintf(stderr, "纹理创建失败: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 创建像素缓冲区
    pixel_buffer = (uint32_t*)malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint32_t));
    if (!pixel_buffer) {
        fprintf(stderr, "内存分配失败\n");
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 清空像素缓冲区（初始化为黑色）
    memset(pixel_buffer, 0, LCD_WIDTH * LCD_HEIGHT * sizeof(uint32_t));

    return 0;
}

void lcd_clear(void) {
    if (pixel_buffer) {
        memset(pixel_buffer, 0, LCD_WIDTH * LCD_HEIGHT * sizeof(uint32_t));
    }
}

/**
 * @brief 在LCD上绘制一个像素点
 * 
 * @param x X坐标
 * @param y Y坐标
 * @param color 像素颜色 (32位ARGB格式)
 */
void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    // 检查坐标是否在有效范围内
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT || !pixel_buffer) {
        return;
    }

    // 直接设置像素颜色
    pixel_buffer[y * LCD_WIDTH + x] = color;
}

void lcd_update(void) {
    if (!renderer || !texture || !pixel_buffer) {
        return;
    }

    // 更新纹理数据
    SDL_UpdateTexture(texture, NULL, pixel_buffer, LCD_WIDTH * sizeof(uint32_t));

    // 清空渲染器
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 渲染纹理
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    
    // 显示渲染内容
    SDL_RenderPresent(renderer);
}

int lcd_get_event(void) {
    SDL_Event event;
    
    // 重置当前按键状态
    current_key_state = KEY_NONE;
    
    // 处理所有待处理的事件
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 1; // 窗口关闭事件
        }
        
        // 检测键盘事件
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_u:
                    current_key_state = KEY_UP_PRESSED;
                    break;
                case SDLK_d:
                    current_key_state = KEY_DOWN_PRESSED;
                    break;
                case SDLK_RETURN:
                case SDLK_SPACE:
                    current_key_state = KEY_ENTER_PRESSED;
                    break;
                default:
                    break;
            }
        }
    }
    
    return 0; // 没有关闭事件
}

// 获取当前按键状态
lcd_key_state_t lcd_get_key_state(void) {
    return current_key_state;
}

void lcd_deinit(void) {
    // 释放资源
    if (pixel_buffer) {
        free(pixel_buffer);
        pixel_buffer = NULL;
    }
    
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    
    SDL_Quit();
} 