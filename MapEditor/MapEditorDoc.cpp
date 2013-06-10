// MapEditorDoc.cpp : implementation of the CMapEditorDoc class
//

#include "stdafx.h"
#include "MapEditor.h"
#include "MapEditorDoc.h"
#include "MapEditorSplitterView.h"
#include "../CodeConvert/CodeConvert.h"
#include "CanvasScrollView/canvasscrollview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc

IMPLEMENT_DYNCREATE(CMapEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CMapEditorDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc construction/destruction

CMapEditorDoc::CMapEditorDoc()
{
	// TODO: add one-time construction code here
	m_bShowGrid = true;
	
	m_nCanvasMapGridWidth = DEFAULT_TILE_WIDTH;		// map space is 64 pixels
	m_nCanvasMapGridHeight = DEFAULT_TILE_HEIGHT;

	m_nCanvasMapGridDefaultWidth = DEFAULT_TILE_WIDTH;
	m_nCanvasMapGridDefaultHeight = DEFAULT_TILE_HEIGHT;

	m_nCanvasMapGridLineStyle = PS_DOT;
	m_nCanvasMapGridLineWeight = 1;

	m_colorCanvasBackGroundColor = RGB(64, 0, 64);
	m_colorCanvasGridLineColor = RGB(128, 128, 128);

	m_bShowEveryMESelectionFrame = true;
	m_colorMESelectionFrame = RGB(255, 0, 0);

	m_nOutputPicMargin = DEFAULT_OUTPUT_PIC_MARGIN;

	m_dZoom = 1.0;

	m_pvMELayers = new VectorMapElementLayerList;
	m_pvMapTiles = new VectorMapElement;
	m_pvMapDoors = new VectorMapElement;
	m_pvMapObstacles = new VectorMapElement;
	m_pvMapObjects = new VectorMapElement;
	m_pvMapTreasure = new VectorMapElement;
	m_pvMapMonsters = new VectorMapElement;
	m_pvMapMasters = new VectorMapElement;
	m_pvMapHeroes = new VectorMapElement;

	m_pvMELayers->push_back(m_pvMapTiles);
	m_pvMELayers->push_back(m_pvMapDoors);
	m_pvMELayers->push_back(m_pvMapObstacles);
	m_pvMELayers->push_back(m_pvMapObjects);
	m_pvMELayers->push_back(m_pvMapTreasure);
	m_pvMELayers->push_back(m_pvMapMonsters);
	m_pvMELayers->push_back(m_pvMapMasters);
	m_pvMELayers->push_back(m_pvMapHeroes);

	m_vSelMELayers.push_back(&m_vSelTiles);
	m_vSelMELayers.push_back(&m_vSelDoors);
	m_vSelMELayers.push_back(&m_vSelObstacles);
	m_vSelMELayers.push_back(&m_vSelObjects);
	m_vSelMELayers.push_back(&m_vSelTeasure);
	m_vSelMELayers.push_back(&m_vSelMonsters);
	m_vSelMELayers.push_back(&m_vSelMasters);
	m_vSelMELayers.push_back(&m_vSelHeroes);

	m_pSelectedMapElement = NULL;
	m_copiedMapElement.nID = -1;

	m_nDraggingMEID = -1;

	m_eEditMode = MAP_EDIT_SELECT;

	m_nMapOffsetX = 0;
	m_nMapOffsetY = 0;

	m_nMaxRedoCnts = 20;	// 此值暂时无用
	m_nMaxUndoCnts = DEFAULT_UNDO_COUNT;

	m_rLBtnDownSelect.SetRectEmpty();

	m_nPlayerCount = MIN_PLAYERS;	// 默认为 2 个玩家

	m_pCurrentPropertyObj = &m_PropObjDefault;

	m_dwLayerVisibleFlag = 0xFFFFFFFF;

	m_nConquestToken = 0;
	m_nThreatToken = 0;

	m_bIsLOSMode = false;
}

CMapEditorDoc::~CMapEditorDoc()
{
	this->DeleteContents();

	// 删除当前层中的数据
	ReleaseVectorMapElementLayerListData(m_pvMELayers);
	m_pvMELayers = NULL;	
}

BOOL CMapEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	this->DeleteContents();

	return TRUE;
}

#define SetMapElementLong(L, p, n, name)  lua_getfield(L, n, #name); p->##name = lua_isnoneornil(L, -1) ? 0 : (long)lua_tonumber(L, n); (lua_pop(L, 1))

#define SetMapElementShort(L, p, n, name)  lua_getfield(L, n, #name); p->##name = lua_isnoneornil(L, -1) ? 0 : (short)lua_tonumber(L, n); (lua_pop(L, 1))

#define SetMapElementENUMMETYPE(L, p, n, name)  lua_getfield(L, n, #name); p->##name = lua_isnoneornil(L, -1) ? ME_TYPE_TILE : (ENUM_ME_TYPE)(long)lua_tonumber(L, n); (lua_pop(L, 1))

#define SetMapElementString(L, p, n, name)  lua_getfield(L, n, #name); p->##name = lua_isnoneornil(L, -1) ? _T("") : lua_tostring(L, n); lua_pop(L, 1)

#define SetMapElementBooleanFromNumber(L, p, n, name)  lua_getfield(L, n, #name); p->##name = lua_isnoneornil(L, -1) ? false : (bool)lua_tointeger(L, n); lua_pop(L, 1)

#define SetMapElementBoolean(L, p, n, name)  lua_getfield(L, n, #name); p->##name = lua_isnoneornil(L, -1) ? false : (bool)lua_toboolean(L, n); lua_pop(L, 1)

BOOL CMapEditorDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here

	// Clean the document
	DeleteContents();

	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
	lua_State* L = pApp->m_LuaEnv;
	
	if (L)
	{
		lua_getglobal(L, _T("ReadMapFile"));

		if (lua_isfunction(L, -1))
		{
			lua_pushstring(L, lpszPathName);

			if (lua_pcall(L, 1, 1, NULL))
			{
				AfxMessageBox(lua_tostring(L, -1));
				return FALSE;
			}

			if (lua_istable(L, -1))
			{
				int nMap = lua_gettop(L);
				lua_pushnil(L);
				while (lua_next(L, nMap))
				{
					lua_pushvalue(L, -2);	// Get ME's array name
					std::string strMEName = lua_tostring(L, -1);
					lua_pop(L, 1);

					VectorMapElement* pVME = NULL;
					if ("Tiles" == strMEName)
					{
						pVME = m_pvMapTiles;
					}
					else if ("Doors" == strMEName)
					{
						pVME = m_pvMapDoors;
					}
					else if ("Obstacles" == strMEName)
					{
						pVME = m_pvMapObstacles;
					}
					else if ("Objects" == strMEName)
					{
						pVME = m_pvMapObjects;
					}
					else if ("Treasure" == strMEName)
					{
						pVME = m_pvMapTreasure;
					}
					else if ("Monsters" == strMEName)
					{
						pVME = m_pvMapMonsters;
					}
					else if ("Masters" == strMEName)
					{
						pVME = m_pvMapMasters;
					}
					else if ("Heroes" == strMEName)
					{
						pVME = m_pvMapHeroes;
					}
					else if ("Players" == strMEName)
					{
						// 人数
						m_nPlayerCount = lua_tonumber(L, -1);
					}
					else if ("ThreatToken" == strMEName)
					{
						m_nThreatToken = lua_tonumber(L, -1);
					}
					else if ("ConquestToken" == strMEName)
					{
						m_nConquestToken = lua_tonumber(L, -1);
					}

					if (pVME && lua_istable(L, -1))
					{
						int nMapType = lua_gettop(L);
						lua_pushnil(L);
						while (lua_next(L, nMapType))
						{
							if (lua_istable(L, -1))	// MapElement array
							{
								MAPELEMENT* pME = new MAPELEMENT();
												
								// 普通地图对象数据
								SetMapElementLong(L, pME, -1, nID);
								SetMapElementLong(L, pME, -1, nPx);
								SetMapElementLong(L, pME, -1, nPy);
								SetMapElementLong(L, pME, -1, nWidth);
								SetMapElementLong(L, pME, -1, nHeight);
								SetMapElementShort(L, pME, -1, nAngle);
								SetMapElementString(L, pME, -1, strName);
								SetMapElementString(L, pME, -1, strDesc);
								SetMapElementENUMMETYPE(L, pME, -1, nType);
								SetMapElementString(L, pME, -1, strExpandsion);

#ifdef _DEBUG
								CString tstrName;
								lua_getfield(L, -1, "strName"); 
								tstrName = lua_tostring(L, -1);
								lua_pop(L, 1);
#endif // _DEBUG
								switch(pME->nType)
								{
								case ME_TYPE_HERO:
									{
//										HEROPROP* pHeroProp = &pME->uPropHero;
										SetMapElementShort(L, pME, -1, nHP);
										SetMapElementShort(L, pME, -1, nSpeed);
										SetMapElementShort(L, pME, -1, nArmor);
										SetMapElementString(L, pME, -1, strSkills);
										/*
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
										*/
										SetMapElementShort(L, pME, -1, nFatigue);
										SetMapElementShort(L, pME, -1, nConquestValue);
										SetMapElementShort(L, pME, -1, nFightingSkills);
										SetMapElementShort(L, pME, -1, nWizardrySkills);
										SetMapElementShort(L, pME, -1, nSubterfugeSkills);
										SetMapElementShort(L, pME, -1, nMagicTrait);
										SetMapElementShort(L, pME, -1, nMeleeTrait);
										SetMapElementShort(L, pME, -1, nRangeTrait);
										SetMapElementShort(L, pME, -1, nGolds);

										SetMapElementString(L, pME, -1, strMainHand);
										SetMapElementString(L, pME, -1, strOffHand);

										SetMapElementString(L, pME, -1, strArmor);
										SetMapElementString(L, pME, -1, strOther);
										SetMapElementString(L, pME, -1, strPotions);
										SetMapElementString(L, pME, -1, strPackage);
										break;
									}
								case ME_TYPE_MONSTER:
								case ME_TYPE_MONSTER_MASTER:
									{
//										MONSTERPROP* pMonsterProp = &pME->uPropMonster;
										// 怪物属性
										SetMapElementBooleanFromNumber(L, pME, -1, bMaster);
										SetMapElementLong(L, pME, -1, nHP);
										SetMapElementLong(L, pME, -1, nSpeed);
										SetMapElementLong(L, pME, -1, nArmor);
										SetMapElementString(L, pME, -1, strSkills);
										SetMapElementString(L, pME, -1, strAttackType);
										SetMapElementString(L, pME, -1, strAttackDice);
										SetMapElementLong(L, pME, -1, nBonus);
										break;
									}
								case ME_TYPE_TREASURE:
									{
//										TREASUREPROP* pTreatureProp = &pME->uPropTreature;
										SetMapElementLong(L, pME, -1, nSellValue);
										break;
									}
								case ME_TYPE_DOOR:
									{
//										DOORPROP* pDoorProp = &pME->uPropDoor;
										SetMapElementLong(L, pME, -1, nDoorType);
										break;
									}
								case ME_TYPE_OBJECT:
									break;
								case ME_TYPE_OBSTACLE:
									break;
								}
								

								MapID2MR::iterator iter_ir = pApp->m_mapImgRes.find(pME->nID);
								ASSERT(iter_ir != pApp->m_mapImgRes.end());

								if (iter_ir != pApp->m_mapImgRes.end())
								{
									pME->pImg = iter_ir->second->pImg;
								}

								pVME->push_back(pME);
							}
							lua_pop(L, 1);
						} // end map type likes "Tiles"
					}
					lua_pop(L, 1);
				} // end root <Map> while
			}
			else
			{
				AfxMessageBox("Load Map file failed...");
				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}
	}

	return TRUE;
}

BOOL CMapEditorDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	// 智能优化保存的文件后缀
	CString strFileName = lpszPathName;
	if (!strFileName.Find(_T(".dmap"), strFileName.GetLength() - 4))
	{
		strFileName += _T(".dmap");
	}
	CFile file(strFileName, CFile::modeCreate | CFile::modeWrite);

	if (file)
	{
		std::vector<std::string> mapXml;
		CString strRoot;
		strRoot.Format(_T("<Map Players=\"%d\" ThreatToken=\"%d\" ConquestToken=\"%d\">"), 
			m_nPlayerCount, m_nThreatToken, m_nConquestToken);
	//	std::string strXML = _T("<Map>");//  = _T("<?xml version=\"1.0\" encoding=\"gb2312\"?>\n<Map>\n");
	//	mapXml.push_back(strXML);
		mapXml.push_back((LPCTSTR)strRoot);

		// 记录当前玩家人数
// 		CString strPlayerLbl;
// 		strPlayerLbl.Format(_T("<Players>%d</Players>"), m_nPlayerCount);
// 		mapXml.push_back((LPCTSTR)strPlayerLbl);
		
		for (VectorMapElementLayerList::iterator it = m_pvMELayers->begin();
		it != m_pvMELayers->end();
		it++)
		{
			VectorMapElement* pVME = *it;
			if (pVME->size() <= 0)
			{
				continue;
			}
			std::string strStartLabel;
			std::string strEndLabel;
			if (pVME == m_pvMapTiles)
			{
				strStartLabel = _T("<Tiles>");
				strEndLabel = _T("</Tiles>");
			}
			else if (pVME == m_pvMapDoors)
			{
				strStartLabel = _T("<Doors>");
				strEndLabel = _T("</Doors>");
			}
			else if (pVME == m_pvMapObstacles)
			{
				strStartLabel = _T("<Obstacles>");
				strEndLabel = _T("</Obstacles>");
			}
			else if (pVME == m_pvMapObjects)
			{
				strStartLabel = _T("<Objects>");
				strEndLabel = _T("</Objects>");
			}
			else if (pVME == m_pvMapTreasure)
			{
				strStartLabel = _T("<Treasure>");
				strEndLabel = _T("</Treasure>");
			}
			else if (pVME == m_pvMapMonsters)
			{
				strStartLabel = _T("<Monsters>");
				strEndLabel = _T("</Monsters>");
			}
			else if(pVME == m_pvMapMasters)
			{
				strStartLabel = _T("<Masters>");
				strEndLabel = _T("</Masters>");
			}
			else if(pVME == m_pvMapHeroes)
			{
				strStartLabel = _T("<Heroes>");
				strEndLabel = _T("</Heroes>");
			}
			mapXml.push_back(strStartLabel);
			for (VectorMapElement::iterator iter=pVME->begin(); iter!=pVME->end(); iter++)
			{
				mapXml.push_back(std::string(_T("<item ")));

				CString strBuff;
				MAPELEMENT* pME = *iter;
// 				std::string strNameUTF8 = GBK2UTF8(std::string((*iter)->strName));
// 				std::string strDescUTF8 = GBK2UTF8(std::string((*iter)->strDesc));

				switch(pME->nType)
				{
// 				case ME_TYPE_TILE:
// 				case ME_TYPE_OBJECT:
// 				case ME_TYPE_OBSTACLE:
// 					break;
				case ME_TYPE_TREASURE:
					strBuff.Format(_T("nID=\"%d\" nPx=\"%d\" nPy=\"%d\" nType=\"%d\""
						" nWidth=\"%d\" nHeight=\"%d\" nAngle=\"%d\""
						" strName=\"%s\" strDesc=\"%s\" strExpandsion=\"%s\""
						" nSellValue=\"%d\""),
						pME->nID,
						pME->nPx,
						pME->nPy,
						(int)pME->nType,
						pME->nWidth,
						pME->nHeight,
						pME->nAngle,
						pME->strName,
						pME->strDesc,
						pME->strExpandsion,
						pME->nSellValue
					);
					break;
				case ME_TYPE_DOOR:
					strBuff.Format(_T("nID=\"%d\" nPx=\"%d\" nPy=\"%d\" nType=\"%d\""
						" nWidth=\"%d\" nHeight=\"%d\" nAngle=\"%d\""
						" strName=\"%s\" strDesc=\"%s\" strExpandsion=\"%s\""
						" nDoorType=\"%d\""),
						pME->nID,
						pME->nPx,
						pME->nPy,
						(int)pME->nType,
						pME->nWidth,
						pME->nHeight,
						pME->nAngle,
						pME->strName,
						pME->strDesc,
						pME->strExpandsion,
						pME->nDoorType
					);
					break;
				case ME_TYPE_MONSTER_MASTER:
				case ME_TYPE_MONSTER:
					strBuff.Format(_T("nID=\"%d\" nPx=\"%d\" nPy=\"%d\" nType=\"%d\" nWidth=\"%d\" nHeight=\"%d\" nAngle=\"%d\""
						" strName=\"%s\" strDesc=\"%s\" bMaster=\"%d\" nHP=\"%d\" nSpeed=\"%d\" nArmor=\"%d\" strSkills=\"%s\""
						" strAttackType=\"%s\" strAttackDice=\"%s\" nBonus=\"%d\""
						" strExpandsion=\"%s\" strStatus=\"%s\""),
						pME->nID,
						pME->nPx,
						pME->nPy,
						(int)pME->nType,
						pME->nWidth,
						pME->nHeight,
						pME->nAngle,
						pME->strName,
						pME->strDesc,
						pME->bMaster ? 1 : 0,
						pME->nHP,
						pME->nSpeed,
						pME->nArmor,
						pME->strSkills,
						pME->strAttackType,
						pME->strAttackDice,
						pME->nBonus,
						pME->strExpandsion,
						pME->strStatus
					);
					break;
				case ME_TYPE_HERO:
					strBuff.Format(_T("nID=\"%d\" nPx=\"%d\" nPy=\"%d\" nType=\"%d\" nWidth=\"%d\" nHeight=\"%d\" nAngle=\"%d\""
						" strName=\"%s\" strDesc=\"%s\" nHP=\"%d\" nSpeed=\"%d\" nArmor=\"%d\" strSkills=\"%s\""
						" strExpandsion=\"%s\" nConquestValue=\"%d\" nFatigue=\"%d\" nFightingSkills=\"%d\""
						" nWizardrySkills=\"%d\" nSubterfugeSkills=\"%d\" nMeleeTrait=\"%d\" nMagicTrait=\"%d\""
						" nRangeTrait=\"%d\" nGolds=\"%d\" strMainHand=\"%s\" strOffHand=\"%s\""
						" strArmor=\"%s\" strOther=\"%s\" strPackage=\"%s\" strPotions=\"%s\" strStatus=\"%s\""),
						pME->nID,
						pME->nPx,
						pME->nPy,
						(int)pME->nType,
						pME->nWidth,
						pME->nHeight,
						pME->nAngle,
						pME->strName,
						pME->strDesc,
						pME->nHP,
						pME->nSpeed,
						pME->nArmor,
						pME->strSkills,
						pME->strExpandsion,
						pME->nConquestValue,
						pME->nFatigue,
						pME->nFightingSkills,
						pME->nWizardrySkills,
						pME->nSubterfugeSkills,
						pME->nMeleeTrait,
						pME->nMagicTrait,
						pME->nRangeTrait,
						pME->nGolds,
						pME->strMainHand,
						pME->strOffHand,
						pME->strArmor,
						pME->strOther,
						pME->strPackage,
						pME->strPotions,
						pME->strStatus
					);
					break;
				default:
					strBuff.Format(_T("nID=\"%d\" nPx=\"%d\" nPy=\"%d\" nType=\"%d\""
						" nWidth=\"%d\" nHeight=\"%d\" nAngle=\"%d\""
						" strName=\"%s\" strDesc=\"%s\" strExpandsion=\"%s\""),
						pME->nID,
						pME->nPx,
						pME->nPy,
						(int)pME->nType,
						pME->nWidth,
						pME->nHeight,
						pME->nAngle,
						pME->strName,
						pME->strDesc,
						pME->strExpandsion
					);
					break;
				}

				mapXml.push_back(std::string((LPCTSTR)strBuff));
				mapXml.push_back(std::string(_T(" />")));
			}
			mapXml.push_back(strEndLabel);
		}
		mapXml.push_back(std::string(_T("</Map>")));

		for (std::vector<std::string>::iterator iter = mapXml.begin();
		iter != mapXml.end();
		iter++)
		{
			file.Write(iter->c_str(), iter->length());
		}
		
		file.Close();
		AfxMessageBox(_T("保存成功"), MB_ICONINFORMATION);
	}
	return TRUE;
//	return CDocument::OnSaveDocument(lpszPathName);
}


/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc serialization

void CMapEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc diagnostics

#ifdef _DEBUG
void CMapEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMapEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDoc commands

void CMapEditorDoc::SetMapGridWidth( int nWidth )
{
	if (nWidth <= 0)
	{
		return;
	}
	this->m_nCanvasMapGridWidth = nWidth;
}

int CMapEditorDoc::GetMapGridWidth()
{
	return this->m_nCanvasMapGridWidth;
}

int CMapEditorDoc::GetMapGridDefaultWidth()
{
	return this->m_nCanvasMapGridDefaultWidth;
}

void CMapEditorDoc::SetMapGridHeight( int nHeight )
{
	if (nHeight <= 0)
	{
		return;
	}
	this->m_nCanvasMapGridHeight = nHeight;
}

int CMapEditorDoc::GetMapGridHeight()
{
	return this->m_nCanvasMapGridHeight;
}

int CMapEditorDoc::GetMapGridDefaultHeight()
{
	return this->m_nCanvasMapGridDefaultHeight;
}

void CMapEditorDoc::SetCanvasBGColor( COLORREF colBG )
{
	this->m_colorCanvasBackGroundColor = colBG;	
}

COLORREF CMapEditorDoc::GetCanvasGBColor()
{
	return this->m_colorCanvasBackGroundColor;	
}

void CMapEditorDoc::SetCanvasGLColor( COLORREF colGL )
{
	this->m_colorCanvasGridLineColor = colGL;	
}

COLORREF CMapEditorDoc::GetCanvasGLColor()
{
	return this->m_colorCanvasGridLineColor;	
}

DWORD CMapEditorDoc::GetCanvasMapGridLineStyle()
{
	return this->m_nCanvasMapGridLineStyle;
}

void CMapEditorDoc::SetCanvasMapGridLineStyle( DWORD nStyle )
{
	this->m_nCanvasMapGridLineStyle = nStyle;
}

void CMapEditorDoc::SetCanvasMapGridLineWeight( int nLineWeight )
{
	// At least grid line weight is 1 pixel
	if (nLineWeight >= 1)
	{
		this->m_nCanvasMapGridLineWeight = nLineWeight;
	}
}

DWORD CMapEditorDoc::GetCanvasMapGridLineWeight()
{
	return this->m_nCanvasMapGridLineWeight;
}

void CMapEditorDoc::SetSelectedMapElement( MAPELEMENT* pME ) // discard
{
	m_pSelectedMapElement = pME;
}

MAPELEMENT* CMapEditorDoc::GetSelectedMapElement()	// discard
{
	return m_pSelectedMapElement;
}

void CMapEditorDoc::SetMapZoom( double dZoom )
{
	ASSERT(dZoom > 0);
	if (dZoom <= 0)
	{
		return;
	}
	
	m_dZoom = dZoom;
}

double CMapEditorDoc::GetMapZoom()
{
	return m_dZoom;	
}

void CMapEditorDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class

	// Clean the Selection ME array and copied ME array
	EmptySelectedMapElementArray(); // Clean the Selected array first, cause the point is delete by next
	EmptyCopiedMapElementArray();

	m_vLOSDst.clear();
	m_vLOSSrc.clear();


	// 清空重做和撤销队列中的数据
	ClearDequeStack(&m_dsRedo);
	ClearDequeStack(&m_dsUndo);

	for (VectorMapElementLayerList::iterator it = m_pvMELayers->begin();
	it != m_pvMELayers->end();
	it++)
	{
		VectorMapElement* pVME = *it;
		for (VectorMapElement::iterator iter = pVME->begin(); iter!=pVME->end(); iter++)
		{
			if (*iter)
			{
				delete *iter;
				*iter = NULL;
			}
		}
		pVME->clear();
	}
	m_nDraggingMEID = -1;

	m_pSelectedMapElement = NULL;
	CDocument::DeleteContents();
}

int CMapEditorDoc::AddMapElement( MAPELEMENT* pME )
{
	int nRet = 0;
	// Add to VectorMapElement by type
	VectorMapElement* pVME = NULL;

	pVME = m_pvMELayers->at(pME->nType);
	ASSERT(pVME != NULL);
	if (pVME != NULL)
	{		
		// Set MapElement Default Data
		SetMapObjectDefaultData(pME);

		// Add the MAPELEMENT
		pVME->push_back(pME);
	}
	else
	{
		nRet = -1;
		return nRet;
	}
	
	// 	if (m_pSelectedMapElement)
	// 	{
	// 		m_pSelectedMapElement->bSelected = false;
	// 	}
	// 	
	// 	m_pSelectedMapElement = pME;
	// 	m_pSelectedMapElement->bSelected = true;

	pVME = NULL;
	return nRet;
}

int CMapEditorDoc::AddMapElement( long nID, long nPx, long nPy, long nAngle )
{
	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();
	MapID2MR::iterator iter_find = pApp->m_mapImgRes.find(nID);

	if (iter_find == pApp->m_mapImgRes.end())
	{
		return -1;
	}

	MAPELEMENT* pME = new MAPELEMENT();
	// Add the ME's ID
	pME->nID = nID;
	// Add the CxImage pointer
	pME->pImg = iter_find->second->pImg;
	ASSERT(pME->pImg);
	// Add the ME's type
	pME->nType = iter_find->second->nType;
	// Add the Angle
	pME->nAngle = nAngle;
	
	pME->nPx = nPx;
	pME->nPy = nPy;

	pME->nWidth = pME->pImg->GetWidth();
	pME->nHeight = pME->pImg->GetHeight();

	pME->strName = iter_find->second->strName.c_str();

	// Add the MAPELEMENT with AddMapElement( MAPELEMENT* )
	return AddMapElement(pME);
}

//////////////////////////////////////////////////////////////////////////
// 这里的地图元素是不分层的
// 当调用 AddMapElement(MAPELEMENT*) 时会自动根据地图元件的 type 添加到对应的图层中去
int CMapEditorDoc::AddMapElement( VectorMapElement* pVME )
{
	ASSERT(pVME);
	if (pVME == NULL)
	{
		return -1;
	}

	for (VectorMapElement::iterator iter = pVME->begin();
	iter != pVME->end();
	iter++)
	{
		MAPELEMENT* pME = new MAPELEMENT();
		*pME = **iter;
		AddMapElement(pME);
	}
	return 0;
}

int CMapEditorDoc::DeleteSelectedMapElement( MAPELEMENT* pME )
{
	// If pME or m_pSelectedMapElement is NULL then return -1;
	if (pME == NULL)
	{
		return -1;
	}

	ENUM_ME_TYPE type = pME->nType;
	VectorMapElement* pVME = NULL;
// 	switch(type)
// 	{
// 	case ME_TYPE_TILE:
// 		pVME = m_pvMapTiles;
// 		break;
// 	case ME_TYPE_DOOR:
// 		pVME = m_pvMapDoors;
// 		break;
// 	case ME_TYPE_OBSTACLE:
// 		pVME = m_pvMapObstacles;
// 		break;
// 	case ME_TYPE_OBJECT:
// 		pVME = m_pvMapObjects;
// 		break;
// 	case ME_TYPE_TREASURE:
// 		pVME = m_pvMapTreasure;
// 		break;
// 	case ME_TYPE_MONSTER:
// 		pVME = m_pvMapMonsters;
// 		break;
// 	case ME_TYPE_MONSTER_MASTER:
// 		pVME = m_pvMapMasters;
// 		break;
// 	}

	pVME = m_pvMELayers->at(type);
	ASSERT(pVME != NULL);

	for (VectorMapElement::iterator iter_find = pVME->begin();
	iter_find != pVME->end();
	iter_find++)
	{
		if (*iter_find == pME)
		{
			// Find the deleted MAPELEMENT
			break;
		}
	}

	// Delete
	if (iter_find != pVME->end())
	{
		// Delete first, erase next.
		// erase method will change the iter's value
		delete *iter_find;
		pVME->erase(iter_find);
	}

	return 0;
}

int CMapEditorDoc::DeleteSelectedMapElements( VectorMapElement* pVME /*= NULL*/ )
{
	if (pVME == NULL)
	{
		pVME = &m_vSelectedME;
	}
	ASSERT(pVME);
	int nCount = 0;

	// Delete the MAPELEMENT in their own layer
	for (VectorMapElement::iterator iter_me = pVME->begin();
	iter_me != pVME->end();
	iter_me++)
	{
		if (DeleteSelectedMapElement(*iter_me) == 0)
		{
			*iter_me = NULL;
			nCount++;
		}
	}
	// Finally empty the selected ME array
	EmptySelectedMapElementArray();

	return nCount;
}

void CMapEditorDoc::SetCopiedMapElement( MAPELEMENT me )
{
	m_copiedMapElement = me;
}

MAPELEMENT CMapEditorDoc::GetCopiedMapElement()
{
	return m_copiedMapElement;
}

int CMapEditorDoc::SetMapElementsFromSelectedArea( CRect rect )
{
	// Clean the select MAPELEMENT array
	EmptySelectedMapElementArray();
	
	double dZoom = GetMapZoom();
	for (VectorMapElementLayerList::iterator iter_layer = m_pvMELayers->begin();
	iter_layer != m_pvMELayers->end();
	iter_layer++)
	{
		for (VectorMapElement::iterator iter_me = (*iter_layer)->begin();
		iter_me != (*iter_layer)->end();
		iter_me++)
		{
			// Calculate the position of the MAPELEMENT
			int nLeft = ((*iter_me)->nPx - 1) * GetMapGridWidth();
			int nTop = ((*iter_me)->nPy - 1) * GetMapGridHeight();
			int nRight = nLeft + (*iter_me)->nWidth * dZoom;
			int nBottom = nTop + (*iter_me)->nHeight * dZoom;
			if ((*iter_me)->nType == ME_TYPE_TILE)
			{
				// Tile should be re-calculate the rectangle
				nRight -= GetMapGridWidth()*2;
				nBottom -= GetMapGridHeight()*2;
			}

			// 加上偏移量
			CRect rME(nLeft, nTop, nRight, nBottom);
			rME.OffsetRect(GetMapOffsetX(), GetMapOffsetY());

			CRect rTmp;
			rTmp.IntersectRect(rect, rME);
			if (!rTmp.IsRectEmpty())
			{
				// Initial the selected rectangle
				m_rSelected = rME;
				(*iter_me)->bSelected = true;
				AddMapElementToSelectedMapElementArray(*iter_me);
// 				m_vSelectedME.push_back(*iter_me);
// 				//////////////////////////////////////////////////////////////////////////
// 				// Add to selected map layer for draw in ordered
// 				VectorMapElement* pVME = m_vSelMELayers[(long)(*iter_me)->nType];
// 				ASSERT(pVME != NULL);
// 				pVME->push_back(*iter_me);
			}
		}
	}
	return 0;
}

int CMapEditorDoc::SetMapElementsFromSelectedArea( CPoint startPoint, CPoint endPoint )
{
	CRect rectTmp;
	// Create the temporary rectangle with the startPoint and endPoint
	rectTmp.SetRect(startPoint, endPoint);
	if (rectTmp.left >= rectTmp.right)
	{
		// Exchange the left and right
		int nTmp = rectTmp.right;
		rectTmp.right = rectTmp.left;
		rectTmp.left = nTmp;
	}
	if (rectTmp.top >= rectTmp.bottom)
	{
		// Exchange the top and bottom
		int nTmp = rectTmp.bottom;
		rectTmp.bottom = rectTmp.top;
		rectTmp.top = nTmp;
	}
	return SetMapElementsFromSelectedArea(CRect(rectTmp.TopLeft(), rectTmp.BottomRight()));
}

void CMapEditorDoc::GetSelectedMapElmentRect( CRect& rect )
{
	CRect rSelect(0, 0, 0, 0);
	double dZoom = GetMapZoom();
	// Calculate the selected rectangle
	for (VectorMapElement::iterator iter_sel = m_vSelectedME.begin();
	iter_sel != m_vSelectedME.end();
	iter_sel++)
	{
		// Calculate the position of the MAPELEMENT
		int nLeft = ((*iter_sel)->nPx - 1) * GetMapGridWidth();
		int nTop = ((*iter_sel)->nPy - 1) * GetMapGridHeight();
		int nRight = nLeft + (*iter_sel)->nWidth * dZoom;
		int nBottom = nTop + (*iter_sel)->nHeight * dZoom;
		if ((*iter_sel)->nType == ME_TYPE_TILE)
		{
			// Tile should be re-calculate the rectangle
			nRight -= GetMapGridWidth()*2;
			nBottom -= GetMapGridHeight()*2;
		}
		CRect rMESel(nLeft, nTop, nRight, nBottom);
		// Union the rectangle
		rSelect.UnionRect(&rSelect, &rMESel);
	}
	// Set the returnable rect object
	rSelect.OffsetRect(GetMapOffsetX(), GetMapOffsetY());
	rect = rSelect;
}

VectorMapElement* CMapEditorDoc::GetVectorSelectedMapElement()
{
	return &m_vSelectedME;	
}

int CMapEditorDoc::GetSelectedMapElementCount() const
{
	return m_vSelectedME.size();
}


int CMapEditorDoc::GetCopiedMapElementCount() const
{
	return m_vCopiedME.size();
}

int CMapEditorDoc::AddMapElementToSelectedMapElementArray( MAPELEMENT* pMEAdd )
{
	pMEAdd->bSelected = true;

// 	if (pMEAdd->nType == ME_TYPE_TILE)
// 	{
// 		pMEAdd->size.cx = (pMEAdd->nPx-2)*GetMapGridWidth();
// 		pMEAdd->size.cy = (pMEAdd->nPy-2)*GetMapGridHeight();
// 	}
// 	else
// 	{
// 		pMEAdd->size.cx = (pMEAdd->nPx-1)*GetMapGridWidth();
// 		pMEAdd->size.cy = (pMEAdd->nPy-1)*GetMapGridHeight();
// 	}

	m_vSelectedME.push_back(pMEAdd);

	//////////////////////////////////////////////////////////////////////////
	// Add to selected map layer for draw in ordered
	VectorMapElement* pVME = m_vSelMELayers[(long)pMEAdd->nType];
	ASSERT(pVME != NULL);
	pVME->push_back(pMEAdd);

	return 0;
}

int CMapEditorDoc::CopySelectedMapElementArray()
{
	if (GetSelectedMapElementCount() <= 0)
	{
		return -1;
	}
	EmptyCopiedMapElementArray();
	for (VectorMapElement::iterator iter_sel = m_vSelectedME.begin();
	iter_sel != m_vSelectedME.end();
	iter_sel++)
	{
		MAPELEMENT* pME = new MAPELEMENT;
		*pME = **iter_sel;
		m_vCopiedME.push_back(pME);
	}
	return 0;
}

int CMapEditorDoc::EmptySelectedMapElementArray()
{
	for (VectorMapElement::iterator iter = m_vSelectedME.begin();
	iter != m_vSelectedME.end();
	iter++)
	{
		// Pointer could be deleted by other operation first
		// because selected array just a pointer array of the 
		// MAPELEMENT, not the object of ME
		if (*iter)
		{
			(*iter)->bSelected = false;
		}
	}
	m_vSelectedME.clear();

	// Clear the map layer ordered vector
	for (VectorMapElementLayerList::iterator iter_layer = m_vSelMELayers.begin();
	iter_layer != m_vSelMELayers.end();
	iter_layer++)
	{
		(*iter_layer)->clear();
	}

	// 通知界面刷新 PropertyView
	// 设置属性
	UpdatePropertyViewData();

	return 0;
}

int CMapEditorDoc::EmptyCopiedMapElementArray()
{
	for (VectorMapElement::iterator iter = m_vCopiedME.begin();
	iter != m_vCopiedME.end();
	iter++)
	{
		delete *iter;
		*iter = NULL;
	}
	m_vCopiedME.clear();
	return 0;
}

int CMapEditorDoc::RemoveMapElementFromSelectedMapElementArray( MAPELEMENT* pMEDel )
{
	VectorMapElement::iterator iter_find = m_vSelectedME.begin();
	for (; iter_find != m_vSelectedME.end(); iter_find++)
	{
		if ((*iter_find) == pMEDel)
		{
			pMEDel->bSelected = false;
			break;
		}
	}
	if (iter_find != m_vSelectedME.end())
	{
		m_vSelectedME.erase(iter_find);
		return 0;
	}
	return -1;
}

bool CMapEditorDoc::IsMapElementInSelectedMapElementArray( MAPELEMENT* pME )
{
	for (VectorMapElement::iterator iter = m_vSelectedME.begin();
	iter != m_vSelectedME.end();
	iter++)
	{
		if ((*iter) == pME)
		{
			return true;
		}
	}
	return false;
}

int CMapEditorDoc::PasteCopiedMapElements()
{
	if (m_pvMELayers->size() < 0)
	{
		return -1;
	}
	// Empty the selected array
	EmptySelectedMapElementArray();

	for (VectorMapElement::iterator iter_copied = m_vCopiedME.begin();
	iter_copied != m_vCopiedME.end();
	iter_copied++)
	{
		MAPELEMENT* pME = new MAPELEMENT;
		*pME = **iter_copied;
		// Add current pasted MAPELEMENT to map
		AddMapElement(pME);
		// Set current pasted MAPELEMENT to Selected Array
		AddMapElementToSelectedMapElementArray(pME);	
	}
	
	return 0;
}

void CMapEditorDoc::UpdateOriginalPositionOfSelectedArray()
{
	// 保存当前位置
	for (VectorMapElement::iterator iter = m_vSelectedME.begin();
	iter != m_vSelectedME.end();
	iter++)
	{
		(*iter)->posOri.x = (*iter)->nPx;
		(*iter)->posOri.y = (*iter)->nPy;
	}
}

ENUM_EDIT_MODE CMapEditorDoc::GetMapEditMode()
{
	return m_eEditMode;
}

void CMapEditorDoc::SetMapEditMode( ENUM_EDIT_MODE eMode )
{
	m_eEditMode = eMode;
}

void CMapEditorDoc::SetMapOffsetX( long nOffset )
{
	m_nMapOffsetX = nOffset;
}

void CMapEditorDoc::SetMapOffsetY( long nOffset )
{
	m_nMapOffsetY = nOffset;
}

long CMapEditorDoc::GetMapOffsetX()
{
	return m_nMapOffsetX;
}

long CMapEditorDoc::GetMapOffsetY()
{
	return m_nMapOffsetY;
}

long CMapEditorDoc::GetMapOriginalOffsetX()
{
	return m_nMapOriginalOffsetX;
}

long CMapEditorDoc::GetMapOriginalOffsetY()
{
	return m_nMapOriginalOffsetY;
}

void CMapEditorDoc::SyncOffset()
{
	m_nMapOriginalOffsetX = m_nMapOffsetX;
	m_nMapOriginalOffsetY = m_nMapOffsetY;
}

bool CMapEditorDoc::ChangeRedoAndUndoDeque( int nType )
{
	bool bRet = false;
	switch(nType)
	{
	case OPERATE_ADD:
	case OPERATE_DELETE:
	case OPERATE_MOVE:
	case OPERATE_ROTATE_LEFT:
	case OPERATE_ROTATE_RIGHT:
	case OPERATE_PASTE:
		// 修改
		// 保存当前所有内容到撤销队列
		{
			VectorMapElementLayerList* pVMELL = GetCurrentMapFullDataLayerList();
			if (NULL != pVMELL)
			{
				// 清空重做队列
				m_dsRedo.clear();

				m_dsUndo.push_back(pVMELL);
				if (m_dsUndo.size() > m_nMaxUndoCnts)
				{
					VectorMapElementLayerList* pItem = m_dsUndo.front();
					// 删除并释放最前的数据
					ReleaseVectorMapElementLayerListData(pItem);
					pItem = NULL;
					// 最前面的数据出队列
					m_dsUndo.pop_front();
				}
				bRet = true;
			}
		}
		break;
	case OPERATE_REDO:
		{
			// 重做
			// 1. 把当前内容放到 Undo 队列中
			m_dsUndo.push_back(m_pvMELayers);
			// 2. 把当前内容指向 Redo 队列的末尾元素
			SetCurrentMapFromTail(&m_dsRedo);
			// 3. Redo 队列末尾出队列
			m_dsRedo.pop_back();

			if (m_dsUndo.size() > m_nMaxUndoCnts)
			{
				// 超过上限，移除最前面的元素
				VectorMapElementLayerList* pItem = m_dsUndo.front();
				ReleaseVectorMapElementLayerListData(pItem);
				pItem = NULL;
				m_dsUndo.pop_front();
			}

			// 清空当前选择的区域
			EmptySelectedMapElementArray();
		}
		break;
	case OPERATE_UNDO:
		{
			// 撤销
			// 1. 把当前内容放到 Redo 队列中
			m_dsRedo.push_back(m_pvMELayers);
			// 2. 把当前内容指向 Undo 队列的末尾元素
			SetCurrentMapFromTail(&m_dsUndo);
			// 3. Undo 队列末尾出队列
			m_dsUndo.pop_back();
			
			if (m_dsRedo.size() > m_nMaxUndoCnts)
			{
				// 超过上限，移除最前面的元素
				VectorMapElementLayerList* pItem = m_dsRedo.front();
				ReleaseVectorMapElementLayerListData(pItem);
				pItem = NULL;
				m_dsRedo.pop_front();
			}

			// 清空当前选择的区域
			EmptySelectedMapElementArray();
		}
		break;
	}
	return bRet;
}

VectorMapElementLayerList* CMapEditorDoc::GetCurrentMapFullDataLayerList()
{
	VectorMapElementLayerList* pVMELL = NULL;

	pVMELL = new VectorMapElementLayerList;

// 	VectorMapElement* pvMapTiles = new VectorMapElement;
// 	VectorMapElement* pvMapDoors = new VectorMapElement;
// 	VectorMapElement* pvMapObstacles = new VectorMapElement;
// 	VectorMapElement* pvMapObjects = new VectorMapElement;
// 	VectorMapElement* pvMapTreasure = new VectorMapElement;
// 	VectorMapElement* pvMapMonsters = new VectorMapElement;
// 	VectorMapElement* pvMapMasters = new VectorMapElement;

	VectorMapElement* pVM[7];
	int i = 0;
	for (; i < 7; i++)
	{
		pVM[i] = new VectorMapElement;
		VectorMapElement* ptVM = NULL;
// 		switch(i)
// 		{
// 		case 0:
// 			ptVM = m_pvMapTiles;
// 			break;
// 		case 1:
// 			ptVM = m_pvMapDoors;
// 			break;
// 		case 2:
// 			ptVM = m_pvMapObstacles;
// 			break;
// 		case 3:
// 			ptVM = m_pvMapObjects;
// 			break;
// 		case 4:
// 			ptVM = m_pvMapTreasure;
// 			break;
// 		case 5:
// 			ptVM = m_pvMapMonsters;
// 			break;
// 		case 6:
// 			ptVM = m_pvMapMasters;
// 			break;
// 		default:
// 			break;
// 		}
		ptVM = m_pvMELayers->at(i);
		if (ptVM != NULL)
		{
			for (VectorMapElement::iterator iter = ptVM->begin();
			iter != ptVM->end();
			iter++)
			{
				MAPELEMENT* pME = new MAPELEMENT;
				*pME = **iter;
				pVM[i]->push_back(pME);
			}
		}
		pVMELL->push_back(pVM[i]);
	}

	return pVMELL;
}

bool CMapEditorDoc::ReleaseVectorMapElementLayerListData( VectorMapElementLayerList* pVMELL )
{
	bool bRet = false;

	if (NULL == pVMELL)
	{
		return bRet;
	}
	// 循环释放节点内所有数据
	for (VectorMapElementLayerList::iterator iterLayer = pVMELL->begin();
	iterLayer != pVMELL->end();
	iterLayer++)
	{
		for (VectorMapElement::iterator iter = (*iterLayer)->begin();
		iter != (*iterLayer)->end();
		iter++)
		{
			if (*iter)
			{
				delete *iter;
				*iter = NULL;
			}
		}
		if (*iterLayer)
		{
			delete *iterLayer;
			*iterLayer = NULL;
		}
	}

	if (pVMELL)
	{
		delete pVMELL;
		pVMELL = NULL;
	}

	return bRet;
}

bool CMapEditorDoc::SetCurrentMapFromTail( DequeStack* pDS )
{
	bool bRet = false;
	if (pDS->size() <= 0)
	{
		return bRet;
	}
	
	m_pvMELayers = pDS->back();
	m_pvMapTiles = m_pvMELayers->at(ME_TYPE_TILE);
	m_pvMapDoors = m_pvMELayers->at(ME_TYPE_DOOR);
	m_pvMapObstacles = m_pvMELayers->at(ME_TYPE_OBSTACLE);
	m_pvMapObjects = m_pvMELayers->at(ME_TYPE_OBJECT);
	m_pvMapTreasure = m_pvMELayers->at(ME_TYPE_TREASURE);
	m_pvMapMonsters = m_pvMELayers->at(ME_TYPE_MONSTER);
	m_pvMapMasters = m_pvMELayers->at(ME_TYPE_MONSTER_MASTER);

	return bRet;
}

bool CMapEditorDoc::ClearDequeStack( DequeStack* pDS )
{
	for (DequeStack::iterator iterDS = pDS->begin();
	iterDS != pDS->end();
	iterDS++)
	{
		ReleaseVectorMapElementLayerListData(*iterDS);
		*iterDS = NULL;
	}
	pDS->clear();
	return true;
}

bool CMapEditorDoc::DeleteUndoLast()
{
	bool bRet = false;

	if (m_dsUndo.size() <= 0)
	{
		return bRet;
	}

	ReleaseVectorMapElementLayerListData(m_dsUndo.back());
	m_dsUndo.pop_back();
	bRet = true;
	return bRet;
}

int CMapEditorDoc::SelectAllMapElment()
{
	int nCount = 0;

	// Empty the Select Array and Temporary Selected Array
	EmptySelectedMapElementArray();

	// Add all data to select array
	for (VectorMapElementLayerList::iterator iter_layer = m_pvMELayers->begin();
	iter_layer != m_pvMELayers->end();
	iter_layer++)
	{
		for (VectorMapElement::iterator iter_me = (*iter_layer)->begin();
		iter_me != (*iter_layer)->end();
		iter_me++)
		{
			if (0 == AddMapElementToSelectedMapElementArray(*iter_me))
			{
				nCount++;
			}
		}
	}
	return nCount;
}

void CMapEditorDoc::BindPropertyData(PropertyObjectCommObject* pPropObj, MAPELEMENT* pME)
{
	pPropObj->m_nPosX = pME->nPx;
	pPropObj->m_nPosY = pME->nPy;
	pPropObj->BindData(pME);
}

bool CMapEditorDoc::UpdatePropertyViewData()
{
	
#define ME_TYPE_COMBINE -1
#define ME_NO_TYPE -2
	VectorMapElement* pSelected = GetVectorSelectedMapElement();
	int nCnt = GetSelectedMapElementCount();
	MAPELEMENT* pME = NULL;
	int nType = ME_NO_TYPE;
	if (nCnt > 1)
	{
		nType = ME_TYPE_COMBINE;
		pME = *pSelected->begin();
	}
	else if (nCnt == 1 && pSelected->begin() != pSelected->end())
	{
		pME = *(pSelected->begin());
		nType = pME->nType;
	}
	
	switch(nType)
	{
	case ME_NO_TYPE:
		m_PropObjDefault.m_nPlayers = m_nPlayerCount;
		m_PropObjDefault.m_nMonsters = m_pvMapMonsters->size();
		m_PropObjDefault.m_nMasters = m_pvMapMasters->size();
		m_PropObjDefault.m_pnThreatToken = &m_nThreatToken;
		m_PropObjDefault.m_pnConquestToken = &m_nConquestToken;
		m_pCurrentPropertyObj = &m_PropObjDefault;
		break;
	case ME_TYPE_TILE:
		BindPropertyData(&m_PropObjTile, pME);
		m_pCurrentPropertyObj = &m_PropObjTile;
		break;
	case ME_TYPE_DOOR:
		BindPropertyData(&m_PropObjDoor, pME);
		m_pCurrentPropertyObj = &m_PropObjDoor;
		break;
	case ME_TYPE_OBJECT:
		BindPropertyData(&m_PropObjComm, pME);
		m_pCurrentPropertyObj = &m_PropObjComm;
		break;
	case ME_TYPE_TREASURE:
		BindPropertyData(&m_PropObjTreature, pME);
		m_pCurrentPropertyObj = &m_PropObjTreature;
		break;
	case ME_TYPE_OBSTACLE:
		BindPropertyData(&m_PropObjObstacle, pME);
		m_pCurrentPropertyObj = &m_PropObjObstacle;
		break;
	case ME_TYPE_MONSTER:
	case ME_TYPE_MONSTER_MASTER:
		BindPropertyData(&m_PropObjMonster, pME);
		m_pCurrentPropertyObj = &m_PropObjMonster;
		break;
	case ME_TYPE_COMBINE:
		m_PropObjCombine.m_nObjCount = nCnt;
		BindPropertyData(&m_PropObjCombine, pME);
		m_pCurrentPropertyObj = &m_PropObjCombine;
		break;
	case ME_TYPE_HERO:
		BindPropertyData(&m_PropObjHero, pME);
		m_pCurrentPropertyObj = &m_PropObjHero;
		break;
	default:
		m_pCurrentPropertyObj = &m_PropObjDefault;
	}

#undef ME_TYPE_COMBINE
#undef ME_NO_TYPE
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd)
	{
		pWnd->PostMessage(WM_USER_PROPERTY_VIEW_UPDATE);
	}

	return m_pCurrentPropertyObj == NULL;
}

bool CMapEditorDoc::IsShowEveryMESelectionFrame()
{
	return m_bShowEveryMESelectionFrame;
}

void CMapEditorDoc::SetShowEveryMESelectionFrame( bool bShowMEFrame )
{
	m_bShowEveryMESelectionFrame = bShowMEFrame;
}

void CMapEditorDoc::SetMapObjectDefaultData( MAPELEMENT *pME )
{
	ASSERT(pME);

	CMapEditorApp* pApp = (CMapEditorApp*)AfxGetApp();

	lua_State* L = NULL;

	bool bHandled = false;
	if (pApp && pApp->m_LuaEnv != NULL)
	{
		// 得到整个程序的 Lua 环境
		L = pApp->m_LuaEnv;
		
		lua_getglobal(L, _T("HandleMapObjectDefaultData"));
		
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, pME->nID);
			lua_pushnumber(L, m_nPlayerCount);
			
			if (lua_pcall(L, 2, 1, NULL))
			{
				// 有问题
			}
			
			if (lua_istable(L, -1))
			{
				// 设置名称
				if (pME->strName.IsEmpty())
				{
					SetMapElementString(L, pME, -1, strName);
				}
				// 返回值有效，循环数组填充结构体
				// Set Default Value
				switch(pME->nType)
				{
				case ME_TYPE_TILE:
					break;
				case ME_TYPE_DOOR:
					break;
				case ME_TYPE_OBSTACLE:
					break;
				case ME_TYPE_OBJECT:
					break;
				case ME_TYPE_TREASURE:
//					SetMapElementLong(L, pME, -1, nSellValue);
					break;
				case ME_TYPE_MONSTER_MASTER:
					// SetMapElementLong(L, pME, -1, nBonus);
					pME->nBonus = 50;
				case ME_TYPE_MONSTER:
					SetMapElementBoolean(L, pME, -1, bMaster);	
					SetMapElementLong(L, pME, -1, nHP);
					SetMapElementLong(L, pME, -1, nArmor);
					SetMapElementLong(L, pME, -1, nSpeed);
					SetMapElementString(L, pME, -1, strSkills);
					SetMapElementString(L, pME, -1, strAttackType);
					SetMapElementString(L, pME, -1, strAttackDice);
					SetMapElementString(L, pME, -1, strExpandsion);

					bHandled = true;
					break;
				case ME_TYPE_HERO:
					SetMapElementString(L, pME, -1, strName);
					SetMapElementLong(L, pME, -1, nSpeed);
					SetMapElementLong(L, pME, -1, nHP);
					SetMapElementLong(L, pME, -1, nArmor);
					SetMapElementLong(L, pME, -1, nFatigue);
					SetMapElementLong(L, pME, -1, nConquestValue);
					SetMapElementLong(L, pME, -1, nFightingSkills);
					SetMapElementLong(L, pME, -1, nSubterfugeSkills);
					SetMapElementLong(L, pME, -1, nWizardrySkills);
					SetMapElementLong(L, pME, -1, nMagicTrait);
					SetMapElementLong(L, pME, -1, nMeleeTrait);
					SetMapElementLong(L, pME, -1, nRangeTrait);
					SetMapElementString(L, pME, -1, strSkills);
					SetMapElementString(L, pME, -1, strExpandsion);
					bHandled = true;
					break;
				}
			}
		}
	}

	if (!bHandled)
	{
		// Set Default Value
		switch(pME->nType)
		{
		case ME_TYPE_TILE:
			break;
		case ME_TYPE_DOOR:
			pME->strName = _T("门");
			switch(pME->nID)
			{
			case 4001:
				pME->nDoorType = PropertyObjectDoor::DOOR_TYPE_BLUE;
				break;
			case 4002:
				pME->nDoorType = PropertyObjectDoor::DOOR_TYPE_RED;
				break;
			case 4003:
				pME->nDoorType = PropertyObjectDoor::DOOR_TYPE_YELLOW;
				break;
			default:
				pME->nDoorType = PropertyObjectDoor::DOOR_TYPE_NORMAL;	// 普通门
			}
			break;
		case ME_TYPE_OBSTACLE:
			break;
		case ME_TYPE_OBJECT:
			break;
		case ME_TYPE_TREASURE:
			switch(pME->nID)
			{
			case 7000:
				pME->nSellValue = 125;
				break;
			case 7001:
				pME->nSellValue = 250;
				break;
			case 7002:
				pME->nSellValue = 375;
				break;
			case 7003:
				pME->nSellValue = 100;
				break;
			default:
				pME->nSellValue = 25;
			}
			break;
		case ME_TYPE_MONSTER:
			pME->bMaster = false;
			break;
		case ME_TYPE_MONSTER_MASTER:
			pME->bMaster = true;
			pME->nBonus = 50;
			break;
		case ME_TYPE_HERO:
			break;
		}
	}
}

COLORREF CMapEditorDoc::GetMESelectionFrameColor()
{
	return m_colorMESelectionFrame;
}

void CMapEditorDoc::SetMESelectionFrameColor( COLORREF rgb )
{
	m_colorMESelectionFrame = rgb;
}

bool CMapEditorDoc::OnPlayerCountChange()
{
	// 暂时只处理怪物属性

	SetMapElementDefaultData(m_pvMapMonsters);
	SetMapElementDefaultData(m_pvMapMasters);
	// 更新复制缓存的元件
	SetMapElementDefaultData(&m_vCopiedME);
	
	return true;
}

void CMapEditorDoc::SetMapElementDefaultData(VectorMapElement* pVME )
{
	ASSERT(pVME);

	if (pVME)
	{
		VectorMapElement::iterator iterME = pVME->begin();
		for (;iterME != pVME->end(); iterME++)
		{
			MAPELEMENT* pME = *iterME;
			SetMapObjectDefaultData(pME);
		}
	}
}

void CMapEditorDoc::SetMapLayerVisible( ENUM_ME_TYPE eLayer, bool bVisible )
{
	DWORD dwValue = 0x00000001 << eLayer;
	if (bVisible)
	{
		m_dwLayerVisibleFlag |= dwValue;
	}
	else
	{
		m_dwLayerVisibleFlag &= ~(dwValue);
	}

	// 刷新界面
	CCanvasScrollView* pView = NULL;
	POSITION pos = GetFirstViewPosition();
	while (pos)
	{
		CView* tpView = GetNextView(pos);
		if (tpView && tpView->IsKindOf(RUNTIME_CLASS(CCanvasScrollView)))
		{
			pView = (CCanvasScrollView*)tpView;
			pView->Invalidate(FALSE);
		}
	}
}

bool CMapEditorDoc::GetMapLayerVisible( ENUM_ME_TYPE eLayer )
{
	return m_dwLayerVisibleFlag & (0x00000001 << eLayer);
}

void CMapEditorDoc::ToggleMapLayerVisible( ENUM_ME_TYPE eLayer )
{
	SetMapLayerVisible(eLayer, !GetMapLayerVisible(eLayer));
}
