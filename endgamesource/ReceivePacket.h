BOOL __fastcall OnGamePacketReceived(LPBYTE aPacket,DWORD aLen);
extern DWORD WINAPI AutoPartyMemberThread(LPVOID P);
extern DWORD InGame;
VOID OnGameEvent(BOOL ExitGame);
bool SpamAcc(char *Name);
extern	bool AutoParty1;
void SquelchLoop();
BOOL MaxBPS();
BOOL LoadBreakpoints(int nChar);
BOOL Load();
int GetIDByName(char* Name);
BOOL Interact(DWORD dwUnitId, DWORD dwType, BOOL bCheckOverride);
BOOL DeathMessage(char* opfer);
void InviteAndLoot2(RosterUnit* pUnit, DWORD dwFlags);
void InviteAndLoot();
char* GetClassByName(char* Name);
char* GetClassByName2(char* Name);
int GetLevelByName(char* Name);
DWORD WINAPI DeathMessageThread(LPVOID P);
RosterUnit* FindPartyByNameA(LPSTR szName);
RosterUnit* FindPartyByIdA(DWORD dwUnitId);