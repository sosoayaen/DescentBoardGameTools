
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CODECONVERT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CODECONVERT_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CODECONVERT_EXPORTS
#define CODECONVERT_API __declspec(dllexport)
#else
#define CODECONVERT_API __declspec(dllimport)
#endif

#include "luna.hpp"

extern "C" CODECONVERT_API int luaopen_codeconvert( lua_State* L);
extern "C" CODECONVERT_API void __fastcall BIG52GBK(char *szBuf, int nSize);
extern "C" CODECONVERT_API void __fastcall GBK2BIG5(char *szBuf, int nSize);
extern "C" CODECONVERT_API void __fastcall GB2GBK(char* szBuf, int nSize);
extern "C" CODECONVERT_API void __fastcall GBK2GB(char* szBuf, int nSize);
extern "C" CODECONVERT_API void __fastcall GB2BIG5(char* szBuf, int nSize);
extern "C" CODECONVERT_API void __fastcall BIG52GB(char* szBuf, int nSize);

#include <string>
CODECONVERT_API std::string GBK2UTF8(const std::string& strGBK);
CODECONVERT_API std::string UTF82GBK(const std::string& strUTF8);