#include "stdafx.h"

#include "chat.h"

#include "ChatGame.h"
#include "LevelTable.h"
BOOL bShowFps					= FALSE;
BOOL bDebug						= FALSE;
BOOL bUIDebug					= FALSE;

extern bool						bNoShowDebugDamage;

DWORD							dwLastTickLogin = 0;
DWORD							dwLastTickGame = 0;
int								iLoginServerPingID = 0;
int								iGameServerPingID = 0;

extern float fSZHPBar;

ChatGame::ChatGame()
{
}

ChatGame::~ChatGame()
{
}

void ChatGame::SendChat( BOOL bLoginServer, const char * pszChat )
{
	int iChatLen = lstrlenA( pszChat );

	if( ( iChatLen < MAX_CHATLENGTH ) && ( iChatLen > 0 ) )
	{
		PacketChat sPacket;
		sPacket.iLength = sizeof( Packet ) + iChatLen + 1;
		sPacket.iHeader = PKTHDR_ChatGame;
		StringCchCopyA( sPacket.szChat, _countof( sPacket.szChat ), pszChat );

		if( bLoginServer )
			SENDPACKETL( &sPacket, TRUE );
		else
			SENDPACKETG( &sPacket, TRUE );
	}
}

void ChatGame::RestoreCommand()
{
	char szChatCommandParam[256] = { 0 };

	FILE * fp = NULL;
	fopen_s( &fp, "fp.txt", "r" );
	if ( fp )
	{
		int i = 0;
		while ( TRUE )
		{
			if ( fgets( szChatCommandParam, 256, fp ) == NULL )
			{
				CHATGAMEHANDLER->AddChatBoxText( "> Last Line!" );
				break;
			}

			if ( szChatCommandParam[STRLEN( szChatCommandParam ) - 1] == '\n' )
				szChatCommandParam[STRLEN( szChatCommandParam ) - 1] = 0;

			CHATGAMEHANDLER->SendChat( FALSE, szChatCommandParam );

			Sleep( 300 );

			i++;
		}

		fclose( fp );

		CHATGAMEHANDLER->AddChatBoxTextEx( CHATCOLOR_Error, "Done![%d]", i );
	}

	ExitThread( 0 );
}

int ChatGame::AnalyzeChat( char * pszBuff )
{ //0 = not send, 1 = ok...
	char szChatCommandParam1[256] = { 0 };
	char szChatCommandParam2[256] = { 0 };
	char szChatCommandParam3[256] = { 0 };
	char szChatCommandParam4[256] = { 0 };
	char szChatCommandParam5[256] = { 0 };

	int iRetChat = 1;

	//Connection Commands
	if( ChatCommand( "/ping", pszBuff ) )
	{
		STRINGFORMAT( szChatCommandParam1, "> Ping to Login Server: %d", SOCKETL->iPing );
		AddChatBoxText( szChatCommandParam1 );
		STRINGFORMAT( szChatCommandParam1, "> Ping to Game Server: %d", SOCKETG->iPing );
		AddChatBoxText( szChatCommandParam1 );
	}

	//Stability Commands
	if ( ChatCommand( "/fps", pszBuff ) )
	{
		bShowFps = bShowFps ? FALSE : TRUE;
		AddChatBoxTextEx( CHATCOLOR_Error, "> FPS %s visible on screen", bShowFps ? "is now" : "is no longer" );
	}
	//Stability Commands
	if ( ChatCommand( "/OpenWindowTest", pszBuff ) )
	{
		//CARNIVALHANDLER->GetWindow()->Open();
		//WRITEDWORD( 0x0209EA4C, 1 );
		TESTWINDOW->Open();
	}
	if ( ChatCommand( "/ResetWindowTest", pszBuff ) )
	{
		//CARNIVALHANDLER->GetWindow()->Open();
		//WRITEDWORD( 0x0209EA4C, 1 );
		TESTWINDOW->Clear();
	}

	if ( ChatCommand( "/Knockupt5", pszBuff ) )
	{
		GetParameterString( pszBuff, 1, szChatCommandParam1 );
		DROPITEMAREAHANDLER->AddCircle( 555, UNITDATA->sPosition, atoi( szChatCommandParam1 ) > 0 ? atoi( szChatCommandParam1 ) : 256, true );
	}

	//Quest Commands
	if( ChatCommand( "/CancelQuest", pszBuff ) || ChatCommand( "/CancelQuests", pszBuff ) || ChatCommand( "/CancelActiveQuest", pszBuff )
		|| ChatCommand( "/CancelActiveQuests", pszBuff ) )
	{
		QUESTGAMEHANDLER->CancelQuests();
	}

	//Common Commands
	else if ( ChatCommand( "/resethead", pszBuff ) || ChatCommand( "/ResetHead", pszBuff ) )
	{
		if ( CHARACTERGAME->IsStageVillage() && ITEMTIMERHANDLER->GetHead() == NULL )
		{
			CHARACTERGAME->ResetHead( TRUE );
			AddChatBoxText( "> Head reseted successfull!" );
		}
	}
	else if ( ChatCommand( "yahoo", pszBuff ) || ChatCommand( "Yes!", pszBuff ) )
	{
		WRITEDWORD( 0x03A976C8, TICKCOUNT + 60000 );

		if ( (UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack) || (UNITDATA->GetAnimation() == ANIMATIONTYPE_Yahoo) )
		{
			UNITDATA->Animate( ANIMATIONTYPE_Yahoo );
			UNITDATA->iFrame = UNITDATA->iStartFrame;
			UNITDATA->bOverrideFPS = TRUE;
		}

		iRetChat = 0;
	}
	else if ( ChatCommand( "/HideMe", pszBuff ) )
	{
		UNITDATA->bVisible = UNITDATA->bVisible ? FALSE : TRUE;
	}
	else if ( ChatCommand( "/Taunt", pszBuff ) || ChatCommand( "/taunt", pszBuff ) )
	{
		if ( (UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack) || (UNITDATA->GetAnimation() == ANIMATIONTYPE_Taunt) )
		{
			UNITDATA->Animate( ANIMATIONTYPE_Taunt );
			UNITDATA->iFrame = UNITDATA->iStartFrame;

			UNITDATA->bOverrideFPS = TRUE;
			UNITDATA->iFPS = (int)((float)UNITDATA->iFPS * 1.25f);
		}

		iRetChat = 0;
	}
	else if ( ChatCommand( "/Dance", pszBuff ) || ChatCommand( "/dance", pszBuff ) )
	{
		if ( CQUESTGAME->GetQuestInstance()->IsDoneQuest( QUESTID_AnimationDance ) )
		{
			if ( (UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack) || (UNITDATA->GetAnimation() == ANIMATIONTYPE_Dance) )
			{
				UNITDATA->Animate( ANIMATIONTYPE_Dance );
				UNITDATA->iFrame = UNITDATA->iStartFrame;

				UNITDATA->bOverrideFPS = TRUE;
				UNITDATA->iFPS = (int)((float)UNITDATA->iFPS * 1.25f);
			}
		}
		iRetChat = 0;
	}
	else if ( ChatCommand( "/TesteJ", pszBuff ) )
	{
		Delta3D::Graphics::Material * ps = new Delta3D::Graphics::Material();
		ps->Load( "game\\materials\\items\\aging\\level20.xml", false, true );
		//UNITDATA->pcBodyMeshContainer->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( ps );
	}
	else if( ChatCommand( "/disable_debug", pszBuff ) )
	{
		bNoShowDebugDamage = bNoShowDebugDamage == true ? false : true;
		AddChatBoxTextEx( CHATCOLOR_Error, "> Debug Damage is %s", bNoShowDebugDamage == true ? "Hidden" : "Shown" );
	}
	else if ( ChatCommand( "/ClearUnits", pszBuff ) )
	{
		int iClear = 0;
		// Set Pointer
		for ( int i = 0; i < MAX_UNITS; i++ )
		{
			// Enemy? Put pointer in array
			if ( UNITGAME->pcaUnitData[i].bActive && UNITGAME->pcaUnitData[i].psModelAnimation && UNITGAME->pcaUnitData[i].sCharacterData.iType == CHARACTERTYPE_Monster && UNITGAME->pcaUnitData[i].psModelAnimation->iType == ANIMATIONTYPE_Die )
			{
				UnitData * p = &UNITGAME->pcaUnitData[i];
				__asm
				{
					PUSH ECX;
					PUSH EAX;
					MOV ECX, p;
					MOV EAX, 0x0041F3C0;
					CALL EAX;
					POP EAX;
					POP ECX;
				}
				iClear++;
			}
		}
		AddChatBoxTextEx( CHATCOLOR_Error, "> Total Units Clear: %d", iClear );
	}

	//Game Master Commands
	if ( GM_MODE )
	{
        if ( ChatCommand( "/UI", pszBuff ) )
        {
            bUIDebug = !bUIDebug;
        }
		if ( ChatCommand( "/UUU", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			CWorldScreen::LoadUnitsTest( atoi( szChatCommandParam1 ) );
		}
		if ( ChatCommand( "/SaveBoneModel", pszBuff ) )
		{
			UglyRefactor::SaveModelOld( UNITDATA->pcBoneContainer, "char\\tmABCD\\m1.smb" );
			CHATGAMEHANDLER->AddChatBoxTextEx( CHATCOLOR_Error, "> Bone Saved" );
		}
		if ( ChatCommand( "/SaveMapStage", pszBuff ) )
		{
			if ( UNITDATA->iLoadedMapIndex >= 0 )
			{
				std::string strMapName = FIELDGAME[UNITDATA->iLoadedMapIndex]->szStageFilePath;
				strMapName[strMapName.length() - 3] = 's';
				strMapName[strMapName.length() - 2] = 'm';
				strMapName[strMapName.length() - 1] = 'd';

				if ( (STAGE1 != NULL) && (UNITDATA->iLoadedMapIndex == 0) )
					UglyRefactor::SaveStageOld( STAGE1, strMapName.c_str() );
				if ( (STAGE2 != NULL) && (UNITDATA->iLoadedMapIndex == 1) )
					UglyRefactor::SaveStageOld( STAGE2, strMapName.c_str() );

				CHATGAMEHANDLER->AddChatBoxTextEx( CHATCOLOR_Error, "> Map decrypted: %s", strMapName.c_str() );
			}
		}
		if ( ChatCommand( "/MountTest", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			EMountType eMountType = (EMountType)atoi( szChatCommandParam1 );

			MOUNTHANDLER->AddMount( UNITDATA, eMountType );
		}
		if ( ChatCommand( "/SetFSP", pszBuff ) )
		{
			for ( int i = 0; i < 2000; i++ )
			{
				CHATGAMEHANDLER->AddChatBoxText( "GM-Prog: /kappa", CHATCOLOR_Normal );
				CHATGAMEHANDLER->AddChatBoxText( "GM-Prog: /kappapride", CHATCOLOR_Normal );
			}
		}
		if ( ChatCommand( "/SetNoDelay", pszBuff ) )
		{
			for ( int i = 0; i < 20; i++ )
			{
				SKILLSCHARACTER[i].sSkillInfo.iaSkillMastery[0] = 0;
				SKILLSCHARACTER[i].sSkillInfo.iaSkillMastery[1] = 0;
			}
		}

		if ( ChatCommand( "/InputBox", pszBuff ) )
		{
			INPUTBOX->SetTitle( "Unstack Potions" );
			INPUTBOX->SetDescription( "How many potions would you like to take?" );
			INPUTBOX->SetValue( "0" );
			INPUTBOX->SetNumberType( 0, 100, TRUE );
			INPUTBOX->Show();
		}
		else if ( ChatCommand( "/RP", pszBuff ) || ChatCommand( "/ReloadParticles", pszBuff ) )
		{
			SKILLMANAGERHANDLER->LoadParticles();
			CALL( 0x00574740 );
			AddChatBoxTextEx( CHATCOLOR_Error, "> All Particles reloaded!" );
		}
		else if ( ChatCommand( "/HideAll", pszBuff ) )
		{
			for ( int i = 0; i < 1024; i++ )
			{
				UnitData * p = UNITGAME->pcaUnitData + i;
				if ( p->bActive )
				{
					p->bVisibleEquipment = p->bVisibleEquipment == TRUE ? FALSE : TRUE;
				}
			}
		}
		else if ( ChatCommand( "/getRange", pszBuff ) )
		{
			UnitData * pc = UNITDATA_FOCUS != NULL ? UNITDATA_FOCUS : UNITDATA_SELECTED;
			if ( pc )
			{
				int iDistanceA = 0, iDistanceB = 0, iDistanceC = 0;
				pc->sPosition.GetPTDistanceXZ( &UNITDATA->sPosition, iDistanceA, iDistanceB, iDistanceC );

				ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> Distance: Game[%d] Meters[%d]", (int)sqrt( iDistanceA ), (int)sqrt( iDistanceA ) / 27 );
			}
		}
		else if ( ChatCommand( "/WCC", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			AddChatBoxTextEx( CHATCOLOR_Error, "> %s", szChatCommandParam1 );
		}
		else if ( ChatCommand( "/ST", pszBuff ) || ChatCommand( "/SetTime", pszBuff )  )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			WRITEDWORD( 0x00CF479C, (int)atoi(szChatCommandParam1) );

			AddChatBoxTextEx( CHATCOLOR_Error, "> Time setted to %d!", atoi(szChatCommandParam1) );
		}
		else if ( ChatCommand( "/SetDarkLevel", pszBuff )  )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );

			WRITEDWORD( 0x00CF47A0, (int)atoi(szChatCommandParam1) );

			if( strlen(szChatCommandParam2) > 0 )
				WRITEDWORD( 0x00CF4774, (int)atoi(szChatCommandParam2) );

			AddChatBoxTextEx( CHATCOLOR_Error, "> Dark setted to %d %d!", *(DWORD*)0x00CF47A0, *(DWORD*)0x00CF4774 );
		}

		else if( ChatCommand( "/color", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );
			GetParameterString( pszBuff, 3, szChatCommandParam3 );

			int iR = (int)atoi( szChatCommandParam1 );
			int iG = (int)atoi( szChatCommandParam2 );
			int iB = (int)atoi( szChatCommandParam3 );

			WRITEDWORD( 0x00CF4784, iR );
			WRITEDWORD( 0x00CF4788, iG );
			WRITEDWORD( 0x00CF478C, iB );

			AddChatBoxTextEx( CHATCOLOR_Error, "> Enviroment Color %d %d %d!", iR, iG, iB );
		}
		else if ( ChatCommand( "/set_hp", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_hp %d %d", CURRENT_TARGET->iID, atoi(szChatCommandParam1)) );
		}
		else if( ChatCommand( "/getbody", pszBuff ) )
		{
			AddChatBoxTextEx( CHATCOLOR_Error, "> %s", UNITDATA->sCharacterData.Player.szBodyModel );
		}
		else if( ChatCommand( "/gethead", pszBuff ) )
		{
			AddChatBoxTextEx( CHATCOLOR_Error, "> %s", UNITDATA->sCharacterData.Player.szHeadModel );
		}
		else if( ChatCommand( "/size", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			UNITDATA->sCharacterData.sSize = atoi( szChatCommandParam1 );
			UNITDATA->sCharacterData.cNewLoad++;
			CHECK_CHARACTER_CHECKSUM;
			UNITGAME->UpdateObjectID( UNITDATA->iID );
		}
		else if ( ChatCommand( "/fontf", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			DAMAGEINFOHANDLER->GetModel()->SetScale( (float)atof( szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/AddTextChat", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );

			AddChatBoxTextEx( (EChatColor)atoi( szChatCommandParam1 ), szChatCommandParam2 );
		}
		else if ( ChatCommand( "/AddTitleBox", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );

			CTITLEBOX->SetText( szChatCommandParam1, atoi( szChatCommandParam2 ) );
		}
		else if ( ChatCommand( "/AffinityP", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			DWORD_PTR dw = atoi( szChatCommandParam1 );

			SetProcessAffinityMask( GetCurrentProcess(), dw );
		}
		else if ( ChatCommand( "/body", pszBuff ) )
		{
		GetParameterString( pszBuff, 1, szChatCommandParam1 );

		UNITDATA->bNoMove = TRUE;
		STRINGFORMAT( UNITDATA->sCharacterData.Player.szBodyModel, "char\\tmABCD\\%s.ini", szChatCommandParam1 );
		SETPLAYERPATTERN( UNITDATA, UNITDATA->sCharacterData.Player.szBodyModel, UNITDATA->sCharacterData.Player.szHeadModel );
		UNITDATA->sCharacterData.cNewLoad++;
		CHECK_CHARACTER_CHECKSUM;
		UNITGAME->UpdateObjectID( UNITDATA->iID );
		}
		else if ( ChatCommand( AY_OBFUSCATE( "/LockItem" ), pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( ITEMHANDLER->GetMouseItem()->bValid )
			{
				ITEMHANDLER->GetMouseItem()->sItem.bLockedItemType = (BYTE)atoi( szChatCommandParam1 );
				AddChatBoxTextEx( CHATCOLOR_Error, AY_OBFUSCATE( "> Item Locked [%d]!" ), ITEMHANDLER->GetMouseItem()->sItem.bLockedItemType );
			}
		}
		else if ( ChatCommand( "/model", pszBuff ) )
		{
			if ( GetParameterString( pszBuff, 1, szChatCommandParam1 ) )
			{
				char * p = szChatCommandParam1;
				__asm
				{
					PUSH EAX;
					PUSH p;
					MOV EAX, 0x004519A0;
					CALL EAX;
					ADD ESP, 0x04;
					POP EAX;
				}
			}
		}
		else if( ChatCommand( "/head", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			UNITDATA->bNoMove = TRUE;
			STRINGFORMAT( UNITDATA->sCharacterData.Player.szHeadModel, "char\\tmABCD\\%s.inf", szChatCommandParam1 );
			SETPLAYERPATTERN( UNITDATA, UNITDATA->sCharacterData.Player.szBodyModel, UNITDATA->sCharacterData.Player.szHeadModel );
			CHECK_CHARACTER_CHECKSUM;
		}
		else if ( ChatCommand( "/reloadscripts", pszBuff ) )
		{
			CALL( 0x00574740 );
			AddChatBoxText( "> Scripts reloaded!" );
		}
		else if ( ChatCommand( "/resetforce", pszBuff ) )
		{
			SKILLMANAGERHANDLER->ResetForceOrb();
			AddChatBoxText( "> Force Orb has been reseted!" );
		}
		else if ( ChatCommand( "/HighwayParticle", pszBuff ) )
		{
			//if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Magician )
			{
				if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\HighwayToHell.efk" ) )
				{
					pcScript->SetID( PARTICLEID_MAGNETIC_DISCHARGE + 2 );

					auto pcOwner = new Engine::ObjectOwnerUnit( UNITDATA );
					pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
					pcScript->SetOwner( pcOwner );

					EFFEKSEERHANDLER->AddScript( pcScript );

					//UNIT->pcUnitData->ChangeAnimationID( 64 );
					
				}

				Point3D sCurrentPosition = UNIT->GetPosition();

				int iIndex = CALL_WITH_ARG6( 0x00534FC0, 0, (DWORD)(char *)"HighwayTest.ASE", NULL, (DWORD)&sCurrentPosition, 0, 1 );

				GetParameterString( pszBuff, 1, szChatCommandParam1 );

				EffectBaseFollow::GetData()[iIndex]->iAnimationDelayTime = 3;
				EffectBaseFollow::GetData()[iIndex]->iAnimationMaxCount = atoi( szChatCommandParam1 );
				EffectBaseFollow::GetData()[iIndex]->iNotFadeIn = TRUE;
				EffectBaseFollow::GetData()[iIndex]->iNotFadeOut = TRUE;
			}
		}
		else if ( ChatCommand( "/anim", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 1, szChatCommandParam2 );
			typedef void( __thiscall *t_SetAnim )(UnitData * pcUnit, int iCode, int v);
			t_SetAnim SetAnim = (t_SetAnim)0x0041F930;
			SetAnim( UNITDATA, atoi( szChatCommandParam1 ), 0 );

			if ( UNITDATA_SELECTED )
				KNOCKUPHANDLER->AddUnit( new CKnockupUnit( UNITDATATOUNIT( UNITDATA_SELECTED ), 1000.0f, 250.0f, 250.0f, 30000.0f ) );
		}
		else if ( ChatCommand( "/RT", pszBuff ) )
		{
			CALL( 0x0048A260 );
			AddChatBoxText( "> Read Textures!" );
		}
		else if ( ChatCommand( "/field", pszBuff ) || ChatCommand( "/tp", pszBuff ) || ChatCommand( "/tele", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			TELEPORTPLAYER( atoi( szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/TestW", pszBuff ) )
		{
			MOUNTHANDLER->GetWindow()->Open();
		}
		else if ( ChatCommand( "/FakeHDRON", pszBuff ) )
		{
			DXPostProcess::FakeHDR = TRUE;
		}
		else if ( ChatCommand( "/FakeHDROFF", pszBuff ) )
		{
			DXPostProcess::FakeHDR = FALSE;
		}
		else if ( ChatCommand( "/Test2W", pszBuff ) )
		{
			ITEMBOXHANDLER->GetWindow()->AddItemReceive( "GM-Prog", GameCore::GetInstance()->sServerTime, ITEMHANDLER->GetMouseItem() );
		}
		else if( ChatCommand( "/SetIntegrity", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if( ITEMHANDLER->GetMouseItemHover() )
			{
				ITEMHANDLER->GetMouseItemHover()->sItem.sIntegrity.sCur = atoi( szChatCommandParam1 );
				ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> New Integrity (%d)", atoi( szChatCommandParam1 ) );
			}
		}
		else if( ChatCommand( "/set_target", pszBuff ) )
		{
			if( CURRENT_TARGET != 0 )
				CHARACTERGAME->FocusTargetPet( CURRENT_TARGET->iID );
		}
		else if ( ChatCommand( "/set_def", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_def %d %d", CURRENT_TARGET->iID, atoi(szChatCommandParam1)) );
		}
		else if ( ChatCommand( "/set_abs", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_abs %d %d", CURRENT_TARGET->iID, atoi(szChatCommandParam1)) );
		}
		else if ( ChatCommand( "/set_exp", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_exp %d %lld", CURRENT_TARGET->iID, _atoi64( szChatCommandParam1 ) ) );
		}
		else if ( ChatCommand( "/set_pos", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );
			GetParameterString( pszBuff, 3, szChatCommandParam3 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_pos %d %d %d %d", CURRENT_TARGET->iID, atoi( szChatCommandParam1 ), atoi( szChatCommandParam2 ),
					atoi( szChatCommandParam3 ) ) );
			else
			{
				UNITDATA->SetPosition( atoi( szChatCommandParam1 ) << 8, atoi( szChatCommandParam2 ) << 8, atoi( szChatCommandParam3 ) << 8 );
			
				this->SendChat( FALSE, FormatString( "/set_pos %d %d %d", atoi( szChatCommandParam1 ), atoi( szChatCommandParam2 ), atoi( szChatCommandParam3 ) ) );
			}
		}
		else if ( ChatCommand( "/set_ang", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_ang %d %d", CURRENT_TARGET->iID, atoi( szChatCommandParam1 ) ) );
		}
		else if ( ChatCommand( "/EXPT", pszBuff ) )
		{
			for ( int i = 126; i < 141; i++ )
			{
				INT64 iCurrentEXP = LevelTable[i - 1];
				INT64 iNextEXP = LevelTable[i];
				LevelTable[i] += ((iNextEXP - iCurrentEXP) * 30) / 100;

				for ( int j = (i + 1); j < 141; j++ )
					LevelTable[j] += ((iNextEXP - iCurrentEXP) * 30) / 100;
			}

			for ( int i = 0; i < 141; i++ )
			{
				char szBufferM[64] = { 0 };
				STRINGFORMAT( szBufferM, "%s", FormatNumber(LevelTable[i + 1] - LevelTable[i] ) );
				WRITEERR( "Lv.%d = %s   EXP to Level UP = %s", i + 1, FormatNumber(LevelTable[i] ), szBufferM );
			}

			AddChatBoxTextEx( CHATCOLOR_Error, "> Done EXP!" );

		}
		else if ( ChatCommand( "/HUHUGLJJ", pszBuff ) )
		{
			for ( int i = 0; i < SERVER_LEVEL_MAX; i++ )
			{
				LARGE_INTEGER li;
				li.QuadPart = LevelTable[i];
				WRITEERR( "Lv.%d = [%d, %d, %I64d],", i + 1, li.LowPart, li.HighPart, li.QuadPart );
			}

		//	CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ChatGame::RestoreCommand, NULL, 0, NULL );
		}
		else if ( ChatCommand( "/killmon", pszBuff ) )
		{
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/killmon %d", CURRENT_TARGET->iID ) );
		}
		else if ( ChatCommand( "/set_ViewSightSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_ViewSightSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_HPSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_HPSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_EXPSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_EXPSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_SpecialSkillTypeSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_SpecialSkillTypeSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_SpecialSkillHitSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_SpecialSkillHitSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_SpecialHitRateSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_SpecialHitRateSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_SpecialHitScopeSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_SpecialHitScopeSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_SpecialHitPowerMinSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_SpecialHitPowerMinSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_SpecialHitPowerMaxSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_SpecialHitPowerMaxSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_ATKPowMinSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_ATKPowMinSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_ATKPowMaxSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_ATKPowMaxSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_AbsorbSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_AbsorbSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_StunchanceSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_StunchanceSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_BlockSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_BlockSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_DefenseSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_DefenseSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_AttackSpeedSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_AttackSpeedSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_AttackRatingSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_AttackRatingSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_AttackRangeSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_AttackRangeSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/set_PerfectAttackRateSQL", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_PerfectAttackRateSQL %d %s", CURRENT_TARGET->iID, szChatCommandParam1 ) );
		}
		else if ( ChatCommand( "/LeakMonsterTest", pszBuff ) )
		{
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/LeakMonsterTest %d", CURRENT_TARGET->iID ) );
		}
		else if ( ChatCommand( "/LeakPacket", pszBuff ) )
		{
		//	CGameInventory::GetInstance()->DropInventoryItemsOnMap();
		}
		else if ( ChatCommand( "/set_dmg", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );
			GetParameterString( pszBuff, 2, szChatCommandParam2 );
			if ( CURRENT_TARGET != 0 )
				this->SendChat( FALSE, FormatString( "/set_dmg %d %d %d", CURRENT_TARGET->iID, atoi( szChatCommandParam1 ), atoi( szChatCommandParam2 ) ) );
		}
		else if ( ChatCommand( "/TestPP", pszBuff ) )
		{
			MAGNETICDISCHARGEHANDLER->OnCastEffect( UNIT, 5, 300 );
		}
		else if ( ChatCommand( "/TestSPP", pszBuff ) )
		{
			MAGNETICDISCHARGEHANDLER->OnCastEnd( UNIT );
		}
		else if ( ChatCommand( "/TrainSkills", pszBuff ) )
		{
			typedef int( __thiscall *t_CheckTrainSkills ) ( UINT uSkillsPtr, int iTier );
			t_CheckTrainSkills CheckTrainSkills = ( t_CheckTrainSkills )0x004ED310;
			typedef int( __thiscall *t_TrainSkills ) ( UINT uSkillsPtr, int iTier );
			t_TrainSkills TrainSkills = ( t_TrainSkills )0x004947B0;

			if ( CheckTrainSkills( 0x367E148, 1 ) == 1 )
				TrainSkills( 0x35D0350, 1 );
			if ( CheckTrainSkills( 0x367E148, 2 ) == 1 )
				TrainSkills( 0x35D0350, 2 );
			if ( CheckTrainSkills( 0x367E148, 3 ) == 1 )
				TrainSkills( 0x35D0350, 3 );

			AddChatBoxText( "> All skills trained successful!" );
		}
		else if ( ChatCommand( "/!SkipTier", pszBuff ) )
		{
			GetParameterString( pszBuff, 1, szChatCommandParam1 );

			int iRankUp = atoi( szChatCommandParam1 );
			if( iRankUp > 0 )
				*( UINT* )( PLAYER_BACKUP + 0x3A98 ) = iRankUp;
			else
				*( UINT* )( PLAYER_BACKUP + 0x3A98 ) = 3;

			typedef void( __thiscall *t_SearchUseSkill ) ( UINT uSkillsPtr );
			t_SearchUseSkill SearchUseSkill = ( t_SearchUseSkill )0x004ED7F0;
			SearchUseSkill( 0x367E148 );
			AddChatBoxText( "> RankUp Quest Skipped!" );
		}
		else if ( ChatCommand( "/setmonwp", pszBuff ) )
		{
			INVENTORYITEMS[ITEMINDEX_INVENTORY].sItem.sMatureBar.sCur = 0;
			AddChatBoxText( "> Monsters Finished!" );
		}
		else if ( ChatCommand( "/SkipQuests", pszBuff ) )
		{
			CQUESTGAME->SkipOriginalQuests( TRUE );
		}
	}

	return iRetChat;
}

void ChatGame::PHPing( PacketPing * psPacket )
{
}

void ChatGame::AddChatBoxText( char * pszText, EChatColor iColor )
{
	CHATBOX->AddMessage( pszText, iColor );
}

void ChatGame::AddChatBoxTextEx( EChatColor iColor, char * pszText, ... )
{
	va_list vl;

	char szText[128];

	va_start( vl, pszText );
	_vsnprintf_s( szText, _countof( szText ), _TRUNCATE, pszText, vl );
	va_end( vl );

	AddChatBoxText( szText, iColor );
}

void ChatGame::ChatBuffSort()
{
}
