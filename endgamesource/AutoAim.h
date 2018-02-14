extern class AutoAim {

private:

	int Current;
	RosterUnit *pUnit;
	UnitAny *pMonster;
	char Name[29];

public:

	BOOL FindNearestUnit(BOOL bMonster);
	BOOL AttackPlayer(DWORD dwUnitId, INT UnitType);
	VOID Init();
	VOID ChangeTarget(INT nPlayer);
	VOID GetNextPlayer();
	VOID GetCurrent(DWORD AimID);
	VOID GetLastPlayer();
	BOOL SelectPlayer(DWORD PID);
	BOOL IsPlayerFriend();
	INT GetPlayerLevel();
	DWORD GetPlayerID();
	DWORD GetPlayerCLASS();
	char* GetPlayerNAME();
	long GetPlayerX();
	long GetPlayerY();
};
