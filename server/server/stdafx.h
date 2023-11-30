#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS


//Libraries
//Windows API
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Imagehlp.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"shlwapi.lib")


//Network
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "Mswsock.lib")

//Includes
//Standard:
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "targetver.h"

//STD:
#include <vector>
#include <list>
#include <map>

//Windows Header Files:
#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <intrin.h>
#include <winternl.h>

#include <Wininet.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Mswsock.h>

#include <string>
#include <memory>
#include <time.h>
#include <Strsafe.h>

#include <sstream>
#include <fstream>

#include <Mmsystem.h>
#include <cctype>
#include <functional>
#include <queue>
#include <Imagehlp.h>
#include <basetsd.h>
#include <ShellAPI.h>

#include <random>
#include <filesystem>
#include <chrono>
#include <mutex>


//Custom Include:
#include "globals.h"

void * operator new[]( std::size_t s ) throw(std::bad_alloc);
void operator delete[]( void *p ) throw();
void * operator new(std::size_t s) throw(std::bad_alloc);
void operator delete(void *p) throw();