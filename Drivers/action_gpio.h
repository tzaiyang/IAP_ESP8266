#ifndef _ACTION_GPIO_H_
#define _ACTION_GPIO_H_
#include "stm32f407xx.h"


#define BEEP_Toggle  	(GPIOF->ODR ^= GPIO_BSRR_BS8)
#define LED0_Toggle		(GPIOF->ODR ^= GPIO_BSRR_BS9)
#define LED1_Toggle		(GPIOF->ODR ^= GPIO_BSRR_BS10)
#define READ_KEY0	(GPIOE->IDR&GPIO_IDR_ID4)
#define READ_KEY1 (GPIOE->IDR&GPIO_IDR_ID3)
#define READ_KEY2	(GPIOE->IDR&GPIO_IDR_ID2)

void LED_Init(void);
void Beep_Init(void);
void KEY_Init(void);


#endif
