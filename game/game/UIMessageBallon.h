#pragma once

struct EXESurfaceOld;

enum ESkinID
{
	SKINID_Undefined				= -1,

	SKINID_Normal,
	SKINID_Blue,
	SKINID_Green,
	SKINID_Pink,
};

namespace UI
{
struct Balloon
{
	class Image						* pImageLeftTop;
	class Image						* pImageMiddleTop;
	class Image						* pImageRightTop;
	class Image						* pImageLeft;
	class Image						* pImageRight;
	class Image						* pImageLeftBottom;
	class Image						* pImageMiddleBottom;
	class Image						* pImageRightBottom;
	class Image						* pImageTailBottom;
	class Image						* pImageCenter;
};

enum EPositionText
{
	POSITIONTEXT_Top,
	POSITIONTEXT_Middle,
	POSITIONTEXT_Bottom,
};

struct ExtraText
{
	Text_ptr						pText;
	EPositionText					ePosition;
};

typedef std::shared_ptr<class MessageBalloon> MessageBallon_ptr;
class MessageBalloon : public Element
{
public:
	//Constructor
									MessageBalloon( Rectangle2D rRectangle );
	virtual						   ~MessageBalloon();

	void							Init();

	void							SetMessage( std::string strMessage );
	void							SetClanName( std::string strText );
	void							SetPetName( std::string strText, DWORD dwColor = -1 );
	void							AddExtraText( int iID, EPositionText ePosition, std::string strText, DWORD dwColor = -1 );
	void							ResetMessage();
	Text_ptr						GetTextMessage(){ return pTextMessage; }

	void							Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	void							SetHover( BOOL b ){ bHover = b; }
	void							SetColor( DWORD dwColor );

	void							IsCharacterMessage( BOOL b );
	void							SetSkin( ESkinID e ){ eSkinID = e; }

	void							SetBellatraIconID( int i ) { iBellatraCrown = i; }

	void							SetClanIconTexture( EXESurfaceOld * ps );

	void							DrawMessageBalloonBox( int iX, int iY, int iWidth, int iHeight );

	void							Update( float fTime );

	int								GetLastHeight() { return iLastHeight; }
	void							ResetLastHeight() { iLastHeight = 0; }

protected:

	struct StringBalloonHash
	{
		std::string					strText;

		DWORD						dwTimeOut;

		int							iHighlightWidth;

		StringBalloonHash( const std::string _strText, int _iHighlightWidth, DWORD _dwTime )
		{
			strText = _strText;
			dwTimeOut = _dwTime + 15000;
			iHighlightWidth = _iHighlightWidth;
		};

		~StringBalloonHash() {};
	};


	Balloon							saMessagesBalloon[4][2];
	ESkinID							eSkinID = SKINID_Normal;

	BOOL							bHover = FALSE;
	BOOL							bIsMessage = FALSE;
	BOOL							bIsCharacterMessage = FALSE;
	BOOL							bClanName = FALSE;
	BOOL							bPetName = FALSE;

	Text_ptr						pTextMessage;
	Text_ptr						pTextClanName;
	Text_ptr						pTextPetName;
	std::vector<ExtraText*>			vExtraText;

	BOOL							bShowClanIcon = FALSE;
	EXESurfaceOld					* psTextureClanIcon;

	int								iLastHeight = 0;

	int								iBellatraCrown = -1;
	class Image						* pcaBellatraCrown[3];

	const char						* FONTNAME_DEFAULT = "Arial";

	std::map<std::size_t, StringBalloonHash> vCache;

};
};