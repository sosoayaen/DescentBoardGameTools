// EditorPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mapeditor.h"
#include "EditorPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorPropertyDlg dialog


CEditorPropertyDlg::CEditorPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditorPropertyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEditorPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorPropertyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CEditorPropertyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorPropertyDlg message handlers

BOOL CEditorPropertyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PropCtrl.Create(this, IDC_STATIC_PROPERTY_EDITOR);
	m_PropCtrl.SetPropertyHost(&m_PropObjEditor);
	m_PropCtrl.EnableCommentPane(true);
	m_PropCtrl.SetCommentPaneHeight(13*4 + 6);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditorPropertyDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
