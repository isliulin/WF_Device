#include "HardwareProfile.h"

#ifndef MatrixKeys

#ifdef BigKeys
unsigned int OldKey;
unsigned int KeyCheck(void)
#else
unsigned char OldKey;
unsigned char KeyCheck(void)
#endif
{
#ifdef BigKeys
	unsigned int x,key;
#else
	unsigned char x,key;
#endif	
	KeyCheck_ReadKey(key);	
	if(key==KeyCheck_NoKey)
	{
		OldKey=KeyCheck_NoKey;
		return KeyCheck_NoKey;
	}
	__delay_ms(20);
	KeyCheck_ReadKey(x);	
	if(x!=key)
	{
		OldKey=KeyCheck_NoKey;
		return KeyCheck_NoKey;
	}
	if(key==OldKey)//֮ǰ�Ѿ����������ˣ������ظ�����
	{
		if(!KeyCheckFlags.bReKey)//�������ظ�����
			return KeyCheck_NoKey;
			
		if(GetDeltaTick(KeyTick)>KeyCheck_ReKeyTick)//�����ظ�����������600ms
		{
			KeyTick=SystemTick;
			KeyTimes++;
			return key;
		}
		else
		{
#ifdef KeyCheck_ReKey
			return KeyCheck_ReKey;
#else
			return KeyCheck_NoKey;
#endif
		}			
	}
	else
	{
		KeyTick=SystemTick;
		OldKey=key;
		KeyTimes=0;
		KeyCheckFlags.bReKey=0;
		return key;
	}
}
#else
unsigned char RowKeyValue;
unsigned char ColKeyValue;
unsigned char KeyCheck(void)
{
	unsigned char x,row,col;	
	KeyCheck_SetRow();
	__delay_ms(1);
	KeyCheck_ReadRow(row);	
	if(row==RowNoKey)
	{
		RowKeyValue=0;
		ColKeyValue=0;
		return 0;
	}
	__delay_ms(20);
	KeyCheck_ReadRow(x);	
	if(row!=x)
	{
		RowKeyValue=0;
		ColKeyValue=0;
		return 0;
	}
	KeyCheck_SetCol();
	__delay_ms(1);
	KeyCheck_ReadCol(col);
	if(col==ColNoKey)
	{
		RowKeyValue=0;
		ColKeyValue=0;
		return 0;
	}
	__delay_ms(20);		
	KeyCheck_ReadCol(x);
	if(col!=x)
	{
		RowKeyValue=0;
		ColKeyValue=0;
		return 0;
	}
	if(ColKeyValue==col && RowKeyValue==row)//֮ǰ�Ѿ����������ˣ������ظ�����
	{
		if(!KeyCheckFlags.bReKey)//�������ظ�����
			return 0;
		if(SystemTick-KeyTick>KeyCheck_ReKeyTick)//�����ظ�����������600ms
		{
			KeyTick=SystemTick;
			KeyTimes++;
			KeyCheck_ExFun();
			return 1;
		}
		else 
#ifdef ReKey
			return 2;
#else
			return 0;
#endif
	}
	else
	{
		KeyTick=SystemTick;
		ColKeyValue=col;
		RowKeyValue=row;
		KeyTimes=0;
		KeyCheckFlags.bReKey=0;
		KeyCheck_ExFun();
		return 1;
	}
}
#endif


