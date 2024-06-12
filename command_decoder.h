#define MAX_KEYWORD_STRING_LTH 		11
#define MAX_TOKEN_NR   						3
#define MAX_KEYWORD_NR 						4

enum KeywordCode{
	ID,
	CC,
	CL,
	GT
};

enum TokenType{
	KEYWORD,
	NUMBER,
	STRING
};

union TokenValue{
	enum KeywordCode eKeywordCode;
	unsigned int uiValue;
	char* pcString;
};

struct Keyword{
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
};

struct Token{
	enum TokenType eType;
	union TokenValue uValue;
};



extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;
extern struct Keyword asKeywordList[MAX_KEYWORD_NR];

void DecodeMsg(char *pcString);
