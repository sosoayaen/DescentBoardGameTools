// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__9D78C045_A787_42CD_978D_3BFD0A190C55__INCLUDED_)
#define AFX_MAINFRM_H__9D78C045_A787_42CD_978D_3BFD0A190C55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrueColorToolbar/TrueColorToolBar.h"
#include "InfoShowDialog.h"
#include "StatusBar/ExStatusBar.h"
#include "CanvasScrollView/canvasscrollview.h"
#include "ObjectPropertyView.h"
//#include "MapEditorSplitterView.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	void ShowDebugDialog();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
//	CStatusBar  m_wndStatusBar;
	CExStatusBar  m_wndStatusBar;
//	CToolBar    m_wndToolBar;
	CTrueColorToolBar m_wndToolBar;

	CInfoShowDialog* m_pDlgDebugInfo;

//	CMapEditorSplitterView* m_pSplitterView;

	CCanvasScrollView* m_pCanvasView;

	CObjectPropertyView* m_PropertyView;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnShowDebugInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateMousePosInformation(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateZoomedRate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePropertyView(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg void OnUpdateButtonShowInfoDlg(CCmdUI* pCmdUI);
	afx_msg void OnSetEditor();
	afx_msg void OnSetMap();
	afx_msg void OnButtonHideProperty();
	afx_msg void OnMiShowLayerDoor();
	afx_msg void OnUpdateMiShowLayerDoor(CCmdUI* pCmdUI);
	afx_msg void OnMiShowLayerMaster();
	afx_msg void OnUpdateMiShowLayerMaster(CCmdUI* pCmdUI);
	afx_msg void OnMiShowLayerMonster();
	afx_msg void OnUpdateMiShowLayerMonster(CCmdUI* pCmdUI);
	afx_msg void OnMiShowLayerObject();
	afx_msg void OnUpdateMiShowLayerObject(CCmdUI* pCmdUI);
	afx_msg void OnMiShowLayerObstacle();
	afx_msg void OnUpdateMiShowLayerObstacle(CCmdUI* pCmdUI);
	afx_msg void OnMiShowLayerTile();
	afx_msg void OnUpdateMiShowLayerTile(CCmdUI* pCmdUI);
	afx_msg void OnMiShowLayerTreasure();
	afx_msg void OnUpdateMiShowLayerTreasure(CCmdUI* pCmdUI);
	afx_msg void OnButtonLos();
	afx_msg void OnUpdateButtonLos(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9D78C045_A787_42CD_978D_3BFD0A190C55__INCLUDED_)
