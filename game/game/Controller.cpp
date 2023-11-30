#include "stdafx.h"
#include "Controller.h"
#include "PremiumView.h"
#include "TestUI.h"


Controller::Controller( View_ptr pViewReceive, Model_ptr pModelReceive )
{
	pModel = pModelReceive;
	pView  = pViewReceive;
}

Controller::~Controller()
{

}

View_ptr Controller::GetView()
{
	return pView;
}

Model_ptr Controller::GetModel()
{
	return pModel;
}

void Controller::OnEndScene()
{

}

void Controller::OnBeginScene()
{
	
}

BOOL Controller::OnMouseClick( int eEvent )
{
	return FALSE;
}

BOOL Controller::OnMouseMove()
{

	return FALSE;
}

BOOL Controller::OnMouseWheel( int iDelta )
{

	return FALSE;
}

BOOL Controller::OnKeyChar( char cChar )
{

	return FALSE;
}

