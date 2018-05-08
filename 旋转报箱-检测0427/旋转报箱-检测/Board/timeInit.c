
#include "timeInit.h"

//void NVIC_Configuration(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//		#ifdef  VECT_TAB_RAM  
//	  /* Set the Vector Table base location at 0x20000000 */ 
//	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
//	#else  /* VECT_TAB_FLASH  */
//	  /* Set the Vector Table base location at 0x08000000 */ 
//	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
//	#endif 
//		/* Enable the TIM4 gloabal Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//		/* Enable the TIM3 gloabal Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;  
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;  
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//// NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			
//// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
//// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           
//// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             	
//// NVIC_Init(&NVIC_InitStructure);    
//	
//	
//  /* Enable the USART3 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//}


 /*******************************************************************************
* ��������    �� TIM2_Configuration
* ��������    �� ʹ�ö�ʱ��Time2����ʱ 1mS
* ����        �� 
* ����ֵ      ��  ��
* ȫ�ֱ���    ��  ��
* ȫ�־�̬������  ��
* �ֲ���̬������  ��
*******************************************************************************/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);  		//Timer2 ʱ��ʹ�� 
	
	TIM_DeInit(TIM2);	 //��λ��ʱ��								
	TIM_TimeBaseStructure.TIM_Period=1000-1;	 //��ʱ����ʼֵ���Զ���װ�ؼĴ������ڵ�ֵ(����ֵ).
	TIM_TimeBaseStructure.TIM_Prescaler=72-1;  //ʱ��Ԥ��Ƶ����1���ж����һ�Σ�ʱ��Ƶ��=72MHZ/(ʱ��Ԥ��Ƶ+1)  0.001
	//TIM_TimeBaseStructure.TIM_Prescaler=360-1;//ʱ��Ԥ��Ƶ����1���ж����һ�Σ�ʱ��Ƶ��=36MHZ/(ʱ��Ԥ��Ƶ+1) 0.005
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //������Ƶ0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//��ʼ����ʱ��3
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);   //���жϱ�־����ֹ���ж�˲����������
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//���ж�,����ʱ������ж� 
	TIM_Cmd(TIM2, ENABLE);
}

// period ���� �Ͷ���us
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure1;
void TIM3_Configuration(void)
{
	//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);  		//Timer2 ʱ��ʹ�� 
	TIM_DeInit(TIM3);	 //��λ��ʱ��								
	TIM_TimeBaseStructure1.TIM_Period=4000-1;	 //��ʱ����ʼֵ���Զ���װ�ؼĴ������ڵ�ֵ(����ֵ).
	TIM_TimeBaseStructure1.TIM_Prescaler=72-1;                 //ʱ��Ԥ��Ƶ����1���ж����һ�Σ�ʱ��Ƶ��=36MHZ/(ʱ��Ԥ��Ƶ+1)
	TIM_TimeBaseStructure1.TIM_ClockDivision=TIM_CKD_DIV1;     //������Ƶ0
	TIM_TimeBaseStructure1.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure1);	          //��ʼ����ʱ��3
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);                     //���жϱ�־����ֹ���ж�˲����������
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                  //���ж�,����ʱ������ж� 
	TIM_Cmd(TIM3, DISABLE);
}
//TIM2->CNT

void TimEnale(u8 flag, unsigned short int Period)
{
		if(flag)
		{
			TIM_ClearFlag(TIM3, TIM_FLAG_Update); 
			TIM_Cmd(TIM3, DISABLE);	 
			
			TIM_TimeBaseStructure1.TIM_Period=Period-1;	                     
			TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure1);	
			TIM_Cmd(TIM3, ENABLE);
			//TIM3->CNT =0;
		}
		else
		{
			TIM_ClearFlag(TIM3, TIM_FLAG_Update); 
			TIM_Cmd(TIM3, DISABLE);	
		}
}









