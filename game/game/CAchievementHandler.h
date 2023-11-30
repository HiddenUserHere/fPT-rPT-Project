#pragma once
#include "CAchievementWindow.h"

#define MAX_ACHIEVEMENT_PACKET_CACHE 150

class CAchievementHandler
{
public:
	CAchievementHandler();
	~CAchievementHandler();

	void							Init();

	void							Render();

	void							Update( float fTime );

	void							HandlePacket( PacketAchievement * psPacket );
	void							HandlePacket( PacketAchievementUser * psPacket, BOOL bCanCache = TRUE );

	CAchievement					* GetAchievement( int iID );
	int								GetAchievementID( EAchievementType iType );
	CAchievement					* GetAchievement( EAchievementType iType );
	BOOL							HaveAchievement( Unit * pcUnit, EAchievementType iType );

	void							AddWindowAchievementList( Unit * pcUnit );

	std::vector<CAchievement*>		GetAchievements() { return vAchievement; };

	CAchievementWindow				* GetWindow() { return pcWindow; }

	void							ProcessCache( Unit * pcUnit );

private:
	CAchievementWindow				* pcWindow;

	std::vector<CAchievement*>		vAchievement;

	UINT							uCachePacketWheel = 0;
	PacketAchievementUser			saPacketCache[MAX_ACHIEVEMENT_PACKET_CACHE];

	BOOL							AddUnitAchievement( Unit * pcUnit, CAchievementUser * pcAchievementUser );

};

