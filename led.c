#include <LPC21xx.H>
#include "led.h"

#define LED0_bm       (1<<16)  //0x10000
#define LED1_bm				(1<<17)  //0x20000
#define LED2_bm				(1<<18)  //0x40000
#define LED3_bm       (1<<19)  //0x80000

enum StepChoice{
	LEFT,
	RIGHT
};

void LedInit(){
	IO1DIR |= (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	IO1SET = LED0_bm;
}

void LedOn (unsigned char ucLedIndeks){
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	
	switch(ucLedIndeks){
		case 0:
			IO1SET = LED0_bm;
			break;
		case 1:
			IO1SET = LED1_bm;
			break;
		case 2:
			IO1SET = LED2_bm;
			break;
		case 3:
			IO1SET = LED3_bm;
			break;
	}
}

void LedStep(enum StepChoice DesiredStep){
	
	static unsigned int iCurrentLed = 0;
	
	switch(DesiredStep){
		
		case(LEFT):
			iCurrentLed++;
			break;
		case(RIGHT):
			iCurrentLed--;
			break;
	}
	LedOn(iCurrentLed%4);
}

void LedStepLeft(){
	LedStep(LEFT);
}

void LedStepRight(){
	LedStep(RIGHT);
}

char LedRead(){
	
	int iLedCheck;
	iLedCheck = IO1PIN;
	
	if((iLedCheck & LED0_bm) != 0){
		return 0;
	}
	else if ((iLedCheck & LED1_bm) != 0){
		return 1;
	}
	else if ((iLedCheck & LED2_bm) != 0){
		return 2;
	}
	else if ((iLedCheck & LED3_bm) != 0){
		return 3;
	}
	return 4;
}
