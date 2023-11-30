#pragma once


struct ClanInfoHeader
{
	char					szClanTexture[40];
	int						iClanID;
	char					szClanName[40];
	char					szClanText[90];
	char					cGserver[4];
	char					szUniqueClanInfo[64];
};

struct PacketGetClanInfo : Packet
{
	PacketGetClanInfo( int iClanID ) { this->iClanID = iClanID; }
	~PacketGetClanInfo() {}

	int					iClanID;
};


struct PacketClanInfoData : Packet
{
	ClanInfoHeader			sHeader;
};