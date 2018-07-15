#include "stm8l15x.h"
#include "main.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_flash.h"
//#include  "iostm8l052C6.h"




unsigned char Spi_Write_one_byte(unsigned char data_in);
unsigned char Spi_Read_one_byte(void);
unsigned char ReadCFGDataFromEEPROM(void);
void SaveCFGDataToEEPROM(void);
void SaveCFGDataToEEPROM2(void);
