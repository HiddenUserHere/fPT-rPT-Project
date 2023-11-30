#include "stdafx.h"
#include "CSound.h"
#include "Musics.h"

int iCurrentBGMVolume = 0;
int iMusicNumCurrent = -1;
DWORD dwCurrentTimeBGM = 0;
int iNextBGM = -1;
BOOL bFirstBGM = FALSE;

CSound::CSound()
{
}

CSound::~CSound()
{
}

LPDIRECTSOUND CSound::GetPrimaryDevice( )
{
	lpDSPrimary = ( LPDIRECTSOUND )*( UINT* )0x3880230;
	return lpDSPrimary;
}

LPDIRECTSOUND CSound::GetSecondaryDevice( )
{
	lpDSSecondary = ( LPDIRECTSOUND )*( UINT* )0x388023C;
	return lpDSSecondary;
}

LPDIRECTSOUNDBUFFER CSound::GetPrimarySndBuf( )
{
	return ( LPDIRECTSOUNDBUFFER )*( UINT* )0x3880240;
}

LPDIRECTSOUNDBUFFER CSound::GetSecondarySndBuf( int Num )
{
	return ( LPDIRECTSOUNDBUFFER )*( UINT* )(0x387DB18 + (Num*4));
}

void CSound::PlayWave( int Num )
{
	//If there is no buffer we can't play anything so leave 
	if ( lpdsbStatic == NULL ) return;

	//If the buffer is already playing we MUST set the play position 
	
	//restore the buffer 
	if ( SUCCEEDED( lpdsbStatic->Restore( ) ) )
	{
		lpdsbStatic->SetCurrentPosition( Num );
		lpdsbStatic->Play( 0, 0, DSBPLAY_LOOPING );
	}
}

void CSound::LoadMusic( int iNum )
{
	char *pMusic = (char*)BGMList[ iNum ];
	typedef void( __cdecl *t_LoadBGM ) ( char * pBGM );
	t_LoadBGM LoadBGMPT = ( t_LoadBGM )0x00542A30;

	if ( FILEEXIST( pMusic ) )
	{
		LoadBGMPT( pMusic );
		((void( *)()) 0x00542A80)();

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic )
			SetVolumeMusic( 0 );
		else
		{
			if ( this->GetPrimarySndBuf( ) != NULL )
				this->GetPrimarySndBuf( )->SetVolume( -4999 );
			iMusicNumCurrent = iNum;
			iCurrentBGMVolume = 0;
		}
	}
}

void CSound::SetVolumeMusic( int iPercent )
{

	int iPerCalc = 100 - iPercent;

	if ( iPercent <= 0 )
		iPerCalc = 285;

	if ( this->GetPrimarySndBuf( ) != NULL )
		this->GetPrimarySndBuf( )->SetVolume( ( iPerCalc * -35 ) );

}

void CSound::SetVolumeEffect( int iPercent )
{
	int iPerCalc = 100 - iPercent;

	if ( iPercent <= 0 )
		iPerCalc = 285;

	int iSkill = READDWORD( 0x03893C30 ) + 246;

	for ( int i = 0; i < 2500; i++ )
	{
		if ( (iSkill == i) )
			continue;

		if ( this->GetSecondarySndBuf( i ) != NULL )
		{
			LONG iVolume = 0;
			this->GetSecondarySndBuf( i )->GetVolume( &iVolume );
			if ( iVolume != (iPerCalc * -35) )
				this->GetSecondarySndBuf( i )->SetVolume( (iPerCalc * -35) );
		}
	}
}

void CSound::SetVolumeSkill( int iID, int iPercent )
{
	int iPerCalc = 100 - iPercent;

	if ( iPercent <= 0 )
		iPerCalc = 285;

	int iSkill = READDWORD( 0x03893C30 ) + iID;

	if ( this->GetSecondarySndBuf( iSkill ) != NULL )
		this->GetSecondarySndBuf( iSkill )->SetVolume( (iPerCalc * -35) );
}

NAKED void CSound::ChangeMusic( int iID )
{
	JMP( pfnChangeBGM );
}

NAKED void CSound::PlayMusic( int iID )
{
	JMP( pfnPlayDirectBGM );
}

BOOL CSound::ChangeBackgroundMusic( int iID )
{
	// TGF
	if ( GRANDFURYHANDLER->IsStarted() && iID == BACKGROUNDMUSICID_Dungeon && CHARACTERGAME->GetStageID() == MAPID_CursedTempleF2 )
		return TRUE;

	if ( AKATSUKIHANDLER->IsActiveMusicTheme() )
		return TRUE;

	if ( (*(BOOL*)0x00CDC4DC) == FALSE )
		return TRUE;
	
	if ( (*(int*)0x03893C4C) )
	{
		(*(int*)0x0388026C) = iID;
		return TRUE;
	}

	if ( (*(int*)0x0388026C) == iID )
		return FALSE;

	(*(int*)0x0388026C) = iID;
	(*(int*)0x03893C4C) = (*(int*)0x00CF4750);
	return TRUE;
}

void CSound::Update()
{
	if ( dwCurrentTimeBGM < TICKCOUNT && !SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic )
	{
		if ( (*(int*)0x0388026C) >= 0 && (*(int*)0x0388026C) != iMusicNumCurrent && iMusicNumCurrent != 13 && iMusicNumCurrent != 14 )
		{
			if ( iCurrentBGMVolume > 0 )
			{
				iCurrentBGMVolume -= 5;
				if ( iCurrentBGMVolume < 0 )
					iCurrentBGMVolume = 0;

				SetVolumeMusic( iCurrentBGMVolume );
			}
		}
		else		
		{
			if ( iCurrentBGMVolume != SETTINGSHANDLER->GetModel()->GetSettings().iMusicVolume )
			{
				SetVolumeMusic( iCurrentBGMVolume );
				iCurrentBGMVolume += 5;
			}

			if ( iCurrentBGMVolume > SETTINGSHANDLER->GetModel()->GetSettings().iMusicVolume )
			{
				iCurrentBGMVolume = SETTINGSHANDLER->GetModel()->GetSettings().iMusicVolume;
				SetVolumeMusic( iCurrentBGMVolume );
			}
		}

		dwCurrentTimeBGM = TICKCOUNT + 250;
	}
}

void CSound::CreateDevices( )
{
	*( UINT* )0x387DB10 = ( UINT )DirectSoundCreate( NULL, ( LPDIRECTSOUND * )0x3880230, NULL );
	*( UINT* )0x3856A08 = ( UINT )DirectSoundCreate( NULL, ( LPDIRECTSOUND * )0x388023C, NULL );
}

void CSound::UpdateVolume()
{
    if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoSoundEffects )
        SOUNDHANDLER->SetVolumeEffect( 0 );
    else
        SOUNDHANDLER->SetVolumeEffect( SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume );
}

void CSound::SetWeaponSound( EWeaponSoundID iID, int iX, int iY, int iZ )
{
	CALL_WITH_ARG4( pfnSetWeaponSound, iX, iY, iZ, (int)iID );
}

void CSound::SetSkillSound( int iID, int iX, int iY, int iZ )
{
	CALL_WITH_ARG5( pfnSetSkillSound, iID, iX, iY, iZ, FALSE );
}

NAKED BOOL CSound::PlaySoundID( int iID, DWORD dwFlags, DWORD dwVolume, DWORD dwFrequency )
{
    JMP( 0x00541E70 );
}

void CSound::ChangeFrequencySoundSkill( int iID, DWORD dwFrequency )
{
	int i = 0;
	while ( TRUE )
	{
		if ( saSoundData[i].pszSkillSoundFile == NULL )
			break;

		if ( saSoundData[i].iID == iID )
		{
			CALL_WITH_ARG2( 0x00541670, i + READDWORD( 0x03893C30 ), dwFrequency );
			return;
		}

		i++;
	}
}

int CSound::GetDistanceVolume( Point3D sPosition )
{
	int iVolume = 0;

	if ( UNIT )
		sPosition = (UNIT->GetPosition() - sPosition);

	sPosition >>= 8;

	iVolume = Point3D( 0, 0, 0 ).GetPTDistance( sPosition ) / 1100;

	if ( iVolume > 400 )
		iVolume = 400;

	iVolume = 400 - iVolume;

	return iVolume;
}
