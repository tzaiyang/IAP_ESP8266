#include "action_sys.h"
#include "stm32f407xx.h"

/**
  * @brief  This function is used to initialize the Device; it must be the first
  *         instruction to be executed in the main program (before to call any other
  *         action drives function), it performs the following:
  *           Configure the Flash prefetch, instruction and Data caches.
  *           Set NVIC Group Priority to 2.
  *           which is clocked by the HSE
	*						Update SystemCoreClock Value
  *           Configures the SysTick to generate an interrupt each 1 millisecond
  *
  * @note   SysTick is used as time base for the Delay_Ms() function, the application
  *         need to ensure that the SysTick time base is always set to 1 millisecond
  *         to have correct HAL operation.
  * @retval void
  */
void Device_Init(void)
{
/* Configure Flash prefetch, Instruction cache, Data cache */
	 FLASH->ACR |= 0x1U << 8U;/* PREFETCH_ENABLE */
   FLASH->ACR |= 0x1U << 9U;/* INSTRUCTION_CACHE_ENABLE */
   FLASH->ACR |= 0x1U <<10U;/* DATA_CACHE_ENABLE */
	 FLASH->ACR |= 5<<0;			/* represent the ratio of the CPU clock period to the Flash memory access time*/

  /* Set Interrupt Group Priority */
	 NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	/*Set System Clock*/
	 SystemClock_Config();
	/*Update SystemCoreClock Value*/
	 SystemCoreClockUpdate();
	/* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
	 SysTick_Init();

}

/**
  * @brief  This function is used to set the clock;
  *
  * @retval status
  */
uint8_t SystemClock_Config(void)
{
	uint32_t pllm=8;
	uint32_t plln=336;
	uint32_t pllp=2;
	uint32_t pllq=7;

	uint16_t retry=0;
	uint8_t status=0;

	RCC->CR&=~RCC_CR_HSEBYP;
	RCC->CIR=0x00000000;
	RCC->CR|=RCC_CR_HSEON;
	while(((RCC->CR&RCC_CR_HSERDY)==0)&&(retry<0X1FFF))retry++;
	if(retry==0X1FFF)status=1;
	else
	{
		RCC->APB1ENR|=RCC_APB1ENR_PWREN;
		PWR->CR|=PWR_CR_VOS; 			/*Scale 1 mode (default value at reset)*/
		RCC->CFGR|=RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV4|RCC_CFGR_PPRE2_2;/*system clock not divided;AHB clock divided by 4;AHB clock divided by 2*/
		RCC->CR&=~RCC_CR_PLLON;
		RCC->PLLCFGR=pllm|(plln<<RCC_PLLCFGR_PLLN_Pos)|(((pllp>>1)-1)<<RCC_PLLCFGR_PLLP_Pos)|(pllq<<RCC_PLLCFGR_PLLQ_Pos)|RCC_PLLCFGR_PLLSRC_HSE;
		RCC->CR|=RCC_CR_PLLON;
		while((RCC->CR&RCC_CR_PLLRDY)==0);

		RCC->CFGR&=~RCC_CFGR_SW;		  /*HSI oscillator selected as system clock*/
	  RCC->CFGR|=RCC_CFGR_SW_1;			/*PLL selected as system clock*/
	  while((RCC->CFGR&RCC_CFGR_SWS)!=RCC_CFGR_SWS_1);//System clock switch status,waitting for setting succeed */
	}
	return status;
}

/**
  * @brief  This function is used to Configures the SysTick to generate an interrupt each 1 millisecond;
  *
  * @retval void
  */
void SysTick_Init(void)
{
	SysTick->LOAD  = (uint32_t)(SystemCoreClock/1000U - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
	NVIC_InitPriority(SysTick_IRQn, 0U, 0U);
	NVIC_EnableIRQ(SysTick_IRQn);
}


/**
  * @brief This function provides accurate delay (in milliseconds) based
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @param Delay: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay_Ms(__IO uint32_t Delay)
{
	uint32_t tickstart = 0U;
  tickstart = GetTick();
  while((GetTick() - tickstart) < Delay)
  {
  }
}


__IO uint32_t uwTick;
/**
  * @brief This function is called to increment  a global variable "uwTick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in Systick ISR.
  * @retval None
  */
void IncTick(void)
{
  uwTick++;
}
/**
  * @brief Provides a tick value in millisecond.
  * @retval tick value
  */
uint32_t GetTick(void)
{
	return uwTick;
}

void SysTick_Handler(void)
{
	 IncTick();
}
/**
  * @brief  Sets the priority of an interrupt.
  * @param  IRQn: External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32f4xxxx.h))
  * @param  PreemptPriority: The preemption priority for the IRQn channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority
  * @param  SubPriority: the subpriority level for the IRQ channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority.
  * @retval None
  */
void NVIC_InitPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t prioritygroup = 0x00U;

  prioritygroup = NVIC_GetPriorityGrouping();

  NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));
}
