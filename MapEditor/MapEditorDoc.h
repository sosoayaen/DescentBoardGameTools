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
// 存放视点的数组
typedef std::vector<POINT> VectorLOS;

class CMapEditorDoc : public CDocument
{
protected: // create from serialization only
	CMapEditorDoc();
	DECLARE_DYNCREATE(CMapEditorDoc)

// Attributes
public:
	// 当前选择的拖放的地图ID
	// -1 表示当前没有拖放选择的地图元件
	long m_nDraggingMEID;

	// 仅仅为一个标记，记录鼠标左键点下拖动前选择区域的大小
	// 用作判断鼠标左键放开后，对应选择区域是否有移动，以便
	// 处理撤销和重做的功能
	CRect m_rLBtnDownSelect;

	// 是否显示网格线
	bool m_bShowGrid;

	// 玩家个数，至少为2玩家
	int m_nPlayerCount;

	int m_nThreatToken;
	int m_nConquestToken;

	VectorMapElement* m_pvMapTiles;			// 地形板块
	VectorMapElement* m_pvMapDoors;			// 地形板块
	VectorMapElement* m_pvMapObstacles;		// 障碍物
	VectorMapElement* m_pvMapObjects;			// 杂物
	VectorMapElement* m_pvMapTreasure;		// 宝物
	VectorMapElement* m_pvMapMonsters;		// 普通怪物
	VectorMapElement* m_pvMapMasters;			// 普通怪物
	VectorMapElement* m_pvMapHeroes;	// 英雄玩家

	VectorMapElementLayerList* m_pvMELayers;	// 按图层从低到高顺序摆放的图层

	//////////////////////////////////////////////////////////////////////////
	// 绘制选择物品移动时缓存的结构
	VectorMapElement m_vSelTiles;
	VectorMapElement m_vSelDoors;
	VectorMapElement m_vSelObstacles;
	VectorMapElement m_vSelObjects;
	VectorMapElement m_vSelTeasure;
	VectorMapElement m_vSelMonsters;
	VectorMapElement m_vSelMasters;
	VectorMapElement m_vSelHeroes;

	VectorMapElementLayerList m_vSelMELayers;	// 按图层从低到高顺序摆放的图层

	IPropertyHost* m_pCurrentPropertyObj;	// 当前属性对象 在选定不同的地图对象时显示不同的属性内容
	PropertyObjectDefault m_PropObjDefault;	// 默认的空属性

	PropertyObjectCommObject m_PropObjComm;	// 一般物品
	PropertyObjectTile m_PropObjTile;		// 地板
	PropertyObjectMonster m_PropObjMonster; // 地图元件属性对应的选项
	PropertyObjectDoor m_PropObjDoor;		// 门
	PropertyObjectObstacle m_PropObjObstacle;	// 障碍物
	PropertyObjectCombination m_PropObjCombine;	// 组合
	PropertyObjectTreature m_PropObjTreature;	//  宝物
	PropertyObjectHero m_PropObjHero;	// 英雄

	bool m_bIsLOSMode;	// Line of Sight 模式开关

	VectorLOS m_vLOSSrc;	// 视点源数组
	VectorLOS m_vLOSDst;	// 终点数组

	ElPoint m_elPointSrc;
	ElPoint m_elPointDst;

// Operations
public:
	// 获得默认网格的宽（像素）
	int GetMapGridDefaultWidth();
	// 获得默认网格的高（像素）
	int GetMapGridDefaultHeight();

	// 设置当前网格的宽（实际）
	void SetMapGridWidth(int nWidth);
	// 获得当前网格的宽（实际）
	int GetMapGridWidth();

	// 设置当前网格的高（实际）
	void SetMapGridHeight(int nHeight);
	// 获得当前网格的高（实际）
	int GetMapGridHeight();

	// 设置背景的颜色
	void SetCanvasBGColor(COLORREF colBG);
	// 获得背景的颜色
	COLORREF GetCanvasGBColor();

	// 设置背景网格线的颜色
	void SetCanvasGLColor(COLORREF colGL);
	// 获得背景网格线的颜色
	COLORREF GetCanvasGLColor();

	// 设置背景网格线的风格，默认为 PS_DOT
	void SetCanvasMapGridLineStyle(DWORD nStyle);
	// 获得当前背景网格线的风格，默认为 PS_DOT
	DWORD GetCanvasMapGridLineStyle();

	// 设置背景网格线的粗细
	void SetCanvasMapGridLineWeight(int nLineWeight);
	// 获得背景网格线的粗细，默认为1
	DWORD GetCanvasMapGridLineWeight();

	// 设置当前鼠标选中的地图元件，可以为空，空表示没有选中任何元件
	// discard
	void SetSelectedMapElement(MAPELEMENT* pME);
	// 得到当前鼠标选中的地图元件，可以为空
	// discard
	MAPELEMENT* GetSelectedMapElement();

	void SetCopiedMapElement(MAPELEMENT me);
	MAPELEMENT GetCopiedMapElement();

	// 设置当前的缩放比
	void SetMapZoom(double dZoom);
	// 获得当前缩放比
	double GetMapZoom();

	//////////////////////////////////////////////////////////////////////////
	// 增加一个地图元件到地图内容中
	// 入参
	// MAPELEMENT*
	//     这里的 MAPELEMENT 对象必须要在堆中用 new 申请。清理时会释放
	// long nID, long nPx = 2, long nPy = 2, long nAngle = 0
	//     对应图片的唯一 ID，添加的位置，旋转的角度。默认后面三个参数可以忽略
	// VectorMapElement*
	//     添加一系列的地图元件集合。多用在粘贴(Ctrl+V)功能上
	// 返回
	// int	0表示成功，非0表示失败
	int AddMapElement(MAPELEMENT* pME);

	int AddMapElement(long nID, long nPx = 2, long nPy = 2, long nAngle = 0);

	int AddMapElement(VectorMapElement* pVME);

	//////////////////////////////////////////////////////////////////////////
	// 删除选择的地图元件数组
	// 如果入参为空则表示删除默认选择选中数组
	// 注意，此删除操作不仅会清空选择的数组，并且会从对应图层中
	//     把地图元件的结构从内存中释放
	// [入参]
	// VectorMapElement* pVME 待删除的地图元件数组
	// [返回]
	// int
	// 大于0 表示删除成功，并且返回是删除的数量
	int DeleteSelectedMapElements(VectorMapElement* pVME = NULL);

	//////////////////////////////////////////////////////////////////////////
	// 获得当前选择区域的 MAPELEMENT 数组
	// [入参]
	// CRect rect 当前区域矩形
	// [返回]
	// int
	// 0 表示成功 非 0 表示失败
	int SetMapElementsFromSelectedArea(CRect rect);
	
	//////////////////////////////////////////////////////////////////////////
	// 获得当前选择区域的 MAPELEMENT 数组
	// [入参]
	// CPoint startPoint 起始点
	// CPoint endPoint 终止点
	// [返回]
	// int
	// 0 表示成功 非 0 表示失败
	int SetMapElementsFromSelectedArea(CPoint startPoint, CPoint endPoint);

	//////////////////////////////////////////////////////////////////////////
	// 向选择数组中增加一个当前选择的地图元件
	// 不要新建对象，直接添加描述该已存在结构的指针即可
	// [入参]
	// MAPELEMENT* pME 需要添加的地图元件结构指针
	// [返回]
	// 0 表示成功 非 0 表示失败
	int AddMapElementToSelectedMapElementArray(MAPELEMENT* pMEAdd);

	//////////////////////////////////////////////////////////////////////////
	// 向拷贝数组内复制当前选择的数组数据 每个对象都是new出来
	int CopySelectedMapElementArray();
	
	//////////////////////////////////////////////////////////////////////////
	// 把当前『复制数组』内的数据增加到地图中
	int PasteCopiedMapElements();

	//////////////////////////////////////////////////////////////////////////
	// 从被选择的地图列表中移除一个
	// [入参]
	// MAPELEMENT* pMEDel 待移除的地图元件指针
	// [返回]
	// 0 表示成功 非 0 表示失败
	int RemoveMapElementFromSelectedMapElementArray(MAPELEMENT* pMEDel);

	//////////////////////////////////////////////////////////////////////////
	// 判断给出的地图元件是否在选择的地图元件数组中
	// [入参]
	// MAPELEMENT* pME 给出用于判断的地图元件对象结构指针
	// [返回]
	// true 表示找到 false 表示没找到
	bool IsMapElementInSelectedMapElementArray(MAPELEMENT* pME);

	//////////////////////////////////////////////////////////////////////////
	// 清空选择数组
	// [返回]
	// 0 表示成功 非 0 表示失败
	int EmptySelectedMapElementArray();

	//////////////////////////////////////////////////////////////////////////
	// 清空复制数组
	// [返回]
	// 0 表示成功 非 0 表示失败
	int EmptyCopiedMapElementArray();

	//////////////////////////////////////////////////////////////////////////
	// 得到当前选择的地图元素数组的大小
	int GetSelectedMapElementCount() const;

	//////////////////////////////////////////////////////////////////////////
	// 得到当前在剪贴板中的地图元件数组的大小
	int GetCopiedMapElementCount() const;

	// 得到当前选择的地图元件的外框
	void GetSelectedMapElmentRect(CRect& rect);

	// 得到当前选择的地图元件数组
	VectorMapElement* GetVectorSelectedMapElement();

	//////////////////////////////////////////////////////////////////////////
	// 更新选中元件数组中每个元件的起始移动位置
	void UpdateOriginalPositionOfSelectedArray();

	// 获得当前地图选择模式
	ENUM_EDIT_MODE GetMapEditMode();

	// 设置当前地图选择模式
	void SetMapEditMode(ENUM_EDIT_MODE eMode);

	// 设置和得到当前偏移量
	void SetMapOffsetX(long nOffset);
	void SetMapOffsetY(long nOffset);
	long GetMapOffsetX();
	long GetMapOffsetY();

	// 得到原始偏移量
	long GetMapOriginalOffsetX();
	long GetMapOriginalOffsetY();

	// 同步当前偏移量到原始偏移量
	void SyncOffset();

	// 修改重做和撤销列表
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
	// 获得 Redo 队列长度
	int GetRedoListSize()
	{
		return m_dsRedo.size();
	}

	//////////////////////////////////////////////////////////////////////////
	// 获得 Undo 队列长度
	int GetUndoListSize()
	{
		return m_dsUndo.size();
	}

	//////////////////////////////////////////////////////////////////////////
	// 删除添加到 Undo 的最后一个元素
	// 鼠标拖动的时候，在鼠标按下时（在选择区域内或者选中一个元件）表示可能会拖动
	// 则先增加一个 Undo 的元素，如果鼠标放开后选择区域的位置没有移动则删除，否则
	// 则认为在移动前保存了一份数据且正确
	bool DeleteUndoLast();

	//////////////////////////////////////////////////////////////////////////
	// 清空重做和撤销队列
// 	void ClearUndoAndRedoDeque()
// 	{
// 		ClearDequeStack(&m_dsUndo);
// 		ClearDequeStack(&m_dsRedo);
// 	}
	int SelectAllMapElment();

	//////////////////////////////////////////////////////////////////////////
	// 设置地图留边的宽度
	void SetOutputPicMargin(int nMargin)
	{
		m_nOutputPicMargin = nMargin;
	}

	//////////////////////////////////////////////////////////////////////////
	// 得到地图留边的宽度
	int GetOutputPicMargin()
	{
		return m_nOutputPicMargin;
	}

	//////////////////////////////////////////////////////////////////////////
	// 设置网格线的粗细
	void SetGridLineWeight(int nWeight)
	{
		m_nCanvasMapGridLineWeight = nWeight;
	}

	//////////////////////////////////////////////////////////////////////////
	// 得到网格线粗细
	int GetGridLineWeight()
	{
		return m_nCanvasMapGridLineWeight;
	}

	//////////////////////////////////////////////////////////////////////////
	// 设置当前属性页面的对象，用于显示不同的属性内容
	bool UpdatePropertyViewData();

	bool IsShowEveryMESelectionFrame();

	void SetShowEveryMESelectionFrame( bool bShowMEFrame );

	COLORREF GetMESelectionFrameColor();

	void SetMESelectionFrameColor(COLORREF rgb);

	//////////////////////////////////////////////////////////////////////////
	// 通知对应地图元件的属性修改（一般是修改了玩家人数后需要对元件属性做出响应）
	bool OnPlayerCountChange();

	//////////////////////////////////////////////////////////////////////////
	// 设置对应的图层显示与否
	// eLayer 对应要操作的图层
	// bVisible 设置是否可见
	void SetMapLayerVisible( ENUM_ME_TYPE eLayer, bool bVisible );

	//////////////////////////////////////////////////////////////////////////
	// 得到对应图层是否显示
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
	long m_nMapOffsetX;					// 地图X轴偏移
	long m_nMapOffsetY;					// 地图Y轴偏移

	long m_nMapOriginalOffsetX;			// 移动时记录地图X轴原始偏移
	long m_nMapOriginalOffsetY;			// 移动时记录地图Y轴原始偏移

	ENUM_EDIT_MODE m_eEditMode;			// 地图编辑模式枚举变量
	int m_nCanvasMapGridWidth;			// 地图画布网格的宽度
	int m_nCanvasMapGridHeight;			// 地图网不网格的高度
	int m_nCanvasMapGridDefaultWidth;	// 默认宽
	int m_nCanvasMapGridDefaultHeight;	// 默认高
	DWORD m_nCanvasMapGridLineStyle;	// 地图网格类型
	int m_nCanvasMapGridLineWeight;		// 地图网格线粗细
	
	double m_dZoom;						// 当前地图的缩放比

	MAPELEMENT* m_pSelectedMapElement;	// 当鼠标左键按下时选中的地图元件
	MAPELEMENT m_copiedMapElement;		// 复制的地图元件

	VectorMapElement m_vSelectedME;		// 当前选中（区域）地图元件数组
	
	CRect m_rSelected;					// 选中区域的外框(暂时不用，直接计算得到）
	VectorMapElement m_vCopiedME;		// 当前拷贝（区域）地图元件数组

	//////////////////////////////////////////////////////////////////////////
	// 撤销重做数组
	// 每个数组都是一个 VectorMapElementLayerList 列表
	// 每一个内容包含了所有的数据
	DequeStack m_dsRedo;	// 重做队列
	DequeStack m_dsUndo;	// 撤销队列

	int m_nMaxRedoCnts;		// 最大重做次数 暂时无用
	int m_nMaxUndoCnts;		// 最大撤销次数

	COLORREF m_colorCanvasBackGroundColor;	// 画布背景色
	COLORREF m_colorCanvasGridLineColor;	// 画布网格线颜色
	
	int m_nOutputPicMargin;	// 导出图片时对应的边沿留白
	
	bool m_bShowEveryMESelectionFrame;	// 是否显示每个选中的地图的边框
	COLORREF m_colorMESelectionFrame;	// 每个选中的地图的边框的颜色

	DWORD m_dwLayerVisibleFlag;	// 图层显示与否的标志位，默认32位，可以最多控制32个图层的显示与否



// Generated message map functions
protected:
	//{{AFX_MSG(CMapEditorDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//////////////////////////////////////////////////////////////////////////
	// 删除单个地图元件
	// 返回值	int  0表示成功， 非0表示失败
	//				-1表示无元件可删除
	int DeleteSelectedMapElement(MAPELEMENT* pME);

	//////////////////////////////////////////////////////////////////////////
	// 得到当前地图元件的一个拷贝
	// 数据是 new 出来需要释放
	VectorMapElementLayerList* GetCurrentMapFullDataLayerList();
	
	//////////////////////////////////////////////////////////////////////////
	// 释放内容的一个拷贝
	bool ReleaseVectorMapElementLayerListData(VectorMapElementLayerList* pVMELL);

	//////////////////////////////////////////////////////////////////////////
	// 把当前数据指向队列的末尾状态数据
	bool SetCurrentMapFromTail(DequeStack* pDS);

	//////////////////////////////////////////////////////////////////////////
	// 清空重做或者撤销队列
	bool ClearDequeStack(DequeStack* pDS);

	//////////////////////////////////////////////////////////////////////////
	// 此函数只负责重复的工作提炼
	void BindPropertyData(PropertyObjectCommObject* PropObj, MAPELEMENT* me);

	//////////////////////////////////////////////////////////////////////////
	// 根据当前玩家人数，设置对应地图对象的默认属性
	// 优先从Lua中处理数据，如果没有对应的处理过程，则默认走C++的数据默认处理
	void SetMapObjectDefaultData(MAPELEMENT *pME);
	
	//////////////////////////////////////////////////////////////////////////
	// 给一组MAPELEMENT的Vector循环设置属性
	void SetMapElementDefaultData(VectorMapElement* pVME);
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITORDOC_H__323B4819_89CD_4C87_A9E9_C94DDEE435F4__INCLUDED_)
