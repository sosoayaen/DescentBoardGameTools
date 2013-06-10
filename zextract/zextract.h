
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ZEXTRACT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ZEXTRACT_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef ZEXTRACT_EXPORTS
#define ZEXTRACT_API __declspec(dllexport)
#else
#define ZEXTRACT_API __declspec(dllimport)
#endif

#include "MemBuffer.h"

#include "luna.hpp"

ZEXTRACT_API void ListZip(const char* fname);
ZEXTRACT_API bool FindFileInZip(const char* zfn, const char* fname);
ZEXTRACT_API int GetFileInZip(CMemBuffer& buffer, const char* zfn, const char* fname, const char* password);

extern "C" ZEXTRACT_API int luaopen_zextract(lua_State* L);
