#include "Houdini.h"
#include "ArrayEX.h"

int Blind;
bool AutoParty1;
bool dLeft;
POINT Camp;
DWORD QuestID;

int GetMaxTrackTime() 
{ 
	HWND win = FindWindowA("Winamp v1.x",NULL); 
	if(win) return SendMessage(win,WM_USER,1,105); 

	return 0; 
} 

int GetCurrentTrackTime() 
{ 
	HWND win = FindWindowA("Winamp v1.x",NULL); 
	if(win) return SendMessage(win,WM_USER,0,105); 

	return 0; 
} 

DWORD WINAPI AdThread(void* p)
{
	INT delay=v_SpamDelay;
	CHAR Buffer[8672], Buffer2[8672], dc[8672];
	sprintf(dc,"%i",v_GameCount);
	PCHAR dcount=dc;

	PCHAR nSpam[] = {{Spam1},{Spam2},{Spam3},{Spam4},{Spam5},{Spam6},{Spam7},{Spam8},{Spam9},{Spam10},{Spam11},{Spam12},{Spam13},{Spam14},{Spam15},{Spam16},{Spam17},{Spam18}};

	if (v_SpamBusy)
		return NULL;

	v_SpamBusy=TRUE;

	for (INT i=0;i<ArraySize(nSpam);i++)
	{
		strcpy(Buffer,nSpam[i]);
		strcpy(Buffer2,nSpam[i]);
		if(strstr(Buffer2,"!song!")!=NULL)
			strcpy(Buffer,ReplaceString(Buffer2,"!song!",GetCurrentWinampTrack()));
		strcpy(Buffer2,Buffer);
		if(strstr(Buffer2,"!him!")!=NULL)		
			strcpy(Buffer,ReplaceString(Buffer2,"!him!", GetNameByID(Auto.GetPlayerID())));
		strcpy(Buffer2,Buffer);
		if(strstr(Buffer2,"!me!")!=NULL)		
			strcpy(Buffer,ReplaceString(Buffer2,"!me!", GetGameInfo()->szCharName));
		strcpy(Buffer2,Buffer);
		if(strstr(Buffer2,"!score1!")!=NULL)		
			strcpy(Buffer,ReplaceString(Buffer2,"!score1!", dcount));
		strcpy(Buffer2,Buffer);
		if(Buffer2!=NULL)
		{
			Speak(Buffer2);
			Sleep(delay);
		}
	}
	
	v_SpamBusy=FALSE;

	return NULL;	
}

void __fastcall KeyHook(BYTE keycode, BYTE repeat)
{ 	
	if(repeat) return;

	if( keycode ==  cfg.SilentExit)
	{
		if(!GetSkill(D2S_CONFUSE)) SetSkill(D2S_CONFUSE, FALSE);

		else SetSkill(D2S_HOLYSHIELD, FALSE);
	}

	if(keycode==cfg.PlayerInfoToggle)
	{
		if(!cfg.PlayerInfo) cfg.PlayerInfo=true;

		else cfg.PlayerInfo=false;
	}

	if( keycode == cfg.FastExit)
	{
		D2CLIENT_ExitGame();
	}

	if(keycode == cfg.Hostile) 
	{
		if(IsTownLevel(GetPlayerArea())) 
		{
			v_HostileTarget ? Hostile(Auto.GetPlayerID()) : HostileAll();

			PrintConsoleString("ÿc8You have declared hostility!");
		}
	}
	if(keycode == cfg.TeleToPlayer)
	{
		char Buffer[200];

		if(FindUnit(Auto.GetPlayerID(),0)!=NULL)
		{
			Tele(GetPlayerX(Auto.GetPlayerID()),GetPlayerY(Auto.GetPlayerID()));
		}

		if(FindUnit(Auto.GetPlayerID(),0)==NULL) return;
	}
	if( keycode == cfg.BlindAAToggle)
	{
		v_BlindToggled=!(v_BlindToggled);

		CHAR bToggle[50];

		if (v_BlindToggled) sprintf(bToggle,"On [%i]",v_Blind);
		else sprintf(bToggle,"Off");

		PrintConsoleString("ÿc1Blind Aiming is toggled %s",bToggle);

	}

	//v_Blind

	if (keycode == cfg.CycleBlinds)
	{
		Blind++;
		 if (Blind > 5)
	 {
	 Blind = 1;
	 }
		  PrintConsoleString("BlindSpot Switched To ÿc2%d!",Blind);
		 v_Blind = Blind;
	}

	if( keycode == cfg.TeleToPlayerHost)
	{
		if(FindUnit(Auto.GetPlayerID(),0)!=NULL)
		{
			if(IsTownLevel(GetPlayerArea()))
			{
				Hostile(Auto.GetPlayerID());
				Tele(GetPlayerX(Auto.GetPlayerID()),GetPlayerY(Auto.GetPlayerID()));
			}
			if(!IsTownLevel(GetPlayerArea()))
			{
				Tele(GetPlayerX(Auto.GetPlayerID()),GetPlayerY(Auto.GetPlayerID()));
			}

			if(FindUnit(Auto.GetPlayerID(),0)==NULL) return;
		}
	}
	if(keycode==cfg.FlashKey)
	{
		v_Flashing=!(v_Flashing);

		if(v_Flashing)
		{				
			PrintConsoleString("ÿc1Now Flashing %s",GetNameByID(Auto.GetPlayerID()));
		}
		else if(!v_Flashing)
		{
			PrintConsoleString("ÿc1Stopped Flashing %s",GetNameByID(Auto.GetPlayerID()));
		}
	}

		if (keycode==cfg.VigorCharge)
		if (GetSkill(D2S_VIGOR) && GetSkill(D2S_CHARGE))

		{
			SetSkill(D2S_CHARGE, !dLeft);
			SetSkill(D2S_VIGOR, dLeft);
		}
		else
		{
			OverheadMessage("You don't have vigor and/or charge!");
		}
		//
//ConcHammer
		if(keycode==cfg.ConcHammer)
		if (GetSkill(D2S_CONCENTRATION) && GetSkill(D2S_BLESSEDHAMMER))

		{

		SetSkill(D2S_BLESSEDHAMMER, !dLeft);
		SetSkill(D2S_CONCENTRATION, dLeft);

		}
		else
		{
			OverheadMessage("You dont have concentration and/or blessed hammer!");
		}


	if(keycode==cfg.OpenTP)
	{
		v_TakeNextTP=v_AntiTppkBusy=v_HostPlayer=v_CampAny=v_CampTarget=FALSE;

		if(IsTownLevel(GetPlayerArea()))
		{ 
			return;
		}
		else if(MakeTP())
		{
			return;
		}
	}
	if(keycode==cfg.FastTP)
	{
		v_TakeNextTP=true; v_AntiTppkBusy=true; v_HostPlayer=false;

		if(IsTownLevel(GetPlayerArea()))
		{ 
			v_TakeNextTP=false; return;
		}
		if(MakeTP())
		{
			v_CampAny=false; v_CampTarget=false;

			return;
		}

		else v_TakeNextTP=false;
	}

	if(keycode==cfg.TPPK)
	{
		v_TakeNextTP=true; v_AntiTppkBusy=true; v_HostPlayer=true;

		if(IsTownLevel(GetPlayerArea()))
		{ 
			v_TakeNextTP=false; v_HostPlayer=false; return;
		}
		if(MakeTP())
		{
			v_CampAny=false; v_CampTarget=false;

			return;
		}
		else
		{
			v_TakeNextTP=false; v_HostPlayer=false;
		}
	}

	if(keycode == cfg.NextTarget)
	{		
		Auto.GetNextPlayer();
	}
	if(keycode == cfg.LastTarget)
	{		
		Auto.GetLastPlayer();
	}

	if(keycode==cfg.AimSlotToggle)
	{
		LeftAA=!(LeftAA);
		PCHAR pChar=LeftAA ? "Left" : "Right";
		PrintConsoleString("ÿc8Aim slot switched to %s",pChar);
	}
	
	if(keycode==cfg.ViewLogs)
	{
		v_ConsoleLogs=!(v_ConsoleLogs);
	}

	if(keycode==cfg.Camp)
	{
		if(v_CampTarget || IsTownLevel(GetPlayerArea())) return;

		if(!v_CampAny)
		{
			GetD2MousePos(Camp); char Buf[100]; v_CampAny=true;

			PrintConsoleString("ÿc1Auto Camp Activated, Waiting for any player");
		}
		else if(v_CampAny)
		{
			v_CampAny=false;

			PrintConsoleString("ÿc1Auto Camp Deactivated");

			HoldRightUp(Camp.x,Camp.y);
		}
	}

	if(keycode==cfg.CampUnit)
	{
		if(v_CampAny || IsTownLevel(GetPlayerArea())) return;

		if(v_CampTarget)
		{
			v_CampTarget=false;
			PrintConsoleString("ÿc1Camp Unit Deactivated");

			HoldRightUp(Camp.x,Camp.y);
		}
		else if(!v_CampTarget)
		{
			GetD2MousePos(Camp);
			char Buf[100];
			v_CampTarget=true;
			PrintConsoleString("ÿc1Camp Unit Activated, Waiting for %s",GetNameByID(Auto.GetPlayerID()));
		}
	}

	if(keycode==cfg.NextLevelPath)
	{
		if(!IsTownLevel(GetPlayerArea()) && GetSkill(D2S_TELEPORT)!=NULL) 
		{					
			QuestID=1;
			PathToVector(1);

			return;
		}
	}
	if(keycode==cfg.LastLevelPath)
	{
		if(!IsTownLevel(GetPlayerArea()) && GetSkill(D2S_TELEPORT)!=NULL) 
		{					
			QuestID=2;
			PathToVector(2);

			return;
		}
	}
	if(keycode==cfg.WPPath)
	{
		if(!IsTownLevel(GetPlayerArea()) && GetSkill(D2S_TELEPORT)!=NULL) 
		{			
			QuestID=0;
			PathToVector(3);

			return;
		}
	}
	if(keycode==cfg.SpecialPath)
	{
		if(!IsTownLevel(GetPlayerArea()) && GetSkill(D2S_TELEPORT)!=NULL) 
		{					
			QuestID=3;
			PathToVector(4);

			return;
		}
	}

	if(keycode==cfg.ClearConsole)
	{
		for(INT i = 0; i < v_ConsoleList.GetSize(); i++)
			v_ConsoleList[i]->TimeStamp = 1;

		return;
	}
	if(keycode==cfg.ShowConfig)
	{
		v_ConfigBox=!(v_ConfigBox);
	}

	if(keycode==cfg.EnchBotToggle)
	{
		v_EnchantBot=!(v_EnchantBot);		
		v_AutoPartyEnchant=v_EnchantBot;

		if(v_EnchantBot)
		{
			PrintConsoleString("ÿc1Enchant Bot Activated");
			Speak("Enchant Bot has been Activated! Commands: <%s> <%s> <%s>",Enchant1,Enchant2,Enchant3);

			InviteAndLoot();

			return;
		}

		PrintConsoleString("ÿc1Enchant Bot Deactivated");
		Speak("Enchant Bot has been Deactivated!");
	}

	if(keycode==cfg.ItemViewer)
	{
		char Buffer[200];
		if(FindUnit(Auto.GetPlayerID(),0)!=NULL)
		{
			DrawPlayerInventory=true;
			InstallPatches();
			ToggleInventory(true); 
		}
		else return;
	}
	if(keycode==cfg.AutoPartyToggle)
	{
		AutoParty1=(!AutoParty1);

		if(AutoParty1)
		{
			PrintConsoleString("Auto Party ÿc2Activated!");
			CreateThread(0,0,AutoPartyThread1,0,0,0);
		}
		else
		{
			PrintConsoleString("Auto Party ÿc1Deactivated");
				TerminateThread(AutoPartyThread1,0);
		}
	}

	if(keycode==cfg.ADKey)
	{
		CreateThread(0,0,AdThread,0,0,0);
	}

	if(keycode==cfg.WNext)
	{
		NextSong();
		PrintConsoleString("ÿc1Now Playing: %s",GetCurrentWinampTrack());
	}
	if(keycode==cfg.WPrev)
	{
		PrevSong();
		PrintConsoleString("ÿc1Now Playing: %s",GetCurrentWinampTrack());
	}
	if(keycode==cfg.WStop)
	{ 
		StopSong();
		PrintConsoleString("ÿc1Stopped Winamp");
	}
	if(keycode==cfg.WPlay)
	{
		PlayPause();
		PrintConsoleString("ÿc1Playing/Pausing Winamp");
	}
	if(keycode==cfg.WSong)
	{
		HWND win = FindWindowA("Winamp v1.x",NULL);
		char Buffer[200];
		int ct = GetCurrentTrackTime(); 
		int mt = GetMaxTrackTime(); 

		if(win)
		{
			if((mt)%60 == -1)
			{
				Speak("%s: %s <%02d:%02d:%02d>",WinampPre,GetCurrentWinampTrack(),ct/(1000*60*60),ct/(1000*60)%60, (ct/1000)%60);
			}
			else
			{
				Speak("%s: %s <%02d:%02d:%02d/%02d:%02d:%02d>",WinampPre,GetCurrentWinampTrack(),ct/(1000*60*60),ct/(1000*60)%60, (ct/1000)%60,mt/(60*60),mt/(60)%60, (mt)%60);
			}
		}
		else
		{	
			Speak("%s: %s",WinampPre,GetCurrentWinampTrack());
		}
	}

	if(keycode==cfg.StashKey)
	{
		if(D2CLIENT_GetUiVar(0x19))
		{
			D2CLIENT_SetUIVar(0x19, 1, 0);
		}
		else
		{
			BYTE data[2] = {0x77, 0x10};
			D2NET_ReceivePacket(data, 2);
		}
	}
}