// MapPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mapeditor.h"
#include "MapPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapPropertyDlg dialog


CMapPropertyDlg::CMapPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapPropertyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMapPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapPropertyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CMapPropertyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapPropertyDlg message handlers

BOOL CMapPropertyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_nPlayerCount = MIN_PLAYERS;

	m_PropCtrl.Create(this, IDC_STATIC_PROPERTY_MAP);
	m_PropCtrl.SetPropertyHost(&m_PropObjMap);
	m_PropCtrl.EnableCommentPane(true);
	m_PropCtrl.SetCommentPaneHeight(13*4 + 6);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapPropertyDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_nPlayerCount = m_PropObjMap.m_nPlayerCount;

	CDialog::OnOK();
}
