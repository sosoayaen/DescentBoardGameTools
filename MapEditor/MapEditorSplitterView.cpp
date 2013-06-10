// MapEditorSplitterView.cpp : implementation of the CMapEditorSplitterView class
//

#include "stdafx.h"
#include "MapEditor.h"

#include "MapEditorDoc.h"
#include "MapEditorSplitterView.h"
#include "CanvasScrollView\CanvasScrollView.h"
#include "LeftFormView.h"
#include "ObjectPropertyView.h"
#include "PropertyObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditorSplitterView

IMPLEMENT_DYNCREATE(CMapEditorSplitterView, CView)

BEGIN_MESSAGE_MAP(CMapEditorSplitterView, CView)
	//{{AFX_MSG_MAP(CMapEditorSplitterView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditorSplitterView construction/destruction

#define PROPERTY_VIEW_DEFAULT_WIDTH 300
#define LEFTFORMVIEW_DEFAULT_WIDTH 210

CMapEditorSplitterView::CMapEditorSplitterView()
{
	// TODO: add construction code here
	m_pWndSplitter = &m_wndSplitter;
	m_nPropertyViewWidth = PROPERTY_VIEW_DEFAULT_WIDTH;
}

CMapEditorSplitterView::~CMapEditorSplitterView()
{
	m_pWndSplitter = NULL;
}

BOOL CMapEditorSplitterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditorSplitterView drawing

void CMapEditorSplitterView::OnDraw(CDC* pDC)
{
	CMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditorSplitterView printing

BOOL CMapEditorSplitterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMapEditorSplitterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMapEditorSplitterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditorSplitterView diagnostics

#ifdef _DEBUG
void CMapEditorSplitterView::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditorSplitterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditorDoc* CMapEditorSplitterView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditorDoc)));
	return (CMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapEditorSplitterView message handlers

int CMapEditorSplitterView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	CCreateContext* pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;

	m_wndSplitter.CreateStatic(this, 1, 3);

	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftFormView), CSize(0, 0), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CCanvasScrollView), CSize(0, 0), pContext);
	m_wndSplitter.CreateView(0, 2, RUNTIME_CLASS(CObjectPropertyView), CSize(0, 0), pContext);

	// 设定Splitter窗口的大小
	m_wndSplitter.SetColumnInfo(0, LEFTFORMVIEW_DEFAULT_WIDTH, 0);
//	m_wndSplitter.SetColumnInfo(1, 800, 400);
	m_wndSplitter.SetColumnInfo(2, m_nPropertyViewWidth, 0);
	return 0;
}

void CMapEditorSplitterView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_wndSplitter.MoveWindow(0, 0, cx, cy);

	int nMiddleViewWidth = cx - LEFTFORMVIEW_DEFAULT_WIDTH - m_nPropertyViewWidth;
	if (nMiddleViewWidth < 0)
	{
		nMiddleViewWidth = 0;
	}
	m_wndSplitter.SetColumnInfo(1, nMiddleViewWidth, 0);

#ifdef _DEBUG
	int cxCur, cxMin = 0;
	m_wndSplitter.GetColumnInfo(2, cxCur, cxMin);
#endif // _DEBUG

	if (m_nPropertyViewWidth < 0)
	{
		m_nPropertyViewWidth = 0;
	}
	m_wndSplitter.SetColumnInfo(2, m_nPropertyViewWidth, 0);

	m_wndSplitter.RecalcLayout();
}

void CMapEditorSplitterView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	

	// TODO: Add your specialized code here and/or call the base class
// 	for(int i=0; i<2; i++)
// 	{
// 		CView* pView = (CView*)(m_wndSplitter.GetPane(0, i));
// 		pView->OnInitialUpdate();
// 	}
}

void CMapEditorSplitterView::TogglePropertyView()
{
	if (m_nPropertyViewWidth <= 0)
	{
		m_nPropertyViewWidth = PROPERTY_VIEW_DEFAULT_WIDTH;
	}
	else
	{
		m_nPropertyViewWidth = 0;
	}

	CRect rc;
	GetWindowRect(&rc);
	int nMiddleViewWidth = rc.Width() - LEFTFORMVIEW_DEFAULT_WIDTH - m_nPropertyViewWidth;
	if (nMiddleViewWidth < 0)
	{
		nMiddleViewWidth = 0;
	}

	m_wndSplitter.SetColumnInfo(1, nMiddleViewWidth, 0);
	m_wndSplitter.SetColumnInfo(2, m_nPropertyViewWidth, 0);
	
	m_wndSplitter.RecalcLayout();
}