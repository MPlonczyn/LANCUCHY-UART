enum CompResult{
	DIFFERENT,
	EQUAL
};

enum Result{
	OK,
	ERROR 
};

void CopyString(char pcSource[], char pcDestination[]);
enum CompResult eCompareString(char pcStr1[], char pcStr2[]);
void ReplaceCharactersInString (char pcString[], char cOldChar, char cNewChar);
void UIntToHexStr (unsigned int uiValue, char pcStr[]);
enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue);
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]);
void AppendString(char pcSourceStr[], char pcDestinationStr[]);
