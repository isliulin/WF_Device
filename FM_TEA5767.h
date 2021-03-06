#ifndef __FM_TEA5767_h__
#define __FM_TEA5767_h__

#ifndef FM_TEA5767_RetryCount
#define FM_TEA5767_RetryCount 2
#endif

#ifndef FM_TEA5767_PerStep
#define FM_TEA5767_PerStep 14
#endif


#ifndef FM_TEA5767_MaxPLL
#define FM_TEA5767_MaxPLL 0x331b
#endif

#ifndef FM_TEA5767_MinPLL
#define FM_TEA5767_MinPLL 0x299d
#endif

typedef union   
{
	struct
	{		
		unsigned bAddPLL:1;	
		unsigned bSearch:1;
	};
	uint8_t AllFlag;
} _FM_TEA5767_Flags;        // general flags

_FM_TEA5767_Flags FM_TEA5767_Flags;
uint8_t FM_TEA5767_ReadData[5];        //TEA5767读出的状态
uint8_t FM_TEA5767_WriteData[5];  //要写入TEA5767的数据
uint16_t FM_SearchTick;

uint8_t FM_TEA5767_Read(void);
void FM_TEA5767_Init(void);
uint8_t FM_TEA5767_Write(void);
void FM_TEA5767_NextChannel(void);
#endif
