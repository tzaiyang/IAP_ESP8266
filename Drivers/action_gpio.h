#ifndef _ACTION_GPIO_H_
#define _ACTION_GPIO_H_
#include "stm32f407xx.h"


#define BEEP_Toggle  (GPIOF->ODR ^= GPIO_BSRR_BS8)
#define LED0_Toggle		(GPIOF->ODR ^=GPIO_BSRR_BS9)
#define LED1_Toggle		(GPIOF->ODR ^=GPIO_BSRR_BS10)

void LED_Init(void);
void Beep_Init(void);

#endif
