#include "stdafx.h"
#include "account.h"

void PacketAccountLogin::Encrypt( int key )
{
	DWORD xor			 = ((this->dwSpecial1) + (key << 8) + (key << 16) + (key << 24)) ^ 0x49A3BACF;
	this->dwSpecial1		^= 0x529ADFA3;

	for( int i = (sizeof( Packet ) + sizeof( DWORD )); i < sizeof( PacketAccountLogin ); i += sizeof( DWORD ) )
	{
		int j = i / sizeof( DWORD );

		DWORD * pp = (DWORD*)this;

		DWORD & dw = pp[j];

		dw ^= xor;
	}
}

void PacketAccountLogin::Decrypt( int key )
{
	this->dwSpecial1		^= 0x529ADFA3;
	DWORD xor			 = ((this->dwSpecial1) + (key << 8) + (key << 16) + (key << 24)) ^ 0x49A3BACF;

	for( int i = (sizeof( Packet ) + sizeof( DWORD )); i < sizeof( PacketAccountLogin ); i += sizeof( DWORD ) )
	{
		int j = i / sizeof( DWORD );

		DWORD * pp = (DWORD*)this;

		DWORD & dw = pp[j];

		dw ^= xor;
	}
}