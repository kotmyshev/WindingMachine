#include "WindingMachine.h"

WindingMachine::WindingMachine(){
	settings_.counterPrescaler = 25;
	settings_.pulseDutyCycle = 25;
	settings_.stepsPerTurn = 200;
}

void WindingMachine::UpSpeedCorrections(void){
	
	if (state_.speed != target_.speed){
		UpdateSpeedOnRun();
		}
	}

	Worder WindingMachine::GetCurrentState(void){
	return state_;
	}
	
	Worder WindingMachine::GetTargetOrder(void){
	return target_;
	}
	
	void WindingMachine::AddOneTurnToCurState(void){
	++state_.turns;			
		UpSpeedCorrections();
	}
	
	void WindingMachine::SubtracktTurnToCurState(void){
	--state_.turns;	
		UpSpeedCorrections();
	}
	
	void WindingMachine::ResetCurrentState(void){
		state_.speed = 0;
		state_.turns = 0;		
	}
	
	void WindingMachine::SetTargetOrder( unsigned int turnscount, unsigned int turnspeed, bool cclockwise){
	target_.turns = turnscount;
	target_.speed = turnspeed;
	target_.ccw = cclockwise;
	}	
	
	
	void WindingMachine::SetCurrentState( unsigned int turnscount, unsigned int turnspeed, bool cclockwise){
	state_.turns = turnscount;
	state_.speed = turnspeed;
	state_.ccw = cclockwise;
	}	
	
void WindingMachine::UpdateSpeedOnRun(void){
	
	uint8_t increment = (state_.speed > 700) ? 1 : 50;
	
			int tempspeed = ((target_.speed - state_.speed) > increment ) ? state_.speed + increment : target_.speed;
			MotorsUpdateSpeed(settings_, tempspeed);	
			state_.speed = tempspeed;
	}

	void WindingMachine::UpdateTargetSpeed(unsigned int newspeed){
			target_.speed = newspeed;
			UpdateSpeedOnRun();
	}
		
	void WindingMachine::Start(void){		
		int tempspeed = ((target_.speed - state_.speed) > 100 ) ? state_.speed + 100 : target_.speed;	
		MotorsStart(settings_, tempspeed, target_.ccw);
		state_.speed = tempspeed;		
	}
	
	
	void WindingMachine::StopMachine(void){
	MotorsStop();
		state_.speed = 0;
	}
	

  void WindingMachine::UpdateSettings(const int preScaler, const int dutyCycle, const int motorStepsPerTurn){
	settings_.counterPrescaler = preScaler;
	settings_.pulseDutyCycle = dutyCycle;
	settings_.stepsPerTurn = motorStepsPerTurn;
	
	}
	
