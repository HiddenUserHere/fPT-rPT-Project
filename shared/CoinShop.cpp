#include "stdafx.h"
#include "CoinShop.h"

CCoinShop::~CCoinShop()
{
	for ( std::vector<class CCoinShopTab*>::iterator it = vTabs.begin(); it != vTabs.end(); it++ )
	{
		CCoinShopTab * pcTab = *it;
		if ( pcTab )
		{
			pcTab->Clear();
			delete pcTab;
		}
	}
	vTabs.clear();
}

CCoinShopTab * CCoinShop::GetTabByID( int iID )
{
	for ( std::vector<class CCoinShopTab*>::iterator it = vTabs.begin(); it != vTabs.end(); it++ )
	{
		CCoinShopTab * pcTab = *it;
		if ( pcTab && pcTab->GetID() == iID )
			return pcTab;

		// Get Child
		if ( pcTab )
		{
			std::vector<CCoinShopTab*> vTabs = pcTab->GetChildTabs();
			if ( vTabs.size() > 0 )
			{
				for ( std::vector<class CCoinShopTab*>::iterator it2 = vTabs.begin(); it2 != vTabs.end(); it2++ )
				{
					CCoinShopTab * pcTab2 = *it2;
					if ( pcTab2 && pcTab2->GetID() == iID )
						return pcTab2;
				}
			}
		}
	}
	return NULL;
}

std::vector<class CCoinShopTab*> CCoinShop::GetAllTabs()
{
	std::vector<class CCoinShopTab*> v;
	for ( std::vector<class CCoinShopTab*>::iterator it = vTabs.begin(); it != vTabs.end(); it++ )
	{
		CCoinShopTab * pcTab = *it;
		if ( pcTab )
		{
			std::vector<CCoinShopTab*> vTabs = pcTab->GetChildTabs();
			if ( vTabs.size() > 0 )
			{
				for ( std::vector<class CCoinShopTab*>::iterator it2 = vTabs.begin(); it2 != vTabs.end(); it2++ )
				{
					CCoinShopTab * pcChildTab = *it2;
					if ( pcChildTab )
						v.push_back( pcChildTab );
				}
			}
			v.push_back( pcTab );
		}
	}
	return v;
}
