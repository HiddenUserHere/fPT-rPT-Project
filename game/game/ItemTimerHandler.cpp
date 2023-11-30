#include "stdafx.h"
#include "ItemTimerHandler.h"

UI::Image * ItemTimer::pcImageBox;

int CItemTimerHandler::GetFreeSlot()
{
	for ( int i = 0; i < MAX_ITEMTIMER; i++ )
		if ( pcaItemTimer[i] == NULL )
			return i;

	return -1;
}

ItemTimer * CItemTimerHandler::GetItemTimer( int iType )
{
	for ( int i = 0; i < MAX_ITEMTIMER; i++ )
	{
		ItemTimer * j = pcaItemTimer[i];

		if ( (j) && ((j->iType == iType) || (ItemTimerIsSame( j->iType, iType ) == TRUE)) )
			return j;
	}

	return NULL;
}

BOOL CItemTimerHandler::CanStack( int iType )
{
	BOOL bRet = FALSE;

	switch ( iType )
	{
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixHeal:
		case ITEMTIMERTYPE_PhoenixLightning:
			bRet = TRUE;
			break;

		default:
			break;
	}

	return bRet;
}

int CItemTimerHandler::GetIndex( ItemTimer * pcTimer )
{
	if ( pcTimer != NULL )
	{
		for ( int i = 0; i < MAX_ITEMTIMER; i++ )
		{
			if ( pcaItemTimer[i] != NULL )
			{
				if ( pcaItemTimer[i] == pcTimer )
					return i;
			}
		}
	}
	return -1;
}

BOOL CItemTimerHandler::IsHead( int iType )
{
	BOOL bRet = FALSE;

	switch ( iType )
	{
		case ITEMTIMERTYPE_BigHead:
		case ITEMTIMERTYPE_BigHeadHappiness:
		case ITEMTIMERTYPE_BigHeadLove:
		case ITEMTIMERTYPE_BigHeadSadness:
		case ITEMTIMERTYPE_BigHeadShyness:
		case ITEMTIMERTYPE_BigHeadAngry:
		case ITEMTIMERTYPE_BigHeadSurprised:
		case ITEMTIMERTYPE_BigHeadSensual:
		case ITEMTIMERTYPE_CartolaHat:
		case ITEMTIMERTYPE_WitchHat:
		case ITEMTIMERTYPE_XMasGreenHat:
		case ITEMTIMERTYPE_XMasRedHat:
		case ITEMTIMERTYPE_SheepHat:
		case ITEMTIMERTYPE_GiraffeHat:
		case ITEMTIMERTYPE_SoccerHat:
		case ITEMTIMERTYPE_PumpkinHat:
		case ITEMTIMERTYPE_HalloweenHat:
		case ITEMTIMERTYPE_TuttiFruttiHat:
		case ITEMTIMERTYPE_EasterHat:
		case ITEMTIMERTYPE_ChemistHat:
		case ITEMTIMERTYPE_PatronumHat:
		case ITEMTIMERTYPE_GoldenHopyHat:
		case ITEMTIMERTYPE_ObscureHat:
			bRet = TRUE;
			break;

		default:
			break;
	}

	return bRet;
}

void CItemTimerHandler::ProcessCancel( ItemTimer * pcTimer )
{
	int iIndex = GetIndex( pcTimer );
	if ( iIndex >= 0 )
	{
		PacketCancelItemTimer s;
		s.iType = pcTimer->iType;
		s.iItemID = pcTimer->iItemID;
		ProcessPacket( &s );

		//Remove and Stop Timer
		RemoveTimer( iIndex );
	}
}

void CItemTimerHandler::OnClickCancelOK( ItemTimer * pcTimer, UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
		ProcessCancel( pcTimer );
}

void CItemTimerHandler::OnClickBeginnerRequest( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		PacketRequestBeginnerItemsTimer sPacket;
		sPacket.iLength = sizeof( PacketRequestBeginnerItemsTimer );
		sPacket.iHeader = PKTHDR_NPCBeginnerItemTimer;
		SENDPACKETG( &sPacket, TRUE );
	}
}

void CItemTimerHandler::OnClickChemistBuffRequest( UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		if ( auto pcTimer = GetHead() )
			ProcessCancel( pcTimer );

		PacketRequestChemistItemTimer sPacket;
		sPacket.iLength = sizeof( PacketRequestChemistItemTimer );
		sPacket.iHeader = PKTHDR_ChemistHatItemTimer;
		SENDPACKETG( &sPacket, TRUE );
	}
}

CItemTimerHandler::CItemTimerHandler()
{
	ZeroMemory( pcaItemTimerPrefab, sizeof( ItemTimer* ) * NUM_ITEMTIMERTYPE );
	ZeroMemory( pcaItemTimer, sizeof( ItemTimer* ) * MAX_ITEMTIMER );
}


CItemTimerHandler::~CItemTimerHandler()
{
	for ( int i = 0; i < NUM_ITEMTIMERTYPE; i++ )
		if ( pcaItemTimerPrefab[i] )
			DELET( pcaItemTimerPrefab[i] );
	
	for ( int i = 0; i < MAX_ITEMTIMER; i++ )
		if ( pcaItemTimer[i] )
			DELET( pcaItemTimer[i] );
}

void CItemTimerHandler::Init()
{
	ItemTimer * pcEXP = new ItemTimer( ITEMTIMERTYPE_ExpUp, ITEMID_None, "EXP Potion", "30% Additional Experience", 0, 0, NULL, NULL );
	ItemTimer * pcTHE = new ItemTimer( ITEMTIMERTYPE_ThirdEye, ITEMID_None, "Third Eye", "One Additional Drop", 0, 0, NULL, NULL );
	ItemTimer * pcPH1 = new ItemTimer( ITEMTIMERTYPE_PhoenixFire, ITEMID_None, "Phoenix Pet", "Fire Element", 0, 0, NULL, NULL );
	ItemTimer * pcPH2 = new ItemTimer( ITEMTIMERTYPE_PhoenixIce, ITEMID_None, "Phoenix Pet", "Ice Element", 0, 0, NULL, NULL );
	ItemTimer * pcPH3 = new ItemTimer( ITEMTIMERTYPE_PhoenixLightning, ITEMID_None, "Phoenix Pet", "Lightning Element", 0, 0, NULL, NULL );
	ItemTimer * pcPH4 = new ItemTimer( ITEMTIMERTYPE_PhoenixHeal, ITEMID_None, "Phoenix Pet", "Heal Element", 0, 0, NULL, NULL );
	ItemTimer * pcMPD = new ItemTimer( ITEMTIMERTYPE_MPDown, ITEMID_None, "Mana Down", "Reduces 25% Mana Usage", 0, 0, NULL, NULL );
	ItemTimer * pcSPD = new ItemTimer( ITEMTIMERTYPE_SPDown, ITEMID_None, "Stamina Down", "Reduces 25% Stamina Usage", 0, 0, NULL, NULL );
	ItemTimer * pcHPU = new ItemTimer( ITEMTIMERTYPE_HPUp, ITEMID_None, "Vampire Teeth", "+10 HP Regen", 0, 0, NULL, NULL );
	ItemTimer * pcMPU = new ItemTimer( ITEMTIMERTYPE_MPUp, ITEMID_None, "Mana Aura", "+10 MP Regen", 0, 0, NULL, NULL );
	ItemTimer * pcBHN = new ItemTimer( ITEMTIMERTYPE_BigHead, ITEMID_None, "Big Head", " ", 0, 0, NULL, NULL );
	ItemTimer * pcWGS = new ItemTimer( ITEMTIMERTYPE_WeightStone, ITEMID_None, "Weight Stone", "+1000 Additional Weight", 0, 0, NULL, NULL );
	ItemTimer * pcMTS = new ItemTimer( ITEMTIMERTYPE_MatureStone, ITEMID_None, "Mature Stone", " ", 0, 0, NULL, NULL );
	ItemTimer * pcSKS = new ItemTimer( ITEMTIMERTYPE_SkillStone, ITEMID_None, "Skill Stone", " ", 0, 0, NULL, NULL );
	ItemTimer * pcCLH = new ItemTimer( ITEMTIMERTYPE_CartolaHat, ITEMID_None, "Magician Hat", "+10 HP Regen +10 Add HP", 0, 0, NULL, NULL );
	ItemTimer * pcWTH = new ItemTimer( ITEMTIMERTYPE_WitchHat, ITEMID_None, "Witch Hat", "+10 HP Regen +10 Add HP", 0, 0, NULL, NULL );
	ItemTimer * pcHHT = new ItemTimer( ITEMTIMERTYPE_HalloweenHat, ITEMID_None, "Halloween Hat", "+10 HP Regen +10 Add HP", 0, 0, NULL, NULL );
	ItemTimer * pcSHH = new ItemTimer( ITEMTIMERTYPE_SheepHat, ITEMID_None, "Sheep Hat", "Cutie *-* +150 Defense", 0, 0, NULL, NULL );
	ItemTimer * pcGFH = new ItemTimer( ITEMTIMERTYPE_GiraffeHat, ITEMID_None, "Giraffe Hat", "+75 Attack Rating", 0, 0, NULL, NULL );
	ItemTimer * pcSCH = new ItemTimer( ITEMTIMERTYPE_SoccerHat, ITEMID_None, "Soccer Hat", "+2 Move Speed", 0, 0, NULL, NULL );
	ItemTimer * pcXMG = new ItemTimer( ITEMTIMERTYPE_XMasGreenHat, ITEMID_None, "XMas Green Hat", "+2 Move Speed", 0, 0, NULL, NULL );
	ItemTimer * pcXMR = new ItemTimer( ITEMTIMERTYPE_XMasRedHat, ITEMID_None, "XMas Red Hat", "+50 MP and STM", 0, 0, NULL, NULL );
	ItemTimer * pcBHH = new ItemTimer( ITEMTIMERTYPE_BigHeadHappiness, ITEMID_None, "Big Head of Happiness", "+50 Attack Rating", 0, 0, NULL, NULL );
	ItemTimer * pcBHL = new ItemTimer( ITEMTIMERTYPE_BigHeadLove, ITEMID_None, "Big Head of Love", "+10 HP Regen +10 HP", 0, 0, NULL, NULL );
	ItemTimer * pcBSD = new ItemTimer( ITEMTIMERTYPE_BigHeadSadness, ITEMID_None, "Big Head of Sadness", "+10 MP Regen +10 MP", 0, 0, NULL, NULL );
	ItemTimer * pcBSY = new ItemTimer( ITEMTIMERTYPE_BigHeadShyness, ITEMID_None, "Big Head of Shyness", "+100 Defense", 0, 0, NULL, NULL );
	ItemTimer * pcBHA = new ItemTimer( ITEMTIMERTYPE_BigHeadAngry, ITEMID_None, "Big Head of Angry", "+50 HP/MP/STM", 0, 0, NULL, NULL );
	ItemTimer * pcBSU = new ItemTimer( ITEMTIMERTYPE_BigHeadSurprised, ITEMID_None, "Big Head of Surprised", "+2% Block", 0, 0, NULL, NULL );
	ItemTimer * pcBSN = new ItemTimer( ITEMTIMERTYPE_BigHeadSensual, ITEMID_None, "Big Head of Sensual", "+2% Critical", 0, 0, NULL, NULL );
	ItemTimer * pcBCB = new ItemTimer( ITEMTIMERTYPE_BCBuff, ITEMID_None, "PvP Honor", "+8% Attack Power", 0, 0, NULL, NULL );
	ItemTimer * pcCVH = new ItemTimer( ITEMTIMERTYPE_CaravanHopy, ITEMID_None, "Caravan Hopy", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCVR = new ItemTimer( ITEMTIMERTYPE_CaravanRudolph, ITEMID_None, "Caravan Rudolph", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcFT1 = new ItemTimer( ITEMTIMERTYPE_SkillCooldownReduceT1, ITEMID_None, "Feather Tier 1", "Reduces cooldown of Tier 1\nskills by 10%", 0, 0, NULL, NULL );
	ItemTimer * pcFT2 = new ItemTimer( ITEMTIMERTYPE_SkillCooldownReduceT2, ITEMID_None, "Feather Tier 2", "Reduces cooldown of Tier 2\nskills by 10%", 0, 0, NULL, NULL );
	ItemTimer * pcFT3 = new ItemTimer( ITEMTIMERTYPE_SkillCooldownReduceT3, ITEMID_None, "Feather Tier 3", "Reduces cooldown of Tier 3\nskills by 10%", 0, 0, NULL, NULL );
	ItemTimer * pcFT4 = new ItemTimer( ITEMTIMERTYPE_SkillCooldownReduceT4, ITEMID_None, "Feather Tier 4", "Reduces cooldown of Tier 4\nskills by 10%", 0, 0, NULL, NULL );
	ItemTimer * pcFT5 = new ItemTimer( ITEMTIMERTYPE_SkillCooldownReduceT5, ITEMID_None, "Feather Tier 5", "Reduces cooldown of Tier 5\nskills by 10%", 0, 0, NULL, NULL );
	ItemTimer * pcHPB = new ItemTimer( ITEMTIMERTYPE_HPBooster, ITEMID_None, "HP Booster", "+15% HP", 0, 0, NULL, NULL );
	ItemTimer * pcMPB = new ItemTimer( ITEMTIMERTYPE_MPBooster, ITEMID_None, "MP Booster", "+15% MP", 0, 0, NULL, NULL );
	ItemTimer * pcSPB = new ItemTimer( ITEMTIMERTYPE_STMBooster, ITEMID_None, "STM Booster", "+15% STM", 0, 0, NULL, NULL );
	ItemTimer * pcCVA = new ItemTimer( ITEMTIMERTYPE_CaravanArma, ITEMID_None, "Caravan Arma", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCVB = new ItemTimer( ITEMTIMERTYPE_CaravanBuma, ITEMID_None, "Caravan Buma", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCVC = new ItemTimer( ITEMTIMERTYPE_CaravanCuepy, ITEMID_None, "Caravan Cuepy", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCVU = new ItemTimer( ITEMTIMERTYPE_CaravanHungky, ITEMID_None, "Caravan Hungky", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCVI = new ItemTimer( ITEMTIMERTYPE_CaravanRabie, ITEMID_None, "Caravan Rabie", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCVT = new ItemTimer( ITEMTIMERTYPE_CaravanTobie, ITEMID_None, "Caravan Tobie", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCN1 = new ItemTimer( ITEMTIMERTYPE_TuttiFruttiHat, ITEMID_None, "TuttiFrutti Hat", "Gives +6% EXP", 0, 0, NULL, NULL );
	ItemTimer * pcEAH = new ItemTimer( ITEMTIMERTYPE_EasterHat, ITEMID_None, "Easter Hat", "Gives +2 Move Speed +5% EXP", 0, 0, NULL, NULL );
	ItemTimer * pcALC = new ItemTimer( ITEMTIMERTYPE_ChemistHat, ITEMID_None, "Chemist Hat", "Gives +1 Extra drop", 0, 0, NULL, NULL );
	ItemTimer * pcPAT = new ItemTimer( ITEMTIMERTYPE_PatronumHat, ITEMID_None, "Patronum Hat", "Gives +3% Critical", 0, 0, NULL, NULL );
	ItemTimer * pcCVP1 = new ItemTimer( ITEMTIMERTYPE_CaravanSirKnight, ITEMID_None, "Caravan Sir Knight", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCVP2 = new ItemTimer( ITEMTIMERTYPE_CaravanPingu, ITEMID_None, "Caravan Pingu", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCVP3 = new ItemTimer( ITEMTIMERTYPE_CaravanYeti, ITEMID_None, "Caravan Yeti", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcCVP4 = new ItemTimer( ITEMTIMERTYPE_CaravanGoldenHopy, ITEMID_None, "Caravan Golden Hopy", "At your service! ^^", 0, 0, NULL, NULL );
	ItemTimer * pcGHH = new ItemTimer( ITEMTIMERTYPE_GoldenHopyHat, ITEMID_None, "Golden Hopy Hat", "Gives +3% Critical +2% Evade", 0, 0, NULL, NULL );
	ItemTimer * pcMUM = new ItemTimer( ITEMTIMERTYPE_MountPinguMysterious, ITEMID_None, "Pingu", "Gives +10 Movement Speed", 0, 0, NULL, NULL );
	ItemTimer * pcOBH = new ItemTimer( ITEMTIMERTYPE_ObscureHat, ITEMID_None, "Obscure Hat", "Gives +5% Attack Power", 0, 0, NULL, NULL );

	int iIndex = 0;

	pcEXP->FreezeMap( MAPID_RicartenTown );
	pcEXP->FreezeMap( MAPID_PhillaiTown );
	pcEXP->FreezeMap( MAPID_Eura );
	pcEXP->FreezeMap( MAPID_BattleTown );
	pcEXP->FreezeMap( MAPID_Atlantis );
	pcEXP->FreezeMap( MAPID_BlessCastle );
	pcEXP->FreezeMap( MAPID_GhostCastle );
	pcEXP->FreezeMap( MAPID_ForestDungeon );
	pcEXP->FreezeMap( MAPID_Bellatra );
	pcEXP->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\EXP.png" );
	pcEXP->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\EXP_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcEXP;

	pcTHE->FreezeMap( MAPID_RicartenTown );
	pcTHE->FreezeMap( MAPID_PhillaiTown );
	pcTHE->FreezeMap( MAPID_Eura );
	pcTHE->FreezeMap( MAPID_BattleTown );
	pcTHE->FreezeMap( MAPID_Atlantis );
	pcTHE->FreezeMap( MAPID_BlessCastle );
	pcTHE->FreezeMap( MAPID_GhostCastle );
	pcTHE->FreezeMap( MAPID_ForestDungeon );
	pcTHE->FreezeMap( MAPID_Bellatra );
	pcTHE->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\ThirdEye.png" );
	pcTHE->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\ThirdEye_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcTHE;

	pcPH1->FreezeMap( MAPID_RicartenTown );
	pcPH1->FreezeMap( MAPID_PhillaiTown );
	pcPH1->FreezeMap( MAPID_Eura );
	pcPH1->FreezeMap( MAPID_BattleTown );
	pcPH1->FreezeMap( MAPID_Atlantis );
	pcPH1->FreezeMap( MAPID_BlessCastle );
	pcPH1->FreezeMap( MAPID_Bellatra );
	pcPH1->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixFire.png" );
	pcPH1->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixFire_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcPH1;

	pcPH2->FreezeMap( MAPID_RicartenTown );
	pcPH2->FreezeMap( MAPID_PhillaiTown );
	pcPH2->FreezeMap( MAPID_Eura );
	pcPH2->FreezeMap( MAPID_BattleTown );
	pcPH2->FreezeMap( MAPID_Atlantis );
	pcPH2->FreezeMap( MAPID_BlessCastle );
	pcPH2->FreezeMap( MAPID_Bellatra );
	pcPH2->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixIce.png" );
	pcPH2->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixIce_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcPH2;

	pcPH3->FreezeMap( MAPID_RicartenTown );
	pcPH3->FreezeMap( MAPID_PhillaiTown );
	pcPH3->FreezeMap( MAPID_Eura );
	pcPH3->FreezeMap( MAPID_BattleTown );
	pcPH3->FreezeMap( MAPID_Atlantis );
	pcPH3->FreezeMap( MAPID_BlessCastle );
	pcPH3->FreezeMap( MAPID_Bellatra );
	pcPH3->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixLightning.png" );
	pcPH3->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixLightning_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcPH3;

	pcPH4->FreezeMap( MAPID_RicartenTown );
	pcPH4->FreezeMap( MAPID_PhillaiTown );
	pcPH4->FreezeMap( MAPID_Eura );
	pcPH4->FreezeMap( MAPID_BattleTown );
	pcPH4->FreezeMap( MAPID_Atlantis );
	pcPH4->FreezeMap( MAPID_BlessCastle );
	pcPH4->FreezeMap( MAPID_Bellatra );
	pcPH4->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixHeal.png" );
	pcPH4->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PhoenixHeal_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcPH4;

	pcMPD->FreezeMap( MAPID_RicartenTown );
	pcMPD->FreezeMap( MAPID_PhillaiTown );
	pcMPD->FreezeMap( MAPID_Atlantis );
	pcMPD->FreezeMap( MAPID_Bellatra );
	pcMPD->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MPDown.png" );
	pcMPD->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MPDown_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcMPD;

	pcSPD->FreezeMap( MAPID_RicartenTown );
	pcSPD->FreezeMap( MAPID_PhillaiTown );
	pcSPD->FreezeMap( MAPID_Atlantis );
	pcSPD->FreezeMap( MAPID_Bellatra );
	pcSPD->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SPDown.png" );
	pcSPD->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SPDown_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcSPD;

	pcHPU->FreezeMap( MAPID_RicartenTown );
	pcHPU->FreezeMap( MAPID_PhillaiTown );
	pcHPU->FreezeMap( MAPID_Atlantis );
	pcHPU->FreezeMap( MAPID_Bellatra );
	pcHPU->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\HPUp.png" );
	pcHPU->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\HPUp_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcHPU;

	pcMPU->FreezeMap( MAPID_RicartenTown );
	pcMPU->FreezeMap( MAPID_PhillaiTown );
	pcMPU->FreezeMap( MAPID_Atlantis );
	pcMPU->FreezeMap( MAPID_Bellatra );
	pcMPU->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MPUp.png" );
	pcMPU->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MPUp_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcMPU;

	pcBHN->FreezeMap( MAPID_RicartenTown );
	pcBHN->FreezeMap( MAPID_PhillaiTown );
	pcBHN->FreezeMap( MAPID_Atlantis );
	pcBHN->FreezeMap( MAPID_Bellatra );
	pcBHN->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBHN->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcBHN;

	pcWGS->FreezeMap( MAPID_RicartenTown );
	pcWGS->FreezeMap( MAPID_PhillaiTown );
	pcWGS->FreezeMap( MAPID_Atlantis );
	pcWGS->FreezeMap( MAPID_Bellatra );
	pcWGS->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Gravity.png" );
	pcWGS->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Gravity_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcWGS;

	pcMTS->FreezeMap( MAPID_RicartenTown );
	pcMTS->FreezeMap( MAPID_PhillaiTown );
	pcMTS->FreezeMap( MAPID_Atlantis );
	pcMTS->FreezeMap( MAPID_Bellatra );
	pcMTS->pcImage					= NULL;
	pcMTS->pcImageBackground		= NULL;
	pcaItemTimerPrefab[iIndex++]	= pcMTS;

	pcSKS->FreezeMap( MAPID_RicartenTown );
	pcSKS->FreezeMap( MAPID_PhillaiTown );
	pcSKS->FreezeMap( MAPID_Atlantis );
	pcSKS->FreezeMap( MAPID_Bellatra );
	pcSKS->pcImage					= NULL;
	pcSKS->pcImageBackground		= NULL;
	pcaItemTimerPrefab[iIndex++]	= pcSKS;

	pcCLH->FreezeMap( MAPID_RicartenTown );
	pcCLH->FreezeMap( MAPID_PhillaiTown );
	pcCLH->FreezeMap( MAPID_Atlantis );
	pcCLH->FreezeMap( MAPID_Bellatra );
	pcCLH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MagicianHat.png" );
	pcCLH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MagicianHat.png" );
	pcaItemTimerPrefab[iIndex++]	= pcCLH;

	pcWTH->FreezeMap( MAPID_RicartenTown );
	pcWTH->FreezeMap( MAPID_PhillaiTown );
	pcWTH->FreezeMap( MAPID_Atlantis );
	pcWTH->FreezeMap( MAPID_Bellatra );
	pcWTH->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\WitchHat.png" );
	pcWTH->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\WitchHat_.png" );
	pcaItemTimerPrefab[iIndex++] = pcWTH;

	pcHHT->FreezeMap( MAPID_RicartenTown );
	pcHHT->FreezeMap( MAPID_PhillaiTown );
	pcHHT->FreezeMap( MAPID_Atlantis );
	pcHHT->FreezeMap( MAPID_Bellatra );
	pcHHT->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\HalloweenHat.png" );
	pcHHT->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\HalloweenHat_.png" );
	pcaItemTimerPrefab[iIndex++] = pcHHT;

	pcSHH->FreezeMap( MAPID_RicartenTown );
	pcSHH->FreezeMap( MAPID_PhillaiTown );
	pcSHH->FreezeMap( MAPID_Atlantis );
	pcSHH->FreezeMap( MAPID_Bellatra );
	pcSHH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SheepHat.png" );
	pcSHH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SheepHat_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcSHH;

	pcGFH->FreezeMap( MAPID_RicartenTown );
	pcGFH->FreezeMap( MAPID_PhillaiTown );
	pcGFH->FreezeMap( MAPID_Atlantis );
	pcGFH->FreezeMap( MAPID_Bellatra );
	pcGFH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\GiraffeHat.png" );
	pcGFH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\GiraffeHat_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcGFH;

	pcSCH->FreezeMap( MAPID_RicartenTown );
	pcSCH->FreezeMap( MAPID_PhillaiTown );
	pcSCH->FreezeMap( MAPID_Atlantis );
	pcSCH->FreezeMap( MAPID_Bellatra );
	pcSCH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SoccerHat.png" );
	pcSCH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\SoccerHat_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcSCH;

	pcXMG->FreezeMap( MAPID_RicartenTown );
	pcXMG->FreezeMap( MAPID_PhillaiTown );
	pcXMG->FreezeMap( MAPID_Atlantis );
	pcXMG->FreezeMap( MAPID_Bellatra );
	pcXMG->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\XmasHat.png" );
	pcXMG->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\XmasHat_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcXMG;

	pcXMR->FreezeMap( MAPID_RicartenTown );
	pcXMR->FreezeMap( MAPID_PhillaiTown );
	pcXMR->FreezeMap( MAPID_Atlantis );
	pcXMR->FreezeMap( MAPID_Bellatra );
	pcXMR->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\XmasHat.png" );
	pcXMR->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\XmasHat_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcXMR;

	pcBHH->FreezeMap( MAPID_RicartenTown );
	pcBHH->FreezeMap( MAPID_PhillaiTown );
	pcBHH->FreezeMap( MAPID_Atlantis );
	pcBHH->FreezeMap( MAPID_Bellatra );
	pcBHH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBHH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcBHH;

	pcBHL->FreezeMap( MAPID_RicartenTown );
	pcBHL->FreezeMap( MAPID_PhillaiTown );
	pcBHL->FreezeMap( MAPID_Atlantis );
	pcBHL->FreezeMap( MAPID_Bellatra );
	pcBHL->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBHL->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcBHL;

	pcBSD->FreezeMap( MAPID_RicartenTown );
	pcBSD->FreezeMap( MAPID_PhillaiTown );
	pcBSD->FreezeMap( MAPID_Atlantis );
	pcBSD->FreezeMap( MAPID_Bellatra );
	pcBSD->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBSD->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcBSD;

	pcBSY->FreezeMap( MAPID_RicartenTown );
	pcBSY->FreezeMap( MAPID_PhillaiTown );
	pcBSY->FreezeMap( MAPID_Atlantis );
	pcBSY->FreezeMap( MAPID_Bellatra );
	pcBSY->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBSY->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcBSY;

	pcBHA->FreezeMap( MAPID_RicartenTown );
	pcBHA->FreezeMap( MAPID_PhillaiTown );
	pcBHA->FreezeMap( MAPID_Atlantis );
	pcBHA->FreezeMap( MAPID_Bellatra );
	pcBHA->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBHA->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcBHA;

	pcBSU->FreezeMap( MAPID_RicartenTown );
	pcBSU->FreezeMap( MAPID_PhillaiTown );
	pcBSU->FreezeMap( MAPID_Atlantis );
	pcBSU->FreezeMap( MAPID_Bellatra );
	pcBSU->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBSU->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcBSU;

	pcBSN->FreezeMap( MAPID_RicartenTown );
	pcBSN->FreezeMap( MAPID_PhillaiTown );
	pcBSN->FreezeMap( MAPID_Atlantis );
	pcBSN->FreezeMap( MAPID_Bellatra );
	pcBSN->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead.png" );
	pcBSN->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\BigHead_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcBSN;

	pcBCB->FreezeMap( MAPID_RicartenTown );
	pcBCB->FreezeMap( MAPID_PhillaiTown );
	pcBCB->FreezeMap( MAPID_Atlantis );
	pcBCB->FreezeMap( MAPID_Bellatra );
	pcBCB->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\TopPvp.png" );
	pcBCB->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\TopPvp_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcBCB;

	pcCVH->FreezeMap( MAPID_RicartenTown );
	pcCVH->FreezeMap( MAPID_PhillaiTown );
	pcCVH->FreezeMap( MAPID_Eura );
	pcCVH->FreezeMap( MAPID_BattleTown );
	pcCVH->FreezeMap( MAPID_Atlantis );
	pcCVH->FreezeMap( MAPID_BlessCastle );
	pcCVH->FreezeMap( MAPID_Bellatra );
	pcCVH->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanHopy.png" );
	pcCVH->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanHopy_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcCVH;

	pcCVR->FreezeMap( MAPID_RicartenTown );
	pcCVR->FreezeMap( MAPID_PhillaiTown );
	pcCVR->FreezeMap( MAPID_Eura );
	pcCVR->FreezeMap( MAPID_BattleTown );
	pcCVR->FreezeMap( MAPID_Atlantis );
	pcCVR->FreezeMap( MAPID_BlessCastle );
	pcCVR->FreezeMap( MAPID_Bellatra );
	pcCVR->pcImage					= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanRudolph.png" );
	pcCVR->pcImageBackground		= UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanRudolph_.png" );
	pcaItemTimerPrefab[iIndex++]	= pcCVR;

	pcFT1->FreezeMap( MAPID_RicartenTown );
	pcFT1->FreezeMap( MAPID_PhillaiTown );
	pcFT1->FreezeMap( MAPID_Atlantis );
	pcFT1->FreezeMap( MAPID_Bellatra );
	pcFT1->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\FeatherT1.png" );
	pcFT1->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Feather_.png" );
	pcaItemTimerPrefab[iIndex++] = pcFT1;

	pcFT2->FreezeMap( MAPID_RicartenTown );
	pcFT2->FreezeMap( MAPID_PhillaiTown );
	pcFT2->FreezeMap( MAPID_Atlantis );
	pcFT2->FreezeMap( MAPID_Bellatra );
	pcFT2->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\FeatherT2.png" );
	pcFT2->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Feather_.png" );
	pcaItemTimerPrefab[iIndex++] = pcFT2;

	pcFT3->FreezeMap( MAPID_RicartenTown );
	pcFT3->FreezeMap( MAPID_PhillaiTown );
	pcFT3->FreezeMap( MAPID_Atlantis );
	pcFT3->FreezeMap( MAPID_Bellatra );
	pcFT3->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\FeatherT3.png" );
	pcFT3->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Feather_.png" );
	pcaItemTimerPrefab[iIndex++] = pcFT3;

	pcFT4->FreezeMap( MAPID_RicartenTown );
	pcFT4->FreezeMap( MAPID_PhillaiTown );
	pcFT4->FreezeMap( MAPID_Atlantis );
	pcFT4->FreezeMap( MAPID_Bellatra );
	pcFT4->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\FeatherT4.png" );
	pcFT4->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Feather_.png" );
	pcaItemTimerPrefab[iIndex++] = pcFT4;

	pcFT5->FreezeMap( MAPID_RicartenTown );
	pcFT5->FreezeMap( MAPID_PhillaiTown );
	pcFT5->FreezeMap( MAPID_Atlantis );
	pcFT5->FreezeMap( MAPID_Bellatra );
	pcFT5->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\FeatherT5.png" );
	pcFT5->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Feather_.png" );
	pcaItemTimerPrefab[iIndex++] = pcFT5;

	pcHPB->FreezeMap( MAPID_RicartenTown );
	pcHPB->FreezeMap( MAPID_PhillaiTown );
	pcHPB->FreezeMap( MAPID_Atlantis );
	pcHPB->FreezeMap( MAPID_Bellatra );
	pcHPB->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\HPBooster.png" );
	pcHPB->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Boosters_.png" );
	pcaItemTimerPrefab[iIndex++] = pcHPB;

	pcMPB->FreezeMap( MAPID_RicartenTown );
	pcMPB->FreezeMap( MAPID_PhillaiTown );
	pcMPB->FreezeMap( MAPID_Atlantis );
	pcMPB->FreezeMap( MAPID_Bellatra );
	pcMPB->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\MPBooster.png" );
	pcMPB->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Boosters_.png" );
	pcaItemTimerPrefab[iIndex++] = pcMPB;

	pcSPB->FreezeMap( MAPID_RicartenTown );
	pcSPB->FreezeMap( MAPID_PhillaiTown );
	pcSPB->FreezeMap( MAPID_Atlantis );
	pcSPB->FreezeMap( MAPID_Bellatra );
	pcSPB->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\STMBooster.png" );
	pcSPB->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\Boosters_.png" );
	pcaItemTimerPrefab[iIndex++] = pcSPB;

	pcCVA->FreezeMap( MAPID_RicartenTown );
	pcCVA->FreezeMap( MAPID_PhillaiTown );
	pcCVA->FreezeMap( MAPID_Eura );
	pcCVA->FreezeMap( MAPID_BattleTown );
	pcCVA->FreezeMap( MAPID_Atlantis );
	pcCVA->FreezeMap( MAPID_BlessCastle );
	pcCVA->FreezeMap( MAPID_Bellatra );
	pcCVA->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanArma.png" );
	pcCVA->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanArma_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVA;

	pcCVB->FreezeMap( MAPID_RicartenTown );
	pcCVB->FreezeMap( MAPID_PhillaiTown );
	pcCVB->FreezeMap( MAPID_Eura );
	pcCVB->FreezeMap( MAPID_BattleTown );
	pcCVB->FreezeMap( MAPID_Atlantis );
	pcCVB->FreezeMap( MAPID_BlessCastle );
	pcCVB->FreezeMap( MAPID_Bellatra );
	pcCVB->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanBuma.png" );
	pcCVB->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanBuma_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVB;

	pcCVC->FreezeMap( MAPID_RicartenTown );
	pcCVC->FreezeMap( MAPID_PhillaiTown );
	pcCVC->FreezeMap( MAPID_Eura );
	pcCVC->FreezeMap( MAPID_BattleTown );
	pcCVC->FreezeMap( MAPID_Atlantis );
	pcCVC->FreezeMap( MAPID_BlessCastle );
	pcCVC->FreezeMap( MAPID_Bellatra );
	pcCVC->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanCuepy.png" );
	pcCVC->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanCuepy_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVC;

	pcCVU->FreezeMap( MAPID_RicartenTown );
	pcCVU->FreezeMap( MAPID_PhillaiTown );
	pcCVU->FreezeMap( MAPID_Eura );
	pcCVU->FreezeMap( MAPID_BattleTown );
	pcCVU->FreezeMap( MAPID_Atlantis );
	pcCVU->FreezeMap( MAPID_BlessCastle );
	pcCVU->FreezeMap( MAPID_Bellatra );
	pcCVU->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanHungky.png" );
	pcCVU->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanHungky_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVU;

	pcCVI->FreezeMap( MAPID_RicartenTown );
	pcCVI->FreezeMap( MAPID_PhillaiTown );
	pcCVI->FreezeMap( MAPID_Eura );
	pcCVI->FreezeMap( MAPID_BattleTown );
	pcCVI->FreezeMap( MAPID_Atlantis );
	pcCVI->FreezeMap( MAPID_BlessCastle );
	pcCVI->FreezeMap( MAPID_Bellatra );
	pcCVI->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanRabie.png" );
	pcCVI->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanRabie_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVI;

	pcCVT->FreezeMap( MAPID_RicartenTown );
	pcCVT->FreezeMap( MAPID_PhillaiTown );
	pcCVT->FreezeMap( MAPID_Eura );
	pcCVT->FreezeMap( MAPID_BattleTown );
	pcCVT->FreezeMap( MAPID_Atlantis );
	pcCVT->FreezeMap( MAPID_BlessCastle );
	pcCVT->FreezeMap( MAPID_Bellatra );
	pcCVT->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanTobie.png" );
	pcCVT->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanTobie_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVT;

	pcCN1->FreezeMap( MAPID_RicartenTown );
	pcCN1->FreezeMap( MAPID_PhillaiTown );
	pcCN1->FreezeMap( MAPID_Atlantis );
	pcCN1->FreezeMap( MAPID_Bellatra );
	pcCN1->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\TuttiFruttiHat.png" );
	pcCN1->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\TuttiFruttiHat_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCN1;

	pcEAH->FreezeMap( MAPID_RicartenTown );
	pcEAH->FreezeMap( MAPID_PhillaiTown );
	pcEAH->FreezeMap( MAPID_Atlantis );
	pcEAH->FreezeMap( MAPID_Bellatra );
	pcEAH->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\EasterHat.png" );
	pcEAH->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\EasterHat_.png" );
	pcaItemTimerPrefab[iIndex++] = pcEAH;

	pcALC->FreezeMap( MAPID_RicartenTown );
	pcALC->FreezeMap( MAPID_PhillaiTown );
	pcALC->FreezeMap( MAPID_Atlantis );
	pcALC->FreezeMap( MAPID_Bellatra );
	pcALC->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\ChemistHat.png" );
	pcALC->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\ChemistHat_.png" );
	pcaItemTimerPrefab[iIndex++] = pcALC;

	pcPAT->FreezeMap( MAPID_RicartenTown );
	pcPAT->FreezeMap( MAPID_PhillaiTown );
	pcPAT->FreezeMap( MAPID_Atlantis );
	pcPAT->FreezeMap( MAPID_Bellatra );
	pcPAT->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PatronumHat.png" );
	pcPAT->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PatronumHat_.png" );
	pcaItemTimerPrefab[iIndex++] = pcPAT;

	pcCVP1->FreezeMap( MAPID_RicartenTown );
	pcCVP1->FreezeMap( MAPID_PhillaiTown );
	pcCVP1->FreezeMap( MAPID_Eura );
	pcCVP1->FreezeMap( MAPID_BattleTown );
	pcCVP1->FreezeMap( MAPID_Atlantis );
	pcCVP1->FreezeMap( MAPID_BlessCastle );
	pcCVP1->FreezeMap( MAPID_Bellatra );
	pcCVP1->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanSirKnight.png" );
	pcCVP1->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanSirKnight_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVP1;

	pcCVP2->FreezeMap( MAPID_RicartenTown );
	pcCVP2->FreezeMap( MAPID_PhillaiTown );
	pcCVP2->FreezeMap( MAPID_Eura );
	pcCVP2->FreezeMap( MAPID_BattleTown );
	pcCVP2->FreezeMap( MAPID_Atlantis );
	pcCVP2->FreezeMap( MAPID_BlessCastle );
	pcCVP2->FreezeMap( MAPID_Bellatra );
	pcCVP2->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanPingu.png" );
	pcCVP2->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanPingu_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVP2;

	pcCVP3->FreezeMap( MAPID_RicartenTown );
	pcCVP3->FreezeMap( MAPID_PhillaiTown );
	pcCVP3->FreezeMap( MAPID_Eura );
	pcCVP3->FreezeMap( MAPID_BattleTown );
	pcCVP3->FreezeMap( MAPID_Atlantis );
	pcCVP3->FreezeMap( MAPID_BlessCastle );
	pcCVP3->FreezeMap( MAPID_Bellatra );
	pcCVP3->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanYeti.png" );
	pcCVP3->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\CaravanYeti_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVP3;

	pcCVP4->FreezeMap( MAPID_RicartenTown );
	pcCVP4->FreezeMap( MAPID_PhillaiTown );
	pcCVP4->FreezeMap( MAPID_Eura );
	pcCVP4->FreezeMap( MAPID_BattleTown );
	pcCVP4->FreezeMap( MAPID_Atlantis );
	pcCVP4->FreezeMap( MAPID_BlessCastle );
	pcCVP4->FreezeMap( MAPID_Bellatra );
	pcCVP4->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\G_CaravanHopy.png" );
	pcCVP4->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\G_CaravanHopy_.png" );
	pcaItemTimerPrefab[iIndex++] = pcCVP4;

	pcGHH->FreezeMap( MAPID_RicartenTown );
	pcGHH->FreezeMap( MAPID_PhillaiTown );
	pcGHH->FreezeMap( MAPID_Atlantis );
	pcGHH->FreezeMap( MAPID_Bellatra );
	pcGHH->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\GoldenHopyHat.png" );
	pcGHH->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\GoldenHopyHat_.png" );
	pcaItemTimerPrefab[iIndex++] = pcGHH;

	pcMUM->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PinguHMount.png" );
	pcMUM->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\PinguHMount_.png" );
	pcaItemTimerPrefab[iIndex++] = pcMUM;

	pcOBH->FreezeMap( MAPID_RicartenTown );
	pcOBH->FreezeMap( MAPID_PhillaiTown );
	pcOBH->FreezeMap( MAPID_Atlantis );
	pcOBH->FreezeMap( MAPID_Bellatra );
	pcOBH->pcImage = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\ObscureHat.png" );
	pcOBH->pcImageBackground = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\ObscureHat_.png" );
	pcaItemTimerPrefab[iIndex++] = pcOBH;

	//Box
	ItemTimer::pcImageBox = UI::ImageLoader::LoadImage( "game\\images\\items\\premium\\box.png" );
}

void CItemTimerHandler::Render()
{
	if ( Game::GetGameMode() == GAMEMODE_InGame && ISSHOW_INTERFACE )
	{
		for ( int i = 0; i < MAX_ITEMTIMER; i++ )
			if ( pcaItemTimer[i] != NULL )
				pcaItemTimer[i]->RenderTimer();

		for ( int i = 0; i < MAX_ITEMTIMER; i++ )
			if ( pcaItemTimer[i] != NULL )
				pcaItemTimer[i]->RenderBox();
	}
}

void CItemTimerHandler::Loop()
{
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		for ( int i = 0; i < MAX_ITEMTIMER; i++ )
			if ( pcaItemTimer[i] != NULL )
				pcaItemTimer[i]->Loop();
	}
}

void CItemTimerHandler::Tick()
{
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		for ( int i = 0; i < MAX_ITEMTIMER; i++ )
		{
			if ( pcaItemTimer[i] != NULL )
			{
				pcaItemTimer[i]->Tick();
				pcaItemTimer[i]->Freeze( MAP_ID );

				if ( !pcaItemTimer[i]->IsAlive() )
					RemoveTimer( i );
			}
		}
	}
}

void CItemTimerHandler::OnMouseMove( CMouse * pcMouse )
{
	Point2D * ps = pcMouse->GetPosition();

	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		for ( int i = 0; i < MAX_ITEMTIMER; i++ )
		{
			if ( pcaItemTimer[i] != NULL )
			{
				pcaItemTimer[i]->OnMouseMove( ps );
			}
		}
	}
}

BOOL CItemTimerHandler::OnMouseClick( CMouse * pcMouse )
{
	BOOL bRet = FALSE;

	if ( (Game::GetGameMode() == GAMEMODE_InGame) && (pcMouse->GetEvent() == EMouseEvent::ClickDownR) )
	{
		for ( int i = 0; i < MAX_ITEMTIMER; i++ )
		{
			if ( pcaItemTimer[i] != NULL )
			{
				if ( pcaItemTimer[i]->IsViewing() )
				{
					bool bCanCancel = true;
					if ( auto pcItemTimer = CARAVANHANDLER->GetCaravanItemTimer() )
						if ( pcItemTimer == pcaItemTimer[i] )
							if ( CARAVANHANDLER->GetWindow()->IsOpen() )
								bCanCancel = false;
					
					if ( bCanCancel )
					{
						MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
						MESSAGEBOX->SetTitle( "Are you sure?" );
						MESSAGEBOX->SetDescription( FormatString( "Do you want to cancel the %s?", pcaItemTimer[i]->szName ) );
						MESSAGEBOX->SetEvent( std::bind( &CItemTimerHandler::OnClickCancelOK, this, pcaItemTimer[i], std::placeholders::_1 ) );
						MESSAGEBOX->Show();
					}
					
					bRet = TRUE;
					break;
				}
			}
		}
	}

	return bRet;
}

ItemTimer * CItemTimerHandler::GetPrefab( EItemTimerType iType )
{
	for ( int i = 0; i < NUM_ITEMTIMERTYPE; i++ )
	{
		ItemTimer * j = pcaItemTimerPrefab[i];

		if ( j && j->iType == iType )
			return j;
	}

	return NULL;
}

ItemTimer * CItemTimerHandler::GetHead()
{
	for ( int i = 0; i < MAX_ITEMTIMER; i++ )
	{
		if ( pcaItemTimer[i] != NULL )
		{
			if ( IsHead( pcaItemTimer[i]->iType ) )
				return pcaItemTimer[i];
		}
	}

	return NULL;
}

void CItemTimerHandler::EventTimerStart( ItemTimer * pcTimer, bool bFirstTime )
{
	switch ( pcTimer->iType )
	{
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixLightning:
		case ITEMTIMERTYPE_PhoenixHeal:
			int iID;

			iID = PHOENIXTYPEID_Fire;
			
			if ( pcTimer->iType == ITEMTIMERTYPE_PhoenixIce )
				iID = PHOENIXTYPEID_Ice;
			else if ( pcTimer->iType == ITEMTIMERTYPE_PhoenixLightning )
				iID = PHOENIXTYPEID_Lightning;
			else if ( pcTimer->iType == ITEMTIMERTYPE_PhoenixHeal )
				iID = PHOENIXTYPEID_Healing;

			CHARACTERGAME->SetPhoenix( TRUE, iID );
			break;

		case ITEMTIMERTYPE_HPUp:
			CHARACTERGAME->bHPUp = TRUE;
			break;

		case ITEMTIMERTYPE_MPUp:
			CHARACTERGAME->bMPUp = TRUE;
			break;

		case ITEMTIMERTYPE_MPDown:
			CHARACTERGAME->bMPDown = TRUE;
			break;

		case ITEMTIMERTYPE_SPDown:
			CHARACTERGAME->bSPDown = TRUE;
			break;

		case ITEMTIMERTYPE_WeightStone:
			CHARACTERGAME->bWeightStone = TRUE;
			break;

		case ITEMTIMERTYPE_BCBuff:
			CHARACTERGAME->bTopPVP = TRUE;
			break;

		case ITEMTIMERTYPE_WitchHat:
		case ITEMTIMERTYPE_CartolaHat:
		case ITEMTIMERTYPE_HalloweenHat:
		case ITEMTIMERTYPE_SoccerHat:
		case ITEMTIMERTYPE_SheepHat:
		case ITEMTIMERTYPE_GiraffeHat:
		case ITEMTIMERTYPE_XMasGreenHat:
		case ITEMTIMERTYPE_XMasRedHat:
		case ITEMTIMERTYPE_BigHead:
		case ITEMTIMERTYPE_BigHeadHappiness:
		case ITEMTIMERTYPE_BigHeadLove:
		case ITEMTIMERTYPE_BigHeadShyness:
		case ITEMTIMERTYPE_BigHeadSadness:
		case ITEMTIMERTYPE_BigHeadAngry:
		case ITEMTIMERTYPE_BigHeadSurprised:
		case ITEMTIMERTYPE_BigHeadSensual:
		case ITEMTIMERTYPE_TuttiFruttiHat:
		case ITEMTIMERTYPE_EasterHat:
		case ITEMTIMERTYPE_ChemistHat:
		case ITEMTIMERTYPE_PatronumHat:
		case ITEMTIMERTYPE_GoldenHopyHat:
		case ITEMTIMERTYPE_ObscureHat:
			ITEMHANDLER->OnSetHairModel( (DWORD)pcTimer->iItemID );
			UPDATE_CHARACTER_CHECKSUM;
			break;

		case ITEMTIMERTYPE_SkillCooldownReduceT1:
			CHARACTERGAME->iCooldownT1 = 10;
			break;
		case ITEMTIMERTYPE_SkillCooldownReduceT2:
			CHARACTERGAME->iCooldownT2 = 10;
			break;
		case ITEMTIMERTYPE_SkillCooldownReduceT3:
			CHARACTERGAME->iCooldownT3 = 10;
			break;
		case ITEMTIMERTYPE_SkillCooldownReduceT4:
			CHARACTERGAME->iCooldownT4 = 10;
			break;
		case ITEMTIMERTYPE_SkillCooldownReduceT5:
			CHARACTERGAME->iCooldownT5 = 10;
			break;

		case ITEMTIMERTYPE_HPBooster:
			CHARACTERGAME->bHPBooster = TRUE;
			break;
		case ITEMTIMERTYPE_MPBooster:
			CHARACTERGAME->bMPBooster = TRUE;
			break;
		case ITEMTIMERTYPE_STMBooster:
			CHARACTERGAME->bSTMBooster = TRUE;
			break;
		case ITEMTIMERTYPE_MountPinguMysterious:

			break;
	}
}

void CItemTimerHandler::EventTimerStop( ItemTimer * pcTimer )
{
	switch ( pcTimer->iType )
	{
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixLightning:
		case ITEMTIMERTYPE_PhoenixHeal:
			CHARACTERGAME->SetPhoenix( FALSE, PHOENIXTYPEID_DeleteMask );
			break;

		case ITEMTIMERTYPE_HPUp:
			CHARACTERGAME->bHPUp = FALSE;
			break;

		case ITEMTIMERTYPE_MPUp:
			CHARACTERGAME->bMPUp = FALSE;
			break;

		case ITEMTIMERTYPE_MPDown:
			CHARACTERGAME->bMPDown = FALSE;
			break;

		case ITEMTIMERTYPE_SPDown:
			CHARACTERGAME->bSPDown = FALSE;
			break;

		case ITEMTIMERTYPE_WeightStone:
			CHARACTERGAME->bWeightStone = FALSE;
			break;

		case ITEMTIMERTYPE_BCBuff:
			CHARACTERGAME->bTopPVP = FALSE;
			break;

		case ITEMTIMERTYPE_WitchHat:
		case ITEMTIMERTYPE_CartolaHat:
		case ITEMTIMERTYPE_HalloweenHat:
		case ITEMTIMERTYPE_SoccerHat:
		case ITEMTIMERTYPE_SheepHat:
		case ITEMTIMERTYPE_GiraffeHat:
		case ITEMTIMERTYPE_XMasGreenHat:
		case ITEMTIMERTYPE_XMasRedHat:
		case ITEMTIMERTYPE_BigHead:
		case ITEMTIMERTYPE_BigHeadHappiness:
		case ITEMTIMERTYPE_BigHeadLove:
		case ITEMTIMERTYPE_BigHeadShyness:
		case ITEMTIMERTYPE_BigHeadSadness:
		case ITEMTIMERTYPE_BigHeadAngry:
		case ITEMTIMERTYPE_BigHeadSurprised:
		case ITEMTIMERTYPE_BigHeadSensual:
		case ITEMTIMERTYPE_TuttiFruttiHat:
		case ITEMTIMERTYPE_EasterHat:
		case ITEMTIMERTYPE_ChemistHat:
		case ITEMTIMERTYPE_PatronumHat:
		case ITEMTIMERTYPE_GoldenHopyHat:
		case ITEMTIMERTYPE_ObscureHat:
			ProcessPacket( &PacketOldHeadUse( FALSE ) );
			break;

		case ITEMTIMERTYPE_SkillCooldownReduceT1:
			CHARACTERGAME->iCooldownT1 = 0;
			break;
		case ITEMTIMERTYPE_SkillCooldownReduceT2:
			CHARACTERGAME->iCooldownT2 = 0;
			break;
		case ITEMTIMERTYPE_SkillCooldownReduceT3:
			CHARACTERGAME->iCooldownT3 = 0;
			break;
		case ITEMTIMERTYPE_SkillCooldownReduceT4:
			CHARACTERGAME->iCooldownT4 = 0;
			break;
		case ITEMTIMERTYPE_SkillCooldownReduceT5:
			CHARACTERGAME->iCooldownT5 = 0;
			break;

		case ITEMTIMERTYPE_HPBooster:
			CHARACTERGAME->bHPBooster = FALSE;
			break;
		case ITEMTIMERTYPE_MPBooster:
			CHARACTERGAME->bMPBooster = FALSE;
			break;
		case ITEMTIMERTYPE_STMBooster:
			CHARACTERGAME->bSTMBooster = FALSE;
			break;
		case ITEMTIMERTYPE_MountPinguMysterious:
			MOUNTHANDLER->RemoveMount( UNITDATA );
			CHARACTERGAME->bRidingMount = FALSE;
			break;
	}
}

void CItemTimerHandler::RemoveTimer( int iIndex )
{
	if ( iIndex >= 0 && iIndex < MAX_ITEMTIMER )
	{
		if ( pcaItemTimer[iIndex] != NULL )
		{
			EventTimerStop( pcaItemTimer[iIndex] );

			delete pcaItemTimer[iIndex];
			pcaItemTimer[iIndex] = NULL;

			UPDATE_CHARACTER_CHECKSUM;

			int iIndexLast = -1;

			//Update Timers
			for ( int i = (iIndex + 1); i < MAX_ITEMTIMER; i++ )
			{
				if ( pcaItemTimer[i] != NULL )
				{
					int iIndexN = i - 1;
					iIndexLast	= i;

					pcaItemTimer[iIndexN] = pcaItemTimer[i];

					int iColumn = iIndexN / DEFAULT_ITEMS_PER_COLUMN;
					int iRow	= iIndexN % DEFAULT_ITEMS_PER_COLUMN;

					pcaItemTimer[i]->sMoveTo.iX = (RESOLUTION_WIDTH - POS_ITMTMRBASEX) - (iColumn * TMR_SPACEX) - ITEMTIMER_WIDTH;
					pcaItemTimer[i]->sMoveTo.iY = POS_ITMTMRBASEY + (iRow * TMR_SPACEY);
				}
			}

			if ( iIndexLast != -1 )
			{
				pcaItemTimer[iIndexLast] = NULL;
			}
		}
	}
}

void CItemTimerHandler::OnResolutionChanged()
{
	if( Game::GetGameMode() == GAMEMODE_InGame )
	{
		for( int i = 0; i < MAX_ITEMTIMER; i++ )
		{
			if( pcaItemTimer[i] )
			{
				pcaItemTimer[i]->sPosition.iX = (RESOLUTION_WIDTH - POS_ITMTMRBASEX) - (pcaItemTimer[i]->iColumn * TMR_SPACEX) - ITEMTIMER_WIDTH;
				pcaItemTimer[i]->sMoveTo.iX = (RESOLUTION_WIDTH - POS_ITMTMRBASEX) - (pcaItemTimer[i]->iColumn * TMR_SPACEX) - ITEMTIMER_WIDTH;
			}
		}
	}
}

BOOL CItemTimerHandler::AddTimer( EItemTimerType iType, EItemID iItemID, DWORD dwTimeTotal, DWORD dwTimeLeft, bool bFirstTime )
{
	int iFreeSlot = GetFreeSlot();

	if ( iFreeSlot != -1 )
	{
		int iColumn = iFreeSlot / DEFAULT_ITEMS_PER_COLUMN;
		int iRow	= iFreeSlot % DEFAULT_ITEMS_PER_COLUMN;

		ItemTimer * pcTimer = GetItemTimer( iType );

		if ( pcTimer )
		{
			//Can Stack?
			if ( ItemTimerCanStack( pcTimer->iType ) )
			{
				//Remove Old
				int iIndexOld = GetIndex( pcTimer );
				if ( iIndexOld != -1 )
				{
					//Remove and Stop Timer
					RemoveTimer( iIndexOld );
					
					//Use Old Slot
					iFreeSlot	= iIndexOld;

					//Update Position
					iColumn		= iFreeSlot / DEFAULT_ITEMS_PER_COLUMN;
					iRow		= iFreeSlot % DEFAULT_ITEMS_PER_COLUMN;
					
					//Free Pointer
					pcTimer = NULL;
				}
			}
		}

		if ( (pcTimer == NULL) && (IsHead( iType ) == TRUE) )
			pcTimer = GetHead();

		if ( pcTimer == NULL )
		{
			ItemTimer * pcPrefab = GetPrefab( iType );

			if ( pcPrefab )
			{
				ItemTimer * pcItemTimer = new ItemTimer( iType, iItemID, pcPrefab->szName, pcPrefab->szDescription, dwTimeLeft, dwTimeTotal, pcPrefab->pcImage, pcPrefab->pcImageBackground );

				std::vector<int> vFreezeMaps = pcPrefab->GetFreezeMaps();
				for ( vector<int>::iterator it = vFreezeMaps.begin(); it != vFreezeMaps.end(); it++ )
					pcItemTimer->FreezeMap( *it );

				pcItemTimer->iColumn = iColumn;
				pcItemTimer->sPosition.iY = POS_ITMTMRBASEY;

				pcItemTimer->sMoveTo.iX = (RESOLUTION_WIDTH - POS_ITMTMRBASEX) - (iColumn * TMR_SPACEX) - ITEMTIMER_WIDTH;
				pcItemTimer->sMoveTo.iY = POS_ITMTMRBASEY + (iRow * TMR_SPACEY);

				EventTimerStart( pcItemTimer, bFirstTime );

				pcaItemTimer[iFreeSlot] = pcItemTimer;

				UPDATE_CHARACTER_CHECKSUM;

				return TRUE;
			}
		}
		else
			TITLEBOX( "You are already using an item of this type!" );
	}

	return FALSE;
}

void CItemTimerHandler::HandlePacket( SocketData * pcSocketData, PacketNewItemTimer * psPacket )
{
	if ( pcSocketData == SOCKETL )
		SENDPACKETG( psPacket );
	else
		AddTimer( psPacket->iType, psPacket->iItemID, psPacket->dwTimeTotal, psPacket->dwTimeLeft, true );
}

void CItemTimerHandler::HandlePacket( PacketItemPremiumData * psPacket )
{
	for ( int i = 0; i < _countof( psPacket->saItemList ); i++ )
	{
		ItemPremium * p = psPacket->saItemList + i;

		if ( p && p->dwCode )
			AddTimer( p->eType, (EItemID)p->dwCode, p->dwTimeTotal, p->dwTimeLeft );
	}
}

void CItemTimerHandler::HandlePacket( PacketOldHeadUse * psPacket )
{
	UNITDATA->bNoMove = TRUE;
	
	if ( psPacket->szHeadModel[0] != 0 )
		STRINGCOPY( UNITDATA->sCharacterData.Player.szHeadModel, psPacket->szHeadModel );
	else
	{
		CHARACTERGAME->ResetHead();
		return;
	}

	SETPLAYERPATTERN( UNITDATA, UNITDATA->sCharacterData.Player.szBodyModel, UNITDATA->sCharacterData.Player.szHeadModel );	
	CHECK_CHARACTER_CHECKSUM;
}

void CItemTimerHandler::HandlePacket( PacketRequestBeginnerItemsTimer * psPacket )
{
	if ( UNITDATA->sCharacterData.iLevel <= MAX_LEVEL_BEGINNERSBUFF )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Beginner's Buff" );
		MESSAGEBOX->SetDescription( "Do you want to receive the buff?" );
		MESSAGEBOX->SetEvent( std::bind( &CItemTimerHandler::OnClickBeginnerRequest, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
}

void CItemTimerHandler::HandlePacket( PacketRequestChemistItemTimer * psPacket )
{
	if ( ITEM_TIMER( ITEMTIMERTYPE_ChemistHat ) == NULL )
	{
		bool bHat = GetHead() != NULL;

		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Chemist Buff" );
		MESSAGEBOX->SetDescription( bHat ? "Do you want to receive the buff and replace the current hat?" : "Do you want to receive the buff?" );
		MESSAGEBOX->SetEvent( std::bind( &CItemTimerHandler::OnClickChemistBuffRequest, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
}

void CItemTimerHandler::ProcessPacket( PacketOldHeadUse * psPacket )
{
	psPacket->iLength = sizeof( PacketOldHeadUse );
	psPacket->iHeader = PKTHDR_CharacterHeadOldUse;
	SENDPACKETG( psPacket );
}

void CItemTimerHandler::ProcessPacket( PacketNewItemTimer * psPacket )
{
	psPacket->iLength = sizeof( PacketNewItemTimer );
	psPacket->iHeader = PKTHDR_SetItemTimer;
	SENDPACKETG( psPacket );
}

void CItemTimerHandler::ProcessPacket( PacketCancelItemTimer * psPacket )
{
	psPacket->iLength = sizeof( PacketCancelItemTimer );
	psPacket->iHeader = PKTHDR_CancelItemTimer;
	SENDPACKETG( psPacket );
}

void CItemTimerHandler::UpdateOldHead()
{
	PacketOldHeadUse s;
	STRINGCOPY( s.szHeadModel, UNITDATA->sCharacterData.Player.szHeadModel );
	ProcessPacket( &s );
}

ItemTimer::ItemTimer( EItemTimerType iType, EItemID iItemID, char * pszName, char * pszDescription, DWORD dwTimeLeft, DWORD dwTimeTotal, UI::Image * pcImage, UI::Image * pcImageBackground )
{
	this->iType				= iType;
	this->iItemID			= iItemID;
	
	STRINGCOPY( szName, pszName );
	STRINGCOPY( szDescription, pszDescription );

	this->dwTimeLeft		= dwTimeLeft;
	this->dwTimeTotal		= dwTimeTotal;

	this->pcImage			= pcImage;
	this->pcImageBackground = pcImageBackground;

	bView					= FALSE;
	bFreeze					= FALSE;
	bAlive					= TRUE;
	iFade					= 0;
	sPosition.iX			= RESOLUTION_WIDTH;
	sPosition.iY			= 0;
	sPosition.iWidth		= ITEMTIMER_WIDTH;
	sPosition.iHeight		= ITEMTIMER_HEIGHT;
	sMoveTo.iX				= 0;
	sMoveTo.iY				= 0;
	sMoveTo.iWidth			= ITEMTIMER_WIDTH;
	sMoveTo.iHeight			= ITEMTIMER_HEIGHT;
	iColumn					= 0;
}

ItemTimer::~ItemTimer()
{
	vFreezeMaps.clear();
}

void ItemTimer::RenderTimer()
{
	UI::ImageRender::Render( pcImageBackground, sPosition.iX, sPosition.iY, ITEMTIMER_WIDTH, ITEMTIMER_HEIGHT, D3DCOLOR_ARGB( iFade, 255, 255, 255 ) );

	float fTimePast = (float)(dwTimeTotal - dwTimeLeft);
	int iTop		= int( (fTimePast * ITEMTIMER_HEIGHT) / (float)dwTimeTotal );

	if ( iTop < ITEMTIMER_HEIGHT )
		UI::ImageRender::Render( pcImage, sPosition.iX, sPosition.iY + iTop, ITEMTIMER_WIDTH, ITEMTIMER_HEIGHT - iTop, 0, iTop, D3DCOLOR_ARGB( iFade, 255, 255, 255 ) );
}

void ItemTimer::RenderBox()
{
	static char szTimeDescription[64] = { 0 };

	if ( bView )
	{
		UI::ImageRender::Render( ItemTimer::pcImageBox, sMoveTo.iX - ItemTimer::pcImageBox->GetWidth(), sMoveTo.iY, ItemTimer::pcImageBox->GetWidth(), ItemTimer::pcImageBox->GetHeight(), -1 );

		auto pFont = GetGraphicsDevice()->GetDefaultFont();

		pFont->Draw( szName, sMoveTo.iX - ItemTimer::pcImageBox->GetWidth() + 8, sMoveTo.iY + 6, 0, DX::Color( 255, 225, 225, 225 ) );

		pFont->Draw( szDescription, sMoveTo.iX - ItemTimer::pcImageBox->GetWidth() + 8, sMoveTo.iY + 22, 0, DX::Color( 255, 225, 225, 225 ) );

		int iHour	 = (dwTimeLeft / 3600);
		int iMinutes = (dwTimeLeft / 60) % 60;
		int iSeconds = (dwTimeLeft % 60);

		STRINGFORMAT( szTimeDescription, "Time Left: %02d:%02d:%02d", iHour, iMinutes, iSeconds );

		pFont->Draw( szTimeDescription, sMoveTo.iX - ItemTimer::pcImageBox->GetWidth() + 8, sMoveTo.iY + 51, 0, DX::Color( 255, 50, 205, 50 ) );
	}
}

void ItemTimer::Loop()
{
	if ( iFade < ALPHA_HIGH )
		iFade++;

	if ( sMoveTo.iX > sPosition.iX )
		sPosition.iX++;
	else if ( sMoveTo.iX < sPosition.iX )
		sPosition.iX--;

	if ( sMoveTo.iY > sPosition.iY )
		sPosition.iY++;
	else if ( sMoveTo.iY < sPosition.iY )
		sPosition.iY--;
}

void ItemTimer::Tick()
{
	if ( dwTimeLeft != 0 )
	{
		if ( (bFreeze == FALSE) && (bAlive == TRUE) )
		{
			if ( (dwTimeLeft--) == 0 )
				Die();
		}
	}
	else
		Die();
}

void ItemTimer::OnMouseMove( Point2D * psPosition )
{
	if ( !Moving() )
	{
		if ( sMoveTo.Inside( psPosition ) )
			bView = TRUE;
		else
			bView = FALSE;
	}
	else
		bView = FALSE;
}

void ItemTimer::Freeze( int iMapID )
{
	bFreeze = FALSE;

	for ( std::vector<int>::iterator it = vFreezeMaps.begin(); it != vFreezeMaps.end(); it++ )
	{
		int i = (*it);

		if ( i == iMapID )
		{
			bFreeze = TRUE;
			break;
		}
	}
}

BOOL ItemTimer::Moving()
{
	if ( sPosition.Equals( &sMoveTo ) )
		return FALSE;

	return TRUE;
}
