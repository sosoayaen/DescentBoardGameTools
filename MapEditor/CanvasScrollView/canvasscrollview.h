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
	// ��ͼ�Ŵ�
	void ZoomOut();
	//////////////////////////////////////////////////////////////////////////
	// ��ͼ��С
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
	// ȷ����ǰ��ͼ�Ƿ�����������״̬
	BOOL m_bLBtnDown;
	
	// ��ͼ�϶���־
	BOOL m_bDragging;

	// �Ƿ����ѡ���
	BOOL m_bIsPaintSelectionRect;

	// ��ͼ���ű�־
	BOOL m_bViewSizeChanged;

	// �Ƿ��ڶ������ѡ���ͼ��
	BOOL m_bDrawSelectedMapElementInTopLayer;

	// ����ѡ������ʼ��
	CPoint m_ptStartPoint;

	// ��ǰ���ƺõ�λͼ����
	CBitmap* m_pBitmap;

	// �ڴ�DC
	CDC m_memDC;

	//////////////////////////////////////////////////////////////////////////
	// ���Ʊ����������������Լ�����ɫ
	void DrawBackground(CDC* pDC);

	//////////////////////////////////////////////////////////////////////////
	// ���Ƶ�ͼԪ�ز�
	// CDC*              pDC          ���Ƶ��豸������
	// VectorMapElement* pVME         ���Ƶĵ�ͼԪ�� Vector
	// bool              bSavePicture �Ƿ����ͼƬ��־���������е���ֻ�е��˲���Ϊ true ʱ��Ч
	// double            dZoom        ����ʱ�����ű���
	// int               nSPOffX      ��� X ��λ�� �����λ����Ϊ����ͼƬ�����ʱ�������к��ʵ�λ��
	// int               nSPOffY      ��� Y ��λ��
	void DrawMapLayer(CDC* pDC, VectorMapElement* pVME, bool bSavePicture = false, double dZoom = 0.0f, int nSPOffX = 0, int nSPOffY = 0);

	//////////////////////////////////////////////////////////////////////////
	// �����϶�ѡ���еĵ�ͼ
	// CDC*              pDC   ���Ƶ��豸������
	// VectorMapElement* pVME  ���Ƶĵ�ͼԪ�� Vector
	void DrawSelMapLayer(CDC* pDC, VectorMapElement* pVME);

	//////////////////////////////////////////////////////////////////////////
	// ���ɵ͵��ߵ�˳�����ͼ��
	// CDC*   pDC          ���Ƶ��豸������
	// bool   bSavePicture �Ƿ����ͼƬ��־���������е���ֻ�е��˲���Ϊ true ʱ��Ч
	// double dZoom        ���ű��������Ϊ0���ʾȡ��ǰ��ʾ�����ű����
	// int    nOffX        ��� X ��λ�� ���λ����Ϊ����ͼƬ�����ʱ�������к��ʵ�λ��
	// int    nOffY        ��� Y ��λ��
	void DrawMap(CDC* pDC, bool bSavePicture = false, double dZoom = 0.0f, int nOffX = 0, int nOffY = 0);

	//////////////////////////////////////////////////////////////////////////
	// ���ƿ�ѡ����ʱ��ѡ���
	void DrawCursorSelectedRectangle(CDC* pDC);

	//////////////////////////////////////////////////////////////////////////
	// ����ѡ�е�ͼԪ����������Χ��Ϸ���
	void DrawSelectedMapElementFrameRectangle(CDC* pDC);

	//////////////////////////////////////////////////////////////////////////
	// �����϶��ĵ�ͼ
	void DrawDraggingMapElements(CDC* pDC);

	//////////////////////////////////////////////////////////////////////////
	// ���ź���
	// bool bZoomedOut  �Ƿ�Ŵ� Ĭ��Ϊtrue
	// int  nZoomedStep ���Ų��� Ĭ��Ϊ16
	void Zoom( bool bZoomedOut = true, int nZoomedStep = 16 );

	//////////////////////////////////////////////////////////////////////////
	// ��õ�ǰ�������� MAPELEMENT
	// ���
	//     CPoint    ��ǰ��ͼ�����µ�����
	// ����
	//     MAPELEMENT* ����ҵ������ض�Ӧ�ҵ���ͼԪ��������ָ��
	//                 ���û���ҵ�������NULL
	MAPELEMENT* GetMapElementFromPoint(CPoint point);

	//////////////////////////////////////////////////////////////////////////
	// ���ݽǶ���תѡ��ĵ�ͼԪ��
	void RotateSelectedMapElements(short nAngle);

	//////////////////////////////////////////////////////////////////////////
	// �Ե������ת
	void RotatePoint(short nAngle, CPoint& dstPoint, CPoint centerPoint);

	//////////////////////////////////////////////////////////////////////////
	// ������ת�Ƕȵõ���Ӧ�油���Ͻǵĵ㵱ǰ�ĵ�λ��
	POINT RotatedTopLeftSubstitution(short nAngle, const CRect& rect);
	POINT RotatedTopLeftSubstitution(short nAngle, int left, int top, int right, int bottom);

	//////////////////////////////////////////////////////////////////////////
	// Debug information
	void OutputDebugInformation(LPCTSTR lpszDebugInformation);

	//////////////////////////////////////////////////////////////////////////
	// ����������ͼ������ͼƬ��Ĵ�С
	// ���
	//		int	�߿���Ķ����ȣ����ߣ�
	CRect CalculateExportPictureBound(int nMargin = 64);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANVASSCROLLVIEW_H__E72626F5_8082_450F_88CF_8E23C53B14EC__INCLUDED_)
