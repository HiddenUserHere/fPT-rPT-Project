#pragma once

enum ETickType
{
	TICKTYPE_None,
	TICKTYPE_100MilliSecond,
	TICKTYPE_500MilliSecond,
	TICKTYPE_1Second,
	TICKTYPE_1Minute,
	TICKTYPE_1Hour,
};

#define KB1				(1ull * 1024ull)
#define KB2				(2ull * 1024ull)
#define KB4				(4ull * 1024ull)
#define KB8				(8ull * 1024ull)
#define KB16			(16ull * 1024ull)
#define KB32			(32ull * 1024ull)
#define KB64			(64ull * 1024ull)
#define KB128			(128ull * 1024ull)
#define KB192			(192ull * 1024ull)
#define KB256			(256ull * 1024ull)
#define KB512			(512ull * 1024ull)
#define MB1				(1ull * KB1 * KB1)
#define MB2				(2ull * KB1 * KB1)
#define MB4				(4ull * KB1 * KB1)
#define MB8				(8ull * KB1 * KB1)
#define GB1				(1ull * KB1 * MB1)
#define GB2				(2ull * KB1 * MB1)
#define GB4				(4ull * KB1 * MB1)
#define GB8				(8ull * KB1 * MB1)

#define ID				DWORD
#define ITEMCHECKSUM	DWORD

#define IMAGE_INACTIVE	0
#define IMAGE_ACTIVE	1
#define IMAGE_GRAY		2

#define RGBA(a,r,g,b)	((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)|(((DWORD)(BYTE)(a))<<24)))
#define ACOLOR			unsigned long
#define COLOR			unsigned char
#define COLORI			unsigned int
#define ALPHA_LOW		0x00
#define ALPHA_HIGH		0xFF

#define FILEEXIST		FileExist

#define GetAValue(rgb)	(LOBYTE((rgb)>>24))

short   RandomS( short l, short h );
int		RandomI( int l, int h );
UINT	RandomU( UINT l, UINT h );
float	RandomF( float l, float h );
double	RandomD( double l, double h );

BOOL	IsNull( const char * pszString );
BOOL	IsNull( int iValue );

void	ChangeFileExtension( const char * pszFile, const char * pszExt, char * pszBuffer );
void	GetDateTime( char * pszDateTime );

std::string GetDayOfWeek( int iDay );

BOOL	FileExist( const std::string& name );

void		SetClipboardText( char * szText );
std::string	GetClipboardText();

int		GetDaysCountMonth( int iMonth, int iYear );
UINT64	SystemTimeToQWORD( SYSTEMTIME * ps );
bool	GetFileLastModifiedSystemTime( const std::string strFile, SYSTEMTIME & sTime );
DWORD	GetFileSize( const char * pszFile );

class CEncryptXOR
{
public:
	CEncryptXOR() {};
	virtual ~CEncryptXOR() {};

	virtual bool			EncDecrypt( BYTE * pbaData, int iLength, const char * pszHash = nullptr );
};

struct CurMax
{
	short	  sCur;
	short	  sMax;

	CurMax() {}
	CurMax( short sCur, short sMax ) : sCur(sCur), sMax(sMax) {}
	
	bool operator ==(const CurMax &o) { return (sCur == o.sCur) && (sMax == o.sMax); }
	bool operator !=(const CurMax &o) { return !(*this == o); }
};

struct UCurMax
{
	USHORT	  sCur;
	USHORT	  sMax;

	UCurMax() {}
	UCurMax( int sCur, int sMax ) : sCur(sCur), sMax(sMax) {}
};

struct MinMax
{
	short	  sMin;
	short	  sMax;

	MinMax() {}
	MinMax( int sMin, int sMax ) : sMin(sMin), sMax(sMax) {}

	short RandomValue() const { return RandomS( sMin, sMax ); }
	
	bool operator ==(const MinMax &o) { return (sMin == o.sMin) && (sMax == o.sMax); }
	bool operator !=(const MinMax &o) { return !(*this == o); }
};

struct IMinMax
{
	int		  iMin;
	int		  iMax;

	IMinMax() {}
	IMinMax( int iMin, int iMax ) : iMin(iMin), iMax(iMax) {}

	int RandomValue() const { return RandomI( iMin, iMax ); }
};

struct FMinMax
{
	float	  fMin;
	float	  fMax;

	FMinMax() {}
	FMinMax( float fMin, float fMax ) : fMin(fMin), fMax(fMax) {}

	float RandomValue() const { return RandomF( fMin, fMax ); }
};

std::wstring StringToWideString( const std::string & str );
std::string WideStringToString( const std::wstring & wstr );