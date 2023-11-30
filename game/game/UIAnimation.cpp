#include "stdafx.h"
#include "UIAnimation.h"

namespace UI
{
void Animation::Constructor()
{
	iFrameInterval	= 70;

	fTimeUpdate		= (1000.0f / (float)iFrameInterval);

	iFrameWheel		= 0;

	iMaxFrames		= 0;

	fTime			= 0.0f;
}
Animation::Animation( std::string strFileName ) : _strFileName( strFileName )
{
	Constructor();

	INI::CReader cReader( (char*)strFileName.c_str() );

	int iCount		= cReader.ReadInt( "Animation", "Count" );
	iFrameInterval	= cReader.ReadInt( "Animation", "FrameInterval" );
	fTimeUpdate		= (1000.0f / (float)iFrameInterval);

	for ( int i = 0; i < iCount; i++ )
	{
		Rectangle2D r( 0, 0, 0, 0 );

		char szSection[64] = { 0 };

		STRINGFORMAT( szSection, "Animation%d", i + 1 );

		r.iX		= cReader.ReadInt( szSection, "X" );
		r.iY		= cReader.ReadInt( szSection, "Y" );
		r.iWidth	= cReader.ReadInt( szSection, "Width" );
		r.iHeight	= cReader.ReadInt( szSection, "Height" );

		AnimationData_ptr pData = std::make_shared<AnimationData>( r );

		Add( pData );
	}
}

Animation::Animation() : _strFileName( "" )
{
	Constructor();
}


Animation::~Animation()
{
}

void Animation::SetFrameInterval( int i )
{
	iFrameInterval	= i;

	fTimeUpdate		= iFrameInterval > 0 ? (1000.0f / (float)iFrameInterval) : 0.0f;
}

void Animation::Add( AnimationData_ptr pData )
{
	vData.push_back( pData );

	iMaxFrames = (int)vData.size();
}

void Animation::Update( float fTime )
{
	if ( iMaxFrames > 0 )
	{
		this->fTime += fTime;

		if ( this->fTime >= fTimeUpdate )
		{
			iFrameWheel++;

			this->fTime = 0.0f;
		}
	}
}

AnimationData_ptr Animation::GetAnimationData()
{
	if ( iMaxFrames > 0 )
		return vData[iFrameWheel % iMaxFrames];

	return NULL;
}

AnimationData::AnimationData( Rectangle2D rRectangle ) : rRect( rRectangle )
{

}

AnimationData::~AnimationData()
{
}

}