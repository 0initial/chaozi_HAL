/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComIT/Src/stm32f1xx_it.c 
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"

#include "key.h"
#include "uart.h"
#include "time.h"
   
/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_TwoBoards_ComIT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declared in "main.c" file */
extern UART_HandleTypeDef UartHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/* ---------------------串口--------------------- */
void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uart1.uart);
	
	// 判断空闲中断
	if(__HAL_UART_GET_FLAG(&uart1.uart, UART_FLAG_IDLE))
	{
		// 先清除标志位
		__HAL_UART_CLEAR_IDLEFLAG(&uart1.uart);
		
		// 在终止接收函数中，会把RxCounter清零，所以要先累加上
		uart1.RxCounter += (U1_RX_MAX - __HAL_DMA_GET_COUNTER(&uart1.dmarx));
		// 因为不会触发接收完成，所以要手动完成接收终止
		HAL_UART_AbortReceive_IT(&uart1.uart);
	}
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uart2.uart);
	
	// 判断空闲中断
	if(__HAL_UART_GET_FLAG(&uart2.uart, UART_FLAG_IDLE))
	{
		// 先清除标志位
		__HAL_UART_CLEAR_IDLEFLAG(&uart2.uart);
		
		// 在终止接收函数中，会把RxCounter清零，所以要先累加上
		uart2.RxCounter += (U2_RX_MAX - uart2.uart.RxXferCount);
		// 因为不会触发接收完成，所以要手动完成接收终止
		HAL_UART_AbortReceive_IT(&uart2.uart);
	}
}

void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uart3.uart);
	
	// 判断空闲中断
	if(__HAL_UART_GET_FLAG(&uart3.uart, UART_FLAG_IDLE))
	{
		// 先清除标志位
		__HAL_UART_CLEAR_IDLEFLAG(&uart3.uart);
		
		// 在终止接收函数中，会把RxCounter清零，所以要先累加上
		uart3.RxCounter += (U3_RX_MAX - uart3.uart.RxXferCount);
		// 因为不会触发接收完成，所以要手动完成接收终止
		HAL_UART_AbortReceive_IT(&uart3.uart);
	}
}

/* ---------------------按键--------------------- */
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(KEY1_PIN);
}

void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(KEY2_PIN);
}

/* ---------------------定时器--------------------- */
void TIM1_UP_TIM10_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim1);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim1);
}

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
