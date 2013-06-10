// PropertyObject.cpp: implementation of the CPropertyObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mapeditor.h"
#include "PropertyObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PropertyObjectMap::PropertyObjectMap()
{
	m_nPlayerCount = MIN_PLAYERS;
}

PropertyObjectMap::~PropertyObjectMap()
{

}

void PropertyObjectMap::GetProperties( EPropList& PropList )
{
	PropList.AddTab(_T("普通设置"));
	PropList.AddPropInt(this, _T("玩家人数"), &m_nPlayerCount, _T("人"), true, MIN_PLAYERS, MAX_PLAYERS)
		->SetComment(_T("人数一般为2-5人（包括OverLord），值不能小于2也不能大于5，"\
			"只能输入半角数字。玩家人数会影响地图怪物的属性，"\
			"人越多，怪物属性越强。"));
}

bool PropertyObjectMap::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChangeOK = true;
	
	return bChangeOK;
}

bool PropertyObjectMap::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = true;

	return bEnable;
}

PropertyObjectEditor::PropertyObjectEditor()
{
	m_strMapUnitWidthHeight = _T("[64×64]");
	m_bShowGridLine = true;
	m_bEditWidthHeight = false;
	m_nGridHeight = DEFAULT_TILE_HEIGHT;
	m_nGridWidth = DEFAULT_TILE_WIDTH;
	m_colorBackground = RGB(128, 128, 0);
	m_colorGridLine = RGB(0, 0, 0);
	m_nGridLineWeight = 1;
	m_nGridLineStyle = PS_DOT;
}

PropertyObjectEditor::~PropertyObjectEditor()
{
	
}

void PropertyObjectEditor::GetProperties( EPropList& PropList )
{
	//////////////////////////////////////////////////////////////////////////
	// 普通属性界面
	//
	PropList.AddTab(_T("普通"));
	PropList.AddPropCheck(this, _T("是否显示网格线"), &m_bShowGridLine)
		->SetComment(_T("设置是否显示地图编辑器的网格线"));
	PropList.AddPropColor(this, _T("网格线颜色"), &m_colorGridLine)
		->SetComment(_T("设置对应网格线的颜色，默认为灰色"));
	PropList.AddPropColor(this, _T("背景颜色"), &m_colorBackground)
		->SetComment(_T("设置当前编辑器的背景颜色"));
	PropList.AddPropCheck(this, _T("显示边框"), &m_bShowEverySelectedMapElementFrame)
		->SetComment(_T("是否显示每个被选中的元件的边框，默认为红色"));
	PropList.AddPropColor(this, _T("边框颜色"), &m_colorEverySelectedMapElementFrame)
		->SetComment(_T("默认为红色"));

	//////////////////////////////////////////////////////////////////////////
	// 高级属性界面
	//
	PropList.AddTab(_T("高级"));	
	PropList.AddPropCheck(this, _T("地图单元尺寸"), &m_bEditWidthHeight)
		->SetComment(_T("设置显示用的地图元件最小单元的长和高，\
		根据不同地图资源的大小调整，默认是64×64。\
		可接受长宽不等的矩形，但是需要所有的对齐格子都满足此比例。\
		注意：在了解此参数的功能前，请勿随意修改，切记！"));
	PropList.StepIndentIn();
		PropList.AddPropInt(this, _T("单元长"), &m_nGridWidth, _T("像素(px)"), true, MIN_TILE_WIDTH, MAX_TILE_WIDTH)
			->SetComment(_T("地图元件最小单位的长度，默认是64px，这个值需要根据载入\
			的地图资源的实际大小调整。目前设定可接受的值最小为5像素，最大为100像素"));
		PropList.AddPropInt(this, _T("单元高"), &m_nGridHeight, _T("像素(px)"), true, MIN_TILE_HEIGHT, MAX_TILE_HEIGHT)
			->SetComment(_T("地图元件最小单位的高度，默认是64px，这个值需要根据载入\
			的地图资源的实际大小调整。目前设定可接受的值最小为5像素，最大为100像素"));
	PropList.StepIndentOut();

	PropList.AddPropInt(this, _T("网格线粗细"), &m_nGridLineWeight,  _T("像素(px)"), true, 1, 100)
		->SetComment(_T("设置网格线的粗细，默认是1像素，基本不需要修改\
		这个粗细，太粗比较影响视觉效果"));
	PropList.AddPropCombo(this, _T("网格线画笔风格"), &m_nGridLineStyle)
		->AddString(_T("实线"), PS_SOLID)
		->AddString(_T("虚线"), PS_DASH)
		->AddString(_T("点"), PS_DOT)
		->AddString(_T("虚线点"), PS_DASHDOT)
		->AddString(_T("虚线点点"), PS_DASHDOTDOT)
		->SetComment(_T("设置网格线的画笔风格，可以选择实线、虚线、点、\
		虚线点和虚线点点这5种风格，默认为点"));
}

bool PropertyObjectEditor::PropertyChanging( const void* pProperty , void* pNewValue )
{
	return true;
}

bool PropertyObjectEditor::IsPropertyEnabled( const void* pProperty )
{
	bool bEnabled = true;
	if (pProperty == &m_nGridHeight || pProperty == &m_nGridWidth)
	{
		return m_bEditWidthHeight;
	}
	return bEnabled;
}

PropertyObjectCommObject::PropertyObjectCommObject()
{
	SetDefaultValue();
}

PropertyObjectCommObject::~PropertyObjectCommObject()
{
	
}

void PropertyObjectCommObject::GetProperties( EPropList& PropList )
{
	PropList.AddPropString(this, _T("对象名称"), m_pstrName)
		->SetComment(_T("元件名称"));
	PropList.AddPropString(this, _T("对象描述"), m_pstrDesc)
		->SetComment(_T("对象的描述内容"));
	PropList.AddPropInt(this, _T("X坐标"), &m_nPosX)
		->SetComment(_T("当前地图的X坐标位置，以地图的单元格为单位，非像素单位"));
	PropList.AddPropInt(this, _T("Y坐标"), &m_nPosY)
		->SetComment(_T("当前地图的Y坐标位置，以地图的单元格为单位，非像素单位"));
}

bool PropertyObjectCommObject::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChanged = true;
	
	if (m_bCommitChange == false)
	{
		return false;
	}

	if (pProperty == &m_nPosY || pProperty == &m_nPosX || pProperty == m_pstrExpandsion)
	{
		bChanged = false;
	}
	return true;
}

bool PropertyObjectCommObject::IsPropertyEnabled( const void* pProperty )
{
	bool bEnabled = true;

	if (!m_bEnable)
	{
		bEnabled = false;
	}
	return bEnabled;
}

bool PropertyObjectCommObject::SetDefaultValue()
{
	m_bEnable = true;
	m_bCommitChange = true;
	m_nPosX = 0;
	m_nPosY = 0;

	m_pstrName = NULL;
	m_pstrDesc = NULL;

	return true;
}

bool PropertyObjectCommObject::BindData( MAPELEMENT* pMe )
{
	bool bRet = false;
	if (pMe)
	{
		m_pstrName = &pMe->strName;
		m_pstrDesc = &pMe->strDesc;
		m_pstrExpandsion = &pMe->strExpandsion;
		bRet = true;
	}

	return bRet;
}


PropertyObjectTile::PropertyObjectTile():PropertyObjectCommObject()
{

}

PropertyObjectTile::~PropertyObjectTile()
{
	
}

void PropertyObjectTile::GetProperties( EPropList& PropList )
{
	PropertyObjectCommObject::GetProperties(PropList);
}

bool PropertyObjectTile::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = true;
	if (!m_bCommitChange)
	{
		bChange = false;
	}
	return bChange;
}

bool PropertyObjectTile::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	
	if (bEnable)
	{
		// 处理子类属性
		bEnable = true;
	}
	return bEnable;
}

bool PropertyObjectTile::SetDefaultValue()
{
	return true;
}

bool PropertyObjectTile::BindData( MAPELEMENT* pMe )
{
	return PropertyObjectCommObject::BindData(pMe);
}

void PropertyObjectCreature::GetProperties( EPropList& PropList )
{
	PropertyObjectCommObject::GetProperties(PropList);

	PropList.AddPropInt(this, _T("血量"), m_pnHP)
		->SetComment(_T("对应对象的生命值"));
	PropList.AddPropInt(this, _T("护甲"), m_pnArmor)
		->SetComment(_T("对应的防御值"));
	PropList.AddPropInt(this, _T("移动力"), m_pnSpeed)
		->SetComment(_T("移动距离"));
	PropList.AddPropString(this, _T("技能"), m_pstrSkills)
		->SetComment(_T("生物所拥有的能力"));
	PropList.AddPropString(this, _T("状态"), m_pstrStatus)
		->SetComment(_T("当前对象状态，如晕眩，中毒等等"));
}

bool PropertyObjectCreature::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCommObject::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// 处理子类的选项
		bChange = true;
	}
	return bChange;
}

bool PropertyObjectCreature::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	if (bEnable)
	{
		// 处理子类属性
		bEnable = true;
	}
	return bEnable;
}

bool PropertyObjectCreature::SetDefaultValue()
{
	PropertyObjectCommObject::SetDefaultValue();

	return true;
}

PropertyObjectCreature::PropertyObjectCreature():PropertyObjectCommObject()
{
	SetDefaultValue();
}

PropertyObjectCreature::~PropertyObjectCreature()
{
	
}

bool PropertyObjectCreature::BindData( MAPELEMENT* pMe )
{
	bool bRet = PropertyObjectCommObject::BindData(pMe);

//	CREATUREPROP* pCreatureProp = &pMe->uPropCreature;
	m_pnHP = &pMe->nHP;
	m_pnSpeed = &pMe->nSpeed;
	m_pnArmor = &pMe->nArmor;
	m_pstrSkills = &pMe->strSkills;
	m_pstrStatus = &pMe->strStatus;
	return bRet;

}

void PropertyObjectMonster::GetProperties( EPropList& PropList )
{
	PropertyObjectCreature::GetProperties(PropList);
	PropList.AddPropString(this, _T("攻击类型"), m_pstrAttackType)
		->SetComment(_T("攻击的类型，比如近战、远程、魔法等，有的怪物甚至还有随机掷骰决定"));
	PropList.AddPropString(this, _T("攻击骰"), m_pstrAttaciDice)
		->SetComment(_T("攻击骰的颜色和个数"));
	PropList.AddPropCheck(this, _T("精英"), m_pbIsMaster)
		->SetComment(_T("打勾表示此生物是精英怪物，否则是普通怪物"));
	PropList.AddPropInt(this, _T("报酬奖励"), m_pnBonus, _T("金币"))
		->SetComment(_T("击倒此怪物所获得的报酬，一般只有精英怪物会有报酬奖励"));
	PropList.AddPropString(this, _T("游戏版本"), m_pstrExpandsion)
		->SetComment(_T("怪物对应的游戏版本，JITD:Journy Into The Dark! AOD..."));
}

bool PropertyObjectMonster::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCreature::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// 处理子类的选项
		if (pProperty == m_pbIsMaster)
		{
			bChange = false;
		}
	}
	return bChange;
}

bool PropertyObjectMonster::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	
	if (bEnable)
	{
		// 处理子类属性
	}
	return bEnable;
}

bool PropertyObjectMonster::SetDefaultValue()
{
	PropertyObjectCreature::SetDefaultValue();

	return true;
}

PropertyObjectMonster::PropertyObjectMonster():PropertyObjectCreature()
{
	SetDefaultValue();
}

PropertyObjectMonster::~PropertyObjectMonster()
{
	
}

bool PropertyObjectMonster::BindData( MAPELEMENT* pMe )
{
	bool bRet = PropertyObjectCreature::BindData(pMe);

	if (bRet)
	{
//		MONSTERPROP* pMonsterProp = &pMe->uPropMonster;

		m_pnBonus = &pMe->nBonus;
		m_pbIsMaster = &pMe->bMaster;
		m_pstrAttackType = &pMe->strAttackType;
		m_pstrAttaciDice = &pMe->strAttackDice;
	}

	return bRet;
}

void PropertyObjectHero::GetProperties( EPropList& PropList )
{
	PropertyObjectCreature::GetProperties(PropList);
	PropList.AddPropInt(this, _T("疲劳值"), m_pnFatigue)
		->SetComment(_T(""));
	PropList.AddPropInt(this, _T("近战攻击黑骰"), m_pnMeleeTrait);
	PropList.AddPropInt(this, _T("远程攻击黑骰"), m_pnRangeTrait);
	PropList.AddPropInt(this, _T("魔法攻击黑骰"), m_pnMagicTrait);
	PropList.AddPropInt(this, _T("战斗技能"), m_pnFightingSkills);
	PropList.AddPropInt(this, _T("杂技技能"), m_pnSubterfugeSkills);
	PropList.AddPropInt(this, _T("魔法技能"), m_pnWizardrySkills);

	PropList.AddPropString(this, _T("左手装备"), m_pstrMainHand);
	PropList.AddPropString(this, _T("右手装备"), m_pstrOffHand);
	PropList.AddPropString(this, _T("护甲"), m_pstrArmor);
	PropList.AddPropString(this, _T("其他装备"), m_pstrOther)
		->SetComment(_T("最多只能装备两个其他装备"));
	PropList.AddPropString(this, _T("药剂"), m_pstrPotions)
		->SetComment(_T("最多同时装备3个药剂"));
	PropList.AddPropString(this, _T("包裹"), m_pstrPackage)
		->SetComment(_T("包裹内可以放置三件物品"));

	PropList.AddPropInt(this, _T("死亡消耗"), m_pnConquestValue, _T("胜利指示物"));
	PropList.AddPropInt(this, _T("财富"), m_pnGolds, _T("金币"));
	PropList.AddPropString(this, _T("游戏版本"), m_pstrExpandsion)
		->SetComment(_T("怪物对应的游戏版本，JITD:Journy Into The Dark! AOD..."));
}

bool PropertyObjectHero::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCreature::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// 处理子类的选项
		if (pProperty == m_pnConquestValue)
		{
			bChange = false;
		}
	}
	return bChange;
}

bool PropertyObjectHero::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	
	if (bEnable)
	{
		// 处理子类属性
	}
	return bEnable;
}

bool PropertyObjectHero::SetDefaultValue()
{
	PropertyObjectCreature::SetDefaultValue();

	return true;
}

bool PropertyObjectHero::BindData( MAPELEMENT* pMe )
{
	bool bRet = PropertyObjectCreature::BindData(pMe);
	
	if (bRet)
	{
//		HEROPROP* pHeroProp = &pMe->uPropHero;

		m_pnFatigue = &pMe->nFatigue;			// 疲劳值
		m_pnConquestValue = &pMe->nConquestValue;		// 死亡消耗
		m_pnFightingSkills = &pMe->nFightingSkills;	// 战斗技巧
		m_pnWizardrySkills = &pMe->nWizardrySkills;	// 法术技巧
		m_pnSubterfugeSkills = &pMe->nSubterfugeSkills;	// 杂技技巧
		m_pnMagicTrait = &pMe->nMagicTrait;		// 魔法攻击黑骰加成
		m_pnMeleeTrait = &pMe->nMeleeTrait;		// 近战攻击黑骰加成
		m_pnRangeTrait = &pMe->nRangeTrait;		// 远程攻击黑骰加成
		m_pnGolds = &pMe->nGolds;				// 金币
		
		m_pstrMainHand = &pMe->strMainHand; // 主手装备
		m_pstrOffHand = &pMe->strOffHand; // 副手装备
		
		m_pstrArmor = &pMe->strArmor;	// 护甲装备
		m_pstrOther = &pMe->strOther;	// 其他
		m_pstrPotions = &pMe->strPotions;	// 药物
		m_pstrPackage = &pMe->strPackage;	// 包裹
	}
	
	return bRet;
}

PropertyObjectHero::PropertyObjectHero():PropertyObjectCreature()
{
	SetDefaultValue();
}

PropertyObjectHero::~PropertyObjectHero()
{
	
}

void PropertyObjectDoor::GetProperties( EPropList& PropList )
{
	PropertyObjectCommObject::GetProperties(PropList);
	
	PropList.AddPropCombo(this, _T("门类型"), m_pnDoorType)
		->AddString(_T("普通门"), this->DOOR_TYPE_NORMAL)
		->AddSeparator()
		->AddString(_T("黄色符文门"), this->DOOR_TYPE_YELLOW)
		->AddString(_T("蓝色符文门"), this->DOOR_TYPE_BLUE)
		->AddString(_T("红色符文门"), this->DOOR_TYPE_RED)
		->SetComment(_T("当前门的类型，可以为普通的门（可随意打开和关闭），和对应颜色的符文门（需要有对应颜色符文才能刚打开）"));
}

bool PropertyObjectDoor::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCommObject::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// 处理子类的选项
		if (pProperty == m_pnDoorType)
		{
			// 门类型不能修改
			bChange = false;
		}
	}
	return bChange;
}

bool PropertyObjectDoor::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	if (bEnable)
	{
		// 处理子类的选项
		
	}
	return bEnable;
}

bool PropertyObjectDoor::SetDefaultValue()
{
	PropertyObjectCommObject::SetDefaultValue();

	return true;
}

PropertyObjectDoor::PropertyObjectDoor():PropertyObjectCommObject()
{
	SetDefaultValue();
}

PropertyObjectDoor::~PropertyObjectDoor()
{
	
}

bool PropertyObjectDoor::BindData( MAPELEMENT* pMe )
{
	bool bRet = PropertyObjectCommObject::BindData(pMe);

//	DOORPROP* pDoorProp = &pMe->uPropDoor;

	m_pnDoorType = &pMe->nDoorType;

	return bRet;
}

void PropertyObjectCombination::GetProperties( EPropList& PropList )
{
	PropertyObjectCommObject::GetProperties(PropList);
	
	PropList.AddPropInt(this, _T("组合对象数量"), &m_nObjCount)
		->SetComment(_T("当前组合中对象的数量"));
}

bool PropertyObjectCombination::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCommObject::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// 处理子类的选项
		bChange = true;
	}
	return bChange;
}

bool PropertyObjectCombination::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	if (bEnable)
	{
		// 处理子类的选项

		// 当前只有组合，都是默认不允许修改并且是只读的
		bEnable = false;
	}
	return bEnable;
}

bool PropertyObjectCombination::SetDefaultValue()
{
	PropertyObjectCommObject::SetDefaultValue();

	// 组合的信息不允许修改
	m_bCommitChange = false;
	m_bEnable = true;

	return true;
}

PropertyObjectCombination::PropertyObjectCombination():PropertyObjectCommObject()
{
	SetDefaultValue();
}

PropertyObjectCombination::~PropertyObjectCombination()
{
	
}

void PropertyObjectTreature::GetProperties( EPropList& PropList )
{
	PropertyObjectCommObject::GetProperties(PropList);

	PropList.AddPropInt(this, _T("价值"), m_pnSellValue, _T("金币"))
		->SetComment(_T("在商店卖出或者可换得的金币数量。"));
}

bool PropertyObjectTreature::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCommObject::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// 处理子类的选项
		bChange = true;
	}
	return bChange;
}

bool PropertyObjectTreature::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	if (bEnable)
	{
		// 处理子类的选项
		
		bEnable = true;
	}
	return bEnable;
}

bool PropertyObjectTreature::SetDefaultValue()
{
	PropertyObjectCommObject::SetDefaultValue();

	return true;
}

PropertyObjectTreature::PropertyObjectTreature():PropertyObjectCommObject()
{
	SetDefaultValue();
}

PropertyObjectTreature::~PropertyObjectTreature()
{
	
}

bool PropertyObjectTreature::BindData( MAPELEMENT* pMe )
{
	PropertyObjectCommObject::BindData(pMe);

//	TREASUREPROP* pTreatureProp = &pMe->uPropTreature;

	m_pnSellValue = &pMe->nSellValue;

	return true;
}

void PropertyObjectObstacle::GetProperties( EPropList& PropList )
{
	PropertyObjectCommObject::GetProperties(PropList);
}

bool PropertyObjectObstacle::PropertyChanging( const void* pProperty , void* pNewValue )
{	
	bool bChange = PropertyObjectCommObject::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// 处理子类的选项
		bChange = true;
	}
	return bChange;
}

bool PropertyObjectObstacle::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	if (bEnable)
	{
		// 处理子类的选项
		
		bEnable = true;
	}
	return bEnable;
}

bool PropertyObjectObstacle::SetDefaultValue()
{
	PropertyObjectCommObject::SetDefaultValue();
	// 不可通过
	return true;
}

PropertyObjectObstacle::PropertyObjectObstacle()
{
	SetDefaultValue();
}

PropertyObjectObstacle::~PropertyObjectObstacle()
{
	
}

bool PropertyObjectObstacle::BindData( MAPELEMENT* pMe )
{
	PropertyObjectCommObject::BindData(pMe);

	return true;
}

void PropertyObjectDefault::GetProperties( EPropList& PropList )
{
	PropList.AddTab(_T("地图属性"));
	PropList.AddPropInt(this, _T("玩家人数"), &m_nPlayers)
		->SetComment(_T("当前玩家人数，包括OverLord"));
	// PropList.AddPropInt(this, _T(), &m_strGameVersion);
	PropList.AddPropInt(this, _T("普通怪物数量"), &m_nMonsters)
		->SetComment(_T("地图上剩余的普通怪物的数量"));
	PropList.AddPropInt(this, _T("精英怪物数量"), &m_nMasters)
		->SetComment(_T("地图上剩余的精英怪物的数量"));
// 	PropList.AddPropInt(this, _T("宝箱数量"), &m_nChests)
// 		->SetComment(_T("剩余未开宝箱数量"));

	PropList.AddTab(_T("OverLord"));
	PropList.AddPropInt(this, _T("Threat Token"), m_pnThreatToken)
		->SetComment(_T("OverLord拥有的召唤值，OL可以用来支付使用卡片费用。这里的数量只用作记录用"));
// 	for (int nIdx = 0; nIdx <= 7; nIdx++)
// 	{
// 		PropList.AddPropString(this, _T(""), );
// 	}

	PropList.AddTab(_T("玩家"));
	PropList.AddPropInt(this, _T("Conquest Token"), m_pnConquestToken)
		->SetComment(_T("Hero玩家所拥有的能量值，小于等于0则表示游戏结束，OL获胜。这里的数量之用作记录用。"));
//	PropList.AddPropCheck(this, _T(), )
}

PropertyObjectDefault::PropertyObjectDefault()
{
	m_nChests = 0;
	m_nMasters = 0;
	m_nMonsters = 0;
	m_nPlayers = 0;

	m_pnConquestToken = NULL;
	m_pnThreatToken = NULL;
	
}

bool PropertyObjectDefault::PropertyChanging( const void* pProperty , void* pNewValue )
{
	if (pProperty == m_pnThreatToken || pProperty == m_pnConquestToken)
	{
		return true;
	}
	return false;
}
