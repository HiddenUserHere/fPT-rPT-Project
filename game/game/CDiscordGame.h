#pragma once
class CDiscordGame
{
private:
	INT64						iStartTime = 0;

public:
	CDiscordGame();
	virtual ~CDiscordGame();

	void						Init();

	void						Shutdown();

	void						Update();
};

