#pragma once
#ifndef ITEMCREATOR_H_
#define ITEMCREATOR_H_



class ItemCreator
{
private:
	const int iTotalItems = 1500;
	int ItemV;
	void refMem(DWORD addrr, DWORD value);
	void createItem(int idIt, const char *nomeIt, const char *codIt, const char *codIt2, int LarIt, int altIt, const char *dropIt, int mIt, int cityIt,
		int somIt, int rangeIt);
	void endIT();
	void createIT();
	void refIT();
	void ReadItems();
	UINT findItemPtr( UINT ItemID );
	UINT findItemPos( UINT ItemID );
	int itemPivotMesh( UINT PositionPtr, UINT ItemID );
public:
	int ItemPivotMotion( UINT PositionPtr, UINT ItemID );

	void Init();
};

#endif