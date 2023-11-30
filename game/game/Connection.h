#ifndef __CONNECTION_H_
#define __CONNECTION_H_
#include "stdafx.h"

namespace Conn
{
	typedef std::shared_ptr<class RecvPacket> Packet_ptr;
	typedef class RecvPacket Packet;
}; 


#endif