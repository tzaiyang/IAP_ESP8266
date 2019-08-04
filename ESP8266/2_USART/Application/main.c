#include "action_sys.h"
#include "action_gpio.h"
#include "action_uart.h"
#include <stdio.h>


int main(void)
{
	int res=0;
	Device_Init();
	
	USART1_Init(115200);
	USART3_Init(115200);
	while(1)
	{
		if(res<50)res++;
		else 
			res=0;
		USART_printf(USART3,"s=%d\r\n",res);
	}

}
