#pragma once

#include "Coin.h"
#include "CoinShopTab.h"

class CCoinShop
{
protected:
	int													iID;
	char												szName[32];
	char												szMessage[128];
	int													iDiscount;
	BOOL												bIsTimeShop;

	std::vector<class CCoinShopTab*>					vTabs;

public:
	CCoinShop( int iID ) { this->iID = iID; }
	~CCoinShop();
	std::vector<class CCoinShopTab*>					GetTabs() { return vTabs; }
	std::vector<class CCoinShopTab*>					GetAllTabs();
	class CCoinShopTab									* GetTabByID( int iID );

	int													GetID() { return iID; };
	char												* GetName() { return szName; }
	char												* GetMessage() { return szMessage; }
	int													GetDiscount() { return iDiscount; }
	BOOL												IsTimeShop() { return bIsTimeShop; }

	void												SetName( char * pszName ) { StringCbCopy( szName, 32, pszName ); }
	void												SetMessage( char * pszMessage ) { StringCbCopy( szMessage, 128, pszMessage ); }
	void												SetDiscount( int iDiscount ) { this->iDiscount = iDiscount; }
	void												SetIsTimeShop( BOOL b ) { bIsTimeShop = b; }

	void												AddTab( CCoinShopTab * pcTab ) { vTabs.push_back( pcTab ); }
};
