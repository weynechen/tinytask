#ifndef __STM32_DEF_H__
#define __STM32_DEF_H__

#include "stm32f1xx_hal.h"

// Define SysTick_IRQn
#ifndef SysTick_IRQn
#define SysTick_IRQn SysTick_IRQn
#endif

#ifndef GPIOC
#define GPIOC GPIOC
#endif

#ifndef GPIOA
#define GPIOA GPIOA
#endif

// Define RCC-related macros
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

// Define USART-related macros
#ifndef USART1
#define USART1 USART1
#endif

#ifndef USART_CR1_TE
#define USART_CR1_TE USART_CR1_TE
#endif

// Define DMA-related macros
#ifndef DMA1_Channel4_IRQn
#define DMA1_Channel4_IRQn DMA1_Channel4_IRQn
#endif

#ifndef GPIO_CRL_MODE0_1
#define GPIO_CRL_MODE0_1 GPIO_CRL_MODE0_1
#endif

// Define IO register type
#ifndef __IO
#define __IO volatile
#endif

#endif // __STM32_DEF_H__ 
