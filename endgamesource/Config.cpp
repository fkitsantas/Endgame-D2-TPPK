#include "Houdini.h"
tConfig cfg;
tBreakpoint bp;


BOOL bLoaded=FALSE;

BOOL ReadBoxes()
{
	
	char pFile[65535];
	char nRet[65535];
	strncpy_s(pFile, sizeof(pFile), szPath, sizeof(pFile));
	strcat_s(pFile, "\\Boxes.ini");
	
GetPrivateProfileString("Screen Hooks","Screen Hooks Toggled","true",nRet, sizeof(nRet),pFile);
	cfg.PlayerInfo=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","Clear Map With Screen Hooks","true",nRet,sizeof(nRet),pFile);
	v_ClearMap=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","Clear Console With Screen Hooks","true",nRet,sizeof(nRet),pFile);
	cfg.IncludeConsole=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","Display Target List","true",nRet, sizeof(nRet),pFile);
	cfg.ShowTargetList=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","Display Ping Tag","true",nRet, sizeof(nRet),pFile);
	cfg.IHPKTag=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","Display Game Info","true",nRet, sizeof(nRet),pFile);
	cfg.ShowInfoBox=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","Display Extra Info","true",nRet, sizeof(nRet),pFile);
	cfg.ExtraInfo=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","Display Time Stamp","true",nRet, sizeof(nRet),pFile);
	cfg.TimeStamp=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","Display Orb Percents","true",nRet, sizeof(nRet),pFile);
	cfg.ShowLifeManaPer=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","Display Orb Percents","true",nRet, sizeof(nRet),pFile);
	cfg.ShowLifeManaPer=getBool(nRet);

	
	GetPrivateProfileString("Screen Hooks","ShowTabsAlways","true",nRet, sizeof(nRet),pFile);
	cfg.ShowTabsAlways=getBool(nRet);

	GetPrivateProfileString("Screen Hooks","ShowCoordinates","false",nRet, sizeof(nRet),pFile);
	cfg.ShowCoordinates=getBool(nRet);

	GetPrivateProfileString("BoxPosition","MoveBoxes","",nRet, sizeof(nRet),pFile);
	cfg.MoveBox=GetKey(nRet);
	
	GetPrivateProfileString("BoxPosition","UtilBoxX","400",nRet, sizeof(nRet),pFile);
	cfg.UtilBoxX=atoi(nRet);

	GetPrivateProfileString("BoxPosition","UtilBoxY","100",nRet, sizeof(nRet),pFile);
	cfg.UtilBoxY=atoi(nRet);

	GetPrivateProfileString("BoxPosition","DrinkBoxX","50",nRet, sizeof(nRet),pFile);
	cfg.DrinkBoxX=atoi(nRet);

	GetPrivateProfileString("BoxPosition","DrinkBoxY","100",nRet, sizeof(nRet),pFile);
	cfg.DrinkBoxY=atoi(nRet);

	
	GetPrivateProfileString("BoxPosition","TargetBoxX","430",nRet, sizeof(nRet),pFile);
	cfg.TargetBoxX=atoi(nRet);

	GetPrivateProfileString("BoxPosition","TargetBoxY","443",nRet, sizeof(nRet),pFile);
	cfg.TargetBoxY=atoi(nRet);

	
	GetPrivateProfileString("BoxPosition","WinampBoxX","15",nRet, sizeof(nRet),pFile);
	cfg.AmpBoxX=atoi(nRet);

	
	GetPrivateProfileString("BoxPosition","WinampBoxY","335",nRet, sizeof(nRet),pFile);
	cfg.AmpBoxY=atoi(nRet);

	
	GetPrivateProfileString("BoxPosition","ChickenBoxX","215",nRet, sizeof(nRet),pFile);
	cfg.ChickenBoxX=atoi(nRet);

	
	GetPrivateProfileString("BoxPosition","ChickenBoxY","390",nRet, sizeof(nRet),pFile);
	cfg.ChickenBoxY=atoi(nRet);

	
	GetPrivateProfileString("BoxPosition","PlayerBoxX","650",nRet, sizeof(nRet),pFile);
	cfg.PlayerBoxX=atoi(nRet);

	
	GetPrivateProfileString("BoxPosition","PlayerBoxY","410",nRet, sizeof(nRet),pFile);
	cfg.PlayerBoxY=atoi(nRet);

	
	GetPrivateProfileString("BoxPosition","StatBoxX","670",nRet, sizeof(nRet),pFile);
	cfg.StatBoxX=atoi(nRet);

	
	GetPrivateProfileString("BoxPosition","StatBoxY","260",nRet, sizeof(nRet),pFile);
	cfg.StatBoxY=atoi(nRet);

	return TRUE;
}

BOOL ReadBps()
{
		
	char pFile[65535];
	char nRet[65535];
	strncpy_s(pFile, sizeof(pFile), szPath, sizeof(pFile));
	strcat_s(pFile, "\\BreakPoints.ini");


	GetPrivateProfileString("Amazon","FCR","",nRet, sizeof(nRet),pFile);
	bp.FCR1=atoi(nRet);

	GetPrivateProfileString("Amazon","FHR","",nRet, sizeof(nRet),pFile);
	bp.FHR1=atoi(nRet);

	GetPrivateProfileString("Amazon","FBR","",nRet, sizeof(nRet),pFile);
	bp.FBR1=atoi(nRet);

	GetPrivateProfileString("Amazon","IAS","",nRet, sizeof(nRet),pFile);
	bp.IAS1=atoi(nRet);

	
	GetPrivateProfileString("Assassin","FCR","",nRet, sizeof(nRet),pFile);
	bp.FCR2=atoi(nRet);

	GetPrivateProfileString("Assassin","FHR","",nRet, sizeof(nRet),pFile);
	bp.FHR2=atoi(nRet);

	GetPrivateProfileString("Assassin","FBR","",nRet, sizeof(nRet),pFile);
	bp.FBR2=atoi(nRet);

	GetPrivateProfileString("Assassin","IAS","",nRet, sizeof(nRet),pFile);
	bp.IAS2=atoi(nRet);

	
	GetPrivateProfileString("Barbarian","FCR","",nRet, sizeof(nRet),pFile);
	bp.FCR3=atoi(nRet);

	GetPrivateProfileString("Barbarian","FHR","",nRet, sizeof(nRet),pFile);
	bp.FHR3=atoi(nRet);

	GetPrivateProfileString("Barbarian","FBR","",nRet, sizeof(nRet),pFile);
	bp.FBR3=atoi(nRet);

	GetPrivateProfileString("Barbarian","IAS","",nRet, sizeof(nRet),pFile);
	bp.IAS3=atoi(nRet);

	
	
	GetPrivateProfileString("Druid","FCR","",nRet, sizeof(nRet),pFile);
	bp.FCR4=atoi(nRet);

	GetPrivateProfileString("Druid","FHR","",nRet, sizeof(nRet),pFile);
	bp.FHR4=atoi(nRet);

	GetPrivateProfileString("Druid","FBR","",nRet, sizeof(nRet),pFile);
	bp.FBR4=atoi(nRet);

	GetPrivateProfileString("Druid","IAS","",nRet, sizeof(nRet),pFile);
	bp.IAS4=atoi(nRet);

	
	GetPrivateProfileString("Necromancer","FCR","",nRet, sizeof(nRet),pFile);
	bp.FCR5=atoi(nRet);

	GetPrivateProfileString("Necromancer","FHR","",nRet, sizeof(nRet),pFile);
	bp.FHR5=atoi(nRet);

	GetPrivateProfileString("Necromancer","FBR","",nRet, sizeof(nRet),pFile);
	bp.FBR5=atoi(nRet);

	GetPrivateProfileString("Necromancer","IAS","",nRet, sizeof(nRet),pFile);
	bp.IAS5=atoi(nRet);

	
	
	GetPrivateProfileString("Paladin","FCR","",nRet, sizeof(nRet),pFile);
	bp.FCR6=atoi(nRet);

	GetPrivateProfileString("Paladin","FHR","",nRet, sizeof(nRet),pFile);
	bp.FHR6=atoi(nRet);

	GetPrivateProfileString("Paladin","FBR","",nRet, sizeof(nRet),pFile);
	bp.FBR6=atoi(nRet);

	GetPrivateProfileString("Paladin","IAS","",nRet, sizeof(nRet),pFile);
	bp.IAS6=atoi(nRet);

	
	
	GetPrivateProfileString("Sorceress","FCR","",nRet, sizeof(nRet),pFile);
	bp.FCR7=atoi(nRet);

	GetPrivateProfileString("Sorceress","FHR","",nRet, sizeof(nRet),pFile);
	bp.FHR7=atoi(nRet);

	GetPrivateProfileString("Sorceress","FBR","",nRet, sizeof(nRet),pFile);
	bp.FBR7=atoi(nRet);

	GetPrivateProfileString("Sorceress","IAS","",nRet, sizeof(nRet),pFile);
	bp.IAS7=atoi(nRet);



	return TRUE;
}

BOOL ReadMainConfig()
{	
	
	char pFile[65535];
	char nRet[65535];
	strncpy_s(pFile, sizeof(pFile), szPath, sizeof(pFile));
	strcat_s(pFile, "\\Endgame.ini");


	GetPrivateProfileString("Key Settings","Command Trigger",",",nRet, sizeof(nRet),pFile);
	cfg.Trigger=GetKey(nRet);

	GetPrivateProfileString("Key Settings","VigorCharge","",nRet, sizeof(nRet),pFile);
	cfg.VigorCharge=GetKey(nRet);

	GetPrivateProfileString("Key Settings","ConcHammer","",nRet, sizeof(nRet),pFile);
	cfg.ConcHammer=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Open Portal Key","",nRet, sizeof(nRet),pFile);
	cfg.OpenTP=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Fast Portal Key","",nRet, sizeof(nRet),pFile);
	cfg.FastTP=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Fast Portal Hostile Key","",nRet, sizeof(nRet),pFile);
	cfg.TPPK=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Window Control Key","VK_CONTROL",nRet, sizeof(nRet),pFile);
	cfg.MoveBox=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Next Target Key","",nRet, sizeof(nRet),pFile);
	cfg.NextTarget=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Last Target Key","",nRet, sizeof(nRet),pFile);
	cfg.LastTarget=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Auto Camp Key","",nRet, sizeof(nRet),pFile);
	cfg.Camp=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Auto Camp Unit Key","",nRet, sizeof(nRet),pFile);
	cfg.CampUnit=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Spy Target Inventory Key","",nRet, sizeof(nRet),pFile);
	cfg.ItemViewer=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Auto Party Toggle Key","",nRet, sizeof(nRet),pFile);
	cfg.AutoPartyToggle=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Attack Player Key","",nRet, sizeof(nRet),pFile);
	cfg.AttackPlayer=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Auto Aim Slot Toggle Key","",nRet, sizeof(nRet),pFile);
	cfg.AimSlotToggle=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Blind Spot Toggle Key","",nRet, sizeof(nRet),pFile);
	cfg.BlindAAToggle=GetKey(nRet);

	GetPrivateProfileString("Spam Message","Spam Key","",nRet, sizeof(nRet),pFile);
	cfg.ADKey=GetKey(nRet);
	
	GetPrivateProfileString("Key Settings","Open Stash Key","",nRet, sizeof(nRet),pFile);
	cfg.StashKey=GetKey(nRet);
	
	GetPrivateProfileString("Key Settings","Flash Target Key","",nRet, sizeof(nRet),pFile);
	cfg.FlashKey=GetKey(nRet);
	
	GetPrivateProfileString("Key Settings","Silent Exit Key","",nRet, sizeof(nRet),pFile);
	cfg.SilentExit=GetKey(nRet);
	
	GetPrivateProfileString("Key Settings","Hostile Key","",nRet, sizeof(nRet),pFile);
	cfg.Hostile=GetKey(nRet);
	
	GetPrivateProfileString("Key Settings","Fast Exit Key","",nRet, sizeof(nRet),pFile);
	cfg.FastExit=GetKey(nRet);
	
	GetPrivateProfileString("Key Settings","Teleport To Player Key","",nRet, sizeof(nRet),pFile);
	cfg.TeleToPlayer=GetKey(nRet);
	
	GetPrivateProfileString("Key Settings","Hostile And Teleport To Player Key","",nRet, sizeof(nRet),pFile);
	cfg.TeleToPlayerHost=GetKey(nRet);
	
	GetPrivateProfileString("Key Settings","Screen Hooks Toggle Key","",nRet, sizeof(nRet),pFile);
	cfg.PlayerInfoToggle=GetKey(nRet);

	GetPrivateProfileString("Key Settings","View Console Logs Key","",nRet, sizeof(nRet),pFile);
	cfg.ViewLogs=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Clear Console Key","",nRet, sizeof(nRet),pFile);
	cfg.ClearConsole=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Next Blind","",nRet, sizeof(nRet),pFile);
	cfg.CycleBlinds=GetKey(nRet);

	GetPrivateProfileString("Key Settings","View In Game Config Key","",nRet, sizeof(nRet),pFile);
	cfg.ShowConfig=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Attack Nearest Monster","",nRet, sizeof(nRet),pFile);
	cfg.AttackMonster=GetKey(nRet);

	GetPrivateProfileString("Key Settings","Attack Nearest Player","",nRet, sizeof(nRet),pFile);
	cfg.AttackNearest=GetKey(nRet);
	
	GetPrivateProfileString("Chicken Settings","On Hostile","0",nRet, sizeof(nRet),pFile);
	v_ChickenHostile=atoi(nRet);

	GetPrivateProfileString("Chicken Settings","On Namelock","0",nRet, sizeof(nRet),pFile);
	v_ChickenNamelock=atoi(nRet);

	GetPrivateProfileString("Chicken Settings","Town Life","0",nRet, sizeof(nRet),pFile);
	v_TownLife=atoi(nRet);

	GetPrivateProfileString("Chicken Settings","Town Mana","0",nRet, sizeof(nRet),pFile);
	v_TownMana=atoi(nRet);

	GetPrivateProfileString("Chicken Settings","Quit Life","50",nRet, sizeof(nRet),pFile);
	v_ExitLife=atoi(nRet);

	GetPrivateProfileString("Chicken Settings","Quit Mana","0",nRet, sizeof(nRet),pFile);
	v_ExitMana=atoi(nRet);

	GetPrivateProfileString("Chicken Settings","CheckSafe Rejuv","false",nRet, sizeof(nRet),pFile);
	v_SafeRejuv=getBool(nRet);

	GetPrivateProfileString("Chicken Settings","CheckSafe Town","false",nRet, sizeof(nRet),pFile);
	v_SafeTown=getBool(nRet);



	GetPrivateProfileString("Potion Settings","Use Auto Potion","true",nRet, sizeof(nRet),pFile);
	v_AutoPot=getBool(nRet);

	GetPrivateProfileString("Potion Settings","Life Potion Delay","2500",nRet, sizeof(nRet),pFile);
	v_LifePotDelay=atoi(nRet);
	GetPrivateProfileString("Potion Settings","Mana Potion Delay","2500",nRet, sizeof(nRet),pFile);
	v_ManaPotDelay=atoi(nRet);
	GetPrivateProfileString("Potion Settings","Rejuv Delay","250",nRet, sizeof(nRet),pFile);
	v_RejuvDelay=atoi(nRet);

	GetPrivateProfileString("Potion Settings","Drink Life","90",nRet, sizeof(nRet),pFile);
	v_DrinkLifePot=atoi(nRet);

	GetPrivateProfileString("Potion Settings","Drink Mana","15",nRet, sizeof(nRet),pFile);
	v_DrinkManaPot=atoi(nRet);

	GetPrivateProfileString("Potion Settings","Drink Life Rejuv","75",nRet, sizeof(nRet),pFile);
	v_DrinkLifeRej=atoi(nRet);

	GetPrivateProfileString("Potion Settings","Drink Mana Rejuv","0",nRet, sizeof(nRet),pFile);
	v_DrinkManaRej=atoi(nRet);



	
	GetPrivateProfileString("Winamp Settings","Stop Key","",nRet, sizeof(nRet),pFile);
	cfg.WStop=GetKey(nRet);
	GetPrivateProfileString("Winamp Settings","Play Key","",nRet, sizeof(nRet),pFile);
	cfg.WPlay=GetKey(nRet);
	GetPrivateProfileString("Winamp Settings","Next Song Key","",nRet, sizeof(nRet),pFile);
	cfg.WNext=GetKey(nRet);
	GetPrivateProfileString("Winamp Settings","Prev Song Key","",nRet, sizeof(nRet),pFile);
	cfg.WPrev=GetKey(nRet);
	GetPrivateProfileString("Winamp Settings","Print Title Key","",nRet, sizeof(nRet),pFile);
	cfg.WSong=GetKey(nRet);
	GetPrivateProfileString("Winamp Settings","Winamp Text","Current Track:",WinampPre, sizeof(WinampPre),pFile);

	GetPrivateProfileString("Auto Teleport Settings","Next Vector Key","",nRet, sizeof(nRet),pFile);
	cfg.NextLevelPath=GetKey(nRet);

	GetPrivateProfileString("Auto Teleport Settings","Last Vector Key","",nRet, sizeof(nRet),pFile);
	cfg.LastLevelPath=GetKey(nRet);

	GetPrivateProfileString("Auto Teleport Settings","Waypoint Vector Key","",nRet, sizeof(nRet),pFile);
	cfg.WPPath=GetKey(nRet);

	GetPrivateProfileString("Auto Teleport Settings","Special Vector Key","",nRet, sizeof(nRet),pFile);
	cfg.SpecialPath=GetKey(nRet);
	
	GetPrivateProfileString("Auto Teleport Settings","Next Vector Color","155",nRet, sizeof(nRet),pFile);
	cfg.NextColor=atoi(nRet);

	GetPrivateProfileString("Auto Teleport Settings","Last Vector Color","109",nRet, sizeof(nRet),pFile);
	cfg.PrevColor=atoi(nRet);

	GetPrivateProfileString("Auto Teleport Settings","Waypoint Vector Color","132",nRet, sizeof(nRet),pFile);
	cfg.WaypointColor=atoi(nRet);

	GetPrivateProfileString("Auto Teleport Settings","Special Vector Color","152",nRet, sizeof(nRet),pFile);
	cfg.SpecialColor=atoi(nRet);



	GetPrivateProfileString("Map Hack","Auto Reveal Map","true",nRet, sizeof(nRet),pFile);
	cfg.Maphack=getBool(nRet);

	GetPrivateProfileString("Map Hack","Normal Monster Color","98",nRet, sizeof(nRet),pFile);
	cfg.MonsterColor=atoi(nRet);

	GetPrivateProfileString("Map Hack","Champion Monster Color","151",nRet, sizeof(nRet),pFile);
	cfg.ChampionMonsterColor=atoi(nRet);

	GetPrivateProfileString("Map Hack","Boss Monster Color","155",nRet, sizeof(nRet),pFile);
	cfg.BossMonsterColor=atoi(nRet);

	GetPrivateProfileString("Map Hack","Draw Monster Blobs","true",nRet, sizeof(nRet),pFile);
	cfg.DrawMonsters=getBool(nRet);

	GetPrivateProfileString("Map Hack","Draw Monster Immunities","true",nRet, sizeof(nRet),pFile);
	cfg.DrawImunities=getBool(nRet);
	
	GetPrivateProfileString("Map Hack","Draw Missiles","true",nRet, sizeof(nRet),pFile);
	cfg.DrawMissiles=getBool(nRet);
	
	GetPrivateProfileString("Map Hack","Draw Missiles","true",nRet, sizeof(nRet),pFile);
	cfg.DrawMissiles=getBool(nRet);
	
	GetPrivateProfileString("Map Hack","Draw Level Names","true",nRet, sizeof(nRet),pFile);
	cfg.LevelNames=getBool(nRet);
	
	GetPrivateProfileString("Map Hack","Draw Teleport Vectors","true",nRet, sizeof(nRet),pFile);
	cfg.DrawVectors=getBool(nRet);
	
	GetPrivateProfileString("Map Hack","Draw Aim Vector","true",nRet, sizeof(nRet),pFile);
	cfg.PlayerLine=getBool(nRet);
	
	GetPrivateProfileString("Map Hack","Friend Missile Color","132",nRet, sizeof(nRet),pFile);
	cfg.FriendMissile=atoi(nRet);
	
	GetPrivateProfileString("Map Hack","Neutral Missile Color","153",nRet, sizeof(nRet),pFile);
	cfg.NeutralMissile=atoi(nRet);
	
	GetPrivateProfileString("Map Hack","Enemy Missile Color","98",nRet, sizeof(nRet),pFile);
	cfg.EnemyMissile=atoi(nRet);
	
	GetPrivateProfileString("Map Hack","My Missile Color","151",nRet, sizeof(nRet),pFile);
	cfg.MyMissile=atoi(nRet);



	
	GetPrivateProfileString("Enchant Bot","Enchant Bot Toggle Key","",nRet, sizeof(nRet),pFile);
	cfg.EnchBotToggle=GetKey(nRet);

	GetPrivateProfileString("Enchant Bot","Enchant Me String","",Enchant1, sizeof(Enchant1),pFile);

	GetPrivateProfileString("Enchant Bot","Enchant Merc String","",Enchant2, sizeof(Enchant2),pFile);

	GetPrivateProfileString("Enchant Bot","Enchant All String","",Enchant3, sizeof(Enchant3),pFile);

	GetPrivateProfileString("Enchant Bot","Whisper Join Messages","true",nRet, sizeof(nRet),pFile);
	cfg.PMOnJoin=getBool(nRet);

	
	GetPrivateProfileString("Messages","Print Chicken","true",nRet, sizeof(nRet),pFile);
	cfg.PrintChicken=getBool(nRet);
	
	GetPrivateProfileString("Messages","Print Commands","true",nRet, sizeof(nRet),pFile);
	cfg.PrintCommands=getBool(nRet);


	
	GetPrivateProfileString("Main Settings","Fast Teleport","false",nRet, sizeof(nRet),pFile);
	cfg.FastTele=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Speed Hack","false",nRet, sizeof(nRet),pFile);
	cfg.IncreaseCastSpeed=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Show Item Levels","false",nRet, sizeof(nRet),pFile);
	cfg.ItemLevel=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Show Item Sockets","false",nRet, sizeof(nRet),pFile);
	cfg.ItemSockets=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Show Item Ethereal","false",nRet, sizeof(nRet),pFile);
	cfg.ItemEth=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Remove Portal Delay","false",nRet, sizeof(nRet),pFile);
	v_NoPortalDelay=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Hostile Target Only","false",nRet, sizeof(nRet),pFile);
	v_HostileTarget=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Anti-Minimize","false",nRet, sizeof(nRet),pFile);
	v_NoMinimize=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Farcast Enabled","false",nRet, sizeof(nRet),pFile);
	v_FarcastEnabled=getBool(nRet);

	GetPrivateProfileString("Main Settings","Full Light Radius","false",nRet, sizeof(nRet),pFile);
	v_FullRadius=getBool(nRet);

	GetPrivateProfileString("Main Settings","Flash Delay","100",nRet, sizeof(nRet),pFile);
	v_FlashDelay=atoi(nRet);

	GetPrivateProfileString("Main Settings","Lowest Level To Log","",nRet, sizeof(nRet),pFile);
	cfg.LowestLevelToLog=atoi(nRet);
	
	GetPrivateProfileString("Death Message","Use Death Message","false",nRet, sizeof(nRet),pFile);
	v_UseDeathMessage=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Blind Spot","1",nRet, sizeof(nRet),pFile);
	v_Blind=atoi(nRet);
	
	GetPrivateProfileString("Main Settings","Spam Delay","30",nRet, sizeof(nRet),pFile);
	v_SpamDelay=atoi(nRet);
	
	GetPrivateProfileString("Main Settings","Auto Clean Screenshots","false",nRet, sizeof(nRet),pFile);
	v_CS=getBool(nRet);
	
	GetPrivateProfileString("Main Settings","Aim Box Enabled","false",nRet, sizeof(nRet),pFile);
	cfg.AimBoxEnabled=getBool(nRet);

	GetPrivateProfileString("Main Settings","Aim Box Color","155",nRet, sizeof(nRet),pFile);
	cfg.AimBoxColor=atoi(nRet);
	
	GetPrivateProfileString("Main Settings","Console Delay","6000",nRet, sizeof(nRet),pFile);
	v_ConsoleDelay=atoi(nRet);

	GetPrivateProfileString("Main Settings","OverHeadTrace","true",nRet, sizeof(nRet),pFile);
	cfg.OverHeadTrace=getBool(nRet);

	
	GetPrivateProfileString("Main Settings","StrBugFix","true",nRet, sizeof(nRet),pFile);
	cfg.StrBugFix=getBool(nRet);

	
	GetPrivateProfileString("Exra Town Options","TownOnIronMaiden","true",nRet, sizeof(nRet),pFile);
	cfg.TownOnIronMaiden=getBool(nRet);

		GetPrivateProfileString("Exra Town Options","TownOnBoOut","true",nRet, sizeof(nRet),pFile);
	cfg.TownOnBoOut=getBool(nRet);

		GetPrivateProfileString("Exra Town Options","TownOnLoweResis","true",nRet, sizeof(nRet),pFile);
	cfg.TownOnLowerResis=getBool(nRet);

		GetPrivateProfileString("Exra Town Options","TownOnAmp","true",nRet, sizeof(nRet),pFile);
	cfg.TownOnAmp=getBool(nRet);

	GetPrivateProfileString("Spam Message", "Line 1", "", Spam1, sizeof(Spam1), pFile);
	GetPrivateProfileString("Spam Message", "Line 2", "", Spam2, sizeof(Spam2), pFile);
	GetPrivateProfileString("Spam Message", "Line 3", "", Spam3, sizeof(Spam3), pFile);
	GetPrivateProfileString("Spam Message", "Line 4", "", Spam4, sizeof(Spam4), pFile);
	GetPrivateProfileString("Spam Message", "Line 5", "", Spam5, sizeof(Spam5), pFile);
	GetPrivateProfileString("Spam Message", "Line 6", "", Spam6, sizeof(Spam6), pFile);
	GetPrivateProfileString("Spam Message", "Line 7", "", Spam7, sizeof(Spam7), pFile);
	GetPrivateProfileString("Spam Message", "Line 8", "", Spam8, sizeof(Spam8), pFile);
	GetPrivateProfileString("Spam Message", "Line 9", "", Spam9, sizeof(Spam9), pFile);
	GetPrivateProfileString("Spam Message", "Line 10", "", Spam10, sizeof(Spam10), pFile);
	GetPrivateProfileString("Spam Message", "Line 11", "", Spam11, sizeof(Spam11), pFile);
	GetPrivateProfileString("Spam Message", "Line 12", "", Spam12, sizeof(Spam12), pFile);
	GetPrivateProfileString("Spam Message", "Line 13", "", Spam13, sizeof(Spam13), pFile);
	GetPrivateProfileString("Spam Message", "Line 14", "", Spam14, sizeof(Spam14), pFile);
	GetPrivateProfileString("Spam Message", "Line 15", "", Spam15, sizeof(Spam15), pFile);
	GetPrivateProfileString("Spam Message", "Line 16", "", Spam16, sizeof(Spam16), pFile);
	GetPrivateProfileString("Spam Message", "Line 17", "", Spam17, sizeof(Spam17), pFile);
	GetPrivateProfileString("Spam Message", "Line 18", "", Spam18, sizeof(Spam18), pFile);

	GetPrivateProfileString("Death Message", "Line 1", "", Line1, sizeof(Line1), pFile);
	GetPrivateProfileString("Death Message", "Line 2", "", Line2, sizeof(Line2), pFile);
	GetPrivateProfileString("Death Message", "Line 3", "", Line3, sizeof(Line3), pFile);
	GetPrivateProfileString("Death Message", "Line 4", "", Line4, sizeof(Line4), pFile);
	GetPrivateProfileString("Death Message", "Line 5", "", Line5, sizeof(Line5), pFile);
	GetPrivateProfileString("Death Message", "Line 6", "", Line6, sizeof(Line6), pFile);
	GetPrivateProfileString("Death Message", "Line 7", "", Line7, sizeof(Line7), pFile);
	GetPrivateProfileString("Death Message", "Line 8", "", Line8, sizeof(Line8), pFile);
	GetPrivateProfileString("Death Message", "Line 9", "", Line9, sizeof(Line9), pFile);
	GetPrivateProfileString("Death Message", "Line 10", "", Line10, sizeof(Line10), pFile);
	GetPrivateProfileString("Death Message", "Line 11", "", Line11, sizeof(Line11), pFile);
	GetPrivateProfileString("Death Message", "Line 12", "", Line12, sizeof(Line12), pFile);
	GetPrivateProfileString("Death Message", "Line 13", "", Line13, sizeof(Line13), pFile);
	GetPrivateProfileString("Death Message", "Line 14", "", Line14, sizeof(Line14), pFile);
	GetPrivateProfileString("Death Message", "Line 15", "", Line15, sizeof(Line15), pFile);
	GetPrivateProfileString("Death Message", "Line 16", "", Line16, sizeof(Line16), pFile);
	GetPrivateProfileString("Death Message", "Line 17", "", Line17, sizeof(Line17), pFile);
	GetPrivateProfileString("Death Message", "Line 18", "", Line18, sizeof(Line18), pFile);

//Config Errors

	if(v_Blind <1 || v_Blind>5) v_Blind=1;
	if(v_TownLife < 0 || v_TownLife > 99) v_TownLife = 0;
	if(v_ExitLife < 0 || v_ExitLife > 99) v_ExitLife = 0;
	if(v_TownMana < 0 || v_TownMana > 99) v_TownMana = 0;
	if(v_ExitMana < 0 || v_ExitMana > 99) v_ExitMana = 0;
	if(v_DrinkLifePot < 0 || v_DrinkLifePot >99) v_DrinkLifePot = 0;
	if(v_DrinkLifeRej < 0 || v_DrinkLifeRej >99) v_DrinkLifeRej = 0;
	if(v_DrinkManaPot < 0 || v_DrinkManaPot >99) v_DrinkManaPot = 0;
	if(v_DrinkManaRej < 0 || v_DrinkManaRej >99) v_DrinkManaRej = 0;

	if(v_SpamDelay < 0 || v_SpamDelay > 200) v_SpamDelay=0;

	if(cfg.ItemLevel) 
	{
		InstallItemlvl(); ItemLevel=true;
	}
	else UninstallItemlvl();

	return TRUE;
}
DWORD GetKey(char* Buffer) 
{
	if(!_strcmpi("~",Buffer)) return '~';
	if(!_strcmpi("\\",Buffer)) return '\\';
	if(!_strcmpi(";",Buffer)) return ';';
	if(!_strcmpi("`",Buffer)) return '`';
	if(!_strcmpi("´",Buffer)) return '´';
	if(!_strcmpi("'",Buffer)) return '\'';
	if(!_strcmpi("[",Buffer)) return '[';
	if(!_strcmpi("]",Buffer)) return ']';
	if(!_strcmpi("A",Buffer)) return 'A';
	if(!_strcmpi("B",Buffer)) return 'B';
	if(!_strcmpi("C",Buffer)) return 'C';
	if(!_strcmpi("D",Buffer)) return 'D';
	if(!_strcmpi("E",Buffer)) return 'E';
	if(!_strcmpi("F",Buffer)) return 'F';
	if(!_strcmpi("G",Buffer)) return 'G';
	if(!_strcmpi("H",Buffer)) return 'H';
	if(!_strcmpi("I",Buffer)) return 'I';
	if(!_strcmpi("J",Buffer)) return 'J';
	if(!_strcmpi("K",Buffer)) return 'K';
	if(!_strcmpi("L",Buffer)) return 'L';
	if(!_strcmpi("M",Buffer)) return 'M';
	if(!_strcmpi("N",Buffer)) return 'N';
	if(!_strcmpi("O",Buffer)) return 'O';
	if(!_strcmpi("P",Buffer)) return 'P';
	if(!_strcmpi("Q",Buffer)) return 'Q';
	if(!_strcmpi("R",Buffer)) return 'R';
	if(!_strcmpi("S",Buffer)) return 'S';
	if(!_strcmpi("T",Buffer)) return 'T';
	if(!_strcmpi("U",Buffer)) return 'U';
	if(!_strcmpi("V",Buffer)) return 'V';
	if(!_strcmpi("W",Buffer)) return 'W';
	if(!_strcmpi("X",Buffer)) return 'X';
	if(!_strcmpi("Y",Buffer)) return 'Y';
	if(!_strcmpi("Z",Buffer)) return 'Z';
	if(!_strcmpi("+",Buffer)) return '+';
	if(!_strcmpi("#",Buffer)) return '#';

	if(!_strcmpi("<",Buffer)) return '<';
	if(!_strcmpi(">",Buffer)) return '>';

	if(!_strcmpi("VK_0",Buffer)) return '0';
	if(!_strcmpi("VK_1",Buffer)) return '1';
	if(!_strcmpi("VK_2",Buffer)) return '2';
	if(!_strcmpi("VK_3",Buffer)) return '3';
	if(!_strcmpi("VK_4",Buffer)) return '4';
	if(!_strcmpi("VK_5",Buffer)) return '5';
	if(!_strcmpi("VK_6",Buffer)) return '6';
	if(!_strcmpi("VK_7",Buffer)) return '7';
	if(!_strcmpi("VK_8",Buffer)) return '8';
	if(!_strcmpi("VK_9",Buffer)) return '9';
	if(!_strcmpi("VK_NUMPAD0",Buffer)) return VK_NUMPAD0;
	if(!_strcmpi("VK_NUMPAD1",Buffer)) return VK_NUMPAD1;
	if(!_strcmpi("VK_NUMPAD2",Buffer)) return VK_NUMPAD2;
	if(!_strcmpi("VK_NUMPAD3",Buffer)) return VK_NUMPAD3;
	if(!_strcmpi("VK_NUMPAD4",Buffer)) return VK_NUMPAD4;
	if(!_strcmpi("VK_NUMPAD5",Buffer)) return VK_NUMPAD5;
	if(!_strcmpi("VK_NUMPAD6",Buffer)) return VK_NUMPAD6;
	if(!_strcmpi("VK_NUMPAD7",Buffer)) return VK_NUMPAD7;
	if(!_strcmpi("VK_NUMPAD8",Buffer)) return VK_NUMPAD8;
	if(!_strcmpi("VK_NUMPAD9",Buffer)) return VK_NUMPAD9;
	if(!_strcmpi("VK_F1",Buffer)) return VK_F1;
	if(!_strcmpi("VK_F2",Buffer)) return VK_F2;
	if(!_strcmpi("VK_F3",Buffer)) return VK_F3;
	if(!_strcmpi("VK_F4",Buffer)) return VK_F4;
	if(!_strcmpi("VK_F5",Buffer)) return VK_F5;
	if(!_strcmpi("VK_F6",Buffer)) return VK_F6;
	if(!_strcmpi("VK_F7",Buffer)) return VK_F7;
	if(!_strcmpi("VK_F8",Buffer)) return VK_F8;
	if(!_strcmpi("VK_F9",Buffer)) return VK_F9;
	if(!_strcmpi("VK_F10",Buffer)) return VK_F10;
	if(!_strcmpi("VK_F11",Buffer)) return VK_F11;
	if(!_strcmpi("VK_F12",Buffer)) return VK_F12;
	if(!_strcmpi("VK_HOME",Buffer)) return VK_HOME;
	if(!_strcmpi("VK_DELETE",Buffer)) return VK_DELETE;
	if(!_strcmpi("VK_PAGEDOWN",Buffer)) return VK_NEXT;
	if(!_strcmpi("VK_PAGEUP",Buffer)) return VK_PRIOR;
	if(!_strcmpi("VK_INSERT",Buffer)) return VK_INSERT;
	if(!_strcmpi("VK_END",Buffer)) return VK_END;
	if(!_strcmpi("VK_TAB",Buffer)) return VK_TAB;
	if(!_strcmpi("VK_SHIFT",Buffer)) return VK_SHIFT;
	if(!_strcmpi("VK_RETURN",Buffer)) return VK_RETURN;
	if(!_strcmpi("VK_CONTROL",Buffer)) return VK_CONTROL;
	if(!_strcmpi("VK_ESCAPE",Buffer)) return VK_ESCAPE;
	if(!_strcmpi("VK_SPACE",Buffer)) return VK_SPACE;
	if(!_strcmpi("VK_LEFT",Buffer)) return VK_LEFT;
	if(!_strcmpi("VK_UP",Buffer)) return VK_UP;
	if(!_strcmpi("VK_RIGHT",Buffer)) return VK_RIGHT;
	if(!_strcmpi("VK_DOWN",Buffer)) return VK_DOWN;
	if(!_strcmpi("VK_MULTIPLY",Buffer)) return VK_MULTIPLY;
	if(!_strcmpi("VK_ADD",Buffer)) return VK_ADD;
	if(!_strcmpi("VK_SUBTRACT",Buffer)) return VK_SUBTRACT;
	if(!_strcmpi("VK_DECIMAL",Buffer)) return VK_DECIMAL;
	if(!_strcmpi("VK_DIVIDE",Buffer)) return VK_DIVIDE;
	if(!_strcmpi("VK_ALT",Buffer)) return VK_MENU;
	if(!_strcmpi("VK_LMENU",Buffer)) return VK_LMENU;
	if(!_strcmpi("VK_RMENU",Buffer)) return VK_RMENU;
	if(!_strcmpi("VK_OEM_3",Buffer)) return VK_OEM_3;
	if(!_strcmpi("VK_PERIOD",Buffer)) return VK_OEM_PERIOD;

	if(!_strcmpi("VK_MOUSEWHEEL",Buffer)) return WM_MOUSEWHEEL;

	if(!_strcmpi(".",Buffer)) return '.';
	if(!_strcmpi(",",Buffer)) return ',';

	return -1;
}

bool getBool(char* pLower)
{
	if ((_stricmp(pLower, "1") == 0) || (_stricmp(pLower, "y") == 0) || (_stricmp(pLower, "yes") == 0) || (_stricmp(pLower, "true") == 0))
		return true;
	return false;
}