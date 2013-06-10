#if !defined(AFX_INFOSHOWDIALOG_H__62C048F8_5BAC_4DEA_AC71_22138AF28AC8__INCLUDED_)
#define AFX_INFOSHOWDIALOG_H__62C048F8_5BAC_4DEA_AC71_22138AF28AC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoShowDialog.h : header file
//
#include "anchor.h"
#include "TrueColorToolbar/TrueColorToolBar.h"
/////////////////////////////////////////////////////////////////////////////
// CInfoShowDialog dialog

class CInfoShowDialog : public CDialog
{
// Construction
public:
	CInfoShowDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInfoShowDialog)
	enum { IDD = IDD_INFO_SHOW };
	CListBox	m_listCtl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoShowDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTrueColorToolBar m_wndToolBar;
	CDlgAnchor m_anchar;
	bool m_bScrollWithData;

	// Generated message map functions
	//{{AFX_MSG(CInfoShowDialog)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonClearLog();
	afx_msg LRESULT OnShowDebugInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonScrollLog();
	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOSHOWDIALOG_H__62C048F8_5BAC_4DEA_AC71_22138AF28AC8__INCLUDED_)
