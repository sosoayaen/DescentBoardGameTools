// FlyAntsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FlyAnts.h"
#include "FlyAntsDlg.h"
#include "FVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

// 在 debug 模式下增加命令行调试输出
#include <CONIO.H>

#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strCopyRight;
	CString	m_strVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strCopyRight = _T("");
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_COPYRIGHT, m_strCopyRight);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CFVersionInfo version;
	if (version.Init())
	{
		m_strVersion.Format(_T("文件版本：%s\n产品版本：%s"), version.m_strFileVersion, version.m_strProductVersion);
		m_strCopyRight = version.m_strLegalCopyright;
		UpdateData(FALSE);
	}
	CString strInfo;
	strInfo.LoadString(IDS_STRING_INFOMATION);
	SetDlgItemText(IDC_EDIT_INFO, strInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CFlyAntsDlg dialog

CFlyAntsDlg::CFlyAntsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlyAntsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlyAntsDlg)
	m_strOutputFilePath = _T("");
	m_strSourceFilePath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFlyAntsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlyAntsDlg)
	DDX_Text(pDX, IDC_EDIT_OUTPUT_FILE, m_strOutputFilePath);
	DDX_Text(pDX, IDC_EDIT_SOURCE_FILE, m_strSourceFilePath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlyAntsDlg, CDialog)
	//{{AFX_MSG_MAP(CFlyAntsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_BN_CLICKED(IDC_BTN_DST_BRSW, OnBtnDstBrsw)
	ON_BN_CLICKED(IDC_BTN_SRC_BRSW, OnBtnSrcBrsw)
	ON_BN_CLICKED(IDC_BTN_CONVERT, OnBtnConvert)
	ON_COMMAND(ID_MENUITEM_EXIT, OnMenuitemExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlyAntsDlg message handlers

BOOL CFlyAntsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
#ifdef _DEBUG
	// Debug 模式下增加命令行调试
	AllocConsole();
#endif // _DEBUG
	
	// TODO: Add extra initialization here
	// 初始化Lua环境
	m_pLuaEnv = luaL_newstate();
	if (NULL != m_pLuaEnv)
	{
		luaL_openlibs(m_pLuaEnv);
		
		if (luaL_loadfile(m_pLuaEnv, _T("script\\main.lua")) || lua_pcall(m_pLuaEnv, 0, 0, 0))
		{
			AfxMessageBox(lua_tostring(m_pLuaEnv, -1));
			lua_close(m_pLuaEnv);
			m_pLuaEnv = NULL;
		}
	}

	m_eIFT = DFT_EXCEL;	// 默认输入为excel
	m_eOFT = DFT_XML;	// 默认输出为xml

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFlyAntsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFlyAntsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFlyAntsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CFlyAntsDlg::OnAppAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CFlyAntsDlg::OnBtnDstBrsw() 
{
	// TODO: Add your control notification handler code here
	CString strFilter = _T("");
	strFilter.LoadString(IDS_STR_OUTPUT_FILE_FILTER);

	CFileDialog openDstDlg(FALSE);
	SetFileDialogFilter(openDstDlg.m_ofn, strFilter);

	int nRet = openDstDlg.DoModal();
	if (IDOK == nRet)
	{
		SetDlgItemText(IDC_EDIT_OUTPUT_FILE, openDstDlg.GetPathName());
		switch(openDstDlg.m_ofn.nFilterIndex)
		{
		case 1:	// 根据字符串配置，默认第一个为XML
			break;
		case 2: // Lua
			break;
		}
	}
}

void CFlyAntsDlg::OnBtnSrcBrsw() 
{
	// TODO: Add your control notification handler code here
	CString strFilter = _T("");
	strFilter.LoadString(IDS_STR_OPEN_FILE_FILTER);

	CFileDialog saveSrcDlg(TRUE);
	SetFileDialogFilter(saveSrcDlg.m_ofn, strFilter);

	int nRet = saveSrcDlg.DoModal();
	if (IDOK == nRet)
	{
		SetDlgItemText(IDC_EDIT_SOURCE_FILE, saveSrcDlg.GetPathName());
		switch(saveSrcDlg.m_ofn.nFilterIndex)
		{
		case 1: // EXCEL
			break;
		case 2:
			break;
		}
	}
}

void CFlyAntsDlg::SetFileDialogFilter( OPENFILENAME& ofn, CString& strFilter )
{
	// 默认选中第一个文件过滤器
	ofn.nFilterIndex = 1;
	
	// Translate filter into commdlg format (lots of \0)
	if (!strFilter.IsEmpty())
	{
		LPTSTR pch = strFilter.GetBuffer(0); // modify the buffer in place
		// MFC delimits with '|' not '\0'
		while ((pch = _tcschr(pch, '|')) != NULL)
			*pch++ = '\0';
		ofn.lpstrFilter = strFilter;
		// do not call ReleaseBuffer() since the string contains '\0' characters
	}
}

void CFlyAntsDlg::OnBtnConvert() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	// 判断文件是否存在
	if (m_strOutputFilePath.IsEmpty() && m_strSourceFilePath.IsEmpty())
	{
		return;
	}

	if (NULL == m_pLuaEnv)
	{
		AfxMessageBox(_T("Lua 环境未能初始化"));
		return;
	}

	// 运行私有脚本格式化输出文件格式
#if 0
	if (luaL_loadstring(m_pLuaEnv, _T("print('hello')")) && lua_pcall(m_pLuaEnv, 0, 0, 0))
	{
		AfxMessageBox(lua_tostring(m_pLuaEnv, -1));
		return;
	}
#endif // 0

	CString strMsg = _T("转换完成，请检查对应输出文件");
	DoConvert(strMsg);
	AfxMessageBox(strMsg, MB_OK|MB_ICONINFORMATION);
}

BOOL CFlyAntsDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_pLuaEnv != NULL)
	{
		lua_close(m_pLuaEnv);
		m_pLuaEnv = NULL;
	}

#ifdef _DEBUG
	FreeConsole();
#endif // _DEBUG

	return CDialog::DestroyWindow();
}

void CFlyAntsDlg::OnMenuitemExit() 
{
	// TODO: Add your command handler code here
	SendMessage(WM_CLOSE);
}

bool CFlyAntsDlg::DoConvert( CString& strErrMsg )
{
	bool bConvert = false;
	// 转换输入文件数据到中间格式
	if (m_eIFT == DFT_EXCEL)
	{
		// 暂时交给Lua脚本处理

	}
	// 从中间数据格式转换到对应的输出文件的格式
	return true;
}
