// LogManager.cpp: implementation of the CLogManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogManager.h" 
#include <DIRECT.H>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogManager::CLogManager():m_nMaxBuffer(100)
{
	m_bSave2File = false;
}

CLogManager::CLogManager( LPCTSTR lpszFileName, long nMaxBufferStored )
{
	m_strLogFileName = _T("\\log\\");
	m_strLogFileName.append(lpszFileName);
	m_nMaxBuffer = nMaxBufferStored;
	m_bSave2File = false;
}

CLogManager::~CLogManager()
{
	if (m_bSave2File)
	{
		Flush();
	}
}

int CLogManager::Append( std::string& strLog )
{
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		// 塞到队列尾部
		m_qBuffer.push(strLog);
	}
	return 0;
}

int CLogManager::Append( LPCTSTR lpszLog )
{
	std::string strLog = lpszLog;
	return Append(strLog);
}

long CLogManager::GetCount()
{
	return m_qBuffer.size();
}

int CLogManager::Flush()
{
	return WriteLog2File(GetCount());
}

int CLogManager::WriteLog2File( unsigned long nCount /*= 100*/ )
{
	CSingleLock lock(&m_mutex);

	if (lock.Lock())
	{
		CFile fileLog;
		// Create the directory
		char appPath[MAX_PATH];
		GetModuleFileName(NULL, appPath, MAX_PATH);
		CString filePath = appPath;
		filePath += _T("\\log");
		_mkdir(filePath);

		if (!fileLog.Open(m_strLogFileName.c_str(), CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeBinary))
		{
			TRACE(_T("打开日志文件失败！\n"));
			return -1;
		}
		
		std::string strLog;
		unsigned long nSize = max(nCount, m_nMaxBuffer);
		nSize = min(nSize, this->GetCount());
		for (unsigned long idx = 0; idx < nSize; idx++)
		{
			CTime tm = CTime::GetCurrentTime();
			
			std::string strPrefix = tm.Format("[%Y-%m-%d %H:%M:%S]");
			strLog = m_qBuffer.front();
			fileLog.Write(strPrefix.c_str(), strPrefix.length());
			fileLog.Write("\r\n", 2);
			fileLog.Write(strLog.c_str(), strLog.length());
			m_qBuffer.pop();
		}
		fileLog.Close();
	}
	
	return 0;
}

void CLogManager::SetLogFileName( LPCTSTR lpszFileName )
{
	m_strLogFileName = _T("\\log\\");
	m_strLogFileName.append(lpszFileName);
}

void CLogManager::EnableWrite2File()
{
	m_bSave2File = true;
}

void CLogManager::DisableWrite2File()
{
	m_bSave2File = false;	
}

void CLogManager::GetLineData( LPTSTR lpszData, int nSize )
{
	if (lpszData == NULL)
	{
		return;
	}
	char* pCh = lpszData;
	std::string strLine = GetLineData();
	for (int idx = 0; idx < nSize; idx++)
	{
		*pCh++ = strLine[idx];
	}
}

std::string CLogManager::GetLineData()
{
	std::string strLine = m_qBuffer.front();

	if (m_bSave2File)
	{
		WriteLog2File(1);
	}
	else
	{
		m_qBuffer.pop();
	}
	return strLine;
}
