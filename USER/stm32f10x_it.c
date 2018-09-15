/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "uart.h"
#include "timer.h"
#include "work.h"

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	unsigned char rc=0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //如果是接收中断
	{     
		rc = USART_ReceiveData(USART1);
		com1.rcv_buff[com1.rcv_len] = rc;
		com1.rcv_len++;
		if(com1.rcv_len >= RCV1_BUFF_LENGH)
			com1.rcv_len = 0;

		com1.jg_time = 0;
		com1.rcv_flag = 1;

	}
}


/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	unsigned char rc=0;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //如果是接收中断
	{     
		rc = USART_ReceiveData(USART2);
		com2.rcv_buff[com2.rcv_len] = rc;
		com2.rcv_len++;
		if(com2.rcv_len >= RCV2_BUFF_LENGH)
			com2.rcv_len = 0;

		com2.jg_time = 0;
		com2.rcv_flag = 1;
	}
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
	unsigned char rc=0;//,rcn=0;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //如果是接收中断
	{     
		rc = USART_ReceiveData(USART3);
		com3.rcv_buff[com3.rcv_len] = rc;
		com3.rcv_len++;
		if(com3.rcv_len >= RCV3_BUFF_LENGH)
			com3.rcv_len = 0;

		com3.jg_time = 0;
		com3.rcv_flag = 1;
	}
}

/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		com1.jg_time++;
		com2.jg_time++;
		com3.jg_time++;

		sendNetConTimeCs++;
		testTimeCs++;

	}    
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
}


/*******************************************************************************
* Function Name  : DMA1_Channel4_IRQHandler
* Description    : This function handles DMA1 Channel 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
//	if(DMA_GetITStatus(DMA1_IT_TC4))
//	{
//		//清除标志位  
//		DMA_ClearFlag(DMA1_FLAG_TC4);  
//		//DMA_ClearITPendingBit(DMA1_FLAG_TC4);  
//		//DMA1->IFCR |= DMA1_FLAG_TC4;  
//		//关闭DMA  
//		DMA_Cmd(DMA1_Channel4,DISABLE);  
//		//DMA1_Channel4->CCR &= ~(1<<0);  

//		//允许再次发送  
//		//Flag_Uart_Send = 0;  
//		com1.jg_time = 0;
//		clearbuff(com1.rcv_buff, 200);
//	}
}


void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
