#pragma once

namespace DX
{

typedef std::shared_ptr<class Effect> Effect_ptr;

class Effect
{
public:
							  Effect( LPD3DXEFFECT lpEffect, std::string strEffectFile );
							  Effect( const Effect & c );
	virtual					 ~Effect();

	LPD3DXEFFECT			  Get() { return lpEffect; }
	const std::string		& GetFile() const { return strEffectFile; }
	UINT					  GetPasses() { return uPasses; }

	void					  OnLostDevice();
	void					  OnResetDevice();

	BOOL					  SetTechnique( std::string strTechnique );

	int						  Begin();

	void					  SetMatrix( const std::string & strVariableName, const D3DXMATRIX & mMatrix );
	void					  SetMatrixArray( const std::string & strVariableName, const D3DXMATRIX * pmaValue, UINT uCount );
	void					  SetInt( const std::string & strVariableName, int iValue );
	void					  SetIntArray( const std::string & strVariableName, const int * piaValue, UINT uCount );
	void					  SetFloat( const std::string & strVariableName, float fValue );
	void					  SetFloatArray( const std::string & strVariableName, const float * pfaValue, UINT uCount );
	void					  SetBool( const std::string & strVariableName, BOOL bValue );
	void					  SetBoolArray( const std::string & strVariableName, const BOOL * pbaValue, UINT uCount );

	BOOL					  BeginPass( UINT uPass );

	void					  CommitChanges();

	void					  EndPass();

	void					  End();

	void					  Renew( LPD3DXEFFECT _lpEffect );

private:
	LPD3DXEFFECT			  lpEffect;
	std::string				  strEffectFile;

	UINT					  uPasses = 0;
};

}