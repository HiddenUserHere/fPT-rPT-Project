#include "stdafx.h"
#include "EXEMatrix.h"
#include "point.h"

EXEMatrixF EXEMatrixConvertIF( const EXEMatrixI & i )
{
	EXEMatrixF f;

	for( int a = 0; a < 4; a++ )
	{
		for( int b = 0; b < 4; b++ )
		{
			f.f[a][b] = (float)i.i[a][b] / 256.f;
		}
	}

	return f;
}

EXEMatrixI EXEMatrixConvertFI( const EXEMatrixF & f )
{
	EXEMatrixI r;

	for( int a = 0; a < 4; a++ )
	{
		for( int b = 0; b < 4; b++ )
		{
			r.i[a][b] = (int)(f.f[a][b] * 256.f);
		}
	}

	return r;
}

EXEMatrixE EXEMatrixConvertIE( const EXEMatrixI & i )
{
	EXEMatrixE e;

	for( int a = 0; a < 4; a++ )
	{
		for( int b = 0; b < 4; b++ )
		{
			e.e[a][b] = i.i[a][b];
		}
	}

	return e;
}

EXEMatrixI EXEMatrixConvertEI( const EXEMatrixE & e )
{
	EXEMatrixI i;

	for( int a = 0; a < 4; a++ )
	{
		for( int b = 0; b < 4; b++ )
		{
			i.i[a][b] = e.e[a][b];
		}
	}

	return i;
}

EXEMatrixF EXEMatrixMultiplyF( const EXEMatrixF & a, const EXEMatrixF & b )
{
	EXEMatrixF r;

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			float total = 0.f;

			for( int k = 0; k < 4; k++ )
				total += a.f[j][k] * b.f[k][i];

			r.f[j][i] = total;
		}
	}

	return r;
}

EXEMatrixI EXEMatrixMultiplyI( const EXEMatrixI & a, const EXEMatrixI & b )
{
	EXEMatrixI r;

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			int total = 0;

			for( int k = 0; k < 4; k++ )
				total += (int)((a.i[j][k] * b.i[k][i]) >> 8);

			r.i[j][i] = (int)total;
		}
	}

	return r;
}

EXEMatrixE EXEMatrixMultiplyE( const EXEMatrixE & a, const EXEMatrixE & b )
{
	EXEMatrixE r;

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			int total = 0;

			for( int k = 0; k < 4; k++ )
				total += (int)((a.e[j][k] * b.e[k][i]) >> 15);

			r.e[j][i] = (int)total;
		}
	}

	return r;
}

EXEMatrixI EXEMatrixInvert( const EXEMatrixI & f )
{
	int inv[16];
	int * m = (int *)&f.i;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	EXEMatrixI r;
	EXEMatrixIdentityI( r );

	float det = (float)(m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12]) / 256.0f;

	if ( det == 0 )
		return r;

	det = 1.0f / det;

	int * invOut = (int *)&r.i;

	for ( int i = 0; i < 16; i++ )
		invOut[i] = (int)(((float)inv[i] / 256.0f) * det) << 8;

	return r;
}


EXEMatrixF EXEMatrixInvert( const EXEMatrixF & f )
{
	float inv[16];
	float * m = (float*)&f.f;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

	EXEMatrixF r;
	EXEMatrixIdentityF( r );

	float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return r;

    det = 1.0f / det;

	float * invOut = (float*)&r.f;

    for (int i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
	
	return r;
}

void EXEMatrixRotateXI( EXEMatrixI & i, int rad )
{
	const int c = (int)( PTCOS[rad] >> 8 );
	const int s = (int)( PTSEN[rad] >> 8 );

	i.i[1][1] = c;
	i.i[1][2] = s;
	i.i[2][1] = -s;
	i.i[2][2] = c;
}

void EXEMatrixRotateYI( EXEMatrixI & i, int rad )
{
	const int c = (int)( PTCOS[rad] >> 8 );
	const int s = (int)( PTSEN[rad] >> 8 );

	i.i[0][0] = c;
	i.i[0][2] = -s;
	i.i[2][0] = s;
	i.i[2][2] = c;
}

void EXEMatrixRotateZI( EXEMatrixI & i, int rad )
{
	const int c = (int)( PTCOS[rad] >> 8 );
	const int s = (int)( PTSEN[rad] >> 8 );

	i.i[0][0] = c;
	i.i[0][1] = s;
	i.i[1][0] = -s;
	i.i[1][1] = c;
}

void EXEMatrixRotateXF( EXEMatrixF & f, int rad )
{
	const float c = (float)( PTCOS[rad] >> 8 );
	const float s = (float)( PTSEN[rad] >> 8 );

	f.f[1][1] = c;
	f.f[1][2] = s;
	f.f[2][1] = -s;
	f.f[2][2] = c;
}

void EXEMatrixRotateYF( EXEMatrixF & f, int rad )
{
	const float c = (float)( PTCOS[rad] >> 8 );
	const float s = (float)( PTSEN[rad] >> 8 );

	f.f[0][0] = c;
	f.f[0][2] = -s;
	f.f[2][0] = s;
	f.f[2][2] = c;
}

void EXEMatrixRotateZF( EXEMatrixF & f, int rad )
{
	const float c = (float)( PTCOS[rad] >> 8 );
	const float s = (float)( PTSEN[rad] >> 8 );

	f.f[0][0] = c;
	f.f[0][1] = s;
	f.f[1][0] = -s;
	f.f[1][1] = c;
}

void EXEMatrixRotateXE( EXEMatrixE & e, int rad )
{
	const int c = (int)( PTCOS[rad] >> 8 );
	const int s = (int)( PTSEN[rad] >> 8 );

	e.e[1][1] = c;
	e.e[1][2] = s;
	e.e[2][1] = -s;
	e.e[2][2] = c;
}

void EXEMatrixRotateYE( EXEMatrixE & e, int rad )
{
	const int c = (int)( PTCOS[rad] >> 8 );
	const int s = (int)( PTSEN[rad] >> 8 );

	e.e[0][0] = c;
	e.e[0][2] = -s;
	e.e[2][0] = s;
	e.e[2][2] = c;
}

void EXEMatrixRotateZE( EXEMatrixE & e, int rad )
{
	const int c = (int)( PTCOS[rad] >> 8 );
	const int s = (int)( PTSEN[rad] >> 8 );

	e.e[0][0] = c;
	e.e[0][1] = s;
	e.e[1][0] = -s;
	e.e[1][1] = c;
}

void EXEMatrixIdentityF( EXEMatrixF & f )
{
	for( int i = 0; i < 4; i++ )
		for( int j = 0; j < 4; j++ )
			f.f[i][j] = (i == j ? 1.f : 0.f);
}

void EXEMatrixIdentityI( EXEMatrixI & i )
{
	for( int j = 0; j < 4; j++ )
		for( int k = 0; k < 4; k++ )
			i.i[j][k] = (j == k ? 256 : 0);
}

void EXEMatrixIdentityE( EXEMatrixE & e )
{
	for( int j = 0; j < 4; j++ )
		for( int k = 0; k < 4; k++ )
			e.e[j][k] = (j == k ? 32768 : 0);
}

void EXEMatrixLeftShift( EXEMatrixI & i, int iLeftShift )
{
	for( int j = 0; j < 4; j++ )
		for( int k = 0; k < 4; k++ )
			i.i[j][k] <<= iLeftShift;
}

void EXEMatrixRightShift( EXEMatrixI & i, int iRightShift )
{
	for ( int j = 0; j < 4; j++ )
		for ( int k = 0; k < 4; k++ )
			i.i[j][k] >>= iRightShift;
}

void EXEMatrixTranslate( EXEMatrixI & i, int iX, int iY, int iZ )
{
	i.i[3][0] = iX << 8;
	i.i[3][1] = iY << 8;
	i.i[3][2] = iZ << 8;
}

void EXEMatrixFromQuaterion( EXEMatrixF & s, float x, float y, float z, float w )
{
	float xx = x * x; 
	float yy = y * y;
	float zz = z * z;
	float xy = x * y; 
	float xz = x * z; 
	float yz = y * z;
	float wx = w * x; 
	float wy = w * y; 
	float wz = w * z;

	s.f[0][0] = 1 - 2 * ( yy + zz );
	s.f[0][1] = 2 * ( xy - wz );
	s.f[0][2] = 2 * ( xz + wy );

	s.f[1][0] = 2 * ( xy + wz );
	s.f[1][1] = 1 - 2 * ( xx + zz );
	s.f[1][2] = 2 * ( yz - wx );

	s.f[2][0] = 2 * ( xz - wy );
	s.f[2][1] = 2 * ( yz + wx );
	s.f[2][2] = 1 - 2 * ( xx + yy );

	s.f[0][3] = s.f[1][3] = s.f[2][3] = 0;
	s.f[3][0] = s.f[3][1] = s.f[3][2] = 0;
	s.f[3][3] = 1;
}


Vector4DF EXEQuaternionSlerp( Vector4DF & v0, Vector4DF & v1, float T )
{
	//First of all, an inside function to clamp...
	auto Clamp = []( float value, float low, float high )->float
	{
		return max( min( high, value ), low );
	};

	float fT = T;

	Vector4DF vResult( 0, 0, 0, 0 );

	//Get dot product from vectors...
	float fDot = v0.Dot( v1 );

	//Negative? Flip Quaternion...
	if ( fDot < 0.0f )
	{
		fDot = -fDot;

		v0.Negative();
	}

	//Too close? use linear interpolation
	if ( fDot > 0.9995f )
	{
		//Scale using difference between frames
		Vector4DF vScale = Vector4DF( v1 - v0 );
		vScale.Scale( fT );

		//Pass Scaled vector to Quartenion and normalize it...
		vResult = Vector4DF( v0 + vScale );
	}
	else
	{
		//Clamp it, then makes a range [-1, 1]
		Clamp( fDot, -1, 1 );

		//Get Theta
		float fTheta = acos( fDot ) * fT;

		//Scale dot product from vectors
		Vector4DF vScale = v0;
		vScale.Scale( fDot );

		//Set scaled vector and normalize it...
		Vector4DF v2 = (v1 - vScale);

		//Set new vector and scale it cos
		Vector4DF vcos = v0;
		vcos.Scale( cos( fTheta ) );

		//Scale sin
		Vector4DF vsin = v2;
		vsin.Scale( sin( fTheta ) );

		//Pass Scaled vectors to Quartenion
		vResult = Vector4DF( vcos + vsin );
	}

	return vResult;
}

Vector4DF EXEQuaternionLerp( Vector4DF & v0, Vector4DF & v1, float T )
{
	//First of all, an inside function to clamp...
	auto Clamp = []( float value, float low, float high )->float
	{
		return max( min( high, value ), low );
	};

	Vector4DF vResult( 0, 0, 0, 0 );

	float fT = 1 - T;

	vResult.fX = fT * v0.fX + fT * v1.fX;
	vResult.fY = fT * v0.fY + fT * v1.fY;
	vResult.fZ = fT * v0.fZ + fT * v1.fZ;
	vResult.fW = fT * v0.fW + fT * v1.fW;

	vResult.Normalize();

	return vResult;
}

Point3D EXEMatrixGetPosition( EXEMatrixI & i )
{
	return Point3D( i.i[3][0], i.i[3][2], i.i[3][1] );
}
