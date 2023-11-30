#pragma once

#include "UIElement.h"

struct UIEventArgs {};

namespace UI
{
typedef std::shared_ptr<class Event> Event_ptr;
class Event : public Element
{
public:
	//Constructor
												Event();
	virtual									   ~Event();

	//Setters and Getters
	void										SetEventID( int iEvent );

	static int									Build( std::function<void( UIEventArgs const & )> const & f );
	static void									Invoke( int iHandle );

	//Events Handlers
	void										OnClick( EButton eButton );

private:
	int											iEventID = 0;
};
}