#ifndef KDG_H
#define KDG_H
#include "stm32f10x.h" 


void USART_printf ( USART_TypeDef * USARTx, char * Data, ... );
void  RetCmdNum(void);
void Kdg_Poll(void);

#endif




