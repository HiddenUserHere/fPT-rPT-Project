#pragma once
class CCamera
{
public:
	CCamera();
	virtual ~CCamera();

	void					Update( double fTime );

	void					SetViewMatrix( const D3DXMATRIX & mat ) { matView = mat; };
	D3DXMATRIX				GetViewMatrix() { return matView; }

	void					SetPosition( const D3DXVECTOR3 & v ) { vPosition = v; };
	D3DXVECTOR3				& GetPosition() { return vPosition; }
	
	void					SetLookAt( const D3DXVECTOR3 & v ) { vLookAt = v; };
	D3DXVECTOR3				& GetLookAt() { return vLookAt; }

	void					AddToYawPitch( float fYaw, float fPitch ) { fYawVelocity = fYaw; fPitchVelocity = fPitch; }
	void					AddToVelocity( const D3DXVECTOR3 & v ) { vVelocity += v; }

	void					SetYaw( float f ) { fYaw = f; }
	void					SetPitch( float f ) { fPitch = f; }

	Point3D sCameraPosition;
	Point3D sCameraAngle;

protected:
	D3DXMATRIX					  matView;
	D3DXMATRIX					  matOrientation;

	D3DXVECTOR3					  vPosition;
	D3DXVECTOR3					  vLookAt;
	D3DXVECTOR3					  vVelocity;

	float						  fYaw;
	float						  fYawVelocity;

	float						  fPitch;
	float						  fPitchVelocity;
};

