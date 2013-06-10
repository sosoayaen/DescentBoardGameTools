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
	
	//���ñ���ɫ��λͼ����ɾ�
	pDC->FillSolidRect(&canvasRect, pDoc->GetCanvasGBColor());
	if (pDoc && pDoc->m_bShowGrid)
	{
		// ���Ʊ�������
		CPen _gridPen(pDoc->GetCanvasMapGridLineStyle(), pDoc->GetCanvasMapGridLineWeight(), pDoc->GetCanvasGLColor());
		
		CPen* pOldPen = pDC->SelectObject(&_gridPen);
		
		// ͨ������Ŀ��������
		int nxCounts = canvasRect.Width() / pDoc->GetMapGridWidth();
		int nyCounts = canvasRect.Height() / pDoc->GetMapGridHeight();
		
		// ����ȷ����һ���ߵ�λ��
		int nStartX = pDoc->GetMapOffsetX();
		if (abs(pDoc->GetMapOffsetX()) > pDoc->GetMapGridWidth())
		{
			// ƫ�ƴ��ڳ���Ԫ��λ��ƫ��ֵΪ��ǰ��ͼ��Ԫ��ƫ��ȡ��
			nStartX = pDoc->GetMapOffsetX() % pDoc->GetMapGridWidth();
		}
		int nStartY = pDoc->GetMapOffsetY();
		if (abs(pDoc->GetMapOffsetY()) > pDoc->GetMapGridHeight())
		{
			// ƫ�ƴ��ڳ���Ԫ��λ��ƫ��ֵΪ��ǰ��ͼ��Ԫ��ƫ��ȡ��
			nStartY = pDoc->GetMapOffsetY() % pDoc->GetMapGridHeight();
		}

		// �������ߣ�����������ͼ��ƫ��
		for (int idx_x=0; idx_x<=nxCounts; idx_x++)
		{
			int nOffset = idx_x*pDoc->GetMapGridWidth() - 1;
			int nFixedX = (nOffset>0 ? nOffset : 0) + nStartX;
			pDC->MoveTo(nFixedX, 0);
			pDC->LineTo(nFixedX, canvasRect.bottom);
		}
		
		// ���ƺ��ߣ�����������ͼ��ƫ��
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
		// �жϵ�ǰͼ���Ƿ���Ҫ����
		if (!pDoc->GetMapLayerVisible((*iter)->nType))
		{
			continue;
		}

		// ����� Tile����������� Tiles ֮����Ҫ����
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
		// �ڳ�ͼ�Ĺ����У�ֱ��ʹ�ô����λ����
		nOffX = nSPOffX;
		nOffY = nSPOffY;
	}

	for (VectorMapElement::iterator iter=pVME->begin(); iter!=pVME->end(); iter++)
	{
		// �жϵ�ǰͼ���Ƿ���Ҫ����
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
		
		// ����� Tile����������� Tiles ֮����Ҫ����
		if ((*iter)->nType != ME_TYPE_TILE)
		{
			nOverlap = 0;
		}

		// ����λ����Ҫ����ƫ����
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
			// �ӿ�����ٶȣ�Ĭ�ϼ��١��������ͼƬ�����������������
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

	// ������Կ��ǰѶ�Ӧ�Ļ��Ʋ���� Vector �У�ѭ������
// 	DrawMapLayer(pDC, &pDoc->m_vMapTiles, dZoom);		// ���Ƶذ壨�������Ż��Ŀռ䣬���Ե������ذ�һ�����Ƶĺ�����
// 	DrawMapLayer(pDC, &pDoc->m_vMapDoors, dZoom);			// ������
// 	DrawMapLayer(pDC, &pDoc->m_vMapObstacles, dZoom);		// �����ϰ���
// 	DrawMapLayer(pDC, &pDoc->m_vMapObjects, dZoom);			// ��������
// 	DrawMapLayer(pDC, &pDoc->m_vMapTreasure, dZoom);		// ���Ʊ���
// 	DrawMapLayer(pDC, &pDoc->m_vMapMonsters, dZoom);		// ���ƹ���
// 	DrawMapLayer(pDC, &pDoc->m_vMapMasters, dZoom);			// ���ƾ�Ӣ����
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
	// �Ŵ���С����
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
	// �ػ汳��
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

	// ��õ�ͼ��ǰ�����ű�
	double dZoom = pDoc->GetMapZoom();
	
	VectorMapElement::reverse_iterator r_elementIter;

	// ȷ�϶�Ӧ��ͼ���е�Element�Ƿ�����������ڣ���ͼ���˳�򣬶��ϵ����ж�
	for (VectorMapElementLayerList::reverse_iterator r_layerIter = pDoc->m_pvMELayers->rbegin();
	r_layerIter < pDoc->m_pvMELayers->rend() && !bFind;
	r_layerIter++)
	{
		// ѭ��ÿ��ͼ���е�Ԫ���Ƿ�����귶Χ����Ӧ
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
			// Ԫ����λ�ö���Ҫ�ȼ���ƫ����
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
	// ������Ϊ��Ҫ����Ԫ����λ�ã����Ի���Ҫ�Ƴ�ƫ������Ȼ���ټ���
	// ���Ƶ�ʱ������ƫ������ȷ������λ�ã�����Ԫ����λ����Ҫ��δƫ�Ƶ�����ϵ�¼���
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
			// �϶���ͼģʽ
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
			// �����ƶ��ĸ���
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
		// LOS ģʽ
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
			// ������ʼ�������Ѿ�ѡ����������
			pDoc->m_vLOSSrc.clear();
			pDoc->m_vLOSDst.clear();
			pDoc->m_vLOSSrc.push_back(pt);
		}
		else if (pDoc->m_vLOSSrc.size() == 1 && pDoc->m_vLOSDst.size() == 0)
		{
			// ֻѡ����һ����
			pDoc->m_vLOSDst.push_back(pt);
		}
	}
	else // Except CONTROL state
	{
		//////////////////////////////////////////////////////////////////////////
		// �����ǵ����ѡ�����򣬿����������еĿհ�λ�ã�Ҳ�����������еĵ�ͼԪ��
		// ������Ҫ��¼��ÿ����ѡ��ĵ�ͼ������ÿ����ͼԪ���ĵ�ǰλ��

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
				// ���õ�ǰ����µĵ�ͼԪ��Ϊ����ѡ��
				pDoc->AddMapElementToSelectedMapElementArray(pMEUnderCursor);
				// Update the original position of selected array
				pDoc->UpdateOriginalPositionOfSelectedArray();
			}

			// ��¼��ǰѡ�������λ�úʹ�С
			pDoc->m_rLBtnDownSelect = rSelected;
			// �ƶ�
			pDoc->ChangeRedoAndUndoDeque(OPERATE_MOVE);
		}
		else if ( pMEUnderCursor != NULL )
		{
			// Clear the selected MAPELEMENT data
			pDoc->EmptySelectedMapElementArray();
			// ���õ�ǰ����µĵ�ͼԪ��Ϊ����ѡ��
			pDoc->AddMapElementToSelectedMapElementArray(pMEUnderCursor);
			// Update the original position of selected array
			pDoc->UpdateOriginalPositionOfSelectedArray();
			
			m_hCursor = m_hCurDragging;
			SetCursor(m_hCurDragging);

			// Set selected array object to show in the top layer
			m_bDrawSelectedMapElementInTopLayer = TRUE;
			// ��¼��ǰѡ�������λ�úʹ�С
			pDoc->GetSelectedMapElmentRect(rSelected);
			pDoc->m_rLBtnDownSelect = rSelected;
			// �ƶ�
			pDoc->ChangeRedoAndUndoDeque(OPERATE_MOVE);
		}
		else
		{
			// Empty the selected array which store the current MAPELEMENTs
			pDoc->EmptySelectedMapElementArray();
			// �������Ϊ����ѡ��ͼ��
			SetCursor(m_hCurRectangle);
			// �������ѡ���
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

	// ͬ��ƫ����
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

			// ����
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
				// ��ͬλ�ã���ʾû���ƶ�
				pDoc->DeleteUndoLast();
			}
			// ����ſ��������һ�ε�ѡ��������С���
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

	// ��������
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
			// �ƶ�
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
				// �ƶ�
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
				// �ƶ�
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
			// �ƶ�
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
			// ȫѡ
			pDoc->SelectAllMapElment();
			Invalidate(FALSE);
		}
		break;
	case VK_ESCAPE:
		// ��յ�ǰѡ��
		pDoc->EmptySelectedMapElementArray();
		Invalidate(FALSE);
		break;
	}
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////
// ��ʾ/���� ������ɫ������
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
		// ��ת
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
		// ��ת
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

		// View ����ı��С����Ҫ�������û滭�õ�λͼ�Ĵ�С
		// ������ʹ��ԭ����С�� bitmap
		m_pBitmap->DeleteObject();
	
		m_pBitmap->CreateCompatibleBitmap(pDC, canvasRect.Width(), canvasRect.Height());
	}

	// �����ڴ�DC���û��������յ�λͼ��֮��ֱ��ճ������Ļ
	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL);

	// ������ʱ�Ļ���
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, canvasRect.Width(), canvasRect.Height());

	CBitmap* pOldMemBit = MemDC.SelectObject(&MemBitmap);

	// Fill the Background Color
	MemDC.FillSolidRect(&canvasRect, pDoc->GetCanvasGBColor());

	if (!m_bIsPaintSelectionRect && !m_bDragging) // ���ж��Ƿ���Ҫ�ػ�ײ�
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
		
		// ���������е�λͼ����ʱ DC ��
		MemDC.BitBlt(canvasRect.left, canvasRect.top, canvasRect.Width(), canvasRect.Height(), &m_memDC, 0, 0, SRCCOPY);
		
		// �װ��ͼ���»��ƺ���Ҫ����ѡ�� m_memDC ԭ���� GDI ����
		m_memDC.SelectObject(pOldBit);
	}
	else
	{
		CBitmap* pOldBit = m_memDC.SelectObject(m_pBitmap);
		// ��������Ļ���λͼ����ʱ DC ��
		MemDC.BitBlt(canvasRect.left, canvasRect.top, canvasRect.Width(), canvasRect.Height(), &m_memDC, 0, 0, SRCCOPY);
		m_memDC.SelectObject(pOldBit);
	}

	if (m_bIsPaintSelectionRect || m_bDragging || m_bDrawSelectedMapElementInTopLayer) // ����ǻ���ѡ���������϶�ͼƬ������ڲ�������
	{
		CBitmap* pOldMemberMemDC = m_memDC.SelectObject(m_pBitmap);
		
		if (m_bIsPaintSelectionRect)
		{
			//////////////////////////////////////////////////////////////////////////
			// ����ʱ DC �л���ѡ���
			DrawCursorSelectedRectangle(&MemDC);
			//////////////////////////////////////////////////////////////////////////
		}
		else if (m_bDragging || m_bDrawSelectedMapElementInTopLayer)
		{
			//////////////////////////////////////////////////////////////////////////
			// ����ʱ DC �л��Ƶ�ǰѡ�񲢸�������ƶ��ĵ�ͼ
			DrawDraggingMapElements(&MemDC);
			// Paint the multi-selected MAPELEMENT's union frame rectangle
			DrawSelectedMapElementFrameRectangle(&MemDC);
		}

		// ����ʱ DC �е�λͼ��������ͼ DC ˢ��
		// pDC->BitBlt(canvasRect.left, canvasRect.top, canvasRect.Width(), canvasRect.Height(), &MemDC, 0, 0, SRCCOPY);

		m_memDC.SelectObject(pOldMemberMemDC);
	}

	if (pDoc->m_bIsLOSMode && pDoc->m_vLOSSrc.size() == 1 && pDoc->m_vLOSDst.size() == 1)
	{
		// �����LOSģʽ���򶥲����LOS�߶�
		CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen* pOldPen = MemDC.SelectObject(&pen);

		// ����λ��
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

	// ����ʱ DC �е�λͼ��������ͼ DC ˢ��
	pDC->BitBlt(canvasRect.left, canvasRect.top, canvasRect.Width(), canvasRect.Height(), &MemDC, 0, 0, SRCCOPY);

	// ��������ƺ���
	MemDC.SelectObject(pOldMemBit);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	TRACE("OnDraw!\n");
}

void CCanvasScrollView::OnEditDelete()
{
	// TODO: Add your command handler code here
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	// ɾ��
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

	// ճ��
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
	
	// ����ֱ�ӷ��� TRUE����ʾ�Ѿ�����������ͼ��Ĺ��ܣ���Ҫ�ٸ�ϵͳ����
	// ϵͳ������ܻ��ٴΰ�ͼ�����û�ԭ���ļ�ͷ
	// ���� WM_MOUSEMOVE ʱ����ָ�������
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
	// ͼƬ����
	CFileDialog expDlg(FALSE);
	OPENFILENAME& ofn = expDlg.m_ofn;
	ofn.lpstrTitle = _T("��ͼ����Ϊ");
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
		// �ȼ���ͼ�Ĵ�С��Χ
		CRect rBound = CalculateExportPictureBound(nMargin);
		// �õ����Ƶ�ͼƬʱ��������ͼ���Ͻ�Ԫ��Ϊԭ���ƶ���ԭλ��ƫ����
		// �����ƫ����ֻ��Ҫֱ�ӵ��Ӽ����ڹ�ʽ�м��ɣ�����Ҫ���ű任
		int nOffX = -rBound.left;
		int nOffY = -rBound.top;

// 		CxImage transparentImage;
// 		transparentImage.Create(rBound.Width(), rBound.Height(), 24, CXIMAGE_FORMAT_PNG);
// 		transparentImage.AlphaCreate();
// 		// ��ͼƬ����Ϊ͸��
// 		transparentImage.AlphaClear();

		MemBitMap.CreateCompatibleBitmap(pDC, rBound.Width(), rBound.Height());

		CBitmap* pOldBitMap = MemDC.SelectObject(&MemBitMap);

		// �ѱ���ˢ�ɰ�ɫ
		MemDC.FloodFill(0, 0, RGB(255, 255, 255));

		// ��ͼ
		// ���������ʱ�ϳ����������ͳһ���� App �ṩ�Ľ�����ģʽ
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
		// ���ļ���׺��������ƥ��
		if (expDlg.GetFileExt().IsEmpty())
		{
			strFileName += strTypeExt;
		}

		// �����ļ�����
		if (!outputImage.Save(strFileName, dwImageType))
		{
			AfxMessageBox(_T("ͼƬ����ʧ��"), MB_OK | MB_ICONERROR);
		}
	}
}

CRect CCanvasScrollView::CalculateExportPictureBound( int nMargin /*= 64*/ )
{
	CMapEditorDoc* pDoc = (CMapEditorDoc*)GetDocument();

	// ��¼��ͼ�������С
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
				// ��¼left��С��top��С��right���bottom��� CRect
				int nLeft = ((*iter)->nPx - 1)*nXStep;
				int nTop = ((*iter)->nPy - 1)*nYStep;
				int nRight = nLeft + (*iter)->nWidth;
				int nBottom = nTop + (*iter)->nHeight;

				if ((*iter)->nType == ME_TYPE_TILE)
				{
					// ��ͼ���ڱ�����һ����λ���͸����
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

	// �����ͼ���ݲ�Ϊ�գ�����ϱ߽�Ŀ��
	if (!rect.IsRectNull() && nMargin != 0)
	{
		rect.left -= nMargin;
		rect.top -= nMargin;
		rect.right += nMargin;
		rect.bottom += nMargin;
	}
	return rect;
}
