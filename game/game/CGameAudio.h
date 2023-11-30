#pragma once
class CGameAudio
{
public:
	CGameAudio();
	virtual ~CGameAudio();

	BOOL					Init();
	void					Shutdown();

	void					Update( float fTime );
};

