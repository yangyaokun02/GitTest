
#include  "DoorCtr.h" 
#include  "myUsart.h"
#include  "systick.h"



u16  lockOpenFlag  =0 ;      //����500ms
u16  doorOpenFlag  =0;       //�������ʱʱ��    3000ms  
u16  doorCloseFlag = 0;		   //�رյ����ʱʱ��
u16  doorOpenTime  = 0;      //������ŵȴ�ʱ��  3000ms
u8   doorMsg = 0x00;         //�ܵ���Ϣ
u8   doorMsgLock  = 0x00;		 //������״̬
u8   doorLockTemp =0x00;
u8   doorMsgDoor  = 0x00;		 //����ŵ�״̬
u8   doorSendData[5] = {0,0,0,0,0};  //

u8   doorBtn1 =0x00;          //�ر�
u8   doorBtn1Temp =0x00; 
u8   doorBtn2 =0x00;          //��
u8   doorBtn2Temp =0x00;  

unsigned int  doorCrc =0x00;     //����crc
unsigned int  doorRxCrc =0x00; 
unsigned int  doorCrcTemp =0x00; //����crc


u16   CloseDoorAllTime = 0; //4s �г���һ��
u8    CloseDoorCount =0;    //�������������

u16   OpenDoorAllTime =0;   //����ʱ��
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
	
			/* �������ݿڵ�GPIO�ܽ�ģʽ�����ģʽ* j1��j2 J3 J4*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 					//ʹ��GPIOʱ��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//|GPIO_Pin_9|GPIO_Pin_7|GPIO_Pin_6; //ʹ��PC8~PC9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO���������ģʽ
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);  				//��ص�GPIO�ڳ�ʼ��
	

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 					//ʹ��GPIOʱ��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO���������ģʽ
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
		LOCK_1(ON); 		
		M1_OPN_TQ;
		M1_CLS_TQ;
	
		//pc4 pc5  pb0  pb1 
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 					//ʹ��GPIOʱ��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;            //ʹ��PC8~PC9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				
  	GPIO_Init(GPIOC, &GPIO_InitStructure);  
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 					//ʹ��GPIOʱ��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;            //ʹ��PC8~PC9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				
  	GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 					//ʹ��GPIOʱ��
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				            //GPIO���������ģʽ
  	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure); 
		
}

/*
		�ϵ��������Ƿ�رգ���û����ر�
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

	���Ƶ������


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
					  doorMsgDoor = 0x05; //�쳣
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
					if(doorOpenFlag>0&&doorOpenFlag<DoorOverTime) //������
					{
						 doorMsgDoor =0x01;
					}
					else if(doorCloseFlag>0&&doorCloseFlag<DoorOverTime) //������
					{
						 doorMsgDoor =0x03;
					}
					else
					{
						 doorMsgDoor = 0x04;   //�쳣
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
		�������ߣ��ر�����
		�������Ӧ��ʱ���ڣ���û�м�⵽�ű��رգ�����������ţ��ظ�open_count�� �󳣿�
*/



#define   OPEN_COUNT   5           //�����û�й���������ر�opencount�� ��� 9��
#define   OPEN_TIME    8000			   //���ŵ����ŵ�ʱ�� OPEN_TIME ����  �ٴο������ٴιر�


void doorCtrFunc(void)
{
	
			//�������   ������������
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
								
								doorCloseFlag = 0;//�ر����ӵ���; ���У����������
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
		
		if(doorOpenTime > OPEN_TIME)   //����֮�����
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
		
		
		// ��ǰ������
		if(doorOpenFlag> 0)             //O
		{
					if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
					{
						delay_ms(1);
						if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
						{
								if(CloseDoorCount < OPEN_COUNT-1) //�ڹرմ�����
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
		//һֱû�п�����
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
		
		//��ǰ�ر���
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
		
		
		
		
		
		//һֱû�йر�
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
		//4s �������û�йر��ظ��ر�3��
		
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


//֮�������������е��������� �����ţ���ȡ�źţ����Ƶ��
void  DoorAndRelay(void)
{
			doorRelayCtr(); 
			doorGetGpio();
			doorCtrFunc(); 	
}
	












