#if !defined(AFX_LEFTFORMVIEW_H__784F3AA1_688F_409E_AC62_3134ED6EA444__INCLUDED_)
#define AFX_LEFTFORMVIEW_H__784F3AA1_688F_409E_AC62_3134ED6EA444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//////////////////////////////////////////////////////////////////////////
// Thumb item struct
typedef struct tagThumbItem {
	long nID;					// Map image's ID
	long nImageIndex;			// Index of the thumb CImagelist
	std::string strDisplayText;	// Text

	tagThumbItem(long ID, long imgIdx, const char* pszText)
	{
		nID = ID;
		nImageIndex = imgIdx;
		strDisplayText = pszText;
	}
} THUMBITEM;

typedef std::vector<THUMBITEM*> VectorThumbItem;		// �б�ؼ�����ʾ������ͼ���飨��Ӧ���ͣ�

typedef std::map<long, VectorThumbItem*> MapType2VTI;	// ��ͼ���͵���Ӧ���б�Ԫ����������б������Դ��

typedef std::map<long, CImageList*> MapType2ImgList;

typedef std::map<long, long> MapLong2Long;

class CLeftFormView : public CFormView
{
protected:
	CLeftFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLeftFormView)

// Form Data
public:
	//{{AFX_DATA(CLeftFormView)
	enum { IDD = IDD_LEFTFORMVIEW_FORM };
	CListCtrl	m_ctlThumbList;
	CComboBox	m_comboItemType;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Lua ���л���
	lua_State* m_L;
	// ��Ӧ���е�ͼԪ�ص�ͼ���б�����ָ��
	CImageList* m_pImageList;
	// �Ƿ���ק
	bool m_bDragging;
	//////////////////////////////////////////////////////////////////////////
	// ���õ�ͼԪ������������
	int SetMapTypeComboBoxContent();
	//////////////////////////////////////////////////////////////////////////
	// ���õ�ͼImageList
	int InitImageList();
	// Fill the CListCtrl by image type
	int FillListCtrl(ENUM_ME_TYPE nType);
	virtual ~CLeftFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLeftFormView)
	afx_msg void OnSelchangeComboItemsType();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBegindragListImage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListImage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// ����
	MapType2ImgList m_mapType2Imglist;

	//////////////////////////////////////////////////////////////////////////
	// ����ͼƬΨһ�� ID �� CImageList ��ͼƬ��������
	// ������ CListCtrl ����ʾ��Ӧ��ͼƬ
	MapLong2Long m_mapImgID2ImgListIndex;

	// ��ͼ���͵�����ͼ�����б�
	MapType2VTI m_mapType2ThumbItemList;

	// Initial flag for being sure initialize once
	static bool m_bInitialed;

	// Clean the data
	void CleanData();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTFORMVIEW_H__784F3AA1_688F_409E_AC62_3134ED6EA444__INCLUDED_)
