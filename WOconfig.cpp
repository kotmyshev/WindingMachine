
#include "WOconfig.h"

void CPUClockConfig(void){
//HCLK = PLLCPUo = HSE_Value*(3+1) = 32 MHz
// #define HSE_Value       ((uint32_t)8000000)
		
MDR_RST_CLK->HS_CONTROL = ((uint32_t)0x00000001); //HSE ON
while((MDR_RST_CLK->CLOCK_STATUS & ((uint32_t)0x00000004)) != ((uint32_t)0x00000004));	// WAIT RST_CLK_CLOCK_STATUS_HSE_RDY

MDR_RST_CLK->CPU_CLOCK |= ((uint32_t)0x00000002); //RST_CLK_CPU_CLOCK_CPU_C1_SEL_HSE
	
MDR_RST_CLK->PLL_CONTROL |= ((uint32_t)0x00000300); //PLL_CPU_MULL = 3, PLLCPUo = HSE_Value*(3+1) = 32 MHz
MDR_RST_CLK->PLL_CONTROL |= ((uint32_t)0x00000004);	//PLL_CPU_ON -> PLL Enable
	
while((MDR_RST_CLK->CLOCK_STATUS & ((uint32_t)0x00000002)) != ((uint32_t)0x00000002));	// WAIT RST_CLK_CLOCK_STATUS_PLL_CPU_RDY	

MDR_RST_CLK->CPU_CLOCK |= ((uint32_t)0x00000004); //RST_CLK_CPU_CLOCK_CPU_C2_SEL_PLLCPUo
	
MDR_RST_CLK->CPU_CLOCK |= ((uint32_t)0x00000100); //RST_CLK_CPU_CLOCK_HCLK_SEL_CPU_C3
 	
SystemCoreClockUpdate();
}

void GpiosConfig (void){
/* ************** General Purpose Input-Output Ports A-F Configuration  *********** */	
	MDR_RST_CLK->PER_CLOCK |= (1<<21); 		// clock turn on: PORTA
	MDR_RST_CLK->PER_CLOCK |= (1<<22); 		// clock turn on: PORTB
	MDR_RST_CLK->PER_CLOCK |= (1<<23); 		// clock turn on: PORTC
	MDR_RST_CLK->PER_CLOCK |= (1<<24); 		// clock turn on: PORTD
	MDR_RST_CLK->PER_CLOCK |= (1<<25); 		// clock turn on: PORTE
	MDR_RST_CLK->PER_CLOCK |= (1<<29); 		// clock turn on: PORTF
	

MDR_PORTA->OE |= 0xFF;														// PA0...PA7 -> Output mode	
MDR_PORTA->ANALOG |= 0xFF; 												// PA0...PA7 -> Digital mode
MDR_PORTA->PULL	|= 0xFF;													// PA0...PA7 -> Pull-down
MDR_PORTA->PWR |= 0xFFFF;													// PA0...PA7 -> High-speed mode (10 ns)
MDR_PORTA->FUNC |= 	0xC20;												// PA2, PA5 -> Alternate Function Modes	
	
MDR_PORTB->OE |= 0x7FF;														// PB0...PB10 -> Output mode
MDR_PORTB->ANALOG |= 0x7FF;												// PB0...PB10 -> Digital mode
MDR_PORTB->PULL	|= 0x7FF;													// PB0...PB10 -> Pull-down	
MDR_PORTB->PWR |= 0x3FFFFF;												// PB0...PB10 -> High-speed mode

MDR_PORTC->OE |= 0x7;															// PC0...PC2 -> Output mode	
MDR_PORTC->ANALOG |= 0x7;													// PC0...PC2 -> Digital mode
MDR_PORTC->PULL |= 0x7;														// PC0...PC2 -> Pull-down
MDR_PORTC->PWR |= 0x2A;														// PC0...PC2 -> Midl-speed mode

MDR_PORTD->OE |= 0xC0;														// PD6, PD7 -> Output mode	
MDR_PORTD->ANALOG |= 0xDF;												// PD0..PD4, PD6, PD7 -> Digital mode
MDR_PORTD->PULL |= 0x1F00E0;											// PD0...PD4 -> UP, PD5...PD7 -> DN
MDR_PORTD->PWR |= 0xA000;													// PD6, PD7 -> Midl-speed mode

MDR_PORTE->OE |= 0x0;															// Port E - Input mode
MDR_PORTE->ANALOG |= 0xCF;												// PE0...PE3, PE6, PE7 Digital mode
MDR_PORTE->PULL |= 0xCF;													// PE0...PE3, PE6, PE7 Pull-down
MDR_PORTE->PWR |= 0x0;														// Port E - Input mode, TX Off
MDR_PORTE->GFEN |= 0xCF;													// PE0...PE3, PE6, PE7 Filter Enable
	

// PF3...PF0 -> SPI

MDR_PORTF->OE |= 0x07;														// PF2, PF1, PF0 -> Output mode
MDR_PORTF->ANALOG |= 	0x0F;												// PF3...PF0 -> Digital mode
MDR_PORTF->PWR |= 0xFF;														// PF3...PF0 -> High-speed mode
MDR_PORTF->FUNC |= 0x8A;													// PF3, PF1, PF0 -> SSP/SPI; PF2 -> GPIO CS#
MDR_PORTF->PULL |= 	0x60000;											// PF1, PF2 -> Pull-UP

MDR_PORTF->RXTX |=  0x04;													// PF2 -> GPIO CS# -> High

}

void SPIConfig (void){

	MDR_RST_CLK->PER_CLOCK |= (1<<8); 		// clock turn on: SPI1
	MDR_RST_CLK->SSP_CLOCK |= (1<<24); 		// clock enable on: SPI1
	
	MDR_SSP1->CPSR |= 0x10;	// clk dev by 16
	MDR_SSP1->CR0 |= 0x07;	// word len = 8 bit, SPI Motorola, SPO=0, SPH=0, SCR=0
	MDR_SSP1->CR1 |= 0x02; 	//Enabled, Master
	
}


void SysTickConfig(void){

SysTick->LOAD = 0x5FFFFF;
SysTick->CTRL = 0x06;	

}



// Timer 2 in PWM mode - for stepmotorDiver pulseCLK
void Timer2Config(void){

		MDR_RST_CLK->PER_CLOCK |= (1<<15); 		// clock turn on: TIMER2
		MDR_RST_CLK->TIM_CLOCK |= 0x500; 			// TIM2 BRG 0x5 -> TIM2_CLK == HCLK/32
		MDR_RST_CLK->TIM_CLOCK |= (1<<25); 		// clock enabled: TIMER2	
		MDR_TIMER2->CNTRL = 0x00000000; 			// Initialization TIMER2	
		MDR_TIMER2->CNT = 0x00000000; 				// Counter register initial value 
		MDR_TIMER2->PSG = 99;									// Prescaler register 
		MDR_TIMER2->ARR = 0x0003; 						// Auto-reload register value 	
		//MDR_TIMER2->IE = 0x000003; 					// Interrupt Enabled for timer chanels
		MDR_TIMER2->CCR3 = 2;
		MDR_TIMER2->CH3_CNTRL1 |= 0x09;				// 
		//MDR_TIMER2->CH3_CNTRL	 |= 0xE00;		//  REF = 1, THEN CNT > CCR 
		MDR_TIMER2->CH3_CNTRL	 |= 0xC00;			//  REF = 1, THEN CNT < CCR 
		//MDR_TIMER2->CH3_CNTRL	 |= 0x200;		//  REF = 1, THEN CNT = CCR only
}

// Timer 3 in counter mode with EVENT_SEL 0010 -> CNT==ARR in Timer2
void Timer3Config(void){

		MDR_RST_CLK->PER_CLOCK |= (1<<16); 		// clock turn on: TIMER3	
		MDR_RST_CLK->TIM_CLOCK |= 0x50000;  	// TIM3 BRG 0x5 -> TIM3_CLK == HCLK/32	
		MDR_RST_CLK->TIM_CLOCK |= (1<<26);
		MDR_TIMER3->CNTRL = 0x00000280; 			// Initialization TIMER3
		MDR_TIMER3->CNT = 0x0000000; 					// Counter register initial value 
		MDR_TIMER3->PSG = 0x0;								// Prescaler register 
		MDR_TIMER3->ARR = 200; 								// Auto-reload register value 	
		MDR_TIMER3->IE = 0x2;									// Timer3 CNT==ARR Interrup Enable
		NVIC_EnableIRQ(Timer3_IRQn);					// NVIC Interrup Enable for Timer1 	
		
}

// ADC Configuration on gpio PD5 -> Analog mode ADC5 
void ADC5Config(void){
	MDR_RST_CLK->PER_CLOCK |= (1<<17); 		// clock turn on: ADC
	MDR_ADC->ADC1_CFG |= 0xE00A059;	// ADC-ON, CHS=5, DIVCLK = PCLK/1024, DelayGo=8, TR=0
}




void InitialConfigAll(void){
	
	CPUClockConfig();	
__disable_irq();
	SysTickConfig();	
	GpiosConfig();	
	ADC5Config();
	//Timer1Config();
	Timer2Config();
	Timer3Config();
	//SPIConfig();
	
__enable_irq(); 
	
	
	
}


/*

// Timer1 not used in this project
void Timer1Config(void){
		//MDR_RST_CLK->PER_CLOCK |= (1<<14); 		//  clock turn on: TIMER1
		//MDR_RST_CLK->TIM_CLOCK |= (1<<24); 		// clock enabled: TIMER1		
		//MDR_TIMER1->CNTRL = 0x00000000; 			// Initialization TIMER1
		//MDR_TIMER1->CNT = 0x00000001; 				// Counter register initial value 
		//MDR_TIMER1->PSG = 999;								// Prescaler register 
		//MDR_TIMER1->ARR = 0x000FFA0; 					// Auto-reload register value 
		//MDR_TIMER1->IE = 0x2;									// Timer2 CNT==ARR Interrup Enable

		//NVIC_SetPriority(Timer1_IRQn, 3);	
		//NVIC_EnableIRQ(Timer1_IRQn);					// NVIC Interrup Enable for Timer1 			
}



// ----------------- Timers Configuration ---------------------- 


	MDR_RST_CLK->PER_CLOCK |= (1<<15); 		//  clock turn on: TIMER2
	MDR_RST_CLK->PER_CLOCK |= (1<<16); 		//  clock turn on: TIMER3
	
	
	MDR_RST_CLK->TIM_CLOCK |= (1<<25); 		// clock enabled: TIMER2
	//MDR_RST_CLK->TIM_CLOCK |= (1<<26); 		// clock enabled: TIMER3
	
	// Timer 2 Configuration -> PWM Mode 
	
	MDR_TIMER2->CNTRL = 0x00000000; 			// Initialization TIMER2
	
	MDR_TIMER2->CNT = 0x00000000; 				// Counter register initial value 
	MDR_TIMER2->PSG = 99;									// Prescaler register 
	MDR_TIMER2->ARR = 0x000A0; 				// Auto-reload register value 
	
	//MDR_TIMER2->IE = 0x000001E0; 					// Interrupt Enabled
	
	MDR_TIMER2->CCR3 = 50;
	//MDR_TIMER2->CH3_CNTRL2 |= 0x04;			// CCR1?
	MDR_TIMER2->CH3_CNTRL1 |= 0x09;				// ?? ????? ???????? ?????? REF, ????? ???????? ?? ????? ??????
	MDR_TIMER2->CH3_CNTRL	 |= 0xE00;			//  REF = 1, THEN CNT > CCR 
	

	// Timer 3 Configuration 
	
	MDR_TIMER1->CNTRL = 0x280;  	// Initialization TIMER3
																// Set Event from TIMER2 then CNT == ARR, CNT_MODE = 10
	MDR_TIMER1->CNT = 0x001; 			// Counter register initial value 
	MDR_TIMER1->PSG = 0x000;			// Prescaler register 
	MDR_TIMER1->ARR = 0x200; 			// Auto-reload register value 
	
	


	
}

*/
