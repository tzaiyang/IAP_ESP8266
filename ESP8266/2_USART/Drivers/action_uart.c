#include "action_uart.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "action_gpio.h"
#include "action_sys.h"
#include "action_rcc.h"

void USART1_Init(uint32_t bound)
{
		float 	 USARTDIV=0.0f;
		uint16_t USART_BRR=0;
		uint16_t DIV_Mantissa=0;
		uint16_t DIV_Fraction=0;

	  RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN; 
	  RCC->APB2ENR |=RCC_APB2ENR_USART1EN; 

		GPIOA->MODER |= (GPIO_MODER_MODER9_1| GPIO_MODER_MODER10_1);//Set pin 8 to 9 as general purpose alternate fuction mode (pull-push)
		GPIOA->OTYPER|= 0; // No need to change - use pull-push
		GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9_1| GPIO_OSPEEDER_OSPEEDR10);//USE_50MHZ_OUTPUT
		GPIOA->PUPDR |=(GPIO_PUPDR_PUPD9_0|GPIO_PUPDR_PUPD10_0); //  pull up
    GPIOA->AFR[1]|=((0x07<<GPIO_AFRH_AFSEL9_Pos )|(0x07<<GPIO_AFRH_AFSEL10_Pos ));

		USARTDIV=(float)(RCC_GetPCLK2Freq()/(bound*16));
		DIV_Mantissa=(int16_t)USARTDIV;
		DIV_Fraction=(USARTDIV-DIV_Mantissa)*16;
	  USART_BRR=(DIV_Mantissa<<4)+DIV_Fraction;
	
		USART1->BRR=USART_BRR;
	  USART1->CR1&=~USART_CR1_OVER8; 
		USART1->CR1|=(USART_CR1_TE|USART_CR1_RE|USART_CR1_RXNEIE); 
		NVIC_InitPriority(USART1_IRQn, 3U, 3U);
		NVIC_EnableIRQ(USART1_IRQn);
		USART1->CR1|=USART_CR1_UE;//Enable USART1
}


void USART3_Init(uint32_t bound)
{
		float 	 USARTDIV=0.0f;
		uint16_t USART_BRR=0;
		uint16_t DIV_Mantissa=0;
		uint16_t DIV_Fraction=0;
		GPIO_TypeDef *GPIOx;
		USART_TypeDef *USARTx;

	  RCC->AHB1ENR |=RCC_AHB1ENR_GPIOBEN; 
	  RCC->APB1ENR |=RCC_APB1ENR_USART3EN; 

		GPIOx=GPIOB;
		GPIOx->MODER |= (GPIO_MODER_MODER10_1| GPIO_MODER_MODER11_1);//as general purpose alternate fuction mode (pull-push)
		GPIOx->OTYPER|= 0; // No need to change - use pull-push
		GPIOx->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10_1| GPIO_OSPEEDER_OSPEEDR11);//USE_50MHZ_OUTPUT
		GPIOx->PUPDR |=(GPIO_PUPDR_PUPD10_0|GPIO_PUPDR_PUPD11_0); //  pull up
    GPIOx->AFR[1]|=((0x07<<GPIO_AFRH_AFSEL10_Pos )|(0x07<<GPIO_AFRH_AFSEL11_Pos ));

		USARTDIV=(float)(42000000/(bound*16));//PCLK1=42M
		DIV_Mantissa=(int16_t)USARTDIV;
		DIV_Fraction=(USARTDIV-DIV_Mantissa)*16;
	  USART_BRR=(DIV_Mantissa<<4)+DIV_Fraction;
	
		USARTx=USART3;
		USARTx->BRR=USART_BRR;
	  USARTx->CR1&=~USART_CR1_OVER8; 
		USARTx->CR1|=(USART_CR1_TE|USART_CR1_RE|USART_CR1_RXNEIE); 
		NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 1));
		NVIC_EnableIRQ(USART3_IRQn);
		USARTx->CR1|=USART_CR1_UE;//Enable USART3
}


__align(8) uint8_t USART_TX_BUF[TXBUFFERSIZE]; 	
uint8_t USART_RX_BUF[RXBUFFERSIZE] __attribute__ ((at(0X20001000)));

uint16_t USART_RX_STA=0;     
uint32_t USART_RX_CNT=0;
void USART1_IRQHandler(void)
{
	uint8_t res;	
	if(USART1->SR&USART_SR_RXNE)
	{
		res=USART1->DR; 
		if(USART_RX_CNT<RXBUFFERSIZE)
		{
			USART_RX_BUF[USART_RX_CNT]=res;
			USART_RX_CNT++;			 									     
		}
	}
}



void USART_printf(USART_TypeDef * USARTx,char* fmt,...)
{
//	uint16_t i;
//	va_list ap;
//	va_start(ap,fmt);
//	vsprintf((char*)USART_TX_BUF,fmt,ap);
//	va_end(ap);

//	i=strlen((const char*)USART_TX_BUF);

//	USARTx->CR1 |=USART_CR1_TXEIE;
//	//if(HAL_UART_Transmit_IT(huart, (uint8_t*)(USART_TX_BUF), i)!= HAL_OK)

//	while (1);
	
	
	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, fmt);

	while(*fmt!=0){	
		if(*fmt==0x5c){							
			switch (*++fmt){
				case 'r':							 
					USARTx->DR = (0x0d & (uint16_t)0x01FF);
					fmt++;
					break;
				
				case 'n':			
					USARTx->DR = (0x0a & (uint16_t)0x01FF);
					fmt++;
					break;

				default:
					fmt++;
				  break;
			}
			
			 
		}
		else if(*fmt=='%'){			
			switch (*++fmt){				
				case 's':										 
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						USARTx->DR = (*s & (uint16_t)0x01FF);
						while((USARTx->SR&USART_SR_TC)==0);
					}
					fmt++;
					break;
        case 'd':		
					d = va_arg(ap, int);
					itoa(d, buf, 10);
					for (s = buf; *s; s++) 
					{
						USARTx->DR = (*s & (uint16_t)0x01FF);
						while((USARTx->SR&USART_SR_TC)==0);
					}
					fmt++;
					break;
				default:
					fmt++;
					break;
			}		 
		}
	else USARTx->DR = (*fmt++ & (uint16_t)0x01FF);
	while((USARTx->SR&USART_SR_TC)==0);
	}
	
}


char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }
    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;
        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
    /* Null terminate the string. */
    *ptr = 0;

    return string;
}
