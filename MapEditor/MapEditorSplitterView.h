// MapEditorSplitterView.h : interface of the CMapEditorSplitterView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITORSPLITTERVIEW_H__47D4F6BA_CA44_4518_B5C9_3EE4E9A4C4D0__INCLUDED_)
#define AFX_MAPEDITORSPLITTERVIEW_H__47D4F6BA_CA44_4518_B5C9_3EE4E9A4C4D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MESplitterWnd.h"

class CMapEditorSplitterView : public CView
{
protected: // create from serialization only
	CMapEditorSplitterView();
	DECLARE_DYNCREATE(CMapEditorSplitterView)

// Attributes
public:
	CMapEditorDoc* GetDocument();

	CMESplitterWnd m_wndSplitter;
	CMESplitterWnd* m_pWndSplitter;

	int m_nPropertyViewWidth;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditorSplitterView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapEditorSplitterView();
	//////////////////////////////////////////////////////////////////////////
	// 隐藏和显示属性窗口
	void TogglePropertyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMapEditorSplitterView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MapEditorSplitterView.cpp
inline CMapEditorDoc* CMapEditorSplitterView::GetDocument()
   { return (CMapEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITORSPLITTERVIEW_H__47D4F6BA_CA44_4518_B5C9_3EE4E9A4C4D0__INCLUDED_)
