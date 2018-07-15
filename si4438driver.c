//#include "boarddefinition.h"
#include "mcudriver.h"
#include "si4438driver.h"


//#define Power_up_4438()			 SDN=0	 //yellow LED is on
//#define Shut_down_4438()		 SDN=1	 //off

//#define Enable_4438()	         nSEL=0	//Blue_LED is On
//#define Disable_4438()			 nSEL=1	//Blue_LED is OFF
//si4463,4800bps
/*
	ram unsigned char DATA_ARRAY4800_1[]={0x10, RF_4800PKT_LEN_12};
	ram unsigned char DATA_ARRAY4800_2[]={0x10, RF_4800MODEM_MOD_TYPE_12};
	ram unsigned char DATA_ARRAY4800_3[]={0x05, RF_4800MODEM_FREQ_DEV_0_1};
	ram unsigned char DATA_ARRAY4800_4[]={0x0C, RF_4800MODEM_TX_RAMP_DELAY_8};
	ram unsigned char DATA_ARRAY4800_5[]={0x0D, RF_4800MODEM_BCR_OSR_1_9};
	ram unsigned char DATA_ARRAY4800_6[]={0x0B, RF_4800MODEM_AFC_GEAR_7};
	ram unsigned char DATA_ARRAY4800_7[]={0x07, RF_4800MODEM_AGC_WINDOW_SIZE_3};
	ram unsigned char DATA_ARRAY4800_8[]={0x05, RF_4800MODEM_OOK_PDTC_1};
	ram unsigned char DATA_ARRAY4800_9[]={0x0F, RF_4800MODEM_OOK_CNT1_11};
	ram unsigned char DATA_ARRAY4800_10[]={0x10, RF_4800MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
	ram unsigned char DATA_ARRAY4800_11[]={0x10, RF_4800MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12};
	ram unsigned char DATA_ARRAY4800_12[]={0x10, RF_4800MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12};
	ram unsigned char DATA_ARRAY4800_13[]={0x0B, RF_4800SYNTH_PFDCP_CPFF_7};
//si4463,115200bps
	ram unsigned char DATA_ARRAY115200_1[]={0x10, RF_115200PKT_LEN_12};
	ram unsigned char DATA_ARRAY115200_2[]={0x10, RF_115200MODEM_MOD_TYPE_12};
	ram unsigned char DATA_ARRAY115200_3[]={0x05, RF_115200MODEM_FREQ_DEV_0_1};
	ram unsigned char DATA_ARRAY115200_4[]={0x0C, RF_115200MODEM_TX_RAMP_DELAY_8};
	ram unsigned char DATA_ARRAY115200_5[]={0x0D, RF_115200MODEM_BCR_OSR_1_9};
	ram unsigned char DATA_ARRAY115200_6[]={0x0B, RF_115200MODEM_AFC_GEAR_7};
	ram unsigned char DATA_ARRAY115200_7[]={0x07, RF_115200MODEM_AGC_WINDOW_SIZE_3};
	ram unsigned char DATA_ARRAY115200_8[]={0x05, RF_115200MODEM_OOK_PDTC_1};
	ram unsigned char DATA_ARRAY115200_9[]={0x0F, RF_115200MODEM_OOK_CNT1_11};
	ram unsigned char DATA_ARRAY115200_10[]={0x10, RF_115200MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
	ram unsigned char DATA_ARRAY115200_11[]={0x10, RF_115200MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12};
	ram unsigned char DATA_ARRAY115200_12[]={0x10, RF_115200MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12};
	ram unsigned char DATA_ARRAY115200_13[]={0x0B, RF_115200SYNTH_PFDCP_CPFF_7};
*/
extern unsigned int wDelay;
extern unsigned char abApi_Write[];
extern unsigned char abApi_Read[];		  
//const far rom 
unsigned char radio_configuration_data4800[]=RADIO_CONFIGURATION_DATA_ARRAY4800;
//ram unsigned char radio_configuration_data115200[]=RADIO_CONFIGURATION_DATA_ARRAY115200;
extern unsigned char TX_FIFO[];
extern unsigned char RX_FIFO[];
extern unsigned char FRRA;
extern unsigned char FRRB;
extern unsigned char FRRC;
extern unsigned char FRRD;
extern unsigned int LBD;
extern unsigned char u8LBD;


unsigned int wDelay; // 延时代码中使用的变量
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +
  + FUNCTION NAME:  unsigned char bSpi_SendDataNoResp(unsigned char bDataInLength, unsigned char *pbDataIn)
  +
  + DESCRIPTION:    send data over SPI no response expected
  +
  + INPUT:          bDataInLength - length of data
  +                 *pbDataIn     - pointer to the data
  +
  + RETURN:         None
  +
  + NOTES:          None
  +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
unsigned char bSpi_SendDataNoResp(unsigned char bDataInLength, unsigned char *pbDataIn)
{
  unsigned char bCnt;

  for (bCnt=0; bCnt<bDataInLength; bCnt++)    // Send input data array via SPI
  {
    Spi_Write_one_byte(pbDataIn[bCnt]);
  }
  return 0;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +
  + FUNCTION NAME:  unsigned char bSpi_SendDataGetResp(unsigned char bDataOutLength, unsigned char *pbDataOut)
  +
  + DESCRIPTION:    send dummy data over SPI and get the response
  +
  + INPUT:          bDataOutLength  - length of data to be read
  +                 *pbDataOut      - response
  +
  + RETURN:         None
  +
  + NOTES:          None
  +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
unsigned char bSpi_SendDataGetResp(unsigned char bDataOutLength,unsigned char *pbDataOut)   // Send data, get the response
{
  // send command and get response from the radio IC
 
   //Note that commands without any input parameters require a dummy byte for proper CTS responses.
   	unsigned char SPI_cnt ;
	
	if(bDataOutLength == 1)	  //只需要获取一个输出值，pbDataout是作为一个U8变量传入的
	 {
	     //*pbDataOut= Spi0ReadWrite(0xAA);
		  return 0;
	
	 }
	
	 else	   //需要获取多个输出值的时候，pbDataout是作为一个数组名传入的
	 {
	 	  for(SPI_cnt=0;SPI_cnt<bDataOutLength;SPI_cnt++)
		  {
		  //	  pbDataOut[SPI_cnt] = Spi0ReadWrite(0xAA);
		  }
		  return 0;
		 
	 }
  
 
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +
  + FUNCTION NAME:  unsigned char bSpi_SendDataByte(unsigned char bDataIn)
  +
  + DESCRIPTION:    send one byte via SPI
  +
  + INPUT:          bDataIn - data to be sent
  +
  + RETURN:         None
  +
  + NOTES:          None
  +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
unsigned char bSpi_SendDataByte(unsigned char bDataIn)      // Send a single byte via SPI
{
  //Spi0ReadWrite(bDataIn);
  return 0;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +
  + FUNCTION NAME:  unsigned char bApi_GetResponse(unsigned char bRespLength, unsigned char *pbRespData)
  +
  + DESCRIPTION:    wait for CTS and get the read bytes from the chip
  +
  + INPUT:          bRespLength - nmbr of bytes to be read
  +                 *pbRespData - pointer to the read data
  +
  + RETURN:         0 - operation successful
  +                 1 - no CTS within MAX_CTS_RETRY
  +
  + NOTES:          None
  +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  //该函数用于获取命令流发送完后的返回值流，返回值存储在pbRespData 指向的位置

unsigned char bApi_GetResponse(unsigned char bRespLength) // Get response from the chip (used after a command)
{

  unsigned char CtsValue = 0;
  unsigned int bErrCnt = 0;
  unsigned char bCnt=0;
	unsigned char i;
	for(i=0;i<20;i++)
	{
		delay_ms(1);
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN);      // Select radio IC by pulling its nSEL pin low
		Spi_Write_one_byte(CMD_CTS_READ);
		CtsValue=Spi_Read_one_byte();
		if(CtsValue==0xff)
		{
			for(wDelay=0; wDelay<bRespLength; wDelay++)
			{
				abApi_Read[wDelay]=Spi_Read_one_byte();
			} 
			GPIO_SetBits(nSEL_PORT, nSEL_PIN);
			return bRespLength	;//返回收到数据的长度
			break;
		}
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
	}
	return 0;
}

//该函数不对接收到第一个SPI数据是否是CTS进行检测
unsigned char bApi_GetResponse_without_CTS_detection(unsigned char bRespLength, unsigned char *pbRespData)
{
  unsigned char cnt=0;
  for(cnt=0; cnt<bRespLength; cnt++)
  {	
	  pbRespData[cnt]=Spi_Read_one_byte();
  }
  return 0;  
}

unsigned char bApi_WriteTxDataBuffer(unsigned char bTxFifoLength, unsigned char *pbTxFifoData)   // Write Tx FIFO
{
GPIO_ResetBits(nSEL_PORT, nSEL_PIN);  
//GPIO_ResetBits(nSEL_PORT, nSEL_PIN);                                // Select radio IC by pulling its nSEL pin low
  Spi_Write_one_byte(CMD_TX_FIFO_WRITE);                 // Send Tx write command
  bSpi_SendDataNoResp(bTxFifoLength, pbTxFifoData); // Write Tx FIFO
GPIO_SetBits(nSEL_PORT, nSEL_PIN);  
//GPIO_SetBits(nSEL_PORT, nSEL_PIN);                                     // De-select radio IC by putting its nSEL pin high
  return 0;
}

unsigned char bApi_ReadRxDataBuffer(unsigned char bRxFifoLength, unsigned char *pbRxFifoData)
{
GPIO_ResetBits(nSEL_PORT, nSEL_PIN);  
//GPIO_ResetBits(nSEL_PORT, nSEL_PIN);                                       // Select radio IC by pulling its nSEL pin low
  Spi_Write_one_byte(CMD_RX_FIFO_READ);                // Send Rx read command
  bApi_GetResponse_without_CTS_detection(bRxFifoLength, pbRxFifoData);  // Write Tx FIFO
  //GPIO_SetBits(nSEL_PORT, nSEL_PIN);                                        // De-select radio IC by putting its nSEL pin high
GPIO_SetBits(nSEL_PORT, nSEL_PIN);  
return 0;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +
  + FUNCTION NAME:  unsigned char bApi_SendCommand(unsigned char bCmdLength, unsigned char *pbCmdData)
  +
  + DESCRIPTION:    send API command, no response expected
  +
  + INPUT:          bCmdLength - nmbr of bytes to be sent
  +                 *pbCmdData - pointer to the commands
  +
  + RETURN:         None
  +
  + NOTES:          None
  +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
unsigned char bApi_SendCommand(unsigned char bCmdLength, unsigned char *pbCmdData)   // Send a command + data to the chip
{
  //GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
GPIO_ResetBits(nSEL_PORT, nSEL_PIN);

  bSpi_SendDataNoResp(bCmdLength, pbCmdData);   // Send data array to the radio IC via SPI
  //GPIO_SetBits(nSEL_PORT, nSEL_PIN);
GPIO_SetBits(nSEL_PORT, nSEL_PIN);
  return 0;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +
  + FUNCTION NAME:  unsigned char bSendPacket(unsigned char bLength, unsigned char *abPayload)
  +
  + DESCRIPTION:    Send a packet
  +
  + INPUT:      Payload to be transmitted and its length
  +
  + RETURN:         Zero
  +
  + NOTES:          None
  +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
unsigned char bSendPacket(unsigned char bLength, unsigned char *abPayload,unsigned char ch)
{
	int i;
	SI4438SwitchToTX();
	bApi_WriteTxDataBuffer(bLength, abPayload);   // Write data to Tx FIFO
	vApi_WaitforCTS();                // Wait for CTS
	// Set TX packet length
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PKT_GROUP;        // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_PKT_FIELD_1_LENGTH_7_0; // Specify first property
	abApi_Write[4] = bLength;           // Field length (variable packet length info)
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	//++++++++++获取中断寄存器的当前值并清除挂起的中断++++++++++++++++
	// Read ITs, clear pending ones
	abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
	abApi_Write[1] = 0;               // Clear PH_CLR_PEND
	abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
	abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
	bApi_SendCommand(4,abApi_Write);
	vApi_WaitforCTS();                // Wait for CTS
	// Start Tx
	abApi_Write[0] = CMD_START_TX;      // Use Tx Start command
	abApi_Write[1] = ch;           // Set channel number
	abApi_Write[2] = 0x30;          // Ready state after Tx, start Tx immediately
	abApi_Write[3] = 0x00;          // Packet fields used, do not enter packet length here
	abApi_Write[4] = 0x00;          // Packet fields used, do not enter packet length here
	bApi_SendCommand(5,abApi_Write);    // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	// Wait for packet sent interrupt
	i = 0;
	while (GPIO_ReadInputDataBit(nIRQ_PORT, nIRQ_PIN) == 1)
	{
		delay_ms(1);
		i++;
		if (i >= 500)
		{
			break;
		}
	}
	//++++++++++获取中断寄存器的当前值并清除挂起的中断++++++++++++++++
	// Read ITs, clear pending ones
	abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
	abApi_Write[1] = 0;               // Clear PH_CLR_PEND
	abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
	abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
	bApi_SendCommand(4,abApi_Write);
	vApi_WaitforCTS();                // Wait for CTS
	SI4438SwitchToRX();
	return 0;
}

void Reset_4438(void)
{
   // Reset the radio
  //Shut_down_4438();
GPIO_SetBits(SDN_PORT, SDN_PIN);
  // Wait ~300us (SDN pulse width)
  delay_1us(100);
delay_1us(100);
delay_1us(100);
delay_1us(100);
delay_ms(5);
  // Wake up the chip from SDN
  //Power_up_4438(); 
GPIO_ResetBits(SDN_PORT, SDN_PIN);
 
   // Wait for POR (power on reset); ~5ms
  delay_ms(5);


}

void SI4438SetConfiguration(unsigned int bps)
{
	//设置4463配置数据

	//先放不同波特率公共的配置
	//#define RF_4800POWER_UP 0x02, 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80
	abApi_Write[0] = 0x02;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x01; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x00;              // Write global control registers
	abApi_Write[3] = 0x01;
	abApi_Write[4] = 0xC9;
	abApi_Write[5] = 0xC3;
	abApi_Write[6] = 0x80;
	bApi_SendCommand(7,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800GPIO_PIN_CFG 0x13, 0x20, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00
	abApi_Write[0] = 0x13;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x21;              // Write global control registers
	abApi_Write[3] = 0x16;
	abApi_Write[4] = 0x17;
	abApi_Write[5] = 0x00;
	abApi_Write[6] = 0x00;
	bApi_SendCommand(7,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//efine RF_4800GLOBAL_XO_TUNE_1 0x11, 0x00, 0x01, 0x00, 0x52
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x00; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x01;              // Write global control registers
	abApi_Write[3] = 0x00;
	abApi_Write[4] = 0x52;
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();	
	//#define RF_4800GLOBAL_CONFIG_1 0x11, 0x00, 0x01, 0x03, 0x60
	abApi_Write[0] = 0x01;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x00; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x01;              // Write global control registers
	abApi_Write[3] = 0x03;
	abApi_Write[4] = 0x60;
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800INT_CTL_ENABLE_2 0x11, 0x01, 0x02, 0x00, 0x01, 0x38
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x01; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x02;              // Write global control registers
	abApi_Write[3] = 0x00;
	abApi_Write[4] = 0x01;
	abApi_Write[5] = 0x38;
	bApi_SendCommand(6,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800FRR_CTL_A_MODE_4 0x11, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x02; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x04;              // Write global control registers
	abApi_Write[3] = 0x00;
	abApi_Write[4] = 0x00;
	abApi_Write[5] = 0x00;
	abApi_Write[6] = 0x00;
	abApi_Write[7] = 0x00;
	bApi_SendCommand(8,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800PREAMBLE_TX_LENGTH_9 0x11, 0x10, 0x09, 0x00, 0x05, 0x14, 0x00, 0x0F, 0x31, 0x00, 0x00, 0x00, 0x00
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x10; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x09;              // Write global control registers
	abApi_Write[3] = 0x00;
	abApi_Write[4] = 0x05;
	abApi_Write[5] = 0x14;
	abApi_Write[6] = 0x00;
	abApi_Write[7] = 0x0f;
	abApi_Write[8] = 0x31;
	abApi_Write[9] = 0x00;
	abApi_Write[10] = 0x00;
	abApi_Write[11] = 0x00;
	abApi_Write[12] = 0x00;
	bApi_SendCommand(13,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800SYNC_CONFIG_5 0x11, 0x11, 0x05, 0x00, 0x01, 0xB4, 0x2B, 0x00, 0x00
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x11; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x05;              // Write global control registers
	abApi_Write[3] = 0x00;
	abApi_Write[4] = 0x01;
	abApi_Write[5] = 0xb4;
	abApi_Write[6] = 0x2b;
	abApi_Write[7] = 0x00;
	abApi_Write[8] = 0x00;
	bApi_SendCommand(9,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();	
	//#define RF_4800PKT_CRC_CONFIG_1 0x11, 0x12, 0x01, 0x00, 0x84
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x12; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x01;              // Write global control registers
	abApi_Write[3] = 0x00;
	abApi_Write[4] = 0x84;
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800PKT_WHT_SEED_15_8_4 0x11, 0x12, 0x04, 0x03, 0xFF, 0xFF, 0x00, 0x02
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x12; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x04;              // Write global control registers
	abApi_Write[3] = 0x03;
	abApi_Write[4] = 0xff;
	abApi_Write[5] = 0xff;
	abApi_Write[6] = 0x00;
	abApi_Write[7] = 0x02;
	bApi_SendCommand(8,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800PKT_FIELD_2_CRC_CONFIG_12 0x11, 0x12, 0x0C, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x12; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x0c;              // Write global control registers
	abApi_Write[3] = 0x14;
	abApi_Write[4] = 0x00;
	abApi_Write[5] = 0x00;
	abApi_Write[6] = 0x00;
	abApi_Write[7] = 0x00;
	abApi_Write[8] = 0x00;
	abApi_Write[9] = 0x00;
	abApi_Write[10] = 0x00;
	abApi_Write[11] = 0x00;
	abApi_Write[12] = 0x00;
	abApi_Write[13] = 0x00;
	abApi_Write[14] = 0x00;
	abApi_Write[15] = 0x00;
	abApi_Write[16] = 0x00;
	abApi_Write[17] = 0x00;
	abApi_Write[18] = 0x00;
	bApi_SendCommand(19,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800PKT_FIELD_5_CRC_CONFIG_1 0x11, 0x12, 0x01, 0x20, 0x00
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x12; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x01;              // Write global control registers
	abApi_Write[3] = 0x20;
	abApi_Write[4] = 0x00;
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800MODEM_AGC_CONTROL_1 0x11, 0x20, 0x01, 0x35, 0xE2
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x01;              // Write global control registers
	abApi_Write[3] = 0x35;
	abApi_Write[4] = 0xe2;
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800MODEM_RSSI_COMP_1 0x11, 0x20, 0x01, 0x4E, 0x40
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x01;              // Write global control registers
	abApi_Write[3] = 0x4e;
	abApi_Write[4] = 0x40;
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800MODEM_CLKGEN_BAND_1 0x11, 0x20, 0x01, 0x51, 0x0A
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x01;              // Write global control registers
	abApi_Write[3] = 0x51;
	abApi_Write[4] = 0x0a;
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800PA_MODE_4 0x11, 0x22, 0x04, 0x00, 0x08, 0x7F, 0x00, 0x3D
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x22; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x04;              // Write global control registers
	abApi_Write[3] = 0x00;
	abApi_Write[4] = 0x08;
	//内部PA，0x7f最大,0x00最小
	abApi_Write[5] = 0x7f;
	abApi_Write[6] = 0x00;
	abApi_Write[7] = 0x3d;
	bApi_SendCommand(8,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//#define RF_4800MATCH_VALUE_1_12 0x11, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x30; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x0c;              // Write global control registers
	abApi_Write[3] = 0x00;
	abApi_Write[4] = 0x00;
	abApi_Write[5] = 0x00;
	abApi_Write[6] = 0x00;
	abApi_Write[7] = 0x00;
	abApi_Write[8] = 0x00;
	abApi_Write[9] = 0x00;
	abApi_Write[10] = 0x00;
	abApi_Write[11] = 0x00;
	abApi_Write[12] = 0x00;
	abApi_Write[13] = 0x00;
	abApi_Write[14] = 0x00;
	abApi_Write[15] = 0x00;
	bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();	
	//#define RF_4800FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08, 0x00, 0x38, 0x0E, 0x37, 0xFA, 0x5A, 0x1D, 0x20, 0xFE
	abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x40; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x08;              // Write global control registers
	abApi_Write[3] = 0x00;
	abApi_Write[4] = 0x38;
	abApi_Write[5] = 0x0e;
	abApi_Write[6] = 0x37;
	abApi_Write[7] = 0xfa;
	abApi_Write[8] = 0x5a;
	abApi_Write[9] = 0x1d;
	abApi_Write[10] = 0x20;
	abApi_Write[11] = 0xfe;
	bApi_SendCommand(12,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();
	//不同波特率下的配置
	/*
	//内存空间够，为了快可用此配置
	switch(bps)
	{
		case 48:
		{
			bApi_SendCommand(DATA_ARRAY4800_1[0],&DATA_ARRAY4800_1[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_2[0],&DATA_ARRAY4800_2[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_3[0],&DATA_ARRAY4800_3[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_4[0],&DATA_ARRAY4800_4[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_5[0],&DATA_ARRAY4800_5[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_6[0],&DATA_ARRAY4800_6[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_7[0],&DATA_ARRAY4800_7[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_8[0],&DATA_ARRAY4800_8[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_9[0],&DATA_ARRAY4800_9[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_10[0],&DATA_ARRAY4800_10[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_11[0],&DATA_ARRAY4800_11[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_12[0],&DATA_ARRAY4800_12[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY4800_13[0],&DATA_ARRAY4800_13[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			break;
		}
		case 1152:
		{
			bApi_SendCommand(DATA_ARRAY115200_1[0],&DATA_ARRAY115200_1[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_2[0],&DATA_ARRAY115200_2[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_3[0],&DATA_ARRAY115200_3[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_4[0],&DATA_ARRAY115200_4[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_5[0],&DATA_ARRAY115200_5[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_6[0],&DATA_ARRAY115200_6[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_7[0],&DATA_ARRAY115200_7[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_8[0],&DATA_ARRAY115200_8[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_9[0],&DATA_ARRAY115200_9[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_10[0],&DATA_ARRAY115200_10[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_11[0],&DATA_ARRAY115200_11[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_12[0],&DATA_ARRAY115200_12[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			bApi_SendCommand(DATA_ARRAY115200_13[0],&DATA_ARRAY115200_13[1]); // Send API command to the radio IC
			vApi_WaitforCTS();
			break;
		}
	}
	*/
	//将所有配置写成程序

	switch(bps)
	{
		case 48:
		{
			//0x10, RF_4800PKT_LEN_12};
			//#define RF_4800PKT_LEN_12 0x11, 0x12, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x13, 0x04, 0x80, 0x00, 0x00, 0x00
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x12; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x08;
			abApi_Write[4] = 0x00;
			abApi_Write[5] = 0x00;
			abApi_Write[6] = 0x00;
			abApi_Write[7] = 0x30;
			abApi_Write[8] = 0x30;
			abApi_Write[9] = 0x00;
			abApi_Write[10] = 0x13;
			abApi_Write[11] = 0x04;
			abApi_Write[12] = 0x80;
			abApi_Write[13] = 0x00;
			abApi_Write[14] = 0x00;
			abApi_Write[15] = 0x00;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x10, RF_4800MODEM_MOD_TYPE_12};
			//#define RF_4800MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00, 0x03, 0x00, 0x07, 0x02, 0xEE, 0x00, 0x05, 0xC9, 0xC3, 0x80, 0x00, 0x0C
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x00;
			abApi_Write[4] = 0x03;
			abApi_Write[5] = 0x00;
			abApi_Write[6] = 0x07;
			abApi_Write[7] = 0x02;
			abApi_Write[8] = 0xee;
			abApi_Write[9] = 0x00;
			abApi_Write[10] = 0x05;
			abApi_Write[11] = 0xc9;
			abApi_Write[12] = 0xC3;
			abApi_Write[13] = 0x80;
			abApi_Write[14] = 0x00;
			abApi_Write[15] = 0x0c;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x05, RF_4800MODEM_FREQ_DEV_0_1};
			//#define RF_4800MODEM_FREQ_DEV_0_1 0x11, 0x20, 0x01, 0x0C, 0x4A
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x01;              // Write global control registers
			abApi_Write[3] = 0x0c;
			abApi_Write[4] = 0x4a;
			bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0C, RF_4800MODEM_TX_RAMP_DELAY_8};
			//#define RF_4800MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08, 0x18, 0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x10, 0x10
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x08;              // Write global control registers
			abApi_Write[3] = 0x18;
			abApi_Write[4] = 0x01;
			abApi_Write[5] = 0x80;
			abApi_Write[6] = 0x08;
			abApi_Write[7] = 0x03;
			abApi_Write[8] = 0x80;
			abApi_Write[9] = 0x00;
			abApi_Write[10] = 0x10;
			abApi_Write[11] = 0x10;
			bApi_SendCommand(12,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0D, RF_4800MODEM_BCR_OSR_1_9};
			//#define RF_4800MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09, 0x22, 0x04, 0x12, 0x00, 0x7D, 0xD4, 0x00, 0x3F, 0x02, 0xC2
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x09;              // Write global control registers
			abApi_Write[3] = 0x22;
			abApi_Write[4] = 0x04;
			abApi_Write[5] = 0x12;
			abApi_Write[6] = 0x00;
			abApi_Write[7] = 0x7d;
			abApi_Write[8] = 0xd4;
			abApi_Write[9] = 0x00;
			abApi_Write[10] = 0x3f;
			abApi_Write[11] = 0x02;
			abApi_Write[12] = 0xC2;
			bApi_SendCommand(13,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0B, RF_4800MODEM_AFC_GEAR_7};
			//#define RF_4800MODEM_AFC_GEAR_7 0x11, 0x20, 0x07, 0x2C, 0x04, 0x36, 0x80, 0x0E, 0x31, 0x44, 0x80
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x07;              // Write global control registers
			abApi_Write[3] = 0x2c;
			abApi_Write[4] = 0x04;
			abApi_Write[5] = 0x36;
			abApi_Write[6] = 0x80;
			abApi_Write[7] = 0x0e;
			abApi_Write[8] = 0x31;
			abApi_Write[9] = 0x44;
			abApi_Write[10] = 0x80;
			bApi_SendCommand(11,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x07, RF_4800MODEM_AGC_WINDOW_SIZE_3};
			//#define RF_4800MODEM_AGC_WINDOW_SIZE_3 0x11, 0x20, 0x03, 0x38, 0x11, 0xE4, 0xE4
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x03;              // Write global control registers
			abApi_Write[3] = 0x38;
			abApi_Write[4] = 0x11;
			abApi_Write[5] = 0xe4;
			abApi_Write[6] = 0xe4;
			bApi_SendCommand(7,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x05, RF_4800MODEM_OOK_PDTC_1};
			//#define RF_4800MODEM_OOK_PDTC_1 0x11, 0x20, 0x01, 0x40, 0x2B
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x01;              // Write global control registers
			abApi_Write[3] = 0x40;
			abApi_Write[4] = 0x2b;
			bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0F, RF_4800MODEM_OOK_CNT1_11};
			//#define RF_4800MODEM_OOK_CNT1_11 0x11, 0x20, 0x0B, 0x42, 0xA4, 0x02, 0xD6, 0x83, 0x01, 0x59, 0x01, 0x80, 0xFF, 0x0C, 0x00
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0b;              // Write global control registers
			abApi_Write[3] = 0x42;
			abApi_Write[4] = 0xa4;
			abApi_Write[5] = 0x02;
			abApi_Write[6] = 0xd6;
			abApi_Write[7] = 0x83;
			abApi_Write[8] = 0x01;
			abApi_Write[9] = 0x59;
			abApi_Write[10] = 0x01;
			abApi_Write[11] = 0x80;
			abApi_Write[12] = 0xff;
			abApi_Write[13] = 0x0c;
			abApi_Write[14] = 0x00;
			bApi_SendCommand(15,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x10, RF_4800MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
			//#define RF_4800MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x21; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x00;
			abApi_Write[4] = 0xcc;
			abApi_Write[5] = 0xa1;
			abApi_Write[6] = 0x30;
			abApi_Write[7] = 0xa0;
			abApi_Write[8] = 0x21;
			abApi_Write[9] = 0xd1;
			abApi_Write[10] = 0xb9;
			abApi_Write[11] = 0xc9;
			abApi_Write[12] = 0xea;
			abApi_Write[13] = 0x05;
			abApi_Write[14] = 0x12;
			abApi_Write[15] = 0x11;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x10, RF_4800MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12};
			//#define RF_4800MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x21; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x0c;
			abApi_Write[4] = 0x0a;
			abApi_Write[5] = 0x04;
			abApi_Write[6] = 0x15;
			abApi_Write[7] = 0xfc;
			abApi_Write[8] = 0x03;
			abApi_Write[9] = 0x00;
			abApi_Write[10] = 0xcc;
			abApi_Write[11] = 0xa1;
			abApi_Write[12] = 0x30;
			abApi_Write[13] = 0xa0;
			abApi_Write[14] = 0x21;
			abApi_Write[15] = 0xd1;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x10, RF_4800MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12};
			//#define RF_4800MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x21; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x18;
			abApi_Write[4] = 0xb9;
			abApi_Write[5] = 0xc9;
			abApi_Write[6] = 0xea;
			abApi_Write[7] = 0x05;
			abApi_Write[8] = 0x12;
			abApi_Write[9] = 0x11;
			abApi_Write[10] = 0x0a;
			abApi_Write[11] = 0x04;
			abApi_Write[12] = 0x15;
			abApi_Write[13] = 0xfc;
			abApi_Write[14] = 0x03;
			abApi_Write[15] = 0x00;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0B, RF_4800SYNTH_PFDCP_CPFF_7};
			//#define RF_4800SYNTH_PFDCP_CPFF_7 0x11, 0x23, 0x07, 0x00, 0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x23; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x07;              // Write global control registers
			abApi_Write[3] = 0x00;
			abApi_Write[4] = 0x2c;
			abApi_Write[5] = 0x0e;
			abApi_Write[6] = 0x0b;
			abApi_Write[7] = 0x04;
			abApi_Write[8] = 0x0c;
			abApi_Write[9] = 0x73;
			abApi_Write[10] = 0x03;
			bApi_SendCommand(11,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			break;
		}
		case 1152:
		{
			//0x10, RF_115200PKT_LEN_12};
			//#define RF_115200PKT_LEN_12 0x11, 0x12, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x13, 0x04, 0x00, 0x00, 0x00, 0x00
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x12; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x08;
			abApi_Write[4] = 0x00;
			abApi_Write[5] = 0x00;
			abApi_Write[6] = 0x00;
			abApi_Write[7] = 0x30;
			abApi_Write[8] = 0x30;
			abApi_Write[9] = 0x00;
			abApi_Write[10] = 0x13;
			abApi_Write[11] = 0x04;
			abApi_Write[12] = 0x00;
			abApi_Write[13] = 0x00;
			abApi_Write[14] = 0x00;
			abApi_Write[15] = 0x00;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x10, RF_115200MODEM_MOD_TYPE_12};
			//#define RF_115200MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00, 0x03, 0x00, 0x07, 0x23, 0x28, 0x00, 0x09, 0xC9, 0xC3, 0x80, 0x00, 0x0F
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x00;
			abApi_Write[4] = 0x03;
			abApi_Write[5] = 0x00;
			abApi_Write[6] = 0x07;
			abApi_Write[7] = 0x23;
			abApi_Write[8] = 0x28;
			abApi_Write[9] = 0x00;
			abApi_Write[10] = 0x09;
			abApi_Write[11] = 0xc9;
			abApi_Write[12] = 0xC3;
			abApi_Write[13] = 0x80;
			abApi_Write[14] = 0x00;
			abApi_Write[15] = 0x0f;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x05, RF_115200MODEM_FREQ_DEV_0_1};
			//#define RF_115200MODEM_FREQ_DEV_0_1 0x11, 0x20, 0x01, 0x0C, 0xBB
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x01;              // Write global control registers
			abApi_Write[3] = 0x0c;
			abApi_Write[4] = 0xbb;
			bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0C, RF_115200MODEM_TX_RAMP_DELAY_8};
			//#define RF_115200MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08, 0x18, 0x01, 0x00, 0x08, 0x03, 0x80, 0x00, 0x10, 0x20
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x08;              // Write global control registers
			abApi_Write[3] = 0x18;
			abApi_Write[4] = 0x01;
			abApi_Write[5] = 0x00;
			abApi_Write[6] = 0x08;
			abApi_Write[7] = 0x03;
			abApi_Write[8] = 0x80;
			abApi_Write[9] = 0x00;
			abApi_Write[10] = 0x10;
			abApi_Write[11] = 0x20;
			bApi_SendCommand(12,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0D, RF_115200MODEM_BCR_OSR_1_9};
			//#define RF_115200MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09, 0x22, 0x00, 0x41, 0x07, 0xDD, 0x44, 0x07, 0xE0, 0x02, 0x00
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x09;              // Write global control registers
			abApi_Write[3] = 0x22;
			abApi_Write[4] = 0x00;
			abApi_Write[5] = 0x41;
			abApi_Write[6] = 0x07;
			abApi_Write[7] = 0xdd;
			abApi_Write[8] = 0x44;
			abApi_Write[9] = 0x07;
			abApi_Write[10] = 0xe0;
			abApi_Write[11] = 0x02;
			abApi_Write[12] = 0x00;
			bApi_SendCommand(13,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0B, RF_115200MODEM_AFC_GEAR_7};
			//#define RF_115200MODEM_AFC_GEAR_7 0x11, 0x20, 0x07, 0x2C, 0x00, 0x23, 0x87, 0xDD, 0x00, 0xCD, 0xA0
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x07;              // Write global control registers
			abApi_Write[3] = 0x2c;
			abApi_Write[4] = 0x00;
			abApi_Write[5] = 0x23;
			abApi_Write[6] = 0x87;
			abApi_Write[7] = 0xdd;
			abApi_Write[8] = 0x00;
			abApi_Write[9] = 0xcd;
			abApi_Write[10] = 0xa0;
			bApi_SendCommand(11,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x07, RF_115200MODEM_AGC_WINDOW_SIZE_3};
			//#define RF_115200MODEM_AGC_WINDOW_SIZE_3 0x11, 0x20, 0x03, 0x38, 0x11, 0x0E, 0x0E
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x03;              // Write global control registers
			abApi_Write[3] = 0x38;
			abApi_Write[4] = 0x11;
			abApi_Write[5] = 0x0e;
			abApi_Write[6] = 0x0e;
			bApi_SendCommand(7,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x05, RF_115200MODEM_OOK_PDTC_1};
			//#define RF_115200MODEM_OOK_PDTC_1 0x11, 0x20, 0x01, 0x40, 0x28
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x01;              // Write global control registers
			abApi_Write[3] = 0x40;
			abApi_Write[4] = 0x28;
			bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0F, RF_115200MODEM_OOK_CNT1_11};
			//#define RF_115200MODEM_OOK_CNT1_11 0x11, 0x20, 0x0B, 0x42, 0xA4, 0x03, 0xD6, 0x03, 0x01, 0x26, 0x01, 0x80, 0xFF, 0x0C, 0x00
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x20; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0b;              // Write global control registers
			abApi_Write[3] = 0x42;
			abApi_Write[4] = 0xa4;
			abApi_Write[5] = 0x03;
			abApi_Write[6] = 0xd6;
			abApi_Write[7] = 0x03;
			abApi_Write[8] = 0x01;
			abApi_Write[9] = 0x26;
			abApi_Write[10] = 0x01;
			abApi_Write[11] = 0x80;
			abApi_Write[12] = 0xff;
			abApi_Write[13] = 0x0c;
			abApi_Write[14] = 0x00;
			bApi_SendCommand(15,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x10, RF_115200MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
			//#define RF_115200MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00, 0xFF, 0xBA, 0x0F, 0x51, 0xCF, 0xA9, 0xC9, 0xFC, 0x1B, 0x1E, 0x0F, 0x01
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x21; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x00;
			abApi_Write[4] = 0xff;
			abApi_Write[5] = 0xba;
			abApi_Write[6] = 0x0f;
			abApi_Write[7] = 0x51;
			abApi_Write[8] = 0xcf;
			abApi_Write[9] = 0xa9;
			abApi_Write[10] = 0xc9;
			abApi_Write[11] = 0xfc;
			abApi_Write[12] = 0x1b;
			abApi_Write[13] = 0x1e;
			abApi_Write[14] = 0x0f;
			abApi_Write[15] = 0x01;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x10, RF_115200MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12};
			//#define RF_115200MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C, 0xFC, 0xFD, 0x15, 0xFF, 0x00, 0x0F, 0xFF, 0xBA, 0x0F, 0x51, 0xCF, 0xA9
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x21; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x0c;
			abApi_Write[4] = 0xfc;
			abApi_Write[5] = 0xfd;
			abApi_Write[6] = 0x15;
			abApi_Write[7] = 0xff;
			abApi_Write[8] = 0x00;
			abApi_Write[9] = 0x0f;
			abApi_Write[10] = 0xff;
			abApi_Write[11] = 0xba;
			abApi_Write[12] = 0x0f;
			abApi_Write[13] = 0x51;
			abApi_Write[14] = 0xcf;
			abApi_Write[15] = 0xa9;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x10, RF_115200MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12};
			//#define RF_115200MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18, 0xC9, 0xFC, 0x1B, 0x1E, 0x0F, 0x01, 0xFC, 0xFD, 0x15, 0xFF, 0x00, 0x0F
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x21; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x0c;              // Write global control registers
			abApi_Write[3] = 0x18;
			abApi_Write[4] = 0xc9;
			abApi_Write[5] = 0xfc;
			abApi_Write[6] = 0x1b;
			abApi_Write[7] = 0x1e;
			abApi_Write[8] = 0x0f;
			abApi_Write[9] = 0x01;
			abApi_Write[10] = 0xfc;
			abApi_Write[11] = 0xfd;
			abApi_Write[12] = 0x15;
			abApi_Write[13] = 0xff;
			abApi_Write[14] = 0x00;
			abApi_Write[15] = 0x0f;
			bApi_SendCommand(16,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			//0x0B, RF_115200SYNTH_PFDCP_CPFF_7};
			//#define RF_115200SYNTH_PFDCP_CPFF_7 0x11, 0x23, 0x07, 0x00, 0x34, 0x04, 0x0B, 0x04, 0x07, 0x70, 0x03
			abApi_Write[0] = 0x11;          // Use API command to power up the radio IC
			abApi_Write[1] = 0x23; //参数必须是  Stay in bootload
			abApi_Write[2] = 0x07;              // Write global control registers
			abApi_Write[3] = 0x00;
			abApi_Write[4] = 0x34;
			abApi_Write[5] = 0x04;
			abApi_Write[6] = 0x0b;
			abApi_Write[7] = 0x04;
			abApi_Write[8] = 0x07;
			abApi_Write[9] = 0x70;
			abApi_Write[10] = 0x03;
			bApi_SendCommand(11,abApi_Write);        // Send command to the radio IC
			vApi_WaitforCTS();
			break;
		}
	}

}

void SI4438Init(unsigned int bps)
{
unsigned int i;
	abApi_Write[0] = CMD_POWER_UP;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x01; //第一次复位后，发送POWER-UP COMMAND 的参数必须是  Stay in bootload？？ 
	abApi_Write[2] = 0x00;              // Write global control registers
	bApi_SendCommand(3,abApi_Write);        // Send command to the radio IC
	delay_ms(10);
	//一个SPI命令，及其参数发送完成后，必须先拉高NSS引脚，
	//接下来，4438在处理这些命令期间GPIO1会被从高电平拉低，处理完这条命令后GPIO1 又会变成高电平
	//检测芯片是否正常可以用示波仪监测GPIO1电平的变化是否正常
	//++++++++++++++++++++++++++++++++++++   
	
	//++++++++第二次发POWER-UP COMMAND++++++++++++++++++++++++++++++++
	//第二次使能，发送POWER-UP COMMAND 的参数必须是  Boot main application image  
	
	abApi_Write[0] = CMD_POWER_UP;          // Use API command to power up the radio IC
	abApi_Write[1] = 0x01; //参数必须是  Stay in bootload
	abApi_Write[2] = 0x00;              // Write global control registers
	abApi_Write[3] = 0x01;
	abApi_Write[4] = 0xC9;
	abApi_Write[5] = 0xC3;
	abApi_Write[6] = 0x80;
	bApi_SendCommand(7,abApi_Write);        // Send command to the radio IC
	delay_ms(14);	//14ms
	
	//++++++++++++激活 完成++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++获取中断寄存器的当前值并清除挂起的中断++++++++++++++++
	// Read ITs, clear pending ones
	abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
	abApi_Write[1] = 0;               // Clear PH_CLR_PEND
	abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
	abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
	bApi_SendCommand(4,abApi_Write);
	vApi_WaitforCTS();
	//++++++++++获取中断寄存器的当前值并清除挂起的中断++++++++++++++++
	// Read ITs, clear pending ones
	abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
	abApi_Write[1] = 0;               // Clear PH_CLR_PEND
	abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
	abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
	bApi_SendCommand(4,abApi_Write);
	vApi_WaitforCTS();
	//SI4438SetConfiguration(bps);
	
	switch(bps)
	{
		case 48:
		{
			i=0;
			do{
				bApi_SendCommand(radio_configuration_data4800[i],&radio_configuration_data4800[i+1]); // Send API command to the radio IC
				vApi_WaitforCTS();
				i+=radio_configuration_data4800[i] ;
				i++;
			}
			while(radio_configuration_data4800[i]);
			break;
		}
			/*
		case 1152:
		{
			i=0;
			do{
				bApi_SendCommand(radio_configuration_data115200[i],&radio_configuration_data115200[i+1]); // Send API command to the radio IC
				vApi_WaitforCTS();
				i+=radio_configuration_data115200[i] ;
				i++;
			}
			while(radio_configuration_data115200[i]);
			break;
		}
		*/
		default:
			;
	}
	
	// Enable the proper interrupts
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_INT_CTL_GROUP;      // Select property group
	abApi_Write[2] = 4;               // Number of properties to be written
	abApi_Write[3] = PROP_INT_CTL_ENABLE;     // Specify property
	abApi_Write[4] = 0x01;              // INT_CTL: PH interrupt enable
	abApi_Write[5] = 0x30;              // INT_CTL_PH: PH PACKET_SENT, PACKET_RX, CRC2_ERR enabled
	abApi_Write[6] = 0x00;              // INT_CTL_MODEM: -
	abApi_Write[7] = 0x00;              // INT_CTL_CHIP_EN: -
	bApi_SendCommand(8,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS

	// Configure Fast response registers
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_FRR_CTL_GROUP;      // Select property group
	abApi_Write[2] = 4;               // Number of properties to be written
	abApi_Write[3] = PROP_FRR_CTL_A_MODE;     // Specify fast response registers
	abApi_Write[4] = 0x04;              // FRR A: PH IT pending
	abApi_Write[5] = 0x06;              // FRR B: Modem IT pending
	abApi_Write[6] = 0x0A;              // FRR C: Latched RSSI
	abApi_Write[7] = 0x00;              // FRR D: disabled
	bApi_SendCommand(8,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	// Set packet content
	// Set preamble length - Rx
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PREAMBLE_GROUP;     // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_PREAMBLE_CONFIG_STD_1;  // Specify property
	abApi_Write[4] = 20;              // 20 bits preamble detection threshold
	bApi_SendCommand(5,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	// Set preamble length - Tx
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PREAMBLE_GROUP;     // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_PREAMBLE_TX_LENGTH;   // Specify property
	abApi_Write[4] = 0x05;              // 5 bytes Tx preamble
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	
	// Set preamble pattern
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PREAMBLE_GROUP;     // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_PREAMBLE_CONFIG;      // Specify property
	abApi_Write[4] = 0x31;              // Use `1010` pattern, length defined in bytes
	bApi_SendCommand(5,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	// Set sync word
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_SYNC_GROUP;       // Select property group
	abApi_Write[2] = 3;               // Number of properties to be written
	abApi_Write[3] = PROP_SYNC_CONFIG;        // Specify property
	abApi_Write[4] = 0x01;              // SYNC_CONFIG: 2 bytes sync word
	abApi_Write[5] = 0xB4;              // SYNC_BITS_31_24: 1st sync byte: 0x2D; NOTE: LSB transmitted first!
	abApi_Write[6] = 0x2B;              // SYNC_BITS_23_16: 2nd sync byte: 0xD4; NOTE: LSB transmitted first!
	bApi_SendCommand(7,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	//****************************************************************************
	// General packet config (set bit order)
	//已确定OK
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PKT_GROUP;        // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_PKT_CONFIG1;        // Specify property
	//msb first,no invert
	abApi_Write[4] = 0x82;              //82 Separate RX and TX FIELD properties are used, payload data goes MSB first
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
     // Set variable packet length
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PKT_GROUP;        // Select property group
	abApi_Write[2] = 3;               // Number of properties to be written
	abApi_Write[3] = PROP_PKT_LEN;          // 08Specify property
	abApi_Write[4] = 0x0a;              //PKT_LEN: length is put in the Rx FIFO, FIELD 2 is used for the payload (with variable length)
	abApi_Write[5] = 0x01;              // PKT_LEN_FIELD_SOURCE: FIELD 1 is used for the length information
	abApi_Write[6] = 0x00;              // PKT_LEN_ADJUST: no adjustment (FIELD 1 determines the actual payload length)
	bApi_SendCommand(7,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	// Set packet fields for Tx (one field is used)
	//已经确定ok
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PKT_GROUP;        // Select property group
	abApi_Write[2] = 4;               // Number of properties to be written
	abApi_Write[3] = PROP_PKT_FIELD_1_LENGTH_12_8;  //0d Specify first property
	abApi_Write[4] = 0x00;              // PKT_FIELD_1_LENGTH_12_8: defined later (under bSendPacket() )
	abApi_Write[5] = 0x00;              // PKT_FIELD_1_LENGTH_7_0: defined later (under bSendPacket() )
	abApi_Write[6] = 0x00;              // PKT_FIELD_1_CONFIG : No 4(G)FSK/Whitening/Manchester coding for this field
	abApi_Write[7] = 0xa2;              //0xa2,ok... PKT_FIELD_1_CRC_CONFIG: Start CRC calc. from this field, send CRC at the end of the field
	bApi_SendCommand(8,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	//***********************************************************************************
	// Set packet fields for Rx (two fields are used)
	// FIELD1 is fixed, 1byte length, used for PKT_LEN
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PKT_GROUP;        // Select property group
	abApi_Write[2] = 4;               // Number of properties to be written
	abApi_Write[3] = PROP_PKT_RX_FIELD_1_LENGTH_12_8; // Specify first property
	abApi_Write[4] = 0x00;              // PKT_RX_FIELD_1_LENGTH_12_8: 1 byte (containing variable packet length info)
	abApi_Write[5] = 1;              // PKT_RX_FIELD_1_LENGTH_7_0: 1 byte (containing variable packet length info)
	abApi_Write[6] = 0x00;              // PKT_RX_FIELD_1_CONFIG: No 4(G)FSK/Whitening/Manchester coding for this field
	abApi_Write[7] = 0x82;              //82 PKT_RX_FIELD_1_CRC_CONFIG: Start CRC calc. from this field, enable CRC calc.
	bApi_SendCommand(8,abApi_Write);        // Send command to the radio IC
	// FIELD2 is variable length, contains the actual payload
	vApi_WaitforCTS();                // Wait for CTS
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PKT_GROUP;        // Select property group
	abApi_Write[2] = 4;               // Number of properties to be written
	abApi_Write[3] = PROP_PKT_RX_FIELD_2_LENGTH_12_8; // Specify first property
	abApi_Write[4] = 0x00;              // PKT_RX_FIELD_2_LENGTH_12_8: max. field length (variable packet length)
	abApi_Write[5] = 64;     // PKT_RX_FIELD_2_LENGTH_7_0: max. field length (variable packet length)
	abApi_Write[6] = 0x00;              // PKT_RX_FIELD_2_CONFIG: No 4(G)FSK/Whitening/Manchester coding for this field
	abApi_Write[7] = 0x0a;              // PKT_RX_FIELD_2_CRC_CONFIG: check CRC at the end, enable CRC calc.
	bApi_SendCommand(8,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	
	// Configure CRC polynomial and seed
	//已确定ok
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_PKT_GROUP;        // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_PKT_CRC_CONFIG;     // Specify property
	abApi_Write[4] = 0x04;              //ok... CRC seed: all `0`s, poly: No. 3, 16bit, ibm16
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	
	// Set RSSI latch to sync word
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_MODEM_GROUP;        // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_MODEM_RSSI_CONTROL;   // Specify property
	abApi_Write[4] = 0x12;              // RSSI average over 4 bits, latches at sync word detect
	bApi_SendCommand(5,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	
	
	// Adjust XTAL clock frequency
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_GLOBAL_GROUP;       // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_GLOBAL_XO_TUNE;     // Specify property
	abApi_Write[4] = 0x48;        // Set cap bank value to adjust XTAL clock frequency
	bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +
  + FUNCTION NAME:  unsigned char vApi_WaitforCTS(void)
  +
  + DESCRIPTION:    wait for CTS
  +
  + INPUT:          None
  +
  + RETURN:         0 - CTS arrived
  +                 1 - CTS didn't arrive within MAX_CTS_RETRY
  +
  + NOTES:          None
  +
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
unsigned char vApi_WaitforCTS(void)
{
unsigned char   CtsValue=0;
unsigned int  ErrCnt=0;
unsigned char   string[]="CTS is OK\n"	;

  
  while (CtsValue!=0xFF)   // Wait until radio IC is ready with the data
  {	 
	delay_1us(10);
    GPIO_ResetBits(nSEL_PORT, nSEL_PIN);      // Select radio IC by pulling its nSEL pin low
    Spi_Write_one_byte(CMD_CTS_READ);
    CtsValue=Spi_Read_one_byte();
//CLRWDT
//UART0_Send_byte(0xAA);	//标记收到的0xFF 是CTS value ,而不是普通的0xFF
//UART0_Send_byte(CtsValue);	  //输出获取的CTS  value
//UART0_Send_byte(0xAA);		//标记收到的0xFF 是CTS value
//UART0_Send_String(string);
    GPIO_SetBits(nSEL_PORT, nSEL_PIN);
	//GPIO_SetBits(nSEL_PORT, nSEL_PIN);                        // If CTS is not 0xFF, put NSS high and stay in waiting loop

     if (++ErrCnt > MAX_CTS_RETRY)
    {
	  return 1;     // Error handling; if wrong CTS reads exceeds a limit
    }
	 
  }	  
  

  return 0;
}
void SI4438StartRX(unsigned char ch) //启动接收，收到正常数据后转到READYT状态
{
		//清空缓冲区中的数据
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN); 
		Spi_Write_one_byte(CMD_FIFO_INFO);
		Spi_Write_one_byte(0x02);
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		delay_ms(1);
		//++++++++++获取中断寄存器的当前值并清除挂起的中断++++++++++++++++
		// Read ITs, clear pending ones
		abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
		abApi_Write[1] = 0;               // Clear PH_CLR_PEND
		abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
		abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
		bApi_SendCommand(4,abApi_Write);
		vApi_WaitforCTS();                // Wait for CTS
  	// Start Rx
	//发8个字节，返回1个字节的CTS
	abApi_Write[0] = CMD_START_RX;      // Use start Rx command
	abApi_Write[1] = ch;           // Set channel number
	abApi_Write[2] = 0x00;          // Start Rx immediately
	abApi_Write[3] = 0x00;          // Packet fields used, do not enter packet length here
	abApi_Write[4] = 0x00;          // Packet fields used, do not enter packet length here
	abApi_Write[5] = 0x00;          // No change if Rx timeout
	abApi_Write[6] = 0x03;          // Ready state after Rx valid state
	abApi_Write[7] = 0x03;          // RX state after Rx invalid state
	bApi_SendCommand(8,abApi_Write);    // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
}
void SI4438ChangeState(unsigned char cmd) //不改变其它参数，只做状态切换
{
	/*
	0-NOCHANGE	1-SLEEP	2-SPI_ACTIVE	3-READY	5-TX_TUNE
	6-RX_TUNE	7-TX	8-RX
	*/
	if(cmd>8)
		cmd=0;
	abApi_Write[0] = CMD_CHANGE_STATE;        // Use change state command
	abApi_Write[1] = cmd;               // Go to Ready mode
	bApi_SendCommand(2,abApi_Write);        // Send command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
}

unsigned char SI4438ReadRXDataFromFIFO(void)
{
	unsigned char i,j;
	unsigned int k;
	/*
	读取FIFO中数据到RX_FIFO中，长度存于RX_FIFO[0]中
	*/
	if(GPIO_ReadInputDataBit(nIRQ_PORT, nIRQ_PIN))	//如果未产生射频中断，直接返回
		return 0;
	FRRA=SI4438GetFRRA(); //PH IT pending
	FRRB=SI4438GetFRRB(); //MODEM IT pending
	FRRC=SI4438GetFRRC(); //RSSI
	//++++++++++获取中断寄存器的当前值并清除挂起的中断++++++++++++++++
	// Read ITs, clear pending ones
	/*
	//可直接读取FRR寄存器，以节约时间
//	abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
//	abApi_Write[1] = 0;               // Clear PH_CLR_PEND
//	abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
//	abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
//	bApi_SendCommand(4,abApi_Write);
//	vApi_WaitforCTS();
//	//+++++++++++获取中断寄存器值+++++++++++++++++++
//	bApi_GetResponse(8, abApi_Read ); // Make sure that CTS is ready then get the response
	//+++++++++++++++++++++++++++END+++++++++++++++++++++++++++++++++++++++		
//	if( (abApi_Read[2]&0x10) == 0x10 ) //接收到有效包中断
*/
	if( (FRRA&0x10) == 0x10 )
	{
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN); 
		Spi_Write_one_byte(CMD_FIFO_INFO);
		Spi_Write_one_byte(0x00);//不清空FIFO
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		delay_1us(100);
		bApi_GetResponse(2);
		/*
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
		Spi_Write_one_byte(CMD_CTS_READ);
		while(abApi_Read[0]!=0xFF)
		{ 
			abApi_Read[0]=Spi_Read_one_byte();
			delay_1us(100);	//延时10ms 
		}
		delay_1us(100);
		for(wDelay=0; wDelay<3; wDelay++)
		{
			abApi_Read[wDelay]=Spi_Read_one_byte();
		}
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		*/
		delay_1us(100);
//USART1SendByte(rxpknum);
		//num_of_ResBytes=abApi_Read[0];
		if(abApi_Read[0]>=33)
			abApi_Read[0]=33;
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
		Spi_Write_one_byte(CMD_RX_FIFO_READ);
//USART1SendByte(rxpknum);
		for(i=0; i<abApi_Read[0]; i++)
		{	
			RX_FIFO[i]=Spi_Read_one_byte();
		}
		//bApi_GetResponse_without_CTS_detection(abApi_Read[0], RX_FIFO ); //recieved number of bytes stored in abApi_Read[1]
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
//USART1SendByte(rxpknum);
		if(RX_FIFO[0]>=33)
			RX_FIFO[0]=33;
		//清空缓冲区中的数据
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN); 
		Spi_Write_one_byte(CMD_FIFO_INFO);
		Spi_Write_one_byte(0x02);
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		delay_ms(1);
		//++++++++++获取中断寄存器的当前值并清除挂起的中断++++++++++++++++
		// Read ITs, clear pending ones
		abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
		abApi_Write[1] = 0;               // Clear PH_CLR_PEND
		abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
		abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
		bApi_SendCommand(4,abApi_Write);
		vApi_WaitforCTS();                // Wait for CTS
		//while(!GPIO_ReadInputDataBit(nIRQ_PORT, nIRQ_PIN));//	中断释放后，再往下执行
		return RX_FIFO[0];//有数据进来，返回长度信息
	}
	else
	{
		// Read ITs, clear pending ones
		abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
		abApi_Write[1] = 0;               // Clear PH_CLR_PEND
		abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
		abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
		bApi_SendCommand(4,abApi_Write);
		vApi_WaitforCTS();                // Wait for CTS
		return 0;//没有数据进来，返回0
	}
}

void SI4438SendTXFIFOData(unsigned char ch)
{
	//LED=0;
	bSendPacket(TX_FIFO[0]+1, TX_FIFO,ch); // Send the packet
}

unsigned char SI4438GetFRRA(void)
{
	unsigned char dat;
	GPIO_ResetBits(nSEL_PORT, nSEL_PIN); 
	Spi_Write_one_byte(0x50);
	dat=Spi_Read_one_byte();
	GPIO_SetBits(nSEL_PORT, nSEL_PIN);
	return dat;
}
unsigned char SI4438GetFRRB(void)
{
	unsigned char dat;
	GPIO_ResetBits(nSEL_PORT, nSEL_PIN); 
	Spi_Write_one_byte(0x51);
	dat=Spi_Read_one_byte();
	GPIO_SetBits(nSEL_PORT, nSEL_PIN);
	return dat;
}
unsigned char SI4438GetFRRC(void)
{
	unsigned char dat;
	GPIO_ResetBits(nSEL_PORT, nSEL_PIN); 
	Spi_Write_one_byte(0x53);
	dat=Spi_Read_one_byte();
	GPIO_SetBits(nSEL_PORT, nSEL_PIN);
	return dat;
}
unsigned char SI4438GetFRRD(void)
{
	unsigned char dat;
	GPIO_ResetBits(nSEL_PORT, nSEL_PIN); 
	Spi_Write_one_byte(0x57);
	dat=Spi_Read_one_byte();
	GPIO_SetBits(nSEL_PORT, nSEL_PIN);
	return dat;
}

unsigned char SI4438GetLBD(void)
{
	/*
	电压值=3*LBD/1280 V	 (LBD 16位）
	需要转换一下，与SI4432兼容
	SI4432 IS 1.7V+LBD*0.05V
	*/
	float temp;
	unsigned char u8LBD;
	abApi_Write[0] = 0x14;       
	abApi_Write[1] = 0x08;              
	abApi_Write[2] = 0xa5;
	bApi_SendCommand(3,abApi_Write);        // Send command to the radio IC
	delay_ms(1);
	bApi_GetResponse(6);
	LBD=abApi_Read[2]*256+abApi_Read[3];
	//temp=3*LBD/1280;
	//u8LBD=temp-1.7/0.05;
	temp=LBD*0.046875;
	u8LBD=temp-34;
	return u8LBD;
}

void SI4438WakeUpDevice(unsigned char channel)
{
	unsigned char i;
	SI4438SwitchToTX();
	TX_FIFO[0]=5;
	TX_FIFO[1]=0x01;
	TX_FIFO[2]=0x00;
	TX_FIFO[3]=0x00;
	TX_FIFO[4]=0x00;
	TX_FIFO[5]=0x0f;
	for(i=0;i<45;i++)
	//for(i=0;i<1;i++)
	{
		TX_FIFO[3]=i;	
		bApi_WriteTxDataBuffer(TX_FIFO[0]+1, TX_FIFO);   // Write data to Tx FIFO
		//delay_ms(1);
		vApi_WaitforCTS();                // Wait for CTS
		// Set TX packet length
		abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
		abApi_Write[1] = PROP_PKT_GROUP;        // Select property group
		abApi_Write[2] = 1;               // Number of properties to be written
		abApi_Write[3] = PROP_PKT_FIELD_1_LENGTH_7_0; // Specify first property
		abApi_Write[4] = TX_FIFO[0]+1;           // Field length (variable packet length info)
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
			bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
		//GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		//delay_ms(1);
		//GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
			vApi_WaitforCTS();                // Wait for CTS
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		//++++++++++获取中断寄存器的当前值并清除挂起的中断++++++++++++++++
		// Read ITs, clear pending ones
		abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
		abApi_Write[1] = 0;               // Clear PH_CLR_PEND
		abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
		abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
		bApi_SendCommand(4,abApi_Write);
		//GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		//delay_ms(1);
		//GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
			vApi_WaitforCTS();                // Wait for CTS
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		// Start Tx
		abApi_Write[0] = CMD_START_TX;      // Use Tx Start command
		abApi_Write[1] = channel;           // Set channel number
		abApi_Write[2] = 0x30;          // Ready state after Tx, start Tx immediately
		abApi_Write[3] = 0x00;          // Packet fields used, do not enter packet length here
		abApi_Write[4] = 0x00;          // Packet fields used, do not enter packet length here
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
		bApi_SendCommand(5,abApi_Write);    // Send command to the radio IC
		//GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		//delay_ms(1);
		//GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
		vApi_WaitforCTS();                // Wait for CTS
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		// Wait for packet sent interrupt
		
		while(GPIO_ReadInputDataBit(nIRQ_PORT, nIRQ_PIN) == 1);
		//++++++++++获取中断寄存器的当前值并清除挂起的中断++++++++++++++++
		// Read ITs, clear pending ones
		abApi_Write[0] = CMD_GET_INT_STATUS;      // Use interrupt status command
		abApi_Write[1] = 0;               // Clear PH_CLR_PEND
		abApi_Write[2] = 0;               // Clear MODEM_CLR_PEND
		abApi_Write[3] = 0;               // Clear CHIP_CLR_PEND
		GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
		bApi_SendCommand(4,abApi_Write);
		//GPIO_SetBits(nSEL_PORT, nSEL_PIN);
		//delay_ms(1);
		//GPIO_ResetBits(nSEL_PORT, nSEL_PIN);
		vApi_WaitforCTS();                // Wait for CTS
		GPIO_SetBits(nSEL_PORT, nSEL_PIN);
	}
	SI4438SwitchToRX();
}


unsigned char SI4438GetState(void)
{
  	// Start Rx
	//发1字节，返回CTS回2个字节
	abApi_Write[0] = CMD_REQUEST_DEVICE_STATE;      // Use start Rx command
	bApi_SendCommand(1,abApi_Write);    // Send API command to the radio IC
	bApi_GetResponse(2);
	return abApi_Read[0];
}



void SI4438SwitchToRX(void)
{
	//#define RF_115200GPIO_PIN_CFG 0x13, 0x20, 0x21, 0x16, 0x17, 0x00, 0x00, 0x00
	abApi_Write[0] = 0x13;        //command
	abApi_Write[1] = 0x20;      
	abApi_Write[2] = 0x21;              
	abApi_Write[3] = 0x16;      
	abApi_Write[4] = 0x17;               
	abApi_Write[5] = 0x00;             
	abApi_Write[6] = 0x00;     
	abApi_Write[7] = 0x00;              
	bApi_SendCommand(8,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
}

void SI4438SwitchToTX(void)
{
	//#define RF_115200GPIO_PIN_CFG 0x13, 0x20, 0x21, 0x16, 0x17, 0x00, 0x00, 0x00
	abApi_Write[0] = 0x13;        //command
	abApi_Write[1] = 0x20;      
	abApi_Write[2] = 0x21;              
	abApi_Write[3] = 0x17;      
	abApi_Write[4] = 0x16;               
	abApi_Write[5] = 0x00;             
	abApi_Write[6] = 0x00;     
	abApi_Write[7] = 0x00;              
	bApi_SendCommand(8,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
}

void SI4438StartLDC(void)
{
//??è??μ
//	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
//	abApi_Write[1] = PROP_GLOBAL_GROUP;      // Select property group
//	abApi_Write[2] = 10;               // Number of properties to be written
//	abApi_Write[3] = PROP_GLOBAL_XO_TUNE;     // Specify property
//	abApi_Write[4] = 0x40;              //PROP_GLOBAL_XO_TUNE 
//	abApi_Write[5] = 0x00;              //PROP_GLOBAL_CLK_CFG 
//	abApi_Write[6] = 0x18;              //GLOBAL_LOW_BATT_THRESH 
//	abApi_Write[7] = 0x20;              //PROP_GLOBAL_CONFIG
//	abApi_Write[8] = 0x00;				//PROP_GLOBAL_WUT_CONFIG
//	abApi_Write[9] = 0x00;				//PROP_GLOBAL_WUT_M_15_8
//	abApi_Write[10] = 0x01;				//PROP_GLOBAL_WUT_M_7_0
//	abApi_Write[11] = 0x60;				//PROP_GLOBAL_WUT_R
//	abApi_Write[12] = 0x00;				//PROP_GLOBAL_WUT_LDC
//	abApi_Write[13] = 0x00;				//PROP_GLOBAL_WUT_CAL
//	bApi_SendCommand(14,abApi_Write);        // Send API command to the radio IC
//	vApi_WaitforCTS();                // Wait for CTS
	
	//?è????pin,WUTê±?ó?èía2??§??ê±?′DD????μ?????
//	abApi_Write[0] = 0x13;        //command
//	abApi_Write[1] = 1;	 //GPIO0,èyì?￡?1?±?ê?è?ê?3??y?ˉ
//	abApi_Write[2] = 1;	 //GPIO1,èyì?￡?1?±?ê?è?ê?3??y?ˉ
//	abApi_Write[3] = 0;	 //GPIO2,TR￡?2???±??-à′μ?éè??
//	abApi_Write[4] = 0;	 //GPIO3,BYP, 2???±??-à′μ?éè??
//	abApi_Write[5] = 0x00;
//	abApi_Write[6] = 0x00;
//	abApi_Write[7] = 0x00;
//	bApi_SendCommand(8,abApi_Write);        // Send API command to the radio IC
//	vApi_WaitforCTS();                // Wait for CTS
	//?aê?LDC?￡ê?ê±￡??è′ò?a32kμ?ê±?ó￡??óê±??oó?ù???ˉWUT
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_GLOBAL_GROUP;      // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_GLOBAL_CLK_CFG;     // Specify property
	//?????ú2?32kê±?ó
	abApi_Write[4] = 0x01;  //PROP_GLOBAL_CLK_CFG 
	//????ía2?32kê±?ó￡?μ?GPIO0,GPIO1òy??é?±?D??ó?§??
	//GPIO0,GPIO1±?D??????aê?è??òèyì?
	//abApi_Write[4] = 0x02;  //PROP_GLOBAL_CLK_CFG
	bApi_SendCommand(5,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	//?óê±￡?μè′yê±?ó?è?¨
	delay_ms(500);
	//?è1?WUT/LDC
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_GLOBAL_GROUP;      // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = 0X04;     // ?aê?????
	abApi_Write[4] = 0X00;	//PROP_GLOBAL_WUT_CONFIG
	bApi_SendCommand(5,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	//·￠ò????ú2??üá?
	abApi_Write[0] = 0x1b;      // ò????ú2??üá?
	bApi_SendCommand(1,abApi_Write);
	vApi_WaitforCTS();
	//?a??WUT
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_GLOBAL_GROUP;      // Select property group
	abApi_Write[2] = 5;               // Number of properties to be written
	abApi_Write[3] = PROP_GLOBAL_WUT_M_15_8;     // Specify property
	//abApi_Write[4] = 0x30;	//PROP_GLOBAL_WUT_M_15_8,3秒
	abApi_Write[4] = 0x2c;	//PROP_GLOBAL_WUT_M_15_8,2.8秒
	//abApi_Write[4] = 0x25;	//PROP_GLOBAL_WUT_M_15_8，2.3秒
//abApi_Write[4] = 0x13;	//PROP_GLOBAL_WUT_M_15_8
	abApi_Write[5] = 0x00;	//PROP_GLOBAL_WUT_M_7_0
	abApi_Write[6] = 0x21;	//PROP_GLOBAL_WUT_R
	//abApi_Write[7] = 0x0f;	//PROP_GLOBAL_WUT_LDC
//此处参数需要根据足够多的芯片来确定.以保证所有的板子都可以被唤醒
//abApi_Write[7] = 0x18;	//PROP_GLOBAL_WUT_LDC,5ms
	abApi_Write[7] = 0x24;	//PROP_GLOBAL_WUT_LDC,8ms
//abApi_Write[7] = 0x30;	//PROP_GLOBAL_WUT_LDC,10ms
//abApi_Write[7] = 0x3a;	//PROP_GLOBAL_WUT_LDC,13.45ms,回数正常
	abApi_Write[8] = 0x05;	//PROP_GLOBAL_WUT_CAL
	bApi_SendCommand(9,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	//×?oó′ò?aWUT/LDC  5	
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_GLOBAL_GROUP;      // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_GLOBAL_WUT_CONFIG;     // ?aê?????
	abApi_Write[4] = 0X42;	//PROP_GLOBAL_WUT_CONFIG
	bApi_SendCommand(5,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS

}

void SI4438StartLDC1(void)
{
	//×?oó′ò?aWUT/LDC  5	
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_GLOBAL_GROUP;      // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_GLOBAL_WUT_CONFIG;     // ?aê?????
	abApi_Write[4] = 0X42;	//PROP_GLOBAL_WUT_CONFIG
	bApi_SendCommand(5,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS

}
void SI4438StopLDC1(void)
{
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_GLOBAL_GROUP;      // Select property group
	abApi_Write[2] = 1;               // Number of properties to be written
	abApi_Write[3] = PROP_GLOBAL_WUT_CONFIG;     // ?aê?????
	abApi_Write[4] = 0X00;	//PROP_GLOBAL_WUT_CONFIG
	bApi_SendCommand(5,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
}
void SI4438StopLDC(void)
{
//??è??μ
	abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
	abApi_Write[1] = PROP_GLOBAL_GROUP;      // Select property group
	abApi_Write[2] = 10;               // Number of properties to be written
	abApi_Write[3] = PROP_GLOBAL_XO_TUNE;     // Specify property
	abApi_Write[4] = 0x40;              //PROP_GLOBAL_XO_TUNE 
	abApi_Write[5] = 0x00;              //PROP_GLOBAL_CLK_CFG 
	abApi_Write[6] = 0x18;              //GLOBAL_LOW_BATT_THRESH 
	abApi_Write[7] = 0x20;              //PROP_GLOBAL_CONFIG
	abApi_Write[8] = 0x00;				//PROP_GLOBAL_WUT_CONFIG
	abApi_Write[9] = 0x00;				//PROP_GLOBAL_WUT_M_15_8
	abApi_Write[10] = 0x01;				//PROP_GLOBAL_WUT_M_7_0
	abApi_Write[11] = 0x60;				//PROP_GLOBAL_WUT_R
	abApi_Write[12] = 0x00;				//PROP_GLOBAL_WUT_LDC
	abApi_Write[13] = 0x00;				//PROP_GLOBAL_WUT_CAL
	bApi_SendCommand(14,abApi_Write);        // Send API command to the radio IC
	vApi_WaitforCTS();                // Wait for CTS
	
}

void SI4438EnPSMMode(unsigned char en)
{
		abApi_Write[0] = 0x11;          
		abApi_Write[1] = 0x20; 
		abApi_Write[2] = 0x02;            
		abApi_Write[3] = 0x59;
		if(en)
		{
			abApi_Write[4] = 0x00;
			abApi_Write[5] = 0x80;
		}
		else
		{
			abApi_Write[4] = 0x00;
			abApi_Write[5] = 0x00;
		}
		abApi_Write[6] = 0x60; //ê1?üDSA
		bApi_SendCommand(7,abApi_Write);        // Send command to the radio IC
		vApi_WaitforCTS();
}