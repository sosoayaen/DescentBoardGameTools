// MapEditorDoc.h : interface of the CMapEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITORDOC_H__323B4819_89CD_4C87_A9E9_C94DDEE435F4__INCLUDED_)
#define AFX_MAPEDITORDOC_H__323B4819_89CD_4C87_A9E9_C94DDEE435F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ximage.h"
#include <string>
#include <map>
#include <DEQUE>
#include "PropertyObject.h"

typedef std::vector<VectorMapElement*> VectorMapElementLayerList;
typedef std::deque<VectorMapElementLayerList*> DequeStack;
// ����ӵ������
typedef std::vector<POINT> VectorLOS;

class CMapEditorDoc : public CDocument
{
protected: // create from serialization only
	CMapEditorDoc();
	DECLARE_DYNCREATE(CMapEditorDoc)

// Attributes
public:
	// ��ǰѡ����Ϸŵĵ�ͼID
	// -1 ��ʾ��ǰû���Ϸ�ѡ��ĵ�ͼԪ��
	long m_nDraggingMEID;

	// ����Ϊһ����ǣ���¼�����������϶�ǰѡ������Ĵ�С
	// �����ж��������ſ��󣬶�Ӧѡ�������Ƿ����ƶ����Ա�
	// �������������Ĺ���
	CRect m_rLBtnDownSelect;

	// �Ƿ���ʾ������
	bool m_bShowGrid;

	// ��Ҹ���������Ϊ2���
	int m_nPlayerCount;

	int m_nThreatToken;
	int m_nConquestToken;

	VectorMapElement* m_pvMapTiles;			// ���ΰ��
	VectorMapElement* m_pvMapDoors;			// ���ΰ��
	VectorMapElement* m_pvMapObstacles;		// �ϰ���
	VectorMapElement* m_pvMapObjects;			// ����
	VectorMapElement* m_pvMapTreasure;		// ����
	VectorMapElement* m_pvMapMonsters;		// ��ͨ����
	VectorMapElement* m_pvMapMasters;			// ��ͨ����
	VectorMapElement* m_pvMapHeroes;	// Ӣ�����

	VectorMapElementLayerList* m_pvMELayers;	// ��ͼ��ӵ͵���˳��ڷŵ�ͼ��

	//////////////////////////////////////////////////////////////////////////
	// ����ѡ����Ʒ�ƶ�ʱ����Ľṹ
	VectorMapElement m_vSelTiles;
	VectorMapElement m_vSelDoors;
	VectorMapElement m_vSelObstacles;
	VectorMapElement m_vSelObjects;
	VectorMapElement m_vSelTeasure;
	VectorMapElement m_vSelMonsters;
	VectorMapElement m_vSelMasters;
	VectorMapElement m_vSelHeroes;

	VectorMapElementLayerList m_vSelMELayers;	// ��ͼ��ӵ͵���˳��ڷŵ�ͼ��

	IPropertyHost* m_pCurrentPropertyObj;	// ��ǰ���Զ��� ��ѡ����ͬ�ĵ�ͼ����ʱ��ʾ��ͬ����������
	PropertyObjectDefault m_PropObjDefault;	// Ĭ�ϵĿ�����

	PropertyObjectCommObject m_PropObjComm;	// һ����Ʒ
	PropertyObjectTile m_PropObjTile;		// �ذ�
	PropertyObjectMonster m_PropObjMonster; // ��ͼԪ�����Զ�Ӧ��ѡ��
	PropertyObjectDoor m_PropObjDoor;		// ��
	PropertyObjectObstacle m_PropObjObstacle;	// �ϰ���
	PropertyObjectCombination m_PropObjCombine;	// ���
	PropertyObjectTreature m_PropObjTreature;	//  ����
	PropertyObjectHero m_PropObjHero;	// Ӣ��

	bool m_bIsLOSMode;	// Line of Sight ģʽ����

	VectorLOS m_vLOSSrc;	// �ӵ�Դ����
	VectorLOS m_vLOSDst;	// �յ�����

	ElPoint m_elPointSrc;
	ElPoint m_elPointDst;

// Operations
public:
	// ���Ĭ������Ŀ����أ�
	int GetMapGridDefaultWidth();
	// ���Ĭ������ĸߣ����أ�
	int GetMapGridDefaultHeight();

	// ���õ�ǰ����Ŀ�ʵ�ʣ�
	void SetMapGridWidth(int nWidth);
	// ��õ�ǰ����Ŀ�ʵ�ʣ�
	int GetMapGridWidth();

	// ���õ�ǰ����ĸߣ�ʵ�ʣ�
	void SetMapGridHeight(int nHeight);
	// ��õ�ǰ����ĸߣ�ʵ�ʣ�
	int GetMapGridHeight();

	// ���ñ�������ɫ
	void SetCanvasBGColor(COLORREF colBG);
	// ��ñ�������ɫ
	COLORREF GetCanvasGBColor();

	// ���ñ��������ߵ���ɫ
	void SetCanvasGLColor(COLORREF colGL);
	// ��ñ��������ߵ���ɫ
	COLORREF GetCanvasGLColor();

	// ���ñ��������ߵķ��Ĭ��Ϊ PS_DOT
	void SetCanvasMapGridLineStyle(DWORD nStyle);
	// ��õ�ǰ���������ߵķ��Ĭ��Ϊ PS_DOT
	DWORD GetCanvasMapGridLineStyle();

	// ���ñ��������ߵĴ�ϸ
	void SetCanvasMapGridLineWeight(int nLineWeight);
	// ��ñ��������ߵĴ�ϸ��Ĭ��Ϊ1
	DWORD GetCanvasMapGridLineWeight();

	// ���õ�ǰ���ѡ�еĵ�ͼԪ��������Ϊ�գ��ձ�ʾû��ѡ���κ�Ԫ��
	// discard
	void SetSelectedMapElement(MAPELEMENT* pME);
	// �õ���ǰ���ѡ�еĵ�ͼԪ��������Ϊ��
	// discard
	MAPELEMENT* GetSelectedMapElement();

	void SetCopiedMapElement(MAPELEMENT me);
	MAPELEMENT GetCopiedMapElement();

	// ���õ�ǰ�����ű�
	void SetMapZoom(double dZoom);
	// ��õ�ǰ���ű�
	double GetMapZoom();

	//////////////////////////////////////////////////////////////////////////
	// ����һ����ͼԪ������ͼ������
	// ���
	// MAPELEMENT*
	//     ����� MAPELEMENT �������Ҫ�ڶ����� new ���롣����ʱ���ͷ�
	// long nID, long nPx = 2, long nPy = 2, long nAngle = 0
	//     ��ӦͼƬ��Ψһ ID����ӵ�λ�ã���ת�ĽǶȡ�Ĭ�Ϻ��������������Ժ���
	// VectorMapElement*
	//     ���һϵ�еĵ�ͼԪ�����ϡ�������ճ��(Ctrl+V)������
	// ����
	// int	0��ʾ�ɹ�����0��ʾʧ��
	int AddMapElement(MAPELEMENT* pME);

	int AddMapElement(long nID, long nPx = 2, long nPy = 2, long nAngle = 0);

	int AddMapElement(VectorMapElement* pVME);

	//////////////////////////////////////////////////////////////////////////
	// ɾ��ѡ��ĵ�ͼԪ������
	// ������Ϊ�����ʾɾ��Ĭ��ѡ��ѡ������
	// ע�⣬��ɾ���������������ѡ������飬���һ�Ӷ�Ӧͼ����
	//     �ѵ�ͼԪ���Ľṹ���ڴ����ͷ�
	// [���]
	// VectorMapElement* pVME ��ɾ���ĵ�ͼԪ������
	// [����]
	// int
	// ����0 ��ʾɾ���ɹ������ҷ�����ɾ��������
	int DeleteSelectedMapElements(VectorMapElement* pVME = NULL);

	//////////////////////////////////////////////////////////////////////////
	// ��õ�ǰѡ������� MAPELEMENT ����
	// [���]
	// CRect rect ��ǰ�������
	// [����]
	// int
	// 0 ��ʾ�ɹ� �� 0 ��ʾʧ��
	int SetMapElementsFromSelectedArea(CRect rect);
	
	//////////////////////////////////////////////////////////////////////////
	// ��õ�ǰѡ������� MAPELEMENT ����
	// [���]
	// CPoint startPoint ��ʼ��
	// CPoint endPoint ��ֹ��
	// [����]
	// int
	// 0 ��ʾ�ɹ� �� 0 ��ʾʧ��
	int SetMapElementsFromSelectedArea(CPoint startPoint, CPoint endPoint);

	//////////////////////////////////////////////////////////////////////////
	// ��ѡ������������һ����ǰѡ��ĵ�ͼԪ��
	// ��Ҫ�½�����ֱ������������Ѵ��ڽṹ��ָ�뼴��
	// [���]
	// MAPELEMENT* pME ��Ҫ��ӵĵ�ͼԪ���ṹָ��
	// [����]
	// 0 ��ʾ�ɹ� �� 0 ��ʾʧ��
	int AddMapElementToSelectedMapElementArray(MAPELEMENT* pMEAdd);

	//////////////////////////////////////////////////////////////////////////
	// �򿽱������ڸ��Ƶ�ǰѡ����������� ÿ��������new����
	int CopySelectedMapElementArray();
	
	//////////////////////////////////////////////////////////////////////////
	// �ѵ�ǰ���������须�ڵ��������ӵ���ͼ��
	int PasteCopiedMapElements();

	//////////////////////////////////////////////////////////////////////////
	// �ӱ�ѡ��ĵ�ͼ�б����Ƴ�һ��
	// [���]
	// MAPELEMENT* pMEDel ���Ƴ��ĵ�ͼԪ��ָ��
	// [����]
	// 0 ��ʾ�ɹ� �� 0 ��ʾʧ��
	int RemoveMapElementFromSelectedMapElementArray(MAPELEMENT* pMEDel);

	//////////////////////////////////////////////////////////////////////////
	// �жϸ����ĵ�ͼԪ���Ƿ���ѡ��ĵ�ͼԪ��������
	// [���]
	// MAPELEMENT* pME ���������жϵĵ�ͼԪ������ṹָ��
	// [����]
	// true ��ʾ�ҵ� false ��ʾû�ҵ�
	bool IsMapElementInSelectedMapElementArray(MAPELEMENT* pME);

	//////////////////////////////////////////////////////////////////////////
	// ���ѡ������
	// [����]
	// 0 ��ʾ�ɹ� �� 0 ��ʾʧ��
	int EmptySelectedMapElementArray();

	//////////////////////////////////////////////////////////////////////////
	// ��ո�������
	// [����]
	// 0 ��ʾ�ɹ� �� 0 ��ʾʧ��
	int EmptyCopiedMapElementArray();

	//////////////////////////////////////////////////////////////////////////
	// �õ���ǰѡ��ĵ�ͼԪ������Ĵ�С
	int GetSelectedMapElementCount() const;

	//////////////////////////////////////////////////////////////////////////
	// �õ���ǰ�ڼ������еĵ�ͼԪ������Ĵ�С
	int GetCopiedMapElementCount() const;

	// �õ���ǰѡ��ĵ�ͼԪ�������
	void GetSelectedMapElmentRect(CRect& rect);

	// �õ���ǰѡ��ĵ�ͼԪ������
	VectorMapElement* GetVectorSelectedMapElement();

	//////////////////////////////////////////////////////////////////////////
	// ����ѡ��Ԫ��������ÿ��Ԫ������ʼ�ƶ�λ��
	void UpdateOriginalPositionOfSelectedArray();

	// ��õ�ǰ��ͼѡ��ģʽ
	ENUM_EDIT_MODE GetMapEditMode();

	// ���õ�ǰ��ͼѡ��ģʽ
	void SetMapEditMode(ENUM_EDIT_MODE eMode);

	// ���ú͵õ���ǰƫ����
	void SetMapOffsetX(long nOffset);
	void SetMapOffsetY(long nOffset);
	long GetMapOffsetX();
	long GetMapOffsetY();

	// �õ�ԭʼƫ����
	long GetMapOriginalOffsetX();
	long GetMapOriginalOffsetY();

	// ͬ����ǰƫ������ԭʼƫ����
	void SyncOffset();

	// �޸������ͳ����б�
	// int nType OPERATE_ADD
	//	         OPERATE_DELETE
	//	         OPERATE_MOVE
	//	         OPERATE_PASTE
	//	         OPERATE_ROTATE_LEFT
	//	         OPERATE_ROTATE_RIGHT
	//	         OPERATE_REDO
	//	         OPERATE_UNDO
	bool ChangeRedoAndUndoDeque(int nType);

	//////////////////////////////////////////////////////////////////////////
	// ��� Redo ���г���
	int GetRedoListSize()
	{
		return m_dsRedo.size();
	}

	//////////////////////////////////////////////////////////////////////////
	// ��� Undo ���г���
	int GetUndoListSize()
	{
		return m_dsUndo.size();
	}

	//////////////////////////////////////////////////////////////////////////
	// ɾ����ӵ� Undo �����һ��Ԫ��
	// ����϶���ʱ������갴��ʱ����ѡ�������ڻ���ѡ��һ��Ԫ������ʾ���ܻ��϶�
	// ��������һ�� Undo ��Ԫ�أ�������ſ���ѡ�������λ��û���ƶ���ɾ��������
	// ����Ϊ���ƶ�ǰ������һ����������ȷ
	bool DeleteUndoLast();

	//////////////////////////////////////////////////////////////////////////
	// ��������ͳ�������
// 	void ClearUndoAndRedoDeque()
// 	{
// 		ClearDequeStack(&m_dsUndo);
// 		ClearDequeStack(&m_dsRedo);
// 	}
	int SelectAllMapElment();

	//////////////////////////////////////////////////////////////////////////
	// ���õ�ͼ���ߵĿ��
	void SetOutputPicMargin(int nMargin)
	{
		m_nOutputPicMargin = nMargin;
	}

	//////////////////////////////////////////////////////////////////////////
	// �õ���ͼ���ߵĿ��
	int GetOutputPicMargin()
	{
		return m_nOutputPicMargin;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���������ߵĴ�ϸ
	void SetGridLineWeight(int nWeight)
	{
		m_nCanvasMapGridLineWeight = nWeight;
	}

	//////////////////////////////////////////////////////////////////////////
	// �õ������ߴ�ϸ
	int GetGridLineWeight()
	{
		return m_nCanvasMapGridLineWeight;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���õ�ǰ����ҳ��Ķ���������ʾ��ͬ����������
	bool UpdatePropertyViewData();

	bool IsShowEveryMESelectionFrame();

	void SetShowEveryMESelectionFrame( bool bShowMEFrame );

	COLORREF GetMESelectionFrameColor();

	void SetMESelectionFrameColor(COLORREF rgb);

	//////////////////////////////////////////////////////////////////////////
	// ֪ͨ��Ӧ��ͼԪ���������޸ģ�һ�����޸��������������Ҫ��Ԫ������������Ӧ��
	bool OnPlayerCountChange();

	//////////////////////////////////////////////////////////////////////////
	// ���ö�Ӧ��ͼ����ʾ���
	// eLayer ��ӦҪ������ͼ��
	// bVisible �����Ƿ�ɼ�
	void SetMapLayerVisible( ENUM_ME_TYPE eLayer, bool bVisible );

	//////////////////////////////////////////////////////////////////////////
	// �õ���Ӧͼ���Ƿ���ʾ
	bool GetMapLayerVisible( ENUM_ME_TYPE eLayer );

	//////////////////////////////////////////////////////////////////////////
	// Toggle Layer visible
	void ToggleMapLayerVisible( ENUM_ME_TYPE eLayer );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	long m_nMapOffsetX;					// ��ͼX��ƫ��
	long m_nMapOffsetY;					// ��ͼY��ƫ��

	long m_nMapOriginalOffsetX;			// �ƶ�ʱ��¼��ͼX��ԭʼƫ��
	long m_nMapOriginalOffsetY;			// �ƶ�ʱ��¼��ͼY��ԭʼƫ��

	ENUM_EDIT_MODE m_eEditMode;			// ��ͼ�༭ģʽö�ٱ���
	int m_nCanvasMapGridWidth;			// ��ͼ��������Ŀ��
	int m_nCanvasMapGridHeight;			// ��ͼ��������ĸ߶�
	int m_nCanvasMapGridDefaultWidth;	// Ĭ�Ͽ�
	int m_nCanvasMapGridDefaultHeight;	// Ĭ�ϸ�
	DWORD m_nCanvasMapGridLineStyle;	// ��ͼ��������
	int m_nCanvasMapGridLineWeight;		// ��ͼ�����ߴ�ϸ
	
	double m_dZoom;						// ��ǰ��ͼ�����ű�

	MAPELEMENT* m_pSelectedMapElement;	// ������������ʱѡ�еĵ�ͼԪ��
	MAPELEMENT m_copiedMapElement;		// ���Ƶĵ�ͼԪ��

	VectorMapElement m_vSelectedME;		// ��ǰѡ�У����򣩵�ͼԪ������
	
	CRect m_rSelected;					// ѡ����������(��ʱ���ã�ֱ�Ӽ���õ���
	VectorMapElement m_vCopiedME;		// ��ǰ���������򣩵�ͼԪ������

	//////////////////////////////////////////////////////////////////////////
	// ������������
	// ÿ�����鶼��һ�� VectorMapElementLayerList �б�
	// ÿһ�����ݰ��������е�����
	DequeStack m_dsRedo;	// ��������
	DequeStack m_dsUndo;	// ��������

	int m_nMaxRedoCnts;		// ����������� ��ʱ����
	int m_nMaxUndoCnts;		// ���������

	COLORREF m_colorCanvasBackGroundColor;	// ��������ɫ
	COLORREF m_colorCanvasGridLineColor;	// ������������ɫ
	
	int m_nOutputPicMargin;	// ����ͼƬʱ��Ӧ�ı�������
	
	bool m_bShowEveryMESelectionFrame;	// �Ƿ���ʾÿ��ѡ�еĵ�ͼ�ı߿�
	COLORREF m_colorMESelectionFrame;	// ÿ��ѡ�еĵ�ͼ�ı߿����ɫ

	DWORD m_dwLayerVisibleFlag;	// ͼ����ʾ���ı�־λ��Ĭ��32λ������������32��ͼ�����ʾ���



// Generated message map functions
protected:
	//{{AFX_MSG(CMapEditorDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//////////////////////////////////////////////////////////////////////////
	// ɾ��������ͼԪ��
	// ����ֵ	int  0��ʾ�ɹ��� ��0��ʾʧ��
	//				-1��ʾ��Ԫ����ɾ��
	int DeleteSelectedMapElement(MAPELEMENT* pME);

	//////////////////////////////////////////////////////////////////////////
	// �õ���ǰ��ͼԪ����һ������
	// ������ new ������Ҫ�ͷ�
	VectorMapElementLayerList* GetCurrentMapFullDataLayerList();
	
	//////////////////////////////////////////////////////////////////////////
	// �ͷ����ݵ�һ������
	bool ReleaseVectorMapElementLayerListData(VectorMapElementLayerList* pVMELL);

	//////////////////////////////////////////////////////////////////////////
	// �ѵ�ǰ����ָ����е�ĩβ״̬����
	bool SetCurrentMapFromTail(DequeStack* pDS);

	//////////////////////////////////////////////////////////////////////////
	// ����������߳�������
	bool ClearDequeStack(DequeStack* pDS);

	//////////////////////////////////////////////////////////////////////////
	// �˺���ֻ�����ظ��Ĺ�������
	void BindPropertyData(PropertyObjectCommObject* PropObj, MAPELEMENT* me);

	//////////////////////////////////////////////////////////////////////////
	// ���ݵ�ǰ������������ö�Ӧ��ͼ�����Ĭ������
	// ���ȴ�Lua�д������ݣ����û�ж�Ӧ�Ĵ�����̣���Ĭ����C++������Ĭ�ϴ���
	void SetMapObjectDefaultData(MAPELEMENT *pME);
	
	//////////////////////////////////////////////////////////////////////////
	// ��һ��MAPELEMENT��Vectorѭ����������
	void SetMapElementDefaultData(VectorMapElement* pVME);
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITORDOC_H__323B4819_89CD_4C87_A9E9_C94DDEE435F4__INCLUDED_)
