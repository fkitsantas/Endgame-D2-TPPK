#ifndef _STDIO_H
#define _STDIO_H

typedef struct ConsoleMessage_t
{
	LPWSTR Message;
	DWORD TimeStamp;
} CONSOLEMESSAGE, *PCONSOLEMESSAGE;

VOID PrintConsoleString(LPSTR lpMessage, ...);
INT GetConsoleSize();

#endif