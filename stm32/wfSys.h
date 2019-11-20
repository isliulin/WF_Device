#ifndef _wfSys_H
#define _wfSys_H

#include <stdint.h>

#define GetDeltaTick(a) (HAL_GetTick() - a)
#define GetCurTick() HAL_GetTick()

#ifdef KEIL
#define nop() __nop()
#else
#define nop() asm("nop")
#endif

void wfDelay_init(uint32_t SYSCLK);
void wfDelay_nNOP(uint32_t nCount);
void wfDelay_us(uint32_t nus);
#define NOP5()  nop();nop();nop();nop();nop()
#define NOP10() nop();nop();nop();nop();nop();nop();nop();nop();nop();nop()  
#ifdef Delay_ms_wfus
void wfDelay_ms(uint16_t nms);
#else
#define wfDelay_ms(x) HAL_Delay(x)
#endif

#ifdef USE_USART_PUTSTRING
#ifndef  Usart_PutString_Len
#define Usart_PutString_Len 100
#endif
void Usart_PutString(UART_HandleTypeDef *huart,char* pString, ...);
#endif


// #ifndef PRINTF_UART
// #define PRINTF_UART USART1
// #endif
//����Ϊ��ຯ��
// void WFI_SET(void);		//ִ��WFIָ��
// void INTX_DISABLE(void);//�ر������ж�
// void INTX_ENABLE(void);	//���������ж�
// void MSR_MSP(uint32_t addr);	//���ö�ջ��ַ 
#endif

