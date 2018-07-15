#ifndef __UART1_H
#define __UART1_H

/* 包含系统头文件 */
#include <stdio.h>
#include <stdarg.h>
#include "stm8l15x.h"

/* Private define ------------------------------------------------------------*/
#ifdef _RAISONANCE_
#define PUTCHAR_PROTOTYPE int putchar (char c)
#define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
#define PUTCHAR_PROTOTYPE char putchar (char c)
#define GETCHAR_PROTOTYPE char getchar (void)
#else /* _IAR_ */
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */

void Uart1_Init(void);
void UART1_SendByte(u8 data);
void UART1_SendStr(u8 *str);

#ifdef _IAR_
int fputc(int ch, FILE *f);
#endif

static char *itoa(int value, char *string, int radix);

void UART1_printf(uint8_t *Data,...);

#endif

