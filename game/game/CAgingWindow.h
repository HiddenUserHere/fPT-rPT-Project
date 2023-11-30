#pragma once

#define AGING_FREE				(*(BOOL*)0x00CDC4E4)

class CAgingWindow
{
private:
	int											iReduceTax = 0;
public:
	CAgingWindow();
	virtual ~CAgingWindow();

	UI::Window_ptr								pWindow = NULL;
	int											iAging = -1;

	enum
	{
		TextID_None,
		TextID_Fail,
	};

	void										Render();

	void										Init();

	void										HandlePacket( PacketAgingFreeEvent * psPacket );
};

