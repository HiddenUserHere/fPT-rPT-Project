#pragma once

namespace UI
{
enum EDropdownType
{
	DROPDOWNTYPE_None,

	DROPDOWNTYPE_Vertical,
	DROPDOWNTYPE_Horizontal,
};
enum EDropdownPosition
{
	DROPDOWNPOSITION_None,

	DROPDOWNPOSITION_Top,
	DROPDOWNPOSITION_Bottom,
	DROPDOWNPOSITION_Left,
	DROPDOWNPOSITION_Right,
};
struct Option
{
	Text_ptr					pText;
	BOOL						bChild;
};
typedef std::shared_ptr<class DropdownMenu> DropdownMenu_ptr;
class DropdownMenu : public Element
{
public:
	//Constructor
								DropdownMenu();
								DropdownMenu( Rectangle2D sBox );
	virtual					   ~DropdownMenu();

	void						Open();
	void						Close();

	void						Clear();

	void						SetMenuBackground( std::string strImageTop, std::string strImageMiddle, std::string strImageBottom, std::string strImageMain );

	Text_ptr					MakeOption( const std::string & strText, const Rectangle2D & rBox, BOOL bBold, const DWORD dwColor, const DWORD dwHoverColor );
	void						AddOption( Text_ptr p, BOOL bChild );
	void						AddOption( Text_ptr p, BOOL bChild, int iID );

	void						RemoveOption( int iID );

	void						SetEvent( int iEvent ) { iEventID = iEvent; };
	void						SetGroup( Group_ptr p ){ pGroup = p; }

	int							GetSelectedOptionID(){ return iSelectedOptionID; }

	//Getters and Setters
	EDropdownPosition			GetDropdownPosition(){ return ePosition; }
	void						SetDropdownPosition( EDropdownPosition e ){ ePosition = e; }

	void						SetMouseEvent( EMouseEvent e ){ eMouseEvent = e; }
	EMouseEvent					GetMouseEvent(){ return eMouseEvent; }

	//Render
	void						Animate();
	void						Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void						Update( float fTime );

	//Event Handlers
	BOOL						OnMouseClick( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
	void						OnMouseMove( class CMouse * pcMouse, int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );
private:
	EDropdownType				eType = DROPDOWNTYPE_Vertical;
	EDropdownPosition			ePosition = DROPDOWNPOSITION_Top;

	std::vector<Option>			vOptions;
	int							iHeightMenu = 0;

	int							iSelectedOptionID = -1;
	int							iEventID = -1;

	Image					  * pImageTop, * pImageMiddle, * pImageBottom, * pImageMain;

	BOOL						bAnimate = FALSE;
	BOOL						bOpen = FALSE;
	BOOL						bClose = FALSE;

	Group_ptr					pGroup = NULL;

	Point2D						sAnimateBox = Point2D();
	float						fProgress = 0.0f;

	EMouseEvent					eMouseEvent = ClickDownL;
};
}