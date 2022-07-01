
#include "display.h"


// COMAND 		(MDR_PORTB->RXTX &= ~0x400); 	// Comand Mode for LCD
// WRDATA 		(MDR_PORTB->RXTX |= 0x400); 	// Write/Read Data Mode for LCD


LCD1602_8bit::LCD1602_8bit()
	{ 
		_delayMultiplier = 1;
}

LCD1602_8bit::LCD1602_8bit(int delayMultiplier):
	_delayMultiplier(delayMultiplier){ 
}

/*
LCD1602_8bit::LCD1602_8bit(volatile unsigned int& DB07pinsport, volatile unsigned int& Ctrlpinsport, 
	unsigned int E, unsigned int RW, unsigned int RS ):
		DataPortPtr(&DB07pinsport), CtrlPortPtr(&Ctrlpinsport){
		
	 PinE = E;
	 PinRS = RS;
	 PinRW = RW;
		
		Init();				
		}
*/

void LCD1602_8bit::SetDelayMultiplier(int delayMultiplier){

_delayMultiplier = delayMultiplier;
}

void LCD1602_8bit::SendPosAddress(const char& lineNumber, const char& position){

unsigned char code = 0;
	
	switch(lineNumber){	
		case 1: code = 0x80 + position; break;
		case 2: code = 0xC0 + position; break;	
		default: return;
	}
		
	SetCommandMode();
	DataSend(code) ; // Line and Position Select
	SetWRdataMode();

}

void LCD1602_8bit::ClearLn(const char lineNumber, const char position, const char clearSize){

	SendPosAddress(lineNumber, position);
	
	for (unsigned char i=0; i<clearSize; ++i){
	 DataSend(0x20);
	}
}

void LCD1602_8bit::ClearAll(){

	SetCommandMode();
	DataSend(0x1) ; 					// Display Clear
	Delay(5100);							// Wait
	SetWRdataMode();
}

void LCD1602_8bit::PrintLn(const char lineNumber, const char position, const char lineString[]){
		

	SendPosAddress(lineNumber, position);
	
	const char *strPtr;
	strPtr = lineString;
	
		int i =0;
		while( strPtr[i] != '\0')
	{
  DataSend(strPtr[i]);
  i++;
	}
		

	}



void LCD1602_8bit::SetCommandMode(){
*CtrlPortPtr &= ~PinRS;
}	

void LCD1602_8bit::SetWRdataMode(){
*CtrlPortPtr |= PinRS;
}


void LCD1602_8bit::Init(){

	
	Delay(70000); 						// Display Start Time >20 ms
	SetCommandMode(); 												
	DataSend(0x38) ; 					// Function Set: 8-bit, 2-line
	Delay(1500);							// Wait >37 us
	DataSend(0x0C) ; 					// Display Set (No Cursor & No Blinking)
	//DataSend(0x0F) ; 					// Display Set (Cursor & Blinking)
	Delay(1500);							// Wait >37 us
	DataSend(0x6) ; 					// Entery Mode Set (Right-Moving Cursor)
	Delay(1500);							// Wait >37 us
	DataSend(0x1) ; 					// Display Clear
	Delay(51000);							// Wait >1.53 ms
	
}

void LCD1602_8bit::Delay(unsigned int delayTime)
{
	for (unsigned int i=0; i < (delayTime * _delayMultiplier); i++);
}


void LCD1602_8bit::Strob(){
	
*CtrlPortPtr |= PinE;
 Delay(10);
*CtrlPortPtr &= ~PinE;
Delay(40);
}

	void LCD1602_8bit::DataSend (unsigned char data){
	
	*DataPortPtr |= data;	
	Delay(10);
		Strob();
	*DataPortPtr &= ~0xFF;	
		
	}
	
	char Itoa(unsigned int number){
	
		return 0;
	}

