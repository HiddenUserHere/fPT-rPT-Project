#include "View.h"
#include "Model.h"
#include "UI.h"


class Controller
{
private:
	Model_ptr									pModel;
	View_ptr									pView;
public:
												Controller( View_ptr pViewReceive, Model_ptr pModelReceive );
	virtual										~Controller();
	
	View_ptr									GetView();
	Model_ptr									GetModel();


	static void									OnEndScene();
	static void									OnBeginScene();
	BOOL										OnMouseClick( int eEvent );
	BOOL										OnMouseMove();
	BOOL										OnMouseWheel( int iDelta );
	BOOL										OnKeyChar( char cChar );
};