#include "RTC.h"

struct tm timer =
{
			0x00,   //hour
			0x00,		//min
			0x00,		//sec
			0x0000,	//year
			0x00,   //month
			0x00,		//date
			0x00,   //week
};


void RTC_Config(void)
{
     u16 u16_WaitForOscSource;
     //������BKP�ĺ󱸼Ĵ���1�У�����һ�������ַ�0xA5A5
    //��һ���ϵ��󱸵�Դ����󣬸üĴ������ݶ�ʧ��
    //����RTC���ݶ�ʧ����Ҫ��������
    if (BKP_ReadBackupRegister(BKP_DR1) != 0x5A5A)
    {
        //��������RTC
        /* Enable PWR and BKP clocks */
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
      /* Allow access to BKP Domain */
      PWR_BackupAccessCmd(ENABLE);
      /* Reset Backup Domain */
       BKP_DeInit(); 
      /* Enable LSE */
      RCC_LSEConfig(RCC_LSE_ON);
       for(u16_WaitForOscSource=0;u16_WaitForOscSource<5000;u16_WaitForOscSource++)
           {
           }
      /* Wait till LSE is ready */
      while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
      /* Select LSE as RTC Clock Source */
      RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
      /* Enable RTC Clock */
      RCC_RTCCLKCmd(ENABLE);
      /* Wait for RTC registers synchronization */
      RTC_WaitForSynchro();
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
      /* Enable the RTC Second */
      RTC_ITConfig(RTC_IT_SEC, ENABLE);
      /* Wait until last write operation on RTC registers has finished */
       RTC_WaitForLastTask();
      /* Set RTC prescaler: set RTC period to 1sec */
      RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
        //������ɺ���󱸼Ĵ�����д�����ַ�0xA5A5
      BKP_WriteBackupRegister(BKP_DR1, 0x5A5A);
      RTC_Set(2017,07,07,12,12,50);//Ĭ��ʱ��
    }
    else
    {
        //���󱸼Ĵ���û�е��磬��������������RTC
        //�������ǿ�������RCC_GetFlagStatus()�����鿴���θ�λ����
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
      for(u16_WaitForOscSource=0;u16_WaitForOscSource<5000;u16_WaitForOscSource++);
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            //�����ϵ縴λ
        }
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            //�����ⲿRST�ܽŸ�λ
        }
        //���RCC�и�λ��־
        RCC_ClearFlag();
        //��ȻRTCģ�鲻��Ҫ�������ã��ҵ���������󱸵����Ȼ����
        //����ÿ���ϵ�󣬻���Ҫʹ��RTCCLK???????
        //RCC_RTCCLKCmd(ENABLE);
        //�ȴ�RTCʱ����APB1ʱ��ͬ��
        //RTC_WaitForSynchro();
        //ʹ�����ж�
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        //�ȴ��������
        RTC_WaitForLastTask();
    }
    //return;
}
//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{                     
       if(year%4==0) //�����ܱ�4����
       { 
              if(year%100==0) 
              { 
			  if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400����          
                     else return 0;   
              }else return 1;   
       }else return 0; 
}                           
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�                                                                        
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�   
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{

       u16 t;
       u32 seccount=0;
       if(syear<2000||syear>2099)return 1;//syear��Χ1970-2099���˴����÷�ΧΪ2000-2099        
       for(t=1970;t<syear;t++) //��������ݵ��������
       {
              if(Is_Leap_Year(t))seccount+=31622400;//�����������
              else seccount+=31536000;                    //ƽ���������
       }
       smon-=1;
       for(t=0;t<smon;t++)         //��ǰ���·ݵ����������
       {
              seccount+=(u32)mon_table[t]*86400;//�·����������
              if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������         
       }
       seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
       seccount+=(u32)hour*3600;//Сʱ������
	     seccount+=(u32)min*60;      //����������
       seccount+=sec;//�������Ӽ���ȥ                                                                                           
       //����ʱ��
     /*RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ��
       RCC->APB1ENR|=1<<27;//ʹ�ܱ���ʱ��
       PWR->CR|=1<<8;    //ȡ��������д����
       //���������Ǳ����!*/
       PWR_BackupAccessCmd(ENABLE);
       RTC_WaitForLastTask();
       RTC_SetCounter(seccount);
       RTC_WaitForLastTask();
       return 0;      
}
//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Get(void)
{
       static u16 daycnt=0;
       u32 timecount=0; 
       u32 temp=0;
       u16 temp1=0;
       timecount=RTC_GetCounter();       
       /*timecount=RTC->CNTH;//�õ��������е�ֵ(������)
       timecount<<=16;
       timecount+=RTC->CNTL;    */           
       temp=timecount/86400;   //�õ�����(��������Ӧ��)
       if(daycnt!=temp)//����һ����
       {       
              daycnt=temp;
              temp1=1970;  //��1970�꿪ʼ
              while(temp>=365)
              {                          
                     if(Is_Leap_Year(temp1))//������
                     {
                           if(temp>=366)temp-=366;//�����������
                            else {temp1++;break;}  
                     }
                     else temp-=365;       //ƽ�� 
                     temp1++;  
              }   
              timer.w_year=temp1;//�õ����
              temp1=0;
              while(temp>=28)//������һ����
              {
                    if(Is_Leap_Year(timer.w_year)&&temp1==1)//�����ǲ�������/2�·�
                     {
                            if(temp>=29)temp-=29;//�����������
                            else break; 
                     }
                     else 
                     {
                            if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
                            else break;
                     }
                    temp1++;  
              }
              timer.w_month=temp1+1;//�õ��·�
              timer.w_date=temp+1;  //�õ����� 
       }
       temp=timecount%86400;     //�õ�������      
       timer.hour=temp/3600;     //Сʱ
       timer.min=(temp%3600)/60; //����      
       timer.sec=(temp%3600)%60; //����
       timer.week=RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);//��ȡ����   
       return 0;
}     

//������������ڼ�

//��������:���빫�����ڵõ�����(ֻ����1901-2099��)

//������������������� 

//����ֵ�����ں�                                                                                                                                                     
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{
       u16 temp2;
       u8 yearH,yearL;
       yearH=year/100;     
			 yearL=year%100; 
       // ���Ϊ21����,�������100  
       if (yearH>19)yearL+=100;
       // ����������ֻ��1900��֮���  
       temp2=yearL+yearL/4;
       temp2=temp2%7; 
       temp2=temp2+day+table_week[month-1];
       if (yearL%4==0&&month<3)temp2--;
       return(temp2%7);
}



void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);                      //������ж�

    /* Toggle LED1 */
   // STM_EVAL_LEDToggle(LED1);																//DS1����ʾ��ת��ÿ��

    /* Enable time update */
   // TimeDisplay = 1;                                        //�������ݱ�־λ��ÿ�����

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();   
  }
}





























































