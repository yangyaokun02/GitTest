#include   "UpData.h"
#include   "string.h"
#include   "myUsart.h"
#include   "DownData.h"
#include   "RTC.h"



struct  upstruct   myUpData;



void  debugOut(char * str)
{
		u8 templen =0;
		templen = strlen(str);
		my_sendstring(UART4,str,templen);		
//	my_sendstring(UART5,str,templen);		
}




/***********************************************/
char *itoa(int val, char *buf, unsigned radix)
{
    char   *p;             
    char   *firstdig;      
    char   temp;           
    unsigned   digval; 
		char myflag=0;
    p = buf;
    if(val <0)
    {
        *p++ = '-';
        val = (unsigned long)(-(long)val);
    }
    firstdig = p; 
		if(radix == 10)
		{
						//if(val > 0 && val<10)
						if(val<10)
						{
							myflag =1;
						}
		}
		else if(radix ==16)
		{
					//	if(val > 0 && val<16)
						if(val<16)
						{
							myflag =1;
						}					
		}		
    do{
        digval = (unsigned)(val % radix);
        val /= radix;
        if  (digval > 9)
            *p++ = (char)(digval - 10 + 'a'); 
        else
            *p++ = (char)(digval + '0');      
    }while(val > 0);
    *p-- = '\0';         
    do{
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;
        --p;
        ++firstdig;        
    }while(firstdig < p);  
		
		if(myflag)
		{
			myflag =0;
			buf[1]=buf[0];
			buf[0]='0';
			buf[2]='\0';
		}	
    return buf;
}

/*
接收安卓端的数据


*/

/******************************************签到协议***********************************************/
/*
签到协议

数据长度  指令    0x78
					特征    0xef 0xee 0xfe
					数据    50 byte
					和校验  1 byte
该函数获取所有签到需要的数据，并使用串口发送报文， 如果没有收到ack 1S后继续发送 

ack        指令
数据       0x01  (0x15  nck)
校验和			
*/

u16 Up_tempYear =0;
char  Up_tiembuff[10] ={0};
void  Up_SignIn(void)
{
	
	u8  i =0;
	 memset(myUpData.upDataBuff,0,dataLength1-1);
	RTC_Get();
	//指令
	myUpData.upDataBuff[0] = 0x78;
	//特征
	myUpData.upDataBuff[1] = 0xef;
	myUpData.upDataBuff[2] = 0xee;
	myUpData.upDataBuff[3] = 0xfe;
	//数据
	//主控板版本号 0xff 0xff
	myUpData.upDataBuff[4] = 0x03;
	myUpData.upDataBuff[5] = 0x00;
	//驱动板版本号
	myUpData.upDataBuff[6] = 0x01;
	myUpData.upDataBuff[7] = 0x00;
	//vmc协议版本号 0xff
	myUpData.upDataBuff[8] = 0x02;
	//售货机编号 0xff 0xff 0xff 0xff
	myUpData.upDataBuff[9]  = 0x12;
	myUpData.upDataBuff[10] = 0x34;
	myUpData.upDataBuff[11] = 0x56;
	myUpData.upDataBuff[12] = 0x78;
	//时间 bcd yyyy mm dd hh mm ss
	Up_tempYear = timer.w_year;       //0x07E1
	itoa(timer.w_year,Up_tiembuff,10);
	  myUpData.upDataBuff[13] = (Up_tiembuff[0]<<4) + (Up_tiembuff[1]&0x0F); 
	  myUpData.upDataBuff[14] = (Up_tiembuff[2]<<4) + (Up_tiembuff[3]&0x0F); 	
	itoa(timer.w_month,Up_tiembuff,10);
		myUpData.upDataBuff[15]  = (Up_tiembuff[0]<<4) + (Up_tiembuff[1]&0x0F); 
	itoa(timer.w_date,Up_tiembuff,10);
		myUpData.upDataBuff[16]  = (Up_tiembuff[0]<<4) + (Up_tiembuff[1]&0x0F); //0x14 --0x20	
	itoa(timer.hour,Up_tiembuff,10);
		myUpData.upDataBuff[17]  = (Up_tiembuff[0]<<4) + (Up_tiembuff[1]&0x0F); //0x14 --0x20		
	itoa(timer.min,Up_tiembuff,10);
		myUpData.upDataBuff[18]  = (Up_tiembuff[0]<<4) + (Up_tiembuff[1]&0x0F); //0x14 --0x20
	itoa(timer.sec,Up_tiembuff,10);
		myUpData.upDataBuff[19]  = (Up_tiembuff[0]<<4) + (Up_tiembuff[1]&0x0F); //0x14 --0x20
	//主柜类型 0x01 饮料机 0x02 综合机， 0x03 格子机
	myUpData.upDataBuff[20]  = 0x02;
	//主柜货道数
	myUpData.upDataBuff[21]  = 0x14;      //这个指什么？
	//16个扩展柜信息
	myUpData.upDataBuff[22]  = 0x00;      //1   类型
	myUpData.upDataBuff[23]  = 0x00;      //货道数量
	
	myUpData.upDataBuff[24]  = 0x00;  //2
	myUpData.upDataBuff[25]  = 0x00;
	 
	myUpData.upDataBuff[26]  = 0x00;  //3
	myUpData.upDataBuff[27]  = 0x00;
	
	myUpData.upDataBuff[28]  = 0x00;  //4
	myUpData.upDataBuff[29]  = 0x00;
	
	myUpData.upDataBuff[30]  = 0x00;  //5
	myUpData.upDataBuff[31]  = 0x00;
	
	myUpData.upDataBuff[32]  = 0x00;  //6
	myUpData.upDataBuff[33]  = 0x00;
	
	myUpData.upDataBuff[34]  = 0x00;  //7
	myUpData.upDataBuff[35]  = 0x00;
	
	myUpData.upDataBuff[36]  = 0x00;  //8
	myUpData.upDataBuff[37]  = 0x00;
	
	myUpData.upDataBuff[38]  = 0x00;  //9
	myUpData.upDataBuff[39]  = 0x00;
	
	myUpData.upDataBuff[40]  = 0x00;  //10
	myUpData.upDataBuff[41]  = 0x00;
	
	myUpData.upDataBuff[42]  = 0x00;  //11
	myUpData.upDataBuff[43]  = 0x00;
	
	myUpData.upDataBuff[44]  = 0x00;  //12
	myUpData.upDataBuff[45]  = 0x00;
	
	myUpData.upDataBuff[46]  = 0x00;  //13
	myUpData.upDataBuff[47]  = 0x00;
	
	myUpData.upDataBuff[48]  = 0x00;  //14
	myUpData.upDataBuff[49]  = 0x00;
	
	myUpData.upDataBuff[50]  = 0x00;  //15
	myUpData.upDataBuff[51]  = 0x00;
	
	myUpData.upDataBuff[52]  = 0x00;  //16
	myUpData.upDataBuff[53]  = 0x00;
	
	//校验和
	for(i=0;i< 54; i++)
	{
		 myUpData.upDataBuff[54] +=  myUpData.upDataBuff[i]; 
	}
	
	//send data
	my_sendstring(USART2,(char *)myUpData.upDataBuff,55);	
	myUpData.signInFlag = 0;   //记录发送了签到协议 但未首单ack  如果等于1 表示收到ack
	myUpData.signInTime = 0;
	
}


/*
			自动补发签到报文
			1S 发送一次，知道收到ack
return 0x00  已经签到       0x01 发送等待接收ack  0x02  间隔时间没有到
*/

u8  Up_autoSignIn(void)
{
			if(!myUpData.signInFlag)
			{
					if(myUpData.signInTime > 1000) //1000ms 
					{
							debugOut("sendSignIn\r\n");
							myUpData.signInTime =0;
							Up_SignIn();
						return 1;
					}
					return 2;
			}
			else
			{		
			return 0;				
			}
}



/*
		 收到签到的ack 或者 nak

*/
void  Up_SignInAck(void)
{
			if(usart2_Up.data[1]==0x00) //ack
			{
					myUpData.autoNum = 0;  
					myUpData.signInFlag = 1;   //
					myUpData.signInTime = 0;
					myUpData.binConTime = 1;   //计时500ms 发送货柜连接信息
				
					myUpData.binConFlag = 0;     //副柜连接信息
					myUpData.binSetFlag = 0;    
					myUpData.binPreicFlag = 0;
					myUpData.binFaultFlag = 0;
					myUpData.runMsgFlag = 0;
					myUpData.sysSetFlag = 0;	
				
				//myUpData.runMsgTime  =1; //test
				  debugOut("ackSingIn\r\n");  
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.signInFlag = 0;   //记录发送了签到协议
					myUpData.signInTime =0;	
				 debugOut("NckSingIn\r\n");  
			}
			else
			{
					myUpData.signInFlag = 0;   //记录发送了签到协议
					myUpData.signInTime =0;	
				 debugOut("WTFSingIn\r\n");  
			}

}


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
执行返回0x00 收到ack
*/
u8  Poll_check=0;
u8 Up_Poll(void)
{
	if(myUpData.signInFlag) //签到成功
	{
				if(myUpData.pollTime > 300)
				{
						myUpData.pollTime =0;
					
					 memset(myUpData.upDataBuff,0,dataLength1-1);
						//指令
						myUpData.upDataBuff[0] = 0x76;
						//特征
						myUpData.upDataBuff[1] = 0xef;
						myUpData.upDataBuff[2] = 0xee;
						myUpData.upDataBuff[3] = 0xfe;
						//数据
						//用户投入的现金金额 分为单位12byte
					  //getUserInputCash();
						myUpData.upDataBuff[4] = 0x00;   //接收总金额 
						myUpData.upDataBuff[5] = 0x00;
						myUpData.upDataBuff[6] = 0x00;
						myUpData.upDataBuff[7] = 0x00;
					   
						myUpData.upDataBuff[8] = 0x00;   //pos 接收金额
						myUpData.upDataBuff[9]  = 0x00;
						myUpData.upDataBuff[10] = 0x00;
						myUpData.upDataBuff[11] = 0x00;
						
						myUpData.upDataBuff[12] = 0x00;  //现金金额接收
						myUpData.upDataBuff[13] = 0x00;
						myUpData.upDataBuff[14] =  0x00;
						myUpData.upDataBuff[15] =  0x00;
					//键盘输入货道 6byte
					//getUserAisle();
						myUpData.upDataBuff[16] =  0x00; //货柜编号 0x00 主柜
						myUpData.upDataBuff[17] =  0x00; //货道编号，如果没有上传0
						myUpData.upDataBuff[18] =  0x00; //商品价格
						myUpData.upDataBuff[19] =  0x00;
						myUpData.upDataBuff[20]  = 0x00;
						myUpData.upDataBuff[21]  = 0x00;
					//键盘输入productID 1byte
					//getProductID();
						myUpData.upDataBuff[22]  = 0x00;						
						
					//求和校验
					for(Poll_check =0; Poll_check <23; Poll_check++ )
					{								
						myUpData.upDataBuff[23] += myUpData.upDataBuff[Poll_check];
					}	
					//send data
					my_sendstring(USART2,(char *)myUpData.upDataBuff,24);	
          //记录发送数据
				//	debugOut("Poll\r\n");
					if(myUpData.pollFlag)  //没有收到ack
					{
						myUpData.pollFlag = 1;  //发送置为1 收到ack置为 0
						return 0x01;
					}
					else
					{
						myUpData.pollFlag = 1; 
						return 0x00;	
					}
				}	
	}	
	return 0x00;
}



/*
		whatdo 0x01 ack    0x00 nck   
*/
unsigned char poll_ack[5] = {0x00,0x00,0x00,0x00}; //回复ack  nck

void   Up_PollAck(u8 whatdo) 
{
		memset(poll_ack,0,3);
		poll_ack[0] = 0x76;
		if(whatdo)
		{
			poll_ack[1] = 0x00;	
			poll_ack[2] = 0x76;
		}
		else
		{
			poll_ack[1] = 0x15;	
			poll_ack[2] = 0x8B;
		}	
		my_sendstring(USART2,(char *)poll_ack,3);		
}	

/*
	设置时间  
	76 02 20 17 08 02 10 25 55  和校验
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
*/
void  Up_PollSetTime(void)
{
		u16 tempYear =0;
		u8  tempMon =0,tempDay =0,tempHour=0,tempMin =0,tempSec=0; 
		
		tempYear = usart2_Up.data[2];                    //0x2017
		tempYear = (tempYear <<8) + usart2_Up.data[3];
		//0x2017 --> 2017
		tempYear = ((tempYear & 0xF000)>>12)*1000	+ ((tempYear & 0x0F00)>>8)*100 + ((tempYear & 0x00F0)>>4)*10+(tempYear & 0x000F);  
	
		tempMon  = usart2_Up.data[4];                    //0x08
		  tempMon  = ((tempMon & 0xF0)>>4)*10+(tempMon & 0x0F);  //0x08 -- 08
		tempDay  = usart2_Up.data[5];                    //0x02
		  tempDay  = ((tempDay & 0xF0)>>4)*10+(tempDay & 0x0F);
		tempHour  = usart2_Up.data[6];                    //0x10
		  tempHour  = ((tempHour & 0xF0)>>4)*10+(tempHour & 0x0F);
		tempMin  = usart2_Up.data[7];                    //0x25
		  tempMin  = ((tempMin & 0xF0)>>4)*10+(tempMin & 0x0F);
		tempSec  = usart2_Up.data[8];                    //0x55
		  tempSec  = ((tempSec & 0xF0)>>4)*10+(tempSec & 0x0F);
	
	 RTC_Set(tempYear,tempMon,tempDay,tempHour,tempMin,tempSec);
		//debugOut();
}

/*
		

*/





/*
发送开箱指令给 驱动板， 串口3  3线电机 无光幕
*/

/*
			poll ack处理    有普通ack  nak  带有配置信息的 ack
			
			出货请求，

		  收到poll ack 指令
*/
void   	Up_PollParse(void)
{		
			memset(myUpData.upDataBuff,0,dataLength1-1);
			myUpData.pollFlag =0;     //记录收到ack
			switch (usart2_Up.data[1])
			{
				case 0x00:    //ack
					break;
//				case 0x15:    //nak
//					break;
				case 0x01:    //ack
					//Up_PollAck(1);
					break;
				case  0x02:   //设置系统时间
						Up_PollAck(1);	
						Up_PollSetTime();
						debugOut("PollTimeSet\r\n");
					return;
				case  0x03:   //出货请求
					  Down_SendOpen(usart2_Up.data);
						debugOut("PollOutGoods\r\n");
					return;
				case  0x04:   //设置货道价格信息
					//Up_setPrice();
					debugOut("PollSetPrice\r\n");
					break;
				case  0x05:   //设置系统配置信息
					//Up_setSysMsg();
					debugOut("PollSysSet\r\n");
				  break;
				case  0x06:   //清除销售统计
					//Up_clearStat();
					debugOut("PollClear\r\n");
					break;
				case  0x07:   //获取销售信息
					//Up_getSaleMsg();
				debugOut("PollGetSaleMsg\r\n");
					break;
				case  0x08:   //开始设置货道价格
					//Up_startSetPrice();
					debugOut("PollStartSetPrice\r\n");
					break;
				case  0x09:   //货道价格设置完成
					//Up_doneSetPrice();
					debugOut("PollDoneSetPrice\r\n");
					break;
				case  0x0A:   //设置货道优惠信息
					//Up_setPreferential();
				  debugOut("PollSetPreferent\r\n");
					break; 
				case  0x0B:   //设置交易的发起和结束	
					debugOut("PollStarOrEnd\r\n");
					break;
				default:   //error
					       Up_PollAck(0);	
									debugOut("WTFPoll\r\n");
					return;
			}
			Up_PollAck(1);	
	//		debugOut("ackPoll\r\n");
}




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

void   Up_OutStatus(unsigned char boardId,unsigned char boxNum,unsigned int mymoney ,unsigned char way,unsigned char ret,unsigned int number)
{
		u8 i =0 ;
		
		  memset(myUpData.upDataBuff,0,dataLength1-1);
		
			myUpData.upDataBuff[0]  = 0x7c ; //指令
			myUpData.upDataBuff[1]  = 0xef ; //特征
			myUpData.upDataBuff[2]  = 0xee ;
			myUpData.upDataBuff[3]  = 0xfe ;	
			myUpData.upDataBuff[4]  = boardId ;// 货柜编号
			myUpData.upDataBuff[5]  = boxNum  ; // 货道编号
//这里需要处理金额
//			myUpData.upDataBuff[6]  = 0x7c ;  //售卖金额
//			myUpData.upDataBuff[7]  = 0x7c ;
//			myUpData.upDataBuff[8]  = 0x7c ;
//			myUpData.upDataBuff[9]  = 0x7c ;
			myUpData.upDataBuff[6]  = mymoney >>24;
			myUpData.upDataBuff[7]  = mymoney >>16;
			myUpData.upDataBuff[8]  = mymoney >>8;
			myUpData.upDataBuff[9]  = mymoney ;
	
	
			myUpData.upDataBuff[10] = way ;  //支付方式
			myUpData.upDataBuff[11] = ret ; //出货状态
	//这里需要处理
//			myUpData.upDataBuff[12] = 0x7c ; //流水号
//			myUpData.upDataBuff[13] = 0x7c ;
//			myUpData.upDataBuff[14] = 0x7c ;
			
			myUpData.upDataBuff[12] = (number >> 16);
			myUpData.upDataBuff[13] = (number >> 8);
			myUpData.upDataBuff[14] =  number;
			
	
			for(i=0; i< 15 ; i++)
			{
				myUpData.upDataBuff[15] +=  myUpData.upDataBuff[i] ; //和校验
			}
			
			//记录发送出货请求，指令等待接收ack
			myUpData.outFlag  = 1;   //0x01表示发送了  0x00 表示接收到了ack
			myUpData.outTime  = 1;
//			myUpData.lastBoardId = boardId-1;
//			myUpData.lastboxNum  = boxNum+1;
			myUpData.lastMoney   = mymoney;
			myUpData.lastWay     = way;
			myUpData.lastRet     = ret;
			myUpData.lastNumber  = number;
			my_sendstring(USART2,(char *)myUpData.upDataBuff,16);		
			if(ret) //0 成功
				debugOut("sendOpenFail\r\n");
			else
				debugOut("sendOpenSucces\r\n");			
}

/*

  发送反馈出货报文，300ms 没有收到ack 则重发
	myUpData.outFlag  = 1;  表示发送了 报文 和 签到相反
  myUpData.outTime  > 100;
*/
u8  Up_AutoOutStatus(void)
{
	if(myUpData.outFlag) 
	{
		if(myUpData.outTime > 300) //300ms
		{
				myUpData.outTime =1;			
		    Up_OutStatus(myUpData.lastBoardId,myUpData.lastboxNum,myUpData.lastMoney,myUpData.lastWay,myUpData.lastRet,myUpData.lastNumber);
			  return 0x01;
		}
	}		
	return 0x00;
}



void  Up_OutAck(void)
{
	
			if(usart2_Up.data[1]==0x00) //ack
			{
					myUpData.outFlag = 0;   // 表示收到ack
					myUpData.outTime = 0;	
				debugOut("ackOutStatus\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.outFlag  = 1;   //
					myUpData.outTime  = 1;	
				debugOut("nckOutStatus\r\n");
			}
			else
			{
					myUpData.outFlag  =  1;   //
					myUpData.outTime  =  1;
				debugOut("WTFOutStatus\r\n");
			}
}
/*****************************************货柜连接信息***************************************************/

//获取设备连接信息，myUpData.binConBuff[7],  
void  getBinMsg(void)
{
	
		myUpData.binConBuff[0] = 0x01; //货柜连接信息
		myUpData.binConBuff[1] = 0x00;
		myUpData.binConBuff[2] = 0x00;
	
		myUpData.binConBuff[3] = 0x00; //是否支持设价到vmc
		myUpData.binConBuff[4] = 0x00;
		myUpData.binConBuff[5] = 0x00;
	
		myUpData.binConBuff[6] = 0x00;
}





/*
		货柜链接信息 发送
	指令     特征字               货柜连接信息            vmc设置
	0x74     0xef 0xee 0xfe       byte3                   byte3
	return  0x00 发送了数据收到ack      0x01  发送数据 没有没有收到ack   0x02 时间没有到
*/ 

u8   Up_BinConnectionMsg(void)
{
		 u8   i = 0;

			 if(myUpData.binConFlag == 0)
			 {
				 if(myUpData.binConTime >500)
				 {
					  myUpData.binConTime = 0 ;
						//获取箱柜信息
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x74 ; //指令
						myUpData.upDataBuff[1]  = 0xef ; //特征
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	
						//getBinMsg()
						myUpData.upDataBuff[4]  = myUpData.binConBuff[0] ; //货柜连接信息 
						myUpData.upDataBuff[5]  = myUpData.binConBuff[1] ; 
						myUpData.upDataBuff[6]  = myUpData.binConBuff[2] ;
					 
						myUpData.upDataBuff[7]  = myUpData.binConBuff[3] ; //是否支持设置价格到vmc
						myUpData.upDataBuff[8]  = myUpData.binConBuff[4] ; 
						myUpData.upDataBuff[9]  = myUpData.binConBuff[5] ;
					 
					 for(i =0; i<10;i++)
					 {
							myUpData.upDataBuff[10]  += myUpData.upDataBuff[i];	 
					 }
					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,11);
					 debugOut("sendConnectMsg\r\n");
					 myUpData.binConFlag =0; //表示发送了数据，没有收到ack
					 myUpData.binConTime =1;
					 return 0x01;
				}
				else
				{					
					 return 0x02;
				}
			}
			else
			{		
				return 0x00; 			
			}	 
}	

/*
		接收到ack
*/
void   Up_BinConnectionAck(void)
{
			if(usart2_Up.data[1]== 0x00) //ack
			{		
					myUpData.autoNum =0; 
					myUpData.binConFlag = 1; //表示发送了数据，收到了ack
					myUpData.binConTime = 0;
				
					if(myUpData.signInFrist == 0)
					{
						myUpData.binSetTime = 1; //开始发送货道设置信息				
					}
				  debugOut("ackConnect\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.binConFlag =0; //表示发送了数据，没有收到ack
				  debugOut("NckConnect\r\n");
			}
			else
			{
					myUpData.binConFlag =0; //表示发送了数据，没有收到ack
				  debugOut("WTFConnect\r\n");
			}
}



/*****************************************货道设置信息***************************************************/
/*
		货道设置信息发送
	指令     特征字               货柜编号            货道设置信息
	0x75     0xef 0xee 0xfe       byte1 0-16           byte13
	return  0x00 发送了数据，没有收到ack      0x01  收到了   0x03 其他
	暂时设置一个柜子的 信息 主柜
*/ 


u8   Up_BinSetMsg(void)
{
		u8 i = 0;
		if(myUpData.binSetFlag == 0x00) // 
		{
			if(myUpData.binSetTime > 500)
			{
						myUpData.binSetTime  =0;
				
				    //获取箱柜设置信息
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x75 ; //指令
						myUpData.upDataBuff[1]  = 0xef ; //特征
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //箱柜编号 0x00 主柜
			
						myUpData.upDataBuff[5]   = 0xFF ; //货道设置信息
						myUpData.upDataBuff[6]   = 0xFF ;				 
						myUpData.upDataBuff[7]   = 0xFF ; 
						myUpData.upDataBuff[8]   = 0xFF ; 
						myUpData.upDataBuff[9]   = 0xFF ;
						myUpData.upDataBuff[10]  = 0xFF ;				 
						myUpData.upDataBuff[11]  = 0xFF ; 
						myUpData.upDataBuff[12]  = 0x0F ; 
						myUpData.upDataBuff[13]  = 0x00 ;
						myUpData.upDataBuff[14]  = 0x00 ;				 
						myUpData.upDataBuff[15]  = 0x00 ; 
						myUpData.upDataBuff[16]  = 0x00 ; 
						myUpData.upDataBuff[17]  = 0x00 ;
						
					 for(i =0; i<18;i++)
					 {
							myUpData.upDataBuff[18]  += myUpData.upDataBuff[i];	 
					 }
					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,19);
					 debugOut("sendBinSet\r\n");
					 myUpData.binSetFlag  = 0; //表示发送了数据，没有收到ack
					 myUpData.binSetTime  =1;
					 return 0x01;	
				}
				else
				{
						return 0x02;
				}					
		}
		else
		{
			return 0x00; 		
		}

}


/*
			货道设置信息 接收到ack
*/

void  Up_BinSetAck(void)
{
			if(usart2_Up.data[1]== 0x00) //ack
			{
					myUpData.autoNum =0; 
					myUpData.binSetFlag =1; //表示发送了数据，收到了ack
					myUpData.binSetTime =0;
				
					if(myUpData.signInFrist ==0)
					{
						myUpData.binPreicTime =1;//开始下一条货道价格信息				
					}

				  debugOut("ackBinSet\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.binSetFlag =0; //表示发送了数据，没有收到ack
				  debugOut("nckBinSet\r\n");
			}
			else
			{
					myUpData.binSetFlag =0; //表示发送了数据，没有收到ack
				  debugOut("WTFBinSet\r\n");
			}	
}

/*****************************************货道价格信息***************************************************/
/*
	货道价格信息
	指令     特征字               货柜编号            货道价格信息
	0x7F     0xef 0xee 0xfe       byte1 0-16           byte400
	return  0x00 发送了数据，没有收到ack      0x01  收到了   0x03 其他
	暂时设置一个柜子的 信息 主柜
binPreicFlag
*/ 

u8  Up_BinPrice(void)
{
			 u16 i = 0;
			 if(myUpData.binPreicFlag == 0)
			 {
						
				 if(myUpData.binPreicTime > 500)
				 {
					  myUpData.binPreicTime =0;
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x7F ; //指令
						myUpData.upDataBuff[1]  = 0xef ; //特征
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //货柜编号 0-16
				 
																							//货道价格信息 byte400   暂时设置为0
					 for(i =0; i<400;i++) 
					 {
								myUpData.upDataBuff[i+5] = 0x00; 
					 }
					 
					 for(i =0; i<405;i++)
					 {
							myUpData.upDataBuff[405]  += myUpData.upDataBuff[i];	 
					 }
					 
					  my_sendstring(USART2,(char *)myUpData.upDataBuff,406); 
					  debugOut("sendPreice\r\n");
					  myUpData.binPreicFlag =0; //表示发送了数据，没有收到ack
					  myUpData.binPreicTime =1; 
					 return 0x01;
				 }
				 else
				 {
						return 0x02;
				 }
			}	
		return 0x00;
}


/*
		
	接收货道价格信息 ack
*/

void   Up_BinPriceAck(void)
{
			if(usart2_Up.data[1]== 0x00) //ack
			{
					myUpData.autoNum =0; 
					myUpData.binPreicFlag =1; //表示发送了数据，收到了ack
					myUpData.binPreicTime =0;
					if(myUpData.signInFrist ==0)				
							myUpData.binFaultTime =1; //发送货道故障信息
				  debugOut("ackPreice\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.binPreicFlag =0; //表示发送了数据，没有收到ack
				  debugOut("nckPreice\r\n");
			}
			else
			{
					myUpData.binPreicFlag =0; //表示发送了数据，没有收到ack
				  debugOut("WTFPreice\r\n");
			}	
}

/*****************************************货道故障信息***************************************************/
/*
	货道故障信息
	指令     特征字               货柜编号            货道是否故障
	0x79     0xef 0xee 0xfe       byte1 0-16           byte13
	return  0x00 发送了数据，没有收到ack      0x01  收到了   0x03 其他
	暂时设置一个柜子的 信息 主柜

*/
u8   Up_BinFault(void)
{
			u8 i =0;
			if(myUpData.binFaultFlag ==0x00)
			{
					if(myUpData.binFaultTime > 500)
					{
						myUpData.binFaultTime  = 0;
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x79 ; //指令
						myUpData.upDataBuff[1]  = 0xef ; //特征
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //货柜编号 0-16
				 
																							
					 for(i =0; i<13;i++)                    //货道故障
					 {
								myUpData.upDataBuff[i+5] = 0x00; 
					 }
					 
					 for(i =0; i<18;i++)
					 {
							myUpData.upDataBuff[18]  += myUpData.upDataBuff[i];	 
					 }
					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,19); 
					  debugOut("sendBinFualt\r\n");
					 myUpData.binFaultFlag =0;   //表示发送了数据，没有收到ack
					 myUpData.binFaultTime =1;
						return 0x01;	
				 }
				 else
				 {
						return 0x02;
				 }	
			}
			else
			{
				return 0x00;			
			}

}


/*
		故障设置  接收到了ack
*/
 void  Up_BinFaultAck(void)
 {
			
			if(usart2_Up.data[1]== 0x00) //ack
			{			
					myUpData.autoNum =0; 
					myUpData.binFaultFlag =1; //表示发送了数据，收到了ack
					myUpData.binFaultTime =0;
					if(myUpData.signInFrist ==0)
							myUpData.runMsgTime = 1;//机器运行信息
				  debugOut("ackBinFualt\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.binFaultFlag =0; //表示发送了数据，没有收到ack
				  debugOut("nckBinFualt\r\n");
			}
			else
			{
					myUpData.binFaultFlag =0; //表示发送了数据，没有收到ack
				 debugOut("WTFBinFualt\r\n");
			}	
 }


 
/*****************************************机器运行信息***************************************************/
/*
	货道故障信息
	指令     特征字               运行状态     售货机状态    纸币器状态  硬币器状态  硬币器检测到硬币数量    
	0x7D     0xef 0xee 0xfe       byte1       byte1           byte1       byte1       byte8
	return  0x00 发送了数据，没有收到ack      0x01  收到了   0x03 其他
	暂时设置一个柜子的 信息 主柜
*/
//u8  Byj_DoorStatus(void);
 
u8   Up_RunMsg(void)
{
				u8 i =0;
				if(myUpData.runMsgFlag == 0x00)
				{
					if(myUpData.runMsgTime > 500)
					{
						myUpData.runMsgTime  =0;
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x7D ; //指令
						myUpData.upDataBuff[1]  = 0xef ; //特征
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	
						if(1)
						{
							myUpData.upDataBuff[4]  = 0x00 ; //运行状态 门关 营业
						}
						else
						{
							myUpData.upDataBuff[4]  = 0x03 ; //运行状态 门开暂停销售
						}
						
						myUpData.upDataBuff[5]  = 0x00 ; //售货机状态			 
						myUpData.upDataBuff[6]  = 0x00 ; //纸币器状态
						myUpData.upDataBuff[7]  = 0x00 ; //硬币器状态
					
						myUpData.upDataBuff[8]  = 0x00 ; //硬币器检测到硬币数量
						myUpData.upDataBuff[9]  = 0x00 ;
						myUpData.upDataBuff[10]  = 0x00 ;
						myUpData.upDataBuff[11]  = 0x00 ;
						myUpData.upDataBuff[12]  = 0x00 ;
						myUpData.upDataBuff[13]  = 0x00 ;
						myUpData.upDataBuff[14]  = 0x00 ;
						myUpData.upDataBuff[15]  = 0x00 ;
																													
						myUpData.upDataBuff[16]  = 0x00 ;	//纸币数量
						myUpData.upDataBuff[17]  = 0x00 ;	
						myUpData.upDataBuff[18]  = 0x00 ;	
						myUpData.upDataBuff[19]  = 0x00 ;	
						

					 for(i =0; i<20;i++)
					 {
							myUpData.upDataBuff[20]  += myUpData.upDataBuff[i];	 
					 }
					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,21); 
					 debugOut("sendRunMsg\r\n");
					 myUpData.runMsgFlag =0;   //表示发送了数据，没有收到ack
					 myUpData.runMsgTime =1;
					 myUpData.runMsgPollTime =0; 
//					 myUpData.runMsgFlag =1;
//					 myUpData.sysSetTime = 1;   //系统配置信息 计时开始
								return 0x01;		
				 }
				else
					{
								return 0x2;
					}
				}
				else
				{
					return 0x00;				
				}
}

/*
	设备运行信息 接收到ack
*/ 
void   Up_RunAck(void)
{
			if(usart2_Up.data[1]== 0x00)   //ack
			{
					myUpData.autoNum = 0; 
					myUpData.runMsgFlag = 1;   //表示发送了数据，收到了ack
					myUpData.runMsgTime = 0;
					if(myUpData.signInFrist ==0)
					   myUpData.sysSetTime = 1;   //系统配置信息 及时开始
				  debugOut("ackRunMsg\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.runMsgFlag =0;    //表示发送了数据，没有收到ack
				debugOut("nckRunMsg\r\n");
			}
			else
			{
					myUpData.runMsgFlag =0;    //表示发送了数据，没有收到ack
				  debugOut("WTFRunMsg\r\n");
			}	
}

/*****************************************系统配置信息***************************************************/
/*
	指令     特征字               照明灯  日光灯时间 温控状态 节能时间 制冷温度 加热温度 常温温度 保留
	0x7A     0xef 0xee 0xfe       byte1       byte4   byte1     byte4   byte1    byte1    byte1   byte4
	return  0x00 发送了数据，没有收到ack      0x01  收到了   0x03 其他
	暂时设置一个柜子的 信息 主柜
*/

u8   Up_SysSet(void)
{
				u8 i =0;
				if(myUpData.sysSetFlag == 0x00)
				{
					if(myUpData.sysSetTime > 500)
					{
						myUpData.sysSetTime=0;
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x7A ; //指令
						myUpData.upDataBuff[1]  = 0xef ; //特征
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //照明状态
					
						myUpData.upDataBuff[5]  = 0x99 ; //照明灯时间			 
						myUpData.upDataBuff[6]  = 0x99 ; 
						myUpData.upDataBuff[7]  = 0x99 ; 
						myUpData.upDataBuff[8]  = 0x99 ; 
					
						myUpData.upDataBuff[9]  = 0x99 ; //温控状态
					
						myUpData.upDataBuff[10]  = 0x99 ; //节能时间
						myUpData.upDataBuff[11]  = 0x99 ;
						myUpData.upDataBuff[12]  = 0x99 ;
						myUpData.upDataBuff[13]  = 0x99 ;
					
						myUpData.upDataBuff[14]  = 0x00 ; //制冷温度
						myUpData.upDataBuff[15]  = 0x00 ;
					
						myUpData.upDataBuff[16]  = 0x00 ; //加热温度
						myUpData.upDataBuff[17]  = 0x00 ;
						
//						myUpData.upDataBuff[18]  = 0x00 ; //常温温度
						
//						myUpData.upDataBuff[19]  = 0x00 ; //保留
//						myUpData.upDataBuff[20]  = 0x00 ; 
//						myUpData.upDataBuff[21]  = 0x00 ; 
//						myUpData.upDataBuff[22]  = 0x00 ; 
						
					 for(i =0; i<18;i++)
					 {
						 myUpData.upDataBuff[18]  += myUpData.upDataBuff[i];	 
					 }					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,19); 
					 debugOut("sendSysSet\r\n");
					 myUpData.sysSetFlag =0;            //表示发送了数据，没有收到ack
					 myUpData.sysSetTime =1;
					 return 0x01;		
				}
				else
					{
						return 0x02;
					}
			}
			return 0x00;
}



void  Up_SysSetAck(void)
{
			if(usart2_Up.data[1]== 0x00)       //ack
			{
					myUpData.autoNum = 0; 
					myUpData.sysSetFlag =1;      //表示发送了数据，收到了ack
					myUpData.sysSetTime =0;
				debugOut("ackSysSet\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.sysSetFlag =0;          //表示发送了数据，没有收到ack
				debugOut("nckSysSet\r\n");
			}
			else
			{
					myUpData.sysSetFlag =0;      //表示发送了数据，没有收到ack
				debugOut("WTFSysSet\r\n");
			}	
}

/*****************************************pos 状态信息***************************************************/
/*
	指令     特征字               pos状态 （0 等待刷卡 1 刷卡认证 2 授权成功 3 授权失败）
	0x81     0xef 0xee 0xfe       byte1       
	return  0x00 发送了数据，没有收到ack      0x01  收到了   0x03 其他
*/

u8  Up_PosMsg(void)
{
			u8 i =0;
			if(myUpData.posMsgFlag == 0x00)
			{					
					memset(myUpData.upDataBuff,0,dataLength1-1);
				 
					myUpData.upDataBuff[0]  = 0x81 ; //指令
					myUpData.upDataBuff[1]  = 0xef ; //特征
					myUpData.upDataBuff[2]  = 0xee ;
					myUpData.upDataBuff[3]  = 0xfe ;	

					myUpData.upDataBuff[4]  = 0x00 ;     //pos 机状态				
				
					 for(i =0; i<5;i++)
					 {
						 myUpData.upDataBuff[5]  += myUpData.upDataBuff[i];	 
					 }					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,6); 
					 debugOut("sendPosMsg\r\n");
					 myUpData.sysSetFlag =0;            //表示发送了数据，没有收到ack
					 return 0x00;		
			}			
			return 0x01;
}


void  Up_PosMsgAck(void)
{
			if(usart2_Up.data[1]== 0x00)     //ack
			{
					myUpData.posMsgFlag =1;      //表示发送了数据，收到了ack
					debugOut("ackPosMsg\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.posMsgFlag =0;      //表示发送了数据，没有收到ack
				  debugOut("NckPosMsg\r\n");
			}
			else
			{
					myUpData.posMsgFlag =0;      //表示发送了数据，没有收到ack
					debugOut("WTFPosMsg\r\n");
			}	
}

/*****************************************pos 状态信息***************************************************/
/*
	指令     特征字               货柜编号      货道优惠信息
	0x82     0xef 0xee 0xfe       byte1          byte400
	return  0x00 发送了数据，没有收到ack      0x01  收到了   0x03 其他
*/

u8   Up_preferential(void)
{
				u16 i= 0;
				if(myUpData.PreFlag==0x00)
				{
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x82 ; //指令
						myUpData.upDataBuff[1]  = 0xef ; //特征
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //货柜编号 0-16
				 
																							//货道价格信息 byte400   暂时设置为0
					 for(i =0; i<400;i++) 
					 {
								myUpData.upDataBuff[i+5] = 0x00; 
					 }
					 
					 for(i =0; i<405;i++)
					 {
							myUpData.upDataBuff[405]  += myUpData.upDataBuff[i];	 
					 }
					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,405); 
					 debugOut("sendpreferential\r\n");
					 myUpData.PreFlag =0; //表示发送了数据，没有收到ack
					 return 0x00;		
				 }
	
	return 0x01;
}


void  Up_PreAck(void)
{
			if(usart2_Up.data[1]== 0x00)     //ack
			{
					myUpData.PreFlag =1;      //表示发送了数据，收到了ack
				  debugOut("ackpreferen\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.PreFlag =0;      //表示发送了数据，没有收到ack
					debugOut("nckpreferen\r\n");
			}
			else
			{
					myUpData.PreFlag =0;      //表示发送了数据，没有收到ack
				  debugOut("WTFpreferen\r\n");
			}	
}


/************************************自动补发***********************************************************/
/*
		如果300ms没有指令没有收到工控机的ack 则自动补发
		最后执行poll 如果有其他指令发送了，则不发送poll指令
		
*/
u8  FuncRet =0;
void  Up_AllCmdAuto(void) 
{
	if(myUpData.autoTime > 300) 
	{
		myUpData.autoTime = 0; 
//		printf("1");
		if(myUpData.autoNum > 30) // 30没有收到ack 则重新签到
		{
				debugOut("timeover\r\n");
				myUpData.autoNum = 0;  
				myUpData.signInFrist =0;   //重新轮询发送报文 
				myUpData.signInFlag = 0;   //记录发送了签到协议 但未首单ack  如果等于1 表示收到ack
				myUpData.signInTime = 0;
			
				myUpData.binConFlag = 0;     //副柜连接信息
				myUpData.binSetFlag = 0;    
				myUpData.binPreicFlag = 0;
				myUpData.binFaultFlag = 0;
				myUpData.runMsgFlag = 0;
				myUpData.sysSetFlag = 0;
					
				myUpData.outFlag = 0;       //出货状态清零			
		}
		
		if(myUpData.runMsgPollTime >30)
		{
				myUpData.runMsgPollTime =0;
				myUpData.runMsgFlag  =0;
				myUpData.runMsgTime  =1;    //开始发送一次
		}
		
		
		FuncRet = Up_autoSignIn();      //没有收到ack重复发送
		if(FuncRet)
		{			
			myUpData.autoNum++;
			return;		
		}
		
		FuncRet = Up_BinConnectionMsg();  //箱柜连接状态发送 
		if(FuncRet)                       //没有收到ack，或者时间没到没有发送
		{
			myUpData.autoNum++;
			return; 
		}
		
		FuncRet = Up_BinSetMsg();         //货道设置信息
		if(FuncRet)                       //没有收到ack，或者时间没到没有发送
		{
			myUpData.autoNum++;
			return; 
		}		
		
		FuncRet = Up_BinPrice();          //货道价格信息
		if(FuncRet)                       //没有收到ack，或者时间没到没有发送
		{
			myUpData.autoNum++;
			return; 
		}	
		
		FuncRet = Up_BinFault();          //货道故障信息
		if(FuncRet)                       //没有收到ack，或者时间没到没有发送
		{
			myUpData.autoNum++;
			return; 
		}	
		
		FuncRet = Up_RunMsg();            //机器运行信息
		if(FuncRet)                       //没有收到ack，或者时间没到没有发送
		{
			myUpData.autoNum++;
			return; 
		}		
		
		FuncRet = Up_SysSet();            //系统设置信息
		if(FuncRet)                       //没有收到ack，或者时间没到没有发送
		{
			myUpData.autoNum++;
			return; 
		}		
					

		FuncRet = Up_AutoOutStatus();    //出货状态,没有收到ack继续重发
		if(FuncRet)											 //没有收到ack，或者时间没到没有发送
		{
			myUpData.autoNum++;
			return;		
		}
		
		FuncRet = Up_Poll();
		if(FuncRet)
		{
			myUpData.autoNum++;
			return;		
		}
		myUpData.autoNum = 0;           //
		myUpData.signInFrist =1;        //记录已经发送了一次完整的报文
	}
}
/*
	 串口2接收到数据，对数据进行分析
	 有ack  
*/

u8   checkout_i    =0;
u8   checkout_data =0;
void  Up_DataParse(void)
{

		if(usart2_Up.flag)
		{
			 usart2_Up.flag = 0;
			 //printf("%s",usart2_Up.data);
			 //校验
			for(checkout_i =0,checkout_data=0; checkout_i <usart2_Up.count -1; checkout_i++)
			{	
			  checkout_data += usart2_Up.data[checkout_i];
			}
			
			if(checkout_data == usart2_Up.data[usart2_Up.count-1]) //校验通过
			{
							switch (usart2_Up.data[0])
									{
										case 0x74:           //货柜连接信息
												Up_BinConnectionAck();
											break;
										case 0x75:					 //货道设置信息
											  Up_BinSetAck();
											break;
										case 0x76:					 //poll轮询
												Up_PollParse();
											break;
//										case 0x77:				 //
//											break;
										case 0x78:					 //签到
												Up_SignInAck();														
											break;	
										case 0x79:					 //货道故障（有货无货）
												Up_BinFaultAck(); 
											break;
										case 0x7A:					 //vmc配置信息
											  Up_SysSetAck();
											break;
										case 0x7B:					 //商品销售汇总信息
											break;
										case 0x7C:					 //出货
												Up_OutAck();												
											break;
										case 0x7D:           //机器运行 （故障）信息
											  Up_RunAck();
											break;
										case 0x7F:					 //货道价格信息
											  Up_BinPriceAck();
											break;
										case 0x81:					 //pos机状态信息
												Up_PosMsgAck(); 
											break;
										case 0x82:					 //货道优惠信息
												Up_PreAck();
											break;
										default:
											break;
									}		
			}
			else
			{	
				//	printf("校验错误");
			}	
			usart2_Up.count = 0;
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		}
}
































