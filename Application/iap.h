#ifndef _IAP_H_
#define _IAP_H_

#include <stdint.h>

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.   
#define FLASH_APP1_ADDR		0x08004000  	//第一个应用程序起始地址(存放在FLASH)
											//保留0X08000000~0X08007FFF的空间为Bootloader使用(共32KB)	   
void iap_load_app(uint32_t appxaddr);			//跳转到APP程序执行
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t applen);	//在指定地址开始,写入bin
void MSR_MSP(uint32_t addr);
#endif

