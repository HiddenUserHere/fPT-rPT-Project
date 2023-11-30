#pragma once

#include "strings.h"
#include "Coin.h"

class CCoinShopItem : Base::Coin
{
protected:
	int											iID;
	int											iTabID;
	char										szName[32];
	char										szDescription[128];
	char										szCode[8];
	char										szImage[64];
	int											iValue;
	int											iDiscount;
	BOOL										bSpec;
	BOOL										bQuantity;
	int											iBulk;
	int											iMaxBulk;

public:
	CCoinShopItem( int iID, int iTabID, char szName[32], char szDescription[128], char szCode[8], char szImage[64], int iValue, int iDiscount, int iBulk = 0, int iMaxBulk = 0, BOOL bSpec = FALSE, BOOL bQuantity = FALSE );

	// Getters

	int											GetID() { return iID; };
	int											GetTabID() { return iTabID; };
	char										* GetName() { return szName; };
	char										* GetDescription() { return szDescription; };
	char										* GetCode() { return szCode; };
	char										* GetImage() { return szImage; };
	int											GetValue() { return iValue; };
	int											GetDiscount() { return iDiscount; };
	BOOL										GetIsSpec() { return bSpec; };
	BOOL										GetIsQuantity() { return bQuantity; };
	int											GetBulk() { return iBulk; };
	int											GetMaxBulk() { return iMaxBulk; };

	// Setters
	
	void										SetName( const char * pszName );
	void										SetDescription( const char * pszDescription );
	void										SetCode( const char * pszCode );
	void										SetImage( const char * pszImage );
	void										SetIsQuantity( BOOL b ) { bQuantity = b; }
	void										SetValue( int iValue ) { this->iValue = iValue; };
	void										SetDiscount( int iDiscount ) { this->iDiscount = iDiscount; };
	void										SetBulk( int i ) { iBulk = i; };
	void										SetMaxBulk( int i ) { iMaxBulk = i; };

};