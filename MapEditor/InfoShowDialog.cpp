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

	// �趨��ǰ���Զ�����
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
	//ȡ�öԻ���ͻ����ľ��������С����rcClientStart����
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);
	//���洴���˹��������Ե���һ�ι�������Ĭ�ϸ߶Ⱦ���30(�������ó��Ľ���),ǰ���Ѿ�������һ��������,
	// ��ͨ�� RepositionBars�����˵������Կͻ���rcClientNow����30
	
	CPoint ptOffset(rcClientNow.left-rcClientStart.left, rcClientNow.top-rcClientStart.top); 
	CRect rcChild;      //���������Ӵ��ڵľ��������С                          
	CWnd* pwndChild = GetWindow(GW_CHILD);//ȡ��CWnd��һ���Ӵ��ڵ�ָ��
	while (pwndChild)
	{ //ѭ�������е��Ӵ������µ�����λ��                             
		pwndChild->GetWindowRect(rcChild);//ȡ�þ������Ӧ�Ӵ��ڵľ�������(��Ļ����)����rcChild
		ScreenToClient(rcChild);//����Ļ����ת��Ϊ���ͻ�����
		rcChild.OffsetRect(ptOffset);//������rcChild����ptOffset����ƫ�Ƽ���
		pwndChild->MoveWindow(rcChild, FALSE);//���Ӵ����ƶ���rcChild��
		pwndChild = pwndChild->GetNextWindow();//ȡ����һ���Ӵ��ڵ�ָ��
	}
	///////////////���潫�Ի����С����////////////////////////////////////////////////////////
	CRect rcWindow;
	GetWindowRect(rcWindow);//ȡ�ô��ڵ���Ļ��������,������������߿�����
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);//���ؼ�����һ��,�Ķ��˶Ի���Ĵ�С,�ٵ���һ�οؼ���λ��,����ؼ��Ͳ�����
    CenterWindow();

	// ������ʾ��־�Ŀؼ���ê�㣬�ߴ���Ÿ����ڵı仯���仯
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
	
	// UNICODE ��Ϣ
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	
	if(pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom Ϊ��������HWND
		nID = ::GetDlgCtrlID((HWND)nID);
	}
	
	if(nID != 0) // ��Ϊ�ָ���
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
		
		// ʹ��������ʾ������������
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE|
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
		return TRUE;
	}
	return TRUE;
}
