// ObjectPropertyView.cpp : implementation file
//

#include "stdafx.h"
#include "mapeditor.h"
#include "ObjectPropertyView.h"
#include "MapEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectPropertyView

IMPLEMENT_DYNCREATE(CObjectPropertyView, EPropView)

CObjectPropertyView::CObjectPropertyView()
{
}

CObjectPropertyView::~CObjectPropertyView()
{
}


BEGIN_MESSAGE_MAP(CObjectPropertyView, EPropView)
	//{{AFX_MSG_MAP(CObjectPropertyView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectPropertyView drawing

void CObjectPropertyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CObjectPropertyView diagnostics

#ifdef _DEBUG
void CObjectPropertyView::AssertValid() const
{
	CView::AssertValid();
}

void CObjectPropertyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjectPropertyView message handlers

void CObjectPropertyView::OnInitialUpdate() 
{
	EPropView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	m_PropCtrl.EnableCommentPane(true);
	m_PropCtrl.SetPropertyHost(pDoc->m_pCurrentPropertyObj);

}

void CObjectPropertyView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	
}
