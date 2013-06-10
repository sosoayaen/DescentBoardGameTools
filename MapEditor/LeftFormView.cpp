// LeftFormView.cpp : implementation file
//

#include "stdafx.h"
#include "MapEditor.h"
#include "LeftFormView.h"
#include "MapEditorDoc.h"
#include "CanvasScrollView/canvasscrollview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftFormView

IMPLEMENT_DYNCREATE(CLeftFormView, CFormView)

#define THUMBNIL_WIDTH 128
#define THUMBNIL_HEIGHT 128

bool CLeftFormView::m_bInitialed = false;

CLeftFormView::CLeftFormView()
	: CFormView(CLeftFormView::IDD)
{
	//{{AFX_DATA_INIT(CLeftFormView)
	//}}AFX_DATA_INIT

	m_pImageList = new CImageList();
	m_pImageList->Create(THUMBNIL_WIDTH, THUMBNIL_HEIGHT, ILC_COLOR32, 10, 1);

}

CLeftFormView::~CLeftFormView()
{
	CleanData();
	delete m_pImageList;
	m_pImageList = NULL;
}

void CLeftFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftFormView)
	DDX_Control(pDX, IDC_LIST_IMAGE, m_ctlThumbList);
	DDX_Control(pDX, IDC_COMBO_ITEMS_TYPE, m_comboItemType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftFormView, CFormView)
	//{{AFX_MSG_MAP(CLeftFormView)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEMS_TYPE, OnSelchangeComboItemsType)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_IMAGE, OnBegindragListImage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IMAGE, OnClickListImage)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftFormView diagnostics

#ifdef _DEBUG
void CLeftFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLeftFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftFormView message handlers

void CLeftFormView::OnSelchangeComboItemsType() 
{
	// TODO: Add your control notification handler code here
	int nSelIdx = m_comboItemType.GetCurSel();

#ifdef _DEBUG
	CString strComboSelectType;
	m_comboItemType.GetLBText(nSelIdx, strComboSelectType);
	TRACE("Selected Index:%d Text:%s\n", nSelIdx, strComboSelectType);
#endif // _DEBUG
	
	FillListCtrl((ENUM_ME_TYPE)nSelIdx);
}

void CLeftFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	// Call OnInitialUpdate function one time
	if (m_bInitialed)
	{
		return;
	}

	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	//////////////////////////////////////////////////////////////////////////
	// Add map editor types ComboBox content
	SetMapTypeComboBoxContent();

	//////////////////////////////////////////////////////////////////////////
	// Add image to the m_pImageList
	InitImageList();

	//////////////////////////////////////////////////////////////////////////
	// Select Default Item
	if (m_comboItemType.GetCount() > 0)
	{
		m_comboItemType.SetCurSel(0);
		// Call the ComboBox select function
		OnSelchangeComboItemsType();
	}
	m_bInitialed = true;
}

void CLeftFormView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	TRACE("LeftFormView nType:%d, cx:%d, cy:%d\n", nType, cx, cy);
	// TODO: Add your message handler code here

	if (::IsWindow(m_ctlThumbList.GetSafeHwnd()))
	{
		CRect listRect;
		CRect leftViewRect;

		GetWindowRect(&leftViewRect);
		m_ctlThumbList.GetWindowRect(&listRect);

 		listRect.bottom = leftViewRect.bottom;
		ScreenToClient(&listRect);

		m_ctlThumbList.MoveWindow(&listRect);	
	}

	ShowScrollBar(SB_BOTH, FALSE);
}

void CLeftFormView::OnBegindragListImage(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	// Set the Dragging state
	pApp->m_bListImageDragging = true;
	m_bDragging = true;
	
	// Get Dragging Item in the ListCtrl
	LV_ITEM lvi;
	ZeroMemory(&lvi, sizeof(LV_ITEM));
	lvi.pszText = new char[MAX_PATH];
	lvi.cchTextMax = MAX_PATH - 1;
	lvi.iItem = pNMListView->iItem;
	lvi.mask = LVIF_PARAM | LVIF_IMAGE | LVIF_TEXT;
	m_ctlThumbList.GetItem(&lvi);
	THUMBITEM* pTI = (THUMBITEM*)lvi.lParam;
	
	// Get the dragging image index
	int nImgIdx = pTI->nImageIndex;

	// Draw the dragging image in the MainFrame
	CWnd* pDragOwnerWnd = AfxGetMainWnd();
	ASSERT(::IsWindow(pDragOwnerWnd->GetSafeHwnd()));

	m_pImageList->BeginDrag(nImgIdx, CPoint(THUMBNIL_WIDTH/2, 0));
	m_pImageList->DragEnter(pDragOwnerWnd, pNMListView->ptAction);
	
	TRACE("Dragged index:%d, ImageIdx:%d\n", pNMListView->iItem, nImgIdx);
	delete lvi.pszText;

	// Set the Dragging MAPELEMENT ID
	// When Drop in to the canvas we will add the ME into the Map content
	pDoc->m_nDraggingMEID = pTI->nID;

	SetCapture();
	*pResult = 0;
}

void CLeftFormView::OnClickListImage(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	TRACE("OnClickListImage\n");
	*pResult = 0;
}

void CLeftFormView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
	
	if (m_bDragging)
	{
		CPoint ptMove = point;
		ClientToScreen(&ptMove);
		CWnd* pWnd = AfxGetMainWnd();
		if (pWnd)
		{	
			pWnd->ScreenToClient(&ptMove);
		}
		m_pImageList->DragMove(ptMove);
	}
	CFormView::OnMouseMove(nFlags, point);
}

void CLeftFormView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();

	TRACE("CLeftFormView::OnLButtonUp\n");
	if (m_bDragging)
	{
		m_bDragging = false;
		CWnd* pMainFrameWnd = AfxGetMainWnd();
		ASSERT(pMainFrameWnd);
		
		m_pImageList->DragLeave(pMainFrameWnd);
		m_pImageList->EndDrag();
		ReleaseCapture();

		CPoint dropPoint = point;
		ClientToScreen(&dropPoint);

		CWnd* pWnd = WindowFromPoint(dropPoint);
		ASSERT(pWnd != NULL);

		// Judge the CWnd hovered by cursor is the CanvasView
		if (!pWnd->IsKindOf(RUNTIME_CLASS(CCanvasScrollView)))
		{
			pApp->m_bListImageDragging = false;
		}
		else
		{
			pWnd->ScreenToClient(&dropPoint);
			pWnd->PostMessage(WM_LBUTTONUP, (WPARAM)nFlags, MAKELPARAM(dropPoint.x, dropPoint.y));
		}
	}

	CFormView::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
// CLeftFormView protected function implement

int CLeftFormView::SetMapTypeComboBoxContent()
{
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();

	if (pApp != NULL && pApp->m_vMET.size() > 0)
	{
		// Clean the ComboBox's content
		m_comboItemType.ResetContent();
		for (VectorMapElementType::iterator iter = pApp->m_vMET.begin();
			iter != pApp->m_vMET.end();
			iter++)
		{
			m_comboItemType.AddString(iter->c_str());
		}
		return 0;
	}
	return -1;
}

int CLeftFormView::InitImageList()
{
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	MapID2MR* pMapImgRes = &pApp->m_mapImgRes;

	// Set the ImageList to the CListCtrl
	m_ctlThumbList.SetImageList(m_pImageList, LVSIL_NORMAL);

	for (MapID2MR::iterator iter = pMapImgRes->begin();
	iter != pMapImgRes->end();
	iter++)
	{
		long nType = (long)iter->second->nType;
		long nID = iter->second->nID;

#ifdef _DEBUG
		ENUM_ME_TYPE type = iter->second->nType;
		std::string strPath = iter->second->strPath;
#endif // _DEBUG
		
		CxImage* pImg = (*pMapImgRes)[nID]->pImg;

		CClientDC tDC(this);

		// Set the memory DC to paint the stretch BMP
		CDC MemDC;
		MemDC.CreateCompatibleDC(NULL);

		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&tDC, THUMBNIL_WIDTH, THUMBNIL_WIDTH);

		CBitmap* pOldBitmap = MemDC.SelectObject(&bitmap);

		MemDC.FillSolidRect(0, 0, THUMBNIL_WIDTH, THUMBNIL_WIDTH, RGB(255, 255, 255));

		// Draw area
		int nWidth = 0;
		int nHeight = 0;
		int x = 0;
		int y = 0;

		// Zoom rate
		float fRate = 1.0*pImg->GetHeight()/pImg->GetWidth();

		// Calculate the draw area for zoom out
		if (pImg->GetHeight() <= THUMBNIL_HEIGHT &&
			pImg->GetWidth() <= THUMBNIL_WIDTH)
		{
			x = (THUMBNIL_WIDTH - pImg->GetWidth())/2;
			y = (THUMBNIL_HEIGHT - pImg->GetHeight())/2;
			nWidth = pImg->GetWidth();
			nHeight = pImg->GetHeight();
		}
		else if (fRate > 1.0)
		{
			nWidth = THUMBNIL_HEIGHT/fRate;
			nHeight = THUMBNIL_HEIGHT;
			x = (THUMBNIL_WIDTH-nWidth)/2;
			y = 0;
		}
		else
		{
			nWidth = THUMBNIL_WIDTH;
			nHeight = THUMBNIL_WIDTH*fRate;
			x = 0;
			y = (THUMBNIL_HEIGHT-nHeight)/2;
		}

		pImg->Draw(MemDC.m_hDC, x, y, nWidth, nHeight);
		
		// Release the bitmap
		MemDC.SelectObject(pOldBitmap);

		// pImgList->Add(&bitmap, RGB(0, 0, 0));
		int nImgIdx = m_pImageList->Add(&bitmap, RGB(0, 0, 0));

		bitmap.DeleteObject();
		MemDC.DeleteDC();

		// Create the DataSource of the CListCtrl
		VectorThumbItem* pVTI = NULL;
		MapType2VTI::iterator iter_find = m_mapType2ThumbItemList.find(nType);
		if (iter_find == m_mapType2ThumbItemList.end())
		{
			// Not find the type list and CREATE one which add to the MapType2VTI
			pVTI = new VectorThumbItem();
			m_mapType2ThumbItemList[nType] = pVTI;
		}
		else
		{
			// Get the exist VectorThumbItem object
			pVTI = iter_find->second;
		}
		
		// Add a new THUMBITEM
		pVTI->push_back(new THUMBITEM(nID, nImgIdx, iter->second->strName.c_str()));
	}

	return 0;
}

int CLeftFormView::FillListCtrl( ENUM_ME_TYPE nType )
{
	VectorThumbItem* pVIT = NULL;
	MapType2VTI::iterator iter_find = m_mapType2ThumbItemList.find((long)nType);
	if (iter_find != m_mapType2ThumbItemList.end())
	{
		pVIT = iter_find->second;
	}
	else
	{
		// Not found the VectorThrumbItem
		ASSERT(0);
		return -1;
	}

	ASSERT(pVIT != NULL);

	// 操作对应的ImageList
/*
	if (pImgList != NULL)
	{
		switch (nType)
		{
		case ME_TYPE_TILE:
			break;
		case ME_TYPE_DOOR:
			break;
		case ME_TYPE_OBSTACLE:
			break;
		case ME_TYPE_OBJECT:
			break;
		case ME_TYPE_MONSTER:
			break;
		case ME_TYPE_MONSTER_MASTER:
			break;
		}
	}
*/
	// Rebuild the ThumbList
	m_ctlThumbList.DeleteAllItems();

	m_ctlThumbList.SetRedraw(FALSE);

	int nIndex = 0;
	for (VectorThumbItem::iterator iter_ti = pVIT->begin();
	iter_ti != pVIT->end();
	iter_ti++, nIndex++)
	{
		LV_ITEM lvi;
		ZeroMemory(&lvi, sizeof(LV_ITEM));

		lvi.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;	// Show IMAGE and TEXT
		lvi.iItem = nIndex;
		lvi.pszText = (LPSTR)(LPTSTR)(*iter_ti)->strDisplayText.c_str();
		lvi.iImage = (*iter_ti)->nImageIndex;
		lvi.lParam = (LPARAM)(*iter_ti);	// Bind the item and DataSource
		
		m_ctlThumbList.InsertItem(&lvi);
	}
	m_ctlThumbList.SetRedraw();
	return 0;
}

void CLeftFormView::CleanData()
{
	for (MapType2ImgList::iterator iter_imglist = m_mapType2Imglist.begin();
	iter_imglist != m_mapType2Imglist.end();
	iter_imglist++)
	{
		// Delete the CImageList object
		if (iter_imglist->second)
		{
			delete iter_imglist->second;
			iter_imglist->second = NULL;
		}
	}

	for (MapType2VTI::iterator iter_vti = m_mapType2ThumbItemList.begin();
	iter_vti != m_mapType2ThumbItemList.end();
	iter_vti++)
	{
		for (VectorThumbItem::iterator iter_ti = iter_vti->second->begin();
		iter_ti != iter_vti->second->end();
		iter_ti++)
		{
			delete *iter_ti;
			*iter_ti = NULL;
		}
		delete iter_vti->second;
		iter_vti->second = NULL;
	}
}
