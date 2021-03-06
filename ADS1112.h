#ifndef __ADS1112_h__
#define __ADS1112_h__

#include "main.h"
#include "TypeDefine.h"

#ifndef ADS1112_RetryCount
#define ADS1112_RetryCount 2
#endif

#ifndef ADS1112_ADDR_W
#define  ADS1112_ADDR_W	0x90//Ѱַ�ֽ�д
#endif
#ifndef ADS1112_ADDR_R
#define  ADS1112_ADDR_R	0x91//Ѱַ�ֽڶ�
#endif
void ADS1112_Init(void);
typedef union   
{
	struct
	{
		unsigned PGA:2;	
		unsigned DR:2;
		unsigned SC:1;
		unsigned INP:2;//�Ƿ����˵�
		unsigned ST_DRDY:1;//�˵�״̬��0���£�1����
	};
	uint8_t AllFlag;
} _ADS1112_Status;
extern u16_wf ADS1112_Result;
extern _ADS1112_Status ADS1112_Status;
uint8_t ADS1112_Read(void);
uint8_t ADS1112_Write(void);

#endif

