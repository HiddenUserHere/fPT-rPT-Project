#pragma once
class CChristmasHandler
{
private:
	DWORD												dwTimeSnow = 0;
	int													iParticleSnowLoad = 0;
	int													iParticleSnow = 0;
public:
	CChristmasHandler();
	virtual ~CChristmasHandler();

	BOOL												IsValidRune( class ItemData * pcItem );
	BOOL												IsPremiumXMas( int iItemID );

	void												OpenRudolphWindow();

	void												OnClickOKNPC();

	void												OnClickBox( UIEventArgs e );

	void												LoadParticles();

	void												Render();
};

