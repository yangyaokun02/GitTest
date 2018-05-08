
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
* 函数名称    ： TIM2_Configuration
* 功能描述    ： 使用定时器Time2，定时 1mS
* 参数        ： 
* 返回值      ：  无
* 全局变量    ：  无
* 全局静态变量：  无
* 局部静态变量：  无
*******************************************************************************/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);  		//Timer2 时钟使能 
	
	TIM_DeInit(TIM2);	 //复位定时器								
	TIM_TimeBaseStructure.TIM_Period=1000-1;	 //定时器初始值，自动重装载寄存器周期的值(计数值).
	TIM_TimeBaseStructure.TIM_Prescaler=72-1;  //时钟预分频数。1秒中断溢出一次，时钟频率=72MHZ/(时钟预分频+1)  0.001
	//TIM_TimeBaseStructure.TIM_Prescaler=360-1;//时钟预分频数。1秒中断溢出一次，时钟频率=36MHZ/(时钟预分频+1) 0.005
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//初始化定时器3
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);   //清中断标志，防止打开中断瞬间产生误操作
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//打开中断,即定时器溢出中断 
	TIM_Cmd(TIM2, ENABLE);
}

// period 多少 就多少us
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure1;
void TIM3_Configuration(void)
{
	//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);  		//Timer2 时钟使能 
	TIM_DeInit(TIM3);	 //复位定时器								
	TIM_TimeBaseStructure1.TIM_Period=4000-1;	 //定时器初始值，自动重装载寄存器周期的值(计数值).
	TIM_TimeBaseStructure1.TIM_Prescaler=72-1;                 //时钟预分频数。1秒中断溢出一次，时钟频率=36MHZ/(时钟预分频+1)
	TIM_TimeBaseStructure1.TIM_ClockDivision=TIM_CKD_DIV1;     //采样分频0
	TIM_TimeBaseStructure1.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure1);	          //初始化定时器3
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);                     //清中断标志，防止打开中断瞬间产生误操作
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                  //打开中断,即定时器溢出中断 
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









