// FlyAntsDlg.h : header file
//

#if !defined(AFX_FLYANTSDLG_H__B81C7E1F_7F16_44C5_84AF_66E702C48055__INCLUDED_)
#define AFX_FLYANTSDLG_H__B81C7E1F_7F16_44C5_84AF_66E702C48055__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "luna.hpp"

// 数据文件类型枚举
typedef enum {
	DFT_EXCEL,
	DFT_XML,
		DFT_LUA,
} DATA_FILE_TYPE;

/////////////////////////////////////////////////////////////////////////////
// CFlyAntsDlg dialog

class CFlyAntsDlg : public CDialog
{
// Construction
public:
	CFlyAntsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlyAntsDlg)
	enum { IDD = IDD_FLYANTS_DIALOG };
	CString	m_strOutputFilePath;
	CString	m_strSourceFilePath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlyAntsDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	lua_State* m_pLuaEnv;

	DATA_FILE_TYPE m_eOFT;	// 文件输出类型
	DATA_FILE_TYPE m_eIFT;	// 文件输入类型

	// Generated message map functions
	//{{AFX_MSG(CFlyAntsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAppAbout();
	afx_msg void OnBtnDstBrsw();
	afx_msg void OnBtnSrcBrsw();
	afx_msg void OnBtnConvert();
	afx_msg void OnMenuitemExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//////////////////////////////////////////////////////////////////////////
	// 设置弹出的文件对话框的过滤文件
	void SetFileDialogFilter( OPENFILENAME& ofn, CString& strFilter );

	//////////////////////////////////////////////////////////////////////////
	// 数据转换函数
	bool DoConvert(CString& strErrMsg);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLYANTSDLG_H__B81C7E1F_7F16_44C5_84AF_66E702C48055__INCLUDED_)
