#pragma once

typedef void( __thiscall *tfnPhoenix )(DWORD pPhoenix);



class CPremiumModel
{
protected:

	struct PremiumInfoData						* psaPremiumInfoData[50];

	// Phoenix Pet
	const DWORD									bShowPhoenixPet		= 0x0362B590;
	const DWORD									pfnSetPhoenix		= 0x004D65B0;
	const DWORD									pfnDeletePhoenix	= 0x004D6640;
	const DWORD									pcPhoenix			= 0x0362B388;
	const DWORD									pActivePhoenix		= pcPhoenix + 0x204;

	// Premium Data
	const DWORD									pcPremiumData		= 0x035D0350;
	DWORD										pdwHPUpTime			= pcPremiumData + 0x8F0;
	DWORD										pdwMPUpTime			= pcPremiumData + 0x8F4;
	DWORD										pdwMPDownTime		= 0x035D0C48;
	DWORD										dwMPDownPercent		= 25;
	DWORD										pdwMPDownPercent	= 0x035D0C4C;
	DWORD										pdwSPDownTime		= 0x035D0C60;
	DWORD										dwSPDownPercent		= 25;
	DWORD										pdwSPDownPercent	= 0x035D0C64;
	DWORD										pdwGravityStnTime	= 0x035D0C50;
	DWORD										dwGravityStnAdd		= 1000;
	DWORD										pdwGravityStnAdd	= 0x035D0C54;

	// Max items count in vertical
	const int									iCountWall			= 5;
	const int									iPremiumWidth		= 38;
	const int									iPremiumHeight		= 38;
	const int									iPremiumBoxWidth	= 189;
	const int									iPremiumBoxHeight	= 71;
	const int									iYDown				= iPremiumHeight + 2;
	const int									iXRight				= iPremiumWidth + 2;
	const int									iAlphaTime			= 1;
	const int									iXBegin				= 6;
	const int									iYBegin				= 84;

	PremiumResource								psaPremiumResource[ 50 ];

	class UI::Image								* pHandlerPremiumBox = NULL;											

	BOOL										bLoaded = FALSE;

	DWORD										dwTime = 0;
private:
	DWORD										GetPremiumTime( EItemID eID );
	BOOL										IsHeadPremium( EItemID eID );
	EItemTimerType								GetPremiumType( EItemID eID );
	PremiumInfoData								* GetPremiumInfo( EItemTimerType eID );
	ItemPremium									* GetPremium( EItemID eID );
	PremiumResource								* GetResource( EItemTimerType eType );
	PremiumResource								* GetResource( EItemID eID );

	void										SetTimeFunction( DWORD pdwTime, int iTime );
	void										SetPremiumFunction( EItemID eID, EItemTimerType eType, int iTimeLeft );
	void										SetPhoenix( DWORD dwCode );

	BOOL										SetPremiumSlot( EItemID eID );

	void										LoadPremiumImages();

	void										RenderBox( ItemPremium * psItem, int iX, int iY );

	void										DrawBox( EItemID eID );

	struct PremiumInfoData						* CreatePremiumDataInfo( PremiumInfoData * psPremiumData, EItemTimerType eType, int iLevel, const char szTitle[32], const char szDescription[64], BOOL bRepeatUse, BOOL bUseServer, const char * pszImage, const char * pszImageDisabled );

public:
	CPremiumModel();
	virtual ~CPremiumModel();

	void										SetPremiumServer( struct PacketActivePremium * psPacket );

	BOOL										IsUsingHeadPremium();

	char										* OnDescription( DWORD dwCode );

	BOOL										SetPremium( EItemID eID );

	void										Update( float fTime );

	BOOL										OnMouseClick( class CMouse * pcMouse );
	void										OnDraw();

	void										Init();

};

