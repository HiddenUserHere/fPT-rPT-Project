#pragma once
class RecvPacket
{
private:

public:
	RecvPacket();
	virtual ~RecvPacket();

	BOOL										AnalyzePacket( SocketData * pcSocketData, Packet * psPacket );
};

