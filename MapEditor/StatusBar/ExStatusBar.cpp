// ExStatusBar.cpp: implementation of the CExStatusBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\resource.h"
#include "ExStatusBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CExStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CExStatusBar)
	ON_MESSAGE(WM_USER_STATUSBAR_MOUSE_POSTION_UPDATE, OnShowMousePosition)
	ON_MESSAGE(WM_USER_STATUSBAR_ZOOMED_RATE_UPDATE, OnShowZoomedRate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExStatusBar::CExStatusBar()
{

}

CExStatusBar::~CExStatusBar()
{

}

LRESULT CExStatusBar::OnShowMousePosition( WPARAM wParam, LPARAM lParam )
{
	int nIdx = CommandToIndex(IDS_STATUS_MOUSE_POS);
	if (nIdx >= 0)
	{
		char szMousePos[20] = {0};
		ZeroMemory(szMousePos, sizeof(szMousePos));
		sprintf(szMousePos, _T("% 7ld, %ld"), wParam, lParam);
		this->SetPaneText(nIdx, szMousePos);
		// TRACE("CExStatusBar::OnShowMousePosition x:%d, y:%d\n", wParam, lParam);
	}
	return TRUE;
}

void CExStatusBar::InitStatusBar()
{
	DWORD dwUP[] =
	{
		IDS_STATUS_MOUSE_POS,
	};
	int nCount = sizeof(dwUP) / sizeof(dwUP[0]);
	for (int i = 0; i < nCount; i++)
	{
		int nIndex = this->CommandToIndex(dwUP[i]);
		if ( nIndex >= 0 )
		{
			SetPaneStyle(nIndex, SBPS_NORMAL);
		}
	}
}

LRESULT CExStatusBar::OnShowZoomedRate( WPARAM wParam, LPARAM lParam )
{
	int nIdx = CommandToIndex(IDS_STATUS_ZOOMED_RATE);
	if (nIdx >= 0)
	{
		double* pRate = (double*)lParam;
		char szZoomedRate[10] = {0};
		ZeroMemory(szZoomedRate, sizeof(szZoomedRate));
		sprintf(szZoomedRate, _T("%.2f%%"), *pRate*100);
		this->SetPaneText(nIdx, szZoomedRate);
	}
	return TRUE;
}