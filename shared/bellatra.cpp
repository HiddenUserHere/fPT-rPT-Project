#include "stdafx.h"
#include "bellatra.h"

namespace SOD
{
CBellatraRound::CBellatraRound()
{
}

CBellatraRound::~CBellatraRound()
{
	for ( auto psMonster : vMonsters )
		DELET( psMonster );

	vMonsters.clear();

	for ( auto psBoss : vBosses )
		DELET( psBoss );

	vBosses.clear();

	for ( auto psDrop : vItemsDrop )
		DELET( psDrop );

	vItemsDrop.clear();

	for ( auto psArea : vAreas )
		DELET( psArea );

	vAreas.clear();
}

CBellatra::CBellatra()
{
}

CBellatra::~CBellatra()
{
	for ( auto pcRound : vRounds )
		DELET( pcRound );

	vRounds.clear();
}
};