#ifndef _UPDATA_INIT
#define _UPDATA_INIT

#include "stm32f10x.h"
#include "misc.h"

#define    dataLength1   410

void  debugOut(char * str);

void   Up_PollAck(u8 whatdo);


struct  upstruct
{
		unsigned  char   upDataBuff[dataLength1]; //需要发送的buff
		unsigned  char   upDataCount;             //需要发送的长度
		unsigned  char   signInFlag;              //发送签到的指令，0x00  发送， 0x01收到ack
		unsigned  short  signInTime;              //放间隔时间  ms
		unsigned  char   signInFrist;             //第一次签到 需要轮询发送后面的报文，发送完毕一次后，就不用轮询发送了
	/*******************************POLL*******************************************************/
		unsigned  short  pollTime;								//300ms发送一次poll指令
		unsigned  char   pollFlag;                //发送指令后 1   ，收到指令 置为0  
	/**********************************出货反馈指令*********************************************/
		unsigned  char   outFlag;                 //出货反馈指令发送标记 0x01 表示发送了 0x02 表示收到了
		unsigned  short  outTime;             		//发送间隔
		unsigned  char   lastBoardId;
		unsigned  char   lastboxNum;
		unsigned  int    lastMoney;
		unsigned  char   lastWay;
		unsigned  char   lastRet;
		unsigned  int    lastNumber;	
	/****************************************auto time*****************************************************/
		unsigned  short   autoTime;     //200ms  发送一次
		unsigned  char    autoNum;      //连续发送的次数，也就是没有收到ack  如果30次没有收到，则重新签到
	/****************************************bin connection msg***************************************/
		unsigned  char    binConFlag;   // 0x01 表示发送箱柜信息，并接收到了ack
		unsigned  short   binConTime;   // 500ms  发送一次
		unsigned  char    binConBuff[7]; //货柜连接信息 0-2  货柜是否支持设价到vmc 3-5
	/****************************************bin set msg***************************************/	
		unsigned  char    binSetFlag;   // 0x01   表示发送了箱柜设置信息，并接收到了ack 
		unsigned  short   binSetTime;   // 500ms  发送一次  
	/****************************************bin price***************************************/	
		unsigned  char    binPreicFlag;   // 0x01  表示发送了货道价格信息，并接收到了ack	
		unsigned  short   binPreicTime;   //500ms  发送一次
	/****************************************bin fault***************************************/		
		unsigned  char    binFaultFlag;  //0x01 表示发送了故障信息 并接收到了ack 
		unsigned  short   binFaultTime;  //500ms 发送一次
	/****************************************run msg***************************************/	
		unsigned  char    runMsgFlag;        // 0x01 表示发送了设备运行信息，并接收到了ack
		unsigned  short   runMsgTime;        //如果没有收到ack 500ms 发送一次
		unsigned  char    runMsgPollTime;    //30s 各个周期
	/****************************************sys***************************************/		
		unsigned  char    sysSetFlag;     //0x01  表示发送了 系统配置信息，并接收到了ack
		unsigned  short   sysSetTime;     //500ms 发送一次
	/****************************************pos***************************************/
		unsigned  char    posMsgFlag;
	/***************************************优惠信息***************************************/	
		unsigned  char    PreFlag;      
		unsigned  short   PreTime;   
	//机器运行信息
		unsigned  char    DoorStatus; //0x
};


extern struct upstruct   myUpData;
/***********************函数***************************/

void  Up_SignIn(void);
/*
			自动补发签到报文
			1S 发送一次，知道收到ack
*/

u8  Up_autoSignIn(void);

/*
		 收到签到的ack 或者 nak

*/
void  Up_SignInAck(void);



/******************************************POLL指令***********************************************/
/*
签到协议

数据长度  指令    0x76
					特征    0xef 0xee 0xfe
					数据    19 byte
					和校验  1 byte
在签到成功的前提下
该函数 300ms 发送一次报文，

ack        指令
数据       0x01  (0x15  nck)
校验和			
*/
u8 Up_Poll(void);





/******************************************出货指令***********************************************/

/*
出货指令  反馈出货结果

数据长度  指令    0x7C
					特征    0xef 0xee 0xfe
					数据    11 byte
					和校验  1 byte

ack        指令
数据       0x01  (0x15  nck)
校验和
boarid  箱柜id
boxnum  格口号
mymoney  售卖金额  分为单位
way  支付方式
number  流水号，

*/

void   Up_OutStatus(unsigned char boardId,unsigned char boxNum,unsigned int mymoney ,unsigned char way,unsigned char ret,unsigned int number);













/*
	 串口2接收到数据，对数据进行分析
	有ack  

*/
void  Up_DataParse(void);


/*
		如果100ms没有指令没有收到工控机的ack 则自动补发
		最后执行poll 如果有其他指令发送了，则不发送poll指令
*/
void  Up_AllCmdAuto(void);







#endif













