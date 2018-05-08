#ifndef _MY_USAER_H
#define _MY_USAER_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#define Baud		115200    //���Ĳ����ʿ������������


void USART_Config(void);
	


#define   dataLength   100    //���ڽ������ݵĳ���


struct usartData
{
		unsigned  char  data[dataLength];  //��������buff
		unsigned  char  count;             //����
		unsigned  char  flag;              //������ϱ�־
		unsigned  char  overtime;          //��ʱ
		unsigned  char  acktime;					 //����acktime
};



extern struct usartData   usart2_Up;   //�Ͱ�׿��ͨѶ����2
extern struct usartData   usart3_Down; //��������ͨѶ����3
extern struct usartData   uart4_Door;  //���տ���ָ��




void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void USART_Config(void);
void OverTimeFlagAdd(void);	
void myUsartOverTimeDeal(void);

void Usart_SendString485(USART_TypeDef * pUSARTx, char *str,u8 len);
void my_sendstring( USART_TypeDef * pUSARTx, char *str,u16 len);

#endif













