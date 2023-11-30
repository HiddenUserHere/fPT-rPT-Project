#pragma once


class CPremiumModel;
class CPremiumView;



struct PremiumResource
{
	DWORD										dwCode;
	enum EItemTimerType							eType;
	int											iImageHandler;
	int											iImageBlackHandler;
	int											iAlpha;
};

struct PremiumInfoData
{
	EItemTimerType								eType;
	char										szTitle[32];
	char										szDescription[64];
	int											iLevel;
	class UI::Image								* pImage;
	class UI::Image								* pDisabledImage;
	int											iAlpha;
	BOOL										bUseServer;
	BOOL										bRepeatUse;
};

class CPremiumControllerHandler
{
private:
	struct PacketItemPremiumData				* psPacketPremium = NULL;

	CPremiumModel								* pcPremiumModel = NULL;
	CPremiumView								* pcPremiumView = NULL;

public:
	CPremiumControllerHandler();
	virtual ~CPremiumControllerHandler();

	void										Init();

	PacketItemPremiumData						* GetData() { return psPacketPremium; };
	CPremiumModel								* GetModel() { return pcPremiumModel; };
	CPremiumView								* GetView() { return pcPremiumView; };

	void										Update(float fTime);
	void										UpdateTimers();

	BOOL										OnMouseClick( class CMouse * pcMouse );

	void										SetPremiumServer( struct PacketActivePremium * psPacket );

	void										OnDraw();
};

