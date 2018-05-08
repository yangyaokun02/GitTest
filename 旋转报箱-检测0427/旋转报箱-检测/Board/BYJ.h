
#ifndef _BYJ_CTR_H
#define _BYJ_CTR_H


#include "stm32f10x.h" 


struct  gpiostatus
{
		u8  door1s;
		u8  door2s;
		u8  door3s;
		u8  door4s;
		u8  door5s;
		u8  door6s;
		u8  door7s;
		u8  door8s;	
		u8  door9s;	
		u8  sensor;  //			
};


extern struct  gpiostatus  status;

extern volatile  int   pwmNum;       //脉冲个数
extern u8  pwmNumFlag;   
//extern u8  tim3flag; //

extern  int curPosition; //当前位置
extern  int checkNum ;  //校准
//extern  u8   checkFlag;

extern int  sMaiChong;      //输出脉冲数
extern int  zhouqi;         //实际脉冲周期 pwm
 /********************TIM参数定义，只限TIM2、3、4或5**************************/
#define             macTIMx                                TIM4
#define             macTIM_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macTIM_CLK                             RCC_APB1Periph_TIM4
#define             macTIM_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macTIM_GPIO_CLK                        ( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB )
#define             macTIM_CH4_PORT                        GPIOB
#define             macTIM_CH4_PIN                         GPIO_Pin_9


//#define             BYJ_ENABLE                             GPIO_ResetBits(GPIOB,GPIO_Pin_14);
//#define             BYJ_DISABLE													   GPIO_SetBits(GPIOB,GPIO_Pin_14);

//#define             BYJ_RIGHT															 GPIO_SetBits(GPIOB,GPIO_Pin_13); 
//#define             BYJ_LEFT															 GPIO_ResetBits(GPIOB,GPIO_Pin_13);



#define BYJDIRECT(a)	if (a)	\
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);	\
					else		\
					GPIO_SetBits(GPIOA,GPIO_Pin_6)



/************************** TIM 函数声明********************************/


void 	  TIMx_PWM_Init(void);
void    Byj_Start( u8  whichDirection,u16 ByjPeriod);
void    allGpioInit(void); 
void    addMoveByj(void);	
void    Byj_cmd(u8 FangXiang, int JuLi);		
void    Byj_GoTo(u16 DoorNum);
void    Byj_usart(void);

extern  u16 CloseFlag;					
extern  u16 OpenFlag; 
void    OpenDoor(u16 num);				
	void Byj_Poll(void);				
					
					
#endif




