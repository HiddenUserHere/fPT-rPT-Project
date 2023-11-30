#pragma once

enum ECharacterScreenStep : int
{
	CHARACTERSCREENSTEP_None,
	
	CHARACTERSCREENSTEP_NewCharacterTribe,
	CHARACTERSCREENSTEP_NewCharacterClass,
	CHARACTERSCREENSTEP_NewCharacterHair,
};

class CCharacterScreenModel
{
public:
	CCharacterScreenModel();
	virtual ~CCharacterScreenModel();

	void						Init();

	BOOL						Shutdown();

	void						Render3D();


	ECharacterScreenStep		GetStep() { return iStep; }
	ECharacterScreenStep		GetNewStep() { return iNewStep; }


	void						AddCameraAxis( Point3D sPosition );
	void						AddNextCameraAxis( Point3D sPosition );
	void						SetCameraTimeChangeDuration( float fTime ) { fCameraTimeChangeDuration = fTime; }

	void						Frame();

	void						Update( float fTime );

	void						ClearUserCharacters();
	void						AddUserCharacter( const char * pszName, const char * pszModelFile, const char * pszHeadFile, ECharacterClass iClass, int iLevel, int iClanID );

	void						ResetCharacterSelect();
	UnitData					* GetCharacter() { return pcCharacter; };
	void						SetCharacter( UnitData * pcUnitData );

	void						MoveCharacter( UnitData * pcUnitData );


	bool						GetCharacterLocked() { return bCharacterLocked; }
	void						SetCharacterLocked( bool b ) { bCharacterLocked = b; }

	bool						CanSelectUserCharacters();
	UnitData					* GetUserCharacterMouseOver( Point2D * psPoint );

	bool						CanBackLoginScreen();


private:
	Stage						* pcStage			= nullptr;
	Point3D						sCameraPosition;
	Point3D						sNextCameraPosition;
	Point3D						sCameraAngle;
	float						fCameraTimeUpdate			= 0.0f;
	float						fCameraTimeChangeDuration	= 2000.0f; //Duration beetween position changes

	Point3D						sLookAt;

	std::vector<UnitData*>		vCharacters;
	std::vector<UnitData*>		vUserCharacters;
	Point3D						sUserCharactersPosition;

	UnitData					* pcCharacter		= nullptr;
	bool						bCharacterLocked	= false;
	float						fCharacterSelectTimeUpdate = 0.0f;

	ECharacterScreenStep		iStep				= CHARACTERSCREENSTEP_None;
	ECharacterScreenStep		iNewStep			= CHARACTERSCREENSTEP_None;
	DWORD						dwUpdateTimeStep	= 0;


	void						UpdateCamera( float fTime );
	void						SetCamera( Point3D sPosition, Point3D sLookAt );

	bool						CanRenderUserCharacters();
	void						UpdateUserCharactersPosition();
	void						RenderUserCharacters();
	void						UpdateUserCharacters( float fTime );
	void						FrameUserCharacters();

	void						UpdateTextures();

};

