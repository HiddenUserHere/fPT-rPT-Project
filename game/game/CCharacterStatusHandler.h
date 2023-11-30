#pragma once

#define MAX_CHARACTERSTATUS_SAVE_POINTS		5

class CCharacterStatusHandler
{
private:
	enum ECharacterStatus
	{
		CHARACTERSTATUS_None,
		CHARACTERSTATUS_Strength,
		CHARACTERSTATUS_Spirit,
		CHARACTERSTATUS_Talent,
		CHARACTERSTATUS_Agility,
		CHARACTERSTATUS_Health,
		CHARACTERSTATUS_StatusPoints,
	};

	struct CharacterStatusSavePoints
	{
		ECharacterStatus	iStatus;
		int					iAddPoints;

		DWORD				dwTime;
	};

public:

	CCharacterStatusHandler();
	virtual ~CCharacterStatusHandler();

	void							UpdateBaseStatus( ECharacterStatus iStatus, int iAddPoints );

private:
	CharacterStatusSavePoints		saSavePoints[MAX_CHARACTERSTATUS_SAVE_POINTS];
	int								iSavePointsWheel;

	void							AddSavePoints( ECharacterStatus iStatus, int iAddPoints );
	CharacterStatusSavePoints		* GetLastSavePoints();

};

