// InfoShowDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mapeditor.h"
#include "InfoShowDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoShowDialog dialog


CInfoShowDialog::CInfoShowDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoShowDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoShowDialog)
	//}}AFX_DATA_INIT

	// 设定当前非自动滚动
	m_bScrollWithData = false;
}


void CInfoShowDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoShowDialog)
	DDX_Control(pDX, IDC_LIST_INFO_SHOW_LOG, m_listCtl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoShowDialog, CDialog)
	//{{AFX_MSG_MAP(CInfoShowDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_CLEAR_LOG, OnButtonClearLog)
	ON_MESSAGE(WM_USER_DEBUG_INFO_SHOW, OnShowDebugInfo)
	ON_COMMAND(ID_BUTTON_SCROLL_LOG, OnButtonScrollLog)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoShowDialog message handlers

BOOL CInfoShowDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_anchar.Init(GetSafeHwnd());

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |
		CBRS_TOOLTIPS | CBRS_ALIGN_TOP, CRect(4, 4, 0, 0)) ||
		!m_wndToolBar.LoadToolBar(IDR_TB_INFO_SHOW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	m_wndToolBar.LoadTrueColorToolBar(16, 16, IDB_DEBUG_TOOLBAR, IDB_DEBUG_TOOLBAR);
	
	m_wndToolBar.SetButtonStyle(1, TBBS_CHECKBOX);
	//	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	//取得对话框客户区的矩形区域大小存于rcClientStart当中
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);
	//上面创建了工具栏所以调整一次工具栏的默认高度就是30(经测量得出的结论),前面已经创建了一个工具栏,
	// 并通过 RepositionBars进行了调整所以客户区rcClientNow矮了30
	
	CPoint ptOffset(rcClientNow.left-rcClientStart.left, rcClientNow.top-rcClientStart.top); 
	CRect rcChild;      //用来保存子窗口的矩形区域大小                          
	CWnd* pwndChild = GetWindow(GW_CHILD);//取得CWnd第一个子窗口的指针
	while (pwndChild)
	{ //循环将所有的子窗口重新调整其位置                             
		pwndChild->GetWindowRect(rcChild);//取得句柄所对应子窗口的矩形坐标(屏幕坐标)存入rcChild
		ScreenToClient(rcChild);//将屏幕坐标转化为窗客户坐标
		rcChild.OffsetRect(ptOffset);//将矩形rcChild按照ptOffset进行偏移计算
		pwndChild->MoveWindow(rcChild, FALSE);//将子窗口移动到rcChild处
		pwndChild = pwndChild->GetNextWindow();//取得下一个子窗口的指针
	}
	///////////////下面将对话框大小重整////////////////////////////////////////////////////////
	CRect rcWindow;
	GetWindowRect(rcWindow);//取得窗口的屏幕矩形坐标,包含标题栏与边框在内
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);//将控件重排一次,改动了对话框的大小,再调整一次控件的位置,否则控件就不见啦
    CenterWindow();

	// 增加显示日志的控件的锚点，尺寸跟着父窗口的变化而变化
	m_anchar.Add(IDC_LIST_INFO_SHOW_LOG, ANCHOR_ALL);

	return TRUE; // return TRUE unless you set the focus to a control
                     // EXCEPTION: OCX Property Pages should return FALSE
}

int CInfoShowDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void CInfoShowDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_anchar.OnSize();
}

void CInfoShowDialog::OnButtonClearLog()
{
	// TODO: Add your command handler code here
	m_listCtl.ResetContent();
	UpdateData(FALSE);
}

#define CHAR_COUNTS_IN_SCREEN 1024*10
LRESULT CInfoShowDialog::OnShowDebugInfo( WPARAM wParam, LPARAM lParam )
{
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();

	if (IsWindowVisible())
	{
		if (m_listCtl.GetCount() > CHAR_COUNTS_IN_SCREEN)
		{
			m_listCtl.ResetContent();
		}
		CTime tm = CTime::GetCurrentTime();
		CString str;
		str.Format("[%s]%s", 
			tm.Format("%Y%m%d %H:%M:%S"),
			pApp->m_logManager.GetLineData().c_str());
		m_listCtl.AddString(str);

		m_listCtl.SetRedraw(FALSE);
		UpdateData(FALSE);
		if (m_bScrollWithData)
		{
			m_listCtl.SetTopIndex(m_listCtl.GetCount() - 1);
		}
		m_listCtl.SetRedraw(TRUE);
	}
	return 0;
}

void CInfoShowDialog::OnButtonScrollLog() 
{
	// TODO: Add your command handler code here
	m_bScrollWithData = !m_bScrollWithData;
}

BOOL CInfoShowDialog::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
	
	// UNICODE 消息
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	
	if(pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom 为工具条的HWND
		nID = ::GetDlgCtrlID((HWND)nID);
	}
	
	if(nID != 0) // 不为分隔符
	{
		strTipText.LoadString(nID);
		strTipText = strTipText.Mid(strTipText.Find('\n', 0) + 1);
		
#ifndef _UNICODE
		if(pNMHDR->code == TTN_NEEDTEXTA)
		{
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#else
		if(pNMHDR->code == TTN_NEEDTEXTA)
		{
			_wcstombsz(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#endif
		
		*pResult = 0;
		
		// 使工具条提示窗口在最上面
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE|
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
		return TRUE;
	}
	return TRUE;
}
