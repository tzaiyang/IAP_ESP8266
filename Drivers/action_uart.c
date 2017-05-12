#include "action_uart.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/**
  \brief USART1初始化
  \details 
  \param  bound：波特率

 */
void USART1_Init(uint32_t bound)
{
		float 	 USARTDIV=0.0f;
		uint16_t USART_BRR=0;
		uint16_t DIV_Mantissa=0;
		uint16_t DIV_Fraction=0;
		GPIO_TypeDef *GPIOx;
		USART_TypeDef *USARTx;
	
	  RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN; 
	  RCC->APB2ENR |=RCC_APB2ENR_USART1EN; 

		GPIOx=GPIOA;
		GPIOx->MODER |= (GPIO_MODER_MODER9_1| GPIO_MODER_MODER10_1);//as general purpose alternate fuction mode (pull-push)
		GPIOx->OTYPER|= 0; // No need to change - use pull-push
		GPIOx->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9_1| GPIO_OSPEEDER_OSPEEDR10);//USE_50MHZ_OUTPUT
		GPIOx->PUPDR |=(GPIO_PUPDR_PUPD9_0|GPIO_PUPDR_PUPD10_0); //  pull up
    GPIOx->AFR[1]|=((0x07<<GPIO_AFRH_AFSEL9_Pos )|(0x07<<GPIO_AFRH_AFSEL10_Pos ));

		USARTDIV=(float)(84000000/(bound*16));//PCLK2=84M
		DIV_Mantissa=(int16_t)USARTDIV;
		DIV_Fraction=(USARTDIV-DIV_Mantissa)*16;
	  USART_BRR=(DIV_Mantissa<<4)+DIV_Fraction;
	
		USARTx=USART1;
		USARTx->BRR=USART_BRR;
	  USARTx->CR1&=~USART_CR1_OVER8; 
		USARTx->CR1|=(USART_CR1_TE|USART_CR1_RE|USART_CR1_RXNEIE); 
		NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 1));
		NVIC_EnableIRQ(USART1_IRQn);
		USARTx->CR1|=USART_CR1_UE;//Enable USART1
}

/**
  \brief USART3初始化
  \details 
  \param  bound：波特率

 */
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

/**
  \brief UART5初始化
  \details 
  \param  bound：波特率

 */
void UART5_Init(uint32_t bound)
{
		float 	 USARTDIV=0.0f;
		uint16_t USART_BRR=0;
		uint16_t DIV_Mantissa=0;
		uint16_t DIV_Fraction=0;
		GPIO_TypeDef *GPIOx;
		USART_TypeDef *USARTx;
	
	  RCC->AHB1ENR |=(RCC_AHB1ENR_GPIOCEN|RCC_AHB1ENR_GPIODEN); 
	  RCC->APB1ENR |= RCC_APB1ENR_UART5EN; 

		GPIOx=GPIOC;
		GPIOx->MODER |= (GPIO_MODER_MODER12_1);//as general purpose alternate fuction mode (pull-push)
		GPIOx->OTYPER|= 0; // No need to change - use pull-push
		GPIOx->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR12_1);//USE_50MHZ_OUTPUT
		GPIOx->PUPDR |=(GPIO_PUPDR_PUPD12_0); //  pull up
    GPIOx->AFR[1]|=((0x08<<GPIO_AFRH_AFSEL12_Pos ));
	
		GPIOx=GPIOD;
		GPIOx->MODER |= (GPIO_MODER_MODER2_1);//as general purpose alternate fuction mode (pull-push)
		GPIOx->OTYPER|= 0; // No need to change - use pull-push
		GPIOx->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR2_1);//USE_50MHZ_OUTPUT
		GPIOx->PUPDR |=(GPIO_PUPDR_PUPD2_0); //  pull up
    GPIOx->AFR[1]|=((0x08<<GPIO_AFRL_AFSEL2_Pos ));

		USARTDIV=(float)(42000000/(bound*16));//PCLK1=42M
		DIV_Mantissa=(int16_t)USARTDIV;
		DIV_Fraction=(USARTDIV-DIV_Mantissa)*16;
	  USART_BRR=(DIV_Mantissa<<4)+DIV_Fraction;
	
		USARTx=UART5;
		USARTx->BRR=USART_BRR;
	  USARTx->CR1&=~USART_CR1_OVER8; 
		USARTx->CR1|=(USART_CR1_TE|USART_CR1_RE|USART_CR1_RXNEIE); 
		NVIC_SetPriority(UART5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 1));
		NVIC_EnableIRQ(UART5_IRQn);
		USARTx->CR1|=USART_CR1_UE;//Enable UART5
}

/**
  \brief USART1中断
  \details 

 */    
uint8_t USART_RX_BUF_BIN[RXBUFFERSIZE_BIN] __attribute__ ((at(0X20001000)));    
uint32_t USART_RX_CNT=0;

void USART1_IRQHandler(void)
{
	
	if(USART1->SR&USART_SR_RXNE)
	{
		
	}
}

/**
  \brief USART3中断
  \details 

 */ 
void USART3_IRQHandler(void)
{
		uint8_t RxBuffer;
	if(USART3->SR&USART_SR_RXNE)
	{
		RxBuffer=USART3->DR; 
		if(USART_RX_CNT<RXBUFFERSIZE_BIN)
		{
			USART_RX_BUF_BIN[USART_RX_CNT]=RxBuffer;
			USART_RX_CNT++;			 									     
		}
	}
}

/**
  \brief UART5中断
  \details 

 */    
void UART5_IRQHandler(void)
{
	if(UART5->SR&USART_SR_RXNE)
	{
		//update_esp8266_ack();
	}
}

/**
  \brief 格式化输出
  \details 
  \param USARTx：串口
				 fmt:格式%s,%d,%c
 */ 
void USART_printf(USART_TypeDef * USARTx,char* fmt,...)
{
	uint16_t i;
	__align(8) uint8_t USART_TX_BUF[TXBUFFERSIZE];
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART_TX_BUF,fmt,ap);
	va_end(ap);

	i=strlen((const char*)USART_TX_BUF);	
	
	for(int j=0;j<i;j++)
	{
		USARTx->DR =USART_TX_BUF[j];
		while((USART_SR_TC&USARTx->SR)==0);
	}
}


/**
  \brief 计算字符串长度
  \details 
  \param str:字符串常量

  \return  字符串长度

 */ 
int getstrlen(const char * str)
{
    const char *eos = str;
    while( *eos++ ) ;
		return( eos - str - 1 );
}
/**
  \brief 串口发送
  \details 
  \param USARTx：串口
				 str:字符串常量

	\return  结束状态：0-成功

 */ 
uint8_t USART_send(USART_TypeDef * USARTx,uint8_t * str)
{
	int temp=0;
	temp=getstrlen((const char*)str);
	for(int j=0;j<temp;j++)
	{
		USARTx->DR =*(str+j);
		while((USART_SR_TC&USARTx->SR)==0);
	}
	USARTx->DR =0x0d;
	while((USART_SR_TC&USARTx->SR)==0);
	USARTx->DR =0x0a;
	while((USART_SR_TC&USARTx->SR)==0);
		
	return 0;
}









