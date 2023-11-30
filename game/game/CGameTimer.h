#pragma once

#include "CTimer.h"

#include "Timer.h"
#include "CList.h"

class CGameTimer : public CTimer
{
public:
	CGameTimer();
	virtual				 ~CGameTimer();

	BOOL				  Init();
	void				  Shutdown();

	void				  Update( double fTime );

	void				  Add( Base::Timer * pTimer );

private:
	CList<Base::Timer*>	* pcList;

};