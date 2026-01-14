#include "lcd_driver.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// SDL-related variables
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static uint32_t* pixel_buffer = NULL;

// Pixel color definitions - extended to support Windows-style
#define PIXEL_OFF_COLOR  0xFF000000 // Black (background)
#define PIXEL_ON_COLOR   0xFFFFFFFF // White (foreground)
#define PIXEL_GRAY_COLOR 0xFFD0D0D0 // Gray (button background)
#define PIXEL_BLUE_COLOR 0xFF4B9EFF // Light blue (selected border)

// Current key state
static lcd_key_state_t current_key_state = KEY_NONE;

// Pixel size (zoom factor, for clearer display)
#define PIXEL_SCALE 4

int lcd_init(void) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return -1;
    }

    // Create window
    window = SDL_CreateWindow(
        "LCD Simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LCD_WIDTH * PIXEL_SCALE,
        LCD_HEIGHT * PIXEL_SCALE,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Create texture
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        LCD_WIDTH,
        LCD_HEIGHT
    );

    if (!texture) {
        fprintf(stderr, "Texture creation failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Create pixel buffer
    pixel_buffer = (uint32_t*)malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint32_t));
    if (!pixel_buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Clear pixel buffer (initialize to black)
    memset(pixel_buffer, 0, LCD_WIDTH * LCD_HEIGHT * sizeof(uint32_t));

    return 0;
}

void lcd_clear(void) {
    if (pixel_buffer) {
        memset(pixel_buffer, 0, LCD_WIDTH * LCD_HEIGHT * sizeof(uint32_t));
    }
}

/**
 * @brief Draw a single pixel on the LCD
 * 
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Pixel color (32-bit ARGB format)
 */
void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    // Check if coordinates are within valid range
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT || !pixel_buffer) {
        return;
    }

    // Directly set pixel color
    pixel_buffer[y * LCD_WIDTH + x] = color;
}

void lcd_update(void) {
    if (!renderer || !texture || !pixel_buffer) {
        return;
    }

    // Update texture data
    SDL_UpdateTexture(texture, NULL, pixel_buffer, LCD_WIDTH * sizeof(uint32_t));

    // Clear renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render texture
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    
    // Display rendered content
    SDL_RenderPresent(renderer);
}

int lcd_get_event(void) {
    SDL_Event event;
    
    // Reset current key state
    current_key_state = KEY_NONE;
    
    // Process all pending events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 1; // Window close event
        }
        
        // Detect keyboard events
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
    
    return 0; // No close event
}

// Get current key state
lcd_key_state_t lcd_get_key_state(void) {
    return current_key_state;
}

void lcd_deinit(void) {
    // Release resources
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