#include "stdafx.h"
#include "TestEditor.h"


CTestEditor::CTestEditor()
{
}


CTestEditor::~CTestEditor()
{
}

void CTestEditor::BuildWindowAnimator()
{
}

void CTestEditor::BuildWindow()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( RESOLUTION_WIDTH - 200, 0, 200, 600 ) );


}

void CTestEditor::Init()
{
}

void CTestEditor::Render()
{
}

void CTestEditor::OnMouseMove( CMouse * pcMouse )
{
}

BOOL CTestEditor::OnMouseClick( CMouse * pcMouse )
{
	return 0;
}

BOOL CTestEditor::OnMouseScroll( CMouse * pcMouse )
{
	return 0;
}

BOOL CTestEditor::OnKeyPress( CKeyboard * pcKeyboard )
{
	return 0;
}

BOOL CTestEditor::OnKeyChar( CKeyboard * pcKeyboard )
{
	return 0;
}
