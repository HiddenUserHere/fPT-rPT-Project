#pragma once
class InputBox
{
protected:
	UI::Window_ptr									pWindow1;

	enum
	{
		WINDOWID_Main,
		
		BUTTONID_Ok,
		BUTTONID_Cancel,

		TEXTID_Value,
		LABELID_Title,
		LABELID_Description,

		INPUTID_Value,

	};

	struct SImageAdd
	{
		UI::Image									* pcImage;
		Rectangle2D									sBox;
	};

	std::vector<SImageAdd*>							vImages;

	BOOL											bType = FALSE;

	BOOL											bIsNumberType = FALSE;



public:
	InputBox();
	virtual ~InputBox();


	void											Init();

	void											Render();

	void											OnMouseMove( class CMouse * pcMouse );
	BOOL											OnMouseClick( class CMouse * pcMouse );
	BOOL											OnMouseScroll( class CMouse * pcMouse );

	BOOL											OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL											OnKeyChar( class CKeyboard * pcKeyboard );

	BOOL											GetType() { return bType; }

	int												GetValue();
	const char										* GetText();

	void											SetNumberType( int iMin, int iMax, BOOL bCanSelect = FALSE );
	void											SetTitle( char * pszText );
	void											SetDescription( char * pszText );
	void											SetValue( char * pszText );

	void											SetEvent( std::function<void( UIEventArgs const & )> const & f );

	void											AddImage( UI::Image * pcImage, Rectangle2D sBox );

	void											Hide();
	void											Show();

protected:
	void											OnOKButtonClick( BOOL bTyped, UIEventArgs eArgs );


};

