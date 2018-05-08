#ifndef _DOWNDATA_INIT
#define _DOWNDATA_INIT

#include "stm32f10x.h"
#include "misc.h"


#define    Rx3_Length   100

struct  downstruct
{
		unsigned  char   downDataBuff[Rx3_Length];  //��Ҫ���͵�buff
		unsigned  char   downDataCount;             //��Ҫ���͵ĳ���
		unsigned  char   downOpenFlag;              //0 �����ɹ�  1 �Ļ����޻����߹��� 2����ԭ��û�г��� 3 ���Ĳ��� 4 ֧��ʧ��
//		unsigned  short  signInTime;                //�ż��ʱ��  ms
		unsigned  short  openTime;								  //�������� ��ʱʱ��    7000ms
		unsigned  short  openTime200;               //200ms ����һ�� ��ȡ����״̬
		unsigned  char   boardID;                   //������
		unsigned  char   cargoID;                   //�������
		unsigned  int    cargoMoney;                //�۸�
		unsigned  char   cargoWay;									//֧����ʽ
		unsigned  int    cargoNumber;								//��ˮ��
		unsigned  char   openFlag;                  //���ɵ�������ɹ����  1��ʾ�ɹ�
	
	
	
	
//		unsigned  char   lastBoardId;
//		unsigned  char   lastboxNum;
//		unsigned  int    lastMoney;
//		unsigned  char   lastWay;
//		unsigned  char   lastRet;
//		unsigned  int    lastNumber;
};


extern  struct downstruct  myDownData;



/*************************����***************************/
/*
	crc-16 У��

���ܣ�   crcУ��
pcMess:  �������ݵ���ָ��
wLen:    �������ݵĳ���
*/


unsigned int crcVal(unsigned char *pcMess,unsigned int wLen);

void   Down_SendOpen(unsigned char * strData);




/*
	���ͻ�ȡ���״̬ �����ʱ �������ػ�

	 �ӷ���������� 5s ��ʼ���ͣ�200ms ����һ�� ֱ��700ms   7000ms ��ʱ
	z1  0x01 ����  0x01 ִ����  0x02 ִ�����
	z2  �������еĵ����� 00-59
	z3  ִ�н�� 0 �ɹ� 1 ���� 2 Ƿ��  3 ���� 4 ��Ļ�Լ�ʧ�ܣ�δ������� 5 �з����������δ��
	z8-z9 ����ʱ��
	z10 ��Ļ״̬
*/

void   Down_GetStatus(void);




/*
		vmc��������ͨѶ
		���������巢�������ݣ�������������ʲô��
		ָ�����-> �ӻ�
    �ӻ���ַ��1byte��  ָ�� ��1byte��   ���ݣ�16byte�� У�飨2byte��
		��Ӧ���ӻ�->����
		������ַ��0x0��    ָ�� ��1byte��   ���ݣ�16byte�� У�飨2byte��
		
*/

void   Down_DataParse(void);

#endif













