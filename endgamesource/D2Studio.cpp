#include "Houdini.h"
VOID PrintConsoleString(LPSTR lpMessage, ...)
{
	CHAR szOutput[0x400] = "";
	PCONSOLEMESSAGE Msg = new CONSOLEMESSAGE;
	va_list Args;

	if(!Myself)
		return;

	va_start(Args, lpMessage);
	vsprintf(szOutput, lpMessage, Args);
	va_end(Args);

	Msg->Message = new WCHAR[strlen(szOutput) + 1];
	memset(Msg->Message, 0, strlen(szOutput) * sizeof(WCHAR));

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szOutput, (INT)strlen(szOutput), Msg->Message, (INT)strlen(szOutput));

	Msg->TimeStamp = GetTickCount();

	Msg->Message[strlen(szOutput)] = L'\0';

	if(v_ConsoleList.GetSize() == 26)
	{
		delete v_ConsoleList[0]->Message;
		delete v_ConsoleList[0];

		v_ConsoleList.RemoveAt(0);
		v_ConsoleList.FreeExtra();
	}

	v_ConsoleList.Add(Msg);

	return;
}

INT GetConsoleSize()
{
	if(v_ConsoleList.IsEmpty())
		return 0;

	INT nSize = 0;

	for(INT i = ((v_ConsoleList.GetSize() > 3) ? v_ConsoleList.GetSize() - 3 : 0); i < v_ConsoleList.GetSize(); i++)
	{
		if((GetTickCount() - v_ConsoleList[i]->TimeStamp) < v_ConsoleDelay)
			nSize++;
	}

	return nSize;
}