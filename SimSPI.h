#ifndef __SimSPI_h__
#define __SimSPI_h__

#include "HardwareProfile.h"//SimSPI_Conf.h"

#ifndef SimSPI_Delay_Ex
#define SimSPI_Delay() 
#endif

#if defined(SimSPI_SPI0) || defined(SimSPI_SPI1) || defined(SimSPI_SPI2) || defined(SimSPI_SPI3) 
#else
#define SimSPI_SPI0
#endif

void SimSPI_Init(void);
uint8_t SimSPI_Proc(uint8_t c);

#ifdef SimSPI2
	#ifndef SimSPI2_Delay_Ex
	#define SimSPI2_Delay()
	#endif

	#if defined(SimSPI2_SPI0) || defined(SimSPI2_SPI1) || defined(SimSPI2_SPI2) || defined(SimSPI2_SPI3) 
	#else
	#define SimSPI2_SPI0
	#endif

	void SimSPI2_Init(void);
	uint8_t SimSPI2_Proc(uint8_t c);
#endif

#ifdef SimSPI3
	#ifndef SimSPI3_Delay_Ex
	#define SimSPI3_Delay()
	#endif

	#if defined(SimSPI3_SPI0) || defined(SimSPI3_SPI1) || defined(SimSPI3_SPI2) || defined(SimSPI3_SPI3) 
	#else
	#define SimSPI3_SPI0
	#endif

	void SimSPI3_Init(void);
	uint8_t SimSPI3_Proc(uint8_t c);
#endif

#endif // __SimSPI_h__
