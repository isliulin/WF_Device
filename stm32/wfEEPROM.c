
#include "wfEEPROM.h"


void wfEEPROM_ReadBytes(uint32_t Addr,uint8_t *Buffer,uint32_t Length) 
{  
	uint8_t *wAddr;  
	wAddr=(uint8_t*)(wfEEPROM_BASE_ADDR+Addr);  
	while(Length--)
	{  
		*Buffer++=*wAddr++;  
	}     
}

void wfEEPROM_ReadHalfWords(uint32_t Addr,uint16_t *Buffer,uint32_t Length)  
{  
	uint32_t *wAddr;  
	wAddr=(uint32_t*)(wfEEPROM_BASE_ADDR+Addr);  
	while(Length--)
	{  
		*Buffer++=*wAddr++;  
	}     
} 

void wfEEPROM_ReadWords(uint32_t Addr,uint32_t *Buffer,uint32_t Length)  
{  
	uint32_t *wAddr;  
	wAddr=(uint32_t*)(wfEEPROM_BASE_ADDR+Addr);  
	while(Length--)
	{  
		*Buffer++=*wAddr++;  
	}     
}
#ifdef STM32_Flash4EEPROM
// void wfEEPROM_WriteBytes(uint32_t WriteAddr,uint8_t *pBuffer,uint32_t NumToWrite)  
// {
// 	FLASH_EraseInitTypeDef FlashEraseInit;
// 	HAL_StatusTypeDef FlashStatus=HAL_OK;
// 	uint32_t SectorError=0;
// 	uint32_t addrx=0;
// 	uint32_t endaddr=0;	
// 	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//�Ƿ���ַ
// 
// 	HAL_FLASH_Unlock();             //����	
// 	addrx=WriteAddr;				//д�����ʼ��ַ
// 	endaddr=WriteAddr+NumToWrite*4;	//д��Ľ�����ַ
// 
// 	if(addrx<0X1FFF0000)
// 	{
// 		while(addrx<endaddr)		//ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
// 		{
// 			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
// 			{   
// 				FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //�������ͣ��������� 
// 				FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //Ҫ����������
// 				FlashEraseInit.NbSectors=1;                             //һ��ֻ����һ������
// 				FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //��ѹ��Χ��VCC=2.7~3.6V֮��!!
// 				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
// 				{
// 					break;//����������	
// 				}
// 			}else addrx+=4;
// 			FLASH_WaitForLastOperation(FLASH_WAITETIME);                //�ȴ��ϴβ������
// 		}
// 	}
// 	FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //�ȴ��ϴβ������
// 	if(FlashStatus==HAL_OK)
// 	{
// 		while(WriteAddr<endaddr)//д����
// 		{
// 			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//д������
// 			{ 
// 				break;	//д���쳣
// 			}
// 			WriteAddr+=4;
// 			pBuffer++;
// 		}  
// 	}
// 	HAL_FLASH_Lock();           //����
// } 
// 
// void wfEEPROM_WriteHalfWords(uint32_t WriteAddr,uint16_t *pBuffer,uint32_t NumToWrite)  
// {  
// 	uint16_t t;  
// 	HAL_FLASHEx_DATAEEPROM_Unlock();      
// 	for(t = 0;t < NumToWrite;t++)  
// 	{  
// 		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_HALFWORD,wfEEPROM_BASE_ADDR + WriteAddr + t,*(pBuffer + t));  
// 	}  
// 	HAL_FLASHEx_DATAEEPROM_Unlock();  
// }
// 
// void wfEEPROM_WriteWords(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)  
// {  
// 	uint16_t t;  
// 	HAL_FLASHEx_DATAEEPROM_Unlock();      
// 	for(t = 0;t < NumToWrite;t++)  
// 	{  
// 		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,wfEEPROM_BASE_ADDR + WriteAddr + t,*(pBuffer + t));  
// 	}  
// 	HAL_FLASHEx_DATAEEPROM_Unlock();  
// }
void wfEEPROM_WriteWords(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	
{ 
	uint32_t i;
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus=HAL_OK;
	uint32_t SectorError=0;
	HAL_FLASH_Unlock();             //����	
	FlashEraseInit.TypeErase=FLASH_TYPEERASE_PAGES;       //�������ͣ�ҳ���� 
	FlashEraseInit.PageAddress=WriteAddr;   //Ҫ������ҳ����д���ַ
	FlashEraseInit.NbPages=1;               //ֻ����һҳ
	if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
	{
		HAL_FLASH_Lock();           //����
		return;//����������	
	}	
	FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //�ȴ��ϴβ������
	if(FlashStatus==HAL_OK)
	{
		for(i=0;i<NumToWrite;i++)
		{
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//д������
			{ 
				break;	//д���쳣
			}
			WriteAddr+=4;
			pBuffer++;
		}
	}
	HAL_FLASH_Lock();           //����
}
#else
void wfEEPROM_WriteBytes(uint32_t WriteAddr,uint8_t *pBuffer,uint32_t NumToWrite)  
{  
	uint32_t t;  
	HAL_FLASHEx_DATAEEPROM_Unlock();      
	for(t = 0;t < NumToWrite;t++)  
	{  
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,wfEEPROM_BASE_ADDR + WriteAddr + t,*(pBuffer + t));  
	}  
	HAL_FLASHEx_DATAEEPROM_Lock();  
} 

void wfEEPROM_WriteHalfWords(uint32_t WriteAddr,uint16_t *pBuffer,uint32_t NumToWrite)  
{  
	uint32_t t;  
	HAL_FLASHEx_DATAEEPROM_Unlock();      
	for(t = 0;t < NumToWrite;t++)  
	{  
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_HALFWORD,wfEEPROM_BASE_ADDR + WriteAddr + t,*(pBuffer + t));  
	}  
	HAL_FLASHEx_DATAEEPROM_Lock();  
}

void wfEEPROM_WriteWords(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)  
{  
	uint32_t t;  
	HAL_FLASHEx_DATAEEPROM_Unlock();      
	for(t = 0;t < NumToWrite;t++)  
	{  
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,wfEEPROM_BASE_ADDR + WriteAddr + t,*(pBuffer + t));  
	}  
	HAL_FLASHEx_DATAEEPROM_Lock();  
}
#endif


// void wfEEPROM_WriteByte( uint16_t WriteAddr,uint8_t data )
// {
// 	uint16_t t;  
// 	HAL_FLASHEx_DATAEEPROM_Unlock();      
// 	for(t = 0;t < NumToWrite;t++)  
// 	{  
// 		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,wfEEPROM_BASE_ADDR + WriteAddr + t,*(pBuffer + t));  
// 	}  
// 	HAL_FLASHEx_DATAEEPROM_Unlock();  
// }
// 
// void wfEEPROM_WriteHalfWord( uint16_t WriteAddr,uint16_t data )
// {
// 
// }
// 
// void wfEEPROM_WriteWord( uint16_t WriteAddr,uint32_t data )
// {
// 
// }
