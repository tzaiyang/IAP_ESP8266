#include "action_gpio.h"

void LED_Init(void)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;// Enable Port F clock
	/***********LED*******************/
	GPIOF->MODER |= (GPIO_MODER_MODER9_0| GPIO_MODER_MODER10_0);//Set pin 8 to 9 as general purpose output mode (pull-push)
	GPIOF->OTYPER |= 0; // No need to change - use pull-push
	GPIOF->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9| GPIO_OSPEEDER_OSPEEDR10);//USE_100MHZ_OUTPUT
	GPIOF->PUPDR |= 0; // No pull up , no pull down
	GPIOF->BSRR |= (GPIO_BSRR_BS9 |GPIO_BSRR_BR10); // ReSet bit 12
}

void Beep_Init(void)
{
	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOFEN;
	/**********BEEP*******************/
	GPIOF->MODER |= (GPIO_MODER_MODER8_0);//Set pin 8 to 9 as general purpose output mode (pull-push)
	GPIOF->OTYPER |= 0; // No need to change - use pull-push
	GPIOF->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 );//USE_100MHZ_OUTPUT
	GPIOF->PUPDR |= GPIO_PUPDR_PUPD8_1; // No pull up , no pull down
	GPIOF->BSRR |= GPIO_BSRR_BR8; // Set bit 12
}

void KEY_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;// Enable Port E clock
	/***********KEY*******************/
	GPIOE->MODER &= ~(GPIO_MODER_MODER4);
	GPIOE->PUPDR |=(GPIO_PUPDR_PUPD4_0); 
}

