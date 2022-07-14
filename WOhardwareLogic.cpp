#include "WOhardwareLogic.h"


uint32_t rpmToTicks (const WinMachSettings& sets, const int prmspeed){
return (prmspeed * sets.stepsPerTurn) / 60;
}


void MotorsUpdateSpeed (const WinMachSettings& sets, const int speed) {
	// Timer2Clock = 1 MHz
		uint32_t tckSpeed	= rpmToTicks(sets, speed);
		uint32_t psgARR = ((1000000/sets.counterPrescaler)/tckSpeed);
		MDR_TIMER2->ARR = psgARR-1; 							
		MDR_TIMER2->CCR3 = (uint32_t)((sets.pulseDutyCycle*psgARR)/100);	
}

void MotorsEnable(void){

	BUFENB;
	MOT1ENB;	
}

void MotorsDisable(void){
		MOT1DIS;
		BUFDIS;
}

void MotorsStart(const WinMachSettings& sets, const int speed, bool cw){
		MDR_TIMER2->CNT = 0x00000000; 								
		// Timer2Clock = 1 MHz
		uint32_t tckSpeed	= rpmToTicks(sets, speed);
		uint32_t psgARR = ((1000000/sets.counterPrescaler)/tckSpeed);
			
		MDR_TIMER2->PSG = sets.counterPrescaler - 1;	
		MDR_TIMER2->ARR = psgARR-1; 							
		MDR_TIMER2->CCR3 = (uint32_t)((sets.pulseDutyCycle*psgARR)/100);
		MDR_TIMER3->ARR = sets.stepsPerTurn; 				
	
		MDR_TIMER3->IE = 0x2;					// Timer3 CNT==ARR Interrup Enable
		NVIC_EnableIRQ(Timer3_IRQn);	// NVIC Interrup Enable for Timer1 	
	
	if (cw) {MOT1FWD;} else {MOT1REV;}
	

	MDR_TIMER2->CNTRL |= 0x1;		
	MDR_TIMER3->CNTRL |= 0x1;	
}

void MotorsStop(void){

	MDR_TIMER2->CNTRL &= ~0x1;		
	MDR_TIMER3->CNTRL &= ~0x1;	

	NVIC_DisableIRQ(Timer3_IRQn);
}

void MotorsUpdateParams(void){

	MDR_TIMER2->CNTRL &= ~0x1;		
	MDR_TIMER3->CNTRL &= ~0x1;	

	NVIC_DisableIRQ(Timer3_IRQn);
}

int GetADC5Result (void){
	while(!(MDR_ADC->ADC1_STATUS & 0x04)){}
return (((uint32_t)0x00000FFF) & MDR_ADC->ADC1_RESULT);
}




