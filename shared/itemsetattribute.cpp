#include "stdafx.h"
#include "itemsetattribute.h"

CItemAttributeBonus::CItemAttributeBonus( ECharacterClass iClass, EItemSetAttributeBonus iBonusAdd, float fValueAdd, int iItemCount, BOOL bPercentType ) : iClass( iClass ), iBonus( iBonusAdd ), fValue( fValueAdd ), iItemCount( iItemCount ), bPercent( bPercentType )
{
}

CItemAttributeBonus::~CItemAttributeBonus()
{
}

CItemAttribute::CItemAttribute()
{
}

CItemAttribute::~CItemAttribute()
{
	for ( auto & pc : vItems )
		DELET( pc );

	vItems.clear();

	for ( auto & pc : vBonuses )
		DELET( pc );

	vBonuses.clear();

	for ( auto & ps : vTextItems )
		DELET( ps );

	vTextItems.clear();
	
	for ( auto & ps : vTextEffects )
		DELET( ps );

	vTextEffects.clear();
}

void CItemAttribute::AddItem( int iTextID, EItemAttributeType iType, int iItem, int iCount )
{
	ItemDataAttr * ps	= new ItemDataAttr();
	ps->iTextID			= iTextID;
	ps->iType			= iType;
	ps->iItem			= iItem;
	ps->iCount			= iCount;
	vItems.push_back( ps );
}

void CItemAttribute::AddBonus( CItemAttributeBonus cBonus )
{
	vBonuses.push_back( new CItemAttributeBonus( cBonus.GetSpec(), cBonus.GetBonus(), cBonus.GetValue(), cBonus.GetItemCount(), cBonus.IsPercent() ) );
}

void CItemAttribute::AddTextItem( int iID, std::string strText )
{
	ItemAttributeText * ps = new ItemAttributeText();
	ps->iID = iID;
	ps->strText = strText;
	ps->iClass = CHARACTERCLASS_None;
	vTextItems.push_back( ps );
}

void CItemAttribute::AddTextEffect( int iItemCount, std::string strText, ECharacterClass iClass )
{
	ItemAttributeText * ps = new ItemAttributeText();
	ps->iID = iItemCount;
	ps->strText = strText;
	ps->iClass = iClass;
	vTextEffects.push_back( ps );
}

CItemAttribute::ItemAttributeText * CItemAttribute::GetTextItem( int iID )
{
	for ( auto & ps : vTextItems )
		if ( ps->iID == iID )
			return ps;

	return nullptr;
}

CItemAttribute::ItemAttributeText * CItemAttribute::GetTextEffect( int iID )
{
	for ( auto & ps : vTextEffects )
		if ( ps->iID == iID )
			return ps;

	return nullptr;
}
