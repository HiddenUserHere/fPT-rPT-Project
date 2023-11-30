#include "stdafx.h"
#include "Event.h"


namespace Base
{

Event::Event() : Node()
{
}

Event::Event( const Event & c ) : Node(c)
{
	for( vector<EventHandler*>::const_iterator it = c.vEventHandlers.begin(); it != c.vEventHandlers.end(); it++ )
	{
		EventHandler * pEventHandler = *it;

		vEventHandlers.push_back( new EventHandler( *pEventHandler ) );
	}
}

Event::~Event()
{
	for( vector<EventHandler*>::iterator it = vEventHandlers.begin(); it != vEventHandlers.end(); it++ )
	{
		EventHandler * pEventHandler = *it;

		DELET( pEventHandler );
	}
}

void Event::AddEventHandler( EventHandler * pEventHandler )
{
	vEventHandlers.push_back( pEventHandler );
}

void Event::Fire()
{
	for( vector<EventHandler*>::iterator it = vEventHandlers.begin(); it != vEventHandlers.end(); it++ )
	{
		EventHandler * c = *it;

		c->Fire();
	}
}

Event * EventFactory::BuildEvent( void * pOnEventHandler )
{
	Event * pEvent = new Event();

	pEvent->AddEventHandler( new EventHandler( pOnEventHandler ) );

	return pEvent;
}

Event * EventFactory::BuildEvent( void * pOnEventHandler, void * pPointer1 )
{
	Event * pEvent = new Event();

	pEvent->AddEventHandler( new EventHandler1( pOnEventHandler, pPointer1 ) );

	return pEvent;
}

Event * EventFactory::BuildEvent( void * pOnEventHandler, void * pPointer1, void * pPointer2 )
{
	Event * pEvent = new Event();

	pEvent->AddEventHandler( new EventHandler2( pOnEventHandler, pPointer1, pPointer2 ) );

	return pEvent;
}

}