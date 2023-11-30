#include "stdafx.h"
#include "CGameTimer.h"


CGameTimer::CGameTimer() : CTimer()
{
	pcList = NULL;
}

CGameTimer::~CGameTimer()
{
	RELEASE( pcList );
}

BOOL CGameTimer::Init()
{
	return TRUE;
}

void CGameTimer::Shutdown()
{
	CList<Base::Timer*> * item = pcList;
	while ( item )
	{
		CList<Base::Timer*> * next = item->next;

		DELET( item->obj );

		item = next;
	}
}

void CGameTimer::Update( double fTime )
{

	CList<Base::Timer*> * item = pcList;
	while ( item != NULL )
	{
		CList<Base::Timer*> * next = item->next;

		Base::Timer * timer = item->obj;
		if ( timer != NULL )
		{
			if ( timer->Update( fTime ) )
			{
				DELET( timer );
				DELET( item );
			}
		}
		else
		{
			DELET( item );
		}

		item = next;
	}
}

void CGameTimer::Add( Base::Timer * pTimer )
{
	if ( pcList == NULL )
	{
		pcList = new CList<Base::Timer*>( pTimer );
		return;
	}

	pcList->Add( pTimer );
}