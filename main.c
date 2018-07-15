#include "stm8l15x.h"
#include "main.h"
#include "stm8l_discovery_lcd.h"
#include <math.h>
#include "delay.h"
#include "si4438driver.h"
#include "stm8l15x_iwdg.h"
#include "stm8l15x_gpio.h"
#include "uart1.h"
//-------------------------------------------------
#define cmdCHANNEL 2
#define dataCHANNEL 0
//---------------------------------------------
//声明
#define USE_LSE
#define VREFINT_FACTORY_CONV
#define ADC_CONV 	4096
#define VREFINT_Factory_CONV_ADDRESS ((uint8_t*)0x4910)
#define VREFINT_Factory_CONV_MSB 0x600 /* Le value MSB always 0x600 */
#define VREFINT_Factory_CONV_MIN 0x60 /* Low byte min */
#define VREFINT_Factory_CONV_MAX 0xA0 /* Low byte max */
#define VDD_FACTORY 3.0L
#define VREF 		1.224L


unsigned char abApi_Write[33] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
unsigned char abApi_Read[33] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
unsigned char TX_FIFO[33] = { 0 };
unsigned char RX_FIFO[33] = { 0 };
unsigned char FRRA;
unsigned char FRRB;
unsigned char FRRC;
unsigned char FRRD;
unsigned int LBD;
unsigned char RSSI;
unsigned char ID03[10];//04下来的命令的存放位置
unsigned char CFGData[16];//正常工作时配置数据的XRAM存储区
unsigned char uJ3;
unsigned char DEVICECLASSID;		  //03下面01一级设备类型号
unsigned char flag;
unsigned char flag1;
unsigned char flag2;
unsigned char enRSSI;
unsigned char waitingflag = 0;
unsigned int synchronous_timedelay = 0;
unsigned char syncOK;
unsigned char CFGData[16];//正常工作时配置数据的XRAM存储区
unsigned int WakeCounter;
//全局变量
u16 display_array[6];
volatile bool wakeup_flag = FALSE;
u16 COUNT=0;
volatile u16 value[15];//存放15路ad采样值
u16 TEMPERATURE[15];
float fvTEMPERATURE[15];
unsigned char tmp0;
unsigned char DATAOK;
unsigned char temp18b20format[30];


float temp;
float AVG_SLOPE;
u16 VAL_T90;


/*
¶¼ÑÇºãÐÅ
R25=2.89kohm,B25/50:3520KÈÈÃôµç×èÎÂ¶È¶ÔÕÕ±í
*/
unsigned int TEMPSENSOR1[] = { 0x3ff,		   //1023¿ªÂ·
968, 963, 957, 951, 945,
939, 933, 926, 919, 913,
906, 899, 892, 884, 877,
870, 862, 854, 847, 839,
831, 822, 814, 805, 796,
787, 778, 769, 759, 750,
740, 730, 720, 709, 699,
688, 678, 667, 656, 645,
634, 623, 612, 601, 590,
579, 568, 557, 546, 535,
524, 513, 502, 491, 480,
470, 459, 449, 439, 428,
418, 408, 399, 389, 380,
370, 361, 352, 343, 334,
326, 318, 309, 301, 293,
286, 278, 271, 264, 257,
250, 243, 236, 230, 224,
218, 212, 206, 200, 195,
190, 184, 179, 174, 170,
165, 160, 156, 152, 147,
143, 0 };					 //60,¶ÌÂ·
//函数声明
void GPIO_Initialization(void);
void RTC_Periph_Init(void);
void ADC_Periph_Init(void);
u16 ADCGetTemperature(void);
float Vdd_appli(void);
u16 ADC_Supply(void);
u16 ADCGetMCUTemperature(void);
void ConvertTo18b20Format(unsigned char index);
void GPIO_LowPower_Config(void);
void MCUReset(void);

//主函数开始
void main( void )
{
	int i;
	unsigned char length;
	unsigned char temp;
	unsigned int tmp;
	//上电默认时钟为8MHz
	//将时钟改为16MHz
	CLK->CKDIVR = 0x00; // Fcpu = 16MHz
	//RST->CR = 0xd0;
	tmp0 = RST->CR;
        
        
	GPIO_Initialization(); //i/o脚初始化
        
	/*
        GPIO_ResetBits(D1_PORT, D1_PIN);
        GPIO_SetBits(D1_PORT, D1_PIN);
        delay_ms(50);
	GPIO_ResetBits(D1_PORT, D1_PIN);
        delay_ms(50);
        GPIO_SetBits(D1_PORT, D1_PIN);
        delay_ms(50);
	GPIO_ResetBits(D1_PORT, D1_PIN);
	*/
	RTC_WakeUpCmd(DISABLE);
	WakeCounter = 0; 
        
        
	//关SI4438
	GPIO_SetBits(SDN_PORT, SDN_PIN);
	GPIO_SetBits(nSEL_PORT, nSEL_PIN);
	GPIO_ResetBits(SDO_PORT, SDO_PIN);
	GPIO_SetBits(SDI_PORT, SDI_PIN);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	GPIO_ResetBits(unuse1_PORT, unuse1_PIN);
	GPIO_ResetBits(unuse2_PORT, unuse2_PIN);


	if (1)
	{
		//SDN = 1;
		GPIO_SetBits(D1_PORT, D1_PIN);
		GPIO_SetBits(SDN_PORT, SDN_PIN);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		GPIO_ResetBits(D1_PORT, D1_PIN);
		//GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		//Delayms(5);
		delay_ms(5);
		Reset_4438();
		SI4438Init(48);
	}
	
	SI4438ChangeState(3);  //ready
	SI4438SwitchToRX();
        delay_1us(300);
	//SI4438StartRX(CHANNEL); //??ˉ??¨??￥?”?
	SI4438StartLDC();
	SI4438StartRX(cmdCHANNEL);
	SI4438EnPSMMode(1);
	

	//LCD显示初始化
	//LCD_GLASS_Init();
	//RTC初始化
	RTC_Periph_Init();
	//display_array[0] = '8';
	//display_array[1] = '1';
	//display_array[2] = '2';
	//display_array[3] = '3';
	//display_array[4] = '4';
	//display_array[5] = '8';
	//LCD_GLASS_DisplayStrDeci(display_array);
	//GPIO_ResetBits(LED_BL_PORT, LED_BL_PIN);
	//GPIO_SetBits(LED_BL_PORT, LED_BL_PIN);
	//GPIO_SetBits(LED_GR_PORT, LED_GR_PIN);
	GPIO_ResetBits(D1_PORT, D1_PIN);
	enableInterrupts();
	VAL_T90 = 597;
	AVG_SLOPE = 1.62;
	DATAOK = 0;
	//GPIO_Init(nIRQ_PORT, nIRQ_PIN, GPIO_Mode_In_FL_IT);
	//halt();
	//GPIO_Init(nIRQ_PORT, nIRQ_PIN, GPIO_Mode_In_FL_No_IT);
	IWDG_Enable();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(0xff);
	IWDG_ReloadCounter();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
	
	CLRWDT
          	//RTC_WakeUpCmd(DISABLE);
	//从复位开始到进while循环共560ms，其中包括ldc的启动时间500ms
	while (1)
	{
		CLRWDT
		flag1 = 0;
                		//rtc唤醒一次	
		if (wakeup_flag == TRUE)
		{
			WakeCounter++;
			if (WakeCounter >=40)//10分钟，过一段时间复位一次
			{
 
                         /*
                           GPIO_SetBits(D1_PORT, D1_PIN);
                           		delay_ms(100);
                                        WakeCounter=0;
                                        		Reset_4438();
								SI4438Init(48);
								SI4438ChangeState(3);  //ready
								SI4438SwitchToRX();
                                                                delay_1us(300);
							//	SI4438StartRX(0); //??ˉ??¨??￥?”?
								SI4438StartLDC();
								SI4438StartRX(CHANNEL);
								SI4438EnPSMMode(1);
                                                                GPIO_ResetBits(D1_PORT, D1_PIN);
                          DATAOK = 0;
                          */
					MCURESET
                                                                
                  
			}
			CLRWDT
			//GPIO_Init(NRST_PORT, NRST_PIN, GPIO_Mode_In_FL_No_IT);
			//GPIO_ResetBits(VTSENSOR_PORT, VTSENSOR_PIN);
			//保持SI4438打开
			GPIO_ResetBits(SDN_PORT, SDN_PIN);
			//GPIO_SetBits(LED_GR_PORT, LED_GR_PIN);
			//GPIO_SetBits(LED_BL_PORT, LED_BL_PIN);
			GPIO_SetBits(D1_PORT, D1_PIN);
			wakeup_flag = FALSE;
                       delay_ms(1);
CLRWDT
    
			//RTC_WakeUpCmd(DISABLE);
			//LCD_Cmd(ENABLE);
			//display_array[0] = (COUNT % 100) /10 + 0X30;
			//display_array[1] = COUNT % 10 + 0X30;
			//display_array[2] = ((u16)fvTEMPERATURE[COUNT] % 10000) / 1000 + 0X30;
			//display_array[3] = ((u16)fvTEMPERATURE[COUNT] % 1000) / 100 + 0X30;
			//display_array[4] = ((u16)fvTEMPERATURE[COUNT] % 100) / 10 + 0X30;
			//display_array[5] = ((u16)fvTEMPERATURE[COUNT] % 10) + 0X30;
		//	COUNT++;
			//LCD_GLASS_DisplayStrDeci(display_array);
			//delay_ms(1);
			//GPIO_ResetBits(LED_GR_PORT, LED_GR_PIN);
			//GPIO_ResetBits(LED_BL_PORT, LED_BL_PIN);
			GPIO_ResetBits(D1_PORT, D1_PIN);
			//GPIO_SetBits(SDN_PORT, SDN_PIN);
			
			//LCD_GLASS_Clear();
			//LCD_Cmd(DISABLE);
			//RTC_WakeUpCmd(ENABLE);
		//	if (COUNT >= 15)
		//	{
		//	   COUNT = 0;
		//	}
			CLRWDT
		}	
		//产生外部中断，si4438ldc上收到包后产生中断信号唤醒cpu
		if (DATAOK)
		{
			DATAOK = 0;
			CLRWDT
			if (GPIO_ReadInputDataBit(nIRQ_PORT, nIRQ_PIN) == 0)//只有tx,rx中断
			{
				//产生中断源后，先读中断
				//然后判断中断源，再处理
				CLRWDT
				if (SI4438ReadRXDataFromFIFO())	//结果存于RX_FIFO中
				{
                                  	 SI4438StopLDC();
			                 SI4438ChangeState(3);  //ready
					//2个字节  03ID+03延时的时间                                       
					for (i = 0; i < RX_FIFO[0]; i++)
					ID03[i] = RX_FIFO[i + 1];
					RSSI = FRRC;
					length = RX_FIFO[0];
					ReadCFGDataFromEEPROM();
					uJ3 = 0;
					uJ3 = GPIO_ReadInputDataBit(CFG_PORT, CFG_PIN);
                                        
					if (length == 4)
					{
						//CFGData[0]=0x01;
						if (ID03[0] == CFGData[0]&&(ID03[3]==0||ID03[3]==CFGData[1]))
						{
							i = ID03[2] & 0Xf0;
							if (!i)
							{
								flag2 = 0;//跳出本循环
								RTC_WakeUpCmd(DISABLE);
								//2048对应1秒，307.2对应1ms
								tmp = (u16)(307.2 *CFGData[1]);//150ms的回数间隔
								synchronous_timedelay = 2048 + tmp;
								RTC_SetWakeUpCounter(synchronous_timedelay);
								syncOK = 0;
								RTC_WakeUpCmd(ENABLE);
								flag1 = 1;//核心功能代码
								//GPIO_SetBits(VTSENSOR_PORT, VTSENSOR_PIN);
								i = ID03[2] & 0X0F;
								if (i == 0X01)
								{
									enRSSI = 1;
								}
								else
								{
									enRSSI = 0;
								}
								DEVICECLASSID = ID03[2] & 0XF0;		  //00为01类设备，60为905设备

								synchronous_timedelay = 5500 - (unsigned int)(ID03[1] * 27.2);

								waitingflag = 1;//等待04发完序列
							}
							else
							{
								
                                Reset_4438();
								SI4438Init(48);
								SI4438ChangeState(3);  //ready
								SI4438SwitchToRX();
                                                                delay_1us(300);
								//SI4438StartRX(0); //??ˉ??¨??￥?”?
								SI4438StartLDC();
								SI4438StartRX(cmdCHANNEL);
								SI4438EnPSMMode(1);
								
								MCURESET
                                                                	DATAOK = 0;
							}
						}
						else
						{
							
                            Reset_4438();
							SI4438Init(48);
							SI4438ChangeState(3);  //ready
							SI4438SwitchToRX();
                                                        delay_1us(300);
							//SI4438StartRX(0); //??ˉ??¨??￥?”?
							SI4438StartLDC();
							SI4438StartRX(cmdCHANNEL);
							SI4438EnPSMMode(1);
							
                                                        	DATAOK = 0;
							MCURESET
						}

					}
					/*
					处理03配置命令数据，更新03配置数据
					*/
					
					else if (length == 6)
					{
						if (uJ3)
						{
							uJ3 = 1;
						}
						else
						{
							uJ3 = 0;
						}
						switch (uJ3)
						{
							case 1:
							{
								if (ID03[0] == 0XC1)
								{
									//SI4438Init(48);
									//SI4438ChangeState(3);  //ready
									SI4438StopLDC();
									//给04返数
									TX_FIFO[1] = RSSI;
									TX_FIFO[0] = 1;
									delay_ms(100);
									GPIO_SetBits(D1_PORT, D1_PIN);
									SI4438SendTXFIFOData(dataCHANNEL);
									GPIO_ResetBits(D1_PORT, D1_PIN);
									GPIO_SetBits(D1_PORT, D1_PIN);
									delay_ms(500);
									GPIO_ResetBits(D1_PORT, D1_PIN);
									delay_ms(500);
									GPIO_SetBits(D1_PORT, D1_PIN);
									//复位
									MCURESET
								}
								if (ID03[0] == 0X01)
								{
									/*
									if (RSSI<0X50)
									;//RSTSRC = 0x10;
									SI4438StartRX(CHANNEL);
									CLRWDT
									T0WINDOWFLAG = 4;//200ms
									T0CONbits.TMR0ON = 1;
									T0COUNTER = 0;
									CFGFlag = 1;
									while (1)
									{//等待接收01温度 中断
									if (0x08 != SI4438GetState())
									{
									SI4438StartRX(CHANNEL);
									//USART1SendByte(0x03);
									}
									if (nIRQ == 0)
									{
									//产生中断源后，先读中断
									//然后判断中断源，再处理
									if (SI4438ReadRXDataFromFIFO())	//结果存于RX_FIFO中
									{
									Delayms(25);
									LED = ~LED;
									//进来数据计数清0
									T0COUNTER = 0;
									SI4438StartRX(CHANNEL);
									}
									//如果长时间没有数据进来，则退出
									}//if( NIRQ == 0 )
									CLRWDT
									if (CFGFlag == 0)//超过200ms没有数据下来，可以返数
									break;
									}
									//Delayms(200);
									//给04返数
									for (i = 1; i <= 6; i++)
									{
									TX_FIFO[i] = ID03[i - 1];
									}
									TX_FIFO[7] = RSSI;
									TX_FIFO[0] = 7;
									TRISAbits.TRISA3 = 0;	//MOSEN
									PORTAbits.RA3 = 0;
									Delayms(1);
									LED = 1;
									SI4438SendTXFIFOData(0);
									LED = 0;
									TRISAbits.TRISA3 = 1;	//MOSEN
									PORTAbits.RA3 = 0;
									//存配置数据到FLASH中
									for (i = 1; i<6; i++)
									CFGData[i - 1] = ID03[i];
									SaveCFGDataToFlash();
									LED = 1;
									Delayms(500);
									LED = 0;
									Delayms(500);
									LED = 1;
									//复位
									MCURESET
									*/
								}
								MCURESET
								break;
							}
							case 0:
							{
								if (ID03[0] == 0X81)//短接方式
								{
									//SI4438Init(48);
									//SI4438ChangeState(3);  //ready
									SI4438StopLDC();
									SI4438ChangeState(3);  //ready
									//给04返数
									for (i = 1; i <= 6; i++)
									{
										TX_FIFO[i] = ID03[i - 1];
									}
									TX_FIFO[7] = RSSI;
									TX_FIFO[0] = 7;
									delay_ms(100);
									GPIO_SetBits(D1_PORT, D1_PIN);
									SI4438SendTXFIFOData(dataCHANNEL);
									GPIO_ResetBits(D1_PORT, D1_PIN);
									//存配置数据到FLASH中
									for (i = 1; i <= 3; i++)
										CFGData[i - 1] = ID03[i];
									//先将配置数据存一个备份
									SaveCFGDataToEEPROM2();
									delay_ms(100);
									SaveCFGDataToEEPROM();
									GPIO_SetBits(D1_PORT, D1_PIN);
									delay_ms(500);
									GPIO_ResetBits(D1_PORT, D1_PIN);
									delay_ms(500);
									GPIO_SetBits(D1_PORT, D1_PIN);
									//复位
									MCURESET
								}
								MCURESET
								break;
							}
						}//switch(uJ3)
					} //if(length==6)
					else
					{
						
                        Reset_4438();
						SI4438Init(48);
						SI4438ChangeState(3);  //ready
						SI4438SwitchToRX();
                                                delay_1us(300);
						//SI4438StartRX(0); //??ˉ??¨??￥?”?
						SI4438StartLDC();
						SI4438StartRX(cmdCHANNEL);
						SI4438EnPSMMode(1);
						
                                                 DATAOK = 0;
						MCURESET
					}
				}
				else
				{
					
					//没有收到数据
                    Reset_4438();
					SI4438Init(48);
					SI4438ChangeState(3);  //ready
					SI4438SwitchToRX();
                                        delay_1us(300);
					//SI4438StartRX(0); //??ˉ??¨??￥?”?
					SI4438StartLDC();
					SI4438StartRX(cmdCHANNEL);
					SI4438EnPSMMode(1);
					
                                         DATAOK = 0;
					MCURESET
				}
			}
                         DATAOK = 0;
		}
		//核心功能代码  
		if (flag1 == 1)
		{
			CLRWDT
			//Reset_4438();
			//SI4438StopLDC();
			//SI4438ChangeState(3);  //ready
			//测量15路电压
			//GPIO_SetBits(VTSENSOR_PORT, VTSENSOR_PIN);
			delay_ms(50);
			//ADCGetTemperature();
			//GPIO_ResetBits(VTSENSOR_PORT, VTSENSOR_PIN);
                        
                        /*
			for (i = 0; i < 15; i++)
			{
			   ConvertTo18b20Format(i);
			}
                        */

			//delay_ms(100);
			//等待RTC同步信号
			if (0)
			{
				while (syncOK == 0)
				{
					CLRWDT;
					GPIO_TOGGLE(D1_PORT, D1_PIN);
				}
			}
			else
			{
				GPIO_LowPower_Config();
				GPIO_ResetBits(D1_PORT, D1_PIN);
				GPIO_Init(nIRQ_PORT, nIRQ_PIN, GPIO_Mode_In_FL_IT);
				GPIO_SetBits(nSEL_PORT, nSEL_PIN);
				GPIO_SetBits(SDO_PORT, SDO_PIN);
				GPIO_SetBits(SCLK_PORT, SCLK_PIN);

				halt();
				//	GPIO_Init(nIRQ_PORT, nIRQ_PIN, GPIO_Mode_In_FL_No_IT);
				GPIO_SetBits(D1_PORT, D1_PIN);
				GPIO_DeInit(GPIOB);
				GPIO_DeInit(GPIOD);
				GPIO_Initialization();
			}
			GPIO_SetBits(D1_PORT, D1_PIN);
                        wakeup_flag = FALSE;
			//Reset_4438();
			//SI4438Init(48);//设置射频参数  4800kps
			//SI4438StopLDC();
			//SI4438ChangeState(3);  //ready
			switch (ID03[2] & 0X0F)
			{
			case 0:
			{
					  TX_FIFO[0] = 26;
					  TX_FIFO[1] = 0xfc;
					  TX_FIFO[2] = CFGData[1];
					  for (i = 0; i < 24; i++)
					  {
						  TX_FIFO[i + 3] = temp18b20format[i];
					  }
					  //for (i = 0; i < 12; i++)
					  //{
					  //	TX_FIFO[i * 2 + 3] = TEMPERATURE[i] / 256;
					  //	TX_FIFO[i * 2 + 4] = TEMPERATURE[i] % 256;
					  //}
					  SI4438SendTXFIFOData(dataCHANNEL);
                                           
					  delay_ms(1);
                                           DATAOK = 0;
					  if (CFGData[2] == 0x01)
					  {
						  TX_FIFO[0] = 26;
						  TX_FIFO[1] = 0xfc;
						  TX_FIFO[2] = CFGData[1];
						  for (i = 0; i < 6; i++)
						  {
							  TX_FIFO[i + 3] = temp18b20format[i + 24];
						  }
						  //for (i = 0; i < 3; i++)
						  //{
						  //	TX_FIFO[i * 2 + 3] = TEMPERATURE[i+12] / 256;
						  //	TX_FIFO[i * 2 + 4] = TEMPERATURE[i+12] % 256;
						  //}
						  for (i = 0; i < 9; i++)
						  {
							  TX_FIFO[i * 2 + 9] = 0xff;
							  TX_FIFO[i * 2 + 10] = 0xff;
						  }
						  CLRWDT
							  SI4438SendTXFIFOData(dataCHANNEL);
                                                   
					  }
					  delay_ms(100);
                                           DATAOK = 0;
					  break;
			}
			case 1:
			{
					  TX_FIFO[0] = 26;
					  TX_FIFO[1] = 0xfc;
					  TX_FIFO[2] = CFGData[1];
					  TX_FIFO[3] = RSSI;
					  TX_FIFO[4] = 0XFF;
					  LBD = SI4438GetLBD();
					  TX_FIFO[5] = LBD;
					  for (i = 6; i <= 26; i++)
					  {
						  TX_FIFO[i] = 0XFF;
					  }
					  CLRWDT
					  SI4438SendTXFIFOData(dataCHANNEL);
                                     
					  delay_ms(100);
                                                 DATAOK = 0;
					  break;
			}
			}
			//SI4438StartLDC();
			RTC_WakeUpCmd(DISABLE);
			//RTC_SetWakeUpCounter(2048 * 2);
                        RTC_SetWakeUpCounter(2048*15);
			RTC_WakeUpCmd(ENABLE);
			//没有收到数据
      //                  Reset_4438();
	//		SI4438Init(48);
			//SI4438ChangeState(3);  //ready
			//SI4438SwitchToRX();
   //                     delay_1us(300);
			////SI4438StartRX(2); //??ˉ??¨??￥?”?
			//SI4438StartLDC();
			//SI4438StartRX(2);
			//SI4438EnPSMMode(1);
   //                      DATAOK = 0;
			//采一次复位一次
			MCURESET
		}//if(flag1),核心工作代码结束
		//rtc唤醒一次	
		
                
		////测量15路电压
		//GPIO_SetBits(VTSENSOR_PORT, VTSENSOR_PIN);
		//delay_ms(50);
		//ADCGetTemperature();
		//GPIO_ResetBits(VTSENSOR_PORT, VTSENSOR_PIN);
        if(DATAOK ==0)
        {
			GPIO_LowPower_Config();
			GPIO_ResetBits(D1_PORT, D1_PIN);
			GPIO_Init(nIRQ_PORT, nIRQ_PIN, GPIO_Mode_In_FL_IT);
			GPIO_SetBits(nSEL_PORT, nSEL_PIN);
			GPIO_SetBits(SDO_PORT, SDO_PIN);
			GPIO_SetBits(SCLK_PORT, SCLK_PIN);
                
			halt();
		//	GPIO_Init(nIRQ_PORT, nIRQ_PIN, GPIO_Mode_In_FL_No_IT);
			GPIO_SetBits(D1_PORT, D1_PIN);
			GPIO_DeInit(GPIOB);
			GPIO_DeInit(GPIOD);
			GPIO_Initialization();
        }
	}
}

void GPIO_Initialization(void)
{
	//上电后所有引脚除复位脚外,全为float 输入脚
	//默认都为I/O脚
	GPIO_Init(D1_PORT, D1_PIN, GPIO_Mode_Out_PP_High_Slow);
	GPIO_Init(SDN_PORT, SDN_PIN, GPIO_Mode_Out_PP_Low_Slow);
	GPIO_Init(nSEL_PORT, nSEL_PIN, GPIO_Mode_Out_PP_High_Slow);
	GPIO_Init(SDO_PORT, SDO_PIN, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(SDI_PORT, SDI_PIN, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(SCLK_PORT, SCLK_PIN, GPIO_Mode_Out_PP_High_Fast);
	//GPIO_Init(VTSENSOR_PORT, VTSENSOR_PIN, GPIO_Mode_Out_PP_Low_Slow);
	//GPIO_Init(NRST_PORT, NRST_PIN, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(nIRQ_PORT, nIRQ_PIN, GPIO_Mode_In_FL_IT);
	//GPIO_Init(nIRQ_PORT, nIRQ_PIN, GPIO_Mode_In_FL_No_IT);
	EXTI->CR1 = 0x02; /* PE0 (push-button) ext. interrupt to falling edge low level */
	GPIO_Init(CFG_PORT, CFG_PIN, GPIO_Mode_In_FL_No_IT);
	GPIO_Init(unuse1_PORT, unuse1_PIN, GPIO_Mode_Out_PP_High_Slow);
	GPIO_Init(unuse2_PORT, unuse2_PIN, GPIO_Mode_Out_PP_High_Slow);
	////////////参考代码的初始化过程////////////////////////////////////////////
	/* Push-button initialization */
	//GPIO_Init(BUTTON_GPIO_PORT, FUNCTION_GPIO_PIN, GPIO_Mode_In_FL_IT);
	/* Led ports initialization */
	//GPIO_Init(LED_GR_PORT, LED_GR_PIN, GPIO_Mode_Out_PP_High_Fast);
	//GPIO_Init(LED_BL_PORT, LED_BL_PIN, GPIO_Mode_Out_PP_Low_Fast);
	/* PC0 in output push-pull low because never used by the application */
	//GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Slow);
	//EXTI->CR1 = 0x00; /* PC1 (push-button) ext. interrupt to falling edge low level */
}


void RTC_Periph_Init(void)
{
	RTC_WakeUpCmd(DISABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
#ifdef USE_LSE
	//rtc使用外部32.768kHz晶体
	CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
#else
	CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
#endif
	/* Configures the RTC wakeup timer_step = RTCCLK/16 = LSE/16 = 488.28125 us */
	RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
	//rtc周期为1/16384=0.06103515625ms
	/* Enable wake up unit Interrupt */
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	//1638*0.06103515625=99.97ms
	//0.06103515625*16384=1000ms
	RTC_SetWakeUpCounter(2048*15);
	RTC_WakeUpCmd(ENABLE);
	//下面的代码用处不大
	/*RTC_InitStr.RTC_AsynchPrediv = 0x7f;
	RTC_InitStr.RTC_SynchPrediv = 0x00ff;
	RTC_Init(&RTC_InitStr);*/
}


void ADC_Periph_Init(void)
{
	u8 idx;
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
	//打开adc外设
	ADC1->CR1 |= 0x01; //ADON
	//温度传感器内部参考电源使能
	ADC1->TRIGR[0] = 0x20; //TSON
	for (idx = 0; idx < 50; idx++);
	ADC_Init(ADC1, ADC_ConversionMode_Single,
		ADC_Resolution_12Bit, ADC_Prescaler_1);
	ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_192Cycles);
	ADC_ChannelCmd(ADC1, ADC_Channel_TempSensor, ENABLE);
	ADC1->SQR[0] |= 0x80; //DMA disable
}

u16 ADCGetTemperature(void)
{
	//测量15路的温度数据,测量的结果在TEMPERATURE[15]中
	uint16_t res;
	uint8_t i;
	float fv;
	uint16_t ADCChannel[15] = { ADCin1, ADCin2, ADCin3, ADCin4, ADCin5,
		ADCin6, ADCin7, ADCin8, ADCin9, ADCin10,
		ADCin11, ADCin12, ADCin13, ADCin14, ADCin15 };
	sim(); //关中断
	/* re-start ADC chanel 24 for Current measurement */
	/* 使能adc时钟 */
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
	/*adc寄存器复位*/
	ADC_DeInit(ADC1);
	/*  ADC配置如下:
	- NbrOfChannel = 1 - ADC_Channel_24
	- Mode = Single ConversionMode(ContinuousConvMode disabled)
	- Resolution = 12Bit
	- Prescaler = /1
	- sampling time 159 */
	/*使能ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);
	ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_96Cycles);
	//有15路ad数据
	for (i = 0; i<15; i++)
	{
		//选择测量通道
		ADC_ChannelCmd(ADC1, ADCChannel[i], ENABLE);
		/* disable SchmittTrigger for ADC_Channel_24, to save power */
		ADC_SchmittTriggerConfig(ADC1, ADCChannel[i], DISABLE);
		/* a short time of delay is required after enable ADC */
		delay_1us(30);
		/* Read ADC for current measurmeent */
		/* initialize result */
		/* start ADC convertion by software */
		ADC_SoftwareStartConv(ADC1);
		/* wait until end-of-covertion */
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
		/* read ADC convertion result */
		TEMPERATURE[i]= ADC_GetConversionValue(ADC1);
		ADC_ChannelCmd(ADC1, ADCChannel[i], DISABLE);
	}
	rim(); //开中断
	//测参考电源电压,单位mV
	fv = Vdd_appli();
	//每一路ad采样值转换为电压,单位mV
	for (i = 0; i < 15; i++)
	{
		//测量电压单位mV
		fvTEMPERATURE[i] = TEMPERATURE[i] * fv / ADC_CONV;
	}
	/* Disable ADC 1 for reduce current */
	ADC_Cmd(ADC1, DISABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
	if (0)
	{
		CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
		LCD_GLASS_Init();
	}
	return 1;
}

/**
* @brief Function to return the VDD measurement
* @caller All measurements: VDD display or Current
* See STM8L152x4/6 and STM8L151x4/6 Errata sheet
* Limitation: "Bandgap VREFINT_Factory_CONV byte value not programmed"
*
* 2 Methods for VDD measurement:
* The first one offers a better accuracy
*
* 1st case: The VREFINT is stored in memory during factory tests
* We use this value for better accuracy in this case
*   Vdd_appli = ( VREF_Factory/Vref_measured ) * VDD_Factory
*   VDD_Factory = 3V+-10mV
*   Vref_Factory +-5mV
*
* 2nd case: The VREFINT is not stored in memory.
*   In this case:
*   Vdd_appli = (Theorical_Vref/Vref mesure) * ADC_Converter
*   Theorical_Vref = 1.224V
*   ADC_Converter 4096
*   ---> LSBIdeal = VREF/4096 or VDA/4096
* @param None
* @retval VDD measurements 测量参考电压
*/
float Vdd_appli(void)
{
	uint16_t MeasurINT, FullVREF_FACTORY;
	uint8_t *P_VREFINT_Factory;
	float f_Vdd_appli;
	P_VREFINT_Factory = VREFINT_Factory_CONV_ADDRESS;
	/*Read the BandGap value on ADC converter*/
	//使用外部参考源为基准,测量内部基准的电压对应的采样值
	MeasurINT = ADC_Supply();
	/* To check if VREFINT_Factory_CONV has been set
	the value is one byte we must add 0x600 to the factory byte */
	/* For use VREFINT_Factory_CONV, we must to define VREFINT_FACTORY_CONV (file discover_functions.h */
#ifdef VREFINT_FACTORY_CONV
	if ((*P_VREFINT_Factory>VREFINT_Factory_CONV_MIN) && (*P_VREFINT_Factory<VREFINT_Factory_CONV_MAX))
	{
		/* If the value exists:
		Adds the hight byte to FullVREF_FACTORY */
		FullVREF_FACTORY = VREFINT_Factory_CONV_MSB;
		FullVREF_FACTORY += *P_VREFINT_Factory;
		f_Vdd_appli = (float)(FullVREF_FACTORY*VDD_FACTORY);
		f_Vdd_appli /= MeasurINT;
	}
	else {
		/* If the value doesn't exist (or not correct) in factory setting takes the theorical value 1.224 volt */
		f_Vdd_appli = (VREF / MeasurINT) * ADC_CONV;
	}
#else
	/* We use the theorcial value */
	//内部参考源为1.224V
	//测量原理为ad测量中的参考电压为3V,使用这个3V作为基准,来测量内部参考源
	//的电压,得到的值在MeasurINT中,那么可得到ad的分辨率VREF/MeasurINT
	//ad的分辨力*4096可得到使用的外部参考引脚上的真实电压.
	f_Vdd_appli = (VREF / MeasurINT) * ADC_CONV;
#endif
	/* Vdd_appli in mV */
	f_Vdd_appli *= 1000L;
	//返回值为测到的参考电压,单位mV
	return f_Vdd_appli;
}

/**
* @brief  Read ADC1
* @caller several functions
* @param None
* @retval ADC value
*/
u16 ADC_Supply(void)
{
	//在当前外部参考源上,测量内部基准的ad采样值,之后以此来反推参考电压
	uint8_t i;
	uint16_t res;

	/* Enable ADC clock */
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

	/* de-initialize ADC */
	ADC_DeInit(ADC1);

	/*ADC configuration
	ADC configured as follow:
	- Channel VREF
	- Mode = Single ConversionMode(ContinuousConvMode disabled)
	- Resolution = 12Bit
	- Prescaler = /1
	- sampling time 9 */

	//使能内部电压参考源
	ADC_VrefintCmd(ENABLE);
	delay_1us(30);


	ADC_Cmd(ADC1, ENABLE);
	ADC_Init(ADC1, ADC_ConversionMode_Single,
		ADC_Resolution_12Bit, ADC_Prescaler_1);

	ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_9Cycles);
	//选择内部基准源通路
	ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);
	delay_1us(30);

	/* initialize result */
	res = 0;
	for (i = 8; i>0; i--)
	{
		/* start ADC convertion by software */
		ADC_SoftwareStartConv(ADC1);
		/* wait until end-of-covertion */
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
		/* read ADC convertion result */
		res += ADC_GetConversionValue(ADC1);
	}

	/* de-initialize ADC */
	ADC_VrefintCmd(DISABLE);

	ADC_DeInit(ADC1);

	/* disable SchmittTrigger for ADC_Channel_24, to save power */
	//触发器默认为打开的
	ADC_SchmittTriggerConfig(ADC1, ADC_Channel_24, DISABLE);

	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
	ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE);

	return (res >> 3);
}

u16 ADCGetMCUTemperature(void)
{
	ADC_Periph_Init();
	delay_1us(25); // TS startup time
	//启动ad转换
	ADC_SoftwareStartConv(ADC1);
	//等待ad采样完成
	while (!(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)));
	//读取ad数据
	value[0] = ADC_GetConversionValue(ADC1);
	//认为ad采样的参考电压为3V
	temp = value[0] * 0.7277;
	if (temp > VAL_T90) // temp >90C
	{
		temp = (temp - VAL_T90) / AVG_SLOPE;
		temp = 90 + temp;
	}
	else // temp < 90C
	{
		temp = (VAL_T90 - temp) / AVG_SLOPE;
		temp = 90 - temp;
	}
	return (u16)temp * 10;
}


void ConvertTo18b20Format(unsigned char index)
{
	unsigned char ii;
	unsigned int temp, temp1, num;
	float  temperature = 0.0, diff = 0.0, d1 = 0.0;
	//temp = databuf[index * 2] * 256 + databuf[2 * index + 1];
	temp = TEMPERATURE[index];
	temp = temp >> 2;
	if (temp == 0)
	{
		temperature = 61.0;
	}
	else if (temp == 1023)
	{
		temperature = -41.0;
	}
	else
	{
		for (ii = 0; ii<102; ii++)
		{
			if (temp <= TEMPSENSOR1[101 - ii])
				break;
		}
		num = 101 - ii;
		diff = 1.0 / (TEMPSENSOR1[num] - TEMPSENSOR1[num + 1]);
		d1 = (TEMPSENSOR1[num] - temp)*diff;
		temperature = num - 41.0 + d1;
	}
	if (temperature >= 0)
	{
		temp1 = (unsigned int)(temperature / 0.0625);
	}
	else
	{
		temp1 = ~((unsigned int)(temperature / -0.0625)) + 1;
	}
	temp18b20format[index * 2] = temp1 / 256;
	temp18b20format[index * 2 + 1] = temp1 % 256;
}

/**
* @brief  configure GPIOs before entering low power
* @caller Halt_Init and ADC_Icc_Test
* @param None
* @retval None
*/
void GPIO_LowPower_Config(void)
{

	/* Port A in output push-pull 0 */
	GPIO_Init(GPIOA, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
        
	/* Port B in output push-pull 0 */
	GPIO_Init(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 , GPIO_Mode_In_FL_No_IT); 
        GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 , GPIO_Mode_Out_OD_Low_Slow);

	/* Port C in output push-pull 0 except Button pins */
	GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_In_FL_No_IT);
        GPIO_Init(GPIOC, GPIO_Pin_7 | GPIO_Pin_4,  GPIO_Mode_Out_OD_Low_Slow);
	//GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
	
        /* Port D in output push-pull 0 */
	GPIO_Init(GPIOD, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_In_FL_No_IT);  //GPIO_Mode_In_FL_No_IT
        GPIO_Init(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_7, GPIO_Mode_Out_OD_Low_Slow);

	/* Port E in output push-pull 0 */
	GPIO_Init(GPIOE, GPIO_Pin_4 | GPIO_Pin_5 , GPIO_Mode_Out_PP_Low_Slow);
        GPIO_Init(GPIOE, GPIO_Pin_1 , GPIO_Mode_Out_OD_Low_Slow); 
        GPIO_Init(GPIOE, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7,  GPIO_Mode_Out_OD_Low_Slow);

	/* Port F in output push-pull 0 */
	/* Not PF0 because Input for ICC measurement */
	 GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Slow);  //GPIO_Mode_In_FL_No_IT
	 GPIO_Init(GPIOF, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);

}


void MCUReset(void)
{
	//复位时间实测2.4ms
	GPIO_ResetBits(D1_PORT, D1_PIN);
	RTC_WakeUpCmd(DISABLE);
	//关SI4438
	GPIO_SetBits(SDN_PORT, SDN_PIN);
	GPIO_SetBits(nSEL_PORT, nSEL_PIN);
	GPIO_ResetBits(SDO_PORT, SDO_PIN);
	GPIO_SetBits(SDI_PORT, SDI_PIN);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	IWDG_Enable();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(0x01);
	IWDG_ReloadCounter();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
	GPIO_ResetBits(D1_PORT, D1_PIN);
	while (1)
	{
		//板子上没有灯则看不出电流的变化
		GPIO_SetBits(D1_PORT, D1_PIN);
		delay_ms(500);

	}
}