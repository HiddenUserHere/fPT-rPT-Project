#pragma once

enum EWorldID
{
	WORLDID_None		= 0,
	WORLDID_Fortress	= 1,
	WORLDID_Test		= 2,
	WORLDID_TestLocal	= 3,
};

class CWorld
{
public:
					  CWorld( EWorldID _iWorldID, std::string _strName, unsigned long _lIP, USHORT _uPort, std::string _strGameName, std::string _strShortGameName, std::string _strWebsite, unsigned long _lIP2, unsigned long _lIP3 );
	virtual			 ~CWorld();

	//Getters
	EWorldID		  GetWorldID() { return iWorldID; }

	std::string			  GetName() { return strName; }
	unsigned long	  GetIPLong() { return lIP; }
	USHORT			  GetPort() { return uPort; }

	std::string			  GetGameName() { return strGameName; }
	std::string			  GetShortGameName() { return strShortGameName; }
	std::string			  GetWebsite() { return strWebsite; }

	unsigned long	  GetIP2Long() { return lIP2; }
	unsigned long	  GetIP3Long() { return lIP3; }

	BOOL			  HasIP() { return lIP != 0x00000000; }
	BOOL			  HasIP2() { return lIP2 != 0x00000000; }
	BOOL			  HasIP3() { return lIP3 != 0x00000000; }

	std::string			  GetIP() { return strIP; }
	std::string			  GetIP2() { return strIP2; }
	std::string			  GetIP3() { return strIP3; }

	//Setters
	void			  SetIP( unsigned long _lIP );

private:
	EWorldID		  iWorldID;

	std::string			  strName;
	unsigned long	  lIP;
	USHORT			  uPort;

	std::string			  strGameName;
	std::string			  strShortGameName;
	std::string			  strWebsite;

	//Reserve IPs
	unsigned long	  lIP2;
	unsigned long	  lIP3;

	//IPs as Strings
	std::string			  strIP;
	std::string			  strIP2;
	std::string			  strIP3;

};