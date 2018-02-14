#define LIFE 0
#define MANA 1

extern class Chicken {

private:

	BOOL SupplyHealth();
	BOOL SupplyMana();
	BOOL SupplyJuv(INT iType);
	BOOL TownChicken(INT iType);

public:

	VOID ChickenCheck();

};
typedef struct t_ChickenType {
	PCHAR pType;
	PINT Setting;
	PINT Timer;
} CHICKENTYPE, *LPCHICKENTYPE;

BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin = 0);