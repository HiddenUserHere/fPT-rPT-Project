#pragma once

#define AGINGSERVER_INCREASE_AMOUNT		(*(WORD*)0x08B80DA4)

class CAgeHandler
{
public:
	CAgeHandler();
	virtual ~CAgeHandler();

	void						SQLItemBroke( User * pcUser, Item * psItem );
	void						SQLItemBrokeRecovered( User * pcUser, int iID );
	BOOL						GetItemDataBroke( User * pcUser, Item * psItem, int iID );

	std::vector<AgingRecoveryDataHandle>	SQLGetBrokeItems( char * pszAccountName );

	void						DownDamage( Item * psItem, int iDown, int iLevel );
	void						DownDefense( Item * psItem, int iDown, int iPercent );
	void						DownAttackRating( Item * psItem, int iDown, int iNum );
	void						DownAbsorbRating( Item * psItem, int iDown, int iLevel );
	void						DownCritical( Item * psItem, int iDown );
	void						DownBlockRating( Item * psItem, int iDown );

	void						UpDamage( Item * psItem );
	void						UpDefense( Item * psItem, int iPercent );
	void						UpAttackRating( Item * psItem, int iNum );
	void						UpAbsorbRating( Item * psItem );
	void						UpCritical( Item * psItem );
	void						UpBlockRating( Item * psItem );

	BOOL						OnDownAge( Item * psItem, int iCount );
	BOOL						OnDownAgeHandler( Item * psItem, int iCount );

	BOOL						OnUpAge( Item * psItem );
	BOOL						OnUpAgeHandler( Item * psItem, int iCount = 1 );

	void						OnBrokeItemHandler( User * pcUser, Item * psItem );

	int							AgingChanceHandler( User * pcUser );

	bool						CanAge( ItemID sItemID );

private:
	void						SetAgingIncreaseAmount( int iAmount ) { AGINGSERVER_INCREASE_AMOUNT = iAmount; }

	Item						* GetItem() { return ((Item*)0x0786B838); }
	ItemID						GetStoneItemID() { return ItemID( READDWORD( 0x0786BAE4 ) ); }

};

