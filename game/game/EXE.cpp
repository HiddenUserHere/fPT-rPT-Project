#include "stdafx.h"
#include "EXE.h"

std::vector<EXERenderCacheData*> EXERenderCache::vRender;

extern DropItemView saItemsStage[1024];

UI::Image * pcLowHPWarningImage = nullptr;
UI::Image * pcBossFixedPositionImages[4] = { 0 };

void EXERenderCache::FinalRender()
{
}

void EXERenderCache::FinalRenderHandler( PTMesh * pcThis )
{
	EXERenderCacheData * psRenderData = new EXERenderCacheData;
	
	CopyMemory( &psRenderData->cMesh, pcThis, sizeof( PTMesh ) );
	CopyMemory( &psRenderData->cRender, EXERender::GetRender(), sizeof( PTMesh ) );

	vRender.push_back( psRenderData );
}

void EXERenderCache::RenderCacheData()
{
}

NAKED BOOL AddUnitQueueID( int iID )
{
	JMP( 0x0061E640 );
}

void RenderDropItem( DropItemView * ps )
{
	if ( ps->pcModel && ps->pcModel->pcNewModel )
	{
		ps->sSelfPosition = ps->sPosition;
		ps->sSelfPosition.iY += (6 << 8);

		Point3D sAngle = ps->sAngle;
		if ( ItemID( ps->iItemID ).ToItemBase() == ITEMBASE_Weapon )
			sAngle.iX = PTANGLE_90;

		int iScale = 256;

		//SoD scale items
		if ( MAP_ID == MAPID_Bellatra )
		{
			if ( (ItemID( ps->iItemID ).ToItemType() == ITEMTYPE_ManaPotion) || (ItemID( ps->iItemID ).ToItemType() == ITEMTYPE_Bellatra) )
			{
				iScale = (TICKCOUNT >> 2) & 0x1FF;
				if ( (iScale & 0x100) != 0 )
					iScale = 256 - (iScale & 0xFF);

				if ( ItemID( ps->iItemID ).ToItemType() == ITEMTYPE_ManaPotion )
					iScale *= 3;
			}
		}

		float fScale = (float)iScale / 256;

		ps->pcModel->pcNewModel->scaling = Math::Vector3( fScale );
		ps->pcModel->SetPosition( &ps->sSelfPosition, &sAngle );

		ps->pcModel->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( GRAPHICENGINE->GetDropEffectMaterial() );
		ps->pcModel->pcNewModel->SetFrame( RandomI( 1, 1000 ) );
		ps->pcModel->pcNewModel->Render();

		ps->pcModel->pcNewModel->scaling = Math::Vector3( 1.0f );
	}
}

void ClearRenderDropView( DropItemView * ps )
{
	CALLT( pfnClearRenderDropView, (DWORD)ps );
}

void RenderCharacterHP( UnitData * pcUnitData, int iX, int iY )
{
	CALLT_WITH_ARG2( 0x00421B30, (DWORD)pcUnitData, iX, iY );
}

void RenderCharacterChangingBar( UnitData * pcUnitData, int iX, int iY, int iMin, int iMax )
{
	CALLT_WITH_ARG4( 0x0042B320, (DWORD)pcUnitData, iX, iY, iMin, iMax );
}

void RenderShadowMap()
{
	if ( SETTINGSHANDLER->GetModel()->GetSettings().bDynamicShadows == FALSE )
		return;

	for( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData* pc = UNITGAME->pcaUnitData + i;

		//Character Displayed?
		if( pc->bActive && pc->iTransparencySelf == 0 )
			pc->Render();
	}

	if( UNITDATA )
		UNITDATA->Render();
}

BOOL Render3D( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	int iSkillDisplay = UNITDATA->eSkillDisplayFlag;

	//Reset
	UNITDATA->eSkillDisplayFlag = 0;

	//Render Success Counter
	int iRenderedCounter = 0;

	//Instance
	Skill * psaSkill = (Skill*)TIMERSKILLHANDLER->GetPointerSkillTimer();


	//Z Distance from selected target
	int iSelectedZ = 65536 * 16384;

	for ( int i = 0; i < 80; i++ )
	{
		Skill * ps = psaSkill + i;

		if ( ps->bActive )
		{
			if ( ps->iID )
			{
				switch ( ps->iID )
				{
					case SKILLID_ExtremeShield:
						UNITDATA->eSkillDisplayFlag |= SKILLMASK_ExtremeShield;
						break;

					case SKILLID_PhysicalAbsorb:
						UNITDATA->eSkillDisplayFlag |= SKILLMASK_PhysicalAbsorb;
						break;

					case SKILLID_Maximize:
					case SKILLID_Automation:
						UNITDATA->eSkillDisplayFlag |= SKILLMASK_Automation;
						break;

					case SKILLID_CriticalHit:
					case SKILLID_Zenith:
						UNITDATA->eSkillDisplayFlag |= SKILLMASK_CriticalHit;
						break;

					case SKILLID_HolyBody:
						UNITDATA->eSkillDisplayFlag |= SKILLMASK_PhysicalAbsorb;
						UNITDATA->eSkillDisplayFlag |= SKILLMASK_HolyBody;
						break;

					case SKILLID_SparkShield:
					case SKILLID_DivineShield:
						UNITDATA->eSkillDisplayFlag |= SKILLMASK_SparkShield;
						break;

					case SKILLID_GodlyShield:
					case SKILLID_FrostJavelin:
					case SKILLID_SummonMuspell:
						UNITDATA->eSkillDisplayFlag |= SKILLMASK_GodlyShield;
						break;

					default:
						break;
				}
			}
		}
	}


	//Stop Skill Sounds if it finished...
	{
		if ( (UNITDATA->eSkillDisplayFlag & SKILLMASK_PhysicalAbsorb) == 0 && (iSkillDisplay & SKILLMASK_PhysicalAbsorb) != 0 )
			SKILLSOUND_STOP( SKILLSOUND_PhysicalAbsorb3 );

		if ( (UNITDATA->eSkillDisplayFlag & SKILLMASK_Automation) == 0 && (iSkillDisplay & SKILLMASK_Automation) != 0 )
			SKILLSOUND_STOP( SKILLSOUND_Automation3 );

		if ( (UNITDATA->eSkillDisplayFlag & SKILLMASK_HolyBody) == 0 && (iSkillDisplay & SKILLMASK_HolyBody) != 0 )
			SKILLSOUND_STOP( SKILLSOUND_PhysicalAbsorb3 );
	}

	//Set Camera of Render
	RENDERER->SetCameraPosition( iX, iY, iZ, iAX, iAY, iAZ );

	
	//Set Dynamic Lights to the map
	if ( STAGE1 )
		SetStageDynamicLight( STAGE1, iX, iY, iZ );
	if ( STAGE2 )
		SetStageDynamicLight( STAGE2, iX, iY, iZ );

	//New Camera Here
	DX::Lens_ptr pLens = std::make_shared<DX::Lens>();
	pLens->SetFOVY( D3DXToDegree( (PI / 4.4f) ) );
	pLens->SetRatio( (FLOAT)RESOLUTION_WIDTH / (FLOAT)RESOLUTION_HEIGHT );
	pLens->SetZNear( 20.0f );
	pLens->SetZFar( 4000.0f );
	GRAPHICENGINE->GetSRenderer()->SetLens( pLens );

	DX::Camera_ptr pCamera = std::make_shared<DX::Camera>();

	X3D::Vector3 vEye;
	vEye.x = (float)iX / 256.0f;
	vEye.y = (float)iY / 256.0f;
	vEye.z = (float)iZ / 256.0f;
	pCamera->SetEye( vEye );

	X3D::Vector3 vLookAtPT;
	vLookAtPT.x = (float)UNITDATA->sPosition.iX / 256.0f;
	vLookAtPT.y = ((float)UNITDATA->sPosition.iY / 256.0f) + 32.0f;
	vLookAtPT.z = (float)UNITDATA->sPosition.iZ / 256.0f;
	pCamera->SetLookAt( vLookAtPT );

	X3D::Vector3 vUp;
	vUp.x = 0.0f;
	vUp.y = 1.0f;
	vUp.z = 0.0f;
	pCamera->SetUp( vUp );
	GRAPHICENGINE->GetSRenderer()->SetCamera( pCamera );

	Effekseer::Matrix44 mV;
	CopyMemory( &mV, &GRAPHICENGINE->GetSRenderer()->GetViewMatrix(), sizeof( Effekseer::Matrix44 ) );
	Effekseer::Matrix44 mP;
	CopyMemory( &mP, &GRAPHICENGINE->GetSRenderer()->GetProjectionMatrix(), sizeof( Effekseer::Matrix44 ) );

	EFFEKSEERHANDLER->GetRenderer()->SetProjectionMatrix( (Effekseer::Matrix44 &)GRAPHICENGINE->GetCamera()->Projection() );

	EFFEKSEERHANDLER->GetRenderer()->SetCameraMatrix( (Effekseer::Matrix44 &)GRAPHICENGINE->GetCamera()->View() );

	
    auto v = Delta3D::Math::Vector3( 0.25f, 0.5f, 0.75f );

    GRAPHICS->GetRenderer()->SetGlobalLightDirection( v );

	//Set Camera of Character
	CAMERACHARACTER( iX, iY, iZ, iAX, iAY, iAZ );

	int iRestarterTime = RESTARTCOUNTER;
	if ( UNITDATA->bNoDeath )
		iRestarterTime = (int)UNITDATA->bNoDeath;

	//Restart Color
	if ( iRestarterTime > 0 && (iRestarterTime & 0x10) == 0 )
	{
		//Keep it safe...
		int R = RENDERER->r;
		int B = RENDERER->b;

		RENDERER->r += 80;
		RENDERER->b += 80;
		
		UNITDATA->Render();

		RENDERER->r = R;
		RENDERER->b = B;
	}
	else 
	{
		//Update Vanish need to render again??? ugly...
		if ( UNITDATA->iVanishTimeLeft <= 0 && UNITDATA->iBlindSkill <= 0 )
			UNITDATA->Render();
	}

	//Clan Master of BC? Set Crown...
	if ( CASTLEMASTERID && UNITDATA->sCharacterData.iClanID )
		SetClanMaster( CASTLEMASTERID );

	//Reset Target
	UnitData * pcUnitDataSelected = NULL;

	std::vector<UnitData *> vUnitsDataBroyaleGlow;

	AKATSUKIHANDLER->UpdateVerify( true );

	AKATSUKIHANDLER->VerifyUnit( UNITDATA );

	CWorldScreen::UpdateTestUnits();

	CWorldScreen::RenderTestUnits();

	//All Characters Render...
	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITGAME->pcaUnitData + i;
		Unit * pcUnitRender = UNITDATATOUNIT( pc );

		//Character Displayed?
		if ( pc->bActive && pc->iTransparencySelf == 0 )
		{
			if ( BATTLEROYALEHANDLER->InBattle() )
			{
				if ( BATTLEROYALEHANDLER->IsEnemy( pc ) == false )
					vUnitsDataBroyaleGlow.push_back( pc );
			}

			//Akatsuki
			AKATSUKIHANDLER->VerifyUnit( pc );

			if ( RENDERDISTANCE_LIMIT )
			{
				//Far from Distance? next Character...
				if ( UNITDATA->sPosition.WithinPTDistance( &pc->sPosition, DISTANCE_UNIT_FAR ) == FALSE )
					continue;
			}

			int R = RENDERER->r;
			int G = RENDERER->g;
			int B = RENDERER->b;

			//Update Pet Skill
			if ( UNITDATA->iPetID && pc->sCharacterData.sHP.sCur > 0 )
			{
				//Scout Hawk Buff?
				if ( UNITDATA->iPetID == SKILLID_ScoutHawk )
				{
					//Note: Get Attack Rating from Scout Hawk

					//Update Character Color based on Dark
					if ( GetDamageFromAreaSkill( UNITDATA, pc, 0, 0, 0 ) > 0 )
					{
						int iColor = DARKCOLOR_AMOUNT >> 2;

						RENDERER->r += iColor + 40;
						RENDERER->g += iColor + 40;
						RENDERER->b += iColor + 100;
					}
				}
			}

			DWORD dwOutlineEffectColor = 0;

			//Outline Update
			if ( UNITDATA_SELECTED && UNITDATA_SELECTED == pc )
			{
				RENDERER->r = R + 40;
				RENDERER->g = G + 60;
				RENDERER->b = B + 60;

				//Red
                dwOutlineEffectColor = RGBA( 255, 255, 30, 0 );

				//Purple for PK Mode
				if ( pcUnitRender && pcUnitRender->bPvPMode )
					dwOutlineEffectColor = RGBA( 255, 102, 0, 204 );

				//Green
				if ( UNITDATA_SELECTED->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC )
					dwOutlineEffectColor = RGBA( 255, 0, 255, 100 ); //RGBA( 255, 160, 32, 240 ); = Halloween

				//White
				if ( UNITDATA_SELECTED->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Player )
					dwOutlineEffectColor = RGBA( 255, 255, 255, 255 );

				//Compute Distance...
				int a = 0, b = 0, c = 0, d = 0;

				Point3D sPositionC = Point3D( iX, iY, iZ );

				UNITDATA_SELECTED->sPosition.GetPTDistanceXZY( &sPositionC, a, b, c, d );

				int iDistance = (int)sqrt( a );
			}

			if ( pc->bRenderLast == FALSE && pc->iIceOverlay == 0 && pc->iTransparency == 0 && (pc->iVanishTimeLeft <= 0 && pc->iBlindSkill <= 0) )
			{
				//Athanasia Color
				if ( pc->bNoDeath && ((int)pc->bNoDeath & 0x10) == 0 )
				{
					RENDERER->r += 80;
					RENDERER->b += 80;
				}

				if ( (dwOutlineEffectColor > 0) && SETTINGSHANDLER->GetModel()->GetSettings().bGlow )
				{
					if ( DX::cSelectGlow1.Begin( dwOutlineEffectColor, TRUE ) )
					{
						RenderUnitData( pc, true, true, true );
						DX::cSelectGlow1.End( TRUE );

						if ( DX::cSelectGlow1.Begin( dwOutlineEffectColor, FALSE ) )
						{
							RenderUnitData( pc, true, true, true );
							DX::cSelectGlow1.End( FALSE );
						}
					}
				}

				RenderUnitData( pc, true, true, true );
			}
			else
			{
				/*
				if ( (dwOutlineEffectColor > 0) && SETTINGSHANDLER->GetModel()->GetSettings().bGlow )
				{
					if ( DX::cSelectGlow1.Begin( dwOutlineEffectColor, TRUE ) )
					{
						CALLT( 0x00420AC0, (DWORD)pc );
						DX::cSelectGlow1.End( TRUE );

						if ( DX::cSelectGlow1.Begin( dwOutlineEffectColor, FALSE ) )
						{
							CALLT( 0x00420AC0, (DWORD)pc );
							DX::cSelectGlow1.End( FALSE );
						}
					}
				}

				//Fake Render?
				CALLT( 0x00420AC0, (DWORD)pc );
				*/
			}

			//Reset Color and Glow
			RENDERER->r = R;
			RENDERER->g = G;
			RENDERER->b = B;

			//Rendered?
			if ( pc->bRendered )
			{
				//Increment to Handle it later...
				iRenderedCounter++;

				int iZP = pc->sRenderPoint.iZ;

				RECT * psRect2D = &pc->rRenderRectangle;

				//In distance and inside 2D Plane and select on mouse?
				if ( ((pc->sCharacterData.sHP.sCur > 0) || (GM_MODE && (pc->GetAnimation() != ANIMATIONTYPE_Die))) && iZP > (96 << 8) && iZP < iSelectedZ &&
					 psRect2D->left < MOUSEHANDLER->GetPosition()->iX && psRect2D->right > MOUSEHANDLER->GetPosition()->iX &&
					 psRect2D->top < MOUSEHANDLER->GetPosition()->iY && psRect2D->bottom > MOUSEHANDLER->GetPosition()->iY )
				{
					if ( (pc->dwLastActiveTime + 7000) > TICKCOUNT )
					{
						//Set new target
						iSelectedZ = iZP;
						pcUnitDataSelected = pc;
					}
				}
			}
		}
	}

	if ( vUnitsDataBroyaleGlow.size() > 0 )
	{
		DWORD dwColor = BATTLEROYALEHANDLER->GetTeam() == BattleRoyale::EPlayerTeam::PLAYERTEAM_White ? D3DCOLOR_XRGB( 255, 255, 255 ) : D3DCOLOR_XRGB( 0, 255, 0 );
		if ( DX::cSelectGlow4.Begin( dwColor, TRUE ) )
		{
			for ( auto pc : vUnitsDataBroyaleGlow )
				pc->Render();
			DX::cSelectGlow4.End( TRUE );

			if ( DX::cSelectGlow4.Begin( dwColor, FALSE ) )
			{
				for ( auto pc : vUnitsDataBroyaleGlow )
					pc->Render();
				DX::cSelectGlow4.End( FALSE );
			}
		}
	}
	vUnitsDataBroyaleGlow.clear();

	int iLastRenderedCounter = iRenderedCounter;

	AKATSUKIHANDLER->UpdateVerify( false );

	CARAVANHANDLER->UpdateCaravanParticle( UNIT );

	//Press A key?
	if ( ISPRESSEDKEY('A') )
	{
		//Reset target
		UNITDATA_SELECTED	= NULL;
		iSelectedZ			= 65536 * 16384;

		pcUnitDataSelected	= NULL;
	}

	int iRenderedItems = 0;

	RECT sRect;
	Point2D sPosition2D;

	DropItemView * psItem = NULL;

	RECT sRectS;
	Point2D sPosition2DS;

	DROPITEMAREAHANDLER->Render3D();

	for ( int i = 0; i < 1024; i++ )
	{
		DropItemView * ps = saItemsStage + i;

		if ( ps->bActive )
		{
			//GM Debug or is an Item?
			if ( (ps->iItemID || DEBUG_GM) && ITEMHANDLER->CanViewDropItem( ps ) )
			{
				//It's a gold item? So format the number
				if( ps->iItemID == ITEMID_Gold && !ps->bFormatted )
					ITEMHANDLER->FormatDropItemGold( ps );

				if ( (ITEMVIEW_SELECTED == ps) && SETTINGSHANDLER->GetModel()->GetSettings().bGlow )
				{
					DWORD dwColor = D3DCOLOR_XRGB( 255, 255, 0 );

					if ( DX::cSelectGlow2.Begin( dwColor, TRUE ) )
					{
						RenderDropItem( ps );
						DX::cSelectGlow2.End( TRUE );

						if ( DX::cSelectGlow2.Begin( dwColor, FALSE ) )
						{
							RenderDropItem( ps );
							DX::cSelectGlow2.End( FALSE );
						}
					}
				}

				//Render it
				RenderDropItem( ps );


				//Set Camera of Render
				RENDERER->SetCameraPosition( iX, iY, iZ, iAX, iAY, iAZ );

				//Get 2D Plane of item
				int iZP = RENDERER->GetRectangle2D( ps->sPosition.iX, ps->sPosition.iY + (16 << 8), ps->sPosition.iZ, 32 << 8, 32 << 8, &sRect, &sPosition2D );

				//Set to Item
				ps->sRenderPosition.iX = sPosition2D.iX;
				ps->sRenderPosition.iY = sPosition2D.iY;
				ps->sRenderPosition.iZ = iZP;

				//Item on plane?
				if ( iZP > 0 && iZP < iSelectedZ &&
					sRect.left < MOUSEHANDLER->GetPosition()->iX && sRect.right > MOUSEHANDLER->GetPosition()->iX &&
					sRect.top < MOUSEHANDLER->GetPosition()->iY && sRect.bottom > MOUSEHANDLER->GetPosition()->iY )
				{
					//Set it selected...
					iSelectedZ	= iZP;
					psItem		= ps;
					CopyMemory( &sRectS, &sRect, sizeof( RECT ) );
					sPosition2DS = sPosition2D;
				}
			}

			//Check if item data keeps up
			if ( (ps->dwLastUpdate + 10000) < TICKCOUNT )
				ClearRenderDropView( ps );
		}
	}

	BOOL bIgnoreMouse = FALSE;

	//Target selected?
	if ( pcUnitDataSelected )
	{
		//Set It
		UNITDATA_SELECTED	= pcUnitDataSelected;

		psItem				= NULL;
	}
	else //Otherwise, reset
		UNITDATA_SELECTED	= NULL;

	//Item selected?
	if ( psItem )
	{
		RECT * psSelectR = (RECT*)0x00CF4208;
		Point2D * psSelect2D = (Point2D*)0x00CF3FBC;

		//Set It
		CopyMemory( psSelectR, &sRectS, sizeof( RECT ) );
		CopyMemory( psSelect2D, &sPosition2DS, sizeof( Point2D ) );

		ITEMVIEW_SELECTED = psItem;
	}
	else //Otherwise, reset
		ITEMVIEW_SELECTED = NULL;

	bool bCanMouseHoverTarget = true;
	if ( SHOP_OPEN || MOUSEHANDLER->IsNoMove() || MOUSEHANDLER->IsDisabled() )
	{
		bCanMouseHoverTarget = false;

		if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() && PERSONALSHOPHANDLER->GetWindow()->IsSeller() )
			bCanMouseHoverTarget = true;
	}
	if ( !bCanMouseHoverTarget )
	{
		UNITDATA_SELECTED = NULL;
		ITEMVIEW_SELECTED = NULL;

		if ( MOUSE_ICON == EMOUSEICON_Attack || MOUSE_ICON == EMOUSEICON_Talk || MOUSE_ICON == EMOUSEICON_GetItem1 || MOUSE_ICON == EMOUSEICON_GetItem2 )
			MOUSE_ICON = EMOUSEICON_Default;

		return TRUE;
	}

	if ( GLAMOROUSBRUSHHANDLER->IsGlamorousMouse() == false )
		MOUSE_ICON = EMOUSEICON_None;

	if ( (ITEMHANDLER->GetMouseItem()->bValid == FALSE) && (MOUSE_ICON == EMOUSEICON_None) )
	{

		MOUSE_ICON = EMOUSEICON_Default;

		if ( UNITDATA_SELECTED )
		{
			if ( UNITDATA_SELECTED->sCharacterData.iType == CHARACTERTYPE_Monster )
				MOUSE_ICON = EMOUSEICON_Attack;
			else if ( UNITDATA_SELECTED->sCharacterData.iType == CHARACTERTYPE_NPC )
				MOUSE_ICON = EMOUSEICON_Talk;
		}

		if ( ITEMVIEW_SELECTED )
		{
			if ( ISMOUSELEFT_CLICKED )
				MOUSE_ICON = EMOUSEICON_GetItem2;
			else
				MOUSE_ICON = EMOUSEICON_GetItem1;
		}
	}

	return TRUE;
}

NAKED BOOL Render3DAlpha()
{
	JMP( pfnRender3DAlpha );
}

void WINAPI Render( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	if ( UNITDATA )
		MOUNTHANDLER->Update( UNITDATA );

	POLYCOUNT = 0;

	//Render Sight
	if ( RENDERER->iGameFieldView )
	{
		VIEWPOINTSIGHT = 38 * 64 + RENDERSIGHTPOSITION;
		ZDISTANCEMIN = VIEWPOINTSIGHT - (20 * 64 + RENDERSIGHTPOSITION / 4);
	}

	//Y Check?
	if ( DEBUG_USER == FALSE )
	{
		Stage * pcStage = STAGE2 != NULL ? STAGE2 : STAGE1;

		if ( pcStage )
		{
			int iMapY = pcStage->GetHighestPoint( iX, iZ );

			if ( iMapY > iY )
			{
				iY = iMapY;

				iY += (8 << 8);
			}
		}
	}

	if( GRAPHICENGINE->GetTerrainShader() )
	{
		GRAPHICENGINE->GetTerrainShader()->SetMatrix( "Projection", GRAPHICENGINE->GetCamera()->Projection().Get() );
		GRAPHICENGINE->GetTerrainShader()->SetTechnique( "Terrain" );
		GRAPHICENGINE->GetTerrainShader()->SetFloat( "FogEnd", 1500.f );
		GRAPHICENGINE->GetTerrainShader()->SetInt( "NumActiveLights", 0 );

		GRAPHICS->GetRenderer()->SetFogEnd( 1500.f );
	}

	//Video Ram
	CALL( 0x00457990 );
	
	//Update Color...
	{
		RENDERER->r = (-DARKCOLOR_AMOUNT) + COLOR_R;
		RENDERER->g = (-DARKCOLOR_AMOUNT) + COLOR_G;
		RENDERER->b = (-DARKCOLOR_AMOUNT) + COLOR_B;
		RENDERER->a = BRIGHTNESS;

        int iAdd = ((RENDERER->r < -64) || (RENDERER->g < -64) || (RENDERER->b < -64)) ? abs( (RENDERER->r + RENDERER->g + RENDERER->b) / 3 ) : 0;
        if ( iAdd > 128 )
            iAdd = 128;


		//ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "%d %d %d %d %d", RENDERER->r, RENDERER->g, RENDERER->b, RENDERER->a, iAdd );
		GRAPHICS->GetRenderer()->SetAmbientColor( Math::Color( RENDERER->r + iAdd, RENDERER->g + iAdd, RENDERER->b + iAdd, RENDERER->a) );
	}

	iY += 32 << 8;

	//Y Distance?
	if ( ANGLE_X <= 40 )
	{
		if ( GAME_DISTANCE <= 100 )
			iY -= (110 - GAME_DISTANCE) << 8;
	}

	ACTIONMODE = FALSE;

	TRACETARGET->iX = iX;
	TRACETARGET->iY = iY;
	TRACETARGET->iZ = iZ;

	//Camera Target Update... auto mode
	if ( ISAUTOCAMERA )
	{
		UpdateMovementCamera();

		iX = TRACECAMERA->iX;
		iY = TRACECAMERA->iY;
		iZ = TRACECAMERA->iZ;
	}

	//Compute Y Player
	int iYBase = UNITDATA->sPosition.iY + (32 << 8);

	if ( ANGLE_X <= 40 )
	{
		if ( GAME_DISTANCE <= 100 )
			iYBase += (100 - GAME_DISTANCE) << 8;
	}

	BOOL bWalking = FALSE;

	//Camera
	EXECameraTrace sCamera;

	if ( UNITDATA && UNITDATA->iLoadedMapIndex >= 0 && FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Distorted )
	{
		//Update Camera
		SetupCamera( &sCamera, &iX, &iY, &iZ );

		iAX = sCamera.sAngle.iX;
		iAY = sCamera.sAngle.iY;

		//Set Camera Position
		RENDERER->SetCameraPosition( iX, iY, iZ, &sCamera.sRotationMatrix );
		GRAPHICENGINE->GetCamera()->SetPosition( Math::Vector3( iX / 256.0f, iY / 256.0f, iZ / 256.0f ), Math::Vector3( iX / 256.0f, iY / 256.0f, UNITDATA->sPosition.iZ / 256.0f ) );

		ACTIONMODE = TRUE;
	}
	else if ( DEBUG_USER == FALSE )
	{
		//Setup Camera Matrix
		SetupCameraMatrix( &sCamera, iX, iY, iZ, UNITDATA->sPosition.iX, iYBase, UNITDATA->sPosition.iZ );

		iAX = sCamera.sAngle.iX;
		iAY = sCamera.sAngle.iY;

		//Set Camera Position
		RENDERER->SetCameraPosition( iX, iY, iZ, &sCamera.sRotationMatrix );
		GRAPHICENGINE->GetCamera()->SetPosition( Math::Vector3( iX / 256.0f, iY / 256.0f, iZ / 256.0f ), Math::Vector3( UNITDATA->sPosition.iX / 256.0f, iYBase / 256.0f, UNITDATA->sPosition.iZ / 256.0f ) );
	}

	RENDERER->ClearLight();

	if ( DARKCOLOR_AMOUNT > 0 )
	{
		if ( UNITDATA->iLoadedMapIndex >= 0 )
		{
			int iLight = DARKCOLOR_AMOUNT + (DARKCOLOR_AMOUNT >> 2);

			//Dungeon Map?
			if ( FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Dungeon )
			{
				DARKLIGHTCOLOR_RANGE = 400 << 8;

				iLight = DARKCOLOR_AMOUNT;
			}
			else
			{
				DARKLIGHTCOLOR_RANGE = 260 << 8;
			}

			if ( FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Town )
			{
				//Nothing...
			}
			else
			{
				if ( FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Distorted )
				{
					DARKLIGHTCOLOR_RANGE = 260 << 8;

					DARKCOLOR_AMOUNT = 180;

					iLight = DARKCOLOR_AMOUNT;

					//Add Dynamic Light
					RENDERER->AddDynamicLight( UNITDATA->sPosition.iX, UNITDATA->sPosition.iY + (32 << 8), UNITDATA->sPosition.iZ, iLight, iLight, iLight, 0, DARKLIGHTCOLOR_RANGE );
				}
				else
				{
					//Add Dynamic Light
					RENDERER->AddDynamicLight( UNITDATA->sPosition.iX, UNITDATA->sPosition.iY + (32 << 8), UNITDATA->sPosition.iZ, iLight, iLight, iLight, 0, DARKLIGHTCOLOR_RANGE );
				}
			}
		}
	}

	GRAPHICS->GetRenderer()->SetRenderShadowMap( SETTINGSHANDLER->GetModel()->GetSettings().bDynamicShadows ? true : false );

	if ( SETTINGSHANDLER->GetModel()->GetSettings().bDynamicShadows )
		GRAPHICS->GetRenderer()->RenderShadowMap();

	GRAPHICENGINE->Begin3D();

	//Update to render the lights
	SetDynamicLights();

	//Render Sky
	RenderSky( iX, iY, iZ, iAX, iAY, iAZ );

	EXERender::SetRenderWithShaders( TRUE );

	if ( EXERender::IsRenderingWithShaders() )
	{
		if ( GRAPHICENGINE->GetTerrainShader() )
		{
			GRAPHICS->GetRenderer()->PrepareLights( GRAPHICENGINE->GetTerrainShader() );
			GRAPHICS->GetRenderer()->PrepareShadowMap( GRAPHICENGINE->GetTerrainShader() );
		}
	}

	POLYCOUNT = RenderStage( iX, iY, iZ, iAX, iAY, iAZ );

	EXERender::SetRenderWithShaders( FALSE );


	//Update Fog Render State
	RENDERER->UpdateFogState( TRUE );

	//Apply Lights on next render
	RENDERER->bApplyObjectLight = TRUE;

	WRITEDWORD( 0x04B0D9E0, FALSE );

	Render3D( iX, iY, iZ, iAX, iAY, iAZ );

	//Not Apply lights on next render
	RENDERER->bApplyObjectLight = FALSE;

	/*
	if ( MAP_ID == 3 )
	{
		static DX::Terrain_ptr pTerrain = nullptr;

		if ( pTerrain == nullptr )
		{
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ENABLEADAPTIVETESSELLATION, TRUE );
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_MAXTESSELLATIONLEVEL, TRUE );

			GetGraphicsDevice()->Get()->SetNPatchMode( 8.0f );

			pTerrain = GetGraphicsDevice()->GetTerrainFactory()->MakeTerrain( "Field\\Ricarten\\village-2.smd" );
		}
		else
		{
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ENABLEADAPTIVETESSELLATION, FALSE );

			FLOAT floatTesselationFactor = 15.0f;
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_POINTSIZE, 0x7FA03001 );
			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_MAXTESSELLATIONLEVEL, *(DWORD *)(&floatTesselationFactor) );

			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ENABLEADAPTIVETESSELLATION, TRUE );

			GetGraphicsDevice()->Get()->SetNPatchMode( 8.0f );

			GRAPHICENGINE->GetSRenderer()->DrawTerrain( pTerrain );

			GetGraphicsDevice()->Get()->SetRenderState( D3DRS_ENABLEADAPTIVETESSELLATION, FALSE );
		}
	}
	*/
	BATTLEROYALEHANDLER->Render3D();

	RENDERER->bApplyObjectLight = TRUE;

	Render3DAlpha();


	//Clear Lights
	RENDERER->bApplyObjectLight = FALSE;
	RENDERER->iObjectLightCount = 0;

	RENDERER->UpdateFogState( FALSE );

	RENDERER->ClearLight();
	RENDERER->r = 0;
	RENDERER->g = 0;
	RENDERER->b = 0;
	RENDERER->a = 0;

	UpdateUITextures();

	RenderEffects( iX, iY, iZ, iAX, iAY, iAZ );

	Render3DEngine( iX, iY, iZ, iAX, iAY, iAZ );

	GRAPHICENGINE->End3D();

	RenderModelShadow( iX, iY, iZ, iAX, iAY, iAZ );

	RenderModel2D( iX, iY, iZ, iAX, iAY, iAZ );

	//Run Mode
	if ( FOLLOWMODE_ACTION == FALSE && ISMOUSELEFT_CLICKED == 0 || UNITDATA->sCharacterData.sSP.sCur > (UNITDATA->sCharacterData.sSP.sMax >> 2) )
	{
		UNITDATA->bRunMode = RUN_UIMODE;

		if ( ACTIONMODE )
			UNITDATA->bRunMode = ACTIONMODE_MOVE;
	}
	else
	{
		if ( UNITDATA->sCharacterData.sSP.sCur == 0 )
			UNITDATA->bRunMode = FALSE;
	}

	RENDERER->CloseCamera();

	if( !pcLowHPWarningImage )
		pcLowHPWarningImage = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\lowhpwarning.png" );

	//Low HP Warning
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		if ( CHARACTERGAME->GetCurrentHP() > 0 && UNITDATA->GetAnimation() != ANIMATIONTYPE_Die )
		{
			float fDelta = 0.5f - (float)CHARACTERGAME->GetCurrentHP() / (float)UNITDATA->sCharacterData.sHP.sMax;
			if ( fDelta > 1.0f )
				fDelta = 1.0f;
			else if ( fDelta < 0.0f )
				fDelta = 0.0f;

			UI::ImageRender::Render( pcLowHPWarningImage, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, D3DCOLOR_ARGB( (int)(fDelta * 255), 255, 255, 255 ) );
		}
	}

	if ( ISBIGMAP > 0 )
		FakeRenderGameState();
	else
		RenderGameState();

	if ( SETTINGSHANDLER->GetModel()->GetSettings().bGlow && ISSHOW_INTERFACE )
	{
		DX::cSelectGlow1.Draw();
		DX::cSelectGlow2.Draw();
		DX::cSelectGlow3.Draw();
	}

	DX::cSelectGlow4.Draw();

	TEXTUREFRAME++;
}

BOOL IsCreatedNewRenderTarget()
{
	return (BOOL)CALL( 0x0046EE40 );
}

NAKED void SetStageDynamicLight( Stage * pcStage, int iX, int iY, int iZ )
{
	JMP( pfnSetDynamicLight );
}

NAKED void SetCharacterCamera( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnSetCameraCharacter );
}

NAKED void SetClanMaster( DWORD dwID )
{
	JMP( pfnSetCastleMaster );
}

NAKED int GetDamageFromAreaSkill( UnitData * pcUnitData, UnitData * pcTarget, int iDestX, int iDestY, int iDestZ )
{
	JMP( pfnGetDamageFromArea );
}

NAKED int SetupCamera( EXECameraTrace * psCamera, int * iX, int * iY, int * iZ )
{
	JMP( pfnSetupCamera );
}

NAKED int SetupCameraMatrix( EXECameraTrace * psCamera, int iX, int iY, int iZ, int iDX, int iDY, int iDZ )
{
	JMP( pfnSetupCameraMatrix );
}

NAKED void RenderSky( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderSky );
}

NAKED int RenderStage( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderStage );
}

NAKED int RenderModelShadow( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderModelShadow );
}

NAKED void UpdateUITextures()
{
	JMP( pfnUpdateUITextures );
}

NAKED void RenderEffects( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderEffects );
}

NAKED void RenderModel2D( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnRenderModel2D );
}

void Render3DEngine( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	CALLT_WITH_ARG6( pfnRender3DEngine, 0x03707170, iX, iY, iZ, iAX, iAY, iAZ );
}

NAKED void RenderZoomMap()
{
	JMP( pfnRenderZoomMap );
}

NAKED void PrepareUnitDataHUD( UnitData * p )
{
	JMP( 0x004558F0 );
}

void RenderUnitDataHUD( Point2D * sCenterPos, int iCameraDistance, int iType )
{
	UnitData * pcCurrentTarget = (*(UnitData**)0x0CF48D0);

	//Render UnitData
	if ( TARGET_UNITDATA->bActive && pcCurrentTarget && pcCurrentTarget->bActive )
	{
		if ( pcCurrentTarget->sCharacterData.bExclusiveBoss )
		{
			if ( HUDHANDLER->GetTargetUnitData() )
			{
				HUDHANDLER->UpdateTargetInfo( HUDHANDLER->GetTargetUnitData() );
				HUDHANDLER->SetRenderTarget( TRUE );
			}
			else
				HUDHANDLER->SetRenderTarget( FALSE );
		}
		else if ( !pcCurrentTarget->sCharacterData.bExclusiveBoss )
		{
			HUDHANDLER->UpdateTargetInfo( pcCurrentTarget );
			HUDHANDLER->SetRenderTarget( TRUE );
		}
	}
	else
		HUDHANDLER->SetRenderTarget( FALSE );

	//Prepare UnitData to Render, if dont have yet
	if ( UNITDATA_FOCUS )
	{
		if ( pcCurrentTarget != UNITDATA_FOCUS )
		{
			WRITEDWORD( 0x0CF48D4, FALSE );
			PrepareUnitDataHUD( UNITDATA_FOCUS );
			WRITEDWORD( 0x00CF48DC, TRUE );
		}
	}
	else if ( UNITDATA_SELECTED && pcCurrentTarget != UNITDATA_SELECTED )
	{
		PrepareUnitDataHUD( UNITDATA_SELECTED );
		WRITEDWORD( 0x00CF48DC, TRUE );
	}
	//Render Character Debug Text
	if ( GM_MODE && *(int*)0x35E11D8 != 0 && DEBUG_GM )
		UNITGAME->RenderCharacterDebugText();
}

NAKED void UpdateMovementCamera()
{
	JMP( pfnUpdateMovementCamera );
}

NAKED void FakeRenderGameState()
{
	JMP( pfnFakeRenderGameState );
}

BOOL RenderGameState()
{
	if( pcBossFixedPositionImages[0] == NULL )
		pcBossFixedPositionImages[0] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\bossbackground.png" );

	if( pcBossFixedPositionImages[1] == NULL )
		pcBossFixedPositionImages[1] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\bosshp.png" );

	if( pcBossFixedPositionImages[2] == NULL )
		pcBossFixedPositionImages[2] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\bosshp_.png" );

	if( pcBossFixedPositionImages[3] == NULL )
		pcBossFixedPositionImages[3] = UI::ImageLoader::LoadImage( "game\\images\\UI\\hud\\target\\type\\background.png" );

	int iOldDarkColor = 0;

	RENDERER->ClearLight();

	iOldDarkColor = DARKCOLOR_AMOUNT;
	DARKCOLOR_AMOUNT = 0;

	UnitData * pcUnitData = NULL, * pcBossUnitData = NULL;

	if ( ISPRESSEDKEY( 'A' ) )
	{
		for ( int i = 0; i < 1024; i++ )
		{
			DropItemView * ps = saItemsStage + i;

			if ( ps->bActive && ps->iItemID != 0 && ps != ITEMVIEW_SELECTED && ITEMHANDLER->CanViewDropItem( ps ) )
			{
				if ( ps->sRenderPosition.iZ >= (32 * 256) && ps->sRenderPosition.iZ < (64 * 12 * 256) )
				{
					//Draw Box
					MESSAGEBALLOON->ResetMessage();
					MESSAGEBALLOON->SetSkin( SKINID_Normal );
					MESSAGEBALLOON->SetMessage( ps->szName );
					MESSAGEBALLOON->SetColor( D3DCOLOR_XRGB( 210, 210, 210 ) );
					MESSAGEBALLOON->Render( ps->sRenderPosition.iX, ps->sRenderPosition.iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				}
			}
		}
	}

	if ( ITEMVIEW_SELECTED && UNITDATA_SELECTED == NULL && UNITDATA_FOCUS == NULL )
	{
		Point2D * psSelect2D = (Point2D*)0x00CF3FBC;

		//X
		(*(int*)0x00A16314) = psSelect2D->iX;
		//Y
		(*(int*)0x00A17534) = psSelect2D->iY;

		//Draw Selected Item.
		MESSAGEBALLOON->ResetMessage();
		MESSAGEBALLOON->SetSkin( SKINID_Normal );
		MESSAGEBALLOON->SetMessage( ITEMVIEW_SELECTED->szName );
		MESSAGEBALLOON->SetColor( D3DCOLOR_XRGB( 180, 180, 255 ) );
		MESSAGEBALLOON->Render( psSelect2D->iX, psSelect2D->iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	}

	CHARACTERMESSAGE_COUNT = 0;
	int iLastCount = 0;

	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITGAME->pcaUnitData + i;

		Unit * pcUnit = UNITDATATOUNIT( pc );

		if( pc->bActive && pc->bRendered && pc->sCharacterData.szName[0] )
		{
			//Party Handler
			if( PARTYHANDLER->IsPartyMember( pc->iID ) )
				pc->dwExclusiveNum = 1;
			else if( PARTYHANDLER->IsRaidMember( pc->iID ) )
				pc->dwExclusiveNum = 2;
			else
				pc->dwExclusiveNum = 0;

			//Exclusive Boss
			if( pc->sCharacterData.bExclusiveBoss && !pc->sCharacterData.bGrandFuryBoss )
				pcBossUnitData = pc;

			int iSelected = 0;
			if ( pc->sRenderPoint.iZ < (64 * 32 * 256) )
			{
				if ( pc->szMessage[0] )
				{
					if ( pc->dwMessageEndTime < TICKCOUNT )
					{
						pc->szMessage[0] = 0;
					}
					else if( (lstrlenA(pc->szMessage) == 2 && pc->szMessage[0] == 'e') && isdigit(pc->szMessage[1]) )
						pc->szMessage[0] = 0;
					else if ( pc->sCharacterData.iType )
					{
						CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = pc;
						iSelected++;
					}
				}

				if ( pc->iPersonalShop && iSelected == 0 )
				{
					CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = pc;
					iSelected++;
				}
			}

			if ( iSelected == 0 )
			{
				if ( UNITDATA_FOCUS == pc || UNITDATA_SELECTED == pc ||
					(pc->sCharacterData.iType == CHARACTERTYPE_NPC && pc->sRenderPoint.iZ < (64 * 64 * 256)) ||
					(pc->sCharacterData.sHP.sCur > 0 && pc->sCharacterData.iType == CHARACTERTYPE_Monster && pc->sCharacterData.iMonsterType == MONSTERTYPE_Friendly) ||
					(pc->sCharacterData.iClanID && pc->sCharacterData.iClanID == UNITDATA->sCharacterData.iClanID) ||
					pc->iBellatraCrown || pc->dwExclusiveNum )
				{
					CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = pc;
				}
			}

			if ( iLastCount == CHARACTERMESSAGE_COUNT )
				CHARACTERPVPMODEHANDLER->RenderHeaderTopPK( pcUnit, 0 );

			iLastCount = CHARACTERMESSAGE_COUNT;
		}
	}

	if ( UNITDATA->szMessage[0] )
	{
		if ( UNITDATA->dwMessageEndTime < TICKCOUNT )
			UNITDATA->szMessage[0] = 0;
		else if( (lstrlenA(UNITDATA->szMessage) == 2 && UNITDATA->szMessage[0] == 'e') && isdigit(UNITDATA->szMessage[1]) )
			UNITDATA->szMessage[0] = 0;
		else
			CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = UNITDATA;
	}
	else
	{
		if ( UNITDATA->iPersonalShop )
		{
			CHARACTERMESSAGE[CHARACTERMESSAGE_COUNT++] = UNITDATA;
		}
	}

	if ( CHARACTERMESSAGE_COUNT > 0 )
	{
		for ( int i = 0; i < CHARACTERMESSAGE_COUNT; i++ )
		{
			for ( int j = i + 1; j < CHARACTERMESSAGE_COUNT; j++ )
			{
				UnitData * pc1 = CHARACTERMESSAGE[i];
				UnitData * pc2 = CHARACTERMESSAGE[j];

				if ( pc1->sRenderPoint.iZ < pc2->sRenderPoint.iZ ||
					pc1 == UNITDATA_FOCUS || pc1 == UNITDATA_SELECTED )
				{
					pcUnitData = pc1;
					CHARACTERMESSAGE[i] = CHARACTERMESSAGE[j];
					CHARACTERMESSAGE[j] = pc1;
				}
			}
		}
	}

	int iY = 8 + ((*(int*)0x00824FA8) >> 6);
	for ( int i = 0; i < CHARACTERMESSAGE_COUNT; i++ )
	{
		DWORD dwColor = RGBA( 255, 255, 255, 200 );
		BOOL bShowBar = FALSE;
		BOOL bShowOwner = FALSE;
		BOOL bSelected = FALSE;

		UnitData * pc = CHARACTERMESSAGE[i];
		Unit * pcUnit = UNITDATATOUNIT( pc );

		MESSAGEBALLOON->ResetLastHeight();

		if ( pc->dwExclusiveNum == 1 )				//Party
		{
			dwColor = RGBA( 255, 220, 255, 160 );
			bShowBar = TRUE;
		}
		else if( pc->dwExclusiveNum == 2 )			//Raid
		{
			dwColor = RGBA( 255, 40, 240, 230 );	
			bShowBar = TRUE;
		}
		else if ( pc->sCharacterData.iType == CHARACTERTYPE_Monster )
		{
			if ( pc->sCharacterData.iMonsterType == MONSTERTYPE_Friendly )
			{
				dwColor = RGBA( 255, 220, 255, 160 );

				if ( pc->sCharacterData.iOwnerID == UNITDATA->iID )
				{
					dwColor = RGBA( 255, 0, 162, 232 );
					bShowOwner = TRUE;
					bShowBar = TRUE;
				}
				else
					bShowOwner = TRUE;
			}
			else
				dwColor = RGBA( 255, 255, 210, 210 );
		}
		else if ( pc->sCharacterData.iType == CHARACTERTYPE_NPC )
		{
			dwColor = RGBA( 255, 180, 180, 255 );
		}

		if ( pc == UNITDATA_FOCUS )
		{
			if ( pc->sCharacterData.iType == CHARACTERTYPE_Monster && pc->sCharacterData.iMonsterType != MONSTERTYPE_Friendly )
				dwColor = RGBA( 255, 255, 230, 200 );
			else
				dwColor = RGBA( 255, 255, 255, 255 );

			bSelected = TRUE;
		}
		else
		{
			if ( pc == UNITDATA_SELECTED && pc->szMessage[0] )
				bSelected = TRUE;
		}

		EXESurfaceOld * psTextureClanIcon = NULL;
		char * pszClanName = NULL;
		DWORD dwClanBit = 0;

		ClanInfo * psClanInfo = (ClanInfo*)0x03AA4608;

		if ( pc->sCharacterData.iClanID )
		{
			if ( pc == UNITDATA )
			{
				psTextureClanIcon = (EXESurfaceOld*)READDWORD( 0x03B18BC4 );
				pszClanName = (char*)0x03B17984;
			}
			else if ( pc->iClanInfoIndex >= 0 )
			{
				psTextureClanIcon = psClanInfo[pc->iClanInfoIndex].psTexture32;
				pszClanName = psClanInfo[pc->iClanInfoIndex].sHeader.szClanName;
			}
		}

		if ( ((pc->szMessage[0] || pc->iPersonalShop) && (pc->sRenderPoint.iZ < (64 * 64 * 256)) || pc == UNITDATA) && (!BATTLEROYALEHANDLER->InWaiting() && !BATTLEROYALEHANDLER->InBattle()) )
		{
			if ( pszClanName && pc != UNITDATA_FOCUS && pc != UNITDATA_SELECTED )
			{
				if ( UNITDATA->sCharacterData.iClanID == 0 || pc->sCharacterData.iClanID != UNITDATA->sCharacterData.iClanID )
					pszClanName = NULL;
			}

			if ( pc->szMessage[0] )
			{
				//Draw Box...
				MESSAGEBALLOON->ResetMessage();

				if ( psTextureClanIcon )
					MESSAGEBALLOON->SetClanIconTexture( psTextureClanIcon );

				MESSAGEBALLOON->IsCharacterMessage( TRUE );
				MESSAGEBALLOON->SetSkin( SKINID_Normal );

				if( pc->sCharacterData.iType == CHARACTERTYPE_NPC )
				{
					MESSAGEBALLOON->SetMessage( FormatString( "%s: %s", pc->sCharacterData.szName, pc->szMessage ) );
					MESSAGEBALLOON->GetTextMessage()->SetHighlightTextColor( dwColor );
					MESSAGEBALLOON->SetColor( -1 );
				}
				else
				{
					MESSAGEBALLOON->SetMessage( pc->szMessage );
					MESSAGEBALLOON->SetColor( dwColor );
				}

				MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			}
			else
			{
				if ( pc->szPersonalShop[0] )
				{
					MESSAGEBALLOON->ResetMessage();

					if ( psTextureClanIcon )
						MESSAGEBALLOON->SetClanIconTexture( psTextureClanIcon );

					ESkinID iSkinID = SKINID_Blue;
					if ( PERSONALSHOPHANDLER->GetVisitedShop( pc->iID ) )
						iSkinID = SKINID_Green;
					if ( PERSONALSHOPHANDLER->GetPinnedShop( pc->iID ) )
						iSkinID = SKINID_Pink;

					MESSAGEBALLOON->IsCharacterMessage( TRUE );
					MESSAGEBALLOON->SetSkin( iSkinID );
					MESSAGEBALLOON->SetMessage( pc->szPersonalShop );
					MESSAGEBALLOON->SetColor( dwColor );
					MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				}
				else
				{
					if ( PERSONALSHOPHANDLER->GetVisitedShop( pc->iID ) )
						PERSONALSHOPHANDLER->RemoveVisitedShop( pc->iID );
					else if ( PERSONALSHOPHANDLER->GetPinnedShop( pc->iID ) )
						PERSONALSHOPHANDLER->RemovePinnedShop( pc->iID );
				}
			}
		}
		else if( pc->sRenderPoint.iZ < ( 64 * 20 * 256 ) )
		{
			if ( (pc->bVisible) && (!BATTLEROYALEHANDLER->InWaiting() && !BATTLEROYALEHANDLER->InBattle()) )
			{
				if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CrystalTower )
				{
					//Nothing...
				}
				else if ( bShowOwner && pc->sCharacterData.iClanID == 0 )
				{
					STRINGFORMATLEN( ((char*)0x00A17420), 240, "(%s)", pc->sCharacterData.Pet.szOwnerName );

					//Draw Box Two Lines
					MESSAGEBALLOON->ResetMessage();

					if ( psTextureClanIcon )
						MESSAGEBALLOON->SetClanIconTexture( psTextureClanIcon );

					MESSAGEBALLOON->IsCharacterMessage( TRUE );
					MESSAGEBALLOON->SetSkin( SKINID_Normal );
					MESSAGEBALLOON->SetMessage( pc->sCharacterData.szName );
					MESSAGEBALLOON->SetPetName( ((char*)0x00A17420) );
					MESSAGEBALLOON->SetColor( dwColor );
					MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				}
				else
				{

					if ( pszClanName )
					{
						if ( GM_MODE && ISPRESSEDKEY( VK_CONTROL ) )
							STRINGFORMATLEN( ((char*)0x00A17420), 240, "%d/%d", pc->sCharacterData.iClanID, pc->iClanInfoIndex );
						else
							STRINGFORMATLEN( ((char*)0x00A17420), 240, "[%s]", pszClanName );

						//Draw Box Two Lines
						MESSAGEBALLOON->ResetMessage();

						if ( psTextureClanIcon )
							MESSAGEBALLOON->SetClanIconTexture( psTextureClanIcon );

						MESSAGEBALLOON->IsCharacterMessage( TRUE );
						MESSAGEBALLOON->SetSkin( SKINID_Normal );
						MESSAGEBALLOON->SetMessage( pc->sCharacterData.szName );

						if ( ACHIEVEMENTHANDLER->HaveAchievement( pcUnit, ACHIEVEMENTTYPE_LegacyMember ) )
							MESSAGEBALLOON->AddExtraText( 1, UI::EPositionText::POSITIONTEXT_Middle, "[Legacy Member]", D3DCOLOR_XRGB( 0, 162, 232 ) );
						if ( ACHIEVEMENTHANDLER->HaveAchievement( pcUnit, ACHIEVEMENTTYPE_GameMaster ) )
							MESSAGEBALLOON->AddExtraText( 2, UI::EPositionText::POSITIONTEXT_Middle, "[Game Master]", D3DCOLOR_XRGB( 246, 240, 134 ) );
						if (ACHIEVEMENTHANDLER->HaveAchievement( pcUnit, ACHIEVEMENTTYPE_Streamer ) )
							MESSAGEBALLOON->AddExtraText( 3, UI::EPositionText::POSITIONTEXT_Middle, "[Streamer]", D3DCOLOR_XRGB( 255, 0, 0 ) );

						MESSAGEBALLOON->SetBellatraIconID( pc->iBellatraCrown );
						MESSAGEBALLOON->SetClanName( ((char*)0x00A17420) );
						MESSAGEBALLOON->SetColor( dwColor );

						//Only if user want to see
						if ( MESSAGEBOX_SHOWALWAYS || (pc->sCharacterData.iType == CHARACTERTYPE_NPC) || (pc->sCharacterData.iType == CHARACTERTYPE_Monster) )
							MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
					}
					else
					{
						//Draw Box
						MESSAGEBALLOON->ResetMessage();

						if ( psTextureClanIcon )
							MESSAGEBALLOON->SetClanIconTexture( psTextureClanIcon );

						MESSAGEBALLOON->IsCharacterMessage( TRUE );
						MESSAGEBALLOON->SetSkin( SKINID_Normal );
						MESSAGEBALLOON->SetMessage( pc->sCharacterData.szName );
						MESSAGEBALLOON->SetBellatraIconID( pc->iBellatraCrown );
						MESSAGEBALLOON->SetColor( dwColor );

						if ( ACHIEVEMENTHANDLER->HaveAchievement( pcUnit, ACHIEVEMENTTYPE_LegacyMember ) )
							MESSAGEBALLOON->AddExtraText( 1, UI::EPositionText::POSITIONTEXT_Middle, "[Legacy Member]", D3DCOLOR_XRGB( 0, 162, 232 ) );
						if ( ACHIEVEMENTHANDLER->HaveAchievement( pcUnit, ACHIEVEMENTTYPE_GameMaster ) )
							MESSAGEBALLOON->AddExtraText( 2, UI::EPositionText::POSITIONTEXT_Middle, "[Game Master]", D3DCOLOR_XRGB( 246, 240, 134 ) );
						if (ACHIEVEMENTHANDLER->HaveAchievement( pcUnit, ACHIEVEMENTTYPE_Streamer ) )
							MESSAGEBALLOON->AddExtraText( 3, UI::EPositionText::POSITIONTEXT_Middle, "[Streamer]", D3DCOLOR_XRGB( 255, 0, 0 ) );

						//Only if user want to see
						if ( MESSAGEBOX_SHOWALWAYS || (pc->sCharacterData.iType == CHARACTERTYPE_NPC) || (pc->sCharacterData.iType == CHARACTERTYPE_Monster) )
							MESSAGEBALLOON->Render( pc->sRenderPoint.iX, pc->rRenderRectangle.top + iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

					}
				}
			}
		}

		if ( pc->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CrystalTower )
		{
			HUDHANDLER->RenderHPCrystalTower( pc->sRenderPoint.iX - 30, pc->rRenderRectangle.top + iY - 16, pc->sCharacterData.sHP.sCur, pc->sCharacterData.sHP.sMax );
		}
		else if ( bShowBar || (SETTINGSHANDLER->GetModel()->GetSettings().bHPBarOnHead && UNITDATA_SELECTED == pc && pc->sCharacterData.iType == CHARACTERTYPE_Monster) )
		{
			//Show Bar
			RenderCharacterHP( pc, pc->sRenderPoint.iX - 30, pc->rRenderRectangle.top + iY - 16 );
		}

		CHARACTERPVPMODEHANDLER->RenderHeaderTopPK( pcUnit, MESSAGEBALLOON->GetLastHeight() );
	}

	if ( UNITDATA->sActiveSkill.iValue )
	{
		switch ( UNITDATA->sCharacterData.iClass )
		{
			case CHARACTERCLASS_Archer:
				if ( UNITDATA->sActiveSkill.cSkillTinyID == SKILLPLAYID_T42 )
				{
					if ( UNITDATA->psModelAnimation )
					{
						int iFrame = UNITDATA->iFrame - UNITDATA->psModelAnimation->iBeginFrame * 160;
						if ( (DWORD)UNITDATA->psModelAnimation->iaEventFrame[0] < (DWORD)iFrame && (DWORD)UNITDATA->psModelAnimation->iaEventFrame[1] > (DWORD)iFrame )
						{
							int iEventFrame1 = iFrame - UNITDATA->psModelAnimation->iaEventFrame[0];
							int iEventFrame2 = UNITDATA->psModelAnimation->iaEventFrame[1] - UNITDATA->psModelAnimation->iaEventFrame[0];

							//Draw Charging Bar...
							RenderCharacterChangingBar( UNITDATA, UNITDATA->sRenderPoint.iX - 30, UNITDATA->rRenderRectangle.bottom, iEventFrame1, iEventFrame2 );
						}
					}
				}
				break;

			case CHARACTERCLASS_Pikeman:
				if ( UNITDATA->sActiveSkill.cSkillTinyID == SKILLPLAYID_T42 )
				{
					if ( UNITDATA->psModelAnimation )
					{
						int iFrame = UNITDATA->iFrame - UNITDATA->psModelAnimation->iBeginFrame * 160;
						if ( (DWORD)UNITDATA->psModelAnimation->iaEventFrame[0] < (DWORD)iFrame && (DWORD)UNITDATA->psModelAnimation->iaEventFrame[1] > (DWORD)iFrame )
						{
							int iEventFrame1 = iFrame - UNITDATA->psModelAnimation->iaEventFrame[0];
							int iEventFrame2 = UNITDATA->psModelAnimation->iaEventFrame[1] - UNITDATA->psModelAnimation->iaEventFrame[0];

							//Draw Charging Bar...
							RenderCharacterChangingBar( UNITDATA, UNITDATA->sRenderPoint.iX - 30, UNITDATA->rRenderRectangle.bottom, iEventFrame1, iEventFrame2 );
						}
					}
				}
				break;
		}
	}

	if ( ISSHOW_INTERFACE )
	{
		if( (pcBossUnitData) && pcBossUnitData->sCharacterData.sHP.sCur > 0 && pcBossUnitData->psModelAnimation->iType != ANIMATIONTYPE_Die )
		{
			Unit * pcUnit = UNITDATATOUNIT( pcBossUnitData );
			int iTextWidth = 800, iTextHeight = 100;

			auto pFont = GetGraphicsDevice()->GetDefaultFont();
			pFont->Calc( pcBossUnitData->sCharacterData.szName, 0, iTextWidth, iTextHeight );

			if( pcBossFixedPositionImages[0] )
			{
				UI::ImageRender::Render( pcBossFixedPositionImages[0], (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1), 100, pcBossFixedPositionImages[0]->GetWidth(), pcBossFixedPositionImages[0]->GetHeight(), -1 );

				RECT rRect1 = RECT{ (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1) + 1, 105, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1), 124 };
				RECT rRect2 = RECT{ (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1) + 1, 104, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1), 124 };

				pFont->Draw( pcBossUnitData->sCharacterData.szName, rRect1, DT_EXPANDTABS | DT_SINGLELINE | DT_CENTER, DX::Color( 255, 0, 0, 0 ) );
				pFont->Draw( pcBossUnitData->sCharacterData.szName, rRect2, DT_EXPANDTABS | DT_SINGLELINE | DT_CENTER, DX::Color( 255, 255, 255, 255 ) );
			}

			if( pcBossFixedPositionImages[1] )
				UI::ImageRender::Render( pcBossFixedPositionImages[1], (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[1]->GetWidth() >> 1), 108, pcBossFixedPositionImages[1]->GetWidth(), pcBossFixedPositionImages[1]->GetHeight(), -1 );

			if( pcBossFixedPositionImages[2] )
				UI::ImageRender::Render( pcBossFixedPositionImages[2], (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[2]->GetWidth() >> 1), 129,(pcBossUnitData->sCharacterData.sHP.sCur * pcBossFixedPositionImages[2]->GetWidth()) / pcBossUnitData->sCharacterData.sHP.sMax, pcBossFixedPositionImages[2]->GetHeight(), 0, 0, -1, 1.0f );
		
			if ( pcBossFixedPositionImages[0] && pcUnit )
			{
				RECT rRect1 = RECT{ (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1) + 1, 130, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1), 150 };
				RECT rRect2 = RECT{ (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1) + 1, 129, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1), 150 };

				std::string strMonsterHP = FormatString( "%d/%d", pcUnit->sHPLong.iMin, pcUnit->sHPLong.iMax );

				pFont->Draw( strMonsterHP, rRect1, DT_EXPANDTABS | DT_SINGLELINE | DT_CENTER, DX::Color( 255, 0, 0, 0 ) );
				pFont->Draw( strMonsterHP, rRect2, DT_EXPANDTABS | DT_SINGLELINE | DT_CENTER, DX::Color( 255, 255, 170, 170 ) );

				if ( pcUnit->sHPLong.iMax > 0 )
				{
					int iPercentHP = (pcUnit->sHPLong.iMin * 100) / pcUnit->sHPLong.iMax;

					RECT rRect3 = RECT{ (RESOLUTION_WIDTH >> 1) - (pcBossFixedPositionImages[0]->GetWidth() >> 1), 129, (RESOLUTION_WIDTH >> 1) + (pcBossFixedPositionImages[0]->GetWidth() >> 1) - 40, 150 };

					pFont->Draw( FormatString( "%d%%", iPercentHP ), rRect3, DT_EXPANDTABS | DT_SINGLELINE | DT_RIGHT, DX::Color( 255, 255, 170, 170 ) );
				}
			}

			if( iTextWidth )
			{
				UI::ImageRender::Render( pcBossFixedPositionImages[3], (RESOLUTION_WIDTH >> 1) - (iTextWidth >> 1) - 28, 104, pcBossFixedPositionImages[3]->GetWidth(), pcBossFixedPositionImages[3]->GetHeight(), -1 );
				
				UI::Image * pImageMonsterType = NULL;
				if( pcBossUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Demon )
					pImageMonsterType = HUDHANDLER->GetTargetTypeImage(0);
				else if( pcBossUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Mechanic )
					pImageMonsterType = HUDHANDLER->GetTargetTypeImage(1);
				else if( pcBossUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Mutant )
					pImageMonsterType = HUDHANDLER->GetTargetTypeImage(2);
				else if( pcBossUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Undead )
					pImageMonsterType = HUDHANDLER->GetTargetTypeImage(4);
				else if( pcBossUnitData->sCharacterData.iMonsterType == MONSTERTYPE_Normal )
					pImageMonsterType = HUDHANDLER->GetTargetTypeImage(3);

				if( pImageMonsterType )
					UI::ImageRender::Render( pImageMonsterType, (RESOLUTION_WIDTH >> 1) - (iTextWidth >> 1) - 26, 105, pImageMonsterType->GetWidth(), pImageMonsterType->GetHeight(), -1 );
			}
		}
		
		Point2D * psPointFix = (Point2D*)0x00A04790;
		psPointFix->iX = RESOLUTION_WIDTH - 800;
		psPointFix->iY = RESOLUTION_HEIGHT - 600;
		WRITEDWORD( 0x00A0478C, 3 );

		Point2D * p = ((Point2D*)0x00A17518);

		CALL_WITH_ARG2( 0x00409CC0, (DWORD)p, 3 );

		//Party, Friends...
		CALL( 0x0057ED70 );

		WRITEDWORD( 0x00A0478C, 0 );
		psPointFix->iX = 0;
		psPointFix->iY = 0;

		CALL_WITH_ARG2( 0x00409CC0, (DWORD)p, 0 );

		//Draw Unit Data Target
		RenderUnitDataHUD( &Point2D( (RESOLUTION_WIDTH >> 1) - 113, 55 ), 150 * 256, 0 );
	}

	UnitData * pp = (*(UnitData**)0x00CF48D0);

	if ( (*(BOOL*)0x00CF48D4) && pp )
	{
		CALL( 0x00454D00 );
	}

	//Display Sod Score
	CALL( 0x00460110 );

	Point2D * psPointFix = (Point2D*)0x00A04790;
	psPointFix->iX = RESOLUTION_WIDTH - 800;
	psPointFix->iY = RESOLUTION_HEIGHT - 600;

	if ( ISSHOW_INTERFACE )
		CALL( 0x004CFBF0 ); //Interface...

	psPointFix->iX = 0;
	psPointFix->iY = 0;

	if ( (*(BOOL*)0x03A97614) )
		CALL( 0x004060E0 ); //Bellatra Font Effect...

	//Render Debug Text
	if ( DEBUG_GM && GM_MODE )
		UNITGAME->RenderDebugText();

	if ( DISCONNECT_TIME )
	{
		switch ( DISCONNECT_TYPE )
		{
			case 0:
				DrawMessageMiddle( "Disconnect with Server" );
				break;
			case 1:
				DrawMessageMiddle( "Disconnect with Server (1)" );
				break;
			case 2:
				DrawMessageMiddle( "Disconnect with Server (2)" );
				break;
			case 3:
				DrawMessageMiddle( "Disconnect with Server (3)" );
				break;
			case 4:
				DrawMessageMiddle( "Disconnect with Server (4)" );
				break;
		}

		if ( GM_MODE )
		{
			if ( GameCore::QuitGame() )
			{
				if ( (DISCONNECT_TIME + 5000) < TICKCOUNT )
					GameCore::QuitGame( TRUE );
			}
		}
	}
	else
	{
		if ( GameCore::QuitGame() && !PERSONALSHOPHANDLER->GetWindow()->IsOpen() )
			DrawMessageMiddle( "Exiting..." );
		else
		{
			DWORD dwCloseWindow = READDWORD( 0x00CF476C );

			if ( dwCloseWindow && dwCloseWindow > TICKCOUNT )
				DrawMessageMiddle( "Please, close all windows first!" );
			else
			{
				if ( (*(DWORD*)0x00CF4768) )
				{
					if ( ((*(DWORD*)0x00CF4768) + 5000) > TICKCOUNT )
						DrawMessageMiddle( "Cannot exit during battle!" );
					else
						(*(DWORD*)0x00CF4768) = 0;
				}
			}
		}
	}

	DARKCOLOR_AMOUNT = iOldDarkColor;

	return TRUE;
}

NAKED void SetDynamicLights()
{
	JMP( pfnSetDynamicLights );
}

BOOL SetDynamicLight( int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iPower )
{
	GRAPHICS->GetRenderer()->PushLight( Graphics::RenderLight{ Math::Vector3( iX / 256.0f, iY / 256.0f, iZ / 256.0f ),  (float)iPower, Math::Color( iR, iG, iB, iA ) } );

	return 0;
}

BOOL SetDynamicLight( int iType, int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iRange )
{
	GRAPHICS->GetRenderer()->PushLight( Graphics::RenderLight{ Math::Vector3( iX / 256.0f, iY / 256.0f, iZ / 256.0f ),  (float)iRange, Math::Color( iR, iG, iB, iA ) } );

	return 0;
}

int __stdcall RenderStageMesh( EXEStageModel * psStageModel )
{
	struct RenderStageModel
	{
		char			cPadding01[0x4B004];

		Point3D			sCamera;
		Point3D			sAngle;

		int				iSum;
	};

	RenderStageModel * pcThis;
	READTHIS( pcThis );

	Point3D s = psStageModel->sPosition;
	Point3D a = psStageModel->sAngle;

	int iRenderedCount = 0;

	DWORD dwLastTime = psStageModel->dwTimeUpdate;

	int iFrame		= psStageModel->pcModel->uFrame;
	int iMaxFrame	= psStageModel->pcModel->uMaxFrame;

	if ( (GetTickCount() - dwLastTime) >= 14 )
	{
		int iDiff	= (GetTickCount() - dwLastTime) / 14;

		iFrame		+= (iDiff * 70);

		psStageModel->dwTimeUpdate = GetTickCount();
	}

	if ( psStageModel->pcModel->pcBoneContainer )
		iMaxFrame = psStageModel->pcModel->pcBoneContainer->uMaxFrame;

	if ( iFrame < 0 || iFrame > iMaxFrame )
		iFrame = 0;

	psStageModel->pcModel->uFrame = iFrame;
	psStageModel->pcModel->SetPosition( &s, &a );
	psStageModel->pcModel->SetFixedPosition();


	//Fix Camera
	WRITEDWORD( 0x02619824, TRUE );

	RENDERER->SetRenderClip( 0x3C );

	iRenderedCount = psStageModel->pcModel->Render( &pcThis->sCamera, &pcThis->sAngle );

	WRITEDWORD( 0x02619824, FALSE );

	psStageModel->iSum = pcThis->iSum;

	return iRenderedCount;
}

void DrawMessageMiddle( const char * pszText )
{
	CTITLEBOX->RenderTextMiddle( pszText );
}

void DrawCharacterMessage( int iX, int iY, char * pszText, int iLineSize, DWORD dwClan, char * pszClanName, DWORD dwColor, BOOL bSelected, int iBellatra )
{
	BOOL bMessage		= FALSE;
	BOOL bCharacterName = FALSE;
	BOOL bClan			= dwClan != NULL ? TRUE : FALSE;

	int iMessageIndex	= 0;

	char szName[32]			= { 0 };

	char szNameFinder[20]	= { 0 };
		
	STRINGCOPY( szNameFinder, pszText );

	if ( strstr( szNameFinder, ":" ) != NULL )
	{
		bMessage		= TRUE;
		bCharacterName	= TRUE;

		for ( int i = 0; i < STRLEN( szNameFinder ); i++ )
		{
			if ( szNameFinder[i] == ':' )
			{
				iMessageIndex = i + 1;
				STRINGCOPYLEN( szName, i + 1, szNameFinder );
				break;
			}
		}
	}


	if ( pszText )
	{
		auto DrawMessageBox = []( int iX, int iY, int iWidth, int iHeight, bool bUnk = false, BOOL bSelected = FALSE ) -> int
		{
			return CALLT_WITH_ARG6( 0x0066B260, 0x040C2FC0, iX, iY, iWidth, iHeight, bUnk, bSelected );
		};

		int iWidthName = GETWIDTHTEXT( szName );
		int iWidthText = GETWIDTHTEXT( szName ) + GETWIDTHTEXT( &pszText[iMessageIndex] );
		int iHeight = (iWidthText / 200);

		iX -= ((iWidthText >> 1) + 4);
		iY -= 40;

		if ( iHeight == 0 )
			iHeight++;

		//Compute Space
		std::string str = &pszText[iMessageIndex];
		
		while ( GETWIDTHTEXT( str.c_str() ) < iWidthName )
			str.insert( str.begin(), ' ' );


		if ( (bMessage == TRUE) && (bCharacterName == TRUE) )
			DrawMessageBox( iX, iY, iWidthText, iHeight, (bCharacterName == TRUE), bSelected );
		else
			DrawMessageBox( iX + 2, iY, str.length(), iHeight, (bCharacterName == TRUE), bSelected );


		RECT r;
		r.left		= iX + 10;
		r.top		= iY + 4;
		r.right		= r.left + 200;
		r.bottom	= r.top + 32;

		if ( bCharacterName == TRUE )
			GetGraphicsDevice()->GetDefaultFont()->Draw( szName, r, 0, dwColor );

		GetGraphicsDevice()->GetDefaultFont()->Draw( str, r, DT_WORDBREAK, dwColor );
	}
}

NAKED BOOL LoadUnitData( UnitData * pcUnitData, const char * pszBodyModel, const char * pszHeadModel, Point3D * psPosition, Point3D * psAngle )
{
	JMP( 0x00438DD0 );
}

int DrawGameInterface3D()
{
	return 1;
}

NAKED int SetItemTextureColor( UnitTool * psTool )
{
	JMP( 0x00420940 );
}

NAKED void ResetItemTextureColor()
{
	JMP( 0x00420A80 );
}

NAKED void SetShieldStrikeEffect( Point3D * psPosition )
{
	JMP( 0x00531780 );
}

BOOL __cdecl RenderUnitData( UnitData * pcUnitData, bool bRenderUnitDataBody, bool bRenderUnitDataHead, bool bRenderUnitDataWeapon )
{
	if ( (pcUnitData->bNoMove == TRUE) || (pcUnitData->bVisible == FALSE) || (pcUnitData->psModelAnimation == NULL) )
		return FALSE;

	auto pcUnit = UNITDATATOUNIT( pcUnitData );

	if ( pcUnitData->pcBody && pcUnitData->pcBody->iCount <= 0 )
	{
		pcUnitData->Free();

		if ( DEBUG_GM )
			ChatGame::AddChatBoxText( "Model Error" );

		return FALSE;
	}

	if ( (pcUnitData->pcBodyMeshContainer == nullptr) || (pcUnitData->pcBodyMeshContainer->pcNewModel == nullptr) )
		return FALSE;

	if ( pcUnitData->eUpdateMode == UPDATEMODE_ClientUnit )
	{
		if ( pcUnitData->iLastUnitInfoPacketType )
		{
			if ( (pcUnitData->dwLastActiveTime + 4000) < TICKCOUNT )
			{
				pcUnitData->eSkillDisplayFlag = 0;
				return TRUE;
			}
		}
		else
		{
			if ( (pcUnitData->dwLastActiveTime + 7000) < TICKCOUNT )
			{
				pcUnitData->eSkillDisplayFlag = 0;
				return TRUE;
			}
		}
	}

	if ( pcUnitData != UNITDATA )
		MOUNTHANDLER->UpdateAnimation( pcUnitData );

	pcUnitData->bRendered = 0;
	pcUnitData->bFlagShow = TRUE;

	Point3D sDistance;
	sDistance.iX = UNITTARGETPOSITION_X - pcUnitData->sPosition.iX;
	sDistance.iY = UNITTARGETPOSITION_Y - pcUnitData->sPosition.iY;
	sDistance.iZ = UNITTARGETPOSITION_Z - pcUnitData->sPosition.iZ;

	sDistance >>= 8;

	int iDistance	= sDistance.iX * sDistance.iX;
	iDistance	   += sDistance.iY * sDistance.iY;
	iDistance	   += sDistance.iZ * sDistance.iZ;

	Point3D sCameraPosition;
	sCameraPosition.iX = UNITTARGETPOSITION_X;
	sCameraPosition.iY = UNITTARGETPOSITION_Y;
	sCameraPosition.iZ = UNITTARGETPOSITION_Z;

	Point3D sCameraAngle;
	sCameraAngle.iX = UNITTARGETANGLE_X;
	sCameraAngle.iY = UNITTARGETANGLE_Y;
	sCameraAngle.iZ = UNITTARGETANGLE_Z;

	Point3D sAngle = pcUnitData->sAngle;
	sAngle.iY = (-sAngle.iY + PTANGLE_180) & PTANGLE_Mask;

	if ( ACTIONMODE )
	{
		if ( sAngle.iY > PTANGLE_180 )
			sAngle.iY = PTANGLE_270;
		else
			sAngle.iY = PTANGLE_90;
	}

	bool bRiding = MOUNTHANDLER->IsRiding( pcUnitData );

	pcUnitData->sPosi = pcUnitData->sPosition;

	PTModel * pcBodyMeshContainer = pcUnitData->pcBodyMeshContainer;
	PTModel * pcHeadMeshContainer = pcUnitData->pcHeadMeshContainer;

	MOUNTHANDLER->Render( pcUnitData );

	pcBodyMeshContainer->SetPosition( &pcUnitData->sPosi, &sAngle );

	if ( pcBodyMeshContainer->pcBoneContainer && ((int)pcBodyMeshContainer->pcBoneContainer->uMaxFrame > pcUnitData->iFrame) )
		pcBodyMeshContainer->uFrame = (UINT)pcUnitData->iFrame;
	else
		pcBodyMeshContainer->uFrame = 80;

	int iRed, iGreen, iBlue, iAlpha, iERed, iEGreen, iEBlue, iEAlpha;

	iRed	= RENDERER->r;
	iGreen	= RENDERER->g;
	iBlue	= RENDERER->b;
	iAlpha	= RENDERER->a;

	iERed	= pcUnitData->sAddColorEffect[2];
	iEGreen	= pcUnitData->sAddColorEffect[1];
	iEBlue	= pcUnitData->sAddColorEffect[0];
	iEAlpha	= pcUnitData->sAddColorEffect[3];

	Delta3D::Graphics::ModelGroup* pModelsGroup = nullptr;

	if ( pcUnitData->sCharacterData.sGlow == 1 )
	{
		int iCount = pcUnitData->iLoopTimer & 0x1F;

		if ( (pcUnitData->iLoopTimer & 0x20) == 0 )
			iCount = 0x1F - iCount;

		iERed	= (iERed * iCount) >> 5;
		iEGreen = (iEGreen * iCount) >> 5;
		iEBlue	= (iEBlue * iCount) >> 5;
	}

	RENDERER->r = iRed + pcUnitData->iFireOverlay + iERed;
	RENDERER->g = iGreen - (pcUnitData->iFireOverlay >> 1) + iEGreen + abs( pcUnitData->iPoison );
	RENDERER->b = iBlue + abs( pcUnitData->iIceOverlay ) + iEBlue;
	RENDERER->a = iAlpha + pcUnitData->iTransparencySelf - (abs( pcUnitData->iIceOverlay ) >> 1) + iEAlpha;

	if ( pcUnitData->bBitterOrdeal )
	{
		RENDERER->r += 100;
		RENDERER->b += 100;
	}

	if ( pcUnitData->bDistortion && (pcUnitData->IsDead() == FALSE) )
	{
		RENDERER->iDistortionMesh = 1;

		RENDERER->r += 80;
		RENDERER->g += 80;
	}

	if ( pcUnitData->iCurseLazy && (pcUnitData->IsDead() == FALSE) )
	{
		RENDERER->iDistortionMesh = 0;

		RENDERER->b += 40;

		EffectGame::InertiaTarget( "CurseLazy2", pcUnitData );
	}

	if ( pcUnitData->sCharacterData.sSize )
		RENDERER->iObjectVertexScale = pcUnitData->sCharacterData.sSize;

    float fScaleModel = (RENDERER->iObjectVertexScale == 0 ? 256.0f : (float)RENDERER->iObjectVertexScale) / 256.0f;
    auto vScaleModel = Math::Vector3( fScaleModel, fScaleModel, fScaleModel );

	int iDark = 0;

	if ( DARKCOLOR_AMOUNT > 0 && pcUnitData->iLoadedMapIndex >= 0 )
	{
		if ( FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapType == MAPTYPE_Town )
		{
			iDark = DARKCOLOR_AMOUNT;
			RENDERER->r += iDark;
			RENDERER->g += iDark;
			RENDERER->b += iDark;

			iDark = 0;

			if ( RENDERER->iLightCount > 0 && RENDERER->sObjectLight[0].iRange == DARKLIGHTCOLOR_RANGE )
			{
				RENDERER->sObjectLight[0].iRange = 0;
				iDark = 1;
			}
		}
	}

	GRAPHICS->GetRenderer()->SetDissolveAmount( 0.0f );

	if ( pcUnitData->IsDead() )
	{
		if ( pcUnitData->iFrameTimer > 400 )
		{
			RENDERER->r -= 100;
			RENDERER->g -= 100;
			RENDERER->b -= 100;
		}
		else
		{
			if ( pcUnitData->iFrameTimer > 200 )
			{
				int iColor = (pcUnitData->iFrameTimer - 200) >> 1;
				RENDERER->r -= iColor;
				RENDERER->g -= iColor;
				RENDERER->b -= iColor;
			}
		}

		if ( pcUnitData->iFrameTimer >= 200 )
			GRAPHICS->GetRenderer()->SetDissolveAmount( ((float)pcUnitData->iFrameTimer - 200.0f) / (400.0f - 200.0f) );

		int iMonsterEffectID = pcUnitData->sCharacterData.iMonsterEffectID;

		if ( iMonsterEffectID == MONSTEREFFECTID_FIGON || iMonsterEffectID == MONSTEREFFECTID_STYGIANLORD ||
			iMonsterEffectID == MONSTEREFFECTID_GUARDIAN_SAINT || iMonsterEffectID == MONSTEREFFECTID_S_F_ELEMENTAL ||
			iMonsterEffectID == MONSTEREFFECTID_CHAOSCARA || iMonsterEffectID == MONSTEREFFECTID_DarkGuardian )
		{
			RENDERER->a = iAlpha - (pcUnitData->iFrameTimer << 1);

			if ( RENDERER->a < (-400) )
			{
				RENDERER->r = iRed;
				RENDERER->g = iGreen;
				RENDERER->b = iBlue;
				RENDERER->a = iAlpha;

				if ( iDark )
					RENDERER->sObjectLight[0].iRange = DARKLIGHTCOLOR_RANGE;

				RENDERER->iDistortionMesh = 0;

				RENDERER->iObjectVertexScale = 0;

				return TRUE;
			}

			pcUnitData->iTransparency = 100;
		}
	}

	X3D::Vector3 sCharacterPosition = X3D::Vector3( (float)pcUnitData->sPosition.iX / 256.0f, (float)pcUnitData->sPosition.iY / 256.0f, (float)pcUnitData->sPosition.iZ / 256.0f );
	X3D::Matrix4 mMatrixWorld = X3D::Matrix4().rotateY( DEGTORAD( PTAngleToDegrees( pcUnitData->sAngle.iY ) ) ) * X3D::Matrix4().translate( sCharacterPosition );
//	GRAPHICENGINE->GetSRenderer()->PushWorldMatrix( mMatrixWorld );

	if ( pcUnitData->psBody )
	{
		ModelData::MeshQuality * psMeshQuality = &pcUnitData->psBody->sMediumMeshQuality;

		if ( pcUnitData->iTalkFrame )
		{
			if ( (int)pcUnitData->pcTalkBoneContainer->uMaxFrame > pcUnitData->iTalkFrame )
				pcUnitData->pcTalkBoneContainer->uMaxFrame = (UINT)pcUnitData->iTalkFrame;
			else
				pcUnitData->iTalkFrame = 0;
		}

		if ( (pcUnitData->iAnimationMode & 1) && bRenderUnitDataBody )
		{
			if ( pcBodyMeshContainer->pcBoneContainer && pcBodyMeshContainer->pcBoneContainer->pcNewModel && pcUnit )
				pcBodyMeshContainer->pcBoneContainer->pcNewModel->SetBlendingAnimationValues( pcUnit->fAnimationBlendingTime, pcUnit->vMeshesAnimation );


			int iRedAge = RENDERER->r;
			int iGreenAge = RENDERER->g;
			int iBlueAge = RENDERER->b;
			int iAlphaAge = RENDERER->a;

			if ( SETTINGSHANDLER->GetModel()->GetSettings().bArmorAgeEffect )
			{
				if ( pcUnitData->saEquippedItems[0] >= 0 )
				{
					if ( pcUnitData->sCharacterData.sLevelAgeArmor >= 4 )
					{
						pcBodyMeshContainer->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( GRAPHICENGINE->GetAgingMaterial( (int)pcUnitData->sCharacterData.sLevelAgeArmor ) );
					}
				}
			}

			DX::Model_ptr pModelNew = nullptr;

			if ( (Game::GetGameMode() == GAMEMODE_InGame) && (pModelNew = DX::ModelFactory::MakeModel( pcUnitData )) )
			{
				pModelNew->SetFrame( pcUnitData->iFrame );

				GRAPHICENGINE->GetSRenderer()->DrawModel( pModelNew );
			}
			else
			{
				MOUNTHANDLER->OnRenderCharacter( pcUnitData );

				pcBodyMeshContainer->pcNewModel->SetAddColor( Math::Color( RENDERER->r, RENDERER->g, RENDERER->b, RENDERER->a ) );
				pcBodyMeshContainer->pcNewModel->SetDiffuseColor( Math::Color( 255, 255, 255, 255 + RENDERER->a ) );

				if ( pcUnitData->pcBody->m_ModelsGroup )
					pModelsGroup = &pcUnitData->pcBody->m_ModelsGroup->DefaultModel;
				else
					pModelsGroup = nullptr;

				pcBodyMeshContainer->pcNewModel->scaling = vScaleModel;

				pcUnitData->bRendered += pcBodyMeshContainer->pcNewModel->Render( (IO::SMD::FrameInfo *) & pcUnitData->iStartFrame, pModelsGroup ? pModelsGroup->meshes.size() ? pModelsGroup : nullptr : nullptr );
			}

			RENDERER->r = iRedAge;
			RENDERER->g = iGreenAge;
			RENDERER->b = iBlueAge;
			RENDERER->a = iAlphaAge;

			pcBodyMeshContainer->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( nullptr );

			if ( pcBodyMeshContainer->pcBoneContainer && pcBodyMeshContainer->pcBoneContainer->pcNewModel && pcUnit )
				pcBodyMeshContainer->pcBoneContainer->pcNewModel->GetBlendingAnimationValues( pcUnit->vMeshesAnimation );
		}
	}
	
	if ( pcUnitData->pcHead && pcHeadMeshContainer && pcHeadMeshContainer->pcNewModel && (pcUnitData->iAnimationMode & 2) && bRenderUnitDataHead )
	{
		ModelData::MeshQuality * psMeshQuality = &pcUnitData->pcHead->psModelData->sMediumMeshQuality;

		pcHeadMeshContainer->SetPosition( &pcUnitData->sPosi, &sAngle );

		if ( pcHeadMeshContainer->pcBoneContainer && (int)pcHeadMeshContainer->pcBoneContainer->uMaxFrame > pcUnitData->iFrame )
			pcHeadMeshContainer->uFrame = (UINT)pcUnitData->iFrame;

		pcHeadMeshContainer->pcNewModel->SetAddColor( Math::Color( RENDERER->r, RENDERER->g, RENDERER->b, RENDERER->a ) );
		pcHeadMeshContainer->pcNewModel->SetDiffuseColor( Math::Color( 255, 255, 255, 255 + RENDERER->a ) );
	
		if( pcUnitData->pcHead->m_ModelsGroup )
			pModelsGroup = &pcUnitData->pcHead->m_ModelsGroup->DefaultModel;
		else
			pModelsGroup = nullptr;

		pcHeadMeshContainer->pcNewModel->scaling = vScaleModel;

		pcUnitData->bRendered += pcHeadMeshContainer->pcNewModel->Render( (IO::SMD::FrameInfo*)&pcUnitData->iStartFrame, pModelsGroup ? pModelsGroup->meshes.size() ? pModelsGroup : nullptr : nullptr );
	}

//	GRAPHICS->GetRenderer()->PopWorldMatrix();

	if ( pcUnitData->bRendered )
	{
		if ( ISSHOW_INTERFACE )
			RenderBlessCastleCrown( pcUnitData );

		int iWidthModel = pcUnitData->iModelWidth;
		int iHeightModel = pcUnitData->iModelHeight;

		if ( (pcUnitData->sCharacterData.sSize > 0) && (pcUnitData->sCharacterData.sSize != 256) )
		{
			iWidthModel = (iWidthModel * pcUnitData->sCharacterData.sSize) >> 8;
			iHeightModel = (iHeightModel * pcUnitData->sCharacterData.sSize) >> 8;
		}

		if ( iWidthModel < (44 << 8) )
			iWidthModel = (44 << 8);

		if ( iHeightModel < (44 << 8) )
			iHeightModel = (44 << 8);

		RENDERER->SetCameraPosition( sCameraPosition.iX, sCameraPosition.iY, sCameraPosition.iZ, sCameraAngle.iX, sCameraAngle.iY, sCameraAngle.iZ );

		Point2D sPoint( 0, 0 );

		pcUnitData->sRenderPoint.iZ = RENDERER->GetRectangle2D( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY + (iHeightModel >> 1), pcUnitData->sPosition.iZ, iWidthModel, iHeightModel, &pcUnitData->rRenderRectangle, &sPoint );
		pcUnitData->sRenderPoint.iX = sPoint.iX;
		pcUnitData->sRenderPoint.iY = sPoint.iY;

		BATTLEROYALEHANDLER->RenderUnitArrow( pcUnitData );
		if ( bUIDebug )
		{
			int iX = pcUnitData->rRenderRectangle.left;
			int iY = pcUnitData->rRenderRectangle.top;
			int iWidth = pcUnitData->rRenderRectangle.right - iX;
			int iHeight = pcUnitData->rRenderRectangle.bottom - iY;

			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX, iY, iWidth, 1 ), D3DCOLOR_XRGB( 255, 255, 100 ) );
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX, iY + iHeight - 1, iWidth, 1 ), D3DCOLOR_XRGB( 255, 255, 100 ) );
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX, iY, 1, iHeight ), D3DCOLOR_XRGB( 255, 255, 100 ) );
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX + iWidth - 1, iY, 1, iHeight ), D3DCOLOR_XRGB( 255, 255, 100 ) );
		}
	}

	if ( !pcBodyMeshContainer->pcBoneContainer || pcUnitData->bVisibleEquipment == FALSE )
	{
		RENDERER->r = iRed;
		RENDERER->g = iGreen;
		RENDERER->b = iBlue;

		if ( iDark )
			RENDERER->sLight[0].iRange = DARKLIGHTCOLOR_RANGE;

		return TRUE;
	}

	BOOL bColorIncrease = FALSE;

	if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack && TICKCOUNT & 0x20 )
		bColorIncrease = TRUE;

	if ( bColorIncrease && (pcUnitData->bProjectileMode == FALSE) )
	{
		RENDERER->r += 80;
		RENDERER->g += 80;
		RENDERER->b += 80;
	}

	pcUnitData->bRenderLeftHand = TRUE;
	pcUnitData->bRenderRightHand = TRUE;

	PTModel * pcModelShield = NULL;

	auto PTDegreeToRadians = []( const int deg ) { return (float)(deg & 4095) * D3DX_PI / 2048.0f; };

	if ( (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player || pcUnitData->iCharacterType == CHARACTERTYPE_Player) && bRenderUnitDataWeapon )
	{
		//Wing
		if ( pcUnitData->pcaBackBone[0] && pcUnitData->pcaBackBone[0]->pParentMesh )
			pcUnitData->sWingMatrix = pcUnitData->pcaBackBone[0]->pParentMesh->GetWorldMatrix();

		if ( pcUnitData->pcaBackBone[2] )
			pcUnitData->sWingSkillMatrix = pcUnitData->pcaBackBone[2]->GetWorldMatrix();

		bool bNotEquipTownItem = false;
		if ( pcUnitData->eToolEquipModeTown == TOOLEQUIPMODETOWN_None || (pcUnitData->iLoadedMapIndex >= 0 && FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapType != MAPTYPE_Town && pcUnitData->GetAnimation() != ANIMATIONTYPE_Yahoo && pcUnitData->GetAnimation() != ANIMATIONTYPE_Taunt &&
																		   pcUnitData->GetAnimation() != ANIMATIONTYPE_Dance) )
			bNotEquipTownItem = true;

		if ( (ItemID( pcUnitData->sRightHandTool.eItemID ).ToItemID() == ITEMID_LoveSword) )
			bNotEquipTownItem = true;

		if ( bRiding )
			bNotEquipTownItem = false;

		if ( bNotEquipTownItem )
		{
			if ( pcUnitData->sLeftHandTool.pModel )
			{
				pcModelShield = pcUnitData->sLeftHandTool.pModel;

				if ( ItemID(pcUnitData->sLeftHandTool.eItemID).ToItemType() == ITEMTYPE_Shield )
				{
					if ( (pcUnitData->eSkillDisplayFlag & SKILLMASK_SparkShield) && SPARKSHIELD_MODEL && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Mechanician )
					{
						if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T34 )
						{

						}
						else
							pcModelShield = SPARKSHIELD_MODEL;
					}
					if ( (pcUnitData->eSkillDisplayFlag & SKILLMASK_SparkShield) && DIVINESHIELD_MODEL && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Knight )
					{
						if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T32 )
						{

						}
						else
							pcModelShield = DIVINESHIELD_MODEL;
					}
					if ( (pcUnitData->eSkillDisplayFlag & SKILLMASK_GodlyShield) && GODLYSHIELD_MODEL && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Knight )
					{
						if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T42 )
						{

						}
						else
							pcModelShield = GODLYSHIELD_MODEL;
					}
				}

				if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill && pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Atalanta && (pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T11 || pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T23) && pcUnitData->pcShieldBone )
				{
					pcModelShield->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->pcShieldBone );

					RENDERER->r += 100;
					RENDERER->g += 100;
					RENDERER->b += 80;
				}
				else
				{
					if ( pcUnitData->sLeftHandTool.pBone )
					{
						//Dagger? ( PVP and PVE )
						if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Assassin )
						{
							if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Dagger )
							{
								EXEMatrixI sMatrix;
								EXEMatrixIdentityI( sMatrix );
								EXEMatrixRotateZI( sMatrix, PTANGLE_180 & PTANGLE_Mask );

								pcUnitData->sLeftHandTool.pBone->GetWorldMatrix() = EXEMatrixMultiplyI( sMatrix, pcUnitData->sLeftHandTool.pBone->GetWorldMatrix() );
							}
						}

						pcModelShield->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->sLeftHandTool.pBone );

					}
				}

				pcModelShield->SetPosition( &pcUnitData->sPosi, &sAngle );
				pcModelShield->uFrame = (UINT)pcUnitData->iFrame;

				if ( pcUnitData->sLeftHandTool.pBone )
					pcUnitData->sLeftHandMatrix = pcUnitData->sLeftHandTool.pBone->GetWorldMatrix();

				if ( !pcUnitData->sLeftHandTool.iBlinkSpeed || !pcUnitData->sLeftHandTool.saBlinkColor[3] )
				{
					if ( pcModelShield->pcNewModel )
					{
						pcModelShield->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( pcUnitData->sLeftHandTool.pcBlendingMaterial );

						pcModelShield->pcNewModel->scaling = vScaleModel;

						pcModelShield->pcNewModel->Animate( pcUnitData->iFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );

						pcModelShield->pcNewModel->Render();
					}

					ResetItemTextureColor();


					if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Atalanta && (pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T11 || pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T23) && pcUnitData->pcShieldBone )
					{
						if ( pcModelShield->GetMeshes()[0] )
						{
							Point3D sShieldPosition;
							sShieldPosition.iX = pcModelShield->GetMeshes()[0]->GetWorldMatrix().i[3][0] + pcUnitData->sPosition.iX;
							sShieldPosition.iZ = pcModelShield->GetMeshes()[0]->GetWorldMatrix().i[3][1] + pcUnitData->sPosition.iZ;
							sShieldPosition.iY = pcModelShield->GetMeshes()[0]->GetWorldMatrix().i[3][2] + pcUnitData->sPosition.iY;
							SetShieldStrikeEffect( &sShieldPosition );
						}
					}
				}
				else
					pcUnitData->bRenderLeftHand = FALSE;

				pcModelShield->SetParentBone( NULL, NULL );
				pcModelShield->iLastFrame = -1;
			}

			if ( pcUnitData->sRightHandTool.pModel && !pcUnitData->bHideRightWeapon )
			{
				if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Javelin && pcUnitData->iProjectileType )
				{

				}
				else
				{
					PTModel * pcModelWeapon = pcUnitData->sRightHandTool.pModel;
					PTMesh * pcBoneWeapon = pcUnitData->sRightHandTool.pBone;
					
					if ( pcModelWeapon && pcModelWeapon->pcNewModel )
						pcModelWeapon->pcNewModel->scaling = vScaleModel;

					if ( pcBoneWeapon && pcModelWeapon->pcNewModel )
					{
						//Shaman & Phantom? ( PVP & PVE )
						if ( (pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Shaman) && (pcUnitData->eToolEquipModeTown > 0) && (pcUnitData->eToolEquipModeTown <= TOOLEQUIPMODETOWN_BackBow) )
						{
							if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Phantom )
							{
								if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] )
								{
									pcBoneWeapon = pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1];

                                    Delta3D::Math::Matrix4 sMatrixF = Delta3D::Math::Matrix4::Identity;
                                    sMatrixF.RotateY( PTDegreeToRadians( 2250 ) );
                                    sMatrixF.Translate( Delta3D::Math::Vector3( 0, 0, 10.0f ) );

                                    pcUnitData->sPosi.iY += 1000;

                                    pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

                                    if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh )
                                    {
                                        pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh->Animate( pcModelWeapon->uFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );

                                        pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh->world = sMatrixF * pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh->world;
                                    }
								}
							}
						}
						
						pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcBoneWeapon );
					}

					pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
					pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

					if ( pcBoneWeapon )
						pcUnitData->sRightHandMatrix = pcBoneWeapon->GetWorldMatrix();

					if ( !pcUnitData->sRightHandTool.iBlinkSpeed || !pcUnitData->sRightHandTool.saBlinkColor[3] )
					{
						if ( pcModelWeapon->pcNewModel )
						{
							pcModelWeapon->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( pcUnitData->sRightHandTool.pcBlendingMaterial );

							pcModelWeapon->pcNewModel->Animate( pcUnitData->iFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );

							pcModelWeapon->pcNewModel->Render();
						}
					}
					else
						pcUnitData->bRenderRightHand = FALSE;

					pcModelWeapon->SetParentBone( NULL, NULL );
					pcModelWeapon->iLastFrame = -1;
				}
			}
		}
		else
		{
			PTModel * pcModelWeapon = NULL;

			if ( pcUnitData->eWeaponItemID )
			{
				int iColorScale = 0;

				if ( pcUnitData->sLeftHandTool.eItemID == pcUnitData->eWeaponItemID )
				{
					pcModelWeapon = pcUnitData->sLeftHandTool.pModel;

					if ( !pcUnitData->sLeftHandTool.iBlinkSpeed || !pcUnitData->sLeftHandTool.saBlinkColor[3] )
					{
					}
					else
						pcUnitData->bRenderLeftHand = FALSE;
				}

				if ( pcUnitData->sRightHandTool.eItemID == pcUnitData->eWeaponItemID )
				{
					pcModelWeapon = pcUnitData->sRightHandTool.pModel;

					if ( !pcUnitData->sRightHandTool.iBlinkSpeed || !pcUnitData->sRightHandTool.saBlinkColor[3] )
					{
					}
					else
						pcUnitData->bRenderRightHand = FALSE;
				}

				if ( pcModelWeapon && pcModelWeapon->pcNewModel )
				{
                    if ( pcModelWeapon && pcModelWeapon->pcNewModel )
                        pcModelWeapon->pcNewModel->scaling = vScaleModel;

					if ( (pcUnitData->eToolEquipModeTown > 0) && (pcUnitData->eToolEquipModeTown <= TOOLEQUIPMODETOWN_BackBow) && pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] )
					{
						//Shaman & Phantom? ( Town )
						if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Shaman )
						{
							if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Phantom )
							{
								Delta3D::Math::Matrix4 sMatrixF = Delta3D::Math::Matrix4::Identity;
								sMatrixF.RotateY( PTDegreeToRadians( 2250 ) );
								sMatrixF.Translate( Delta3D::Math::Vector3( 0, 0, 10.0f ) );

								pcUnitData->sPosi.iY += 1000;

								pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

								if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh )
								{
									pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh->Animate( pcModelWeapon->uFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );

									pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh->world = sMatrixF * pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh->world;
								}
							}
						}
						
						//Xmas Bow?
						if ( ItemID( pcUnitData->eWeaponItemID ).ToItemID() == ITEMID_XmasCostumeOldBow )
						{
							pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

							if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh )
							{
								pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh->Animate( pcModelWeapon->uFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );

								Delta3D::Math::Matrix4 sMatrix = Delta3D::Math::Matrix4::Identity, sMatrixX = Delta3D::Math::Matrix4::Identity, sMatrixY = Delta3D::Math::Matrix4::Identity;
								sMatrix.RotateZ( PTDegreeToRadians( -PTANGLE_90 & PTANGLE_Mask ) );
								sMatrixY.RotateX( PTDegreeToRadians( -PTANGLE_45 & PTANGLE_Mask ) );

								sMatrixX = sMatrixX * sMatrixY;
								sMatrix = sMatrix * sMatrixX;

								pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh->world = sMatrix * pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->pcNewMesh->world;
							}
						}

						if ( pcModelWeapon->uMeshCount > 1 )
							pcModelWeapon->uMeshCount = 1;

						pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] );
					}

					pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
					pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

					if ( iColorScale )
					{
						EXEMatrixI sMatrix;

						EXEMatrixIdentityI( sMatrix );
						sMatrix.i[0][0] = iColorScale;
						sMatrix.i[1][1] = iColorScale;
						sMatrix.i[2][2] = iColorScale;
						pcModelWeapon->uFrame = -1;

						if ( pcModelWeapon->GetMeshes()[0] )
							pcModelWeapon->GetMeshes()[0]->GetWorldMatrix() = EXEMatrixMultiplyI( sMatrix, pcModelWeapon->GetMeshes()[0]->GetWorldMatrix() );
					}

					if ( pcUnitData->sLeftHandTool.eItemID == pcUnitData->eWeaponItemID )
					{
						if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] && (pcUnitData->eToolEquipModeTown > 0) && (pcUnitData->eToolEquipModeTown <= TOOLEQUIPMODETOWN_BackBow) )
							pcUnitData->sLeftHandMatrix = pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->GetWorldMatrix();

						if ( pcUnitData->bRenderLeftHand )
						{
							//Assassin & Dagger? ( Town )
							if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Assassin )
							{
								if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Dagger )
								{
									for ( int i = 0; i < 2; i++ )
									{
										PTMesh * pcBone = pcUnitData->pcaWeaponDaggerBone[i];

										if ( pcBone )
										{
											EXEMatrixI sMatrix;
											EXEMatrixIdentityI( sMatrix );

											if ( pcUnitData->eWeaponItemID == pcUnitData->sLeftHandTool.eItemID )
												EXEMatrixRotateZI( sMatrix, PTANGLE_180 & PTANGLE_Mask );

											pcBone->GetWorldMatrix() = EXEMatrixMultiplyI( sMatrix, pcBone->GetWorldMatrix() );

											pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcBone );

											pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
											pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

											if ( pcModelWeapon->GetMeshes()[0] )
												pcModelWeapon->GetMeshes()[0]->GetWorldMatrix() = EXEMatrixMultiplyI( sMatrix, pcModelWeapon->GetMeshes()[0]->GetWorldMatrix() );

											pcModelWeapon->uFrame++;

											BOOL bRenderWeapon = FALSE;

											if ( pcUnitData->eWeaponItemID == pcUnitData->sLeftHandTool.eItemID )
											{
												pcUnitData->sLeftHandMatrix = pcBone->GetWorldMatrix();
												bRenderWeapon = pcUnitData->bRenderLeftHand;
											}
											else
											{
												pcUnitData->sRightHandMatrix = pcBone->GetWorldMatrix();
												bRenderWeapon = pcUnitData->bRenderRightHand;
											}

											if ( bRenderWeapon )
											{
												ResetItemTextureColor();

												if ( pcModelWeapon->pcNewModel )
												{
													pcModelWeapon->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( pcUnitData->sLeftHandTool.pcBlendingMaterial );

													pcModelWeapon->pcNewModel->Animate( pcUnitData->iFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );

													pcModelWeapon->pcNewModel->Render();
												}
											}

											pcModelWeapon->SetParentBone( NULL, NULL );
											pcModelWeapon->iLastFrame = -1;
										}
									}
								}
							}
							else
							{
								if ( pcModelWeapon->pcNewModel )
								{
									pcModelWeapon->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( pcUnitData->sLeftHandTool.pcBlendingMaterial );

									pcModelWeapon->pcNewModel->Animate( pcUnitData->iFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );

									pcModelWeapon->pcNewModel->Render();
								}

								pcModelWeapon->SetParentBone( NULL, NULL );
								pcModelWeapon->iLastFrame = -1;
							}

							ResetItemTextureColor();
						}
					}
					else
					{
						if ( pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1] && (pcUnitData->eToolEquipModeTown > 0) && (pcUnitData->eToolEquipModeTown <= TOOLEQUIPMODETOWN_BackBow) )
							pcUnitData->sRightHandMatrix = pcUnitData->pcaBackBone[pcUnitData->eToolEquipModeTown - 1]->GetWorldMatrix();

						if ( pcUnitData->bRenderRightHand )
						{
							if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Claw )
							{
								PTMesh * pcBone = pcUnitData->pcBoneContainer->GetMesh( "Bip01 R Hand" );
								if ( pcBone )
								{
									EXEMatrixI sMatrix;
									EXEMatrixIdentityI( sMatrix );

									pcBone->GetWorldMatrix() = EXEMatrixMultiplyI( sMatrix, pcBone->GetWorldMatrix() );

									pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcBone );

									pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
									pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

									if ( pcModelWeapon->GetMeshes()[0] )
										pcModelWeapon->GetMeshes()[0]->GetWorldMatrix() = EXEMatrixMultiplyI( sMatrix, pcModelWeapon->GetMeshes()[0]->GetWorldMatrix() );

									pcModelWeapon->uFrame++;

									pcUnitData->sRightHandMatrix = pcBone->GetWorldMatrix();
								}
							}

							if ( pcModelWeapon->pcNewModel )
							{
								pcModelWeapon->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( pcUnitData->sRightHandTool.pcBlendingMaterial );

								pcModelWeapon->pcNewModel->Animate( pcUnitData->iFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );

								pcModelWeapon->pcNewModel->Render();
							}

							ResetItemTextureColor();
						}
					}

					pcModelWeapon->SetParentBone( NULL, NULL );
					pcModelWeapon->iLastFrame = -1;
				}
			}

			if ( pcUnitData->sLeftHandTool.pModel && pcUnitData->eWeaponItemID != pcUnitData->sLeftHandTool.eItemID )
			{
				pcModelWeapon = pcUnitData->sLeftHandTool.pModel;

                if ( pcModelWeapon && pcModelWeapon->pcNewModel )
                    pcModelWeapon->pcNewModel->scaling = vScaleModel;
				
				if ( pcUnitData->sLeftHandTool.pBone )
					pcModelWeapon->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->sLeftHandTool.pBone );

				pcModelWeapon->SetPosition( &pcUnitData->sPosi, &sAngle );
				pcModelWeapon->uFrame = (UINT)pcUnitData->iFrame;

				if ( pcUnitData->sLeftHandTool.pBone )
					pcUnitData->sLeftHandMatrix = pcUnitData->sLeftHandTool.pBone->GetWorldMatrix();

				if ( !pcUnitData->sLeftHandTool.iBlinkSpeed || !pcUnitData->sLeftHandTool.saBlinkColor[3] )
				{
					if ( pcModelWeapon->pcNewModel )
					{
						pcModelWeapon->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( pcUnitData->sLeftHandTool.pcBlendingMaterial );

						pcModelWeapon->pcNewModel->Animate( pcUnitData->iFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );

						pcModelWeapon->pcNewModel->Render();
					}

					ResetItemTextureColor();
				}
				else
					pcUnitData->bRenderLeftHand = FALSE;

				pcModelWeapon->SetParentBone( NULL, NULL );
				pcModelWeapon->iLastFrame = -1;
			}
		}

		PTModel * pcModelProjectile = NULL;

		if ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack || pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill) &&
			ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Bow &&
			pcUnitData->sRightHandTool.pModel == NULL && pcUnitData->iProjectileType == FALSE && pcUnitData->bHideRightWeapon == FALSE )
		{
			pcModelProjectile = PROJECTILE_MODEL;
			if ( pcModelProjectile && pcModelProjectile->pcNewModel )
			{
				if ( pcModelProjectile->pcNewModel )
				{
					pcModelProjectile->pcNewModel->scaling = vScaleModel;

					if ( pcUnitData->sRightHandTool.pBone )
						pcModelProjectile->SetParentBone( pcUnitData->pcBoneContainer, pcUnitData->sRightHandTool.pBone );

					pcModelProjectile->SetPosition( &pcUnitData->sPosi, &sAngle );
					pcModelProjectile->uFrame = (UINT)pcUnitData->iFrame;
					pcModelProjectile->pcNewModel->Animate( pcUnitData->iFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );
					pcModelProjectile->pcNewModel->Render();
				}

				pcModelProjectile->SetParentBone( NULL, NULL );
				pcModelProjectile->iLastFrame = -1;
			}
		}
	}

	if ( pcUnitData->iProjectileType && bRenderUnitDataWeapon )
	{
		PTModel * pcModelProjectile = NULL;

		CopyMemory( &sAngle, &pcUnitData->sProjectileAngle, sizeof( Point3D ) );
		sAngle.iY = (-sAngle.iY + PTANGLE_180) & PTANGLE_Mask;

		if ( ItemID( pcUnitData->eWeaponItemID ).ToItemType() == ITEMTYPE_Bow )
			pcModelProjectile = PROJECTILE_MODEL;
		else
		{
			pcModelProjectile = pcUnitData->sRightHandTool.pModel;
			
			sAngle.iX = (sAngle.iX - PTANGLE_90) & PTANGLE_Mask;
		}

		if ( pcModelProjectile && pcModelProjectile->pcNewModel )
		{
            pcModelProjectile->pcNewModel->scaling = vScaleModel;

			pcModelProjectile->SetPosition( &pcUnitData->sProjectilePosition, &sAngle );
			pcModelProjectile->uFrame = 0;
			pcModelProjectile->pcNewModel->Animate( pcUnitData->iFrame, Delta3D::Math::Vector3Int( sAngle.iX, sAngle.iY, sAngle.iZ ) );
			pcModelProjectile->pcNewModel->Render();
		}
	}

	RENDERER->r = iRed;
	RENDERER->g = iGreen;
	RENDERER->b = iBlue;
	RENDERER->a = iAlpha;

	if ( iDark )
		RENDERER->sLight[0].iRange = DARKLIGHTCOLOR_RANGE;

	RENDERER->iDistortionMesh = 0;

	RENDERER->iObjectVertexScale = 0;

	return TRUE;
}

BOOL ClientUnitDataUpdate( UnitData * pcUnitData )
{
	Unit * pcUnit = UNITDATATOUNIT( pcUnitData );

	CARAVANHANDLER->UpdateCaravanParticle( pcUnit );

	if ( pcUnitData->iMoveSequenceCounter == pcUnitData->iMoveSequenceEnd )
	{
		if ( (pcUnitData->GetAnimation() != ANIMATIONTYPE_Idle) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Special) &&
			(pcUnitData->GetAnimation() != ANIMATIONTYPE_Attack) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Skill) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Revive) )
		{
			if ( pcUnitData->IsPlayer() )
			{
				if ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Walking) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Potion) ||
					(pcUnitData->GetAnimation() == ANIMATIONTYPE_Flinch) )
				{
					if ( pcUnitData->psModelAnimation && ((pcUnitData->iFrame / 160) >= pcUnitData->psModelAnimation->iEndFrame) )
						pcUnitData->Animate( ANIMATIONTYPE_Idle );
					else if ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Walking) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running) )
					{
						int iMovementSpeed = 250 + 10 * pcUnitData->sCharacterData.iMovementSpeed;

						pcUnitData->Move( iMovementSpeed >> 1 );
					}
				}
				else
					pcUnitData->Animate( ANIMATIONTYPE_Idle );
			}
			else
				pcUnitData->Animate( ANIMATIONTYPE_Idle );
		}

		pcUnitData->iFrame += 80;

		int iCompareFrame = pcUnitData->iFrame / 160;
		if ( pcUnitData->psModelAnimation && (iCompareFrame >= pcUnitData->psModelAnimation->iEndFrame) )
		{
			if ( pcUnitData->psModelAnimation->bLoop )
				pcUnitData->ChangeAnimationID( pcUnitData->iAnimationID, pcUnitData->bAnimation );
			else
				pcUnitData->iFrame = pcUnitData->psModelAnimation->iEndFrame * 160;

			if ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill) ||
				 (pcUnitData->GetAnimation() == ANIMATIONTYPE_Walking) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running) )
				pcUnitData->Animate( ANIMATIONTYPE_Idle );
		}
	}
	else
	{
		MoveSequence * psMoveSequence = pcUnitData->saMoveSequence + (pcUnitData->iMoveSequenceCounter % _countof( pcUnitData->saMoveSequence ));

		EAnimationID iOldAnimationID = pcUnitData->iAnimationID;

		pcUnitData->sAngle.iX		= psMoveSequence->saAngle[0];
		pcUnitData->sAngle.iY		= psMoveSequence->saAngle[1];
		pcUnitData->sAngle.iZ		= psMoveSequence->saAngle[2];
		pcUnitData->iAnimationID	= (EAnimationID)psMoveSequence->saAngle[3];

		if ( (iOldAnimationID != pcUnitData->iAnimationID) && (pcUnitData->iAnimationID != ANIMATIONID_None) )
			if ( pcUnit )
				pcUnit->fAnimationBlendingTime = 0.4f;


		if ( pcUnitData->iAnimationID == ANIMATIONID_None )
		{
			pcUnitData->iAnimationID = iOldAnimationID;

			Point3D sDifferencePosition;
			sDifferencePosition.iX = abs( pcUnitData->sPosition.iX - psMoveSequence->sPosition.iX );
			sDifferencePosition.iY = abs( pcUnitData->sPosition.iY - psMoveSequence->sPosition.iY );
			sDifferencePosition.iZ = abs( pcUnitData->sPosition.iZ - psMoveSequence->sPosition.iZ );

			int iDistance	= sDifferencePosition.iX + sDifferencePosition.iY + sDifferencePosition.iZ;
			if ( iDistance > 8 )
			{
				if ( (pcUnitData->GetAnimation() != ANIMATIONTYPE_Walking) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Running) )
				{
					if ( iDistance < 512 )
						pcUnitData->Animate( ANIMATIONTYPE_Walking );
					else if ( !pcUnitData->Animate( ANIMATIONTYPE_Running ) )
						pcUnitData->Animate( ANIMATIONTYPE_Walking );
				}
			}
			else
			{
				if ( pcUnitData->GetAnimation() != ANIMATIONTYPE_Idle )
					pcUnitData->Animate( ANIMATIONTYPE_Idle );
			}
		}

		Point3D sOldPosition = pcUnitData->sPosition;
		pcUnitData->sPosition		= psMoveSequence->sPosition;
		if ( KNOCKUPHANDLER->CanChangeUnitY( UNITDATATOUNIT( pcUnitData ) ) == FALSE )
			pcUnitData->sPosition = sOldPosition;
		
		pcUnitData->iFloorHeight = pcUnitData->sPosition.iY;

		if ( psMoveSequence->lTargetID )
		{
			if ( (pcUnitData->pcTarget && (pcUnitData->pcTarget->iID != psMoveSequence->lTargetID)) || (pcUnitData->pcTarget == NULL) )
				pcUnitData->pcTarget = UNITDATABYID( psMoveSequence->lTargetID );
		}
		else
			pcUnitData->pcTarget = NULL;

		if ( psMoveSequence->iFrame < 0xFFFF0000 )
		{
			pcUnitData->iFrame = psMoveSequence->iFrame & 0x00FFFFFF;

			pcUnitData->iFrameType = psMoveSequence->iFrame >> 24;
			if ( pcUnitData->bAnimation )
			{
				if ( pcUnitData->iFrameType )
					pcUnitData->iAnimationMode = 2;
				else
					pcUnitData->iAnimationMode = 1;
			}

			if ( pcUnitData->iAnimationID )
			{
				if ( (pcUnitData->iAnimationMode == 2) && pcUnitData->psHead )
					pcUnitData->psModelAnimation = pcUnitData->psHead->saModelAnimation + pcUnitData->iAnimationID;
				else if ( pcUnitData->psBody )
					pcUnitData->psModelAnimation = pcUnitData->psBody->saModelAnimation + pcUnitData->iAnimationID;

				pcUnitData->iStartFrame = pcUnitData->psModelAnimation->iBeginFrame * 160;
				pcUnitData->iEndFrame = pcUnitData->psModelAnimation->iEndFrame * 160;
			}

			if ( pcUnitData->iOldAnimationType != pcUnitData->GetAnimation() )
			{
				if ( (pcUnitData->GetAnimation() >= ANIMATIONTYPE_Attack) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Attack) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Skill) )
					PlayUnitSound( pcUnitData );
				else
				{
					if ( (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster) && (pcUnitData->GetAnimation() == ANIMATIONTYPE_Idle) )
					{
						if ( RandomI( 0, 4 ) == 0 )
							PlayUnitSound( pcUnitData );
					}
				}

				if ( (pcUnitData->iAnimationEvent == 0) && ((pcUnitData->iOldAnimationType == ANIMATIONTYPE_Attack) || (pcUnitData->iOldAnimationType == ANIMATIONTYPE_Skill)) )
				{
					if ( pcUnitData->psModelAnimation && pcUnitData->psModelAnimation->iaEventFrame[0] )
					{
						CALLT_WITH_ARG1( 0x00438EF0, (DWORD)pcUnitData, FALSE );
						pcUnitData->dwLastDamageTime = 0;
						CALLT( 0x00422170, (DWORD)pcUnitData );
					}
					pcUnitData->UpdateAttack( FALSE );
				}

				pcUnitData->iAnimationEvent = 0;

				if ( psMoveSequence->saEquippedItems[3] >= 0 )
					SKILLMANAGERHANDLER->SkillAnimationStart( pcUnitData, psMoveSequence->saEquippedItems[3], 0, pcUnitData->pcTarget, 0, 0, 0 );

				if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster )
				{
					if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill )
						SKILLMANAGERHANDLER->SkillAnimationStart( pcUnitData, 0, 0, pcUnitData->pcTarget, 0, 0, 0 );

					if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack )
						CALLT( 0x00424880, (DWORD)pcUnitData );

					if ( pcUnitData->iOldAnimationType == ANIMATIONTYPE_Skill )
						pcUnitData->UpdateAttack( FALSE );
				}

				switch ( pcUnitData->GetAnimation() )
				{
					case ANIMATIONTYPE_Special:
						if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN )
						{
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY + 0x3000, pcUnitData->sPosition.iZ, 3500 );
							SKILLMANAGERHANDLER->SkillPlaySound( 0x1000, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
							SetDynamicLight( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, 50, 100, 70, 0, 300, 1 );
							PlayUnitSound( pcUnitData );
						}
						break;
					case ANIMATIONTYPE_Revive:
						SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, 500 );
						SetDynamicLight( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, 100, 100, 100, 0, 200 );
						PlayUnitSound( pcUnitData );
						break;

					case ANIMATIONTYPE_Die:
						pcUnitData->iFrameTimer = 0;
						switch ( pcUnitData->sCharacterData.iMonsterEffectID )
						{
							case MONSTEREFFECTID_DEADZONE:
								CALL_WITH_ARG1( 0x005B4340, (DWORD)pcUnitData );
								break;
							case MONSTEREFFECTID_HYPERMACHINE:
								CALL_WITH_ARG1( 0x005B4380, (DWORD)pcUnitData );
								break;
							case MONSTEREFFECTID_MYSTIC:
								CALL_WITH_ARG1( 0x005B0380, (DWORD)pcUnitData );
								break;
							case MONSTEREFFECTID_DARKMAGE:
								CALL_WITH_ARG1( 0x005B1900, (DWORD)pcUnitData );
								break;
							case MONSTEREFFECTID_SEALCRASHER:
								CALL_WITH_ARG1( 0x005B53A0, (DWORD)pcUnitData );
								break;
							default:
								break;
						}

						break;

					default:
						break;
				}
			}
		}
		else
			pcUnitData->iFrame += psMoveSequence->iFrame & 0x0000FFFF;

		pcUnitData->sActiveSkill.iValue = (int)psMoveSequence->saEquippedItems[3];
		CALLT_WITH_ARG1( 0x00438EF0, (DWORD)pcUnitData, FALSE );

		if ( (pcUnitData->sCharacterData.iType != CHARACTERTYPE_Player) && ((pcUnitData->GetAnimation() == ANIMATIONTYPE_Walking) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running)) )
		{
			if ( pcUnitData->psModelAnimation->iaEventFrame[0] )
			{
				int iCompareFrame = pcUnitData->iFrame - (pcUnitData->psModelAnimation->iBeginFrame * 160);
				if ( (pcUnitData->psModelAnimation->iaEventFrame[0] > (iCompareFrame - 80)) && (pcUnitData->psModelAnimation->iaEventFrame[0] <= iCompareFrame) )
					PlayUnitSound( pcUnitData );
			}
			if ( pcUnitData->psModelAnimation->iaEventFrame[1] )
			{
				int iCompareFrame = pcUnitData->iFrame - (pcUnitData->psModelAnimation->iBeginFrame * 160);
				if ( (pcUnitData->psModelAnimation->iaEventFrame[1] > (iCompareFrame - 80)) && (pcUnitData->psModelAnimation->iaEventFrame[1] <= iCompareFrame) )
					PlayUnitSound( pcUnitData );
			}
		}

		pcUnitData->iFPS = 80;

		if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Potion )
		{
			if ( pcUnitData->psModelAnimation->iaEventFrame[0] )
			{
				int iCompareFrame = pcUnitData->iFrame - (pcUnitData->psModelAnimation->iBeginFrame * 160);
				if ( (pcUnitData->psModelAnimation->iaEventFrame[0] > (iCompareFrame - pcUnitData->iFPS)) && (pcUnitData->psModelAnimation->iaEventFrame[0] <= iCompareFrame) )
				{
					int iHeightEffect = pcUnitData->sPosition.iY + pcUnitData->iModelHeight;
					if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player )
						iHeightEffect = pcUnitData->sPosition.iY + 0x3000;

					switch ( pcUnitData->iPotionStatus )
					{
						case POTIONSTATUS_HP:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 600 );
							PLAYMINISOUND( 20, SOUNDHANDLER->GetDistanceVolume( pcUnitData->sPosition ) );
							break;
						case POTIONSTATUS_MP:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 700 );
							PLAYMINISOUND( 20, SOUNDHANDLER->GetDistanceVolume( pcUnitData->sPosition ) );
							break;
						case POTIONSTATUS_SP:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 800 );
							PLAYMINISOUND( 20, SOUNDHANDLER->GetDistanceVolume( pcUnitData->sPosition ) );
							break;
						case POTIONSTATUS_HPMPSP:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 800 );
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 700 );
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ, 600 );
							PLAYMINISOUND( 20, SOUNDHANDLER->GetDistanceVolume( pcUnitData->sPosition ) );
							break;
						case POTIONSTATUS_EtherCore:
							SKILLMANAGERHANDLER->StartSkillEffectOld( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY + 0x3000, pcUnitData->sPosition.iZ, 3500 );
							SKILLMANAGERHANDLER->SkillPlaySound( 0x1000, pcUnitData->sPosition.iX, iHeightEffect, pcUnitData->sPosition.iZ );
							break;
						default:
							break;
					}
				}
			}
		}

		int iCompareFrame = pcUnitData->iFrame / 160;
		if ( iCompareFrame < pcUnitData->psModelAnimation->iBeginFrame )
			pcUnitData->iFrame = pcUnitData->psModelAnimation->iBeginFrame * 160;

		if ( iCompareFrame >= pcUnitData->psModelAnimation->iEndFrame )
		{
			if ( pcUnitData->psModelAnimation->bLoop )
				pcUnitData->ChangeAnimationID( pcUnitData->iAnimationID, pcUnitData->bAnimation );
			else
				pcUnitData->iFrame = pcUnitData->psModelAnimation->iEndFrame * 160;

			pcUnitData->iAttackEffect = 0;
		}

		if ( (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) || (pcUnitData->iCharacterType == CHARACTERTYPE_Player) )
		{
			//Left Hand
			if ( psMoveSequence->saEquippedItems[1] != pcUnitData->saEquippedItems[1] )
			{
				if ( psMoveSequence->saEquippedItems[1] >= 0 )
				{
					ITEMHANDLER->EquipItem( pcUnitData, ITEMHANDLER->FindItemTable( psMoveSequence->saEquippedItems[1] )->sBaseItemID.ToItemID(), 2 );
					pcUnitData->sLeftHandTool.iBlinkSpeed = 0;
					pcUnitData->sLeftHandTool.bStarEffect = FALSE;
				}
				else
					ITEMHANDLER->EquipItem( pcUnitData, ITEMID_None, 2 );
			}

			//Right Hand
			if ( psMoveSequence->saEquippedItems[2] != pcUnitData->saEquippedItems[2] )
			{
				if ( psMoveSequence->saEquippedItems[2] >= 0 )
				{
					ITEMHANDLER->EquipItem( pcUnitData, ITEMHANDLER->FindItemTable( psMoveSequence->saEquippedItems[2] )->sBaseItemID.ToItemID(), 1 );
					pcUnitData->sRightHandTool.iBlinkSpeed = 0;
					pcUnitData->sRightHandTool.bStarEffect = FALSE;
				}
				else
					ITEMHANDLER->EquipItem( pcUnitData, ITEMID_None, 1 );
			}


			//Body
			if ( psMoveSequence->saEquippedItems[0] != pcUnitData->saEquippedItems[0] )
			{
				ITEMHANDLER->EquipBody( pcUnitData, psMoveSequence->saEquippedItems[0] );
				pcUnitData->saEquippedItems[0] = psMoveSequence->saEquippedItems[0];
			}
		}

		pcUnitData->iMoveSequenceCounter++;
		pcUnitData->iMoveCount = 0;

		if ( ((pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Flinch)) &&
			(pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_MINIG) )
		{
			pcUnitData->iFireOverlay += 2;
			if ( pcUnitData->iFireOverlay > 100 )
				pcUnitData->iFireOverlay = 100;

		}
		else
		{
			pcUnitData->iFireOverlay--;
			if ( pcUnitData->iFireOverlay < 0 )
				pcUnitData->iFireOverlay = 0;
		}

		if ( pcUnitData->iIceOverlay != 0 )
		{
			if ( ((pcUnitData->iIceOverlay > 0) && (pcUnitData->iIceOverlay < 160)) || (pcUnitData->iIceOverlay < 0) )
				pcUnitData->iIceOverlay++;

			if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Die )
				pcUnitData->iIceOverlay = 0;
		}

		if ( pcUnitData->iPoison != 0 )
		{
			if ( ((pcUnitData->iPoison > 0) && (pcUnitData->iPoison < 160)) || (pcUnitData->iPoison < 0) )
				pcUnitData->iPoison++;

			if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Die )
				pcUnitData->iPoison = 0;
		}

		if ( pcUnitData->bDistortion > 0 )
			pcUnitData->bDistortion--;

		if ( ((pcUnitData->iMoveSequenceCounter % 64) == 0) && (pcUnitData->sCharacterData.iType != CHARACTERTYPE_Monster) )
		{
			pcUnitData->iLoadedMapIndex = -1;

			int iHeight = 0;

			if ( STAGE1 )
			{
				iHeight = STAGE1->GetHighestPoint( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ, pcUnitData->pcBodyMeshContainer->uMeshHeight );

				pcUnitData->iLoadedMapIndex = 0;
			}

			if ( (iHeight == (-32767)) && STAGE2 )
				pcUnitData->iLoadedMapIndex = 1;
		}

		if ( pcUnitData->dwLastDamageTime && (pcUnitData->dwLastDamageTime < TICKCOUNT) )
		{
			pcUnitData->UpdateAttack( TRUE );
			pcUnitData->dwLastDamageTime = 0;
		}

		pcUnitData->iOldAnimationType = pcUnitData->GetAnimation();

		pcUnitData->bVisible = TRUE;
	}

	if ( (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN) && (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster) )
	{
		if ( ((pcUnitData->iLoopTimer & 1) == 0) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Die) && (pcUnitData->GetAnimation() == ANIMATIONTYPE_Running) )
		{
			CALL_WITH_ARG6( 0x0048AF40, 0, 0, (RandomI(0, 48) - 24) << 8, 0, pcUnitData->sAngle.iY, 0 );
			CALL_WITH_ARG7( 0x005266B0, 0x01000001, pcUnitData->sPosition.iX + RADIAN3D_X, pcUnitData->sPosition.iY + (30 << 8), pcUnitData->sPosition.iZ + RADIAN3D_Z, 1000, 1, 0 );
		}

		if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Die )
		{
			//Red
			if ( pcUnitData->sAddColorEffect[2] > 0 )
				pcUnitData->sAddColorEffect[2]--;
			//Green
			if ( pcUnitData->sAddColorEffect[1] > 0 )
				pcUnitData->sAddColorEffect[1]--;
			//Blue
			if ( pcUnitData->sAddColorEffect[0] > 0 )
				pcUnitData->sAddColorEffect[0]--;
		}
	}

	if ( pcUnitData->sCharacterData.sHP.sMax > 0 )
	{
		if ( pcUnitData->sCharacterData.sSize )
		{
			//Xmas Goblin Size, ho ho ho! ^_^
			if ( (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_HOBGOBLIN) && (pcUnitData->sCharacterData.iLevel == 60) )
			{
				int iSize = ((pcUnitData->sCharacterData.sHP.sMax - pcUnitData->sCharacterData.sHP.sCur) * 460) / pcUnitData->sCharacterData.sHP.sMax;
				iSize += 120;
				pcUnitData->sCharacterData.sSize = iSize;
			}
		}
		
		//Moriph
		if ( (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_NPC_MORIF) && (pcUnitData->sCharacterData.iLevel == 60) )
		{
			int iSize = ((pcUnitData->sCharacterData.sHP.sMax - pcUnitData->sCharacterData.sHP.sCur) * 700) / pcUnitData->sCharacterData.sHP.sMax;
			iSize += 300;
			pcUnitData->sCharacterData.sSize = iSize;
		}
	}

	if ( pcUnitData->sCharacterData.sHP.sCur != pcUnitData->iCurHPBar )
	{
		if ( pcUnitData->iCurHPBar < pcUnitData->sCharacterData.sHP.sCur )
			pcUnitData->iCurHPBar++;
		else
			pcUnitData->iCurHPBar--;
	}

	if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Special )
	{
		if ( (pcUnitData->iFrameTimer > 10) && (pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN) )
		{
			//Red
			if ( pcUnitData->sAddColorEffect[2] > 0 )
				pcUnitData->sAddColorEffect[2] -= 8;
			//Green
			if ( pcUnitData->sAddColorEffect[1] > 0 )
				pcUnitData->sAddColorEffect[1] -= 8;
			//Blue
			if ( pcUnitData->sAddColorEffect[0] > 0 )
				pcUnitData->sAddColorEffect[0] -= 8;
			//Alpha
			if ( pcUnitData->sAddColorEffect[3] > (-256) )
				pcUnitData->sAddColorEffect[3] -= 8;

			pcUnitData->iTransparencySelf = -16;
		}
	}

	if ( pcUnitData->dwEnchantWeaponTimeOut )
	{
		if ( pcUnitData->dwEnchantWeaponTimeOut < TICKCOUNT )
		{
			pcUnitData->dwEnchantWeaponTimeOut	= 0;
			pcUnitData->iEnchantWeaponType		= 0;
		}
	}

	if ( pcUnitData->iVanishTimeLeft > 0 )
		pcUnitData->iVanishTimeLeft--;

	if ( pcUnitData->bNoDeath )
		pcUnitData->bNoDeath--;

	UNITGAME->HandleElementalWeapon( pcUnit );

	if ( pcUnitData->sCharacterData.iClanID && (pcUnitData->iClanInfoIndex < 0) && (((pcUnitData->dwClanUpdateTime + (60 * 1000)) < TICKCOUNT) || (pcUnitData->dwClanUpdateTime == 0xFFFFFFFF)) )
	{
		BOOL bFound = FALSE;
		ClanInfo * psClanInfo = (ClanInfo*)0x03AA4608;
		for ( int i = 0; i < 500; i++ )
		{
			if ( psClanInfo[i].sHeader.iClanID && (pcUnitData->sCharacterData.iClanID == psClanInfo[i].sHeader.iClanID) )
			{
				pcUnitData->iClanInfoIndex		= i;
				pcUnitData->dwClanUpdateTime	= TICKCOUNT;
				bFound = TRUE;
				break;
			}
		}

		if ( bFound == FALSE )
		{
			pcUnitData->iClanInfoIndex		= CALL_WITH_ARG1( 0x0065B900, pcUnitData->sCharacterData.iClanID );
			pcUnitData->dwClanUpdateTime	= TICKCOUNT;
		}
	}

	MOUNTHANDLER->Update( pcUnitData );

	return TRUE;
}

BOOL SendSingleDamageUnit( UnitData * pcUnitData, int iDamageMin, int iDamageMax, int iState, int iResistance, int iSkillID, BOOL bChance, SocketData * pcSocketData )
{
	if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
	{
		if ( (pcUnitData && (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster)) || (pcUnitData == nullptr) )
			return FALSE;
	}

	ANTICHEATHANDLER->CheckThread();

	PacketSkillDamageNormalEx sPacket;

	ZeroMemory( &sPacket, sizeof( PacketSkillDamageNormalEx ) );

	if ( (UNITDATA->iEndFrame - UNITDATA->iStartFrame) <= (160 * 5) )
		return FALSE;

    int iRestarterTime = RESTARTCOUNTER;
    if ( UNITDATA->bNoDeath )
        iRestarterTime = (int)UNITDATA->bNoDeath;

    if ( iRestarterTime > 0 )
        return FALSE;

	DAMAGESINGLE_COUNT++;

	if ( bChance )
	{
		int iOldAttackRating = 0;
		int iChanceDefense = 0;
		int iChance = 0;
		int iDamage = 0;

		switch ( pcUnitData->sCharacterData.iType )
		{
			case CHARACTERTYPE_Monster:

				BATTLETIME = TICKCOUNT;

				iOldAttackRating = UNITDATA->sCharacterData.iAttackRating;

				if ( UNITDATA->iPetID )
				{
					switch ( UNITDATA->iPetID )
					{
						case SKILLID_ScoutHawk:
							iDamage = GetDamageFromAreaSkill( UNITDATA, pcUnitData, 0, 0, 0 );
							if ( iDamage > 0 )
								UNITDATA->sCharacterData.iAttackRating += (UNITDATA->sCharacterData.iAttackRating * iDamage) / 100;
							break;

						default:
							break;
					}
				}

				iChanceDefense = 100 - UNITGAME->DefendChance( pcUnitData->sCharacterData.iDefenseRating, UNITDATA->sCharacterData.iAttackRating );
				UNITDATA->sCharacterData.iAttackRating = iOldAttackRating;

				iChance = rand() % 100;
			
				if ( pcUnitData->IsPlayer() == FALSE )
				{
					if ( DEBUG_GM && ISPRESSEDKEY( VK_SHIFT ) )
						ChatGame::AddChatBoxTextEx( CHATCOLOR_Error, "> Chance: %d / %d [%s]", iChance, iChanceDefense, iChance > iChanceDefense ? "Miss" : "Hit" );


					if ( iChance > iChanceDefense )
						return FALSE;
				}
				break;

			case CHARACTERTYPE_NPC:
				if ( ITEMONMOUSE )
					return FALSE;

				iState		= iResistance;
				iResistance = 0;
				break;

			default:
				break;
		}
	}

	AddUnitQueueID( pcUnitData->sCharacterData.iStatPoints );

	sPacket.iLength = sizeof( PacketSkillDamageNormalEx );
	sPacket.iHeader = PKTHDR_SingleDamage;

	sPacket.sPosition = pcUnitData->sPosition;

	sPacket.iAttackRating = UNITDATA->sCharacterData.iAttackRating;

	sPacket.sAttackPower.sMin = UNITDATA->sCharacterData.iMinDamage;
	sPacket.sAttackPower.sMax = UNITDATA->sCharacterData.iMaxDamage;

	if ( iDamageMin || iDamageMax )
	{
		sPacket.WeaponAttackPower[0] = iDamageMin;
		sPacket.WeaponAttackPower[1] = iDamageMax;
	}
	else
	{
		if ( iSkillID == 0 )
		{
			ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1];		
			if ( pcItemData )
			{
				sPacket.WeaponAttackPower[0] = sPacket.sAttackPower.sMin - pcItemData->sItem.sDamage.sMin;
				sPacket.WeaponAttackPower[1] = sPacket.sAttackPower.sMax - pcItemData->sItem.sDamage.sMax;
			}
		}
	}

	sPacket.saStatus[0] = UNITDATA->sCharacterData.iCritical;

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:
		case CHARACTERCLASS_Mechanician:
		case CHARACTERCLASS_Pikeman:
		case CHARACTERCLASS_Knight:
		case CHARACTERCLASS_Assassin:
			sPacket.saStatus[1] = UNITDATA->sCharacterData.iStrength;
			break;

		case CHARACTERCLASS_Magician:
		case CHARACTERCLASS_Priestess:
		case CHARACTERCLASS_Shaman:
			sPacket.saStatus[1] = UNITDATA->sCharacterData.iSpirit;
			break;

		case CHARACTERCLASS_Archer:
		case CHARACTERCLASS_Atalanta:
			sPacket.saStatus[1] = UNITDATA->sCharacterData.iAgility;
			break;

		default:
			break;
	}

	sPacket.iDamageState = iState;
	sPacket.iRange = 60 << 8;
	sPacket.MotionCount[0] = UNITDATA->iAnimationEvent;
	sPacket.iDamageCount = DAMAGESINGLE_COUNT;

	sPacket.dwTime = TICKCOUNT;
	sPacket.iSkillID = iSkillID;

	sPacket.dwWeaponCode = UNITDATA->eWeaponItemID;

	//Automation Drain
	if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Mechanician )
	{
		if ( iSkillID == SKILLID_None )
		{
			if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Automation ) )
				CHARACTERGAME->SetDecreaseAutomation();
		}
	}

	if ( UNITDATA->iLoadedMapIndex >= 0 && pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster )
		sPacket.Area[0] = FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapID;
	else
		sPacket.Area[0] = -1;

	sPacket.iTargetReceiveDamageID = UNITDATA->iID;
	sPacket.iID = pcUnitData->iID;
	pcUnitData->iTryAttackCount++;

	sPacket.dwCheckSum = CALL_WITH_ARG1( 0x00408280, (DWORD)&sPacket );

	//Check Damage
	CALL( 0x00408DF0 );

	SocketData * pcSocketSend = SOCKETG;

	if ( UNITDATA != pcUnitData )
		pcSocketSend = GETSERVER_SOCKET( pcUnitData->iServerCode );

	if ( pcSocketData )
		pcSocketSend = pcSocketData;

	if ( pcSocketSend )
	{
		//Encode
		CALL_WITH_ARG1( READDWORD( 0x0A04758 ), (DWORD)&sPacket );

		SOCKETGAME->SendPacket( pcSocketSend, &sPacket, FALSE );

		//Show Bar
		CALL_WITH_ARG2( 0x00408340, (DWORD)pcUnitData, iSkillID );

		return TRUE;
	}

	return FALSE;
}

BOOL SendAreaDamageUnit( int iX, int iY, int iZ, UnitData * pcUnitData, int iDamageMin, int iDamageMax, int iState, int iResistance, int iSkillID )
{
	ANTICHEATHANDLER->CheckThread();

	PacketSkillDamageRangeEx * psPacket = (*(PacketSkillDamageRangeEx**)0x00A04760);

	if ( psPacket == NULL )
		return FALSE;

	if ( psPacket->iCount == 0 && pcUnitData == NULL )
	{
		ZeroMemory( psPacket, sizeof( PacketSkillDamageRangeEx ) );
		DELET( psPacket );
		WRITEDWORD( 0x00A04760, 0 );
		return FALSE;
	}

    int iRestarterTime = RESTARTCOUNTER;
    if ( UNITDATA->bNoDeath )
        iRestarterTime = (int)UNITDATA->bNoDeath;

	if ( (iRestarterTime > 0) || (UNITGAME->IsBlockedRangeTarget() == TRUE) )
	{
		ZeroMemory( psPacket, sizeof( PacketSkillDamageRangeEx ) );
		DELET( psPacket );
		WRITEDWORD( 0x00A04760, 0 );
		return FALSE;
	}

	if ( (UNITDATA->iEndFrame - UNITDATA->iStartFrame) <= (160 * 5) )
	{
		ZeroMemory( psPacket, sizeof( PacketSkillDamageRangeEx ) );
		DELET( psPacket );
		WRITEDWORD( 0x00A04760, 0 );
		return FALSE;
	}

	DAMAGESINGLE_COUNT++;

	psPacket->iLength = (sizeof( PacketSkillDamageRangeEx ) - (sizeof( int ) * 128));
	psPacket->iLength += (sizeof( int ) * psPacket->iCount) + 16;
	psPacket->iHeader = PKTHDR_RangeDamage;
	psPacket->sPosition.iX = iX;
	psPacket->sPosition.iY = iY;
	psPacket->sPosition.iZ = iZ;
	psPacket->iDamageCount = DAMAGESINGLE_COUNT;
	psPacket->dwTime = TICKCOUNT;
	psPacket->iDamageState = iState;

	if ( pcUnitData )
	{
		if ( PRECISIONTARGET_RANGE )
		{
			for ( int i = 0; i < psPacket->iCount; i++ )
			{
				if ( psPacket->iaTargetID[i] == pcUnitData->iID )
				{
					psPacket->iTargetID = pcUnitData->iID;
					break;
				}
			}
		}
		else
			psPacket->iTargetID = pcUnitData->iID;

		pcUnitData->iTryAttackCount++;
	}

	psPacket->iSkillID = iSkillID;
	psPacket->saStatus[0] = UNITDATA->sCharacterData.iCritical;
	psPacket->iTargetReceiveDamageID = UNITDATA->iID;
	psPacket->sAttackPower.sMin = UNITDATA->sCharacterData.iMinDamage;
	psPacket->sAttackPower.sMax = UNITDATA->sCharacterData.iMaxDamage;
	psPacket->WeaponAttackPower[0] = iDamageMin;
	psPacket->WeaponAttackPower[1] = iDamageMax;
	psPacket->iAttackRating = UNITDATA->sCharacterData.iAttackRating;

	switch ( UNITDATA->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Fighter:
		case CHARACTERCLASS_Mechanician:
		case CHARACTERCLASS_Pikeman:
		case CHARACTERCLASS_Knight:
		case CHARACTERCLASS_Assassin:
			psPacket->saStatus[1] = UNITDATA->sCharacterData.iStrength;
			if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Knight )
				if ( UNITDATA->sActiveSkill.cSkillTinyID == SKILLPLAYID_T52 )
					psPacket->iAttackRating += (UNITDATA->sCharacterData.iAttackRating * iaKnightT52AtkRtg[UNITDATA->sActiveSkill.cLevel - 1]) / 100;
			break;

		case CHARACTERCLASS_Magician:
		case CHARACTERCLASS_Priestess:
		case CHARACTERCLASS_Shaman:
			psPacket->saStatus[1] = UNITDATA->sCharacterData.iSpirit;
			break;

		case CHARACTERCLASS_Archer:
		case CHARACTERCLASS_Atalanta:
			psPacket->saStatus[1] = UNITDATA->sCharacterData.iAgility;
			break;

		default:
			break;
	}

	psPacket->iRange = 240;

	psPacket->dwWeaponCode = UNITDATA->eWeaponItemID;

	if ( UNITDATA->iLoadedMapIndex >= 0 )
		psPacket->sMapID = FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapID;

	psPacket->dwChkSum = CALL( 0x00418DD0 );

	//Send
	CALL( 0x00408DF0 );

	SocketData * pcSocketSend = (SocketData*)CALL( 0x0061EB70 );

	if ( pcSocketSend && psPacket->iCount > 0 )
	{
		SOCKETGAME->SendPacket( pcSocketSend, psPacket, FALSE );

		CALL_WITH_ARG1( 0x00408380, psPacket->iSkillID );
	}

	ZeroMemory( psPacket, sizeof( PacketSkillDamageRangeEx ) );
	DELET( psPacket );
	WRITEDWORD( 0x00A04760, 0 );

	return TRUE;
}

BOOL FreeUnitData( UnitData * pcUnitData )
{
//	if ( MOUNTHANDLER )
//		MOUNTHANDLER->RemoveMount( pcUnitData );

	if ( auto pcUnit = UNITDATATOUNIT( pcUnitData ) )
		pcUnit->vMeshesAnimation.clear();

	DX::ModelFactory::FreeModel( pcUnitData );

	auto DeleteModelData = []( EXEModelData * ps )->void
	{
		CALL_WITH_ARG1( 0x0045A5F0, (DWORD)ps );
	};

	pcUnitData->bActive = FALSE;

	pcUnitData->iClanInfoIndex = (-1);

	if ( pcUnitData->pcBody )
	{
		DeleteModelData( pcUnitData->pcBody );
		pcUnitData->pcBody = NULL;
	}

	if ( pcUnitData->pcHead )
	{
		DeleteModelData( pcUnitData->pcHead );
		pcUnitData->pcHead = NULL;
	}

	if ( pcUnitData->pcLeftHandTool )
	{
		DeleteModelData( pcUnitData->pcLeftHandTool );
		pcUnitData->pcLeftHandTool = NULL;
	}

	if ( pcUnitData->pcRightHandTool )
	{
		DeleteModelData( pcUnitData->pcRightHandTool );
		pcUnitData->pcRightHandTool = NULL;
	}

	pcUnitData->pcBodyMeshContainer = NULL;
	pcUnitData->pcHeadMeshContainer = NULL;

	pcUnitData->sLeftHandTool.pModel = NULL;
	pcUnitData->sRightHandTool.pModel = NULL;
	pcUnitData->sLeftHandTool.eItemID = ITEMID_None;
	pcUnitData->sRightHandTool.eItemID = ITEMID_None;

	pcUnitData->psBody = NULL;
	pcUnitData->psHead = NULL;

	pcUnitData->eSkillDisplayFlag = 0;

	if ( pcUnitData->psaDamageUsersData )
	{
		DELETA( pcUnitData->psaDamageUsersData );
	}

	if ( pcUnitData->pcaUnitDataTargetList )
	{
		DELETA( pcUnitData->pcaUnitDataTargetList );
	}

	if ( (pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) || (pcUnitData->iCharacterType == CHARACTERTYPE_Player) )
		pcUnitData->psModelAnimation = ITEMHANDLER->GetModelDefault( pcUnitData->sCharacterData.iClass );

	Unit * pcUnit = UNITDATATOUNIT( pcUnitData );
	pcUnit->uElementWeaponID = 0;

	MAGNETICDISCHARGEHANDLER->OnCastEnd( pcUnit );

	return TRUE;
}

int SaveCharacterFrames( UnitData * pcUnitData )
{
	pcUnitData->iMoveSequenceCounter = (pcUnitData->iMoveSequenceCounter + 1) & 255;

	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saAngle[0] = pcUnitData->sAngle.iX;
	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saAngle[1] = pcUnitData->sAngle.iY;
	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saAngle[2] = pcUnitData->sAngle.iZ;
	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saAngle[3] = pcUnitData->iAnimationID;

	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].sPosition = pcUnitData->sPosition;
	if ( KNOCKUPHANDLER->CanChangeUnitY( UNITDATATOUNIT( pcUnitData ) ) == FALSE )
		pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].sPosition.iY = pcUnitData->iFloorHeight;

	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].iFrame = (pcUnitData->iFrameType << 24) | pcUnitData->iFrame;

	if ( pcUnitData->pcTarget )
		pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].lTargetID = pcUnitData->pcTarget->iID;
	else
		pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].lTargetID = 0;

	CopyMemory( pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saEquippedItems, pcUnitData->saEquippedItems, sizeof( short ) * 4 );

	return pcUnitData->iMoveSequenceCounter;
}

int SaveCharacterFramesEx( UnitData * pcUnitData )
{
	pcUnitData->iMoveSequenceCounter = (pcUnitData->iMoveSequenceCounter + 1) & 255;

	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saAngle[0] = pcUnitData->sAngle.iX;
	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saAngle[1] = pcUnitData->sAngle.iY;
	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saAngle[2] = pcUnitData->sAngle.iZ;
	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saAngle[3] = pcUnitData->iAnimationID;

	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].sPosition = pcUnitData->sPosition;
	if ( KNOCKUPHANDLER->CanChangeUnitY( UNITDATATOUNIT( pcUnitData ) ) == FALSE )
		pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].sPosition.iY = pcUnitData->iFloorHeight;

	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].iFrame = (pcUnitData->iFrameType << 24) | pcUnitData->iFrame;

	pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].lTargetID = 0;

	if ( pcUnitData->pcAttackTarget )
		pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].lTargetID = pcUnitData->pcAttackTarget->iID;

	if ( pcUnitData->pcFocusTarget )
		pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].lTargetID = pcUnitData->pcFocusTarget->iID;

	CopyMemory( pcUnitData->saMoveSequence[pcUnitData->iMoveSequenceCounter].saEquippedItems, pcUnitData->saEquippedItems, sizeof( short ) * 4 );

	return pcUnitData->iMoveSequenceCounter;
}

BOOL __cdecl DeleteEXEModelData( EXEModelData * psModelData )
{
	if ( psModelData->iCount > 0 )
	{
		psModelData->iCount--;

        if ( psModelData->psUnused && (psModelData->iCount == 0) )
            DeleteEXEModelData( psModelData->psUnused );

        if ( psModelData->psMotion && (psModelData->iCount == 0) )
            DeleteEXEModelData( psModelData->psMotion );

		if ( psModelData->iCount <= 0 )
		{
			DELET( psModelData->pcModel );

			DELET( psModelData->psModelData );

			DELET( psModelData->m_ModelsGroup );
		}
	}
	return 0;
}

void RenderBlessCastleCrown( UnitData * pcUnitData )
{
	Point3D sCameraPosition;
	sCameraPosition.iX = UNITTARGETPOSITION_X;
	sCameraPosition.iY = UNITTARGETPOSITION_Y;
	sCameraPosition.iZ = UNITTARGETPOSITION_Z;

	Point3D sCameraAngle;
	sCameraAngle.iX = UNITTARGETANGLE_X;
	sCameraAngle.iY = UNITTARGETANGLE_Y;
	sCameraAngle.iZ = UNITTARGETANGLE_Z;

	BOOL bLeader = STRINGCOMPAREI( ((char *)0x03B36C10), pcUnitData->sCharacterData.szName );

	if ( pcUnitData->bBlessCastleCrown && pcUnitData->sCharacterData.iClanID && BLESSCASTLE_BLUE_CROWN_MODEL && BLESSCASTLE_GOLDEN_CROWN_MODEL && pcUnitData->bRendered && (pcUnitData->sRenderPoint.iZ < (12 * 64 * 256)) && (pcUnitData->iVanishTimeLeft <= 0) )
	{
		GetMoveLocation( 0, 0, 0, 0, pcUnitData->sAngle.iY, 0 );

		Point3D sCrownPosition;
		sCrownPosition.iX = pcUnitData->sPosition.iX + RADIAN3D_X;
		sCrownPosition.iY = pcUnitData->sPosition.iY + pcUnitData->iModelHeight + 11 * 256;
		sCrownPosition.iZ = pcUnitData->sPosition.iZ + RADIAN3D_Z;

		int cnt = (TICKCOUNT >> 2) & 0xFF;

		sCrownPosition.iY += (cnt & 0x80) ? (cnt & 0x7F) : (0x80 - (cnt & 0x7F));

		Point3D sCrownAngle;
		sCrownAngle.iX = 0;
		sCrownAngle.iY = (TICKCOUNT >> 2) & 4095;
		sCrownAngle.iZ = 0;

		GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
		RENDERER->SetTextureManager( EXETEXTUREMANAGER_DEFAULT );

		PTModel * pcCrownModel = bLeader ? BLESSCASTLE_BLUE_CROWN_MODEL : BLESSCASTLE_GOLDEN_CROWN_MODEL;
		if ( pcCrownModel )
		{
			pcCrownModel->SetPosition( &sCrownPosition, &sCrownAngle );
			pcCrownModel->Render( &sCameraPosition, &sCameraAngle );
		}

		GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	}
}

void RenderPTModel( PTModel * pcModel, X3D::Vector3 sPosition, X3D::Vector3 sAngle, X3D::Vector3 sCameraPosition, X3D::Vector3 sCameraAngle )
{
#ifdef _PT_NEW3DENGINE

	Delta3D::Math::Vector3 sPositionModel = Delta3D::Math::Vector3( sPosition.x, sPosition.y, sPosition.z );
	Delta3D::Math::Vector3Int sAngleModel = Delta3D::Math::Vector3Int( (int)sAngle.x, (int)sAngle.y, (int)sAngle.z );

	pcModel->SetPositionRotation( &sPositionModel, &sAngleModel );

	pcModel->Render();
#else

#endif
}
