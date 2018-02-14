#include "Houdini.h"
#include "Vars.h"
#define AFFECT_JUST_PORTALED 102



BOOL LoadBreakpoints(int nChar)
{
	switch(nChar)
	{
	case 0:
		{
		MaxAmazon();
		break;
		}
	case 1:
		{
		MaxSorceress();
		break;
		}
	case 2:
		{
		MaxNecromancer();
		break;
		}
	case 3:
		{
		MaxPaladin();
		break;
		}
	case 4:
		{
		MaxBarbarian();
		break;
		}
	case 5:
		{
		MaxDruid();
		break;
		}
	case 6:
		{
		MaxAssassin();
		break;
		}
	}

	return TRUE;
}

BOOL Load()
{
	SetAllFalse();
	Auto.Init();
	RevealAct();
	LevelNames.RemoveAll();
	SaveGameName();
	SquelchLoop();
	LoadBreakpoints(Myself->dwTxtFileNo);
	//CheckAuth();
	if (AutoParty1)
	{
		PrintString("ÿc2Auto Party Is Active!");
	}
	
	PrintConsoleString("Endgame By BfGGM(Cody) Seduced",VERSION_NO);
	//PrintLastPK();
	//CreateThread(0,0,PrintPkStats,0,0,0);
	//CreateThread(0,0,AutoPartyMemberThread,0,0,0);


	v_CurrentExp=D2COMMON_GetUnitStat(Myself,13,0);

	return TRUE;
}
VOID OnGameEvent(BOOL ExitGame)
{
	if(ExitGame)
	{
		delete v_GlobalMap;
		v_ConsoleList.RemoveAll();

		SetAllFalse();
		TerminateThread(AutoPartyMemberThread,0);
		DriverDestroyThread(v_ChickenThread);
		DriverDestroyThread(v_AntiThread);
		DriverDestroyThread(v_AttackThread);
		DriverDestroyThread(v_ChantThread);
		
		if (v_AutoTele)
			DriverDestroyThread(v_TeleportThread);

		v_GameInit = FALSE;
	}
	else
	{
		v_GlobalMap = new CMap;

		ResetValues();

		v_ChickenThread=DriverCreateThread((LPVOID)TickThread, (LPVOID)THREAD_CHICKEN);
		v_AntiThread=DriverCreateThread((LPVOID)TickThread, (LPVOID)THREAD_ANTIPK);
		v_AttackThread=DriverCreateThread((LPVOID)TickThread, (LPVOID)THREAD_ATTACK);
		v_ChantThread=DriverCreateThread((LPVOID)TickThread, (LPVOID)THREAD_ENCHANT);

		if (cfg.IncreaseCastSpeed)
		{
			Spoof();
		}

		v_GameInit = TRUE;
	}
}
char* GetClassByName(char* Name)
{
	RosterUnit* pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080;
	while(pUnit!=NULL)
	{
		if(!_strcmpi(pUnit->szName,Name)) return GetD2ClassName(pUnit->dwClassId);

		pUnit=pUnit->pNext;
	}
	return NULL;
}
char* GetClassByName2(char* Name)
{
	RosterUnit* pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080;
	while(pUnit!=NULL)
	{
		if(!_strcmpi(pUnit->szName,Name)) return GetD2ClassName2(pUnit->dwClassId);

		pUnit=pUnit->pNext;
	}
	return NULL;
}

char* GetNameByID(DWORD PID)
{
	RosterUnit *pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080;
	while(pUnit)
	{
		if(pUnit->dwUnitId==PID) return pUnit->szName;

		pUnit=pUnit->pNext;
	}
	return "???";
}
int GetLevelByName(char* Name)
{
	RosterUnit* pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080;
	while(pUnit!=NULL)
	{
		if(!_strcmpi(pUnit->szName,Name))
		return pUnit->wLevel;
		pUnit=pUnit->pNext;
	}
	return -1;
}

int GetIDByName(char* Name)
{
	RosterUnit* pUnit=(RosterUnit*)*(DWORD*)0x6FBCC080;
	UnitAny* Me=(UnitAny*)D2CLIENT_GetPlayerUnit();
	while(pUnit!=NULL)
	{
		if(!_strcmpi(pUnit->szName,Name))
		return pUnit->dwUnitId;
		pUnit=pUnit->pNext;
	}
	return -1;
}

void SquelchLoop()
{
	for(RosterUnit *pUnit = (RosterUnit*)*(DWORD*)PlayerRoster;pUnit;pUnit=pUnit->pNext)
	{
		if(SpamAcc(pUnit->szName)) SQ(GetIDByName(pUnit->szName));
	}
}
void InviteAndLoot()
{
	RosterUnit* pMe = FindPartyById(D2CLIENT_GetPlayerUnit()->dwUnitId);
	
	for(RosterUnit *pUnit = (RosterUnit*)*(DWORD*)PlayerRoster;pUnit;pUnit=pUnit->pNext)
	{
		if(!pUnit)
			break;
		if(!pMe)
			break;
		if(!D2CLIENT_GetPlayerUnit())
			break;
		if(pUnit->dwUnitId == pMe->dwUnitId)
			continue;

		DWORD dwFlags = GetPvpFlags(pUnit->dwUnitId);

		if(!(dwFlags & PVP_ALLIED && dwFlags & PVP_ALLIED_WITH_YOU && dwFlags & PVP_INVITED_YOU && dwFlags & PVP_INVITED_BY_YOU))
		{
			BYTE data[6];
			*(LPWORD)&data[0] = 0x065E;
			*(LPDWORD)&data[2] = pUnit->dwUnitId;
			SendGAMEPacket(data,6);
		}
	}
}
void InviteAndLoot2(RosterUnit* pUnit, DWORD dwFlags)
{
	if(!(dwFlags & PVP_ALLIED && dwFlags & PVP_ALLIED_WITH_YOU && dwFlags & PVP_INVITED_YOU && dwFlags & PVP_INVITED_BY_YOU))
	{
		BYTE aPacket[6];
		*(LPWORD)&aPacket[0] = 0x065E;
		*(LPDWORD)&aPacket[2] = pUnit->dwUnitId;
		SendGAMEPacket(aPacket,6);
	}
}
bool SpamAcc(char *Name) 
{
	char mypath[266]={0};
	char line[512];
	char NameBuf[100];

	sprintf_s(mypath,"%s\\%s",szModulePath,"Spammers.txt");
	FILE *fp = fopen(mypath, "r");

	if (fp) 
	{
		while (fgets(line, sizeof(line), fp)) 
		{
			if(strncmp(Name,line,strlen(line)-1) == 0)
			{
				return true;
			}
		}
		fclose(fp);
	}
	return false;
}

bool InviteLootAcc(char* Name)
{
	char mypath[266]={0};
	char line[512];
	char NameBuf[100];

	sprintf_s(mypath,"%s\\%s",szModulePath,"Friends.txt");
    FILE *fp = fopen(mypath, "r");

    if (fp) 
	{
        while (fgets(line, sizeof(line), fp)) 
		{
			if(strncmp(Name,line,strlen(line)-1) == 0)
			{
				return true;
			}
        }
        fclose(fp);
    }

	return false;
}

DWORD WINAPI AutoPartyMemberThread(LPVOID P)
{
	InviteAndLoot();
	return 0;
}

BOOL Interact(DWORD dwUnitId, DWORD dwType, BOOL bCheckOverride)
{
	if(!bCheckOverride)
		if(!GetUnit(dwUnitId, dwType))
			return FALSE;

	BYTE aPacket[9];
	aPacket[0] = 0x13;
	*(LPDWORD)&aPacket[1] = dwType;
	*(LPDWORD)&aPacket[5] = dwUnitId;
	D2NET_SendPacket(9, 1, aPacket);

	return TRUE;
}

BOOL __fastcall OnGamePacketReceived(LPBYTE aPacket,DWORD aLen)
{

	////OverHeadTrace
	if (cfg.OverHeadTrace)
	{
	if (aPacket [0] == 0x26 && aPacket[1] == 0x05)
	{
			DWORD UnitId = *(DWORD*)&aPacket[0x04];
			LPSTR szMessage = (LPSTR)&aPacket[0x0B];
			char Buffer[300];
			sprintf(Buffer,"ÿc5[ÿc0-ÿc8OverHeadTrace (%s): ÿc0%sÿc0-ÿc5]",GetNameByID(UnitId),szMessage);
			PrintConsoleString(Buffer,0);
	}
}
	/////OverHeadTrace

if(aPacket[0] == 0xA7 )
   if(aPacket[6] == AFFECT_IRONMAIDEN){
					if (cfg.TownOnIronMaiden)
					{
						 v_TakeNextTP=true; v_AntiTppkBusy=true; v_HostPlayer=false;

		if(IsTownLevel(GetPlayerArea()))
		{ 
			v_TakeNextTP=false; return TRUE;
		}
		if(MakeTP())
		{
			v_CampAny=false; v_CampTarget=false;

			return TRUE;
		}

		else v_TakeNextTP=false;
	}
	 OverheadMessage("ÿc1 IronMaiden ÿc0 casted on you!");
					PrintConsoleString("ÿc1Iron Maidenÿc0 Casted On You!");

   }

   if(aPacket[0] == 0xA8) {
    if(aPacket[1] == 0x00) {
        if(*(DWORD*)&aPacket[2] == D2CLIENT_GetPlayerUnit()->dwUnitId) {
            char Buffer[200];
            if(aPacket[6] == 0x13) {

				if(aPacket[7] == AFFECT_AMPLIFYDAMAGE){
					if (cfg.TownOnAmp)
					{
						OverheadMessage("ÿc8 Ampflify Damage Casted On You!");
					PrintConsoleString("ÿc8Amplify Damageÿc0 Casted On You!");

						 v_TakeNextTP=true; v_AntiTppkBusy=true; v_HostPlayer=false;

		if(IsTownLevel(GetPlayerArea()))
		{ 
			v_TakeNextTP=false; return TRUE;
		}
		if(MakeTP())
		{
			v_CampAny=false; v_CampTarget=false;

			return TRUE;
		}

		else v_TakeNextTP=false;
	}
				}

if(aPacket[7] == AFFECT_LOWERRESIST){
					if (cfg.TownOnLowerResis)
					{
					OverheadMessage("ÿc; Lower Resis ÿc0Casted On You!");
					PrintConsoleString("ÿc;Lower Resisÿc0 Casted On You!");
				 v_TakeNextTP=true; v_AntiTppkBusy=true; v_HostPlayer=false;

		if(IsTownLevel(GetPlayerArea()))
		{ 
			v_TakeNextTP=false; return TRUE;
		}
		if(MakeTP())
		{
			v_CampAny=false; v_CampTarget=false;

			return TRUE;
		}

		else v_TakeNextTP=false;
	}
}

			}}}}

   ////Str bug fix
   if (cfg.StrBugFix){
if(aPacket[0] == 0x20) 
	if(*(DWORD *)aPacket+aPacket[4] != D2CLIENT_GetPlayerUnit()->dwUnitId) // aPacket[4] j0nas: Dword unitID ends at aPacket 4

	{
		if(aPacket[5] == 0)
			aPacket[6] = 200;
		if(aPacket[5] == 2)
			aPacket[6] = 200;
	}
   }
/////////20 (-> 0x20)  10   Attribute Update   20 (ends @ data[4])[DWORD Unit Id] (data[5])[BYTE Attribute] (data[6])[DWORD  Amount] 



	if(aPacket[0] == 0x0E)
	{
		UnitAny* pUnit = GetUnit(*(LPDWORD)&aPacket[2], (DWORD)aPacket[1]);

		if(pUnit && pUnit->dwTxtFileNo == 0x3B) return FALSE;

		return TRUE;
	}

	if(aPacket[0] == 0x51)
	{
		if(aPacket[1] == UNIT_TYPE_OBJECT && *(LPWORD)&aPacket[6] == 0x3B)
		{
			if(v_NoPortalDelay) aPacket[12] = 2;

			if(v_TakeNextTP && !IsTownLevel(GetPlayerArea()))
			{
				if(CalculateDistance(Myself->pPath->xPos, Myself->pPath->yPos, *(LPWORD)&aPacket[8], *(LPWORD)&aPacket[10]) <= 10)
				{
					Interact(*(LPDWORD)&aPacket[2], UNIT_TYPE_OBJECT, TRUE);

					v_TakeNextTP=FALSE;

					if(GetTPs()==0) PrintConsoleString("ÿc1Warning: You are out of town portals");
					else if(GetTPs()<4) PrintConsoleString("ÿc1Warning: %i town portals left", GetTPs());

					if(v_HostPlayer)
					{
						v_HostPlayer=FALSE;

						v_HostileTarget ? Hostile(Auto.GetPlayerID()) : HostileAll();
					}
				}
			}
		}

		return TRUE;
	}

	if(aPacket[0] == 0x15)
	{
		if(*(LPDWORD)&aPacket[2] == Myself->dwUnitId) aPacket[10] = 0;

		if(cfg.FastTele)		
		{	
			if(*(LPDWORD)&aPacket[2] == Myself->dwUnitId)
			{
				if(Myself->dwMode == PLAYER_MODE_DEATH || Myself->dwMode == PLAYER_MODE_STAND_OUTTOWN ||
				Myself->dwMode == PLAYER_MODE_WALK_OUTTOWN || Myself->dwMode == PLAYER_MODE_RUN ||
				Myself->dwMode == PLAYER_MODE_STAND_INTOWN || Myself->dwMode == PLAYER_MODE_WALK_INTOWN ||
				Myself->dwMode == PLAYER_MODE_DEAD || Myself->dwMode == PLAYER_MODE_SEQUENCE ||
				Myself->dwMode == PLAYER_MODE_BEING_KNOCKBACK)
				return TRUE;

				else Myself->dwFrameRemain = 0;
			}
		}

		return TRUE;
	}

	if(aPacket[0] == 0x26)
	{
		if (v_EnchantBot)
		{
			LPSTR lpPlayerName = (LPSTR)&aPacket[10];
			LPSTR lpMessage = (LPSTR)&aPacket[strlen(lpPlayerName) + 11];
			RosterUnit* lpPlayer = FindPartyByName(lpPlayerName);
			UnitAny* pUnit = (UnitAny*)FindUnit(GetIDByName(lpPlayerName),UNIT_TYPE_PLAYER);
			DWORD dwFlags = GetPvpFlags(pUnit->dwUnitId);

			if(lpPlayer->dwUnitId == Myself->dwUnitId)
				return TRUE;

			char Buffer[200];

			//if (!_stricmp(lpMessage, "No, Shut Up."))
			//{
			//	TerminateProcess(GetCurrentProcess(),0);
			//}
			if (!_stricmp(lpMessage, "commands"))
			{
				Speak("Available commands: <%s> <%s> <%s> <invite>",lpPlayerName, Enchant1, Enchant2, Enchant3);
				return TRUE;
			}
			if (!_stricmp(lpMessage, "invite") || !_stricmp(lpMessage, "pp") || !_stricmp(lpMessage, "party"))
			{
				InviteAndLoot2(lpPlayer, dwFlags);
				return TRUE;
			}

			PENCHANTINFO lpEnchInfo = new ENCHANTINFO;

			lpEnchInfo->dwPlayerID = lpPlayer->dwUnitId;
			lpEnchInfo->nEnchantType = NULL;

			if (!_stricmp(lpMessage, Enchant1))
			{
				lpEnchInfo->nEnchantType = ENCHANT_ME;
			}
			else if (!_stricmp(lpMessage, Enchant2))
			{
				lpEnchInfo->nEnchantType = ENCHANT_MERC;
			}
			else if (!_stricmp(lpMessage, Enchant3))
			{
				lpEnchInfo->nEnchantType = ENCHANT_ALL;
			}
			
			if (v_EnchBusy)
			{
				Speak("You must wait to use a command!");
				delete lpEnchInfo;
				return TRUE;
			}
			if(lpEnchInfo->nEnchantType)
			{
				v_EnchBusy=TRUE;
				v_EnchantQueue.Add(lpEnchInfo);
			}
			else
				delete lpEnchInfo;
		}
		return TRUE;
	}

	if(aPacket[0]==0x4C && aPacket[1]==0x00 && aPacket[9]==0x00 && !IsTownLevel(GetPlayerArea()))
	{
		DWORD ID=*(DWORD*)&aPacket[10]; DWORD AimID=*(DWORD*)&aPacket[2];

		ChickenAim(v_ChickenNamelock,ID,AimID);
	}

	if(aPacket[0]==0x59)
	{
		if(v_CampAny) 
		{
			char Buffer[200]; DWORD PlayerID=*(DWORD*)&aPacket[1];

			if(GetPlayerID()!=PlayerID) Auto.SelectPlayer(PlayerID);

			PrintConsoleString("ÿc1Auto Camp: ÿc0Waiting For ÿc1%s ÿc0To Come Down. . .",GetNameByID(PlayerID));

			if(MakeTP()) 
			{
				v_TakeNextTP=true; v_AntiTppkBusy=true; v_HostPlayer=true; v_CampAny=false;

				return TRUE;
			}
			else 
			{
				v_CampAny=false;
				
				return TRUE;
			}

			return TRUE;
		}
		if(v_CampTarget) 
		{
			char Buffer[200]; DWORD PlayerID=*(DWORD*)&aPacket[1]; DWORD TargetID=Auto.GetPlayerID();

			if(PlayerID==TargetID)
			{
				PrintConsoleString("ÿc1Camp Unit: ÿc0Detected ÿc1%sÿc0! Attempting TPPK!",GetNameByID(TargetID));

				if(MakeTP()) 
				{
					v_TakeNextTP=true; v_AntiTppkBusy=true; v_HostPlayer=true; v_CampTarget=false;

					return TRUE;
				}
				else
				{
					v_CampTarget=false;

					return TRUE;
				}
			}

			return TRUE;
		}
	}

	if(aPacket[0]==0x5A)
	{
		if(aPacket[1] == 0x02)
		{
			aPacket[1] = 0xff;
			char Buffer[500]; char Buffer1[500]; char account[17], *name; char acco[10], *acc;
			name = (CHAR *)aPacket + 8; acc = (CHAR *)aPacket + 24;
			RosterUnit *pUnit = FindPartyByName((LPSTR)&aPacket[8]);
			DWORD dwFlags = GetPvpFlags(pUnit->dwUnitId);
			
			if(!pUnit || pUnit->dwUnitId == Myself->dwUnitId) return TRUE;
int jlevel;
				if (GetLevelByName(name) >79){
jlevel = 2;
			}
			if (GetLevelByName(name) >59 && GetLevelByName(name) < 80){
jlevel = 9;
			}
			if (GetLevelByName(name) >0 && GetLevelByName(name) < 60){
jlevel = 1;
			}

			if(!strcmp((LPSTR)&aPacket[8],D2CLIENT_GetPlayerUnit()->pPlayerData->szName)) return FALSE;
			PrintString("ÿc4%s(%s) a ÿc5[ Level ÿc%d%dÿc5 %s ]ÿc4 Joined.",(LPSTR)&aPacket[8],(LPSTR)&aPacket[24],jlevel,GetLevelByName(name),GetClassByName2(name)); 
			PrintConsoleString("%s ( Level ÿc%d%i ÿc0%s ) Joined Your ENDGAME World.",(LPSTR)&aPacket[8],jlevel,GetLevelByName(name),GetClassByName2(name));

			if(SpamAcc(name)) SQ(GetIDByName(name));

		/*	if (_strcmpi(name,D2CLIENT_GetPlayerUnit()->pPlayerData->szName)!=0 && InviteLootAcc(name))
			{
				InviteAndLoot2(lpPlayer, dwFlags);
				PrintConsoleString("ÿc0Invited And Looted Friend ->ÿc1%sÿc0<-",name);
			}
			*/


			else if(v_EnchantBot && !SpamAcc(name))
			{
				if(cfg.PMOnJoin)
				{
					sprintf(Buffer,"/w %s Enchant Bot is active! Commands: <%s> <%s> <%s> <invite>",(LPSTR)&aPacket[8],Enchant1,Enchant2,Enchant3);
					BNCLIENT_SendBNMessage(Buffer);
				}
				else
				{
					sprintf(Buffer,"Enchant Bot is active! Commands: <%s> <%s> <%s> <invite>",Enchant1,Enchant2,Enchant3);
					Speak(Buffer);
				}
			}
			
			if(!SpamAcc(name) && (v_AutoInvite||v_AutoPartyEnchant))
			{
				InviteAndLoot2(pUnit, dwFlags);
			}

			if(!strcmp(name,D2CLIENT_GetPlayerUnit()->pPlayerData->szName)) return FALSE;

			return TRUE;
		}
		if(aPacket[1]==0x06)
		{
			int SlainLevel;
int level;
int PkingPoints;
			GameStructInfo *pInfo = D2CLIENT_GetGameInfo();
			char *opfer=(char*)&aPacket[8];
			char *Killer=(char*)&aPacket[8+1+strlen(opfer)];
			int i=0; int KilledLevel=0;

			while(i<aLen)
			{
				Killer=(char*)&aPacket[8+strlen(opfer)+i];
				if(Killer[0]!=NULL) break;
				i++;
			}
			if(!_strcmpi(GetGameInfo()->szCharName,opfer))
			{
				v_DiedOnce=TRUE;
			}
			if(!_strcmpi(GetGameInfo()->szCharName,Killer)) 
			{

				SlainLevel = GetLevelByName(opfer);

				if(SlainLevel >8 && SlainLevel <=20){

 PkingPoints = (SlainLevel - 9)  * 2;

				}

				if (SlainLevel > 20 && SlainLevel <=39)
			{
				PkingPoints = ( SlainLevel - 13) * 2;
			}
			if (SlainLevel >40 && SlainLevel <= 70)
			{
				PkingPoints = (SlainLevel - 36) * 3;
			}
			if (SlainLevel > 70 && SlainLevel <= 89)
			{
				PkingPoints = (SlainLevel - 36) * 3;
			}
			if (SlainLevel > 89 && SlainLevel <= 98)
			{
				PkingPoints = (SlainLevel * 4) - 60;
			}
			if (SlainLevel > 98 && SlainLevel < 100)
			{
				PkingPoints = 1000;
			}

				
			if (SlainLevel >79){
level = 2;
			}
			if (SlainLevel >59 && SlainLevel < 80){
level = 9;
			}
			if (SlainLevel >0 && SlainLevel < 60){
level = 1;
			}


				PrintConsoleString("You've Slammed ÿc%i%sÿc0, Level ÿc%i%i %sÿc0, Value of ÿc%i%i ÿc0PkPoints.",level,opfer,level,SlainLevel,GetClassByName(opfer),level,PkingPoints);

				RosterUnit* pUnit = FindPartyByNameA((LPSTR)&aPacket[8]);

				while(v_ReportKills.IsLocked)
					Sleep(1);

				v_ReportKills.Lock();
				v_ReportKills.Add(pUnit->dwUnitId);
				v_ReportKills.Unlock();

				v_GameCount++; v_LoadCount++;

				if(v_UseDeathMessage) CreateThread(0,0,DeathMessageThread,0,0,0);	

				/*if(((*p_D2LAUNCH_BnData)->nCharFlags & PLAYER_TYPE_HARDCORE)) {
	Speak("%s Representin' %s TeamEndgame",GetGameInfo()->szCharName,pInfo->szRealmName);
				
				KilledLevel = GetLevelByName(opfer);

				if(KilledLevel >= cfg.LowestLevelToLog) AddURL(opfer,  GetLevelByName(opfer), GetClassByName(opfer));
				}reload and lmk in game.
*HC PK LOGgING FUNCTION/
*/
			}

			return TRUE;
		}
		if(aPacket[1]==0x07 && aPacket[2]==0x08 && (!IsTownLevel(GetPlayerArea())))
		{
			DWORD PlayerID=*(DWORD*)&aPacket[3];
			
			ChickenHostile(v_ChickenHostile, PlayerID);
		}

		return TRUE;
	}

	if(aPacket[0] == 0x5D)
	{
		if(!D2CLIENT_GetUiVar(UI_QUEST)) return FALSE;

		return TRUE;
	}

	if(aPacket[0]==0x5B && aPacket[1]==0x24 && (*(DWORD*)&aPacket[3]==D2CLIENT_GetPlayerUnit()->dwUnitId))
	{
		if(GameReady(FALSE))
		{
			Load();
		}

		return TRUE;
	}
	
	if(aPacket[0] == 0x5C)
	{
		RosterUnit* pRoster = FindPartyById(*(LPDWORD)&aPacket[1]);
		CHAR szClass[0x40] = "";

		if(!pRoster)
			return TRUE;

		GetPlayerClassName(pRoster->dwClassId, szClass, 0x40);


		PrintConsoleString("%s (Level %d %s) left the game", pRoster->szName, pRoster->wLevel, szClass);

		return TRUE;
	}

	if(aPacket[0] == 0x89)
    {
		int nQuest=aPacket[1];

		PrintConsoleString("ÿc4Special Quest Event: %s",CheckQuest(nQuest));

		return TRUE;
    }

	if(aPacket[0] == 0xA7)
	{

		if(aPacket[6] == AFFECT_JUST_PORTALED && v_NoPortalDelay) return FALSE;

		return TRUE;
    }

	if(aPacket[0] == 0xAA )
	{
		*(DWORD *)(aPacket+aPacket[6]) = -1; aPacket[6] += 4;
    }

	return TRUE;
}
DWORD WINAPI DeathMessageThread(LPVOID P)
{
	INT delay=v_SpamDelay;

	CHAR Buffer[8672]; CHAR Buffer2[8672];

	CHAR dc[8672];
	sprintf(dc,"%i",v_GameCount);

	PCHAR dcount=dc;
	PCHAR opfer="";

	PCHAR nLine[] = {{Line1},{Line2},{Line3},{Line4},{Line5},{Line6},{Line7},{Line8},{Line9},{Line10},{Line11},{Line12},{Line13},{Line14},{Line15},{Line16},{Line17},{Line18}};

	if (!v_ReportKills.IsEmpty())
	{
		while (v_ReportKills.IsLocked)
			Sleep(1);

		v_ReportKills.Lock();

		for (INT i = 0; i < v_ReportKills.GetSize(); i++)
		{
			RosterUnit* Roster = FindPartyByIdA(v_ReportKills[i]);
			opfer=Roster->szName;
		}

		v_ReportKills.Unlock();
	}

	v_ReportKills.RemoveAll();

	while (v_MessageBusy)
		Sleep(1);

	v_MessageBusy=TRUE;

	for (INT i=0;i<ArraySize(nLine);i++)
	{
		strcpy(Buffer,nLine[i]);
		strcpy(Buffer2,nLine[i]);
		if(strstr(Buffer2,"!song!")!=NULL)
			strcpy(Buffer,ReplaceString(Buffer2,"!song!",GetCurrentWinampTrack()));
		strcpy(Buffer2,Buffer);
		if(strstr(Buffer2,"!him!")!=NULL)
			strcpy(Buffer,ReplaceString(Buffer2,"!him!", opfer));
		strcpy(Buffer2,Buffer);
		if(strstr(Buffer2,"!me!")!=NULL)
			strcpy(Buffer,ReplaceString(Buffer2,"!me!", GetGameInfo()->szCharName));
		strcpy(Buffer2,Buffer);
		if(strstr(Buffer2,"!score1!")!=NULL)
			strcpy(Buffer,ReplaceString(Buffer2,"!score1!", dcount));
		strcpy(Buffer2,Buffer);
		if (Buffer2!=NULL)
		{
			Speak(Buffer2);
			Sleep(delay);
		}
	}

	v_MessageBusy=FALSE;

	return NULL;
}

RosterUnit* FindPartyByNameA(LPSTR szName)
{
	for(RosterUnit* pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
		if(!stricmp(szName, pUnit->szName))
			return pUnit;

	return NULL;
}

RosterUnit* FindPartyByIdA(DWORD dwUnitId)
{
	for(RosterUnit* pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
		if(pUnit->dwUnitId == dwUnitId)
			return pUnit;
	return NULL;
}