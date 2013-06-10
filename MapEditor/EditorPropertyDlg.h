#if !defined(AFX_EDITORPROPERTYDLG_H__2E7F041F_B1C1_4024_995B_125F595AE385__INCLUDED_)
#define AFX_EDITORPROPERTYDLG_H__2E7F041F_B1C1_4024_995B_125F595AE385__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorPropertyDlg.h : header file
//
#include "PropertyObject.h"
#include "EPropCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CEditorPropertyDlg dialog

class CEditorPropertyDlg : public CDialog
{
public:

	EPropCtrl m_PropCtrl;
	PropertyObjectEditor m_PropObjEditor;

// Construction
public:
	CEditorPropertyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditorPropertyDlg)
	enum { IDD = IDD_SET_EDITOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorPropertyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditorPropertyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORPROPERTYDLG_H__2E7F041F_B1C1_4024_995B_125F595AE385__INCLUDED_)
