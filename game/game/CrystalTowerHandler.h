#pragma once
class CCrystalTowerHandler
{
private:
	IMinMax					sTowerHP;

	UnitData				* pcTower = NULL;

	int						iRound = 0;

	DWORD					dwTimeLeft = 0;

	BOOL					bStarted = FALSE;

	DX::Font_ptr			pFont = nullptr;

public:
	CCrystalTowerHandler();
	virtual ~CCrystalTowerHandler();

	void					Init();

	void					Update();

	void					Render();

	void					HandlePacket( PacketQuestArenaTier5 * psPacket );

};

