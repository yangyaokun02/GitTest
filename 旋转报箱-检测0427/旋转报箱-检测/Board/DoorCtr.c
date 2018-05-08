
#include  "DoorCtr.h" 
#include  "myUsart.h"
#include  "systick.h"



u16  lockOpenFlag  =0 ;      //开锁500ms
u16  doorOpenFlag  =0;       //开电机超时时间    3000ms  
u16  doorCloseFlag = 0;		   //关闭电机超时时间
u16  doorOpenTime  = 0;      //电机开门等待时间  3000ms
u8   doorMsg = 0x00;         //总的信息
u8   doorMsgLock  = 0x00;		 //电子锁状态
u8   doorLockTemp =0x00;
u8   doorMsgDoor  = 0x00;		 //电机门的状态
u8   doorSendData[5] = {0,0,0,0,0};  //

u8   doorBtn1 =0x00;          //关闭
u8   doorBtn1Temp =0x00; 
u8   doorBtn2 =0x00;          //打开
u8   doorBtn2Temp =0x00;  

unsigned int  doorCrc =0x00;     //发送crc
unsigned int  doorRxCrc =0x00; 
unsigned int  doorCrcTemp =0x00; //接收crc


u16   CloseDoorAllTime = 0; //4s 中尝试一次
u8    CloseDoorCount =0;    //连续三次则故障

u16   OpenDoorAllTime =0;   //关门时间
u8    OpenDoorCount =0;




unsigned int crcVal(unsigned char *pcMess,unsigned int wLen);


/*
		GPIO_init		
		j1  m1+  m1-
*/
void   doorGpioInit(void)
{
		GPIO_InitTypeDef   GPIO_InitStructure;
	  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE); 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOB, ENABLE); 
	
			/* 配置数据口的GPIO管脚模式，输出模式* j1，j2 J3 J4*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 					//使能GPIO时钟
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//|GPIO_Pin_9|GPIO_Pin_7|GPIO_Pin_6; //使用PC8~PC9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO工作在输出模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);  				//相关的GPIO口初始化
	

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 					//使能GPIO时钟
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO工作在输出模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
		LOCK_1(ON); 		
		M1_OPN_TQ;
		M1_CLS_TQ;
	
		//pc4 pc5  pb0  pb1 
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 					//使能GPIO时钟
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;            //使用PC8~PC9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				
  	GPIO_Init(GPIOC, &GPIO_InitStructure);  
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 					//使能GPIO时钟
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;            //使用PC8~PC9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				
  	GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 					//使能GPIO时钟
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				            //GPIO工作在输出模式
  	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure); 
		
}

/*
		上电检测箱门是否关闭，若没有则关闭
*/
void   startCheckDoor(void)
{
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))
	{
			delay_ms(1);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))
			{
						M1_CLS_TQ;
						M1_OPN_XH;
						doorOpenFlag = MY_FLAG;
						doorOpenTime = MY_FLAG;
						OpenDoorAllTime = MY_FLAG;
						OpenDoorCount =0;
						CloseDoorCount =0;		
			}
		}						
}



/*

	控制电机工作


*/

void  doorRelayCtr(void) 
{
	
	if(uart4_Door.flag)
	{
		
		uart4_Door.flag =0;
		uart4_Door.count =0;
		
		doorRxCrc = crcVal(uart4_Door.data,2);
		doorCrcTemp =  uart4_Door.data[2];
		doorCrcTemp = (doorCrcTemp<<8) + uart4_Door.data[3]; 
		
		if(doorRxCrc == doorCrcTemp)  //
		{
				
			switch(uart4_Door.data[1]) 
			{
				case 0x01:    //admin
						LOCK_1(OFF); 					
						lockOpenFlag  = MY_FLAG; 
					break;
				case 0x02:    //user
						M1_CLS_TQ;
						M1_OPN_XH;
						doorOpenFlag = MY_FLAG;
						doorOpenTime = MY_FLAG;
						OpenDoorAllTime = MY_FLAG;
						OpenDoorCount =0;
						CloseDoorCount =0;
					break;
				case 0x03:
						my_sendstring(UART4,(char *)doorSendData,4); 	
					break;
				default:
					break;
			}
		}
		else
		{
			my_sendstring(UART4,(char *)uart4_Door.data,4);
		}
	}	
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
}



/*
		
*/
void  doorGetGpio(void)
{
	
		doorLockTemp  =	!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1);
		if(doorLockTemp != doorMsgLock)
		{
				delay_ms(1);
		 doorLockTemp  =	!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1);		
			if(doorLockTemp != doorMsgLock)
				{
					doorMsgLock  = doorLockTemp;  
				}			
		}

		doorBtn1Temp = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
		if(doorBtn1Temp != doorBtn1)
		{
				delay_ms(1);
				doorBtn1Temp = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
				if(doorBtn1Temp != doorBtn1)
				{
					doorBtn1 = doorBtn1Temp;
				}
		}

		doorBtn2Temp = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);	
		if(doorBtn2Temp != doorBtn2)
		{
			delay_ms(1);
			doorBtn2Temp = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);
			if(doorBtn2Temp != doorBtn2)
			{
					doorBtn2 = doorBtn2Temp; 
			}
		}
		
		if(doorBtn1)  
		{
				if(doorBtn2)
				{
						//error
					  doorMsgDoor = 0x05; //异常
				}
				else //open   //0  1 
				{
					doorMsgDoor = 0x00;
				}
		}
		else
		{
				if(doorBtn2)
				{
					doorMsgDoor = 0x02; 
				}
				else
				{
					if(doorOpenFlag>0&&doorOpenFlag<DoorOverTime) //开箱中
					{
						 doorMsgDoor =0x01;
					}
					else if(doorCloseFlag>0&&doorCloseFlag<DoorOverTime) //关箱中
					{
						 doorMsgDoor =0x03;
					}
					else
					{
						 doorMsgDoor = 0x04;   //异常
					}
				}
		}	
		
		
		doorSendData[1] =  (doorMsgLock << 4) + doorMsgDoor;
		if(doorSendData[1] != doorMsg)
		{
			doorMsg = doorSendData[1];
			doorSendData[0] = 0xA5; 
			doorCrc  = crcVal(doorSendData,2);
			doorSendData[2] = doorCrc >> 8; 
			doorSendData[3] = doorCrc;
			my_sendstring(UART4,(char *)doorSendData,4); 
		}
}




/*
		弹起锁具，关闭箱门
		如果再相应的时间内，还没有检测到门被关闭，则立马打开箱门，重复open_count次 后常开
*/



#define   OPEN_COUNT   5           //如果门没有关上则持续关闭opencount次 最多 9次
#define   OPEN_TIME    8000			   //开门到关门的时间 OPEN_TIME 毫秒  再次开启，再次关闭


void doorCtrFunc(void)
{
	
			//方便测试   可用来做防夹
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))
		{
				delay_ms(3);
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))
				{
//						LOCK_1(OFF); 					
//						lockOpenFlag  = MY_FLAG; 
					if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
					{
						delay_ms(1);
						if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
						{
								M1_CLS_TQ;
								M1_OPN_XH;
								doorOpenFlag = MY_FLAG;
								doorOpenTime = MY_FLAG;
								OpenDoorAllTime = MY_FLAG;
							
								OpenDoorCount = 0;
								CloseDoorCount =0;	
								
								doorCloseFlag = 0;//关闭箱子的中途 防夹，清除管箱子
								CloseDoorAllTime = 0;
						}
						else
						{
								doorOpenTime = 0;
								OpenDoorAllTime = 0;
							  doorOpenTime = OPEN_TIME - 3000;
						
								M1_OPN_TQ;
								OpenDoorCount =0;
								CloseDoorCount =0;	
						}
					}
					else
					{
								doorOpenTime = 0;
								OpenDoorAllTime = 0;
							  doorOpenTime = OPEN_TIME - 3000;
						
								M1_OPN_TQ;
								OpenDoorCount =0;
								CloseDoorCount =0;	
					}
				}
		}

		if(lockOpenFlag >500)
		{
			lockOpenFlag =MY_CLEAR ;
			LOCK_1(ON); 
		}

		if(	doorOpenFlag > DoorOverTime)   //O
		{
				doorOpenFlag =MY_CLEAR;
				M1_OPN_TQ;
		}
		
		if(doorOpenTime > OPEN_TIME)   //开门之后关门
		{
				doorOpenTime = 0;
				M1_OPN_TQ;
				M1_CLS_XH;
				doorCloseFlag = MY_FLAG;
				CloseDoorAllTime = MY_FLAG;
		} 
		
		if(doorCloseFlag > DoorOverTime)
		{
				doorCloseFlag = MY_CLEAR ;
				M1_CLS_TQ;
		}
		
		
		// 提前开好门
		if(doorOpenFlag> 0)             //O
		{
					if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
					{
						delay_ms(1);
						if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
						{
								if(CloseDoorCount < OPEN_COUNT-1) //在关闭次数内
								{
									doorOpenTime = 0;
									OpenDoorAllTime = 0;
									doorOpenTime = OPEN_TIME - 3000;
									M1_OPN_TQ;
									OpenDoorCount =0;
								}
								else
								{
									doorOpenTime = 0;
									OpenDoorAllTime = 0;
									M1_OPN_TQ;
									OpenDoorCount =0;
								}
						}
					}
		}
		//一直没有开好门
		if(OpenDoorAllTime > OPEN_TIME)
		{
			OpenDoorAllTime =0; 
					if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
					{
							delay_ms(1);
							if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
							{							
								if(OpenDoorCount++ < OPEN_COUNT-1) 
								{
									M1_CLS_TQ;
									M1_OPN_XH;
									doorOpenFlag = MY_FLAG;
									doorOpenTime = MY_FLAG;
									OpenDoorAllTime = MY_FLAG;	
								}
								else
								{
										
								}
								if(OpenDoorCount>10)
									OpenDoorCount =10;
							}
					}
		}
		
		//提前关闭门
		if(doorCloseFlag > 0)
		{
					if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))
					{
							delay_ms(1);
							if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))
							{
									doorCloseFlag = MY_CLEAR ;
									M1_CLS_TQ;
									CloseDoorCount=0;	
									CloseDoorAllTime =0;								
							}								
					}
		}
		
		
		
		
		
		//一直没有关闭
		if(CloseDoorAllTime > OPEN_TIME-3000)
		{
				CloseDoorAllTime =0;
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))
				{
					delay_ms(1);
					if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))
					{
							if(CloseDoorCount++ < OPEN_COUNT-1)
							{
//									M1_OPN_TQ;
//									M1_CLS_XH;
//									doorCloseFlag = MY_FLAG;	
//								  CloseDoorAllTime =MY_FLAG;
										M1_CLS_TQ;
										M1_OPN_XH;
										doorOpenFlag = MY_FLAG;
										doorOpenTime = MY_FLAG;
										OpenDoorAllTime = MY_FLAG;
										OpenDoorCount =0;
										//CloseDoorCount =0;
							}
							else
							{
										M1_CLS_TQ;
										M1_OPN_XH;
										doorOpenFlag = MY_FLAG;
										doorOpenTime = 0;
										OpenDoorAllTime = 0;
								
							}
							if(CloseDoorCount > 10)
								CloseDoorCount = 10;
					}	
				}
		}		
		//4s 如果箱门没有关闭重复关闭3次
		
}

/*
1	ms  Add
*/
void  doorCountAdd(void)
{
		if(lockOpenFlag)   
			lockOpenFlag++;
		
		if(doorOpenFlag)
			doorOpenFlag++;	
		
		if(doorCloseFlag)
		doorCloseFlag++;
		
		if(doorOpenTime)
			doorOpenTime++;
		
		if(CloseDoorAllTime)
			CloseDoorAllTime++;
		
		if(OpenDoorAllTime)
			OpenDoorAllTime++;
}


//之江回收箱门运行的三个函数 控制门，获取信号，控制电机
void  DoorAndRelay(void)
{
			doorRelayCtr(); 
			doorGetGpio();
			doorCtrFunc(); 	
}
	












