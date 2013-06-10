#ifndef _COMM_H_
#define _COMM_H_

#include "ximage.h"
#include <string>
#include <VECTOR>
#include <MAP>

//////////////////////////////////////////////////////////////////////////
// �����ṹ��

// Every MapElement's rotate status
enum ENUM_ME_ROTATE 
{
	ME_ROTATE_ORIGINAL,	// original
		ME_ROTATE_LEFT,		// -��/2
		ME_ROTATE_RIGHT,	// ��/2
		ME_ROTATE_180		// ��
};

// ��ͼ��ǰѡ��ģʽ
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
	// Door ����
	short nDoorType;	// ������
} DOORPROP;

typedef struct tagTreasureProp
{
	// һ����Ʒ
	// ��ֵ
	int nSellValue;	// �����۸�
} TREASUREPROP;

typedef struct tagCreatureProperty
{
	// Creature ����
	short nHP;	// Ѫ��
	short nSpeed;	// �ƶ���
	short nArmor;	// ����
	CString strSkills;	// ��������
} CREATUREPROP;

typedef struct tagHeroProperty: public CREATUREPROP
{
	short nFatigue;			// ƣ��ֵ
	short nConquestValue;		// ��������
	short nFightingSkills;	// ս������
	short nWizardrySkills;	// ��������
	short nSubterfugeSkills;	// �Ӽ�����
	short nMagicTrait;		// ħ�����������ӳ�
	short nMeleeTrait;		// ��ս���������ӳ�
	short nRangeTrait;		// Զ�̹��������ӳ�
	short nGolds;				// ���
	
	CString strMainHand; // ����װ��
	CString strOffHand; // ����װ��
	
	CString strArmor;	// ����װ��
	CString strOther;	// ����
	CString strPotions;	// ҩ��
	CString strPackage;	// ����
} HEROPROP;

typedef struct tagMonsterProperty: public CREATUREPROP
{
	// Monster ����
	int nBonus;	// ��������
	bool bMaster; // �Ƿ�Ϊ��Ӣ
	CString strAttackType; // ��������
	CString strAttackDice;	// ������
} MONSTERPROP;

// Map Element struct
typedef struct tagMapElement
{
	// public:
	long nPx;				// x scale in map
	long nPy;				// y scale in map
	long nWidth;			// element's width ʵ��ͼƬ�ĳ�
	long nHeight;			// element's height ʵ��ͼƬ�Ŀ�
	long nID;				// element's ID
	CxImage* pImg;			// image point
	ORIGINALPOS posOri;		// original size of LButtonDown. Valid in selected		 
	ENUM_ME_TYPE nType;		// type enum
	short nAngle;			// rotate angle
	bool bSelected;			// selected status

	// ���ԣ�����nType�Ĳ�ͬ����ͬ
	CString strName;	// ����
	CString strDesc;	// ����
	CString strExpandsion;	// ��ͼԪ���İ汾

// 	DOORPROP uPropDoor;
// 	TREASUREPROP uPropTreature;
// 	HEROPROP uPropHero;
// 	MONSTERPROP uPropMonster;

	// Door ����
	int nDoorType;	// ������

	// һ����Ʒ
	// ��ֵ
	int nSellValue;	// �����۸�

	// Creature ����
	int nHP;	// Ѫ��
	int nSpeed;	// �ƶ���
	int nArmor;	// ����
	CString strSkills;	// ��������
	CString strStatus;	// ״̬

	// Monster ����
	int nBonus;	// ��������
	bool bMaster; // �Ƿ�Ϊ��Ӣ
	CString strAttackType; // ��������
	CString strAttackDice;	// ������

	// Heroes ����
	int nFatigue;			// ƣ��ֵ
	int nConquestValue;		// ��������
	int nFightingSkills;	// ս������
	int nWizardrySkills;	// ��������
	int nSubterfugeSkills;	// �Ӽ�����
	int nMagicTrait;		// ħ�����������ӳ�
	int nMeleeTrait;		// ��ս���������ӳ�
	int nRangeTrait;		// Զ�̹��������ӳ�
	int nGolds;				// ���
	CString strMainHand; // ����װ��
	CString strOffHand; // ����װ��
	CString strArmor;	// ����װ��
	CString strOther;	// ����
	CString strPotions;	// ҩ��
	CString strPackage;	// ����

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

		nHP = 0;	// Ѫ��
		nSpeed = 0;	// �ƶ���
		nArmor = 0;	// ����

		nFatigue = 0;			// ƣ��ֵ
		nConquestValue = 0;		// ��������
		nFightingSkills = 0;	// ս������
		nWizardrySkills = 0;	// ��������
		nSubterfugeSkills = 0;	// �Ӽ�����
		nMagicTrait = 0;		// ħ�����������ӳ�
		nMeleeTrait = 0;		// ��ս���������ӳ�
		nRangeTrait = 0;		// Զ�̹��������ӳ�
		nGolds = 0;				// ���
	}
	~tagMapElement()
	{
		pImg = NULL;
	}
}MAPELEMENT;

// ��Ԫ���е�
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

typedef std::vector<MAPELEMENT*> VectorMapElement;	// ��ͼģ�鼯�Ͻṹ

typedef std::vector<std::string> VectorMapElementType; // ��¼���Ƶ����͹�ϣ��

// typedef std::map<long, CxImage*> MapID2Image;	// ��ͼͼƬ��Դ���󼯺ϣ�ID��ͼ��

typedef std::map<long, MAPRESOURCE*> MapID2MR;	// ��ͼͼƬ��Ϣ��ϣ��

typedef std::map<std::string, long> MapName2ENUM; // ��ͼ���͵����Ƶ�ö������
//////////////////////////////////////////////////////////////////////////
// �����궨��
//////////////////////////////////////////////////////////////////////////
#define MIN_PLAYERS 2	// ��С�������
#define MAX_PLAYERS 5	// ����������
#define MIN_TILE_WIDTH 5	// ��С��ͼ��Ԫ��
#define MAX_TILE_WIDTH 100	// ����ͼ��Ԫ��
#define MIN_TILE_HEIGHT 5	// ��С��ͼ��Ԫ��
#define MAX_TILE_HEIGHT 100	// ����ͼ��Ԫ��
#define DEFAULT_UNDO_COUNT 20	// Ĭ�ϳ�������
#define DEFAULT_TILE_WIDTH 64	// Ĭ�ϵ�ͼ��Ԫ���
#define DEFAULT_TILE_HEIGHT 64	// Ĭ�ϵ�ͼ��Ԫ���
#define DEFAULT_OUTPUT_PIC_MARGIN 64 // ����ͼƬ�������׿��

//////////////////////////////////////////////////////////////////////////
// �޸ġ�������������Ӧ�����궨��
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
// �Զ������Ϣ
//////////////////////////////////////////////////////////////////////////

// ֪ͨ DEBUG ������ʾ������Ϣ
#define WM_USER_DEBUG_INFO_SHOW (WM_USER + 1)
// VIEW �����ĵ�������µ�״̬��֪ͨ
#define WM_USER_STATUSBAR_MOUSE_POSTION_UPDATE (WM_USER + 2)
// ���µ�ǰ��ͼ�����ű�
#define WM_USER_STATUSBAR_ZOOMED_RATE_UPDATE (WM_USER + 3)
// �������Կ�
#define WM_USER_PROPERTY_VIEW_UPDATE (WM_USER + 4)

//////////////////////////////////////////////////////////////////////////
// ״̬���ؼ�����
//////////////////////////////////////////////////////////////////////////

#endif
