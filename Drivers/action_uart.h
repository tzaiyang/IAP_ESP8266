#ifndef _ACTION_UART_H_
#define _ACTION_UART_H_

#include <stdint.h>
#include "stm32f407xx.h"

#define TXBUFFERSIZE 400
#define RXBUFFERSIZE_BIN 120*1024 	/*定义最大接收字节数*/

extern uint8_t 	USART_RX_BUF_BIN[RXBUFFERSIZE_BIN];
extern uint32_t USART_RX_CNT;

void USART1_Init(uint32_t bound);
void USART3_Init(uint32_t bound);
int getstrlen(const char * str);
uint8_t USART_send(USART_TypeDef * USARTx,uint8_t * str);
void USART_printf(USART_TypeDef * USARTx,char* fmt,...);
#endif


