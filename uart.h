// Odbior sygnalow
#define TERMINATOR																 '\r'
#define RECIEVER_SIZE 														 13
#define TRANSMITER_SIZE														 13
enum eRecieverStatus {EMPTY, READY, OVERFLOW};

struct RecieverBuffer{ 
	char cData[RECIEVER_SIZE];
	unsigned char ucCharCtr;
	enum eRecieverStatus eStatus;
};

extern struct RecieverBuffer sRxBuffer;

enum eTransmiterStatus {FREE, BUSY};

struct TransmiterBuffer{
	char cData[TRANSMITER_SIZE];
	enum eTransmiterStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char cCharCtr;
};

extern char cOdebranyZnak;

void UART_InitWithInt(unsigned int uiBaudRate);
void Reciever_PutCharacterToBuffer(char cCharacter);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
void Transmiter_SendString(char cString[]);
enum eTransmiterStatus Transmiter_GetStatus(void);
char Transmiter_GetCharacterFromBuffer(void);

