#pragma once

namespace Base
{

class Node
{
public:
					  Node();
					  Node( int _iID );
					  Node( const Node & c );
	virtual			 ~Node();

	int				  GetID() { return iID; }
	void			  SetID( int _iID ) { iID = _iID; }

	virtual	void	  Reset() {}

protected:
	int				  iID;

};

};