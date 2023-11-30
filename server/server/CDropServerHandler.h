#pragma once

class RDSTable;

class RDSObject
{
public:
	RDSObject();
	RDSObject( double dProbability );
	RDSObject( double dProbability, BOOL bUnique, BOOL bAlways, BOOL bEnabled );
	~RDSObject() {};

	double						GetProbality() { return dProbability; }
	void						SetProbality( double d ) { dProbability = d; }

	RDSTable					* GetTable() { return pcTable; }
	void						SetTable( RDSTable * pc ) { pcTable = pc; }

	BOOL						IsEnabled() { return bEnabled; }
	void						SetEnabled( BOOL b ) { bEnabled = b; }

	BOOL						IsUnique() { return bUnique; }
	void						SetUnique( BOOL b ) { bUnique = b; }

	BOOL						IsAlways() { return bAlways; }
	void						SetAlways( BOOL b ) { bAlways = b; }

private:
	BOOL						bEnabled;

	double						dProbability;

	BOOL						bUnique;
	BOOL						bAlways;

public:
	RDSTable					* pcTable;
};

class RDSTable
{
public:
	RDSTable();
	RDSTable( std::vector<RDSObject *> & vData, int iCount, double dProbability );
	RDSTable( std::vector<RDSObject *> & vData, int iCount, double dProbability, BOOL bUnique, BOOL bAlways, BOOL bEnabled );
	~RDSTable();


	double						GetProbality() { return dProbability; }
	void						SetProbality( double d ) { dProbability = d; }

	int							GetCount() { return iCount; }
	void						SetCount( int i ) { iCount = i; }

	BOOL						IsEnabled() { return bEnabled; }
	void						SetEnabled( BOOL b ) { bEnabled = b; }

	BOOL						IsUnique() { return bUnique; }
	void						SetUnique( BOOL b ) { bUnique = b; }

	BOOL						IsAlways() { return bAlways; }
	void						SetAlways( BOOL b ) { bAlways = b; }

	void						Add( RDSObject * obj );
	void						Add( RDSObject * obj, double dProbability );

	void						Remove( RDSObject * obj );
	void						Remove( int iIndex );

private:
	BOOL						bEnabled;

	int							iCount;
	std::vector<RDSObject*>		vContents;

	double						dProbability;

	BOOL						bUnique;
	BOOL						bAlways;
};

class CDropServerHandler
{
public:
	CDropServerHandler();
	virtual ~CDropServerHandler();
};

