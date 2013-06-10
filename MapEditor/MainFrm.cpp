// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MapEditor.h"
#include "MapEditorDoc.h"
#include "MainFrm.h"
#include "MapPropertyDlg.h"
#include "EditorPropertyDlg.h"
#include "CanvasScrollView/canvasscrollview.h"
#include "MapEditorSplitterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER_DEBUG_INFO_SHOW, OnShowDebugInfo)
	ON_MESSAGE(WM_USER_STATUSBAR_MOUSE_POSTION_UPDATE, OnUpdateMousePosInformation)
	ON_MESSAGE(WM_USER_STATUSBAR_ZOOMED_RATE_UPDATE, OnUpdateZoomedRate)
	ON_MESSAGE(WM_USER_PROPERTY_VIEW_UPDATE, OnUpdatePropertyView)
	ON_WM_ENDSESSION()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_INFO_DLG, OnUpdateButtonShowInfoDlg)
	ON_COMMAND(ID_SET_EDITOR, OnSetEditor)
	ON_COMMAND(ID_SET_MAP, OnSetMap)
	ON_COMMAND(ID_BUTTON_HIDE_PROPERTY, OnButtonHideProperty)
	ON_COMMAND(ID_MI_SHOW_LAYER_DOOR, OnMiShowLayerDoor)
	ON_UPDATE_COMMAND_UI(ID_MI_SHOW_LAYER_DOOR, OnUpdateMiShowLayerDoor)
	ON_COMMAND(ID_MI_SHOW_LAYER_MASTER, OnMiShowLayerMaster)
	ON_UPDATE_COMMAND_UI(ID_MI_SHOW_LAYER_MASTER, OnUpdateMiShowLayerMaster)
	ON_COMMAND(ID_MI_SHOW_LAYER_MONSTER, OnMiShowLayerMonster)
	ON_UPDATE_COMMAND_UI(ID_MI_SHOW_LAYER_MONSTER, OnUpdateMiShowLayerMonster)
	ON_COMMAND(ID_MI_SHOW_LAYER_OBJECT, OnMiShowLayerObject)
	ON_UPDATE_COMMAND_UI(ID_MI_SHOW_LAYER_OBJECT, OnUpdateMiShowLayerObject)
	ON_COMMAND(ID_MI_SHOW_LAYER_OBSTACLE, OnMiShowLayerObstacle)
	ON_UPDATE_COMMAND_UI(ID_MI_SHOW_LAYER_OBSTACLE, OnUpdateMiShowLayerObstacle)
	ON_COMMAND(ID_MI_SHOW_LAYER_TILE, OnMiShowLayerTile)
	ON_UPDATE_COMMAND_UI(ID_MI_SHOW_LAYER_TILE, OnUpdateMiShowLayerTile)
	ON_COMMAND(ID_MI_SHOW_LAYER_TREASURE, OnMiShowLayerTreasure)
	ON_UPDATE_COMMAND_UI(ID_MI_SHOW_LAYER_TREASURE, OnUpdateMiShowLayerTreasure)
	ON_COMMAND(ID_BUTTON_LOS, OnButtonLos)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LOS, OnUpdateButtonLos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	// ID_SEPARATOR,
	IDS_STATUS_ZOOMED_RATE,
	IDS_STATUS_MOUSE_POS,	// status mouse position label
 	// ID_INDICATOR_CAPS,
 	// ID_INDICATOR_NUM,
 	// ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pDlgDebugInfo = new CInfoShowDialog;

	m_pCanvasView = NULL;
	m_PropertyView = NULL;
//	m_pSplitterView = NULL;
}

CMainFrame::~CMainFrame()
{
	m_pDlgDebugInfo->DestroyWindow();
	delete m_pDlgDebugInfo;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.LoadTrueColorToolBar(16, 16, IDB_BITMAP_MAIN_TOOLBAR, IDB_BITMAP_MAIN_TOOLBAR, IDB_BMP_MAIN_TOOLBAR_DISABLED);
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_SHOW_GRID), TBBS_CHECKBOX | TBBS_CHECKED);
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_SELECT_MODE), TBBS_CHECKBOX);
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_SHOW_INFO_DLG), TBBS_CHECKBOX);
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_HIDE_PROPERTY), TBBS_CHECKBOX);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM);
	DockControlBar(&m_wndToolBar);
	
	m_pDlgDebugInfo->Create(IDD_INFO_SHOW, this);
	m_pDlgDebugInfo->CenterWindow();

	CString strTitle;
	strTitle.LoadString(AFX_IDS_APP_TITLE);
	SetWindowText(strTitle);
	m_wndStatusBar.InitStatusBar();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	TRACE("CMainFrame::PreTranslateMessage MSG:0x%04d\n", pMsg->message);
	if (pMsg->message == WM_LBUTTONUP)
	{
		TRACE("CMainFrame::PreTranslateMessage MSG:WM_LBUTTONUP\n");
		SendMessage(WM_LBUTTONUP, pMsg->wParam, pMsg->lParam);
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

LRESULT CMainFrame::OnShowDebugInfo( WPARAM wParam, LPARAM lParam )
{
	ASSERT(IsWindow(m_pDlgDebugInfo->GetSafeHwnd()));
	if (m_pDlgDebugInfo->IsWindowVisible())
	{
		// Notify the Dialog to show debug info from the buffer
		m_pDlgDebugInfo->PostMessage(WM_USER_DEBUG_INFO_SHOW, wParam, lParam);
	}
	return 0;
}

void CMainFrame::ShowDebugDialog()
{
	ASSERT(IsWindow(m_pDlgDebugInfo->GetSafeHwnd()));
	if (m_pDlgDebugInfo->IsWindowVisible())
	{
		m_pDlgDebugInfo->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pDlgDebugInfo->ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnEndSession(BOOL bEnding) 
{
	CFrameWnd::OnEndSession(bEnding);
	
	// TODO: Add your message handler code here
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
	pApp->m_logManager.Flush();
}

void CMainFrame::OnUpdateButtonShowInfoDlg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_pDlgDebugInfo->IsWindowVisible());
}

LRESULT CMainFrame::OnUpdateMousePosInformation( WPARAM wParam, LPARAM lParam )
{
	m_wndStatusBar.PostMessage(WM_USER_STATUSBAR_MOUSE_POSTION_UPDATE, wParam, lParam);
	return TRUE;
}

LRESULT CMainFrame::OnUpdateZoomedRate( WPARAM wParam, LPARAM lParam )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (pDoc)
	{
		double dZoomedRate = pDoc->GetMapZoom();
		return m_wndStatusBar.SendMessage(WM_USER_STATUSBAR_ZOOMED_RATE_UPDATE, wParam, (LPARAM)&dZoomedRate);
	}
	return FALSE;
}

void CMainFrame::OnSetEditor() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();

	CEditorPropertyDlg dlg;
	// 初始化属性值
	dlg.m_PropObjEditor.m_bShowGridLine = pDoc->m_bShowGrid;
	dlg.m_PropObjEditor.m_nGridLineWeight = pDoc->GetCanvasMapGridLineWeight();
	dlg.m_PropObjEditor.m_colorBackground = pDoc->GetCanvasGBColor();
	dlg.m_PropObjEditor.m_colorGridLine = pDoc->GetCanvasGLColor();
	dlg.m_PropObjEditor.m_nGridLineStyle = pDoc->GetCanvasMapGridLineStyle();
	dlg.m_PropObjEditor.m_bShowEverySelectedMapElementFrame = pDoc->IsShowEveryMESelectionFrame();
	dlg.m_PropObjEditor.m_colorEverySelectedMapElementFrame = pDoc->GetMESelectionFrameColor();
	
	if (IDOK == dlg.DoModal())
	{
		// 更新配置
		pDoc->SetCanvasBGColor(dlg.m_PropObjEditor.m_colorBackground);
		pDoc->m_bShowGrid = dlg.m_PropObjEditor.m_bShowGridLine;
		pDoc->SetCanvasGLColor(dlg.m_PropObjEditor.m_colorGridLine);
		pDoc->SetCanvasMapGridLineWeight(dlg.m_PropObjEditor.m_nGridLineWeight);
		pDoc->SetCanvasMapGridLineStyle(dlg.m_PropObjEditor.m_nGridLineStyle);
		pDoc->SetShowEveryMESelectionFrame(dlg.m_PropObjEditor.m_bShowEverySelectedMapElementFrame);
		pDoc->SetMESelectionFrameColor(dlg.m_PropObjEditor.m_colorEverySelectedMapElementFrame);
		// 通知界面刷新
		if (NULL == m_pCanvasView)
		{
			CView* tView = NULL;
			// 寻找对应的绘图区View
			POSITION pos = pDoc->GetFirstViewPosition();
			while (pos)
			{
				tView = pDoc->GetNextView(pos);
				if (tView->IsKindOf(RUNTIME_CLASS(CCanvasScrollView)))
				{
					m_pCanvasView = (CCanvasScrollView*)tView;
					break;
				}
			}
		}

		if (m_pCanvasView)
		{
			m_pCanvasView->Invalidate(FALSE);
		}
	}
}

void CMainFrame::OnSetMap() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	CMapPropertyDlg dlg;
	dlg.m_PropObjMap.m_nPlayerCount = pDoc->m_nPlayerCount;

	if (IDOK == dlg.DoModal())
	{
		pDoc->m_nPlayerCount = dlg.m_nPlayerCount;
		// 通知怪物属性修改
		pDoc->OnPlayerCountChange();
	}
}

LRESULT CMainFrame::OnUpdatePropertyView( WPARAM wParam, LPARAM lParam )
{

	BOOL bRet = FALSE;
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (NULL == m_PropertyView)
	{
		CView* tView = NULL;
		// 寻找对应的绘图区View
		POSITION pos = pDoc->GetFirstViewPosition();
		while (pos)
		{
			tView = pDoc->GetNextView(pos);
			if (tView->IsKindOf(RUNTIME_CLASS(CObjectPropertyView)))
			{
				m_PropertyView = (CObjectPropertyView*)tView;
				bRet = TRUE;
				break;
			}
		}
	}
	
	if (m_PropertyView)
	{
		m_PropertyView->SetPropertyHost(pDoc->m_pCurrentPropertyObj);
		// m_PropertyView->Invalidate(FALSE);
	}
	
	return bRet;
}

void CMainFrame::OnButtonHideProperty() 
{
	// TODO: Add your command handler code here

 	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (pDoc)
	{
		CView* tView = NULL;
		// 寻找对应的绘图区View
		POSITION pos = pDoc->GetFirstViewPosition();
		while (pos)
		{
			tView = pDoc->GetNextView(pos);
			if (tView && tView->IsKindOf(RUNTIME_CLASS(CMapEditorSplitterView)))
			{
				CMapEditorSplitterView* pSplitterView = (CMapEditorSplitterView*)tView;
				pSplitterView->TogglePropertyView();
				break;
			}
		}
	}
}

void CMainFrame::OnMiShowLayerDoor() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (pDoc)
	{
		pDoc->ToggleMapLayerVisible(ME_TYPE_DOOR);
	}
}

void CMainFrame::OnUpdateMiShowLayerDoor(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->GetMapLayerVisible(ME_TYPE_DOOR));
}

void CMainFrame::OnMiShowLayerMaster() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (pDoc)
	{
		pDoc->ToggleMapLayerVisible(ME_TYPE_MONSTER_MASTER);
	}
}

void CMainFrame::OnUpdateMiShowLayerMaster(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->GetMapLayerVisible(ME_TYPE_MONSTER_MASTER));
}

void CMainFrame::OnMiShowLayerMonster() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (pDoc)
	{
		pDoc->ToggleMapLayerVisible(ME_TYPE_MONSTER);
	}
}

void CMainFrame::OnUpdateMiShowLayerMonster(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->GetMapLayerVisible(ME_TYPE_MONSTER));
}

void CMainFrame::OnMiShowLayerObject() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (pDoc)
	{
		pDoc->ToggleMapLayerVisible(ME_TYPE_OBJECT);
	}
}

void CMainFrame::OnUpdateMiShowLayerObject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->GetMapLayerVisible(ME_TYPE_OBJECT));
}

void CMainFrame::OnMiShowLayerObstacle() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (pDoc)
	{
		pDoc->ToggleMapLayerVisible(ME_TYPE_OBSTACLE);
	}
}

void CMainFrame::OnUpdateMiShowLayerObstacle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->GetMapLayerVisible(ME_TYPE_OBSTACLE));
}

void CMainFrame::OnMiShowLayerTile() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (pDoc)
	{
		pDoc->ToggleMapLayerVisible(ME_TYPE_TILE);
	}
}

void CMainFrame::OnUpdateMiShowLayerTile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->GetMapLayerVisible(ME_TYPE_TILE));
}

void CMainFrame::OnMiShowLayerTreasure() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	if (pDoc)
	{
		pDoc->ToggleMapLayerVisible(ME_TYPE_TREASURE);
	}
}

void CMainFrame::OnUpdateMiShowLayerTreasure(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->GetMapLayerVisible(ME_TYPE_TREASURE));
}

void CMainFrame::OnButtonLos()
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	pDoc->m_bIsLOSMode = !pDoc->m_bIsLOSMode;
}

void CMainFrame::OnUpdateButtonLos(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetActiveDocument();
	pCmdUI->SetCheck(pDoc->m_bIsLOSMode);
}
