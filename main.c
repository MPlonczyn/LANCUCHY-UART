#include <LPC210X.H>

#include "uart.h"
#include "servo.h"
#include "command_decoder.h"
#include "string.h"
#include "timer_interrupts.h"

#define NULL			'\0'

struct Watch {
	unsigned char ucMinutes;
	unsigned char ucSecconds;
	unsigned char fSeccondsValueChanged; 
	unsigned char fMinutesValueChanged;
};

struct Watch sWatch = {0,0,0,0};

void WatchUpdate(){
	sWatch.ucSecconds++;
	sWatch.fSeccondsValueChanged = 1;
	if(sWatch.ucSecconds == 5){
		sWatch.ucSecconds = 0;
		sWatch.ucMinutes++;
		sWatch.fMinutesValueChanged = 1;
	}
}

int main(){
	char cCalcBufor[RECIEVER_SIZE];
	unsigned int uiValueBufor;
	char cStringToSend[13];
	Timer0Interrupts_Init(1000000,WatchUpdate);
	UART_InitWithInt(9600);
	
	while(1){
		if(Transmiter_GetStatus() == FREE){
			if(eReciever_GetStatus() == READY){
				Reciever_GetStringCopy(cCalcBufor);
				DecodeMsg(cCalcBufor);
				uiValueBufor = 2 * asToken[1].uValue.uiValue;
				CopyString("calc ",cStringToSend);
				AppendUIntToString(uiValueBufor,cStringToSend);
				AppendString("\n", cStringToSend);
				Transmiter_SendString(cStringToSend);
			}
			else if(sWatch.fMinutesValueChanged == 1){
				CopyString("min ",cStringToSend);
				AppendUIntToString(sWatch.ucMinutes,cStringToSend);
				AppendString("\n", cStringToSend);
				Transmiter_SendString(cStringToSend);
				sWatch.fMinutesValueChanged = 0;
			}
			else if(sWatch.fSeccondsValueChanged == 1){
				CopyString("sec ",cStringToSend);
				AppendUIntToString(sWatch.ucSecconds,cStringToSend);
				AppendString("\n", cStringToSend);
				Transmiter_SendString(cStringToSend);
				sWatch.fSeccondsValueChanged = 0;
			}
		}
	}
}
