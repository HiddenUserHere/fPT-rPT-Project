#pragma once

namespace UI
{
typedef std::shared_ptr<class ItemInfoBox> ItemInfoBox_ptr;
class ItemInfoBox : public Element
{
public:
										ItemInfoBox();
	virtual								~ItemInfoBox();

	void								Clear() {}

	void								AddString( int iSide, const std::string & strString, DWORD dwColor = -1, BOOL bBold = FALSE );
	void								AddCompareString( const std::string & strString, DWORD dwColor = -1 );
	void								AppendLineText( std::string str, int iSide = -1, int iLineCount = -1 );
	void								SetLineColor( DWORD dwColor, int iSide = -1, int iLineCount = -1 );

	void								Init();

	void								ClearCompareItem();
	void								SetCompareItem( const ItemData * pcCompareItemData, BOOL bEquippedItem = FALSE );

	BOOL								IsViewingPerfectItem(){ return bIsViewingPerfectItem; }
	void								SetViewingPerfectItem( BOOL b ){ bIsViewingPerfectItem = b; }

	bool								IsCostumeItem( ItemData * pcItemData );

	BOOL								IsBeginnerSet( ItemData * pcItemData );
	BOOL								IsSameItem( ItemData * pcItemData );

	BOOL								FormatItemInfo( ItemData * pcItemData );
	static DWORD						GetItemPlayTime( Item * psItem );
	static BOOL							IsExpiredItemTime( Item * psItemInfo );
	BOOL								FormatItemTimeInfo( ItemData * pcItemData );
	BOOL								PrepareShowItem( ItemData * pcItemData, BOOL bShowBuyPrice, BOOL bShowSellPrice, BOOL bShowChecksums );

	void								Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void								Update( float fTime );

	void								DrawItemInfoBox( int iX, int iY, int iWidth, int iHeight );

protected:
	enum
	{
		WINDOWS_HeadPreview,
		UNITMESHID_HeadPreview,
		UNITMESHID_BodyPreview,
	};

	//Images Box
	class Image							* pImageMaskLeftTop;
	class Image							* pImageMaskRightTop;
	class Image							* pImageMaskLeftBottom;
	class Image							* pImageMaskRightBottom;

	class Image							* pImageBorderLeftTop;
	class Image							* pImageBorderRightTop;
	class Image							* pImageBorderTop;
	class Image							* pImageBorderLeftBottom;
	class Image							* pImageBorderRightBottom;
	class Image							* pImageBorderBottom;
	class Image							* pImageBorderLeft;
	class Image							* pImageBorderRight;

	//Aging
	class Image							* pImageAgingBar;
	class Image							* pImageAgingBarGage;
	class Image							* pImageAgingBarNum;

	//Icons
	class Image							* pImageIconArmor;
	class Image							* pImageIconRobe;
	class Image							* pImageIconWeaponOneHand;
	class Image							* pImageIconWeaponTwoHand;

	//Socket
	class Image							* pImageSocketBackground;
	class Image							* paSocket[6];
	class Image							* paSocketGems[15];

	Window_ptr							pWindowHair = nullptr;
	Window_ptr							pWindowBody = nullptr;
	float								fTimeAngleHair = 0.0f;

	DX::RenderTarget_ptr				pRenderTargetMask = NULL;
	DX::Effect_ptr						pEffectMask;

	int									iSocketGem[2];
	int									iSocketCount = 0;

	BOOL								bFadeToColor = TRUE;
	float								fProgress = 0.0f;

	std::vector<Text_ptr>				vLeftText, vRightText, vCompareText;
	std::vector<int>					vRowCompare;
	Text_ptr							pTextAging, paSocketText[2][2];

	BOOL								bCompareItem = FALSE;
	BOOL								bIsEquippedItem = FALSE;
	BOOL								bCantUseItem = FALSE;
	BOOL								bIsPerfectItem = FALSE;

	BOOL								bIsViewingPerfectItem = FALSE;

	ItemData							cItemWrapper;
	ItemData							cItemCompare;

	UI::Color							cBackgroundColor1;
	UI::Color							cBackgroundColor2;

	static const DWORD					pdwItemInfoData = 0x03625788;
};
}
