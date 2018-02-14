#include "Houdini.h"
DWORD nGameTimer;
bool PrintExp=false;
int AreaLocaleIds2[] = {0, 5055, 5054, 5053, 5052, 5051, 5050, 5049, 3714, 5047, 5046, 5045, 5044, 5043, 5042, 5041, 5040, 5039, 5038, 5037, 5036, 5035, 5034, 5033, 5032, 5031, 5030, 5029, 5028, 5027, 5026, 5025, 5024, 5023, 5022, 5021, 5020, 5019, 5018, 788, 852, 851, 850, 849, 848, 847, 846, 845, 844, 843, 842, 841, 840, 839, 838, 837, 836, 835, 834, 833, 832, 831, 830, 829, 828, 827, 826, 826, 826, 826, 826, 826, 826, 825, 824, 820, 819, 818, 817, 816, 815, 814, 813, 812, 810, 811, 809, 808, 806, 805, 807, 804, 845, 844, 803, 802, 801, 800, 799, 798, 797, 796, 795, 790, 792, 793, 794, 791, 789, 22646, 22647, 22648, 22649, 22650, 22651, 22652, 22653, 22654, 22655, 22656, 22657, 22660, 21865, 21866, 21867, 22658, 22659, 22662, 22663, 22664, 22665, 22667, 22666, 11155, 11156, 11157, 5018};

extern unsigned long int nLevelExp[] = {0, 500, 1500, 3750, 7875, 14175, 22680, 32886, 44396, 57715, 72144, 90180, 112725, 140906, 176132, 220165, 275207, 344008, 
		430010, 537513, 671891, 839864, 1049830, 1312287, 1640359, 2050449, 2563061, 3203826, 3902260, 4663553, 5493363, 
		6397855, 7383752, 8458379, 9629723, 10906488, 12298162, 13815086, 15468534, 17270791, 19235252, 21376515, 23710491, 
		26254525, 29027522, 32050088, 35344686, 38935798, 42850109, 47116709, 51767302, 56836449, 62361819, 68384473, 74949165, 
		82104680, 89904191, 98405658, 107672256, 117772849, 128782495, 140783010, 153863570, 168121381, 183662396, 200602101, 
		219066380, 239192444, 261129853, 285041630, 311105466, 339515048, 370481492, 404234916, 441026148, 481128591, 524840254, 
		572485967, 624419793, 681027665, 742730244, 809986056, 883294891, 963201521, 1050299747, 1145236814, 1248718217, 
		1361512946, 1484459201, 1618470619, 1764543065, 1923762030, 2097310703, 2286478756, 2492671933, 2717422497, 2962400612, 
		3229426756, 3520485254, 0};

void DrawOriginal(void)
{
	GameStructInfo *pInfo = D2CLIENT_GetGameInfo();
	BnetData *pInfo2 = *p_D2LAUNCH_BnData;
	UnitAny *pUnit = D2CLIENT_GetPlayerUnit();
	wchar_t* pLocaleText;
 
	DWORD CurrentExpierences=D2COMMON_GetUnitStat(Myself,13,0)-v_CurrentExp;
	DWORD MaxExp=GetExpLeft()-D2COMMON_GetUnitStat(Myself,13,0);
	
	char nLevelName[50];
	pLocaleText = D2LANG_GetLocaleText(AreaLocaleIds2[pUnit->pPath->pRoom1->pRoom2->pLevel->dwLevelNo]);
	WToC(pLocaleText, nLevelName);

	char nGame[15];
	pLocaleText = D2LANG_GetLocaleText(4181);
	WToC(pLocaleText, nGame);
	strcat_s(nGame, "%s");

	char nPass[15];
	pLocaleText = D2LANG_GetLocaleText(4182);
	WToC(pLocaleText, nPass);
	strcat_s(nPass, "%s");

	char nDiffName[25];
	pLocaleText = D2LANG_GetLocaleText(4183);
	WToC(pLocaleText, nDiffName);

	int DiffIds[] = {5156, 5155, 5154};
	char nDiff[25];
	pLocaleText = D2LANG_GetLocaleText(DiffIds[D2CLIENT_GetDifficulty()]);
	WToC(pLocaleText, nDiff);
	
	char szTime[200];
	int nTime = ((GetTickCount () - nGameTimer) / 1000);
	sprintf_s(szTime, 200, "%.2d:%.2d:%.2d", nTime/3600, (nTime/60)%60, nTime%60);

	char nExpPercent[50];
	int nLevel = D2COMMON_GetUnitStat(Myself, 12, 0);
	unsigned long int nExp = D2COMMON_GetUnitStat(pUnit, 13, 0);
	int nExpPer = abs((float)((double)(nExp - nLevelExp[nLevel - 1]) / (double)(nLevelExp[nLevel] - nLevelExp[nLevel - 1])) * 100);
	int nExpDec1 = abs((float)((double)(nExp - nLevelExp[nLevel - 1]) / (double)(nLevelExp[nLevel] - nLevelExp[nLevel - 1])) * 1000);
	int nExpDec2 = (nExpDec1 - (nExpPer * 10));
	if(nExpPer <100)
	{
		if(nExpDec2 != 0) sprintf_s(nExpPercent, sizeof(nExpPercent), "%i.%i", nExpPer,nExpDec2);
		else sprintf_s(nExpPercent, sizeof(nExpPercent), "%i", nExpPer);
	}
	else
	{
		nExpPer=0;
		sprintf_s(nExpPercent, sizeof(nExpPercent), "%i", nExpPer);
	}

	char ServerIP[200];	
	char* sip = ((char*)((DWORD)GetModuleHandle("D2Client.dll")+0xDFFB0));
	sprintf_s(ServerIP, 200, "%s",sip);

	char GainedExp[200];
	INT gExp=0;
	if(D2COMMON_GetUnitStat(Myself,13,0)-v_CurrentExp!=NULL)
	gExp = D2COMMON_GetUnitStat(Myself,13,0)-v_CurrentExp;
	if(gExp != NULL) sprintf_s(GainedExp, 200, "%i",gExp);
	else sprintf_s(GainedExp, 200, "0",gExp);
	char *GainedExpFormat = FormatOutput(GainedExp);

	char RemainExp[200];
	DWORD RExp = nLevelExp[nLevel] - nExp;
	if(nLevelExp[nLevel] != NULL) sprintf_s(RemainExp, sizeof(RemainExp), "%d", RExp);
	else sprintf_s(RemainExp, sizeof(RemainExp), "0", RExp);
	char *RemainExpFormat = FormatOutput(RemainExp);

	DWORD GamesLe=0;
	char GamesL[200];
	if(MaxExp!=NULL&&gExp!=NULL)
	{
		GamesLe=RExp/gExp;
		sprintf_s(GamesL, 200, "%d",GamesLe);
	}
	else
		sprintf_s(GamesL, 200, "0");

	char *RemainGameFormat = FormatOutput(GamesL);

	char Type[50];
	int C1=4;

	if(IsExpansion()) strcpy(Type,"EXPANSION");
	else strcpy(Type,"");

	AutomapInfo Automap[] = 
	{
		{nGame, pInfo->szGameName},
		{nPass, pInfo->szGamePassword},
		{"%s", nLevelName},
		{"v 1.12", "v 1.12"},
		{nDiffName, nDiff},
		{"%s", Type},
	};	
	//AutomapInfo Automap2[] = 
	//{
	//	{nGame, pInfo->szGameName},
	//	{nPass, pInfo->szGamePassword},
	//	{nDiffName, nDiff},
	//	{"%s", ServerIP},
	//	{"%s", nLevelName},
	//};
	InfoBox Info[] = 
	{
		{"%s", ServerIP},
		{"Remain Exp: %s", RemainExpFormat},
		{"Gained Exp: %s", GainedExpFormat},
		{"Games Left: %s", RemainGameFormat},
		{"Total Exp: %s%%", nExpPercent},
	};
	int yLen = 8;

	for (int i = 0; i < ArraySize(Automap); i++)
	{
		if(strncmp(nDiff,"Normal",strlen("Normal")) == 0) strcpy(nDiff,"");

		if (strlen(Automap[i].Data) == 0) continue;

		yLen+=16;
			
		TextHook(780, yLen, 4, false, 1, 0, Automap[i].Message, Automap[i].Data);
	}
	yLen+=6;
	for (int i=0;i < ArraySize(Info);i++)
	{
		if (strlen(Info[i].Data) == 0) continue;

		yLen+=16;

		if(cfg.PlayerInfo && cfg.ShowInfoBox && !v_HideConsole) TextHook(780, yLen, C1, false, 13, 0, Info[i].Message, Info[i].Data);
	}
	if(PrintExp)
	{
		PrintExp=false;

		Speak("Remaining Exp: %s", RemainExpFormat);
		Speak("Gained Exp: %s", GainedExpFormat);
		Speak("Games Left: %s", RemainGameFormat);
		Speak("Total Exp: %s%%", nExpPercent);
	}
}


char* FormatOutput(char * String)
{
	int	x,c,d,i;
	char Work[20];

	x = strlen(String)-1;

	for (i=1,c=x+(x/3)+1,d=x; c>0; --c,--d,++i)
	{
		if(i%4==0)
		{
			*(Work+(--c)) = ',';
			i=1;
		}
		*(Work+(c-1)) = *(String+d);
	}

	Work[x+(x/3)+1] = '\0';

	strcpy(String, Work);

	return String;
}

