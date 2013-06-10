// FlyAnts.h : main header file for the FLYANTS application
//

#if !defined(AFX_FLYANTS_H__0F24A551_DAFE_4E10_85EF_43D723F4E112__INCLUDED_)
#define AFX_FLYANTS_H__0F24A551_DAFE_4E10_85EF_43D723F4E112__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFlyAntsApp:
// See FlyAnts.cpp for the implementation of this class
//

class CFlyAntsApp : public CWinApp
{
public:
	CFlyAntsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlyAntsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFlyAntsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLYANTS_H__0F24A551_DAFE_4E10_85EF_43D723F4E112__INCLUDED_)
