#ifndef INFOBOX_H
#define INFOBOX_H

struct InfoBox {
	char* Message;
	char* Data;
};
struct AutomapInfo
{
	char* Message;
	char* Data;
};
extern void	DrawOriginal(void);
extern DWORD nGameTimer;
extern bool PrintExp;
#endif

char* FormatOutput(char * String);