#pragma once
class CAccountHandler
{
public:
	CAccountHandler();
	virtual ~CAccountHandler();

	int								GetCoinAmount() { return iCoinAmount; }
	void							SetCoinAmount( int i ) { iCoinAmount = i; }

	int								GetTimePointsAmount() { return iTimePoints; }
	void							SetTimePointsAmount( int i ) { iTimePoints = i; }

	BOOL							IsAccountShareDenied( EAccountShare _eAccountShare ) { return (eAccountShare & _eAccountShare) == _eAccountShare; }
	void							SetAccountShare( EAccountShare _eAccountShare ) { eAccountShare = _eAccountShare; }

	void							HandlePacket( PacketServerList * psPacket );

protected:
	int								iCoinAmount;
	int								iTimePoints;

	EAccountShare					eAccountShare = ACCOUNTSHARE_None;
};

