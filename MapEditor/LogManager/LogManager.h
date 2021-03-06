// LogManager.h: interface for the CLogManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMANAGER_H__A8F5ABCE_AA3E_4025_AD0E_2D8F71F389F2__INCLUDED_)
#define AFX_LOGMANAGER_H__A8F5ABCE_AA3E_4025_AD0E_2D8F71F389F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXMT.H>
#include <string>
#include <queue>

class CLogManager  
{
public:
	CLogManager();
	CLogManager(LPCTSTR lpszFileName, long nMaxBufferStored = 100);
	virtual ~CLogManager();

	//////////////////////////////////////////////////////////////////////////
	// 设置日志输出文件名
	void SetLogFileName(LPCTSTR lpszFileName);

	//////////////////////////////////////////////////////////////////////////
	// 不记录日志到文件
	void DisableWrite2File();

	//////////////////////////////////////////////////////////////////////////
	// 记录日志到文件
	void EnableWrite2File();

	//////////////////////////////////////////////////////////////////////////
	// 得到一行日志数据文件
	void GetLineData(LPTSTR lpszData, int nSize);
	std::string GetLineData();

	//////////////////////////////////////////////////////////////////////////
	// 增加日志
	int Append(std::string& strLog);
	int Append(LPCTSTR lpszLog);

	//////////////////////////////////////////////////////////////////////////
	// 得到当前日志队列的长度
	long GetCount();

	//////////////////////////////////////////////////////////////////////////
	// 全部输出日志文件
	int Flush();

	//////////////////////////////////////////////////////////////////////////
	// 写日志文件
	int WriteLog2File(unsigned long nCount = 100);
protected:
	std::string m_strLogFileName;
	std::queue<std::string> m_qBuffer;

	//////////////////////////////////////////////////////////////////////////
	// 是否记录日志数据到日志文件标识
	bool m_bSave2File;
	//////////////////////////////////////////////////////////////////////////
	// 指明最多缓存多少数据
	unsigned long m_nMaxBuffer;
	//////////////////////////////////////////////////////////////////////////
	// 互斥量
	CMutex m_mutex;
};

#endif // !defined(AFX_LOGMANAGER_H__A8F5ABCE_AA3E_4025_AD0E_2D8F71F389F2__INCLUDED_)
