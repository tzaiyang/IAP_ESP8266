#include "action_flash.h"
#include "stm32f407xx.h"
#include "action_sys.h"


//解锁STM32的FLASH
void STMFLASH_Unlock(void)
{
	FLASH->KEYR=FLASH_KEY1;	//写入解锁序列.
	FLASH->KEYR=FLASH_KEY2; 
}
//flash上锁
void STMFLASH_Lock(void)
{
	FLASH->CR|=(uint32_t)1<<31;//上锁
}
//得到FLASH状态
//返回值:
//0,操作完成
//1,忙 
//2,操作异常 
uint8_t STMFLASH_GetStatus(void)
{	
	 uint32_t res=0;		
	res=FLASH->SR;  
	if(res&(1<<16))return 1;   		//忙
	else if(res&(1<<4))return 2;	//操作异常 
	else if(res&(1<<5))return 2;	//操作异常 
	else if(res&(1<<6))return 2;	//操作异常 
	else if(res&(1<<7))return 2;	//操作异常 
	return 0;						//没有任何状态/操作完成.
} 
//等待操作完成
//time:要延时的长短(单位:10us)
//返回值:
//0,完成
//2,操作异常
//0XFF,超时       
uint8_t STMFLASH_WaitDone(uint32_t time)
{
	uint8_t res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//非忙,无需等待了,直接退出.
		Delay_Ms(1);
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//擦除扇区
//sectoraddr:扇区地址,范围是:0~11.
//0~3,16K扇区;4,64K扇区;5~11,128K扇区.
//返回值:执行情况
uint8_t STMFLASH_EraseSector(uint32_t sectoraddr)
{
	uint8_t res=0;
	res=STMFLASH_WaitDone(200000);//等待上次操作结束,最大2s    
	if(res==0)
	{ 
		FLASH->CR&=~(3<<8);	//清除PSIZE原来的设置
		FLASH->CR|=2<<8;	//设置为32bit宽,确保VCC=2.7~3.6V之间!!
		FLASH->CR&=~(0X1F<<3);//清除原来的设置
		FLASH->CR|=sectoraddr<<3;//设置要擦除的扇区 
		FLASH->CR|=1<<1;	//扇区擦除 
		FLASH->CR|=1<<16;	//开始擦除		  
		res=STMFLASH_WaitDone(200000);//等待操作结束,最大2s  
		if(res!=1)			//非忙
		{
			FLASH->CR&=~(1<<1);//清除扇区擦除标志.
		}
	}
	return res;
}
//在FLASH指定地址写一个字
//faddr:指定地址(此地址必须为4的倍数!!)
//dat:要写入的数据
//返回值:0,写入成功
//    其他,写入失败
uint8_t STMFLASH_WriteWord(uint32_t faddr, uint32_t dat)
{
	uint8_t res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR&=~(3<<8);	//清除PSIZE原来的设置
		FLASH->CR|=2<<8;	//设置为32bit宽,确保VCC=2.7~3.6V之间!!
 		FLASH->CR|=1<<0;	//编程使能
		*(__IO uint32_t *)faddr=dat;	//写入数据
		res=STMFLASH_WaitDone(0XFF);//等待操作完成,一个字编程,最多100us.
		if(res!=1)//操作成功
		{
			FLASH->CR&=~(1<<0);//清除PG位.
		}
	} 
	return res;
} 
//读取指定地址的一个字(32位数据) 
//faddr:读地址 
//返回值:对应数据.
uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
	return *(__IO uint32_t *)faddr; 
}  
//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
uint8_t STMFLASH_GetFlashSector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return 0;
	else if(addr<ADDR_FLASH_SECTOR_2)return 1;
	else if(addr<ADDR_FLASH_SECTOR_3)return 2;
	else if(addr<ADDR_FLASH_SECTOR_4)return 3;
	else if(addr<ADDR_FLASH_SECTOR_5)return 4;
	else if(addr<ADDR_FLASH_SECTOR_6)return 5;
	else if(addr<ADDR_FLASH_SECTOR_7)return 6;
	else if(addr<ADDR_FLASH_SECTOR_8)return 7;
	else if(addr<ADDR_FLASH_SECTOR_9)return 8;
	else if(addr<ADDR_FLASH_SECTOR_10)return 9;
	else if(addr<ADDR_FLASH_SECTOR_11)return 10; 
	return 11;	
}
//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F(注意：最后16字节，用于OTP数据块锁定，别乱写！！)
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	
{ 
	uint8_t status=0;
	uint32_t addrx=0;
	uint32_t endaddr=0;	
  	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
	STMFLASH_Unlock();									//解锁 
 	FLASH->ACR&=~(1<<10);			//FLASH擦除期间,必须禁止数据缓存!!!搞了我两晚上才发现这个问题!
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
	if(addrx<0X1FFF0000)			//只有主存储区,才需要执行擦除操作!!
	{
		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
				status=STMFLASH_EraseSector(STMFLASH_GetFlashSector(addrx));
				if(status)break;	//发生错误了
			}else addrx+=4;
		} 
	}
	if(status==0)
	{
		while(WriteAddr<endaddr)//写数据
		{
			if(STMFLASH_WriteWord(WriteAddr,*pBuffer))//写入数据
			{ 
				break;	//写入异常
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
	FLASH->ACR|=1<<10;		//FLASH擦除结束,开启数据fetch
	STMFLASH_Lock();//上锁
} 

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32位)数
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}

//////////////////////////////////////////测试用///////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(uint32_t WriteAddr,uint32_t WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}


