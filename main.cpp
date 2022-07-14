
/**
******************************
* K1986BE92QI Winding Board One
* Start Date: 2021 Oct 20
* Redact Date: 2022 Jul 15
******************************
**/

#include "display.h"
#include "WindingMachine.h"
#include "conversion.h"

const int maxturns = 9999;
const int minturns = 1;

const int maxspeed = 750; //RPM
const int minspeed = 1;

const int defmulti = 1;
const int addmulti = 10;
const int maxmulti = 1000;

enum WOstate
{
	STATE_PWR_ON = 0,
	STATE_SELECT_PRG = 1,
	STATE_EDIT_TURNS = 2,
	STATE_EDIT_SPEED = 3,
	STATE_READY = 4,
	STATE_PROCESS_RUN = 5,
	STATE_PAUSE = 6,
	STATE_FINISH = 7,
	STATE_MANUAL_MODE = 8
};

WOstate state = STATE_PWR_ON;

LCD1602_8bit lcd;
WindingMachine wmo;

bool holdbuttons = false;
bool cwccwbutton = false;
bool motreverse = false;

uint16_t percents = 0;

int turnsvalue = 10;
int speedvalue = 10;
int multiplicator = 1;

void lcdInitialization(void)
{
	lcd.SetDelayMultiplier(4);
	lcd.DataPortPtr = &MDR_PORTB->RXTX; //port for dara lines DB0...DB7 => PortB B0...B7
	lcd.CtrlPortPtr = &MDR_PORTB->RXTX; //port for ctrl lines E, RW,RS
	lcd.PinE = 0x100;
	lcd.PinRW = 0x200;
	lcd.PinRS = 0x400; // E pin -> PB8, RW pin -> PB9, RS pin -> PB10
	lcd.Init();		   // LCD initialization process start
}

void simpleDelay(unsigned int delayTime)
{
	for (unsigned int i = 0; i < delayTime; i++)
		;
}

void SetStateSelectProg(void)
{
	state = STATE_SELECT_PRG;
	lcd.ClearAll();
	MotorsDisable();
}

void SetStateEditTurns(void)
{
	state = STATE_EDIT_TURNS;
	multiplicator = defmulti;
	lcd.ClearAll();
	MotorsEnable();
}

void SetStateEditSpeed(void)
{
	state = STATE_EDIT_SPEED;
	multiplicator = defmulti;
	lcd.ClearAll();
}

void SetStateMachineReady(void)
{
	state = STATE_READY;
	lcd.ClearAll();
}

void SetStateMachineProcessRun(void)
{
	lcd.ClearAll();
	wmo.SetCurrentState(0, 0, cwccwbutton);
	wmo.SetTargetOrder(static_cast<unsigned int>(turnsvalue), static_cast<unsigned int>(speedvalue), cwccwbutton);
	motreverse = false;
	state = STATE_PROCESS_RUN;
	wmo.Start();
}

void SetStateMachineRunAfterPause(void)
{
	uint32_t tempvalue = (percents * speedvalue) / 100;
	wmo.SetTargetOrder(static_cast<unsigned int>(turnsvalue), static_cast<unsigned int>(tempvalue), cwccwbutton);
	motreverse = (wmo.GetCurrentState().ccw != wmo.GetTargetOrder().ccw) ? true : false;
	if (wmo.GetCurrentState().turns == 0 && motreverse)
	{
		return;
	}
	state = STATE_PROCESS_RUN;
	wmo.Start();
}

void SetStateMachineFinish(void)
{
	state = STATE_FINISH;
	wmo.StopMachine();
	lcd.ClearAll();
}

void SetStateWindingPause(void)
{
	state = STATE_PAUSE;
	wmo.StopMachine();
}

void SetManualMode(void)
{
	state = STATE_MANUAL_MODE;
	wmo.SetCurrentState(0, 0, cwccwbutton);
	lcd.ClearAll();
}

// Defenetion of IRQ_Handlers with extern "C" for working in C++ project ----------
extern "C"
{
	void Timer3_IRQHandler(void)
	{
		MDR_TIMER3->STATUS &= ~0x2; //Clear "CNT ARR EVENT" bit

		if (!motreverse)
		{
			wmo.AddOneTurnToCurState();
			if (wmo.GetCurrentState().turns == wmo.GetTargetOrder().turns)
			{
				SetStateMachineFinish();
			}
		}
		else
		{

			wmo.SubtracktTurnToCurState();
			if (wmo.GetCurrentState().turns < 1 || wmo.GetCurrentState().turns > maxturns)
			{
				SetStateWindingPause();
			}
		}
	}

	void SysTick_Handler(void)
	{
		holdbuttons = false;
		SysTick->CTRL &= ~((unsigned int)0x00000001);
	}
}
// ------------- End of extern "C" ------------------------------------------------

void updateMultiplicator(void)
{
	multiplicator *= addmulti;
	if (multiplicator > maxmulti)
		multiplicator = defmulti;
}

void ButtonLeftUpAction(void)
{

	switch (state)
	{

	case STATE_EDIT_TURNS:
		turnsvalue += multiplicator;
		if (turnsvalue > maxturns)
			turnsvalue = maxturns;
		break;

	case STATE_EDIT_SPEED:
		speedvalue += multiplicator;
		if (speedvalue > maxspeed)
			speedvalue = maxspeed;
		break;

	case STATE_READY:
		SetStateEditTurns();
		break;

	case STATE_FINISH:
		SetStateSelectProg();
		break;

	case STATE_PAUSE:
		SetStateMachineRunAfterPause();
		break;

	case STATE_MANUAL_MODE:
		MotorsEnable();
		wmo.SetTargetOrder(static_cast<unsigned int>(maxturns), static_cast<unsigned int>((percents * maxspeed) / 100), cwccwbutton);
		wmo.Start();
		break;

	default:
		return;
	}
}

void ButtonLeftDownAction(void)
{

	switch (state)
	{

	case STATE_EDIT_TURNS:
		turnsvalue -= multiplicator;
		if (turnsvalue < minturns)
			turnsvalue = minturns;
		break;

	case STATE_EDIT_SPEED:
		speedvalue -= multiplicator;
		if (speedvalue < minspeed)
			speedvalue = minspeed;
		break;

	case STATE_READY:
		SetStateEditTurns();
		break;

	case STATE_FINISH:
		SetStateSelectProg();
		break;

	case STATE_PROCESS_RUN:
		SetStateWindingPause();
		break;

	case STATE_MANUAL_MODE:
		
		wmo.StopMachine();
		MotorsDisable();
		break;

	default:
		return;
	}
}

void ButtonRightUpAction(void)
{

	switch (state)
	{

	case STATE_SELECT_PRG:
		SetManualMode();
		break;

	case STATE_EDIT_TURNS:
		updateMultiplicator();
		break;

	case STATE_EDIT_SPEED:
		if (BTNRTDN && BTNRTUP)
		{
			speedvalue = (percents * maxspeed) / 100;
		}
		else
		{
			updateMultiplicator();
		}

		break;

	case STATE_READY:;
		break;

	case STATE_FINISH:
		SetStateSelectProg();
		break;

	case STATE_PAUSE:
		if (BTNRTDN && BTNRTUP)
		{
			SetStateSelectProg();
		}
		break;

	case STATE_MANUAL_MODE:
		wmo.StopMachine();
		wmo.SetCurrentState(0, 0, cwccwbutton);
		if (BTNRTDN && BTNRTUP)
		{
			SetStateSelectProg();
		}
		break;

	default:
		return;
	}
}

void ButtonRightDownAction(void)
{

	switch (state)
	{

	case STATE_PWR_ON:
		SetStateSelectProg();
		break;

	case STATE_SELECT_PRG:
		SetStateEditTurns();
		break;

	case STATE_EDIT_TURNS:
		SetStateEditSpeed();
		break;

	case STATE_EDIT_SPEED:
		SetStateMachineReady();
		break;

	case STATE_READY:
		SetStateMachineProcessRun();
		break;

	case STATE_FINISH:
		SetStateSelectProg();
		break;

	default:
		return;
	}
}

void ButtonsHold(void)
{
	holdbuttons = true;
	SysTick->CTRL |= ((unsigned int)0x00000001);
}

void ButtonsPolling()
{
	if (holdbuttons)
		return;

	if (BTNLFUP)
	{
		ButtonLeftUpAction();
		ButtonsHold();
		return;
	}

	if (BTNLFDN)
	{
		ButtonLeftDownAction();
		ButtonsHold();
		return;
	}

	if (BTNRTUP)
	{
		ButtonRightUpAction();
		ButtonsHold();
		return;
	}

	if (BTNRTDN)
	{
		ButtonRightDownAction();
		ButtonsHold();
		return;
	}

	cwccwbutton = static_cast<bool>(BTNCNTR);
}

void HelloMsg(void)
{ // Print Starting Message
	lcd.PrintLn(1, 0, "Privet UserName!");
	lcd.PrintLn(2, 0, "WindingOne V0.93");
}

void PrintGlobalTarget(void)
{

	char t[5];
	char s[4];

	uintostr(size_t(turnsvalue), t, 4);
	uintostr(size_t(speedvalue), s, 3);
	lcd.PrintLn(1, 0, "N");
	lcd.PrintLn(1, 1, t);
	lcd.PrintLn(1, 5, " V");
	lcd.PrintLn(1, 7, s);
}

void PrintCurrentState(void)
{

	char tt[5];
	char ss[4];

	uintostr(size_t(wmo.GetCurrentState().turns), tt, 4);
	uintostr(size_t(wmo.GetCurrentState().speed), ss, 3);

	lcd.PrintLn(2, 0, "n");
	lcd.PrintLn(2, 1, tt);
	lcd.PrintLn(2, 5, " v");
	lcd.PrintLn(2, 7, ss);
}

int main(void)
{
	InitialConfigAll();
	lcdInitialization();
	wmo.UpdateSettings(25, 50, 400);

	while (1)
	{
		ButtonsPolling();

		simpleDelay(75000);
		simpleDelay(75000);
		simpleDelay(75000);
		simpleDelay(75000);
		simpleDelay(75000);
		simpleDelay(75000);

		percents = (GetADC5Result() * 100) / 4095;

		char m[5];
		uintostr(size_t(multiplicator), m, 4);

		char n[5];
		uintostr(size_t(turnsvalue), n, 4);

		char v[4];
		uintostr(size_t(speedvalue), v, 3);

		char r[4];
		uintostr(size_t(percents), r, 3);

		char c[4];
		booltoccw(c, cwccwbutton);

		switch (state)
		{

		case STATE_PWR_ON:
			HelloMsg();
			break;

		case STATE_SELECT_PRG:
			lcd.PrintLn(1, 0, "SET: Manual mode");
			lcd.PrintLn(2, 0, "ENTER: Automatic");

			break;

		case STATE_EDIT_TURNS:
			lcd.PrintLn(1, 0, "N Turns +/-");
			lcd.PrintLn(1, 12, m);
			lcd.PrintLn(2, 0, n);
			break;

		case STATE_EDIT_SPEED:

			lcd.PrintLn(1, 0, "V Speed +/-");
			lcd.PrintLn(1, 12, m);
			lcd.PrintLn(2, 0, v);
			lcd.PrintLn(2, 4, "RPM");
			lcd.PrintLn(2, 10, "VR");

			uintostr(size_t((percents * maxspeed) / 100), r, 3);
			lcd.PrintLn(2, 12, r);
			lcd.PrintLn(2, 15, " ");
			break;

		case STATE_READY:

			PrintGlobalTarget();

			lcd.PrintLn(2, 0, "Set CW/CCW:");
			lcd.PrintLn(1, 11, "READY");

			booltoccw(c, cwccwbutton);
			lcd.PrintLn(2, 13, c);

			break;

		case STATE_PROCESS_RUN:

			PrintGlobalTarget();

			if (motreverse)
			{
				lcd.PrintLn(1, 11, "<REV<");
			}
			else
			{
				lcd.PrintLn(1, 11, ">RUN>");
			}

			PrintCurrentState();

			booltoccw(c, wmo.GetTargetOrder().ccw);
			lcd.PrintLn(2, 13, c);

			break;

		case STATE_PAUSE:

			PrintGlobalTarget();

			lcd.PrintLn(1, 11, "PAUSE");
			uintostr(size_t(wmo.GetCurrentState().turns), n, 4);
			lcd.PrintLn(2, 0, "n");
			lcd.PrintLn(2, 1, n);
			lcd.PrintLn(2, 5, " r");
			uintostr(size_t((percents * speedvalue) / 100), v, 3);
			lcd.PrintLn(2, 7, v);

			booltoccw(c, cwccwbutton);
			lcd.PrintLn(2, 13, c);
			break;

		case STATE_FINISH:

			PrintGlobalTarget();
			booltoccw(c, wmo.GetTargetOrder().ccw);
			lcd.PrintLn(1, 13, c);

			lcd.PrintLn(2, 0, "I'm Finished!  ");

			break;

		case STATE_MANUAL_MODE:
			lcd.PrintLn(1, 0, "MAN  ");

			lcd.PrintLn(1, 5, "RV");
			uintostr(size_t((percents * maxspeed) / 100), v, 3);
			lcd.PrintLn(1, 7, v);

			booltoccw(c, cwccwbutton);
			lcd.PrintLn(1, 13, c);

			PrintCurrentState();

			booltoccw(c, wmo.GetTargetOrder().ccw);
			lcd.PrintLn(2, 13, c);

			break;
		}
	}
	//return 0;
}
