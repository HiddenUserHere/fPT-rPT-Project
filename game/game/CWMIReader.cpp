#include "stdafx.h"
#include "CWMIReader.h"


CWMIReader::CWMIReader()
{
}


CWMIReader::~CWMIReader()
{
}

std::string CWMIReader::GetValue( std::string strClass, std::string strProperty )
{
	std::string strResult = "";

	if ( Initialize() )
	{
		if ( SetSecurityLevels() )
		{
			if ( CreateLocator() )
			{
				if ( Connect() )
				{
					if ( SetProxySecurityLevels() )
					{
						if ( Prepare( strClass ) )
						{
							strResult = Get( strProperty );

							Unprepare();
							Disconnect();
							ReleaseLocator();
							Uninitialize();
						}
						else
						{
							Disconnect();
							ReleaseLocator();
							Uninitialize();
						}
					}
					else
					{
						Disconnect();
						ReleaseLocator();
						Uninitialize();
					}
				}
				else
				{
					ReleaseLocator();
					Uninitialize();
				}
			}
			else
				Uninitialize();
		}
		else
			Uninitialize();
	}

	return strResult;
}

std::string CWMIReader::GetCPUID()
{
	std::string retVal = GetValue( "Win32_Processor", "UniqueId" );
	if ( retVal.length() > 0 )
		return retVal;
	
	retVal = GetValue( "Win32_Processor", "ProcessorId" );
	if ( retVal.length() > 0 )
		return retVal;

	retVal = GetValue( "Win32_Processor", "Name" );
	if ( retVal.length() == 0 ) //If no Name, use Manufacturer
		retVal = GetValue( "Win32_Processor", "Manufacturer" );
	
	//Add clock speed for extra security
	retVal += GetValue( "Win32_Processor", "MaxClockSpeed" );

	return retVal;
}

std::string CWMIReader::GetBIOSID()
{
	return GetValue( "Win32_BIOS", "Manufacturer" ) + GetValue( "Win32_BIOS", "SMBIOSBIOSVersion" ) + 
		GetValue( "Win32_BIOS", "IdentificationCode" ) + GetValue( "Win32_BIOS", "SerialNumber" ) + 
		GetValue( "Win32_BIOS", "ReleaseDate" ) + GetValue( "Win32_BIOS", "Version" );
}

std::string CWMIReader::GetDiskID()
{
	return GetValue( "Win32_DiskDrive", "Model" ) + GetValue( "Win32_DiskDrive", "Manufacturer" ) +
		GetValue( "Win32_DiskDrive", "TotalHeads" );
}

std::string CWMIReader::GetBaseID()
{
	return GetValue( "Win32_BaseBoard", "Model" ) + GetValue( "Win32_BaseBoard", "Manufacturer" ) + 
		GetValue( "Win32_BaseBoard", "Name" ) + GetValue( "Win32_BaseBoard", "SerialNumber" );
}

std::string CWMIReader::GetVideoID()
{
	return GetValue( "Win32_VideoController", "DriverVersion" ) + GetValue( "Win32_VideoController", "Name" );
}

std::string CWMIReader::GetHardwareID()
{
	return GetHash( "CPU >> " + GetCPUID() + "\nBIOS >> " + GetBIOSID() + "\nBASE >> " + GetBaseID() + "\nVIDEO >> " + GetVideoID() );
}

BOOL CWMIReader::Initialize()
{
	hr = CoInitializeEx( 0, COINIT_MULTITHREADED );
	if ( FAILED( hr ) )
	{
		WRITEDBG( "Failed to initialize COM library. ErrorCode[0x%08X]", hr );

		return FALSE;
	}

	return TRUE;
}

void CWMIReader::Uninitialize()
{
	CoUninitialize();
}

BOOL CWMIReader::SetSecurityLevels()
{
	hr = CoInitializeSecurity(
		NULL,
		-1,                          //COM authentication
		NULL,                        //Authentication services
		NULL,                        //Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   //Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, //Default Impersonation  
		NULL,                        //Authentication info
		EOAC_NONE,                   //Additional capabilities 
		NULL                         //Reserved
	);

	if ( FAILED( hr ) )
	{
		WRITEDBG( "Failed to initialize security. ErrorCode[0x%08X]", hr );
	}
	return TRUE;
}

BOOL CWMIReader::CreateLocator()
{
	hr = CoCreateInstance( CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLocator );

	if ( FAILED( hr ) )
	{
		WRITEDBG( "Failed to create IWbemLocator object. ErrorCode[0x%08X]", hr );

		return FALSE;
	}

	return TRUE;
}

void CWMIReader::ReleaseLocator()
{
	RELEASE( pLocator );
}

BOOL CWMIReader::Connect()
{
	hr = pLocator->ConnectServer(
		_bstr_t( L"ROOT\\CIMV2" ),	//Object path of WMI namespace
		NULL,						//User name. NULL = current user
		NULL,						//User password. NULL = current
		0,							//Locale. NULL indicates current
		NULL,						//Security flags.
		0,							//Authority (for example, Kerberos)
		0,							//Context object 
		&pServices					//Pointer to IWbemServices proxy
	);

	bConnected = TRUE;

	if ( FAILED( hr ) )
	{
		WRITEDBG( "Could not connect to WMI. ErrorCode[0x%08X]", hr );

		bConnected = FALSE;
	}

	return bConnected;
}

void CWMIReader::Disconnect()
{
	RELEASE( pServices );

	bConnected = FALSE;
}

BOOL CWMIReader::SetProxySecurityLevels()
{
	hr = CoSetProxyBlanket(
		pServices,                   //Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           //RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            //RPC_C_AUTHZ_xxx
		NULL,                        //Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      //RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, //RPC_C_IMP_LEVEL_xxx
		NULL,                        //Client identity
		EOAC_NONE                    //Proxy capabilities 
	);

	if ( FAILED( hr ) )
	{
		WRITEDBG( "Could not set proxy blanket. ErrorCode[0x%08X]", hr );

		return FALSE;
	}

	return TRUE;
}

BOOL CWMIReader::Prepare( std::string strClass )
{
	char szQuery[256] = { 0 };
	STRINGFORMAT( szQuery, "SELECT * FROM %s", strClass.c_str() );

	hr = pServices->ExecQuery(
		bstr_t( "WQL" ),
		bstr_t( szQuery ),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator );

	if ( FAILED( hr ) )
	{
		WRITEDBG( "Query for %s ErrorCode[0x%08X]", strClass.c_str(), hr );

		return FALSE;
	}

	return TRUE;
}

void CWMIReader::Unprepare()
{
	RELEASE( pEnumerator );
}

std::string CWMIReader::Get( std::string strProperty )
{
	std::string strRet = "";

	ULONG uReturn					= 0;
	IWbemClassObject * pClassObject = NULL;

	while ( pEnumerator )
	{
		hr = pEnumerator->Next( WBEM_INFINITE, 1, &pClassObject, &uReturn );

		if ( uReturn == 0 )
			break;

		VARIANT vtProp;
		hr = pClassObject->Get( std::wstring( strProperty.begin(), strProperty.end() ).c_str(), 0, &vtProp, 0, 0 );
		std::wstring ws( STRLENW( vtProp.bstrVal ) > 0 ? vtProp.bstrVal : L"" );
		strRet = std::string( ws.begin(), ws.end() );
		VariantClear( &vtProp );

		pClassObject->Release();

		if ( strRet.length() > 0 )
			break;
	}

	return strRet;
}

std::string CWMIReader::GetHex( std::string str )
{
	std::string s = "";

	for ( UINT i = 0; i < str.length(); i++ )
	{
		BYTE b = str[i];
		int n = b;
		int n1 = n & 15;
		int n2 = (n >> 4) & 15;
		
		if ( n2 > 9 )
			s += (char)(n2 - 10 + 'A');
		else
			s += (char)n2 + '0';
		
		if ( n1 > 9 )
			s += (char)(n1 - 10 + 'A');
		else
			s += (char)n1 + '0';

		if ( (i + 1) != str.length() && (i + 1) % 2 == 0 )
			s += "-";
	}

	return s;
}

std::string CWMIReader::GetHash( std::string str )
{
	return md5( str );
}
