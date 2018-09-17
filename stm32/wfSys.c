

#include "wfSys.h"
#include "main.h"
#include <stdarg.h>

static uint32_t fac_us=0;							//us��ʱ������

//��ʼ���ӳٺ���
//SYSCLK:ϵͳʱ��Ƶ��
void wfDelay_init(uint8_t SYSCLK)
{
	fac_us=SYSCLK;
}

//��ʱnus
//nusΪҪ��ʱ��us��.	
//nus:0~190887435(���ֵ��2^32/fac_us@fac_us=22.5)	 
void wfDelay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
}
//��ʱnms
//nms:Ҫ��ʱ��ms��
#ifdef Delay_ms_wfus
void wfDelay_ms(uint16_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) wfDelay_us(1000);
}
#endif

#ifdef USE_USART_PUTSTRING
void Usart_PutString(UART_HandleTypeDef *huart,char* pString, ...)
{
	//vsnprintf
	va_list ap;
	char s[Usart_PutString_Len];
	int n=0;
	va_start(ap, pString); //��ÿɱ�����б�,����fmt���ó��������������ͣ�Ҳ�Ƿ������������ơ�%d%s�����ַ���
	n=vsnprintf (s, Usart_PutString_Len, pString, ap); //д���ַ���s
	va_end(ap); //�ͷ���Դ,������va_start�ɶԵ���
	HAL_UART_Transmit(huart,s,n,2000);
}
#endif
// 
// //THUMBָ�֧�ֻ������
// //�������·���ʵ��ִ�л��ָ��WFI  
// __asm void WFI_SET(void)
// {
// 	WFI;		  
// }
// //�ر������ж�(���ǲ�����fault��NMI�ж�)
// __asm void INTX_DISABLE(void)
// {
// 	CPSID   I
// 	BX      LR	  
// }
// //���������ж�
// __asm void INTX_ENABLE(void)
// {
// 	CPSIE   I
// 	BX      LR  
// }
// //����ջ����ַ
// //addr:ջ����ַ
// __asm void MSR_MSP(uint32_t addr) 
// {
// 	MSR MSP, r0 			//set Main Stack value
// 	BX r14
// }
