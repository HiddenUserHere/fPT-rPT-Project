#include "stdafx.h"
#include "SettingsView.h"


CSettingsView::CSettingsView()
{
}


CSettingsView::~CSettingsView()
{
}


void CSettingsView::Render()
{
	SETTINGSHANDLER->GetWindow()->Render();
}