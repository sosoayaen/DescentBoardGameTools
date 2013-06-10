// CanvasScrollView.cpp : implementation file
//

#include "stdafx.h"
#include "..\\MapEditor.h"
#include "..\\MapEditorDoc.h"
#include "CanvasScrollView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanvasScrollView

IMPLEMENT_DYNCREATE(CCanvasScrollView, CScrollView)

CCanvasScrollView::CCanvasScrollView()
{
	HINSTANCE pAppInstance = AfxGetInstanceHandle();
	m_hCurHand = LoadCursor(pAppInstance, MAKEINTRESOURCE(IDC_CURSOR_HAND));
	m_hCurDragging = LoadCursor(pAppInstance, MAKEINTRESOURCE(IDC_CURSOR_DRAGGING));
	m_hCurRectangle = LoadCursor(pAppInstance, MAKEINTRESOURCE(IDC_CURSOR_RECTANGLE));
	m_hCurArrow = LoadCursor(NULL, IDC_ARROW);

	m_hCursor = 0;
	m_hLastCursor = 0;
	m_ptStartPoint.x = 0;
	m_ptStartPoint.y = 0;

	m_bLBtnDown = FALSE;
	m_bIsPaintSelectionRect = FALSE;
	m_bViewSizeChanged = FALSE;
	m_bDragging = FALSE;
	m_bDrawSelectedMapElementInTopLayer = FALSE;

	m_pBitmap = new CBitmap();
	m_memDC.CreateCompatibleDC(NULL);
}

CCanvasScrollView::~CCanvasScrollView()
{
	m_pBitmap->DeleteObject();
	delete m_pBitmap;
	m_memDC.DeleteDC();
}

BEGIN_MESSAGE_MAP(CCanvasScrollView, CScrollView)
	//{{AFX_MSG_MAP(CCanvasScrollView)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EDIT_ORA_SIZE, OnCanvasRecoverSize)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_BUTTON_SHOW_GRID, OnButtonShowGrid)
	ON_COMMAND(ID_BUTTON_ROTATE_LEFT_90, OnButtonRotateLeft90)
	ON_COMMAND(ID_BUTTON_ROTATE_RIGHT_90, OnButtonRotateRight90)
	ON_COMMAND(ID_BUTTON_ZOOMIN, OnButtonZoomin)
	ON_COMMAND(ID_BUTTON_ZOOMOUT, OnButtonZoomout)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_BACKGROUND_COLOR, OnButtonBackgroundColor)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ROTATE_LEFT_90, OnUpdateButtonRotateLeft90)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ROTATE_RIGHT_90, OnUpdateButtonRotateRight90)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_BUTTON_SELECT_MODE, OnButtonSelectMode)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SELECT_MODE, OnUpdateButtonSelectMode)
	ON_COMMAND(ID_BUTTON_MOVE_CANVAS, OnButtonMoveCanvas)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MOVE_CANVAS, OnUpdateButtonMoveCanvas)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_GRID, OnUpdateButtonShowGrid)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanvasScrollView drawing

/////////////////////////////////////////////////////////////////////////////
// CCanvasScrollView diagnostics

#ifdef _DEBUG
void CCanvasScrollView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCanvasScrollView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCanvasScrollView message handlers

//////////////////////////////////////////////////////////////////////////
// private implement

void CCanvasScrollView::ZoomOut()
{
	Zoom(false);
}

void CCanvasScrollView::ZoomIn()
{
	Zoom();
}

void CCanvasScrollView::DrawBackground( CDC* pDC )
{
//	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	
	CRect canvasRect;
	GetClientRect(&canvasRect);
	
	//先用背景色将位图清除干净
	pDC->FillSolidRect(&canvasRect, pDoc->GetCanvasGBColor());
	if (pDoc && pDoc->m_bShowGrid)
	{
		// 绘制背景网格
		CPen _gridPen(pDoc->GetCanvasMapGridLineStyle(), pDoc->GetCanvasMapGridLineWeight(), pDoc->GetCanvasGLColor());
		
		CPen* pOldPen = pDC->SelectObject(&_gridPen);
		
		// 通过网格的宽高来绘制
		int nxCounts = canvasRect.Width() / pDoc->GetMapGridWidth();
		int nyCounts = canvasRect.Height() / pDoc->GetMapGridHeight();
		
		// 首先确定第一根线的位置
		int nStartX = pDoc->GetMapOffsetX();
		if (abs(pDoc->GetMapOffsetX()) > pDoc->GetMapGridWidth())
		{
			// 偏移大于长度元单位，偏移值为当前地图单元对偏移取余
			nStartX = pDoc->GetMapOffsetX() % pDoc->GetMapGridWidth();
		}
		int nStartY = pDoc->GetMapOffsetY();
		if (abs(pDoc->GetMapOffsetY()) > pDoc->GetMapGridHeight())
		{
			// 偏移大于长度元单位，偏移值为当前地图单元对偏移取余
			nStartY = pDoc->GetMapOffsetY() % pDoc->GetMapGridHeight();
		}

		// 绘制竖线，加上整个地图的偏移
		for (int idx_x=0; idx_x<=nxCounts; idx_x++)
		{
			int nOffset = idx_x*pDoc->GetMapGridWidth() - 1;
			int nFixedX = (nOffset>0 ? nOffset : 0) + nStartX;
			pDC->MoveTo(nFixedX, 0);
			pDC->LineTo(nFixedX, canvasRect.bottom);
		}
		
		// 绘制横线，加上整个地图的偏移
		for (int idx_y=0; idx_y<=nyCounts; idx_y++)
		{	
			int nOffset = idx_y*pDoc->GetMapGridHeight() - 1;
			int nFixedY = (nOffset>0 ? nOffset : 0) + nStartY;
			pDC->MoveTo(0, nFixedY);
			pDC->LineTo(canvasRect.right, nFixedY);
		}
		
		pDC->SelectObject(pOldPen);
	}
}

void CCanvasScrollView::DrawDraggingMapElements( CDC* pDC )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	if (pDoc->GetSelectedMapElementCount()>0)
	{
		double dZoom = pDoc->GetMapZoom();
		for (VectorMapElementLayerList::iterator iter_layer = pDoc->m_vSelMELayers.begin();
		iter_layer != pDoc->m_vSelMELayers.end();
		iter_layer++)
		{
			DrawSelMapLayer(pDC, *iter_layer);
		}
	}
}

void CCanvasScrollView::DrawSelMapLayer( CDC* pDC, VectorMapElement* pVME )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
//	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();

	double dZoom = pDoc->GetMapZoom();
	int nOverlap = pDoc->GetMapGridDefaultWidth() * dZoom;

	// Get the view's rectangle
	CRect rView;
	GetClientRect(&rView);

	long nOffX = pDoc->GetMapOffsetX();
	long nOffY = pDoc->GetMapOffsetY();

	for (VectorMapElement::iterator iter=pVME->begin(); iter!=pVME->end(); iter++)
	{
		// 判断当前图层是否需要绘制
		if (!pDoc->GetMapLayerVisible((*iter)->nType))
		{
			continue;
		}

		// 如果是 Tile，则相接壤的 Tiles 之间需要覆盖
		if ((*iter)->nType != ME_TYPE_TILE)
		{
			nOverlap = 0;
		}

		int nX = int(((*iter)->nPx-1)*pDoc->GetMapGridWidth() - nOverlap + 0.5) + nOffX;
		int nY = int(((*iter)->nPy-1)*pDoc->GetMapGridHeight() - nOverlap + 0.5) + nOffY;
		
		// Copy a new CxImage object to draw
		CxImage* pImg = new CxImage();
		pImg->Copy(*(*iter)->pImg);

		//////////////////////////////////////////////////////////////////////////
		// ! Important ! It should be 
		// Rotate first
		switch ((*iter)->nAngle)
		{
		case 90:	// right
			pImg->RotateRight();
			break;
		case 180: // 180
			pImg->Rotate180();
			break;
		case 270: // left
			pImg->RotateLeft();
			break;
		default:
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		// Update the ME's width and height
		// It should be update when rotate the ME, not here
		(*iter)->nWidth = pImg->GetWidth();
		(*iter)->nHeight = pImg->GetHeight();

		CRect rZoomed;
		rZoomed.SetRect(nX, nY, nX + pImg->GetWidth()*dZoom, nY + pImg->GetHeight()*dZoom);
		// Do not draw the image if it's rectangle is out of the view screen
		CRect rIntersect;
		rIntersect.IntersectRect(&rZoomed, &rView);
		if (rIntersect.IsRectEmpty())
		{
			//////////////////////////////////////////////////////////////////////////
			// IMPORTANT!!!
			// !!!Delete the new CxImage object to prevent memory LEAKING!!!
			delete pImg;
			//////////////////////////////////////////////////////////////////////////

			continue;
		}

		pImg->Draw(pDC->GetSafeHdc(), 
			nX,
			nY,
			pImg->GetWidth()*dZoom,
			pImg->GetHeight()*dZoom,
			NULL,
			false);

		// Show the outline which ME is selected
		if (pDoc->IsShowEveryMESelectionFrame() && (*iter)->bSelected)
		{
			int nSavedDC = pDC->SaveDC();

			CPen pen(PS_DASHDOTDOT, 1, pDoc->GetMESelectionFrameColor());	// red
			
			CPen* pOldPen = pDC->SelectObject(&pen);
			
			int nLeft = nX;
			int nRight = pImg->GetWidth()*dZoom + nX;
			int nTop = nY;
			int nBottom = pImg->GetHeight()*dZoom + nY;

			// Make a special rectangle if ME is tile
			if ((*iter)->nType == ME_TYPE_TILE)
			{
				nLeft += nOverlap;
				nTop += nOverlap;
				nRight -= nOverlap;
				nBottom -= nOverlap;
			}

			// Set the zoomed ME's rectangle
			CRect rect(nLeft, nTop, nRight, nBottom);
			// Draw the outline
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->Rectangle(&rect);

			pDC->SelectObject(pOldPen);

			pDC->RestoreDC(nSavedDC);
		}

		// Delete the CxImage object just created
		delete pImg;
	}
}

void CCanvasScrollView::DrawMapLayer( CDC* pDC, VectorMapElement* pVME, bool bSavePicture /*= false*/, double dZoom /*= 0.0f*/, int nSPOffX /*= 0*/, int nSPOffY /*= 0*/ )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
//	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();

	if (dZoom == 0.0f)
	{
		dZoom = pDoc->GetMapZoom();
	}
	int nOverlap = pDoc->GetMapGridDefaultWidth() * dZoom;

	long nOffX = pDoc->GetMapOffsetX();
	long nOffY = pDoc->GetMapOffsetY();

	if (bSavePicture)
	{
		// 在出图的过程中，直接使用传入的位移量
		nOffX = nSPOffX;
		nOffY = nSPOffY;
	}

	for (VectorMapElement::iterator iter=pVME->begin(); iter!=pVME->end(); iter++)
	{
		// 判断当前图层是否需要绘制
		if (!pDoc->GetMapLayerVisible((*iter)->nType))
		{
			continue;
		}
		
		// If is dragged then do not draw the ME
		// Leave it to the dragging draw function
		if (m_bDrawSelectedMapElementInTopLayer && (*iter)->bSelected)
		{
			continue;
		}
		
		// 如果是 Tile，则相接壤的 Tiles 之间需要覆盖
		if ((*iter)->nType != ME_TYPE_TILE)
		{
			nOverlap = 0;
		}

		// 计算位置需要加上偏移量
		int nX = int(((*iter)->nPx-1)*pDoc->GetMapGridWidth() - nOverlap + 0.5) + nOffX;
		int nY = int(((*iter)->nPy-1)*pDoc->GetMapGridHeight() - nOverlap + 0.5) + nOffY;
		
		// Copy a new CxImage object to draw
		CxImage* pImg = new CxImage();
		pImg->Copy(*(*iter)->pImg);

		//////////////////////////////////////////////////////////////////////////
		// ! Important ! It should be 
		// Rotate first
		switch ((*iter)->nAngle)
		{
		case 90:	// right
			pImg->RotateRight();
			break;
		case 180: // 180
			pImg->Rotate180();
			break;
		case 270: // left
			pImg->RotateLeft();
			break;
		default:
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		// Update the ME's width and height
		// It should be update when rotate the ME, not here
		(*iter)->nWidth = pImg->GetWidth();
		(*iter)->nHeight = pImg->GetHeight();

		if (!bSavePicture)
		{
			// 加快绘制速度，默认加速。如果用于图片输出，则不能启用流程
			// Get the view's rectangle
			CRect rView;
			GetClientRect(&rView);

			CRect rZoomed;
			rZoomed.SetRect(nX, nY, nX + pImg->GetWidth()*dZoom, nY + pImg->GetHeight()*dZoom);
			// Do not draw the image if it's rectangle is out of the view screen
			CRect rIntersect;
			rIntersect.IntersectRect(&rZoomed, &rView);
			if (rIntersect.IsRectEmpty())
			{
				//////////////////////////////////////////////////////////////////////////
				// IMPORTANT!!!
				// !!!Delete the new CxImage object to prevent memory LEAKING!!!
				delete pImg;
				//////////////////////////////////////////////////////////////////////////
				TRACE("SKIP the image which is out of the view rectangle!\n");
				continue;
			}
		}

		pImg->Draw(pDC->GetSafeHdc(), 
			nX,
			nY,
			pImg->GetWidth()*dZoom,
			pImg->GetHeight()*dZoom,
			NULL,
			false);

		// Show the outline which ME is selected
		if (!bSavePicture && pDoc->IsShowEveryMESelectionFrame() && (*iter)->bSelected)
		{
			int nSavedDC = pDC->SaveDC();

			CPen pen(PS_DASHDOTDOT, 1, pDoc->GetMESelectionFrameColor());	// red
			
			CPen* pOldPen = pDC->SelectObject(&pen);
			
			int nLeft = nX;
			int nRight = pImg->GetWidth()*dZoom + nX;
			int nTop = nY;
			int nBottom = pImg->GetHeight()*dZoom + nY;

			// Make a special rectangle if ME is tile
			if ((*iter)->nType == ME_TYPE_TILE)
			{
				nLeft += nOverlap;
				nTop += nOverlap;
				nRight -= nOverlap;
				nBottom -= nOverlap;
			}

			// Set the zoomed ME's rectangle
			CRect rect(nLeft, nTop, nRight, nBottom);
			// Draw the outline
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->Rectangle(&rect);

			pDC->SelectObject(pOldPen);

			pDC->RestoreDC(nSavedDC);
		}

		// Delete the CxImage object just created
		delete pImg;
	}
}

void CCanvasScrollView::DrawMap( CDC* pDC, bool bSavePicture /*= false*/, double dZoom /*= 0.0f*/, int nOffX /*= 0*/, int nOffY /*= 0*/ )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	// Get the zoom rate first
//	double dZoom = pDoc->GetMapZoom();

	// 这里可以考虑把对应的绘制层放在 Vector 中，循环调用
// 	DrawMapLayer(pDC, &pDoc->m_vMapTiles, dZoom);		// 绘制地板（这里有优化的空间，可以单独给地板一个绘制的函数）
// 	DrawMapLayer(pDC, &pDoc->m_vMapDoors, dZoom);			// 绘制门
// 	DrawMapLayer(pDC, &pDoc->m_vMapObstacles, dZoom);		// 绘制障碍物
// 	DrawMapLayer(pDC, &pDoc->m_vMapObjects, dZoom);			// 绘制杂物
// 	DrawMapLayer(pDC, &pDoc->m_vMapTreasure, dZoom);		// 绘制宝物
// 	DrawMapLayer(pDC, &pDoc->m_vMapMonsters, dZoom);		// 绘制怪物
// 	DrawMapLayer(pDC, &pDoc->m_vMapMasters, dZoom);			// 绘制精英怪物
	for (VectorMapElementLayerList::iterator iter_layer = pDoc->m_pvMELayers->begin();
	iter_layer != pDoc->m_pvMELayers->end();
	iter_layer++)
	{
		if ((*iter_layer)->size() > 0)
		{
			DrawMapLayer(pDC, *iter_layer, bSavePicture, dZoom, nOffX, nOffY);
		}
	}
}

void CCanvasScrollView::DrawSelectedMapElementFrameRectangle( CDC* pDC )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	if (pDoc->GetSelectedMapElementCount() > 0)
	{
		// Get the frame RECT
		CRect rFrame;
		pDoc->GetSelectedMapElmentRect(rFrame);
		
		int nSavedDC = pDC->SaveDC();
		
		pDC->SelectStockObject(NULL_BRUSH);
		CPen pen(PS_SOLID, 2, RGB(200, 64, 200)); // highlight purple
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->Rectangle(&rFrame);
		
		pDC->SelectObject(pOldPen);
		pDC->RestoreDC(nSavedDC);
	}
}

void CCanvasScrollView::DrawCursorSelectedRectangle( CDC* pDC )
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	// Draw frame rectangle
	CPen pen(PS_DOT, 1, RGB(255, 0, 0));	// red

	int nSavedDC = pDC->SaveDC();

	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SelectObject(&pen);
	pDC->Rectangle(m_ptStartPoint.x, m_ptStartPoint.y,
		point.x, point.y);

	pDC->RestoreDC(nSavedDC);
}

void CCanvasScrollView::Zoom( bool bZoomedOut /*= true*/, int nZoomedStep /*= 16 */ )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	
	CRect canvasRect;
	this->GetWindowRect(&canvasRect);
	ScreenToClient(canvasRect);
	
	// Get current grid's width and height
	int nGridWidth = pDoc->GetMapGridWidth();
	int nGridHeight = pDoc->GetMapGridHeight();
	
	// Get current grid line weight
	int nGridLineWeight = pDoc->GetCanvasMapGridLineWeight();
	
	// The nScaleRate indicates the Mouse Wheel's scale rate
	// it is valued 1 here
	int nScaleRateX = nGridWidth / nZoomedStep;
	int nScaleRateY = nGridHeight / nZoomedStep;
	
	nScaleRateX = nScaleRateX<=0 ? 1 : nScaleRateX;
	nScaleRateY = nScaleRateY<=0 ? 1 : nScaleRateY;
	
	//	TRACE("nScaleRateX:%d, nScaleRateY:%d\n", nScaleRateX, nScaleRateY);
	
	if (bZoomedOut)
	{
		nScaleRateX *= 1;
		nScaleRateY *= 1;
	}
	else
	{
		nScaleRateX *= -1;
		nScaleRateY *= -1;
	}
	
	TRACE("canvasRect.Width():%d, nGridWidth:%d, canvasRect.Height():%d, nGridHeight:%d\n",
		canvasRect.Width(), nGridWidth, canvasRect.Height(), nGridHeight);
	// Zoom the grid
	// 放大缩小网格
	if ((canvasRect.Width()>nGridWidth && canvasRect.Height()>nGridHeight)
		|| !bZoomedOut)
	{
		pDoc->SetMapGridHeight(nGridHeight + nScaleRateX);
		pDoc->SetMapGridWidth(nGridWidth + nScaleRateY);
	}
	
	// Set Zoom
	pDoc->SetMapZoom(pDoc->GetMapGridWidth()*1.0/pDoc->GetMapGridDefaultWidth());
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd)
	{
		pWnd->PostMessage(WM_USER_STATUSBAR_ZOOMED_RATE_UPDATE);
	}
	
	// Redraw the background
	// 重绘背景
	if (nGridWidth > 2*nGridLineWeight  && nGridHeight > 2*nGridLineWeight
		&& canvasRect.Width() >= nGridWidth && canvasRect.Height() >= nGridHeight)
	{
		Invalidate(FALSE);
	}
}

MAPELEMENT* CCanvasScrollView::GetMapElementFromPoint( CPoint point )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();

	bool bFind = false;

	// 获得地图当前的缩放比
	double dZoom = pDoc->GetMapZoom();
	
	VectorMapElement::reverse_iterator r_elementIter;

	// 确认对应的图层中的Element是否在鼠标区域内，按图层的顺序，都上到下判断
	for (VectorMapElementLayerList::reverse_iterator r_layerIter = pDoc->m_pvMELayers->rbegin();
	r_layerIter < pDoc->m_pvMELayers->rend() && !bFind;
	r_layerIter++)
	{
		// 循环每个图层中的元素是否在鼠标范围内响应
		for (r_elementIter = (*r_layerIter)->rbegin();
		r_elementIter < (*r_layerIter)->rend();
		r_elementIter++)
		{
			// Draw position: TopLeft
			long nLeft = ((*r_elementIter)->nPx - 1) * pDoc->GetMapGridWidth();
			long nTop = ((*r_elementIter)->nPy - 1) * pDoc->GetMapGridHeight();
			
			long nWidth = long((*r_elementIter)->nWidth*dZoom + 0.5);
			long nHeight = long((*r_elementIter)->nHeight*dZoom + 0.5);
			
			long nRight = nWidth + nLeft;
			long nBottom = nHeight + nTop;
			
			// Select the showed area when object is tile
			// if (*r_layerIter == &pDoc->m_vMapTiles)
			if((*r_elementIter)->nType == ME_TYPE_TILE)
			{
				// Tile's real width or height should be reduced 2 blocks
				nRight -= pDoc->GetMapGridWidth()*2;
 				nBottom -= pDoc->GetMapGridHeight()*2;
			}
			
			CRect rect(nLeft, nTop, nRight, nBottom);
			// 元件的位置都需要先加上偏移量
			rect.OffsetRect(pDoc->GetMapOffsetX(), pDoc->GetMapOffsetY());

			if (rect.PtInRect(point))
			{
				bFind = true;
				break;
			}
		}
	}

	if (bFind)
	{
		// Return the found ME's point
		return *r_elementIter;
	}
	else
	{
		return NULL;
	}
}

void CCanvasScrollView::RotateSelectedMapElements( short nAngle )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	// Record the TopLeft
	CRect rSelected;
	pDoc->GetSelectedMapElmentRect(rSelected);
	// 这里因为需要计算元件的位置，所以还是要移除偏移量，然后再计算
	// 绘制的时候会根据偏移量来确定最新位置，所以元件的位置需要在未偏移的坐标系下计算
	rSelected.OffsetRect(-pDoc->GetMapOffsetX(), -pDoc->GetMapOffsetY());

	CPoint ptSelectedFrameTopLeft = rSelected.TopLeft();
	
	//////////////////////////////////////////////////////////////////////////
	// Get center point to confirm the coordinates
	CPoint centerPoint = rSelected.CenterPoint();

	// Get substitution top-left point of selected rectangle
	CPoint ptTopLeftSubstitution = RotatedTopLeftSubstitution(nAngle, rSelected);
	// ptTopLeftSubstitution.Offset(pDoc->GetMapOffsetX(), pDoc->GetMapOffsetY());

	// Move to new XY
	RotatePoint(nAngle, ptTopLeftSubstitution, centerPoint);
	
	// delta SIZE structure
	CSize deltaMove = ptSelectedFrameTopLeft - ptTopLeftSubstitution;
	
	VectorMapElement* pVME = pDoc->GetVectorSelectedMapElement();
	for (VectorMapElement::iterator iter_me = pVME->begin();
	iter_me != pVME->end();
	iter_me++)
	{
		(*iter_me)->nAngle += nAngle;
		(*iter_me)->nAngle %= 360;
		// Calculate the new nPx and nPy
		// Get ME's width and height
		long nMEWidth = (*iter_me)->nWidth;
		long nMEHeight = (*iter_me)->nHeight;
		if ((*iter_me)->nType == ME_TYPE_TILE)
		{
			nMEWidth -= 2*pDoc->GetMapGridDefaultWidth();
			nMEHeight -= 2*pDoc->GetMapGridDefaultHeight();
		}

		//////////////////////////////////////////////////////////////////////////
		// Get the substitution top-left point of MAPELEMENT	
		int left = ((*iter_me)->nPx - 1)*pDoc->GetMapGridWidth();
		int top = ((*iter_me)->nPy - 1)*pDoc->GetMapGridHeight();
		int right = left + nMEWidth*pDoc->GetMapZoom();
		int bottom = top + nMEHeight*pDoc->GetMapZoom();
		CPoint ptMETopLeftSubstitution = RotatedTopLeftSubstitution(nAngle, left, top, right, bottom);

		// Rotate
		RotatePoint(nAngle, ptMETopLeftSubstitution, centerPoint);
		// Drop to the original point
		ptMETopLeftSubstitution += deltaMove;
		// Update nPx and nPy
		(*iter_me)->nPx = ptMETopLeftSubstitution.x / pDoc->GetMapGridWidth() + 1;
		(*iter_me)->nPy = ptMETopLeftSubstitution.y / pDoc->GetMapGridHeight() + 1;
	}
}

void CCanvasScrollView::RotatePoint( short nAngle, CPoint& dstPoint, CPoint centerPoint )
{
	// Move to new coordinates
//	dstPoint -= centerPoint;
	// Rotate
	int nTmp = dstPoint.x;
	if (nAngle == 90)
	{
		// x = -y; y = x right 90
		dstPoint.x = -dstPoint.y;
		dstPoint.y = nTmp;
	}
	else if (nAngle == 270)
	{
		// x = y; y = -x left 90
		dstPoint.x = dstPoint.y;
		dstPoint.y = -nTmp;
	}
	else if (nAngle == 180)
	{
		// x = -y; y = -x
		dstPoint.x = -dstPoint.y;
		dstPoint.y = -nTmp;
	}
	
	// Move back to old coordinates
//	dstPoint += centerPoint;
}

POINT CCanvasScrollView::RotatedTopLeftSubstitution( short nAngle, const CRect& rect )
{
	POINT pt = {0, 0};
	if (nAngle == 90) // right
	{
		// Get BottomLeft for the Substitution of TopLeft after rotate
		pt.x = rect.left;
		pt.y = rect.bottom;
	}
	else if (nAngle == 270) // left
	{
		// Get TopRight for the Substitution of TopLeft after rotate
		pt.x = rect.right;
		pt.y = rect.top;
	}
	else if (nAngle == 180)
	{
		// Get BottomRight for the Substitution of TopLeft after rotate
		pt = rect.BottomRight();
	}
	return pt;
}

POINT CCanvasScrollView::RotatedTopLeftSubstitution( short nAngle, int left, int top, int right, int bottom )
{
	CRect rect(left, top, right, bottom);
	return RotatedTopLeftSubstitution(nAngle, rect);
}

//////////////////////////////////////////////////////////////////////////
// protected implement

BOOL CCanvasScrollView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bLBtnDown)
	{
		return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
	}
	
	// Ctrl + MouseWheel to zoom the canvas
	short keyState = GetKeyState(VK_CONTROL);
	if (keyState & 0x8000)
	{
		CString strInfo;
		strInfo.Format("OnMouseWheel nFlags:%d, zDelta:%d, pt.x:%d, pt.y:%d\n", nFlags, zDelta, pt.x, pt.y);
		OutputDebugInformation((LPCTSTR)strInfo);
		// TRACE("nFlags:%d, zDelta:%d, pt.x:%d, pt.y:%d\n", nFlags, zDelta, pt.x, pt.y);
		if (zDelta > 0)
		{
			ZoomIn();
		}
		else
		{
			ZoomOut();
		}
	}
	
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CCanvasScrollView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd)
	{
		pWnd->PostMessage(WM_USER_STATUSBAR_MOUSE_POSTION_UPDATE,
			point.x - pDoc->GetMapOffsetX(), point.y - pDoc->GetMapOffsetY());
	}

	if (m_bLBtnDown)
	{
// 		TRACKMOUSEEVENT tme;
// 		tme.cbSize = sizeof(TRACKMOUSEEVENT);
// 		tme.hwndTrack = m_hWnd;
// 		tme.dwFlags = TME_LEAVE;
// 		_TrackMouseEvent(&tme);

		// Drag the selected ME
		
		if (m_bIsPaintSelectionRect)
		{
			Invalidate(FALSE);
		}
		else if (pDoc->GetMapEditMode() == MAP_EDIT_DRAG_MOVE)
		{
			// m_hCursor = m_hCurHand;
			// 拖动地图模式
			pDoc->SetMapOffsetX(pDoc->GetMapOriginalOffsetX() + point.x - m_ptStartPoint.x);
			pDoc->SetMapOffsetY(pDoc->GetMapOriginalOffsetY() + point.y - m_ptStartPoint.y);

			Invalidate(FALSE);
		}
		else if (pDoc->GetSelectedMapElementCount() > 0)
		{
			VectorMapElement* pVMESel = pDoc->GetVectorSelectedMapElement();

			// Calculate the offset of the mouse
			CRect rSelected;
			pDoc->GetSelectedMapElmentRect(rSelected);
			// Get move distance after zoomed
			CSize sizeOffset = point - m_ptStartPoint;
			// 计算移动的格子
			long deltaX = sizeOffset.cx/pDoc->GetMapGridWidth() + 0.5*(sizeOffset.cx > 0 ? 1 : -1);
			long deltaY = sizeOffset.cy/pDoc->GetMapGridHeight() + 0.5*(sizeOffset.cy > 0 ? 1 : -1);
			TRACE("cx:%d, cy:%d, deltaX:%d, deltaY:%d\n",
				sizeOffset.cx, sizeOffset.cy, deltaX, deltaY);
//			if (deltaX != 0 || deltaY != 0)
			{
				for (VectorMapElement::iterator iter_sel = pVMESel->begin();
				iter_sel != pVMESel->end();
				iter_sel++)
				{
 					(*iter_sel)->nPx = (*iter_sel)->posOri.x + deltaX;
 					(*iter_sel)->nPy = (*iter_sel)->posOri.y + deltaY;
				}
			}
			CString strInfo;
			strInfo.Format("OnMouseMove point.x:%d, point.y:%d, nOriPx:%d, nOriPy:%d, deltaX:%d, deltaY:%d\n",
				point.x, point.y, m_ptStartPoint.x, m_ptStartPoint.y,
				deltaX, deltaY);
			OutputDebugInformation((LPCTSTR)strInfo);
			Invalidate(FALSE);
			m_bDragging = TRUE;
		}
	}
	else
	{
		// Calculate the hover ME include the selected area
		CRect rSelectd;
		pDoc->GetSelectedMapElmentRect(rSelectd);
		// Judge the select area first
		if (rSelectd.PtInRect(point) || GetMapElementFromPoint(point))
		{
			m_hCursor = m_hCurHand;
		}
		else
		{
			m_hCursor = m_hCurArrow;
		}
	}

	// Store the last cursor for restoring except in MAPELEMENT dragging
	// generally in LButtonUp
	if (!m_bLBtnDown)
	{
		m_hLastCursor = m_hCursor;
	}
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CCanvasScrollView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();

	m_bLBtnDown = TRUE;

	SetCapture();
	
	// Save the mouse left button down point for rectangle
 	m_ptStartPoint = point;

	if ( pDoc->GetMapEditMode() == MAP_EDIT_DRAG_MOVE )
	{
		SetCursor(m_hCurDragging);
		return;
	}

	// Find the MAPELEMENT which be hovered by cursor
	MAPELEMENT* pMEUnderCursor = GetMapElementFromPoint(point);
	// Get selected MAPELEMENT's rectangle
	CRect rSelected;
	pDoc->GetSelectedMapElmentRect(rSelected);

	//////////////////////////////////////////////////////////////////////////
	// Show debug info in dlg
	CString strDebugInfo;
	strDebugInfo.Format("OnLButtonDown rSelected.left:%d, rSelected.top:%d, rSelected.right:%d, rSelected.bottom:%d, point.x:%d, point.y:%d\n", 
		rSelected.left, rSelected.top, rSelected.right, rSelected.bottom,
		point.x, point.y);
	OutputDebugInformation((LPCTSTR)strDebugInfo);
	//////////////////////////////////////////////////////////////////////////

	// Update the original position of selected array
	pDoc->UpdateOriginalPositionOfSelectedArray();

	short keyState = GetKeyState(VK_CONTROL);

	if (keyState & 0x8000)	// Add or Remove the MAPELEMENT which click by CONTROL button
	{
		// SELECTED AREA is not EMPTY and point is within the SELECTED AREA
		// Make sure clicked MAPELEMENT is in the area of selected
		if ( !rSelected.IsRectEmpty() && rSelected.PtInRect(point) &&
			// It's important that the MAPELEMENT which under the cursor
			// is the member of selected MAPELEMENT array
			pMEUnderCursor != NULL && pDoc->IsMapElementInSelectedMapElementArray(pMEUnderCursor))
		{
			// Remove the click MAPELEMENT
			pDoc->RemoveMapElementFromSelectedMapElementArray(pMEUnderCursor);
		}
		else if (pMEUnderCursor != NULL) // Otherwise it will be added to the selected array
		{
			// Add MAPELEMENT which click to Selected MAPELEMENT array
			pDoc->AddMapElementToSelectedMapElementArray(pMEUnderCursor);
			// Update the original position of selected array
			pDoc->UpdateOriginalPositionOfSelectedArray();
		}
		// Else do nothing
	}
	else if (pDoc->m_bIsLOSMode)
	{
		// LOS 模式
		// int nStep = pDoc->GetMapZoom()*pDoc->GetMapGridDefaultWidth();
		int nStep = pDoc->GetMapGridWidth();
		POINT pt;
		float fX = (point.x - pDoc->GetMapOffsetX())*1.0 / nStep;
		float fY = (point.y - pDoc->GetMapOffsetY())*1.0 / nStep;

		pt.x = fX > 0 ? ceil(fX) : floor(fX);
		pt.y = fY > 0 ? ceil(fY) : floor(fY);

		CString strDbg;
		strDbg.Format(_T("LOS Mode, point.x:%d, point.y:%d, offsetX:%d, offsetY:%d, step:%d, pt.x:%d pt.y:%d"), 
			point.x, point.y, pDoc->GetMapOffsetX(), pDoc->GetMapOffsetY(), nStep, pt.x, pt.y);
		OutputDebugInformation((LPCTSTR)strDbg);

		if ((pDoc->m_vLOSSrc.size() == 1 && pDoc->m_vLOSDst.size() == 1) ||
			(pDoc->m_vLOSSrc.size() == 0 && pDoc->m_vLOSDst.size() == 0))
		{
			// 程序起始，或者已经选则了两个点
			pDoc->m_vLOSSrc.clear();
			pDoc->m_vLOSDst.clear();
			pDoc->m_vLOSSrc.push_back(pt);
		}
		else if (pDoc->m_vLOSSrc.size() == 1 && pDoc->m_vLOSDst.size() == 0)
		{
			// 只选择了一个点
			pDoc->m_vLOSDst.push_back(pt);
		}
	}
	else // Except CONTROL state
	{
		//////////////////////////////////////////////////////////////////////////
		// 这里是点击已选择区域，可能是区域中的空白位置，也可能是区域中的地图元件
		// 首先需要记录下每个已选择的地图区域内每个地图元件的当前位置

		// SELECTED AREA is not EMPTY and point is within the SELECTED AREA
		if ( !rSelected.IsRectEmpty() && rSelected.PtInRect(point) )
		{
			m_hCursor = m_hCurDragging;
			SetCursor(m_hCurDragging);

			// Set selected array object to show in the top layer
			m_bDrawSelectedMapElementInTopLayer = TRUE;

			// Click a MAPELEMENT that is covering the multi-selected area and it was not selected yet
			if (pMEUnderCursor != NULL &&
				// If the clicked MAPELEMENT is not in selected array then set new selected array
				// e.g. click area is a transparent space cover on a MAPELEMENT
				!pDoc->IsMapElementInSelectedMapElementArray(pMEUnderCursor))
			{
				// Clear the selected MAPELEMENT data
				pDoc->EmptySelectedMapElementArray();
				// 设置当前鼠标下的地图元件为区域选中
				pDoc->AddMapElementToSelectedMapElementArray(pMEUnderCursor);
				// Update the original position of selected array
				pDoc->UpdateOriginalPositionOfSelectedArray();
			}

			// 记录当前选择区域的位置和大小
			pDoc->m_rLBtnDownSelect = rSelected;
			// 移动
			pDoc->ChangeRedoAndUndoDeque(OPERATE_MOVE);
		}
		else if ( pMEUnderCursor != NULL )
		{
			// Clear the selected MAPELEMENT data
			pDoc->EmptySelectedMapElementArray();
			// 设置当前鼠标下的地图元件为区域选中
			pDoc->AddMapElementToSelectedMapElementArray(pMEUnderCursor);
			// Update the original position of selected array
			pDoc->UpdateOriginalPositionOfSelectedArray();
			
			m_hCursor = m_hCurDragging;
			SetCursor(m_hCurDragging);

			// Set selected array object to show in the top layer
			m_bDrawSelectedMapElementInTopLayer = TRUE;
			// 记录当前选择区域的位置和大小
			pDoc->GetSelectedMapElmentRect(rSelected);
			pDoc->m_rLBtnDownSelect = rSelected;
			// 移动
			pDoc->ChangeRedoAndUndoDeque(OPERATE_MOVE);
		}
		else
		{
			// Empty the selected array which store the current MAPELEMENTs
			pDoc->EmptySelectedMapElementArray();
			// 设置鼠标为区域选择图标
			SetCursor(m_hCurRectangle);
			// 允许绘制选择框
			m_bIsPaintSelectionRect = TRUE;
		}
	}
	Invalidate(FALSE);
	//	TRACE("Current selected MapElement's name is [%s]\n", pDoc->GetSelectedMapElement() ? pDoc->GetSelectedMapElement()->strName.c_str() : "NULL");
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CCanvasScrollView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	ReleaseCapture();

	m_bLBtnDown = FALSE;

	// 同步偏移量
	pDoc->SyncOffset();

	OutputDebugInformation(_T("OnLButtonUp"));

// 	TRACE("pApp->m_bListImageDragging:%s\n", pApp->m_bListImageDragging ? "true" : "false");
	if (pApp->m_bListImageDragging)
	{
		// Get the focus
		::SetFocus(this->GetSafeHwnd());

// 		TRACE("CCanvasScrollView::OnLButtonUp\n");
		pApp->m_bListImageDragging = false;

		// Add a MapElement object to the Canvas
		long nID = pDoc->m_nDraggingMEID;
		ASSERT(nID != -1);
		if (nID != -1)
		{
			// Calculate the nPx and nPy
// 			long nPx = point.x / pDoc->GetMapGridWidth() + 1;
// 			long nPy = point.y / pDoc->GetMapGridHeight() + 1;
			int nOffX = point.x - pDoc->GetMapOffsetX();
			int nOffY = point.y - pDoc->GetMapOffsetY();
			long nPx = nOffX / pDoc->GetMapGridWidth();
			long nPy = nOffY / pDoc->GetMapGridHeight();

			nPx += (nOffX>=0 ? 1 : 0);
			nPy += (nOffY>=0 ? 1 : 0);

			// 增加
			pDoc->ChangeRedoAndUndoDeque(OPERATE_ADD);

			pDoc->AddMapElement(nID, nPx, nPy, 0);
			
			Invalidate(FALSE);
		}
	}
	else if (m_bDragging ||
		// !!! IMPORTANT !!!
		// m_bDrawSelectedMapElementInTopLayer in the expression to
		// force redraw map to show the selected MAPELEMENT in lower layer
		// because of it was hidden and drew in operate layer
		m_bDrawSelectedMapElementInTopLayer) 
	{
		m_bDrawSelectedMapElementInTopLayer = FALSE;

		CRect rSelected;
		pDoc->GetSelectedMapElmentRect(rSelected);

		if (m_bDragging)
		{	
			if (!pDoc->m_rLBtnDownSelect.IsRectEmpty()
				&& pDoc->m_rLBtnDownSelect == rSelected)
			{
				// 相同位置，表示没有移动
				pDoc->DeleteUndoLast();
			}
			// 左键放开后清空上一次的选择的区域大小标记
			pDoc->m_rLBtnDownSelect.SetRectEmpty();
		}
		
		// Expand the Selected Area for one blocks
		rSelected.InflateRect(pDoc->GetMapGridWidth(), pDoc->GetMapGridHeight());
		InvalidateRect(&rSelected, FALSE);

		m_bDragging = FALSE;
	}
	else if (m_bIsPaintSelectionRect)
	{
		// Refresh the view for erase the selection rectangle
		// and repaint the frame rectangle of selected MAPELEMENTs
		m_bIsPaintSelectionRect = FALSE;

//		DWORD dwTm = GetTickCount();
		// Set the select MAPELEMENT's array
		pDoc->SetMapElementsFromSelectedArea(m_ptStartPoint, point);
//		TRACE("Select a empty area used %d time.\n", GetTickCount() - dwTm);

		// Redraw the content except selects no MAPELEMENT
		// if (pDoc->GetSelectedMapElementCount() > 0)
		{
			Invalidate(FALSE);
		}
	}

	// 设置属性
	pDoc->UpdatePropertyViewData();

	// Restore the last cursor
	m_hCursor = m_hLastCursor;
	SetCursor(m_hCursor);

//	Invalidate(FALSE);

	CScrollView::OnLButtonUp(nFlags, point);
}

LRESULT CCanvasScrollView::OnMouseLeave( LPARAM lParam, WPARAM wParam )
{
	TRACE("Out of Canvas's area!\n");
	return TRUE;
}

void CCanvasScrollView::OnCanvasRecoverSize() 
{
	// TODO: Add your command handler code here

	// Set the grid width and height in original size
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	pDoc->SetMapGridWidth(pDoc->GetMapGridDefaultWidth());
	pDoc->SetMapGridHeight(pDoc->GetMapGridDefaultHeight());
	pDoc->SetMapZoom(1.0);
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd)
	{
		pWnd->PostMessage(WM_USER_STATUSBAR_ZOOMED_RATE_UPDATE);
	}

	pDoc->SetMapOffsetX(0);
	pDoc->SetMapOffsetY(0);
	pDoc->SyncOffset();

	Invalidate(FALSE);
}

void CCanvasScrollView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	MAPELEMENT* pME = pDoc->GetSelectedMapElement();
	VectorMapElement* pVMESel = pDoc->GetVectorSelectedMapElement();
	int nSelSize = pVMESel->size();

	short keyState = 0;
	keyState = GetKeyState(VK_CONTROL);

	DWORD CONTROL = 0x8000;

// 	TRACE("KEYDOWN nChar:%d, keyState:0x%08x\n", nChar, keyState);
	switch(nChar)
	{
	case VK_UP:
		if (keyState & CONTROL)
		{
			ZoomIn();
			Invalidate(FALSE);
		}
		else if (nSelSize > 0)
		{
			// Move up
			// 移动
			pDoc->ChangeRedoAndUndoDeque(OPERATE_MOVE);

			for (VectorMapElement::iterator iter_sel = pVMESel->begin();
			iter_sel != pVMESel->end();
			iter_sel++)
			{
				(*iter_sel)->nPy--;
			}
			Invalidate(FALSE);
		}
		break;
	case VK_LEFT:
		if (nSelSize > 0)
		{
			if (keyState & CONTROL)
			{
				OnButtonRotateLeft90();
			}
			else if (nSelSize > 0)
			{
				// Move left
				// 移动
				pDoc->ChangeRedoAndUndoDeque(OPERATE_MOVE);

				for (VectorMapElement::iterator iter_sel = pVMESel->begin();
				iter_sel != pVMESel->end();
				iter_sel++)
				{
					(*iter_sel)->nPx--;
				}
			}
			Invalidate(FALSE);
		}
		break;
	case VK_RIGHT:
		if (nSelSize > 0)
		{
			if (keyState & CONTROL)
			{
				OnButtonRotateRight90();
			}
			else if (nSelSize > 0)
			{
				// Move right
				// 移动
				pDoc->ChangeRedoAndUndoDeque(OPERATE_MOVE);

				for (VectorMapElement::iterator iter_sel = pVMESel->begin();
				iter_sel != pVMESel->end();
				iter_sel++)
				{
					(*iter_sel)->nPx++;
				}
			}
			Invalidate(FALSE);
		}
		break;
	case VK_DOWN:
		if (keyState & CONTROL)
		{
			ZoomOut();
			Invalidate(FALSE);
		}
		else if (nSelSize > 0)
		{
			// Move down
			// 移动
			pDoc->ChangeRedoAndUndoDeque(OPERATE_MOVE);

			for (VectorMapElement::iterator iter_sel = pVMESel->begin();
			iter_sel != pVMESel->end();
			iter_sel++)
			{
				(*iter_sel)->nPy++;
			}
			Invalidate(FALSE);
		} 
		break;
	case VK_DELETE:
		// OnEditDelete will call the Invalidate(FALSE)
		OnEditDelete();
		break;
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCanvasScrollView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	
	short keyState = 0;
	keyState = GetKeyState(VK_CONTROL);
	
	DWORD CONTROL = 0x8000;

	switch(nChar)
	{
	case 'a':
	case 'A':
		if (CONTROL & keyState)
		{
			// 全选
			pDoc->SelectAllMapElment();
			Invalidate(FALSE);
		}
		break;
	case VK_ESCAPE:
		// 清空当前选择
		pDoc->EmptySelectedMapElementArray();
		Invalidate(FALSE);
		break;
	}
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////
// 显示/隐藏 背景颜色和网格
void CCanvasScrollView::OnButtonShowGrid()
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	if (pDoc)
	{
		pDoc->m_bShowGrid = !(pDoc->m_bShowGrid);
		Invalidate(FALSE);
	}
}

#define ROTATE_LEFT 270
#define ROTATE_RIGHT 90
void CCanvasScrollView::OnButtonRotateLeft90() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	
	// Be sure that selected MAPELEMENT is not null
	if (pDoc->GetSelectedMapElementCount() > 0)
	{
		// 旋转
		pDoc->ChangeRedoAndUndoDeque(OPERATE_ROTATE_LEFT);

		RotateSelectedMapElements(ROTATE_LEFT);
		Invalidate(FALSE);
	}
}

void CCanvasScrollView::OnButtonRotateRight90() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	
	// Be sure that selected MAPELEMENT is not null
	if (pDoc->GetSelectedMapElementCount() > 0)
	{
		// 旋转
		pDoc->ChangeRedoAndUndoDeque(OPERATE_ROTATE_RIGHT);

		RotateSelectedMapElements(ROTATE_RIGHT);
		Invalidate(FALSE);
	}
}

void CCanvasScrollView::OnButtonZoomin() 
{
	// TODO: Add your command handler code here
	ZoomIn();
}

void CCanvasScrollView::OnButtonZoomout() 
{
	// TODO: Add your command handler code here
	ZoomOut();
}

//////////////////////////////////////////////////////////////////////////
// public implement

void CCanvasScrollView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
}

void CCanvasScrollView::OnDraw(CDC* pDC)
{
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	// TODO: add draw code here

	CRect canvasRect;
	GetClientRect(&canvasRect);

	if (m_bViewSizeChanged)
	{
		m_bViewSizeChanged = FALSE;

		// View 区域改变大小后需要重新设置绘画用的位图的大小
		// 否则还是使用原来大小的 bitmap
		m_pBitmap->DeleteObject();
	
		m_pBitmap->CreateCompatibleBitmap(pDC, canvasRect.Width(), canvasRect.Height());
	}

	// 创建内存DC，用户绘制最终的位图，之后直接粘贴到屏幕
	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL);

	// 创建临时的画布
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, canvasRect.Width(), canvasRect.Height());

	CBitmap* pOldMemBit = MemDC.SelectObject(&MemBitmap);

	// Fill the Background Color
	MemDC.FillSolidRect(&canvasRect, pDoc->GetCanvasGBColor());

	if (!m_bIsPaintSelectionRect && !m_bDragging) // 先判断是否需要重绘底层
	{
		// Save the member Bitmap
		CBitmap* pOldBit = m_memDC.SelectObject(m_pBitmap);
		
		//////////////////////////////////////////////////////////////////////////
		// Draw background color and grid
		DrawBackground(&m_memDC);
		
		// Draw map element
		DrawMap(&m_memDC);
		
		// Draw the selected MAPELEMENTs in lower layer if flag is false
		if (m_bDrawSelectedMapElementInTopLayer)
		{
			
		}
		else
		{
			// Paint the multi-selected MAPELEMENT's union frame rectangle
			DrawSelectedMapElementFrameRectangle(&m_memDC);
			//////////////////////////////////////////////////////////////////////////
		}
		
		// 拷贝缓存中的位图到临时 DC 上
		MemDC.BitBlt(canvasRect.left, canvasRect.top, canvasRect.Width(), canvasRect.Height(), &m_memDC, 0, 0, SRCCOPY);
		
		// 底板的图重新绘制后需要重新选择 m_memDC 原本的 GDI 对象
		m_memDC.SelectObject(pOldBit);
	}
	else
	{
		CBitmap* pOldBit = m_memDC.SelectObject(m_pBitmap);
		// 拷贝不变的缓存位图到临时 DC 中
		MemDC.BitBlt(canvasRect.left, canvasRect.top, canvasRect.Width(), canvasRect.Height(), &m_memDC, 0, 0, SRCCOPY);
		m_memDC.SelectObject(pOldBit);
	}

	if (m_bIsPaintSelectionRect || m_bDragging || m_bDrawSelectedMapElementInTopLayer) // 如果是绘制选择框或者是拖动图片则绘制在操作层中
	{
		CBitmap* pOldMemberMemDC = m_memDC.SelectObject(m_pBitmap);
		
		if (m_bIsPaintSelectionRect)
		{
			//////////////////////////////////////////////////////////////////////////
			// 在临时 DC 中绘制选择框
			DrawCursorSelectedRectangle(&MemDC);
			//////////////////////////////////////////////////////////////////////////
		}
		else if (m_bDragging || m_bDrawSelectedMapElementInTopLayer)
		{
			//////////////////////////////////////////////////////////////////////////
			// 在临时 DC 中绘制当前选择并跟随鼠标移动的地图
			DrawDraggingMapElements(&MemDC);
			// Paint the multi-selected MAPELEMENT's union frame rectangle
			DrawSelectedMapElementFrameRectangle(&MemDC);
		}

		// 把临时 DC 中的位图拷贝到视图 DC 刷新
		// pDC->BitBlt(canvasRect.left, canvasRect.top, canvasRect.Width(), canvasRect.Height(), &MemDC, 0, 0, SRCCOPY);

		m_memDC.SelectObject(pOldMemberMemDC);
	}

	if (pDoc->m_bIsLOSMode && pDoc->m_vLOSSrc.size() == 1 && pDoc->m_vLOSDst.size() == 1)
	{
		// 如果是LOS模式，则顶层绘制LOS线段
		CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen* pOldPen = MemDC.SelectObject(&pen);

		// 计算位置
		POINT ptStart = pDoc->m_vLOSSrc[0];
		POINT ptEnd = pDoc->m_vLOSDst[0];

		int nStepX = pDoc->GetMapGridWidth();	// nStepX
		int nStepY = pDoc->GetMapGridHeight();	// nStepY
		int nOffsetX = pDoc->GetMapOffsetX();
		int nOffsetY = pDoc->GetMapOffsetY();

		ptStart.x = ptStart.x * nStepX - nStepX/2 + nOffsetX;
		ptStart.y = ptStart.y * nStepY - nStepY/2 + nOffsetY;
		ptEnd.x = ptEnd.x * nStepX - nStepX/2 + nOffsetX;
		ptEnd.y = ptEnd.y * nStepY - nStepY/2 + nOffsetY;

		MemDC.MoveTo(ptStart);
		MemDC.LineTo(ptEnd);

		MemDC.SelectObject(pOldPen);
	}

	// 把临时 DC 中的位图拷贝到视图 DC 刷新
	pDC->BitBlt(canvasRect.left, canvasRect.top, canvasRect.Width(), canvasRect.Height(), &MemDC, 0, 0, SRCCOPY);

	// 清除变量善后工作
	MemDC.SelectObject(pOldMemBit);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	TRACE("OnDraw!\n");
}

void CCanvasScrollView::OnEditDelete()
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	// 删除
	pDoc->ChangeRedoAndUndoDeque(OPERATE_DELETE);

	// Delete current selected MAPELEMENT
	if (pDoc->DeleteSelectedMapElements() < 0)
	{
		// Deleted failed
		return;
	}

	Invalidate(FALSE);
}

void CCanvasScrollView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	// 粘贴
	pDoc->ChangeRedoAndUndoDeque(OPERATE_PASTE);

	if (pDoc->PasteCopiedMapElements() < 0)
	{
		// Paste failed
		return;
	}

	Invalidate(FALSE);
}

void CCanvasScrollView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	pDoc->CopySelectedMapElementArray();
}

BOOL CCanvasScrollView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	// 这里直接返回 TRUE，表示已经处理了设置图标的功能，不要再给系统处理
	// 系统处理可能会再次把图标设置回原来的箭头
	// 导致 WM_MOUSEMOVE 时设置指针的闪动
	// return CScrollView::OnSetCursor(pWnd, nHitTest, message);

	SetCursor(m_hCursor);
	return TRUE;
}

void CCanvasScrollView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_bViewSizeChanged = TRUE;
}

void CCanvasScrollView::OnButtonBackgroundColor() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	CColorDialog colorDlg(pDoc->GetCanvasGBColor());
	if (colorDlg.DoModal() == IDOK)
	{
		pDoc->SetCanvasBGColor(colorDlg.GetColor());
		// Update the color
		Invalidate(FALSE);
	}
}

void CCanvasScrollView::OnEditCut() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	OnEditCopy();
	OnEditDelete();
}

//////////////////////////////////////////////////////////////////////////
// OnUpdateBottonState

void CCanvasScrollView::OnUpdateButtonRotateLeft90(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->GetSelectedMapElementCount() > 0);
}

void CCanvasScrollView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->GetSelectedMapElementCount() > 0);
}

void CCanvasScrollView::OnUpdateButtonRotateRight90(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->GetSelectedMapElementCount() > 0);
}

void CCanvasScrollView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->GetSelectedMapElementCount() > 0);
}

void CCanvasScrollView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->GetSelectedMapElementCount() > 0);
}

void CCanvasScrollView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->GetCopiedMapElementCount() > 0);
}

void CCanvasScrollView::OnUpdateButtonSelectMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->SetCheck(pDoc->GetMapEditMode() == MAP_EDIT_SELECT);
}

void CCanvasScrollView::OnUpdateButtonMoveCanvas(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->SetCheck(pDoc->GetMapEditMode() == MAP_EDIT_DRAG_MOVE);
}

void CCanvasScrollView::OnUpdateButtonShowGrid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->SetCheck(pDoc->m_bShowGrid);
}

void CCanvasScrollView::OnEditRedo() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pDoc->ChangeRedoAndUndoDeque(OPERATE_REDO);
	Invalidate(FALSE);
}

void CCanvasScrollView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->GetRedoListSize() > 0);
}

void CCanvasScrollView::OnEditUndo() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pDoc->ChangeRedoAndUndoDeque(OPERATE_UNDO);
	Invalidate(FALSE);
}

void CCanvasScrollView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->GetUndoListSize() > 0);
}

void CCanvasScrollView::OutputDebugInformation( LPCTSTR lpszDebugInformation )
{
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();

	TRACE(lpszDebugInformation);
	pApp->m_logManager.Append(lpszDebugInformation);
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd)
	{
		pWnd->PostMessage(WM_USER_DEBUG_INFO_SHOW);
	}
}

void CCanvasScrollView::OnButtonSelectMode() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	if (pDoc->GetMapEditMode() != MAP_EDIT_SELECT)
	{
		pDoc->SetMapEditMode(MAP_EDIT_SELECT);
//		m_hCursor = m_hCurArrow;
	}
}

void CCanvasScrollView::OnButtonMoveCanvas() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	if (pDoc->GetMapEditMode() != MAP_EDIT_DRAG_MOVE)
	{
		pDoc->SetMapEditMode(MAP_EDIT_DRAG_MOVE);
//		m_hCursor = m_hCurHand;
	}	
}

void CCanvasScrollView::OnFileExport() 
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();
	// 图片导出
	CFileDialog expDlg(FALSE);
	OPENFILENAME& ofn = expDlg.m_ofn;
	ofn.lpstrTitle = _T("地图导出为");
	CString strFilter = _T("PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|Bitmap (*.bmp)|*.bmp|");
#define IMAGE_SAVE_TYPE_PNG 1
#define IMAGE_SAVE_TYPE_JPG 2
#define IMAGE_SAVE_TYPE_BMP 3
	char* pch = strFilter.GetBuffer(0);
	while ((pch = _tcschr(pch, '|')) != NULL)
			*pch++ = '\0';
	strFilter.ReleaseBuffer();

	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;

	if (IDOK == expDlg.DoModal())
	{
		CBitmap MemBitMap;
		
		CDC* pDC = GetWindowDC();
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);

		int nMargin = pDoc->GetOutputPicMargin();
		// 先计算图的大小范围
		CRect rBound = CalculateExportPictureBound(nMargin);
		// 得到绘制到图片时，整个地图左上角元件为原点移动到原位的偏移量
		// 这里的偏移量只需要直接叠加计算在公式中即可，不需要符号变换
		int nOffX = -rBound.left;
		int nOffY = -rBound.top;

// 		CxImage transparentImage;
// 		transparentImage.Create(rBound.Width(), rBound.Height(), 24, CXIMAGE_FORMAT_PNG);
// 		transparentImage.AlphaCreate();
// 		// 把图片设置为透明
// 		transparentImage.AlphaClear();

		MemBitMap.CreateCompatibleBitmap(pDC, rBound.Width(), rBound.Height());

		CBitmap* pOldBitMap = MemDC.SelectObject(&MemBitMap);

		// 把背景刷成白色
		MemDC.FloodFill(0, 0, RGB(255, 255, 255));

		// 出图
		// 这里如果耗时较长，这里可以统一调用 App 提供的进度条模式
		DrawMap(&MemDC, true, 1.0, nOffX, nOffY);

		MemDC.SelectObject(pOldBitMap);

		CxImage outputImage;
		bool bRet = outputImage.CreateFromHBITMAP((HBITMAP)MemBitMap);

		MemBitMap.DeleteObject();

		//////////////////////////////////////////////////////////////////////////
		MemDC.SelectObject(pOldBitMap);
		MemDC.DeleteDC();
		//////////////////////////////////////////////////////////////////////////
		CString strTypeExt = "";
		DWORD dwImageType = CXIMAGE_FORMAT_BMP;
		switch(ofn.nFilterIndex)
		{
		case IMAGE_SAVE_TYPE_JPG:
			dwImageType = CXIMAGE_FORMAT_JPG;
			strTypeExt = _T(".jpg");
			break;
		case IMAGE_SAVE_TYPE_PNG:
			dwImageType = CXIMAGE_FORMAT_PNG;
			strTypeExt = _T(".png");
			break;
		case IMAGE_SAVE_TYPE_BMP:
			dwImageType = CXIMAGE_FORMAT_BMP;
			strTypeExt = _T(".bmp");
		default:
			break;
		}
#undef IMAGE_SAVE_TYPE_PNG
#undef IMAGE_SAVE_TYPE_JPG
#undef IMAGE_SAVE_TYPE_BMP
		
		CString strFileName = expDlg.GetPathName();
		// 对文件后缀进行智能匹配
		if (expDlg.GetFileExt().IsEmpty())
		{
			strFileName += strTypeExt;
		}

		// 保存文件错误
		if (!outputImage.Save(strFileName, dwImageType))
		{
			AfxMessageBox(_T("图片保存失败"), MB_OK | MB_ICONERROR);
		}
	}
}

CRect CCanvasScrollView::CalculateExportPictureBound( int nMargin /*= 64*/ )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	// 记录地图的网格大小
	int nXStep = pDoc->GetMapGridDefaultWidth();
	int nYStep = pDoc->GetMapGridDefaultHeight();
	CRect rect(0, 0, 0, 0);
	for (VectorMapElementLayerList::iterator iter_layer = pDoc->m_pvMELayers->begin();
	iter_layer != pDoc->m_pvMELayers->end();
	iter_layer++)
	{
		if ((*iter_layer)->size() > 0)
		{
			// DrawMapLayer(pDC, *iter_layer, bSavePicture, dZoom);
			for (VectorMapElement::iterator iter = (*iter_layer)->begin();
			(*iter_layer)->end() != iter;
			iter++
			)
			{
				// 记录left最小，top最小，right最大，bottom最大到 CRect
				int nLeft = ((*iter)->nPx - 1)*nXStep;
				int nTop = ((*iter)->nPy - 1)*nYStep;
				int nRight = nLeft + (*iter)->nWidth;
				int nBottom = nTop + (*iter)->nHeight;

				if ((*iter)->nType == ME_TYPE_TILE)
				{
					// 地图由于边上有一个单位宽的透明条
					nRight -= 2*nXStep;
					nBottom -= 2*nYStep;
				}
				
				if (rect.IsRectNull())
				{
					rect.left = nLeft;
					rect.right = nRight;
					rect.top = nTop;
					rect.bottom = nBottom;
				}
				else
				{
					rect.left = min(nLeft, rect.left);
					rect.right = max(nRight, rect.right);
					rect.top = min(nTop, rect.top);
					rect.bottom = max(nBottom, rect.bottom);
				}
			}
		}
	}

	// 如果地图内容不为空，则加上边界的宽度
	if (!rect.IsRectNull() && nMargin != 0)
	{
		rect.left -= nMargin;
		rect.top -= nMargin;
		rect.right += nMargin;
		rect.bottom += nMargin;
	}
	return rect;
}
