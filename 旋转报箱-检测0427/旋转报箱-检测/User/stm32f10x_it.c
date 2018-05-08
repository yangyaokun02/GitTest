/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x_it.h"
#include  "myUsart.h"
//#include  "UpData.h"
//#include  "DownData.h"
#include   "BYJ.h"

extern u8  mainRunTime;  //��������������ms

//������
#define macLED1_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define macLED1_GPIO_PIN		  GPIO_Pin_8	
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BRR=i;}				//����͵�ƽ
#define digitalToggle(p,i)	{p->ODR ^=i;}			//�����ת״̬
/* �������IO�ĺ� */
#define LED1_TOGGLE		 digitalToggle(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_ON			   digitalLo(macLED1_GPIO_PORT,macLED1_GPIO_PIN)

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
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
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/





/*
*   ��ɺͰ�׿�˵�ͨѶ
* 
 */ 

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE)!= RESET)
	{
		//LED1_OFF;
		usart2_Up.overtime = 1;
		usart2_Up.data[usart2_Up.count++] = USART_ReceiveData(USART2);
		
		if(usart2_Up.count > dataLength-2)
		{
			usart2_Up.overtime = 0;
			usart2_Up.count = 0;
		}	
	}
}



/*
*   ��ɺ��������ͨѶ
*
*
**/

void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //������ռĴ�������ʾ���յ�����
	{
			usart3_Down.overtime = 1;
			usart3_Down.data[usart3_Down.count++] = USART_ReceiveData(USART3);
		
			if(usart3_Down.count > dataLength-2)
			{
				usart3_Down.overtime =0;
				usart3_Down.count =0;
			}
	}	
}


void UART4_IRQHandler(void)
{
  if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)    //������ռĴ�������ʾ���յ�����
	{
		uart4_Door.overtime =1;
		uart4_Door.data[uart4_Door.count++] = USART_ReceiveData(UART4);
		
		if(uart4_Door.count > dataLength-2)
		{
			uart4_Door.overtime =0;
			uart4_Door.count =0;
		}
	}	
}



void TIM2_IRQHandler(void)  //1ms
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET )
	{
			TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 
/***************************USART**************************************/
			if(usart2_Up.overtime )
			{
					if(usart2_Up.overtime++ > 5)
					{
							usart2_Up.overtime =0;
							usart2_Up.flag   = 1;	
						//���ٽ������ݣ�֪�����������ˣ��ٿ���
						  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
					}
			}
		
			if(usart3_Down.overtime)
			{
					if(usart3_Down.overtime++ > 5)
					{
							usart3_Down.overtime =0;
							usart3_Down.flag = 1;
						//���ٽ�������
						  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
					}
			}
/****************************uart4***************************************/
			if(uart4_Door.overtime)
			{
					if(uart4_Door.overtime++ > 5)
					{
						uart4_Door.overtime =0;
						//uart4_Door.flag =1;
						uart4_Door.count =0;
					}
			}
			mainRunTime++; 			
			if(CloseFlag)
			{
				CloseFlag++;
			}
	}
}


//u8 RTC_Get(void);
//
void TimEnale(u8 flag, unsigned short int Period);
void TIM3_IRQHandler(void)  
{
  if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) //�ж��Ƿ�ʱ�ж� 
  { 
			TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);    //��������жϱ�־λ����һֱ�ж�
			//TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE); 			
			TIM_Cmd(TIM3, DISABLE);	
			TIM_Cmd(TIM4, DISABLE); 
			LED1_TOGGLE;		
			//EXTI->RTSR &= ~0X00002;	//��ֹ 		
			addMoveByj();    
  }
}


//��λλ��
void EXTI2_IRQHandler (void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		EXTI_ClearITPendingBit(EXTI_Line2);     //����жϱ�־λ
		EXTI->RTSR &= ~0X00004;	//��ֹ 		
		checkNum =sMaiChong +((TIM3->CNT)/zhouqi)+15625-800;
	}  
}



/// IO ���жϣ��жϿ�ΪPC13
void EXTI1_IRQHandler (void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		EXTI_ClearITPendingBit(EXTI_Line1);     //����жϱ�־λ
	}  
}








/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
