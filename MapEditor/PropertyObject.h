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
	// ��ͨ����
	bool m_bShowGridLine;	// �Ƿ���ʾ������
	COLORREF m_colorGridLine; // ��������ɫ
	COLORREF m_colorBackground;	// ������ɫ
	bool m_bShowEverySelectedMapElementFrame; // �Ƿ���ʾѡ���Ԫ���ı߿�
	COLORREF m_colorEverySelectedMapElementFrame; // ѡ�е�ÿ��Ԫ���߿����ɫ 

	int m_nOutputPicMargin;	// ͼƬ�������߿��

	// �߼�����
	bool m_bEditWidthHeight;	// ��ͼ��Ԫ��͸��Ƿ�ɱ༭
	int m_nGridWidth;		// �����߿�
	int m_nGridHeight;		// �����߸�

	int m_nGridLineWeight;	// �����ߴ�ϸ
	int m_nGridLineStyle;	// �����߷��

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
// View�е����Կ��еĲ�ͬ�������������

// Ĭ�ϣ���ʾ��ǰ��ͼ���Ա�ǩ
class PropertyObjectDefault : public IPropertyHost
{
public:
	PropertyObjectDefault();
	virtual ~PropertyObjectDefault(){}

public:
	int m_nPlayers;	// ��Ҹ���
	CString m_strGameVersion;	// ��ǰ��Ҫ����Ͱ汾
	int m_nMonsters; // ��ͨ�������
	int m_nMasters;	// ��Ӣ�ָ���
	int m_nChests; // �������
	int* m_pnThreatToken;	// ��вָʾ����� OL��
	int* m_pnConquestToken;	// ʤ����������ָʾ��

	//
	// IPropertyHost
	//
    virtual void GetProperties( EPropList& PropList );
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
};

// ��ͼ����Ļ��࣬���������й�������
class PropertyObjectCommObject : public IPropertyHost  
{
// Constructor	
public:
	PropertyObjectCommObject();
	virtual ~PropertyObjectCommObject();
// Member
	CString* m_pstrName;	// Ԫ������
	CString* m_pstrDesc;	// ����
	CString* m_pstrExpandsion; // �汾
	
	int m_nType;	// ��ͼԪ������(���ɱ༭��
	int m_nPosX;	// ��ǰ��ͼ�����Xλ�ã����ɱ༭��
	int m_nPosY;	// ��ǰ��ͼ�����Yλ�ã����ɱ༭��
	bool m_bPassable;	// �Ƿ�ɾ���

	bool m_bEnable;	// ���������Ƿ�ɱ༭
	bool m_bCommitChange; // �Ƿ��ύ�޸�
	
public:
// Implementation
	//
	// IPropertyHost
	//
    virtual void GetProperties( EPropList& PropList );
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	virtual bool IsPropertyEnabled( const void* pProperty );

	//////////////////////////////////////////////////////////////////////////
	// ����Ĭ��ֵ
	virtual bool SetDefaultValue();

	//////////////////////////////////////////////////////////////////////////
	// ������
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
	// ����Ĭ��ֵ
	virtual bool SetDefaultValue();

	//////////////////////////////////////////////////////////////////////////
	// ������
	virtual bool BindData(MAPELEMENT* pMe);
};

// �ţ���Ϊ�����Ż�����ͨ�� 
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
	int* m_pnDoorType; // ���ĵ���ɫ����

// Implementation
	//
	// IPropertyHost
	//
	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	//////////////////////////////////////////////////////////////////////////
	// ����Ĭ��ֵ	
	virtual bool SetDefaultValue();

	//////////////////////////////////////////////////////////////////////////
	// ������
	virtual bool BindData(MAPELEMENT* pMe);
};

// �������ԣ����е�ΪѪ�������ס��ƶ���������
class PropertyObjectCreature : public PropertyObjectCommObject
{
public:
	PropertyObjectCreature();
	virtual ~PropertyObjectCreature();

	int* m_pnHP;
	int* m_pnArmor;
	int* m_pnSpeed;

//	CString* m_pstrExpandsion; // �����Ӧ�İ汾
	
	CString* m_pstrSkills; // ӵ�еļ�������
	CString* m_pstrStatus;	// ״̬����stun��web��
// Implementation
	//
	// IPropertyHost
	//
	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	//////////////////////////////////////////////////////////////////////////
	// ����Ĭ��ֵ	
	virtual bool SetDefaultValue();

	//////////////////////////////////////////////////////////////////////////
	// ������
	virtual bool BindData(MAPELEMENT* pMe);
};

// ����
class PropertyObjectMonster: public PropertyObjectCreature
{
public:
	PropertyObjectMonster();
	virtual ~PropertyObjectMonster();

	CString* m_pstrAttackType; // ��������
	CString* m_pstrAttaciDice; // ������
	bool* m_pbIsMaster;	// �Ƿ�Ӣ
	int* m_pnBonus;	// ����
	
	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	virtual bool SetDefaultValue();
	
	virtual bool BindData( MAPELEMENT* pMe );
};

// Ӣ��
class PropertyObjectHero: public PropertyObjectCreature
{
public:
	PropertyObjectHero();
	virtual ~PropertyObjectHero();

	int* m_pnFatigue;			// ƣ��ֵ
	int* m_pnConquestValue;		// ��������
	int* m_pnFightingSkills;	// ս������
	int* m_pnWizardrySkills;	// ��������
	int* m_pnSubterfugeSkills;	// �Ӽ�����
	int* m_pnMagicTrait;		// ħ�����������ӳ�
	int* m_pnMeleeTrait;		// ��ս���������ӳ�
	int* m_pnRangeTrait;		// Զ�̹��������ӳ�
	int* m_pnGolds;				// ���

	CString* m_pstrMainHand; // ����װ��
	CString* m_pstrOffHand; // ����װ��

	CString* m_pstrArmor;	// ����װ��
	CString* m_pstrOther;	// ����
	CString* m_pstrPotions;	// ҩ��
	CString* m_pstrPackage;	// ����

	virtual void GetProperties( EPropList& PropList );
	
	virtual bool PropertyChanging( const void* pProperty , void* pNewValue );
	
	virtual bool IsPropertyEnabled( const void* pProperty );
	
	virtual bool SetDefaultValue();
	
	virtual bool BindData( MAPELEMENT* pMe );
};

// ���
class PropertyObjectCombination: public PropertyObjectCommObject
{
public:
	PropertyObjectCombination();
	virtual ~PropertyObjectCombination();

	int m_nObjCount;	// ����ڵĶ�������
	
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
	int* m_pnSellValue;	// �����۸�

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
