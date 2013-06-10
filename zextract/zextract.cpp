// zextract.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "zextract.h"

#include "unzip.h"
#include "zip.h"

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

ZEXTRACT_API bool FindFileInZip(const char* zfn, const char* fname)
{
	bool ret = true;
	unzFile uf = unzOpen(zfn);

	if (NULL == uf)
	{
		printf("unzOpen failed...\n");
		return false;
	}

	int err = unzLocateFile (uf, fname, 0);
	if (UNZ_OK != err)
	{
		printf("unzLocateFile failed... error:%d", err);
		ret = false;
	}

	unzClose(uf);
	return ret;
}

ZEXTRACT_API int GetFileInZip(CMemBuffer& buffer, const char* zfn, const char* fname, const char* password)
{
	unzFile uf = unzOpen(zfn);
	
	if (NULL == uf)
	{
		printf("unzOpen failed...\n");
		return -1;
	}
	
	int err = unzLocateFile(uf, fname, 0);
	if (UNZ_OK != err)
	{
		printf("GetFileInZip unzLocateFile failed... error:%d\n");
		return err;
	}
	
	unz_file_info file_info;
	char filename_inzip[256];
	
	err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

	if (UNZ_OK != err)
	{
		printf("unzGetCurrentFileInfo failed... error:%d\n", err);
		return err;
	}

	err = unzOpenCurrentFilePassword(uf, password);

	if (UNZ_OK != err)
	{
		printf("unzOpenCurrentFilePassword failed... error:%d\n", err);
		return err;
	}

	char* pBuff = new char[file_info.uncompressed_size];

	if (pBuff == NULL)
	{
		unzCloseCurrentFile(uf);
		unzClose(uf);
		return -2;
	}

	err = unzReadCurrentFile(uf, pBuff, file_info.uncompressed_size);

	if (err < 0)
	{
		printf("unzReadCurrentFile failed... error:%d\n", err);
		delete [] pBuff;
		unzCloseCurrentFile(uf);
		unzClose(uf);
		return err;
	}
	// Append data to the MemBuffer
	buffer.Append(pBuff, file_info.uncompressed_size);

	unzCloseCurrentFile(uf);

	unzClose(uf);
	return err;
}

ZEXTRACT_API void ListZip(const char* fname)
{
	unzFile uf = unzOpen(fname);
	
	if (NULL == uf)
	{
		printf("unzOpen failed...\n");
		return;
	}
	
	unz_global_info gi;
	int err = unzGetGlobalInfo (uf, &gi);
	// printf("Hello World!\n");
	
	if (UNZ_OK != err)
	{
		printf("unzGetGlobalINfo failed...\n");
		unzClose(uf);
		return;
	}
	
	printf(" Length  Method   Size  Ratio   Date    Time   CRC-32     Name\n");
    printf(" ------  ------   ----  -----   ----    ----   ------     ----\n");
	err = UNZ_OK;
	// for (uLong i = 0; i < gi.number_entry; i++)
	while(UNZ_OK == err)
	{
		char filename_inzip[256] = {0};
		unz_file_info file_info;
		uLong ratio = 0;
		char charCrypt = ' ';
		const char* string_method = NULL;
		
		err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
		
		if (UNZ_OK != err)
		{
			printf("unzGetCurrentFileInfo failed...\n");
			unzClose(uf);
			return;
		}
		//////////////////////////////////////////////////////////////////////////
		// List file
		if (file_info.uncompressed_size > 0)
		{
			ratio = (file_info.compressed_size*100)/file_info.uncompressed_size;
		}
		
        /* display a '*' if the file is crypted */
        if ((file_info.flag & 1) != 0)
		{
			charCrypt = '*';
		}
		
        if (file_info.compression_method == 0)
		{
			string_method = "Stored";
		}
        else if (file_info.compression_method == Z_DEFLATED)
		{
			uInt iLevel = (uInt)((file_info.flag & 0x6)/2);
			if (iLevel == 0)
				string_method = "Defl:N";
			else if (iLevel == 1)
				string_method = "Defl:X";
			else if ((iLevel == 2) || (iLevel == 3))
				string_method = "Defl:F"; /* 2:fast , 3 : extra fast*/
		}
		else
		{
			string_method = "Unkn. ";
		}
		
		printf("%7lu  %6s%c%7lu %3lu%%  %2.2lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu  %8.8lX   %s\n",
            file_info.uncompressed_size, string_method,
            charCrypt,
            file_info.compressed_size,
            ratio,
            (uLong)file_info.tmu_date.tm_mon + 1,
            (uLong)file_info.tmu_date.tm_mday,
            (uLong)file_info.tmu_date.tm_year % 100,
            (uLong)file_info.tmu_date.tm_hour,(uLong)file_info.tmu_date.tm_min,
            (uLong)file_info.crc,filename_inzip);
		//////////////////////////////////////////////////////////////////////////
		
		err = unzGoToNextFile(uf);
	}
	
	unzClose(uf);
}

//////////////////////////////////////////////////////////////////////////
// @Param
//   zipName   which archive
//   fileName  which file to extract from the archive named zipName's value
//   password  password to extract file from zip
// @Return
//   binary data
//   error number

static int luacf_getFileInZip(lua_State* L)
{
	CMemBuffer buff;
	const char* pZipName = luaL_checkstring(L, 1);
	const char* pFileName = luaL_checkstring(L, 2);
	const char* pPassword = lua_tostring(L, 3);
	int ret = GetFileInZip(buff, pZipName, pFileName, pPassword);

	if (ret >= 0)
	{
		lua_pushlstring(L, buff.GetBuffer(), buff.GetBufferLen());
		return 1;
	}

	lua_pushnil(L);
	lua_pushnumber(L, ret);
	return 2;
}

const struct luaL_reg libs[] =
{
	{"getFileInZip", luacf_getFileInZip},
	{NULL, NULL}
};

ZEXTRACT_API int luaopen_zextract(lua_State* L)
{
	luaL_register(L, "zextract", libs);
	return 1;
}