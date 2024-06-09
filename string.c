#include "string.h"

#define NULL				'\0'
#define HEX_mask 		0xF

void CopyString(char pcSource[], char pcDestination[]){
	
	unsigned char iCharacterCounter;
	
	for (iCharacterCounter = 0; pcSource[iCharacterCounter] != NULL; iCharacterCounter++){
		pcDestination[iCharacterCounter] = pcSource[iCharacterCounter];
	}
	pcDestination[iCharacterCounter] = NULL;
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[]){	
	unsigned char iCharacterCounter;
	
	for (iCharacterCounter = 0; pcStr1[iCharacterCounter] != NULL; iCharacterCounter++){
		if(pcStr1[iCharacterCounter] != pcStr2[iCharacterCounter]){
			return DIFFERENT;
		}
	}
	if(pcStr2[iCharacterCounter] == NULL){
		return EQUAL;
	} 
	return DIFFERENT;
}

void AppendString (char pcSourceStr[], char pcDestinationStr[]){
	
	unsigned char iNullPosition;
	
	for (iNullPosition = 0; pcDestinationStr[iNullPosition] != NULL; iNullPosition++){
	}
	CopyString(pcSourceStr, pcDestinationStr + iNullPosition);
}

void ReplaceCharactersInString (char pcString[], char cOldChar, char cNewChar){
	unsigned char iCharacterCounter;
	
	for(iCharacterCounter = 0; pcString[iCharacterCounter] != NULL; iCharacterCounter++){
		if(pcString[iCharacterCounter] == cOldChar){
			pcString[iCharacterCounter] = cNewChar;
		}
	}
}

void UIntToHexStr (unsigned int uiValue, char pcStr[]){
	
	unsigned char ucNibbleCounter;
	unsigned char ucNibbleBufor;
	
	pcStr[0] = '0';
	pcStr[1] = 'x';
	pcStr[6] = NULL;

	for(ucNibbleCounter = 0; ucNibbleCounter < 4 ; ucNibbleCounter++){
		
		ucNibbleBufor = ((uiValue >> (ucNibbleCounter*4)) & HEX_mask);
		
		if(ucNibbleBufor > 9){
			pcStr[5-ucNibbleCounter] = ucNibbleBufor + 'A' - 10;
		}
		else{
			pcStr[5-ucNibbleCounter] = ucNibbleBufor + '0';
		}
	}
}

enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue){
	
	unsigned char ucCharacterCounter;
	char cCharacterBufor;
	
	*puiValue = 0;
	
	if((pcStr[0] != '0') || (pcStr[1] != 'x') || (pcStr[2] == NULL)){
		return ERROR;
	}
	
	for(ucCharacterCounter = 2; pcStr[ucCharacterCounter] != NULL; ucCharacterCounter++){
		
		cCharacterBufor = pcStr[ucCharacterCounter];
		
		if(ucCharacterCounter == 6){
			return ERROR;
		}
		
		*puiValue = *puiValue << 4;
		
		if(cCharacterBufor >= 'A' && cCharacterBufor <= 'F'){
			*puiValue = *puiValue + (cCharacterBufor + 10 - 'A');
		}
		else if (cCharacterBufor >= '0' && cCharacterBufor <= '9'){
			*puiValue = *puiValue + (cCharacterBufor - '0');
		}
		else{
			return ERROR;
		}
	}
	return OK;
}

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]){
	
	unsigned char iNullPosition;
	
	for (iNullPosition = 0; pcDestinationStr[iNullPosition] != NULL; iNullPosition++){
	}
	
	UIntToHexStr(uiValue, pcDestinationStr + iNullPosition);
}
