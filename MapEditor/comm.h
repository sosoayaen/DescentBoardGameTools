#ifndef _COMM_H_
#define _COMM_H_

#include "ximage.h"
#include <string>
#include <VECTOR>
#include <MAP>

//////////////////////////////////////////////////////////////////////////
// 公共结构体

// Every MapElement's rotate status
enum ENUM_ME_ROTATE 
{
	ME_ROTATE_ORIGINAL,	// original
		ME_ROTATE_LEFT,		// -π/2
		ME_ROTATE_RIGHT,	// π/2
		ME_ROTATE_180		// π
};

// 地图当前选择模式
enum ENUM_EDIT_MODE
{
	MAP_EDIT_SELECT,
		MAP_EDIT_DRAG_MOVE
};

enum ENUM_ME_TYPE
{
	ME_TYPE_TILE,
		ME_TYPE_DOOR,
		ME_TYPE_OBSTACLE,
		ME_TYPE_OBJECT,
		ME_TYPE_TREASURE,
		ME_TYPE_MONSTER,
		ME_TYPE_MONSTER_MASTER,
		ME_TYPE_HERO
};
// Map Element's original position in selected array
typedef struct tagOrigialPosition
{
	long x;
	long y;
}ORIGINALPOS;

typedef struct tagDoorProp
{
	// Door 属性
	short nDoorType;	// 门类型
} DOORPROP;

typedef struct tagTreasureProp
{
	// 一般物品
	// 价值
	int nSellValue;	// 卖出价格
} TREASUREPROP;

typedef struct tagCreatureProperty
{
	// Creature 属性
	short nHP;	// 血量
	short nSpeed;	// 移动力
	short nArmor;	// 护甲
	CString strSkills;	// 技能描述
} CREATUREPROP;

typedef struct tagHeroProperty: public CREATUREPROP
{
	short nFatigue;			// 疲劳值
	short nConquestValue;		// 死亡消耗
	short nFightingSkills;	// 战斗技巧
	short nWizardrySkills;	// 法术技巧
	short nSubterfugeSkills;	// 杂技技巧
	short nMagicTrait;		// 魔法攻击黑骰加成
	short nMeleeTrait;		// 近战攻击黑骰加成
	short nRangeTrait;		// 远程攻击黑骰加成
	short nGolds;				// 金币
	
	CString strMainHand; // 主手装备
	CString strOffHand; // 副手装备
	
	CString strArmor;	// 护甲装备
	CString strOther;	// 其他
	CString strPotions;	// 药物
	CString strPackage;	// 包裹
} HEROPROP;

typedef struct tagMonsterProperty: public CREATUREPROP
{
	// Monster 属性
	int nBonus;	// 奖励酬劳
	bool bMaster; // 是否为精英
	CString strAttackType; // 攻击类型
	CString strAttackDice;	// 攻击骰
} MONSTERPROP;

// Map Element struct
typedef struct tagMapElement
{
	// public:
	long nPx;				// x scale in map
	long nPy;				// y scale in map
	long nWidth;			// element's width 实际图片的长
	long nHeight;			// element's height 实际图片的宽
	long nID;				// element's ID
	CxImage* pImg;			// image point
	ORIGINALPOS posOri;		// original size of LButtonDown. Valid in selected		 
	ENUM_ME_TYPE nType;		// type enum
	short nAngle;			// rotate angle
	bool bSelected;			// selected status

	// 属性，根据nType的不同而不同
	CString strName;	// 名称
	CString strDesc;	// 描述
	CString strExpandsion;	// 地图元件的版本

// 	DOORPROP uPropDoor;
// 	TREASUREPROP uPropTreature;
// 	HEROPROP uPropHero;
// 	MONSTERPROP uPropMonster;

	// Door 属性
	int nDoorType;	// 门类型

	// 一般物品
	// 价值
	int nSellValue;	// 卖出价格

	// Creature 属性
	int nHP;	// 血量
	int nSpeed;	// 移动力
	int nArmor;	// 护甲
	CString strSkills;	// 技能描述
	CString strStatus;	// 状态

	// Monster 属性
	int nBonus;	// 奖励酬劳
	bool bMaster; // 是否为精英
	CString strAttackType; // 攻击类型
	CString strAttackDice;	// 攻击骰

	// Heroes 属性
	int nFatigue;			// 疲劳值
	int nConquestValue;		// 死亡消耗
	int nFightingSkills;	// 战斗技巧
	int nWizardrySkills;	// 法术技巧
	int nSubterfugeSkills;	// 杂技技巧
	int nMagicTrait;		// 魔法攻击黑骰加成
	int nMeleeTrait;		// 近战攻击黑骰加成
	int nRangeTrait;		// 远程攻击黑骰加成
	int nGolds;				// 金币
	CString strMainHand; // 主手装备
	CString strOffHand; // 副手装备
	CString strArmor;	// 护甲装备
	CString strOther;	// 其他
	CString strPotions;	// 药物
	CString strPackage;	// 包裹

	// public:
	tagMapElement()
	{
		nPx = 1;
		nPy = 1;
		nAngle = 0;
		nType = ME_TYPE_TILE;
		posOri.x = 0;
		posOri.y = 0;
		bSelected = false;
		pImg = NULL;

		nDoorType = 0;

		nSellValue = 0;

		nBonus = 0;

		nHP = 0;	// 血量
		nSpeed = 0;	// 移动力
		nArmor = 0;	// 护甲

		nFatigue = 0;			// 疲劳值
		nConquestValue = 0;		// 死亡消耗
		nFightingSkills = 0;	// 战斗技巧
		nWizardrySkills = 0;	// 法术技巧
		nSubterfugeSkills = 0;	// 杂技技巧
		nMagicTrait = 0;		// 魔法攻击黑骰加成
		nMeleeTrait = 0;		// 近战攻击黑骰加成
		nRangeTrait = 0;		// 远程攻击黑骰加成
		nGolds = 0;				// 金币
	}
	~tagMapElement()
	{
		pImg = NULL;
	}
}MAPELEMENT;

// 单元格中点
typedef struct tagElementCenterPoint
{
	long nX;
	long nY;
	tagElementCenterPoint()
	{
		nX = nY = 0;
	}
	tagElementCenterPoint(long x, long y)
	{
		nX = x;
		nY = y;
	}
}ElPoint;

// Image resource struct
typedef struct tagMapResource {
	long nID;		// Map image's ID
	long nWidth;	// Map image's width show in the screen, not include the transparent area
	long nHeight;	// Map image's height show in the screen, not include the transparent area
	ENUM_ME_TYPE nType;		// Map image's type
	CxImage* pImg;			// image pointer
	std::string strName;	// image's name
	std::string strPath;	// image's path
} MAPRESOURCE;

typedef std::vector<MAPELEMENT*> VectorMapElement;	// 地图模块集合结构

typedef std::vector<std::string> VectorMapElementType; // 记录名称到类型哈希表

// typedef std::map<long, CxImage*> MapID2Image;	// 地图图片资源对象集合，ID到图像

typedef std::map<long, MAPRESOURCE*> MapID2MR;	// 地图图片信息哈希表

typedef std::map<std::string, long> MapName2ENUM; // 地图类型的名称到枚举类型
//////////////////////////////////////////////////////////////////////////
// 公共宏定义
//////////////////////////////////////////////////////////////////////////
#define MIN_PLAYERS 2	// 最小玩家数量
#define MAX_PLAYERS 5	// 最大玩家数量
#define MIN_TILE_WIDTH 5	// 最小地图单元宽
#define MAX_TILE_WIDTH 100	// 最大地图单元宽
#define MIN_TILE_HEIGHT 5	// 最小地图单元高
#define MAX_TILE_HEIGHT 100	// 最大地图单元高
#define DEFAULT_UNDO_COUNT 20	// 默认撤销次数
#define DEFAULT_TILE_WIDTH 64	// 默认地图单元格宽
#define DEFAULT_TILE_HEIGHT 64	// 默认地图单元格高
#define DEFAULT_OUTPUT_PIC_MARGIN 64 // 导出图片边沿留白宽度

//////////////////////////////////////////////////////////////////////////
// 修改、重做、撤销对应操作宏定义
//////////////////////////////////////////////////////////////////////////

#define OPERATE_ADD          0
#define OPERATE_DELETE       1
#define OPERATE_MOVE         2
#define OPERATE_ROTATE_LEFT  3
#define OPERATE_ROTATE_RIGHT 4
#define OPERATE_PASTE        5
#define OPERATE_UNDO         6
#define OPERATE_REDO         7

//////////////////////////////////////////////////////////////////////////
// 自定义的消息
//////////////////////////////////////////////////////////////////////////

// 通知 DEBUG 窗口显示调试信息
#define WM_USER_DEBUG_INFO_SHOW (WM_USER + 1)
// VIEW 中鼠标的的坐标更新到状态栏通知
#define WM_USER_STATUSBAR_MOUSE_POSTION_UPDATE (WM_USER + 2)
// 更新当前地图的缩放比
#define WM_USER_STATUSBAR_ZOOMED_RATE_UPDATE (WM_USER + 3)
// 更新属性框
#define WM_USER_PROPERTY_VIEW_UPDATE (WM_USER + 4)

//////////////////////////////////////////////////////////////////////////
// 状态栏控件定义
//////////////////////////////////////////////////////////////////////////

#endif
