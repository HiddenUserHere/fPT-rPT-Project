#pragma once

typedef void( __thiscall *tfnCallUIDraw )(DWORD pcClass);

class CUIControllerGeneral
{
protected:
	// Pointer Classes
	static const DWORD							pcCharStatus			= 0x035D0EA0;
	static const DWORD							pcSkill					= 0x0367E148;
	static const DWORD							pcInventory				= 0x035EBB20;
	static const DWORD							pcShop					= 0x03653E40;
	static const DWORD							pcTrade					= 0x036EFD98;
	static const DWORD							pcItemSocket			= 0x03628C08;
	static const DWORD							pcInterface				= 0x035E11D0;
	static const DWORD							pcItem					= 0x03628800;
	static const DWORD							pcHelp					= 0x035E05D8;
	static const DWORD							pcWapgate				= 0x03707108;
	static const DWORD							pcHelpPet				= 0x035E0658;
	static const DWORD							pcPhoenixPet			= 0x0362B388;
	static const DWORD							pcSod					= 0x0362BF88;
	static const DWORD							pcSiege					= 0x0362BEA8;
	static const DWORD							pcQuest					= 0x035BF418;
	static const DWORD							pcPremiumItem			= 0x035D0350;
	static const DWORD							pcMessageBox			= 0x0362B210;

	static const DWORD							DrawOffsetArray			= 0x00A0478C;

	// Functions
	static const DWORD							pfnCharStatusDraw		= 0x004986F0;
	static const DWORD							pfnCharStatusDrawText	= 0x00499BB0;
	static const DWORD							pfnPremiumDrawUp		= 0x0044953C;
	static const DWORD							pfnSkillDraw			= 0x004E9D70;
	static const DWORD							pfnSkillDrawUp			= 0x004ED760;
	static const DWORD							pfnInventoryDraw		= 0x004B6AD0;
	static const DWORD							pfnInventoryDrawText	= 0x004ACBA0;
	static const DWORD							pfnShopDraw				= 0x004E5B20;
	static const DWORD							pfnShopDrawText			= 0x004E7B20;
	static const DWORD							pfnTradeDraw			= 0x00513FA0;
	static const DWORD							pfnTradeDrawText		= 0x0050B350;
	static const DWORD							pfnSocketItemDraw		= 0x004CDDD0;
	static const DWORD							pfnInterfaceDraw		= 0x004A76B0;
	static const DWORD							pfnInterfaceDrawText	= 0x004A6CF0;
	static const DWORD							pfnItemDraw				= 0x004C7160;
	static const DWORD							pfnItemDrawText			= 0x004C7830;
	static const DWORD							pfnHelpDraw				= 0x0049D0B0;
	static const DWORD							pfnHelpDrawText			= 0x0049B460;
	static const DWORD							pfnWarpGateDraw			= 0x0051FFA0;
	static const DWORD							pfnWarpGateDrawText		= 0x00520370;
	static const DWORD							pfnInterfaceEffectDraw	= 0x0052B4C0;
	static const DWORD							pfnClanMenuDraw			= 0x0065E000;
	static const DWORD							pfnSkillTextDraw		= 0x004EAFB0;
	static const DWORD							pfnHelpPetDraw			= 0x004A1B00;
	static const DWORD							pfnHelpPetDrawText		= 0x004A4690;
	static const DWORD							pfnPhoenixPetDraw		= 0x004D66D0;
	static const DWORD							pfnPhoenixPetDrawText	= 0x004D66E0;
	static const DWORD							pfnSodDraw				= 0x004DC600;
	static const DWORD							pfnSodDrawText			= 0x004E0240;
	static const DWORD							pfnSiegeDraw			= 0x004DD3B0;
	static const DWORD							pfnSiegeDrawText		= 0x004DEAF0;
	static const DWORD							pfnQuestDraw			= 0x0048D970;
	static const DWORD							pfnPremiumDraw			= 0x00492F10;
	static const DWORD							pfnPremiumDrawText		= 0x004939E0;
	static const DWORD							pfnMessageBoxDraw		= 0x004D0AE0;
	static const DWORD							pfnMessageBoxDrawText	= 0x004D1880;

	UI::ImageBox_ptr							pImageBoxClan;
	UI::Text_ptr								pNameClan;
	UI::Text_ptr								pDescriptionClan;
private:
	void										CallpFunction( DWORD pFunction, DWORD pClass );
public:
												CUIControllerGeneral();
	virtual									   ~CUIControllerGeneral();


	void										Init();

	BOOL										OnMouseClick( class CMouse * pcMouse );

	void										Render();
	void										RenderMouseItem(){ CallpFunction( pfnItemDraw, pcItem ); }

	void										OnDrawClanInfo( int iX );
};

