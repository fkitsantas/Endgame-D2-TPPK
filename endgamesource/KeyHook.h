void __fastcall OnGameMouseEvent(BYTE iType, WORD x, WORD y);
void __fastcall KeyHook(BYTE keycode, BYTE repeat);
int GetCurrentTrackTime();
int GetMaxTrackTime();
extern POINT Camp;
extern DWORD QuestID; 
DWORD WINAPI TeleportThread(LPVOID P);
