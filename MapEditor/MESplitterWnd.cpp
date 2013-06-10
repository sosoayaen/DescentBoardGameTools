// MESplitterWnd.cpp: implementation of the CMESplitterWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapEditor.h"
#include "MESplitterWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMESplitterWnd, CSplitterWnd)

BEGIN_MESSAGE_MAP(CMESplitterWnd, CSplitterWnd)

	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()

END_MESSAGE_MAP()

CMESplitterWnd::CMESplitterWnd()
{

}

CMESplitterWnd::~CMESplitterWnd()
{

}

void CMESplitterWnd::OnLButtonDown( UINT nFlags, CPoint point )
{
// 	int p, m;
// 	GetColumnInfo(0, p, m);
// 	TRACE("pos:%d, min:%d\n", p, m);
// 	CSplitterWnd::OnLButtonDown(nFlags, point);
	return;
}

BOOL CMESplitterWnd::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
	return FALSE;
//	return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMESplitterWnd::OnMouseMove( UINT nFlags, CPoint point )
{
	CWnd::OnMouseMove(nFlags, point);
//	CSplitterWnd::OnMouseMove(nFlags, point);
}

void CMESplitterWnd::OnSize( UINT nType, int cx, int cy )
{
	TRACE("Main SplitterWnd's cx=%d and cy=%d\n", cx, cy);
// 	int nPosX = 0;
// 	int nMinX = 0;
// 	this->GetColumnInfo(0, nPosX, nMinX);
//	TRACE("ColumnInfo, nPosX:%d, nMinX:%d\n", nPosX, nMinX);

	CSplitterWnd::OnSize(nType, cx, cy);
}