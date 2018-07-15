#include "uart1.h"
#include "stm8l15x_usart.h"
void Uart1_Init(void)
{
  	CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
	GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast);//TXD
  	GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);//RXD
  	USART_DeInit(USART1);		//复位UART1 
	/*
	 * 将UART1配置为：
	 * 波特率 = 115200
	 * 数据位 = 8
	 * 1位停止位
	 * 无校验位
	 * 使能接收和发送
	 */
	USART_Init(USART1, (u32)115200, USART_WordLength_8b, USART_StopBits_1, 
				USART_Parity_No, USART_Mode_Tx|USART_Mode_Rx);
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接收中断
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);//开启接收中断
	USART_Cmd(USART1, ENABLE);	//使能UART2
}

/*******************************************************************************
 * 名称: UART1_SendByte
 * 功能: UART1发送一个字节
 * 形参: data -> 要发送的字节
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void UART1_SendByte(u8 data)
{
  	USART_SendData8(USART1, data);
	/* 等待传输结束 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

/*******************************************************************************
 * 名称: UART1_SendStr
 * 功能: UART1发送len个字符
 * 形参: data -> 指向要发送的字符串
 *       len -> 要发送的字节数
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void UART1_SendStr(u8 *str)
{
	while(*str != '\0')
	{
		UART1_SendByte(*str++);	/* 循环调用发送一个字符函数 */
	}	
}

/*******************************************************************************
 * 名称: UART2_ReceiveByte
 * 功能: UART2接收一个字符
 * 形参: 无
 * 返回: 接收到的字符
 * 说明: 无 
 ******************************************************************************/
u8 UART1_ReceiveByte(void)
{
	u8 UART1_RX_BUF; 
	
	/* 等待接收完成 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	
	UART1_RX_BUF = USART_ReceiveData8(USART1);
	
	return  UART1_RX_BUF;
}

/*******************************************************************************
 * 名称: fputc
 * 功能: 重定向c库函数printf到UART1
 * 形参: 无
 * 返回: 要打印的字符
 * 说明: 由printf调用 
 ******************************************************************************/
#ifdef _IAR_
int fputc(int ch, FILE *f)
{  
	/* 将Printf内容发往串口 */
	UART1_SendByte(ch);
	
	return (ch);
}
#else
PUTCHAR_PROTOTYPE
{
   /* Write a character to the UART1 */
	UART1_SendByte(c);
	return (c);
}
#endif

GETCHAR_PROTOTYPE
{
#ifdef _COSMIC_
   char c = 0;
#else
   int c = 0;
#endif
   /* Loop until the Read data register flag is SET */
   while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
   c = USART_ReceiveData8(USART1);
   return (c);
}

/********************************************************
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 *******************************************************/
static char *itoa(int value, char *string, int radix)
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

/************************************************************************
 * 函数名：UART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-UARTx 串口通道，这里只用到了串口1，即UART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用	 UART1_printf( "\r\n this is a demo \r\n" );
 *            		 UART1_printf(  "\r\n %d \r\n", i );
 *            		 UART1_printf(  "\r\n %s \r\n", j );
 ***************************************************************************/
void UART1_printf(uint8_t *Data,...)
{
	const char *s;
	int d;   
	char buf[16];
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData8(USART1, 0x0d);
					Data ++;
				break;
	
				case 'n':							          //换行符
					USART_SendData8(USART1, 0x0a);	
					Data ++;
				break;
	
				default:
					Data ++;
				break;
			}			 
		}
		else if ( *Data == '%')
		{					//
			switch ( *++Data )
			{				
				case 's':						//字符串
				s = va_arg(ap, const char *);
				for ( ; *s; s++) 
				{
					USART_SendData8(USART1, *s);
					while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				}
				Data++;
				break;	
				case 'd':	//十进制
					d = va_arg(ap, int);
					itoa(d, buf, 10);
				for (s = buf; *s; s++) 
				{
					USART_SendData8(USART1, *s);
					while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				}
				Data++;
				break;			
				default: Data++;
				break;
			}		 
		} /* end of else if */
		else USART_SendData8(USART1, *Data++);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}



