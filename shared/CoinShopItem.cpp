#include "stdafx.h"
#include "CoinShopItem.h"

CCoinShopItem::CCoinShopItem( int iID, int iTabID, char szName[32], char szDescription[128], char szCode[8], char szImage[64], int iValue, int iDiscount, int iBulk, int iMaxBulk, BOOL bSpec, BOOL bQuantity )
{
	this->iID		= iID;
	this->iTabID	= iTabID;
	this->iValue	= iValue;
	this->iDiscount = iDiscount;
	SetName( szName );
	SetDescription( szDescription );
	SetCode( szCode );
	SetImage( szImage );

	this->bSpec		= bSpec;
	this->bQuantity = bQuantity;
	this->iBulk		= iBulk;
	this->iMaxBulk	= iMaxBulk;
}

void CCoinShopItem::SetName( const char * pszName )
{
	STRINGCOPY( szName, pszName );
}

void CCoinShopItem::SetDescription( const char * pszDescription )
{
	STRINGCOPY( szDescription, pszDescription );
}

void CCoinShopItem::SetCode( const char * pszCode )
{
	STRINGCOPY( szCode, pszCode );
}

void CCoinShopItem::SetImage( const char * pszImage )
{
	STRINGCOPY( szImage, pszImage );
}

