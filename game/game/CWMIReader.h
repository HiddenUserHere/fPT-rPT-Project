#pragma once
class CWMIReader
{
public:
	CWMIReader();
	virtual ~CWMIReader();

	std::string						GetValue( std::string strClass, std::string strProperty );

	std::string						GetCPUID();
	std::string						GetBIOSID();
	std::string						GetDiskID();
	std::string						GetBaseID();
	std::string						GetVideoID();

	std::string						GetHardwareID();


private:
	HRESULT							hr;

	IWbemLocator					* pLocator		= NULL;
	IWbemServices					* pServices		= NULL;
	IEnumWbemClassObject			* pEnumerator	= NULL;

	BOOL							bConnected		= FALSE;

	BOOL							Initialize();
	void							Uninitialize();

	BOOL							SetSecurityLevels();

	BOOL							CreateLocator();
	void							ReleaseLocator();

	BOOL							Connect();
	void							Disconnect();

	BOOL							SetProxySecurityLevels();

	BOOL							Prepare( std::string strClass );
	void							Unprepare();

	std::string						Get( std::string strProperty );

	std::string						GetHex( std::string str );

	std::string						GetHash( std::string str );
};

