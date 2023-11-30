#include "stdafx.h"
#include "DXEffect.h"

#include "DX.h"

namespace DX
{

Effect::Effect( LPD3DXEFFECT lpEffect, std::string strEffectFile ) : lpEffect(lpEffect), strEffectFile(strEffectFile)
{
	ADDREF( lpEffect );
}

Effect::Effect( const Effect & c ) : lpEffect(c.lpEffect), strEffectFile(c.strEffectFile)
{
	ADDREF( lpEffect );
}

Effect::~Effect()
{
	RELEASE( lpEffect );
}

void Effect::OnLostDevice()
{
	if( lpEffect )
		lpEffect->OnLostDevice();
}

void Effect::OnResetDevice()
{
	if( lpEffect )
		lpEffect->OnResetDevice();
}

BOOL Effect::SetTechnique( std::string strTechnique )
{
	if( !lpEffect )
		return FALSE;

	return SUCCEEDED( lpEffect->SetTechnique( strTechnique.c_str() ) );
}

int Effect::Begin()
{
	if( !lpEffect )
		return -1;
	
	uPasses = 0;
	return SUCCEEDED( lpEffect->Begin( &uPasses, D3DXFX_DONOTSAVESAMPLERSTATE ) ) ? uPasses : -1;
}

void Effect::SetMatrix( const std::string & strVariableName, const D3DXMATRIX & mMatrix )
{
	if( !lpEffect )
		return;
	
	HRESULT hr;
	if ( FAILED( hr = lpEffect->SetMatrix( strVariableName.c_str(), &mMatrix ) ) )
	{
	//	//Logger::GetInstance().Print( Logger::Debug, hr, "ID3DXEffect::SetMatrix() failed" );
	}
}

void Effect::SetMatrixArray( const std::string & strVariableName, const D3DXMATRIX * pmaValue, UINT uCount )
{
	if( !lpEffect )
		return;
	
	HRESULT hr;
	if ( FAILED( hr = lpEffect->SetMatrixArray( strVariableName.c_str(), pmaValue, uCount ) ) )
	{
	//	//Logger::GetInstance().Print( Logger::Debug, hr, "ID3DXEffect::SetMatrixArray() failed" );
	}
}

void Effect::SetInt( const std::string & strVariableName, int iValue )
{
	if( lpEffect )
		lpEffect->SetInt( strVariableName.c_str(), iValue );
}

void Effect::SetIntArray( const std::string & strVariableName, const int * piaValue, UINT uCount )
{
	if( lpEffect )
		lpEffect->SetIntArray( strVariableName.c_str(), piaValue, uCount );
}

void Effect::SetFloat( const std::string & strVariableName, float fValue )
{
	if( lpEffect )
		lpEffect->SetFloat( strVariableName.c_str(), fValue );
}

void Effect::SetFloatArray( const std::string & strVariableName, const float * pfaValue, UINT uCount )
{
	if( lpEffect )
		lpEffect->SetFloatArray( strVariableName.c_str(), pfaValue, uCount );
}

void Effect::SetBool( const std::string & strVariableName, BOOL bValue )
{
	if( lpEffect )
		lpEffect->SetBool( strVariableName.c_str(), bValue );
}

void Effect::SetBoolArray( const std::string & strVariableName, const BOOL * pbaValue, UINT uCount )
{
	if( lpEffect )
		lpEffect->SetBoolArray( strVariableName.c_str(), pbaValue, uCount );
}

BOOL Effect::BeginPass( UINT uPass )
{
	if( !lpEffect )
		return FALSE;

	return SUCCEEDED( lpEffect->BeginPass( uPass ) );
}

void Effect::CommitChanges()
{
	if( !lpEffect )
		return;

	lpEffect->CommitChanges();
}

void Effect::EndPass()
{
	if( lpEffect )
		lpEffect->EndPass();
}

void Effect::End()
{
	if( lpEffect )
		lpEffect->End();
}

void Effect::Renew( LPD3DXEFFECT _lpEffect )
{
	RELEASE( lpEffect );

	lpEffect = _lpEffect;

	ADDREF( lpEffect );
}

}