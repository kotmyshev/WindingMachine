#pragma once

/**
******************************
* LCD1602 8bit class Display
* Date: 2021 Oct 29
*
* Example:
*
*	lcd.DataPortPtr = &MDR_PORTB->RXTX; //port for dara lines DB0...DB7 => PortB B0...B7
*	lcd.CtrlPortPtr = &MDR_PORTB->RXTX; //port for ctrl lines E, RW,RS
*	lcd.PinE = 0x100; 	// E pin -> PB8
*	lcd.PinRW = 0x200;	// RW pin -> PB9
*	lcd.PinRS = 0x400;	// RS pin -> PB10
*	lcd.Init();					// LCD initialization process start
*
*	unsigned char Text1[]= "Privet 999";
*	unsigned char Text2[]= "Privet Andrey";
* lcd.PrintLn(1, Text1);
*	lcd.PrintLn(2, Text2);
******************************
**/

class LCD1602_8bit {

	public:
	
	//Constructors
	LCD1602_8bit();
	LCD1602_8bit(int delayMultiplier);
	
	//Methods
	void PrintLn(const char lineNumber, const char position, const char lineString[]);
	
	void ClearLn(const char lineNumber, const char position, const char clearSize);
	
	void ClearAll();
	
	void Init();
	void SetCommandMode();	
	void SetWRdataMode();
	
	volatile unsigned int *DataPortPtr;
	volatile unsigned int *CtrlPortPtr;
	
	unsigned int PinE;
	unsigned int PinRS;
	unsigned int PinRW;
	
	void DataSend (unsigned char data);
	
	void SetDelayMultiplier(int delayMultiplier);
	
	private:

	void Delay(unsigned int delayTime);
	void SendPosAddress(const char& lineNumber, const char& position);
	void Strob();
	
	int _delayMultiplier;
	
};




