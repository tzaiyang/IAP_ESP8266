#include "action_sys.h"
#include "action_uart.h"
#include "action_gpio.h"
#include "iap.h"

int main(void)
{
	uint16_t oldcount=0;	//老的串口接收数据值
	uint32_t applenth=0;	//接收到的app代码长度
	
	Device_Init();
	Beep_Init();
	KEY_Init();
	//USART1_Init(115200);
	USART3_Init(115200);
	uint8_t download_flag=0;
	
	while(1)
	{
		if(!READ_KEY0||download_flag){
			if(download_flag<100)
			{
				download_flag++;
				GPIOF->ODR = GPIO_BSRR_BS8;
			}				
			else GPIOF->ODR = GPIO_BSRR_BR8;
			if(USART_RX_CNT)
			{
				if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
				{
					applenth=USART_RX_CNT;
					oldcount=0;
					USART_RX_CNT=0;
					USART_printf(USART3,"APP received completely\r\n");
					USART_printf(USART3,"Code:%dBytes\r\n",applenth);
					
					if(applenth)
				{
					USART_printf(USART3,"开始更新固件...\r\n");	
					if(((*(__IO uint32_t *)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
					{	 
						iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF_BIN,applenth);//更新FLASH代码   
						USART_printf(USART3,"固件更新完成!\r\n");	
					}
					else USART_printf(USART3,"非FLASH应用程序!\r\n");
				}
				else 
				{
					USART_printf(USART3,"没有可以更新的固件!\r\n");
					
				}
				
				USART_printf(USART3,"开始执行FLASH用户代码!!\r\n");
				Delay_Ms(500);
				if(((*(__IO uint32_t *)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000){//判断是否为0X08XXXXXX.	
							GPIOF->ODR = GPIO_BSRR_BR8;		
						  iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
				}
				else USART_printf(USART3,"非FLASH应用程序,无法执行!\r\n");							
				
				
				}
				else oldcount=USART_RX_CNT;			
			}
		 }
	else if(!download_flag){
		GPIOF->ODR = GPIO_BSRR_BR8;		
		iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
	}
	Delay_Ms(10);	
	}

}
