#include "stdafx.h"
#include "CDropServerHandler.h"


CDropServerHandler::CDropServerHandler()
{
}


CDropServerHandler::~CDropServerHandler()
{
}

RDSObject::RDSObject()
{
	RDSObject( 0.0, FALSE, FALSE, TRUE );

}

RDSObject::RDSObject( double dProbability )
{
	RDSObject( dProbability, FALSE, FALSE, TRUE );
}

RDSObject::RDSObject( double dProbability, BOOL bUnique, BOOL bAlways, BOOL bEnabled )
{
	this->dProbability = dProbability;
	this->bUnique = bUnique;
	this->bAlways = bAlways;
	this->bEnabled = bEnabled;
	pcTable = NULL;
}

RDSTable::RDSTable()
{
	RDSTable( std::vector<RDSObject*>(), 1, 1.0, FALSE, FALSE, TRUE );
}

RDSTable::RDSTable( std::vector<RDSObject*> & vData, int iCount, double dProbability )
{
	RDSTable( vData, iCount, dProbability, FALSE, FALSE, TRUE );
}

RDSTable::RDSTable( std::vector<RDSObject*> & vData, int iCount, double dProbability, BOOL bUnique, BOOL bAlways, BOOL bEnabled )
{
	vContents.clear();
	for ( auto & obj : vData )
		vContents.push_back( obj );

	this->iCount = iCount;
	this->dProbability = dProbability;
	this->bUnique = bUnique;
	this->bAlways = bAlways;
	this->bEnabled = bEnabled;
}

RDSTable::~RDSTable()
{
}

void RDSTable::Add( RDSObject * obj )
{
	vContents.push_back( obj );

	if ( obj->GetTable() )
		obj->GetTable()->Remove( obj );
}

void RDSTable::Add( RDSObject * obj, double dProbability )
{
	Add( obj );
	obj->SetProbality( dProbability );
}

void RDSTable::Remove( RDSObject * obj )
{
	for ( std::vector<RDSObject*>::iterator it = vContents.begin(); it != vContents.end(); )
	{
		RDSObject * pc = (*it);
		if ( pc == obj )
			it = vContents.erase( it );
		else
			it++;
	}
}

void RDSTable::Remove( int iIndex )
{
	if ( vContents.size() > 0 )
	{
		if ( (iIndex >= 0) && ((UINT)iIndex < vContents.size()) )
			Remove( vContents[iIndex] );
	}
}
