#pragma once

struct EXEMatrixI
{
	union
	{
		int i[4][4];

		struct  
		{
			int _11, _12, _13, _14;
			int _21, _22, _23, _24;
			int _31, _32, _33, _34;
			int _41, _42, _43, _44;
		};
	};
	EXEMatrixI() {};

#if defined(_GAME)
	EXEMatrixI( Delta3D::Math::Matrix4 & m )
	{
		for ( int i = 0; i < 4; i++ )
			for ( int j = 0; j < 4; j++ )
				this->i[i][j] = (int)m.m[i][j];
	};

	const void operator=( const Delta3D::Math::Matrix4 & m )
	{
		for ( int i = 0; i < 4; i++ )
			for ( int j = 0; j < 4; j++ )
				this->i[i][j] = (int)m.m[i][j];
	};
	const void operator=( const Delta3D::Math::Matrix4 * m )
	{
		for ( int i = 0; i < 4; i++ )
			for ( int j = 0; j < 4; j++ )
				this->i[i][j] = (int)m->m[i][j];
	};
#endif

	~EXEMatrixI() {};
};

struct EXEMatrixE
{
	int e[4][4];
};

struct EXEMatrixF
{
	float f[4][4];
};

struct EXEMatrixD
{
	double d[4][4];
};

EXEMatrixF EXEMatrixConvertIF( const EXEMatrixI & i );
EXEMatrixI EXEMatrixConvertFI( const EXEMatrixF & f );
EXEMatrixE EXEMatrixConvertIE( const EXEMatrixI & i );
EXEMatrixI EXEMatrixConvertEI( const EXEMatrixE & e );

EXEMatrixF EXEMatrixMultiplyF( const EXEMatrixF & a, const EXEMatrixF & b );
EXEMatrixI EXEMatrixMultiplyI( const EXEMatrixI & a, const EXEMatrixI & b );
EXEMatrixE EXEMatrixMultiplyE( const EXEMatrixE & a, const EXEMatrixE & b );

EXEMatrixI EXEMatrixInvert( const EXEMatrixI & f );
EXEMatrixF EXEMatrixInvert( const EXEMatrixF & f );

void EXEMatrixRotateXI( EXEMatrixI & i, int rad );
void EXEMatrixRotateYI( EXEMatrixI & i, int rad );
void EXEMatrixRotateZI( EXEMatrixI & i, int rad );

void EXEMatrixRotateXF( EXEMatrixF & f, int rad );
void EXEMatrixRotateYF( EXEMatrixF & f, int rad );
void EXEMatrixRotateZF( EXEMatrixF & f, int rad );

void EXEMatrixRotateXE( EXEMatrixE & e, int rad );
void EXEMatrixRotateYE( EXEMatrixE & e, int rad );
void EXEMatrixRotateZE( EXEMatrixE & e, int rad );

void EXEMatrixIdentityF( EXEMatrixF & f );
void EXEMatrixIdentityI( EXEMatrixI & i );
void EXEMatrixIdentityE( EXEMatrixE & e );

void EXEMatrixLeftShift( EXEMatrixI & i, int iShiftLeft );
void EXEMatrixRightShift( EXEMatrixI & i, int iShiftRight );

void EXEMatrixTranslate( EXEMatrixI & i, int iX, int iY, int iZ );

void EXEMatrixFromQuaterion( EXEMatrixF & s, float x, float y, float z, float w );

Vector4DF EXEQuaternionSlerp( Vector4DF & v0, Vector4DF & v1, float T );
Vector4DF EXEQuaternionLerp( Vector4DF & v0, Vector4DF & v1, float T );

Point3D EXEMatrixGetPosition( EXEMatrixI & i );