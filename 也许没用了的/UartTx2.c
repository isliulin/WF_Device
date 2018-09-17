#include "HardwareProfile.h"
//#define Uart1_BRGVAL ((FCY/Uart1_BAUDRATE)/4)-1
#ifdef UartTxQueue2
uint8_t UartTxQueue2Buf[UartTxQueue2_BufLen];
uint8_t *pInUartTxQueue2Buf, *pOutUartTxQueue2Buf;

// void UartTxQueue2_TxStart()
// {	
// 	if (UartTx2_TXIF && !UartTx2_TXIE)        
// 	{
// 		UartTx2_TXIE=1;
// 	}		
// }
void UartTxQueue2_AddStr(const char* pt)
{
	const char *p;
	p = pt;
	while (*p != '\0')                         //������뻺����
	{
		UartTxQueue2_AddByte(*p++);
	}
}
void UartTxQueue2_AddByte(uint8_t tx)
{
	uint8_t *d;
	d = pInUartTxQueue2Buf;
	d++;
	if (d == (UartTxQueue2Buf+UartTxQueue2_BufLen))         //����
		d = UartTxQueue2Buf;
	while(d == pOutUartTxQueue2Buf);         //TxBuf��
	*pInUartTxQueue2Buf = tx;  //����һ������
	pInUartTxQueue2Buf = d;    //inTxBuf����
	UartTx2_TXIE=1;
	//UartTxQueue2_TxStart();
}
void UartTxQueue2_AddBytes(const void *tx, uint8_t size, uint16_t n)
{
	uint16_t count = size * n;
	uint8_t *s;
	if(count==0)
		return;
	s = (uint8_t*)tx;
	while (count--)                         //������뻺����
	{
		UartTxQueue2_AddByte(*s++);
	}
}
void UartTxQueue2_Interrupt(void)
{	
	if (pOutUartTxQueue2Buf == pInUartTxQueue2Buf)             //TxBuf��
	{
		UartTx2_TXIE=0;
		return;
	}
	UartTx2_TXIF=0;
	UartTx2_TXREG=*pOutUartTxQueue2Buf;
	pOutUartTxQueue2Buf++;
	if (pOutUartTxQueue2Buf == (UartTxQueue2Buf+UartTxQueue2_BufLen))
		pOutUartTxQueue2Buf = UartTxQueue2Buf;	
}
void UartTxQueue2_Init( void )
{
// 	U1MODE=0;
// 	//U1MODEbits.UFRZ=1;//���������ڵ���ģʽʱ��ģ��ֹͣ����
// 	U1MODEbits.USIDL=1;//�������������ģʽʱ��ģ��ֹͣ����							
// 	U1MODEbits.BRGH=1;//����
// 	U1STA=0;
// 	//U2STAbits.UTXINV=0;//UxTX �Ŀ���״̬Ϊ1
// 
// 	U1BRG=Uart1_BRGVAL;//38400(8MHz)
// 	U1MODEbits.UARTEN=1;
// 	U1STAbits.UTXEN=1;//ʹ��UARTx ����������UARTx ����UxTX ���ţ����UARTEN = 1��
	pInUartTxQueue2Buf=pOutUartTxQueue2Buf=UartTxQueue2Buf;
}
#else
void UartTx2Byte(uint16_t tx)
{
	while(!UartTx2_TXIF);
	UartTx2_TXREG=tx;
}

void UartTx2Bytes(const void *tx, uint8_t size, uint16_t n)
{
	uint16_t count=n*size;
	uint8_t* ptx=(uint8_t*)tx;
	while(count--)
	{
		UartTx2_TXREG=*ptx++;
		while(!UartTx2_TXIF);
	}
}
#endif
