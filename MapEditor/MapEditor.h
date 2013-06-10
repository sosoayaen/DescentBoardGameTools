// MapEditor.h : main header file for the MAPEDITOR application
//

#if !defined(AFX_MAPEDITOR_H__8B49E4FF_B8CD_467A_A36C_BE7E6221FE88__INCLUDED_)
#define AFX_MAPEDITOR_H__8B49E4FF_B8CD_467A_A36C_BE7E6221FE88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMapEditorApp:
// See MapEditor.cpp for the implementation of this class
//
#include "ximage.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "LogManager/LogManager.h" 

class CMapEditorApp : public CWinApp
{
public:
	lua_State* m_LuaEnv;	// Lua script runtime  environment

	VectorMapElementType m_vMET;	// Map element type vector
	MapID2MR m_mapImgRes;				// All resource structure
	MapName2ENUM m_mapTypeName2ENUM;	// MAPELEMENT TypeName to TypeValue
//	bool m_bShowGrid;					// Show the background grid and BG color

	bool m_bListImageDragging;			// Show if LeftFormView's Image is DRAGGING

	CMapEditorApp();

	CLogManager m_logManager;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMapEditorApp)
	afx_msg void OnAppAbout();
	afx_msg void OnButtonShowInfoDialog();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool GetIgnoreImageTransparent();
	void SetIgnoreImageTransparent(bool bIgnore);

protected:
	// Show the every MAPELEMENT's frame rectangle when they were selected
	bool m_bShowEveryMESelectionFrame;

private:
	// Ignore the image's transparent property 
	// when select the MAPELEMENT which covered by cursor
	bool m_bIgnoreIamgeTransparent;

	// Initialize the Lua environment
	void InitLuaENV();
	// Initialize the ComboBox content
	int InitLoadMapElementTypes();
	// Initialize the image map
	void InitImageToMem();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITOR_H__8B49E4FF_B8CD_467A_A36C_BE7E6221FE88__INCLUDED_)
