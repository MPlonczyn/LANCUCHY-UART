#include <LPC210X.H>
#include "uart.h"
#include "string.h"

#define NULL 																			 '\0'

/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

// Setting RxD and TxD on UART0
#define mRXD_ON_P0_1															 0x00000004
#define mTXD_ON_P0_0															 0x00000001

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

////////////// Zmienne globalne ////////////
char cOdebranyZnak = '1';

struct RecieverBuffer sRxBuffer = {"", 0, EMPTY};

void Reciever_PutCharacterToBuffer(char cCharacter){
	
	if(sRxBuffer.eStatus == EMPTY){
			if(cCharacter == TERMINATOR){
				sRxBuffer.eStatus = READY;
				sRxBuffer.cData[sRxBuffer.ucCharCtr] = NULL;
				sRxBuffer.ucCharCtr = 0;
			}
			else if (sRxBuffer.ucCharCtr == (RECIEVER_SIZE - 1)){
				sRxBuffer.eStatus = OVERFLOW;
				sRxBuffer.ucCharCtr = 0;
			}
			else{
				sRxBuffer.eStatus = EMPTY;
				sRxBuffer.cData[sRxBuffer.ucCharCtr] = cCharacter;
				sRxBuffer.ucCharCtr++;
			}
	}
}

enum eRecieverStatus eReciever_GetStatus(){
	return sRxBuffer.eStatus;
}

void Reciever_GetStringCopy(char * ucDestination){
	CopyString(sRxBuffer.cData,ucDestination);
	sRxBuffer.eStatus = EMPTY;
}

struct TransmiterBuffer sTransmiterBuffer = {"",FREE,0,0};

char Transmiter_GetCharacterFromBuffer(){
	if(sTransmiterBuffer.fLastCharacter == 1){
		sTransmiterBuffer.eStatus = FREE;
		sTransmiterBuffer.fLastCharacter = 0;
		return NULL;
	}
	else if(sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr] == NULL){
		sTransmiterBuffer.fLastCharacter = 1;
		return TERMINATOR;
	}
	else {
		char cDataBufor = sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr];
		sTransmiterBuffer.cCharCtr++;
		return cDataBufor;
		
	}
	
}

void Transmiter_SendString(char cString[]){
	sTransmiterBuffer.eStatus = BUSY;
	CopyString(cString,sTransmiterBuffer.cData);
	U0THR = sTransmiterBuffer.cData[0];
	sTransmiterBuffer.cCharCtr = 1;
}

enum eTransmiterStatus Transmiter_GetStatus(void){
	return sTransmiterBuffer.eStatus;
}

///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
      Reciever_PutCharacterToBuffer(U0RBR);
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
		 unsigned char ucTransmiterBufor = Transmiter_GetCharacterFromBuffer();
		 
     if(ucTransmiterBufor != NULL){
				U0THR=ucTransmiterBufor;
		 }
   }
	
   VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){

   // UART0
   PINSEL0 = PINSEL0 | mRXD_ON_P0_1 | mTXD_ON_P0_0;                                     // ustawic pina na odbiornik i nadajnik uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = ((15000000/16)/uiBaudRate);										// predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
   U0IER  |= (mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE);  // aktywacja zrodla przerwania dla UART0, przez linie Rx, oraz nadawania

   // INT
   VICVectAddr0  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
   VICVectCntl0  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
}
