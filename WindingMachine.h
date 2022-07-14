#pragma once

#include "WOhardwareLogic.h"
#include "Worder.h"

class WindingMachine {

	public:	
		
	WindingMachine();
		
	Worder GetCurrentState(void);
	
	Worder GetTargetOrder(void);
	
	void AddOneTurnToCurState(void);
	void SubtracktTurnToCurState(void);
	
	void ResetCurrentState(void);
		
	void SetTargetOrder(unsigned int turnscount, unsigned int turnspeed, bool clockwise);
	
	void SetCurrentState(unsigned int turnscount, unsigned int turnspeed, bool clockwise);
	
	void Start(void);
	
	void StopMachine(void);
	
	void UpdateTargetSpeed(unsigned int newspeed );
	
	void UpdateSettings(const int preScaler, const int dutyCycle, const int motorStepsPerTurn);
	
	private:
		
	void UpSpeedCorrections (void);
		
	void UpdateSpeedOnRun(void);
		
	Worder state_;	// Current State
	
	Worder target_;	// Target Parameters
	
	WinMachSettings settings_; // Winding Machine Counters settings
	
};
