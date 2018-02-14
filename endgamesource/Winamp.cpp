#include "Houdini.h"

static int playerID = 0;
char currTitle[4096];

char* GetCurrentWinampTrack() 
{ 
   static char szTitle[2048], *rTitle; 
   HWND win = FindWindowA("Winamp v1.x",NULL); 
  if(!win) 
   return "Not active...";

   GetWindowTextA(win, szTitle, sizeof(szTitle)); 

   rTitle = szTitle+strlen(szTitle)-8;
   while (rTitle >= szTitle)
   {
	   if (!_strnicmp(rTitle,"- Winamp",8))
		   break;
	   rTitle--;
   }
   if (rTitle >= szTitle)
	   rTitle--;
   while (rTitle >= szTitle && *rTitle == ' ')
	   rTitle--;
	   *++rTitle = 0;

   if(strlen(szTitle) <= 0) 
   return ""; 

   return szTitle; 
} 

void StopSong()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, W2_STOP, 0);
}
void NextSong()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40048, 0);
}
void PrevSong()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40044, 0);
}
void PlayPause()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40046, 0);
}
void FastForward()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40148, 0);
}
void Rewind()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40144, 0);
}
void RaiseVolume()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40058, 0);
	SendMessage(hwndWinamp, WM_COMMAND, 40058, 0);
	SendMessage(hwndWinamp, WM_COMMAND, 40058, 0);
	SendMessage(hwndWinamp, WM_COMMAND, 40058, 0);
	SendMessage(hwndWinamp, WM_COMMAND, 40058, 0);
}
void LowerVolume()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40059, 0);
	SendMessage(hwndWinamp, WM_COMMAND, 40059, 0);
	SendMessage(hwndWinamp, WM_COMMAND, 40059, 0);
	SendMessage(hwndWinamp, WM_COMMAND, 40059, 0);
	SendMessage(hwndWinamp, WM_COMMAND, 40059, 0);
}
void Shuffle()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40023, 0);
}
void Repeat()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40022, 0);
}

void Close()
{
	HWND hwndWinamp = FindWindow("Winamp v1.x",NULL);
	SendMessage(hwndWinamp, WM_COMMAND, 40001, 0);
}

