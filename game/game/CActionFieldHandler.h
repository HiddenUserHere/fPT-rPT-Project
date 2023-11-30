#pragma once

#include "CActionFieldChestWindow.h"
#include "CActionFieldRequestWindow.h"

#define ACTIONFIELD_CAMERAX		(*(int*)0x00824004)

class CActionFieldHandler
{
public:
	CActionFieldHandler();
	virtual ~CActionFieldHandler();

	void					HandlePacket( PacketActionFieldRequest * psPacket );
	void					HandlePacket( PacketActionFieldEnter * psPacket );
	void					HandlePacket( PacketActionFieldFail * psPacket );
	void					HandlePacket( PacketActionFieldUpdate * psPacket );
	void					HandlePacket( PacketActionFieldExchange * psPacket );
	void					HandlePacket( PacketActionFieldStart * psPacket );
	void					HandlePacket( PacketActionFieldEnd * psPacket );

	void					ProcessPacket( PacketActionFieldExchange * psPacket );
	void					ProcessPacket( PacketActionFieldRequest * psPacket );

	void					Init();

	void					Render();

	void					Update( float fTime );

	void					OnResolutionChanged();

	void					OnMouseMove( class CMouse * pcMouse );
	BOOL					OnMouseClick( class CMouse * pcMouse );

	BOOL					IsInActionFieldMap( EMapID iMapID );

	int						GetXRange();

	int						GetRound() { return iRound; };

	void					Clear();

	CActionFieldChestWindow * GetChestWindow() { return pcChestWindow; }

	CActionFieldRequestWindow * GetRequestWindow() { return pcRequestWindow; }

private:
	enum
	{
		WINDOWID_Main,
		TEXTID_Name,
		TEXTID_Round,
		TEXTID_Time,

		TEXTID_PlayerNameOne,
		TEXTID_PlayerNameTwo,
		TEXTID_PlayerNameThree,

		TEXTID_PlayerKillsOne,
		TEXTID_PlayerKillsTwo,
		TEXTID_PlayerKillsThree,

		TEXTID_PlayerPointsOne,
		TEXTID_PlayerPointsTwo,
		TEXTID_PlayerPointsThree,

		TEXTID_PlayerPlaceOne,
		TEXTID_PlayerPlaceTwo,
		TEXTID_PlayerPlaceThree,
	};

	EMapID					GetMapID( EActionFieldModeID iModeID );

	void					AddParticleNextRound();
	void					AddParticleLimitRange();

	void					BuildWindow();

	CActionFieldChestWindow * pcChestWindow;
	CActionFieldRequestWindow * pcRequestWindow;

	UI::Window_ptr			pGhostCastleInfo;

	//Round
	UI::Image				* pcaImageRound[5];
	int						iAlphaRound;
	BOOL					bShowImageRound;
	float					fTimeImageRound;

	int						iRound = 0;
	
	EActionFieldModeID		iModeID;

	char					szName[64];
};

