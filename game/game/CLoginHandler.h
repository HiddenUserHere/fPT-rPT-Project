#pragma once
#include "LoginModel.h"

class CLoginModel;

class CLoginControllerHandler
{
protected:
	CLoginModel									* pcLoginModel = NULL;
public:
	CLoginControllerHandler();
	virtual ~CLoginControllerHandler();

	void										OnRender();
	void										RenderServerSelectBox();
	void										RenderBGLogin();
	void										Init();
};

