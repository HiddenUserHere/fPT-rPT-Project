#pragma once

#define NUM_KEYS	10

static int iKeysDecrypt[NUM_KEYS] = { 36, 20, 12, 8, 69, 3, 99, 13, 45, 17 };

volatile static int iNumKeys1 = 2;
volatile static int iNumKeys2 = 5;
volatile static int iNumKeys3 = 3;

//#define LUAPT_DEBUG

class CLua;

struct lua_State;

struct LuaState
{
	LuaState() { self_ptr = this; }

	lua_State		* get_ptr() { return (lua_State*)self_ptr; }

	lua_State		  state;

	LuaState		* self_ptr;
	lua_State		* state_ptr;
	CLua			* lua;
};


class CLua
{
public:
						  CLua();
	virtual				 ~CLua();

protected:
	void				  Register( std::string strName, void * pFunction );
	void				  DoFile( std::string strFilePath, bool bIgnoreEncrypt = false );

	static void			* ReadParamPointer( LuaState * pLuaState, int iParam );
	static std::string	  ReadParamString( LuaState * pLuaState, int iParam );
	static int			  ReadParamInteger( LuaState * pLuaState, int iParam );
	static double		  ReadParamDouble( LuaState * pLuaState, int iParam );

protected:
	LuaState		* pLuaState;

	int					iFunctions;
};