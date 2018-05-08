#include   "KDG.h"
#include   "string.h"
#include   <stdarg.h>
#include   "myUsart.h"
#include   "BYJ.h"


void USART_printf ( USART_TypeDef * USARTx, char * Data, ... );

static u8  CMD[15][10]={

	{0x0c, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x02},  //反馈左边锁板
	{0x0c, 0x55, 0x00, 0x00, 0x00, 0x00, 0x02},  //反馈右边锁板
	{0x0D, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x02},  //打开左边继电器
	{0x0D, 0x55, 0x00, 0x00, 0x00, 0x00, 0x02},  //打开右边继电器
	{0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},  //掉电检测指令
	{0x16, 0x57, 0x00, 0x00, 0x00, 0x00, 0x02},
	{0x17, 0x52, 0x00, 0x00, 0x00, 0x00, 0x02},
	{0x0D, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x02}	
};



u8  LeftMsg[45]  ={0x0c,0xaa,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00};
u8  RightMsg[45] ={0x0c,0x55,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00};
u8 Power[4] ={0x14,0xAA,0x00};

u8  LeftOpen[10]={0x0D,0xAA,0xAA,0x55,0x0D,0x00};
u8  RightOpen[10]={0x0D,0x55,0xAA,0x55,0x0D,0x00};

u16 OpenNum =0;
void  CmdDo(u8 i)
{

	switch(i)
	{
		case 0x00:
				my_sendstring(USART2,(char *)LeftMsg,40);			
			break;
		case 0x01:
				 my_sendstring(USART2,(char *)RightMsg,40);	 
			break;
		case 0x02:
					my_sendstring(USART2,(char *)LeftOpen,5);	 //
					OpenNum = ((usart2_Up.data[3]&0xF0)>>4)*10 + (usart2_Up.data[3]&0xF);
					Byj_GoTo(OpenNum);
			break;
		case 0x03:
					my_sendstring(USART2,(char *)RightOpen,5);	
			break;
		case 0x04:
			  my_sendstring(USART2,(char *)Power,2);	
		break;
		default:
			break;	
	}
}

u8 i =0;
//分析指令 动作
void  RetCmdNum(void)
{
		if(usart2_Up.flag)
		{
			usart2_Up.flag =0;
			for(i=0; i<10; i++)
			{
						if(!memcmp(usart2_Up.data,CMD[i],CMD[i][6]))
						{
								//return i;
								CmdDo(i);
						}	
			}
			usart2_Up.count = 0;
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		}			
	//return 10;
}



/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART2_printf()调用
 */
static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
		
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */




/*
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
char ch1 =0;
void USART_printf ( USART_TypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];

	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //换行符
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;

				default:
				Data ++;
				break;
			}			 
		}
		
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
				s = va_arg(ap, const char *);
				
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;

				case 'd':			
					//十进制
				d = va_arg(ap, int);
				
				itoa(d, buf, 10);
				
				for (s = buf; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;
				
//				case 'c':
//					
//					for ( ; *s; s++) 
//					{
//						 ch1= va_arg(ap,char);   
//						USART_SendData(USARTx,ch1);
//						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
//					}
//				break;					
				default:
				Data++;
				break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
	}
}




void Kdg_Poll(void)
{
	RetCmdNum();

}








