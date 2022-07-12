#pragma once
#include "MDR32Fx.h"
//#include "MDR32F9Qx_config.h"

void CPUClockConfig(void);
void SysTickConfig(void);
void GpiosConfig (void);
//void Timer1Config(void);
void Timer2Config(void);
void Timer3Config(void);
void ADC5Config(void);
int GetADC5Result(void);
void SPIConfig(void);
void InitialConfigAll(void);


#define GPORTB 		MDR_PORTB->RXTX;	// GPIO PORT B RXTX registr

#define BUFENB 		(MDR_PORTA->RXTX &= ~0x1);	// Output Buffer Enable
#define BUFDIS 		(MDR_PORTA->RXTX |= 0x1);		// Output Buffer Disable

#define AUD3ENB 		(MDR_PORTA->RXTX |= 0x2);		// PA1 -> AUD3 Output Enable
#define AUD3DIS 		(MDR_PORTA->RXTX &= ~0x2);	// PA1 -> AUD3 Output Disable

