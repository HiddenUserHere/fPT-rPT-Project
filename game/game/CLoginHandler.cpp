#include "stdafx.h"
#include "CLoginHandler.h"


CLoginControllerHandler::CLoginControllerHandler()
{
}


CLoginControllerHandler::~CLoginControllerHandler()
{
	DELET( pcLoginModel );
}

void CLoginControllerHandler::OnRender()
{	
	if ( !pcLoginModel )
		pcLoginModel = new CLoginModel;
	pcLoginModel->OnRender();
}

void CLoginControllerHandler::RenderServerSelectBox()
{
	if ( !pcLoginModel )
		pcLoginModel = new CLoginModel;
	pcLoginModel->RenderServerSelectBox();
}

void CLoginControllerHandler::RenderBGLogin()
{
	if ( !pcLoginModel )
		pcLoginModel = new CLoginModel;
	pcLoginModel->RenderLoginBG();
}

void CLoginControllerHandler::Init()
{
}
