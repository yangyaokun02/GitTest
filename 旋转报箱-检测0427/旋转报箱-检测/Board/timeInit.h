#ifndef _TIME_INIT
#define _TIME_INIT

#include "stm32f10x.h"
#include "misc.h"

//void NVIC_Configuration(void);
void TIM2_Configuration(void);
void TIM3_Configuration(void);
void TimEnale(u8 flag, unsigned short  int Period);
#endif













