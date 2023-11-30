#pragma once

namespace Base
{

struct EventHandler
{
					  EventHandler( void * _pOnEventHandler );
					  EventHandler( const EventHandler & c );
	virtual			 ~EventHandler() {}

	virtual void	  Fire();

protected:
	void			* pOnEventHandler;

};

struct EventHandler1 : EventHandler
{
public:
					  EventHandler1( void * _pOnEventHandler, void * _pPointer1 );
					  EventHandler1( const EventHandler1 & c );
	virtual			 ~EventHandler1() {}

	virtual void	  Fire();

protected:
	void			* pPointer1;

};

struct EventHandler2 : EventHandler1
{
public:
					  EventHandler2( void * _pOnEventHandler, void * _pPointer1, void * _pPointer2 );
					  EventHandler2( const EventHandler2 & c );
	virtual			 ~EventHandler2() {}

	virtual void	  Fire();

protected:
	void			* pPointer2;

};

}