#include "Houdini.h"
#include <iostream>
#include <fstream>
using namespace std;

DWORD AA,LeftAA,InRange;

bool AddSpam(char *Name);
bool AddFriend(char *Name);

struct ConfigEntry {
	char* Setting;
	char* Usage;
};
ConfigEntry MainCommands[] = {
	{"AimBox"," <on/off>"},
	{"CleanShot"," <on/off>"},
	{"DeathMessage"," <on/off>"},
	{"Enchant"," <on/off>"},
	{"Speed"," <on/off>"},
	{"AddSpammer"," <character name>"},
	{"AddFriend"," <Character Name>"},
	{"Blind"," <1-5>"},
	{"Reload",""},
	{"Chicken",""},
	{"Drink",""},
	{"CheckSafe",""},
	{"Graphics",""}
};
ConfigEntry ChickenCommands[] = {
	{"Chicken"," Off"},
	{"Town Life"," <%>"},
	{"Town Mana"," <%>"},
	{"Quit Life"," <%>"},
	{"Quit Mana"," <%>"},
	{"On Aim"," <0-4>"},
	{"On Host"," <0-4>"}
};
ConfigEntry DrinkCommands[] = {
	{"Drink"," <on/off>"},
	{"Drink Life"," <%>"},
	{"Drink Mana"," <%>"},
	{"Drink LifeRej"," <%>"},
	{"Drink ManaRej"," <%>"}
};
ConfigEntry CheckSafeCommands[] = {
	{"CheckSafe Rejuv"," <on/off>"},
	{"CheckSafe Town"," <on/off>"}
};
ConfigEntry GraphicCommands[] = {
	{"Maphooks"," <on/off>"},
	{"AimVector"," <on/off>"},
	{"Missiles"," <on/off>"},
	{"Monsters"," <on/off>"},
	{"Vectors"," <on/off>"},
	{"Extra"," <on/off>"},
	{"Tag"," <on/off>"},
	{"Info"," <on/off>"},
	{"Orbs"," <on/off>"},
	{"Target"," <on/off>"},
	{"TimeStamp"," <on/off>"}
};
char* ChickenString[]={{"Off"},{"Town"},{"Tele"},{"Exit"},{"Warn"}};

struct SyntaxCorrect {
	char *nMsgA;
	char *nMsgB;
	char *nCorrect;
};
SyntaxCorrect SyntaxErrors[] = {
	{"Drink","Life","Drink Life <%>"},
	{"Drink","Mana","Drink Mana <%>"},
	{"Drink","LifeRej","Drink LifeRej <%>"},
	{"Drink","ManaRej","Drink ManaRej <%>"},
	{"Town","Life","Town Life <%>"},
	{"Town","Mana","Town Mana <%>"},
	{"Quit","Life","Quit Life <%>"},
	{"Quit","Mana","Quit Mana <%>"},
	{"CleanShot","","CleanShot <on/off>"},
	{"Monsters","","Monsters <on/off>"},
	{"Vectors","","Vectors <on/off>"},
	{"Missiles","","Missiles <on/off>"},
	{"AimBox","","AimBox <on/off>"},
	{"DeathMessage","","DeathMessage <on/off>"},
	{"Enchant","","Enchant <on/off>"},
	{"Orbs","","Orbs <on/off>"},
	{"Info","","Info <on/off>"},
	{"Extra","","Extra <on/off>"},
	{"Target","","Target <on/off>"},
	{"Tag","","Tag <on/off>"},
	{"AddSpammer","","AddSpammer <character name>"},
	{"AddFriend","","AddFriend <Character name>"},
};
bool InputMessage(char** Msg, int argc) 
{
	for(int i=0;i<10;i++)
	{
		if(!Msg[i])
			Msg[i]="???";
	}
	if(!strcmpi(Msg[0],"exp")) 
	{
		Speak("?exp");
		PrintExp=true;
		return true;
	}
	if(!strcmpi(Msg[0], "reload"))
	{
		ReadMainConfig();
		ReadBps();
		ReadBoxes();

		PrintConsoleString("ÿc2Successfully Reloaded Config");

		return true;
	}
	if(!strcmpi(Msg[0],"help")) 
	{
		PrintString("ÿc2:: ÿc4Main Commands");
		for(int i=0;i<ArraySize(MainCommands);i++)
		{
			PrintString("ÿc4.%s%s",MainCommands[i].Setting,MainCommands[i].Usage);
		}

		return true;
	}
	if(!strcmpi(Msg[0], "chicken") && argc == 1)
	{
		PrintString("ÿc2:: ÿc4Chicken Commands");
		for(int i=0;i<ArraySize(ChickenCommands);i++)
		{
			PrintString("ÿc4.%s%s",ChickenCommands[i].Setting,ChickenCommands[i].Usage);
		}

		return true;
	}
	if((!strcmpi(Msg[0], "drink")) && argc == 1)
	{
		PrintString("ÿc2:: ÿc4Drink Commands");
		for(int i=0;i<ArraySize(DrinkCommands);i++)
		{
			PrintString("ÿc4.%s%s",DrinkCommands[i].Setting,DrinkCommands[i].Usage);
		}

		return true;
	}
	if(!strcmpi(Msg[0], "CheckSafe") && argc == 1)
	{

		PrintString("ÿc2:: ÿc4CheckSafe Commands");
		for(int i=0;i<ArraySize(CheckSafeCommands);i++)
		{
			PrintString("ÿc4.%s%s",CheckSafeCommands[i].Setting,CheckSafeCommands[i].Usage);
		}

		return true;		
	}
	if(!strcmpi(Msg[0], "graphics") && argc == 1) 
	{
		PrintString("ÿc2:: ÿc4Graphics Commands");
		for(int i=0;i<ArraySize(GraphicCommands);i++)
		{
			PrintString("ÿc4.%s%s",GraphicCommands[i].Setting,GraphicCommands[i].Usage);
		}

		return true;
	}
	for (int i=0;i<ArraySize(SyntaxErrors);i++)
	{
		if (!strcmpi(Msg[0],SyntaxErrors[i].nMsgA))
		{
			if (SyntaxErrors[i].nMsgB!="")
			{
				if (!strcmpi(Msg[1],SyntaxErrors[i].nMsgB))
				{
					if (argc==2)
					{
						PrintConsoleString("ÿc1Correct usage is .%s",SyntaxErrors[i].nCorrect);

						return TRUE;
					}
				}

				continue;
			}
			else if (SyntaxErrors[i].nMsgB=="")
			{
				if (argc==1)
				{	
					PrintConsoleString("ÿc1Correct usage is .%s",SyntaxErrors[i].nCorrect);

					return TRUE;
				}
			}
		}
	}
	if(!strcmpi(Msg[0],"test")) 
	{
		while(v_ReportKills.IsLocked)
			Sleep(1);

		v_ReportKills.Lock();
		v_ReportKills.Add(Myself->dwUnitId);
		v_ReportKills.Unlock();

		CreateThread(0,0,DeathMessageThread,0,0,0);

		return TRUE;
	}
	if(!strcmpi(Msg[0],"config"))
	{
		v_ConfigBox= !(v_ConfigBox);
		return TRUE;
	}
	if(!strcmpi(Msg[0], "chicken"))
	{
		if(!strcmpi(Msg[1], "off"))
		{
			v_AutoPot=0;
			v_DrinkLifePot=0;
			v_DrinkLifeRej=0;
			v_DrinkManaPot=0;
			v_DrinkManaRej=0;
			v_TownLife=0;
			v_ExitLife=0;
			v_TownMana=0;
			v_ExitMana=0;
			v_SafeTown=false;
			v_SafeRejuv=false;
			v_ChickenNamelock=0;
			v_ChickenHostile=0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1All Chicken Settings Disabled");
			}

			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "drink"))
	{
		if(!strcmpi(Msg[1], "on"))
		{
			if(!v_AutoPot) 
			{
				if(cfg.PrintCommands)
				{
					PrintConsoleString("ÿc1Auto Potion Enabled");
				}
				v_AutoPot = 1;

				return TRUE;
			}
			return TRUE;
		}
		else if(!strcmpi(Msg[1], "off"))
		{
			if(v_AutoPot) 
			{
				if(cfg.PrintCommands)
				{
					PrintConsoleString("ÿc1Auto Potion Disabled");
				}
				v_AutoPot = 0;

				return TRUE;
			}
			return TRUE;
		}
		if(!strcmpi(Msg[1], "Life") && argc == 3)
        {
			char Buffer[20];

			v_DrinkLifePot = atoi(Msg[2]);
			
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Drink Life is set to %i", v_DrinkLifePot);
			}

			return TRUE;
		}

		if(!strcmpi(Msg[1] ,"Mana") && argc == 3)
        {
			char Buffer[20];

			v_DrinkManaPot = atoi(Msg[2]);
			
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Drink Mana is set to %i", v_DrinkManaPot);
			}

			return TRUE;
		}

		if(!strcmpi(Msg[1], "liferej") && argc == 3)
        {
			char Buffer[20];

			v_DrinkLifeRej=atoi(Msg[2]);
			
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Drink LifeRej is set to %i", v_DrinkLifeRej);
			}

			return TRUE;
		}

		if(!strcmpi(Msg[1], "manarej") && argc == 3)
        {
			char Buffer[20];

			v_DrinkManaRej = atoi(Msg[2]);
			
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Drink ManaRej is set to %i", v_DrinkManaRej);
			}

			return TRUE;
		}
	}

	if(!strcmpi(Msg[0], "town"))
    {
		if(!strcmpi(Msg[1], "life") && argc == 3)
        {
			char Buffer[20];

			v_TownLife = atoi(Msg[2]);
			
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Town Life is set to %i", v_TownLife);
			}

			return TRUE;
		}

		if(!strcmpi(Msg[1], "mana") && argc == 3)
        {
			char Buffer[20];

			v_TownMana = atoi(Msg[2]);
			
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Town Mana is set to %i", v_TownMana);
			}

			return TRUE;
		}
	}

	if(!strcmpi(Msg[0], "quit"))
    {
		if(!strcmpi(Msg[1], "life") && argc == 3)
        {
			char Buffer[20];

			v_ExitLife = atoi(Msg[2]);
			
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Quit Life is set to %i", v_ExitLife);
			}

			return TRUE;
		}

		if(!strcmpi(Msg[1], "mana") && argc == 3)
        {
			char Buffer[20];

			v_ExitMana = atoi(Msg[2]);
			
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Quit Mana is set to %i", v_ExitMana);
			}

			return TRUE;
		}
	}

	if(!strcmpi(Msg[0], "On") && !strcmpi(Msg[1], "Aim") && argc == 3)
	{
		if(atoi(Msg[2]) <0 || atoi(Msg[2]) > 4)

			return TRUE;

		v_ChickenNamelock = atoi(Msg[2]);

		if(cfg.PrintCommands)
		{
			for (int i=0;i<=4;i++)
			{
				if (v_ChickenNamelock==i)
				{
					PrintConsoleString("ÿc1Aim Chicken is set to %s",ChickenString[i]);
				}
			}
		}
		return TRUE;
	}
	if(!strcmpi(Msg[0], "On") && !strcmpi(Msg[1], "Host")&& argc == 3)
    {
		if(atoi(Msg[2]) <0 || atoi(Msg[2]) > 4)

			return TRUE;

		v_ChickenHostile = atoi(Msg[2]);

		if(cfg.PrintCommands)
		{
			for (int i=0;i<=4;i++)
			{
				if (v_ChickenHostile==i)
				{
					PrintConsoleString("ÿc1Hostile Chicken is set to %s",ChickenString[i]);
				}
			}
		}
		return TRUE;
	}
	if(!strcmpi(Msg[0], "Commands")) 
	{
		if(!strcmpi(Msg[1], "on"))
		{
			cfg.PrintCommands = 1;
			return TRUE;
		}

		if(!strcmpi(Msg[1], "off"))
		{
			cfg.PrintCommands = 0;
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "blind")) 
	{
		if(atoi(Msg[1]) >=1 && atoi(Msg[1])<=5)
		{
			v_Blind = atoi(Msg[1]);

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Blind Aiming is now set to %i",v_Blind);
			}
			return TRUE;
		}
	}

	if((!strcmpi(Msg[0], "speed")) && (!strcmpi(Msg[1], "on")) && argc == 2)
	{
		cfg.IncreaseCastSpeed=TRUE;

		if(cfg.PrintCommands)
		{
			PrintConsoleString("ÿc1Speed Enabled");
		}
		return TRUE;
	}
	if((!strcmpi(Msg[0], "speed")) && (!strcmpi(Msg[1], "off")) && argc == 2)
	{
		cfg.IncreaseCastSpeed=false;

		if(cfg.PrintCommands)
		{
			PrintConsoleString("ÿc1Speed Disabled");
		}
		return TRUE;
	}
	if(!strcmpi(Msg[0], "deathmessage")) 
	{
		if(!strcmpi(Msg[1], "on"))
		{
			v_UseDeathMessage = 1;

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Death Message Enabled");
			}
			return TRUE;
		}
		if(!strcmpi(Msg[1], "off"))
		{
			v_UseDeathMessage = 0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Death Message Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "Monsters")) 
	{
		if(!strcmpi(Msg[1], "on")) 
		{
			cfg.DrawMonsters = 1;

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Monster Blob Drawing Enabled");
			}
			return TRUE;
		}
		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.DrawMonsters = 0;

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Monster Blob Drawing Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "Vectors")) 
	{
		if(!strcmpi(Msg[1], "on"))
		{
			cfg.DrawVectors = 1;

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Vector Drawing Enabled");
			}
			return TRUE;
		}

		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.DrawVectors = 0;

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Vector Drawing Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "Missiles")) 
	{
		if(!strcmpi(Msg[1], "on"))
		{
			cfg.DrawMissiles = 1;

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Missile Drawing Enabled");
			}
			return TRUE;
		}

		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.DrawMissiles = 0;

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Missile Drawing Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "AimVector")) 
	{
		if(!strcmpi(Msg[1], "on"))
		{
			cfg.PlayerLine = 1;

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Aim Vector Enabled");
			}
			return TRUE;
		}

		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.PlayerLine = 0;

			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Aim Vector Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "maphooks")) 
	{
		if(!strcmpi(Msg[1], "on"))
		{
			cfg.DrawVectors=1;
			cfg.DrawMonsters=1;
			cfg.DrawMissiles=1;
			cfg.PlayerLine=1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Map Hooks Enabled");
			}
			return TRUE;
		}

		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.LevelNames=0;
			cfg.DrawVectors=0;
			cfg.DrawMonsters=0;
			cfg.DrawMissiles=0;
			cfg.PlayerLine=0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Map Hooks Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "drop")) 
	{
		TerminateProcess(GetCurrentProcess(),0);
	}
	if(!strcmpi(Msg[0], "AimBox")) 
	{
		if(!strcmpi(Msg[1], "on")) 
		{
			cfg.AimBoxEnabled = 1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Aim Box Enabled");
			}
			return TRUE;
		}

		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.AimBoxEnabled = 0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Aim Box Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "Timestamp")) 
	{
		if(!strcmpi(Msg[1], "on")) 
		{
			cfg.TimeStamp = 1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Time Stamp Enabled");
			}
			return TRUE;
		}

		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.TimeStamp = 1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Time Stamp Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "tag")) 
	{
		if(!strcmpi(Msg[1], "on")) 
		{
			cfg.IHPKTag = 1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Ping Tag Enabled");
			}
			return TRUE;
		}

		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.IHPKTag = 0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Ping Tag Disabled");
			}
			return TRUE;
		}
	}

	if(!strcmpi(Msg[0], "orbs"))
	{
		if(!strcmpi(Msg[1], "on"))
		{
			cfg.ShowLifeManaPer = 1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Orb Percents Enabled");
			}
			return TRUE;
		}	
		if(!strcmpi(Msg[1], "off"))
		{
			cfg.ShowLifeManaPer = 0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Orb Percents Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "info")) 
	{
		if(!strcmpi(Msg[1], "on")) 
		{
			cfg.ShowInfoBox = 1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Game Info Enabled");
			}
			return TRUE;
		}
		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.ShowInfoBox = 0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Game Info Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "Extra")) 
	{
		if(!strcmpi(Msg[1], "on")) 
		{
			cfg.ExtraInfo = 1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Extra Info Enabled");
			}
			return TRUE;
		}

		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.ExtraInfo = 0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Extra Info Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "target")) 
	{
		if(!strcmpi(Msg[1], "on")) 
		{
			cfg.ShowTargetList = 1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Target List Enabled");
			}
			return TRUE;
		}
		if(!strcmpi(Msg[1], "off")) 
		{
			cfg.ShowTargetList = 0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Target List Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "cleanshot")) 
	{
		if(!strcmpi(Msg[1], "on")) 
		{
			v_CS = 1;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Clean Screenshots Enabled");
			}
			return TRUE;
		}
		if(!strcmpi(Msg[1], "off")) 
		{
			v_CS = 0;
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc1Clean Screenshots Disabled");
			}
			return TRUE;
		}
	}
	if(!strcmpi(Msg[0], "checksafe")) 
	{
		if(!strcmpi(Msg[1], "rejuv")) 
		{
			if(!strcmpi(Msg[2], "on")) 
			{
				v_SafeRejuv = 1;
				if(cfg.PrintCommands)
				{
					PrintConsoleString("ÿc1Checksafe Rejuv Enabled");
				}
				return TRUE;
			}
			if(!strcmpi(Msg[2], "off")) 
			{
				v_SafeRejuv = 0;
				if(cfg.PrintCommands)
				{
					PrintConsoleString("ÿc1Checksafe Rejuv Disabled");
				}
				return TRUE;
			}
		}
		if(!strcmpi(Msg[1], "town")) 
		{
			if(!strcmpi(Msg[2], "on")) 
			{
				v_SafeTown = 1;
				if(cfg.PrintCommands)
				{
					PrintConsoleString("ÿc1Checksafe Town Enabled");
				}
				return TRUE;
			}
			if(!strcmpi(Msg[2], "off")) 
			{
				v_SafeTown = 0;
				if(cfg.PrintCommands)
				{
					PrintConsoleString("ÿc1Checksafe Town Disabled");
				}
				return TRUE;
			}
		}
	}
	if(!strcmpi(Msg[0], "Enchant")) 
	{
		if(!strcmpi(Msg[1], "on") && !v_EnchantBot) 
		{
			v_EnchantBot=v_AutoPartyEnchant=TRUE;

			if(v_EnchantBot) 
			{
				PrintConsoleString("ÿc1Enchant Bot Activated");
				Speak("Enchant Bot has been Activated! Commands: <%s> <%s> <%s>", Enchant1, Enchant2, Enchant3);
			}

			return TRUE;
		}
		else if(!strcmpi(Msg[1], "off") && v_EnchantBot) 
		{
			v_EnchantBot=v_AutoPartyEnchant=FALSE;

			if(!v_EnchantBot)
			{
				PrintConsoleString("ÿc1Enchant Bot ÿc1Deactivated");
				Speak("Enchant Bot has been Deactivated!");
			}

			return TRUE;
		}

		return TRUE;
	}
	if(!strcmpi(Msg[0], "addspammer"))
	{
		char Buffer[100];
		if(AddSpam(Msg[1]))
		{
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc8Added %s to spammer list!", Msg[1]);
			}
			return TRUE;
		}
		if(cfg.PrintCommands)
		{
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc8 Failed to add %s to spammer list!", Msg[1]);
			}
			PrintString(Buffer);
		}
	}

	if(!strcmpi(Msg[0], "addfriend"))
	{
		char Buffer[100];
		if(AddFriend(Msg[1]))
		{
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc8Added %s to friends list!", Msg[1]);
			}
			return TRUE;
		}
		if(cfg.PrintCommands)
		{
			if(cfg.PrintCommands)
			{
				PrintConsoleString("ÿc8 Failed to add %s to friends list!", Msg[1]);
			}
			PrintString(Buffer);
		}
	}
	if(!strcmpi(Msg[0], "ping"))
	{
		Speak("WHAT THE FUCK IS MY LAG");
		Speak("FPS: %i, Skip: %i, Ping: %i",D2CLIENT_FPS,*p_D2CLIENT_Skip,D2CLIENT_Ping);
		return TRUE;
	}
	if(!strcmpi(Msg[0], "ver"))
	{
		Speak("Message for the game:",VERSION_NO);
		Speak("Miguel Has Injected /Endgame. If Kill You with Xp shrine, Is it Increased?",VERSION_NO);
		return TRUE;
	}

	if(!strcmpi(Msg[0], "score"))
	{
		Speak("Whats my score sir?");
		Speak("Computer: Total Kills This Game: %i", v_GameCount);
		Speak("Computer: Total Kills Since Injection: %i", v_LoadCount);
		return TRUE;
	}


	if(!strcmpi(Msg[0], "time"))
	{
		SYSTEMTIME T;
		GetLocalTime(&T);

		char CurrentTime[200];
		char CurrentTime2[200];

		if(T.wHour == 0) sprintf(CurrentTime,"%.2d:%.2d:%.2d AM", T.wHour + 12, T.wMinute, T.wSecond);
		if(T.wHour >= 1 && T.wHour <= 11) sprintf(CurrentTime,"%.2d:%.2d:%.2d AM", T.wHour, T.wMinute, T.wSecond);
		if(T.wHour == 12) sprintf(CurrentTime,"%.2d:%.2d:%.2d PM", T.wHour, T.wMinute, T.wSecond);
		if(T.wHour >= 13 && T.wHour <= 23) sprintf(CurrentTime,"%.2d:%.2d:%.2d PM", T.wHour - 12, T.wMinute, T.wSecond);

		sprintf(CurrentTime2,"%d/%d/%d", T.wMonth, T.wDay, T.wYear);

		Speak("?time");
		Speak("%s - %s",CurrentTime2,CurrentTime);
		return TRUE;
	}
	if(!strcmpi(Msg[0], "fake1") && argc == 3)
	{
		char Buffer[100];
		if((Msg[1]!=NULL && Msg[2]!=NULL))
		{
			PrintString("ÿc4%s was slain by %s", Msg[1], GetGameInfo()->szCharName);
			PrintString("ÿc4%s(%s) left our world. Diablo's minions weaken.", Msg[1], Msg[2]);
			return TRUE;
		}
		return TRUE;
	}
	if(!strcmpi(Msg[0], "fake2") && argc == 2)
	{
		char Buffer[100];
		if((Msg[1]!=NULL))
		{
			PrintString("ÿc4%s was slain by %s", Msg[1], GetGameInfo()->szCharName);
			return TRUE;
		}
		return TRUE;
	}

	return FALSE;
}

bool AddSpam(char *Name)
{	
	char mypath[266]={0};
	char Buffer[100];

	sprintf_s(mypath,"%s\\%s",szModulePath,"Spammers.txt");
	sprintf_s(Buffer, "%s\n", Name);
	ofstream myfile(mypath, ios::app);
	if (myfile.is_open())
	{
		myfile << Buffer;
		myfile.close();
		return true;
	}
	if(cfg.PrintCommands)
	{
		PrintConsoleString("ÿc1Failed to Open Spammer List");
	}
	return false;
}
bool AddFriend(char *Name)
{	
	char mypath[266]={0};
	char Buffer[100];

	sprintf_s(mypath,"%s\\%s",szModulePath,"Friends.txt");
	sprintf_s(Buffer, "%s\n", Name);
	ofstream myfile(mypath, ios::app);
	if (myfile.is_open())
	{
		myfile << Buffer;
		myfile.close();
		return true;
	}
	if(cfg.PrintCommands)
	{
		PrintConsoleString("ÿc1Failed to Open Friends List");
	}
	return false;
}

bool Input(char* Text) 
{
	char *argv[32], Buffer2[100];

	if (Text[0] == cfg.Trigger)  
	{
		strcpy(Buffer2,Text+1);

		int argc=MakeParameter(Buffer2,argv,' ',FALSE);

		if (InputMessage(argv,argc))
		{
			return FALSE;
		}
		if (cfg.PrintCommands)
		{
			PrintConsoleString("ÿc1Invalid command"); 
		}

		return FALSE;
	}

	return TRUE;
}
BOOL __fastcall SendPacket(byte* data, DWORD aLen) 
{
	if(data[0]==0x15) 
	{
		char* bdata = (char*)data+3;

		if(strlen(bdata)>1)
			if(!Input(bdata))
				return FALSE;
	}
	
	if(v_AntiTppkBusy || v_TakeNextTP)
	{
		if(data[0]>=0x01 && data[0]<=0x11)
		{
			if(GetTPs()>0)
			return FALSE;
		}
	}

	//if(v_AttackPlayer) 
	//{
	//	if(GameReady(FALSE) && !IsTownLevel(GetPlayerArea()))
	//	{
	//		WORD SkillID = GetCurrentSkill(LeftAA);

	//		if(!v_BlindToggled||v_BlindToggled&&v_Blind!= 5&&SkillID != D2S_TELEPORT)
	//		{
	//			if(LeftAA)
	//			{
	//				if(data[0]==0x05||data[0]==0x08) 
	//				{						
	//					BYTE *aPacket = new BYTE[9];
	//					aPacket[0] = 0x06;
	//					*(DWORD*)&aPacket[1] = UNIT_TYPE_PLAYER;
	//					*(DWORD*)&aPacket[5] = Auto.GetPlayerID();
	//					D2NET_SendPacket(9, 1, aPacket);
	//					delete aPacket;

	//					return FALSE;
	//				}
	//			}
	//			else
	//			{
	//				if(data[0]==0x0c||data[0]==0x0f) 
	//				{						
	//					BYTE *aPacket = new BYTE[9];
	//					aPacket[0] = 0x0D;
	//					*(DWORD*)&aPacket[1] = UNIT_TYPE_PLAYER;
	//					*(DWORD*)&aPacket[5] = Auto.GetPlayerID();
	//					D2NET_SendPacket(9, 1, aPacket);
	//					delete aPacket;

	//					return FALSE;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (v_AttackMonster || v_AttackNearPlayer)
	//{
	//	if(GameReady(FALSE) && !IsTownLevel(GetPlayerArea()))
	//	{
	//		WORD SkillID = GetCurrentSkill(LeftAA);

	//		if(LeftAA)
	//		{
	//			if(data[0]==0x05||data[0]==0x08) 
	//			{						
	//				BYTE *aPacket = new BYTE[9];
	//				aPacket[0] = 0x07;
	//				*(DWORD*)&aPacket[1] = v_CurrentTarget->dwType;
	//				*(DWORD*)&aPacket[5] = v_CurrentTarget->dwUnitId;
	//				D2NET_SendPacket(9, 1, aPacket);
	//				delete aPacket;

	//				return FALSE;
	//			}
	//		}
	//		else
	//		{
	//			if(data[0]==0x0c||data[0]==0x0f) 
	//			{						
	//				BYTE *aPacket = new BYTE[9];
	//				aPacket[0] = 0x0E;
	//				*(DWORD*)&aPacket[1] = v_CurrentTarget->dwType;
	//				*(DWORD*)&aPacket[5] = v_CurrentTarget->dwUnitId;
	//				D2NET_SendPacket(9, 1, aPacket);
	//				delete aPacket;

	//				return FALSE;
	//			}
	//		}
	//	}
	//}

	return TRUE;
}