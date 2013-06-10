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
	PropList.AddTab(_T("��ͨ����"));
	PropList.AddPropInt(this, _T("�������"), &m_nPlayerCount, _T("��"), true, MIN_PLAYERS, MAX_PLAYERS)
		->SetComment(_T("����һ��Ϊ2-5�ˣ�����OverLord����ֵ����С��2Ҳ���ܴ���5��"\
			"ֻ�����������֡����������Ӱ���ͼ��������ԣ�"\
			"��Խ�࣬��������Խǿ��"));
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
	m_strMapUnitWidthHeight = _T("[64��64]");
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
	// ��ͨ���Խ���
	//
	PropList.AddTab(_T("��ͨ"));
	PropList.AddPropCheck(this, _T("�Ƿ���ʾ������"), &m_bShowGridLine)
		->SetComment(_T("�����Ƿ���ʾ��ͼ�༭����������"));
	PropList.AddPropColor(this, _T("��������ɫ"), &m_colorGridLine)
		->SetComment(_T("���ö�Ӧ�����ߵ���ɫ��Ĭ��Ϊ��ɫ"));
	PropList.AddPropColor(this, _T("������ɫ"), &m_colorBackground)
		->SetComment(_T("���õ�ǰ�༭���ı�����ɫ"));
	PropList.AddPropCheck(this, _T("��ʾ�߿�"), &m_bShowEverySelectedMapElementFrame)
		->SetComment(_T("�Ƿ���ʾÿ����ѡ�е�Ԫ���ı߿�Ĭ��Ϊ��ɫ"));
	PropList.AddPropColor(this, _T("�߿���ɫ"), &m_colorEverySelectedMapElementFrame)
		->SetComment(_T("Ĭ��Ϊ��ɫ"));

	//////////////////////////////////////////////////////////////////////////
	// �߼����Խ���
	//
	PropList.AddTab(_T("�߼�"));	
	PropList.AddPropCheck(this, _T("��ͼ��Ԫ�ߴ�"), &m_bEditWidthHeight)
		->SetComment(_T("������ʾ�õĵ�ͼԪ����С��Ԫ�ĳ��͸ߣ�\
		���ݲ�ͬ��ͼ��Դ�Ĵ�С������Ĭ����64��64��\
		�ɽ��ܳ����ȵľ��Σ�������Ҫ���еĶ�����Ӷ�����˱�����\
		ע�⣺���˽�˲����Ĺ���ǰ�����������޸ģ��мǣ�"));
	PropList.StepIndentIn();
		PropList.AddPropInt(this, _T("��Ԫ��"), &m_nGridWidth, _T("����(px)"), true, MIN_TILE_WIDTH, MAX_TILE_WIDTH)
			->SetComment(_T("��ͼԪ����С��λ�ĳ��ȣ�Ĭ����64px�����ֵ��Ҫ��������\
			�ĵ�ͼ��Դ��ʵ�ʴ�С������Ŀǰ�趨�ɽ��ܵ�ֵ��СΪ5���أ����Ϊ100����"));
		PropList.AddPropInt(this, _T("��Ԫ��"), &m_nGridHeight, _T("����(px)"), true, MIN_TILE_HEIGHT, MAX_TILE_HEIGHT)
			->SetComment(_T("��ͼԪ����С��λ�ĸ߶ȣ�Ĭ����64px�����ֵ��Ҫ��������\
			�ĵ�ͼ��Դ��ʵ�ʴ�С������Ŀǰ�趨�ɽ��ܵ�ֵ��СΪ5���أ����Ϊ100����"));
	PropList.StepIndentOut();

	PropList.AddPropInt(this, _T("�����ߴ�ϸ"), &m_nGridLineWeight,  _T("����(px)"), true, 1, 100)
		->SetComment(_T("���������ߵĴ�ϸ��Ĭ����1���أ���������Ҫ�޸�\
		�����ϸ��̫�ֱȽ�Ӱ���Ӿ�Ч��"));
	PropList.AddPropCombo(this, _T("�����߻��ʷ��"), &m_nGridLineStyle)
		->AddString(_T("ʵ��"), PS_SOLID)
		->AddString(_T("����"), PS_DASH)
		->AddString(_T("��"), PS_DOT)
		->AddString(_T("���ߵ�"), PS_DASHDOT)
		->AddString(_T("���ߵ��"), PS_DASHDOTDOT)
		->SetComment(_T("���������ߵĻ��ʷ�񣬿���ѡ��ʵ�ߡ����ߡ��㡢\
		���ߵ�����ߵ����5�ַ��Ĭ��Ϊ��"));
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
	PropList.AddPropString(this, _T("��������"), m_pstrName)
		->SetComment(_T("Ԫ������"));
	PropList.AddPropString(this, _T("��������"), m_pstrDesc)
		->SetComment(_T("�������������"));
	PropList.AddPropInt(this, _T("X����"), &m_nPosX)
		->SetComment(_T("��ǰ��ͼ��X����λ�ã��Ե�ͼ�ĵ�Ԫ��Ϊ��λ�������ص�λ"));
	PropList.AddPropInt(this, _T("Y����"), &m_nPosY)
		->SetComment(_T("��ǰ��ͼ��Y����λ�ã��Ե�ͼ�ĵ�Ԫ��Ϊ��λ�������ص�λ"));
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
		// ������������
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

	PropList.AddPropInt(this, _T("Ѫ��"), m_pnHP)
		->SetComment(_T("��Ӧ���������ֵ"));
	PropList.AddPropInt(this, _T("����"), m_pnArmor)
		->SetComment(_T("��Ӧ�ķ���ֵ"));
	PropList.AddPropInt(this, _T("�ƶ���"), m_pnSpeed)
		->SetComment(_T("�ƶ�����"));
	PropList.AddPropString(this, _T("����"), m_pstrSkills)
		->SetComment(_T("������ӵ�е�����"));
	PropList.AddPropString(this, _T("״̬"), m_pstrStatus)
		->SetComment(_T("��ǰ����״̬������ѣ���ж��ȵ�"));
}

bool PropertyObjectCreature::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCommObject::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// ���������ѡ��
		bChange = true;
	}
	return bChange;
}

bool PropertyObjectCreature::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	if (bEnable)
	{
		// ������������
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
	PropList.AddPropString(this, _T("��������"), m_pstrAttackType)
		->SetComment(_T("���������ͣ������ս��Զ�̡�ħ���ȣ��еĹ����������������������"));
	PropList.AddPropString(this, _T("������"), m_pstrAttaciDice)
		->SetComment(_T("����������ɫ�͸���"));
	PropList.AddPropCheck(this, _T("��Ӣ"), m_pbIsMaster)
		->SetComment(_T("�򹴱�ʾ�������Ǿ�Ӣ�����������ͨ����"));
	PropList.AddPropInt(this, _T("���꽱��"), m_pnBonus, _T("���"))
		->SetComment(_T("�����˹�������õı��꣬һ��ֻ�о�Ӣ������б��꽱��"));
	PropList.AddPropString(this, _T("��Ϸ�汾"), m_pstrExpandsion)
		->SetComment(_T("�����Ӧ����Ϸ�汾��JITD:Journy Into The Dark! AOD..."));
}

bool PropertyObjectMonster::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCreature::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// ���������ѡ��
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
		// ������������
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
	PropList.AddPropInt(this, _T("ƣ��ֵ"), m_pnFatigue)
		->SetComment(_T(""));
	PropList.AddPropInt(this, _T("��ս��������"), m_pnMeleeTrait);
	PropList.AddPropInt(this, _T("Զ�̹�������"), m_pnRangeTrait);
	PropList.AddPropInt(this, _T("ħ����������"), m_pnMagicTrait);
	PropList.AddPropInt(this, _T("ս������"), m_pnFightingSkills);
	PropList.AddPropInt(this, _T("�Ӽ�����"), m_pnSubterfugeSkills);
	PropList.AddPropInt(this, _T("ħ������"), m_pnWizardrySkills);

	PropList.AddPropString(this, _T("����װ��"), m_pstrMainHand);
	PropList.AddPropString(this, _T("����װ��"), m_pstrOffHand);
	PropList.AddPropString(this, _T("����"), m_pstrArmor);
	PropList.AddPropString(this, _T("����װ��"), m_pstrOther)
		->SetComment(_T("���ֻ��װ����������װ��"));
	PropList.AddPropString(this, _T("ҩ��"), m_pstrPotions)
		->SetComment(_T("���ͬʱװ��3��ҩ��"));
	PropList.AddPropString(this, _T("����"), m_pstrPackage)
		->SetComment(_T("�����ڿ��Է���������Ʒ"));

	PropList.AddPropInt(this, _T("��������"), m_pnConquestValue, _T("ʤ��ָʾ��"));
	PropList.AddPropInt(this, _T("�Ƹ�"), m_pnGolds, _T("���"));
	PropList.AddPropString(this, _T("��Ϸ�汾"), m_pstrExpandsion)
		->SetComment(_T("�����Ӧ����Ϸ�汾��JITD:Journy Into The Dark! AOD..."));
}

bool PropertyObjectHero::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCreature::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// ���������ѡ��
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
		// ������������
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

		m_pnFatigue = &pMe->nFatigue;			// ƣ��ֵ
		m_pnConquestValue = &pMe->nConquestValue;		// ��������
		m_pnFightingSkills = &pMe->nFightingSkills;	// ս������
		m_pnWizardrySkills = &pMe->nWizardrySkills;	// ��������
		m_pnSubterfugeSkills = &pMe->nSubterfugeSkills;	// �Ӽ�����
		m_pnMagicTrait = &pMe->nMagicTrait;		// ħ�����������ӳ�
		m_pnMeleeTrait = &pMe->nMeleeTrait;		// ��ս���������ӳ�
		m_pnRangeTrait = &pMe->nRangeTrait;		// Զ�̹��������ӳ�
		m_pnGolds = &pMe->nGolds;				// ���
		
		m_pstrMainHand = &pMe->strMainHand; // ����װ��
		m_pstrOffHand = &pMe->strOffHand; // ����װ��
		
		m_pstrArmor = &pMe->strArmor;	// ����װ��
		m_pstrOther = &pMe->strOther;	// ����
		m_pstrPotions = &pMe->strPotions;	// ҩ��
		m_pstrPackage = &pMe->strPackage;	// ����
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
	
	PropList.AddPropCombo(this, _T("������"), m_pnDoorType)
		->AddString(_T("��ͨ��"), this->DOOR_TYPE_NORMAL)
		->AddSeparator()
		->AddString(_T("��ɫ������"), this->DOOR_TYPE_YELLOW)
		->AddString(_T("��ɫ������"), this->DOOR_TYPE_BLUE)
		->AddString(_T("��ɫ������"), this->DOOR_TYPE_RED)
		->SetComment(_T("��ǰ�ŵ����ͣ�����Ϊ��ͨ���ţ�������򿪺͹رգ����Ͷ�Ӧ��ɫ�ķ����ţ���Ҫ�ж�Ӧ��ɫ���Ĳ��ܸմ򿪣�"));
}

bool PropertyObjectDoor::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCommObject::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// ���������ѡ��
		if (pProperty == m_pnDoorType)
		{
			// �����Ͳ����޸�
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
		// ���������ѡ��
		
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
	
	PropList.AddPropInt(this, _T("��϶�������"), &m_nObjCount)
		->SetComment(_T("��ǰ����ж��������"));
}

bool PropertyObjectCombination::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCommObject::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// ���������ѡ��
		bChange = true;
	}
	return bChange;
}

bool PropertyObjectCombination::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	if (bEnable)
	{
		// ���������ѡ��

		// ��ǰֻ����ϣ�����Ĭ�ϲ������޸Ĳ�����ֻ����
		bEnable = false;
	}
	return bEnable;
}

bool PropertyObjectCombination::SetDefaultValue()
{
	PropertyObjectCommObject::SetDefaultValue();

	// ��ϵ���Ϣ�������޸�
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

	PropList.AddPropInt(this, _T("��ֵ"), m_pnSellValue, _T("���"))
		->SetComment(_T("���̵��������߿ɻ��õĽ��������"));
}

bool PropertyObjectTreature::PropertyChanging( const void* pProperty , void* pNewValue )
{
	bool bChange = PropertyObjectCommObject::PropertyChanging(pProperty, pNewValue);
	if (bChange)
	{
		// ���������ѡ��
		bChange = true;
	}
	return bChange;
}

bool PropertyObjectTreature::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	if (bEnable)
	{
		// ���������ѡ��
		
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
		// ���������ѡ��
		bChange = true;
	}
	return bChange;
}

bool PropertyObjectObstacle::IsPropertyEnabled( const void* pProperty )
{
	bool bEnable = PropertyObjectCommObject::IsPropertyEnabled(pProperty);
	if (bEnable)
	{
		// ���������ѡ��
		
		bEnable = true;
	}
	return bEnable;
}

bool PropertyObjectObstacle::SetDefaultValue()
{
	PropertyObjectCommObject::SetDefaultValue();
	// ����ͨ��
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
	PropList.AddTab(_T("��ͼ����"));
	PropList.AddPropInt(this, _T("�������"), &m_nPlayers)
		->SetComment(_T("��ǰ�������������OverLord"));
	// PropList.AddPropInt(this, _T(), &m_strGameVersion);
	PropList.AddPropInt(this, _T("��ͨ��������"), &m_nMonsters)
		->SetComment(_T("��ͼ��ʣ�����ͨ���������"));
	PropList.AddPropInt(this, _T("��Ӣ��������"), &m_nMasters)
		->SetComment(_T("��ͼ��ʣ��ľ�Ӣ���������"));
// 	PropList.AddPropInt(this, _T("��������"), &m_nChests)
// 		->SetComment(_T("ʣ��δ����������"));

	PropList.AddTab(_T("OverLord"));
	PropList.AddPropInt(this, _T("Threat Token"), m_pnThreatToken)
		->SetComment(_T("OverLordӵ�е��ٻ�ֵ��OL��������֧��ʹ�ÿ�Ƭ���á����������ֻ������¼��"));
// 	for (int nIdx = 0; nIdx <= 7; nIdx++)
// 	{
// 		PropList.AddPropString(this, _T(""), );
// 	}

	PropList.AddTab(_T("���"));
	PropList.AddPropInt(this, _T("Conquest Token"), m_pnConquestToken)
		->SetComment(_T("Hero�����ӵ�е�����ֵ��С�ڵ���0���ʾ��Ϸ������OL��ʤ�����������֮������¼�á�"));
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
