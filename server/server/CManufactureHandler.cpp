#include "stdafx.h"
#include "CManufactureHandler.h"


CManufactureHandler::CManufactureHandler()
{
	iaHalloweenEvilID[0]		= ITEMID_SpiderPlastic;
	iaHalloweenEvilID[1]		= ITEMID_GriffenEgg;
	iaHalloweenEvilID[2]		= ITEMID_ToiletPaper;

	iaHalloweenInfernalID[0]	= ITEMID_GriffenEgg;
	iaHalloweenInfernalID[1]	= ITEMID_HopyToy;
	iaHalloweenInfernalID[2]	= ITEMID_ToiletPaper;
	iHalloweenCombinationID		= 0;
}


CManufactureHandler::~CManufactureHandler()
{
}

void CManufactureHandler::HandlePacket( User * pcUser, PacketManufactureItem * psPacket )
{
	//Draxos Boots
	if ( HaveItem( psPacket, ITEMID_MokovaBoots, false, ITEMID_DarkRedRune, 1, ITEMID_Sapphire, 2 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice;
			if ( iPrice >= 0 )
			{
				//Get Item
				if ( SwapItem( pcUser, psPacket, ITEMID_DraxosBoots, TRUE, TRUE ) )
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

					//Send Item
					SENDPACKET( pcUser, psPacket );
					return;
				}
			}
		}
	}

	//Tulla Amulet
	if ( HaveItem( psPacket, ITEMID_KelvezuAmulet, false, ITEMID_LightBlueRune, 1, ITEMID_Oredo, 2 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 10;
			if ( iPrice >= 0 )
			{
				//Get Item
				if ( SwapItem( pcUser, psPacket, ITEMID_TullaAmulet, TRUE, TRUE ) )
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

					//Send Item
					SENDPACKET( pcUser, psPacket );
					return;
				}
			}
		}
	}

	//Devil Shy Ring
	if ( HaveItem( psPacket, ITEMID_ValentoRing, false, ITEMID_LightPurpleRune, 1, ITEMID_Bellum, 2 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 10;
			if ( iPrice >= 0 )
			{
				//Get Item
				if ( SwapItem( pcUser, psPacket, ITEMID_DevilShyRing, TRUE, TRUE ) )
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

					//Send Item
					SENDPACKET( pcUser, psPacket );
					return;
				}
			}
		}
	}

	//Throne Ring
	if (HaveItem(psPacket, ITEMID_DevilShyRing, false, ITEMID_ThroneYellowRune, 1, ITEMID_Sapphire, 2))
	{
		//Delete Runes
		if (DeleteRunes(pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2))
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 10;
			if (iPrice >= 0)
			{
				//Get Item
				if (SwapItem(pcUser, psPacket, ITEMID_ThroneRing, TRUE, TRUE))
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD(pcUser, (-iPrice));

					//Send Item
					SENDPACKET(pcUser, psPacket);
					return;
				}
			}
		}
	}

	//Throne Amulet
	if (HaveItem(psPacket, ITEMID_TullaAmulet, false, ITEMID_ThroneRedRune, 1, ITEMID_Sapphire, 2))
	{
		//Delete Runes
		if (DeleteRunes(pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2))
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 10;
			if (iPrice >= 0)
			{
				//Get Item
				if (SwapItem(pcUser, psPacket, ITEMID_ThroneAmulet, TRUE, TRUE))
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD(pcUser, (-iPrice));

					//Send Item
					SENDPACKET(pcUser, psPacket);
					return;
				}
			}
		}
	}

	//Throne Sheltom
	if (HaveItem(psPacket, ITEMID_FurySheltom, false, ITEMID_ThroneGreenRune, 1, ITEMID_Sapphire, 2))
	{
		//Delete Runes
		if (DeleteRunes(pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2))
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 10;
			if (iPrice >= 0)
			{
				//Get Item
				if (SwapItem(pcUser, psPacket, ITEMID_ThroneSheltom, TRUE, TRUE))
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD(pcUser, (-iPrice));

					//Send Item
					SENDPACKET(pcUser, psPacket);
					return;
				}
			}
		}
	}

	//Fury Sheltom
	if ( HaveItem( psPacket, ITEMID_DarkGuardianSheltom, false, ITEMID_GreenCrystalRune, 1, ITEMID_RedCrystalRune, 1, ITEMID_BlueCrystalRune, 1 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 10;
			if ( iPrice >= 0 )
			{
				//Get Item
				if ( SwapItem( pcUser, psPacket, ITEMID_FurySheltom, TRUE, TRUE ) )
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

					//Send Item
					SENDPACKET( pcUser, psPacket );
					return;
				}
			}
		}
	}

	//Throne Sheltom
	if (HaveItem(psPacket, ITEMID_FurySheltom, false, ITEMID_GoldCrystalRune, 1, ITEMID_SilverCrystalRune, 1, ITEMID_BronzeCrystalRune, 1))
	{
		//Delete Runes
		if (DeleteRunes(pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2))
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 20;
			if (iPrice >= 0)
			{
				//Get Item
				if (SwapItem(pcUser, psPacket, ITEMID_ThroneSheltom, TRUE, TRUE))
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD(pcUser, (-iPrice));

					//Send Item
					SENDPACKET(pcUser, psPacket);
					return;
				}
			}
		}
	}

	//Tropical Bracelets
	if ( HaveItem( psPacket, ITEMID_GhostCastleBracelets, false, ITEMID_LightRedRune, 3 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 10;
			if ( iPrice >= 0 )
			{
				//Get Item
				if ( SwapItem( pcUser, psPacket, ITEMID_TropicalBracelets, TRUE, TRUE ) )
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

					//Send Item
					SENDPACKET( pcUser, psPacket );
					return;
				}
			}
		}
	}

	//Ice Recipe
	if ( HaveItem( psPacket, ITEMID_IceRecipe, false, ITEMID_IceRune, 3 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = 200000;
			if ( iPrice >= 0 )
			{
				//Get Item
				if ( SwapItem( pcUser, psPacket, RandomI( 1, 2 ) == 1 ? ITEMID_IceCostumeMale : ITEMID_IceCostumeFemale, TRUE, TRUE ) )
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

					//Send Item
					SENDPACKET( pcUser, psPacket );
					return;
				}
			}
		}
	}

	//Secret Recipe
	if ( HaveItem( psPacket, ITEMID_SecretRecipe, false, ITEMID_Bellum, 3 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = 200000;
			if ( iPrice >= 0 )
			{
				//Get Item
				if ( SwapItem( pcUser, psPacket, ITEMID_MountUnicorn3, TRUE, TRUE ) )
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

					//Send Item
					SENDPACKET( pcUser, psPacket );
					return;
				}
			}
		}
	}

	//Greedy Gauntlets
	if ( HaveItem( psPacket, ITEMID_ArgonianGauntlets, false, ITEMID_LightOrangeRune, 1, ITEMID_Sapphire, 2 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 10;
			if ( iPrice >= 0 )
			{
				//Get Item
				if ( SwapItem( pcUser, psPacket, ITEMID_GreedyGauntlets, TRUE, TRUE ) )
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

					//Send Item
					SENDPACKET( pcUser, psPacket );
					return;
				}
			}
		}
	}

	//Volcanos Gauntlets
	if ( HaveItem( psPacket, ITEMID_GorgonyteEarring, false, ITEMID_LightSilverRune, 1, ITEMID_Sapphire, 2 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = psPacket->sItemData.sItem.iSalePrice * 10;
			if ( iPrice >= 0 )
			{
				//Get Item
				if ( SwapItem( pcUser, psPacket, ITEMID_VolcanosEarring, TRUE, TRUE ) )
				{
					//Give gold from User
					CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

					//Send Item
					SENDPACKET( pcUser, psPacket );
					return;
				}
			}
		}
	}

	//Golden Hopy Costume
	if ( HaveItem( psPacket, ITEMID_MysteriousBookRecipe, false, ITEMID_CuepyRune, 1, ITEMID_HopyRune, 1, ITEMID_GoldenHopyRune, 1 ) )
	{
		//Delete Runes
		if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
		{
			//Get price
			int iPrice = 200000;
			if ( iPrice >= 0 )
			{
				//Get Reward
				SPrizeData * ps = EVENTSERVER->GetPrizeRewardItemEaster();
				if ( ps )
				{
					char szItemName[32] = {0};

					//Find Item Name to put on Log...
					if ( ITEMSERVER->FindItemName( ps->szCode, szItemName ) )
					{
						//Find Item by Code to get ID to use on Log...
						ItemData * pcItemData = ITEMSERVER->FindItemPointerTable( ps->szCode );
						if ( pcItemData )
						{
							EItemID eRewardItemID = pcItemData->sBaseItemID.ToItemID();

							if ( RandomI( 0, 99 ) < 80 )
								eRewardItemID = pcItemData->sBaseItemID.ToItemID();
							else
							{
								if ( RandomI( 0, 99 ) < 5 )
									eRewardItemID = RandomI( 1, 2 ) == 1 ? ITEMID_GoldenHopyCostumeMale : ITEMID_GoldenHopyCostumeFemale;
								else
									eRewardItemID = RandomI( 1, 2 ) == 1 ? ITEMID_GoldenHopyCostumeMaleTime : ITEMID_GoldenHopyCostumeFemaleTime;
							}

							//Get Item
							if ( SwapItem( pcUser, psPacket, eRewardItemID, TRUE, TRUE ) )
							{
								//Give gold from User
								CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

								//Send Item
								SENDPACKET( pcUser, psPacket );
								return;
							}
						}
					}
				}
			}
		}
	}

	//Xmas
	if ( EVENT_XMAS )
	{
		/*
		if ( HaveItem( psPacket, ITEMID_RecipeXMas01, false, ITEMID_RedButton, 1, ITEMID_GreenCloth, 1, ITEMID_Cotton, 1 ) ||
			HaveItem( psPacket, ITEMID_RecipeXMas02, false, ITEMID_RedButton, 1, ITEMID_GreenCloth, 1, ITEMID_Cotton, 1 ) )
		{
			EItemID iItemID = ITEMID_None;

			//Cloat
			if ( psPacket->iRecipeID == ITEMID_RecipeXMas01 )
			{
				int iRand = rand() % 100;
				if ( iRand < 50 )
				{
					if ( iRand < 25 )
						iItemID = ITEMID_XMasCloatCostumeTM;
					else
						iItemID = ITEMID_XMasCloatCostumeTF;
				}
				else
					iItemID = ITEMID_XMasRedHat;
			}
			//Elegant
			else if ( psPacket->iRecipeID == ITEMID_RecipeXMas02 )
			{
				int iRand = rand() % 100;
				if ( iRand < 75 )
				{
					if ( iRand < 25 )
						iItemID = ITEMID_XMasElegantCostumeM;
					else
						iItemID = ITEMID_XMasElegantCostumeF;
				}
				else
					iItemID = ITEMID_XMasRedHat;
			}

			//Delete Runes
			if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
			{
				//Get price
				int iPrice = psPacket->sItemData.sItem.iSalePrice;
				if ( iPrice >= 0 )
				{
					//Get Item
					if ( SwapItem( pcUser, psPacket, iItemID, TRUE, TRUE ) )
					{
						//Give gold from User
						CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

						//Send Item
						SENDPACKET( pcUser, psPacket );
						return;
					}
				}
			}
		}
		*/
		if ( HaveItem( psPacket, ITEMID_XmasRecipeGift, false, ITEMID_RedXmasGiftRune, 1, ITEMID_GreenXmasGiftRune, 1, ITEMID_BlueXmasGiftRune, 1 ) )
		{
			//Delete Runes
			if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
			{
				//Get price
				int iPrice = 200000;
				if ( iPrice >= 0 )
				{
					static const EItemID eaTemporaryItemID[] =
					{
						ITEMID_XmasCostumeOldAxe,
						ITEMID_XmasCostumeOldAxe1Hand,
						ITEMID_XmasCostumeOldClaw,
						ITEMID_XmasCostumeOldDagger,
						ITEMID_XmasCostumeOldHammer,
						ITEMID_XmasCostumeOldStaff,
						ITEMID_XmasCostumeOldPhantom,
						ITEMID_XmasCostumeOldScythe,
						ITEMID_XmasCostumeOldBow,
						ITEMID_XmasCostumeOldSword,
						ITEMID_XmasCostumeOldSword1Hand,
						ITEMID_XmasCostumeOldJavelin
					};

					static const EItemID eaPermanentItemID[] =
					{
						ITEMID_XmasCostumeAxe,
						ITEMID_XmasCostumeAxe1Hand,
						ITEMID_XmasCostumeClaw,
						ITEMID_XmasCostumeDagger,
						ITEMID_XmasCostumeHammer,
						ITEMID_XmasCostumeStaff,
						ITEMID_XmasCostumeWand,
						ITEMID_XmasCostumePhantom,
						ITEMID_XmasCostumeScythe,
						ITEMID_XmasCostumeBow,
						ITEMID_XmasCostumeSword,
						ITEMID_XmasCostumeSword1Hand,
						ITEMID_XmasCostumeJavelin,
						ITEMID_XmasCostumeShield
					};


					static const EItemID eaXmasCostumes[] =
					{
						ITEMID_XMasCloatCostumeTM,
						ITEMID_XMasCloatCostumeTF,
					};


					static const EItemID eaXmasElegantCostumes[] =
					{
						ITEMID_XMasElegantCostumeM,
						ITEMID_XMasElegantCostumeF,
					};

					int iRandom = RandomI( 0, 999 );

					//15% Chance to get Nothing
					EItemID eSwapItemID = ITEMID_None;
					if ( iRandom >= 150 )
					{
						iRandom -= 150;

						//55% Chance to get Temporary Set
						if ( iRandom < 550 )
							eSwapItemID = eaTemporaryItemID[rand() % _countof( eaTemporaryItemID )];
						//20% Chance to get Xmas Costumes
						else if ( iRandom < 750 )
							eSwapItemID = eaXmasCostumes[rand() % _countof( eaXmasCostumes )];
						//7% Chance to get Xmas Elegant Costumes
						else if ( iRandom < 820 )
							eSwapItemID = eaXmasElegantCostumes[rand() % _countof( eaXmasElegantCostumes )];
						//3% Chance to get Permanent Set
						else
							eSwapItemID = eaPermanentItemID[rand() % _countof( eaPermanentItemID )];
					}

					//Get Item
					if ( SwapItem( pcUser, psPacket, eSwapItemID, TRUE, TRUE ) )
					{
						//Give gold from User
						CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

						//Send Item
						SENDPACKET( pcUser, psPacket );
						return;
					}
				}
			}
		}
	}

	//Halloween
	if ( EVENT_HALLOWEEN )
	{
		EItemID iLanternID = ITEMID_None;
		if ( HaveItem( psPacket, ITEMID_CoreHW, true, iaHalloweenEvilID[0], 1, iaHalloweenEvilID[1], 1, iaHalloweenEvilID[2], 1 ) )
			iLanternID = ITEMID_EvilLantern;
		else if ( HaveItem( psPacket, ITEMID_CoreHW, true, iaHalloweenInfernalID[0], 1, iaHalloweenInfernalID[1], 1, iaHalloweenInfernalID[2], 1 ) )
			iLanternID = ITEMID_InfernalLantern;
		else
		{
			int iCount = 0;
			iCount += HaveItem( psPacket, ITEMID_CoreHW, false, ITEMID_ToiletPaper, 1 ) ? 1 : 0;
			iCount += HaveItem( psPacket, ITEMID_CoreHW, false, ITEMID_HopyToy, 1 ) ? 1 : 0;
			iCount += HaveItem( psPacket, ITEMID_CoreHW, false, ITEMID_GriffenEgg, 1 ) ? 1 : 0;
			iCount += HaveItem( psPacket, ITEMID_CoreHW, false, ITEMID_SpiderPlastic, 1 ) ? 1 : 0;

			if ( iCount == 3 )
				iLanternID = ITEMID_RottenLantern;
		}

		//Have Lantern?
		if ( iLanternID != ITEMID_None )
		{
			//Delete Runes
			if ( DeleteRunes( pcUser, psPacket->iaRuneID, psPacket->iaChk1, psPacket->iaChk2 ) )
			{
				//Get price
				int iPrice = psPacket->sItemData.sItem.iSalePrice;
				if ( iPrice >= 0 )
				{
					//Get Item
					if ( SwapItem( pcUser, psPacket, iLanternID, FALSE, FALSE ) )
					{
						//Give gold from User
						CHARACTERSERVER->GiveGOLD( pcUser, (-iPrice) );

						//Send Item
						SENDPACKET( pcUser, psPacket );
						return;
					}
				}
			}
		}
	}

	ITEMSERVER->OnManufactureItem( pcUser, psPacket );
}

void CManufactureHandler::SetHalloweenCombination( int iID )
{
	if ( iHalloweenCombinationID != iID )
	{
		iHalloweenCombinationID = iID;

		if ( iHalloweenCombinationID > 3 )
			iHalloweenCombinationID = 0;

		if ( iHalloweenCombinationID == 1 )
		{
			iaHalloweenEvilID[0]		= ITEMID_SpiderPlastic;
			iaHalloweenEvilID[1]		= ITEMID_GriffenEgg;
			iaHalloweenEvilID[2]		= ITEMID_SpiderPlastic;

			iaHalloweenInfernalID[0]	= ITEMID_GriffenEgg;
			iaHalloweenInfernalID[1]	= ITEMID_SpiderPlastic;
			iaHalloweenInfernalID[2]	= ITEMID_ToiletPaper;
		}
		else if ( iHalloweenCombinationID == 2 )
		{
			iaHalloweenEvilID[0]		= ITEMID_SpiderPlastic;
			iaHalloweenEvilID[1]		= ITEMID_GriffenEgg;
			iaHalloweenEvilID[2]		= ITEMID_HopyToy;

			iaHalloweenInfernalID[0]	= ITEMID_HopyToy;
			iaHalloweenInfernalID[1]	= ITEMID_SpiderPlastic;
			iaHalloweenInfernalID[2]	= ITEMID_ToiletPaper;
		}
		else if ( iHalloweenCombinationID == 3 )
		{
			iaHalloweenEvilID[0]		= ITEMID_SpiderPlastic;
			iaHalloweenEvilID[1]		= ITEMID_GriffenEgg;
			iaHalloweenEvilID[2]		= ITEMID_HopyToy;

			iaHalloweenInfernalID[0]	= ITEMID_GriffenEgg;
			iaHalloweenInfernalID[1]	= ITEMID_ToiletPaper;
			iaHalloweenInfernalID[2]	= ITEMID_HopyToy;
		}
		else
		{
			iaHalloweenEvilID[0]		= ITEMID_HopyToy;
			iaHalloweenEvilID[1]		= ITEMID_GriffenEgg;
			iaHalloweenEvilID[2]		= ITEMID_ToiletPaper;

			iaHalloweenInfernalID[0]	= ITEMID_GriffenEgg;
			iaHalloweenInfernalID[1]	= ITEMID_SpiderPlastic;
			iaHalloweenInfernalID[2]	= ITEMID_ToiletPaper;
		}

		if ( EVENT_HALLOWEEN )
			CHATSERVER->SendChatAll( CHATCOLOR_Global, "Halloween> Rune Combination Changed!" );
	}
}

void CManufactureHandler::RandomizeCombinations()
{
	auto rd = std::random_device{};
	auto rng = std::default_random_engine{rd()};

	std::vector<EItemID> vItems;
	vItems.push_back( ITEMID_HopyToy );
	vItems.push_back( ITEMID_GriffenEgg );
	vItems.push_back( ITEMID_SpiderPlastic );
	vItems.push_back( ITEMID_ToiletPaper );

	std::shuffle( std::begin( vItems ), std::end( vItems ), rng );

	int iRandomID = RandomI( 0, 3 );

	iaHalloweenEvilID[0] = vItems[iRandomID++ % vItems.size()];
	iaHalloweenEvilID[1] = vItems[iRandomID++ % vItems.size()];
	iaHalloweenEvilID[2] = vItems[iRandomID++ % vItems.size()];

	iaHalloweenInfernalID[0] = vItems[iRandomID++ % vItems.size()];
	iaHalloweenInfernalID[1] = vItems[iRandomID++ % vItems.size()];
	iaHalloweenInfernalID[2] = vItems[iRandomID++ % vItems.size()];

	vItems.clear();
}

BOOL CManufactureHandler::DeleteItem( User * pcUser, EItemID iItemID, int iChk1, int iChk2 )
{
	LogItem sLog;
	ZeroMemory( &sLog, sizeof( LogItem ) );

	if ( ITEMSERVER->UseItem( pcUser->pcUserData, iItemID, iChk1, iChk2 ) < 0 )
	{
		STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
		STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

		sLog.ItemCount = 1;
		sLog.Flag = ITEMLOGID_ManufactureFail;
		sLog.Item[0].dwCode = iItemID;
		sLog.Item[0].dwHead = iChk1;
		sLog.Item[0].dwCheckSum = iChk2;
		LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );
		return FALSE;
	}

	return TRUE;
}

BOOL CManufactureHandler::DeleteRunes( User * pcUser, EItemID * piaItemID, int * piaChk1, int * piaChk2 )
{
	for ( int i = 0; i < 3; i++ )
	{
		if ( piaItemID[i] != ITEMID_None )
		{
			if ( DeleteItem( pcUser, piaItemID[i], piaChk1[i], piaChk2[i] ) == FALSE )
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CManufactureHandler::AddItem( User * pcUser, EItemID iItemID, int iChk1, int iChk2 )
{
	if ( ITEMSERVER->AddItemInventory( pcUser->pcUserData, iItemID, iChk1, iChk2 ) >= 0 )
	{
		LogItem sLog;
		ZeroMemory( &sLog, sizeof( LogItem ) );

		STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
		STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

		sLog.ItemCount = 1;
		sLog.Flag = ITEMLOGID_ManufactureGet;
		sLog.Item[0].dwCode = iItemID;
		sLog.Item[0].dwHead = iChk1;
		sLog.Item[0].dwCheckSum = iChk2;
		LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );

		return TRUE;
	}

	return FALSE;
}

BOOL CManufactureHandler::AddItem( User * pcUser, Item * psItem )
{
	if ( AddItem( pcUser, psItem->sItemID.ToItemID(), psItem->iChk1, psItem->iChk2 ) )
	{
		ITEMSERVER->SaveItemData( psItem );

		return TRUE;
	}

	return FALSE;
}

BOOL CManufactureHandler::HaveItem( PacketManufactureItem * psPacket, EItemID iItemRecipeID, bool bUniqueSlot, EItemID iItemSlot1ID, int iCount1, EItemID iItemSlot2ID, int iCount2, EItemID iItemSlot3ID, int iCount3 )
{
	BOOL bRet = FALSE;

	if ( (psPacket->iRecipeID == iItemRecipeID) || (iItemRecipeID == ITEMID_None) )
	{
		if ( bUniqueSlot )
		{
			if ( (iItemSlot1ID != ITEMID_None) && (psPacket->iaRuneID[0] == iItemSlot1ID) )
				iCount1--;
			if ( (iItemSlot2ID != ITEMID_None) && (psPacket->iaRuneID[1] == iItemSlot2ID) )
				iCount2--;
			if ( (iItemSlot3ID != ITEMID_None) && (psPacket->iaRuneID[2] == iItemSlot3ID) )
				iCount3--;
		}
		else
		{
			for ( int i = 0; i < 3; i++ )
			{
				if ( (iItemSlot1ID != ITEMID_None) && (psPacket->iaRuneID[i] == iItemSlot1ID) )
					iCount1--;
				if ( (iItemSlot2ID != ITEMID_None) && (psPacket->iaRuneID[i] == iItemSlot2ID) )
					iCount2--;
				if ( (iItemSlot3ID != ITEMID_None) && (psPacket->iaRuneID[i] == iItemSlot3ID) )
					iCount3--;
			}
		}
		if ( (iCount1 == 0) && (iCount2 == 0) && (iCount3 == 0) )
			bRet = TRUE;
	}

	return bRet;
}

BOOL CManufactureHandler::SwapItem( User * pcUser, PacketManufactureItem * psPacket, EItemID iNewItemID, BOOL bSameSpec, BOOL bPerfect )
{
	if ( iNewItemID == ITEMID_None )
	{
		psPacket->iResult = FALSE;

		return TRUE;
	}

	DefinitionItem * psItemDef = ITEMSERVER->FindItemDefByCode( iNewItemID );
	if ( psItemDef )
	{
		if ( DeleteItem( pcUser, psPacket->iRecipeID, psPacket->iRecipeChk1, psPacket->iRecipeChk2 ) )
		{
			int iClass = bSameSpec ? ClassFlagToCharacterClass( (EClassFlag)psPacket->sItemData.sItem.eSpecialization ) : 0;

			if ( bPerfect && psPacket->sItemData.sItem.bPerfectItem )
				ITEMSERVER->CreatePerfectItem( &psPacket->sItemData.sItem, psItemDef, iClass );
			else
				ITEMSERVER->CreateItem( &psPacket->sItemData.sItem, psItemDef, iClass );

			AddItem( pcUser, &psPacket->sItemData.sItem );

			psPacket->iResult = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}
