#pragma once

#include <windows.h>

typedef struct LinkedItem_t
{
	LinkedItem_t*	pPrev;
	LinkedItem_t*	pNext;
	char Name[100];
	DWORD		dwUnitId;
} LinkedItem;

class LinkedList
{
private:
	LinkedItem*	pFirst;
	LinkedItem*	pLast;
	UINT		cItemCount;
public:
	LinkedList();
	virtual ~LinkedList();
	LinkedItem* AddItem(char* name);
	LinkedItem* AddItem1(DWORD dwUnitId);
	LinkedItem* RemoveItem(LinkedItem* pItem);
	LinkedItem* RemoveItem1(DWORD dwUnitId);
	LinkedItem* GetFirstItem();
	LinkedItem* GetLastItem();
	LinkedItem* GetPrevItem(LinkedItem* pItem);
	LinkedItem* GetNextItem(LinkedItem* pItem);
	UINT LinkedList::GetItemCount();
};
