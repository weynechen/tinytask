#include "key.h"
#include "tt_task.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

#ifdef PLATFORM_WINDOWS
#include <conio.h>
#endif

static key_device_t *key_dev = NULL;

// Initialize key device
static int key_init(tt_device_t *dev)
{
    key_device_t *key = (key_device_t *)dev;
    key->last_state = KEY_NO_PRESS;
    key->press_time = 0;
    printf("Key device initialized successfully\n");
    return 0;
}

// Suspend key device
static int key_suspend(tt_device_t *dev)
{
    printf("Key device suspended\n");
    return 0;
}

// Resume key device
static int key_resume(tt_device_t *dev)
{
    printf("Key device resumed\n");
    return 0;
}

// Deinitialize key device
static int key_deinit(tt_device_t *dev)
{
    printf("Key device released\n");
    return 0;
}

// Register key device
void register_key_device(key_device_t *key)
{
    if (key == NULL) {
        return;
    }
    
    key->base.name = "key";
    key->base.init = key_init;
    key->base.deinit = key_deinit;
    key->base.suspend = key_suspend;
    key->base.resume = key_resume;
    key->scan = key_scan;
    
    tt_device_add(&key->base);
    key_dev = key;
    
    key->base.init(&key->base);
}

#ifdef PLATFORM_LINUX
// Non-blocking keyboard input function for Linux
static int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

static int getch(void)
{
    int ch;
    struct termios oldt, newt;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    return ch;
}
#endif

// Key scan function
uint8_t key_scan(void)
{
    uint8_t key_status = KEY_NO_PRESS;
    int key_pressed = 0;
    int key_val = 0;
    
#ifdef PLATFORM_WINDOWS
    if (_kbhit()) {
        key_pressed = 1;
        key_val = _getch();
    }
#elif defined(PLATFORM_LINUX)
    if (kbhit()) {
        key_pressed = 1;
        key_val = getch();
    }
#endif
    
    // 'k' key as button input, 's' key as short press, 'l' key as long press
    if (key_pressed) {
        if (key_val == 's') {
            // Directly map 's' key to short press
            key_dev->last_state = KEY_SHORT_PRESS;
        } else if (key_val == 'l') {
            // Directly map 'l' key to long press
            key_dev->last_state = KEY_LONG_PRESS;
        }
    } else if (key_dev != NULL && key_dev->last_state != KEY_NO_PRESS) {
        // Key released, return last state
        key_status = key_dev->last_state;
        key_dev->last_state = KEY_NO_PRESS;
    }
    
    return key_status;
} 