#if !defined(AFX_CANVASSCROLLVIEW_H__E72626F5_8082_450F_88CF_8E23C53B14EC__INCLUDED_)
#define AFX_CANVASSCROLLVIEW_H__E72626F5_8082_450F_88CF_8E23C53B14EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CanvasScrollView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCanvasScrollView view

#include "ximage.h"

class CCanvasScrollView : public CScrollView
{
protected:
	CCanvasScrollView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCanvasScrollView)

// Attributes
public:

// Operations
public:
	//////////////////////////////////////////////////////////////////////////
	// 视图放大
	void ZoomOut();
	//////////////////////////////////////////////////////////////////////////
	// 视图缩小
	void ZoomIn();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanvasScrollView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Cursor Hand
	HCURSOR m_hCurHand;
	// Cursor Dragging
	HCURSOR m_hCurDragging;
	// Cursor Select rectangle
	HCURSOR m_hCurRectangle;
	// Cursor Arrow
	HCURSOR m_hCurArrow;
	// CurrentCursor
	HCURSOR m_hCursor;
	// Last Cursor
	HCURSOR m_hLastCursor;

	virtual ~CCanvasScrollView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCanvasScrollView)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCanvasRecoverSize();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButtonShowGrid();
	afx_msg void OnButtonRotateLeft90();
	afx_msg void OnButtonRotateRight90();
	afx_msg void OnButtonZoomin();
	afx_msg void OnButtonZoomout();
	afx_msg void OnEditDelete();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCopy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonBackgroundColor();
	afx_msg void OnUpdateButtonRotateLeft90(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonRotateRight90(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnButtonSelectMode();
	afx_msg void OnUpdateButtonSelectMode(CCmdUI* pCmdUI);
	afx_msg void OnButtonMoveCanvas();
	afx_msg void OnUpdateButtonMoveCanvas(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnFileExport();
	afx_msg void OnUpdateButtonShowGrid(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(LPARAM lParam, WPARAM wParam);
	DECLARE_MESSAGE_MAP()
private:
	// 确定当前视图是否鼠标左键按下状态
	BOOL m_bLBtnDown;
	
	// 地图拖动标志
	BOOL m_bDragging;

	// 是否绘制选择框
	BOOL m_bIsPaintSelectionRect;

	// 地图缩放标志
	BOOL m_bViewSizeChanged;

	// 是否在顶层绘制选择的图层
	BOOL m_bDrawSelectedMapElementInTopLayer;

	// 绘制选择框的起始点
	CPoint m_ptStartPoint;

	// 当前绘制好的位图缓存
	CBitmap* m_pBitmap;

	// 内存DC
	CDC m_memDC;

	//////////////////////////////////////////////////////////////////////////
	// 绘制背景，包括网格线以及背景色
	void DrawBackground(CDC* pDC);

	//////////////////////////////////////////////////////////////////////////
	// 绘制地图元素层
	// CDC*              pDC          绘制的设备上下文
	// VectorMapElement* pVME         绘制的地图元素 Vector
	// bool              bSavePicture 是否输出图片标志，后面所有当属只有当此参数为 true 时有效
	// double            dZoom        绘制时的缩放比例
	// int               nSPOffX      相对 X 轴位移 这里的位移是为了让图片输出的时候在正中合适的位置
	// int               nSPOffY      相对 Y 轴位移
	void DrawMapLayer(CDC* pDC, VectorMapElement* pVME, bool bSavePicture = false, double dZoom = 0.0f, int nSPOffX = 0, int nSPOffY = 0);

	//////////////////////////////////////////////////////////////////////////
	// 绘制拖动选择中的地图
	// CDC*              pDC   绘制的设备上下文
	// VectorMapElement* pVME  绘制的地图元素 Vector
	void DrawSelMapLayer(CDC* pDC, VectorMapElement* pVME);

	//////////////////////////////////////////////////////////////////////////
	// 按由低到高的顺序绘制图层
	// CDC*   pDC          绘制的设备上下文
	// bool   bSavePicture 是否输出图片标志，后面所有当属只有当此参数为 true 时有效
	// double dZoom        缩放比例，如果为0则表示取当前显示的缩放比输出
	// int    nOffX        相对 X 轴位移 里的位移是为了让图片输出的时候在正中合适的位置
	// int    nOffY        相对 Y 轴位移
	void DrawMap(CDC* pDC, bool bSavePicture = false, double dZoom = 0.0f, int nOffX = 0, int nOffY = 0);

	//////////////////////////////////////////////////////////////////////////
	// 绘制框选区域时的选择框
	void DrawCursorSelectedRectangle(CDC* pDC);

	//////////////////////////////////////////////////////////////////////////
	// 绘制选中地图元件的整个外围组合方框
	void DrawSelectedMapElementFrameRectangle(CDC* pDC);

	//////////////////////////////////////////////////////////////////////////
	// 绘制拖动的地图
	void DrawDraggingMapElements(CDC* pDC);

	//////////////////////////////////////////////////////////////////////////
	// 缩放函数
	// bool bZoomedOut  是否放大 默认为true
	// int  nZoomedStep 缩放步长 默认为16
	void Zoom( bool bZoomedOut = true, int nZoomedStep = 16 );

	//////////////////////////////////////////////////////////////////////////
	// 获得当前鼠标区域的 MAPELEMENT
	// 入参
	//     CPoint    当前视图区域下的坐标
	// 返回
	//     MAPELEMENT* 如果找到，返回对应找到地图元件的数据指针
	//                 如果没有找到，返回NULL
	MAPELEMENT* GetMapElementFromPoint(CPoint point);

	//////////////////////////////////////////////////////////////////////////
	// 根据角度旋转选择的地图元件
	void RotateSelectedMapElements(short nAngle);

	//////////////////////////////////////////////////////////////////////////
	// 对点进行旋转
	void RotatePoint(short nAngle, CPoint& dstPoint, CPoint centerPoint);

	//////////////////////////////////////////////////////////////////////////
	// 根据旋转角度得到对应替补左上角的点当前的点位置
	POINT RotatedTopLeftSubstitution(short nAngle, const CRect& rect);
	POINT RotatedTopLeftSubstitution(short nAngle, int left, int top, int right, int bottom);

	//////////////////////////////////////////////////////////////////////////
	// Debug information
	void OutputDebugInformation(LPCTSTR lpszDebugInformation);

	//////////////////////////////////////////////////////////////////////////
	// 计算整个地图导出到图片后的大小
	// 入参
	//		int	边框外的额外宽度（留边）
	CRect CalculateExportPictureBound(int nMargin = 64);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANVASSCROLLVIEW_H__E72626F5_8082_450F_88CF_8E23C53B14EC__INCLUDED_)
