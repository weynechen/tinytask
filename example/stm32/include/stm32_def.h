#ifndef __STM32_DEF_H__
#define __STM32_DEF_H__

#include "stm32f1xx_hal.h"

// 定义SysTick_IRQn
#ifndef SysTick_IRQn
#define SysTick_IRQn SysTick_IRQn
#endif

// 定义GPIO相关宏
#ifndef GPIOC
#define GPIOC GPIOC
#endif

#ifndef GPIOA
#define GPIOA GPIOA
#endif

// 定义RCC相关宏
#ifndef RCC_CR_HSION
#define RCC_CR_HSION RCC_CR_HSION
#endif

#ifndef RCC_CFGR_SW_HSI
#define RCC_CFGR_SW_HSI RCC_CFGR_SW_HSI
#endif

#ifndef RCC_CFGR_HPRE_DIV1
#define RCC_CFGR_HPRE_DIV1 RCC_CFGR_HPRE_DIV1
#endif

#ifndef RCC_CFGR_PPRE1_DIV1
#define RCC_CFGR_PPRE1_DIV1 RCC_CFGR_PPRE1_DIV1
#endif

// 定义USART相关宏
#ifndef USART1
#define USART1 USART1
#endif

#ifndef USART_CR1_TE
#define USART_CR1_TE USART_CR1_TE
#endif

// 定义DMA相关宏
#ifndef DMA1_Channel4_IRQn
#define DMA1_Channel4_IRQn DMA1_Channel4_IRQn
#endif

// 定义GPIO相关宏
#ifndef GPIO_CRL_MODE0_1
#define GPIO_CRL_MODE0_1 GPIO_CRL_MODE0_1
#endif

// 定义IO寄存器类型
#ifndef __IO
#define __IO volatile
#endif

#endif // __STM32_DEF_H__ 