/*
	�����������������
	
	PB12 -- pul-
	PB13 -- dir-
	PB14 -- enb-
chk2     pd0   ��
chk3     pa12  ����
chk4     pa14  ����
DI4      pb1   ̽���� 
*/
#include   "BYJ.h"
#include   "systick.h"
#include   "myUsart.h"
//#include   "DownData.h"
//#include   "UpData.h"
#include   "timeInit.h"


//u32    Byj_CheckUpTime =0;   //�Լ�������ʱʱ�� 
//u16    Byj_checkDownFlag =0;  //�Լ��½�flag 
u32    ByjTimerPeriod  =  4000; 
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure2;

struct  gpiostatus  status =
{
	 0
};


u8  getGpioStatus(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,u8 lastStatus)
{
	if(lastStatus) //��
	{
			if(!GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)) //��
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
	else   //�ر�
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


void getAllStatus(void) //���ܻ����10ms����ʱ
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



//�����ƣ������һЩ�̵���
void  allGpioInit(void)
{
		GPIO_InitTypeDef   GPIO_InitStructure;
	  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE); 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE); 
	
		/* �������ݿڵ�GPIO�ܽ�ģʽ�����ģʽ* j1��j2 J3 J4*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 					//ʹ��GPIOʱ��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_7|GPIO_Pin_6; //ʹ��PC8~PC9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO���������ģʽ
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);  				//��ص�GPIO�ڳ�ʼ��
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
	  /* �������ݿڵ�GPIO�ܽ�ģʽ�����ģʽ,�������õ���led2 PB8*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;    
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);  

		//����ת
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	//ʹ��GPIOʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO���������ģʽ
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

		GPIO_ResetBits(GPIOD, GPIO_Pin_12);//������1--6
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		
	 /*����*/
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

//���� 0---8
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
								 GPIOC->BSRR = GPIO_Pin_9; //����
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
	����
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
  /* �����ж�Դ */
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
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�½���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	EXTI->RTSR &= ~0X00004;		
}

//�����ش��� �����������
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
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�Ͻ����ж�
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure); 
		//EXTI->RTSR &= ~0X00002;	//��ֹ
		//EXTI->RTSR |= 0x00002;  //����
}



//��λ�豸
u8	initDevice(void)
{
		return 0;	
}

u8  resetFlag =0;    //��λ��־
 //�������Źر�
#define  ALLCLOSE  status.door1s+status.door2s+status.door3s+status.door4s+status.door5s \
									 +status.door6s+status.door7s+status.door8s+status.door9s

//���ٸ�λ
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
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		     // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =  macTIM_CH4_PIN; 
  GPIO_Init(macTIM_CH4_PORT, &GPIO_InitStructure);	
	
//	BYJ_LEFT;
//	BYJ_DISABLE;
	BYJDIRECT(0); //����
}

/**
  * @brief  ����TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
  * @param  ��
  * @retval ��
**/

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
 */


static void TIMx_Mode_Config(void)
{

	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
 	u16 CCR1_Val = 500;       
  /* Time base configuration */		 
  TIM_TimeBaseStructure2.TIM_Period = ByjTimerPeriod-1;          //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure2.TIM_Prescaler = 72-1;	                 //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure2.TIM_ClockDivision = TIM_CKD_DIV1 ;	   //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
  TIM_TimeBaseStructure2.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(macTIMx, &TIM_TimeBaseStructure2);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 20000;	       //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC4Init(macTIMx, &TIM_OCInitStructure);	   //ʹ��ͨ��4
  TIM_OC4PreloadConfig(macTIMx, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(macTIMx, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR
  /* TIM4 enable counter */
	TIM_SetCompare4(TIM4,(ByjTimerPeriod/2)-1);
  TIM_Cmd(macTIMx, DISABLE);                   
	TIM_TimeBaseInit(macTIMx, &TIM_TimeBaseStructure2);
	
 //	macTIMx->ARR  = 1000; //ֱ���޸�����
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
	�ú���4msִ��һ��
*/
int  xTime =0;
int  yXiFen =4000;      //��ʱ������
int  sMaiChong =0;      //���������
int  aNum =0;
int  zhouqi =0;         //ʵ���������� pwm
int  lastzhouqi=0;     
int  ceshiZQ = 10000;   // ������������
int  ceshiTD = 1;       //ÿ�������������
int  ceshiMin = 50;     //1200ת
int  total=0;           //�ܹ�������
int  countnum =0;       // ���� 
u8  whichDirection =0;  //0 ���˶�  1 �� 2��
//  ��⵽�źŴ���
//  ��ʱ��ֵ / pwm���� = �ߵĲ��� 
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
							if(checkNum)	//У׼
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
					EXTI->RTSR &= ~0X00004;	//��ֹ 
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
			
			if(zhouqi <= ceshiMin)  //̫��  
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
   whichDirection =FangXiang; //0 ���˶�  1 �� 2��
	 TIM_Cmd(TIM3, ENABLE);  	
	}
}

int curPosition =0; //��ǰλ��
//��λУ׼   ����У׼  ����ָ��У׼

void  Byj_check(void)
{
	if(whichDirection == 0)
	{
		EXTI->RTSR |= 0x00004;   //����
	  total = 125000+125000/8;   
    whichDirection = 1; //0 ���˶�  1 �� 2��
	  TIM_Cmd(TIM3, ENABLE);		
	}
}


/*
  �����ƶ�
*/

/*
		
*/
u16   sheetL = 7;      //�豸����  
u16   sheetLevel[10]={8,8,8,8,8,8,8,8,8}; //ÿ��ȷ�
u16   zongGK =0, GKcount =0;  ;  //�ܸ����

int  sheet[8][65]={                       //ÿ��ÿ��������0λ����
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
		//i �ǲ��� ��������  DoorNum �ǵ�ǰ�����ĸ��λ��
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

//���յ�ָ��
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

		����* ϸ�֣�400�� =  һȦʱ��
*/
 
/*
				���� ���� 0 �� right  1 left
*/
void   Byj_Start( u8  whichDirection,u16 ByjPeriod)
{
		if(whichDirection)
		{
			BYJDIRECT(1); //����
		}
		else
		{
			BYJDIRECT(0); //����
		}
		ByjTimerPeriod = ByjPeriod; //2ms*400 = 1600ms һȦ
		TIM_TimeBaseStructure2.TIM_Period = ByjTimerPeriod-1; 
	  TIM_SetCompare4(TIM4,(ByjTimerPeriod/2)-1);
    TIM_Cmd(macTIMx, ENABLE);                   
	  TIM_TimeBaseInit(macTIMx, &TIM_TimeBaseStructure2);	
}

/*
   ֱ��ֹͣ
*/
void  Byj_Stop(void)
{
	//		BYJ_DISABLE;	
    TIM_Cmd(macTIMx, DISABLE);    //ʹ�ܶ�ʱ��4	
}




void Byj_Poll(void)
{
	CloseAllDoor();
	OpenDoor(OpenDoorNum );
}













