#include   "DownData.h"
#include   "myUsart.h"
#include   "UpData.h"
#include   "BYJ.h"

struct downstruct  myDownData; //下发驱动电机

char  openResult[25] ={0};
unsigned  int   RxCrc =0;  //收到数据的crc
unsigned  int   tempRxCrc =0; 
unsigned  char  RxCrc_i =0;


/*
发送开箱指令给 驱动板， 串口3  3线电机 无光幕
01 05 00 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 70 48           9
				cmd  交易码   货柜编号   货道编号     变价出货  售卖金额   支付方式    流水号  校验和
strData: 76  03       0x01       0x01        0x01        4byte      0x01       3byte

并记录 主柜id  货道id  金额  支付方式  流水号
*/

unsigned int tempNumber =0; //上一次流水号
void Down_SendOpen(unsigned char * strData)
{
			unsigned int crctemp =0;
			
			memset(myDownData.downDataBuff,0,Rx3_Length);
	
			myDownData.downDataBuff[0] =  strData[2]+0x01; //  ID  0-16
			myDownData.downDataBuff[1] =   0x05;
			
			myDownData.downDataBuff[2] =   strData[3] - 0x01;      //货道  0-59
			myDownData.downDataBuff[3] =   0x03;            //3三线制
			myDownData.downDataBuff[4] =   0x00;            //3三线制
			
			myDownData.downDataBuff[5] =   0x00;
			myDownData.downDataBuff[6] =   0x00;
			myDownData.downDataBuff[7] =   0x00;
			myDownData.downDataBuff[8] =   0x00;
			myDownData.downDataBuff[9] =   0x00;
			myDownData.downDataBuff[10] =   0x00;
			myDownData.downDataBuff[11] =   0x00;
			myDownData.downDataBuff[12] =   0x00;
			myDownData.downDataBuff[13] =   0x00;
			myDownData.downDataBuff[14] =   0x00;
			myDownData.downDataBuff[15] =   0x00;
			myDownData.downDataBuff[16] =   0x00;
			myDownData.downDataBuff[17] =   0x00;
				
			//crc
			crctemp = crcVal(myDownData.downDataBuff,18);
			myDownData.downDataBuff[18] = crctemp;	
			myDownData.downDataBuff[19] = crctemp >> 8;	
						
			
			myDownData.boardID = myDownData.downDataBuff[0]-1;
			myDownData.cargoID = myDownData.downDataBuff[2]+1; //货道id
			//myDownData.cargoMoney = 1;                       //这些暂时都没有处理
			myDownData.cargoMoney = strData[5];
			myDownData.cargoMoney = (myDownData.cargoMoney << 8) + strData[6];
			myDownData.cargoMoney = (myDownData.cargoMoney << 8) + strData[7];
			myDownData.cargoMoney = (myDownData.cargoMoney << 8) + strData[8];
			
			myDownData.cargoWay  =  strData[9];
			//流水号  byte3 
			
			tempNumber = strData[10];
			tempNumber = (tempNumber << 8) + strData[11];
			tempNumber = (tempNumber << 8) + strData[12];
			
			if((tempNumber !=0)&&(tempNumber == myDownData.cargoNumber))
			{
					Up_PollAck(0);	
					debugOut("PollSameOpen\r\n");
					return; 	
			}

//			my_sendstring(USART3,(char *)myDownData.downDataBuff,20);	
					//myDownData.openTime = 1;    //开箱发送 7000ms > 则超时	 等待串口3 返回数据
						
			//先控制升降台到指定位置，然后再转动电机掉或。暂定每层都是10个电机  myDownData.downDataBuff[2] = 1--60
			//   

	
			myUpData.lastBoardId = myDownData.boardID ;
			myUpData.lastboxNum  = myDownData.cargoID ;
			
			
			if(tempNumber)			
				myDownData.cargoNumber = tempNumber;
			else
				myDownData.cargoNumber++;

		
			
		Up_PollAck(1);			    
		debugOut("PollOpen\r\n");
} 

/*
	发送获取电机状态 如果超时 反馈工控机
	从发送启动电机 5s 后开始发送，200ms 发送一次 直到700ms   7000ms 超时
	z1  0x01 空闲  0x01 执行中  0x02 执行完毕
	z2  正在运行的电机编号 00-59
	z3  执行结果 0 成功 1 过流 2 欠流  3 超流 4 光幕自检失败，未启动电机 5 有反馈电磁铁门未打开
	z8-z9 运行时间
	z10 光幕状态
*/
void   Down_GetStatus(void)
{	
		
	if(myDownData.openTime > 7000) //超时 返回工控机
	{
		myDownData.openTime  = 0;
		Up_OutStatus(myDownData.boardID,myDownData.cargoID,myDownData.cargoMoney,myDownData.cargoWay,0x01,myDownData.cargoNumber);								
//			my_sendstring(USART2,(char *)myDownData.downDataBuff,13);	  //超时出货失败
		//printf("%d",myDownData.openTime);
		debugOut("open_timeOver\r\n");
		//控制升降台复位   0-5  
//		Byj_Back((myDownData.cargoID-1) /10);
		return;
	}
	if(myDownData.openTime > 5000)
	{
				if(myDownData.openTime200 >200) //200ms发送一次 到驱动板获取 执行结果
				{
					myDownData.openTime200 = 1;
					myDownData.downDataBuff[0] = 0x01;  //版号
					myDownData.downDataBuff[1] = 0x03;  //指令
					myDownData.downDataBuff[2] = 0x00;	
					myDownData.downDataBuff[3] = 0x00;
					myDownData.downDataBuff[4] = 0x00;
					myDownData.downDataBuff[5] = 0x00;
					myDownData.downDataBuff[6] = 0x00;
					myDownData.downDataBuff[7] = 0x00;
					myDownData.downDataBuff[8] = 0x00;
					myDownData.downDataBuff[9] = 0x00;
					myDownData.downDataBuff[10] = 0x00;
					myDownData.downDataBuff[11] = 0x00;
					myDownData.downDataBuff[12] = 0x00;
					myDownData.downDataBuff[13] = 0x00;
					myDownData.downDataBuff[14] = 0x00;
					myDownData.downDataBuff[15] = 0x00;
					myDownData.downDataBuff[16] = 0x00;
					myDownData.downDataBuff[17] = 0x00;
					myDownData.downDataBuff[18] = 0xD0;   //crc  //暂时固定
					myDownData.downDataBuff[19] = 0xE8;   //					
					my_sendstring(USART3,(char *)myDownData.downDataBuff,20);		
					//printf("%d",myDownData.openTime);
					debugOut("open_getStatus\r\n"); 
				}
	}
}




/*
		 驱动电机 结果
		 7000ms超时
01 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 D0 E8 
	 从发送启动电机 5s 后开始发送，200ms 发送一次 直到700ms   7000ms 超时
	z1  0x01 空闲  0x01 执行中  0x02 执行完毕
	z2  正在运行的电机编号 00-59
	z3  执行结果 0 成功 1 过流 2 欠流  3 超流 4 光幕自检失败，未启动电机 5 有反馈电磁铁门未打开
	z8-z9 运行时间
	z10 光幕状态       &&(myDownData.downDataBuff[2] == usart3_Down.data[3]
*/
//myDownData.openFlag; //电机工作成功 1 
void  Down_OpenResult(void)
{	
	if(usart3_Down.data[1] == 0x03) //反馈
	{
			if((usart3_Down.data[2]== 0x02 && usart3_Down.data[4]== 0x00) )//空闲 运行成功  
			{
				myDownData.openTime =0; myDownData.openTime200 = 0;
				debugOut("tanhuangSecuss\r\n");
//				// 发送	0x7c  给工控机，告之出货成功
//				Up_OutStatus(myDownData.boardID,myDownData.cargoID,myDownData.cargoMoney,myDownData.cargoWay,0x00,myDownData.cargoNumber);	
//				debugOut("open_succes\r\n");
				
					//弹簧运行成功，控制升降台下降
//					Byj_Back(myDownData.downDataBuff[2]%10);
				myDownData.openFlag =1;
				
			}
			else //失败 暂不处理，等待超时
			{
					
			}
	}
}



/*
		vmc和驱动板通讯
		接收驱动板发出的数据，解析数据是做什么的
		指令：主机-> 从机
    从机地址（1byte）  指令 （1byte）   数据（16byte） 校验（2byte）
		相应：从机->主机
		主机地址（0x0）    指令 （1byte）   数据（16byte） 校验（2byte）
		
*/

void   Down_DataParse(void)
{
		if(usart3_Down.flag)
		{
				usart3_Down.flag =0;
				//my_sendstring(USART3,(char *)usart3_Down.data,3);
			
				//crc校验
				RxCrc = crcVal(usart3_Down.data,18);
			
			  tempRxCrc = usart3_Down.data[19];
				tempRxCrc = (tempRxCrc << 8) + usart3_Down.data[18];
				
				if(RxCrc == tempRxCrc)  //通过
				{
							
						//暂不考虑是哪一款板子返回来的数据
					
						switch (usart3_Down.data[1])
										{
											case 0x01: //获取ID
												break;
											case 0x03: //获取状态
												Down_OpenResult();
												break;
											case 0x05: //启动电机
												//Down_OpenResult();
												myDownData.openTime = 1;    //开箱发送 7000ms > 则超时	 等待串口3 返回数据
												break;
											case 0x07: //开关亮输出
												break;
											case 0x09: //读开关量
												break;
											case 0xA0: //启动升降台
												break;
											case 0xA1: //查询升级台的状态
												break;
											case 0xFF: //设置板卡地址
											default:   
												break;
										}
				}
				else
				{
				
				
				}		
			usart3_Down.count =0;
		  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		}
}



/*
crc-16 校验

功能：    crc校验
pcMess:   待求数据的首指针
wLen:     带球数据的长度
*/


unsigned int crcVal(unsigned char *pcMess,unsigned int wLen)
{
	long MSBInfo;
	int i,j ;
	unsigned int nCRCData;
	nCRCData = 0xffff;
	for(i = 0; i < wLen;i++)
	{
		pcMess[i]&=0x00ff;
		nCRCData = nCRCData ^ pcMess[i] ;
		for(j= 0 ; j < 8 ;j ++)
		{
			MSBInfo = nCRCData & 0x0001;
			nCRCData = nCRCData  >> 1;
			if(MSBInfo != 0 )
				nCRCData = nCRCData ^ 0xa001;
		}
	}
	return nCRCData;
}









