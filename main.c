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

struct Calc {
	char cCalcBufor[RECIEVER_SIZE];
	unsigned char fCalcDone;
	unsigned int uiCalcValue;
};

int main(){
	unsigned char fUnknownMessage = 0;
	unsigned char fIdRequest = 0;
	struct Calc sCalc = {"",0,0};
	char cStringToSend[16];
	Timer0Interrupts_Init(1000000,WatchUpdate);
	UART_InitWithInt(9600);
	
	while(1){
		if(eReciever_GetStatus() == READY){
			Reciever_GetStringCopy(sCalc.cCalcBufor);
			DecodeMsg(sCalc.cCalcBufor);
			if((ucTokenNr > 0) && (asToken[0].eType == KEYWORD)){
				
				switch(asToken[0].uValue.eKeywordCode){
					
					case(ID):
						fIdRequest = 1;
						break;
					
					case(CC):
						sCalc.uiCalcValue = 2 * asToken[1].uValue.uiValue;
						sCalc.fCalcDone = 1;
						break;
					
					default:
						break;
				}
			}
			else{
				fUnknownMessage = 1;
			}
		}
		
		if(Transmiter_GetStatus() == FREE){
			if(fUnknownMessage == 1){
				CopyString("unknownmessage\n",cStringToSend);
				Transmiter_SendString(cStringToSend);
				fUnknownMessage = 0;
			}
			else if(fIdRequest == 1){
				CopyString("ID Servo\n",cStringToSend);
				Transmiter_SendString(cStringToSend);
				fIdRequest = 0;
			}
			if(sCalc.fCalcDone == 1){
				CopyString("calc ",cStringToSend);
				AppendUIntToString(sCalc.uiCalcValue,cStringToSend);
				AppendString("\n", cStringToSend);
				Transmiter_SendString(cStringToSend);
				sCalc.fCalcDone = 0;
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
