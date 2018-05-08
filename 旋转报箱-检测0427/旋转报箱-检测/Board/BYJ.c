/*
	驱动步进电机控制器
	
	PB12 -- pul-
	PB13 -- dir-
	PB14 -- enb-
chk2     pd0   门
chk3     pa12  顶上
chk4     pa14  底下
DI4      pb1   探物检测 
*/
#include   "BYJ.h"
#include   "systick.h"
#include   "myUsart.h"
//#include   "DownData.h"
//#include   "UpData.h"
#include   "timeInit.h"


//u32    Byj_CheckUpTime =0;   //自检上升超时时间 
//u16    Byj_checkDownFlag =0;  //自检下降flag 
u32    ByjTimerPeriod  =  4000; 
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure2;

struct  gpiostatus  status =
{
	 0
};


u8  getGpioStatus(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 lastStatus)
{
	if(lastStatus) //开
	{
			if(!GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)) //关
			{
						delay_ms(1);
						if(!GPIO_ReadInputDataBit(GPIOx,GPIO_Pin))
						{
									return 0;
						}
			}
			else
			{
				return lastStatus;
			}	
	}
	else   //关闭
	{
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin))
			{
					delay_ms(1);
					if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin))
					{
								return 1;
					}			
			}
			else
			{
				return lastStatus;
			}
	}
	return 0;
}


void getAllStatus(void) //可能会产生10ms的延时
{
		status.door1s = getGpioStatus(GPIOD,GPIO_Pin_1,status.door1s);		
		status.door2s = getGpioStatus(GPIOD,GPIO_Pin_0,status.door2s);	
		status.door3s = getGpioStatus(GPIOA,GPIO_Pin_12,status.door3s);	
		status.door4s = getGpioStatus(GPIOA,GPIO_Pin_11,status.door4s);	
		status.door5s = getGpioStatus(GPIOA,GPIO_Pin_7,status.door5s);	
		status.door6s = getGpioStatus(GPIOC,GPIO_Pin_4,status.door6s);	
		status.door7s = getGpioStatus(GPIOC,GPIO_Pin_5,status.door7s);	
		status.door8s = getGpioStatus(GPIOA,GPIO_Pin_0,status.door8s);	
		status.door9s = getGpioStatus(GPIOA,GPIO_Pin_1,status.door9s);	
}



//心跳灯，主板的一些继电器
void  allGpioInit(void)
{
		GPIO_InitTypeDef   GPIO_InitStructure;
	  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE); 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE); 
	
		/* 配置数据口的GPIO管脚模式，输出模式* j1，j2 J3 J4*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 					//使能GPIO时钟
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_7|GPIO_Pin_6; //使用PC8~PC9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO工作在输出模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);  				//相关的GPIO口初始化
	  /* y1 -y5   pb12 13 14 15 pd8*/
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 			
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 					
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_5|GPIO_Pin_6; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				     
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);  	

		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	  /* 配置数据口的GPIO管脚模式，输出模式,这里配置的是led2 PB8*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;    
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);  

		//正反转
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//使能GPIO时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO工作在输出模式
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);  

		
//		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
//		GPIO_SetBits(GPIOB,GPIO_Pin_8);
		GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		GPIO_ResetBits(GPIOC,GPIO_Pin_6);
		
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		GPIO_ResetBits(GPIOD,GPIO_Pin_8);

		GPIO_ResetBits(GPIOD, GPIO_Pin_12);//机电器1--6
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		
	 /*输入*/
	 //chk1  chk2  
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 					
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				           
	 GPIO_Init(GPIOD, &GPIO_InitStructure); 
	 //chk3  chk4   di1  
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 					
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_7; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				           
	 GPIO_Init(GPIOA, &GPIO_InitStructure); 
	 //di2 di3
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 					
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				           
	 GPIO_Init(GPIOC, &GPIO_InitStructure); 
	 //di4 5 6
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 					
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				           
	 GPIO_Init(GPIOB, &GPIO_InitStructure); 	 
}

//开门 0---8
u16 OpenFlag =0; 
u16 OpenDoorNum =0x00;
u16 CloseFlag =0;
void  OpenDoor(u16 num)
{	
	if(OpenFlag)
	{
		OpenFlag =0;
		switch(num)
		{
			case 0x00: 
								 GPIOC->BSRR = GPIO_Pin_9; //吸合
				break;
			case 0x01: 
								 GPIOC->BSRR = GPIO_Pin_8;
				break;
			case 0x02: 
								 GPIOC->BSRR = GPIO_Pin_7;
				break;
			case 0x03: 
								 GPIOC->BSRR = GPIO_Pin_6;
				break;
			case 0x04: 
								 GPIOB->BSRR = GPIO_Pin_12;
				break;
			case 0x05: 
								 GPIOB->BSRR = GPIO_Pin_13;
				break;
			case 0x06: 
								 GPIOB->BSRR = GPIO_Pin_14;
				break;
			case 0x07: 
								 GPIOB->BSRR = GPIO_Pin_15;
				break;
			case 0x08: 
				break;		
			default:
				break;
		}
		CloseFlag =1; 
	}
}

/*
	关门
*/
void CloseAllDoor(void)
{
	if(CloseFlag > 500)
	{
		CloseFlag =0;
		GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		GPIO_ResetBits(GPIOC,GPIO_Pin_6);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		GPIO_ResetBits(GPIOD,GPIO_Pin_8);	
	}
}






void EXTI_B2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE); 	
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //下降沿
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	EXTI->RTSR &= ~0X00004;		
}

//上升沿触发 捕获脉冲个数
void BD_EXTI_PxgConfig(void)
{
		GPIO_InitTypeDef GPIO_InitStructure; 
	  EXTI_InitTypeDef EXTI_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO, ENABLE); 	
//		/* config the NVIC */
//	  NVIC_Configuration();	
		/* EXTI line gpio config*/	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;       
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		/* EXTI line mode config */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1); 
		EXTI_InitStructure.EXTI_Line = EXTI_Line1;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上降沿中断
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure); 
		//EXTI->RTSR &= ~0X00002;	//禁止
		//EXTI->RTSR |= 0x00002;  //允许
}



//复位设备
u8	initDevice(void)
{
		return 0;	
}

u8  resetFlag =0;    //复位标志
 //所有箱门关闭
#define  ALLCLOSE  status.door1s+status.door2s+status.door3s+status.door4s+status.door5s \
									 +status.door6s+status.door7s+status.door8s+status.door9s

//低速复位
void startDevice(void)
{
	if(resetFlag == 0)
	{
			if(ALLCLOSE)
			{
					initDevice();	
			}
			else
			{
					
			}	
	}
}



static void TIMx_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	macTIM_APBxClock_FUN (macTIM_CLK, ENABLE);
  macTIM_GPIO_APBxClock_FUN (macTIM_GPIO_CLK, ENABLE); 
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		     // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =  macTIM_CH4_PIN; 
  GPIO_Init(macTIM_CH4_PORT, &GPIO_InitStructure);	
	
//	BYJ_LEFT;
//	BYJ_DISABLE;
	BYJDIRECT(0); //方向
}

/**
  * @brief  配置TIM3输出的PWM信号的模式，如周期、极性、占空比
  * @param  无
  * @retval 无
**/

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 */


static void TIMx_Mode_Config(void)
{

	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
 	u16 CCR1_Val = 500;       
  /* Time base configuration */		 
  TIM_TimeBaseStructure2.TIM_Period = ByjTimerPeriod-1;          //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure2.TIM_Prescaler = 72-1;	                 //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure2.TIM_ClockDivision = TIM_CKD_DIV1 ;	   //设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure2.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(macTIMx, &TIM_TimeBaseStructure2);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 20000;	       //设置通道4的电平跳变值，输出另外一个占空比的PWM
  TIM_OC4Init(macTIMx, &TIM_OCInitStructure);	   //使能通道4
  TIM_OC4PreloadConfig(macTIMx, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(macTIMx, ENABLE);			 // 使能TIM3重载寄存器ARR
  /* TIM4 enable counter */
	TIM_SetCompare4(TIM4,(ByjTimerPeriod/2)-1);
  TIM_Cmd(macTIMx, DISABLE);                   
	TIM_TimeBaseInit(macTIMx, &TIM_TimeBaseStructure2);
	
 //	macTIMx->ARR  = 1000; //直接修改周期
}


void TIMx_PWM_Init(void)
{
	allGpioInit();
	EXTI_B2_Config();
//	BD_EXTI_PxgConfig();
	TIMx_GPIO_Config();
	TIMx_Mode_Config();	
}



/*
	该函数4ms执行一次
*/
int  xTime =0;
int  yXiFen =4000;      //定时器周期
int  sMaiChong =0;      //输出脉冲数
int  aNum =0;
int  zhouqi =0;         //实际脉冲周期 pwm
int  lastzhouqi=0;     
int  ceshiZQ = 10000;   // 增加脉冲周期
int  ceshiTD = 1;       //每次增加脉冲个数
int  ceshiMin = 50;     //1200转
int  total=0;           //总共脉冲数
int  countnum =0;       // 计数 
u8  whichDirection =0;  //0 不运动  1 右 2左
//  检测到信号处理
//  计时器值 / pwm周期 = 走的步数 
int checkNum =0;

void  addMoveByj(void) 
{			
		if(sMaiChong > total || sMaiChong ==total)  
			{
					TIM_Cmd(TIM4, DISABLE); 
					TIM_Cmd(TIM3, DISABLE); 
					if (whichDirection == 1)
					{
							curPosition += sMaiChong;
							if(checkNum)	//校准
							{
								curPosition = (sMaiChong - checkNum);
							}							
					}
					else if (whichDirection == 2)
					{
						curPosition -= sMaiChong;
					} 

					if (curPosition >= 125000)
					{
						curPosition -= 125000;
					}
					else if (curPosition < 0)
					{
						curPosition = 125000 + curPosition;
					}
					printf("N%d\tT%d\ts%d\tA%d\tP%d\ttim%d\tC%d\tN%d\r\n",aNum,zhouqi,sMaiChong,total,curPosition,countnum,checkNum,OpenDoorNum);
					//my_sendstring(USART2,"123",2);
					OpenFlag =1;					
					whichDirection =0;
					zhouqi =0;
					aNum   =0;
					sMaiChong =0;	
					countnum  =0;
					lastzhouqi =0;
					checkNum  =0; 
					EXTI->RTSR &= ~0X00004;	//禁止 
					GPIOB->BRR=GPIO_Pin_8;	
					return;			
			} 
			countnum ++;
			if(sMaiChong+aNum-1 < (total / 2)+1)
			{
					aNum += ceshiTD;
					//aNum = 20;
					zhouqi = (ceshiZQ/aNum);
			}
			else if(sMaiChong < total)
			{
				  aNum = aNum-ceshiTD;
					//aNum =20;
					if(aNum < 10)
					{
							if(sMaiChong < total-60)
							{
								aNum =10; 	
							}
							else
							{	
							}
					}
					if(aNum<1)
					{
						aNum =1;
					}
					zhouqi = (ceshiZQ/aNum);	
			}			
			if(whichDirection == 1)
			{	GPIO_ResetBits(GPIOA,GPIO_Pin_6);}
			else if(whichDirection == 2)
			{	GPIO_SetBits(GPIOA,GPIO_Pin_6);}	
			
			if(zhouqi <= ceshiMin)  //太快  
			{
				zhouqi=ceshiMin;
				sMaiChong += ceshiZQ/zhouqi;
				TIM3->ARR = 10000-1;  // 
			} 
		else
			{
					sMaiChong +=aNum;
					TIM3->ARR = (zhouqi * aNum)-1; //20000;//zhouqi * aNum; 
			}	
			
			if(lastzhouqi!= zhouqi)
			{
				lastzhouqi =zhouqi; 
			  TIM4->ARR = zhouqi-1;
			  TIM4->CCR4 = (zhouqi-1)/2-1;			
			}
			else
			{	
			}
			TIM4->CR1 |= TIM_CR1_CEN;	
			TIM_Cmd(TIM3, ENABLE);
}



void Byj_cmd(u8 FangXiang, int JuLi)
{	
	if(whichDirection == 0)
	{
	 total = JuLi;  
   whichDirection =FangXiang; //0 不运动  1 右 2左
	 TIM_Cmd(TIM3, ENABLE);  	
	}
}

int curPosition =0; //当前位置
//零位校准   开机校准  接收指令校准

void  Byj_check(void)
{
	if(whichDirection == 0)
	{
		EXTI->RTSR |= 0x00004;   //允许
	  total = 125000+125000/8;   
    whichDirection = 1; //0 不运动  1 右 2左
	  TIM_Cmd(TIM3, ENABLE);		
	}
}


/*
  紧急制动
*/

/*
		
*/
u16   sheetL = 7;      //设备层数  
u16   sheetLevel[10]={8,8,8,8,8,8,8,8,8}; //每层等分
u16   zongGK =0, GKcount =0;  ;  //总格口数

int  sheet[8][65]={                       //每层每个格口相对0位距离
	{0, 15625, 31250, 46875, 62500, 78125, 93750, 109375,0},
	{0, 15625, 31250, 46875, 62500, 78125, 93750, 109375,0},
	{0, 15625, 31250, 46875, 62500, 78125, 93750, 109375,0},
	{0, 15625, 31250, 46875, 62500, 78125, 93750, 109375,0},
	{0, 15625, 31250, 46875, 62500, 78125, 93750, 109375,0},
	{0, 15625, 31250, 46875, 62500, 78125, 93750, 109375,0},
	{0, 15625, 31250, 46875, 62500, 78125, 93750, 109375,0},	
};

/*
	
*/
void  Byj_GoTo(u16 DoorNum)
{
		int i =0, juli=0;
		u8  fangxiang =0;
		zongGK =0;
		printf("%d\r\n",DoorNum);
		for(i =0;i<sheetL;i++)
		{
			zongGK +=sheetLevel[i];
		}
		if(DoorNum > zongGK)
		{
			Byj_check();
			printf("no DoorNum\r\n");
			return;
		}
		zongGK =0;
		for(i = 0;i<sheetL;i++)
		{
				zongGK +=sheetLevel[i];
				if(DoorNum < zongGK) 
				{
						DoorNum	-=(zongGK - sheetLevel[i]);
						break;
				}
		}
		//i 是层数 用来开门  DoorNum 是当前层数的格口位置
		OpenDoorNum = i; 
		juli = sheet[i][DoorNum] - curPosition;
			if (juli == 0 || juli==125000) 
			{
					printf("dont move\r\n");
			}
			else if(juli > 0)
			{
				if (juli > 125000 / 2)
				{
					fangxiang = 2;
					juli = 125000 - juli;
				}
				else
				{
					fangxiang = 1;
				}
			}
			else
			{
				juli *= -1;
				if (juli > 125000/2)
				{
					fangxiang = 1;
					juli = 125000 - juli;
				}
				else
				{
					fangxiang = 2;
				}
			}	
				Byj_cmd(fangxiang,juli);		 		
				printf("P%d\t D%d\tS %d\t L%d\t\r\n",curPosition,fangxiang,juli,i);		
}

//接收到指令
void  Byj_usart(void)
{
		if(uart4_Door.flag)
		{
			uart4_Door.flag =0;
			Byj_GoTo((uart4_Door.data[0]-0x30)*10+(uart4_Door.data[1]-0x30)-1);	
		}
}







/**************************************************************************************************/
 /*

		周期* 细分（400） =  一圈时间
*/
 
/*
				参数 方向 0 是 right  1 left
*/
void   Byj_Start( u8  whichDirection,u16 ByjPeriod)
{
		if(whichDirection)
		{
			BYJDIRECT(1); //方向
		}
		else
		{
			BYJDIRECT(0); //方向
		}
		ByjTimerPeriod = ByjPeriod; //2ms*400 = 1600ms 一圈
		TIM_TimeBaseStructure2.TIM_Period = ByjTimerPeriod-1; 
	  TIM_SetCompare4(TIM4,(ByjTimerPeriod/2)-1);
    TIM_Cmd(macTIMx, ENABLE);                   
	  TIM_TimeBaseInit(macTIMx, &TIM_TimeBaseStructure2);	
}

/*
   直接停止
*/
void  Byj_Stop(void)
{
	//		BYJ_DISABLE;	
    TIM_Cmd(macTIMx, DISABLE);    //使能定时器4	
}




void Byj_Poll(void)
{
	CloseAllDoor();
	OpenDoor(OpenDoorNum );
}













