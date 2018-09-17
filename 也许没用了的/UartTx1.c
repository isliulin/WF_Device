#include "HardwareProfile.h"
//#define Uart1_BRGVAL ((FCY/Uart1_BAUDRATE)/4)-1

uint8_t UartTxQueueBuf[UartTxQueue_BufLen];
uint8_t *pInUartTxQueueBuf, *pOutUartTxQueueBuf;
void UartTxQueue_TxStart()
{	
	if (Uart_TXIF && !Uart_TXIE)        
	{
		Uart_TXIE=1;
		gFlags.bUartTxQueueFinish=0;
	}		
}
void UartTxQueue_AddStr(const char* pt)
{
	const char *p;
	p = pt;
	while (*p != '\0')                         //������뻺����
	{
		UartTxQueue_AddByte(*p++);
	}
}
void UartTxQueue_AddByte(uint8_t tx)
{
	uint8_t *d;
	d = pInUartTxQueueBuf;
	d++;
	if (d == (UartTxQueueBuf+UartTxQueue_BufLen))         //����
		d = UartTxQueueBuf;
	while(d == pOutUartTxQueueBuf);         //TxBuf��
	*pInUartTxQueueBuf = tx;  //����һ������
	pInUartTxQueueBuf = d;    //inTxBuf����
	UartTxQueue_TxStart();
}
void UartTxQueue_AddBytes(const void *tx, uint8_t size, uint16_t n)
{
	uint16_t count = size * n;
	uint8_t *s;
	if(count==0)
		return;
	s = (uint8_t*)tx;
	while (count--)                         //������뻺����
	{
		UartTxQueue_AddByte(*s++);
	}
}
void UartTxQueue_Interrupt(void)
{	
	if (pOutUartTxQueueBuf == pInUartTxQueueBuf)             //TxBuf��
	{
		Uart_TXIE=0;
		gFlags.bUartTxQueueFinish=1;
		return;
	}
	//UartTx1_TXIF=0;
	UartTx1_TXREG=*pOutUartTxQueueBuf;
	pOutUartTxQueueBuf++;
	if (pOutUartTxQueueBuf == (UartTxQueueBuf+UartTxQueue_BufLen))
		pOutUartTxQueueBuf = UartTxQueueBuf;	
}
void UartTxQueue_Init( void )
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
	pInUartTxQueueBuf=pOutUartTxQueueBuf=UartTxQueueBuf;
}

