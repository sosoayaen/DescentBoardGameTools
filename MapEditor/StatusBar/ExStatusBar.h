// ExStatusBar.h: interface for the CExStatusBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXSTATUSBAR_H__8BBB024C_9A57_4D4C_8FFC_61DAF945F570__INCLUDED_)
#define AFX_EXSTATUSBAR_H__8BBB024C_9A57_4D4C_8FFC_61DAF945F570__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CExStatusBar
class CExStatusBar : public CStatusBar  
{
	// DECLARE_DYNAMIC(CExStatusBar)
public:
// Construction
	CExStatusBar();
	
// Attributes
private:

// Operations
public:
	// 初始化状态栏，增加对应控件的显示属性
	void InitStatusBar();

private:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExStatusBar)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CExStatusBar();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CExStatusBar)
	afx_msg LRESULT OnShowMousePosition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowZoomedRate(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXSTATUSBAR_H__8BBB024C_9A57_4D4C_8FFC_61DAF945F570__INCLUDED_)
