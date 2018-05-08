#ifndef _UPDATA_INIT
#define _UPDATA_INIT

#include "stm32f10x.h"
#include "misc.h"

#define    dataLength1   410

void  debugOut(char * str);

void   Up_PollAck(u8 whatdo);


struct  upstruct
{
		unsigned  char   upDataBuff[dataLength1]; //��Ҫ���͵�buff
		unsigned  char   upDataCount;             //��Ҫ���͵ĳ���
		unsigned  char   signInFlag;              //����ǩ����ָ�0x00  ���ͣ� 0x01�յ�ack
		unsigned  short  signInTime;              //�ż��ʱ��  ms
		unsigned  char   signInFrist;             //��һ��ǩ�� ��Ҫ��ѯ���ͺ���ı��ģ��������һ�κ󣬾Ͳ�����ѯ������
	/*******************************POLL*******************************************************/
		unsigned  short  pollTime;								//300ms����һ��pollָ��
		unsigned  char   pollFlag;                //����ָ��� 1   ���յ�ָ�� ��Ϊ0  
	/**********************************��������ָ��*********************************************/
		unsigned  char   outFlag;                 //��������ָ��ͱ�� 0x01 ��ʾ������ 0x02 ��ʾ�յ���
		unsigned  short  outTime;             		//���ͼ��
		unsigned  char   lastBoardId;
		unsigned  char   lastboxNum;
		unsigned  int    lastMoney;
		unsigned  char   lastWay;
		unsigned  char   lastRet;
		unsigned  int    lastNumber;	
	/****************************************auto time*****************************************************/
		unsigned  short   autoTime;     //200ms  ����һ��
		unsigned  char    autoNum;      //�������͵Ĵ�����Ҳ����û���յ�ack  ���30��û���յ���������ǩ��
	/****************************************bin connection msg***************************************/
		unsigned  char    binConFlag;   // 0x01 ��ʾ���������Ϣ�������յ���ack
		unsigned  short   binConTime;   // 500ms  ����һ��
		unsigned  char    binConBuff[7]; //����������Ϣ 0-2  �����Ƿ�֧����۵�vmc 3-5
	/****************************************bin set msg***************************************/	
		unsigned  char    binSetFlag;   // 0x01   ��ʾ���������������Ϣ�������յ���ack 
		unsigned  short   binSetTime;   // 500ms  ����һ��  
	/****************************************bin price***************************************/	
		unsigned  char    binPreicFlag;   // 0x01  ��ʾ�����˻����۸���Ϣ�������յ���ack	
		unsigned  short   binPreicTime;   //500ms  ����һ��
	/****************************************bin fault***************************************/		
		unsigned  char    binFaultFlag;  //0x01 ��ʾ�����˹�����Ϣ �����յ���ack 
		unsigned  short   binFaultTime;  //500ms ����һ��
	/****************************************run msg***************************************/	
		unsigned  char    runMsgFlag;        // 0x01 ��ʾ�������豸������Ϣ�������յ���ack
		unsigned  short   runMsgTime;        //���û���յ�ack 500ms ����һ��
		unsigned  char    runMsgPollTime;    //30s ��������
	/****************************************sys***************************************/		
		unsigned  char    sysSetFlag;     //0x01  ��ʾ������ ϵͳ������Ϣ�������յ���ack
		unsigned  short   sysSetTime;     //500ms ����һ��
	/****************************************pos***************************************/
		unsigned  char    posMsgFlag;
	/***************************************�Ż���Ϣ***************************************/	
		unsigned  char    PreFlag;      
		unsigned  short   PreTime;   
	//����������Ϣ
		unsigned  char    DoorStatus; //0x
};


extern struct upstruct   myUpData;
/***********************����***************************/

void  Up_SignIn(void);
/*
			�Զ�����ǩ������
			1S ����һ�Σ�֪���յ�ack
*/

u8  Up_autoSignIn(void);

/*
		 �յ�ǩ����ack ���� nak

*/
void  Up_SignInAck(void);



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
*/
u8 Up_Poll(void);





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

void   Up_OutStatus(unsigned char boardId,unsigned char boxNum,unsigned int mymoney ,unsigned char way,unsigned char ret,unsigned int number);













/*
	 ����2���յ����ݣ������ݽ��з���
	��ack  

*/
void  Up_DataParse(void);


/*
		���100msû��ָ��û���յ����ػ���ack ���Զ�����
		���ִ��poll ���������ָ����ˣ��򲻷���pollָ��
*/
void  Up_AllCmdAuto(void);







#endif













