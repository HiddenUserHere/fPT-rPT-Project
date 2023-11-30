#include "stdafx.h"
#include "MDamageInfo.h"
#include "Particle.h"



// Vars
int							iDefenseInfoPart;
int							iBlockInfoPart;
int							iEvadeInfoPart;
int							iCriticalInfoPart;
int							iFontInfoDamage;
int							iFontInfoCritical;
UINT						uTimeGeneralDamageInfo;
bool						bLoadedResourcesDamageInfo = false;


// Struct Target Info
struct stEventDamageInfo
{
	int  EventID;
	UINT TargetID;
	bool isDefense;
	bool isBlock;
	bool isEvade;
	bool isCritical;
	bool isDamage;
	int  iValue;
	int  iXScreen;
	int	 iYScreen;
	float fTime;
	int iY;
	int iParticleID;
	bool isDelete;
	bool bRender;
};

// Vector Target Info
std::vector<stEventDamageInfo*> vTargetsInfo;


// Files names
#define DEFENSE_INFO_PART_FILE	"game\\scripts\\particles\\DefenseInfo.part"
#define BLOCK_INFO_PART_FILE	"game\\scripts\\particles\\BlockInfo.part"
#define EVADE_INFO_PART_FILE	"game\\scripts\\particles\\EvadeInfo.part"
#define CRITICAL_INFO_PART_FILE	"game\\scripts\\particles\\CriticalInfo.part"

CDamageInfoModel::CDamageInfoModel()
{
}


int CDamageInfoModel::FindTarget( UINT TargetID )
{
	// Loop to find the target in memory
	for ( UINT u = 0; u < vTargetsInfo.size(); u++ )
	{
		// Found? then return vector pos
		if ( vTargetsInfo[ u ]->TargetID == TargetID )
			return u;
	}
	return -1;
}


void CDamageInfoModel::DeleteTarget( int Position )
{
	// If have this pointer, delete it
	if ( ( UINT )Position < vTargetsInfo.size() )
	{
		if ( vTargetsInfo[Position]->isDelete || GetForegroundWindow() != DX::Graphic::GetWindowHandler() )
		{
			delete vTargetsInfo[Position];
			vTargetsInfo.erase( vTargetsInfo.begin() + Position );
		}
	}
}


void CDamageInfoModel::AddTarget( UINT TargetID, int TypeAction, int Value )
{

	if ( CHARACTERGAME->IsStageVillage() || GetForegroundWindow() != DX::Graphic::GetWindowHandler() )
		return;

	// If isnt loaded, load it
	if ( !bLoadedResourcesDamageInfo )
	{
		this->Load();
		bLoadedResourcesDamageInfo = true;
	}

	// Get target Memory
	UnitData * pcUnit = UNITDATABYID( TargetID );

	if ( TargetID == -1 )
		pcUnit = UNITDATA;

	// If dont have target, return
	if ( !pcUnit )
		return;

	// Add Target into vector
	stEventDamageInfo * pEvent = new stEventDamageInfo;
	ZeroMemory( pEvent, sizeof( stEventDamageInfo ) );
	vTargetsInfo.push_back( pEvent );

	int iVecPtr = vTargetsInfo.size() - 1;

	// Add EventID, TargetID and Time
	vTargetsInfo[ iVecPtr ]->EventID = UI::Event::Build( std::bind( &CDamageInfoModel::OnActionEvent, this, iVecPtr, std::placeholders::_1 ) );
	vTargetsInfo[ iVecPtr ]->TargetID = TargetID;

	// Set in the vector, x,y screen relative for 3d
	vTargetsInfo[ iVecPtr ]->iXScreen = pcUnit->rRenderRectangle.right;
	vTargetsInfo[ iVecPtr ]->iYScreen = pcUnit->rRenderRectangle.top;

	vTargetsInfo[iVecPtr]->iY = 80;
	vTargetsInfo[iVecPtr]->iParticleID = 0;
	vTargetsInfo[iVecPtr]->bRender = false;

	if ( TypeAction == this->eActionType::iDefenseRating )
	{
		vTargetsInfo[ iVecPtr ]->fTime = 1.0f;
		vTargetsInfo[ iVecPtr ]->isDefense = true;
	}
	else if ( TypeAction == this->eActionType::iBlockRating )
	{
		vTargetsInfo[ iVecPtr ]->fTime = 1.0f;
		vTargetsInfo[ iVecPtr ]->isBlock = true;
	}
	else if ( TypeAction == this->eActionType::Evade )
	{
		vTargetsInfo[ iVecPtr ]->fTime = 1.0f;
		vTargetsInfo[ iVecPtr ]->isEvade = true;
	}
	else if ( TypeAction == this->eActionType::iCritical )
	{
		vTargetsInfo[ iVecPtr ]->fTime = 10.0f;
		vTargetsInfo[ iVecPtr ]->isCritical = true;
	}
	else if ( TypeAction == this->eActionType::Damage )
	{
		vTargetsInfo[ iVecPtr ]->iValue = Value;
		vTargetsInfo[ iVecPtr ]->fTime = 10.0f;
		vTargetsInfo[ iVecPtr ]->isDamage = true;
	}
	else if ( TypeAction == this->eActionType::CriticalDamage )
	{
		vTargetsInfo[ iVecPtr ]->iValue = Value;
		vTargetsInfo[ iVecPtr ]->fTime = 10.0f;
		vTargetsInfo[ iVecPtr ]->isDamage = true;
		vTargetsInfo[ iVecPtr ]->isCritical = true;
	}

}


void CDamageInfoModel::OnActionEvent( UINT uPosition, UIEventArgs eArgs )
{

	if ( CHARACTERGAME->IsStageVillage() )
		return;

	ParticleOld_ptr pParticle = std::make_shared<ParticleOld>( );
	
	// If iItemSlot is more than vector size, return
	if ( uPosition >= vTargetsInfo.size() )
		return;

	// Get target Memory
	UnitData * pcUnit = UNITDATABYID( vTargetsInfo[uPosition]->TargetID );

	if ( vTargetsInfo[uPosition]->TargetID == -1 )
		pcUnit = UNITDATA;

	if( pcUnit && pcUnit->psModelAnimation && pcUnit->psModelAnimation->iType == ANIMATIONTYPE_Die )
	{
		vTargetsInfo[ uPosition ]->isDelete = true;
		return;
	}

	// If not have target memory, delete in the vector targets and return
	if ( !pcUnit )
	{
		vTargetsInfo[ uPosition ]->isDelete = true;
		return;
	}

	// If its block showing
	if ( vTargetsInfo[ uPosition ]->isBlock )
	{
		// Start particle int x,y,z = 0 and set the real position
		if ( vTargetsInfo[uPosition]->iParticleID == 0 )
			vTargetsInfo[uPosition]->iParticleID = pParticle->Start( iBlockInfoPart, pcUnit->sPosition.iX, pcUnit->sPosition.iY, pcUnit->sPosition.iZ );

		pParticle->SetAttachPosition( vTargetsInfo[uPosition]->iParticleID, D3DVECTOR{ (float)pcUnit->sPosition.iX, (float)pcUnit->sPosition.iY, (float)pcUnit->sPosition.iZ } );
	}
	// If its defense showing
	else if ( vTargetsInfo[ uPosition ]->isDefense )
	{
		// Start particle int x,y,z = 0 and set the real position
		if ( vTargetsInfo[uPosition]->iParticleID == 0 )
			vTargetsInfo[uPosition]->iParticleID = pParticle->Start( iDefenseInfoPart, pcUnit->sPosition.iX, pcUnit->sPosition.iY, pcUnit->sPosition.iZ );
		pParticle->SetAttachPosition( vTargetsInfo[uPosition]->iParticleID, D3DVECTOR{ (float)pcUnit->sPosition.iX, (float)pcUnit->sPosition.iY, (float)pcUnit->sPosition.iZ } );
	}
	// If its evade showing
	else if ( vTargetsInfo[ uPosition ]->isEvade )
	{
		// Start particle int x,y,z = 0 and set the real position
		if ( vTargetsInfo[uPosition]->iParticleID == 0 )
			vTargetsInfo[uPosition]->iParticleID = pParticle->Start( iEvadeInfoPart, pcUnit->sPosition.iX, pcUnit->sPosition.iY, pcUnit->sPosition.iZ );
		pParticle->SetAttachPosition( vTargetsInfo[uPosition]->iParticleID, D3DVECTOR{ (float)pcUnit->sPosition.iX, (float)pcUnit->sPosition.iY, (float)pcUnit->sPosition.iZ } );
	}

	// If its damage showing
	else if ( vTargetsInfo[ uPosition ]->isDamage )
	{
		// Draw Text
		int iX = pcUnit->rRenderRectangle.left;
		int iY = pcUnit->rRenderRectangle.top;

		if( ( iX < 0 ) || ( iY < 0 ) || ( iX > RESOLUTION_WIDTH ) || ( iY > RESOLUTION_WIDTH ) )
			return;

		int iXCenter = (pcUnit->rRenderRectangle.right - pcUnit->rRenderRectangle.left) >> 1;
		
		if ( pcUnit->rRenderRectangle.left < 10 )
		{
			iX = vTargetsInfo[ uPosition ]->iXScreen;
			iY = vTargetsInfo[ uPosition ]->iYScreen;
		}
		else
		{
			vTargetsInfo[ uPosition ]->iXScreen = iX;
			vTargetsInfo[ uPosition ]->iYScreen = iY;
		}

		char szBuf[32] = { 0 };
		STRINGFORMAT( szBuf,  pcUnit != UNITDATA ? "%d" : "-%d", vTargetsInfo[uPosition]->iValue );
		int iWidth = GETWIDTHTEXT( szBuf ) >> 1;

		int iAlpha = ((vTargetsInfo[uPosition]->iY > 0 ? vTargetsInfo[uPosition]->iY : 0) + 5) * 3;

		vTargetsInfo[uPosition]->bRender = true;

		if ( pcUnit != UNITDATA )
		{
			if ( vTargetsInfo[uPosition]->isCritical )
			{
				pBlueFont->SetPosition(iX + iXCenter - iWidth + 1, iY - ((pcUnit->iModelHeight >> 8) + 120) + ((int)vTargetsInfo[uPosition]->iY));
				pBlueFont->SetText( szBuf );
				pBlueFont->SetColor( D3DCOLOR_RGBA( 255, 255, 255, iAlpha ) );
				pBlueFont->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
//				vTargetsInfo[uPosition]->iY--;
			}
			else
			{
				pOrangeFont->SetPosition(iX + iXCenter - iWidth + 1, iY - ((pcUnit->iModelHeight >> 8) + 120) + ((int)vTargetsInfo[uPosition]->iY));
				pOrangeFont->SetText( szBuf );
				pOrangeFont->SetColor( D3DCOLOR_RGBA( 255, 255, 255, iAlpha ) );
				pOrangeFont->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				vTargetsInfo[uPosition]->iY--;
			}
		}
		else
		{
			pRedFont->SetPosition(iX + iXCenter - iWidth + 1, iY - ((pcUnit->iModelHeight >> 8) + 40) + ((int)vTargetsInfo[uPosition]->iY));
			pRedFont->SetText( szBuf );
			pRedFont->SetColor( D3DCOLOR_RGBA( 255, 255, 255, iAlpha ) );
			pRedFont->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			vTargetsInfo[uPosition]->iY--;
		}
	}
	else
		return;
}

void CDamageInfoModel::RenderTargets()
{

}

void CDamageInfoModel::Update()
{

}

void CDamageInfoModel::loadParticles()
{
	// Load Particles
	ParticleOld_ptr pParticle = std::make_shared<ParticleOld>( );
	iDefenseInfoPart	= pParticle->Load( DEFENSE_INFO_PART_FILE );
	iBlockInfoPart		= pParticle->Load( BLOCK_INFO_PART_FILE );
	iEvadeInfoPart		= pParticle->Load( EVADE_INFO_PART_FILE );
	iCriticalInfoPart   = pParticle->Load( CRITICAL_INFO_PART_FILE );
}

void CDamageInfoModel::SetScale( float f )
{
	fScale = f;
	pOrangeFont->SetScale( f );
	pRedFont->SetScale( f );
	pBlueFont->SetScale( f );
}

void CDamageInfoModel::Load()
{
	//Font
	pOrangeFont->ReadFontData( "game\\fonts\\font.dat" );
	pOrangeFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font1.png" ) );

	pRedFont->ReadFontData( "game\\fonts\\font.dat" );
	pRedFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font2.png" ) );

	pBlueFont->ReadFontData( "game\\fonts\\font3.dat" );
	pBlueFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font3.png" ) );
}


CDamageInfoModel::~CDamageInfoModel()
{
}
