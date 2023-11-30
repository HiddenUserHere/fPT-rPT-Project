#pragma once

struct RawSMDVector3I
{
	int			  iX;
	int			  iY;
	int			  iZ;

	D3DXVECTOR3 ToVector() const
	{
		D3DXVECTOR3 v;

		v.x = ((float)iX) / 256.0f;
		v.y = ((float)iY) / 256.0f;
		v.z = ((float)iZ) / 256.0f;

		return v;
	}
};