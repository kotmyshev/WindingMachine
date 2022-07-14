#pragma once
#include "WOconfig.h"


#define MOT2FWD 		(MDR_PORTA->RXTX &= ~0x8);	// PA3 -> CCW2 -> Motor 2 Forward / Reverse
#define MOT2REV			(MDR_PORTA->RXTX |= 0x8);		// PA3 -> CCW2 -> Motor 2 Forward / Reverse

#define MOT2ENB 		(MDR_PORTA->RXTX |= 0x10);	// PA4 -> ENA2 -> Motor 2 Enable
#define MOT2DIS			(MDR_PORTA->RXTX &= ~0x10);	// PA4 -> ENA2 -> Motor 2 Disable

#define MOT1FWD 		(MDR_PORTA->RXTX &= ~0x40);	// PA6 -> CCW1 -> Motor 1 Forward / Reverse
#define MOT1REV			(MDR_PORTA->RXTX |= 0x40);	// PA6 -> CCW1 -> Motor 1 Forward / Reverse

#define MOT1ENB 		(MDR_PORTA->RXTX |= 0x80);	// PA7 -> ENA1 -> Motor 1 Enable
#define MOT1DIS			(MDR_PORTA->RXTX &= ~0x80);	// PA7 -> ENA1 -> Motor 1 Disable

#define BTNCNTR 		(MDR_PORTE->RXTX & 0x02)	// Button: CENTER
#define BTNLFUP 		(MDR_PORTE->RXTX & 0x04)	// Button: LEFT UPPER
#define BTNRTDN 		(MDR_PORTE->RXTX & 0x08)	// Button: RIGHT DOWNER
#define BTNRTUP 		(MDR_PORTE->RXTX & 0x40)	// Button: RIGHT UPPER
#define BTNLFDN 		(MDR_PORTE->RXTX & 0x80)	// Button: LEFT DOWNER

struct WinMachSettings {
		
	int pulseDutyCycle; // default
	int counterPrescaler; // default 25
	int	stepsPerTurn; // default 200
	
};

uint32_t rpmToTicks (const WinMachSettings& sets, const int prmspeed);

void MotorsUpdateSpeed (const WinMachSettings& sets, const int speed);
void MotorsStart(const WinMachSettings& sets, const int speed, bool cw);
void MotorsStop(void);

void MotorsEnable(void);
void MotorsDisable(void);


