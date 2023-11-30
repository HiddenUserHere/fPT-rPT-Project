#include "stdafx.h"
#include "DLL.h"
#include <ShellAPI.h>

#ifdef _WIN32
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif

int iGamePort = 10009;


extern GameCore			* pGameCoreHandler;

extern BOOL __cdecl SendSingleDamageUnit( UnitData * pcUnitData, int iDamageMin, int iDamageMax, int iState, int iResistance, int iSkillID, BOOL bChance, SocketData * pcSocketData );
extern BOOL __cdecl	SendAreaDamageUnit( int iX, int iY, int iZ, UnitData * pcUnitData, int iDamageMin, int iDamageMax, int iState, int iResistance, int iSkillID );

extern BOOL __cdecl RenderUnitData( UnitData * pcUnitData, bool bRenderUnitDataBody, bool bRenderUnitDataHead, bool bRenderUnitDataWeapon );
extern BOOL __cdecl FreeUnitData( UnitData * pcUnitData );

extern int __cdecl	SaveCharacterFrames( UnitData * pcUnitData );
extern int __cdecl	SaveCharacterFramesEx( UnitData * pcUnitData );

void __cdecl ReOrderVolume()
{
	static int iVolumeEffectOld = -15;

	if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic )
		SOUNDHANDLER->SetVolumeMusic( 0 );

	if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoSoundEffects )
	{
		SOUNDHANDLER->SetVolumeEffect( 0 );
		iVolumeEffectOld = 0;
	}
	else
	{
		//if ( SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume != iVolumeEffectOld )
		{
			SOUNDHANDLER->SetVolumeEffect( SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume );
			iVolumeEffectOld = SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume;
		}
	}
}

void __cdecl LoadMusic( int iNum )
{
	SOUNDHANDLER->LoadMusic( iNum );
}

BOOL __cdecl ChangeMusic( int iID )
{
	return SOUNDHANDLER->ChangeBackgroundMusic( iID );
}

void __cdecl DrawTextGameHook( int FontObject, int x, int y, const char * Text, int isize, UINT uFormat, D3DCOLOR Color )
{
	RECT font_rect;
	SetRect( &font_rect, x, y, *(UINT*)0x3A9A30C + x, *(UINT*)0x3A9A310 + y );

	GetGraphicsDevice()->GetDefaultFont()->Draw( Text, font_rect, uFormat, DX::Color( Color ) );
}

BOOL bFirstGetText = FALSE;

void WINAPI GetEditTextW( HWND hwnd, LPSTR lpText, INT Count )
{
	if( hwnd )
	{
		if( (DWORD)lpText == 0x039033E8 )
		{
			if( !bFirstGetText )
			{
				STRINGCOPYLEN( lpText, Count, SETTINGSHANDLER->GetModel()->GetSettings().szLastAccount );
				SetWindowTextA( hwnd, lpText );
				int iCount = lstrlenA( lpText );
				SendMessageA( hwnd, EM_SETSEL, iCount, iCount );
				bFirstGetText = TRUE;
				return;
			}
		}

		GetWindowTextA( hwnd, lpText, Count );
	}
}

bool __cdecl SummonCrystal()
{
	return ITEMHANDLER->DelayCristal();
}

BOOL __cdecl OnNpcQuestClick( DWORD dwCode )
{
	return QUESTGAMEHANDLER->OnClickNpc( dwCode );
}

void __cdecl OnClickOKQuestMessageBox()
{
	QUESTGAMEHANDLER->OnClickOkMessageBox( UIEventArgs{} );
}

BOOL __cdecl OnClickQuestWindow( DWORD dwQuestID )
{
	return QUESTGAMEHANDLER->OnClickQuestWindow( dwQuestID );
}

BOOL __cdecl OnDescriptionItemMixType( Item * psItemInfo )
{
	return TRUE;
}

BOOL __cdecl OnAddColorDescription( int iLine )
{
	return TRUE;
}

void __stdcall DrawTextDXHook( int FontObject, int x, int y, LPCTSTR Text, int size )
{
	std::string pText = Text;
	DrawTextGameHook( 0, x, y, Text, size, DT_WORDBREAK, *(UINT*)0x3A9A32C );
}

BOOL __cdecl IsDebuffSkill( UnitData * pcUnit, DWORD dwSkillCode )
{
	return SKILLMANAGERHANDLER->IsDebuffSkill( pcUnit, dwSkillCode );
}

BOOL __cdecl OnReceivePacket( void * pPacket )
{
	return FALSE;
}

void __cdecl OnLoadArmorCharacter()
{
//	ITEMHANDLER->OnLoadArmorCharacter();
}

BOOL __cdecl OnSetArmorCurrentUnit( UINT uItemCode )
{
	return ITEMHANDLER->OnSetArmorUnit( UNITDATA, uItemCode );
}

BOOL __cdecl OnSetArmorTargetUnit( UnitData * psUnit, UINT uPosition )
{
	return ITEMHANDLER->OnSetArmorTargetUnit( psUnit, uPosition );
}

int __cdecl GetAbsorptionPlayer()
{
	return UNITGAME->GetCharacterAbsorption();
}

void __cdecl OnCharacterUpdateData()
{
	CHARACTERGAME->OnCharacterUpdateData();
}

int __cdecl DefendChance( int iDefenseRating, int iAttackRating )
{
	return UNITGAME->DefendChance( iDefenseRating, iAttackRating );
}

int __cdecl BlockChance( int iBlockRating, int iBlock )
{
	return UNITGAME->BlockChance( iBlockRating, iBlock );
}

int __cdecl GetWidthNormalFont( const char * Text )
{
	return GETWIDTHTEXT( Text );
}

int __stdcall FormatSkillTimerInfo( struct Skill * pSkill )
{
	return SKILLMANAGERHANDLER->FormatSkillTimerInfo( pSkill );
}

INT64 __cdecl OnLoseExp()
{
	return UNITGAME->OnLoseEXP();
}

void __cdecl OnDrawUI()
{
	UIHANDLER->Render();
}

BOOL __stdcall OnSendPacket( void * pPacket, BOOL bEncrypt )
{
	SocketData * sd;
	READTHIS( sd );
	if ( sd )
	{
		if ( sd == SOCKETL )
			SOCKETGAME->SendPacketLogin( (Packet *)pPacket, bEncrypt );
		else
			SOCKETGAME->SendPacketGame( (Packet *)pPacket, bEncrypt );
	}

	return TRUE;
}

SocketData * __cdecl OnConnect( const char * pszIP, int iPort )
{
	return SOCKETGAME->Connect( pszIP, iPort );
}

void __cdecl OnSocketPacket( SocketData * sd, void * pPacket )
{
//	SOCKETGAME->SocketPacket( sd, (PacketReceiving*)pPacket );
}

void __stdcall OnSocketClose()
{
	SocketData * sd;
	READTHIS( sd );
	if ( sd )
	{
		SOCKETGAME->SocketClose( sd );
	}
}

void __cdecl OnCharacterSetCharInfoEx()
{
	CHARACTERGAME->OnCharacterSetCharInfoEx();
}

void __cdecl OnWarpCastle( BOOL bLeader, int * piX, int * piZ )
{
	BLESSCASTLEHANDLER->WarpCastle( bLeader, piX, piZ );
}

int __cdecl OnClickPremium( ItemData * pcItemData )
{
	return ITEMHANDLER->OnUseItem( pcItemData );
}

int __cdecl LoadMaps()
{
	MAPGAME->Load();
	return 1;
}

void __cdecl OnLoginDraw()
{
	LOGINHANDLER->OnRender();
}

void __cdecl RenderSubServerBox()
{
	LOGINHANDLER->RenderServerSelectBox();
}

void __cdecl RenderBGLogin()
{
	LOGINHANDLER->RenderBGLogin();
}

char * __cdecl OnDescriptionItem( DWORD dwCode )
{
	char * pszDescription = NULL;
	return pszDescription;
}

int __stdcall ChangeMotion( int iMotion, int iAnimation )
{
	UnitData * pptr;
	READTHIS( pptr );

	return CHARACTERGAME->OnMotionChange( (UnitData*)pptr, iMotion, iAnimation );
}

BOOL __cdecl ResetAndRecreateDevices()
{
	GRAPHICENGINE->sResolution.uWidth -= 1;
	return GRAPHICENGINE->Reset( SETTINGSHANDLER->GetModel()->GetSettings().bWindowed, SETTINGSHANDLER->GetModel()->GetSettings().dwWidth, SETTINGSHANDLER->GetModel()->GetSettings().dwHeight, TRUE );
}

void __cdecl OnLostDevice()
{
	GRAPHICENGINE->OnLostDevice();
}

void __cdecl Begin3D()
{
	GRAPHICENGINE->Begin3D();
}

void __cdecl End3D()
{
	GRAPHICENGINE->End3D();
}

void __cdecl ShowExpValueFormat( char * szBuf, int expType )
{
	// Retorna o valor da exp nos dados do char com virgulas ( ex: 1,000 )
	if( expType == 0 )
		wsprintfA( szBuf, "%sM", FormatNumber( CALL( 0x00497AB0 ) ) );
	else if( expType == 1 )
		wsprintfA( szBuf, "%s", FormatNumber( CALL( 0x00497AB0 ) ) );
	else if( expType == 2 )
		wsprintfA( szBuf, "%sM", FormatNumber( CALL( 0x00497AE0 ) ) );
	else if( expType == 3 )
		wsprintfA( szBuf, "%s", FormatNumber( CALL( 0x00497AE0 ) ) );
	else if( expType == 4 )
		wsprintfA( szBuf, "EXP     : %sM", FormatNumber( CALL_WITH_ARG4( 0x00768140, READDWORD( 0x020802E8 ), READDWORD( 0x020802E8 + 0x48 ), 0x0F4240, 0 ) ) );
	else if( expType == 5 )
	{
		UINT expv1 = *(UINT*)0x020802E8;
		if( expv1 > 999999 )
		{
			expv1 = expv1 / 1000000;
			wsprintfA( szBuf, "EXP     : %sM", FormatNumber( expv1 ) );
		}
		else
			wsprintfA( szBuf, "EXP     : %s", FormatNumber( expv1 ) );
	}
	// Gold
	else if( expType == 6 )
		wsprintfA( szBuf, "Gold    : %s", FormatNumber( READDWORD( 0x020802F0 ) ) );
}

void __cdecl OnSendPlayDataEx()
{
	UNITGAME->SendUnitDataEx();
}

void __cdecl OnReceiveCommand( char * pszBuff )
{
	CHATGAMEHANDLER->AnalyzeChat( pszBuff );
}

void __stdcall OnStopHitInfo( int iType, int iValue )
{
	DAMAGEINFOHANDLER->SetDefenseInfo( iType, iValue );
}

int __cdecl OnItemPivot( int iPosition, UINT uItemID )
{
	return ITEMHANDLER->OnItemPivot( iPosition, uItemID );
}

extern BOOL bShowFps;
extern BOOL	bDebug;

void __cdecl DrawGMDebugCharacterInfo( int iDebug )
{
	//Render Character Debug Text
	if( iDebug == 1 && GM_MODE && *(int*)0x35E11D8 != 0 && DEBUG_GM )
		UNITGAME->RenderCharacterDebugText();
}

void __cdecl ExitGame()
{
	INI::CReader cReader( "game.ini" );

	if ( (char*)0x39033E8 )
		cReader.WriteString( "Game", "LastAccount", (char*)0x39033E8 );



	if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() )
	{
		WRITEDWORD( 0x00CF476C, (TICKCOUNT + 2000) );
	}
}

void __cdecl OnDrawUILogin()
{
//	CALL( 0x0059E7A0 );
//	GameCore::GetInstance()->Render2D();
}

BOOL __cdecl BeginSkill( UnitData * pcThis, int iSkillID, int iLevel, UnitData * pcUnitData, int iX, int iY, int iZ )
{
	if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
	{
		if ( (UNITDATA->pcAttackTarget && (UNITDATA->pcAttackTarget->sCharacterData.iType == CHARACTERTYPE_Monster)) || (UNITDATA->pcAttackTarget == nullptr) )
			return false;
	}

	SKILLMANAGERHANDLER->SkillAnimationStart( pcThis, iSkillID, iLevel, pcUnitData, iX, iY, iZ );

	return TRUE;
}

bool __cdecl PlaySkillAttack( struct Skill * psSkill, class UnitData * pcUnitData )
{
	if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
	{
		if ( (UNITDATA->pcAttackTarget && (UNITDATA->pcAttackTarget->sCharacterData.iType == CHARACTERTYPE_Monster)) || (UNITDATA->pcAttackTarget == nullptr) )
			return false;
	}

	if( psSkill && psSkill->UseSkillFlag )
		SKILLMANAGERHANDLER->ResetBuff( psSkill->iID, pcUnitData );

	if( psSkill != NULL && SKILLMANAGERHANDLER->SkillPlay( psSkill, pcUnitData ) )
		return true;

	bool bRes = false;
	__asm
	{
		PUSH pcUnitData;
		PUSH psSkill;
		MOV EAX, 0x004160D0;
		CALL EAX;
		ADD ESP, 0x08;
		MOV bRes, AL;
	}
	return bRes;
}

BOOL __cdecl OpenPlaySkill( struct Skill * psSkill )
{
	if ( MOUNTHANDLER->IsRiding( UNITDATA ) )
	{
		if ( (UNITDATA->pcAttackTarget && (UNITDATA->pcAttackTarget->sCharacterData.iType == CHARACTERTYPE_Monster)) || (UNITDATA->pcAttackTarget == nullptr) )
			return FALSE;
	}

	if ( psSkill && psSkill->UseSkillFlag )
	{
		if ( UNITDATA->pcAttackTarget == NULL )
			SKILLMANAGERHANDLER->ResetBuff( psSkill->iID, UNITDATA );
	}
	if( SKILLMANAGERHANDLER->OpenPlaySkill( psSkill ) )
		return TRUE;

	BOOL bRes = FALSE;
	__asm
	{
		PUSH psSkill;
		MOV EAX, 0x00415F80;
		CALL EAX;
		ADD ESP, 0x04;
		MOV bRes, EAX;
	}

	return bRes;
}

BOOL __cdecl EventSkill( class UnitData * pcThis )
{
	if ( pcThis->sCharacterData.iType == CHARACTERTYPE_Monster )
	{
		if ( pcThis->iCharacterType != CHARACTERTYPE_Player )
		{
			if ( SKILLMANAGERHANDLER->EventSkillMonster( pcThis ) )
				return TRUE;
		}
	}

	if ( SKILLMANAGERHANDLER->EventSkill( pcThis ) )
		return TRUE;

	BOOL bRes = FALSE;
	__asm
	{
		MOV ECX, pcThis;
		MOV EAX, 0x004386F0;
		CALL EAX;
		MOV bRes, EAX;
	}

	return bRes;
}

BOOL __cdecl EndSkill( class UnitData * pcThis )
{
	if( SKILLMANAGERHANDLER->EndSkill( pcThis ) )
		return TRUE;
	__asm
	{
		MOV ECX, pcThis;
		MOV EAX, 0x004250F0;
		CALL EAX;
	}
	return TRUE;
}

BOOL __stdcall PrepareSkillInfo( Skill * psSkill, SkillBox * psSkillBox, int iIcon )
{
	if( SKILLINFOBOX->CanPrepareShowSkill( psSkill, Rectangle2D( psSkillBox->BoxRect.left, psSkillBox->BoxRect.top, psSkillBox->BoxRect.right, psSkillBox->BoxRect.bottom ) ) )
	{
		CALLT_WITH_ARG3( 0x004EDC90, 0x0367E148, (DWORD)psSkill, (DWORD)psSkillBox, iIcon );	
		SKILLDATAHANDLER->OnInformationHandler( psSkill, psSkill->iLevel );
		SKILLINFOBOX->PrepareShowSkill( psSkill, FALSE, Rectangle2D( psSkillBox->BoxRect.left, psSkillBox->BoxRect.top, psSkillBox->BoxRect.right, psSkillBox->BoxRect.bottom ) );
	}
	
	return TRUE;
}

BOOL __stdcall PrepareItemInfo( ItemData * psItemData, int iFlag, int iIndex )
{ // 004C9370
	if( !psItemData )
		return FALSE;

	if( ITEMINFOBOX->IsViewingPerfectItem() )
		return FALSE;

	int iOriginalX = psItemData->sPosition.iX;
	int iOriginalY = psItemData->sPosition.iY;

	BOOL bShowBuyPrice = FALSE, bShowSellPrice = FALSE, bShowChecksums = FALSE;

	bShowBuyPrice = iFlag;

	static BOOL bLastShowBuyPrice = 515150;
	static BOOL bLastShowSellPrice = 5151150;

	if( iFlag == 3 )
		bShowSellPrice = TRUE;

	//if ( !ITEMINFOBOX->IsSameItem( psItemData ) /* || (bShowBuyPrice != bLastShowBuyPrice) || (bShowSellPrice != bLastShowSellPrice) */ )
	{
		bLastShowBuyPrice = bShowBuyPrice;
		bLastShowSellPrice = bShowSellPrice;

		if ( ITEMINFOBOX->PrepareShowItem( psItemData, bShowBuyPrice, bShowSellPrice, bDebug ) )
			return TRUE;
	}

	return FALSE;
}

int __stdcall TMAnimationHook( int iFrame, int iAngleX, int iAngleY, int iAngleZ, struct FrameInfoData * psFrameInfo )
{
	void * pcThis;
	READTHIS( pcThis );
	ANIMATIONHANDLER->AddAnimationObject( pcThis, iFrame, iAngleX, iAngleY, iAngleZ, psFrameInfo );
	return 1;
}

struct EXEModelData * __stdcall ReadInx( char * pszFile )
{
	return Game::ReadInx( pszFile );
}

BOOL __cdecl IsTradeUse( ItemData * pcItemData )
{
	if ( pcItemData->sItem.bLockedItemType )
		return FALSE;

	return ITEMWINDOWHANDLER->IsTradeUse( &pcItemData->sItem );
}

BOOL __stdcall MixItem( class ItemData * pcItemData, BOOL bFlag )
{
	return MIXHANDLER->SetItemMix( pcItemData, bFlag );
}

BOOL __cdecl IsUseMix( ItemData * pcItemData )
{
	if ( (pcItemData->sItem.sItemID.ToInt() & 0xFFFF0000) == 0x03070000 )
	{
		if ( (pcItemData->sItem.sItemID.ToInt() & 0xFFFFFF00) == ITEMID_SpecStoneCode )
		{
			TITLEBOX( "Use craft npc to respec with this stone!" );
			return FALSE;
		}
		return TRUE;
	}
	return ITEMWINDOWHANDLER->IsUseMixNPC( &pcItemData->sItem );
}

BOOL __stdcall SetItemToNPC( class ItemData * pcItemData, BOOL bFlag )
{
	return ITEMWINDOWHANDLER->SetItemToNPC( pcItemData, bFlag );
}

BOOL __stdcall CharacterCanEquipItem( class ItemData * pcItemData )
{
	void * p;
	READTHIS( p );
	ITEMHANDLER->CanEquipItem( p, pcItemData );
	return TRUE;
}

void __cdecl ForceOrbDraw( class UnitData * pcUnitData, Point3D * p, Point3D * pa, void * psFace )
{
//	FORCEORBHANDLER->Draw( pcUnitData, p, pa, psFace );
}

void __cdecl ForceOrbMain( class UnitData * pcUnitData )
{
	FORCEORBHANDLER->Main( pcUnitData );
}

void __stdcall DrawClanImage( void * pClanMark, char * pszClanName, int iX, int iY, BOOL bFrame )
{
}

void __stdcall OpenGuideBoxLevelUP( int iLevel, int iClass )
{
	CGuideGame * p;
	READTHIS( p );
	p->guide_handle( iLevel, (ECharacterClass)iClass );
}

BOOL __stdcall OnClickItemSlotPut( ItemData * pcItem )
{
	return MANUFACTUREHANDLER->OnClickItemSlotPut( pcItem );
}

BOOL __stdcall OnCheckCanPutStoneSlotSocket( ItemData * pcItemData, BOOL bGem )
{
	return MIXHANDLER->CanPutStoneSlotSocket( pcItemData, bGem );
}

BOOL __stdcall OnCheckCanPutItemSlotSocket( ItemData * pcItemData, BOOL bGemNPC )
{
	return MIXHANDLER->CanPutItemSlotSocket( pcItemData, bGemNPC );
}

void __cdecl OnUseManaPotion( int iMP )
{
	CHARACTERGAME->OnUseManaPotion( iMP );
}

void __cdecl OnUseHPPotion( int iHP )
{
	CHARACTERGAME->OnUseHPPotion( iHP );
}

void __cdecl OnUseSPPotion( short iSP )
{
	CHARACTERGAME->OnUseSPPotion( (int)iSP );
}

BOOL __stdcall AnimateModelGate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEFrame * pFrameInfo )
{//004769D0 (0x04AF818C)
	PTModel * p;
	READTHIS( p );

	if ( p )
		return p->Animate( iFrame, iAngleX, iAngleY, iAngleZ, pFrameInfo );

	return TRUE;
}

BOOL __stdcall AnimateModelGate2( int iFrame, int iAngleX, int iAngleY, int iAngleZ, PTModel * pcModelTalk, EXEFrame * pFrameInfo )
{//004769D0 (0x04AF818C)
	PTModel * p;
	READTHIS( p );

	if ( p )
		return p->Animate( iFrame, iAngleX, iAngleY, iAngleZ, pcModelTalk, pFrameInfo );

	return TRUE;
}

BOOL __stdcall AnimateMeshGate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEFrame * psFrameInfo )
{//0x004759A0 (0x04AF8188)
	PTMesh * p;
	READTHIS( p );

	if ( p )
		return p->Animate( iFrame, iAngleX, iAngleY, iAngleZ, psFrameInfo );
	
	return TRUE;
}

int __stdcall RenderGate( int i )
{//0x00484370 (0x04AF8190)
	EXERender * p;
	READTHIS( p );

	return p->Render( i );
}

int __stdcall RenderGate1()
{//0x004849B0 (0x04AF81E0)
	EXERender * p;
	READTHIS( p );

	return p->Render();
}

int __stdcall RenderModelGate( Point3D * psCamera, Point3D * psAngle, EXEFrame * psFrameInfo )
{//0x00477CD0 (0x04AF8198)
	PTModel * p;
	READTHIS( p );

	if ( p->pcNewModel )
		return p->pcNewModel->Render( (IO::SMD::FrameInfo *)psFrameInfo ) ? 1 : 0;

	return p->Render( psCamera, psAngle, psFrameInfo );
}

int __stdcall RenderMeshGate( Point3D * psCamera, Point3D * psAngle )
{//0x00476F90 (0x04AF819C)
	PTMesh * p;
	READTHIS( p );

	if ( p->pcNewMesh )
		return p->pcNewMesh->Render() ? 1 : 0;

	return p->PreRender( psCamera, psAngle );
}

BOOL __stdcall ClipFaceRender()
{
	EXERender * p;
	READTHIS( p );
	p->ClipFaceRender();
}

BOOL __cdecl OnRenderRightWeapon( UnitData * pcUnitData, PTModel * pcModel, EXEMatrixI * psSourceMatrix, EXEMatrixI * psDestMatrix )
{
	return ITEMHANDLER->OnRenderRightWeapon( pcUnitData, pcModel, psSourceMatrix, psDestMatrix );
}

BOOL __cdecl OnRenderSecondWeapon( UnitData * pcUnitData, PTModel * pcModel, EXEMatrixI * psSourceMatrix, EXEMatrixI * psDestMatrix, BOOL bRight )
{
	return ITEMHANDLER->OnRenderSecondWeapon( pcUnitData, pcModel, psSourceMatrix, psDestMatrix, bRight );
}

BOOL __stdcall AddFaceRender( EXERenderFace * psFace, int iValue )
{
	EXERender * p;
	READTHIS( p );
	return p->AddFaceRender( psFace, iValue );
}

BOOL __cdecl IsWarehouseUse( ItemData * psItemData )
{
	return ITEMWINDOWHANDLER->IsWarehouseUse( psItemData );
}

BOOL __cdecl IsPersonalShopUse( ItemData * psItemData )
{
	return ITEMWINDOWHANDLER->IsPersonalShopUse( psItemData );
}

BOOL __cdecl TestScreen()
{
#ifdef TEST_SCREEN
//	TESTSCREEN->Render();
	return TRUE;

#endif

	return FALSE;
}

// Title Box
BOOL __stdcall SetEventTitleBox( char * pszText )
{ //0x004D1370

	DWORD dwAddr = 0x00958950;
	do
	{
		WRITEDWORD( dwAddr, FALSE );
		dwAddr += 0x94;
	}
	while ( dwAddr < 0x0095A5A4 );

	CTITLEBOX->SetText( pszText );
	return TRUE;
}

BOOL __cdecl IgnoreMousePointer()
{
	if ( MOUSEHANDLER->IsNoMove() || MOUSEHANDLER->IsDisabled() )
		return TRUE;

	return FALSE;
}

void __cdecl FrameUnitUpdate(UnitData * pcUnitData)
{
	if ( pcUnitData )
	{
		if ( pcUnitData == UNITDATA )
		{
			if ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill) )
			{
				int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_SwiftAxe );
				if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_SwiftAxe ) )
				{
					if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex >= 0 )
					{
						ItemData * pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
						if ( pcItemData && pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Axe )
						{
							pcUnitData->iFPS += (pcUnitData->iFPS * iaFighterT33AttackSpeed[iLevelSkill - 1]) / 100;
						}
					}
				}

				iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Inpes );
				if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_Inpes ) )
					pcUnitData->iFPS += (pcUnitData->iFPS * iaASNT33AttackSpeed[iLevelSkill - 1]) / 100;

				iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_AdventMidranda );
				if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_AdventMidranda ) )
					pcUnitData->iFPS += (pcUnitData->iFPS * iaShaT43AttackSpeed[iLevelSkill - 1]) / 100;
			}
		}

		pcUnitData->iFrame += pcUnitData->iFPS;

		if ( pcUnitData->iFrame > pcUnitData->iEndFrame )
			pcUnitData->iFrame = pcUnitData->iEndFrame;

		if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Knight )
		{
			if ( pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T52 )
			{
				if ( pcUnitData->iFrame >= (pcUnitData->iStartFrame + (30 * 160)) )
				{
					if ( pcUnitData == UNITDATA )
					{
						if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_WhirlWind ) )
						{
							pcUnitData->iFrame = pcUnitData->iStartFrame + (18 * 160);
							SKILLMANAGERHANDLER->SkillPlaySound( 0x2120, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
						}
					}
				}
				else if ( (pcUnitData->iFrame >= (pcUnitData->iStartFrame + (18 * 160))) && (pcUnitData->iFrame < (pcUnitData->iStartFrame + (21 * 160))) )
					SKILLMANAGERHANDLER->SkillPlaySound( 0x2120, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
			}
		}
	}
}

BOOL __stdcall FinalRender()
{
	PTMesh * pcThis;
	READTHIS( pcThis );

	if ( READDWORD( 0x004B0D9E0 ) == TRUE )
	{
		if ( pcThis )
			EXERenderCache::FinalRenderHandler( pcThis );
	}
	else
	{
		if ( pcThis )
			CALLT( 0x00476150, (DWORD)pcThis );
	}

	return TRUE;
}

void __cdecl RenderCacheData()
{
	EXERenderCache::RenderCacheData();
}

void __cdecl DrawEffects( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	GameCore::GetInstance()->DrawEffect( iX, iY, iZ, iAX, iAY, iAZ );
}

int __stdcall RenderGlow3D( int iMaterial )
{
	EXERender * pc;
	READTHIS( pc );
	
	return pc->RenderGlow3D( iMaterial );
}

void __stdcall RenderEXE( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	Render( iX, iY, iZ, iAX, iAY, iAZ );
}

int __cdecl LoadTexture( EXETextureHandle * ps, bool bMipmap, bool bSwap )
{
	return EXETexture::LoadTexture( ps, bMipmap, bSwap );
}

BOOL __cdecl AnimateMeshTree( PTMesh * pcMesh, int iFrame, int iAX, int iAY, int iAZ )
{
	if ( pcMesh == NULL )
		return FALSE;

	PTMesh * pc = pcMesh;
	PTMesh * pcaMesh[128];

	int iCount = 0;

	while ( TRUE )
	{
		pcaMesh[iCount++] = pc;

		if ( pc->pParentMesh == NULL )
			break;

		if ( iCount == 128 )
			break;

		pc = pc->pParentMesh;
	}

	for ( int i = (iCount - 1); i >= 0; i-- )
	{
		pc = pcaMesh[i];

		if( pc )
		{
			if( pc->pcNewMesh )
				pc->pcNewMesh->Animate( iFrame, Math::Vector3Int( iAX, iAY, iAZ ) );
			else
				pc->Animate( iFrame, iAX, iAY, iAZ );
		}
	}

	return TRUE;
}

BOOL __cdecl AnimateMeshTreeNew( PTMesh * pcMesh, int iFrame, int iAX, int iAY, int iAZ )
{
	if ( pcMesh == NULL )
		return FALSE;

	PTMesh * pc = pcMesh;
	PTMesh * pcaMesh[128];

	int iCount = 0;

	while ( TRUE )
	{
		pcaMesh[iCount++] = pc;

		if ( pc->pParentMesh == NULL )
			break;

		if ( iCount == 128 )
			break;

		pc = pc->pParentMesh;
	}

	for ( int i = (iCount - 1); i >= 0; i-- )
	{
		pc = pcaMesh[i];

		if( pc )
		{
			if( pc->pcNewMesh )
				pc->pcNewMesh->Animate( iFrame, Math::Vector3Int( iAX, iAY, iAZ ) );
			else
				pc->Animate( iFrame, iAX, iAY, iAZ );
		}
	}

	return TRUE;
}

void __cdecl UpdateBlockedPosition( UnitData * pcUnitData )
{
	MAPGAME->UpdateBlockedPosition( pcUnitData );
}

BOOL __cdecl SetPVPArea( UnitData * pcUnitData )
{
	return UNITGAME->SetPVPArea( pcUnitData );
}

BOOL __cdecl OnBuySkill()
{
	return SKILLMANAGERHANDLER->OnBuySkill();
}

BOOL __cdecl OpenWorldMapGate()
{//00509B10 (0x04AF8208)
	TITLEBOX( "Temporarily disabled. Soon a new World Map guide!" );

	return FALSE;
}

CharacterClassData * __cdecl GetCharacterClassDataGate( int iClass, int iRankup )
{//0044D0F0 (0x04AF8200)
	return GetCharacterClassData( iClass, iRankup );
}

void __cdecl HandleDebuffSkill( UnitData * pcUnitData, int iID, int iValue )
{
	SKILLMANAGERHANDLER->HandleDebuffSkill( pcUnitData, iID, iValue );
}


int __cdecl GetSkillRange( Skill * psSkill, int iAttackRange, int iAttackMode )
{

	WRITEDWORD( 0x00A1598C, iAttackRange );
	WRITEDWORD( 0x00A15988, iAttackMode );

	int iRange = SKILLMANAGERHANDLER->GetSkillRange( psSkill, iAttackRange, iAttackMode );
	
	if ( iRange == 0 )
		iRange = CALL_WITH_ARG3( 0x0040EFF0, (DWORD)psSkill, iAttackRange, iAttackMode );

	return iRange;
}

BOOL __cdecl IsUnitHighRange( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	EMonsterEffectID iMonsterEffectID = pcUnitData->sCharacterData.iMonsterEffectID;

	switch ( iMonsterEffectID )
	{
		case MONSTEREFFECTID_FIGON:
		case MONSTEREFFECTID_WEB:
		case MONSTEREFFECTID_SKELETONRANGE:
		case MONSTEREFFECTID_IMP:
		case MONSTEREFFECTID_SCORPION:
		case MONSTEREFFECTID_NAZ:
		case MONSTEREFFECTID_NAZSENIOR:
		case MONSTEREFFECTID_Faugn:
		case MONSTEREFFECTID_Yagditha:
		case MONSTEREFFECTID_ITechnician:
		case MONSTEREFFECTID_LTechnician:
		case MONSTEREFFECTID_LEngineer:
		case MONSTEREFFECTID_ISoldier:
		case MONSTEREFFECTID_IBomber:
		case MONSTEREFFECTID_LGuardian:
		case MONSTEREFFECTID_IElite:
		case MONSTEREFFECTID_TULLA:
		case MONSTEREFFECTID_Draxos:
			bRet = TRUE;
			break;

		default:
			break;
	}

	return bRet;
}

void __stdcall SetInventoryItem( ItemData * psItem )
{
	CGameInventory::GetInstance()->SetInventoryItem( psItem );
}

BOOL ProcessSkillInParty( int iID, int iLevel, int iArea, int iP1, int iP2, int iP3, int iP4 )
{ //006234F0 (0x04AF8228)
	SKILLMANAGERHANDLER->ProcessSkillInParty( iID, iLevel, iArea, iP1, iP2, iP3, iP4 );
	return TRUE;
}

BOOL __stdcall GetInventoryItemGate( int iX, int iY, int iPickup )
{ //004C4D30
	return CGameInventory::GetInstance()->GetInventoryItem( iX, iY, iPickup );
}

BOOL ShowTradeRequest( int iTargetID, char * pszCharacterName )
{//00506F80 (0x04AF822C)
	if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() == FALSE )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Trade Request" );
		MESSAGEBOX->SetDescription( FormatString( "Do you want to accept %s's Trade Request?", pszCharacterName ) );
		MESSAGEBOX->SetEvent( std::bind( &HUDController::OnAcceptTradeRequest, HUDHANDLER, iTargetID ) );
		MESSAGEBOX->SetAutoClose( 10 );
		MESSAGEBOX->Show();
	}
	return TRUE;
}

BOOL __cdecl PrivateMessageSocialWindow( char * pszCharacterName )
{//0045C700 (0x04AF8234)
	CHATBOX->OnEnableChatBox();
	CHATBOX->SetInputFieldText( FormatString( "/:%s ", pszCharacterName ) );
	return TRUE;
}

BOOL __cdecl CanGetItem( PacketItem * psPacket, int iPacketHeader )
{
	return CQUESTGAME->CanGetItem( psPacket, iPacketHeader );
}

BOOL __stdcall CanCarryGold( int iGold )
{
	return CHARACTERGAME->CanCarryGold( iGold );
}

BOOL __stdcall GetGoldPersonalShopLimit( int iGold )
{
	return ITEMWINDOWHANDLER->GetGoldPersonalShopLimit( iGold );
}

int __cdecl GetCanCarryGold()
{
	return CHARACTERGAME->GetCanCarryGold();
}

BOOL __cdecl IsSellItem( ItemData * pcItemData )
{
	return ITEMWINDOWHANDLER->IsSellItem( pcItemData );
}

void __cdecl LoadDefaultModels()
{
	ITEMHANDLER->LoadDefaultModels();
}

void __cdecl CanChangeUnitY( UnitData * pcUnitData, int iY )
{
	Unit * pcUnit = UNITDATATOUNIT( pcUnitData );
	if ( pcUnit )
	{
		if ( KNOCKUPHANDLER->CanChangeUnitY( pcUnit ) )
		{
			pcUnitData->sPosition.iY = iY;
		}
		else
		{
			if ( pcUnitData->GetAnimation() != ANIMATIONTYPE_Running )
				pcUnitData->Animate( ANIMATIONTYPE_Running );
		}
	}
}

int __stdcall SaveCharacterFramesGate()
{
	UnitData * pcThis;
	READTHIS( pcThis );
	return SaveCharacterFrames( pcThis );
}

int __stdcall SaveCharacterFramesExGate()
{
	UnitData * pcThis;
	READTHIS( pcThis );
	return SaveCharacterFrames( pcThis );
}

void __cdecl RequestPersonalShop( UnitData * pcUnitData )
{
	PERSONALSHOPHANDLER->RequestPersonalShop( pcUnitData );
}

BOOL __stdcall BuyItemShop( ItemData * pcItemData, int iItemCount )
{
	return ITEMHANDLER->BuyItemShop( pcItemData, iItemCount );
}

BOOL __cdecl ValidateCharacterStatusPoint( CharacterData * psCharacterData, UINT iQuestFlag )
{
	return CGameProtect::ValidateStatusPoints( *psCharacterData, iQuestFlag );
}

BOOL __cdecl AddStatusLevelUP( int iLevelCount, UINT iQuestFlag )
{
	return CGameProtect::AddStatusLevelUP( iLevelCount, iQuestFlag );
}

void __cdecl OnSkillCooldownUpdate()
{
	CHARACTERGAME->OnCooldownSkillUpdate();
}

BOOL __cdecl CanHandleMovementFrames( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;
	if ( pcUnitData->GetAnimation() < ANIMATIONTYPE_Attack )
		bRet = TRUE;
	else if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill )
	{
		if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Knight )
		{
			if ( pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T52 )
			{
				if ( pcUnitData->iFrame >= (pcUnitData->iStartFrame + (18 * 160)) )
				{
					bRet = TRUE;

					if ( pcUnitData == UNITDATA )
					{
						if ( MOUSEHANDLER->GetEvent() == EMouseEvent::ClickDownL )
							WHIRLWINDHANDLER->OnAnimationEnd( pcUnitData );
					}
				}
			}
		}
	}

	return bRet;
}

BOOL __cdecl CanHandleStopMove( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	if ( (pcUnitData->GetAnimation() != ANIMATIONTYPE_Falling) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Idle) )
		bRet = TRUE;

	if ( bRet )
	{
		if ( pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill )
		{
			if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Knight )
			{
				if ( pcUnitData->sActiveSkill.cSkillTinyID == SKILLPLAYID_T52 )
					bRet = FALSE;
			}
		}

		if ( bRet )
			pcUnitData->Animate( ANIMATIONTYPE_Idle );
	}
	return bRet;
}

void __cdecl OpenSocialWindow()
{
	if ( SOCIALHANDLER->IsOpen() )
		SOCIALHANDLER->Close();
	else
		SOCIALHANDLER->Open();
}

BOOL __stdcall MoveUnitDataGate( int iDistance )
{
	UnitData * pcUnitData;
	READTHIS( pcUnitData );

	return pcUnitData->Move( iDistance );
}

void __cdecl ShowDeathWindow()
{
	if ( !BATTLEROYALEHANDLER->IsStarted() )
		WRITEDWORD( 0x035E10F8, TRUE );
}

BOOL __cdecl PlaceInventoryItem( Item * psItem )
{
	return CGameInventory::GetInstance()->PlaceItemInventory( psItem );
}

BOOL __cdecl DropItemToMapGate( ItemData * pcItemData )
{
	return ITEMHANDLER->DropItemToMap( pcItemData );
}

BOOL __cdecl MagneticSphereDamageGate( UnitData * pcUnitData )
{
	return MAGNETICDISCHARGEHANDLER->Damage( pcUnitData );
}

void __stdcall AssaSkillTriumphOfValhallaPartStart( UnitData * pcUnitDataEffect, int iTime )
{
	UglyRefactor::AssaSkillTriumphOfValhallaPart * pcThis;
	READTHIS( pcThis );
	pcThis->Start( pcUnitDataEffect, iTime );
}

void __stdcall AssaSkillTriumphOfValhallaPartStartHallOfValhalla( UnitData * pcUnitDataEffect, int iTime )
{
	UglyRefactor::AssaSkillTriumphOfValhallaPart * pcThis;
	READTHIS( pcThis );
	pcThis->StartHallOfValhalla( pcUnitDataEffect, iTime );
}

void __stdcall AssaSkillTriumphOfValhallaPartMain()
{
	UglyRefactor::AssaSkillTriumphOfValhallaPart * pcThis;
	READTHIS( pcThis );
	pcThis->MainFunction();
}

void __stdcall AssaSkill3SparkShieldStart( UnitData * pcUnitDataEffect, int iTime )
{
	UglyRefactor::AssaSkill3SparkShield * pcThis;
	READTHIS( pcThis );
	pcThis->Start( pcUnitDataEffect, iTime );
}

void __stdcall AssaSkill3SparkShieldMain()
{
	UglyRefactor::AssaSkill3SparkShield * pcThis;
	READTHIS( pcThis );
	pcThis->MainFunction();
}

void __stdcall AssaSkill3DivineInhalationStart( UnitData * pcUnitDataEffect, int iTime )
{
	UglyRefactor::AssaSkill3DivineInhalation * pcThis;
	READTHIS( pcThis );
	pcThis->Start( pcUnitDataEffect, iTime );
}

void __stdcall AssaSkill3DivineInhalationMain()
{
	UglyRefactor::AssaSkill3DivineInhalation * pcThis;
	READTHIS( pcThis );
	pcThis->MainFunction();
}

void __stdcall AssaSkill3HolyIncantationPartHandStart( UnitData * pcUnitDataEffect, int iTime )
{
	UglyRefactor::AssaSkill3HolyIncantationPartHand * pcThis;
	READTHIS( pcThis );
	pcThis->Start( pcUnitDataEffect, iTime );
}

void __stdcall AssaSkill3HolyIncantationPartHandMain()
{
	UglyRefactor::AssaSkill3HolyIncantationPartHand * pcThis;
	READTHIS( pcThis );
	pcThis->MainFunction();
}

void __stdcall AssaSkillTriumphOfValhallaShotStart( UnitData * pcUnitDataEffect, UnitData * pcUnitDataTarget, X3D::Vector3 * psVelocity, int iTimeEffect )
{
	UglyRefactor::AssaSkillTriumphOfValhallaShot * pcThis;
	READTHIS( pcThis );
	pcThis->Start( pcUnitDataEffect, pcUnitDataTarget, psVelocity, iTimeEffect );
}

void __stdcall AssaSkillTriumphOfValhallaShotMain()
{
	UglyRefactor::AssaSkillTriumphOfValhallaShot * pcThis;
	READTHIS( pcThis );
	pcThis->MainFunction();
}

EXEModelData * __stdcall LoadPTModelDataHookGate( const char * pszFileName )
{
	EXEModelDataHandler * pc;
	READTHIS( pc );

	return pc->LoadModelData( pszFileName );
}

EXEModelData * __stdcall LoadPTModelBoneDataHookGate( const char * pszFileName )
{
	EXEModelDataHandler * pc;
	READTHIS( pc );

	return pc->LoadBoneData( pszFileName );
}

BOOL __stdcall LoadStageOldGate( const char * pszFileName )
{
	Stage * pcStage;
	READTHIS( pcStage );

	return UglyRefactor::LoadStageOld( pcStage, pszFileName );
}

BOOL __stdcall SaveStageOldGate( const char * pszFileName )
{
	Stage * pcStage;
	READTHIS( pcStage );

	return UglyRefactor::SaveStageOld( pcStage, pszFileName );
}

BOOL __stdcall UpdateCharacterStatusItemGate( BOOL bCheckHack )
{
	return CGameCharacterStatus::GetInstance()->UpdateCharacterStatusItem( bCheckHack );
}

BOOL __cdecl SetCharacterEquipmentGate( EItemID iItemID, int iPosition, BOOL bActive )
{
	return ITEMHANDLER->SetCharacterItem( iItemID, iPosition, bActive );
}

BOOL __cdecl SaveGameGate()
{
	return CharacterGame::Save();
}

BOOL __cdecl IsBipedReadBone( EXEMesh * pcMesh )
{
	if ( (pcMesh->szName[0] == 'B') || (pcMesh->szName[1] == 'i') || (pcMesh->szName[2] == 'p') )
		return TRUE;

	if ( (pcMesh->pVertexBoneList == nullptr) || (pcMesh->pVertexBoneList[0] == nullptr) )
		return TRUE;

	return FALSE;
}

BOOL __cdecl IsBipedReadModel( EXEMesh * pcMesh )
{
	if ( (pcMesh->szName[0] == 'B') || (pcMesh->szName[1] == 'i') || (pcMesh->szName[2] == 'p') )
		return TRUE;

	if ( pcMesh->iTextureLinkCount <= 0 )
	{
		if ( (pcMesh->pVertexBoneList == nullptr) || (pcMesh->pVertexBoneList[0] == nullptr) )
			return TRUE;
	}

	return FALSE;
}

BOOL __cdecl CanUseItemSlotGate( ItemData * pcItemData, ItemInventoryData * psSlot, BOOL bNegative )
{
	if ( bNegative )
	{
		if ( pcItemData->sItem.bCostumeItem )
		{
			//Armor or Robe? So can use
			if ( (pcItemData->iItemSlotFlag == ITEMSLOTFLAG_Armor) && (psSlot->iItemSlot == ITEMSLOTFLAG_Costume) )
				return FALSE;

			//Otherwise, Can't use
			return TRUE;
		}

		//Can't use
		if ( !(psSlot->iItemSlot & pcItemData->iItemSlotFlag) )
			return TRUE;
	}
	else
	{
		//Can Use
		if ( psSlot->iItemSlot & pcItemData->iItemSlotFlag )
		{
			if ( pcItemData->sItem.bCostumeItem )
				return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL __stdcall PlaceItemCleanNPC( ItemData * pcItemData, int iFlag )
{
	return ITEMWINDOWHANDLER->PlaceItemCleanNPC( pcItemData, iFlag );
}

BOOL __cdecl MatureItemGate( EMatureType eMatureType, bool bInstantMature )
{
	return ITEMHANDLER->MatureItem( eMatureType, bInstantMature );
}

void __stdcall UpdatePhysiqueGate()
{
	EXEMesh * pcMesh;
	READTHIS( pcMesh );

	pcMesh->UpdateVertexPhysique();
}

BOOL __cdecl IsEvadedGate()
{
	if ( UNITDATA->sCharacterData.iClass == CHARACTERCLASS_Shaman )
	{
		int iSkillLevel = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_Rainmaker );
		if ( (iSkillLevel > 0) && (iSkillLevel <= 10) )
		{
			int iEvade = ((int *)0x04B06F50)[iSkillLevel - 1];
			if ( RandomI( 0, 99 ) <= iEvade )
				return TRUE;
		}
	}

	extern int EVADE_GLOBAL_ITEM;
	if ( EVADE_GLOBAL_ITEM > 0 )
	{
		if ( RandomI( 0, 99 ) <= EVADE_GLOBAL_ITEM )
			return TRUE;
	}

	return FALSE;
}

BOOL __stdcall LoadModelOldGate( const char * pszFileName, const char * pszNodeName )
{
	EXEModel * pcModel;
	READTHIS( pcModel );

	bool bNewRender = false;
	auto strFileName = std::string( pszFileName );

	//Is weapon folder and not extreme shield?
	if ( (strFileName.length() > 9) && (strFileName.substr( 0, 8 ).compare( "weapons\\" ) == 0) && (strFileName[8] != 'T') )
		bNewRender = true;

	if ( bNewRender )
		RENDERER->SetUseNewRenderToLoad( true );

	BOOL bRet = UglyRefactor::LoadModelOld( pcModel, pszFileName, pszNodeName ? pszNodeName : "" );

	if ( bNewRender )
		RENDERER->ResetUseNewRenderToLoad();

	return bRet;
}

BOOL __stdcall SaveModelOldGate( const char * pszFileName )
{
	EXEModel * pcModel;
	READTHIS( pcModel );

	return UglyRefactor::SaveModelOld( pcModel, pszFileName );
}

BOOL __cdecl OnRespawn( UnitData * pcUnitData )
{
	bool bRet = false;
	Point3D sPosition = pcUnitData->sPosition;
	bRet = BATTLEROYALEHANDLER->OnRespawn( sPosition );

	if ( bRet )
	{
		//BLESSCASTLEHANDLER->WarpField( MAP_ID, &sPosition.iX, &sPosition.iZ );

		pcUnitData->sPosition = sPosition;
		pcUnitData->Move( 1 );

		pcUnitData->Animate( ANIMATIONTYPE_Revive );

		return TRUE;
	}

	pcUnitData->Animate( ANIMATIONTYPE_Revive );

	return FALSE;
}

BOOL __stdcall UpdateSelfCharacterMovement( int iSpeed )
{
	UnitData * pcUnitData;
	READTHIS( pcUnitData );
	EXECharacter::UpdateSelfCharacterMovement( pcUnitData, iSpeed );
	return TRUE;
}

int __stdcall AnimateUnitDataGate( int iAnimationType )
{
	UnitData * pcUnitData;
	READTHIS( pcUnitData );
	return EXECharacter::AnimateCharacter( pcUnitData, iAnimationType );
}

void __cdecl OnSetModelCharacterSize( UnitData * pcUnitData )
{
	//Copy Mesh Data
	if ( auto pcUnit = UNITDATATOUNIT( pcUnitData ) )
	{
		pcUnit->vMeshesAnimation.clear();

		if ( pcUnitData->pcBoneContainer && pcUnitData->pcBoneContainer->pcNewModel )
		{
			for ( unsigned i = 0; i < pcUnitData->pcBoneContainer->pcNewModel->orderedMeshes.size(); i++ )
				pcUnit->vMeshesAnimation.push_back( Delta3D::Graphics::MeshAnimationDataOld{} );
		}

		pcUnitData->iModelHeight += MOUNTHANDLER->GetMountModelHeight( pcUnitData );
	}
}

void __cdecl DXProjectionWorkGate( EXERenderVertex * psVertex )
{
	EXEDXProjectionWork( psVertex );
}


struct EXEModelData * __stdcall ReadInxCharacters( char * pszFile )
{
	if ( auto pModel = Game::ReadInx( pszFile ) )
	{
		if ( pModel->iCount == 1 )
			pModel->iCount++;

		return pModel;
	}

	return nullptr;
}

BOOL __stdcall SetDynamicLightGate( int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iRange )
{
	return SetDynamicLight( iX, iY, iZ, iR, iG, iB, iA, iRange );
}

BOOL __stdcall SetDynamicLightTypeGate( int iType, int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iRange )
{
	return SetDynamicLight( iType, iX, iY, iZ, iR, iG, iB, iA, iRange );
}

BOOL __cdecl AddStageVertexGate1( int iX, int iY, int iZ )
{
	Math::Vector3 vFloatPosition = Math::Vector3( (float)iX / 256.0f, (float)iY / 256.0f, (float)iZ / 256.0f );
	Math::Vector3 vPosition = (GRAPHICENGINE->GetCamera()->View() * vFloatPosition) * 256.0f;

	iX = (int)vPosition.x;
	iY = (int)vPosition.y;
	iZ = (int)vPosition.z;

	RENDERER->psRenderVertex[RENDERER->iVertexCount].tx = iX;
	RENDERER->psRenderVertex[RENDERER->iVertexCount].ty = iY;
	RENDERER->psRenderVertex[RENDERER->iVertexCount].tz = iZ;
	RENDERER->psRenderVertex[RENDERER->iVertexCount].fViewWorldPos[0] = vFloatPosition.x;
	RENDERER->psRenderVertex[RENDERER->iVertexCount].fViewWorldPos[1] = vFloatPosition.y;
	RENDERER->psRenderVertex[RENDERER->iVertexCount].fViewWorldPos[2] = vFloatPosition.z;

	return TRUE;
}

BOOL __cdecl CameraCharacterScreenPositionGate( int iX, int iY, int iZ )
{
	GRAPHICS->GetRenderer()->SetAmbientColor( Math::Color( RENDERER->r, RENDERER->g, RENDERER->b ) );
	GRAPHICENGINE->GetCamera()->SetPosition( Math::Vector3( iX / 256.0f, (iY / 256.0f) - 7.f, iZ / 256.0f ), Math::Vector3( 0.f, (iY / 256.0f) - 24.f, 0.f ) );

	return TRUE;
}

BOOL __stdcall SetCameraPositionGate1( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	return RENDERER->SetCameraPosition( iX, iY, iZ, iAX, iAY, iAZ );
}
BOOL __stdcall DrawTexture2DGate( EXETextureHandle * psTextureHandle, void * pVertex, BOOL bAlphaBlending )
{
	struct PTD3DTLVERTEX8
	{
		float sx, sy, sz;
		float rhw;
		D3DCOLOR color;
		D3DCOLOR specular;
		float tu1, tv1;
	};

	PTD3DTLVERTEX8 * psVertex = (PTD3DTLVERTEX8 *)pVertex;

	Rectangle2DF sRect = Rectangle2DF( 0.0f, 0.0f, 0.0f, 0.0f );

	if ( psTextureHandle )
	{
		sRect.fX = psVertex[0].sx;
		sRect.fY = psVertex[0].sy;
		sRect.fWidth = (psVertex[3].sx - sRect.fX);
		sRect.fHeight = (psVertex[3].sy - sRect.fY);

		float fRatioW = (psVertex[3].sx - sRect.fX) / (float)psTextureHandle->iWidth;
		float fRatioH = (psVertex[3].sy - sRect.fY) / (float)psTextureHandle->iHeight;

		D3DXVECTOR2 scale( 1.0f, 1.0f );
		D3DXVECTOR2 trans( (float)sRect.fX, (float)sRect.fY );
		D3DXVECTOR2 center( sRect.fWidth / 2.0f, sRect.fHeight / 2.0f );

		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D( &matrix, NULL, 0.0, &scale, &center, 0.0f, &trans );

		//Draw Sprite
		GetGraphicsDevice()->GetDefaultSprite()->Get()->Begin( D3DXSPRITE_ALPHABLEND );
		GRAPHICENGINE->GetDevice()->SetTexture( 0, psTextureHandle->pcD3DTexture );
		GetGraphicsDevice()->GetDefaultSprite()->Get()->SetTransform( &matrix );
		GetGraphicsDevice()->GetDefaultSprite()->Get()->Draw( psTextureHandle->pcD3DTexture, NULL, NULL, NULL, psVertex[0].color );
		GetGraphicsDevice()->GetDefaultSprite()->Get()->End();
		GRAPHICENGINE->GetDevice()->SetTexture( 0, 0 );
	}
	else
	{
		sRect.fX = psVertex[0].sx;
		sRect.fY = psVertex[0].sy;
		sRect.fWidth = psVertex[3].sx - sRect.fX;
		sRect.fHeight = psVertex[3].sy - sRect.fY;

		float fRatioW = (psVertex[3].sx - sRect.fX) / sRect.fWidth;
		float fRatioH = (psVertex[3].sy - sRect.fY) / sRect.fHeight;

		D3DXVECTOR2 scale( fRatioW, fRatioH );
		D3DXVECTOR2 trans( (float)sRect.fX, (float)sRect.fY );
		D3DXVECTOR2 center( sRect.fWidth / 2.0f, sRect.fHeight / 2.0f );

		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D( &matrix, NULL, 0.0, &scale, &center, 0.0f, &trans );

		//Draw Sprite
		GetGraphicsDevice()->GetDefaultSprite()->Get()->Begin( D3DXSPRITE_ALPHABLEND );
		GRAPHICENGINE->GetDevice()->SetTexture( 0, GetGraphicsDevice()->GetDefaultTexture()->Get() );
		GetGraphicsDevice()->GetDefaultSprite()->Get()->SetTransform( &matrix );
		GetGraphicsDevice()->GetDefaultSprite()->Get()->Draw( GetGraphicsDevice()->GetDefaultTexture()->Get(), NULL, NULL, NULL, psVertex[0].color );
		GetGraphicsDevice()->GetDefaultSprite()->Get()->End();
		GRAPHICENGINE->GetDevice()->SetTexture( 0, 0 );
	}


	return TRUE;
}

BOOL __stdcall SetEffectItemsGate( PacketItemEffect * psPacket )
{
	UnitData * pcUnitData;
	READTHIS( pcUnitData );

	auto SetItemColor = [pcUnitData]( UnitTool & sTool, PacketItemEffect * psEffect ) -> void
	{
		if ( sTool.pModel && (sTool.eItemID == psEffect->iItemID) )
		{
			sTool.iBlinkSpeed = psEffect->iColor;
			CopyMemory( sTool.saBlinkColor, psEffect->saColors, sizeof( short ) * _countof( psEffect->saColors ) );
			sTool.bStarEffect = psEffect->uShowEffect;
			sTool.iBlinkScale = psEffect->uScale;
			sTool.pcBlendingMaterial = nullptr;

			//Blending Material
			if ( (sTool.iAgeLevel > 0) && (psEffect->iColor == 9) )
				sTool.pcBlendingMaterial = GRAPHICENGINE->GetAgingMaterial( sTool.iAgeLevel );
			else if( (sTool.iMixID > 0) && (psEffect->iColor == 8) )
				sTool.pcBlendingMaterial = GRAPHICENGINE->GetMixingMaterial( sTool.iMixID - 1 );

			CALL_WITH_ARG1( 0x00458F20, (DWORD)&sTool );
			
			if ( sTool.bStarEffect )
				CALL_WITH_ARG1( 0x005BEA40, (DWORD)pcUnitData );
		}
	};

	SetItemColor( pcUnitData->sRightHandTool, psPacket );
	SetItemColor( pcUnitData->sLeftHandTool, psPacket );
}

void __cdecl RenderExpansionSkillGate(UnitData * pcUnitData)
{
	return UglyRefactor::ExpansionSkillRender( pcUnitData );
}

BOOL __cdecl RenderUnitDataGate( UnitData * pcUnitData )
{
	return RenderUnitData( pcUnitData, true, true, true );
}

void __cdecl ImportASEDataGate( EXEMesh * pcMesh, const char * pszLineBuffer )
{
	UglyRefactor::ImportASEData( pcMesh, pszLineBuffer );
}

void __stdcall SortVerticesMeshGate()
{
	EXEMesh * pMesh;
	READTHIS( pMesh );
	pMesh->SortVertices();
}

void __cdecl MergeBoneGate( EXEMesh * pcBoneTo, EXEMesh * pcBoneFrom )
{
    pcBoneTo->sTransformationMatrixF = pcBoneFrom->sTransformationMatrixF;

	if ( pcBoneFrom->bUseInfluences )
	{
        if ( pcBoneFrom->vInfluences.size() > 0 )
        {
            pcBoneTo->vInfluences.resize( pcBoneFrom->vInfluences.size() );

            CopyMemory( pcBoneTo->vInfluences.data(), pcBoneFrom->vInfluences.data(), pcBoneFrom->vInfluences.size() * sizeof( EXEMeshInfluenceVertex ) );
        }
        if ( pcBoneFrom->vFloatVertices.size() > 0 )
        {
            pcBoneTo->vFloatVertices.resize( pcBoneFrom->vFloatVertices.size() );

            CopyMemory( pcBoneTo->vFloatVertices.data(), pcBoneFrom->vFloatVertices.data(), pcBoneFrom->vFloatVertices.size() * sizeof( EXEVertexF ) );
        }
	}
}

void __stdcall ComputeNormalsMeshGate()
{
	EXEMesh * pMesh;
	READTHIS( pMesh );

	pMesh->ComputeNormals();
}

void __stdcall ComputeTransformationGate()
{
    EXEMesh * pMesh;
    READTHIS( pMesh );

	pMesh->ComputeTransformation();
}

EXERenderFace * __stdcall AddStageFaceGate( Stage * pcStage, StageFace * psStageFace )
{
	if ( pcStage == nullptr )
	{
		if ( (STAGE1 != NULL) && (UNITDATA->iLoadedMapIndex == 0) )
			pcStage = STAGE1;
		if ( (STAGE2 != NULL) && (UNITDATA->iLoadedMapIndex == 1) )
			pcStage = STAGE2;
	}

	return EXERender::AddStageFace( pcStage, psStageFace );
}


Stage * __stdcall NewStageGate1()
{
	Stage * pcStageGame;
	READTHIS( pcStageGame );

    StageDataOld * p = (StageDataOld *)pcStageGame;
    p->Constructor1();
    p->Deconstructor();

    delete p;

    return new Stage();
}

Stage * __stdcall NewStageGate2( int iNumVertices, int iNumFaces )
{
    Stage * pcStageGame;
    READTHIS( pcStageGame );

    StageDataOld * p = (StageDataOld *)pcStageGame;
    p->Constructor1();
    p->Deconstructor();

    delete p;

	return new Stage( iNumVertices, iNumFaces );
}

void __cdecl ImportASEDataStageMeshGate( Stage * pcStage, const char * pszLineBuffer )
{
    UglyRefactor::ImportASEDataStageMesh( pcStage, pszLineBuffer );
}

void __stdcall HandlePacketUnitInfoGate( PacketUnitInfo * psPacket )
{
	UnitData * pcUnitData;
	READTHIS( pcUnitData );

	Game::HandlePacket( pcUnitData, psPacket );
}

void __stdcall HandlePacketUnitInfoQuickGate( PacketUnitInfoQuick * psPacket )
{
	UnitData * pcUnitData;
	READTHIS( pcUnitData );

	Game::HandlePacket( pcUnitData, psPacket );
}

void __cdecl InitUnitDataGate( UnitData * pcUnitData )
{
}

void __cdecl MainUnitDataGate( UnitData * pcUnitData )
{
	if ( auto pcUnit = UNITDATATOUNIT( pcUnitData ) )
	{
		pcUnit->fAnimationBlendingTime += ((1000.0f / 60.0f) / 1000.0f);

		if ( pcUnit->fAnimationBlendingTime < 0.0f )
			pcUnit->fAnimationBlendingTime = 0.0f;

		if ( pcUnit->fAnimationBlendingTime > 1.0f )
			pcUnit->fAnimationBlendingTime = 1.0f;
	}
}

int __stdcall CanMoveToPositionGate( Point3D * psPosition, Point3D * psAngle, Point3D * psMovePosition, int iDistance, int iWidth, int iHeight, BOOL bCheckOverlap )
{
	Stage * pcStageGame;
	READTHIS( pcStageGame );

	return EXECharacter::CanMoveToPosition( pcStageGame, psPosition, psAngle, psMovePosition, iDistance, iWidth, iHeight, bCheckOverlap );
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
	{
		WIN32_FIND_DATA sFindData;
		char szFileName[256] = {0};
		HANDLE hFile = NULL;
		STRINGCOPY( szFileName, AY_OBFUSCATE( "*.dll" ) );

		char szPath[512] = {0};

		if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
		{
			do
			{
				if ( (std::string( sFindData.cFileName ).compare( AY_OBFUSCATE( "game.dll" ) ) != 0) && (std::string( sFindData.cFileName ).compare( AY_OBFUSCATE( "sciter.dll" ) ) != 0) )
				{
					GetCurrentDirectoryA( 512, szPath );
					STRINGCONCAT( szPath, AY_OBFUSCATE( "\\" ) );
					STRINGCONCAT( szPath, sFindData.cFileName );
					DeleteFileA( szPath );
				}
			}
			while ( FindNextFileA( hFile, &sFindData ) );

			FindClose( hFile );
		}

		// Delete Log
		DeleteFileA( "save\\logs\\init.log" );

		(*(DWORD *)0x04AF8030) = (DWORD)&DefendChance;
		(*(DWORD *)0x04AF8034) = (DWORD)&BlockChance;
		(*(DWORD *)0x04AF8038) = (DWORD)&OnSendPlayDataEx;
		(*(DWORD *)0x04AF803C) = (DWORD)&ShowExpValueFormat;
		(*(DWORD *)0x04AF8040) = (DWORD)&OnReceivePacket;
		(*(DWORD *)0x04AF8044) = (DWORD)&DX::Graphic::BeginScene;
		(*(DWORD *)0x04AF8048) = (DWORD)&DX::Graphic::EndScene;
		(*(DWORD *)0x04AF804C) = (DWORD)&OnDrawUI;
		(*(DWORD *)0x04AF8050) = (DWORD)&GRAPHICENGINE->TakenScreenShot;
		(*(DWORD *)0x04AF8054) = (DWORD)&LoadMaps;
		(*(DWORD *)0x04AF8058) = (DWORD)&OnClickPremium;
		(*(DWORD *)0x04AF805C) = (DWORD)&DrawTextDXHook;
		(*(DWORD *)0x04AF8064) = (DWORD)&OnLoadArmorCharacter;
		(*(DWORD *)0x04AF8068) = (DWORD)&OnSetArmorCurrentUnit;
		(*(DWORD *)0x04AF806C) = (DWORD)&OnSetArmorTargetUnit;
		(*(DWORD *)0x04AF8070) = (DWORD)&OnLoginDraw;
		(*(DWORD *)0x04AF8074) = (DWORD)&OnLoseExp;
		(*(DWORD *)0x04AF8078) = (DWORD)&GetAbsorptionPlayer;
		(*(DWORD *)0x04AF807C) = (DWORD)&DrawGMDebugCharacterInfo;
		(*(DWORD *)0x04AF8080) = (DWORD)&OnReceiveCommand;
		(*(DWORD *)0x04AF8084) = (DWORD)&LoadMusic;
		(*(DWORD *)0x04AF8088) = (DWORD)&OnItemPivot;
		(*(DWORD *)0x04AF808C) = (DWORD)&ReOrderVolume;
		(*(DWORD *)0x04AF8090) = (DWORD)&LoadTexture;
		(*(DWORD *)0x04AF8094) = (DWORD)&OnStopHitInfo;
		(*(DWORD *)0x04AF80A4) = (DWORD)&GetWidthNormalFont;
		(*(DWORD *)0x04AF80A8) = (DWORD)&SetCharacterEquipmentGate;
		(*(DWORD *)0x04AF80AC) = (DWORD)&Begin3D;
		(*(DWORD *)0x04AF80B0) = (DWORD)&End3D;
		(*(DWORD *)0x04AF80B4) = (DWORD)&OnDescriptionItem;
		(*(DWORD *)0x04AF80B8) = (DWORD)&GRAPHICENGINE->ShowLoading;
		(*(DWORD *)0x04AF80BC) = (DWORD)&GRAPHICENGINE->HideLoading;
		(*(DWORD *)0x04AF80C0) = (DWORD)&GRAPHICENGINE->UpdateLoading;
		(*(DWORD *)0x04AF80C4) = (DWORD)&OnCharacterUpdateData;
		(*(DWORD *)0x04AF80C8) = (DWORD)&GameCore::SendLoginData;
		(*(DWORD *)0x04AF80CC) = (DWORD)&IsDebuffSkill;
		(*(DWORD *)0x04AF80D0) = (DWORD)&FormatSkillTimerInfo;
		(*(DWORD *)0x04AF80D4) = (DWORD)&GRAPHICENGINE->DrawLoadingImage;
		(*(DWORD *)0x04AF80D8) = (DWORD)&CharacterGame::ChecksumCheck;
		(*(DWORD *)0x04AF80DC) = (DWORD)&ANTICHEATHANDLER->CheckThread;
		(*(DWORD *)0x04AF80E0) = (DWORD)&GetEditTextW;
		(*(DWORD *)0x04AF80E4) = (DWORD)&GameCore::GameRun;
		(*(DWORD *)0x04AF80E8) = (DWORD)&RenderSubServerBox;
		(*(DWORD *)0x04AF80EC) = (DWORD)&SummonCrystal;
		(*(DWORD *)0x04AF80F0) = (DWORD)&OnNpcQuestClick;
		(*(DWORD *)0x04AF80F4) = (DWORD)&OnClickOKQuestMessageBox;
		(*(DWORD *)0x04AF80F8) = (DWORD)&OnClickQuestWindow;
		(*(DWORD *)0x04AF80FC) = (DWORD)&OnDescriptionItemMixType;
		(*(DWORD *)0x04AF8100) = (DWORD)&OnAddColorDescription;
		(*(DWORD *)0x04AF8104) = (DWORD)&OnSendPacket;
		(*(DWORD *)0x04AF8108) = (DWORD)&OnConnect;
		(*(DWORD *)0x04AF810C) = (DWORD)&OnSocketPacket;
		(*(DWORD *)0x04AF8110) = (DWORD)&OnSocketClose;
		(*(DWORD *)0x04AF8114) = (DWORD)&OnCharacterSetCharInfoEx;
		(*(DWORD *)0x04AF8118) = (DWORD)&OnWarpCastle;
		(*(DWORD *)0x04AF811C) = (DWORD)&ChangeMotion;
		(*(DWORD *)0x04AF8120) = (DWORD)&GameCore::OnHandlerFunctions;
		(*(DWORD *)0x04AF8124) = (DWORD)&Game::InternetOpenUrlClan;
		(*(DWORD *)0x04AF8128) = (DWORD)&ExitGame;
		(*(DWORD *)0x04AF812C) = (DWORD)&BeginSkill; // BeginSkill
		(*(DWORD *)0x04AF8130) = (DWORD)&OpenPlaySkill; // OpenPlaySkill
		(*(DWORD *)0x04AF8134) = (DWORD)&PlaySkillAttack; // PlaySkillAttack
		(*(DWORD *)0x04AF8138) = (DWORD)&EventSkill; // EventSkill
		(*(DWORD *)0x04AF813C) = (DWORD)&EndSkill; // EndSkill
		(*(DWORD *)0x04AF8140) = (DWORD)&PrepareSkillInfo;
		(*(DWORD *)0x04AF8144) = (DWORD)&PrepareItemInfo;
		(*(DWORD *)0x04AF8148) = (DWORD)&TMAnimationHook;
		(*(DWORD *)0x04AF814C) = (DWORD)&ReadInx;
		(*(DWORD *)0x04AF8150) = (DWORD)&IsTradeUse;
		(*(DWORD *)0x04AF8154) = (DWORD)&UI::ItemInfoBox::IsExpiredItemTime;
		(*(DWORD *)0x04AF8158) = (DWORD)&CharacterCanEquipItem;
		(*(DWORD *)0x04AF815C) = (DWORD)&ForceOrbDraw;
		(*(DWORD *)0x04AF8160) = (DWORD)&ForceOrbMain;
		(*(DWORD *)0x04AF8164) = (DWORD)&DrawClanImage;
		(*(DWORD *)0x04AF8168) = (DWORD)&MixItem;
		(*(DWORD *)0x04AF816C) = (DWORD)&IsUseMix;
		(*(DWORD *)0x04AF8170) = (DWORD)&SetItemToNPC;
		(*(DWORD *)0x04AF8174) = (DWORD)&OnLostDevice;
		(*(DWORD *)0x04AF8178) = (DWORD)&ChangeMusic;
		(*(DWORD *)0x04AF817C) = (DWORD)&OnClickItemSlotPut;
		(*(DWORD *)0x04AF8180) = (DWORD)&OnCheckCanPutStoneSlotSocket;
		(*(DWORD *)0x04AF8184) = (DWORD)&OnCheckCanPutItemSlotSocket;
		(*(DWORD *)0x04AF8188) = (DWORD)&OnUseManaPotion;
		(*(DWORD *)0x04AF818C) = (DWORD)&AnimateModelGate;
		(*(DWORD *)0x04AF8190) = (DWORD)&RenderGate;
		(*(DWORD *)0x04AF8194) = (DWORD)&ClipFaceRender;
		(*(DWORD *)0x04AF8198) = (DWORD)&RenderModelGate;
		(*(DWORD *)0x04AF819C) = (DWORD)&RenderMeshGate;
		(*(DWORD *)0x04AF81A0) = (DWORD)&AnimateMeshGate;
		(*(DWORD *)0x04AF81A4) = (DWORD)&OnRenderRightWeapon;
		(*(DWORD *)0x04AF81A8) = (DWORD)&OnRenderSecondWeapon;
		(*(DWORD *)0x04AF81AC) = (DWORD)&AddFaceRender;
		(*(DWORD *)0x04AF81B0) = (DWORD)&RenderBGLogin;
		(*(DWORD *)0x04AF81B4) = (DWORD)&IsWarehouseUse;
		(*(DWORD *)0x04AF81B8) = (DWORD)&IsPersonalShopUse;
		(*(DWORD *)0x04AF81BC) = (DWORD)&AnimateMeshTree;
		(*(DWORD *)0x04AF81C0) = (DWORD)&SetEventTitleBox;
		(*(DWORD *)0x04AF81C4) = (DWORD)&IgnoreMousePointer;
		(*(DWORD *)0x04AF81C8) = (DWORD)&FrameUnitUpdate;
		(*(DWORD *)0x04AF81CC) = (DWORD)&FinalRender;
		(*(DWORD *)0x04AF81D0) = (DWORD)&RenderCacheData;
		(*(DWORD *)0x04AF81D4) = (DWORD)&DrawEffects;
		(*(DWORD *)0x04AF81D8) = (DWORD)&OnDrawUILogin;
		(*(DWORD *)0x04AF81DC) = (DWORD)&RenderGlow3D;
		(*(DWORD *)0x04AF81E0) = (DWORD)&RenderGate1;
		(*(DWORD *)0x04AF81E4) = (DWORD)&RenderEXE;
		(*(DWORD *)0x04AF81E8) = (DWORD)&RenderStageMesh;
		(*(DWORD *)0x04AF81EC) = (DWORD)&AnimateModelGate2;
		(*(DWORD *)0x04AF81F0) = (DWORD)&UpdateBlockedPosition;
		(*(DWORD *)0x04AF81F4) = (DWORD)&SendAreaDamageUnit;
		(*(DWORD *)0x04AF81F8) = (DWORD)&SetPVPArea;
		(*(DWORD *)0x04AF81FC) = (DWORD)&OnBuySkill;
		(*(DWORD *)0x04AF8200) = (DWORD)&GetCharacterClassDataGate;
		(*(DWORD *)0x04AF8204) = (DWORD)&HandleDebuffSkill;
		(*(DWORD *)0x04AF8208) = (DWORD)&OpenWorldMapGate;
		(*(DWORD *)0x04AF820C) = (DWORD)&GetSkillRange;
		(*(DWORD *)0x04AF8210) = (DWORD)&OnUseHPPotion;
		(*(DWORD *)0x04AF8214) = (DWORD)&SendSingleDamageUnit;
		(*(DWORD *)0x04AF8218) = (DWORD)&RenderUnitDataGate;
		(*(DWORD *)0x04AF821C) = (DWORD)&IsUnitHighRange;
		(*(DWORD *)0x04AF8220) = (DWORD)&FreeUnitData;
		(*(DWORD *)0x04AF8224) = (DWORD)&SetInventoryItem;
		(*(DWORD *)0x04AF8228) = (DWORD)&ProcessSkillInParty;
		(*(DWORD *)0x04AF822C) = (DWORD)&ShowTradeRequest;
		(*(DWORD *)0x04AF8230) = (DWORD)&OpenGuideBoxLevelUP;
		(*(DWORD *)0x04AF8234) = (DWORD)&PrivateMessageSocialWindow;
		(*(DWORD *)0x04AF8238) = (DWORD)&CanGetItem;
		(*(DWORD *)0x04AF823C) = (DWORD)&CanCarryGold;
		(*(DWORD *)0x04AF8240) = (DWORD)&GetGoldPersonalShopLimit;
		(*(DWORD *)0x04AF8244) = (DWORD)&GetCanCarryGold;
		(*(DWORD *)0x04AF8248) = (DWORD)&IsSellItem;
		(*(DWORD *)0x04AF824C) = (DWORD)&LoadDefaultModels;
		(*(DWORD *)0x04AF8250) = (DWORD)&CanChangeUnitY;
		(*(DWORD *)0x04AF8254) = (DWORD)&SaveCharacterFramesGate;
		(*(DWORD *)0x04AF8258) = (DWORD)&SaveCharacterFramesExGate;
		(*(DWORD *)0x04AF825C) = (DWORD)&RequestPersonalShop;
		(*(DWORD *)0x04AF8260) = (DWORD)&BuyItemShop;
		(*(DWORD *)0x04AF8264) = (DWORD)&ValidateCharacterStatusPoint;
		(*(DWORD *)0x04AF8268) = (DWORD)&AddStatusLevelUP;
		(*(DWORD *)0x04AF826C) = (DWORD)&OnSkillCooldownUpdate;
		(*(DWORD *)0x04AF8270) = (DWORD)&CanHandleMovementFrames;
		(*(DWORD *)0x04AF8274) = (DWORD)&CanHandleStopMove;
		(*(DWORD *)0x04AF8278) = (DWORD)&OpenSocialWindow;
		(*(DWORD *)0x04AF827C) = (DWORD)&MoveUnitDataGate;
		(*(DWORD *)0x04AF8280) = (DWORD)&ShowDeathWindow;
		(*(DWORD *)0x04AF8284) = (DWORD)&PlaceInventoryItem;
		(*(DWORD *)0x04AF8288) = (DWORD)&DropItemToMapGate;
		(*(DWORD *)0x04AF828C) = (DWORD)&MagneticSphereDamageGate;

		//Render Ugly Hook
		(*(DWORD *)0x04AF8290) = (DWORD)&UglyRefactor::SearchObjLocalPos;
		(*(DWORD *)0x04AF8294) = (DWORD)&UglyRefactor::SearchObjLocalPosEx;
		(*(DWORD *)0x04AF8298) = (DWORD)&AssaSkillTriumphOfValhallaPartStart;
		(*(DWORD *)0x04AF829C) = (DWORD)&AssaSkillTriumphOfValhallaPartStartHallOfValhalla;
		(*(DWORD *)0x04AF82A0) = (DWORD)&AssaSkillTriumphOfValhallaPartMain;
		(*(DWORD *)0x04AF82A4) = (DWORD)&AssaSkill3SparkShieldStart;
		(*(DWORD *)0x04AF82A8) = (DWORD)&AssaSkill3SparkShieldMain;
		(*(DWORD *)0x04AF82AC) = (DWORD)&AssaSkill3DivineInhalationStart;
		(*(DWORD *)0x04AF82B0) = (DWORD)&AssaSkill3DivineInhalationMain;
		(*(DWORD *)0x04AF82B4) = (DWORD)&AssaSkill3HolyIncantationPartHandStart;
		(*(DWORD *)0x04AF82B8) = (DWORD)&AssaSkill3HolyIncantationPartHandMain;
		(*(DWORD *)0x04AF82BC) = (DWORD)&AssaSkillTriumphOfValhallaShotStart;
		(*(DWORD *)0x04AF82C0) = (DWORD)&AssaSkillTriumphOfValhallaShotMain;
		(*(DWORD *)0x04AF82C4) = (DWORD)&UglyRefactor::AssaParticle_DivineInhalationStart;
		(*(DWORD *)0x04AF82C8) = (DWORD)&UglyRefactor::SkillSaintGodlyShield;
		(*(DWORD *)0x04AF82CC) = (DWORD)&UglyRefactor::AssaParticleClanMonsterHit1;


		(*(DWORD *)0x04AF82D0) = (DWORD)&LoadPTModelDataHookGate;
		(*(DWORD *)0x04AF82D4) = (DWORD)&LoadPTModelBoneDataHookGate;
		(*(DWORD *)0x04AF82D8) = (DWORD)&LoadStageOldGate;
		(*(DWORD *)0x04AF82DC) = (DWORD)&SaveStageOldGate;
	}

	(*(DWORD *)0x04AF82E0) = (DWORD)&UpdateCharacterStatusItemGate;
	(*(DWORD *)0x04AF82E4) = (DWORD)&SaveGameGate;
	(*(DWORD *)0x04AF82E8) = (DWORD)&IsBipedReadBone;
	(*(DWORD *)0x04AF82EC) = (DWORD)&IsBipedReadModel;
	(*(DWORD *)0x04AF82F0) = (DWORD)&CanUseItemSlotGate;
	(*(DWORD *)0x04AF82F4) = (DWORD)&PlaceItemCleanNPC;
	(*(DWORD *)0x04AF82F8) = (DWORD)&MatureItemGate;
	(*(DWORD *)0x04AF82FC) = (DWORD)&UpdatePhysiqueGate;
	(*(DWORD *)0x04AF8300) = (DWORD)&IsEvadedGate;
	(*(DWORD *)0x04AF8304) = (DWORD)&LoadModelOldGate;
	(*(DWORD *)0x04AF8308) = (DWORD)&SaveModelOldGate;
	(*(DWORD *)0x04AF830C) = (DWORD)&GetInventoryItemGate;
	(*(DWORD *)0x04AF8310) = (DWORD)&OnRespawn;
	(*(DWORD *)0x04AF8314) = (DWORD)&UpdateSelfCharacterMovement;
	(*(DWORD *)0x04AF8318) = (DWORD)&AnimateUnitDataGate;
	(*(DWORD *)0x04AF831C) = (DWORD)&OnSetModelCharacterSize;
	(*(DWORD *)0x04AF8320) = (DWORD)&DXProjectionWorkGate;
	(*(DWORD *)0x04AF8324) = (DWORD)&ReadInxCharacters;
	(*(DWORD *)0x04AF8328) = (DWORD)&SetDynamicLightGate;
	(*(DWORD *)0x04AF832C) = (DWORD)&SetDynamicLightTypeGate;
	(*(DWORD *)0x04AF8330) = (DWORD)&AddStageVertexGate1;
	(*(DWORD *)0x04AF8334) = (DWORD)&CameraCharacterScreenPositionGate;
	(*(DWORD *)0x04AF8338) = (DWORD)&SetCameraPositionGate1;
	(*(DWORD *)0x04AF833C) = (DWORD)&DrawTexture2DGate;
    (*(DWORD *)0x04AF8340) = (DWORD)&SetEffectItemsGate;
    (*(DWORD *)0x04AF8344) = (DWORD)&RenderExpansionSkillGate;
    (*(DWORD *)0x04AF8348) = (DWORD)&EXEMesh::NewMeshGate;
    (*(DWORD *)0x04AF834C) = (DWORD)&EXEMesh::NewMeshGateVertexFaces;
    (*(DWORD *)0x04AF8350) = (DWORD)&ImportASEDataGate;
    (*(DWORD *)0x04AF8354) = (DWORD)&SortVerticesMeshGate;
    (*(DWORD *)0x04AF8358) = (DWORD)&MergeBoneGate;
    (*(DWORD *)0x04AF835C) = (DWORD)&ComputeNormalsMeshGate;
    (*(DWORD *)0x04AF8360) = (DWORD)&ComputeTransformationGate;
    (*(DWORD *)0x04AF8364) = (DWORD)&EXEModel::NewModelGate;
    (*(DWORD *)0x04AF8368) = (DWORD)&EXEModel::CloseNewModelGate;
    (*(DWORD *)0x04AF836C) = (DWORD)&AddStageFaceGate;
    (*(DWORD *)0x04AF8370) = (DWORD)&NewStageGate1;
    (*(DWORD *)0x04AF8374) = (DWORD)&NewStageGate2;
    (*(DWORD *)0x04AF8378) = (DWORD)&ImportASEDataStageMeshGate;
	(*(DWORD *)0x04AF837C) = (DWORD)&DeleteEXEModelData;
	(*(DWORD *)0x04AF8380) = (DWORD)&HandlePacketUnitInfoGate;
	(*(DWORD *)0x04AF8384) = (DWORD)&HandlePacketUnitInfoQuickGate;
	(*(DWORD *)0x04AF8388) = (DWORD)&InitUnitDataGate;
	(*(DWORD *)0x04AF838C) = (DWORD)&MainUnitDataGate;
	(*(DWORD *)0x04AF8390) = (DWORD)&CanMoveToPositionGate;
	(*(DWORD *)0x04AF8394) = (DWORD)&OnUseSPPotion;

	return TRUE;
}

