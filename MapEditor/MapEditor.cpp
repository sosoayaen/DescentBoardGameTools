// MapEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MapEditor.h"

#include "MainFrm.h"
#include "MapEditorDoc.h"
#include "MapEditorSplitterView.h"
#include "LeftFormView.h"

#include "FVersion.h"
#include "HyperLink.h"

#include "../zextract/zextract.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditorApp

BEGIN_MESSAGE_MAP(CMapEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CMapEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_BUTTON_SHOW_INFO_DLG, OnButtonShowInfoDialog)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
 	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditorApp construction

CMapEditorApp::CMapEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_LuaEnv = NULL;
//	m_bShowGrid = true;					// Show grid in default
	m_bListImageDragging = false;		// Dragging flag is false
	m_bIgnoreIamgeTransparent = true;	// Ignore Transparent (reserve, discard)
	m_bShowEveryMESelectionFrame = true;// Show every MAPELEMENT's frame rectangle when it was selected

	m_logManager.SetLogFileName(_T("debug_info.log"));
	m_logManager.DisableWrite2File();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMapEditorApp object

CMapEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMapEditorApp initialization

BOOL CMapEditorApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//////////////////////////////////////////////////////////////////////////
	// Set LUA environment
	InitLuaENV();
	
	InitLoadMapElementTypes();

	InitImageToMem();
	//////////////////////////////////////////////////////////////////////////

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMapEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMapEditorSplitterView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Set the program title
	CString strTitle;
	strTitle.LoadString(AFX_IDS_APP_TITLE);

	m_pMainWnd->SetWindowText(strTitle);

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_ctlEmail;
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
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_MAIL, m_ctlEmail);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMapEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CFVersionInfo version;
	if (version.Init())
	{
		m_strVersion.Format("Version: %s\nFileVersion: %s\nCopyright: %s",
			version.m_strProductVersion, version.m_strFileVersion, version.m_strLegalCopyright);
		UpdateData(FALSE);
	}

	// SetDlgItemText(IDC_STATIC_MAIL, _T("Email: sosoayaen@gmail.com"));
	m_ctlEmail.SetWindowText(_T("sosoayaen@gmail.com"));
	// m_ctlEmail.SetTextColor(RGB(0, 0, 255));
	m_ctlEmail.SetURL("mailto:sosoayaen@gmail.com");

#if WINVER >= 0x0500
	m_ctlEmail.SetLinkCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// void CAboutDlg::OnClickMailLink() 
// {
// 	// TODO: Add your control notification handler code here
// 	ShellExecute(NULL, NULL, _T("mailto:sosoayaen@163.net"), NULL, NULL, SW_SHOW);
// }

/////////////////////////////////////////////////////////////////////////////
// CMapEditorApp message handlers

int CMapEditorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_LuaEnv != NULL)
	{
		// Shutdown the LUA ENV
		lua_close(m_LuaEnv);
		m_LuaEnv = NULL;
	}

	// Free the MAPRESOURCE object
	for (MapID2MR::iterator mrIter = m_mapImgRes.begin();
		mrIter != m_mapImgRes.end();
		mrIter++)
	{
		if (mrIter->second)
		{
			if (mrIter->second)
			{
				if (mrIter->second->pImg)
				{
					delete mrIter->second->pImg;	// Free the CxImage object
				}
				delete mrIter->second;
			}
			
		}
	}

	return CWinApp::ExitInstance();
}

void CMapEditorApp::InitLuaENV()
{
	m_LuaEnv = luaL_newstate();
	if (m_LuaEnv == NULL)
	{
		AfxMessageBox(_T("Lua环境加载失败！"));
		m_logManager.Append(_T("Lua环境加载失败！"));
		lua_close(m_LuaEnv);
		m_LuaEnv = NULL;
	}
	else
	{
		// Open the Lua's libs
		luaL_openlibs(m_LuaEnv);
		
		if (luaL_loadfile(m_LuaEnv, _T("script\\t.lua")) || lua_pcall(m_LuaEnv, 0, 0, 0))
		{
			AfxMessageBox(lua_tostring(m_LuaEnv, -1));
			m_logManager.Append(lua_tostring(m_LuaEnv, -1));
			lua_close(m_LuaEnv);
			m_LuaEnv = NULL;
		}
	}
}

void CMapEditorApp::InitImageToMem()
{
	lua_State* L = m_LuaEnv;

	// Put Image list file into the memory
	if (L != NULL)
	{
		// Initial the type name to type ENUM map
		
		lua_getglobal(L, _T("MapTypeName2TypeValue"));
		if (lua_istable(L, -1))
		{
			int nTop = lua_gettop(L);
			lua_pushnil(L);
			while(lua_next(L, nTop))
			{
				m_mapTypeName2ENUM[lua_tostring(L, -2)] = (long)lua_tointeger(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1); // pop the MapTypeName2TypeValue table
		}

		lua_getglobal(L, _T("ImageFilePath"));
		if (lua_istable(L, -1))
		{
			int nAllTop = lua_gettop(L);
			lua_pushnil(L);
			while(lua_next(L, nAllTop))
			{
				// Table must be string-to-table
				if (lua_istable(L, -1))
				{
					int nElements = lua_gettop(L);
					lua_pushnil(L);
					while(lua_next(L, nElements))
					{
						if (lua_istable(L, -1))
						{
 							MAPRESOURCE* pIr = new MAPRESOURCE();

							lua_getfield(L, -1, _T("id")); pIr->nID = (long)lua_tointeger(L, -1); lua_pop(L, 1);
							lua_getfield(L, -1, _T("width")); pIr->nWidth = (long)lua_tointeger(L, -1); lua_pop(L, 1);
							lua_getfield(L, -1, _T("height")); pIr->nHeight = (long)lua_tointeger(L, -1); lua_pop(L, 1);
							lua_getfield(L, -1, _T("type")); pIr->nType = (ENUM_ME_TYPE)m_mapTypeName2ENUM[lua_tostring(L, -1)]; lua_pop(L, 1);
							
							std::string strPath;
							lua_getfield(L, -1, _T("path")); strPath = lua_tostring(L, -1); lua_pop(L, 1);
							std::string strName;
							lua_getfield(L, -1, _T("name")); strName = lua_tostring(L, -1); lua_pop(L, 1);
							std::string strDisplayName;
 							lua_getfield(L, -1, _T("displayName")); strDisplayName = lua_tostring(L, -1); lua_pop(L, 1);

							pIr->strName = strDisplayName;
#ifdef _DEBUG
							pIr->strPath = _T("../") + strPath + strName;
#else
							pIr->strPath = strPath + strName;
#endif // _DEBUG
							
							// 首先从文件中读取图片资源
							// Create the CxImage
							CxImage* pImg = new CxImage(pIr->strPath.c_str(), CXIMAGE_FORMAT_PNG);
							
							if (pImg && pImg->IsValid())
							{
								if (pImg->IsValid())
								{
									pIr->pImg = pImg;
								}
								else
								{
									delete pImg;
								}
							}
							else
							{
#ifdef _DEBUG
								pIr->strPath = strPath + strName;
#endif // _DEBUG
								// Create the CxImage from ZIP Archive
								CMemBuffer memBuff;
								int ret = GetFileInZip(memBuff, _T("images.dll"), pIr->strPath.c_str(), NULL);
								if (ret <= 0)
								{
									// read resource failed...
									TRACE("Get image from package:%s failed, nID:%d, path:%s\n", _T("images.dll"), pIr->nID, pIr->strPath.c_str());
									return;
								}
								pIr->pImg = new CxImage((BYTE *)memBuff.GetBuffer(), memBuff.GetBufferLen(), CXIMAGE_FORMAT_PNG);
							}
							if (!pIr->pImg->IsValid())
							{
								TRACE("Image is not valid, nID:%d, path:%s\n", pIr->nID, pIr->strPath.c_str());
								delete pIr->pImg;
							}
							m_mapImgRes[pIr->nID] = pIr;
						}
						lua_pop(L, 1);
					} // end the while(lua_next(L, nElements))
				}
				lua_pop(L, 1);
			} // end the while(lua_next(L, nAllTop))
			lua_pop(L, 1); // pop the ImageFilePath table
		}
	}
}

int CMapEditorApp::InitLoadMapElementTypes()
{
	if (m_LuaEnv != NULL)
	{
		lua_getglobal(m_LuaEnv, _T("MapTypeTbl"));
		if (lua_istable(m_LuaEnv, -1))
		{
			int nTop = lua_gettop(m_LuaEnv);
			lua_pushnil(m_LuaEnv);
			while(lua_next(m_LuaEnv, nTop))
			{
				std::string strTypeName = lua_tostring(m_LuaEnv, -1);
				m_vMET.push_back(strTypeName);
				lua_pop(m_LuaEnv, 1);
			}
			lua_pop(m_LuaEnv, 1);
		}
		return 0;
	}
	return -1;
}

bool CMapEditorApp::GetIgnoreImageTransparent()
{
	return m_bIgnoreIamgeTransparent;
}

void CMapEditorApp::SetIgnoreImageTransparent( bool bIgnore )
{
	m_bIgnoreIamgeTransparent = bIgnore;
}

void CMapEditorApp::OnButtonShowInfoDialog() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->ShowDebugDialog();
	}
}
