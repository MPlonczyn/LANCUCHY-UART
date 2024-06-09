#include <LPC21xx.H>
#include "led.h"
#include "servo.h"
#include "timer_interrupts.h"
#include "uart.h"

#define DETECOR_bm	(1<<10)

void DetectorInit(){
	IO0DIR &= ~DETECOR_bm;
}

enum DetectorState{
	ACTIVE,
	INACTIVE
};

enum DetectorState eReadDetector(){
	
	int iDetecorCheck;
	iDetecorCheck = IO0PIN;
	
	if((iDetecorCheck & DETECOR_bm) != 0){
		return INACTIVE;
	}
	return ACTIVE;
}

struct Servo sServo;

void Automat(){
			
	switch(sServo.eState){
		
		case CALLIB:
		if(eReadDetector() == INACTIVE){
			sServo.eState = CALLIB;
			LedStepRight();
		}
		else{
			sServo.eState = IDLE;
			sServo.uiCurrentPosition = 0;
			sServo.uiDesiredPosition = 0;
		}
		break;
		
		case IDLE:
		if(sServo.uiCurrentPosition != sServo.uiDesiredPosition){
			sServo.eState = IN_PROGRESS;
		}
		else{
			sServo.eState = IDLE;
		}
		break;
		
		case IN_PROGRESS:
		if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){
			sServo.eState = IN_PROGRESS;
			LedStepRight();
			sServo.uiCurrentPosition--;
		}
		else if(sServo.uiCurrentPosition < sServo.uiDesiredPosition){
			sServo.eState = IN_PROGRESS;
			LedStepLeft();
			sServo.uiCurrentPosition++;
		}
		else{
			sServo.eState = IDLE;
		}
		break;
	}
}

void ServoInit(unsigned int uiServoFrequency){
	sServo.eState = CALLIB;
	DetectorInit();
	LedInit();
	Timer0Interrupts_Init((1000000/uiServoFrequency),&Automat);
}

void ServoCallib(){
	sServo.eState = CALLIB;
	cOdebranyZnak = 0;
}

void ServoGoTo(unsigned int uiPosition){
	sServo.uiDesiredPosition = uiPosition;
	cOdebranyZnak = 0;
}
