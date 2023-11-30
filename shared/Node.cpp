#include "stdafx.h"
#include "Node.h"

namespace Base
{

Node::Node() : iID(-1)
{
}

Node::Node( int _iID ) : iID(_iID)
{
}

Node::Node( const Node & c ) : iID(c.iID)
{
}

Node::~Node()
{
}

};