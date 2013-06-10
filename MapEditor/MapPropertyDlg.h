#if !defined(AFX_MAPPROPERTYDLG_H__246C4736_2DC1_4233_8988_576F4D9F36E2__INCLUDED_)
#define AFX_MAPPROPERTYDLG_H__246C4736_2DC1_4233_8988_576F4D9F36E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapPropertyDlg.h : header file
//

#include "EPropCtrl.h"
#include "PropertyObject.h"
/////////////////////////////////////////////////////////////////////////////
// CMapPropertyDlg dialog

class CMapPropertyDlg : public CDialog
{
public:
	EPropCtrl m_PropCtrl;
	PropertyObjectMap m_PropObjMap;

	int m_nPlayerCount;
// Construction
public:
	CMapPropertyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMapPropertyDlg)
	enum { IDD = IDD_SET_MAP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapPropertyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapPropertyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPPROPERTYDLG_H__246C4736_2DC1_4233_8988_576F4D9F36E2__INCLUDED_)
