#pragma once
#include "SettingsModel.h"
#include "SettingsView.h"
#include "CSettingsWindow.h"

class CSettingsModel;
class CSettingsView;
class CSettingsWindow;

class CSettingsHandler
{
protected:
	CSettingsModel							* pcSettingsModel	= NULL;
	CSettingsView							* pcSettingsView	= NULL;
	CSettingsWindow							* pcSettingsWindow	= NULL;
public:
	CSettingsHandler();
	virtual ~CSettingsHandler();

	class CSettingsModel					* GetModel();
	class CSettingsView						* GetView();
	class CSettingsWindow					* GetWindow();

	void									Init();
	void									Shutdown();
};

