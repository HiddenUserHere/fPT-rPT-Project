#include "stdafx.h"
#include "MapGame.h"

MapGame::MapGame()
{
	pcaBaseMap = (BaseMap*)0x00AD5FD8;
}

MapGame::~MapGame()
{
}

void MapGame::Load()
{
	ZeroMemory( pcaBaseMap, sizeof( BaseMap ) * 256 );

	for ( int i = 0; i < 256; i++ )
		pcaBaseMap[i].SetID( (EMapID)i );

	STRINGCOPYLEN( (char*)0x00AD5F18, 64, "field\\" );
	STRINGCOPYLEN( (char*)0x00AD5F98, 64, "field\\map\\" );
	STRINGCOPYLEN( (char*)0x00AD5F58, 64, "field\\title\\" );

	BaseMap * pcAcasiaForest				= (pcaBaseMap + 0);
	BaseMap * pcBambooForest				= (pcaBaseMap + 1);
	BaseMap * pcGardenOfFreedom				= (pcaBaseMap + 2);
	BaseMap * pcRicartenTown				= (pcaBaseMap + 3);
	BaseMap * pcRefugeOfTheAncients			= (pcaBaseMap + 4);
	BaseMap * pcCastleOfTheLost				= (pcaBaseMap + 5);
	BaseMap * pcRuinenVillage				= (pcaBaseMap + 6);
	BaseMap * pcCursedLand					= (pcaBaseMap + 7);
	BaseMap * pcForgottenLand				= (pcaBaseMap + 8);
	BaseMap * pcNaviskoTown					= (pcaBaseMap + 9);
	BaseMap * pcOasis						= (pcaBaseMap + 10);
	BaseMap * pcBattlefieldOfTheAncients	= (pcaBaseMap + 11);
	BaseMap * pcForbiddenLand				= (pcaBaseMap + 12);
	BaseMap * pcAncientPrisonF1				= (pcaBaseMap + 13);
	BaseMap * pcAncientPrisonF2				= (pcaBaseMap + 14);
	BaseMap * pcAncientPrisonF3				= (pcaBaseMap + 15);
	BaseMap * pcChessRoom					= (pcaBaseMap + 16);
	BaseMap * pcForestOfSpirits				= (pcaBaseMap + 17);
	BaseMap * pcLandOfDusk					= (pcaBaseMap + 18);
	BaseMap * pcValleyOfTranquility			= (pcaBaseMap + 19);
	BaseMap * pcRoadToTheWind				= (pcaBaseMap + 20);
	BaseMap * pcPhillaiTown					= (pcaBaseMap + 21);
	BaseMap * pcCursedTempleF1				= (pcaBaseMap + 22);
	BaseMap * pcCursedTempleF2				= (pcaBaseMap + 23);
	BaseMap * pcMushroomCave				= (pcaBaseMap + 24);
	BaseMap * pcBeehiveCave					= (pcaBaseMap + 25);
	BaseMap * pcDarkSanctuary				= (pcaBaseMap + 26);
	BaseMap * pcRailwayOfChaos				= (pcaBaseMap + 27);
	BaseMap * pcHeartOfPerum				= (pcaBaseMap + 28);
	BaseMap * pcEura						= (pcaBaseMap + 29);
	BaseMap * pcBellatra					= (pcaBaseMap + 30);
	BaseMap * pcGallubiaValley				= (pcaBaseMap + 31);
	BaseMap * pcQuestArena					= (pcaBaseMap + 32);
	BaseMap * pcBlessCastle					= (pcaBaseMap + 33);
	BaseMap * pcGreedyLake					= (pcaBaseMap + 34);
	BaseMap * pcFrozenSanctuary				= (pcaBaseMap + 35);
	BaseMap * pcKelvezuLair					= (pcaBaseMap + 36);
	BaseMap * pcLandOfChaos					= (pcaBaseMap + 37);
	BaseMap * pcLostTemple					= (pcaBaseMap + 38);
	BaseMap * pcGhostCastle					= (pcaBaseMap + 39);
	BaseMap * pcEndlessTowerF1				= (pcaBaseMap + 40);
	BaseMap * pcEndlessTowerF2				= (pcaBaseMap + 41);
	BaseMap * pcCursedTempleF3				= (pcaBaseMap + 42);
	BaseMap * pcEndlessTowerF3				= (pcaBaseMap + 43);
	BaseMap * pcIceMineF1					= (pcaBaseMap + 44);
	BaseMap * pcAtlantisTown				= (pcaBaseMap + 45);
	BaseMap * pcMysteryForest1				= (pcaBaseMap + 46);
	BaseMap * pcMysteryForest2				= (pcaBaseMap + 47);
	BaseMap * pcMysteryForest3				= (pcaBaseMap + 48);
	BaseMap * pcBattleTown					= (pcaBaseMap + 49);
	BaseMap * pcMysteryDesert3				= (pcaBaseMap + 50);
	BaseMap * pcMysteryDesert2				= (pcaBaseMap + 51);
	BaseMap * pcMysteryDesert1				= (pcaBaseMap + 52);
	BaseMap * pcForgottenTempleF1			= (pcaBaseMap + 53);
	BaseMap * pcForgottenTempleF2			= (pcaBaseMap + 54);
	BaseMap * pcAncientDungeonF1			= (pcaBaseMap + 55);
	BaseMap * pcAncientDungeonF2			= (pcaBaseMap + 56);
	BaseMap * pcAncientDungeonF3			= (pcaBaseMap + 57);
	BaseMap * pcAncientWeapon				= (pcaBaseMap + 58);
	BaseMap * pcAbyssOfTheSea				= (pcaBaseMap + 59);
	BaseMap * pcT5QuestArena				= (pcaBaseMap + 60);
	BaseMap * pcSlab						= (pcaBaseMap + 61);
	BaseMap * pcDistorted					= (pcaBaseMap + 62);
	BaseMap * pcSwamp						= (pcaBaseMap + 63);
	BaseMap * pcOldRuinen2					= (pcaBaseMap + 64);
	BaseMap * pcDeathIsland					= (pcaBaseMap + 65);
	BaseMap * pcRoyalDesert					= (pcaBaseMap + 66);
	BaseMap * pcForestDungeon				= (pcaBaseMap + 67);
	BaseMap * pcDragonsDungeon				= (pcaBaseMap + 68);
	BaseMap * pcCursedDesert				= (pcaBaseMap + 69);
	BaseMap * pcIronCore					= (pcaBaseMap + 70);
	BaseMap * pcOasisRoyale					= (pcaBaseMap + 71);

	//Acasia Forest
	pcAcasiaForest->SetFileName( "forest\\fore-3.ase", "fore-3" );
	pcAcasiaForest->SetType( MAPTYPE_Forest );
	pcAcasiaForest->SetSky( 2, 3, 1 );
	pcAcasiaForest->SetBackgroundMusicID( BACKGROUNDMUSICID_Forest );
	pcAcasiaForest->SetLevelRequirement( MAPLEVEL( MAPID_AcasiaForest ) );

	pcAcasiaForest->SetCenter( -16419, -7054 );

	pcAcasiaForest->AddSpawn( -10585, -11810 );
	pcAcasiaForest->AddSpawn( -13659, -9753 );

	pcAcasiaForest->AddTeleport1( -16638, -6737, 267, 64, 32, pcMushroomCave, 119112, 26028, 510, MAPLEVEL( MAPID_MushroomCave ) );
	pcAcasiaForest->AddBorder( pcBambooForest, -8508, -10576 );

	pcAcasiaForest->AddSound( -13819, 226, -8950, 100, 15 );

	pcAcasiaForest->AddStaticModel( "forest\\3ani-01.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-02.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-03.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-04.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-05.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-06.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-07.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-08.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-09.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-10.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-11.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-12.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-13.ASE" );
	pcAcasiaForest->AddStaticModel( "forest\\3ani-14.ASE" );

	//Bamboo Forest
	pcBambooForest->SetFileName( "forest\\fore-2.ase", "fore-2" );
	pcBambooForest->SetType( MAPTYPE_Forest );
	pcBambooForest->SetSky( 2, 3, 1 );
	pcBambooForest->SetBackgroundMusicID( BACKGROUNDMUSICID_Forest );
	pcBambooForest->SetLevelRequirement( MAPLEVEL( MAPID_BambooForest ) );

	pcBambooForest->SetCenter( -5427, -9915 );

	pcBambooForest->AddSpawn( -2272, -10314 );
	pcBambooForest->AddSpawn( -7192, -11175 );

	pcBambooForest->AddTeleport1( -3408, -12447, 251, 64, 32, pcBambooForest, -3265, -12583, 214, 0, TELEPORTTYPE_WarpGate );
	pcBambooForest->AddBorder( pcGardenOfFreedom, -292, -9548 );

	pcBambooForest->AddSound( -3775, 171, -14447, 160, 14 );

	pcBambooForest->AddStaticModel( "forest\\2ani-04.ASE" );

	//Garden of Freedom
	pcGardenOfFreedom->SetFileName( "forest\\fore-1.ase", "fore-1" );
	pcGardenOfFreedom->SetType( MAPTYPE_Forest );
	pcGardenOfFreedom->SetSky( 2, 3, 1 );
	pcGardenOfFreedom->SetBackgroundMusicID( BACKGROUNDMUSICID_Forest );
	pcGardenOfFreedom->SetLevelRequirement( MAPLEVEL( MAPID_GardenOfFreedom ) );

	pcGardenOfFreedom->SetCenter( 4184, -11016 );

	pcGardenOfFreedom->AddSpawn( 1350, -13672 );
	pcGardenOfFreedom->AddSpawn( 1761, -10815 );
	pcGardenOfFreedom->AddSpawn( 4777, -10916 );

	pcGardenOfFreedom->AddBorder( pcRefugeOfTheAncients, 4844, -6835 );
	pcGardenOfFreedom->AddBorder( pcRicartenTown, 2275, -14828 );

	//Ricarten Town
	if ( HALLOWEEN_EVENT_THINGS )
		pcRicartenTown->SetFileName( "Ricarten\\village-2-halloween.ase", "village-2" );
	else if ( XMAS_EVENT_THINGS )
		pcRicartenTown->SetFileName( "RicartenXmas\\village-2.ase", "village-2" );
	else
		pcRicartenTown->SetFileName( "Ricarten\\village-2.ase", "village-2" );

	pcRicartenTown->SetType( MAPTYPE_Town );
	pcRicartenTown->SetSky( 2, 3, 1 );
	pcRicartenTown->SetBackgroundMusicID( XMAS_EVENT_THINGS ? BACKGROUNDMUSICID_Christmas : BACKGROUNDMUSICID_Ricarten );
	pcRicartenTown->SetLevelRequirement( MAPLEVEL( MAPID_RicartenTown ) );

	pcRicartenTown->SetCenter( 2596, -18738 );

	pcRicartenTown->AddSpawn( 2592, -18566 );
	pcRicartenTown->AddSpawn( -1047, -16973 );

	pcRicartenTown->AddTeleport1( 734, -20119, 312, 64, 32, pcRicartenTown, 822, -19956, 254, 1, TELEPORTTYPE_WarpGate );

	pcRicartenTown->AddSound( -1006, 170, -17835, 80, 27 );
	pcRicartenTown->AddSound( 2632, 321, -17285, 80, 27 );

	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani01.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani02.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani03.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani04.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani05.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani06.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani07.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani08.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani09.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani10.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani11.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani12.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani13.ASE" );
	pcRicartenTown->AddStaticModel( "Ricarten\\v-ani14.ASE" );

	//Refuge of the Ancients
	pcRefugeOfTheAncients->SetFileName( "Ruin\\ruin-4.ase", "ruin-4" );
	pcRefugeOfTheAncients->SetType( MAPTYPE_Wasteland );
	pcRefugeOfTheAncients->SetSky( 7, 11, 9 );
	pcRefugeOfTheAncients->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcRefugeOfTheAncients->SetLevelRequirement( MAPLEVEL( MAPID_RefugeOfTheAncients ) );

	pcRefugeOfTheAncients->SetCenter( 1384, -918 );

	pcRefugeOfTheAncients->AddSpawn( 2578, -5124 );
	pcRefugeOfTheAncients->AddSpawn( 1873, 3060 );

	pcRefugeOfTheAncients->AddBorder( pcCastleOfTheLost, 410, 4902 );

	//Castle of the Lost
	pcCastleOfTheLost->SetFileName( "Ruin\\ruin-3.ase", "ruin-3" );
	pcCastleOfTheLost->SetType( MAPTYPE_Wasteland );
	pcCastleOfTheLost->SetSky( 7, 11, 9 );
	pcCastleOfTheLost->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcCastleOfTheLost->SetLevelRequirement( MAPLEVEL( MAPID_CastleOfTheLost ) );

	pcCastleOfTheLost->SetCenter( 4625, 6701 );

	pcCastleOfTheLost->AddSpawn( 875, 7910 );
	pcCastleOfTheLost->AddSpawn( 1576, 14307 );

	pcCastleOfTheLost->AddBorder( pcRuinenVillage, 3051, 16124 );

	//Ruinen Village
	pcRuinenVillage->SetFileName( "Ruin\\ruin-2.ase", "ruin-2" );
	pcRuinenVillage->SetType( MAPTYPE_Wasteland );
	pcRuinenVillage->SetSky( 8, 12, 10 );
	pcRuinenVillage->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcRuinenVillage->SetLevelRequirement( MAPLEVEL( MAPID_RuinenVillage ) );

	pcRuinenVillage->SetCenter( 7459, 23984 );

	pcRuinenVillage->AddSpawn( 3976, 19645 );
	pcRuinenVillage->AddSpawn( 5832, 23751 );
	pcRuinenVillage->AddSpawn( 5615, 25117 );

	pcRuinenVillage->AddTeleport1( 4428, 22511, 845, 64, 32, pcRuinenVillage, 4547, 22616, 817, 0, TELEPORTTYPE_WarpGate );
	pcRuinenVillage->AddBorder( pcCursedLand, 10019, 18031 );
	pcRuinenVillage->AddBorder( pcForestOfSpirits, 4470, 27774 );
	pcRuinenVillage->AddBorder( pcGreedyLake, 12713, 23409 );

	pcRuinenVillage->AddStaticModel( "Ruin\\ruin_ani01.ASE" );

	//Cursed Land
	pcCursedLand->SetFileName( "Ruin\\ruin-1.ase", "ruin-1" );
	pcCursedLand->SetType( MAPTYPE_Wasteland );
	pcCursedLand->SetSky( 8, 12, 10 );
	pcCursedLand->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcCursedLand->SetLevelRequirement( MAPLEVEL( MAPID_CursedLand ) );

	pcCursedLand->SetCenter( 16362, 14959 );

	pcCursedLand->AddSpawn( 12242, 16034 );
	pcCursedLand->AddSpawn( 12194, 8969 );

	pcCursedLand->AddTeleport1( 16809, 15407, 501, 128, 32, pcAncientPrisonF1, -15395, -24185, 0, MAPLEVEL( MAPID_AncientPrisonF1 ) );
	pcCursedLand->AddBorder( pcForgottenLand, 13319, 7102 );

	//Forgotten Land
	pcForgottenLand->SetFileName( "desert\\De-1.ase", "De-1" );
	pcForgottenLand->SetType( MAPTYPE_Desert );
	pcForgottenLand->SetSky( 4, 5, 6 );
	pcForgottenLand->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcForgottenLand->SetLevelRequirement( MAPLEVEL( MAPID_ForgottenLand ) );

	pcForgottenLand->SetCenter( 15005, -1421 );

	pcForgottenLand->AddSpawn( 13306, 2000 );
	pcForgottenLand->AddSpawn( 13083, -2249 );

	pcForgottenLand->AddBorder( pcOasis, 13466, -5953 );
	pcForgottenLand->AddBorder( pcNaviskoTown, 20041, -892 );

	//Navisko Town
	pcNaviskoTown->SetFileName( "forest\\village-1.ase", "village-1" );
	pcNaviskoTown->SetType( MAPTYPE_Wasteland );
	pcNaviskoTown->SetSky( 4, 5, 6 );
	pcNaviskoTown->SetBackgroundMusicID( BACKGROUNDMUSICID_Navisko );
	pcNaviskoTown->SetLevelRequirement( MAPLEVEL( MAPID_NaviskoTown ) );

	pcNaviskoTown->SetCenter( 21928, -2335 );

	pcNaviskoTown->AddSpawn( 21928, -2335 );

	pcNaviskoTown->AddTeleport1( 21936, -1833, 855, 64, 32, pcNaviskoTown, 21826, -1712, 825, 0, TELEPORTTYPE_WarpGate );
	pcNaviskoTown->AddBorder( pcBattlefieldOfTheAncients, 27110, -479, 0 );
	pcNaviskoTown->AddBorder( pcBellatra, 21840, 1062, 0 );

	pcNaviskoTown->AddStaticModel( "forest\\v2-ani01.ASE" );

	//Oasis
	pcOasis->SetFileName( "desert\\De-2.ase", "De-2" );
	pcOasis->SetType( MAPTYPE_Desert );
	pcOasis->SetSky( 4, 5, 6 );
	pcOasis->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcOasis->SetLevelRequirement( MAPLEVEL( MAPID_Oasis ) );

	pcOasis->SetCenter( 15887, -11161 );

	pcOasis->AddSpawn( 11859, -11257 );
	pcOasis->AddSpawn( 16169, -12768 );

	//Battlefield of the Ancients
	pcBattlefieldOfTheAncients->SetFileName( "desert\\De-3.ase", "De-3" );
	pcBattlefieldOfTheAncients->SetType( MAPTYPE_Desert );
	pcBattlefieldOfTheAncients->SetSky( 4, 5, 6 );
	pcBattlefieldOfTheAncients->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcBattlefieldOfTheAncients->SetLevelRequirement( MAPLEVEL( MAPID_AncientsBattlefield ) );
	
	pcBattlefieldOfTheAncients->SetCenter( 34758, -1323 );

	pcBattlefieldOfTheAncients->AddSpawn( 34758, -1323 );
	pcBattlefieldOfTheAncients->AddSpawn( 29424, 322 );

	pcBattlefieldOfTheAncients->AddTeleport1( 36128, -2162, 804, 64, 32, pcCursedTempleF1, -11108, -41681, 343, MAPLEVEL( MAPID_CursedTempleF1 ) );
	pcBattlefieldOfTheAncients->AddTeleport1( 29632, 2486, 827, 64, 32, pcSlab, -12012, -62738, 173, MAPLEVEL( MAPID_SecretLaboratory ) );
	pcBattlefieldOfTheAncients->AddBorder( pcForbiddenLand, 34372, 4277 );

	//Forbidden Land
	pcForbiddenLand->SetFileName( "desert\\De-4.ase", "De-4" );
	pcForbiddenLand->SetType( MAPTYPE_Desert );
	pcForbiddenLand->SetSky( 4, 5, 6 );
	pcForbiddenLand->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcForbiddenLand->SetLevelRequirement( MAPLEVEL( MAPID_ForbiddenLand ) );

	pcForbiddenLand->SetCenter( 43988, 12053 );

	pcForbiddenLand->AddSpawn( 33740, 6491 );
	pcForbiddenLand->AddSpawn( 40691, 11056 );

	pcForbiddenLand->AddTeleport1( 33979, 6080, 969, 64, 32, pcForbiddenLand, 34100, 6214, 940, MAPLEVEL( MAPID_ForbiddenLand ), TELEPORTTYPE_WarpGate );
	pcForbiddenLand->AddBorder( pcRailwayOfChaos, 44545, 13063 );

	pcForbiddenLand->AddStaticModel( "desert\\d4-ani01.ase" );

	//Ancient Prison F1
	pcAncientPrisonF1->SetFileName( "dungeon\\dun-1.ase", "dun-1" );
	pcAncientPrisonF1->SetType( MAPTYPE_Dungeon );
	pcAncientPrisonF1->SetSky( 0, 0, 0 );
	pcAncientPrisonF1->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcAncientPrisonF1->SetLevelRequirement( MAPLEVEL( MAPID_AncientPrisonF1 ) );

	pcAncientPrisonF1->SetCenter( -15384, -24310 );

	pcAncientPrisonF1->AddTeleport1( -15388, -24073, 100, 64, 32, pcCursedLand, 16649, 15238, 501, MAPLEVEL( MAPID_CursedLand ) );
	pcAncientPrisonF1->AddTeleport1( -15305, -28824, 1, 64, 32, pcAncientPrisonF2, -6027, -26881, 99, MAPLEVEL( MAPID_AncientPrisonF2 ) );

	pcAncientPrisonF1->AddSound( -15385, 100, -24949, 128, 0 );

	pcAncientPrisonF1->AddStaticModel( "dungeon\\dun-1-wheel.ase" );

	//Ancient Prison F2
	pcAncientPrisonF2->SetFileName( "dungeon\\dun-2.ase", "dun-2" );
	pcAncientPrisonF2->SetType( MAPTYPE_Dungeon );
	pcAncientPrisonF2->SetSky( 0, 0, 0 );
	pcAncientPrisonF2->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcAncientPrisonF2->SetLevelRequirement( MAPLEVEL( MAPID_AncientPrisonF2 ) );

	pcAncientPrisonF2->SetCenter( -6108, -26880 );

	pcAncientPrisonF2->AddTeleport1( -5908, -26878, 136, 64, 32, pcAncientPrisonF1, -15314, -28718, 58, MAPLEVEL( MAPID_AncientPrisonF1 ) );
	pcAncientPrisonF2->AddTeleport1( -8019, -25768, 21, 64, 32, pcAncientPrisonF3, 1810, -28802, 0, MAPLEVEL( MAPID_AncientPrisonF3 ) );
	pcAncientPrisonF2->AddTeleport1( -3918, -27988, 21, 64, 32, pcAncientPrisonF3, 1810, -28802, 0, MAPLEVEL( MAPID_AncientPrisonF3 ) );

	//Ancient Prison F3
	pcAncientPrisonF3->SetFileName( "dungeon\\dun-3.ase", "dun-3" );
	pcAncientPrisonF3->SetType( MAPTYPE_Dungeon );
	pcAncientPrisonF3->SetSky( 0, 0, 0 );
	pcAncientPrisonF3->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcAncientPrisonF3->SetLevelRequirement( MAPLEVEL( MAPID_AncientPrisonF3 ) );

	pcAncientPrisonF3->SetCenter( 1827, -28586 );

	pcAncientPrisonF3->AddTeleport2( 1810, -28924, 0, 64, 32, pcAncientPrisonF2, -8176, -25775, 77, -3738, -27990, 73, MAPLEVEL( MAPID_AncientPrisonF2 ) );

	//Chess Room
	pcChessRoom->SetFileName( "room\\office.ase", NULL );
	pcChessRoom->SetType( MAPTYPE_ChessRoom );
	pcChessRoom->SetSky( 0, 0, 0 );
	pcChessRoom->SetBackgroundMusicID( BACKGROUNDMUSICID_Forest );
	pcChessRoom->SetLevelRequirement( MAPLEVEL( MAPID_ChessRoom ) );

	pcChessRoom->SetCenter( -200000, 200000 );

	//Forest of Spirits
	pcForestOfSpirits->SetFileName( "forever-fall\\forever-fall-04.ase", "forever-fall-04" );
	pcForestOfSpirits->SetType( MAPTYPE_Forest );
	pcForestOfSpirits->SetSky( 18, 19, 17 );
	pcForestOfSpirits->SetBackgroundMusicID( BACKGROUNDMUSICID_Forest );
	pcForestOfSpirits->SetLevelRequirement( MAPLEVEL( MAPID_ForestOfSpirits ) );

	pcForestOfSpirits->SetCenter( -2912, 39215 );

	pcForestOfSpirits->AddSpawn( -2135, 33668 );
	pcForestOfSpirits->AddSpawn( 4073, 32119 );

	pcForestOfSpirits->AddBorder( pcLandOfDusk, -2949, 40442 );

	pcForestOfSpirits->AddSound( -1448, 827, 34188, 160, 7 );
	pcForestOfSpirits->AddSound( 618, 531, 35981, 160, 7 );
	pcForestOfSpirits->AddSound( 2282, 537, 32355, 160, 20 );
	pcForestOfSpirits->AddSound( 2597, 793, 30745, 0, 20 );
	pcForestOfSpirits->AddSound( 2562, 641, 32726, 0, 20 );
	pcForestOfSpirits->AddSound( 3789, 770, 30062, 0, 20 );
	pcForestOfSpirits->AddSound( 3698, 909, 34179, 60, 14 );

	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-01.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-02.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-03.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-04.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-05.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-06.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-07.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-08.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-09.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-10.ase" );
	pcForestOfSpirits->AddStaticModel( "forever-fall\\4ani-11.ase" );

	//Land of Dusk
	pcLandOfDusk->SetFileName( "forever-fall\\forever-fall-03.ase", "forever-fall-03" );
	pcLandOfDusk->SetType( MAPTYPE_Forest );
	pcLandOfDusk->SetSky( 18, 19, 17 );
	pcLandOfDusk->SetBackgroundMusicID( BACKGROUNDMUSICID_Forest );
	pcLandOfDusk->SetLevelRequirement( MAPLEVEL( MAPID_LandOfDusk ) );

	pcLandOfDusk->SetCenter( -5823, 43244 );

	pcLandOfDusk->AddSpawn( -1516, 45437 );
	pcLandOfDusk->AddSpawn( -3586, 42886 );

	pcLandOfDusk->AddTeleport1( -4730, 48107, 1173, 64, 32, pcLandOfDusk, -4615, 48002, 1146, MAPLEVEL( MAPID_LandOfDusk ), TELEPORTTYPE_WarpGate );
	pcLandOfDusk->AddTeleport1( -6306, 43241, 779, 64, 32, pcBeehiveCave, 119025, 35680, 499, MAPLEVEL( MAPID_BeehiveCave ) );
	pcLandOfDusk->AddBorder( pcValleyOfTranquility, -2349, 49830 );

	pcLandOfDusk->AddSound( -3573, 861, 42603, 0, 15 );

	pcLandOfDusk->AddStaticModel( "forever-fall\\3ani-01.ase" );
	pcLandOfDusk->AddStaticModel( "forever-fall\\3ani-02.ase" );
	pcLandOfDusk->AddStaticModel( "forever-fall\\3ani-03.ase" );
	pcLandOfDusk->AddStaticModel( "forever-fall\\3ani-04.ase" );
	pcLandOfDusk->AddStaticModel( "forever-fall\\3ani-05.ase" );
	pcLandOfDusk->AddStaticModel( "forever-fall\\3ani-06.ase" );
	pcLandOfDusk->AddStaticModel( "forever-fall\\3ani-07.ase" );

	//Valley of Tranquility
	pcValleyOfTranquility->SetFileName( "forever-fall\\forever-fall-02.ase", "forever-fall-02" );
	pcValleyOfTranquility->SetType( MAPTYPE_Forest );
	pcValleyOfTranquility->SetSky( 18, 19, 17 );
	pcValleyOfTranquility->SetBackgroundMusicID( BACKGROUNDMUSICID_Forest );
	pcValleyOfTranquility->SetLevelRequirement( MAPLEVEL( MAPID_ValleyOfTranquility ) );

	pcValleyOfTranquility->SetCenter( 1691, 52599 );

	pcValleyOfTranquility->AddSpawn( -1024, 45437 );
	pcValleyOfTranquility->AddSpawn( -1543, 52333 );
	pcValleyOfTranquility->AddSpawn( -1310, 54945 );

	pcValleyOfTranquility->AddBorder( pcRoadToTheWind, 667, 59371 );

	pcValleyOfTranquility->AddSound( -0x010A, 0x035E, 0x00E273, 100, 13 );
	pcValleyOfTranquility->AddSound( -0x1396, 0x0275, 0x00DD7E, 100, 13 );
	pcValleyOfTranquility->AddSound(  0x0869, 0x0304, 0x00D148,  80, 13 );
	pcValleyOfTranquility->AddSound( -0x018B, 0x021E, 0x00DCC9,  80, 13 );
	pcValleyOfTranquility->AddSound( -0x0D01, 0x020A, 0x00DC95,  20, 15 );
	pcValleyOfTranquility->AddSound( -0x0027, 0x025C, 0x00D340,  10,  6 );

	pcValleyOfTranquility->AddStaticModel( "forever-fall\\2ani-01.ase" );
	pcValleyOfTranquility->AddStaticModel( "forever-fall\\2ani-02.ase" );
	pcValleyOfTranquility->AddStaticModel( "forever-fall\\2ani-03.ase" );
	pcValleyOfTranquility->AddStaticModel( "forever-fall\\2ani-04.ase" );
	pcValleyOfTranquility->AddStaticModel( "forever-fall\\2ani-05.ase" );

	//Road to the Wind
	pcRoadToTheWind->SetFileName( "forever-fall\\forever-fall-01.ase", "forever-fall-01" );
	pcRoadToTheWind->SetType( MAPTYPE_Forest );
	pcRoadToTheWind->SetSky( 18, 19, 17 );
	pcRoadToTheWind->SetBackgroundMusicID( BACKGROUNDMUSICID_Forest );
	pcRoadToTheWind->SetLevelRequirement( MAPLEVEL( MAPID_RoadToTheWind ) );

	pcRoadToTheWind->SetCenter( 1911, 70630 );

	pcRoadToTheWind->AddSpawn( 1875, 69871 );
	pcRoadToTheWind->AddSpawn( 2637, 60219 );

	pcRoadToTheWind->AddTeleport1( 1962, 71184, 559, 64, 32, pcPhillaiTown, 1993, 73134, 449, MAPLEVEL( MAPID_PhillaiTown ), TELEPORTTYPE_Warp );

	pcRoadToTheWind->AddSound( 0x0B21, 0x0241, 0x010702, 30, 18 );
	pcRoadToTheWind->AddSound( 0x013D, 0x028D, 0x00FDBD, 10, 19 );
	pcRoadToTheWind->AddSound( -0x48E, 0x030B, 0x00F724, 40, 18 );
	pcRoadToTheWind->AddSound( -0x0B5, 0x0360, 0x00F470, 10, 18 );
	pcRoadToTheWind->AddSound( 0x00BE, 0x02AE, 0x00F19A, 10, 18 );
	pcRoadToTheWind->AddSound( -0x42E, 0x029F, 0x00F079, 10, 18 );
	pcRoadToTheWind->AddSound( 0x0717, 0x0232, 0x00FF86, 100, 8 );
	pcRoadToTheWind->AddSound( 0x0708, 0x025B, 0x00EEDD, 100, 8 );

	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-01.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-02.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-03.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-04.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-05.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-06.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-07.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-08.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-09.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-10.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-11.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-12.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-13.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-14.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-15.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-16.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-17.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-18.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-19.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-20.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-20.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-21.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-22.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-23.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-24.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-25.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-26.ase" );
	pcRoadToTheWind->AddStaticModel( "forever-fall\\1ani-27.ase" );

	//Phillai Town
	pcPhillaiTown->SetFileName( "forever-fall\\pilai.ase", "pilai" );
	pcPhillaiTown->SetType( MAPTYPE_Town );
	pcPhillaiTown->SetSky( 18, 19, 17 );
	pcPhillaiTown->SetBackgroundMusicID( BACKGROUNDMUSICID_Phillai );
	pcPhillaiTown->SetLevelRequirement( MAPLEVEL( MAPID_PhillaiTown ) );

	pcPhillaiTown->SetCenter( 2981, 75486 );

	pcPhillaiTown->AddSpawn( 2287, 74131 );
	pcPhillaiTown->AddSpawn( 3547, 75500 );

	pcPhillaiTown->AddTeleport1( 2000, 72907, 474, 64, 32, pcRoadToTheWind, 1958, 70922, 536, MAPLEVEL( MAPID_RoadToTheWind ), TELEPORTTYPE_Warp );
	pcPhillaiTown->AddTeleport1( 2245, 78259, 745, 64, 32, pcPhillaiTown, 2252, 78041, 754, MAPLEVEL( MAPID_PhillaiTown ), TELEPORTTYPE_WarpGate );

	pcPhillaiTown->AddSound( 0x0DE5, 0x265, 0x126E6, 20, 9 );
	pcPhillaiTown->AddSound( 0x08D0, 0x2E9, 0x131A7, 50, 6 );
	pcPhillaiTown->AddSound( 0x0812, 0x1CF, 0x12036, 0, 5 );
	pcPhillaiTown->AddSound( 0x0BD8, 0x22A, 0x11FCA, 10, 4 );
	pcPhillaiTown->AddSound( 0x08C6, 0x22A, 0x12B98, 0, 14 );
	pcPhillaiTown->AddSound( 0x09E3, 0x220, 0x124CB, 30, 12 );
	pcPhillaiTown->AddSound( 0x07F2, 0x220, 0x124A9, 30, 12 );
	pcPhillaiTown->AddSound( 0x03A6, 0x21E, 0x1209E, 0, 17 );
	pcPhillaiTown->AddSound( 0x0F0B, 0x1EC, 0x1238F, 0, 17 );
	pcPhillaiTown->AddSound( 0x0CFE, 0x29A, 0x12A63, 0, 17 );

	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-01.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-02.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-03.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-04.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-05.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-06.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-07.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-08.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-09.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-10.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-11.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-12.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-13.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-14.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-15.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-16.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-17.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-18.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-19.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-20.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-21.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-22.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-23.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-24.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-25.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-26.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-27.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-28.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-29.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-30.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-31.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-32.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-33.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-34.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-35.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-36.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-37.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-38.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-39.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-40.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-41.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-42.ase" );
	pcPhillaiTown->AddStaticModel( "forever-fall\\piani-43.ase" );

	//Cursed Temple F1
	pcCursedTempleF1->SetFileName( "dungeon\\dun-4.ase", "dun-4" );
	pcCursedTempleF1->SetType( MAPTYPE_Dungeon );
	pcCursedTempleF1->SetSky( 0, 0, 0 );
	pcCursedTempleF1->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcCursedTempleF1->SetLevelRequirement( MAPLEVEL( MAPID_CursedTempleF1 ) );

	pcCursedTempleF1->SetCenter( -11108, -41681 );

	pcCursedTempleF1->AddTeleport1( -10860, -41666, 398, 64, 32, pcBattlefieldOfTheAncients, 35872, -2016, 804, MAPLEVEL( MAPID_AncientsBattlefield ) );
	pcCursedTempleF1->AddTeleport1( -12089, -40380, 95, 64, 32, pcCursedTempleF2, -3675, -36597, 732, MAPLEVEL( MAPID_CursedTempleF2 ) );

	pcCursedTempleF1->AddSound( -0x2A69, 0x0157, 0xFFFF5976, 10, 24 );
	pcCursedTempleF1->AddSound( -0x2C23, 0x02B1, 0xFFFF53D3, 10, 24 );
	pcCursedTempleF1->AddSound( -0x32C3, 0x02B1, 0xFFFF53B0, 10, 24 );
	pcCursedTempleF1->AddSound( -0x2DB2, 0x0157, 0xFFFF5718, 10, 24 );
	pcCursedTempleF1->AddSound( -0x34A0, 0x0157, 0xFFFF5A02, 10, 24 );
	pcCursedTempleF1->AddSound( -0x3487, 0x0157, 0xFFFF6044, 10, 24 );
	pcCursedTempleF1->AddSound( -0x26E4, 0x0157, 0xFFFF6143, 10, 24 );
	pcCursedTempleF1->AddSound( -0x26E5, 0x01BB, 0xFFFF5720, 10, 24 );
	pcCursedTempleF1->AddSound( -0x3721, 0x01BB, 0xFFFF5925, 10, 24 );
	pcCursedTempleF1->AddSound( -0x37A5, 0x0157, 0xFFFF6195, 10, 24 );
	pcCursedTempleF1->AddSound( -0x2F27, 0x0153, 0xFFFF6550, 10, 24 );
	pcCursedTempleF1->AddSound( -0x27FC, 0x01BB, 0xFFFF5408,  0, 23 );
	pcCursedTempleF1->AddSound( -0x315D, 0x0153, 0xFFFF68C4,  0, 23 );
	pcCursedTempleF1->AddSound( -0x2FC7, 0x005E, 0xFFFF5CFC, 10, 27 );

	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-02.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-03.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-06.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-07.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-08.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-09.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-14.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-15.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-16.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-17.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-18.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-19.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-20.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-21.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-22.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-23.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-24.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-25.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-26.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-27.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-28.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-29.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-30.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-31.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-32.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-33.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-34.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-35.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-36.ase" );
	pcCursedTempleF1->AddStaticModel( "dungeon\\dun-4-ani-37.ase" );

	//Cursed Temple F2
	pcCursedTempleF2->SetFileName( "dungeon\\dun-5.ase", "dun-5" );
	pcCursedTempleF2->SetType( MAPTYPE_Dungeon );
	pcCursedTempleF2->SetSky( 0, 0, 0 );
	pcCursedTempleF2->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcCursedTempleF2->SetLevelRequirement( MAPLEVEL( MAPID_CursedTempleF2 ) );

	pcCursedTempleF2->SetCenter( -3675, -36597 );

	pcCursedTempleF2->AddTeleport1( -3665, -36334, 762, 64, 32, pcCursedTempleF1, -12073, -40701,  95, MAPLEVEL( MAPID_CursedTempleF1 ) );
	pcCursedTempleF2->AddTeleport1( -4895, -37132, 762, 64, 32, pcCursedTempleF3,  -3660, -45242, 116, MAPLEVEL( MAPID_CursedTempleF3 ) );
	pcCursedTempleF2->AddTeleport1( -2441, -37137, 762, 64, 32, pcCursedTempleF3,  -3660, -50266,   3, MAPLEVEL( MAPID_CursedTempleF3 ) );

	pcCursedTempleF2->AddSound( -0x0E4C, 0x2DA, 0xFFFF6FCD, 40, 23 );
	pcCursedTempleF2->AddSound( -0x0876, 0x2DC, 0xFFFF72E3, 0, 24 );
	pcCursedTempleF2->AddSound( -0x141E, 0x2DC, 0xFFFF72E5, 0, 24 );
	pcCursedTempleF2->AddSound( -0x181F, 0x2FA, 0xFFFF5C86, 0, 24 );
	pcCursedTempleF2->AddSound( -0x04D7, 0x2FA, 0xFFFF5C86, 0, 24 );
	pcCursedTempleF2->AddSound( -0x04E3, 0x31A, 0xFFFF6AC1, 0, 24 );
	pcCursedTempleF2->AddSound( -0x0978, 0x2DC, 0xFFFF6AAB, 0, 7 );
	pcCursedTempleF2->AddSound( -0x0E1B, 0x375, 0xFFFF64B0, 0, 7 );
	pcCursedTempleF2->AddSound( -0x12BE, 0x2DC, 0xFFFF6ABC, 0, 7 );

	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-01.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-02.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-03.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-04.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-05.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-06.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-07.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-08.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-09.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-10.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-11.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-12.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-13.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-14.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-15.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-16.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-17.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-18.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-19.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-20.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-21.ASE" );
	pcCursedTempleF2->AddStaticModel( "dungeon\\dun-5-ani-22.ASE" );

	//Mushroom Cave
	pcMushroomCave->SetFileName( "cave\\Tcave.ase", "Tcave" );
	pcMushroomCave->SetType( MAPTYPE_Dungeon );
	pcMushroomCave->SetSky( 0, 0, 0 );
	pcMushroomCave->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcMushroomCave->SetLevelRequirement( MAPLEVEL( MAPID_MushroomCave ) );

	pcMushroomCave->SetCenter( 120126, 26064 );

	pcMushroomCave->AddSpawn( 119319, 26034 );
	pcMushroomCave->AddSpawn( 125606, 24541 );

	pcMushroomCave->AddTeleport1( 125581, 25086, 480, 80, 32, pcDarkSanctuary, 158627, 20504, 240, MAPLEVEL( MAPID_DarkSanctuary ) );
	pcMushroomCave->AddTeleport1( 118869, 26017, 538, 80, 32, pcAcasiaForest, -16490, -6930, 298, MAPLEVEL( MAPID_AcasiaForest ) );

	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani01.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani02.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani03.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani04.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani05.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani06.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani07.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani08.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani09.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani10.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani11.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani12.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani13.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani14.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani15.ase" );
	pcMushroomCave->AddStaticModel( "cave\\Tcave_ani16.ase" );

	//Beehive Cave
	pcBeehiveCave->SetFileName( "cave\\Mcave.ase", "Mcave" );
	pcBeehiveCave->SetType( MAPTYPE_Dungeon );
	pcBeehiveCave->SetSky( 0, 0, 0 );
	pcBeehiveCave->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcBeehiveCave->SetLevelRequirement( MAPLEVEL( MAPID_BeehiveCave ) );

	pcBeehiveCave->SetCenter( 119966, 35466 );

	pcBeehiveCave->AddSpawn( 119320, 35680 );
	pcBeehiveCave->AddSpawn( 124380, 33260 );

	pcBeehiveCave->AddTeleport1( 124391, 32913,  37, 80, 20, pcDarkSanctuary, 158543, 19557, 290, MAPLEVEL( MAPID_DarkSanctuary ) );
	pcBeehiveCave->AddTeleport1( 118808, 35686, 520, 80, 20, pcLandOfDusk,     -6056, 43245, 787, MAPLEVEL( MAPID_LandOfDusk ) );

	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani01.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani02.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani03.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani04.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani05.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani06.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani07.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani08.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani09.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani10.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani11.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani12.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani13.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani14.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani15.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani16.ase" );
	pcBeehiveCave->AddStaticModel( "cave\\Mcave_ani17.ase" );

	//Dark Sanctuary
	pcDarkSanctuary->SetFileName( "cave\\Dcave.ase", "Dcave" );
	pcDarkSanctuary->SetType( MAPTYPE_Dungeon );
	pcDarkSanctuary->SetSky( 0, 0, 0 );
	pcDarkSanctuary->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcDarkSanctuary->SetLevelRequirement( MAPLEVEL( MAPID_DarkSanctuary ) );

	pcDarkSanctuary->SetCenter( 158814, 20070 );

	pcDarkSanctuary->AddTeleport1( 158042, 19525, 330, 120, 32, pcBeehiveCave, 124396, 33291, 37, MAPLEVEL( MAPID_BeehiveCave ) );
	pcDarkSanctuary->AddTeleport1( 158023, 20453, 240, 120, 32, pcMushroomCave, 125566, 24825, 480, MAPLEVEL( MAPID_DarkSanctuary ) );

	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani01.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani02.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani03.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani04.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani05.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani06.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani07.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani08.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani09.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani10.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani11.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani12.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani13.ase" );
	pcDarkSanctuary->AddStaticModel( "cave\\Dcave_ani14.ase" );

	//Railway Of Chaos
	pcRailwayOfChaos->SetFileName( "Iron\\iron-1.ase", "iron-1" );
	pcRailwayOfChaos->SetType( MAPTYPE_Iron );
	pcRailwayOfChaos->SetSky( 22, 23, 20 );
	pcRailwayOfChaos->SetBackgroundMusicID( BACKGROUNDMUSICID_TRIGGERED );
	pcRailwayOfChaos->SetLevelRequirement( MAPLEVEL( MAPID_RailwayOfChaos ) );

	pcRailwayOfChaos->SetCenter( 47286, 20925 );

	pcRailwayOfChaos->AddSpawn( 46905, 13478 );
	pcRailwayOfChaos->AddSpawn( 47357, 20845 );
	pcRailwayOfChaos->AddSpawn( 46741, 21349 );

	pcRailwayOfChaos->AddBorder( pcHeartOfPerum, 45316, 21407 );

	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani01_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani02_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani03_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani04_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani05_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani06_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani07_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani08_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani09_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani10_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\i1-ani11_Bip.ase", TRUE );
	pcRailwayOfChaos->AddStaticModel( "iron\\iron-ani01.ase" );
	pcRailwayOfChaos->AddStaticModel( "iron\\iron-ani02.ase" );
	pcRailwayOfChaos->AddStaticModel( "iron\\iron-ani03.ase" );

	//Heart of Perum
	pcHeartOfPerum->SetFileName( "Iron\\iron-2.ase", "iron-2" );
	pcHeartOfPerum->SetType( MAPTYPE_Iron );
	pcHeartOfPerum->SetSky( 22, 23, 21 );
	pcHeartOfPerum->SetBackgroundMusicID( BACKGROUNDMUSICID_TRIGGERED );
	pcHeartOfPerum->SetLevelRequirement( MAPLEVEL( MAPID_HeartOfPerum ) );

	pcHeartOfPerum->SetCenter( 33938, 24229 );

	pcHeartOfPerum->AddSpawn( 35149, 24326 );
	pcHeartOfPerum->AddSpawn( 44893, 21380 );

	pcHeartOfPerum->AddBorder( pcEura, 33618, 24011 );

	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip01.ase" );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip02.ase" );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip03.ase" );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip04_ani.ase", TRUE );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip05_ani.ase", TRUE );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip06_ani.ase", TRUE );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip07_ani.ase", TRUE );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip08_ani.ase", TRUE );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip09.ase" );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip10.ase" );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip11_ani.ase", TRUE );
	pcHeartOfPerum->AddStaticModel( "iron\\i2-bip12_ani.ase", TRUE );

	//Eura
	pcEura->SetFileName( "Ice\\ice_ura.ase", "ice_ura" );
	pcEura->SetType( MAPTYPE_Ice );
	pcEura->SetSky( 35, 36, 37 );
	pcEura->SetBackgroundMusicID( BACKGROUNDMUSICID_Ice );
	pcEura->SetLevelRequirement( MAPLEVEL( MAPID_Eura ) );

	pcEura->SetCenter( 31090, 23781 );

	pcEura->AddTeleport1( 30407, 22232, 1349, 64, 32, pcEura, 30610, 22164, 1304, MAPLEVEL( MAPID_Eura ), TELEPORTTYPE_WarpGate );
	pcEura->AddBorder( pcGallubiaValley, 31848, 27225 );

	pcEura->AddStaticModel( "ice\\ice_ani_01.ase" );

	//Bellatra
	pcBellatra->SetFileName( "Sod\\sod-1.ase", "sod-1" );
	pcBellatra->SetType( MAPTYPE_Wasteland );
	pcBellatra->SetSky( 8, 12, 10 );
	pcBellatra->SetBackgroundMusicID( BACKGROUNDMUSICID_TRIGGERED );
	pcBellatra->SetLevelRequirement( MAPLEVEL( MAPID_Bellatra ) );

	pcBellatra->SetCenter( 22018, 6569 );

	pcBellatra->AddStaticModel( "sod\\s-1ani01_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani02_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani03_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani04_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani05_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani06_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani07_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani08_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani09_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani10_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani11_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani12_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani13_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani14_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani15_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani16_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani17_Bip.ase", TRUE );
	pcBellatra->AddStaticModel( "sod\\s-1ani18_Bip.ase", TRUE );

	//Gallubia Valley
	pcGallubiaValley->SetFileName( "Ice\\ice1.ase", "ice1" );
	pcGallubiaValley->SetType( MAPTYPE_Ice );
	pcGallubiaValley->SetSky( 35, 36, 37 );
	pcGallubiaValley->SetBackgroundMusicID( BACKGROUNDMUSICID_Ice );
	pcGallubiaValley->SetLevelRequirement( MAPLEVEL( MAPID_GallubiaValley ) );

	pcGallubiaValley->SetCenter( 32313, 28529 );
	pcGallubiaValley->AddTeleport1( 35433, 31892, 834, 64, 32, pcIceMineF1, 18039, -31696, 970, MAPLEVEL( MAPID_IceMineF1 ) );
	pcGallubiaValley->AddBorder( pcFrozenSanctuary, 34596, 39050 );

	pcGallubiaValley->AddStaticModel( "ice\\ice_ani_01.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leewood01.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leewood03.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leewood04.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leaf01.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leaf02.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leaf03.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leaf04.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leaf05.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leaf06.ase" );
	pcGallubiaValley->AddStaticModel( "ice\\ice_bip_leaf07.ase" );

	//Quest Arena
	pcQuestArena->SetFileName( "quest\\quest_IV.ase", "quest_IV" );
	pcQuestArena->SetType( MAPTYPE_Dungeon );
	pcQuestArena->SetSky( 0, 0, 0 );
	pcQuestArena->SetBackgroundMusicID( BACKGROUNDMUSICID_QuestArena );
	pcQuestArena->SetLevelRequirement( MAPLEVEL( MAPID_QuestArena ) );

	pcQuestArena->SetCenter( 22497, 9605 );

	//Bless Castle
	pcBlessCastle->SetFileName( "castle\\castle.ase", "castle" );
	pcBlessCastle->SetType( MAPTYPE_BlessCastle );
	pcBlessCastle->SetSky( 8, 12, 10 );
	pcBlessCastle->SetBackgroundMusicID( BACKGROUNDMUSICID_BlessCastle );
	pcBlessCastle->SetLevelRequirement( MAPLEVEL( MAPID_BlessCastle ) );

	pcBlessCastle->SetCenter( 35225, -23847 );

	pcBlessCastle->AddSpawn( 33920, -23479 );
	pcBlessCastle->AddSpawn( 35247, -23355 );
	pcBlessCastle->AddSpawn( 36164, -23446 );

	pcBlessCastle->AddTeleport1( 32527, -30693, 774, 64, 32, pcBlessCastle, 32739, -30474, 711, MAPLEVEL( MAPID_BlessCastle ), TELEPORTTYPE_WarpGate );

	pcBlessCastle->AddStaticModel( "castle\\wf_gate_ani.ase" );
	pcBlessCastle->AddStaticModel( "castle\\crystal_ani01.ase" );
	pcBlessCastle->AddStaticModel( "castle\\crystal_ani02.ase" );
	pcBlessCastle->AddStaticModel( "castle\\crystal_ani03.ase" );
	pcBlessCastle->AddStaticModel( "castle\\crystal_ani04.ase" );
	pcBlessCastle->AddStaticModel( "castle\\crystal_ani05.ase" );
	pcBlessCastle->AddStaticModel( "castle\\crystal_ani06.ase" );

	//Greedy Lake
	pcGreedyLake->SetFileName( "Greedy\\Greedy.ase", "Greedy" );
	pcGreedyLake->SetType( MAPTYPE_Ice );
	pcGreedyLake->SetSky( 38, 39, 40 );
	pcGreedyLake->SetBackgroundMusicID( BACKGROUNDMUSICID_Ice );
	pcGreedyLake->SetLevelRequirement( MAPLEVEL( MAPID_GreedyLake ) );

	pcGreedyLake->SetCenter( 13930, 23206 );

	pcGreedyLake->AddStaticModel( "Greedy\\vane1_ani.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\vane2_ani.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\vane3_ani.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\vane4_ani.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\ani_tail.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\ship_ani.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\ship1_ani.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\ship2_ani.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\ship3_ani.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi01.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi02.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi03.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi04.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi05.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi06.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi07.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi08.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi09.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_animi10.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani00.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani01.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani02.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani03.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani04.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani05.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani06.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani07.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani08.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\flower_ani09.ase" );
	pcGreedyLake->AddStaticModel( "Greedy\\door.ase" );

	//Frozen Sanctuary
	pcFrozenSanctuary->SetFileName( "Ice\\ice_2.ase", "ice2" );
	pcFrozenSanctuary->SetType( MAPTYPE_Ice );
	pcFrozenSanctuary->SetSky( 35, 36, 37 );
	pcFrozenSanctuary->SetBackgroundMusicID( BACKGROUNDMUSICID_Ice );
	pcFrozenSanctuary->SetLevelRequirement( MAPLEVEL( MAPID_FrozenSanctuary ) );

	pcFrozenSanctuary->SetCenter( 36264, 40182 );

	pcFrozenSanctuary->AddTeleport1( 37981, 50790, 1216, 64, 32, pcKelvezuLair, 33000, 50036, 1512, MAPLEVEL( MAPID_KelvezuLair ) );

	//Kelvezu Lair
	pcKelvezuLair->SetFileName( "Boss\\Boss.ase", "Boss" );
	pcKelvezuLair->SetType( MAPTYPE_Dungeon );
	pcKelvezuLair->SetSky( 0, 0, 0 );
	pcKelvezuLair->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcKelvezuLair->SetLevelRequirement( MAPLEVEL( MAPID_KelvezuLair ) );

	pcKelvezuLair->SetCenter( 33000, 50036 );

	pcKelvezuLair->AddTeleport1( 33012, 49926, 1533, 64, 32, pcFrozenSanctuary, 37971, 50460, 1209, MAPLEVEL( MAPID_FrozenSanctuary ) );

	//Land of Chaos
	pcLandOfChaos->SetFileName( "lost\\lostisland.ase", "lost" );
	pcLandOfChaos->SetType( MAPTYPE_Wasteland );
	pcLandOfChaos->SetSky( 41, 42, 43 );
	pcLandOfChaos->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcLandOfChaos->SetLevelRequirement( MAPLEVEL( MAPID_LandOfChaos ) );

	pcLandOfChaos->SetCenter( -12555, -1113 );

	pcLandOfChaos->AddTeleport1( -12610, -1349, 694, 64, 32, pcLandOfChaos, -12555, -1113, 668, MAPLEVEL( MAPID_LandOfChaos ), TELEPORTTYPE_WarpGate );
	pcLandOfChaos->AddTeleport1( -14206, -1793, 174, 64, 32, pcAbyssOfTheSea, -21949, -5917, 156, MAPLEVEL( MAPID_AbyssSea ) );
	pcLandOfChaos->AddBorder( pcLostTemple, -11586, 7704 );

	pcLandOfChaos->AddStaticModel( "lost\\wf_ani.ase" );
	pcLandOfChaos->AddStaticModel( "lost\\ani_ship01.ase" );
	pcLandOfChaos->AddStaticModel( "lost\\ani_ship02.ase" );
	pcLandOfChaos->AddStaticModel( "lost\\ani_ship03.ase" );
	pcLandOfChaos->AddStaticModel( "lost\\ani_ship04.ase" );

	//Lost Temple
	pcLostTemple->SetFileName( "Losttemple\\lost_temple.ase", "Losttemple" );
	pcLostTemple->SetType( MAPTYPE_Wasteland );
	pcLostTemple->SetSky( 44, 45, 46 );
	pcLostTemple->SetBackgroundMusicID( BACKGROUNDMUSICID_DesertB );
	pcLostTemple->SetLevelRequirement( MAPLEVEL( MAPID_LostTemple ) );

	pcLostTemple->SetCenter( -11718, 10794 );

	pcLostTemple->AddTeleport1( -12275, 11385, 533, 64, 32, pcEndlessTowerF1, 14251, -39089, 220, MAPLEVEL( MAPID_EndlessTowerF1 ) );


	//Ghost Castle
	pcGhostCastle->SetFileName( "Fall_Game\\fall_game.ase", NULL );
	pcGhostCastle->SetType( MAPTYPE_Distorted );
	pcGhostCastle->SetSky( 0, 0, 0 );
	pcGhostCastle->SetBackgroundMusicID( BACKGROUNDMUSICID_GhostCastle );
	pcGhostCastle->SetLevelRequirement( MAPLEVEL( MAPID_GhostCastle ) );

	pcGhostCastle->SetCenter( 5800, 36747 );
	pcGhostCastle->SetCamera( 0, 213140, 36421, 5583, 11483 );

	//Endless Tower F1
	pcEndlessTowerF1->SetFileName( "endless\\dun-7.ase", "dun-7" );
	pcEndlessTowerF1->SetType( MAPTYPE_Dungeon );
	pcEndlessTowerF1->SetSky( 0, 0, 0 );
	pcEndlessTowerF1->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcEndlessTowerF1->SetLevelRequirement( MAPLEVEL( MAPID_EndlessTowerF1 ) );

	pcEndlessTowerF1->SetCenter( 14255, -39099 );

	pcEndlessTowerF1->AddTeleport1( 14242, -40988, 259, 64, 32, pcEndlessTowerF2, 5255, -37897, 86, MAPLEVEL( MAPID_EndlessTowerF2 ) );
	pcEndlessTowerF1->AddTeleport1( 14246, -38912, 220, 64, 32, pcLostTemple, -12272, 11299, 509, MAPLEVEL( MAPID_LostTemple ) );

	//Endless Tower F2
	pcEndlessTowerF2->SetFileName( "endless\\dun-8.ase", "dun-8" );
	pcEndlessTowerF2->SetType( MAPTYPE_Dungeon );
	pcEndlessTowerF2->SetSky( 0, 0, 0 );
	pcEndlessTowerF2->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcEndlessTowerF2->SetLevelRequirement( MAPLEVEL( MAPID_EndlessTowerF2 ) );

	pcEndlessTowerF2->SetCenter( 5255, -37897 );

	pcEndlessTowerF2->AddTeleport1( 5253, -37708,  69, 64, 32, pcEndlessTowerF1, 14242, -41199, 220, MAPLEVEL( MAPID_EndlessTowerF1 ) );
	pcEndlessTowerF2->AddTeleport2( 5254, -41500, 139, 64, 32, pcEndlessTowerF3, 4896,  -42220, 202, 6408, -42220, 202, MAPLEVEL( MAPID_EndlessTowerF3 ) );


	//Mystery Forest 1
	pcMysteryForest1->SetFileName( "Custom\\fo1.ase", "fo1" );
	pcMysteryForest1->SetType( MAPTYPE_Forest );
	pcMysteryForest1->SetSky( 2, 3, 1 );
	pcMysteryForest1->SetBackgroundMusicID( BACKGROUNDMUSICID_MysteryForest );
	pcMysteryForest1->SetLevelRequirement( MAPLEVEL( MAPID_MysteryForest1 ) );

	pcMysteryForest1->SetCenter( -35220, -11024 );

	pcMysteryForest1->AddSpawn( -34758, -9555 );
	pcMysteryForest1->AddSpawn( -39212, -10106 );
	pcMysteryForest1->AddSpawn( -37514, -10880 );

	pcMysteryForest1->AddBorder( pcAtlantisTown, -37725, -14828 );
	pcMysteryForest1->AddBorder( pcMysteryDesert1, -35254, -7615 );
	
	//Mystery Forest 2
	pcMysteryForest2->SetFileName( "Custom\\fo2.ase", "fo2" );
	pcMysteryForest2->SetType( MAPTYPE_Forest );
	pcMysteryForest2->SetSky( 2, 3, 1 );
	pcMysteryForest2->SetBackgroundMusicID( BACKGROUNDMUSICID_MysteryForest );
	pcMysteryForest2->SetLevelRequirement( MAPLEVEL( MAPID_MysteryForest2 ) );


	pcMysteryForest2->SetCenter( -46733, -14391 );

	pcMysteryForest2->AddSpawn( -46644, -10485 );
	pcMysteryForest2->AddSpawn( -41291, -11138 );

	pcMysteryForest2->AddTeleport1( -44356, -10738, 433, 16, 16, pcMysteryForest1, -35275, -8095, 667, MAPLEVEL( MAPID_MysteryForest1 ), TELEPORTTYPE_Warp );
	pcMysteryForest2->AddBorder( pcMysteryForest1, -40292, -9548 );

	//Mystery Forest 3
	pcMysteryForest3->SetFileName( "Custom\\fo3.ase", "fo3" );
	pcMysteryForest3->SetType( MAPTYPE_Forest );
	pcMysteryForest3->SetSky( 2, 3, 1 );
	pcMysteryForest3->SetBackgroundMusicID( BACKGROUNDMUSICID_MysteryForest );
	pcMysteryForest3->SetLevelRequirement( MAPLEVEL( MAPID_MysteryForest3 ) );

	pcMysteryForest3->SetCenter( -57048, -6894 );

	pcMysteryForest3->AddSpawn( -50646, -6333 );
	pcMysteryForest3->AddSpawn( -55880, -9377 );

	pcMysteryForest3->AddBorder( pcMysteryForest2, -48508, -10576 );

	//Atlantis Town
	pcAtlantisTown->SetFileName( "Custom\\town1.ase", "town1" );
	pcAtlantisTown->SetType( MAPTYPE_Town );
	pcAtlantisTown->SetSky( 2, 3, 1 );
	pcAtlantisTown->SetBackgroundMusicID( BACKGROUNDMUSICID_Atlantis );

	pcAtlantisTown->SetCenter( -37725, -17198 );

	pcAtlantisTown->AddSpawn( -40254, -17511 );
	pcAtlantisTown->AddSpawn( -37050, -17456 );

	for( int i = 1; i <= 4; i++ )
		pcAtlantisTown->AddModelEx( "forest\\v-ani%02d.ASE", i );

	//Forgotten Temple F1
	pcForgottenTempleF1->SetFileName( "Custom\\sanc1.ase", "sanc1" );
	pcForgottenTempleF1->SetType( MAPTYPE_Dungeon );
	pcForgottenTempleF1->SetSky( 0, 0, 0 );
	pcForgottenTempleF1->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcForgottenTempleF1->SetLevelRequirement( MAPLEVEL( MAPID_ForgottenTempleF1 ) );

	pcForgottenTempleF1->SetCenter( 33825, -36598 );

	pcForgottenTempleF1->AddTeleport1( 33832, -36334, 762, 64, 32, pcMysteryDesert1, -37060, -4811, 1457, MAPLEVEL( MAPID_MysteryDesert3 ) );
	pcForgottenTempleF1->AddTeleport1( 32594, -37134, 762, 64, 32, pcForgottenTempleF2, 26338, -41668,  343, MAPLEVEL( MAPID_ForgottenTempleF2 ) );
	//pcForgottenTempleF1->AddTeleport1( 35050, -37134, 762, 64, 32, pcForgottenTempleF2, 26338, -41668,  343, MAPLEVEL( MAPID_ForgottenTempleF2 ) );
	//pcForgottenTempleF1->AddTeleport1( 33843, -44338, 148, 64, 32, pcForgottenTempleF2, 25408, -40665,   95, MAPLEVEL( MAPID_ForgottenTempleF2 ) );

	//Teleport Arena -> FT1
	pcForgottenTempleF1->AddTeleport1( 33843, -44338, 148, 64, 32, pcForgottenTempleF1, 33825, -36598, 714, MAPLEVEL( MAPID_ForgottenTempleF1 ) );

	//Teleport FT1 -> Arena
	pcForgottenTempleF1->AddTeleport1( 35050, -37134, 762, 64, 32, pcForgottenTempleF1, 33844, -44094, 148, MAPLEVEL( MAPID_ForgottenTempleF1 ) );

	for( int i = 1; i <= 36; i++ )
		pcForgottenTempleF1->AddModelEx( "dungeon\\for-1-ani-%02d.ASE", i );

	//Forgotten Temple F2
	pcForgottenTempleF2->SetFileName( "Custom\\sanc2.ase", "sanc2" );
	pcForgottenTempleF2->SetType( MAPTYPE_Dungeon );
	pcForgottenTempleF2->SetSky( 0, 0, 0 );
	pcForgottenTempleF2->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcForgottenTempleF2->SetLevelRequirement( MAPLEVEL( MAPID_ForgottenTempleF2 ) );

	pcForgottenTempleF2->SetCenter( 26338, -41668 );

	pcForgottenTempleF2->AddTeleport1( 26596, -41672, 384, 64, 32, pcForgottenTempleF1, 33825, -36598, 714, MAPLEVEL( MAPID_ForgottenTempleF1 ) );
	pcForgottenTempleF2->AddTeleport1( 25409, -40392,  95, 64, 64, pcForgottenTempleF1, 33844, -44094, 148, MAPLEVEL( MAPID_ForgottenTempleF1 ) );

	for( int i = 1; i <= 37; i++ )
		pcForgottenTempleF2->AddModelEx( "dungeon\\for-2-ani-%02d.ASE", i );

	//Battle Town
	pcBattleTown->SetFileName( "Custom\\ba1.ase", "ba1" );
	pcBattleTown->SetType( MAPTYPE_BattleTown );
	pcBattleTown->SetSky( 4, 5, 6 );
	pcBattleTown->SetBackgroundMusicID( BACKGROUNDMUSICID_Battle );
	pcBattleTown->SetLevelRequirement( MAPLEVEL( MAPID_BattleTown ) );

	pcBattleTown->SetCenter( -23793, 19419 );

	pcBattleTown->AddSpawn( -23793, 19419 );

	pcBattleTown->AddBorder( pcMysteryDesert3, -25680, 20862 );
	pcBattleTown->AddBorder( pcCursedDesert, -23734, 17254 );
	pcBattleTown->AddBorder( pcIronCore, -18613, 1259 );
	
	//Mystery Desert 3
	pcMysteryDesert3->SetFileName( "Custom\\ba2.ase", "ba2" );
	pcMysteryDesert3->SetType( MAPTYPE_Desert );
	pcMysteryDesert3->SetSky( 4, 5, 6 );
	pcMysteryDesert3->SetBackgroundMusicID( BACKGROUNDMUSICID_MysteryDesert );
	pcMysteryDesert3->SetLevelRequirement( MAPLEVEL( MAPID_MysteryDesert3 ) );
	
	pcMysteryDesert3->SetCenter( -26258, 20686 );

	pcMysteryDesert3->AddSpawn( -26258, 20686 );
	pcMysteryDesert3->AddSpawn( -29687, 21324 );

	pcMysteryDesert3->AddBorder( pcMysteryDesert2, -32203, 15803 );

	
	//Ancient Dungeon F1
	pcAncientDungeonF1->SetFileName( "Custom\\ad1.ase", "ad1" );
	pcAncientDungeonF1->SetType( MAPTYPE_Dungeon );
	pcAncientDungeonF1->SetSky( 0, 0, 0 );
	pcAncientDungeonF1->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcAncientDungeonF1->SetLevelRequirement( MAPLEVEL( MAPID_AncientDungeonF1 ) );

	pcAncientDungeonF1->SetCenter( 64616, -24310 );

	pcAncientDungeonF1->AddSpawn( 64616, -24310 );

	pcAncientDungeonF1->AddTeleport1( 64618, -24103, 100, 64, 32, pcMysteryDesert1,   -38268,    296, 1841, MAPLEVEL( MAPID_MysteryDesert3 ) );
	pcAncientDungeonF1->AddTeleport1( 64696, -28927,   5, 64, 32, pcAncientDungeonF2,  73892, -26880,   88, MAPLEVEL( MAPID_AncientDungeonF2 ) );

	//Ancient Dungeon F2
	pcAncientDungeonF2->SetFileName( "Custom\\ad2.ase", "ad2" );
	pcAncientDungeonF2->SetType( MAPTYPE_Dungeon );
	pcAncientDungeonF2->SetSky( 0, 0, 0 );
	pcAncientDungeonF2->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcAncientDungeonF2->SetLevelRequirement( MAPLEVEL( MAPID_AncientDungeonF2 ) );

	pcAncientDungeonF2->SetCenter( 73892, -26880 );

	pcAncientDungeonF2->AddSpawn( 73892, -26880 );
	
	pcAncientDungeonF2->AddTeleport1( 74075, -26881, 136, 64, 32, pcAncientDungeonF1, 64692, -28661,  63, MAPLEVEL( MAPID_AncientDungeonF1 ) );
	pcAncientDungeonF2->AddTeleport1( 71980, -25766,  33, 64, 32, pcAncientDungeonF3, 81812, -28789, 268, MAPLEVEL( MAPID_AncientDungeonF3 ) );
	pcAncientDungeonF2->AddTeleport1( 76105, -28000,  33, 64, 32, pcAncientDungeonF3, 81812, -28789, 268, MAPLEVEL( MAPID_AncientDungeonF3 ) );

	//Ancient Dungeon F3
	pcAncientDungeonF3->SetFileName( "Custom\\ad3.ase", "ad3" );
	pcAncientDungeonF3->SetType( MAPTYPE_Dungeon );
	pcAncientDungeonF3->SetSky( 0, 0, 0 );
	pcAncientDungeonF3->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );

	pcAncientDungeonF3->SetCenter( 81812, -28789 );

	pcAncientDungeonF3->AddSpawn( 81812, -28789 );
	
	pcAncientDungeonF3->AddTeleport2( 81809, -28933, 268, 64, 32, pcAncientDungeonF2, 71770, -25769, 90, 76316, -27998, 90, MAPLEVEL( MAPID_AncientDungeonF2 ) );
	

	//Mystery Desert 2
	pcMysteryDesert2->SetFileName( "Custom\\ba3.ase", "ba3" );
	pcMysteryDesert2->SetType( MAPTYPE_Desert );
	pcMysteryDesert2->SetSky( 4, 5, 6 );
	pcMysteryDesert2->SetBackgroundMusicID( BACKGROUNDMUSICID_MysteryDesert );
	pcMysteryDesert2->SetLevelRequirement( MAPLEVEL( MAPID_MysteryDesert2 ) );
	
	pcMysteryDesert2->SetCenter( -29541, 8994 );

	pcMysteryDesert2->AddSpawn( -32435, 13515 );
	pcMysteryDesert2->AddSpawn( -33661,  8804 );

	pcMysteryDesert2->AddBorder( pcMysteryDesert1, -32225, 4508 );

	//Cursed Temple F3
	pcCursedTempleF3->SetFileName( "dungeon\\Dun-6a.ase", "dun-6" );
	pcCursedTempleF3->SetType( MAPTYPE_Dungeon );
	pcCursedTempleF3->SetSky( 0, 0, 0 );
	pcCursedTempleF3->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcCursedTempleF3->SetLevelRequirement( MAPLEVEL(MAPID_CursedTempleF3) );

	pcCursedTempleF3->SetCenter( -3660, -45200 );

	pcCursedTempleF3->AddTeleport1( -3655, -50603,   3, 64, 32, pcCursedTempleF2, -2700, -37290, 732, MAPLEVEL( MAPID_CursedTempleF2 ) );
	pcCursedTempleF3->AddTeleport1( -3664, -44886, 145, 64, 32, pcCursedTempleF2, -4600, -37290, 732, MAPLEVEL( MAPID_CursedTempleF2 ) );

	pcCursedTempleF3->AddSound( -3648,   3, -50013, 0, 7 );
	pcCursedTempleF3->AddSound( -4349, 454, -47777, 0, 7 );
	pcCursedTempleF3->AddSound( -5130, 454, -47587, 0, 7 );
	pcCursedTempleF3->AddSound( -2183, 172, -47864, 0, 7 );
	pcCursedTempleF3->AddSound( -1785, 167, -48077, 0, 7 );
	pcCursedTempleF3->AddSound( -3725, 116, -46758, 0, 7 );
	pcCursedTempleF3->AddSound( -3632,   3, -47999, 0, 7 );
	pcCursedTempleF3->AddSound(  -627, 229, -45987, 0, 7 );
	pcCursedTempleF3->AddSound( -1896, 116, -45010, 0, 7 );
	pcCursedTempleF3->AddSound( -5077, 116, -45555, 0, 7 );
	pcCursedTempleF3->AddSound( -5289, 116, -46422, 0, 7 );

	for( int i = 1; i <= 19; i++ )
		pcCursedTempleF3->AddModelEx( "dungeon\\Dun-6a-ani-%03d.ase", i );

	//Mystery Desert 1
	pcMysteryDesert1->SetFileName( "Custom\\ba4.ase", "ba4" );
	pcMysteryDesert1->SetType( MAPTYPE_Desert );
	pcMysteryDesert1->SetSky( 4, 5, 6 );
	pcMysteryDesert1->SetBackgroundMusicID( BACKGROUNDMUSICID_MysteryDesert );
	pcMysteryDesert1->SetLevelRequirement( MAPLEVEL( MAPID_MysteryDesert1 ) );
	
	pcMysteryDesert1->SetCenter( -34798, -1959 );

	pcMysteryDesert1->AddSpawn( -34798, -1959 );
	pcMysteryDesert1->AddSpawn( -35393, -2871 );

	pcMysteryDesert1->AddBorder( pcMysteryForest1, -352023, -6464 );

	pcMysteryDesert1->AddTeleport1( -36252,   -95, 1525,  16, 16, pcMysteryDesert1, -33326, 4090, 1129, 100, TELEPORTTYPE_Warp );
	pcMysteryDesert1->AddTeleport1( -37476, -4822, 1456,  64, 32, pcForgottenTempleF1, 33825, -36598, 0, MAPLEVEL( MAPID_ForgottenTempleF1 ) );
	pcMysteryDesert1->AddTeleport1( -38715,   298, 1844, 128, 32, pcAncientDungeonF1, 64614, -24292, 100, MAPLEVEL( MAPID_AncientDungeonF1 ) );
	pcMysteryDesert1->AddTeleport1( -33237, 4230, 1122, 64, 32, pcMysteryDesert1, -32410, 4032, 1106, 100, TELEPORTTYPE_Warp );

	//Endless Tower F3
	pcEndlessTowerF3->SetFileName( "endless\\dun-9.ase", "dun-9" );
	pcEndlessTowerF3->SetType( MAPTYPE_Dungeon );
	pcEndlessTowerF3->SetSky( 0, 0, 0 );
	pcEndlessTowerF3->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcEndlessTowerF3->SetLevelRequirement( MAPLEVEL( MAPID_EndlessTowerF3 ) );

	pcEndlessTowerF3->SetCenter( 4896, -42330 );

	pcEndlessTowerF3->AddSpawn( 4896, -42330 );
	pcEndlessTowerF3->AddSpawn( 6418, -42330 );

	pcEndlessTowerF3->AddTeleport1( 4896, -42090, 230, 64, 32, pcEndlessTowerF2, 5254, -41361, 139, MAPLEVEL( MAPID_EndlessTowerF2 ) );
	pcEndlessTowerF3->AddTeleport1( 6408, -42090, 230, 64, 32, pcEndlessTowerF2, 5254, -41361, 139, MAPLEVEL( MAPID_EndlessTowerF2 ) );
	
	//Ancient Weapon
	pcAncientWeapon->SetFileName( "ancientW\\ancientW.ASE", "AncientW" );
	pcAncientWeapon->SetType( MAPTYPE_Dungeon );
	pcAncientWeapon->SetSky( 0, 0, 0 );
	pcAncientWeapon->SetBackgroundMusicID( BACKGROUNDMUSICID_AncientWeapon );
	pcAncientWeapon->SetLevelRequirement( MAPLEVEL( MAPID_AncientWeapon ) );

	pcAncientWeapon->SetCenter( 12795, -73112 );
	
	pcAncientWeapon->AddSpawn( 12795, -73112 );
	pcAncientWeapon->AddSpawn( 10117, -65417 );
	pcAncientWeapon->AddSpawn( 12761, -59569 );
	
	pcAncientWeapon->AddTeleport1( 12794, -74262, 458, 64, 32, pcSlab, -11999, -54412, 370, MAPLEVEL( MAPID_SecretLaboratory ) );
	pcAncientWeapon->AddTeleport1( 12792, -62235, 530, 64, 32, pcAncientWeapon, 12763, -59593, 562, MAPLEVEL( MAPID_AncientWeapon ) );
	pcAncientWeapon->AddTeleport1( 12784, -60124, 578, 64, 32, pcAncientWeapon, 12795, -63642, 458, MAPLEVEL( MAPID_AncientWeapon ) );

	for( int i = 1; i <= 10; i++ )
		pcAncientWeapon->AddModelEx( "ancientW\\RotObj%02d.ASE", i );

	//Ice Mine F1
	pcIceMineF1->SetFileName( "Mine\\mine-1.ase", "mine-1" );
	pcIceMineF1->SetType( MAPTYPE_Dungeon );
	pcIceMineF1->SetSky( 0, 0, 0 );
	pcIceMineF1->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcIceMineF1->SetLevelRequirement( MAPLEVEL( MAPID_IceMineF1 ) );

	pcIceMineF1->SetCenter( 18039, -31696 );

	pcIceMineF1->AddSpawn( 18039, -31696 );

	pcIceMineF1->AddTeleport1( 17862, -31551, 1018, 64, 32, pcGallubiaValley, 35225, 31965, 829, MAPLEVEL( MAPID_GallubiaValley ) );

	// Abyss of The Sea
	pcAbyssOfTheSea->SetFileName( "SeaA\\SeaA.ase", "SeaA" );
	pcAbyssOfTheSea->SetType( MAPTYPE_Dungeon );
	pcAbyssOfTheSea->SetSky( 0, 0, 0 );
	pcAbyssOfTheSea->SetBackgroundMusicID( BACKGROUNDMUSICID_Dungeon );
	pcAbyssOfTheSea->SetLevelRequirement( MAPLEVEL( MAPID_AbyssSea ) );

	//pcAbyssOfTheSea->SetCenter( -22534, 1246 );
	pcAbyssOfTheSea->SetCenter( -20047, -4017 );

	pcAbyssOfTheSea->AddTeleport1( -21963, -6137, 161, 64, 32, pcLandOfChaos, -13830, -1873, 187, MAPLEVEL( MAPID_LandOfChaos ) );

	for ( int i = 1; i <= 10; i++ )
		pcForgottenTempleF1->AddModelEx( "SeaA\\sea_ani%02d.ASE", i );

	//Tier 5 Quest Arena
	pcT5QuestArena->SetFileName( "Arena\\Arena.ase", "arena" );
	pcT5QuestArena->SetType( MAPTYPE_Dungeon );
	pcT5QuestArena->SetSky( 0, 0, 0 );
	pcT5QuestArena->SetBackgroundMusicID( BACKGROUNDMUSICID_FuryArena );
	pcT5QuestArena->SetCenter( 488537, 0 );

	//Slab
	pcSlab->SetFileName( "Slab\\Slab.ASE", "Slab" );
	pcSlab->SetType( MAPTYPE_Dungeon );
	pcSlab->SetLevelRequirement( MAPLEVEL( MAPID_SecretLaboratory ) );
	pcSlab->SetSky( 0, 0, 0 );
	pcSlab->SetBackgroundMusicID( BACKGROUNDMUSICID_SecretLaboratory );
	pcSlab->SetCenter( -11976, -62455 );
	pcSlab->AddTeleport1( -12007, -62945, 173, 64, 32, pcBattlefieldOfTheAncients, 29511, 2350, 826, MAPLEVEL( MAPID_AncientsBattlefield ) );
	pcSlab->AddTeleport1( -11999, -54172, 370, 64, 32, pcAncientWeapon, 12794, -74073, 458, MAPLEVEL( MAPID_AncientWeapon ) );

	//Distorted
	pcDistorted->SetFileName( "Distorted\\distorted.ase", NULL );
	pcDistorted->SetType( MAPTYPE_Distorted );
	pcDistorted->SetSky( 0, 0, 0 );
	pcDistorted->SetBackgroundMusicID( BACKGROUNDMUSICID_GhostCastle );
	pcDistorted->SetLevelRequirement( MAPLEVEL( MAPID_DistortedForest ) );
	pcDistorted->SetCenter( 5800, 38747 );
	pcDistorted->SetCamera( 0, 213140, 38421, 5583, 11483 );

	//Swamp
	pcSwamp->SetFileName( "Swamp\\swamp.ase", NULL );
	pcSwamp->SetType( MAPTYPE_Distorted );
	pcSwamp->SetSky( 0, 0, 0 );
	pcSwamp->SetBackgroundMusicID( BACKGROUNDMUSICID_GhostCastle );
	pcSwamp->SetLevelRequirement( MAPLEVEL( MAPID_Swamp ) );
	pcSwamp->SetCenter( 4550, 42820 );
	pcSwamp->SetCamera( 0, 44800, 42440, 4350, 8550 );

	//Old Ruinen 2
	pcOldRuinen2->SetFileName( "Ruin\\old_ruin-2.ase", "oldruin2" );
	pcOldRuinen2->SetType( MAPTYPE_Wasteland );
	pcOldRuinen2->SetSky( 41, 42, 43 );
	pcOldRuinen2->SetBackgroundMusicID( BACKGROUNDMUSICID_Desert );
	pcOldRuinen2->SetLevelRequirement( MAPLEVEL( MAPID_OldRuinen2 ) );
	pcOldRuinen2->SetCenter( 15000, 15000 );

	//Death Island
	pcDeathIsland->SetFileName( "Lost\\Lost3.ase", "lost3" );
	pcDeathIsland->SetType( MAPTYPE_Wasteland );
	pcDeathIsland->SetSky( 41, 42, 43 );
	pcDeathIsland->SetBackgroundMusicID( BACKGROUNDMUSICID_DeathIsland );
	pcDeathIsland->SetLevelRequirement( MAPLEVEL( MAPID_DeathIsland ) );

	pcDeathIsland->SetCenter( 115128, 88629 );

	pcDeathIsland->AddSpawn( 115128, 88629 );

	//Royal Desert
	pcRoyalDesert->SetFileName( "RoyalDesert\\RoyalDesert.ase", "rd1" );
	pcRoyalDesert->SetType( MAPTYPE_Desert );
	pcRoyalDesert->SetSky( 41, 42, 43 );
	pcRoyalDesert->SetBackgroundMusicID( BACKGROUNDMUSICID_Desert );
	pcRoyalDesert->SetLevelRequirement( MAPLEVEL( MAPID_RoyalDesert ) );
	pcRoyalDesert->SetCenter( -21335, 27996 );

	for ( int i = 1; i <= 1; i++ )
		pcRoyalDesert->AddModelEx( "RoyalDesert\\anim%02d.ASE", i );

	//Forest Dungeon
	pcForestDungeon->SetFileName( "TropicalIsland\\TropicalIsland.ase", "testmap" );
	pcForestDungeon->SetType( MAPTYPE_Desert );
	pcForestDungeon->SetSky( 8, 12, 10 );
	pcForestDungeon->SetBackgroundMusicID( BACKGROUNDMUSICID_TropicalIsland );
	pcForestDungeon->SetLevelRequirement( MAPLEVEL( MAPID_ForestDungeon ) );

	pcForestDungeon->SetCenter( -109346, 27588 );

	pcForestDungeon->AddSpawn( -109346, 27588 );

	//Dragons Dungeon
	pcDragonsDungeon->SetFileName( "DragonsDungeon\\DragonsDungeon.ase", "testmap" );
	pcDragonsDungeon->SetType( MAPTYPE_Dungeon );
	pcDragonsDungeon->SetSky( 0, 0, 0 );
	pcDragonsDungeon->SetBackgroundMusicID( BACKGROUNDMUSICID_TropicalIsland );
	pcDragonsDungeon->SetLevelRequirement( MAPLEVEL( MAPID_DragonsDungeon ) );

	pcDragonsDungeon->SetCenter( -199877, 199362 );

	pcDragonsDungeon->AddSpawn( -199877, 199362 );

	//Cursed Desert
	pcCursedDesert->SetFileName( "CursedDesert\\CursedDesert.ase", "cd1" );
	pcCursedDesert->SetType( MAPTYPE_Desert );
	pcCursedDesert->SetSky( 8, 12, 10 );
	pcCursedDesert->SetBackgroundMusicID( BACKGROUNDMUSICID_Desert );
	pcCursedDesert->SetLevelRequirement( MAPLEVEL( MAPID_CursedDesert ) );

	pcCursedDesert->SetCenter( -23895, 16209 );

	pcCursedDesert->AddSpawn( -23895, 16209 );

	pcCursedDesert->AddBorder( pcBattleTown, -23734, 17254 );

	//Iron Core
	pcIronCore->SetFileName( "Iron\\ironcore.ase", "iron3" );
	pcIronCore->SetType( MAPTYPE_Iron );
	pcIronCore->SetSky( 8, 12, 10 );
	pcIronCore->SetBackgroundMusicID( BACKGROUNDMUSICID_TRIGGERED );
	pcIronCore->SetLevelRequirement(MAPLEVEL( MAPID_IronCore ) );

	pcIronCore->SetCenter( -17958, 21165 );

	pcIronCore->AddSpawn( -17958, 21165 );

	pcIronCore->AddBorder( pcBattleTown, -18607, 21020);

	//Oasis Royale
	pcOasisRoyale->SetFileName( "desert\\De-5.ase", "De-2" );
	pcOasisRoyale->SetType( MAPTYPE_Desert );
	pcOasisRoyale->SetSky( 41, 42, 43 );
	pcOasisRoyale->SetBackgroundMusicID( BACKGROUNDMUSICID_Desert );
	pcOasisRoyale->SetLevelRequirement( MAPLEVEL( MAPID_OasisRoyale ) );

	pcOasisRoyale->SetCenter( 55626, -45747 );

	pcOasisRoyale->AddSpawn( 55626, -45747 );
}

void MapGame::SetMessageBoxMap( int iMap, BOOL bUsingCore )
{
	typedef void( __thiscall *tfnMessageBoxTitle ) (DWORD dwClass, int iID, char * pszTitle);
	tfnMessageBoxTitle MessageBoxTitle = (tfnMessageBoxTitle)0x004D1680;
	if ( bUsingCore )
	{
		iMap--;
		for ( int i = 0; i < 74; i++ )
		{
			if ( iMap == (*(int*)(0x04B0CF70 + (i * 12))) )
			{
				STRINGCOPYLEN( (char*)0x035D03C2, 32, pszaMapsName[(*(int*)(0x04B0CF74 + (i * 12)))] );
				MessageBoxTitle( 0x0362B210, 0x30, (char*)0x035D03C2 );
				break;
			}
		}
	}
	else
	{
		MessageBoxTitle( 0x0362B210, 52, (char*)pszaMapsName[iMap] );
		(*(DWORD*)0x035E0628) = 0;
		(*(DWORD*)0x035E062C) = iMap;
		(*(DWORD*)0x035E0588) = 0;
	}
}

void MapGame::UpdateBlockedPosition( UnitData * pcUnitData )
{
//	if ( GM_MODE == FALSE )
	{
		if ( UNITDATA == pcUnitData )
		{
			if ( MAP_ID == MAPID_AbyssSea )
			{
				if ( UNITDATA->sPosition.iZ > 967058 )
				{
					UNITDATA->sPosition.iZ = 967058;
				}
			}

			if ( MAP_ID == MAPID_BlessCastle )
			{
				if ( BLESSCASTLEHANDLER->GetSiegeWarMode() == SIEGEWARMODE_TheMassacre && BLESSCASTLEHANDLER->InSiegeWar() )
				{
					if ( UNITDATA->sPosition.iZ < -6419202 )
					{
						UNITDATA->sPosition.iZ = -6419202;
					}
				}
			}

			if ( ACTIONFIELDHANDLER->IsInActionFieldMap( MAP_ID ) )
			{
				if ( MAP_ID == MAPID_GhostCastle )
				{
					if ( UNITDATA->sPosition.iX > ACTIONFIELDHANDLER->GetXRange() )
						UNITDATA->sPosition.iX = ACTIONFIELDHANDLER->GetXRange();
				}
				else if ( MAP_ID == MAPID_ForestDungeon )
				{
					if ( ACTIONFIELDHANDLER->GetRound() == 1 )
					{
						if ( UNITDATA->sPosition.iX < (-111235 << 8) )
							UNITDATA->sPosition.iX = (-111235 << 8);
					}
					else if ( ACTIONFIELDHANDLER->GetRound() == 2 )
					{
						if ( UNITDATA->sPosition.iZ > (28834 << 8) )
							UNITDATA->sPosition.iZ = (28834 << 8);
					}
				}
			}
		}
	}
}

void MapGame::TeleportMapPosition( EMapID eMapID, int iX, int iZ )
{
    if ( UNITDATA->sCharacterData.iLevel >= MAPLEVEL( eMapID ) )
    {
        MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
        MESSAGEBOX->SetTitle( MAPNAME( eMapID ) );
        MESSAGEBOX->SetDescription( FormatString( "Do you want to teleport?" ) );
        MESSAGEBOX->SetEvent( std::bind( &MapGame::TeleportMapPositionHandle, MAPGAME, eMapID, iX, iZ ) );
        MESSAGEBOX->SetAutoClose( 10 );
        MESSAGEBOX->Show();
    }
    else
        TITLEBOX( "You must be level %d to teleport", MAPLEVEL( eMapID ) );
}

void MapGame::TeleportMapPositionHandle( EMapID eMapID, int iX, int iZ )
{
	if ( MESSAGEBOX->GetType() )
	{
		iX <<= 8;
		iZ <<= 8;

		TELEPORTPLAYER( eMapID );

		UNITDATA->SetPosition( iX, 0, iZ );

		CAMERAPOSITION->iX = UNITDATA->sPosition.iX;
		CAMERAPOSITION->iY = UNITDATA->sPosition.iY;
		CAMERAPOSITION->iZ = UNITDATA->sPosition.iZ;
		CAMERATARGETPOSITION->iX = UNITDATA->sPosition.iX;
		CAMERATARGETPOSITION->iY = UNITDATA->sPosition.iY;
		CAMERATARGETPOSITION->iZ = UNITDATA->sPosition.iZ;
	}
}
