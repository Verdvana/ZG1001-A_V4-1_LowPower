#include "uart1.h"
#include "stm8l15x_usart.h"
void Uart1_Init(void)
{
  	CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
	GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast);//TXD
  	GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);//RXD
  	USART_DeInit(USART1);		//��λUART1 
	/*
	 * ��UART1����Ϊ��
	 * ������ = 115200
	 * ����λ = 8
	 * 1λֹͣλ
	 * ��У��λ
	 * ʹ�ܽ��պͷ���
	 */
	USART_Init(USART1, (u32)115200, USART_WordLength_8b, USART_StopBits_1, 
				USART_Parity_No, USART_Mode_Tx|USART_Mode_Rx);
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);//���������ж�
	USART_Cmd(USART1, ENABLE);	//ʹ��UART2
}

/*******************************************************************************
 * ����: UART1_SendByte
 * ����: UART1����һ���ֽ�
 * �β�: data -> Ҫ���͵��ֽ�
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void UART1_SendByte(u8 data)
{
  	USART_SendData8(USART1, data);
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

/*******************************************************************************
 * ����: UART1_SendStr
 * ����: UART1����len���ַ�
 * �β�: data -> ָ��Ҫ���͵��ַ���
 *       len -> Ҫ���͵��ֽ���
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void UART1_SendStr(u8 *str)
{
	while(*str != '\0')
	{
		UART1_SendByte(*str++);	/* ѭ�����÷���һ���ַ����� */
	}	
}

/*******************************************************************************
 * ����: UART2_ReceiveByte
 * ����: UART2����һ���ַ�
 * �β�: ��
 * ����: ���յ����ַ�
 * ˵��: �� 
 ******************************************************************************/
u8 UART1_ReceiveByte(void)
{
	u8 UART1_RX_BUF; 
	
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	
	UART1_RX_BUF = USART_ReceiveData8(USART1);
	
	return  UART1_RX_BUF;
}

/*******************************************************************************
 * ����: fputc
 * ����: �ض���c�⺯��printf��UART1
 * �β�: ��
 * ����: Ҫ��ӡ���ַ�
 * ˵��: ��printf���� 
 ******************************************************************************/
#ifdef _IAR_
int fputc(int ch, FILE *f)
{  
	/* ��Printf���ݷ������� */
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
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART1_printf()����
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
 * ��������UART1_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-UARTx ����ͨ��������ֻ�õ��˴���1����UART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��	 UART1_printf( "\r\n this is a demo \r\n" );
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
	
	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData8(USART1, 0x0d);
					Data ++;
				break;
	
				case 'n':							          //���з�
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
				case 's':						//�ַ���
				s = va_arg(ap, const char *);
				for ( ; *s; s++) 
				{
					USART_SendData8(USART1, *s);
					while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
				}
				Data++;
				break;	
				case 'd':	//ʮ����
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



