#include "ATCommand.h"
#include <string.h>
#include <ctype.h>
_ATCommand_Flags ATCommand_Flags;
uint8_t ATCommand_RxList[ATCommand_MaxRX];
uint32_t ATCommand_WaitACKTick;
uint8_t ATCommand_RetryTimes;
uint8_t ATCommand_RxCount;
uint32_t ATCommand_WaitACKTimes;
//char ATCommand_EndString[2];
char ATCommand_ErrString[15];
char ATCommand_OkString[15];
char ATCommand_Clear;//�����趨��������ĳ���ض��ַ�
//uint8_t ATCommand_Result;
void ATCommand_Init(void)
{
	ATCommand_WaitACKTimes=5000;
	ATCommand_RetryTimes=3;
	strcpy(ATCommand_OkString,"OK");
	strcpy(ATCommand_ErrString,"ERROR");
	ATCommand_Clear='\0';
	//strcpy(ATCommand_EndString,"\r");
}

void ATCommand_ProcRx(uint8_t rx)
{
	if(ATCommand_Flags.bRx)//֮ǰû������
		return;	
	if(ATCommand_Clear!='\0')
	{
		if(rx==ATCommand_Clear)
		{
			ATCommand_RxCount=0;
			ATCommand_RxList[ATCommand_RxCount]='\0';
			return;
		}		
	}
	if(ATCommand_Flags.bChar)
	{		
		ATCommand_RxList[ATCommand_RxCount++]=rx;
		if(ATCommand_RxCount>=ATCommand_MaxRX)
		{
			ATCommand_RxCount=0;			
		}
		else
		{
			ATCommand_RxList[ATCommand_RxCount]='\0';//��ӽ�����
		}
		if(!ATCommand_Flags.bWaitData)
		{
			ATCommand_ResetWait();
			ATCommand_RxCount=0;
		}
	}
	else
	{
		if(isgraph(rx)==1)
		{
			ATCommand_Flags.bChar=1;
			ATCommand_RxList[ATCommand_RxCount++]=rx;
			if(ATCommand_RxCount>=ATCommand_MaxRX)
			{
				ATCommand_RxCount=0;			
			}
			ATCommand_RxList[ATCommand_RxCount]='\0';//��ӽ�����			
		}
	}
}
uint8_t ATCommand_SendCmd(const char *cmd)
{
	uint8_t res=0; 
	uint8_t retry;
	char *strx=0;
	//uint16_t i;	
	retry=ATCommand_RetryTimes;
	res=ATACK_Null;
	while(retry!=0)
	{		
		ATCommand_PutString(cmd);
		ATCommand_PutString(ATCommand_EndChar);		
		ATCommand_Flags.bRx=0;
		ATCommand_Flags.bChar=0;
		ATCommand_WaitACKTick=GetCurTick();
		ATCommand_RxCount=0;
		ATCommand_Flags.bWaitData=1;
		ATCommand_RxList[ATCommand_RxCount]='\0';//��ӽ�����
		while(1)
		{
			strx=strstr((const char*)ATCommand_RxList,ATCommand_OkString);
			if(strx!=0)
			{
				res=ATACK_OK;
				ATCommand_Flags.bRx=1;
				break;
			}
			else
			{
				strx=strstr((const char*)ATCommand_RxList,ATCommand_ErrString);
				if(strx!=0)
				{
					res=ATACK_Err;
					ATCommand_Flags.bRx=1;
					break;
				}
			}				
			if(GetDeltaTick(ATCommand_WaitACKTick)>ATCommand_WaitACKTimes)//200ms�ȴ���ʱ
				break;
		}
		ATCommand_Flags.bWaitData=0;
		if(res==ATACK_OK)
			break;
		ATCommand_WaitClientInterval();
		retry--;
	}
	strcpy(ATCommand_OkString,"OK");
	strcpy(ATCommand_ErrString,"ERROR");
	ATCommand_Clear='\0';
	//strcpy(ATCommand_EndString,"\r");
	ATCommand_RetryTimes=3;
	ATCommand_WaitACKTimes=5000;
	return res;
}
uint8_t ATCommand_WaitData(void)
{
	uint8_t res=0; 
	char *strx=0;
	res=ATACK_Null;
	ATCommand_Flags.bRx=0;
	ATCommand_Flags.bChar=0;
	ATCommand_WaitACKTick=GetCurTick();
	ATCommand_RxCount=0;
	ATCommand_Flags.bWaitData=1;
	ATCommand_RxList[ATCommand_RxCount]='\0';//��ӽ�����
	while(1)
	{
		strx=strstr((const char*)ATCommand_RxList,ATCommand_OkString);
		if(strx!=0)
		{
			res=ATACK_OK;
			ATCommand_Flags.bRx=1;
			break;
		}
		else
		{
			strx=strstr((const char*)ATCommand_RxList,ATCommand_ErrString);
			if(strx!=0)
			{
				res=ATACK_Err;
				ATCommand_Flags.bRx=1;
				break;
			}
		}					
		if(GetDeltaTick(ATCommand_WaitACKTick)>ATCommand_WaitACKTimes)//200ms�ȴ���ʱ
			break;
	}
	ATCommand_Flags.bWaitData=0;
	strcpy(ATCommand_OkString,"OK");
	strcpy(ATCommand_ErrString,"ERROR");
	ATCommand_Clear='\0';
	//strcpy(ATCommand_EndString,"\r");
	ATCommand_WaitACKTimes=5000;
	return res;
}
void ATCommand_InitWaitData(void)
{
	ATCommand_Flags.bRx=0;
	ATCommand_Flags.bChar=0;
	ATCommand_WaitACKTick=GetCurTick();
	ATCommand_RxCount=0;
	ATCommand_Flags.bWaitData=1;
	ATCommand_RxList[ATCommand_RxCount]='\0';//��ӽ�����	
}
