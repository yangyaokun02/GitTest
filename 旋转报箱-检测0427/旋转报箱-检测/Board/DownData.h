#ifndef _DOWNDATA_INIT
#define _DOWNDATA_INIT

#include "stm32f10x.h"
#include "misc.h"


#define    Rx3_Length   100

struct  downstruct
{
		unsigned  char   downDataBuff[Rx3_Length];  //需要发送的buff
		unsigned  char   downDataCount;             //需要发送的长度
		unsigned  char   downOpenFlag;              //0 出货成功  1 改货道无货或者故障 2其他原因没有出货 3 报文不对 4 支付失败
//		unsigned  short  signInTime;                //放间隔时间  ms
		unsigned  short  openTime;								  //发送驱动 超时时间    7000ms
		unsigned  short  openTime200;               //200ms 发送一次 获取运行状态
		unsigned  char   boardID;                   //货柜编号
		unsigned  char   cargoID;                   //货道编号
		unsigned  int    cargoMoney;                //价格
		unsigned  char   cargoWay;									//支付方式
		unsigned  int    cargoNumber;								//流水号
		unsigned  char   openFlag;                  //弹簧电机工作成功标记  1表示成功
	
	
	
	
//		unsigned  char   lastBoardId;
//		unsigned  char   lastboxNum;
//		unsigned  int    lastMoney;
//		unsigned  char   lastWay;
//		unsigned  char   lastRet;
//		unsigned  int    lastNumber;
};


extern  struct downstruct  myDownData;



/*************************函数***************************/
/*
	crc-16 校验

功能：   crc校验
pcMess:  待求数据的首指针
wLen:    带球数据的长度
*/


unsigned int crcVal(unsigned char *pcMess,unsigned int wLen);

void   Down_SendOpen(unsigned char * strData);




/*
	发送获取电机状态 如果超时 反馈工控机

	 从发送启动电机 5s 后开始发送，200ms 发送一次 直到700ms   7000ms 超时
	z1  0x01 空闲  0x01 执行中  0x02 执行完毕
	z2  正在运行的电机编号 00-59
	z3  执行结果 0 成功 1 过流 2 欠流  3 超流 4 光幕自检失败，未启动电机 5 有反馈电磁铁门未打开
	z8-z9 运行时间
	z10 光幕状态
*/

void   Down_GetStatus(void);




/*
		vmc和驱动板通讯
		接收驱动板发出的数据，解析数据是做什么的
		指令：主机-> 从机
    从机地址（1byte）  指令 （1byte）   数据（16byte） 校验（2byte）
		相应：从机->主机
		主机地址（0x0）    指令 （1byte）   数据（16byte） 校验（2byte）
		
*/

void   Down_DataParse(void);

#endif













