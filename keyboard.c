#include <LPC21xx.H>
#include "keyboard.h"

#define SWT0_bm				(1<<4)  //0x10
#define SWT1_bm				(1<<6)  //0x40
#define SWT2_bm				(1<<5)  //0x20
#define SWT3_bm				(1<<7)  //0x80

void KeyboardInit(){
	IO0DIR &= ~(SWT0_bm | SWT1_bm | SWT2_bm | SWT3_bm);
}	

enum KeyboardState eKeyboardRead(){
	int iButtonCheck;
	iButtonCheck = IO0PIN;
	
	if((iButtonCheck & SWT0_bm) == 0){
		return BUTTON_0;
	}
	else if ((iButtonCheck & SWT1_bm) == 0){
		return BUTTON_1;
	}
	else if ((iButtonCheck & SWT2_bm) == 0){
		return BUTTON_2;
	}
	else if ((iButtonCheck & SWT3_bm) == 0){
		return BUTTON_3;
	}
	return RELEASED;
}
