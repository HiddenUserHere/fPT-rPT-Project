#include "stdafx.h"
#include "UIItemInfoBox.h"


const char * pszaQuestMonsterName[] = { "Bargon", "Skeleton Warrior", "Head Cutter", "Armored Beetle", "Skeleton Ranger", "Titan", "Monsters Forgotten Land", "Monsters Oasis", "Monsters Dungeon 1F" };

namespace UI
{
ItemInfoBox::ItemInfoBox()
{
	iSocketGem[0] = iSocketGem[1] = -1;

	pWindowHair = std::make_shared<UI::Window>( Rectangle2D( 0, 62, 102, 102 ) );
	pWindowBody = std::make_shared<UI::Window>( Rectangle2D( 0, 72, 150, 190 ) );
}

ItemInfoBox::~ItemInfoBox()
{
	pWindowHair = nullptr;
	pWindowBody = nullptr;
}

void ItemInfoBox::AddString( int iSide, const std::string & strString, DWORD dwColor, BOOL bBold )
{
	std::vector<std::string> vString;

	//Find breakline in String 
	BOOL bMultiLine = FALSE;
	if( strString.find( '\n' ) != std::string::npos )
	{
		vString = split( strString, '\n' );
		bMultiLine = TRUE;
	}

	switch( iSide )
	{
	case 0: //Left Side
		//Is Multiline?
		if( bMultiLine )
		{
			//String from vector
			int i = 0;
			for( const auto & strSplitted : vString )
			{
				vLeftText.push_back( std::make_shared<Text>( strSplitted, Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor ) );
				i++;

				if( i != vString.size() )
				{
					vRightText.push_back( std::make_shared<Text>( "", Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor ) );
				}
			}
		}
		else
		{
			vLeftText.push_back( std::make_shared<Text>( strString, Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor ) );
		}

		break;

	case 1: //Right Side
		vRightText.push_back( std::make_shared<Text>( strString, Rectangle2D( 0, 0, 180, 20 ), bBold, dwColor ) );
		break;
	}
}

void ItemInfoBox::AddCompareString( const std::string & strString, DWORD dwColor )
{
	vRowCompare.push_back( vLeftText.size() );
	vCompareText.push_back( std::make_shared<Text>( strString, Rectangle2D( 0, 0, 180, 20 ), FALSE, dwColor ) );
}

void ItemInfoBox::SetLineColor( DWORD dwColor, int iSide, int iLineCount )
{
	int iSetColor = 0;
	if( iLineCount < 0 )
		iSetColor = vLeftText.size() - 1;
	else
		iSetColor = iLineCount;

	switch( iSide )
	{
	case -1:
		vLeftText[ iSetColor ]->SetColor( dwColor );
		vRightText[ iSetColor ]->SetColor( dwColor );
		break;
	case 0:
		vLeftText[ iSetColor ]->SetColor( dwColor );
		break;
	case 1:
		vRightText[ iSetColor ]->SetColor( dwColor );
		break;
	}
}

void ItemInfoBox::AppendLineText( std::string str, int iSide, int iLineCount )
{
	int iSetText = 0;
	if( iLineCount < 0 )
		iSetText = vLeftText.size() - 1;
	else
		iSetText = iLineCount;

	switch( iSide )
	{
		case -1:
			vLeftText[ iSetText ]->AppendText(str);
			vRightText[ iSetText ]->AppendText( str );
			break;
		case 0:
			vLeftText[ iSetText ]->AppendText( str );
			break;
		case 1:
			vRightText[ iSetText ]->AppendText( str );
			break;
	}
}

void ItemInfoBox::Init()
{
	//Load Images
	pImageMaskLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\topleft.png" );
	pImageMaskRightTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\topright.png" );
	pImageMaskLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\bottomleft.png" );
	pImageMaskRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\bottomright.png" );

	//Load Border Images
	pImageBorderLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topleft.png" );
	pImageBorderRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topright.png" );
	pImageBorderTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\top.png" );
	pImageBorderRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomright.png" );
	pImageBorderLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomleft.png" );
	pImageBorderBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottom.png" );
	pImageBorderLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\left.png" );
	pImageBorderRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\right.png" );

	//Load Icons
	pImageIconArmor					= ImageLoader::LoadImage( "Image\\SinImage\\Items\\Weapon_Knight.bmp" );
	pImageIconRobe					= ImageLoader::LoadImage( "Image\\SinImage\\Items\\Weapon_Wizard.bmp" );
	pImageIconWeaponOneHand			= ImageLoader::LoadImage( "Image\\SinImage\\Items\\Weapon_Onehand.bmp" );
	pImageIconWeaponTwoHand			= ImageLoader::LoadImage( "Image\\SinImage\\Items\\Weapon_Twohand.bmp" );

	//Load Aging Bar
	pImageAgingBar					= ImageLoader::LoadImage("image\\Sinimage\\shopall\\Aging\\Bar.bmp");
	pImageAgingBarGage				= ImageLoader::LoadImage("image\\Sinimage\\shopall\\Aging\\Bar_.bmp");
	pImageAgingBarNum				= ImageLoader::LoadImage("image\\Sinimage\\shopall\\Aging\\Aging.bmp");

	pTextAging = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	pTextAging->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_ARGB(255,215,215,215) );

	cBackgroundColor2 = UI::Color( 235, 45, 39, 27 );
	cBackgroundColor2 = UI::Color( 245, 9, 23, 22 );

	paSocketText[0][0] = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	paSocketText[0][0]->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );

	paSocketText[0][1] = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	paSocketText[0][1]->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );

	paSocketText[1][0] = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	paSocketText[1][0]->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );

	paSocketText[1][1] = std::make_shared<Text>( Rectangle2D( 0, 0, 0, 0 ) );
	paSocketText[1][1]->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );


	pRenderTargetMask	= GetGraphicsDevice()->MakeRenderTarget( 5001, 600, 600 );
	pEffectMask			= GetGraphicsDevice()->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\UI\\InfoBox.fx" );

	//Socket Images
	pImageSocketBackground			= ImageLoader::LoadImage("image\\Sinimage\\Inven\\InfoBox\\socketbg.png");

	for( int i = 0; i < 6; i++ )
		paSocket[i] = ImageLoader::LoadImage( "image\\Sinimage\\Inven\\InfoBox\\socket%d.png", i + 1 );

	for( int i = 0; i < 15; i++ )
		paSocketGems[i] = ImageLoader::LoadImage( "image\\Sinimage\\Items\\Event\\itPR5%02d.bmp", i + 1 );

	UI::ImageBox_ptr pImagePreview = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 102, 102 ) );
	pImagePreview->SetImage( UI::ImageLoader::LoadImage( "game\\images\\coinshop\\previewhair.png" ) );
	pWindowHair->AddElement( pImagePreview );

	UI::UnitMesh_ptr pUnitMeshPreview = std::make_shared<UI::UnitMesh>( Rectangle2D( 1, 1, 100, 99 ) );
	pUnitMeshPreview->SetID( UNITMESHID_HeadPreview );
	pUnitMeshPreview->SetRenderBody( FALSE );
	pWindowHair->AddElement( pUnitMeshPreview );

	pWindowHair->Hide();

	UI::ImageBox_ptr pImagePreviewBody = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 150, 190 ) );
	pImagePreviewBody->SetImage( UI::ImageLoader::LoadImage( "game\\images\\coinshop\\previewcostume.png" ) );
	pWindowBody->AddElement( pImagePreviewBody );

	UI::UnitMesh_ptr pUnitMeshPreviewBody = std::make_shared<UI::UnitMesh>( Rectangle2D( 1, 1, 148, 188 ) );
	pUnitMeshPreviewBody->SetID( UNITMESHID_BodyPreview );
	pWindowBody->AddElement( pUnitMeshPreviewBody );

	pWindowBody->Hide();
}

void ItemInfoBox::ClearCompareItem()
{
	ZeroMemory( &cItemCompare, sizeof( ItemData ) );
	bCompareItem = FALSE;
}

void ItemInfoBox::SetCompareItem( const ItemData * pcCompareItemData, BOOL bEquippedItem )
{
	CopyMemory( &cItemCompare, pcCompareItemData, sizeof( ItemData ) );
	bCompareItem = TRUE;
	bIsEquippedItem = bEquippedItem;
}

bool ItemInfoBox::IsCostumeItem( ItemData * pcItemData )
{
	return HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->IsSpecialCostume( pcItemData->sItem.sItemID.ToItemID() ) || pcItemData->sItem.bCostumeItem;
}

BOOL ItemInfoBox::IsBeginnerSet( ItemData * pcItemData )
{
	BOOL bRet = FALSE;
	switch ( pcItemData->sItem.sItemID.ToItemID() )
	{
		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerSword:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_BeginnerBracelet:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:
			bRet = TRUE;
			break;

		default:
			break;
	}

	return bRet;
}

BOOL ItemInfoBox::IsSameItem( ItemData * pcItemData )
{
	if ( (cItemWrapper.sItem.sItemID.ToItemID() == pcItemData->sItem.sItemID.ToItemID()) && (cItemWrapper.sItem.iChk1 == pcItemData->sItem.iChk1) && (cItemWrapper.sItem.iChk2 == pcItemData->sItem.iChk2) && (cItemWrapper.sPosition.Equals( &pcItemData->sPosition )) )
		return TRUE;

	return FALSE;
}

BOOL ItemInfoBox::FormatItemInfo( ItemData * pcItemData )
{
	if ( IsBeginnerSet( pcItemData ) )
	{
		AddString( 0, "Quest Item", D3DCOLOR_ARGB( 255, 132, 50, 254 ) );
		AddString( 1, "" );
	}

	if ( auto pszHeadModel = ITEMHANDLER->GetHeadModel( pcItemData->sItem.sItemID.ToInt() ) )
	{
		static EItemID eLastItemHeadID = ITEMID_None;

		if ( (eLastItemHeadID != pcItemData->sItem.sItemID.ToItemID()) || (pWindowHair->IsOpen() == FALSE) )
		{
			auto pUnitMeshPreview = pWindowHair->GetElement<UI::UnitMesh>( UNITMESHID_HeadPreview );
			pUnitMeshPreview->FreeUnit();
			pUnitMeshPreview->SetRenderBody( FALSE );
			pUnitMeshPreview->PrepareUnitData( UNITDATA );
			pUnitMeshPreview->SetHeadModel( ITEMHANDLER->GetHeadModel( pcItemData->sItem.sItemID.ToInt() ) );
			pUnitMeshPreview->SetCameraPosition( Point3D( pUnitMeshPreview->GetCameraPosition().iX, pUnitMeshPreview->GetCameraPosition().iY + (1 << 8), pUnitMeshPreview->GetCameraPosition().iZ ) );
			pWindowHair->Show();

			eLastItemHeadID = pcItemData->sItem.sItemID.ToItemID();
		}
	}
	else
		pWindowHair->Hide();

	if ( auto pszArmorPattern = ITEMHANDLER->GetArmorPatternName( UNITDATA, pcItemData->sItem.sItemID.ToInt() ) )
	{
		static EItemID eLastItemBodyID = ITEMID_None;

		int iGender = ITEMHANDLER->GetItemGender( pcItemData->sItem.sItemID.ToInt() );
		if ( (iGender == UNITDATA->GetCharacterGender()) || (iGender == 0) )
		{
			if ( (eLastItemBodyID != pcItemData->sItem.sItemID.ToItemID()) || (pWindowBody->IsOpen() == FALSE) )
			{
				auto pUnitMeshPreview = pWindowBody->GetElement<UI::UnitMesh>( UNITMESHID_BodyPreview );
				pUnitMeshPreview->FreeUnit();
				pUnitMeshPreview->SetRenderBody( TRUE );
				pUnitMeshPreview->PrepareUnitData( UNITDATA );
				RENDERER->SetUseNewRenderToLoad( true );
				pUnitMeshPreview->SetHeadModel( UNITDATA->sCharacterData.Player.szHeadModel );
				pUnitMeshPreview->SetBodyModel( ITEMHANDLER->GetArmorPatternName( UNITDATA, pcItemData->sItem.sItemID.ToInt() ) );
				RENDERER->ResetUseNewRenderToLoad();
				pUnitMeshPreview->SetCameraPosition( Point3D( pUnitMeshPreview->GetCameraPosition().iX, pUnitMeshPreview->GetCameraPosition().iY - (2 << 8), pUnitMeshPreview->GetCameraPosition().iZ ) );
				pWindowBody->Show();

				eLastItemBodyID = pcItemData->sItem.sItemID.ToItemID();
			}
		}
		else
			pWindowBody->Hide();
	}
	else
		pWindowBody->Hide();

	if ( pcItemData->sItem.eCraftType == ITEMCRAFTTYPE_Mixing )
	{
		switch ( pcItemData->sItem.eMixTypeName )
		{
			case MIXTYPENAME_Devine:
				AddString( 0, "Devine Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Celesto:
				AddString( 0, "Celesto Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Mirage:
				AddString( 0, "Mirage Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Inferna:
				AddString( 0, "Inferna Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Enigma:
				AddString( 0, "Enigma Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Bellum:
				AddString( 0, "Bellum Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Oredo:
				AddString( 0, "Oredo Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;

			case MIXTYPENAME_Devine2:
				AddString( 0, "Devine Mix #2", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Celesto2:
				AddString( 0, "Celesto Mix #2", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Mirage2:
				AddString( 0, "Mirage Mix #2", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Inferna2:
				AddString( 0, "Inferna Mix #2", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Enigma2:
				AddString( 0, "Enigma Mix #2", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Bellum2:
				AddString( 0, "Bellum Mix #2", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
			 break;
			case MIXTYPENAME_Oredo2:
				AddString( 0, "Oredo Mix #2", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;

			case MIXTYPENAME_BellumDefense:
				AddString( 0, "Bellum Defense Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_BellumBlock:
				AddString( 0, "Bellum Block Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_BellumVelocity:
				AddString( 0, "Bellum Velocity Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_BellumPower:
				AddString( 0, "Bellum Power Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_BellumCritical:
				AddString( 0, "Bellum Critical Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_BellumRating:
				AddString( 0, "Bellum Rating Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_BellumVitality:
				AddString( 0, "Bellum Vitality Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoSwiftness:
				AddString( 0, "Oredo Swiftness Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoPower:
				AddString( 0, "Oredo Power Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoCritical:
				AddString( 0, "Oredo Critical Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoDefense:
				AddString( 0, "Oredo Defense Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoBlock:
				AddString( 0, "Oredo Block Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoRating:
				AddString( 0, "Oredo Rating Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoVitality:
				AddString( 0, "Oredo Vitality Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoVelocity:
				AddString( 0, "Oredo Velocity Mix", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoEfficiency1:
				AddString( 0, "Oredo Efficiency Mix #1", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
			case MIXTYPENAME_OredoEfficiency2:
				AddString( 0, "Oredo Efficiency Mix #2", D3DCOLOR_ARGB( 255, 170, 160, 170 ) );
				AddString( 1, "" );
				break;
		}
	}
	if( pcItemData->sItem.sItemID.ToItemID() == ITEMID_ValentoRing )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_KelvezuAmulet )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_DraxosBoots )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if( pcItemData->sItem.sItemID.ToInt() == ITEMID_MokovaBoots )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_FurySheltom )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_ThroneSheltom)
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");
	}
	if (pcItemData->sItem.sItemID.ToItemID() == ITEMID_ThroneAmulet )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255 ) );
		AddString( 1, "");
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_TullaAmulet )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_FallenRing )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_DarkGuardianSheltom )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_ArgonianGauntlets )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_GreedyGauntlets )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_DevilShyRing )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_ThroneRing )
	{
		AddString(0, "Boss Item", D3DCOLOR_RGBA(0, 128, 128, 255));
		AddString(1, "");
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_GorgonyteEarring )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_VolcanosEarring )
	{
		AddString( 0, "Boss Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
		AddString( 1, "" );
	}
	if ( IsCostumeItem( pcItemData ) )
	{
		AddString( 0, "Skin Item", D3DCOLOR_RGBA( 255, 105, 180, 255 ) );
		AddString( 1, "" );
	}
	if ( ((pcItemData->sItem.sItemID.ToInt() & 0xFFFF0000) == ITEMID_RuneCodeManufacture1) || (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Crafting2) )
	{
		AddString( 0, "Craft Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_SilverLockedChest )
	{
		AddString( 0, "Action Field Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_GoldenLockedChest )
	{
		AddString( 0, "Action Field Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_BlueLockedChest )
	{
		AddString( 0, "Action Field Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_PurpleLockedChest )
	{
		AddString( 0, "Action Field Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_TropicalScroll )
	{
		AddString( 0, "Action Field Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_ChestKey )
	{
		AddString( 0, "Action Field Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_GhostBoots )
	{
		AddString( 0, "Ghost Castle Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_GhostGauntlets )
	{
		AddString( 0, "Ghost Castle Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_GhostBracelets )
	{
		AddString( 0, "Ghost Castle Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_GhostCastleBracelets )
	{
		AddString( 0, "Ghost Castle Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_ActionFieldPass )
	{
		AddString( 0, "Ghost Castle Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_ActionFieldPassFD )
	{
		AddString( 0, "Tropical Island Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_ActionFieldPassDD )
	{
		AddString( 0, "Dragon's Dungeon Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_TropicalBracelets )
	{
		AddString( 0, "Tropical Island Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( pcItemData->sItem.sItemID.ToInt() == ITEMID_BrilliantStone )
	{
		AddString( 0, "Bellatra Event Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		AddString( 1, "" );
	}
	if ( PERFECTVIEWHANDLER->IsSet() )
	{
		AddString( 0, "Perfect Item", D3DCOLOR_RGBA( 0, 191, 255, 255 ) );
		AddString( 1, "" );
	}
	if( pcItemData->sItem.bPerfectItem )
	{
		AddString( 0, "Perfect Item", D3DCOLOR_RGBA( 0, 191, 255, 255 ) );
		AddString( 1, "" );
	}
	if( bIsEquippedItem && bCompareItem )
	{
		AddString( 0, "Equipped Item", D3DCOLOR_RGBA( 70, 125, 80, 255 ) );
		AddString( 1, "" );
	}

	int iGender = ITEMHANDLER->GetItemGender( pcItemData->sItem.sItemID.ToInt() );
	if ( iGender != CHARACTERGENDER_None )
	{
		if ( iGender == CHARACTERGENDER_Male )
			AddString( 0, "Male Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );
		else
			AddString( 0, "Female Item", D3DCOLOR_RGBA( 255, 155, 50, 255 ) );

		AddString( 1, "" );
	}

	//Break Line
	AddString( 0, " " );
	AddString( 1, " " );

	EItemID eItemID = pcItemData->sBaseItemID.ToItemID();
	switch( eItemID )
	{
		case ITEMID_HopyCostumeTime:
		case ITEMID_CuepyCostumeTime:
/*			AddString( 0, "Easter Event Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
			AddString( 1, "" );
			AddString( 0, " " );
			AddString( 1, " " ); */
			break;
		case ITEMID_EasterHat:
			AddString( 0, "Easter Event Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
			AddString( 1, "" );
			AddString( 0, "Gives +2 Movement Speed\nGives +5% EXP", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ChemistHat:
			AddString( 0, "Event Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
			AddString( 1, "" );
			AddString( 0, "Gives +1 Extra drop", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_PatronumHat:
		case ITEMID_GoldenHopyHat:
			AddString( 0, "Event Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
			AddString( 1, "" );
			AddString( 0, "Gives +3% Critical", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ObscureHat:
			AddString( 0, "Event Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
			AddString( 1, "" );
			AddString( 0, "Gives +5% Attack Power", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_PartyCore:
			AddString( 0, "Right click to choose a\nParty Member and teleport to him", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ColorfulXmasTree:
		case ITEMID_SnowyXmasTree:
			AddString( 0, "Right click to place this Xmas Tree", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SoulCrystal:
			AddString( 0, "Right click to activate.\nKill a monster while the crystal\nis active so that its soul\nbecomes trapped inside it", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			AddString( 0, "" );
			AddString( 1, "" );
			break;
		case ITEMID_SoulSummonCrystal:
			AddString( 0, "Right click to spawn the soul\ntrapped inside the crystal.\nTrapped monster:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			AddString( 0, pcItemData->sItem.szItemName, D3DCOLOR_ARGB( 255, 0, 191, 255 ), TRUE );
			AddString( 1, "" );
			AddString( 0, "" );
			AddString( 1, "" );
			break;
		case ITEMID_ActionFieldPass:
			AddString( 0, "You can enter one more time on\nGhost Castle in the same day\nwith this item!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_TropicalScroll:
			AddString( 0, "You can use this item to revive at\nTropical Island without\nlose exp!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ActionFieldPassFD:
			AddString( 0, "You can enter one more time on\nTropical Island in the same day\nwith this item!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ActionFieldPassDD:
			AddString( 0, "You can enter one more time on\nDragon's Dungeon in the same day\nwith this item!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_LightPurpleRune:
			AddString( 0, "Used to Craft Devil Shy Ring", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_LightSilverRune:
			AddString( 0, "Used to Craft Volcanos Earrings", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ThroneYellowRune:
			AddString(0, "Used to Craft Throne Ring", D3DCOLOR_ARGB(255, 245, 240, 218 ) );
			AddString(1, "");
			break;
		case ITEMID_ThroneRedRune:
			AddString(0, "Used to Craft Throne Amulet", D3DCOLOR_ARGB(255, 245, 240, 218 ) );
			AddString(1, "");
			break;
		case ITEMID_ThroneGreenRune:
			AddString(0, "Used to Craft Throne Sheltom", D3DCOLOR_ARGB(255, 245, 240, 218 ) );
			AddString(1, "");
			break;
		case ITEMID_LightBlueRune:
			AddString( 0, "Used to Craft Tulla Amulet", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DarkRedRune:
			AddString( 0, "Used to Craft Draxos Boots", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_LightRedRune:
			AddString( 0, "Used to Craft Tropical Bracelets", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_LightGreenRune:
			AddString( 0, "Used to Craft Abyss Ring", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_LightOrangeRune:
			AddString( 0, "Used to Craft Greedy Gauntlets", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_GreenCrystalRune:
		case ITEMID_RedCrystalRune:
		case ITEMID_BlueCrystalRune:
			AddString( 0, "Used to Craft Fury Sheltom", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_GoldCrystalRune:
		case ITEMID_SilverCrystalRune:
		case ITEMID_BronzeCrystalRune:
			AddString(0, "Used to Craft Throne Sheltom", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case ITEMID_BrilliantStone:
			AddString( 0, "Exchange on Ihin\nBellatra Event NPC", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BlueStone:
			AddString( 0, "Reset all stat points.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_RedStone:
			AddString( 0, "Reset all Skills points.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_GreenStone:
			AddString( 0, "Reset all Skills and\n Stats points.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_FatalEdge:
			AddString( 0, "+10% Final Damage\nOnly works on SoD", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_AvertScroll:
			AddString( 0, "+10% of Absorption\nOnly works on SoD", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MatureStoneA:
			AddString( 0, "Mature the aging bar of \nequipped Weapon \nto 100% instantly", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MatureStoneB:
			AddString( 0, "Mature the aging bar of \nequipped Orb or Shield \nto 100% instantly", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MatureStoneC:
			AddString( 0, "Mature the aging bar of \nequipped Robe or Armor \nto 100% instantly", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SapphireStone:
			AddString( 0, "Train all skills from T1\nto T3 to 100% instantly", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_RubyStone:
			AddString( 0, "Train all skills from T1\nto T3 to 10%\nReq. Level: 1-95", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_CaravanHopyCrystal:
		case ITEMID_CaravanRudolphCrystal:
		case ITEMID_CaravanArmaCrystal:
		case ITEMID_CaravanBumaCrystal:
		case ITEMID_CaravanCuepyCrystal:
		case ITEMID_CaravanHungkyCrystal:
		case ITEMID_CaravanRabieCrystal:
		case ITEMID_CaravanTobieCrystal:
		case ITEMID_CaravanSirKnightCrystal:
		case ITEMID_CaravanPinguCrystal:
		case ITEMID_CaravanHopyCheapCrystal:
		case ITEMID_CaravanYetiCrystal:
		case ITEMID_CaravanGoldenHopyCrystal:
			AddString( 0, "Carry items for you", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ExpUp1H:
		case ITEMID_ExpUp3H:
		case ITEMID_ExpUp24H:
		case ITEMID_ExpUp48H:
		case ITEMID_ExpUp72H:
			AddString( 0, "30% Additional Experience", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_HPUp12H:
		case ITEMID_HPUp24H:
		case ITEMID_HPUp48H:
		case ITEMID_HPUp72H:
			AddString( 0, "+10 HP Regen", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MPUp12H:
		case ITEMID_MPUp24H:
		case ITEMID_MPUp48H:
		case ITEMID_MPUp72H:
			AddString( 0, "+10 Mana Regen", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MPDown12H:
		case ITEMID_MPDown24H:
		case ITEMID_MPDown48H:
		case ITEMID_MPDown72H:
			AddString( 0, "Reduces 25% Mana Usage", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SPDown12H:
		case ITEMID_SPDown24H:
		case ITEMID_SPDown48H:
		case ITEMID_SPDown72H:
			AddString( 0, "Reduces 25% Stamina Usage", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_TuttiFruttiHat:
			AddString( 0, "Carnival Item", D3DCOLOR_ARGB( 255, 120, 255, 120 ) );
			AddString( 1, "" );
			AddString( 0, "+6% EXP", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_CarnivalLetterA:
		case ITEMID_CarnivalLetterC:
		case ITEMID_CarnivalLetterI:
		case ITEMID_CarnivalLetterL:
		case ITEMID_CarnivalLetterN:
		case ITEMID_CarnivalLetterR:
		case ITEMID_CarnivalLetterV:
		case ITEMID_CarnivalApple:
		case ITEMID_CarnivalBanana:
		case ITEMID_CarnivalJabuticaba:
		case ITEMID_CarnivalMaracuja:
		case ITEMID_CarnivalPitanga:
		case ITEMID_CarnivalStrawberry:
		case ITEMID_PCandyAxe:
		case ITEMID_PCandyDagger:
		case ITEMID_PCandyHammer:
		case ITEMID_PCandyWand:
		case ITEMID_PCandyPhantom:
		case ITEMID_PCandyScythe:
		case ITEMID_PCandyBow:
		case ITEMID_PCandySword:
		case ITEMID_PCandyJavelin:
		case ITEMID_PCandyShield:
			AddString( 0, "Carnival Item", D3DCOLOR_ARGB( 255, 120, 255, 120 ) );
			AddString( 1, "" );
			break;

		case ITEMID_WitchHat:
		case ITEMID_CartolaHat:
		case ITEMID_HalloweenHat:
			AddString( 0, "Halloween Item", D3DCOLOR_ARGB( 255, 180, 100, 175 ) );
			AddString( 1, "" );
			AddString( 0, "+10 HP Regen\n+10 Add HP", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;

		case ITEMID_PinguMountMysterious:
			AddString( 0, "Halloween Item", D3DCOLOR_ARGB( 255, 180, 100, 175 ) );
			AddString( 1, "" );
			AddString( 0, "A cutie mount for you!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;

		case ITEMID_MountObscureHog:
		case ITEMID_MountSnowflakeUnicorn:
		case ITEMID_MountHog:
		case ITEMID_MountHog1:
		case ITEMID_MountHopy:
		case ITEMID_MountHopy1:
		case ITEMID_MountHopy2:
		case ITEMID_MountHorse:
		case ITEMID_MountHorse1:
		case ITEMID_MountHorse2:
		case ITEMID_MountLion1:
		case ITEMID_MountLion5:
		case ITEMID_MountUnicorn3:
		case ITEMID_MountPingu1:
		case ITEMID_MountGhostWolf:
		case ITEMID_MountHorsePTM:
		case ITEMID_MountSnowdeer:
		case ITEMID_MountRudolf:
		case ITEMID_MountWhiteDeer:
		case ITEMID_MountIronCharger:
		case ITEMID_MountMecaballus:
		case ITEMID_MountZira:
		case ITEMID_MountIceTiger:
		case ITEMID_MountUnicorn6:
		case ITEMID_MountRabie:
		case ITEMID_MountRaptor:
		case ITEMID_MountRaptor1:
		case ITEMID_MountRaptor2:
		case ITEMID_MountRaptor3:
			AddString( 0, "Mount Item", D3DCOLOR_ARGB( 255, 180, 100, 175 ) );
			AddString( 1, "" );
			AddString( 0, "A cutie mount for you!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;

		case ITEMID_VampireSuit:
		case ITEMID_Succubus:
		case ITEMID_HalloweenAxe:
		case ITEMID_HalloweenClaw:
		case ITEMID_HalloweenDagger:
		case ITEMID_HalloweenHammer:
		case ITEMID_HalloweenStaff:
		case ITEMID_HalloweenWand:
		case ITEMID_HalloweenPhantom:
		case ITEMID_HalloweenScythe:
		case ITEMID_HalloweenBow:
		case ITEMID_HalloweenSword:
		case ITEMID_HalloweenBladeSword:
		case ITEMID_HalloweenJavelin:
		case ITEMID_HalloweenShield:
		case ITEMID_RottenLantern:
		case ITEMID_EvilLantern:
		case ITEMID_InfernalLantern:
			AddString( 0, "Halloween Item", D3DCOLOR_ARGB( 255, 180, 100, 175 ) );
			AddString( 1, "" );
			break;

		case ITEMID_IceCostumeMale:
		case ITEMID_IceCostumeFemale:
		case ITEMID_GoldenHopyCostumeMale:
		case ITEMID_GoldenHopyCostumeFemale:
		case ITEMID_GoldenHopyCostumeMaleTime:
		case ITEMID_GoldenHopyCostumeFemaleTime:
			AddString( 0, "Event Item", D3DCOLOR_RGBA( 0, 128, 128, 255 ) );
			AddString( 1, "" );
			break;

		case ITEMID_XmasCostumeOldAxe:
		case ITEMID_XmasCostumeOldAxe1Hand:
		case ITEMID_XmasCostumeOldClaw:
		case ITEMID_XmasCostumeOldDagger:
		case ITEMID_XmasCostumeOldHammer:
		case ITEMID_XmasCostumeOldStaff:
		case ITEMID_XmasCostumeOldPhantom:
		case ITEMID_XmasCostumeOldScythe:
		case ITEMID_XmasCostumeOldBow:
		case ITEMID_XmasCostumeOldSword:
		case ITEMID_XmasCostumeOldSword1Hand:
		case ITEMID_XmasCostumeOldJavelin:

		case ITEMID_XmasCostumeAxe:
		case ITEMID_XmasCostumeAxe1Hand:
		case ITEMID_XmasCostumeClaw:
		case ITEMID_XmasCostumeDagger:
		case ITEMID_XmasCostumeHammer:
		case ITEMID_XmasCostumeStaff:
		case ITEMID_XmasCostumeWand:
		case ITEMID_XmasCostumePhantom:
		case ITEMID_XmasCostumeScythe:
		case ITEMID_XmasCostumeBow:
		case ITEMID_XmasCostumeSword:
		case ITEMID_XmasCostumeSword1Hand:
		case ITEMID_XmasCostumeJavelin:
		case ITEMID_XmasCostumeShield:
			AddString( 0, "Christmas Item", D3DCOLOR_ARGB( 255, 255, 100, 100 ) );
			AddString( 1, "" );
			break;

		case ITEMID_KingStone:
			if ( pcItemData->sItem.eMixTypeName == ITEMCRAFTTYPE_TGFKingFury )
			{
				AddString( 0, "Hagios Item", D3DCOLOR_ARGB( 255, 255, 248, 0 ) );
				AddString( 1, "" );
			}
			else if ( pcItemData->sItem.eMixTypeName == ITEMCRAFTTYPE_TGFLordBabel )
			{
				AddString( 0, "King Fury Item", D3DCOLOR_ARGB( 255, 255, 248, 0 ) );
				AddString( 1, "" );
			}
			AddString( 0, FormatString( "Killed in %d Minute%s", pcItemData->sItem.sQuestMonsters.sCur, pcItemData->sItem.sQuestMonsters.sCur > 1 ? "s" : "" ), D3DCOLOR_ARGB( 255, 125, 180, 175 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ThirdEye1H:
		case ITEMID_ThirdEye3H:
		case ITEMID_ThirdEye24H:
		case ITEMID_ThirdEye48H:
		case ITEMID_ThirdEye72H:
			AddString( 0, "One Additional Drop", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_PhoenixEgg:
			AddString( 0, "Summons a random \nPhoenix Pet", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_GravityStone72H:
			AddString( 0, "Gives +1000 Weight", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_TeleportCore:
			AddString( 0, "Teleport to a field", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_AgingStone:
			AddString( 0, "Decrease 10% chance\nof fail aging", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_RipeStone:
			AddString( 0, "Decrease 20% chance\nof fail aging", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_FluenceStone:
			AddString( 0, "Decrease 30% chance\nof fail aging", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ArcaneStone:
			AddString( 0, "Decrease 40% chance\nof fail aging", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_CopperOre:
			AddString( 0, "Prevents Items \nfrom breaking", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_CleanItem:
			AddString( 0, "Cleans Mix/Age \nfrom an item", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_Fireworks:
			AddString( 0, "Lets Celebrate!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_HairPotionA:
		case ITEMID_HairPotionB:
		case ITEMID_HairPotionC:
		case ITEMID_HairPotionD:
		case ITEMID_HairPotionE:
		case ITEMID_HairPotionF:
		case ITEMID_HairPotionG:
		case ITEMID_HairPotionH:
		case ITEMID_HairPotionI:
		case ITEMID_HairPotionJ:
		case ITEMID_HairPotionK:
		case ITEMID_HairPotionL:
		case ITEMID_HairPotionM:
		case ITEMID_HairPotionN:
		case ITEMID_HairPotionO:
		case ITEMID_HairPotionP:
		case ITEMID_HairPotionQ:
		case ITEMID_HairPotionR:
		case ITEMID_HairPotionS:
		case ITEMID_IceHair:
		case ITEMID_SamuraiHair:
			AddString( 0, "Change your hair!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadHappyness:
			AddString( 0, "Add +50 Attack Rating", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadSadness:
			AddString( 0, "Add +10 MP Regen/MP", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadShyness:
			AddString( 0, "Add +100 Defense", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadLove:
			AddString( 0, "Add +10 HP Regen/HP", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadAngry:
			AddString( 0, "Add +50 HP/MP/STM", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadSurprised:
			AddString( 0, "Add +2% Block Rating", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BigHeadSensual:
			AddString( 0, "Add +2 Critical", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_TopazStone:
			AddString( 0, "Mature the equipped\nitem to 100% instantly", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_TurquoiseStone:
			AddString( 0, "A weak mature stone", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_EmeraldStone:
			AddString( 0, "Trains 10% \nAll skills T1~T3", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SheepHat:
			AddString( 0, "+150 Defense", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_GiraffeHat:
			AddString( 0, "Add +75 Attack Rating", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SoccerHat:
			AddString( 0, "Add +2 Move Speed", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_XMasRedHat:
			AddString( 0, "Add +50 MP/STM", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_FeatherTier1:
			AddString( 0, "Reduces cooldown of\nTier 1 skills by 10%", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_FeatherTier2:
			AddString( 0, "Reduces cooldown of\nTier 2 skills by 10%", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_FeatherTier3:
			AddString( 0, "Reduces cooldown of\nTier 3 skills by 10%", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_FeatherTier4:
			AddString( 0, "Reduces cooldown of\nTier 4 skills by 10%", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_FeatherTier5:
			AddString( 0, "Reduces cooldown of\nTier 5 skills by 10%", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_HPBooster:
			AddString( 0, "Add +15% HP", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MPBooster:
			AddString( 0, "Add +15% MP", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_STMBooster:
			AddString( 0, "Add +15% STM", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_RicartenCore:
			AddString( 0, "Teleport to Ricarten Town", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_NaviskoCore:
			AddString( 0, "Teleport to Navisko Town", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_AtlantisCore:
			AddString( 0, "Teleport to Atlantis Town", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_PillaiCore:
			AddString( 0, "Teleport to Phillai Town", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_UnionCore:
			AddString( 0, "Teleport to a Player", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			if( cItemWrapper.sItem.szItemName[ lstrlenA( cItemWrapper.sItem.szItemName ) + 1 ] != 0 )
			{
				AddString( 0, FormatString( "%s", cItemWrapper.sItem.szItemName + lstrlenA( cItemWrapper.sItem.szItemName ) + 1 ), D3DCOLOR_ARGB( 255, 110, 165, 250 ) );
				AddString( 1, "" );
			}
			break;
		case ITEMID_MightPotionFighter:
			AddString( 0, "5% Damage Boost\nagainst Fighter\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionMechanician:
			AddString( 0, "5% Damage Boost\nagainst Mechanician\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionPikeman:
			AddString( 0, "5% Damage Boost\nagainst Pikeman\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionArcher:
			AddString( 0, "5% Damage Boost\nagainst Archer\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionKnight:
			AddString( 0, "5% Damage Boost\nagainst Knight\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionAtalanta:
			AddString( 0, "5% Damage Boost\nagainst Atalanta\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionMagician:
			AddString( 0, "5% Damage Boost\nagainst Magician\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_MightPotionPriestess:
			AddString( 0, "5% Damage Boost\nagainst Priestess\n", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DeadlyEdge:
			AddString( 0, "Add critical rating by +5% \nin PVP areas", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_AvailOfEvade:
			AddString( 0, "Add evasion rating by +5% \nin PVP areas", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BolsterRecovery:
			AddString( 0, "HP MP SP Recovery\nfor party", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_Restoration:
			AddString( 0, "Resurrection Scroll", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DefianceStoneFire:
			AddString( 0, "Decrease Damage by 50% \nof Fire Crystal Tower", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Fire Attribute", D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DefianceStoneIce:
			AddString( 0, "Decrease Damage by 50% \nof Ice Crystal Tower", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Ice Attribute", D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			AddString( 1, "" );
			break;
		case ITEMID_DefianceStoneLight:
			AddString( 0, "Decrease Damage by 50% \nof Lightning Crystal Tower", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Duration: 60seconds", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );

			AddString( 0, "Light Attribute", D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			AddString( 1, "" );
			break;
		case ITEMID_GoldenLockedChest:
			AddString( 0, "Open it to get an \nexcellent reward!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_SilverLockedChest:
			AddString( 0, "Open it to get an \nexcellent reward!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_BlueLockedChest:
			AddString( 0, "Open it to get an \nexcellent reward!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_PurpleLockedChest:
			AddString( 0, "Open it to get an \nexcellent reward!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_ChestKey:
			AddString( 0, "Use this key to open \na Locked Chest.", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
		case ITEMID_GlamorousAttireGlaze:
			AddString( 0, "Right click and aim on an armor\nor robe to turn into a skin", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, "" );
			break;
        case ITEMID_GlamorousArmamentVarnish:
            AddString( 0, "Right click and aim on a weapon\nor shield to turn into a skin", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
            AddString( 1, "" );
            break;
        case ITEMID_SecretRecipe:
            AddString( 0, "Craft this recipe with 3 Bellums\nto get a secret reward!", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
            AddString( 1, "" );
            break;
		case ITEMID_LucidyForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 2 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 500 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_SerenoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 4 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 500 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_FadeoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 7 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 500 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_SparkyForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 500 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_RaidentForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 15 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 700 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_TransparoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 25 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 0 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 900 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_MurkyForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 40 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 1800 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_DevineForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 60 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 2400 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_CelestoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 80 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 3600 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_MirageForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 100 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 4800 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_InfernaForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 120 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 6000 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_EnigmaForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 140 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 7200 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_BellumForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 160 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 8400 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_OredoForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 180 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 9600 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_SapphireForce:
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", 200 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Add Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d%%", 10 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			AddString( 0, "Duration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%ds", 10800 ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
			break;
		case ITEMID_SolForce:
			AddString(0, "Add Attack Power:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 220), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Add Attack Power:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 10), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duration:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 12000), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case ITEMID_SoccerShirt:
			AddString( 0, "Won't work with costumes", D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			AddString( 1, "" );
			
			AddString( 0, " " );
			AddString( 1, " " );
			break;


	}

	return TRUE;
}

DWORD ItemInfoBox::GetItemPlayTime( Item * psItem )
{
	DWORD dwTime = 0;
	switch ( psItem->sItemID.ToItemID() )
	{
		case ITEMID_BroyaleCostumeMale:
		case ITEMID_BroyaleCostumeFemale:
			dwTime = (60 * 60 * 24 * 15);
			break;

		case ITEMID_GreenCrystalRune:
		case ITEMID_RedCrystalRune:
		case ITEMID_BlueCrystalRune:
		case ITEMID_GoldCrystalRune:
		case ITEMID_SilverCrystalRune:
		case ITEMID_BronzeCrystalRune:
		case ITEMID_PinguMountMysterious:
			dwTime = (60 * 60 * 24 * 30);
			break;

			// 7 Days
		// Beginner Set
		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerSword:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerBracelet:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:

		//Xmas
		case ITEMID_XMasElegantCostumeM:
		case ITEMID_XMasElegantCostumeF:
		case ITEMID_XMasCloatCostumeTM:
		case ITEMID_XMasCloatCostumeTF:
		case ITEMID_XMasCloatCostumeMM:
		case ITEMID_XMasCloatCostumeMF:
		
		//Halloween
		case ITEMID_VampireSuit:
		case ITEMID_Succubus:
		case ITEMID_HalloweenAxe:
		case ITEMID_HalloweenClaw:
		case ITEMID_HalloweenDagger:
		case ITEMID_HalloweenHammer:
		case ITEMID_HalloweenStaff:
		case ITEMID_HalloweenWand:
		case ITEMID_HalloweenPhantom:
		case ITEMID_HalloweenScythe:
		case ITEMID_HalloweenBow:
		case ITEMID_HalloweenSword:
		case ITEMID_HalloweenBladeSword:
		case ITEMID_HalloweenJavelin:
		case ITEMID_HalloweenShield:

		//Easter
//		case ITEMID_HopyCostumeTime:
//		case ITEMID_CuepyCostumeTime:

		case ITEMID_XmasCostumeOldAxe:
		case ITEMID_XmasCostumeOldAxe1Hand:
		case ITEMID_XmasCostumeOldClaw:
		case ITEMID_XmasCostumeOldDagger:
		case ITEMID_XmasCostumeOldHammer:
		case ITEMID_XmasCostumeOldStaff:
		case ITEMID_XmasCostumeOldPhantom:
		case ITEMID_XmasCostumeOldScythe:
		case ITEMID_XmasCostumeOldBow:
		case ITEMID_XmasCostumeOldSword:
		case ITEMID_XmasCostumeOldSword1Hand:
		case ITEMID_XmasCostumeOldJavelin:

		case ITEMID_CandyAxe:	
		case ITEMID_CandyAxe1Hand:
		case ITEMID_CandyClaw:
		case ITEMID_CandyDagger:		
		case ITEMID_CandyHammer:	
		case ITEMID_CandyStaff:	
		case ITEMID_CandyWand:	
		case ITEMID_CandyPhantom:		
		case ITEMID_CandyScythe:	
		case ITEMID_CandyBow:		
		case ITEMID_CandySword:		
		case ITEMID_CandySword1Hand:
		case ITEMID_CandyJavelin:	
		case ITEMID_CandyShield:	
		case ITEMID_CandySheltom:
		case ITEMID_CeruleanRing:
		case ITEMID_IceCostumeMale:
		case ITEMID_IceCostumeFemale:
		case ITEMID_GoldenHopyCostumeMaleTime:
		case ITEMID_GoldenHopyCostumeFemaleTime:
		case ITEMID_IceRecipe:
		case ITEMID_MysteriousBookRecipe:
		case ITEMID_HopyRune:
		case ITEMID_CuepyRune:
		case ITEMID_GoldenHopyRune:
			dwTime = (60 * 60 * 24 * 7);
			break;

		case ITEMID_HopyCostumeTime:
		case ITEMID_CuepyCostumeTime:
		case ITEMID_HopyCostumeIce:
		case ITEMID_CuepyCostumeIce:
		case ITEMID_ChemistCostumeMale:
		case ITEMID_ChemistCostumeFemale:
		case ITEMID_SandurrRedCostumeMale:
		case ITEMID_SandurrRedCostumeFemale:
			dwTime = (60 * 60 * 24);
			break;

		case ITEMID_GoldenLockedChest:
		case ITEMID_SilverLockedChest:
		case ITEMID_BlueLockedChest:
		case ITEMID_PurpleLockedChest:
		case ITEMID_ChestKey:
		case ITEMID_KingStone:
			dwTime = (60 * 60);
			break;

		default:
			break;
	}

	if ( (dwTime == 0) && psItem->bLockedItemType )
	{
		switch ( psItem->bLockedItemType )
		{
			case ITEMLOCKEDTYPE_1Hour:
				dwTime = (60 * 60);
				break;
			case ITEMLOCKEDTYPE_30Days:
				dwTime = (60 * 60 * 24 * 30);
				break;

			default:
				break;
		}

		if ( dwTime )
		{
			if ( PLAYTIME > (psItem->tTime + dwTime) )
			{
				if ( psItem->bLockedItemType != ITEMLOCKEDTYPE_None )
				{
					psItem->bLockedItemType = ITEMLOCKEDTYPE_None;

					dwTime = 0;
				}
			}
		}
	}

	return dwTime;
}

BOOL ItemInfoBox::IsExpiredItemTime( Item * psItemInfo )
{
	DWORD dwTime = ItemInfoBox::GetItemPlayTime( psItemInfo );
	
	if ( dwTime && psItemInfo->szItemName[0] != 0 )
	{
		if ( psItemInfo->bLockedItemType )
		{
			if ( PLAYTIME > (psItemInfo->tTime + dwTime) )
				psItemInfo->bLockedItemType = ITEMLOCKEDTYPE_None;

			return FALSE;
		}

		if ( PLAYTIME > (psItemInfo->tTime + dwTime) )
			return TRUE;
	}
	
	return FALSE;
}

BOOL ItemInfoBox::FormatItemTimeInfo( ItemData * pcItemData )
{
	DWORD dwTime		= 0;
	DWORD dwItemTime	= pcItemData->sItem.tTime;
	BOOL bResult		= FALSE;

	DWORD dwTotalTime = ItemInfoBox::GetItemPlayTime( &pcItemData->sItem );

	if ( dwTotalTime )
	{
		if ( ItemInfoBox::IsExpiredItemTime( &pcItemData->sItem ) )
		{
			AddString( 0, "" );
			AddString( 1, "" );
			AddString( 0, std::string( AY_OBFUSCATE( "Expired Item" ) ), D3DCOLOR_ARGB( 255, 125, 180, 175 ) );
			AddString( 1, "\n " );
		}
		else
		{
			AddString( 0, "" );
			AddString( 1, "" );
			dwTime = dwTotalTime - (PLAYTIME - dwItemTime);

			if ( pcItemData->sItem.bLockedItemType )
				AddString( 0, FormatString( AY_OBFUSCATE( "Time Left to unlock: %02d:%02d:%02d" ), dwTime / 3600, (dwTime / 60) % 60, dwTime % 60 ), D3DCOLOR_ARGB( 255, 125, 180, 175 ) );
			else	
				AddString( 0, FormatString( AY_OBFUSCATE( "Time Left: %02d:%02d:%02d" ), dwTime / 3600, (dwTime / 60) % 60, dwTime % 60 ), D3DCOLOR_ARGB( 255, 125, 180, 175 ) );

			AddString( 1, "\n " );
		}
		bResult = TRUE;
	}

	return bResult;
}

BOOL ItemInfoBox::PrepareShowItem( ItemData * pcItemData, BOOL bShowBuyPrice, BOOL bShowSellPrice, BOOL bShowChecksums )
{
	//Clear Vectors
	vLeftText.clear();
	vRightText.clear();
	vCompareText.clear();
	vRowCompare.clear();
	iSocketCount = 0;
	bIsPerfectItem = FALSE;
	bCantUseItem = FALSE;
	cBackgroundColor1 = UI::Color( 235, 45, 39, 27 );

	// Item Data Copy
	{
		if ( pcItemData->iItemSlotFlag == ITEMSLOTFLAG_LeftRightHand && pcItemData->iItemSlot && !pcItemData->sItem.bCostumeItem )
		{
			CopyMemory( (void*)pdwItemInfoData, &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1].sItem, sizeof( Item ) );
			CopyMemory( &cItemWrapper, &INVENTORYITEMS[INVENTORYITEMSLOT[0].iItemIndex - 1], sizeof( ItemData ) );

			cItemWrapper.sPosition = pcItemData->sPosition;
		}
		else
		{
			CopyMemory( (void*)pdwItemInfoData, &pcItemData->sItem, sizeof( Item ) );
			CopyMemory( &cItemWrapper, pcItemData, sizeof( ItemData ) );
		}
	}

	//Copy psItem to cItemWrapper
	CopyMemory( &cItemWrapper.sItem, (void*)pdwItemInfoData, sizeof( Item ) );

	BOOL bBeginner = IsBeginnerSet( &cItemWrapper );

	bCantUseItem = cItemWrapper.sItem.bCanNotUse;

	//Perfect Item
	if( cItemWrapper.sItem.bPerfectItem )
		bIsPerfectItem = TRUE;
	if( PERFECTVIEWHANDLER->IsSet() )
		bIsPerfectItem = TRUE;

	//Name Item
	if ( cItemWrapper.sBaseItemID.ToItemID() == ITEMID_SoulSummonCrystal )
	{
		AddString( 0, "Soul Crystal", -1, TRUE );
		AddString( 1, "" );
	}
	else
	{
		AddString( 0, cItemWrapper.sItem.szItemName, -1, TRUE );
		AddString( 1, "" );
	}

	//Color Name Item
	if( cItemWrapper.sItem.iItemUniqueID )
	{
		//Unique Color
		SetLineColor( D3DCOLOR_ARGB( 255, 128, 255, 128 ) );
	}
	else
	{
		//Default Color
		SetLineColor( D3DCOLOR_ARGB( 255, 222, 231, 255 ) );

		if ( bBeginner )
			SetLineColor( D3DCOLOR_ARGB( 255, 0, 222, 52 ) );

		//Set Special Colors
		if( cItemWrapper.sItem.eCraftType )
		{
			switch( cItemWrapper.sItem.eCraftType )
			{
				case ITEMCRAFTTYPE_Mixing:
					SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255) );
					break;
				case ITEMCRAFTTYPE_Aging:
					SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ) );
					break;
				case ITEMCRAFTTYPE_QuestWeapon:
					SetLineColor( D3DCOLOR_ARGB( 255, 132, 50, 254 ) );
					break;
			}
		}
	}

	//Make Description Item
	FormatItemInfo( &cItemWrapper );

	//Aging Bar
	if( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging )
	{
		AddString( 0, "\n " );
		AddString( 1, "\n " );
	}

	//Quest Item
	if( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_QuestWeapon && cItemWrapper.sItem.sAgeLevel < 4 )
	{
		AddString( 0, "" );
		AddString( 1, "" );

		AddString( 0, FormatString( "%s:\n%d/%d", pszaQuestMonsterName[ cItemWrapper.sItem.bMaturing ], cItemWrapper.sItem.sMatureBar.sMax - cItemWrapper.sItem.sMatureBar.sCur, cItemWrapper.sItem.sMatureBar.sMax ), D3DCOLOR_ARGB( 255, 125, 180, 175 ) );
		AddString( 1, "\n " );

		AddString( 0, "" );
		AddString( 1, "" );
	}

	//Respec Stone Level Range
	RespecStone * psList = ( RespecStone* )0x0099A0A8;

	for( int i = 0; i < 4; i++ )
	{
		EItemID eItemID = cItemWrapper.sBaseItemID.ToItemID();
		if( psList[ i ].eItemID == eItemID )
		{
			AddString( 0, "Level Req.:" );
			AddString( 1, FormatString( "%d-%d", psList[i].iMinLevel, psList[i].iMaxLevel ) );
			break;
		}
	}

	//Attack Power
	if( cItemWrapper.sItem.sDamage.sMin || cItemWrapper.sItem.sDamage.sMax )
	{
		AddString( 0, "Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d-%d", cItemWrapper.sItem.sDamage.sMin, cItemWrapper.sItem.sDamage.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( ( cItemWrapper.sItem.eMixEffect & MIXEFFECT_MinAttackPower ) || ( cItemWrapper.sItem.eMixEffect & MIXEFFECT_MaxAttackPower ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemBase() == ITEMBASE_Weapon ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sDamage.sMin - cItemCompare.sItem.sDamage.sMin;
			int iCalcMax = cItemWrapper.sItem.sDamage.sMax - cItemCompare.sItem.sDamage.sMax;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 || iCalcMax != 0 )
				AddCompareString( FormatString( "(%s%d-%d)", iCalc > 0 ? "+" : "", iCalc, iCalcMax ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Attack fMovementSpeed
	if( cItemWrapper.sItem.iAttackSpeed )
	{
		AddString( 0, "Attack Speed:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iAttackSpeed ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_WeaponSpeed )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iAttackSpeed - cItemCompare.sItem.iAttackSpeed;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}

	}

	//Range
	if( cItemWrapper.sItem.iAttackRange )
	{
		AddString( 0, "Range:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iAttackRange ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iAttackRange - cItemCompare.sItem.iAttackRange;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Critical
	if( cItemWrapper.sItem.iCritical )
	{
		AddString( 0, "Critical Rate:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d%%", cItemWrapper.sItem.iCritical ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Critical )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Claw || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Scythe || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Dagger || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Hammer ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Bow || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Javelin || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Sword || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Wand || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Phantom ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iCritical - cItemCompare.sItem.iCritical;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Defense
	if( cItemWrapper.sItem.iDefenseRating )
	{
		AddString( 0, "Defense:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iDefenseRating ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_DefenseRating )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Orb || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Armor || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Robe ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Boots || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Gauntlets || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Bracelets || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Shield ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iDefenseRating - cItemCompare.sItem.iDefenseRating;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Attack Rating
	if( cItemWrapper.sItem.iAttackRating )
	{
		AddString( 0, "Attack Rate:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iAttackRating ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_AttackRating )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Axe || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Claw || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Wand ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Hammer || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Scythe || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Sword || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Bracelets ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Phantom || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Dagger ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iAttackRating - cItemCompare.sItem.iAttackRating;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );
			
			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Absorption
	if( cItemWrapper.sItem.fAbsorbRating )
	{
		AddString( 0, "Absorption:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fAbsorbRating ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_AbsorbRating )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Orb || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Armor || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Robe ||
			cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Shield || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Gauntlets || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Boots ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );
	
		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.fAbsorbRating - cItemCompare.sItem.fAbsorbRating;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Block
	if( cItemWrapper.sItem.fBlockRating )
	{
		AddString( 0, "Block:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );

		if( fabs( cItemWrapper.sItem.fBlockRating - round( cItemWrapper.sItem.fBlockRating ) ) < FLT_MIN )
			AddString( 1, FormatString( "%.0f%%", round( cItemWrapper.sItem.fBlockRating ) ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
		else
			AddString( 1, FormatString( "%.1f%%", cItemWrapper.sItem.fBlockRating ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Block )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Shield ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.fBlockRating - cItemCompare.sItem.fBlockRating;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f%%)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Evade
	if ( cItemWrapper.sItem.bEvade )
	{
		AddString( 0, "Evade:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d%%", (int)cItemWrapper.sItem.bEvade ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Compare Item
		if ( bCompareItem && bIsEquippedItem )
		{
			int iCalc = (int)(cItemWrapper.sItem.bEvade - cItemCompare.sItem.bEvade);
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if ( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d%%)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}


	//Movement Speed
	if ( cItemWrapper.sItem.fMovementSpeed )
	{
		AddString( 0, "Speed:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fMovementSpeed ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if ( cItemWrapper.sItem.eMixEffect & MIXEFFECT_WalkSpeed )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255 ), 1 );

		//Compare Item
		if ( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.fMovementSpeed - cItemCompare.sItem.fMovementSpeed;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if ( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Integrity
	if( cItemWrapper.sItem.sIntegrity.sCur || cItemWrapper.sItem.sIntegrity.sMax )
	{
		AddString( 0, "Integrity:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d/%d", cItemWrapper.sItem.sIntegrity.sCur, cItemWrapper.sItem.sIntegrity.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Aging Color
		if( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Zero Integrity
		if ( cItemWrapper.sItem.sIntegrity.sCur == 0 )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 140, 0 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sIntegrity.sCur - cItemCompare.sItem.sIntegrity.sCur;
			int iCalcMax = cItemWrapper.sItem.sIntegrity.sMax - cItemCompare.sItem.sIntegrity.sMax;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d/%d)", iCalc > 0 ? "+" : "", iCalc, iCalcMax ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Mana
	if( cItemWrapper.sItem.sMPRecovery.sMin || cItemWrapper.sItem.sMPRecovery.sMax )
	{
		AddString( 0, "MP Recovery:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d-%d", cItemWrapper.sItem.sMPRecovery.sMin, cItemWrapper.sItem.sMPRecovery.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
	
		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sMPRecovery.sMin - cItemCompare.sItem.sMPRecovery.sMin;
			int iCalcMax = cItemWrapper.sItem.sMPRecovery.sMax - cItemCompare.sItem.sMPRecovery.sMax;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d-%d)", iCalc > 0 ? "+" : "", iCalc, iCalcMax ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//HP
	if( cItemWrapper.sItem.sHPRecovery.sMin || cItemWrapper.sItem.sHPRecovery.sMax )
	{
		AddString( 0, "HP Recovery:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d-%d", cItemWrapper.sItem.sHPRecovery.sMin, cItemWrapper.sItem.sHPRecovery.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
	
		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sHPRecovery.sMin - cItemCompare.sItem.sHPRecovery.sMin;
			int iCalcMax = cItemWrapper.sItem.sHPRecovery.sMax - cItemCompare.sItem.sHPRecovery.sMax;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d-%d)", iCalc > 0 ? "+" : "", iCalc, iCalcMax ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//STM
	if( cItemWrapper.sItem.sSPRecovery.sMin || cItemWrapper.sItem.sSPRecovery.sMax )
	{
		AddString( 0, "STM Recovery:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d-%d", cItemWrapper.sItem.sSPRecovery.sMin, cItemWrapper.sItem.sSPRecovery.sMax ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );
	
		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSPRecovery.sMin - cItemCompare.sItem.sSPRecovery.sMin;
			int iCalcMax = cItemWrapper.sItem.sSPRecovery.sMax - cItemCompare.sItem.sSPRecovery.sMax;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d-%d)", iCalc > 0 ? "+" : "", iCalc, iCalcMax ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Resistances
	{
		if( cItemWrapper.sItem.saElementalDef[0] )
		{
			AddString( 0, "Organic:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[0] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Organic )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && bIsEquippedItem )
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[0] - cItemCompare.sItem.saElementalDef[0];
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.saElementalDef[2] )
		{
			AddString( 0, "Fire:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[2] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Flame )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && bIsEquippedItem )
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[ 2 ] - cItemCompare.sItem.saElementalDef[ 2 ];
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.saElementalDef[3] )
		{
			AddString( 0, "Frost:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[3] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Frost )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && bIsEquippedItem )
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[ 3 ] - cItemCompare.sItem.saElementalDef[ 3 ];
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.saElementalDef[4] )
		{
			AddString( 0, "Lightning:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[4] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Light )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && bIsEquippedItem )
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[ 4 ] - cItemCompare.sItem.saElementalDef[ 4 ];
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.saElementalDef[5] )
		{
			AddString( 0, "Poison:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.saElementalDef[5] ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

			//Mix Color
			if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_Poison )
				SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

			//Compare Item
			if( bCompareItem && bIsEquippedItem )
			{
				int iCalc = cItemWrapper.sItem.saElementalDef[ 5 ] - cItemCompare.sItem.saElementalDef[ 5 ];
				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}
	}

	//HP Regen
	if( cItemWrapper.sItem.fHPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "HP Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fHPRegen ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_HPRegen )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.fHPRegen - cItemCompare.sItem.fHPRegen;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//MP Regen
	if( cItemWrapper.sItem.fMPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "MP Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fMPRegen ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_MPRegen )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.fMPRegen - cItemCompare.sItem.fMPRegen;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//STM Regen
	if( cItemWrapper.sItem.fSPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "STM Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.fSPRegen ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_SPRegen )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.fSPRegen - cItemCompare.sItem.fSPRegen;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Add HP
	if( cItemWrapper.sItem.fAddHP )
	{
		AddString( 0, "Add HP:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.0f", cItemWrapper.sItem.fAddHP ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_HP )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.fAddHP - cItemCompare.sItem.fAddHP;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.0f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Add MP
	if( cItemWrapper.sItem.fAddMP )
	{
		AddString( 0, "Add MP:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.0f", cItemWrapper.sItem.fAddMP ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_MP )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Aging Color
		if( ( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging ) && ( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Wand || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Phantom || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Bow || cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Javelin ) )
			SetLineColor( D3DCOLOR_ARGB( 255, 100, 230, 255 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.fAddMP - cItemCompare.sItem.fAddMP;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.0f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Add STM
	if( cItemWrapper.sItem.fAddSP )
	{
		AddString( 0, "Add STM:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.0f", cItemWrapper.sItem.fAddSP ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if( cItemWrapper.sItem.eMixEffect & MIXEFFECT_SP )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.fAddSP - cItemCompare.sItem.fAddSP;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.0f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Potion Storage
	if( cItemWrapper.sItem.iPotionStorage )
	{
		AddString( 0, "Pot Count:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iPotionStorage ), D3DCOLOR_ARGB( 255, 240, 215, 150 ) );

		//Mix Color
		if ( cItemWrapper.sItem.eMixEffect & MIXEFFECT_PotionCount )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 110, 255 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iPotionStorage - cItemCompare.sItem.iPotionStorage;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Attribute
	if ( auto pcItemAttributeInstance = ITEMSETATTRIBUTEHANDLER->GetAttributes( &cItemWrapper.sItem, TRUE ) )
	{
		auto pcAttribute = pcItemAttributeInstance->GetItemAttribute();

		BOOL bEquipped = FALSE;

		AddString( 0, "\n" );
		AddString( 1, "\n" );

		AddString( 0, pcAttribute->GetName(), D3DCOLOR_XRGB( 208, 32, 144 ) );
		AddString( 1, "\n" );

		AddString( 0, "\n" );
		AddString( 1, "\n" );

		for ( auto & psItem : pcAttribute->GetItems() )
		{
			CItemAttributeInstance::ItemDataEquipped * psItemEquipped = NULL;

			for ( auto & pcItem : pcItemAttributeInstance->GetEquippedItems() )
			{
				if ( pcItem->psItem == psItem )
				{
					psItemEquipped = pcItem;
					break;
				}
			}

			int iItemCount = 0;

			DWORD dwColor = D3DCOLOR_XRGB( 110, 110, 110 );

			if ( psItemEquipped )
			{
				iItemCount = psItemEquipped->iCount;
				if ( iItemCount > psItemEquipped->psItem->iCount )
					iItemCount = psItemEquipped->psItem->iCount;

				dwColor = D3DCOLOR_XRGB( 208, 32, 144 );
			}

			if ( psItem->iCount > 1 )
				AddString( 0, FormatString( "%s (%d/%d)", pcAttribute->GetTextItem( psItem->iTextID )->strText.c_str(), iItemCount, psItem->iCount ), dwColor );
			else
				AddString( 0, pcAttribute->GetTextItem( psItem->iTextID )->strText.c_str(), dwColor );

			AddString( 1, "\n" );
		}

		AddString( 0, "\n" );
		AddString( 1, "\n" );

		AddString( 0, "Set Effect:", D3DCOLOR_XRGB( 255, 140, 0 ) );
		AddString( 1, "\n" );

		int iItemCount = pcItemAttributeInstance->GetItemCount();
		for ( auto & psText : pcAttribute->GetTextEffects() )
		{
			if ( psText->iClass == UNITDATA->sCharacterData.iClass )
			{
				AddString( 0, FormatString( "%d", psText->iItemCount ) );
				AddString( 1, psText->strText.c_str(), iItemCount >= psText->iItemCount ? D3DCOLOR_XRGB( 255, 140, 0 ) : D3DCOLOR_XRGB( 110, 110, 110 ) );

				if ( pcAttribute->IsUnique() )
					break;
			}
		}
	}

	// Time on item
	FormatItemTimeInfo( &cItemWrapper );

	//Req Level
	if( cItemWrapper.sItem.iLevel )
	{
		AddString( 0, "\nReq. Level:" );
		AddString( 1, FormatString( "\n%d", cItemWrapper.sItem.iLevel ) );

		if( PLAYERDATA->iLevel < cItemWrapper.sItem.iLevel )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iLevel - cItemCompare.sItem.iLevel;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Req Strenght
	if( cItemWrapper.sItem.iStrength )
	{
		AddString( 0, "Req. Strength:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iStrength ) );

		if( PLAYERDATA->iStrength < cItemWrapper.sItem.iStrength )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iLevel - cItemCompare.sItem.iLevel;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Req Spirit
	if( cItemWrapper.sItem.iSpirit )
	{
		AddString( 0, "Req. Spirit:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iSpirit ) );

		if( PLAYERDATA->iSpirit < cItemWrapper.sItem.iSpirit )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iSpirit - cItemCompare.sItem.iSpirit;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Req Talent
	if( cItemWrapper.sItem.iTalent )
	{
		AddString( 0, "Req. Talent:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iTalent ) );

		if( PLAYERDATA->iTalent < cItemWrapper.sItem.iTalent )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iTalent - cItemCompare.sItem.iTalent;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Req Agility
	if( cItemWrapper.sItem.iAgility )
	{
		AddString( 0, "Req. Agility:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iAgility ) );

		if( PLAYERDATA->iAgility < cItemWrapper.sItem.iAgility )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iAgility - cItemCompare.sItem.iAgility;
			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Req Health
	if( cItemWrapper.sItem.iHealth )
	{
		AddString( 0, "Req. Health:" );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.iHealth ) );

		if( PLAYERDATA->iHealth < cItemWrapper.sItem.iHealth )
		{
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
			bCantUseItem = TRUE;
		}
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 125, 120, 100 ) );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.iHealth - cItemCompare.sItem.iHealth;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc < 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec
	if( cItemWrapper.sItem.eSpecialization )
	{
		EClassFlag e = static_cast<EClassFlag>( cItemWrapper.sItem.eSpecialization );
		AddString( 0, FormatString( "\n%s Spec", GetCharacterClassByFlag( e ) ) );
		AddString( 1, "\n" );

		if( PLAYERDATA->iFlag == cItemWrapper.sItem.eSpecialization )
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 220, 0 ) );
		else
			SetLineColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
	}

	//Spec ATK fMovementSpeed
	if( cItemWrapper.sItem.sSpecData.iSpecAttackSpeed )
	{
		AddString( 0, "Attack Speed:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.iSpecAttackSpeed ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackSpeed - cItemCompare.sItem.sSpecData.iSpecAttackSpeed;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec Critical
	if( cItemWrapper.sItem.sSpecData.iSpecCritical )
	{
		AddString( 0, "Critical Rate:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d%%", cItemWrapper.sItem.sSpecData.iSpecCritical ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecCritical - cItemCompare.sItem.sSpecData.iSpecCritical;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d%%)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec Defense
	if( cItemWrapper.sItem.sSpecData.iSpecDefenseRating )
	{
		AddString( 0, "Defense:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.iSpecDefenseRating ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecDefenseRating - cItemCompare.sItem.sSpecData.iSpecDefenseRating;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec fAbsorbRating
	if( cItemWrapper.sItem.sSpecData.fSpecAbsorbRating )
	{
		AddString( 0, "Absorption:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecAbsorbRating ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.sSpecData.fSpecAbsorbRating - cItemCompare.sItem.sSpecData.fSpecAbsorbRating;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0.0f )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec Block
	if( cItemWrapper.sItem.sSpecData.fSpecBlockRating )
	{
		AddString( 0, "Block:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );

		if( fabs( cItemWrapper.sItem.sSpecData.fSpecBlockRating - round( cItemWrapper.sItem.sSpecData.fSpecBlockRating ) ) < FLT_MIN )
			AddString( 1, FormatString( "%.0f%%", round( cItemWrapper.sItem.sSpecData.fSpecBlockRating ) ) );
		else
			AddString( 1, FormatString( "%.1f%%", cItemWrapper.sItem.sSpecData.fSpecBlockRating ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.sSpecData.fSpecBlockRating - cItemCompare.sItem.sSpecData.fSpecBlockRating;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f%%)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec MagicAPT
	if( cItemWrapper.sItem.sSpecData.fSpecMagicMastery )
	{
		AddString( 0, "Magic APT:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.0f", cItemWrapper.sItem.sSpecData.fSpecMagicMastery ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.sSpecData.fSpecMagicMastery - cItemCompare.sItem.sSpecData.fSpecMagicMastery;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.0f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec fMovementSpeed
	if( cItemWrapper.sItem.sSpecData.fSpecMovementSpeed )
	{
		AddString( 0, "Speed:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecMovementSpeed ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.sSpecData.fSpecMovementSpeed - cItemCompare.sItem.sSpecData.fSpecMovementSpeed;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec Resistances
	{
		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[0] )
		{
			AddString( 0, "Organic:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

			//Compare Item
			if( bCompareItem && bIsEquippedItem )
			{
				int iCalc = cItemWrapper.sItem.sSpecData.saSpecElementalDef[0] - cItemCompare.sItem.sSpecData.saSpecElementalDef[0];

				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[2] )
		{
			AddString( 0, "Fire:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef[2] ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

			//Compare Item
			if( bCompareItem && bIsEquippedItem )
			{
				int iCalc = cItemWrapper.sItem.sSpecData.saSpecElementalDef[2] - cItemCompare.sItem.sSpecData.saSpecElementalDef[2];

				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[3] )
		{
			AddString( 0, "Frost:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef[3] ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

			//Compare Item
			if( bCompareItem && bIsEquippedItem )
			{
				int iCalc = cItemWrapper.sItem.sSpecData.saSpecElementalDef[3] - cItemCompare.sItem.sSpecData.saSpecElementalDef[3];

				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[4] )
		{
			AddString( 0, "Lightning:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef[4] ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

			//Compare Item
			if( bCompareItem && bIsEquippedItem )
			{
				int iCalc = cItemWrapper.sItem.sSpecData.saSpecElementalDef[4] - cItemCompare.sItem.sSpecData.saSpecElementalDef[4];

				SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

				if( iCalc != 0 )
					AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
			}
		}

		if( cItemWrapper.sItem.sSpecData.saSpecElementalDef[5] )
		{
			AddString( 0, "Poison:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
			AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.saSpecElementalDef[5] ) );

			SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );
		}
	}

	//Spec ATK Power
	if( cItemWrapper.sItem.sSpecData.iSpecAttackPowerDiv )
	{
		AddString( 0, "Attack Power:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "LV/%d", cItemWrapper.sItem.sSpecData.iSpecAttackPowerDiv ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackPowerDiv - cItemCompare.sItem.sSpecData.iSpecAttackPowerDiv;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%sLV/%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 225, 30, 0 ) : D3DCOLOR_ARGB( 255, 0, 130, 255 ) );
		}
	}

	//Spec ATK Rtg
	if( cItemWrapper.sItem.sSpecData.iSpecAttackRatingDiv )
	{
		AddString( 0, "Attack Rate:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "LV/%d", cItemWrapper.sItem.sSpecData.iSpecAttackRatingDiv ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackRatingDiv - cItemCompare.sItem.sSpecData.iSpecAttackRatingDiv;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%sLV/%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 225, 30, 0 ) : D3DCOLOR_ARGB( 255, 0, 130, 255 ) );
		}
	}

	//Spec Range
	if( cItemWrapper.sItem.sSpecData.iSpecAttackRange )
	{
		AddString( 0, "Range:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.iSpecAttackRange ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAttackRange - cItemCompare.sItem.sSpecData.iSpecAttackRange;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec Penetration
	if ( cItemWrapper.sItem.sSpecData.sSpecPenetration )
	{
		AddString( 0, "Spec Penetration:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d", cItemWrapper.sItem.sSpecData.sSpecPenetration ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if ( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.sSpecPenetration - cItemCompare.sItem.sSpecData.sSpecPenetration;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if ( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec HP Potion Efficience
	if ( cItemWrapper.sItem.sSpecData.sSpecHPPotionEfficience )
	{
		AddString( 0, "Spec HP Potion Efficiency:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d%%", cItemWrapper.sItem.sSpecData.sSpecHPPotionEfficience ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if ( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.sSpecHPPotionEfficience - cItemCompare.sItem.sSpecData.sSpecHPPotionEfficience;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if ( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d%%)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec MP Potion Efficience
	if ( cItemWrapper.sItem.sSpecData.sSpecMPPotionEfficience )
	{
		AddString( 0, "Spec MP Potion Efficiency:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d%%", cItemWrapper.sItem.sSpecData.sSpecMPPotionEfficience ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if ( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.sSpecMPPotionEfficience - cItemCompare.sItem.sSpecData.sSpecMPPotionEfficience;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if ( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d%%)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec SP Potion Efficience
	if ( cItemWrapper.sItem.sSpecData.sSpecSPPotionEfficience )
	{
		AddString( 0, "Spec SP Potion Efficiency:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%d%%", cItemWrapper.sItem.sSpecData.sSpecSPPotionEfficience ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if ( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.sSpecSPPotionEfficience - cItemCompare.sItem.sSpecData.sSpecSPPotionEfficience;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if ( iCalc != 0 )
				AddCompareString( FormatString( "(%s%d%%)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec HP Boost
	if ( cItemWrapper.sItem.sSpecData.iSpecAddHPDiv )
	{
		AddString( 0, "Max HP Boost:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "LV/%d", cItemWrapper.sItem.sSpecData.iSpecAddHPDiv ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if ( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAddHPDiv - cItemCompare.sItem.sSpecData.iSpecAddHPDiv;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if ( iCalc != 0 )
				AddCompareString( FormatString( "(%sLV/%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec MP Boost
	if( cItemWrapper.sItem.sSpecData.iSpecAddMPDiv )
	{
		AddString( 0, "Max MP Boost:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "LV/%d", cItemWrapper.sItem.sSpecData.iSpecAddMPDiv ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			int iCalc = cItemWrapper.sItem.sSpecData.iSpecAddMPDiv - cItemCompare.sItem.sSpecData.iSpecAddMPDiv;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( iCalc != 0 )
				AddCompareString( FormatString( "(%sLV/%d)", iCalc > 0 ? "+" : "", iCalc ), iCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec HP Regen
	if( cItemWrapper.sItem.sSpecData.fSpecHPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "HP Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecHPRegen ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.sSpecData.fSpecHPRegen - cItemCompare.sItem.sSpecData.fSpecHPRegen;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec MP Regen
	if( cItemWrapper.sItem.sSpecData.fSpecMPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "MP Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecMPRegen ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.sSpecData.fSpecMPRegen - cItemCompare.sItem.sSpecData.fSpecMPRegen;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	//Spec STM Regen
	if ( cItemWrapper.sItem.sSpecData.fSpecSPRegen && cItemWrapper.sBaseItemID.ToItemType() != ITEMTYPE_ForceOrb )
	{
		AddString( 0, "STM Regen:", D3DCOLOR_ARGB( 255, 245, 240, 218 ) );
		AddString( 1, FormatString( "%.1f", cItemWrapper.sItem.sSpecData.fSpecSPRegen ) );

		SetLineColor( D3DCOLOR_ARGB( 255, 235, 190, 60 ), 1 );

		//Compare Item
		if ( bCompareItem && bIsEquippedItem )
		{
			float fCalc = cItemWrapper.sItem.sSpecData.fSpecSPRegen - cItemCompare.sItem.sSpecData.fSpecSPRegen;

			SetLineColor( D3DCOLOR_ARGB( 255, 165, 157, 140 ), 0 );

			if ( fCalc != 0 )
				AddCompareString( FormatString( "(%s%.1f)", fCalc > 0 ? "+" : "", fCalc ), fCalc > 0 ? D3DCOLOR_ARGB( 255, 0, 130, 255 ) : D3DCOLOR_ARGB( 255, 225, 30, 0 ) );
		}
	}

	// Price
	// Shop is open?
	if ( (*(BOOL*)0x03653E40) )
	{
		// Next Line
		AddString( 0, "" );
		AddString( 1, "" );

		if ( bShowBuyPrice )
		{
			// Normal, price
			if ( bShowBuyPrice == 2 )
			{
				// Get Inventory Price Item
				PriceItem sSellItemData = { 0, 0, 0 };
				ITEMHANDLER->GetItemPrice( (ItemData*)pcItemData, &sSellItemData );
				pcItemData->iSalePrice = sSellItemData.iPrice;

				// Set Price Item		
				AddString( 0, "Price:" );
				AddString( 1, pcItemData->iSalePrice > 0 ? FormatString("%s Gold",FormatNumber( pcItemData->iSalePrice )) : "Free" );
			}
			if ( bShowBuyPrice == 1 )
			{
				// Set Text
				char szTempText[32] = { 0 };
				int iPrice = cItemWrapper.sItem.iSalePrice;

				// Get Price with BC Tax
				__asm
				{
					PUSH iPrice;
					MOV EAX, 0x004E5A10;
					CALL EAX;
					MOV iPrice, EAX;
				}
				
				AddString( 0, "Price:" );
				AddString( 1, iPrice > 0 ? FormatString("%s Gold",FormatNumber( iPrice )) : "Free" );
			}
		}
	}

	// Personal Shop is open?
	if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() )
	{
		if ( auto[bRet, psPrice] = PERSONALSHOPHANDLER->GetPrice( cItemWrapper.sItem.sItemID.ToInt(), cItemWrapper.sItem.iChk1, cItemWrapper.sItem.iChk2 ); bRet )
		{
			// Next Line
			AddString( 0, "" );
			AddString( 1, "" );

			if ( psPrice->bCanBuy )
			{
				AddString( 0, "Price:" );

				if ( psPrice->bCredits )
					AddString( 1, psPrice->iPrice > 0 ? FormatString( "%s coins", FormatNumber( psPrice->iPrice ) ) : "Free", D3DCOLOR_XRGB( 255, 140, 0 ) );
				else
					AddString( 1, psPrice->iPrice > 0 ? FormatString( "%s gp", FormatNumber( psPrice->iPrice ) ) : "Free" );
			}
			else
			{
				AddString( 0, "View Only", D3DCOLOR_XRGB( 84, 174, 232 ), TRUE );
				AddString( 1, "" );
			}
		}
	}

	//Debug Item Info
	if( bShowChecksums )
	{
		AddString( 0, "\n" );
		AddString( 1, "" );

		AddString( 0, FormatString( "%d", cItemWrapper.sItem.iChk1 ) );
		AddString( 1, "" );

		AddString( 0, FormatString( "%d", cItemWrapper.sItem.iChk2 ) );
		AddString( 1, "" );
	}

	//Socket Item
	if( cItemWrapper.sItem.sSocketData[0].eSocketType != SOCKETTYPE_None )
	{
		AddString( 0, "\n\n " );
		AddString( 1, "\n\n " );
	}


	//Is Perfect Item?
	if ( bIsPerfectItem )
	{
		cBackgroundColor1 = UI::Color( 235, 45, 39, 27 );
		//cBackgroundColor2 = UI::Color( 245, 24, 42, 56 );
	}
	//Can't use item?
	else if ( bCantUseItem )
	{
		cBackgroundColor1 = UI::Color( 235, 10, 10, 10 );
		cBackgroundColor2 = UI::Color( 235, 75, 25, 25 );
	}
	else
		cBackgroundColor2 = UI::Color( 245, 9, 23, 22 );


	//Clear Text
	paSocketText[0][0]->SetText( "" );
	paSocketText[0][1]->SetText( "" );
	paSocketText[1][0]->SetText( "" );
	paSocketText[1][1]->SetText( "" );

	for( int i = 0; i < 2; i++ )
	{
		if( cItemWrapper.sItem.sSocketData[i].eSocketType != SOCKETTYPE_None )
		{
			iSocketCount += 1;

			//Get Gem Image
			switch( cItemWrapper.sItem.sSocketData[i].eGemItem )
			{
				case GEMITEM_Ruby:
					iSocketGem[i] = 0;
				break;
				case GEMITEM_Topaz:
					iSocketGem[i] = 3;
				break;
				case GEMITEM_Emerald:
					iSocketGem[i] = 6;
				break;
				case GEMITEM_Aquamarine:
					iSocketGem[i] = 9;
				break;
				case GEMITEM_Amethyst:
					iSocketGem[i] = 12;
				break;
			}

			//Format Gem Type
			if( cItemWrapper.sItem.sSocketData[i].eGemType == GEMTYPE_Triangular )
				paSocketText[i][1]->SetText( "+2" );
			else if( cItemWrapper.sItem.sSocketData[i].eGemType == GEMTYPE_Square )
			{
				paSocketText[i][1]->SetText( "+4" );
				iSocketGem[i] += 1;
			}
			else if( cItemWrapper.sItem.sSocketData[i].eGemType == GEMTYPE_Marquise )
			{
				paSocketText[i][1]->SetText( "+6" );
				iSocketGem[i] += 2;
			}

			//Format Name and Stats of Gem
			switch( cItemWrapper.sItem.sSocketData[i].eGemItem )
			{
				case GEMITEM_Topaz:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 255, 220, 0 ) );
					paSocketText[i][0]->SetText( "Topaz Gem" );

					paSocketText[i][1]->AppendText( " Talent" );
				break;
				case GEMITEM_Aquamarine:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 0, 150, 255 ) );
					paSocketText[i][0]->SetText( "Aquamarine Gem" );

					paSocketText[i][1]->AppendText( " Spirit" );
				break;
				case GEMITEM_Ruby:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 180, 25, 25 ) );
					paSocketText[i][0]->SetText( "Ruby Gem" );

					paSocketText[i][1]->AppendText( " Strenght" );
				break;
				case GEMITEM_Amethyst:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 150, 5, 235 ) );
					paSocketText[i][0]->SetText( "Amethyst Gem" );

					paSocketText[i][1]->AppendText( " Health" );
				break;
				case GEMITEM_Emerald:
					paSocketText[i][0]->SetColor( D3DCOLOR_ARGB( 255, 0, 170, 10 ) );
					paSocketText[i][0]->SetText( "Emerald Gem" );

					paSocketText[i][1]->AppendText( " Agility" );
				break;
			}
		}
	}

	//Not have description or stats? So only show the Title of Item
	if( vLeftText.size() < 3 )
	{
		vLeftText.erase( vLeftText.begin() + 1 );
		vRightText.erase( vRightText.begin() + 1 );
	}

	return TRUE;
}

void ItemInfoBox::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	//If player is viewing Item
	if( *( BOOL* )0x036286EC )
	{
		//Adjusting the iItemSlot
		iX = ( cItemWrapper.sPosition.iX + ( cItemWrapper.iWidth / 2 ) ) - 90;
		iY = cItemWrapper.sPosition.iY - ( vLeftText.size() * 14 ) - 14;

		int iSocketHeightExtra = 0;

		for( int i = 0; i < 2; i++ )
		{
			if( cItemWrapper.sItem.sSocketData[i].eSocketType )
				iSocketHeightExtra += i > 0 ? 36 : 8;
		}

		//Set iWidth and iHeight of Box
		int iItemInfoBoxWidth = 200 - 16 + 26;
		int iItemInfoBoxHeight = ( 14 * vLeftText.size() );

		if ( pWindowHair->IsOpen() )
		{
			iItemInfoBoxHeight += pWindowHair->GetHeight() + 10;
			iY -= (pWindowHair->GetHeight() + 10);
		}

		if ( pWindowBody->IsOpen() )
		{
			iItemInfoBoxHeight += pWindowBody->GetHeight() + 10;
			iY -= (pWindowBody->GetHeight() + 10);
		}

		//iItemSlot of box is out the window?
		if( iX < 0 )
			iX = 0;

		if( iY < 0 )
			iY = 16;

		if( iX + iItemInfoBoxWidth > (int)RESOLUTION_WIDTH )
			iX -= ( ( iX + iItemInfoBoxWidth ) - RESOLUTION_WIDTH ) + 16;

		//Fix position if is showing from Inventory
		if( *( UINT* )0x036286EC == 2 )
			iY += RESOLUTION_HEIGHT - 600 - iSocketHeightExtra;

		//Update Item Info Box Width and Height
		iItemInfoBoxWidth += 10;
		iItemInfoBoxHeight += 20 + iSocketHeightExtra;

		//Draw Item Info Box Background
		DrawItemInfoBox( iX, iY, iItemInfoBoxWidth, iItemInfoBoxHeight );

		//Drawing Border
		UI::ImageRender::Render( pImageBorderLeftTop, iX, iY - 10, pImageBorderLeftTop->GetWidth(), pImageBorderLeftTop->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderTop, iX + pImageBorderLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderTop->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderRightTop, iX + pImageBorderLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), iY - 10, pImageBorderRightTop->GetWidth(), pImageBorderRightTop->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderLeft, iX, iY - 10 + pImageBorderLeftTop->GetHeight(), pImageBorderLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1 );
		UI::ImageRender::Render( pImageBorderRight, iX + iItemInfoBoxWidth - pImageBorderRight->GetWidth(), iY - 10 + pImageBorderLeftTop->GetHeight(), pImageBorderRight->GetWidth(), iItemInfoBoxHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1 );
		UI::ImageRender::Render( pImageBorderLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderLeftBottom->GetHeight(), pImageBorderLeftBottom->GetWidth(), pImageBorderLeftBottom->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderRightBottom, iX + iItemInfoBoxWidth - pImageBorderRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderRightBottom->GetHeight(), pImageBorderRightBottom->GetWidth(), pImageBorderRightBottom->GetHeight(), -1 );
		UI::ImageRender::Render( pImageBorderBottom, iX + pImageBorderLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderBottom->GetHeight(), -1 );

		if ( pWindowHair->IsOpen() )
		{
			pWindowHair->SetPosition( iX + (iItemInfoBoxWidth >> 1) - (pWindowHair->GetWidth() >> 1), iY + iItemInfoBoxHeight - pWindowHair->GetHeight() - 20 );
			pWindowHair->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}

		if ( pWindowBody->IsOpen() )
		{
			pWindowBody->SetPosition( iX + (iItemInfoBoxWidth >> 1) - (pWindowBody->GetWidth() >> 1), iY + iItemInfoBoxHeight - pWindowBody->GetHeight() - 20 );
			pWindowBody->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}

		//Render Item Type Icon
		if( cItemWrapper.IsOneHandedWeapon() )
		{
			if( pImageIconWeaponOneHand )
				UI::ImageRender::Render( pImageIconWeaponOneHand, iX + iItemInfoBoxWidth - 30, iY, 18, 16, 0, 0, -1 );
		}
		else if( cItemWrapper.IsTwoHandedWeapon() )
		{
			if( pImageIconWeaponTwoHand )
				UI::ImageRender::Render( pImageIconWeaponTwoHand, iX + iItemInfoBoxWidth - 30, iY, 18, 16, 0, 0, -1 );
		}
		else if( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Armor )
		{
			if( pImageIconArmor )
				UI::ImageRender::Render( pImageIconArmor, iX + iItemInfoBoxWidth - 30, iY, 18, 16, 0, 0, -1 );
		}
		else if( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Robe )
		{
			if( pImageIconRobe )
				UI::ImageRender::Render( pImageIconRobe, iX + iItemInfoBoxWidth - 30, iY, 18, 16, 0, 0, -1 );
		}
		else if( cItemWrapper.sBaseItemID.ToItemType() == ITEMTYPE_Orb )
		{
			if( pImageIconRobe )
				UI::ImageRender::Render( pImageIconRobe, iX + iItemInfoBoxWidth - 30, iY, 18, 16, 0, 0, -1 );
		}

		//Render Compare Text
		if( bIsEquippedItem && bCompareItem )
		{
			for( UINT i = 0; i < vRowCompare.size(); i++ )
			{
				Text_ptr pLeftText = vLeftText[ vRowCompare[i] - 1 ];
				Text_ptr pRightText = vRightText[ vRowCompare[i] - 1 ];
				Text_ptr pCompareText = vCompareText[i];

				//Load Texts
				pCompareText->SetNoClip( TRUE );
				pCompareText->SetHorizontalAlign( ALIGN_Left );
				pCompareText->SetBox( Rectangle2D( pLeftText->GetTextWidth() + pRightText->GetTextWidth() + 22, 0 + ( 14 * (vRowCompare[i]-1) ), 180, iHeight ) );

				//Render Texts
				pCompareText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			}
		}

		//Render Text
		if( vLeftText.size() == vRightText.size() )
		{
			for( size_t i = 0; i < vLeftText.size(); i++ )
			{
				Text_ptr pLeftText = vLeftText[ i ];
				Text_ptr pRightText = vRightText[ i ];

				//Load Texts
				pLeftText->SetMultiLine( TRUE );
				pLeftText->SetNoClip( TRUE );

				pRightText->SetNoClip( TRUE );

				pLeftText->SetHorizontalAlign( ALIGN_Left );
				pLeftText->SetBox( Rectangle2D( 15, 0 + ( 14 * i ), 180, iHeight ) );

				pRightText->SetHorizontalAlign( ALIGN_Left );
				pRightText->SetBox( Rectangle2D( pLeftText->GetTextWidth() + 20, 0 + ( 14 * i ), 180, iHeight ) );

				//Render Texts
				pLeftText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				pRightText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			}
		}

		//Render Aging Bar
		if( cItemWrapper.sItem.eCraftType == ITEMCRAFTTYPE_Aging )
		{
			int iYExtra = 0;

			if( (bCompareItem && bIsEquippedItem) || bIsPerfectItem )
				iYExtra = 15;

			if( cItemWrapper.sItem.sMatureBar.sMax )
			{
				int iBarLenght = ( int )( ( float )( ( float )125 * ( ( ( float )cItemWrapper.sItem.sMatureBar.sCur / ( float )cItemWrapper.sItem.sMatureBar.sMax ) ) ) );
				
				if( pImageAgingBar )
					UI::ImageRender::Render( pImageAgingBar, ( iX + 90 ) - ( 130 >> 1 ), iY + 26 + iYExtra, 130, 19, 0, 0, -1 );
			
				if( pImageAgingBarGage )
					UI::ImageRender::Render( pImageAgingBarGage, ( ( iX + 90 ) - ( 125 >> 1 ) ) - 1, iY + 33 + iYExtra, iBarLenght, 4, 0, 0, -1 );
			}
			else
			{
				if( pImageAgingBarNum )
					UI::ImageRender::Render( pImageAgingBarNum, ( iX + 90 ) - ( 26 >> 1 ), iY + 26 + iYExtra, 26, 19, 0, 0, -1 );
			}

			//Render Aging Text
			if( pTextAging )
			{
				pTextAging->SetNoClip( TRUE );
				pTextAging->SetText( FormatString("+%d",cItemWrapper.sItem.sAgeLevel) );
				pTextAging->SetBox( Rectangle2D( 79, 26 + iYExtra, 23, 18 ) );
				pTextAging->SetHorizontalAlign( ALIGN_Center );
				pTextAging->SetVerticalAlign( ALIGN_Center );
				pTextAging->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			}
		}

		//Render Socket System
		if( iSocketHeightExtra )
		{
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX + 15, iY + iItemInfoBoxHeight - 30, 150, 1 ), D3DCOLOR_ARGB( 255, 160, 130, 105 ) );

			for( int i = 0; i < iSocketCount; i++ )
			{
				//Render Socket Info
				paSocketText[i][0]->SetNoClip( TRUE );
				paSocketText[i][0]->SetBox( Rectangle2D( 56, iItemInfoBoxHeight - 22 + ( i * 38 ), 120, 18 ) );
				paSocketText[i][0]->SetHorizontalAlign( ALIGN_Left );
				paSocketText[i][0]->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

				paSocketText[i][1]->SetNoClip( TRUE );
				paSocketText[i][1]->SetBox( Rectangle2D( 56, iItemInfoBoxHeight - 8 + ( i * 38 ), 120, 18 ) );
				paSocketText[i][1]->SetHorizontalAlign( ALIGN_Left );
				paSocketText[i][1]->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

				//Render Socket Background
				if( pImageSocketBackground )
					UI::ImageRender::Render( pImageSocketBackground, iX + 17, iY + iItemInfoBoxHeight - 18 + ( i * 38 ), 27, 27, -1 );

				//Render Socket Type
				for( int k = 1; k < SOCKETTYPE_Count; k++ )
				{
					if( cItemWrapper.sItem.sSocketData[i].eSocketType == (ESocketType)k )
					{
						UI::ImageRender::Render( paSocket[k-1], iX + 16, iY + iItemInfoBoxHeight - 19 + ( i * 38 ), 29, 29, -1 );
						break;
					}
				}

				//Render Socket Gem
				if( cItemWrapper.sItem.sSocketData[i].eGemItem != GEMITEM_None && paSocketGems[iSocketGem[i]] )
					UI::ImageRender::Render( paSocketGems[iSocketGem[i]], iX + 19, iY + iItemInfoBoxHeight - 15 + ( i * 38 ), 22, 22, -1 );
			}
		}
	}
}

void ItemInfoBox::Update( float fTime )
{
	if( bIsPerfectItem || ((cItemWrapper.sItem.sIntegrity.sCur == 0) && (cItemWrapper.sItem.sIntegrity.sMax > 0)) )
	{
		if( bFadeToColor )
		{
			fProgress += 0.01f;

			if( fProgress > 1.0f )
			{
				bFadeToColor = FALSE;
				fProgress = 1.0f;
			}
		}
		else if( !bFadeToColor )
		{
			fProgress -= 0.005f;

			if( fProgress < 0.0f )
			{
				bFadeToColor = TRUE;
				fProgress = 0.0f;
			}
		}

		if ( (cItemWrapper.sItem.sIntegrity.sCur == 0) )
			cBackgroundColor2 = UI::Color( 245, 9 + (int)(86.0f * fProgress), 23 + (int)(27.0f * fProgress), 0 );
		else
			cBackgroundColor2 = UI::Color( 245, 9 + (int)(15.0f * fProgress), 23 + (int)(19.0f * fProgress), 22 + (int)(34.0f * fProgress) );
	}

	if ( pWindowHair->IsOpen() )
	{
		auto pUnitMeshPreview = pWindowHair->GetElement<UI::UnitMesh>( UNITMESHID_HeadPreview );
		pUnitMeshPreview->SetAngle( pUnitMeshPreview->GetAngle() + 10 );
	}

	if ( pWindowBody->IsOpen() )
	{
		auto pUnitMeshPreview = pWindowBody->GetElement<UI::UnitMesh>( UNITMESHID_BodyPreview );
		pUnitMeshPreview->SetAngle( pUnitMeshPreview->GetAngle() + 10 );
	}
}

void ItemInfoBox::DrawItemInfoBox( int iX, int iY, int iWidth, int iHeight )
{
	//Draw Item Info Box Background
	if ( pRenderTargetMask )
	{
		if ( GetGraphicsDevice()->SetRenderTarget( pRenderTargetMask ) )
		{
			//Clear Render Target
			GetGraphicsDevice()->Clear( TRUE, TRUE, FALSE );

			//Draw Background
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( 0, 0, iWidth, iHeight ), cBackgroundColor1.Get(), cBackgroundColor2.Get() );

			//Drawing Mask Border
			UI::ImageRender::Render( pImageMaskLeftTop, 0, 0, pImageMaskLeftTop->GetWidth(), pImageMaskLeftTop->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskRightTop, iWidth - pImageMaskRightTop->GetWidth(), 0, pImageMaskRightTop->GetWidth(), pImageMaskRightTop->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskLeftBottom, 0, iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskLeftBottom->GetWidth(), pImageMaskLeftBottom->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskRightBottom, iWidth - pImageMaskRightTop->GetWidth(), iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskRightBottom->GetWidth(), pImageMaskRightBottom->GetHeight(), -1 );

			//Back Old Render Target
			GetGraphicsDevice()->UnsetRenderTarget();
		}

		//Set Sprite Effect...
		UI::ImageRender::SetEffect( pEffectMask );
		pEffectMask->SetTechnique( "Mask" );

		if ( pEffectMask->Begin() >= 0 )
		{
			//Render the Render Target and mask it on effect
			UI::ImageRender::Render( pRenderTargetMask->GetTexture(), iX, iY - 10, pRenderTargetMask->GetWidth(), pRenderTargetMask->GetHeight(), 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ), FALSE );

			pEffectMask->End();
		}
		UI::ImageRender::SetEffect( NULL );
	}
}

}