// FVersion.cpp: implementation of the CFVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFVersionInfo::CFVersionInfo()
{

}

CFVersionInfo::~CFVersionInfo()
{

}

BOOL CFVersionInfo::Init()
{
	// Get File Name
	TCHAR szFileName[_MAX_PATH] = _T("");
	::GetModuleFileName(GetModuleHandle(NULL), szFileName, sizeof(szFileName));
	
	return GetFileVersionFromFile(szFileName);
}

BOOL CFVersionInfo::GetFileVersionFromFile( LPCTSTR lpszFileName )
{
	BOOL bRet = FALSE;
	if (NULL == lpszFileName)
	{
		return FALSE;
	}

	DWORD dwSize = 0;
	dwSize = GetFileVersionInfoSize((LPSTR)lpszFileName, &dwSize);

	char* pBuffer = new char[dwSize];

	do 
	{
		if (!::GetFileVersionInfo((LPSTR)lpszFileName, NULL, dwSize, (LPVOID)pBuffer))
		{
			break;
		}
		UINT querySize; 
		DWORD* pTransTable; 
		DWORD dwLanCharset; 

		if (!::VerQueryValue(pBuffer, _T("\\VarFileInfo\\Translation"), (void**)&pTransTable, &querySize))
		{
			break;
		}

		dwLanCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
		LPVOID pData = NULL;
		char szValue[_MAX_PATH] = {0};

#define QueryValue(name, data) \
		sprintf(szValue, _T("\\StringFileInfo\\%08lx\\%s"), dwLanCharset, #name ); \
		if (::VerQueryValue(pBuffer, szValue, &pData, &querySize)) \
		data = (char*)pData;

		QueryValue(LegalCopyright, m_strLegalCopyright);
		QueryValue(InteralName, m_strInteralName);
		QueryValue(FileDescription, m_strFileDescription);
		QueryValue(Comments, m_strComments);
		QueryValue(FileVersion, m_strFileVersion);
		QueryValue(LegalTrademarks, m_strLegalTrademarks);
		QueryValue(ProductVersion, m_strProductVersion);
		QueryValue(SpecialBuild, m_strSpecialBuild);
		QueryValue(CompanyName, m_strCompanyName);
		QueryValue(PrivateBuild, m_strPrivateBuild);
		QueryValue(OriginalFilename, m_strOriginalFilename);

#undef QueryValue
		bRet = TRUE;
	} while (0);

	delete [] pBuffer;
	return bRet;
}
