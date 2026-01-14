#include "printf_uart.h"
#include "main.h"
#include "stm32_def.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1;

// Initialize UART print interface
void printf_uart_init(void)
{
    // UART already initialized in main.c
}

// Send data via DMA
int printf_uart_write(uint8_t *data, uint16_t len)
{
    HAL_UART_Transmit(&huart1, data, len, 0xFFFF);
    return len;
}

// Redirect printf function to UART
#ifdef __GNUC__
// Implement fputc
int __io_putchar(int ch)
{
    uint8_t c = ch;
    HAL_UART_Transmit(&huart1, &c, 1, 0xFFFF);
    return ch;
}

int _write(int file, char *ptr, int len)
{
    UNUSED(file);
    return printf_uart_write((uint8_t*)ptr, len);
}
#endif 