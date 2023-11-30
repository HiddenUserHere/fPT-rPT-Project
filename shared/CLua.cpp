#include "stdafx.h"
#include "CLua.h"

struct LuaCompiled
{
	size_t		* uLength;
	char	   ** pszData;
};

void Encrypt( char * pszData, int iSize, int iKeysSize )
{
	for( int i = 0; i < iSize; ++i )
		pszData[i] = pszData[i] ^ iKeysDecrypt[i%iKeysSize];
}

void Decrypt( char * pszData, int iSize, int iKeysSize )
{
	Encrypt( pszData, iSize, iKeysSize );
}

int LuaWriter( lua_State* ls, const void* p, size_t sz, void* ud )
{
	LuaCompiled * bd = (LuaCompiled*)ud;
	char* newData = (char*)realloc( *( bd->pszData ), ( *( bd->uLength ) ) + sz );

	if( newData )
	{
		memcpy( newData + ( *( bd->uLength ) ), p, sz );
		*( bd->pszData ) = newData;
		*( bd->uLength ) += sz;
	}
	else
	{
		free( newData );
		return 1;
	}

	return 0;
}

CLua::CLua()
{
	pLuaState = new LuaState();

	pLuaState->state_ptr = luaL_newstate();

	lua_checkstack( pLuaState->state_ptr, 100 );

	luaL_openlibs( pLuaState->state_ptr );

	CopyMemory( pLuaState->self_ptr, pLuaState->state_ptr, sizeof( lua_State ) );

	pLuaState->lua = this;

	iFunctions = 0;
}

CLua::~CLua()
{
	pLuaState->lua = NULL;

	lua_close( pLuaState->state_ptr );

	DELET( pLuaState );
}

void CLua::Register( string strName, void * pFunction )
{
	lua_register( pLuaState->get_ptr(), strName.c_str(), (lua_CFunction)pFunction );

	iFunctions++;
}

void CLua::DoFile( string strFilePath, bool bIgnoreEncrypt )
{
	if( bIgnoreEncrypt )
	{
		luaL_loadfile( pLuaState->get_ptr(), strFilePath.c_str() );
		lua_pcall( pLuaState->get_ptr(), 0, LUA_MULTRET, 0 );
	}

	//Load Compiled LUA File
	if( strFilePath[strFilePath.size()-1] == 'c' )
	{
		FILE * fp = NULL;
		char * pBuffer;
		long lSize;

		fopen_s( &fp, strFilePath.c_str(), "rb" );

		if( fp )
		{
			fseek( fp, 0, SEEK_END );
			lSize = ftell( fp );
			rewind( fp );

			pBuffer = (char*)malloc( sizeof( char )*lSize );
			fread( pBuffer, 1, lSize, fp );

			Decrypt( pBuffer, lSize, iNumKeys1 + iNumKeys2 + iNumKeys3 );

			luaL_loadbuffer( pLuaState->get_ptr(), pBuffer, lSize, "lua" );
			lua_pcall( pLuaState->get_ptr(), 0, LUA_MULTRET, 0 );
			
			fclose( fp );
			free( pBuffer );
		}
	}
/* DISABLED, ONLY FOR ENCRYPT
#ifdef _DEBUG
	else
	{
		char * pszByteCode = 0;
		size_t uByteLength = 0;

		LuaCompiled sLuaCompiled = { &uByteLength, &pszByteCode };

		//Compile LUA and after encrypt it
		int iRet = luaL_loadfile( pLuaState->get_ptr(), strFilePath.c_str() );
		lua_dump( pLuaState->get_ptr(), LuaWriter, &sLuaCompiled, 1 );
		lua_pop( pLuaState->get_ptr(), 1 );

		Encrypt( pszByteCode, uByteLength, NUM_KEYS );

		//Save Compiled File
		strFilePath.append( "c" );

		FILE * fp = NULL;
		fopen_s( &fp, strFilePath.c_str(), "wb" );

		if( fp )
		{
			fwrite( pszByteCode, uByteLength, 1, fp );
			fclose( fp );
		}
	}
#endif

*/
}

void * CLua::ReadParamPointer( LuaState * pLuaState, int iParam )
{
	void * pPointer = (void*)ReadParamInteger( pLuaState, iParam );

	return pPointer;
}

string CLua::ReadParamString( LuaState * pLuaState, int iParam )
{
	return luaL_checkstring( pLuaState->get_ptr(), iParam );
}

int CLua::ReadParamInteger( LuaState * pLuaState, int iParam )
{
	return (int)luaL_checkinteger( pLuaState->get_ptr(), iParam );
}

double CLua::ReadParamDouble( LuaState * pLuaState, int iParam )
{
	return luaL_checknumber( pLuaState->get_ptr(), iParam );
}