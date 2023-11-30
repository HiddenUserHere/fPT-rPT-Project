#pragma once

namespace UI
{
typedef std::shared_ptr<class SkillInfoBox> SkillInfoBox_ptr;
class SkillInfoBox : public Element
{
public:
										SkillInfoBox();
	virtual								~SkillInfoBox();

	void								Clear() {}

	BOOL								IsActiveSkill( ESkillID eSkillID );
	BOOL								IsPassiveSkill( ESkillID eSkillID );
	BOOL								IsDebuffSkill( ESkillID eSkillID );
	BOOL								IsBuffSkill( ESkillID eSkillID );
	BOOL								IsSummonSkill( ESkillID eSkillID );

	BOOL								CanPrepareShowSkill( Skill * psSkill, Rectangle2D sBox );

	void								AddString( int iSide, const std::string & strString, DWORD dwColor = -1, BOOL bBold = FALSE );
	void								SetLineColor( DWORD dwColor, int iSide = -1, int iLineCount = -1 );

	void								Init();

	void								FormatSkillInfo();
	BOOL								PrepareShowSkill( Skill * pcSkill, BOOL bSkillTimer, Rectangle2D sBox );

	void								Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	void								DrawSkillInfoBox( int iX, int iY, int iWidth, int iHeight );

	void								NewFormatSkillInfo();

	void								FormatSkillInfoFighter();
	void								FormatSkillInfoMechanician();
	void								FormatSkillInfoArcher();
	void								FormatSkillInfoPikeman();
	void								FormatSkillInfoAtalanta();
	void								FormatSkillInfoKnight();
	void								FormatSkillInfoMagician();
	void								FormatSkillInfoPriestess();
	void								FormatSkillInfoAssassin();
	void								FormatSkillInfoShaman();

protected:
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

	DX::RenderTarget_ptr				pRenderTargetMask;
	DX::Effect_ptr						pEffectMask;

	//Icons
	class Image							* pImageIconItems[12];

	std::vector<Text_ptr>				vLeftText, vRightText, vCompareText;
	Rectangle2D							sSkillBox;
	Skill								cSkillWrapper;
	int									iSkillInfoBoxHeight;
	UINT								uSkillStatusPos = 0;
	std::string							strSkillNextLevelCost = "";
	BOOL								bInfoBoxSkillTimer = FALSE;
	UINT								uImageIconsItemsY = 0;
};
}
