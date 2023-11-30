#include "stdafx.h"
#include "CServerWorld.h"

#include "CServer.h"

CServerWorld::CServerWorld()
{
	pcWorld = NULL;
}

BOOL CServerWorld::Init()
{
	vWorlds.push_back( new CWorld( WORLDID_None, "None", 0x00000000, 0, "", "", "", 0x00000000, 0x00000000 ) );
	vWorlds.push_back( new CWorld( WORLDID_Fortress, "Regnum", 0x6C911155, 10009, "Regnum World", "FW", "www.RegnumPT.net", 0x8D911155, 0x00000000 ) );
	vWorlds.push_back( new CWorld( WORLDID_Test, "Beta", 0x5B6CD35F, 10009, "Beta", "Beta", "FortressPT.net", 0x00000000, 0x00000000 ) );

	SetWorld( SERVERCONFIG->GetServerWorld() );

	return TRUE;
}

void CServerWorld::Shutdown()
{
	for (std::vector<CWorld*>::iterator it = vWorlds.begin(); it != vWorlds.end(); it++)
	{
		CWorld * p = *it;

		DELET( p );
	}
	vWorlds.clear();

	pcWorld = NULL;
}

void CServerWorld::SetWorld( EWorldID iWorldID )
{
	for (std::vector<CWorld*>::iterator it = vWorlds.begin(); it != vWorlds.end(); it++)
	{
		CWorld * p = *it;

		if( p->GetWorldID() == iWorldID )
		{
			pcWorld = p;
			return;
		}
	}

	pcWorld = NULL;
}

void CServerWorld::SetWorld( std::string strWorld )
{
	for (std::vector<CWorld*>::iterator it = vWorlds.begin(); it != vWorlds.end(); it++)
	{
		CWorld * p = *it;

		if( p->GetName().compare( strWorld ) == 0 )
		{
			pcWorld = p;
			return;
		}
	}

	SetWorld( WORLDID_None );
}