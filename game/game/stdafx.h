#pragma once

#define NOMINMAX

//#define _USE_URHO3D_

//#define _ANTICHEAT_OFF_

//#define _ANIM

//#define D3D_DEBUG_INFO

#define _WIN32_DCOM

#undef __IFontDisp_INTERFACE_DEFINED__

//#define CEF_UI_BROWSER

//Libraries
//Windows API
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Imagehlp.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"rpcrt4.lib")
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Vfw32.lib")

//#define _NOESIS_GUI

#ifdef _NOESIS_GUI
//Noesis
#include <Noesis_pch.h>
#include "Noesis/SpiviNoesisGUIRenderDeviceD3D9.h"

#pragma comment(lib, "Noesis.lib")
#endif

//Network
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib,"wininet.lib")

//Lua
#pragma comment(lib, "lua.lib")

//WebP
#pragma comment(lib, "webp\\lib\\libwebp.lib")

//Discord
#ifdef _DEBUG
#pragma comment(lib,"discord-rpc-md.lib")
#else
#pragma comment(lib,"discord-rpc-mt.lib")
#endif


//Directx
#pragma comment(lib,"d3d9.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")

//Effekseer
#ifdef _DEBUG
#pragma comment(lib, "Effekseer/lib/Debug/Effekseer.lib" )
#pragma comment(lib, "Effekseer/lib/Debug/EffekseerRendererDX9.lib" )
//#pragma comment(lib, "Effekseer/lib/Debug/EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "Effekseer/lib/Release/Effekseer.lib" )
#pragma comment(lib, "Effekseer/lib/Release/EffekseerRendererDX9.lib" )
//#pragma comment(lib, "Effekseer/lib/Release/EffekseerSoundXAudio2.lib" )
#endif

//MyGUI

#ifdef _DEBUG
#pragma comment(lib, "MyGUI/lib/Debug/MyGUIEngineStatic_d.lib" )
#pragma comment(lib, "MyGUI/lib/Debug/MyGUI.DirectXPlatform_d.lib" )
#else
#pragma comment(lib, "MyGUI\\lib\\Release\\MyGUIEngineStatic.lib" )
#pragma comment(lib, "MyGUI\\lib\\Release\\MyGUI.DirectXPlatform.lib" )
#endif


//COM
#pragma comment(lib, "wbemuuid.lib")

//Includes
//Standard:
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "targetver.h"

//STD:
#include <vector>
#include <list>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <mutex>
#include <random>

//Windows Header Files:
#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <Mmsystem.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include <vfw.h>
#include <intrin.h>
#include <winternl.h>
#include <aclapi.h>

#include <Wininet.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Mswsock.h>

#include <memory>
#include <stack>
#include <time.h>
#include <Strsafe.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9types.h>
#include <dsound.h>

#include <sstream>
#include <fstream>

#include <codecvt>

#include <cwctype>
#include <cctype>
#include <cwctype>
#include <functional>
#include <queue>
#include <Imagehlp.h>
#include <basetsd.h>
#include <rpc.h>
#include "wave.h"

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

//COM
#include <comdef.h>
#include <Wbemidl.h>

//Boost
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/crc.hpp>


#ifdef CEF_UI_BROWSER
//CEF
#include <include/cef_base.h>
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#pragma comment(lib, "libcef.lib")
#pragma comment(lib, "libcef_dll_wrapper.lib")
#endif

//User Header Files:
extern "C"
{
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lstate.h"
}

//Effekseer
#include "Effekseer/include/Effekseer.h"
#include "Effekseer/include/EffekseerRendererDX9.h"
//#include "Effekseer/include/EffekseerSoundXAudio2.h"

//MyGUI
#define MYGUI_DONT_USE_OBSOLETE 1
#define MYGUI_STATIC
#include "MyGUI/include/MyGUI.h"
#include "MyGUI/include/MyGUI_DirectXPlatform.h"

//WebP
#include "webp/include/webp/decode.h"

//CEGUI

#ifdef _DEBUG
#pragma comment(lib, "CEGUIBase-0_Static_d.lib" )
#pragma comment(lib, "CEGUIDirect3D9Renderer-0_Static_d.lib" )
#else
#pragma comment(lib, "CEGUIBase-0_Static.lib" )
#pragma comment(lib, "CEGUIDirect3D9Renderer-0_Static.lib" )
#endif


#include "CEGUI/CEGUI.h"
#include "CEGUI/Vector.h"
#include "CEGUI/RendererModules/Direct3D9/Renderer.h"
#include "CEGUI/RendererModules/Direct3D9/Texture.h"

#include "Discord\discord_register.h"
#include "Discord\discord_rpc.h"

#include <dshow.h>
#include "qedit.h"
#pragma comment(lib,"Strmiids.lib")


//Obfuscate
#include "AYObfuscate.h"

//Ultralight
//#define _ULTRALIGHT_WEB

#ifdef _ULTRALIGHT_WEB
#ifdef _DEBUG
#pragma comment(lib, "debug/AppCore.lib")
#pragma comment(lib, "debug/Ultralight.lib")
#pragma comment(lib, "debug/UltralightCore.lib")
#pragma comment(lib, "debug/WebCore.lib")
#else
#pragma comment(lib, "AppCore.lib")
#pragma comment(lib, "Ultralight.lib")
#pragma comment(lib, "UltralightCore.lib")
#pragma comment(lib, "WebCore.lib")
#endif
#include "Ultralight/Ultralight.h"
#include "ultralight/include/AppCore/GPUDriverImpl.h"
#endif

//#define _SCITER_LIB_

#ifdef _SCITER_LIB_
#include "sciter-x.h"
#endif

//Urho3D:
#ifdef _USE_URHO3D_
#include <Urho3D\Urho3DAll.h>
#pragma comment(lib, "SetupAPI.lib")
#endif

//Delta3D
#define _D3D9
#include "ICore.h"
#include "IGraphics.h"
#include "IIO.h"
#include "IMath.h"
#include "IResource.h"
#include "IThirdParty.h"

#ifdef _DEBUG
#pragma comment(lib, "Delta3D_d.lib" )
#else
#pragma comment(lib, "Delta3D.lib" )
#endif

using namespace Delta3D;

//Custom Include:
#include "md5.h"
#include "globals.h"

DWORD GetMajorAllocator();
DWORD GetMajorAllocatorP();


void * operator new[]( std::size_t s ) throw(std::bad_alloc);
void operator delete[]( void *p ) throw();
void * operator new(std::size_t s) throw(std::bad_alloc);
void operator delete(void *p) throw();

using namespace std;
