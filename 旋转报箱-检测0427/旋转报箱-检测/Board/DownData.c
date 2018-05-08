#include   "DownData.h"
#include   "myUsart.h"
#include   "UpData.h"
#include   "BYJ.h"

struct downstruct  myDownData; //�·��������

char  openResult[25] ={0};
unsigned  int   RxCrc =0;  //�յ����ݵ�crc
unsigned  int   tempRxCrc =0; 
unsigned  char  RxCrc_i =0;


/*
���Ϳ���ָ��� �����壬 ����3  3�ߵ�� �޹�Ļ
01 05 00 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 70 48           9
				cmd  ������   ������   �������     ��۳���  �������   ֧����ʽ    ��ˮ��  У���
strData: 76  03       0x01       0x01        0x01        4byte      0x01       3byte

����¼ ����id  ����id  ���  ֧����ʽ  ��ˮ��
*/

unsigned int tempNumber =0; //��һ����ˮ��
void Down_SendOpen(unsigned char * strData)
{
			unsigned int crctemp =0;
			
			memset(myDownData.downDataBuff,0,Rx3_Length);
	
			myDownData.downDataBuff[0] =  strData[2]+0x01; //  ID  0-16
			myDownData.downDataBuff[1] =   0x05;
			
			myDownData.downDataBuff[2] =   strData[3] - 0x01;      //����  0-59
			myDownData.downDataBuff[3] =   0x03;            //3������
			myDownData.downDataBuff[4] =   0x00;            //3������
			
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
			myDownData.cargoID = myDownData.downDataBuff[2]+1; //����id
			//myDownData.cargoMoney = 1;                       //��Щ��ʱ��û�д���
			myDownData.cargoMoney = strData[5];
			myDownData.cargoMoney = (myDownData.cargoMoney << 8) + strData[6];
			myDownData.cargoMoney = (myDownData.cargoMoney << 8) + strData[7];
			myDownData.cargoMoney = (myDownData.cargoMoney << 8) + strData[8];
			
			myDownData.cargoWay  =  strData[9];
			//��ˮ��  byte3 
			
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
					//myDownData.openTime = 1;    //���䷢�� 7000ms > ��ʱ	 �ȴ�����3 ��������
						
			//�ȿ�������̨��ָ��λ�ã�Ȼ����ת����������ݶ�ÿ�㶼��10�����  myDownData.downDataBuff[2] = 1--60
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
	���ͻ�ȡ���״̬ �����ʱ �������ػ�
	�ӷ���������� 5s ��ʼ���ͣ�200ms ����һ�� ֱ��700ms   7000ms ��ʱ
	z1  0x01 ����  0x01 ִ����  0x02 ִ�����
	z2  �������еĵ����� 00-59
	z3  ִ�н�� 0 �ɹ� 1 ���� 2 Ƿ��  3 ���� 4 ��Ļ�Լ�ʧ�ܣ�δ������� 5 �з����������δ��
	z8-z9 ����ʱ��
	z10 ��Ļ״̬
*/
void   Down_GetStatus(void)
{	
		
	if(myDownData.openTime > 7000) //��ʱ ���ع��ػ�
	{
		myDownData.openTime  = 0;
		Up_OutStatus(myDownData.boardID,myDownData.cargoID,myDownData.cargoMoney,myDownData.cargoWay,0x01,myDownData.cargoNumber);								
//			my_sendstring(USART2,(char *)myDownData.downDataBuff,13);	  //��ʱ����ʧ��
		//printf("%d",myDownData.openTime);
		debugOut("open_timeOver\r\n");
		//��������̨��λ   0-5  
//		Byj_Back((myDownData.cargoID-1) /10);
		return;
	}
	if(myDownData.openTime > 5000)
	{
				if(myDownData.openTime200 >200) //200ms����һ�� ���������ȡ ִ�н��
				{
					myDownData.openTime200 = 1;
					myDownData.downDataBuff[0] = 0x01;  //���
					myDownData.downDataBuff[1] = 0x03;  //ָ��
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
					myDownData.downDataBuff[18] = 0xD0;   //crc  //��ʱ�̶�
					myDownData.downDataBuff[19] = 0xE8;   //					
					my_sendstring(USART3,(char *)myDownData.downDataBuff,20);		
					//printf("%d",myDownData.openTime);
					debugOut("open_getStatus\r\n"); 
				}
	}
}




/*
		 ������� ���
		 7000ms��ʱ
01 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 D0 E8 
	 �ӷ���������� 5s ��ʼ���ͣ�200ms ����һ�� ֱ��700ms   7000ms ��ʱ
	z1  0x01 ����  0x01 ִ����  0x02 ִ�����
	z2  �������еĵ����� 00-59
	z3  ִ�н�� 0 �ɹ� 1 ���� 2 Ƿ��  3 ���� 4 ��Ļ�Լ�ʧ�ܣ�δ������� 5 �з����������δ��
	z8-z9 ����ʱ��
	z10 ��Ļ״̬       &&(myDownData.downDataBuff[2] == usart3_Down.data[3]
*/
//myDownData.openFlag; //��������ɹ� 1 
void  Down_OpenResult(void)
{	
	if(usart3_Down.data[1] == 0x03) //����
	{
			if((usart3_Down.data[2]== 0x02 && usart3_Down.data[4]== 0x00) )//���� ���гɹ�  
			{
				myDownData.openTime =0; myDownData.openTime200 = 0;
				debugOut("tanhuangSecuss\r\n");
//				// ����	0x7c  �����ػ�����֮�����ɹ�
//				Up_OutStatus(myDownData.boardID,myDownData.cargoID,myDownData.cargoMoney,myDownData.cargoWay,0x00,myDownData.cargoNumber);	
//				debugOut("open_succes\r\n");
				
					//�������гɹ�����������̨�½�
//					Byj_Back(myDownData.downDataBuff[2]%10);
				myDownData.openFlag =1;
				
			}
			else //ʧ�� �ݲ������ȴ���ʱ
			{
					
			}
	}
}



/*
		vmc��������ͨѶ
		���������巢�������ݣ�������������ʲô��
		ָ�����-> �ӻ�
    �ӻ���ַ��1byte��  ָ�� ��1byte��   ���ݣ�16byte�� У�飨2byte��
		��Ӧ���ӻ�->����
		������ַ��0x0��    ָ�� ��1byte��   ���ݣ�16byte�� У�飨2byte��
		
*/

void   Down_DataParse(void)
{
		if(usart3_Down.flag)
		{
				usart3_Down.flag =0;
				//my_sendstring(USART3,(char *)usart3_Down.data,3);
			
				//crcУ��
				RxCrc = crcVal(usart3_Down.data,18);
			
			  tempRxCrc = usart3_Down.data[19];
				tempRxCrc = (tempRxCrc << 8) + usart3_Down.data[18];
				
				if(RxCrc == tempRxCrc)  //ͨ��
				{
							
						//�ݲ���������һ����ӷ�����������
					
						switch (usart3_Down.data[1])
										{
											case 0x01: //��ȡID
												break;
											case 0x03: //��ȡ״̬
												Down_OpenResult();
												break;
											case 0x05: //�������
												//Down_OpenResult();
												myDownData.openTime = 1;    //���䷢�� 7000ms > ��ʱ	 �ȴ�����3 ��������
												break;
											case 0x07: //���������
												break;
											case 0x09: //��������
												break;
											case 0xA0: //��������̨
												break;
											case 0xA1: //��ѯ����̨��״̬
												break;
											case 0xFF: //���ð忨��ַ
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
crc-16 У��

���ܣ�    crcУ��
pcMess:   �������ݵ���ָ��
wLen:     �������ݵĳ���
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









