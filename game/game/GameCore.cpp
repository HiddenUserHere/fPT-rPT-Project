#include "stdafx.h"
#include "GameCore.h"
#include "LevelTable.h"
GameCore										* pGameCoreHandler = NULL;

tfnDrawBMP fnDrawBMP = (tfnDrawBMP)0x00505B20;
tfnGetPlayTime fnGetPlayTime = (tfnGetPlayTime)0x0045F770;

int & iRadian3D_X								 = *(int*)(0x035BF3F8);
int & iRadian3D_Y								 = *(int*)(0x035BF3FC);
int & iRadian3D_Z								 = *(int*)(0x035BF400);

extern BOOL bDebug;

DropItemView saItemsStage[1024];

UI::ItemBox_ptr pItemBox = std::make_shared<UI::ItemBox>( 0, 0 );

GameCore::GameCore()
{
	pcGraphicEngineHandler				= new DXGraphicEngine();
	pcSystemHandler						= new CSystem();
	pcSkillManagerHandler				= new SkillManager();
	SocketGame::CreateInstance();
	pcChatGameHandler					= new ChatGame();
	pcItemHandler						= new ItemHandler();
	pcSettingsHandler					= new CSettingsHandler();
	pcItemTimerHandler					= new CItemTimerHandler();
	pcLoginControllerHandler			= new CLoginControllerHandler();
	pcCharacterGameHandler				= new CharacterGame();
	pcAntiCheatHandler					= new CAntiCheat();
	pcKeyboardHandler					= new CKeyboard();
	pcMouseHandler						= new CMouse();
	pcSoundHandler						= new CSound();
	pcPacketHandler						= new RecvPacket();
	pcDamageInfoHandler					= new CDamageInfoController();
	pcAgingWindowHandler				= new CAgingWindow();
	pcWarehouseHandler					= new CWarehouse();
	pcItemWindowHandler					= new CItemWindowHandler();
	pcQuestGameHandler					= new QuestGame();
	pcTimerSkillHandler					= new CTimerSkill();
	pcBellatraGameHandler				= new BellatraGame();
	pcMixHandler						= new MixHandler();
	pcDailyQuestHandler					= new DailyQuest();
	pcMessageBoxHandler					= new CMessageBox();
	pcBlessCastleHandler				= new CBlessCastleHandler();
	pcParticleOldHandler				= new CParticleOld();
	pcShamanHandler						= new CShamanHandler();
	pcHUDHandler						= new HUDController();
	pcHalloweenHandler					= new CHalloweenHandler();
	pcManufactureHandler				= new CManufactureHandler();
	pcPerfectItemViewHandler			= new CPerfectItemViewHandler();
	pcCoinHandler						= new CCoinHandler();
	pcCompareItemHandler				= new CCompareItemHandler();
	pcItemInfoBox						= std::make_shared<UI::ItemInfoBox>();
	pcSkillInfoBox						= std::make_shared<UI::SkillInfoBox>();
	pcAccountHandler					= new CAccountHandler();
	pcAnimationHandler					= new CAnimationHandler();
	pcMapGame							= new MapGame();
	pcXMasHandler						= new CChristmasHandler();
	pcSkillEffectHandler				= new CSkillEffectHandler();
	pcForceOrbHandler					= new CForceOrbHandler();
	pcGrandFuryHandler					= new CGrandFury();
	pcUIControllerHandler				= new CUIControllerGeneral();
	pcDamageHandler						= new CDamageHandler();
	pcInputBox							= new InputBox();
	pcCQuestGameHandler					= new CQuestGame();
	pcTestWindowHandler					= new CWindowTest();
	pcBellatraRewardHandler				= new CBellatraReward();
	pcTestScreenHandler					= new CTestScreen();
	pcTitleBoxHandler					= new TitleBox();
	pcSkillDataHandler					= new CSkillDataHandler();
	pcTradeHandler						= new CTradeHandler();
	pcEventGirlHandler					= new CEventGirlHandler();
	pcCamera							= new CCamera();
	pcMessageBalloon					= std::make_shared<UI::MessageBalloon>(Rectangle2D(0,0,200,480));
	pcCrystalTowerHandler				= new CCrystalTowerHandler();
	pcQuestMapHandler					= new CQuestMapHandler();
	pcKnockupUnitHandler				= new CKnockupUnitHandler();
	pcTeleportCoreHandler				= new CTeleportCoreHandler();
	pcActionFieldHandler				= new CActionFieldHandler();
	pcDiscordGame						= new CDiscordGame();
	pcItemSetAttributeHandler			= new CItemSetAttributeHandler();
	pcPersonalShopHandler				= new CPersonalShopHandler();
	pcItemBoxHandler					= new CItemBoxHandler();
	pcCaravanHandler					= new CCaravanHandler();
	pcChristmasTreeHandler				= new CChristmasTreeHandler();
	pcSocialWindowHandler				= new CSocialWindowHandler();
	pcBattleRoyaleHandler				= new CBattleRoyaleHandler();
	pcEffekseerHandler					= new CEffekseerHandler();
    pcEventHandler                      = new CEventHandler();
	pcGlamorousBrushHandler				= new CGlamorousBrushHandler();
	pcDropItemAreaHandler				= new CDropItemAreaHandler();
	pcCarnivalHandler					= new CCarnivalHandler();
	pcHonorHandler						= new CHonorHandler();
	pcRankingHandler					= new CRankingHandler();
	pcAkatsukiHandler					= new CAkatsukiHandler();

	pcMountHandler						= new CMountHandler();

	pcCharacterPvPModeHandler			= new CCharacterPvPModeHandler();

	UnitGame::CreateInstance();

	bCreated = TRUE;
	ZeroMemory( &sServerTime, sizeof( SYSTEMTIME ) );
}
void SetLevelTable()
{
	
	WRITEDWORD((DWORD)(0x00458F9C+ 1), (DWORD)&LevelTable + 2);

	WRITEDWORD((DWORD)(0x00459880 + 1), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00459885 + 2), (DWORD)&LevelTable + 0x4);
	WRITEDWORD((DWORD)(0x004598AF + 3), (DWORD)&LevelTable + 0x4);
	WRITEDWORD((DWORD)(0x004598B8 + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x0045990B + 3), (DWORD)&LevelTable + 0x4);
	WRITEDWORD((DWORD)(0x00459916 + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00459940 + 3), (DWORD)&LevelTable + 0x4);
	WRITEDWORD((DWORD)(0x0045994B + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00459954 + 3), (DWORD)&LevelTable[1] + 4);
	WRITEDWORD((DWORD)(0x0045995F + 3), (DWORD)&LevelTable[1]);
	WRITEDWORD((DWORD)(0x00459A2F + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00459A38 + 3), (DWORD)&LevelTable + 0x4);
	WRITEDWORD((DWORD)(0x00459A94 + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00459A9B + 3), (DWORD)&LevelTable + 0x4);
	WRITEDWORD((DWORD)(0x00460839 + 1), (DWORD)&LevelTable[SERVER_LEVEL_MAX - 1] + 4);
	WRITEDWORD((DWORD)(0x00460846 + 2), (DWORD)&LevelTable[SERVER_LEVEL_MAX - 1]);
	WRITEDWORD((DWORD)(0x004608F8 + 1), (DWORD)&LevelTable[SERVER_LEVEL_MAX - 1] + 4);
	WRITEDWORD((DWORD)(0x004608EE + 2), (DWORD)&LevelTable[SERVER_LEVEL_MAX - 1]);
	WRITEDWORD((DWORD)(0x004608AF + 3), (DWORD)&LevelTable + 0x4);
	WRITEDWORD((DWORD)(0x004608B8 + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x0046090A + 3), (DWORD)&LevelTable[1]);
	WRITEDWORD((DWORD)(0x00460AF7 + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00460AFE + 3), (DWORD)&LevelTable + 0x4);
	WRITEDWORD((DWORD)(0x004A66FE + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x004A6707 + 3), (DWORD)&LevelTable + 0x4);
	WRITEDWORD((DWORD)(0x004A675A + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x004A6763 + 3), (DWORD)&LevelTable + 0x4);


	WRITEDWORD((DWORD)(0x004598F3 + 3), (DWORD)&LevelTable - 0x4);
	WRITEDWORD((DWORD)(0x00459902 + 3), (DWORD)&LevelTable - 0x8);
	WRITEDWORD((DWORD)(0x00459A05 + 3), (DWORD)&LevelTable - 0x8);
	WRITEDWORD((DWORD)(0x00459A0D + 3), (DWORD)&LevelTable - 0x4);
	WRITEDWORD((DWORD)(0x00460B06 + 3), (DWORD)&LevelTable - 0x8);
	WRITEDWORD((DWORD)(0x00460B0F + 3), (DWORD)&LevelTable - 0x4);
	WRITEDWORD((DWORD)(0x004A66E2 + 3), (DWORD)&LevelTable - 0x4);
	WRITEDWORD((DWORD)(0x004A66F7 + 3), (DWORD)&LevelTable - 0x8);
	WRITEDWORD((DWORD)(0x004A6732 + 3), (DWORD)&LevelTable - 0x8);
	WRITEDWORD((DWORD)(0x004A673E + 3), (DWORD)&LevelTable - 0x4);
	WRITEDWORD((DWORD)(0x004A6753 + 3), (DWORD)&LevelTable - 0x8);
	WRITEDWORD((DWORD)(0x004A679C + 3), (DWORD)&LevelTable - 0x8);
	WRITEDWORD((DWORD)(0x004A67A8 + 3), (DWORD)&LevelTable - 0x4);
}
void GameCore::Hooks()
{
	//std::ofstream ofile("LevelTable.txt", std::ofstream::out);
	//INT64 *pExp = (INT64*)0x0082B9E8;
	//for (int i = 0; i < 150; i++)	
	//	ofile << pExp[i] << "," << std::endl;	
	//ofile.close();
	auto ReadChecksumFileCRC32 = []( const std::string & strFile ) -> std::uint32_t
	{
		std::ifstream is{ strFile, std::ios::in | std::ios::binary };

		std::vector<char>   buffer( 8192 );

		boost::crc_32_type result;

		while ( is.read( &buffer[0], buffer.size() ) )
		{
			std::size_t count = (std::size_t)is.gcount();
			result.process_bytes( &buffer[0], count );
		}

		return result.checksum();
	};

	Game::SetGameChecksum( ReadChecksumFileCRC32( "game.exe" ) );
	Game::SetDLLChecksum( ReadChecksumFileCRC32( "game.dll" ) );

	extern void __cdecl		ItemCreatorInitHook();
	ItemCreatorInitHook();

	CTimerSkill::Hooks();

	// Level Cap
	*(int*)(0x04B06F14) = SERVER_LEVEL_MAX;

	//Read Skills
	CopyMemory( (void*)0x00973458, &saSkill, sizeof(Skill) * MAX_SKILLS );
	CopyMemory( (void*)0x00961B18, &saSkillBase, sizeof(SkillBase) * MAX_SKILLS );

	CAntiCheat::UpdateSkillCooldown();

//	CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&CAntiCheat::DebugSelf, (new SDebugSelf{ GetCurrentProcessId(), GetCurrentThreadId() }), 0, NULL );

	// Set sockets to 0
	WRITEBYTE( 0x38FED40, 0 );
	WRITEBYTE( 0x38FEE40, 0 );
	WRITEDWORD( 0x04C38C28, 27242 ); //Char

	// Initialization Game
	CALL( 0x00484B40 );
	CALL( 0x00448D30 );

	DWORD dwOld = 0;
	VirtualProtect( (void*)0x00401000, 0x3AC000, PAGE_EXECUTE_READWRITE, &dwOld );

	const int iRenderVertexTotal	= 200000;
	const int iRenderFaceTotal		= 200000;
	//LevelTable
	SetLevelTable();
	//Aging
	WRITEBYTE( 0x0050F21D, MAX_AGING );

	//Vertex
	WRITEDWORD( 0x0047E571, 0x40 * iRenderVertexTotal );
	WRITEDWORD( 0x0047E582, (0x40 * iRenderVertexTotal) / 4 );
	//Face
	WRITEDWORD( 0x0047E58A, 0x1C * iRenderFaceTotal );
	WRITEDWORD( 0x0047E5A0, (0x1C * iRenderFaceTotal) / 4 );

	WRITEDWORD( (DWORD)(0x00458F70 + 1), (DWORD)(&saMixingColorTable[0]) + 2 );
	WRITEDWORD( (DWORD)(0x00458F9C + 1), (DWORD)(&saMixingColorTable[_countof( saMixingColorTable )]) + 2 );
	WRITEDWORD( (DWORD)(0x00458FD0 + 3), (DWORD)(&saMixingColorTable[0]) + 8 );
	WRITEDWORD( (DWORD)(0x00458FDA + 3), (DWORD)(&saMixingColorTable[0]) + 10 );

	WRITEDWORD( (DWORD)(0x0045934E + 3), (DWORD)(&saMixingColorTable[0]) + 0 );
	WRITEDWORD( (DWORD)(0x00459359 + 3), (DWORD)(&saMixingColorTable[0]) + 2 );
	WRITEDWORD( (DWORD)(0x0045936A + 3), (DWORD)(&saMixingColorTable[0]) + 4 );
	WRITEDWORD( (DWORD)(0x0045937B + 3), (DWORD)(&saMixingColorTable[0]) + 6 );

	WRITEDWORD( (DWORD)(0x0045917D + 3), (DWORD)(&saMixingColorTable[0]) + 0 );
	WRITEDWORD( (DWORD)(0x0045918B + 3), (DWORD)(&saMixingColorTable[0]) + 2 );
	WRITEDWORD( (DWORD)(0x0045919F + 3), (DWORD)(&saMixingColorTable[0]) + 4 );
	WRITEDWORD( (DWORD)(0x004591B3 + 3), (DWORD)(&saMixingColorTable[0]) + 6 );

	// Color Agging
	AgingColorStructure * psAgingColor = (AgingColorStructure*)(0x0082B8B0);
	CopyMemory( psAgingColor, &saAgingColorTable, sizeof( AgingColorStructure )*_countof( saAgingColorTable ) );

	WRITEDWORD( (DWORD)(0x00458F3A + 1), &saAgingColorTable[0].sGreen );
	WRITEDWORD( (DWORD)(0x00458F67 + 1), &saAgingColorTable[_countof( saAgingColorTable )].sGreen );
	WRITEDWORD( (DWORD)(0x004590EE + 3), &saAgingColorTable[0].sRed );
	WRITEDWORD( (DWORD)(0x004590FC + 3), &saAgingColorTable[0].sGreen );
	WRITEDWORD( (DWORD)(0x00459110 + 3), &saAgingColorTable[0].sBlue );
	WRITEDWORD( (DWORD)(0x00459124 + 3), &saAgingColorTable[0].sAlpha );
	WRITEDWORD( (DWORD)(0x004592D2 + 3), &saAgingColorTable[0].sRed );
	WRITEDWORD( (DWORD)(0x004592DD + 3), &saAgingColorTable[0].sGreen );
	WRITEDWORD( (DWORD)(0x004592EE + 3), &saAgingColorTable[0].sBlue );
	WRITEDWORD( (DWORD)(0x004592FF + 3), &saAgingColorTable[0].sAlpha );

	//Skill
	{
		WRITEDWORD( (DWORD)(0x0044EBF6), &saSkillData[0].pszSkillName );
		WRITEDWORD( (DWORD)(0x0044EC10), &saSkillData[0].pszSkillName );
		WRITEDWORD( (DWORD)(0x0044EC23), &saSkillData[1].pszSkillName );
		WRITEDWORD( (DWORD)(0x004297C6), &saSkillData[0].iSkillCode );
	}

	//Sound Skill
	{
		WRITEDWORD( (DWORD)(0x00543992), &saSoundData[0].pszSkillSoundFile );
		WRITEDWORD( (DWORD)(0x0054399B), &saSoundData[0].pszSkillSoundFile );

		WRITEDWORD( (DWORD)(0x00543DA2), &saSoundData[0].pszSkillSoundFile );
		WRITEDWORD( (DWORD)(0x00543DB3), &saSoundData[0].iID );
		WRITEDWORD( (DWORD)(0x00543DBC), &saSoundData[1].pszSkillSoundFile );

		WRITEDWORD( (DWORD)(0x00544871), &saSoundData[0].pszSkillSoundFile );
		WRITEDWORD( (DWORD)(0x00544883), &saSoundData[0].iID );
		WRITEDWORD( (DWORD)(0x0054488C), &saSoundData[1].pszSkillSoundFile );
	}

	// Items Stage
	{
		WRITEDWORD( 0x0061EBED, (DWORD)(&saItemsStage[0]) );
		WRITEDWORD( 0x0061EC25, (DWORD)(&saItemsStage[0]) );

		WRITEDWORD( 0x00401946, (DWORD)(&saItemsStage[0]) + 12 );
		WRITEDWORD( 0x0061EBBD, (DWORD)(&saItemsStage[0]) + 12 );
		WRITEDWORD( 0x004019AC, (DWORD)(&saItemsStage[1023]) + 12 );
		WRITEDWORD( 0x0061EBDA, (DWORD)(&saItemsStage[1023]) + 12 );

		WRITEDWORD( 0x0061EC03, (DWORD)(&saItemsStage[0]) + 8 );
		WRITEDWORD( 0x0061EC15, (DWORD)(&saItemsStage[1023]) + 8 );

		WRITEDWORD( 0x004182DD, (DWORD)(&saItemsStage[0]) + 168 );
		WRITEDWORD( 0x00453FBF, (DWORD)(&saItemsStage[0]) + 168 );
		WRITEDWORD( 0x0041833D, (DWORD)(&saItemsStage[1023]) + 168 );
		WRITEDWORD( 0x004540D3, (DWORD)(&saItemsStage[1023]) + 168 );

	}

	//Monster and character effect sound
	{
		WRITEDWORD( 0x0054369B, &saSoundEffectData[0] );
		WRITEDWORD( 0x0054378A, &saSoundEffectData[_countof( saSoundEffectData )-1] );
	}

	//Sheltom List
	CopyMemory( (void *)0x0082E270, iaSheltomAgingList, sizeof( DWORD ) * MAX_AGING * 12 );

	//Aging Image (OS1..)
	WRITEBYTE( 0x0050B247, 16 );
	WRITEBYTE( 0x0050EECF, 16 );
	WRITEBYTE( 0x004B3C4B, 16 );
}

GameCore::~GameCore()
{

	DELET( pcGraphicEngineHandler );
	DELET( pcSystemHandler );
	DELET( pcSkillManagerHandler );
	DELET( pcChatGameHandler );
	DELET( pcItemHandler );
	DELET( pcSettingsHandler );
	DELET( pcItemTimerHandler );
	DELET( pcLoginControllerHandler );
	DELET( pcCharacterGameHandler );
	DELET( pcAntiCheatHandler );
	DELET( pcKeyboardHandler );
	DELET( pcMouseHandler );
	DELET( pcSoundHandler );
	DELET( pcPacketHandler );
	DELET( pcDamageInfoHandler );
	DELET( pcAgingWindowHandler );
	DELET( pcWarehouseHandler );
	DELET( pcItemWindowHandler );
	DELET( pcQuestGameHandler );
	DELET( pcTimerSkillHandler );
	DELET( pcBellatraGameHandler );
	DELET( pcMixHandler );
	DELET( pcDailyQuestHandler );
	DELET( pcMessageBoxHandler );
	DELET( pcBlessCastleHandler );
	SocketGame::DeleteInstance();
	DELET( pcParticleOldHandler );
	DELET( pcShamanHandler );
	DELET( pcHUDHandler );
	DELET( pcHalloweenHandler );
	DELET( pcManufactureHandler );
	DELET( pcPerfectItemViewHandler );
	DELET( pcCoinHandler );
	DELET( pcCompareItemHandler );
	DELET( pcQuestMapHandler );
	DELET( pcKnockupUnitHandler );
	DELET( pcTeleportCoreHandler );
	DELET( pcItemSetAttributeHandler );
	DELET( pcPersonalShopHandler );
	DELET( pcItemBoxHandler );
	DELET( pcCaravanHandler );
	DELET( pcChristmasTreeHandler );
	DELET( pcSocialWindowHandler );
	DELET( pcBattleRoyaleHandler );
	DELET( pcEffekseerHandler );
    DELET( pcEventHandler );
	DELET( pcGlamorousBrushHandler );
	DELET( pcDropItemAreaHandler );
	DELET( pcCarnivalHandler );
	DELET( pcHonorHandler );
	DELET( pcRankingHandler );
	DELET( pcAkatsukiHandler );
	DELET( pcMountHandler );
	DELET( pcCharacterPvPModeHandler );

	pcItemInfoBox = nullptr;
	pcItemInfoBox = NULL;

	pcSkillInfoBox = nullptr;
	pcSkillInfoBox = NULL;

	DELET( pcAccountHandler );
	DELET( pcAnimationHandler );
	DELET( pcMapGame );
	DELET( pcXMasHandler );
	DELET( pcSkillEffectHandler );
	DELET( pcForceOrbHandler );
	DELET( pcGrandFuryHandler );
	DELET( pcUIControllerHandler );;
	DELET( pcDamageHandler );
	DELET( pcInputBox );
	DELET( pcCQuestGameHandler );
	DELET( pcTestWindowHandler );
	DELET( pcBellatraRewardHandler );
	DELET( pcTestScreenHandler );
	DELET( pcTitleBoxHandler );
	DELET( pcSkillDataHandler );
	DELET( pcTradeHandler );
	DELET( pcEventGirlHandler );
	DELET( pcActionFieldHandler );
	DELET( pcDiscordGame );

	pcMessageBalloon = nullptr;
	pcMessageBalloon = NULL;

	DELET( pcCrystalTowerHandler );

	UnitGame::DeleteInstance();
}

GameCore * GameCore::GetInstance()
{
	return GAMECORE;
}

void GameCore::Init()
{
	INIT( pcEffekseerHandler );
	INIT( pcItemTimerHandler );
	INIT( pcSettingsHandler );	
	INIT( pcAgingWindowHandler );
	INIT( pcWarehouseHandler );
	INIT( pcDailyQuestHandler );
	INIT( pcMessageBoxHandler );
	INIT( pcHalloweenHandler );
	INIT( pcManageWindow );
	INIT( pcCoinHandler );
	INIT( pcItemInfoBox );
	INIT( pcSkillInfoBox );
	INIT( pcCompareItemHandler );
	INIT( pcAnimationHandler );
	INIT( pcSkillEffectHandler );
	INIT( pcGrandFuryHandler );
	INIT( pcUIControllerHandler );
	INIT( pcInputBox );
	INIT( pcDamageInfoHandler );
	INIT( pcCrystalTowerHandler );
	INIT( pcCQuestGameHandler );
	INIT( pcTestWindowHandler );
	INIT( pcBellatraRewardHandler );
	INIT( pcTitleBoxHandler );
	INIT( pcTradeHandler );
	INIT( pcEventGirlHandler );
	INIT( pcBlessCastleHandler );
	INIT( pcHUDHandler->GetInventoryWindow() );
	INIT( pcMessageBalloon );
	INIT( pcQuestMapHandler );
	INIT( pcKnockupUnitHandler );
	INIT( pcTeleportCoreHandler );
	INIT( pcActionFieldHandler );
	INIT( pcDiscordGame );
	INIT( pcItemSetAttributeHandler );
	INIT( pcPersonalShopHandler );
	INIT( pcItemBoxHandler );
	INIT( pcCaravanHandler );
	INIT( pcChristmasTreeHandler );
	INIT( pcSocialWindowHandler );
	INIT( pcBattleRoyaleHandler );
    INIT( pcEventHandler );
	INIT( pcGlamorousBrushHandler );
	INIT( pcDropItemAreaHandler );
	INIT( pcCarnivalHandler );
	INIT( pcRankingHandler );
	INIT( pcMountHandler );
	INIT( pcCharacterPvPModeHandler );

//	DX::cSelectGlow1.Init( GRAPHICENGINE->GetRenderer() );

	bInit = TRUE;

	//Load Textures
	CALL( 0x0048AB40 );

	ANIMATIONHANDLER->LoadDynamicAnimation();

	//Unload Textures
	CALL( 0x0048ABE0 );
	WRITEDWORD( 0x03500CE4, 0 );

    CAntiCheat::UpdateHardwareID();

#ifdef _ANIM

	if ( FILEEXIST( "debug.ini" ) )
	{
		INI::CReader cReader( "debug.ini" );

		std::string strSMBFile, strINXFile, strAnimationFileSMB, strAnimationFileASE;

		std::string strClassFlag = cReader.ReadString( "Animation", "Class" );
		int iClassFlag = 0;

		if ( STRINGCOMPAREI( strClassFlag.c_str(), "Fighter" ) )
			iClassFlag = CLASSFLAG_Fighter;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Mech" ) )
			iClassFlag = CLASSFLAG_Mechanician;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Archer" ) )
			iClassFlag = CLASSFLAG_Archer;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Pikeman" ) )
			iClassFlag = CLASSFLAG_Pikeman;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Assassin" ) )
			iClassFlag = CLASSFLAG_Assassin;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Knight" ) )
			iClassFlag = CLASSFLAG_Knight;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Atalanta" ) )
			iClassFlag = CLASSFLAG_Atalanta;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Priestess" ) )
			iClassFlag = CLASSFLAG_Priestess;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Mage" ) )
			iClassFlag = CLASSFLAG_Magician;
		else if ( STRINGCOMPAREI( strClassFlag.c_str(), "Shaman" ) )
			iClassFlag = CLASSFLAG_Shaman;

		int iAnimationType = cReader.ReadInt( "Animation", "AnimationType" );
		int iSkillCode = cReader.ReadInt( "Animation", "SkillCode" );
		BOOL bLoop = cReader.ReadBool( "Animation", "Loop" );
		BOOL bkPTMode = cReader.ReadBool( "Animation", "KPT" );

		strSMBFile = cReader.ReadString( "Animation", "SMBFile" );
		strINXFile = cReader.ReadString( "Animation", "INXFile" );
		strAnimationFileSMB = cReader.ReadString( "Animation", "AnimationFileSMB" );
		strAnimationFileASE = cReader.ReadString( "Animation", "AnimationFileASE" );

		DeleteFileA( strAnimationFileSMB.c_str() );
		LoadPTModelPhysique( strAnimationFileASE.c_str() );

		ANIMATIONHANDLER->FixSMBINXSubFileMotion( strSMBFile.c_str(), strINXFile.c_str(), strSMBFile.c_str(), strINXFile.c_str() );
		ANIMATIONHANDLER->InjectAnimation( strAnimationFileSMB.c_str(), strSMBFile.c_str(), strINXFile.c_str(), iAnimationType, 3, bLoop, iClassFlag, iSkillCode );

		ANIMATIONHANDLER->PrintInfoINX( strINXFile.c_str() );

		ExitProcess( 0 );

		return;
	}
#endif
}

void GameCore::Shutdown()
{
	SHUTDOWN( pcSettingsHandler );
	SocketGame::GetInstance()->UnLoad();

	SHUTDOWN( pcDiscordGame );

	//SHUTDOWN( pcSocketGameHandler );
}

BOOL GameCore::OnKeyPress(CKeyboard * pcKeyboard)
{
	if ( pcKeyboard->GetEvent() == EKeyboardEvent::KeyDown && pcKeyboard->GetKey() == VK_F9 && GM_MODE )
		bDebug = !bDebug;


	if ( pcInputBox->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcMessageBoxHandler->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcSettingsHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE; 		
	if ( pcPerfectItemViewHandler->OnKeyPress( pcKeyboard ) )
		return TRUE; 		
	if ( pcDailyQuestHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcGrandFuryHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcCoinHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if( pcCompareItemHandler->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcItemBoxHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcCaravanHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcCarnivalHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcRankingHandler->OnKeyPress( pcKeyboard ) )
		return TRUE;
	if ( pcMountHandler->GetWindow()->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

BOOL GameCore::OnKeyChar(CKeyboard * pcKeyboard)
{
	if ( pcInputBox->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcMessageBoxHandler->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcSettingsHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcDailyQuestHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcCoinHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcGrandFuryHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcTradeHandler->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcPersonalShopHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcItemBoxHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcCaravanHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcCarnivalHandler->GetWindow()->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcRankingHandler->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcMountHandler->OnKeyChar( pcKeyboard ) )
		return TRUE;
	if ( pcCharacterPvPModeHandler->OnKeyChar( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

void GameCore::OnResolutionChanged()
{
	ITEMTIMERHANDLER->OnResolutionChanged();
	pcSettingsHandler->GetWindow()->OnResolutionChanged();
	pcDailyQuestHandler->GetWindow()->OnResolutionChanged();
	pcGrandFuryHandler->GetWindow()->OnResolutionChanged();
	pcCQuestGameHandler->GetWindow()->OnResolutionChanged();
	pcBellatraRewardHandler->OnResolutionChanged();
	pcCoinHandler->GetWindow()->OnResolutionChanged();
	pcEventGirlHandler->OnResolutionChanged();
	pcBlessCastleHandler->OnResolutionChanged();
	pcTeleportCoreHandler->OnResolutionChanged();
	pcActionFieldHandler->OnResolutionChanged();
	pcItemBoxHandler->GetWindow()->OnResolutionChanged();
	pcCaravanHandler->GetWindow()->OnResolutionChanged();
	pcChristmasTreeHandler->GetWindow()->OnResolutionChanged();
	pcChristmasTreeHandler->GetWindowGift()->OnResolutionChanged();
	pcSocialWindowHandler->OnResolutionChanged();
	pcBattleRoyaleHandler->OnResolutionChanged();
	pcCarnivalHandler->GetWindow()->OnResolutionChanged();
	pcRankingHandler->OnResolutionChanged();
	pcMountHandler->GetWindow()->OnResolutionChanged();

	if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Character )
		((CCharacterScreen *)GAMESCREEN->GetActiveScreen())->OnResolutionChanged();
}

BOOL GameCore::OnMouseClick( class CMouse * pcMouse )
{
	if ( pcInputBox->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcItemTimerHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if( pcBlessCastleHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcMessageBoxHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcSettingsHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcWarehouseHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcDailyQuestHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcGrandFuryHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcActionFieldHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcItemWindowHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcCoinHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( SKILLMANAGERHANDLER->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcUIControllerHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcGlamorousBrushHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcCQuestGameHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcTradeHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcTestWindowHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcBellatraRewardHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcEventGirlHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcQuestMapHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcTeleportCoreHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcPersonalShopHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;
	
	if ( pcItemBoxHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcCaravanHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcChristmasTreeHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcChristmasTreeHandler->GetWindowGift()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcBattleRoyaleHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcSocialWindowHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcCarnivalHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcRankingHandler->OnMouseClick( pcMouse ) )
		return TRUE;

	if ( pcMountHandler->GetWindow()->OnMouseClick( pcMouse ) )
		return TRUE;

	return FALSE;
}

BOOL GameCore::OnMouseScroll( class CMouse * pcMouse )
{
	if( pcInputBox->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcCoinHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcGrandFuryHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcCQuestGameHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcBellatraRewardHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcSettingsHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if( pcTestWindowHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if ( pcEventGirlHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if ( pcTeleportCoreHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if ( pcBattleRoyaleHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if ( pcSocialWindowHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if ( pcCarnivalHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;
	if ( pcRankingHandler->OnMouseScroll( pcMouse ) )
		return TRUE;
	if ( pcMountHandler->GetWindow()->OnMouseScroll( pcMouse ) )
		return TRUE;

	return FALSE;
}

void GameCore::OnMouseMove( class CMouse * pcMouse )
{
	pcInputBox->OnMouseMove( pcMouse );
	pcItemTimerHandler->OnMouseMove( pcMouse );
	pcMessageBoxHandler->OnMouseMove( pcMouse );
	pcSettingsHandler->GetWindow()->OnMouseMove( pcMouse );
	pcWarehouseHandler->GetWindow()->OnMouseMove( pcMouse );
	pcDailyQuestHandler->GetWindow()->OnMouseMove( pcMouse );
	pcCoinHandler->GetWindow()->OnMouseMove( pcMouse );
	pcGrandFuryHandler->GetWindow()->OnMouseMove( pcMouse );
	pcActionFieldHandler->OnMouseMove( pcMouse );
	pcCQuestGameHandler->GetWindow()->OnMouseMove( pcMouse );
	pcBellatraRewardHandler->OnMouseMove( pcMouse );
	pcTestWindowHandler->OnMouseMove( pcMouse );
	pcTradeHandler->OnMouseMove( pcMouse );
	pcEventGirlHandler->OnMouseMove( pcMouse );
	pcBlessCastleHandler->OnMouseMove( pcMouse );
	pcTeleportCoreHandler->OnMouseMove( pcMouse );
	pcPersonalShopHandler->GetWindow()->OnMouseMove( pcMouse );
	pcItemBoxHandler->GetWindow()->OnMouseMove( pcMouse );
	pcCaravanHandler->GetWindow()->OnMouseMove( pcMouse );
	pcChristmasTreeHandler->GetWindow()->OnMouseMove( pcMouse );
	pcChristmasTreeHandler->GetWindowGift()->OnMouseMove( pcMouse );
	pcBattleRoyaleHandler->GetWindow()->OnMouseMove( pcMouse );
	pcSocialWindowHandler->OnMouseMove( pcMouse );
	pcCarnivalHandler->GetWindow()->OnMouseMove( pcMouse );
	pcRankingHandler->OnMouseMove( pcMouse );
	pcMountHandler->GetWindow()->OnMouseMove( pcMouse );
}

extern DWORD				dwLastTickLogin;
extern DWORD				dwLastTickGame;
extern int					iLoginServerPingID;
extern int					iGameServerPingID;

void GameCore::Update( float fTime )
{
	double fdTime = (double)fTime;
	static double fCount_14MS = 0.0f; static double fFPS70 = (1000.0f / 70.0f);
	static double fCount_16MS = 0.0f; static double fFPS60 = (1000.0f / 60.0f);
	static double fCount_100MS = 0.0f; static double f100MS = 100.0f;
	static double fCount_1SECO = 0.0f; static double f1SECO = 1000.0f;
	static double fCount_1MINU = 0.0f; static double f1MINU = 60000.0f;

	static int iTimes = 0;

	fCount_14MS += fdTime;
	fCount_16MS += fdTime;
	fCount_100MS += fdTime;
	fCount_1SECO += fdTime;
	fCount_1MINU += fdTime;

	Frame();

	pcTitleBoxHandler->Update( fTime );

	//Skills Timers
	while ( fCount_14MS >= fFPS70 )
	{
		if ( Game::GetGameMode() == GAMEMODE_InGame )
		{
			pcItemInfoBox->Update( (float)fFPS70 );

			CALL( 0x005224E0 );
		}

		pcParticleOldHandler->Update( fTime );

		pcSkillEffectHandler->Update();

		pcQuestMapHandler->Update( (float)fFPS70 );

		pcKnockupUnitHandler->Update( (float)fFPS70 );

		pcActionFieldHandler->Update( (float)fFPS70 );

		fCount_14MS -= fFPS70;
	}

	while ( fCount_16MS >= fFPS60 )
	{
		Core::Timer::Update( (float)fFPS60 );

		GRAPHICS->GetRenderer()->Update( (float)fFPS60 );

		pcPersonalShopHandler->GetWindow()->Update( (float)fFPS60 );

		pcBattleRoyaleHandler->Update( (float)fFPS60 );

		pcCaravanHandler->GetWindow()->Update( (float)fFPS60 );

		pcHUDHandler->GetAchievementHandler()->GetWindow()->Update( (float)fFPS60 );

		pcEffekseerHandler->Update( (float)fFPS60 );

		pcDropItemAreaHandler->Update( (float)fFPS60 );

		pcCarnivalHandler->Update( (float)fFPS60 );

		pcTestWindowHandler->Update( (float)fFPS60 );

		pcRankingHandler->Update( (float)fFPS60 );
		
		pcMountHandler->GetWindow()->Update( (float)fFPS60 );

		//Map Dissolve
		if ( Game::GetGameMode() == GAMEMODE_InGame )
		{
			if ( STAGE1 && (STAGE1->fDissolveMapAmount > 0.0f) )
				STAGE1->fDissolveMapAmount -= (float)fFPS60 / 2000.0f;
			if ( STAGE2 && (STAGE2->fDissolveMapAmount > 0.0f) )
				STAGE2->fDissolveMapAmount -= (float)fFPS60 / 2000.0f;
		}

		fCount_16MS -= fFPS60;
	}
	
	// 100 ms
	while ( fCount_100MS >= f100MS )
	{
		pcShamanHandler->Update( fTime );
		pcSoundHandler->Update();
		GameCore::pcManageWindow->Tick();
		pcHUDHandler->Update( fTime );
		pcSettingsHandler->GetWindow()->Update( fTime );

		pcItemTimerHandler->Loop();

		pcBlessCastleHandler->Update( fTime );

		pcTeleportCoreHandler->Update( fTime );

		pcHalloweenHandler->Update( (float)f100MS );

		pcChristmasTreeHandler->Update( (float)f100MS );

		pcSocialWindowHandler->Update( (float)f100MS );

		SocketGame::GetInstance()->Tick( TICKTYPE_100MilliSecond );

		pcGlamorousBrushHandler->Update( (float)f100MS );

		pcMountHandler->Update( (float)f100MS );


		Game::FlushLoaders();

		fCount_100MS -= f100MS;
	}
	// 1000 ms
	while ( fCount_1SECO >= f1SECO )
	{
		pcDamageInfoHandler->Update(fTime);

		pcGrandFuryHandler->Update();

		pcCQuestGameHandler->Update();

		pcTitleBoxHandler->Tick();

		pcItemTimerHandler->Tick();

		pcManufactureHandler->Update();

		pcCrystalTowerHandler->Update();

		pcItemSetAttributeHandler->Update();

		pcDiscordGame->Update();

		pcItemBoxHandler->GetWindow()->Update( (float)f1SECO );

		MESSAGEBOX->Update();

		pcSocialWindowHandler->GetClan()->Update();

		pcAkatsukiHandler->Update( (float)f1SECO );

		pcMessageBalloon->Update( (float)f1SECO );

		iTimes++;

		if ( (iTimes >= 2) && (Game::GetGameMode() == GAMEMODE_InGame) )
		{
			CAntiCheat::UnloadLibrary();

			if ( UNITDATA->iStunTimeLeft > 0 )
				pcSkillManagerHandler->StunEffect( UNITDATA, 5 );
		}

		if( ( iTimes % 2 ) == 0 )
			CManageWindow::UpdateClanIcons();

		EXEVertexCache::Update();

		pcAntiCheatHandler->Update( (float)f1SECO );

		fCount_1SECO -= f1SECO;
	}


	DXGraphicEngine::GetScreenShot();
}


void GameCore::UpdateFrame()
{
	typedef FLOAT( __cdecl *tfnUpdatePT )();
	tfnUpdatePT fnUpdatePT = (tfnUpdatePT)0x004164D0;

	typedef void( __cdecl *tfnFrame )(float fTime);
	tfnFrame fnFrame = (tfnFrame)0x0041B5E0;

	float fTime = fnUpdatePT();

	fnFrame( fTime );

	GameCore::GetInstance()->Update( fTime );
}

void GameCore::Frame()
{
}

void GameCore::Render3D()
{
}

void GameCore::Render2D()
{
	pcXMasHandler->Render();
	pcShamanHandler->RenderParticle();
	pcCrystalTowerHandler->Render();
	pcSettingsHandler->GetView()->Render();
	pcAgingWindowHandler->Render();
	pcWarehouseHandler->GetWindow()->Render();
	pcBellatraGameHandler->SetCrown();
	pcDailyQuestHandler->GetWindow()->Render();
	pcCoinHandler->GetWindow()->Render();
	pcGrandFuryHandler->Render();
	pcTradeHandler->Render();
	pcCQuestGameHandler->GetWindow()->Render();
	pcTestWindowHandler->Render();
	pcBellatraRewardHandler->Render();
	pcEventGirlHandler->Render();
	pcTeleportCoreHandler->Render();
	pcPersonalShopHandler->GetWindow()->Render();
	pcItemBoxHandler->GetWindow()->Render();
	pcCaravanHandler->GetWindow()->Render();
	pcChristmasTreeHandler->GetWindow()->Render();
	pcChristmasTreeHandler->GetWindowGift()->Render();
	pcBattleRoyaleHandler->GetWindow()->Render();
	pcCarnivalHandler->GetWindow()->Render();
	pcRankingHandler->Render2D();
	pcMountHandler->GetWindow()->Render();
	pcSocialWindowHandler->Render();
	pcMessageBoxHandler->Render();
	pcInputBox->Render();
	pcTitleBoxHandler->Render();
	pcPerfectItemViewHandler->Render();

	if( (*GameCore::pcClanManageWindow->pbOpen) )
				if( GameCore::pcManageWindow->iPage == MANAGEWINDOWPAGE_ClanMembers )
					GameCore::pcManageWindow->RenderClanMemberStatus();

	if( (*GameCore::pcManageWindow->piHorScroll) == 0x07D0 )
		if( GameCore::pcManageWindow->iPage == MANAGEWINDOWPAGE_Friends )
			if( GameCore::pcManageWindow->iSubPage == MANAGEWINDOWSUBPAGE_Friends )
				GameCore::pcManageWindow->RenderFriendStatus();

	pcHalloweenHandler->Render();
	pcCompareItemHandler->Render();
	pcItemInfoBox->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pcSkillInfoBox->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pcGlamorousBrushHandler->Render();
	pcQuestMapHandler->Render();
	pcBattleRoyaleHandler->Render();
	pcUIControllerHandler->RenderMouseItem();

	if( bDebug )
		UNITGAME->RenderDebugText();
}

void GameCore::OnEffect()
{
	// Effect Manager (??)
	__asm
	{
		PUSH 0x3C6A0EA1;
		MOV EAX, 0x005CCA70;
		CALL EAX;
		MOV ECX, EAX;
		MOV EAX, 0x005CBE70;
		CALL EAX;
	}
	// Sin 3D
	CALLT( 0x00525CC0, 0x03707170 );

	// Effect Manager Main
	CALLT( 0x00568D80, 0x0389AC98 );
	// Phisycs Manager Main
	CALLT( 0x005A8260, 0x038FEB10 );

	// Particles Main
	__asm
	{
		PUSH 0x3C6A0EA1;
		PUSH 0;
		MOV ECX, 0x039032CC;
		MOV EAX, 0x005989B0;
		CALL EAX;
	}

	// Skills T5 Effect
	SKILLMANAGERHANDLER->OnEffect();

	// ???
	CALL( 0x005A9940 );
	// ???
	CALL( 0x005ADDB0 );
	// ???
	CALL( 0x005AF4B0 );
}

void GameCore::DrawEffect( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoEffects == FALSE )
	{
		//Get Instance
		DWORD dwClass = CALL( 0x005CCA70 );

		//Effect Manager Update View
		CALLT_WITH_ARG6( 0x005CBD80, dwClass, iX, iY, iZ, iAX, iAY, iAZ );
		//Effect Manager Draw
		CALLT_WITH_ARG6( 0x00568E30, 0x0389AC98, iX, iY, iZ, iAX, iAY, iAZ );
		//Particle Draw
		CALLT_WITH_ARG6( 0x00598F40, 0x039032CC, iX, iY, iZ, iAX, iAY, iAZ );

		PARTICLEENGINE->Render3D( NULL, Point3D( iX, iY, iZ ), Point3D( iAX, iAY, iAZ ) );

		EFFEKSEERHANDLER->Render();
	}

	//Pet Draw
	CALL_WITH_ARG6( 0x005ACBE0, iX, iY, iZ, iAX, iAY, iAZ );
	//PHPet Draw
	CALL_WITH_ARG6( 0x005AE3C0, iX, iY, iZ, iAX, iAY, iAZ );
}


DWORD GameCore::OnHandlerFunctions( DWORD dwCode, DWORD * pdwOptional )
{
	DWORD dwRet = 0;
	switch ( dwCode )
	{
		case 0x1:
			TIMERSKILLHANDLER->OnAddChasingHunt( pdwOptional[0], pdwOptional[1] );
			break;

		case 0x2:
			DrawMessageMiddle( (char*)pdwOptional[0] );
			break;

		case 0x3:
			TIMERSKILLHANDLER->OnCharacterKilled();
			break;

		case 0x4:
			DAMAGEINFOHANDLER->Load();
			SHAMANHANDLER->LoadParticles();
			HALLOWEENHANDLER->LoadParticle();
			XMASHANDLER->LoadParticles();
			SKILLMANAGERHANDLER->LoadParticles();
			CGameEffectFollow::LoadParticles();
			break;

		case 0x5:
			SHAMANHANDLER->OnRainMakerParticle( (UnitData*)pdwOptional[0], (int)pdwOptional[1] );
			break;

		case 0x6:
			TIMERSKILLHANDLER->DrawTimerBox();
			break;

		case 0x7:
			HALLOWEENHANDLER->OnSetTimeDay( (DWORD*)pdwOptional[0], (DWORD*)pdwOptional[1] );
			break;

		case 0x8:
			MANUFACTUREHANDLER->SetManufactureAreaCheck( ITEMHANDLER->GetMouseItem() );
			break;

		case 0x9:
			TIMERSKILLHANDLER->KillTimer( pdwOptional[0] );
			break;

		case 0xA:
			if ( ((UnitGame*)pdwOptional[0]) != 0 )
				((UnitGame*)pdwOptional[0])->DrawLifeBar( pdwOptional[1], pdwOptional[2] );
			break;

		case 0xB:
			UNITGAME->ClearFarUnits();
			break;

		case 0xC:
			MAPGAME->SetMessageBoxMap( (int)pdwOptional[0], TRUE );
			break;

		case 0xD:
			GameCore::GetInstance()->OnEffect();
			break;

		case 0xE:
			CHATGAMEHANDLER->ChatBuffSort();
			break;

		case 0xF:
			Game::OnSetRGBATimer();
			break;

		case 0x10:
			ANIMATIONHANDLER->SetFalconMove( (FalconDataPattern*)pdwOptional[0] );
			break;

		case 0x11:
			CGameProtect::OnRegen();
			break;

		case 0x12:
			CGameProtect::OnSetRegen();
			break;

		case 0x13:
			MIXHANDLER->OnHandlerCharacterUpdate();
			break;

		case 0x14:
			Game::CreateLoaderThread();
			break;

		case 0x15:
			EXETexture * pc;
			pc = ((EXETexture*)pdwOptional);
			if ( pc )
				pc->ReadTextures();
			break;

		case 0x16:
			TIMERSKILLHANDLER->SwapSkillTimer( (Skill*)pdwOptional );
			break;

		case 0x17:
			UNITGAME->RenderItemMotionBlur( (UnitData*)pdwOptional[0], (UnitTool*)pdwOptional[1] );
			break;

		case 0x18:
			if ( (UnitData*)pdwOptional[0] )
				CGameEffectFollow::SetEffectUnitSpawn( (UnitData*)pdwOptional[0] );
			break;

		case 0x19:
			if ( (UnitData*)pdwOptional[0] )
				SKILLMANAGERHANDLER->EventHitMonster( (UnitData*)pdwOptional[0] );
			break;

		case 0x1A:
			EffectBaseFollow::MainEffect();
			break;

		case 0x1B:
			CGameInventory::GetInstance()->ChangeInventorySide( pdwOptional[0] );
			break;

		case 0x1C:
			CTITLEBOX->SetText( pdwOptional[0] );
			break;

		case 0x1D:
			SKILLMANAGERHANDLER->OnMouseClickSkillPet();
			break;

		case 0x1F:
			SKILLMANAGERHANDLER->UpdateSkillCanBuy();
			break;

		case 0x20:
			SKILLMANAGERHANDLER->OnClickSkill();
			break;

		case 0x21:
			CQUESTGAME->OnQuestT5OpenProgress();
			break;

		case 0x22:
			dwRet = SKILLMANAGERHANDLER->OnRankUPButtonClick( MOUSEHANDLER->GetPosition()->iX, MOUSEHANDLER->GetPosition()->iY );
			break;

		case 0x23:
			SKILLMANAGERHANDLER->OnEndSkillTimer( (Skill*)pdwOptional[0] );
			break;

		case 0x24:
			UNITGAME->HandlePacketUnitStatus( (UnitData*)pdwOptional[0], (Packet*)pdwOptional[1] );
			break;

		case 0x25:
			ClientUnitDataUpdate( (UnitData*)pdwOptional[0] );
			break;

		case 0x26:
			UNITGAME->HandlePacketUnitBuff( (UnitData*)pdwOptional[0], (PacketBuffData*)pdwOptional[1] );
			break;

		case 0x27:
			CHATGAMEHANDLER->AddChatBoxText( (char*)pdwOptional[0], (EChatColor)pdwOptional[1] );
			break;

		case 0x28:
			dwRet = CGameInventory::GetInstance()->GetInventoryPersonalShopItem( (ItemData*)pdwOptional[0] );
			break;

		case 0x29:
			ITEMHANDLER->ShowDebugCloneItemWarehouse( (ItemData*)pdwOptional[0] );
			break;

		case 0x2A:
			if ( !GLOBALWINDOW_BLOCKACTION )
				PERSONALSHOPHANDLER->GetWindow()->Open( true );
			break;

		case 0x2B:
			CALL( 0x0045BC90 );
			CALLT( 0x004C41A0, 0x035EBB20 );
			CALL( 0x0045BC90 );
			break;

        case 0x2C:
            HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->SwapInventory( (BOOL)pdwOptional[0] );
            break;

        case 0x2D:
            WRITEDWORD( 0x04B06E7A, UNITDATA->sCharacterData.iAttackRating );
            UNITDATA->sCharacterData.iAttackRating += (UNITDATA->sCharacterData.iAttackRating * (UNITDATA->sActiveSkill.cLevel + 5)) / 100;
            UPDATE_CHARACTER_CHECKSUM;
            break;

        case 0x2E:
            UNITDATA->sCharacterData.iAttackRating = READDWORD( 0x04B06E7A );
            UPDATE_CHARACTER_CHECKSUM;
            break;

        default:
			break;
	}

	return dwRet;
}

void GameCore::SendLoginData()
{
	SocketGame::RequestOpenConnection( true, 1500 );

	PacketLoginUser sPacket;
	sPacket.iLength		= sizeof( PacketLoginUser );
	sPacket.iHeader		= PKTHDR_LoginUser;
	sPacket.dwUnk[0]	= sPacket.dwUnk[1] = sPacket.dwUnk[2] = 0;
	STRINGCOPY( sPacket.szUserID, trim(std::string((char*)0x039033E8)).c_str() );
	STRINGCOPY( sPacket.szPassword, trim( std::string( (char*)0x039032E8)).c_str() );
	
	int iLengthStr = STRLEN( sPacket.szUserID );
	for ( int i = 0; i < iLengthStr; i++ )
		sPacket.szUserID[i] ^= 0x2;

	iLengthStr = STRLEN( sPacket.szPassword );
	for ( int i = 0; i < iLengthStr; i++ )
		sPacket.szPassword[i] ^= 0x4;

	sPacket.uWidthScreen	= GetSystemMetrics( SM_CXSCREEN );
	sPacket.uHeightScreen	= GetSystemMetrics( SM_CYSCREEN );
	
	sPacket.iSystemOS		= SYSTEM->GetSystemOS();

	//PC Name
	sPacket.szPCName[0] = 0;
	DWORD dwSize		= 32;
	GetComputerNameA( sPacket.szPCName, &dwSize );

	// Mac Address
	{
		UUID uuid;

		// Ask OS to create UUID
		UuidCreateSequential( &uuid );

		// Bytes 2 through 7 inclusive 
		sprintf_s( sPacket.szMacAddr, 20, AY_OBFUSCATE( "%02X-%02X-%02X-%02X-%02X-%02X" ),
			uuid.Data4[ 2 ], uuid.Data4[ 3 ], uuid.Data4[ 4 ], uuid.Data4[ 5 ], uuid.Data4[ 6 ], uuid.Data4[ 7 ] );

		if ( STRINGCOMPARE( sPacket.szMacAddr, AY_OBFUSCATE( "00-50-56-C0-00-08" ) ) )
			STRINGCOPY( sPacket.szMacAddr, AY_OBFUSCATE( "C0-50-56-C0-00-08" ) );
	}

	//HD Serial
	{
		char szHD[4];

		szHD[0] = 'C';
		szHD[1] = ':';
		szHD[2] = '\\';
		szHD[3] = 0;

		GetVolumeInformationA( szHD, NULL, 0, &sPacket.dwSerialHD, NULL, NULL, NULL, 0 );

		//Video Name
		STRINGCOPY( sPacket.szVideoName, GRAPHICENGINE->GetVideoName().c_str() );
		sPacket.uUniqueVideoID = GRAPHICENGINE->GetVideoGUID();
	}

	char szFolderName[MAX_PATH] = { 0 };
	GetCurrentDirectoryA( MAX_PATH, szFolderName );
	iLengthStr = STRLEN( szFolderName );
	for ( int i = 0; i < iLengthStr; i++ )
		sPacket.uUniqueFolderID += (int)szFolderName[i];

	STRINGCOPY( sPacket.szHardwareID, Game::GetHardwareID() );

	sPacket.iGameVersion = READDWORD( 0x04AF7FFC );
	sPacket.uGameChecksum = Game::GetGameChecksum();
	sPacket.uDLLChecksum = Game::GetDLLChecksum();

	BYTE * pPacket = (BYTE*)&sPacket;
	for ( int i = 8; i < sizeof( PacketLoginUser ); i++ )
	{
		if ( pPacket[i] != 0 )
			pPacket[i] ^= 8;
	}

	SENDPACKETL( &sPacket );
}


void WINAPI GameCore::GameRun()
{
	typedef BOOL( __cdecl *tfnCheckQuitGame )();
	tfnCheckQuitGame fnCheckQuitGame = (tfnCheckQuitGame)pfnCheckExit;

	GameCore::UpdateFrame();

	CALL( pfnRender );
	GameCore::GetInstance()->Render3D();

	if ( fnCheckQuitGame() )
	{
		WRITEDWORD( 0x0A1754C, 1 );
	}
}

int GameCore::GetAgingNumberColor( int iRed, int iGreen, int iBlue )
{
	int iRet = 0;

	for ( int i = 0; i < _countof( saAgingColorTable ); i++ )
	{
		const auto & v = saAgingColorTable[i];
		if ( (v.sRed == iRed) && (v.sGreen == iGreen) && (v.sBlue == iBlue) )
		{
			iRet = i + 4;
			break;
		}
	}

	return iRet;
}

NAKED void GameCore::SetGameScreen( int iScreen )
{
	JMP( pfnScreenChangeGame );
}

void GameCore::HandlePacket( SocketData * sd, PacketPing * psPacket )
{
	DWORD dwTime = CALL_WITH_ARG1( 0x0061F670, psPacket->dwTick );

	if ( sd == SOCKETG )
	{
		if ( abs( (long)dwTime - (long)READDWORD( 0x00CF4794 ) ) > 10 )
			WRITEDWORD( 0x00CF4794, dwTime );
	}

	sd->SetPing( TICKCOUNT - psPacket->dwTime );
}
