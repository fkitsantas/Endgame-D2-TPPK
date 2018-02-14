#include "Houdini.h"

tBoxes box;

bool MovePlayer=false; bool MoveDrink=false; bool MoveTarget=false;
bool MoveChicken=false; bool MoveAmp=false; bool MoveStat=false; bool MoveUtil=false;

bool PlayerToggled; bool DrinkToggled; bool TargetToggled;
bool ChickenToggled; bool WinampToggled; bool StatToggled; bool UtilToggled;

int v_Color = 8; int v_Trans = 1.6; int v_TextFont = 4;
int v_TextColor = 0; int v_TextColor2 = 0;

StatBox sInfo[]=
{
	{STAT_FASTERCAST,"Cast Rate",0,0},
	{STAT_FASTERBLOCK,"Block Rate",0,0},
	{STAT_FASTERHITRECOVERY,"Hit Recovery",0,0},
	{STAT_IAS,"Attack Speed",0,0},
	{STAT_FASTERRUNWALK,"Run/Walk",0,0},
	{STAT_FIRERESIST,"Fire Resist",1,1},
	{STAT_COLDRESIST,"Cold Resist",3,3},
	{STAT_LIGHTNINGRESIST,"Light Resist",9,9},
	{STAT_POISONRESIST,"Poison Resist",2,2},
	{STAT_DAMAGERESIST,"Damage Resist",5,5},
	{STAT_MAGICFIND,"Magic Find",8,8},
	{STAT_GOLDFIND,"Gold Find",8,8},
};


void TabbedWindows(void)
{
	char Buf[200];
	int TextColor=4;
	int x=480; int x2;
	int y=523; int y2=y+17; int y3=y2-3.5;

	if(!cfg.PlayerInfo)
		return;

	if(!cfg.ShowTabsAlways)
	{
		if(!cfg.MoveBoxes)
		{
			return;
		}
		else if(cfg.MoveBoxes)
		{
			if(!(D2CLIENT_MouseY >= y -5 && D2CLIENT_MouseY <= y2 +5 &&
				D2CLIENT_MouseX >= x-5 && D2CLIENT_MouseX <= x+275 +5))
			{
				return;
			}
		}
	}

	sprintf(Buf,"TPPK");
	x2 = GetTextLength(Buf,6) + 5;
	if(!TargetToggled)
	{
		DrawRectangle(x,y,x+x2,y2,v_Color,v_Trans);
		DrawTextToScreenCenter(Buf,x+x2/2,y3,TextColor,v_TextFont);
	}
	box.Tab1X=x;
	box.Tab1Y=y;
	box.Tab1X2=x+x2;
	box.Tab1Y2=y2;

	x=x+x2+5;

	sprintf(Buf,"Chicken");
	x2 = GetTextLength(Buf,6) + 5;
	if(!ChickenToggled)
	{
		DrawRectangle(x,y,x+x2,y2,v_Color,v_Trans);
		DrawTextToScreenCenter(Buf,x+x2/2,y3,TextColor,v_TextFont);
	}
	box.Tab2X=x;
	box.Tab2Y=y;
	box.Tab2X2=x+x2;
	box.Tab2Y2=y2;

	x=x+x2+5;

	sprintf(Buf,"Drink");
	x2 = GetTextLength(Buf,6) + 5;
	if(!DrinkToggled)
	{
		DrawRectangle(x,y,x+x2,y2,v_Color,v_Trans);
		DrawTextToScreenCenter(Buf,x+x2/2,y3,TextColor,v_TextFont);
	}
	box.Tab3X=x;
	box.Tab3Y=y;
	box.Tab3X2=x+x2;
	box.Tab3Y2=y2;

	x=x+x2+5;
	sprintf(Buf,"Winamp");
	x2 = GetTextLength(Buf,6) + 5;
	if(!WinampToggled)
	{
		DrawRectangle(x,y,x+x2,y2,v_Color,v_Trans);
		DrawTextToScreenCenter(Buf,x+x2/2,y3,TextColor,v_TextFont);
	}
	box.Tab4X=x;
	box.Tab4Y=y;
	box.Tab4X2=x+x2;
	box.Tab4Y2=y2;

	x=x+x2+5;
	sprintf(Buf,"Stat");
	x2 = GetTextLength(Buf,6) + 5;
	if(!StatToggled)
	{
		DrawRectangle(x,y,x+x2,y2,v_Color,v_Trans);
		DrawTextToScreenCenter(Buf,x+x2/2,y3,TextColor,v_TextFont);
	}
	box.Tab5X=x;
	box.Tab5Y=y;
	box.Tab5X2=x+x2;
	box.Tab5Y2=y2;

	
	x=x+x2+5;
	/*
	sprintf(Buf,"Config");
	x2 = GetTextLength(Buf,6) + 5;
	if(!UtilToggled)
	{
		DrawRectangle(x,y,x+x2,y2,v_Color,v_Trans);
		DrawTextToScreenCenter(Buf,x+x2/2,y3,TextColor,v_TextFont);
	}
	box.Tab6X=x;
	box.Tab6Y=y;
	box.Tab6X2=x+x2;
	box.Tab6Y2=y2;

	x=x+x2+5;
	*/
}

/*void UtilWindow(void)
{
	char Buf[200];


	//Defining the box variables
	int BoxX=cfg.UtilBoxX;
	int BoxY=cfg.UtilBoxY;
	int xLen=145;
	int yLen=67;

	int x=BoxX;
	int y=BoxY;
	int x2=x+xLen;
	int y2=y+yLen;
	int xMax=799-xLen;
	int yMax=551-yLen;

	box.UtilX=x;
	box.UtilX2=x2;
	box.UtilY=y;
	box.UtilY2=y2;

	//Introducing the mouse variables
	int oldMouseX;
	int oldMouseY;
	int deltaMouseX;
	int deltaMouseY;
	int oldBoxX;
	int oldBoxY;
	int newBoxX;
	int newBoxY;
	
	//Keeping the box from moving off screen...	
	if(cfg.UtilBoxX < 0) cfg.UtilBoxX = 0;
	if(cfg.UtilBoxX > xMax) cfg.UtilBoxX = xMax;
	if(cfg.UtilBoxY < 0) cfg.UtilBoxY = 0;
	if(cfg.UtilBoxY > yMax) cfg.UtilBoxY = yMax;

	//Showing the box's x and y in order to save them to config
	if(cfg.ShowBoxCoordinates && cfg.MoveBoxes)
	{
		if(D2CLIENT_MouseX >= x && D2CLIENT_MouseX <= x2 && D2CLIENT_MouseY >= y && D2CLIENT_MouseY <= y2)
		{
			sprintf(Buf,"[%d, %d]",x,y);
			DrawTextToScreenCenter(Buf,(x+x2)/2,y2+10,8,6);
		}
	}
	//Grabbing the old mouse position while box is stationary
	if(!MoveUtil)
	{
		cfg.oldMouse7X=D2CLIENT_MouseX;
		cfg.oldMouse7Y=D2CLIENT_MouseY;
		cfg.oldBox7X=x;
		cfg.oldBox7Y=y;
	}
	//The equation to move the box successfully
	if(MoveUtil)
	{
		oldMouseX=cfg.oldMouse7X;
		oldMouseY=cfg.oldMouse7Y;
		oldBoxX=cfg.oldBox7X;
		oldBoxY=cfg.oldBox7Y;

		deltaMouseX=D2CLIENT_MouseX - oldMouseX;
		deltaMouseY=D2CLIENT_MouseY - oldMouseY;

		newBoxX=oldBoxX + deltaMouseX;
		newBoxY=oldBoxY + deltaMouseY;

		cfg.UtilBoxX=newBoxX;
		cfg.UtilBoxY=newBoxY;

		if(cfg.UtilBoxX < 0) cfg.UtilBoxX = 0;
		if(cfg.UtilBoxX > xMax) cfg.UtilBoxX = xMax;
		if(cfg.UtilBoxY < 0) cfg.UtilBoxY = 0;
		if(cfg.UtilBoxY > yMax) cfg.UtilBoxY = yMax;
	}

	//Draw the box
	DrawRectangle(x-11,y-11,x2+1,y2+125,v_Color,v_Trans);

	//Drawing the text now...
	x=(x+x2)/2;
	y=y+12;
	DrawTextToScreenCenter("Config Settings",x,y,v_TextColor2,v_TextFont);

	y=y+20;

	if(cfg.Drink)
		DrawTextToScreenCenter("Auto Potion: ÿc2ON",x,y,v_TextColor,v_TextFont);
	else
		DrawTextToScreenCenter("Auto Potion: ÿc1OFF",x,y,v_TextColor,v_TextFont);
	
	y=y+13;
	if (AA)
		DrawTextToScreenCenter("AutoAim: ÿc2ON",x,y,v_TextColor2,v_TextFont);
	else
	DrawTextToScreenCenter("AutoAim: ÿc1OFF",x,y,v_TextColor2,v_TextFont);

y=y+13;

	if(AA && LeftAA)
		DrawTextToScreenCenter("AimClick: ÿc2Left!",x,y,v_TextColor2,v_TextFont);
		else if(AA && !LeftAA)
		DrawTextToScreenCenter("AimClick: ÿc2Right!",x,y,v_TextColor2,v_TextFont);

		y=y+13;
		
		if (cfg.OverHeadTrace)
		DrawTextToScreenCenter("OverHeadTrace: ÿc2ON",x,y,v_TextColor2,v_TextFont);
		else 
			DrawTextToScreenCenter("OverHeadTrace: ÿc1OFF",x,y,v_TextColor2,v_TextFont);

		y=y+13;

		if (cfg.StrBugFix)
			DrawTextToScreenCenter("StrBugFix: ÿc2ON",x,y,v_TextColor2,v_TextFont);
		else
			DrawTextToScreenCenter("StrBugFix: ÿc1OFF",x,y,v_TextColor2,v_TextFont);


		y=y+13;

		if (cfg.v_CS)

		DrawTextToScreenCenter("CleanShot: ÿc2ON",x,y,v_TextColor2,v_TextFont);
		else
			DrawTextToScreenCenter("CleanShot: ÿc1OFF",x,y,v_TextColor2,v_TextFont);

		y=y+13;

		if (cfg.TownIfBoOut)
			DrawTextToScreenCenter("TownOnBoOut: ÿc2ON!",x,y,v_TextColor2,v_TextFont);
		else
			DrawTextToScreenCenter("TownOnBoOut: ÿc1OFF!",x,y,v_TextColor2,v_TextFont);

		y=y+13;

		if (cfg.TownOnAmp)
			DrawTextToScreenCenter("TownOnAmp: ÿc2ON!",x,y,v_TextColor2,v_TextFont);
		else
			DrawTextToScreenCenter("TownOnAmp: ÿc1OFF!",x,y,v_TextColor2,v_TextFont);

		y=y+13;

		if (cfg.TownOnIronMaiden)
			DrawTextToScreenCenter("TownOnIronMaiden: ÿc2ON!",x,y,v_TextColor2,v_TextFont);
		else
			DrawTextToScreenCenter("TownOnIronMaiden: ÿc1OFF!",x,y,v_TextColor2,v_TextFont);

y=y+13;

		if (cfg.TownOnLowerResis)
			DrawTextToScreenCenter("TownOnLowerResis: ÿc2ON!",x,y,v_TextColor2,v_TextFont);
		else
			DrawTextToScreenCenter("TownOnLowerResis: ÿc1OFF!",x,y,v_TextColor2,v_TextFont);

y=y+13;

		if (cfg.TownIfBoOut)
			DrawTextToScreenCenter("TownOnBoOut: ÿc2ON!",x,y,v_TextColor2,v_TextFont);
		else
			DrawTextToScreenCenter("TownOnBoOut: ÿc1OFF!",x,y,v_TextColor2,v_TextFont);

		y=y+13;

		if (cfg.PKMessage)
			DrawTextToScreenCenter("DeathMessage: ÿc2ON!",x,y,v_TextColor2,v_TextFont);
		else
			DrawTextToScreenCenter("DeathMessage: ÿc1OFF!",x,y,v_TextColor2,v_TextFont);

		y=y+13;
		
		//	DrawTextToScreenCenter("LowestLvlToLog: ÿc2%i!",x,y,v_TextColor2,v_TextFont,cfg.LowestLevelToLog);

	
}
*/

void DrinkWindow(void)
{
	char Buf[200];


	//Defining the box variables
	int BoxX=cfg.DrinkBoxX;
	int BoxY=cfg.DrinkBoxY;
	int xLen=145;
	int yLen=67;

	int x=BoxX;
	int y=BoxY;
	int x2=x+xLen;
	int y2=y+yLen;
	int xMax=799-xLen;
	int yMax=551-yLen;

	box.DrinkX=x;
	box.DrinkX2=x2;
	box.DrinkY=y;
	box.DrinkY2=y2;

	//Introducing the mouse variables
	int oldMouseX;
	int oldMouseY;
	int deltaMouseX;
	int deltaMouseY;
	int oldBoxX;
	int oldBoxY;
	int newBoxX;
	int newBoxY;
	
	//Keeping the box from moving off screen...	
	if(cfg.DrinkBoxX < 0) cfg.DrinkBoxX = 0;
	if(cfg.DrinkBoxX > xMax) cfg.DrinkBoxX = xMax;
	if(cfg.DrinkBoxY < 0) cfg.DrinkBoxY = 0;
	if(cfg.DrinkBoxY > yMax) cfg.DrinkBoxY = yMax;

	//Showing the box's x and y in order to save them to config
	if(cfg.ShowBoxCoordinates && cfg.MoveBoxes)
	{
		if(D2CLIENT_MouseX >= x && D2CLIENT_MouseX <= x2 && D2CLIENT_MouseY >= y && D2CLIENT_MouseY <= y2)
		{
			sprintf(Buf,"[%d, %d]",x,y);
			DrawTextToScreenCenter(Buf,(x+x2)/2,y2+10,8,6);
		}
	}
	//Grabbing the old mouse position while box is stationary
	if(!MoveDrink)
	{
		cfg.oldMouse3X=D2CLIENT_MouseX;
		cfg.oldMouse3Y=D2CLIENT_MouseY;
		cfg.oldBox3X=x;
		cfg.oldBox3Y=y;
	}
	//The equation to move the box successfully
	if(MoveDrink)
	{
		oldMouseX=cfg.oldMouse3X;
		oldMouseY=cfg.oldMouse3Y;
		oldBoxX=cfg.oldBox3X;
		oldBoxY=cfg.oldBox3Y;

		deltaMouseX=D2CLIENT_MouseX - oldMouseX;
		deltaMouseY=D2CLIENT_MouseY - oldMouseY;

		newBoxX=oldBoxX + deltaMouseX;
		newBoxY=oldBoxY + deltaMouseY;

		cfg.DrinkBoxX=newBoxX;
		cfg.DrinkBoxY=newBoxY;

		if(cfg.DrinkBoxX < 0) cfg.DrinkBoxX = 0;
		if(cfg.DrinkBoxX > xMax) cfg.DrinkBoxX = xMax;
		if(cfg.DrinkBoxY < 0) cfg.DrinkBoxY = 0;
		if(cfg.DrinkBoxY > yMax) cfg.DrinkBoxY = yMax;
	}

	//Draw the box
	DrawRectangle(x,y,x2,y2,v_Color,v_Trans);

	//Drawing the text now...
	x=(x+x2)/2;
	y=y+12;
	DrawTextToScreenCenter("Drink Settings",x,y,v_TextColor2,v_TextFont);

	y=y+13;
	if(v_AutoPot)
		DrawTextToScreenCenter("Auto Potion: On",x,y,v_TextColor,v_TextFont);
	else
		DrawTextToScreenCenter("Auto Potion: Off",x,y,v_TextColor,v_TextFont);
	
	y=y+10;
	sprintf(Buf,"Auto Drink (Life): %d",v_DrinkLifePot);
	DrawTextToScreenCenter(Buf,x,y,v_TextColor,v_TextFont);

	y=y+10;
	sprintf(Buf,"Auto Drink (LifeRej): %d",v_DrinkLifeRej);
	DrawTextToScreenCenter(Buf,x,y,v_TextColor,v_TextFont);

	y=y+10;
	sprintf(Buf,"Auto Drink (Mana): %d",v_DrinkManaPot);
	DrawTextToScreenCenter(Buf,x,y,v_TextColor,v_TextFont);

	y=y+10;
	sprintf(Buf,"Auto Drink (ManaRej): %d",v_DrinkManaRej);
	DrawTextToScreenCenter(Buf,x,y,v_TextColor,v_TextFont);
	
	y=y+10;

}


void TargetWindow(void)
{
	RosterUnit *pUnit=(RosterUnit*)*(DWORD*)PlayerRoster;
	RosterUnit *rs=(RosterUnit*)*(DWORD*)PlayerRoster;
	UnitAny* Me=(UnitAny*)D2CLIENT_GetPlayerUnit();
	char Buf[200];
	char Buf2[200];
	char Buffer[200];
	int x=cfg.TargetBoxX;
	int x2;
	int y=cfg.TargetBoxY;
	int y2=y;
	int yLen;
	int Farbe;
	rs=rs->pNext;
	int xLen=GetTextLength("Current Target",6);		
	int xLen2=GetTextLength("Target List",6);
	while(rs!=NULL)
	{
		sprintf(Buf,"%s",rs->szName);
		if(GetTextLength(Buf,6) > xLen2)
		{
			xLen2=GetTextLength(Buf,6);
		}
		else
			xLen2=xLen2;
		rs=rs->pNext;		
	}	

	int xMax=799-xLen-10-xLen2-18;
	int centerx1=x+((xLen)/2);
	int centerx2=x+xLen+10+((xLen2)/2)+9;
	pUnit=pUnit->pNext;

	int PlayNum=Players()-1;
	if(PlayNum==0)
		yLen=27;
	else
		yLen=17+(10*PlayNum);

	int oldMouseX;
	int oldMouseY;
	int deltaMouseX;
	int deltaMouseY;
	int oldBoxX;
	int oldBoxY;
	int newBoxX;
	int newBoxY;

	if(cfg.TargetBoxX < 0) cfg.TargetBoxX = 0;
	if(cfg.TargetBoxX > xMax) cfg.TargetBoxX = xMax;
	if(cfg.TargetBoxY < 0) cfg.TargetBoxY = 0;

	if(!MoveTarget)
	{
		cfg.oldMouseX=D2CLIENT_MouseX;
		cfg.oldMouseY=D2CLIENT_MouseY;
		cfg.oldBoxX=x;
		cfg.oldBoxY=y;
	}
	if(MoveTarget)
	{
		oldMouseX=cfg.oldMouseX;
		oldMouseY=cfg.oldMouseY;
		oldBoxX=cfg.oldBoxX;
		oldBoxY=cfg.oldBoxY;
		deltaMouseX=D2CLIENT_MouseX - oldMouseX;
		deltaMouseY=D2CLIENT_MouseY - oldMouseY;
		newBoxX=oldBoxX + deltaMouseX;
		newBoxY=oldBoxY + deltaMouseY;
		cfg.TargetBoxX=newBoxX;
		cfg.TargetBoxY=newBoxY;

		if(cfg.TargetBoxX < 0) cfg.TargetBoxX = 0;
		if(cfg.TargetBoxX > xMax) cfg.TargetBoxX = xMax;
		if(cfg.TargetBoxY < 0) cfg.TargetBoxY = 0;
	}
	if(cfg.ShowBoxCoordinates && cfg.MoveBoxes)
	{
		if(D2CLIENT_MouseX >= box.CurrentTargetX && D2CLIENT_MouseX <= box.CurrentTargetX2 && D2CLIENT_MouseY >= box.CurrentTargetY && D2CLIENT_MouseY <= box.CurrentTargetY2)
		{
			TextHook(centerx1,box.CurrentTargetY2+10,8,0,6,1,"[%d, %d]",cfg.TargetBoxX,cfg.TargetBoxY);	
		}
	}

	int longx=x+xLen+9+10;

	if(pUnit==NULL) 
	{
		DrawRectangle(x,y,x+xLen+9,y+27,v_Color,v_Trans);
		TextHook(centerx1,y2+12,v_TextColor2,0,v_TextFont,1,"Current Target");	
		TextHook(centerx1,y2+25,v_TextColor2,0,v_TextFont,1,"- None -");

		DrawRectangle(longx,y,longx+xLen2+9,y+27,v_Color,v_Trans);
		TextHook(centerx2,y+12,v_TextColor2,0,v_TextFont,1,"Target List");
		TextHook(centerx2,y+25,v_TextColor2,0,v_TextFont,1,"- None -");

		box.CurrentTargetX=x;
		box.CurrentTargetX2=x+xLen+9;
		box.CurrentTargetY=y;
		box.CurrentTargetY2=y+27;

		box.TargetX=x+xLen+9+10;
		box.TargetX2=x+xLen+9+10+xLen2+9;
		box.TargetY=y;
		box.TargetY2=y+27;
	}
	else
	{
		if(Auto.GetPlayerID()==Me->dwUnitId || Auto.GetPlayerID()==NULL || GetNameByID(Auto.GetPlayerID()) == "UNKNOWN")
		{
			DrawRectangle(x,y,x+xLen+9,y+27,v_Color,v_Trans);
			TextHook(centerx1,y2+12,v_TextColor2,0,v_TextFont,1,"Current Target");	
			TextHook(centerx1,y2+25,v_TextColor2,0,v_TextFont,1,"- None -");

			box.CurrentTargetX=x;
			box.CurrentTargetX2=x+xLen+9;
			box.CurrentTargetY=y;
			box.CurrentTargetY2=y+27;
		}
		else
		{
			char PlayerName[200];
			char ClassName[200];
			int xDif;
			sprintf(PlayerName,"%s",GetNameByID(Auto.GetPlayerID()));
			
			if(GetTextLength(PlayerName,6) > xLen)
			{
				xDif=GetTextLength(PlayerName,6) - xLen;
				x=x-xDif;
				xLen=GetTextLength(PlayerName,6);
			}

			sprintf(ClassName,"Class: %s",GetD2ClassName(Auto.GetPlayerCLASS()));

			if(GetTextLength(ClassName,6) > xLen)
			{
				xDif=GetTextLength(ClassName,6) - xLen;
				x=x-xDif;
				xLen=GetTextLength(ClassName,6);
			}

			centerx1=x+((xLen)/2);

			DrawRectangle(x,y,x+xLen+9,y+47,v_Color,v_Trans);
			TextHook(centerx1,y2+12,v_TextColor2,0,v_TextFont,1,"Current Target");	

			box.CurrentTargetX=x;
			box.CurrentTargetX2=x+xLen+9;
			box.CurrentTargetY=y;
			box.CurrentTargetY2=y+47;
		}

		longx=x+xLen+9+10;
		centerx2=x+xLen+10+((xLen2)/2)+9;

		DrawRectangle(longx,y,longx+xLen2+9,y+yLen,v_Color,v_Trans);
		TextHook(centerx2,y+12,v_TextColor2,0,v_TextFont,1,"Target List");

		box.TargetX=x+xLen+9+10;
		box.TargetX2=x+xLen+9+10+xLen2+9;
		box.TargetY=y;
		box.TargetY2=y+yLen;

		while(pUnit!=NULL) 
		{
			if(pUnit->dwUnitId==Auto.GetPlayerID()) 
				Farbe=4;
			else
				Farbe=0;

			if(pUnit->dwUnitId==Auto.GetPlayerID())
			{
				TextHook(centerx2,y+25,Farbe,0,v_TextFont,1,"%s",pUnit->szName);
				TextHook(centerx1,y2+12,v_TextColor2,0,v_TextFont,1,"Current Target");
				TextHook(centerx1,y2+25,v_TextColor,0,v_TextFont,1,"%s",pUnit->szName);
				TextHook(centerx1,y2+35,v_TextColor,0,v_TextFont,1,"Level: %s",mitoa(pUnit->wLevel));
				TextHook(centerx1,y2+45,v_TextColor,0,v_TextFont,1,"Class: %s",GetD2ClassName(pUnit->dwClassId));
			}
			else
			{		
				TextHook(centerx2,y+25,Farbe,0,v_TextFont,1,"%s",pUnit->szName);
			}	

			y+=10;
			pUnit=pUnit->pNext;
		}
	}
}
void ChickenWindow(void)
{
	char Buf[200];
	char Buffer[200];

	//Defining the box variables
	int BoxX=cfg.ChickenBoxX;
	int BoxY=cfg.ChickenBoxY;
	int xLen=140;
	int yLen=77;

	int x=BoxX;
	int y=BoxY;
	int x2=x+xLen;
	int y2=y+yLen;
	int xMax=799-xLen;
	int yMax=551-yLen;

	box.ChickenX=x;
	box.ChickenX2=x2;
	box.ChickenY=y;
	box.ChickenY2=y2;

	//Introducing the mouse variables
	int oldMouseX;
	int oldMouseY;
	int deltaMouseX;
	int deltaMouseY;
	int oldBoxX;
	int oldBoxY;
	int newBoxX;
	int newBoxY;
	
	//Keeping the box from moving off screen...
	if(cfg.ChickenBoxX < 0) cfg.ChickenBoxX = 0;
	if(cfg.ChickenBoxX > xMax) cfg.ChickenBoxX = xMax;
	if(cfg.ChickenBoxY < 0) cfg.ChickenBoxY = 0;
	if(cfg.ChickenBoxY > yMax) cfg.ChickenBoxY = yMax;

	//Showing the box's x and y in order to save them to config
	if(cfg.ShowBoxCoordinates && cfg.MoveBoxes)
	{
		if(D2CLIENT_MouseX >= x && D2CLIENT_MouseX <= x2 && D2CLIENT_MouseY >= y && D2CLIENT_MouseY <= y2)
		{
			sprintf(Buf,"[%d, %d]",x,y);
			DrawTextToScreenCenter(Buf,(x+x2)/2,y2+10,8,6);
		}
	}

	if(!MoveChicken)
	{
		cfg.oldMouse4X=D2CLIENT_MouseX;
		cfg.oldMouse4Y=D2CLIENT_MouseY;
		cfg.oldBox4X=x;
		cfg.oldBox4Y=y;
	}

	if(MoveChicken)
	{
		oldMouseX=cfg.oldMouse4X;
		oldMouseY=cfg.oldMouse4Y;
		oldBoxX=cfg.oldBox4X;
		oldBoxY=cfg.oldBox4Y;

		deltaMouseX=D2CLIENT_MouseX - oldMouseX;
		deltaMouseY=D2CLIENT_MouseY - oldMouseY;

		newBoxX=oldBoxX + deltaMouseX;
		newBoxY=oldBoxY + deltaMouseY;

		cfg.ChickenBoxX=newBoxX;
		cfg.ChickenBoxY=newBoxY;

		if(cfg.ChickenBoxX < 0) cfg.ChickenBoxX = 0;
		if(cfg.ChickenBoxX > xMax) cfg.ChickenBoxX = xMax;
		if(cfg.ChickenBoxY < 0) cfg.ChickenBoxY = 0;
		if(cfg.ChickenBoxY > yMax) cfg.ChickenBoxY = yMax;
	}
	
	//Draw the box
	DrawRectangle(x,y,x2,y2,v_Color,v_Trans);

	//Drawing the text now...
	x=(x+x2)/2;
	y=y+12;

	DrawTextToScreenCenter("Chicken Settings",x,y,v_TextColor2,v_TextFont);

	y=y+13;
	if(v_SafeTown)
	DrawTextToScreenCenter("CheckSafe (Town): On",x,y,v_TextColor,v_TextFont);
	else
	DrawTextToScreenCenter("CheckSafe (Town): Off",x,y,v_TextColor,v_TextFont);
	
	y=y+10;
	if(v_SafeRejuv)
	DrawTextToScreenCenter("CheckSafe (Rejuv): On",x,y,v_TextColor,v_TextFont);
	else
	DrawTextToScreenCenter("CheckSafe (Rejuv): Off",x,y,v_TextColor,v_TextFont);
	
	y=y+10;
	sprintf(Buf,"Chicken Town (Life): %d",v_TownLife);
	DrawTextToScreenCenter(Buf,x,y,v_TextColor,v_TextFont);

	y=y+10;
	sprintf(Buf,"Chicken Quit (Life): %d",v_ExitLife);
	DrawTextToScreenCenter(Buf,x,y,v_TextColor,v_TextFont);

	y=y+10;
	sprintf(Buf,"Chicken Town (Mana): %d",v_TownMana);
	DrawTextToScreenCenter(Buf,x,y,v_TextColor,v_TextFont);

	y=y+10;
	sprintf(Buf,"Chicken Quit (Mana): %d",v_ExitMana);
	DrawTextToScreenCenter(Buf,x,y,v_TextColor,v_TextFont);


	y=y+10;
}

void StatWindow(DWORD UnitID)
{
	char Buf[200];
	int x=cfg.StatBoxX;
	int y=cfg.StatBoxY;
	int totalx=115;
	int totaly=10;
	int x2=x+totalx;
	int y2=y+127;
	int xMax=799-totalx;

	int oldMouseX;
	int oldMouseY;
	int deltaMouseX;
	int deltaMouseY;
	int oldBoxX;
	int oldBoxY;
	int newBoxX;
	int newBoxY;

	if(cfg.ShowBoxCoordinates && cfg.MoveBoxes)
	{
		if(D2CLIENT_MouseX >= x && D2CLIENT_MouseX <= x2 && D2CLIENT_MouseY >= cfg.StatBoxY && D2CLIENT_MouseY <= y2)
		{
			sprintf(Buf,"[%d, %d]",cfg.StatBoxX,cfg.StatBoxY);
			DrawTextToScreenCenter(Buf,x+(125/2),y+146,8,6);
		}
	}

	if(cfg.StatBoxX < 0) cfg.StatBoxX = 0;
	if(cfg.StatBoxX > xMax) cfg.StatBoxX = xMax;
	if(cfg.StatBoxY < 0) cfg.StatBoxY = 0;
	if(cfg.StatBoxY > 435) cfg.StatBoxY = 435;

	if(!MoveStat)
	{
		cfg.oldMouse5X=D2CLIENT_MouseX;
		cfg.oldMouse5Y=D2CLIENT_MouseY;
		cfg.oldBox5X=x;
		cfg.oldBox5Y=y;
	}

	if(MoveStat)
	{
		oldMouseX=cfg.oldMouse5X;
		oldMouseY=cfg.oldMouse5Y;
		oldBoxX=cfg.oldBox5X;
		oldBoxY=cfg.oldBox5Y;
		deltaMouseX=D2CLIENT_MouseX - oldMouseX;
		deltaMouseY=D2CLIENT_MouseY - oldMouseY;
		newBoxX=oldBoxX + deltaMouseX;
		newBoxY=oldBoxY + deltaMouseY;
		cfg.StatBoxX=newBoxX;
		cfg.StatBoxY=newBoxY;

		if(cfg.StatBoxX < 0) cfg.StatBoxX = 0;
		if(cfg.StatBoxX > xMax) cfg.StatBoxX = xMax;
		if(cfg.StatBoxY < 0) cfg.StatBoxY = 0;
		if(cfg.StatBoxY > 435) cfg.StatBoxY = 435;
	}

	UnitAny* pUnit=(UnitAny*)FindUnit(GetPlayerID(),UNIT_PLAYER);

	for(int i=0;i<ARRAYSIZE(sInfo);i++)
	totaly+=10;

	DrawRectangle(x,y,x+totalx,y+totaly+6,v_Color,v_Trans);

	box.StatX=x;
	box.StatX2=x+totalx;
	box.StatY=y;
	box.StatY2=y+totaly+6;

	DrawTextToScreenCenter("Stat Box",x+(totalx/2),y+12,v_TextColor2,v_TextFont);
	y+=13;
	for(int i=0;i<ARRAYSIZE(sInfo);i++)
	{
		y=y+10;
		char Buffer[200];
		int value=D2COMMON_GetUnitStat((UnitAny*)pUnit,sInfo[i].Statnumber,0);
		if(sInfo[i].shift)
		value=value>>8;
		sprintf(Buffer,"ÿc%i%s: %d",sInfo[i].StatColor,sInfo[i].StatInfo,value);
		DrawTextToScreenCenter(Buffer,x+(totalx/2)+10,y,v_TextColor,v_TextFont);	
	}

}





void WinampWindow(void) 
{ 
	HWND win = FindWindowA("Winamp v1.x",NULL);
	char Buffer[200];
	char Buf[200];
	int ct = GetCurrentTrackTime(); 
	int mt = GetMaxTrackTime(); 
	int x=cfg.AmpBoxX;
	int y=cfg.AmpBoxY;
	int oldMouseX;
	int oldMouseY;
	int deltaMouseX;
	int deltaMouseY;
	int oldBoxX;
	int oldBoxY;
	int newBoxX;
	int newBoxY;
	if((mt)%60 == -1)
	{
		sprintf(Buffer,"<%02d:%02d:%02d>",ct/(1000*60*60),ct/(1000*60)%60, (ct/1000)%60);					
	}
	else
	{
		sprintf(Buffer,"<%02d:%02d:%02d/%02d:%02d:%02d>",ct/(1000*60*60),ct/(1000*60)%60, (ct/1000)%60,mt/(60*60),mt/(60)%60, (mt)%60);
	}
	int xplus=GetTextLength(GetCurrentWinampTrack(),6);
	if(xplus < 105) xplus=105;

	//Keeping the box from leaving screen
	if(cfg.AmpBoxX < 0) cfg.AmpBoxX = 0;
	if((cfg.AmpBoxX+xplus+16) > 798) cfg.AmpBoxX = 798-xplus-16;
	if(cfg.AmpBoxY < 0) cfg.AmpBoxY = 0;
	if(cfg.AmpBoxY > 515) cfg.AmpBoxY = 515;

	//Drawing the box
	if(win)
	{
		DrawRectangle(x,y,x+xplus+12,y+37,v_Color,v_Trans);
	}
	else
	{
		xplus=65;
		DrawRectangle(x,y,x+xplus+12,y+27,v_Color,v_Trans);
	}

	//Adding the text
	DrawTextToScreenCenter("Winamp",x+(xplus/2)+6,y+12,v_TextColor2,v_TextFont);
	WinampHook(x+(xplus/2)+3,y+25,v_TextColor,v_TextFont,1,"%s",GetCurrentWinampTrack());
	if(win)
	DrawTextToScreenCenter(Buffer,x+(xplus/2)+6,y+35,v_TextColor,v_TextFont);

	if(cfg.ShowBoxCoordinates && cfg.MoveBoxes)
	{
		if(D2CLIENT_MouseX >= x && D2CLIENT_MouseX <= x+xplus+16 && D2CLIENT_MouseY >= y && D2CLIENT_MouseY <= y+36)
		{
			sprintf(Buf,"[%d, %d]",cfg.AmpBoxX,cfg.AmpBoxY);
			if(win)
			DrawTextToScreenCenter(Buf,x+(xplus/2)+8,y+47,8,6);
			else
			DrawTextToScreenCenter(Buf,x+(xplus/2)+8,y+37,8,6);
		}
	}
	if(!MoveAmp)
	{
		cfg.oldMouse6X=D2CLIENT_MouseX;
		cfg.oldMouse6Y=D2CLIENT_MouseY;
		cfg.oldBox6X=x;
		cfg.oldBox6Y=y;
	}
	if(MoveAmp)
	{
		oldMouseX=cfg.oldMouse6X;
		oldMouseY=cfg.oldMouse6Y;
		oldBoxX=cfg.oldBox6X;
		oldBoxY=cfg.oldBox6Y;
		deltaMouseX=D2CLIENT_MouseX - oldMouseX;
		deltaMouseY=D2CLIENT_MouseY - oldMouseY;
		newBoxX=oldBoxX + deltaMouseX;
		newBoxY=oldBoxY + deltaMouseY;
		cfg.AmpBoxX=newBoxX;
		cfg.AmpBoxY=newBoxY;

		if(cfg.AmpBoxX < 0) cfg.AmpBoxX = 0;
		if((cfg.AmpBoxX+xplus+16) > 798) cfg.AmpBoxX = 798-xplus-16;
		if(cfg.AmpBoxY < 0) cfg.AmpBoxY = 0;
		if(cfg.AmpBoxY > 515) cfg.AmpBoxY = 515;			
	}
	if(win)
	{
		box.AmpX=x;
		box.AmpX2=x+xplus+16;
		box.AmpY=y;
		box.AmpY2=y+37;
	}
	else
	{
		box.AmpX=x;
		box.AmpX2=x+xplus+16;
		box.AmpY=y;
		box.AmpY2=y+27;
	}
}