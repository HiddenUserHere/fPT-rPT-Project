/* MD5
converted to C++ class by Frank Thilo (thilo@unix-ag.org)
for bzflag (http://www.bzflag.org)

based on:

md5.h and md5.c
reference implementation of RFC 1321

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

#ifndef BZF_MD5_H
#define BZF_MD5_H

#include <cstring>
#include <iostream>


// a small class for calculating MD5 hashes of strings or byte arrays
// it is not meant to be fast or secure
//
// usage: 1) feed it blocks of uchars with update()
//      2) finalize()
//      3) get hexdigest() string
//      or
//      MD5(std::string).hexdigest()
//
// assumes that char is 8 bit and int is 32 bit
class MD5
{
public:
	typedef unsigned int size_type; // must be 32bit

	MD5();
	MD5( const std::string& text );
	void update( const unsigned char *buf, size_type length );
	void update( const char *buf, size_type length );
	MD5& finalize();
	std::string hexdigest() const;
	friend std::ostream& operator<<( std::ostream&, MD5 md5 );

private:
	void init();
	enum { blocksize = 64 }; // VC6 won't eat a const static int here

	void transform( const BYTE block[blocksize] );
	static void decode( UINT output[], const BYTE input[], size_type len );
	static void encode( BYTE output[], const UINT input[], size_type len );

	bool finalized;
	BYTE buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
	UINT count[2];   // 64bit counter for number of bits (lo, hi)
	UINT state[4];   // digest so far
	BYTE digest[16]; // the result

					  // low level logic operations
	static inline UINT FMD5( UINT x, UINT y, UINT z );
	static inline UINT GMD5( UINT x, UINT y, UINT z );
	static inline UINT HMD5( UINT x, UINT y, UINT z );
	static inline UINT IMD5( UINT x, UINT y, UINT z );
	static inline UINT rotate_left( UINT x, int n );
	static inline void FF( UINT &a, UINT b, UINT c, UINT d, UINT x, UINT s, UINT ac );
	static inline void GG( UINT &a, UINT b, UINT c, UINT d, UINT x, UINT s, UINT ac );
	static inline void HH( UINT &a, UINT b, UINT c, UINT d, UINT x, UINT s, UINT ac );
	static inline void II( UINT &a, UINT b, UINT c, UINT d, UINT x, UINT s, UINT ac );
};

std::string md5( const std::string str );

#endif