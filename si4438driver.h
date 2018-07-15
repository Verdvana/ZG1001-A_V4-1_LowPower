#include "SI446X_B0_defs.h"
#include "radio_config_si4438.h"


unsigned char bSpi_SendDataNoResp(unsigned char bDataInLength, unsigned char *pbDataIn);
unsigned char bSpi_SendDataGetResp(unsigned char bDataOutLength,unsigned char *pbDataOut);   // Send data, get the response
unsigned char bSpi_SendDataByte(unsigned char bDataIn);
unsigned char bApi_GetResponse(unsigned char bRespLength); // Get response from the chip (used after a command)
unsigned char bApi_GetResponse_without_CTS_detection(unsigned char bRespLength, unsigned char *pbRespData);
unsigned char bApi_WriteTxDataBuffer(unsigned char bTxFifoLength, unsigned char *pbTxFifoData);   // Write Tx FIFO
unsigned char bApi_ReadRxDataBuffer(unsigned char bRxFifoLength, unsigned char *pbRxFifoData);
unsigned char bApi_SendCommand(unsigned char bCmdLength, unsigned char *pbCmdData);   // Send a command + data to the chip
unsigned char bSendPacket(unsigned char bLength, unsigned char *abPayload,unsigned char ch);
unsigned char vApi_WaitforCTS(void);
void Reset_4438(void);
void SI4438Init(unsigned int bps);
void SI4438SetConfiguration(unsigned int bps);
void SI4438ChangeState(unsigned char cmd);
void SI4438StartRX(unsigned char ch);
void SI4438SendTXFIFOData(unsigned char ch);
unsigned char SI4438ReadRXDataFromFIFO(void);
unsigned char SI4438GetFRRA(void);
unsigned char SI4438GetFRRB(void);
unsigned char SI4438GetFRRC(void);
unsigned char SI4438GetFRRD(void);
unsigned char SI4438GetLBD(void);
void SI4438WakeUpDevice(unsigned char channel);
unsigned char SI4438GetState(void);
void SI4438SwitchToTX(void);
void SI4438SwitchToRX(void);
void SI4438StartLDC(void);
void SI4438StartLDC1(void);
void SI4438StopLDC(void);
void SI4438StopLDC1(void);
void SI4438EnPSMMode(unsigned char en);


