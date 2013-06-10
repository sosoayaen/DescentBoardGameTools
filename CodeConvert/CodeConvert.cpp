// CodeConvert.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "CodeConvert.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

// 大五码转GBK码
void __fastcall BIG52GBK(char *szBuf, int nSize)
{
	if (szBuf == NULL || !strcmp(szBuf, "")) 
	{
		return;
	}

	if (nSize == 0)
	{
		nSize = strlen(szBuf);
	}

	wchar_t* pws = new wchar_t[nSize + 1];

	__try
	{
		int nRet = MultiByteToWideChar(950, 0, szBuf, nSize, pws, nSize + 1);
		BOOL bValue = false;
		nRet = WideCharToMultiByte(936, 0, pws, nRet, szBuf, nSize + 1, "?", &bValue);
		szBuf[nRet] = 0;
	}
	__finally
	{
		delete[] pws;
	}
}

// GBK转大五码
void __fastcall GBK2BIG5(char *szBuf, int nSize)
{
	if (szBuf == NULL || !strcmp(szBuf, "")) 
	{
		return;
	}
	
	if (nSize == 0)
	{
		nSize = strlen(szBuf);
	}

	wchar_t* pws = new wchar_t[nSize + 1];
	__try
	{
		MultiByteToWideChar(936, 0, szBuf, nSize, pws, nSize + 1);
		BOOL bValue = false;
		WideCharToMultiByte(950, 0, pws, nSize, szBuf, nSize + 1, "?", &bValue);
	}
	__finally
	{
		delete[] pws;
	}
}

// GB2312转GBK
void __fastcall GB2GBK(char* szBuf, int nSize)
{
	if (szBuf == NULL || !strcmp(szBuf, "")) 
	{
		return;
	}
	
	if (nSize == 0)
	{
		nSize = strlen(szBuf);
	}

	WORD wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	int nRet = LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nSize, NULL, 0);
	if (!nRet)
	{
		return;
	}

	char* pcBuf = new char[nRet + 1];

	__try
	{
		wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
		LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nRet, pcBuf, nRet + 1);
		strncpy(szBuf, pcBuf, nRet);
	}
	__finally
	{
		delete[] pcBuf;
	}
}

// GBK转GB2312
void __fastcall GBK2GB(char* szBuf, int nSize)
{
	if (szBuf == NULL || !strcmp(szBuf, "")) 
	{
		return;
	}
	
	if (nSize == 0)
	{
		nSize = strlen(szBuf);
	}
	
	WORD wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
	int nRet = LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nSize, NULL, 0);
	if (!nRet)
	{
		return;
	}
	
	char* pcBuf = new char[nRet + 1];
	
	__try
	{
		wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
		LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nRet, pcBuf, nRet + 1);
		strncpy(szBuf, pcBuf, nRet);
	}
	__finally
	{
		delete[] pcBuf;
	}
}

// GB2312转BIG5
void __fastcall GB2BIG5(char* szBuf, int nSize)
{
	GB2GBK(szBuf, nSize);
	GBK2BIG5(szBuf, nSize);
}

// BIG5转GB2312
void __fastcall BIG52GB(char* szBuf, int nSize)
{
	GBK2BIG5(szBuf, nSize);
	GB2GBK(szBuf, nSize);
}


std::string GBK2UTF8(const std::string& strGBK) 
{
	std::string strOutUTF8 = ""; 
	WCHAR * str1;
	int nLen = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, nLen); nLen = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[nLen];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, nLen, NULL, NULL);
	strOutUTF8 = str2;

	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;

	return strOutUTF8;
}

std::string UTF82GBK(const std::string& strUTF8) 
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1]; memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, wszGBK, len);
	
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL); //strUTF8 = szGBK;
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;

	return strTemp;
}

//////////////////////////////////////////////////////////////////////////
// Lua interface
#define PAS(L, idx) luaL_checkstring(L, idx)

int luacf_utf82gbk(lua_State* L)
{
	if (lua_isnoneornil(L, 1))
	{
		return 0;
	}
	std::string strUTF8 = PAS(L, 1);
	std::string strGBK = UTF82GBK(strUTF8);

	lua_pushstring(L, strGBK.c_str());
	return 1;
}

int luacf_gbk2utf8(lua_State* L)
{
	if (lua_isnoneornil(L, 1))
	{
		return 0;
	}

	std::string strGBK = PAS(L, 1);
	std::string strUTF8 = GBK2UTF8(strGBK);

	lua_pushstring(L, strUTF8.c_str());
	return 1;
}

int luacf_gb2gbk(lua_State* L)
{
	if (lua_isnoneornil(L, 1))
	{
		return 0;
	}

	int nLen = lua_strlen(L, 1);
	char* pszBuf = new char[lua_strlen(L, 1) + 1];
	strcpy(pszBuf, PAS(L, 1));

	GB2GBK(pszBuf, nLen);

	lua_pushstring(L, pszBuf);

	return 1;
}

int luacf_gbk2gb(lua_State* L)
{
	if (lua_isnoneornil(L, 1))
	{
		return 0;
	}
	
	int nLen = lua_strlen(L, 1);
	char* pszBuf = new char[lua_strlen(L, 1) + 1];
	strcpy(pszBuf, PAS(L, 1));
	
	GBK2GB(pszBuf, nLen);
	
	lua_pushstring(L, pszBuf);
	return 1;
}

int luacf_gbk2big5(lua_State* L)
{
	if (lua_isnoneornil(L, 1))
	{
		return 0;
	}
	
	int nLen = lua_strlen(L, 1);
	char* pszBuf = new char[lua_strlen(L, 1) + 1];
	strcpy(pszBuf, PAS(L, 1));
	
	GBK2BIG5(pszBuf, nLen);
	
	lua_pushstring(L, pszBuf);
	return 1;
}

int luacf_big52gbk(lua_State* L)
{
	if (lua_isnoneornil(L, 1))
	{
		return 0;
	}
	
	int nLen = lua_strlen(L, 1);
	char* pszBuf = new char[lua_strlen(L, 1) + 1];
	strcpy(pszBuf, PAS(L, 1));
	
	BIG52GBK(pszBuf, nLen);
	
	lua_pushstring(L, pszBuf);
	return 1;
}

int luacf_gb2big5(lua_State* L)
{
	if (lua_isnoneornil(L, 1))
	{
		return 0;
	}
	
	int nLen = lua_strlen(L, 1);
	char* pszBuf = new char[lua_strlen(L, 1) + 1];
	strcpy(pszBuf, PAS(L, 1));
	
	GB2BIG5(pszBuf, nLen);
	
	lua_pushstring(L, pszBuf);
	return 1;
}

int luacf_big52gb(lua_State* L)
{
	if (lua_isnoneornil(L, 1))
	{
		return 0;
	}
	
	int nLen = lua_strlen(L, 1);
	char* pszBuf = new char[lua_strlen(L, 1) + 1];
	strcpy(pszBuf, PAS(L, 1));
	
	BIG52GB(pszBuf, nLen);
	
	lua_pushstring(L, pszBuf);
	return 1;
}

const struct luaL_reg lib[] =
{
	{"UTF82GBK", luacf_utf82gbk},
	{"GBK2UTF8", luacf_gbk2utf8},
	{"GB2GBK", luacf_gb2gbk},
	{"GBK2GB", luacf_gbk2gb},
	{"GBK2BIG5", luacf_gbk2big5},
	{"BIG52GBK", luacf_big52gbk},
	{"GB2BIG5", luacf_gb2big5},
	{"BIG52GB", luacf_big52gb},
	{NULL, NULL}
};

CODECONVERT_API int luaopen_codeconvert( lua_State* L )
{
	luaL_register(L, "CodeConvert", lib);
	return 1;
}