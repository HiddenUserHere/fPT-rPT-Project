#include "stdafx.h"
#include "Coin.h"
#include "CoinShopTab.h"

void CCoinShopTab::AddItem( CCoinShopItem * pcItem )
{
	if ( pcItem )
	{
		if ( GetDiscount() )
		{
			pcItem->SetDiscount( (GetDiscount() * pcItem->GetValue()) / 100 );
		}
		if ( GetBulk() )
		{
			pcItem->SetBulk( GetBulk() );
			pcItem->SetMaxBulk( GetMaxBulk() );
		}
		vItems.push_back( pcItem );
	}
}

void CCoinShopTab::AddChildTab( CCoinShopTab * pcTab )
{
	if ( pcTab )
	{
		if ( GetDiscount() )
		{
			pcTab->SetDiscount( GetDiscount() );
		}
		if ( GetBulk() )
		{
			pcTab->SetBulk( GetBulk() );
			pcTab->SetMaxBulk( GetMaxBulk() );
		}
		vTabs.push_back( pcTab );
	}
}

void CCoinShopTab::ClearItems()
{
	std::vector<CCoinShopItem*> & vItemsList = vItems;
	if ( vItemsList.size() > 0 )
	{
		for ( std::vector<CCoinShopItem*>::iterator it = vItemsList.begin(); it != vItemsList.end(); it++ )
		{
			delete *it;
		}
		vItems.clear();
	}
}

void CCoinShopTab::ClearTabs()
{
	std::vector<CCoinShopTab*> & vList = vTabs;
	for ( std::vector<CCoinShopTab*>::iterator it = vList.begin(); it != vList.end(); it++ )
	{
		CCoinShopTab * pcTab = *it;
		if ( pcTab )
		{
			pcTab->Clear();
			delete pcTab;
		}
	}
	vList.clear();
}

void CCoinShopTab::Clear()
{
	ClearTabs();
	ClearItems();
}

CCoinShopItem * CCoinShopTab::GetItemByID( int i )
{
	std::vector<CCoinShopItem*> & vItemsList = vItems;
	for ( std::vector<CCoinShopItem*>::iterator it = vItemsList.begin(); it != vItemsList.end(); it++ )
	{
		CCoinShopItem * pcItem = *it;
		if ( pcItem && pcItem->GetID() == i )
			return pcItem;
	}

	return NULL;
}
