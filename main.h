/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
//#include "iostm8l052c6.h"

/* Private define ------------------------------------------------------------*/
#define NUM_SAM 5

#define CLRWDT IWDG_ReloadCounter();//喂狗
#define MCURESET	MCUReset();

//STM8L-DIRCOVERY板的引脚配置
#define BUTTON_GPIO_PORT              GPIOC
#define FUNCTION_GPIO_PIN             GPIO_Pin_1

#define LED_GR_PORT              GPIOE
#define LED_GR_PIN               GPIO_Pin_7

#define LED_BL_PORT              GPIOC
#define LED_BL_PIN               GPIO_Pin_7

//ZG1001-A V4.0的引脚分配
/*
//spi引脚
PA2=SDN
PA3=nSEL
PA4=SDI
PA5=SDO
PA6=SCLK
PE0=nIRQ
*/
#define NRST_PORT              GPIOA
#define NRST_PIN               GPIO_Pin_1
#define SDN_PORT              GPIOA
#define SDN_PIN               GPIO_Pin_2
#define nSEL_PORT              GPIOA
#define nSEL_PIN               GPIO_Pin_3
#define SDI_PORT              GPIOA
#define SDI_PIN               GPIO_Pin_5
#define SDO_PORT              GPIOA
#define SDO_PIN               GPIO_Pin_4
#define SCLK_PORT              GPIOA
#define SCLK_PIN               GPIO_Pin_6
#define nIRQ_PORT              GPIOE
#define nIRQ_PIN               GPIO_Pin_0



//指示灯
//PA7=D1
#define D1_PORT              GPIOA
#define D1_PIN               GPIO_Pin_7


//配置引脚
//PE4=CFG
#define CFG_PORT              GPIOE
#define CFG_PIN               GPIO_Pin_4

#define unuse1_PORT              GPIOD
#define unuse1_PIN               GPIO_Pin_7
#define unuse2_PORT              GPIOF
#define unuse2_PIN               GPIO_Pin_0
/*
//ad输入引脚对应关系
//io=对外功能=内部ad通道
PD0=ADC1=ADC1_IN22
PD1=ADC2=ADC1_IN21
PD2=ADC3=ADC1_IN20
PD3=ADC4=ADC1_IN19
PB0=ADC5=ADC1_IN18
PB1=ADC6=ADC1_IN17
PB2=ADC7=ADC1_IN16
PB3=ADC8=ADC1_IN15
PB4=ADC9=ADC1_IN14
PB5=ADC10=ADC1_IN13
PB6=ADC11=ADC1_IN12
PB7=ADC12=ADC1_IN11
PD4=ADC13=ADC1_IN10
PD5=ADC14=ADC1_IN9
PD6=ADC15=ADC1_IN8
*/
#define ADCin1		ADC_Channel_22
#define ADCin2		ADC_Channel_21
#define ADCin3		ADC_Channel_20
#define ADCin4		ADC_Channel_19
#define ADCin5		ADC_Channel_18
#define ADCin6		ADC_Channel_17
#define ADCin7		ADC_Channel_16
#define ADCin8		ADC_Channel_15
#define ADCin9		ADC_Channel_14
#define ADCin10		ADC_Channel_13
#define ADCin11		ADC_Channel_12
#define ADCin12		ADC_Channel_11
#define ADCin13		ADC_Channel_10
#define ADCin14		ADC_Channel_9
#define ADCin15		ADC_Channel_8

#define ADCin1_PORT              GPIOD
#define ADCin1_PIN               GPIO_Pin_0
#define ADCin2_PORT              GPIOD
#define ADCin2_PIN               GPIO_Pin_1
#define ADCin3_PORT              GPIOD
#define ADCin3_PIN               GPIO_Pin_2
#define ADCin4_PORT              GPIOD
#define ADCin4_PIN               GPIO_Pin_3
#define ADCin5_PORT              GPIOB
#define ADCin5_PIN               GPIO_Pin_0
#define ADCin6_PORT              GPIOB
#define ADCin6_PIN               GPIO_Pin_1
#define ADCin7_PORT              GPIOB
#define ADCin7_PIN               GPIO_Pin_2
#define ADCin8_PORT              GPIOB
#define ADCin8_PIN               GPIO_Pin_3
#define ADCin9_PORT              GPIOB
#define ADCin9_PIN               GPIO_Pin_4
#define ADCin10_PORT              GPIOB
#define ADCin10_PIN               GPIO_Pin_5
#define ADCin11_PORT              GPIOB
#define ADCin11_PIN               GPIO_Pin_6
#define ADCin12_PORT              GPIOB
#define ADCin12_PIN               GPIO_Pin_7
#define ADCin13_PORT              GPIOD
#define ADCin13_PIN               GPIO_Pin_4
#define ADCin14_PORT              GPIOD
#define ADCin14_PIN               GPIO_Pin_5
#define ADCin15_PORT              GPIOD
#define ADCin15_PIN               GPIO_Pin_6

/* Private macro -------------------------------------------------------------*/
/* #define BAR0_ON  t_bar[0] |= 0x80
#define BAR0_OFF t_bar[0] &= ~0x80

#define BAR1_ON  t_bar[1]|= 0x08
#define BAR1_OFF t_bar[1]&= ~0x08

#define BAR2_ON  t_bar[0]|=0x20
#define BAR2_OFF t_bar[0]&=~0x20

#define BAR3_ON t_bar[1] |= 0x02
#define BAR3_OFF t_bar[1] &= ~0x02
*/
#define GPIO_HIGH(a,b) a->ODR!=b
#define GPIO_LOW(a,b) a->ODR&=~b
#define GPIO_TOGGLE(a,b) a->ODR^=b


#endif /* __MAIN_H*/