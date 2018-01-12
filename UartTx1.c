#include "HardwareProfile.h"
//#define Uart1_BRGVAL ((FCY/Uart1_BAUDRATE)/4)-1

unsigned char UartTxQueueBuf[UartTxQueue_BufLen];
unsigned char *pInUartTxQueueBuf, *pOutUartTxQueueBuf;
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
void UartTxQueue_AddByte(unsigned char tx)
{
	unsigned char *d;
	d = pInUartTxQueueBuf;
	d++;
	if (d == (UartTxQueueBuf+UartTxQueue_BufLen))         //����
		d = UartTxQueueBuf;
	while(d == pOutUartTxQueueBuf);         //TxBuf��
	*pInUartTxQueueBuf = tx;  //����һ������
	pInUartTxQueueBuf = d;    //inTxBuf����
	UartTxQueue_TxStart();
}
void UartTxQueue_AddBytes(const void *tx, unsigned char size, unsigned int n)
{
	unsigned int count = size * n;
	unsigned char *s;
	if(count==0)
		return;
	s = (unsigned char*)tx;
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
