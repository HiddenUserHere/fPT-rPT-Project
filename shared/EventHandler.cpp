#include "stdafx.h"
#include "EventHandler.h"

namespace Base
{

EventHandler::EventHandler( void * _pOnEventHandler )
{
	pOnEventHandler		= _pOnEventHandler;
}

EventHandler::EventHandler( const EventHandler & c )
{
	pOnEventHandler		= c.pOnEventHandler;
}

void EventHandler::Fire()
{
	typedef void OnEventHandler();

	OnEventHandler * pOnEventHandlerFunction = (OnEventHandler *)pOnEventHandler;

	if( pOnEventHandlerFunction )
		pOnEventHandlerFunction();
}

EventHandler1::EventHandler1( void * _pOnEventHandler, void * _pPointer1 ) : EventHandler( _pOnEventHandler )
{
	pPointer1			= _pPointer1;
}

EventHandler1::EventHandler1( const EventHandler1 & c ) : EventHandler(c)
{
	pPointer1			= c.pPointer1;
}

void EventHandler1::Fire()
{
	typedef void OnEventHandler( void * pPointer1 );

	OnEventHandler * pOnEventHandlerFunction = (OnEventHandler *)pOnEventHandler;

	if( pOnEventHandlerFunction )
		pOnEventHandlerFunction( pPointer1 );
}

EventHandler2::EventHandler2( void * _pOnEventHandler, void * _pPointer1, void * _pPointer2 ) : EventHandler1( _pOnEventHandler, _pPointer1 )
{
	pPointer2			= _pPointer2;
}

EventHandler2::EventHandler2( const EventHandler2 & c ) : EventHandler1(c)
{
	pPointer2			= c.pPointer2;
}

void EventHandler2::Fire()
{
	typedef void OnEventHandler( void * pPointer1, void * pPointer2 );

	OnEventHandler * pOnEventHandlerFunction = (OnEventHandler *)pOnEventHandler;

	if( pOnEventHandlerFunction )
		pOnEventHandlerFunction( pPointer1, pPointer2 );
}

}