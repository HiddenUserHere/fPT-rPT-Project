#pragma once
#include "UIElement.h"

namespace UI
{

typedef std::shared_ptr<class AnimationData> AnimationData_ptr;
class AnimationData
{
private:
	Rectangle2D						rRect;
public:
	AnimationData( Rectangle2D rRectangle );

	virtual ~AnimationData();

	Rectangle2D						& GetRect() { return rRect; }
};

typedef std::shared_ptr<class Animation> Animation_ptr;
class Animation
{
private:
	std::vector<AnimationData_ptr>	vData;

	int								iFrameInterval;

	int								iMaxFrames;

	int								iFrameWheel;

	float							fTimeUpdate;

	float							fTime;

	std::string						_strFileName;

	void							Constructor();

public:

	Animation( std::string strFileName );
	Animation();
	virtual ~Animation();

	void							Clear() { }

	void							SetFrameInterval( int i );

	void							Add( AnimationData_ptr pData );
	void							Update( float fTime );

	BOOL							HaveAnimation() { return (iMaxFrames > 0); }

	AnimationData_ptr				GetAnimationData();

};

}