#pragma once

struct RawSMDTextureLink
{
	float				  u[3];
	float				  v[3];

	int					  iUnknown1;

	RawSMDTextureLink	* pNext;
};