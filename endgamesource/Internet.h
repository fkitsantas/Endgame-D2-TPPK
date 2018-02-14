char* GetSiteText(char* Addr ,char* Para);
bool CheckIfAllowed();

extern int PlayerRosterN;

DWORD WINAPI PrintPlayerStats(void* p);
DWORD WINAPI PrintPkStats(void* p);
DWORD WINAPI UpdatedStats(void* p);


struct PKInfoStruct {
	char ForumName[50];
	DWORD TotalKills;
	DWORD TotalScore;
	DWORD Rank;
	DWORD Ready;
};
extern PKInfoStruct PKInfo;

struct AUTHNUMBER {
	char PlayerRoster[50];
	int LifeAddr1;
    DWORD LifeAddr2;
	int totalVars;
};
extern AUTHNUMBER Authn;
//void UpdatePKInfo();


void GetPlayers();
void CheckAuth(void);
void GetNumber(void);
void GetPlayerFriends();
void GetPlayerEnemys();
void GetHCPKEnemys();
void GetHCPKEnemys();
void PrintLastPK();
bool CheckPlayerFriend(char* Name) ;
bool CheckPlayerEnemy(char* Name) ;
bool CheckHCPKEnemy(char* Name) ;
bool CheckHCPKEnemy(char* Name) ;
void GA(void);
BOOL AddURL(char* PKname, int Level, char* Class);
