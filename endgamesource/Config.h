struct tConfig {
	//enchbot
    byte EnchBotToggle;
    bool AutoParty;
    bool PMOnJoin;
	//settings
	bool TownOnBoOut;
	bool TownOnAmp;
	bool TownOnLowerResis;
	bool TownOnIronMaiden;
	bool StrBugFix;
	bool OverHeadTrace;
	bool AntiMinimize;
    bool FastTele;
    bool ItemLevel;
    bool ItemSockets;
    bool ItemEth;
    bool InviteMembers;
    bool LootMembers;
	//messages
	bool PrintCommands;
	bool PrintChicken;
	//key settings
	byte ViewLogs;
    byte Trigger;
	byte OpenTP;
    byte FastTP;
    byte TPPK;
    byte NextTarget;
    byte LastTarget;
    byte Camp;
    byte CampUnit;
	byte VigorCharge;
	byte ConcHammer;
    byte PlayerInfoToggle;
    byte MoveBox;
    byte ItemViewer;
    byte TeleToPlayerHost;
    byte TeleToPlayer;
    byte FastExit;
    byte Hostile;
    byte SilentExit;
    byte AttackPlayer;
    byte AimSlotToggle;
	byte BlindAAToggle;
    byte LastLevelPath;
    byte NextLevelPath;
    byte SpecialPath;
    byte WPPath;
    byte StashKey;
    byte TargetInfo;
    byte FlashKey;
	byte AutoPartyToggle;
	byte ClearConsole;
	byte ShowConfig;
	byte AttackMonster;
	byte AttackNearest;
byte CycleBlinds;
	//aim box
    bool AimBoxEnabled;
    int AimBoxColor;
	//maphack
	bool Maphack;
	bool LevelNames;
    bool DrawMonsters;
    bool DrawImunities;
    bool DrawMissiles;
	bool DrawVectors;
	bool PlayerLine;
	int LowestLevelToLog;
    int MonsterColor;
    int ChampionMonsterColor;
    int BossMonsterColor;
    int MyMissile;
    int FriendMissile;
    int NeutralMissile;
    int EnemyMissile;
	//vector colors	
	int NextColor;
	int PrevColor;
	int WaypointColor;
	int SpecialColor;
	//AD
    byte ADKey;
	//winamp
    BYTE WNext;
    BYTE WPrev;
    BYTE WSong;
    BYTE WPlay;
    BYTE WStop;
	//screenHooks
	bool PlayerInfo;
	bool MoveBoxes;
	bool ShowTargetList;
	bool IncludeConsole;
	bool ShowInfoBox;
	bool TimeStamp;
	bool IHPKTag;
	bool ExtraInfo;
	bool ShowLifeManaPer;
	int RealTombX;
	int ScreenShotTimer;
	bool IncreaseCastSpeed;

	bool ShowCoordinates;
	int UtilBoxX;
	int UtilBoxY;
	int DrinkBoxX;
	int DrinkBoxY;
	int TargetBoxX;
	int TargetBoxY;
	int ChickenBoxX;
	int ChickenBoxY;
	int PlayerBoxX;
	int PlayerBoxY;
	int StatBoxX;
	int StatBoxY;
	int AmpBoxX;
	int AmpBoxY;

	int oldMouseX;
	int oldMouseY;
	int oldBoxX;
	int oldBoxY;

	int oldMouse2X;
	int oldMouse2Y;
	int oldBox2X;
	int oldBox2Y;

	int oldMouse3X;
	int oldMouse3Y;
	int oldBox3X;
	int oldBox3Y;

	int oldMouse4X;
	int oldMouse4Y;
	int oldBox4X;
	int oldBox4Y;

	int oldMouse5X;
	int oldMouse5Y;
	int oldBox5X;
	int oldBox5Y;

	int oldMouse6X;
	int oldMouse6Y;
	int oldBox6X;
	int oldBox6Y;

	int oldMouse7X;
	int oldMouse7Y;
	int oldBox7X;
	int oldBox7Y;


	int TargetX;
	int TargetY;

	bool ShowBoxCoordinates;
	bool ShowTabsAlways;
};

struct tBreakpoint{
	//Create Missile



	//Amazon
    int FCR1;
    int FHR1;
    int FBR1;
    int IAS1;

	//Assassin
    int FCR2;
    int FHR2;
    int FBR2;
    int IAS2;

	//Barbarian
    int FCR3;
    int FHR3;
    int FBR3;
    int IAS3;

	//Druid
    int FCR4;
    int FHR4;
    int FBR4;
    int IAS4;

	//Necromancer
    int FCR5;
    int FHR5;
    int FBR5;
    int IAS5;

	//Paladin
    int FCR6;
    int FHR6;
    int FBR6;
    int IAS6;

	//Sorceress
    int FCR7;
    int FHR7;
    int FBR7;
    int IAS7;

	};

extern tBreakpoint bp;
extern tConfig cfg;

#ifndef _CONFIG_H
#define _CONFIG_H


BOOL ReadMainConfig();
BOOL ReadBps();
BOOL ReadBoxes();
bool getBool(char* pRet);
DWORD GetKey(char* Buffer);

#endif