#pragma once

struct MixServerData
{
	int						iaItemID[8];
	int						iaSheltomID[15];
	int						iaTypeAttributte[8];
	float					faAttributte[8];
	int						iaAttributtePercent[8];
	char					szDescription[64];
	int						iMixNameID;
	int						iType;
	char					cPadding[0x0A0];
};


class CMixHandler
{
public:
	CMixHandler();
	virtual ~CMixHandler();

	void							SetMixStatus( Item * psItem, int iIndex );

	void							SetClearMixStatus( Item * psItem, int iIndex );

	int								GetTypeMixByItemID( int iItemID );
	int								GetIndexMixByNameID( int iMixNameID, int iItemID );

	void							SetItemMixByMixName( Item * psItem, int iMixNameID );

	void							SetMixStatusNPC( Item * psItem, int iIndex );

};

