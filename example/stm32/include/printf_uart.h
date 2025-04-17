#ifndef __PRINTF_UART_H__
#define __PRINTF_UART_H__

#include <stdint.h>

void printf_uart_init(void);
int printf_uart_write(uint8_t *data, uint16_t len);

#endif // __PRINTF_UART_H__ 