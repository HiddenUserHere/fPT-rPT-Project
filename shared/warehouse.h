#pragma once

struct WarehouseData
{
	ItemData			saItem[405];
	int					iGold;
	CurMax				sWeight;
	int					iBuyCount;
	int					iFakeMoney;
};

struct	PacketWarehouse : Packet
{
	union
	{
		DWORD								dwCheckSum;
		int									iCurrentPage;
	};
	union
	{
		struct
		{
			WORD							wPage;
			WORD							wVersion;
		};

		int									iMaxPage;
	};

	union
	{
		int									iGold;
		UINT								uBufferCount;
	};

	int										UserMoney;
	DWORD									dwTemp[5];

	int										iDataLength;
	BYTE									baData[0x1FD0];
};

struct	PacketWarehouseOpen : Packet
{
	int				iUnused;
};

struct	PacketWarehouseData : Packet
{

	DWORD									dwCheckSum;
	WORD									wPage;
	WORD									wVersion;
	int										WareHouseMoney;
	int										UserMoney;
	DWORD									dwTemp[5];

	int										DataSize;
	BYTE									Data[0x4DEB4];
};


class CWarehouseBase
{
public:
	std::string				strFile;

	BOOL					bEncrypted;
	BOOL					bHaveFile;

	WarehouseData			* psWarehouse = NULL;
	BYTE					baData[331748];

	IMinMax					sPacketCount;
	int						iPageCount = 0;

	UINT					uBufferPosition = 0;

	int						iGold = 0;
	int						iMaxPageUser = 0;
};
