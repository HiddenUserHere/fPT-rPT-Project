#include "stdafx.h"
#include "HalloweenModel.h"


CHalloweenModel::CHalloweenModel()
{
}


CHalloweenModel::~CHalloweenModel()
{
}

void CHalloweenModel::SetDayTime( DWORD * pdwHour, DWORD * pdwMinute )
{
	// Event is On?
	if ( HALLOWEENHANDLER->EventIsAlive() )
	{
		// Set Night ever
		//*pdwHour		= 0;
		//*pdwMinute		= 0;
	}
}
