#include "mcudriver.h"
#include "delay.h"
#include  "iostm8l052C6.h"

extern unsigned char CFGData[];//正常工作时配置数据的XRAM存储区
void MCUReset(void);

#define SPI_SCK        		PA_ODR_ODR6
#define SPI_MISO       		PA_IDR_IDR4 //INPUT
#define SPI_MOSI       		PA_ODR_ODR5 
#define SPI_ENABLE   	        PA_ODR_ODR3
//#define SDN			PA_ODR_ODR2
/*
//不能用
unsigned char Spi_Write_one_byte(unsigned char data_in)
{
	u8 bitcnt;

	SPI_SCK = 0;
	for (bitcnt = 8; bitcnt != 0; bitcnt--)
	{
		SPI_SCK = 1;
		asm("nop");
		if (data_in & 0x80)
		{
			SPI_MOSI = 1;
		}
		else
		{
			SPI_MOSI = 0;
		}
		//asm("nop");
		//asm("nop");
		SPI_SCK = 0;
		asm("nop");
		data_in <<= 1;
	}
	SPI_SCK = 0;
	SPI_MOSI = 1;
	return 0;
}
*/

unsigned char Spi_Write_one_byte(unsigned char data_in)
{
	unsigned char i, tmp;
	unsigned char tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	tmp = data_in << 0;
	if (tmp & 0x80)
		tmp0 = 1;
	else
		tmp0 = 0;
	tmp = data_in << 1;
	if (tmp & 0x80)
		tmp1 = 1;
	else
		tmp1 = 0;
	tmp = data_in << 2;
	if (tmp & 0x80)
		tmp2 = 1;
	else
		tmp2 = 0;
	tmp = data_in << 3;
	if (tmp & 0x80)
		tmp3 = 1;
	else
		tmp3 = 0;
	tmp = data_in << 4;
	if (tmp & 0x80)
		tmp4 = 1;
	else
		tmp4 = 0;
	tmp = data_in << 5;
	if (tmp & 0x80)
		tmp5 = 1;
	else
		tmp5 = 0;
	tmp = data_in << 6;
	if (tmp & 0x80)
		tmp6 = 1;
	else
		tmp6 = 0;
	tmp = data_in << 7;
	if (tmp & 0x80)
		tmp7 = 1;
	else
		tmp7 = 0;
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	GPIO_ResetBits(SDO_PORT, SDO_PIN);
	GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp0);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp1);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp2);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp3);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp4);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp5);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp6);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp7);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	GPIO_ResetBits(SDO_PORT, SDO_PIN);
	return 0;
}
/*
unsigned char Spi_Write_one_byte(unsigned char data_in)
{
	unsigned char i, tmp;
	unsigned char tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	tmp = data_in << 0;
	if (tmp & 0x80)
		tmp0 = 1;
	else
		tmp0 = 0;
	tmp = data_in << 1;
	if (tmp & 0x80)
		tmp1 = 1;
	else
		tmp1 = 0;
	tmp = data_in << 2;
	if (tmp & 0x80)
		tmp2 = 1;
	else
		tmp2 = 0;
	tmp = data_in << 3;
	if (tmp & 0x80)
		tmp3 = 1;
	else
		tmp3 = 0;
	tmp = data_in << 4;
	if (tmp & 0x80)
		tmp4 = 1;
	else
		tmp4 = 0;
	tmp = data_in << 5;
	if (tmp & 0x80)
		tmp5 = 1;
	else
		tmp5 = 0;
	tmp = data_in << 6;
	if (tmp & 0x80)
		tmp6 = 1;
	else
		tmp6 = 0;
	tmp = data_in << 7;
	if (tmp & 0x80)
		tmp7 = 1;
	else
		tmp7 = 0;
	SPI_SCK = 0;
	//GPIO_ResetBits(SDO_PORT, SDO_PIN);

		SPI_MOSI = 0;
	//GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp0);
	if (tmp0)
	{
		SPI_MOSI = 1;
	}
	else
	{
		SPI_MOSI = 0;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	//GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp1);
	if (tmp1)
	{
		SPI_MOSI = 1;
	}
	else
	{
		SPI_MOSI = 0;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	//GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp2);
	if (tmp2)
	{
		SPI_MOSI = 1;
	}
	else
	{
		SPI_MOSI = 0;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	//GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp3);
	if (tmp3)
	{
		SPI_MOSI = 1;
	}
	else
	{
		SPI_MOSI = 0;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	//GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp4);
	if (tmp4)
	{
		SPI_MOSI = 1;
	}
	else
	{
		SPI_MOSI = 0;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	//GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp5);
	if (tmp5)
	{
		SPI_MOSI = 1;
	}
	else
	{
		SPI_MOSI = 0;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	//GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp6);
	if (tmp6)
	{
		SPI_MOSI = 1;
	}
	else
	{
		SPI_MOSI = 0;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	//GPIO_WriteBit(SDO_PORT, SDO_PIN, tmp7);
	if (tmp7)
	{
		SPI_MOSI = 1;
	}
	else
	{
		SPI_MOSI = 0;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	//delay_1us(1);
	SPI_SCK = 0;
	//GPIO_ResetBits(SDO_PORT, SDO_PIN);
		SPI_MOSI = 0;
	return 0;
}
*/
/*
不能用
unsigned char Spi_Read_one_byte(void)
{
	unsigned char RdPara = 0;
	unsigned char bitcnt;

	SPI_MOSI = 1;                                                 //Read one byte data from FIFO, MOSI hold to High
	for (bitcnt = 8; bitcnt != 0; bitcnt--)
	{
		SPI_SCK = 0;
		//asm("nop");
		//asm("nop");
		RdPara <<= 1;
		asm("nop");
		SPI_SCK = 1;
		asm("nop");
		if (SPI_MISO)
		{
			RdPara |= 0x01;
		}
		else
		{
			RdPara |= 0x00;
		}
	}
	SPI_SCK = 0;
	return(RdPara);
}
*/

unsigned char Spi_Read_one_byte(void)
{
	unsigned char i, tmp;
	unsigned char tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	tmp0 = 0;
	tmp1 = 0;
	tmp2 = 0;
	tmp3 = 0;
	tmp4 = 0;
	tmp5 = 0;
	tmp6 = 0;
	tmp7 = 0;
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	GPIO_ResetBits(SDO_PORT, SDO_PIN);
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp0 = 1;
	}
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp1 = 1;
	}
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp2 = 1;
	}
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp3 = 1;
	}
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp4= 1;
	}
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp5 = 1;
	}
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp6 = 1;
	}
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	//delay_1us(1);
	GPIO_SetBits(SCLK_PORT, SCLK_PIN);
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp7 = 1;
	}
	GPIO_ResetBits(SCLK_PORT, SCLK_PIN);
	GPIO_ResetBits(SDO_PORT, SDO_PIN);
	tmp = (tmp0 << 7) | (tmp1 << 6) | (tmp2 << 5) | (tmp3 << 4) | (tmp4 << 3) | (tmp5 << 2) | (tmp6 << 1) | (tmp7);
	return tmp;
}
/*
unsigned char Spi_Read_one_byte(void)
{
	unsigned char i, tmp;
	unsigned char tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	tmp0 = 0;
	tmp1 = 0;
	tmp2 = 0;
	tmp3 = 0;
	tmp4 = 0;
	tmp5 = 0;
	tmp6 = 0;
	tmp7 = 0;
	SPI_SCK = 0;
	GPIO_ResetBits(SDO_PORT, SDO_PIN);
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp0 = 1;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp1 = 1;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp2 = 1;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp3 = 1;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp4= 1;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp5 = 1;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp6 = 1;
	}
	SPI_SCK = 0;
	//delay_1us(1);
	SPI_SCK = 1;
	if (GPIO_ReadInputDataBit(SDI_PORT, SDI_PIN))
	{
		tmp7 = 1;
	}
	SPI_SCK = 0;
	GPIO_ResetBits(SDO_PORT, SDO_PIN);
	tmp = (tmp0 << 7) | (tmp1 << 6) | (tmp2 << 5) | (tmp3 << 4) | (tmp4 << 3) | (tmp5 << 2) | (tmp6 << 1) | (tmp7);
	return tmp;
}
*/
/*
CFGData<-EEPROM
*/
unsigned char ReadCFGDataFromEEPROM(void)
{
	unsigned char buf[64];
	unsigned char i;
	unsigned long tmp;
	unsigned char d, e;
	//ReadFlash(CFGDatainFlash, 64, buf);
	//所属03ID
	buf[0] = FLASH_ReadByte(0x1000);
	//01自身ID
	buf[1] = FLASH_ReadByte(0x1001);
	buf[2] = FLASH_ReadByte(0x1002);
	d = FLASH_ReadByte(0x1003);
	e = 0;
	e = buf[0] + e;
	e = buf[1] + e;
	e = buf[2] + e;
	if (buf[0] != 0xff || buf[0] != 0)
	{
		if (d == e)
		{
			for (i = 0; i < 3; i++)
				CFGData[i] = buf[i];
			return 1;
		}
		else
		{
			//校验不通过，从备份位置读出配置数据
			CFGData[0]= FLASH_ReadByte(0x10a0);
			CFGData[1] = FLASH_ReadByte(0x10a1);
			CFGData[2]= FLASH_ReadByte(0x10a2);
			//重写配置数据
			SaveCFGDataToEEPROM();
			return 1;
		}
	}
	else
	{
		CFGData[0] = 0x7f;
		CFGData[1] = 0x01;
		CFGData[2] = 0x00;
		return 0;
	}
}


void SaveCFGDataToEEPROM(void)
{
	unsigned char a, b, c,d;
	//先读取备份的配置数据
	a = FLASH_ReadByte(0x10a0);
	b = FLASH_ReadByte(0x10a1);
	c = FLASH_ReadByte(0x10a2);
	d = 0;
	d = d + a;
	d = d + b;
	d = d + c;
	//备份的配置数据与要修改的配置数据一样才进行修改
	if (a == CFGData[0])
	{
		if (b == CFGData[1])
		{
			if (c == CFGData[2])
			{
				FLASH_Unlock(FLASH_MemType_Data);
				FLASH_ProgramByte(0x1000, CFGData[0]);
				FLASH_ProgramByte(0x1001, CFGData[1]);
				FLASH_ProgramByte(0x1002, CFGData[2]);
				FLASH_ProgramByte(0x1003, d);
				FLASH_Lock(FLASH_MemType_Data);
			}
		}
	}
	
}

void SaveCFGDataToEEPROM2(void)
{
	FLASH_Unlock(FLASH_MemType_Data);
	FLASH_ProgramByte(0x10a0, CFGData[0]);
	FLASH_ProgramByte(0x10a1, CFGData[1]);
	FLASH_ProgramByte(0x10a2, CFGData[2]);
	FLASH_Lock(FLASH_MemType_Data);
}