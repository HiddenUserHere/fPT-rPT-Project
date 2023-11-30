#include "stdafx.h"
#include "userdataserver.h"

BOOL SUserData::IsDead()
{
	if ( this->sCharacterData.sHP.sCur <= 0 )
		return TRUE;

	return FALSE;
}