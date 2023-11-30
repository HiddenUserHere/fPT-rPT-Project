#pragma once

#include "UIElement.h"

namespace UI
{
typedef std::shared_ptr<class Group> Group_ptr;
class Group : public Element
{
public:
	//Constructor
														Group();
	virtual											   ~Group();

	void												Clear() {};

	//Setters and Getters
	void												SetGroup( std::shared_ptr<Group> pGroupObj );
	void												SetIndexObject( int iIDObject ) { iIDIndexObject = iIDObject; };
	int													GetIndexObject() { return iIDIndexObject; };
	int													GetType() { return TYPE_Group; };
	std::shared_ptr<class Group>						GetGroup() { return pGroup; };
protected:
	int													iYMoveObject	= 0;
	int													iIDIndexObject	= -1;
	std::shared_ptr<Group>								pGroup			= NULL;
};
}
