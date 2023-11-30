#pragma once

#define D3DFVF_LIGHTVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE )

struct VertexLight
{
	float x, y, z;    // iItemSlot of vertex in 3D space
    float nx, ny, nz; // Normal for lighting calculations
    DWORD diffuse;    // Diffuse color of vertex
};


class CLightEffect
{
private:
	// Mesh and Light object
	D3DLIGHT9								lLight;
	D3DMATERIAL9							meshMaterial;
	D3DVECTOR								vPositionLight;
	LPDIRECT3DVERTEXBUFFER9					lpVertexMesh;
	D3DCOLORVALUE							sColorLight;
	float									faAttenuation[3];
	float									fRangeLight;
	int										iVertexMeshCount;
	int										iVertexSize;
	EImage									eImageTexture;

	BOOL									bCreatedMesh;

	int										CreateLightMesh( int iSize, float fMeshLengthAlongX, float fMeshLengthAlongZ );
	void									Setup();
public:
	CLightEffect();
	virtual ~CLightEffect();

	BOOL									Init();

	void									SetPosition( D3DVECTOR vPosition );
	void									GetPosition( D3DVECTOR * vPosition );
	void									SetRGBA( D3DCOLORVALUE sColor );
	void									SetRange( float fRange );
	void									SetSize( int iSize );
	void									SetImage( EImage eImage );

	void									Render();
};

