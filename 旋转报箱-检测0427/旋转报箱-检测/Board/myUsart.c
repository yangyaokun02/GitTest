#include "myUsart.h"

struct usartData   usart2_Up =
{
			0,
			0,
			0,
			0,
			0
};   //�Ͱ�׿��ͨѶ����2
struct usartData   usart3_Down =
{
			0,
			0,
			0,
			0,
			0
}; //��������ͨѶ����3


struct usartData   uart4_Door =
{
	0,
	0,
	0,
	0,
	0
} ;  //���հ�׿�������Ŀ���ָ��




 /** 
  * @author  yyk
  * @brief     
  * @param  ��
  * @retval ��
  */

/*******************************************************************************
** ������: USART_ConfigInitRX()
** ����:   ��������Ϊ˫���շ�ģʽ�����ղ�����Ϊ9600
*******************************************************************************/
void USART_Config(void)
{
	
	  GPIO_InitTypeDef GPIO_InitStructure;			//����GPIO��ʼ���ṹ��
    USART_InitTypeDef USART_InitStructure; 		//����IIC��ʼ���ṹ��
	
//		/**********PA9 PA10*****/
//			 /*GPIOA Periph clock enable*/
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//		/*USART2 Periph clock enable*/
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);		   

//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//		USART_InitStructure.USART_BaudRate = Baud;				//
//  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  	USART_InitStructure.USART_Parity = USART_Parity_No;
//  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//  	/* Configure USART1 */
//  	USART_Init(USART1, &USART_InitStructure);
//	  /* Enable USART1 Receive and Transmit interrupts */
//	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				//ʹ�ܽ����ж�  uart 1
//  	//USART_ITConfig(USART1, USART_IT_TXE,  ENABLE);			//ʹ�ܷ����ж�
//    USART_ITConfig(USART1, USART_IT_TXE,  DISABLE);	      //���÷����ж�
//  	/* Enable the USART1 */
//  	USART_Cmd(USART1, ENABLE);	
		
	/*************(������ΪUSRT2) Tx(PA.2***Rx(A.3)*************************/
	
	 /*GPIOA Periph clock enable*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	  USART_InitStructure.USART_BaudRate = 115200;				//
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	/* Configure USART2 */
  	USART_Init(USART2, &USART_InitStructure);
	  /* Enable USART2 Receive and Transmit interrupts */
	  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);				//ʹ�ܽ����ж�  uart 2
  	//USART_ITConfig(USART2, USART_IT_TXE,  ENABLE);				//ʹ�ܷ����ж�
    USART_ITConfig(USART2, USART_IT_TXE,  DISABLE);	      //���÷����ж�
  	/* Enable the USART2 */
  	USART_Cmd(USART2, ENABLE);		
		
		
		/*************(������ΪUSRT3) Tx(PB.10***Rx(B.11)*************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB Periph clock enable
    /*USART2 Periph clock enable*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	  /* Configure USART3 Tx (PB.10) as alternate function open-drain */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					//
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
		 /* Configure USART3 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	  USART_InitStructure.USART_BaudRate = 9600;//Baud;				//
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	/* Configure USART3 */
  	USART_Init(USART3, &USART_InitStructure);
	  /* Enable USART3 Receive and Transmit interrupts */
	  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);				//ʹ�ܽ����ж�  uart3
  	//USART_ITConfig(USART3, USART_IT_TXE,  ENABLE);				//ʹ�ܷ����ж�
		USART_ITConfig(USART3, USART_IT_TXE,  DISABLE);	      //���÷����ж�
  	/* Enable the USART3 */
  	USART_Cmd(USART3, ENABLE);														//USART3
		
	/*************(������ΪUSRT4) Tx(PC.10***Rx(C.11)*************************/		
	  /*GPIOA Periph clock enable*/	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		/*USART4 Periph clock enable*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		/* Configure USART4 Rx (PC.11)  as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(UART4, &USART_InitStructure);
	  /* Enable USART4 Receive and Transmit interrupts */
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);						//ʹ�ܽ����ж�
	  //USART_ITConfig(USART4, USART_IT_TXE,  ENABLE);				//ʹ�ܷ����ж�
		USART_ITConfig(UART4, USART_IT_TXE,  DISABLE);					//�رշ����ж�
		USART_Cmd(UART4, ENABLE);
		
		
/*************(������ΪUSRT5) Tx(PC.12***Rx(D.2)*************************/		
    /*GPIOA Periph clock enable*/	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		/*USART5 Periph clock enable*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
		/* Configure USART5_Tx as alternate function push-pull */
		//Configure USART5 Tx (PC.12) as alternate function open-drain */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		/* Configure USART5 Rx (PD.2)  as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		USART_InitStructure.USART_BaudRate = Baud;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(UART5, &USART_InitStructure);
	  /* Enable USART5 Receive and Transmit interrupts */
		USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);				//ʹ�ܽ����ж�
	  // USART_ITConfig(USART5, USART_IT_TXE,  ENABLE);		//ʹ�ܷ����ж�
		USART_ITConfig(UART5, USART_IT_TXE,  DISABLE);			//�رսӷ����ж�
		USART_Cmd(UART5, ENABLE); 
}



int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	USART_ClearFlag(UART4,USART_FLAG_TC);
  USART_SendData(UART4, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET)
  {}
  return ch;
}


static void Usart_SendByte( USART_TypeDef * pUSARTx, char ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}


/*****
**
**function :use to 485 ctrl voice
**
************/
void Usart_SendString485(USART_TypeDef * pUSARTx, char *str,u8 len)
{

	unsigned int k = len;	
	//MBSerialEnable(FALSE,TRUE);//ʹ�ܷ����ж�
	do
	{
		 Usart_SendByte( pUSARTx, *(str++) );
		 k--;
	}while(k!=0);
}
/*****************  ����ָ�������ַ��� **********************/
void my_sendstring( USART_TypeDef * pUSARTx, char *str,u16 len)
{
	u16 my_k=0;
	my_k =len;
	do
	{
		 Usart_SendByte( pUSARTx, *(str++) );
		 my_k--;
	}while(my_k!=0);

}

/****************************************************************/
//void OverTimeFlagAdd(void)
//{
//		if(UAT2OverTime)
//		{
//				UAT2OverTime++;
//		}
//		if(UAT3OverTime)
//		{
//				UAT3OverTime++;
//		}
//		if(UAT4OverTime)
//		{
//				UAT4OverTime++;
//		}
//}

//void myUsartOverTimeDeal(void)
//{

//			if(UAT2OverTime > OverTime)
//			{
//					UAT2OverTime = 0;
//					Count2=0;
//				  memset(RxBuff2,0,RxBuff2Len-2);
//			}

//			if(UAT3OverTime > OverTime)
//			{
//					UAT3OverTime = 0;
//					Count3=0;
//				  memset(RxBuff3,0,RxBuff3Len-2);
//			}
//			
//			if(UAT4OverTime > OverTime)
//			{
//					UAT4OverTime = 0;
//					Count4=0;
//				  memset(RxBuff4,0,RxBuff4Len-2);
//			}
//}











