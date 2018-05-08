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
���հ�׿�˵�����


*/

/******************************************ǩ��Э��***********************************************/
/*
ǩ��Э��

���ݳ���  ָ��    0x78
					����    0xef 0xee 0xfe
					����    50 byte
					��У��  1 byte
�ú�����ȡ����ǩ����Ҫ�����ݣ���ʹ�ô��ڷ��ͱ��ģ� ���û���յ�ack 1S��������� 

ack        ָ��
����       0x01  (0x15  nck)
У���			
*/

u16 Up_tempYear =0;
char  Up_tiembuff[10] ={0};
void  Up_SignIn(void)
{
	
	u8  i =0;
	 memset(myUpData.upDataBuff,0,dataLength1-1);
	RTC_Get();
	//ָ��
	myUpData.upDataBuff[0] = 0x78;
	//����
	myUpData.upDataBuff[1] = 0xef;
	myUpData.upDataBuff[2] = 0xee;
	myUpData.upDataBuff[3] = 0xfe;
	//����
	//���ذ�汾�� 0xff 0xff
	myUpData.upDataBuff[4] = 0x03;
	myUpData.upDataBuff[5] = 0x00;
	//������汾��
	myUpData.upDataBuff[6] = 0x01;
	myUpData.upDataBuff[7] = 0x00;
	//vmcЭ��汾�� 0xff
	myUpData.upDataBuff[8] = 0x02;
	//�ۻ������ 0xff 0xff 0xff 0xff
	myUpData.upDataBuff[9]  = 0x12;
	myUpData.upDataBuff[10] = 0x34;
	myUpData.upDataBuff[11] = 0x56;
	myUpData.upDataBuff[12] = 0x78;
	//ʱ�� bcd yyyy mm dd hh mm ss
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
	//�������� 0x01 ���ϻ� 0x02 �ۺϻ��� 0x03 ���ӻ�
	myUpData.upDataBuff[20]  = 0x02;
	//���������
	myUpData.upDataBuff[21]  = 0x14;      //���ָʲô��
	//16����չ����Ϣ
	myUpData.upDataBuff[22]  = 0x00;      //1   ����
	myUpData.upDataBuff[23]  = 0x00;      //��������
	
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
	
	//У���
	for(i=0;i< 54; i++)
	{
		 myUpData.upDataBuff[54] +=  myUpData.upDataBuff[i]; 
	}
	
	//send data
	my_sendstring(USART2,(char *)myUpData.upDataBuff,55);	
	myUpData.signInFlag = 0;   //��¼������ǩ��Э�� ��δ�׵�ack  �������1 ��ʾ�յ�ack
	myUpData.signInTime = 0;
	
}


/*
			�Զ�����ǩ������
			1S ����һ�Σ�֪���յ�ack
return 0x00  �Ѿ�ǩ��       0x01 ���͵ȴ�����ack  0x02  ���ʱ��û�е�
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
		 �յ�ǩ����ack ���� nak

*/
void  Up_SignInAck(void)
{
			if(usart2_Up.data[1]==0x00) //ack
			{
					myUpData.autoNum = 0;  
					myUpData.signInFlag = 1;   //
					myUpData.signInTime = 0;
					myUpData.binConTime = 1;   //��ʱ500ms ���ͻ���������Ϣ
				
					myUpData.binConFlag = 0;     //����������Ϣ
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
					myUpData.signInFlag = 0;   //��¼������ǩ��Э��
					myUpData.signInTime =0;	
				 debugOut("NckSingIn\r\n");  
			}
			else
			{
					myUpData.signInFlag = 0;   //��¼������ǩ��Э��
					myUpData.signInTime =0;	
				 debugOut("WTFSingIn\r\n");  
			}

}


/******************************************POLLָ��***********************************************/
/*
ǩ��Э��

���ݳ���  ָ��    0x76
					����    0xef 0xee 0xfe
					����    19 byte
					��У��  1 byte
��ǩ���ɹ���ǰ����
�ú��� 300ms ����һ�α��ģ�

ack        ָ��
����       0x01  (0x15  nck)
У���	
ִ�з���0x00 �յ�ack
*/
u8  Poll_check=0;
u8 Up_Poll(void)
{
	if(myUpData.signInFlag) //ǩ���ɹ�
	{
				if(myUpData.pollTime > 300)
				{
						myUpData.pollTime =0;
					
					 memset(myUpData.upDataBuff,0,dataLength1-1);
						//ָ��
						myUpData.upDataBuff[0] = 0x76;
						//����
						myUpData.upDataBuff[1] = 0xef;
						myUpData.upDataBuff[2] = 0xee;
						myUpData.upDataBuff[3] = 0xfe;
						//����
						//�û�Ͷ����ֽ��� ��Ϊ��λ12byte
					  //getUserInputCash();
						myUpData.upDataBuff[4] = 0x00;   //�����ܽ�� 
						myUpData.upDataBuff[5] = 0x00;
						myUpData.upDataBuff[6] = 0x00;
						myUpData.upDataBuff[7] = 0x00;
					   
						myUpData.upDataBuff[8] = 0x00;   //pos ���ս��
						myUpData.upDataBuff[9]  = 0x00;
						myUpData.upDataBuff[10] = 0x00;
						myUpData.upDataBuff[11] = 0x00;
						
						myUpData.upDataBuff[12] = 0x00;  //�ֽ������
						myUpData.upDataBuff[13] = 0x00;
						myUpData.upDataBuff[14] =  0x00;
						myUpData.upDataBuff[15] =  0x00;
					//����������� 6byte
					//getUserAisle();
						myUpData.upDataBuff[16] =  0x00; //������ 0x00 ����
						myUpData.upDataBuff[17] =  0x00; //������ţ����û���ϴ�0
						myUpData.upDataBuff[18] =  0x00; //��Ʒ�۸�
						myUpData.upDataBuff[19] =  0x00;
						myUpData.upDataBuff[20]  = 0x00;
						myUpData.upDataBuff[21]  = 0x00;
					//��������productID 1byte
					//getProductID();
						myUpData.upDataBuff[22]  = 0x00;						
						
					//���У��
					for(Poll_check =0; Poll_check <23; Poll_check++ )
					{								
						myUpData.upDataBuff[23] += myUpData.upDataBuff[Poll_check];
					}	
					//send data
					my_sendstring(USART2,(char *)myUpData.upDataBuff,24);	
          //��¼��������
				//	debugOut("Poll\r\n");
					if(myUpData.pollFlag)  //û���յ�ack
					{
						myUpData.pollFlag = 1;  //������Ϊ1 �յ�ack��Ϊ 0
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
unsigned char poll_ack[5] = {0x00,0x00,0x00,0x00}; //�ظ�ack  nck

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
	����ʱ��  
	76 02 20 17 08 02 10 25 55  ��У��
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
���Ϳ���ָ��� �����壬 ����3  3�ߵ�� �޹�Ļ
*/

/*
			poll ack����    ����ͨack  nak  ����������Ϣ�� ack
			
			��������

		  �յ�poll ack ָ��
*/
void   	Up_PollParse(void)
{		
			memset(myUpData.upDataBuff,0,dataLength1-1);
			myUpData.pollFlag =0;     //��¼�յ�ack
			switch (usart2_Up.data[1])
			{
				case 0x00:    //ack
					break;
//				case 0x15:    //nak
//					break;
				case 0x01:    //ack
					//Up_PollAck(1);
					break;
				case  0x02:   //����ϵͳʱ��
						Up_PollAck(1);	
						Up_PollSetTime();
						debugOut("PollTimeSet\r\n");
					return;
				case  0x03:   //��������
					  Down_SendOpen(usart2_Up.data);
						debugOut("PollOutGoods\r\n");
					return;
				case  0x04:   //���û����۸���Ϣ
					//Up_setPrice();
					debugOut("PollSetPrice\r\n");
					break;
				case  0x05:   //����ϵͳ������Ϣ
					//Up_setSysMsg();
					debugOut("PollSysSet\r\n");
				  break;
				case  0x06:   //�������ͳ��
					//Up_clearStat();
					debugOut("PollClear\r\n");
					break;
				case  0x07:   //��ȡ������Ϣ
					//Up_getSaleMsg();
				debugOut("PollGetSaleMsg\r\n");
					break;
				case  0x08:   //��ʼ���û����۸�
					//Up_startSetPrice();
					debugOut("PollStartSetPrice\r\n");
					break;
				case  0x09:   //�����۸��������
					//Up_doneSetPrice();
					debugOut("PollDoneSetPrice\r\n");
					break;
				case  0x0A:   //���û����Ż���Ϣ
					//Up_setPreferential();
				  debugOut("PollSetPreferent\r\n");
					break; 
				case  0x0B:   //���ý��׵ķ���ͽ���	
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




/******************************************����ָ��***********************************************/

/*
����ָ��  �����������

���ݳ���  ָ��    0x7C
					����    0xef 0xee 0xfe
					����    11 byte
					��У��  1 byte

ack        ָ��
����       0x01  (0x15  nck)
У���
boarid  ���id
boxnum  ��ں�
mymoney  �������  ��Ϊ��λ
way  ֧����ʽ
number  ��ˮ�ţ�

*/

void   Up_OutStatus(unsigned char boardId,unsigned char boxNum,unsigned int mymoney ,unsigned char way,unsigned char ret,unsigned int number)
{
		u8 i =0 ;
		
		  memset(myUpData.upDataBuff,0,dataLength1-1);
		
			myUpData.upDataBuff[0]  = 0x7c ; //ָ��
			myUpData.upDataBuff[1]  = 0xef ; //����
			myUpData.upDataBuff[2]  = 0xee ;
			myUpData.upDataBuff[3]  = 0xfe ;	
			myUpData.upDataBuff[4]  = boardId ;// ������
			myUpData.upDataBuff[5]  = boxNum  ; // �������
//������Ҫ������
//			myUpData.upDataBuff[6]  = 0x7c ;  //�������
//			myUpData.upDataBuff[7]  = 0x7c ;
//			myUpData.upDataBuff[8]  = 0x7c ;
//			myUpData.upDataBuff[9]  = 0x7c ;
			myUpData.upDataBuff[6]  = mymoney >>24;
			myUpData.upDataBuff[7]  = mymoney >>16;
			myUpData.upDataBuff[8]  = mymoney >>8;
			myUpData.upDataBuff[9]  = mymoney ;
	
	
			myUpData.upDataBuff[10] = way ;  //֧����ʽ
			myUpData.upDataBuff[11] = ret ; //����״̬
	//������Ҫ����
//			myUpData.upDataBuff[12] = 0x7c ; //��ˮ��
//			myUpData.upDataBuff[13] = 0x7c ;
//			myUpData.upDataBuff[14] = 0x7c ;
			
			myUpData.upDataBuff[12] = (number >> 16);
			myUpData.upDataBuff[13] = (number >> 8);
			myUpData.upDataBuff[14] =  number;
			
	
			for(i=0; i< 15 ; i++)
			{
				myUpData.upDataBuff[15] +=  myUpData.upDataBuff[i] ; //��У��
			}
			
			//��¼���ͳ�������ָ��ȴ�����ack
			myUpData.outFlag  = 1;   //0x01��ʾ������  0x00 ��ʾ���յ���ack
			myUpData.outTime  = 1;
//			myUpData.lastBoardId = boardId-1;
//			myUpData.lastboxNum  = boxNum+1;
			myUpData.lastMoney   = mymoney;
			myUpData.lastWay     = way;
			myUpData.lastRet     = ret;
			myUpData.lastNumber  = number;
			my_sendstring(USART2,(char *)myUpData.upDataBuff,16);		
			if(ret) //0 �ɹ�
				debugOut("sendOpenFail\r\n");
			else
				debugOut("sendOpenSucces\r\n");			
}

/*

  ���ͷ����������ģ�300ms û���յ�ack ���ط�
	myUpData.outFlag  = 1;  ��ʾ������ ���� �� ǩ���෴
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
					myUpData.outFlag = 0;   // ��ʾ�յ�ack
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
/*****************************************����������Ϣ***************************************************/

//��ȡ�豸������Ϣ��myUpData.binConBuff[7],  
void  getBinMsg(void)
{
	
		myUpData.binConBuff[0] = 0x01; //����������Ϣ
		myUpData.binConBuff[1] = 0x00;
		myUpData.binConBuff[2] = 0x00;
	
		myUpData.binConBuff[3] = 0x00; //�Ƿ�֧����۵�vmc
		myUpData.binConBuff[4] = 0x00;
		myUpData.binConBuff[5] = 0x00;
	
		myUpData.binConBuff[6] = 0x00;
}





/*
		����������Ϣ ����
	ָ��     ������               ����������Ϣ            vmc����
	0x74     0xef 0xee 0xfe       byte3                   byte3
	return  0x00 �����������յ�ack      0x01  �������� û��û���յ�ack   0x02 ʱ��û�е�
*/ 

u8   Up_BinConnectionMsg(void)
{
		 u8   i = 0;

			 if(myUpData.binConFlag == 0)
			 {
				 if(myUpData.binConTime >500)
				 {
					  myUpData.binConTime = 0 ;
						//��ȡ�����Ϣ
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x74 ; //ָ��
						myUpData.upDataBuff[1]  = 0xef ; //����
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	
						//getBinMsg()
						myUpData.upDataBuff[4]  = myUpData.binConBuff[0] ; //����������Ϣ 
						myUpData.upDataBuff[5]  = myUpData.binConBuff[1] ; 
						myUpData.upDataBuff[6]  = myUpData.binConBuff[2] ;
					 
						myUpData.upDataBuff[7]  = myUpData.binConBuff[3] ; //�Ƿ�֧�����ü۸�vmc
						myUpData.upDataBuff[8]  = myUpData.binConBuff[4] ; 
						myUpData.upDataBuff[9]  = myUpData.binConBuff[5] ;
					 
					 for(i =0; i<10;i++)
					 {
							myUpData.upDataBuff[10]  += myUpData.upDataBuff[i];	 
					 }
					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,11);
					 debugOut("sendConnectMsg\r\n");
					 myUpData.binConFlag =0; //��ʾ���������ݣ�û���յ�ack
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
		���յ�ack
*/
void   Up_BinConnectionAck(void)
{
			if(usart2_Up.data[1]== 0x00) //ack
			{		
					myUpData.autoNum =0; 
					myUpData.binConFlag = 1; //��ʾ���������ݣ��յ���ack
					myUpData.binConTime = 0;
				
					if(myUpData.signInFrist == 0)
					{
						myUpData.binSetTime = 1; //��ʼ���ͻ���������Ϣ				
					}
				  debugOut("ackConnect\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.binConFlag =0; //��ʾ���������ݣ�û���յ�ack
				  debugOut("NckConnect\r\n");
			}
			else
			{
					myUpData.binConFlag =0; //��ʾ���������ݣ�û���յ�ack
				  debugOut("WTFConnect\r\n");
			}
}



/*****************************************����������Ϣ***************************************************/
/*
		����������Ϣ����
	ָ��     ������               ������            ����������Ϣ
	0x75     0xef 0xee 0xfe       byte1 0-16           byte13
	return  0x00 ���������ݣ�û���յ�ack      0x01  �յ���   0x03 ����
	��ʱ����һ�����ӵ� ��Ϣ ����
*/ 


u8   Up_BinSetMsg(void)
{
		u8 i = 0;
		if(myUpData.binSetFlag == 0x00) // 
		{
			if(myUpData.binSetTime > 500)
			{
						myUpData.binSetTime  =0;
				
				    //��ȡ���������Ϣ
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x75 ; //ָ��
						myUpData.upDataBuff[1]  = 0xef ; //����
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //����� 0x00 ����
			
						myUpData.upDataBuff[5]   = 0xFF ; //����������Ϣ
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
					 myUpData.binSetFlag  = 0; //��ʾ���������ݣ�û���յ�ack
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
			����������Ϣ ���յ�ack
*/

void  Up_BinSetAck(void)
{
			if(usart2_Up.data[1]== 0x00) //ack
			{
					myUpData.autoNum =0; 
					myUpData.binSetFlag =1; //��ʾ���������ݣ��յ���ack
					myUpData.binSetTime =0;
				
					if(myUpData.signInFrist ==0)
					{
						myUpData.binPreicTime =1;//��ʼ��һ�������۸���Ϣ				
					}

				  debugOut("ackBinSet\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.binSetFlag =0; //��ʾ���������ݣ�û���յ�ack
				  debugOut("nckBinSet\r\n");
			}
			else
			{
					myUpData.binSetFlag =0; //��ʾ���������ݣ�û���յ�ack
				  debugOut("WTFBinSet\r\n");
			}	
}

/*****************************************�����۸���Ϣ***************************************************/
/*
	�����۸���Ϣ
	ָ��     ������               ������            �����۸���Ϣ
	0x7F     0xef 0xee 0xfe       byte1 0-16           byte400
	return  0x00 ���������ݣ�û���յ�ack      0x01  �յ���   0x03 ����
	��ʱ����һ�����ӵ� ��Ϣ ����
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
					 
						myUpData.upDataBuff[0]  = 0x7F ; //ָ��
						myUpData.upDataBuff[1]  = 0xef ; //����
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //������ 0-16
				 
																							//�����۸���Ϣ byte400   ��ʱ����Ϊ0
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
					  myUpData.binPreicFlag =0; //��ʾ���������ݣ�û���յ�ack
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
		
	���ջ����۸���Ϣ ack
*/

void   Up_BinPriceAck(void)
{
			if(usart2_Up.data[1]== 0x00) //ack
			{
					myUpData.autoNum =0; 
					myUpData.binPreicFlag =1; //��ʾ���������ݣ��յ���ack
					myUpData.binPreicTime =0;
					if(myUpData.signInFrist ==0)				
							myUpData.binFaultTime =1; //���ͻ���������Ϣ
				  debugOut("ackPreice\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.binPreicFlag =0; //��ʾ���������ݣ�û���յ�ack
				  debugOut("nckPreice\r\n");
			}
			else
			{
					myUpData.binPreicFlag =0; //��ʾ���������ݣ�û���յ�ack
				  debugOut("WTFPreice\r\n");
			}	
}

/*****************************************����������Ϣ***************************************************/
/*
	����������Ϣ
	ָ��     ������               ������            �����Ƿ����
	0x79     0xef 0xee 0xfe       byte1 0-16           byte13
	return  0x00 ���������ݣ�û���յ�ack      0x01  �յ���   0x03 ����
	��ʱ����һ�����ӵ� ��Ϣ ����

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
					 
						myUpData.upDataBuff[0]  = 0x79 ; //ָ��
						myUpData.upDataBuff[1]  = 0xef ; //����
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //������ 0-16
				 
																							
					 for(i =0; i<13;i++)                    //��������
					 {
								myUpData.upDataBuff[i+5] = 0x00; 
					 }
					 
					 for(i =0; i<18;i++)
					 {
							myUpData.upDataBuff[18]  += myUpData.upDataBuff[i];	 
					 }
					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,19); 
					  debugOut("sendBinFualt\r\n");
					 myUpData.binFaultFlag =0;   //��ʾ���������ݣ�û���յ�ack
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
		��������  ���յ���ack
*/
 void  Up_BinFaultAck(void)
 {
			
			if(usart2_Up.data[1]== 0x00) //ack
			{			
					myUpData.autoNum =0; 
					myUpData.binFaultFlag =1; //��ʾ���������ݣ��յ���ack
					myUpData.binFaultTime =0;
					if(myUpData.signInFrist ==0)
							myUpData.runMsgTime = 1;//����������Ϣ
				  debugOut("ackBinFualt\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.binFaultFlag =0; //��ʾ���������ݣ�û���յ�ack
				  debugOut("nckBinFualt\r\n");
			}
			else
			{
					myUpData.binFaultFlag =0; //��ʾ���������ݣ�û���յ�ack
				 debugOut("WTFBinFualt\r\n");
			}	
 }


 
/*****************************************����������Ϣ***************************************************/
/*
	����������Ϣ
	ָ��     ������               ����״̬     �ۻ���״̬    ֽ����״̬  Ӳ����״̬  Ӳ������⵽Ӳ������    
	0x7D     0xef 0xee 0xfe       byte1       byte1           byte1       byte1       byte8
	return  0x00 ���������ݣ�û���յ�ack      0x01  �յ���   0x03 ����
	��ʱ����һ�����ӵ� ��Ϣ ����
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
					 
						myUpData.upDataBuff[0]  = 0x7D ; //ָ��
						myUpData.upDataBuff[1]  = 0xef ; //����
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	
						if(1)
						{
							myUpData.upDataBuff[4]  = 0x00 ; //����״̬ �Ź� Ӫҵ
						}
						else
						{
							myUpData.upDataBuff[4]  = 0x03 ; //����״̬ �ſ���ͣ����
						}
						
						myUpData.upDataBuff[5]  = 0x00 ; //�ۻ���״̬			 
						myUpData.upDataBuff[6]  = 0x00 ; //ֽ����״̬
						myUpData.upDataBuff[7]  = 0x00 ; //Ӳ����״̬
					
						myUpData.upDataBuff[8]  = 0x00 ; //Ӳ������⵽Ӳ������
						myUpData.upDataBuff[9]  = 0x00 ;
						myUpData.upDataBuff[10]  = 0x00 ;
						myUpData.upDataBuff[11]  = 0x00 ;
						myUpData.upDataBuff[12]  = 0x00 ;
						myUpData.upDataBuff[13]  = 0x00 ;
						myUpData.upDataBuff[14]  = 0x00 ;
						myUpData.upDataBuff[15]  = 0x00 ;
																													
						myUpData.upDataBuff[16]  = 0x00 ;	//ֽ������
						myUpData.upDataBuff[17]  = 0x00 ;	
						myUpData.upDataBuff[18]  = 0x00 ;	
						myUpData.upDataBuff[19]  = 0x00 ;	
						

					 for(i =0; i<20;i++)
					 {
							myUpData.upDataBuff[20]  += myUpData.upDataBuff[i];	 
					 }
					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,21); 
					 debugOut("sendRunMsg\r\n");
					 myUpData.runMsgFlag =0;   //��ʾ���������ݣ�û���յ�ack
					 myUpData.runMsgTime =1;
					 myUpData.runMsgPollTime =0; 
//					 myUpData.runMsgFlag =1;
//					 myUpData.sysSetTime = 1;   //ϵͳ������Ϣ ��ʱ��ʼ
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
	�豸������Ϣ ���յ�ack
*/ 
void   Up_RunAck(void)
{
			if(usart2_Up.data[1]== 0x00)   //ack
			{
					myUpData.autoNum = 0; 
					myUpData.runMsgFlag = 1;   //��ʾ���������ݣ��յ���ack
					myUpData.runMsgTime = 0;
					if(myUpData.signInFrist ==0)
					   myUpData.sysSetTime = 1;   //ϵͳ������Ϣ ��ʱ��ʼ
				  debugOut("ackRunMsg\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.runMsgFlag =0;    //��ʾ���������ݣ�û���յ�ack
				debugOut("nckRunMsg\r\n");
			}
			else
			{
					myUpData.runMsgFlag =0;    //��ʾ���������ݣ�û���յ�ack
				  debugOut("WTFRunMsg\r\n");
			}	
}

/*****************************************ϵͳ������Ϣ***************************************************/
/*
	ָ��     ������               ������  �չ��ʱ�� �¿�״̬ ����ʱ�� �����¶� �����¶� �����¶� ����
	0x7A     0xef 0xee 0xfe       byte1       byte4   byte1     byte4   byte1    byte1    byte1   byte4
	return  0x00 ���������ݣ�û���յ�ack      0x01  �յ���   0x03 ����
	��ʱ����һ�����ӵ� ��Ϣ ����
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
					 
						myUpData.upDataBuff[0]  = 0x7A ; //ָ��
						myUpData.upDataBuff[1]  = 0xef ; //����
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //����״̬
					
						myUpData.upDataBuff[5]  = 0x99 ; //������ʱ��			 
						myUpData.upDataBuff[6]  = 0x99 ; 
						myUpData.upDataBuff[7]  = 0x99 ; 
						myUpData.upDataBuff[8]  = 0x99 ; 
					
						myUpData.upDataBuff[9]  = 0x99 ; //�¿�״̬
					
						myUpData.upDataBuff[10]  = 0x99 ; //����ʱ��
						myUpData.upDataBuff[11]  = 0x99 ;
						myUpData.upDataBuff[12]  = 0x99 ;
						myUpData.upDataBuff[13]  = 0x99 ;
					
						myUpData.upDataBuff[14]  = 0x00 ; //�����¶�
						myUpData.upDataBuff[15]  = 0x00 ;
					
						myUpData.upDataBuff[16]  = 0x00 ; //�����¶�
						myUpData.upDataBuff[17]  = 0x00 ;
						
//						myUpData.upDataBuff[18]  = 0x00 ; //�����¶�
						
//						myUpData.upDataBuff[19]  = 0x00 ; //����
//						myUpData.upDataBuff[20]  = 0x00 ; 
//						myUpData.upDataBuff[21]  = 0x00 ; 
//						myUpData.upDataBuff[22]  = 0x00 ; 
						
					 for(i =0; i<18;i++)
					 {
						 myUpData.upDataBuff[18]  += myUpData.upDataBuff[i];	 
					 }					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,19); 
					 debugOut("sendSysSet\r\n");
					 myUpData.sysSetFlag =0;            //��ʾ���������ݣ�û���յ�ack
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
					myUpData.sysSetFlag =1;      //��ʾ���������ݣ��յ���ack
					myUpData.sysSetTime =0;
				debugOut("ackSysSet\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.sysSetFlag =0;          //��ʾ���������ݣ�û���յ�ack
				debugOut("nckSysSet\r\n");
			}
			else
			{
					myUpData.sysSetFlag =0;      //��ʾ���������ݣ�û���յ�ack
				debugOut("WTFSysSet\r\n");
			}	
}

/*****************************************pos ״̬��Ϣ***************************************************/
/*
	ָ��     ������               pos״̬ ��0 �ȴ�ˢ�� 1 ˢ����֤ 2 ��Ȩ�ɹ� 3 ��Ȩʧ�ܣ�
	0x81     0xef 0xee 0xfe       byte1       
	return  0x00 ���������ݣ�û���յ�ack      0x01  �յ���   0x03 ����
*/

u8  Up_PosMsg(void)
{
			u8 i =0;
			if(myUpData.posMsgFlag == 0x00)
			{					
					memset(myUpData.upDataBuff,0,dataLength1-1);
				 
					myUpData.upDataBuff[0]  = 0x81 ; //ָ��
					myUpData.upDataBuff[1]  = 0xef ; //����
					myUpData.upDataBuff[2]  = 0xee ;
					myUpData.upDataBuff[3]  = 0xfe ;	

					myUpData.upDataBuff[4]  = 0x00 ;     //pos ��״̬				
				
					 for(i =0; i<5;i++)
					 {
						 myUpData.upDataBuff[5]  += myUpData.upDataBuff[i];	 
					 }					 
					 my_sendstring(USART2,(char *)myUpData.upDataBuff,6); 
					 debugOut("sendPosMsg\r\n");
					 myUpData.sysSetFlag =0;            //��ʾ���������ݣ�û���յ�ack
					 return 0x00;		
			}			
			return 0x01;
}


void  Up_PosMsgAck(void)
{
			if(usart2_Up.data[1]== 0x00)     //ack
			{
					myUpData.posMsgFlag =1;      //��ʾ���������ݣ��յ���ack
					debugOut("ackPosMsg\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.posMsgFlag =0;      //��ʾ���������ݣ�û���յ�ack
				  debugOut("NckPosMsg\r\n");
			}
			else
			{
					myUpData.posMsgFlag =0;      //��ʾ���������ݣ�û���յ�ack
					debugOut("WTFPosMsg\r\n");
			}	
}

/*****************************************pos ״̬��Ϣ***************************************************/
/*
	ָ��     ������               ������      �����Ż���Ϣ
	0x82     0xef 0xee 0xfe       byte1          byte400
	return  0x00 ���������ݣ�û���յ�ack      0x01  �յ���   0x03 ����
*/

u8   Up_preferential(void)
{
				u16 i= 0;
				if(myUpData.PreFlag==0x00)
				{
						memset(myUpData.upDataBuff,0,dataLength1-1);
					 
						myUpData.upDataBuff[0]  = 0x82 ; //ָ��
						myUpData.upDataBuff[1]  = 0xef ; //����
						myUpData.upDataBuff[2]  = 0xee ;
						myUpData.upDataBuff[3]  = 0xfe ;	

						myUpData.upDataBuff[4]  = 0x00 ; //������ 0-16
				 
																							//�����۸���Ϣ byte400   ��ʱ����Ϊ0
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
					 myUpData.PreFlag =0; //��ʾ���������ݣ�û���յ�ack
					 return 0x00;		
				 }
	
	return 0x01;
}


void  Up_PreAck(void)
{
			if(usart2_Up.data[1]== 0x00)     //ack
			{
					myUpData.PreFlag =1;      //��ʾ���������ݣ��յ���ack
				  debugOut("ackpreferen\r\n");
			}
			else if(usart2_Up.data[1]==0x15) //nak
			{
					myUpData.PreFlag =0;      //��ʾ���������ݣ�û���յ�ack
					debugOut("nckpreferen\r\n");
			}
			else
			{
					myUpData.PreFlag =0;      //��ʾ���������ݣ�û���յ�ack
				  debugOut("WTFpreferen\r\n");
			}	
}


/************************************�Զ�����***********************************************************/
/*
		���300msû��ָ��û���յ����ػ���ack ���Զ�����
		���ִ��poll ���������ָ����ˣ��򲻷���pollָ��
		
*/
u8  FuncRet =0;
void  Up_AllCmdAuto(void) 
{
	if(myUpData.autoTime > 300) 
	{
		myUpData.autoTime = 0; 
//		printf("1");
		if(myUpData.autoNum > 30) // 30û���յ�ack ������ǩ��
		{
				debugOut("timeover\r\n");
				myUpData.autoNum = 0;  
				myUpData.signInFrist =0;   //������ѯ���ͱ��� 
				myUpData.signInFlag = 0;   //��¼������ǩ��Э�� ��δ�׵�ack  �������1 ��ʾ�յ�ack
				myUpData.signInTime = 0;
			
				myUpData.binConFlag = 0;     //����������Ϣ
				myUpData.binSetFlag = 0;    
				myUpData.binPreicFlag = 0;
				myUpData.binFaultFlag = 0;
				myUpData.runMsgFlag = 0;
				myUpData.sysSetFlag = 0;
					
				myUpData.outFlag = 0;       //����״̬����			
		}
		
		if(myUpData.runMsgPollTime >30)
		{
				myUpData.runMsgPollTime =0;
				myUpData.runMsgFlag  =0;
				myUpData.runMsgTime  =1;    //��ʼ����һ��
		}
		
		
		FuncRet = Up_autoSignIn();      //û���յ�ack�ظ�����
		if(FuncRet)
		{			
			myUpData.autoNum++;
			return;		
		}
		
		FuncRet = Up_BinConnectionMsg();  //�������״̬���� 
		if(FuncRet)                       //û���յ�ack������ʱ��û��û�з���
		{
			myUpData.autoNum++;
			return; 
		}
		
		FuncRet = Up_BinSetMsg();         //����������Ϣ
		if(FuncRet)                       //û���յ�ack������ʱ��û��û�з���
		{
			myUpData.autoNum++;
			return; 
		}		
		
		FuncRet = Up_BinPrice();          //�����۸���Ϣ
		if(FuncRet)                       //û���յ�ack������ʱ��û��û�з���
		{
			myUpData.autoNum++;
			return; 
		}	
		
		FuncRet = Up_BinFault();          //����������Ϣ
		if(FuncRet)                       //û���յ�ack������ʱ��û��û�з���
		{
			myUpData.autoNum++;
			return; 
		}	
		
		FuncRet = Up_RunMsg();            //����������Ϣ
		if(FuncRet)                       //û���յ�ack������ʱ��û��û�з���
		{
			myUpData.autoNum++;
			return; 
		}		
		
		FuncRet = Up_SysSet();            //ϵͳ������Ϣ
		if(FuncRet)                       //û���յ�ack������ʱ��û��û�з���
		{
			myUpData.autoNum++;
			return; 
		}		
					

		FuncRet = Up_AutoOutStatus();    //����״̬,û���յ�ack�����ط�
		if(FuncRet)											 //û���յ�ack������ʱ��û��û�з���
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
		myUpData.signInFrist =1;        //��¼�Ѿ�������һ�������ı���
	}
}
/*
	 ����2���յ����ݣ������ݽ��з���
	 ��ack  
*/

u8   checkout_i    =0;
u8   checkout_data =0;
void  Up_DataParse(void)
{

		if(usart2_Up.flag)
		{
			 usart2_Up.flag = 0;
			 //printf("%s",usart2_Up.data);
			 //У��
			for(checkout_i =0,checkout_data=0; checkout_i <usart2_Up.count -1; checkout_i++)
			{	
			  checkout_data += usart2_Up.data[checkout_i];
			}
			
			if(checkout_data == usart2_Up.data[usart2_Up.count-1]) //У��ͨ��
			{
							switch (usart2_Up.data[0])
									{
										case 0x74:           //����������Ϣ
												Up_BinConnectionAck();
											break;
										case 0x75:					 //����������Ϣ
											  Up_BinSetAck();
											break;
										case 0x76:					 //poll��ѯ
												Up_PollParse();
											break;
//										case 0x77:				 //
//											break;
										case 0x78:					 //ǩ��
												Up_SignInAck();														
											break;	
										case 0x79:					 //�������ϣ��л��޻���
												Up_BinFaultAck(); 
											break;
										case 0x7A:					 //vmc������Ϣ
											  Up_SysSetAck();
											break;
										case 0x7B:					 //��Ʒ���ۻ�����Ϣ
											break;
										case 0x7C:					 //����
												Up_OutAck();												
											break;
										case 0x7D:           //�������� �����ϣ���Ϣ
											  Up_RunAck();
											break;
										case 0x7F:					 //�����۸���Ϣ
											  Up_BinPriceAck();
											break;
										case 0x81:					 //pos��״̬��Ϣ
												Up_PosMsgAck(); 
											break;
										case 0x82:					 //�����Ż���Ϣ
												Up_PreAck();
											break;
										default:
											break;
									}		
			}
			else
			{	
				//	printf("У�����");
			}	
			usart2_Up.count = 0;
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		}
}
































