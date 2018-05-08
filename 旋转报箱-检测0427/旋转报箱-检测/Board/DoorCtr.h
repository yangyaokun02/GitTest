
#ifndef _DOOR_CTR_H
#define _DOOR_CTR_H


#include "stm32f10x.h" 



#define ON  0
#define OFF 1
#define M1_OPN 1 
#define M1_CLS 2
//#define M2_OPN 3
//#define M2_CLS 4
#define MY_CLEAR  0
#define MY_FLAG   1
#define DoorOverTime   3000 





/* 带参宏，可以像内联函数一样使用 */
#define LOCK_1(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_9);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_9)

					
#define	  M1_OPN_TQ	   	 GPIO_ResetBits(GPIOD, GPIO_Pin_12)//机电器1--6
#define 	M1_OPN_XH	   	 GPIO_SetBits(GPIOD, GPIO_Pin_12) 
#define 	M1_CLS_TQ	   	 GPIO_ResetBits(GPIOD, GPIO_Pin_13)
#define 	M1_CLS_XH	   	 GPIO_SetBits(GPIOD, GPIO_Pin_13) 					
					
	



void  doorGetMsg(void);					
//void  doorCountAdd(void);		
void  doorCtrFunc(void);				
void  doorGetGpio(void);
void  doorRelayCtr(void); 					
void   startCheckDoor(void);					
					
void  DoorAndRelay(void);					
					
					
#endif




