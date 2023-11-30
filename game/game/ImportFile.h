#pragma once

namespace Import
{

class File
{
public:
						  File( const std::string & strFilePath );

	bool				  Exists() const;

	bool				  IsOpen() const;

	const std::string	  GetFileExtension();

	const std::string	& GetFilePath() const { return strFilePath; }

protected:
	bool				  Open();
	long				  Tell();
	long				  Size();
	void				  Seek( long lPosition );
	void				  Read( void * pBuffer, size_t iElementSize, size_t iCount );
	void				  Close();

protected:
	std::string			  strFilePath;

private:
	FILE				* pFile = NULL;
};

}