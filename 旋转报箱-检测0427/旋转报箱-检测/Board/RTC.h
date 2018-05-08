#ifndef __RTC_H
#define __RTC_H
#include "stm32f10x.h"

struct tm
{
       u8 hour;
       u8 min;
       u8 sec;                  
       //公历日月年周
       u16 w_year;
       u8  w_month;
       u8  w_date;
       u8  week;             
}; 

extern struct tm timer;

//获得现在是星期几

//功能描述:输入公历日期得到星期(只允许1901-2099年)

//输入参数：公历年月日 

//返回值：星期号                                                                                                                                                     
u8 RTC_Get_Week(u16 year,u8 month,u8 day);

void RTC_Config(void);

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year);

//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表                                                                        
//u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表   
//平年的月份日期表
//const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);


//得到当前的时间
//返回值:0,成功;其他:错误代码.
u8 RTC_Get(void);

//获得现在是星期几

//功能描述:输入公历日期得到星期(只允许1901-2099年)

//输入参数：公历年月日 

//返回值：星期号                                                                                                                                                     
u8 RTC_Get_Week(u16 year,u8 month,u8 day);


#endif
