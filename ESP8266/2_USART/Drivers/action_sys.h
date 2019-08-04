#ifndef _ACTION_SYS_H_
#define _ACTION_SYS_H_
#include <stdint.h>
#include "stm32f407xx.h"

/** @defgroup CORTEX_Preemption_Priority_Group CORTEX Preemption Priority Group
  * @{
  */
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007U) /*!< 0 bits for pre-emption priority
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006U) /*!< 1 bits for pre-emption priority
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005U) /*!< 2 bits for pre-emption priority
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004U) /*!< 3 bits for pre-emption priority
                                                                 1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003U) /*!< 4 bits for pre-emption priority
																																 0 bits for subpriority */
void Device_Init(void);
void SysTick_Init(void);
uint8_t SystemClock_Config(void);
void NVIC_InitPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);

void Delay_Ms(__IO uint32_t Delay);
void IncTick(void);
uint32_t GetTick(void);


#endif
