#ifndef _MY_USAER_H
#define _MY_USAER_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#define Baud		115200    //更改波特率可以在这里进行


void USART_Config(void);
	


#define   dataLength   100    //串口接收数据的长度


struct usartData
{
		unsigned  char  data[dataLength];  //接收数据buff
		unsigned  char  count;             //计数
		unsigned  char  flag;              //接收完毕标志
		unsigned  char  overtime;          //超时
		unsigned  char  acktime;					 //备用acktime
};



extern struct usartData   usart2_Up;   //和安卓机通讯串口2
extern struct usartData   usart3_Down; //和驱动板通讯串口3
extern struct usartData   uart4_Door;  //接收开箱指令




void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void USART_Config(void);
void OverTimeFlagAdd(void);	
void myUsartOverTimeDeal(void);

void Usart_SendString485(USART_TypeDef * pUSARTx, char *str,u8 len);
void my_sendstring( USART_TypeDef * pUSARTx, char *str,u16 len);

#endif













