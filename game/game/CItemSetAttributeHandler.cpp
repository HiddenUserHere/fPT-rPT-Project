#include "stdafx.h"
#include "CItemSetAttributeHandler.h"


CItemAttributeInstance::CItemAttributeInstance()
{
	pcItemAttribute = NULL;
}

CItemAttributeInstance::~CItemAttributeInstance()
{
}

void CItemAttributeInstance::Update()
{
	for ( auto & ps : vEquippedItems )
		DELET( ps );

	vEquippedItems.clear();

	if ( Game::GetGameMode() != GAMEMODE_InGame )
		return;

	for ( auto & psItem : pcItemAttribute->GetItems() )
	{
		int iCount = 0;

		if ( psItem->iType == ITEMATTRIBUTE_ItemBase )
			iCount += ITEMHANDLER->GetItemEquippedCountByBase( (EItemBase)psItem->iItem, UNITDATA->sCharacterData.iClass );
		else if ( psItem->iType == ITEMATTRIBUTE_ItemType )
			iCount += ITEMHANDLER->GetItemEquippedCountByType( (EItemType)psItem->iItem, UNITDATA->sCharacterData.iClass );
		else if ( psItem->iType == ITEMATTRIBUTE_ItemID )
			iCount += ITEMHANDLER->GetItemEquippedCountByCode( (EItemID)psItem->iItem, UNITDATA->sCharacterData.iClass );

		if ( iCount > 0 )
		{
			ItemDataEquipped * ps = new ItemDataEquipped;
			ps->psItem = psItem;
			ps->iCount = iCount;
			vEquippedItems.push_back( ps );
		}
	}
}

std::vector<CItemAttributeBonus *> CItemAttributeInstance::GetBonuses( int iItemCount )
{
	std::vector<CItemAttributeBonus *> vRet;

	for ( auto & pc : pcItemAttribute->GetBonuses() )
	{
		if ( (pc->GetSpec() == UNITDATA->sCharacterData.iClass) )
		{
			if ( pc->GetItemCount() <= iItemCount )
				vRet.push_back( pc );
		}
	}

	return vRet;
}

std::vector<CItemAttributeBonus*> CItemAttributeInstance::GetHighestBonuses()
{
	int iItemCount = 0;

	std::vector<CItemAttributeBonus *> vRet;

	for ( auto & pc : pcItemAttribute->GetBonuses() )
	{
		if ( (pc->GetSpec() == UNITDATA->sCharacterData.iClass) )
		{
			if ( pc->GetItemCount() <= iItemCount )
				vRet.push_back( pc );
		}
	}

	return vRet;
}

int CItemAttributeInstance::GetItemCount()
{
	int iCount = 0;

	for ( auto & ps : vEquippedItems )
		iCount += ps->iCount;

	return iCount;
}


CItemSetAttributeHandler::CItemSetAttributeHandler()
{
}


CItemSetAttributeHandler::~CItemSetAttributeHandler()
{
}

void CItemSetAttributeHandler::Init()
{
	{
		//Boss Set #1
		CItemAttributeInstance * pcInstanceSetBoss1 = new CItemAttributeInstance();
		CItemAttribute * pcSetBoss1 = new CItemAttribute();
		pcSetBoss1->SetID( 1 );
		pcSetBoss1->SetName( "Boss Set #1" );
		pcSetBoss1->AddTextItem( 1, "Devil Shy Ring" );
		pcSetBoss1->AddTextItem( 2, "Valento Ring" );
		pcSetBoss1->AddTextItem( 3, "Kelvezu Amulet" );
		pcSetBoss1->AddTextItem( 4, "Fury Sheltom" );
		pcSetBoss1->AddTextItem( 5, "Dark Guardian Sheltom" );
		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP", CHARACTERCLASS_Fighter );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP +1 Evasion", CHARACTERCLASS_Fighter );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +1 Evasion", CHARACTERCLASS_Fighter );

		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP", CHARACTERCLASS_Mechanician );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP +1 Evasion", CHARACTERCLASS_Mechanician );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +1 Evasion", CHARACTERCLASS_Mechanician );

		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP +20 Defense", CHARACTERCLASS_Archer );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP +25 Defense", CHARACTERCLASS_Archer );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +35 Defense", CHARACTERCLASS_Archer );

		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP +20 Atk Rtg", CHARACTERCLASS_Pikeman );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP +25 Atk Rtg", CHARACTERCLASS_Pikeman );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +35 Atk Rtg", CHARACTERCLASS_Pikeman );

		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP +20 Defense", CHARACTERCLASS_Atalanta );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP +25 Defense", CHARACTERCLASS_Atalanta );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +35 Defense", CHARACTERCLASS_Atalanta );

		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP", CHARACTERCLASS_Knight );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP +1 Evasion", CHARACTERCLASS_Knight );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +1 Evasion", CHARACTERCLASS_Knight );

		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP", CHARACTERCLASS_Magician );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP", CHARACTERCLASS_Magician );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +1 Critical", CHARACTERCLASS_Magician );

		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP", CHARACTERCLASS_Priestess );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP", CHARACTERCLASS_Priestess );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +1 Critical", CHARACTERCLASS_Priestess );

		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP +20 Atk Rtg", CHARACTERCLASS_Assassin );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP +25 Atk Rtg", CHARACTERCLASS_Assassin );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +35 Atk Rtg", CHARACTERCLASS_Assassin );

		pcSetBoss1->AddTextEffect( 2, "+15 HP +20 MP", CHARACTERCLASS_Shaman );
		pcSetBoss1->AddTextEffect( 3, "+20 HP +25 MP", CHARACTERCLASS_Shaman );
		pcSetBoss1->AddTextEffect( 4, "+25 HP +30 MP +1 Critical", CHARACTERCLASS_Shaman );

		pcSetBoss1->AddItem( 1, ITEMATTRIBUTE_ItemID, ITEMID_DevilShyRing, 2 );
		pcSetBoss1->AddItem( 2, ITEMATTRIBUTE_ItemID, ITEMID_ValentoRing, 2 );
		pcSetBoss1->AddItem( 3, ITEMATTRIBUTE_ItemID, ITEMID_KelvezuAmulet );
		pcSetBoss1->AddItem( 4, ITEMATTRIBUTE_ItemID, ITEMID_FurySheltom );
		pcSetBoss1->AddItem( 5, ITEMATTRIBUTE_ItemID, ITEMID_DarkGuardianSheltom );

		//Fighter & Mechanician & Knight
		{
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_Evade, 1.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_Evade, 1.0f, 4 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_Evade, 1.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_Evade, 1.0f, 4 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_Evade, 1.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_Evade, 1.0f, 4 ) );
		}

		//Pikeman & Assassin
		{
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_AttackRating, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_AttackRating, 25.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_AttackRating, 35.0f, 4 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_AttackRating, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_AttackRating, 25.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_AttackRating, 35.0f, 4 ) );
		}

		//Archer & Atalanta
		{
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_Defense, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_Defense, 25.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_Defense, 35.0f, 4 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_Defense, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_Defense, 25.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_Defense, 35.0f, 4 ) );
		}

		//Magician & Priestess & Shaman
		{
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Magician, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Magician, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Magician, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Magician, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Magician, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Magician, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Magician, ITEMBONUSATTRIBUTE_Critical, 1.0f, 4 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Priestess, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Priestess, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Priestess, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Priestess, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Priestess, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Priestess, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Priestess, ITEMBONUSATTRIBUTE_Critical, 1.0f, 4 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Shaman, ITEMBONUSATTRIBUTE_HP, 15.0f, 2 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Shaman, ITEMBONUSATTRIBUTE_MP, 20.0f, 2 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Shaman, ITEMBONUSATTRIBUTE_HP, 20.0f, 3 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Shaman, ITEMBONUSATTRIBUTE_MP, 25.0f, 3 ) );

			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Shaman, ITEMBONUSATTRIBUTE_HP, 25.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Shaman, ITEMBONUSATTRIBUTE_MP, 30.0f, 4 ) );
			pcSetBoss1->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Shaman, ITEMBONUSATTRIBUTE_Critical, 1.0f, 4 ) );
		}

		pcInstanceSetBoss1->SetItemAttribute( pcSetBoss1 );

		//Boss Set #2
		CItemAttributeInstance * pcInstanceSetBoss2 = new CItemAttributeInstance();
		CItemAttribute * pcSetBoss2 = new CItemAttribute();
		pcSetBoss2->SetID( 2 );
		pcSetBoss2->SetName( "Boss Set #2" );
		pcSetBoss2->AddTextItem( 1, "Devil Shy Ring" );
		pcSetBoss2->AddTextItem( 2, "Throne Ring");
		pcSetBoss2->AddTextItem( 3, "Tulla Amulet" );
		pcSetBoss2->AddTextItem( 4, "Throne Amulet");
		pcSetBoss2->AddTextItem( 5, "Fury Sheltom" );
		pcSetBoss2->AddTextItem( 6, "Throne Sheltom" );
		
		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP", CHARACTERCLASS_Fighter );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +1 Evasion", CHARACTERCLASS_Fighter );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +2 Evasion", CHARACTERCLASS_Fighter );

		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP", CHARACTERCLASS_Mechanician );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +1 Evasion", CHARACTERCLASS_Mechanician );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +2 Evasion", CHARACTERCLASS_Mechanician );

		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP +20 Defense", CHARACTERCLASS_Archer );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +25 Defense", CHARACTERCLASS_Archer );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +35 Defense", CHARACTERCLASS_Archer );

		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP +25 Atk Rtg", CHARACTERCLASS_Pikeman );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +30 Atk Rtg", CHARACTERCLASS_Pikeman );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +40 Atk Rtg", CHARACTERCLASS_Pikeman );

		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP +25 Defense", CHARACTERCLASS_Atalanta );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +30 Defense", CHARACTERCLASS_Atalanta );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +40 Defense", CHARACTERCLASS_Atalanta );

		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP", CHARACTERCLASS_Knight );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +1 Evasion", CHARACTERCLASS_Knight );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +2 Evasion", CHARACTERCLASS_Knight );

		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP", CHARACTERCLASS_Magician );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +1 Critical", CHARACTERCLASS_Magician );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +2 Critical", CHARACTERCLASS_Magician );

		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP", CHARACTERCLASS_Priestess );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +1 Critical", CHARACTERCLASS_Priestess );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +2 Critical", CHARACTERCLASS_Priestess );

		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP +25 Atk Rtg", CHARACTERCLASS_Assassin );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +30 Atk Rtg", CHARACTERCLASS_Assassin );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +40 Atk Rtg", CHARACTERCLASS_Assassin );

		pcSetBoss2->AddTextEffect( 2, "+20 HP +30 MP", CHARACTERCLASS_Shaman );
		pcSetBoss2->AddTextEffect( 3, "+30 HP +35 MP +1 Critical", CHARACTERCLASS_Shaman );
		pcSetBoss2->AddTextEffect( 4, "+40 HP +40 MP +2 Critical", CHARACTERCLASS_Shaman );

		pcSetBoss2->AddItem( 1, ITEMATTRIBUTE_ItemID, ITEMID_DevilShyRing, 2 );
		pcSetBoss2->AddItem( 2, ITEMATTRIBUTE_ItemID, ITEMID_ThroneRing, 2);
		pcSetBoss2->AddItem( 3, ITEMATTRIBUTE_ItemID, ITEMID_TullaAmulet );
		pcSetBoss2->AddItem( 4, ITEMATTRIBUTE_ItemID, ITEMID_ThroneAmulet);
		pcSetBoss2->AddItem( 5, ITEMATTRIBUTE_ItemID, ITEMID_FurySheltom );
		pcSetBoss2->AddItem( 6, ITEMATTRIBUTE_ItemID, ITEMID_ThroneSheltom );


		auto cBonusHP1 = CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_HP, 20.0f, 2 );
		auto cBonusMP1 = CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_MP, 30.0f, 2 );

		auto cBonusHP2 = CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_HP, 30.0f, 3 );
		auto cBonusMP2 = CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_MP, 35.0f, 3 );

		auto cBonusHP3 = CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_HP, 40.0f, 4 );
		auto cBonusMP3 = CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_MP, 40.0f, 4 );

		for ( int i = CHARACTERCLASS_Fighter; i <= CHARACTERCLASS_Assassin; i++ )
		{
			cBonusHP1.SetSpec( (ECharacterClass)i );
			cBonusHP2.SetSpec( (ECharacterClass)i );
			cBonusHP3.SetSpec( (ECharacterClass)i );

			cBonusMP1.SetSpec( (ECharacterClass)i );
			cBonusMP2.SetSpec( (ECharacterClass)i );
			cBonusMP3.SetSpec( (ECharacterClass)i );


			pcSetBoss2->AddBonus( cBonusHP1 );
			pcSetBoss2->AddBonus( cBonusHP2 );
			pcSetBoss2->AddBonus( cBonusHP3 );

			pcSetBoss2->AddBonus( cBonusMP1 );
			pcSetBoss2->AddBonus( cBonusMP2 );
			pcSetBoss2->AddBonus( cBonusMP3 );
		}

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_Evade, 1.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Fighter, ITEMBONUSATTRIBUTE_Evade, 2.0f, 4 ) );

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_Evade, 1.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Mechanician, ITEMBONUSATTRIBUTE_Evade, 2.0f, 4 ) );

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_Defense, 20.0f, 2 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_Defense, 25.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Archer, ITEMBONUSATTRIBUTE_Defense, 35.0f, 4 ) );

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_AttackRating, 25.0f, 2 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_AttackRating, 30.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Pikeman, ITEMBONUSATTRIBUTE_AttackRating, 40.0f, 4 ) );

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_Defense, 25.0f, 2 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_Defense, 30.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Atalanta, ITEMBONUSATTRIBUTE_Defense, 40.0f, 4 ) );

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_Evade, 1.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Knight, ITEMBONUSATTRIBUTE_Evade, 2.0f, 4 ) );

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Magician, ITEMBONUSATTRIBUTE_Critical, 1.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Magician, ITEMBONUSATTRIBUTE_Critical, 2.0f, 4 ) );

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Priestess, ITEMBONUSATTRIBUTE_Critical, 1.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Priestess, ITEMBONUSATTRIBUTE_Critical, 2.0f, 4 ) );

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_AttackRating, 25.0f, 2 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_AttackRating, 30.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Assassin, ITEMBONUSATTRIBUTE_AttackRating, 40.0f, 4 ) );

		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Shaman, ITEMBONUSATTRIBUTE_Critical, 1.0f, 3 ) );
		pcSetBoss2->AddBonus( CItemAttributeBonus( CHARACTERCLASS_Shaman, ITEMBONUSATTRIBUTE_Critical, 2.0f, 4 ) );

		pcInstanceSetBoss2->SetItemAttribute( pcSetBoss2 );
				
		AddInstance( pcInstanceSetBoss1 );
		AddInstance( pcInstanceSetBoss2 );
	}
}

CItemAttributeInstance * CItemSetAttributeHandler::GetAttributes( Item * psItem, BOOL bReverse )
{
	CItemAttributeInstance * pcRet = nullptr;

	if ( pcCurrentInstance )
	{
		for ( auto & pcItem : pcCurrentInstance->GetEquippedItems() )
		{
			if ( pcItem->psItem->iType == ITEMATTRIBUTE_ItemBase )
			{
				if ( ItemID( pcItem->psItem->iItem ).ToItemBase() == psItem->sItemID.ToItemBase() )
				{
					pcRet = pcCurrentInstance;
					break;
				}
			}
			else if ( pcItem->psItem->iType == ITEMATTRIBUTE_ItemType )
			{
				if ( ItemID( pcItem->psItem->iItem ).ToItemType() == psItem->sItemID.ToItemType() )
				{
					pcRet = pcCurrentInstance;
					break;
				}
			}
			else if ( pcItem->psItem->iType == ITEMATTRIBUTE_ItemID )
			{
				if ( ItemID( pcItem->psItem->iItem ).ToItemID() == psItem->sItemID.ToItemID() )
				{
					pcRet = pcCurrentInstance;
					break;
				}
			}
		}
	}

	return pcRet;
}

void CItemSetAttributeHandler::UpdateCharacterData()
{
	CharacterData & sCharacterData = UNITDATA->sCharacterData;

	if ( pcCurrentInstance )
	{
		//Add Bonus
		for ( auto & pcBonus : pcCurrentInstance->GetBonuses( pcCurrentInstance->GetItemCount() ) )
		{
			switch ( pcBonus->GetBonus() )
			{
				case ITEMBONUSATTRIBUTE_HP:
					sCharacterData.sHP.sMax += pcBonus->IsPercent() ? (int)(((float)sCharacterData.sHP.sMax * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;
				case ITEMBONUSATTRIBUTE_MP:
					sCharacterData.sMP.sMax += pcBonus->IsPercent() ? (int)(((float)sCharacterData.sMP.sMax * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;
				case ITEMBONUSATTRIBUTE_STM:
					sCharacterData.sSP.sMax += pcBonus->IsPercent() ? (int)(((float)sCharacterData.sSP.sMax * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;

				case ITEMBONUSATTRIBUTE_DamageMin:
					sCharacterData.iMinDamage += pcBonus->IsPercent() ? (int)(((float)sCharacterData.iMinDamage * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;
				case ITEMBONUSATTRIBUTE_DamageMax:
					sCharacterData.iMaxDamage += pcBonus->IsPercent() ? (int)(((float)sCharacterData.iMaxDamage * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;

				case ITEMBONUSATTRIBUTE_AttackSpeed:
					sCharacterData.iAttackSpeed += pcBonus->IsPercent() ? (int)(((float)sCharacterData.iAttackSpeed * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;

				case ITEMBONUSATTRIBUTE_AttackRating:
					sCharacterData.iAttackRating += pcBonus->IsPercent() ? (int)(((float)sCharacterData.iAttackRating * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;

				case ITEMBONUSATTRIBUTE_Defense:
					sCharacterData.iDefenseRating += pcBonus->IsPercent() ? (int)(((float)sCharacterData.iDefenseRating * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;

				case ITEMBONUSATTRIBUTE_Critical:
					sCharacterData.iCritical += pcBonus->IsPercent() ? (int)(((float)sCharacterData.iCritical * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;

				default:
					break;
			}
		}
	}
}

void CItemSetAttributeHandler::UpdateCharacterDataDefense()
{
	CharacterData & sCharacterData = UNITDATA->sCharacterData;

	int * piCritical = (int*)0x04B0C5EC;
	int * piBlock = (int*)0x04B0C4B0;
	int * piEvade = (int*)0x04B0C210;

	std::vector<CItemAttributeInstance*> vRet;

	if ( pcCurrentInstance )
	{
		//Add Bonus
		for ( auto & pcBonus : pcCurrentInstance->GetBonuses( pcCurrentInstance->GetItemCount() ) )
		{
			switch ( pcBonus->GetBonus() )
			{
				case ITEMBONUSATTRIBUTE_Critical:
	//				*piCritical += pcBonus->IsPercent() ? (int)(((float)*piCritical * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;

				case ITEMBONUSATTRIBUTE_Evade:
					*piEvade += pcBonus->IsPercent() ? (int)(((float)*piEvade * pcBonus->GetValue()) / 100.0f) : (int)pcBonus->GetValue();
					break;

				default:
					break;
			}
		}
	}
}

void CItemSetAttributeHandler::Update()
{
	for ( auto & pcInstance : vInstances )
		pcInstance->Update();

	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		pcCurrentInstance = GetInstance( 1 );

		int iItemCount = ITEMHANDLER->GetItemEquippedCountByCode( ITEMID_DevilShyRing, UNITDATA->sCharacterData.iClass ) +
			ITEMHANDLER->GetItemEquippedCountByCode( ITEMID_FurySheltom, UNITDATA->sCharacterData.iClass ) +
			ITEMHANDLER->GetItemEquippedCountByCode( ITEMID_ThroneAmulet, UNITDATA->sCharacterData.iClass ) +
			ITEMHANDLER->GetItemEquippedCountByCode( ITEMID_ThroneRing, UNITDATA->sCharacterData.iClass ) +
			ITEMHANDLER->GetItemEquippedCountByCode( ITEMID_ThroneSheltom, UNITDATA->sCharacterData.iClass ) +
			ITEMHANDLER->GetItemEquippedCountByCode( ITEMID_TullaAmulet, UNITDATA->sCharacterData.iClass ) ;

		//Effect 2
		if ( iItemCount >= 3 )
			pcCurrentInstance = GetInstance( 2 );

	}

	for (auto& pcInstance : vInstances)
		pcInstance->Update();

}

CItemAttributeInstance * CItemSetAttributeHandler::GetInstance( int _iID )
{
	for ( auto pcInstance : vInstances )
		if ( pcInstance->GetItemAttribute()->GetID() == _iID )
			return pcInstance;

	return nullptr;
}
