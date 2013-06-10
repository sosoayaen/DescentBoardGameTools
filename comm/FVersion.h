// FVersion.h: interface for the CFVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FVERSION_H__87F6D2FE_C016_448A_9F29_C88E8CDAF218__INCLUDED_)
#define AFX_FVERSION_H__87F6D2FE_C016_448A_9F29_C88E8CDAF218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFVersionInfo
{
public:
	CFVersionInfo();
	virtual ~CFVersionInfo();

public:
	CString m_strFileVersion;
	CString m_strProductVersion;
	CString m_strComments;
	CString m_strFileDescription;
	CString m_strCompanyName;
	CString m_strInteralName;
	CString m_strLegalCopyright;
	CString m_strLegalTrademarks;
	CString m_strOriginalFilename;
	CString m_strSpecialBuild;
	CString m_strPrivateBuild;

	virtual BOOL Init();

protected:
	BOOL GetFileVersionFromFile(LPCTSTR lpszFileName);
};

#endif // !defined(AFX_FVERSION_H__87F6D2FE_C016_448A_9F29_C88E8CDAF218__INCLUDED_)
