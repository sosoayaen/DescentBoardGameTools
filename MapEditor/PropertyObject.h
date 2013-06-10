// PropertyObject.h: interface for the CPropertyObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTYOBJECT_H__A0716DCF_1DA3_4ECD_81D4_C4BE7959A616__INCLUDED_)
#define AFX_PROPERTYOBJECT_H__A0716DCF_1DA3_4ECD_81D4_C4BE7959A616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPropertyHost.h"

class PropertyObjectMap : public IPropertyHost  
{
// Constructor	
public:
	PropertyObjectMap();
	virtual ~PropertyObjectMap();
// Member
	int m_nPlayerCount;
// Implementation
	//
	// IPropertyHost
	//
    virtual void GetProperties( EPropList& PropList );
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	virtual bool IsPropertyEnabled( const void* pProperty );

};

class PropertyObjectEditor : public IPropertyHost
{
// Constructor
public:
	PropertyObjectEditor();
	virtual ~PropertyObjectEditor();
// Member
	// 普通属性
	bool m_bShowGridLine;	// 是否显示网格线
	COLORREF m_colorGridLine; // 网格线颜色
	COLORREF m_colorBackground;	// 背景颜色
	bool m_bShowEverySelectedMapElementFrame; // 是否显示选择的元件的边框
	COLORREF m_colorEverySelectedMapElementFrame; // 选中的每个元件边框的颜色 

	int m_nOutputPicMargin;	// 图片导出留边宽度

	// 高级属性
	bool m_bEditWidthHeight;	// 地图单元宽和高是否可编辑
	int m_nGridWidth;		// 网格线宽
	int m_nGridHeight;		// 网格线高

	int m_nGridLineWeight;	// 网格线粗细
	int m_nGridLineStyle;	// 网格线风格

	CString m_strMapUnitWidthHeight;
// Implementation
	//
	// IPropertyHost
	//
    virtual void GetProperties( EPropList& PropList );
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	virtual bool IsPropertyEnabled( const void* pProperty );
};


//////////////////////////////////////////////////////////////////////////
// View中的属性框中的不同对象的属性内容

// 默认，显示当前地图属性标签
class PropertyObjectDefault : public IPropertyHost
{
public:
	PropertyObjectDefault();
	virtual ~PropertyObjectDefault(){}

public:
	int m_nPlayers;	// 玩家个数
	CString m_strGameVersion;	// 当前需要的最低版本
	int m_nMonsters; // 普通怪物个数
	int m_nMasters;	// 精英怪个数
	int m_nChests; // 宝箱个数
	int* m_pnThreatToken;	// 威胁指示物个数 OL用
	int* m_pnConquestToken;	// 胜利（征服）指示物

	//
	// IPropertyHost
	//
    virtual void GetProperties( EPropList& PropList );
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
};

// 地图对象的基类，集合了所有共有属性
class PropertyObjectCommObject : public IPropertyHost  
{
// Constructor	
public:
	PropertyObjectCommObject();
	virtual ~PropertyObjectCommObject();
// Member
	CString* m_pstrName;	// 元件名称
	CString* m_pstrDesc;	// 描述
	CString* m_pstrExpandsion; // 版本
	
	int m_nType;	// 地图元件类型(不可编辑）
	int m_nPosX;	// 当前地图对象的X位置（不可编辑）
	int m_nPosY;	// 当前地图对象的Y位置（不可编辑）
	bool m_bPassable;	// 是否可经过

	bool m_bEnable;	// 整体属性是否可编辑
	bool m_bCommitChange; // 是否提交修改
	
public:
// Implementation
	//
	// IPropertyHost
	//
    virtual void GetProperties( EPropList& PropList );
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	virtual bool IsPropertyEnabled( const void* pProperty );

	//////////////////////////////////////////////////////////////////////////
	// 设置默认值
	virtual bool SetDefaultValue();

	//////////////////////////////////////////////////////////////////////////
	// 绑定数据
	virtual bool BindData(MAPELEMENT* pMe);
	
};

class PropertyObjectTile : public PropertyObjectCommObject
{
public:
// Constructor	
	PropertyObjectTile();
	virtual ~PropertyObjectTile();

// Implementation
	//
	// IPropertyHost
	//
    virtual void GetProperties( EPropList& PropList );
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	virtual bool IsPropertyEnabled( const void* pProperty );

	//////////////////////////////////////////////////////////////////////////
	// 设置默认值
	virtual bool SetDefaultValue();

	//////////////////////////////////////////////////////////////////////////
	// 绑定数据
	virtual bool BindData(MAPELEMENT* pMe);
};

// 门，分为符文门或者普通门 
class PropertyObjectDoor : public PropertyObjectCommObject
{
public:
// Constructor
	PropertyObjectDoor();
	virtual ~PropertyObjectDoor();

	enum {
		DOOR_TYPE_NORMAL,
			DOOR_TYPE_YELLOW,
			DOOR_TYPE_BLUE,
			DOOR_TYPE_RED
	};
// Member
	int* m_pnDoorType; // 符文的颜色属性

// Implementation
	//
	// IPropertyHost
	//
	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	//////////////////////////////////////////////////////////////////////////
	// 设置默认值	
	virtual bool SetDefaultValue();

	//////////////////////////////////////////////////////////////////////////
	// 绑定数据
	virtual bool BindData(MAPELEMENT* pMe);
};

// 生物属性，共有的为血量、护甲、移动力、技能
class PropertyObjectCreature : public PropertyObjectCommObject
{
public:
	PropertyObjectCreature();
	virtual ~PropertyObjectCreature();

	int* m_pnHP;
	int* m_pnArmor;
	int* m_pnSpeed;

//	CString* m_pstrExpandsion; // 怪物对应的版本
	
	CString* m_pstrSkills; // 拥有的技能描述
	CString* m_pstrStatus;	// 状态，如stun，web等
// Implementation
	//
	// IPropertyHost
	//
	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	//////////////////////////////////////////////////////////////////////////
	// 设置默认值	
	virtual bool SetDefaultValue();

	//////////////////////////////////////////////////////////////////////////
	// 绑定数据
	virtual bool BindData(MAPELEMENT* pMe);
};

// 怪物
class PropertyObjectMonster: public PropertyObjectCreature
{
public:
	PropertyObjectMonster();
	virtual ~PropertyObjectMonster();

	CString* m_pstrAttackType; // 攻击类型
	CString* m_pstrAttaciDice; // 攻击骰
	bool* m_pbIsMaster;	// 是否精英
	int* m_pnBonus;	// 奖励
	
	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	virtual bool SetDefaultValue();
	
	virtual bool BindData( MAPELEMENT* pMe );
};

// 英雄
class PropertyObjectHero: public PropertyObjectCreature
{
public:
	PropertyObjectHero();
	virtual ~PropertyObjectHero();

	int* m_pnFatigue;			// 疲劳值
	int* m_pnConquestValue;		// 死亡消耗
	int* m_pnFightingSkills;	// 战斗技巧
	int* m_pnWizardrySkills;	// 法术技巧
	int* m_pnSubterfugeSkills;	// 杂技技巧
	int* m_pnMagicTrait;		// 魔法攻击黑骰加成
	int* m_pnMeleeTrait;		// 近战攻击黑骰加成
	int* m_pnRangeTrait;		// 远程攻击黑骰加成
	int* m_pnGolds;				// 金币

	CString* m_pstrMainHand; // 主手装备
	CString* m_pstrOffHand; // 副手装备

	CString* m_pstrArmor;	// 护甲装备
	CString* m_pstrOther;	// 其他
	CString* m_pstrPotions;	// 药物
	CString* m_pstrPackage;	// 包裹

	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	virtual bool SetDefaultValue();
	
	virtual bool BindData( MAPELEMENT* pMe );
};

// 组合
class PropertyObjectCombination: public PropertyObjectCommObject
{
public:
	PropertyObjectCombination();
	virtual ~PropertyObjectCombination();

	int m_nObjCount;	// 组合内的对象数量
	
	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	virtual bool SetDefaultValue();

};

class PropertyObjectTreature : public PropertyObjectCommObject
{
public:
	PropertyObjectTreature();
	virtual ~PropertyObjectTreature();

public:
	int* m_pnSellValue;	// 卖出价格

public:
	
	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	virtual bool SetDefaultValue();

	virtual bool BindData(MAPELEMENT* pMe);
};

class PropertyObjectObstacle : public PropertyObjectCommObject
{
public:
	PropertyObjectObstacle();
	virtual ~PropertyObjectObstacle();

public:
	
	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	virtual bool SetDefaultValue();
	
	virtual bool BindData( MAPELEMENT* pMe );
};
#endif // !defined(AFX_PROPERTYOBJECT_H__A0716DCF_1DA3_4ECD_81D4_C4BE7959A616__INCLUDED_)
