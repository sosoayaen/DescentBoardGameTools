#if !defined(AFX_OBJECTPROPERTYVIEW_H__006BEF05_104E_4D11_93EE_61EFA8F7EC19__INCLUDED_)
#define AFX_OBJECTPROPERTYVIEW_H__006BEF05_104E_4D11_93EE_61EFA8F7EC19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectPropertyView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectPropertyView view

#include "EPropView.h"
#include "EPropCtrl.h"
#include "PropertyObject.h"

class CObjectPropertyView : public EPropView
{
protected:
	CObjectPropertyView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CObjectPropertyView)

// Attributes
public:

	PropertyObjectCommObject m_PropObjObj;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectPropertyView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CObjectPropertyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CObjectPropertyView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTPROPERTYVIEW_H__006BEF05_104E_4D11_93EE_61EFA8F7EC19__INCLUDED_)
