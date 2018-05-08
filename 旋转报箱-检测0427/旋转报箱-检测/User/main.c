#include  "stm32f10x.h"

#include  "systick.h"
#include  "myUsart.h"
#include  "timeInit.h"
//#include  "UpData.h"
//#include  "DownData.h"
#include  "RTC.h"
#include  "DoorCtr.h"
#include  "BYJ.h"
#include  "KDG.h"


#define ENTER_CRITICAL_SECTION() 			__set_PRIMASK(1);// 关总中断
#define EXIT_CRITICAL_SECTION()   		__set_PRIMASK(0);// 开总中断

u8  mainRunTime = 10;  //主程序运行周期ms



void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif 

    /* Configure the NVIC Preemption Priority Bits配置优先级组 */  
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //两级占先 八级副优先
	/* Enable the TIM4 gloabal Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
  
	/* Enable USART1_IRQn  Channel */
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	  /* Enable the USART3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	  /*使能RTC秒中断*/
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}






void IWDG_Configuration(void)//这里只配置独立看门狗。独立看门狗使用systic的中断来喂狗
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//写入0x5555,用于允许狗狗寄存器写入功能
	IWDG_SetPrescaler(IWDG_Prescaler_256);//狗狗时钟分频,40K时钟256分频  40/256 = 6.4ms杨耀坤
	IWDG_SetReload(500); //喂狗时间 160*256/40K=1.024s .注意不能大于0xfff,计数器数值
	/*****3/6.4ms = 469 杨耀坤*******/
	IWDG_ReloadCounter(); //喂狗函数,重启计数器	// IWDG_ReloadCounter();//重启计数器（喂狗）
	IWDG_Enable(); //使能狗狗,启动看门狗
}


void SoftReset(void)
{
	__set_FAULTMASK(1); //关闭所有中断
	NVIC_SystemReset(); //复位
}





int main(void)
{	
	ENTER_CRITICAL_SECTION();
	SystemInit();
	allGpioInit();
	delay_init();
	delay_ms(100);
	NVIC_Configuration();
	USART_Config();
	TIM2_Configuration();
	TIM3_Configuration();
	RTC_Config();
	IWDG_Configuration();
	EXIT_CRITICAL_SECTION(); //开启开全局中断
	TIMx_PWM_Init();

//	Byj_cmd(0,2083);   
//	Byj_Start(1,150);  //注意这个  
//		USART_printf(UART4,"%s",i);
	while( 1 )
	{
//		if(mainRunTime > 1)
		if(1)
		{
			//	mainRunTime = 0;
/************************************************************************/
			IWDG_ReloadCounter(); 
//			Byj_usart();
//			Byj_checkBtn();		
		 // Usart_SendString(USART3,"123"); 
		Byj_Poll();
		Kdg_Poll();
		}
	}
}

	


