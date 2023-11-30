#pragma once

namespace INI
{
class CReader
{
private:

	std::string							sFileName = "";

public:
	//Constructor
										CReader( char * pszFileName );
	virtual								~CReader( );

	//Reader Functions
	std::string							ReadString( const char * pszSection, const char * pszKey );
	int									ReadInt( const char * pszSection, const char * pszKey, int iDefault = 0 );
	BOOL								ReadOnOff( const char * pszSection, const char * pszKey );
	BOOL								ReadBool( const char * pszSection, const char * pszKey, BOOL bDefault = FALSE );

	//Writer Functions
	void								WriteString( const char * pszSection, const char * pszKey, std::string strText );
	void								WriteInt( const char * pszSection, const char * pszKey, int iValue );
	void								WriteOnOff( const char * pszSection, const char * pszKey, BOOL bValue );
	void								WriteBool( const char * pszSection, const char * pszKey, BOOL bValue );
};
}