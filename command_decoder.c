#include "command_decoder.h"
#include "string.h"

#define NULL											'\0'
#define TOKEN_DELIMITER						' '

struct Keyword asKeywordList[MAX_KEYWORD_NR]={
{CC, "calc"},
{CL, "callib"},
{GT, "goto"},
};

struct Token asToken[MAX_TOKEN_NR];
unsigned char ucTokenNr = 0;


enum FindingState{
	TOKEN,
	DELIMITER
};

unsigned char ucFindTokensInString (char *pcString){
	
	unsigned char ucCharacterCounter;
	char cCharacterBufor;
	enum FindingState eCurrentState = DELIMITER;
	unsigned char ucCurrentToken = 0;
	unsigned char ucTokenNumber = 0;

	for(ucCharacterCounter = 0; ;ucCharacterCounter++){
		
		cCharacterBufor = pcString[ucCharacterCounter];
		
		if(cCharacterBufor == NULL){
			return ucTokenNumber;
		}
		else if((ucCurrentToken + 1) > MAX_TOKEN_NR){
			return ucCurrentToken;
		}
		
		switch(eCurrentState){
			
			case TOKEN:
				if(cCharacterBufor == TOKEN_DELIMITER){
					eCurrentState = DELIMITER;
					ucCurrentToken++;
				}
				else{
					eCurrentState = TOKEN;
				}
				break;
			
			case DELIMITER:
				if(cCharacterBufor != TOKEN_DELIMITER){
					asToken[ucCurrentToken].uValue.pcString = pcString + ucCharacterCounter;
					eCurrentState = TOKEN;
					ucTokenNumber++;
				}
				else{
					eCurrentState = DELIMITER;
				}
				break;
			}
	}
}

enum Result eStringToKeyword (char pcStr[],enum KeywordCode *peKeywordCode){
	
	unsigned char ucKeywordCounter;
	
	for(ucKeywordCounter = 0; ucKeywordCounter < MAX_KEYWORD_NR; ucKeywordCounter++){
		
		if(eCompareString(pcStr,asKeywordList[ucKeywordCounter].cString) == EQUAL){
			*peKeywordCode = asKeywordList[ucKeywordCounter].eCode;
			return OK;
		}
	}
	return ERROR;
}


void DecodeTokens(){
	
	unsigned char ucTokenCounter;
	union TokenValue uValue;
	enum TokenType eType;
	char * pcPointerBufor;
	
	for(ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++){
		
		pcPointerBufor = asToken[ucTokenCounter].uValue.pcString;
		
		if(eStringToKeyword(pcPointerBufor,&uValue.eKeywordCode) == OK){
			eType = KEYWORD;
		}
		else if(eHexStringToUInt(pcPointerBufor,&uValue.uiValue) == OK){
			eType = NUMBER;
		}
		else{
			eType = STRING;
			uValue.pcString = pcPointerBufor;
		}
		asToken[ucTokenCounter].eType = eType;
		asToken[ucTokenCounter].uValue = uValue;
	}
	
}

void DecodeMsg(char *pcString){
	ucTokenNr = ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString,TOKEN_DELIMITER,NULL);
	DecodeTokens();
}
