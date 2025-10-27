#include "printf_uart.h"
#include "main.h"
#include "stm32_def.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1;

// 初始化UART打印接口
void printf_uart_init(void)
{
    // 已经在main.c中初始化了UART
}

// 通过DMA发送数据
int printf_uart_write(uint8_t *data, uint16_t len)
{
    HAL_UART_Transmit(&huart1, data, len, 0xFFFF);
    return len;
}

// 重定向printf函数到UART
#ifdef __GNUC__
// 实现fputc
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