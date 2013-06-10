// MESplitterWnd.h: interface for the CMESplitterWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESPLITTERWND_H__2281DB91_D20C_4ABC_ADC5_F21D33D374D7__INCLUDED_)
#define AFX_MESPLITTERWND_H__2281DB91_D20C_4ABC_ADC5_F21D33D374D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMESplitterWnd : public CSplitterWnd  
{
	DECLARE_DYNCREATE(CMESplitterWnd)
public:
	CMESplitterWnd();
	virtual ~CMESplitterWnd();

protected:
	// MFC 消息处理
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MESPLITTERWND_H__2281DB91_D20C_4ABC_ADC5_F21D33D374D7__INCLUDED_)
