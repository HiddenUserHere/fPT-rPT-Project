#include "stdafx.h"
#include "UIGroup.h"
#include "UI.h"

namespace UI
{
Group::Group() : Element()
{
}

Group::~Group()
{
}

void Group::SetGroup( std::shared_ptr<Group> pGroupObj )
{
	pGroup = pGroupObj;
}
}


