/* Generated by Together */

#include "stdafx.h"
//#include "IGridModel.h"
#include "EPropList.h"
#include "EGridCtrl.h"
#include "IGridRow.h"

IMPLEMENT_DYNCREATE(EGridCtrl, EPropWnd)

BEGIN_MESSAGE_MAP(EGridCtrl, EPropWnd)
	//{{AFX_MSG_MAP(CPropListView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_COMMAND_RANGE( 130 , 140 , OnMenuItemSelected )
	//}}AFX_MSG_MAP
	ON_MESSAGE( UWM_PROPERTY_CHANGED , OnPropertyChanged )
	ON_MESSAGE( UWM_ADD_GRID_ROW     , OnAddGridRow      )
END_MESSAGE_MAP()

void EGridCtrl::OnPropertyChanged( WPARAM wp , LPARAM lp )
{
	void* pProperty = (void*)wp;

	int nCols = m_vCols.size();
	int nRows = m_vRows.size();

	int nNumCells = nCols * nRows;

	EProperty** pCur = m_ppPropertyMap;

	CDC* pDC = NULL;

	for( int i=0 ; i<nNumCells ; i++ )
	{
		if( *pCur != NULL )
		{
			if( pProperty == (*pCur)->m_pProperty )
			{
				if( pDC == NULL )
				{
					pDC = GetDC();
					pDC->SelectObject(&m_Font);
					pDC->SetViewportOrg( m_ViewportOrg );
				}

				(*pCur)->Draw( pDC , this );
			}
		}

		pCur++;
	}

	if( pDC != NULL )
	{
		ReleaseDC(pDC);
	}

//	Invalidate();
}

void EGridCtrl::OnAddGridRow( WPARAM wp , LPARAM lp )
{
	IGridRow* pGridRow = (IGridRow*)wp;

	AddRow(pGridRow);

}


//void EGridCtrl::OnGridRowChanged( WPARAM wp , LPARAM lp )
//{
//	IGridRow* pGridRow = (IGridRow*)wp;
//
//	std::list<Row*>::iterator iter = m_vRows.begin();
//	std::list<Row*>::iterator iend = m_vRows.end();
//
//	EProperty** ppMap = m_ppPropertyMap;
//
//	while( iter != iend )
//	{
//		Row* pRow = *iter;
//
//		if( pRow->m_pRowHost == pGridRow )
//		{
//
//
////	pRow->Clear()
////	Row* pRow = new Row( pRowHost );
////	pRowHost->GetGridColumns( *pRow );
////	pRowHost->m_pGridCtrl = this;
//
//		}
//
//		iter++;
//	}
//
//
//}

EGridCtrl::EGridCtrl()
{
	m_nColumnSeperatorWidth = 1;
//	m_bMouseOverVerticalGridEdge = false;
	m_ViewportOrg.x = 0;
	m_ViewportOrg.y = 0;
	m_pDragColumn   = NULL;
	m_ppPropertyMap = NULL;
}

EGridCtrl::~EGridCtrl()
{
	Clear();
}

void EGridCtrl::OnPaint()
{
	CPaintDC dc(this);


	CRect cr;
	GetClientRect(&cr);

	CDC MemDC;
	CBitmap MemBM;
	MemBM.CreateCompatibleBitmap(&dc,cr.Width(),cr.Height());
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(&MemBM);
	
	DoPaint(&MemDC);

	dc.BitBlt(0,0,cr.Width(),cr.Width(),&MemDC,0,0,SRCCOPY);
}

void EGridCtrl::DoPaint( CDC* pDC )
{
	CPoint vpo;
	vpo.y = -GetScrollPos(SB_VERT);
	vpo.x = -GetScrollPos(SB_HORZ);

	m_ViewportOrg = vpo;

	CRect cr;
	GetClientRect(&cr);

	pDC->SelectObject( &m_Font );
	pDC->SetBkMode(TRANSPARENT);

	//
	// clear bg
	//

	pDC->FillSolidRect( &cr , RGB(255,255,255) );

//pDC->MoveTo(0,0);
//pDC->LineTo(200,100);

//	BITMAPINFO bmi;
//	bmi.bmiHeader.biBitCount      = 32;
//	bmi.bmiHeader.biClrImportant  = 0;
//	bmi.bmiHeader.biClrUsed       = 0;
//	bmi.bmiHeader.biCompression   = BI_RGB;
//	bmi.bmiHeader.biHeight        = 100;
//	bmi.bmiHeader.biPlanes        = 1;
//	bmi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
//	bmi.bmiHeader.biSizeImage     = 0;			//0 is ok for BI_RGB (uncompressed)
//	bmi.bmiHeader.biWidth         = 100;
//	bmi.bmiHeader.biXPelsPerMeter = 1;
//	bmi.bmiHeader.biYPelsPerMeter = 1;

//	StretchDIBits( pDC->GetSafeHdc() , 0 , 100-1 , 100 , -100 , 0 , 0 , 100 , 100 , pBuffer , &bmi , DIB_RGB_COLORS , SRCCOPY );
//	SetDIBitsToDevice( pDC->GetSafeHdc() , 0 , 0 , 100 , 100 , 0 , 0 , 0 , 100 , pBuffer , &bmi , DIB_RGB_COLORS );


	//
	//
	//

	RefreshColumnLefts( vpo.x );
	RefreshRowTops( vpo.y + GetHeaderRowHeight() + m_nColumnSeperatorWidth );

	//
	//
	//

	std::list<Column*>::iterator coliter;
	std::list<Column*>::iterator coliend;

	//
	// draw all rows
	//

//--------

//	CRect r( 0 , 0 ,0 , 0 );
//
//	std::list<Row*>::iterator iter = m_vRows.begin();
//	std::list<Row*>::iterator iend = m_vRows.end();
//
//	while( iter != iend )
//	{
//		Row* pRow = *iter;
//
//		ASSERT( pRow->GetTabCount()==1 );
//		ASSERT( pRow->GetActiveTab()==0 );
//
//		r.top    = pRow->m_nTop;
//		r.bottom = pRow->m_nTop + pRow->m_nHeight;
//
//		coliter = m_vCols.begin();
//		coliend = m_vCols.end();
//
//		while( coliter != coliend )
//		{
//			Column* pColumn = *coliter;
//
//			EProperty* pProperty = pRow->GetProperty( pColumn->m_sName );
//
//			if( pProperty != NULL )
//			{
//				r.left  = pColumn->m_nLeft;
//				r.right = pColumn->m_nLeft + pColumn->m_nWidth;
//				pProperty->Draw( &dc , this , &r );
//			}
//
//
//
//			coliter++;
//		}
//		
//		r.top = r.bottom + m_nColumnSeperatorWidth;
//
//		iter++;
//	}

//--------

	CRect r( 0 , 0 ,0 , 0 );

	std::list<Row*>::iterator iter = m_vRows.begin();
	std::list<Row*>::iterator iend = m_vRows.end();

	EProperty** ppMap = m_ppPropertyMap;

	while( iter != iend )
	{
		Row* pRow = *iter;

		ASSERT( pRow->GetTabCount()==1 );
		ASSERT( pRow->GetActiveTab()==0 );

		r.top    = pRow->m_nTop;
		r.bottom = pRow->m_nTop + pRow->m_nHeight;

		coliter = m_vCols.begin();
		coliend = m_vCols.end();

		while( coliter != coliend )
		{
			Column* pColumn = *coliter;

			EProperty* pProperty = *ppMap;	//pRow->GetProperty( pColumn->m_sName );

			if( pProperty != NULL )
			{
				r.left  = pColumn->m_nLeft;
				r.right = pColumn->m_nLeft + pColumn->m_nWidth;
				pProperty->m_Rect = r;
				pProperty->Draw( pDC , this );
			}

			ppMap++;

			coliter++;
		}
		
		r.top = r.bottom + m_nColumnSeperatorWidth;

		iter++;
	}

//pDC->MoveTo(0,0);
//pDC->LineTo(500,500);
//pDC->FillSolidRect( 0 , 0 , 80 , 80 , RGB(255,240,240) );

//--------

	//
	// vertical grid lines
	//

	pDC->SelectStockObject(BLACK_PEN);

	if( m_vCols.size() > 0 )
	{
		coliter = m_vCols.begin();
		coliend = m_vCols.end();

		Row* pLastRow = m_vRows.back();

		int y0 = 0;
		int y1 = pLastRow->m_nTop + pLastRow->m_nHeight;

		while( coliter != coliend )
		{
			Column* pColumn = *coliter;

			int x = pColumn->m_nLeft + pColumn->m_nWidth;

			pDC->MoveTo( x , y0 );
			pDC->LineTo( x , y1 );

			coliter++;
		}
	}

	//
	// horizontal grid lines
	//
	
	if( m_vRows.size() > 0 )
	{
		iter = m_vRows.begin();
		iend = m_vRows.end();

		Column* pLastCol = m_vCols.back();

		int x0 = 0;
		int x1 = pLastCol->m_nLeft + pLastCol->m_nWidth;

		pDC->MoveTo( x0 , GetHeaderRowHeight() );
		pDC->LineTo( x1 , GetHeaderRowHeight() );

		while( iter != iend )
		{
			Row* pRow = *iter;

			int y = pRow->m_nTop + pRow->m_nHeight;

			pDC->MoveTo( x0 , y );
			pDC->LineTo( x1 , y );

			iter++;
		}
	}

	//
	// print column headers
	//

//	pDC->SetViewportOrg(0,0);

	r.SetRect(0,0,0,GetHeaderRowHeight());

	coliter = m_vCols.begin();
	coliend = m_vCols.end();

	while( coliter != coliend )
	{
		//   0    1                      2    3      4    5
		//   +----+----------------------+----+------+----+
		//   |    |                      |    |      |    |
		//   |    | column text          |    | sort |    |
		//   |    |                      |    |      |    |
		//   +----+----------------------+----+------+----+
		//

		Column* pColumn = *coliter;

		int x0 = pColumn->m_nLeft;
		int x1 = x0 + EIconTextButtonProperty::s_nMargin;

		int x5 = pColumn->m_nLeft + pColumn->m_nWidth;
		int x4 = x5 - EIconTextButtonProperty::s_nMargin;

		int x3 = x4;
		int x2 = x4;

		if( pColumn->m_nSortState != 0 )
		{
			x3 = x4 - 8;
			x2 = x3 - EIconTextButtonProperty::s_nMargin;
		}

		//
		// fill background
		//

		r.left  = x0;
		r.right = x5;
		pDC->FillSolidRect( &r , GetColorPaneKey() );

		//
		// text
		//

		r.left  = x1;
		r.right = x2;
		int nRequired = r.Width();
		CString s = EPropWnd::s_FitStringToWidth( pColumn->m_sName , nRequired , pDC );
		pDC->DrawText( s , &r , DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOCLIP );

		//
		// draw sort state triangle
		//

		if( pColumn->m_nSortState != 0 )
		{
			if( x3>=x1 )
			{
				CBrush brush;
				brush.CreateSolidBrush( RGB(100,100,100) );
				pDC->SelectObject(&brush);
				pDC->SelectStockObject(BLACK_PEN);

	//			CRect trianglerect = r;
	//			trianglerect.left = trianglerect.right - 10 - 4;
	//			trianglerect.right = trianglerect.left + 10;
	//			trianglerect.top += 8;
	//			trianglerect.bottom -= 8;
	//			DrawFrame( pDC , trianglerect , false , RGB(200,180,180) );

				pDC->SetViewportOrg( x3 , GetHeaderRowHeight()/2 );

				POINT points[] =
				{
					{ 0 ,-2 },
					{ 8 ,-2 },
					{ 4 ,2 }
				};

				if( pColumn->m_nSortState == 2 )
				{
					points[0].y *= -1;
					points[1].y *= -1;
					points[2].y *= -1;
				}

				pDC->Polygon( points , 3 );

				pDC->SetViewportOrg( 0 , 0 );
			}
		}

		coliter++;
	}

}

int EGridCtrl::GetRowHeight( EPropList* pRow , CDC* pDC )
{
	int n = pRow->Size();
	int h = 0;
	for( int i=0 ; i<n ; i++ )
	{
		int q = pRow->Get(i)->GetHeight(pDC);
		if( q > h )
		{
			h = q;
		}
	}
	return h;
}


/*
void EGridCtrl::SetGridModel( IGridModel* pModel )
{
	m_pGridModel = pModel;

	Clear();

	//
	// initially read rows of the model
	//

	if( m_pGridModel != NULL )
	{
		m_pGridModel->GetGridRows(*this);
	}

	TRACE("%d rows found\n",m_vRows.size());

	//
	//
	//

	CRect cr;
	GetClientRect(&cr);
	SetScrollPage(cr.Width(),cr.Height());
	Invalidate();

}
*/


void EGridCtrl::RefreshColumnLefts( int nOffset )
{
	std::list<Column*>::iterator jter = m_vCols.begin();
	std::list<Column*>::iterator jend = m_vCols.end();
	int nColumnLeft = nOffset;
	while( jter != jend )
	{
		Column* pColumn = *jter;
		pColumn->m_nLeft = nColumnLeft;
		nColumnLeft += pColumn->m_nWidth + m_nColumnSeperatorWidth;
		jter++;
	}
}

void EGridCtrl::RefreshRowTops( int nOffset )
{
	std::list<Row*>::iterator jter = m_vRows.begin();
	std::list<Row*>::iterator jend = m_vRows.end();
	int nRowTop = nOffset;
	while( jter != jend )
	{
		Row* pRow = *jter;
		pRow->m_nTop = nRowTop;
		nRowTop += pRow->m_nHeight + m_nColumnSeperatorWidth;
		jter++;
	}
}


CSize EGridCtrl::GetScrollPageSize()
{
	CSize size(0,0);

	CDC* pDC = GetDC();
	pDC->SelectObject( &m_Font );

	std::list<Row*>::iterator iter = m_vRows.begin();
	std::list<Row*>::iterator iend = m_vRows.end();
	while( iter != iend )
	{
		Row* pRow = *iter;
		size.cy += pRow->m_nHeight;	//GetRowHeight(pPropList,pDC);
		iter++;
	}

	std::list<Column*>::iterator jter = m_vCols.begin();
	std::list<Column*>::iterator jend = m_vCols.end();
	while( jter != jend )
	{
		Column* pColumn = *jter;
		size.cx += pColumn->m_nWidth;
		jter++;
	}

	//
	//
	//

	size.cx += m_nColumnSeperatorWidth * (m_vCols.size()    );
	size.cy += m_nColumnSeperatorWidth * (m_vRows.size() + 1);

	size.cy += GetHeaderRowHeight();

	ReleaseDC(pDC);

	return size;
}

int EGridCtrl::GetHeaderRowHeight()
{
	return 3 + 13 + 3;
}

EGridCtrl::Column* EGridCtrl::GetColumn( CString sName , bool bCreateIfNotFound , int nNewColumnWidth )
{
	Column* pColumn = NULL;

	std::list<Column*>::iterator iter = m_vCols.begin();
	std::list<Column*>::iterator iend = m_vCols.end();

	while( iter != iend )
	{
		Column* pCur = *iter;
		if( sName == pCur->m_sName )
		{
			pColumn = pCur;
			break;
		}
		iter++;
	}

	if( pColumn == NULL )
	{
		if( bCreateIfNotFound )
		{
			pColumn = new Column( sName );
			pColumn->EnsureWidth( nNewColumnWidth );
			m_vCols.push_back(pColumn);
		}
	}

	return pColumn;
}

//
// IRowList
//

//void EGridCtrl::UpdateColumnList( Row* pNewRow )
//{
//	CDC* pDC = GetDC();
//	pDC->SelectObject( &m_Font );
//
//	ASSERT( pNewRow->GetTabCount()==1 );
//	ASSERT( pNewRow->GetActiveTab()==0 );
//
//	int n = pNewRow->Size();
//
//	for( int i=0 ; i<n ; i++ )
//	{
//		EProperty* pProperty = pNewRow->Get(i);
//
//		CString sName   = pProperty->GetName();
//		Column* pColumn = GetColumn( sName , true , pDC->GetTextExtent(sName).cx + 8 );
//		pColumn->EnsureWidth( pProperty->GetPreferedWidth(pDC) );
//	}
//
//	ReleaseDC(pDC);
//}

void EGridCtrl::AddRow( IGridRow* pRowHost )
{
	//
	// collect properties (columns) for the row
	//

	Row* pRow = new Row( pRowHost );
	pRowHost->GetGridColumns( *pRow );
	pRowHost->m_pGridCtrl = this;

	//
	//  prepare the dc for size calculations
	//

	CDC* pDC = GetDC();
	pDC->SelectObject( &m_Font );

	//
	//
	//
	
	pRow->m_nHeight = GetRowHeight( pRow , pDC );
	m_vRows.push_back( pRow );

	//
	// update column list
	//

	std::list<Row*>::iterator iter = m_vRows.begin();
	std::list<Row*>::iterator iend = m_vRows.end();

	while( iter != iend )	//for all rows...
	{
		Row* pRow = *iter;

		ASSERT( pRow->GetTabCount()==1 );
		ASSERT( pRow->GetActiveTab()==0 );

		int n = pRow->Size();

		for( int i=0 ; i<n ; i++ )
		{
			EProperty* pProperty = pRow->Get(i);

			int m = EIconTextButtonProperty::s_nMargin;

			CString sName   = pProperty->GetName();
			Column* pColumn = GetColumn( sName , true , m + pDC->GetTextExtent(sName).cx + m );
			pColumn->EnsureWidth( pProperty->GetPreferedWidth(pDC) );
		}

		iter++;
	}

	ReleaseDC(pDC);

	//
	// calculate left of each column
	//

	RefreshColumnLefts( m_ViewportOrg.x );
	RefreshRowTops( m_ViewportOrg.y );

	//
	//
	//

	BuildPropertyMap();

	//
	//
	//

	Invalidate();

}


void EGridCtrl::BuildPropertyMap()
{
	//
	// rebuild propertymap
	//

	if( m_ppPropertyMap != NULL )
	{
		delete m_ppPropertyMap;
	}

	int nNumCells = m_vCols.size() * m_vRows.size();

	m_ppPropertyMap = new EProperty*[nNumCells];
	EProperty** ppMap = m_ppPropertyMap;

	//
	//
	//

	std::list<Row*>::iterator rowiter = m_vRows.begin();
	std::list<Row*>::iterator rowiend = m_vRows.end();
	while( rowiter != rowiend )
	{
		Row* pRow = *rowiter;

		std::list<Column*>::iterator coliter = m_vCols.begin();
		std::list<Column*>::iterator coliend = m_vCols.end();
		while( coliter != coliend )
		{
			Column* pColumn = *coliter;

			*ppMap++ = pRow->GetProperty( pColumn->m_sName );

			coliter++;
		}

		rowiter++;
	}
}

//
//
//

void EGridCtrl::Clear()
{
	std::list<Row*>::iterator iter = m_vRows.begin();
	std::list<Row*>::iterator iend = m_vRows.end();
	while( iter != iend )
	{
		Row* pRow = *iter;
		delete pRow;
		iter++;
	}
	m_vRows.clear();

	std::list<Column*>::iterator jter = m_vCols.begin();
	std::list<Column*>::iterator jend = m_vCols.end();
	while( jter != jend )
	{
		Column* pColumn = *jter;
		delete pColumn;
		jter++;
	}
	m_vCols.clear();

}

EGridCtrl::Column* EGridCtrl::GetColumn( int x , bool bRightEdge )
{
	std::list<Column*>::iterator jter = m_vCols.begin();
	std::list<Column*>::iterator jend = m_vCols.end();

	Column* pFound = NULL;

	while( jter != jend )
	{
		Column* pColumn = *jter;

		int x0 = pColumn->m_nLeft + 2;
		int x1 = x0 + pColumn->m_nWidth - 2;

		if( bRightEdge )
		{
			int q = pColumn->m_nLeft + pColumn->m_nWidth;
			x0 = q - 2;
			x1 = q + m_nColumnSeperatorWidth + 2;
		}

		if( x0<=x && x<x1 )
		{
			pFound = pColumn;
			break;
		}

		jter++;
	}

	return pFound;
}

EGridCtrl::Row* EGridCtrl::GetRow( int yCP )
{
	std::list<Row*>::iterator iter = m_vRows.begin();
	std::list<Row*>::iterator iend = m_vRows.end();

	Row* pHitRow = NULL;

	while( iter != iend )
	{
		Row* pRow = *iter;

		int yGP = yCP - m_ViewportOrg.y;

		int ymin = pRow->m_nTop;
		int ymax = pRow->m_nTop + pRow->m_nHeight;

		if( ymin<=yGP && yGP<ymax )
		{
			pHitRow = pRow;
			break;
		}

		iter++;
	}

	return pHitRow;
}

void EGridCtrl::OnMouseMove( UINT nFlags, CPoint point )
{
	char* pCursor = IDC_ARROW;

	if( nFlags & MK_LBUTTON )		//left button is down
	{
		if( m_pDragColumn )			//drag column found on mouse down
		{
			pCursor = IDC_SIZEWE;

			if( m_ButtonDownPoint.x == -1 )	//first time?
			{
				m_ButtonDownPoint  = point;
				m_nButtonDownWidth = m_pDragColumn->m_nWidth;
			}

			CPoint Delta = point - m_ButtonDownPoint;
			m_pDragColumn->m_nWidth = m_nButtonDownWidth + Delta.x;

			if( m_pDragColumn->m_nWidth<3 )
			{
				m_pDragColumn->m_nWidth=3;
			}

			RefreshColumnLefts( m_ViewportOrg.x );
			SetScrollPage();
			Invalidate();
		}
	}
	else
	{
		m_ButtonDownPoint.x = -1;

		bool bOnHeader = (0<=point.y) && (point.y<GetHeaderRowHeight());
		if( bOnHeader )
		{
			bool bOverVerticalEdge = (GetColumn(point.x,true)!=NULL);
			if( bOverVerticalEdge )
			{
				pCursor = IDC_SIZEWE;
			}
		}
	}

	SetCursor( LoadCursor(NULL,pCursor) );

	//
	//
	//

//	EProperty* pProperty = GetProperty( point );
//
//	if( pProperty == NULL )
//	{
//		TRACE("NULL\n");
//	}
//	else
//	{
//		TRACE("%s 0x%08x\n",pProperty->GetName(),pProperty);
//	}

}

EProperty* EGridCtrl::GetProperty( CPoint point , int &nCol , int& nRow )
{
	EProperty* pProperty = NULL;

	nCol = 0;

	std::list<Column*>::iterator coliter = m_vCols.begin();
	std::list<Column*>::iterator coliend = m_vCols.end();

	while( coliter != coliend )
	{
		Column* pColumn = *coliter;
		int min = pColumn->m_nLeft;
		int max = pColumn->m_nLeft + pColumn->m_nWidth;
		if( min<=point.x && point.x<max )
		{
			break;
		}
		nCol++;
		coliter++;
	}

	nRow = 0;

	std::list<Row*>::iterator rowiter = m_vRows.begin();
	std::list<Row*>::iterator rowiend = m_vRows.end();

	while( rowiter != rowiend )
	{
		Row* pRow = *rowiter;
		int min = pRow->m_nTop;
		int max = pRow->m_nTop + pRow->m_nHeight;
		if( min<=point.y && point.y<max )
		{
			break;
		}
		nRow++;
		rowiter++;
	}

	if( nCol<m_vCols.size() && nRow<m_vRows.size() )
	{
		pProperty = m_ppPropertyMap[ nCol + nRow * m_vCols.size() ];
	}

	return pProperty;

//-------

//	Row* pRow = GetRow( point.y );
//
//	EProperty* pProperty = NULL;
//
//	if( pRow != NULL )
//	{
//		Column* pColumn = GetColumn( point.x );
//
//		if( pColumn != NULL )
//		{
//			int n = pRow->Size();
//
//			for( int i=0 ; i<n ; i++ )
//			{
//				pProperty = pRow->Get(i);
//
//				if( pProperty->GetName() == pColumn->m_sName )
//				{
//					break;
//				}
//			}
//		}
//	}

//-------


	return pProperty;
}

BOOL EGridCtrl::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
//-------
//	BOOL bHandled = TRUE;
//
//	if( m_bMouseOverVerticalGridEdge )
//	{
//		SetCursor( LoadCursor(NULL,IDC_SIZEWE) );
//	}
//	else
//	{
//		bHandled = CWnd::OnSetCursor( pWnd , nHitTest , message );
//	}
//
//	return bHandled;
//-------
//	return CWnd::OnSetCursor( pWnd , nHitTest , message );
//-------
	return TRUE;
}

CString g_sSortProperty;
bool g_bSortGreatestAtTop = true;

template<>
bool std::greater<EGridCtrl::Row*>::operator()(EGridCtrl::Row* const& p2,EGridCtrl::Row* const& p1) const
{
	EProperty* pProp1 = p1->GetProperty(g_sSortProperty);
	EProperty* pProp2 = p2->GetProperty(g_sSortProperty);

	bool bGreater = false;

	if( pProp1!=NULL )
	{
		bGreater = pProp1->GreaterThan(pProp2);
	}

	if( g_bSortGreatestAtTop )
	{
		bGreater = !bGreater;
	}

	return bGreater;
}

void EGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_bWasDoubleClick )
	{
		m_bWasDoubleClick = false;
		return;
	}

	bool bOnHeaderRow = ( 0<=point.y && point.y<GetHeaderRowHeight() );

	if( bOnHeaderRow )					//is on header row?
	{
		if( point == m_MouseDownCP )	//mouse up same place as it was down?
		{
			if( m_pDragColumn==NULL )	//not dragging (not on column edge)
			{
				Column* pColumnToSort = GetColumn( point.x );		//get column where mouse was up!

				if( pColumnToSort != NULL )						//column exist here?
				{

					//
					// clear sortstate flag for all columns
					//

					std::list<Column*>::iterator coliter = m_vCols.begin();
					std::list<Column*>::iterator coliend = m_vCols.end();

					while( coliter != coliend )
					{
						Column* pC = *coliter;
						if( pC!=pColumnToSort )	//dont clear flag of column to sort
						{
							pC->m_nSortState = 0;
						}
						coliter++;
					}

					//
					//
					//

					if( m_vRows.size() > 1 )		//more than one column, so that we can actually sort something?
					{
						Row* pF = m_vRows.front();	//first
						Row* pL = m_vRows.back();	//last

						g_sSortProperty = pColumnToSort->m_sName;

						EProperty* pP1 = pF->GetProperty(g_sSortProperty);
						EProperty* pP2 = pL->GetProperty(g_sSortProperty);

						g_bSortGreatestAtTop = true;

						if( pP1 != NULL )
						{
							g_bSortGreatestAtTop = !pP1->GreaterThan(pP2);
						}

						pColumnToSort->m_nSortState = g_bSortGreatestAtTop ? 1 : 2;

						m_vRows.sort( std::greater<Row*>() );
					}
					else
					{
						pColumnToSort->m_nSortState = (pColumnToSort->m_nSortState==1) ? 2 : 1;	//just flip around (only one row, so it doesn't matter, but fells nice if sort indicator flips...)
					}

					BuildPropertyMap();
					Invalidate();

				}
			}
		}
	}

	m_pDragColumn = NULL;
	ReleaseCapture();

}

void EGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	m_MouseDownCP = point;

	bool bOnHeaderRow = ( 0<=point.y && point.y<GetHeaderRowHeight() );

	if( bOnHeaderRow )
	{
		m_pDragColumn = GetColumn(point.x,true);

		if( m_pDragColumn!=NULL )
		{
			SetCursor( LoadCursor(NULL,IDC_SIZEWE) );
		}
	}
	else
	{
		int nCol=-1;
		int nRow=-1;

		EProperty* pProperty = GetProperty( point , nCol , nRow );

		m_SeletectPropertyIndex.x = nCol;
		m_SeletectPropertyIndex.y = nRow;

		SetActiveProperty(pProperty);

		if( m_pActiveProperty != NULL )
		{
			if( m_pActiveProperty->OnLButtonDown( this , point ) )
			{
				NotifyPropertyChanged();
				Invalidate();
			}
		}
	}

}

void EGridCtrl::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	Column* pCol = GetColumn( point.x , true );

	if( pCol != NULL )
	{
		pCol->m_nWidth = 0;

		std::list<Row*>::iterator iter = m_vRows.begin();
		std::list<Row*>::iterator iend = m_vRows.end();

		CDC* pDC = GetDC();
		pDC->SelectObject(&m_Font);

		while( iter != iend )
		{
			Row* pRow = *iter;

			EProperty* pProperty = pRow->GetProperty( pCol->m_sName );

			if( pProperty != NULL )
			{
				pCol->EnsureWidth( pProperty->GetPreferedWidth(pDC) );
			}

			iter++;
		}

		ReleaseDC(pDC);
		RefreshColumnLefts( m_ViewportOrg.x );
		Invalidate();
		m_bWasDoubleClick = true;
	}
}

void EGridCtrl::StepPropertySelection( int nStepx , int nStepy )
{
	int nCol = m_SeletectPropertyIndex.x;
	int nRow = m_SeletectPropertyIndex.y;

	while( true )
	{
		nCol += nStepx;
		nRow += nStepy;

		int nNumCols = m_vCols.size();
		int nNumRows = m_vRows.size();

		nCol %= nNumCols;
		nRow %= nNumRows;

		if( nCol<0 ) nCol += nNumCols;
		if( nRow<0 ) nRow += nNumRows;

		EProperty* pNewActive = m_ppPropertyMap[ nCol + nRow * m_vCols.size() ];

		if( pNewActive != NULL )
		{
			if( pNewActive == m_pActiveProperty )	//the same (there was probably only one..)
			{
				SetActiveProperty( NULL );
				break;
			}

			if( pNewActive->IsSelectable() )
			{
				SetActiveProperty( pNewActive );
				break;
			}
		}
	}

	m_SeletectPropertyIndex.x = nCol;
	m_SeletectPropertyIndex.y = nRow;

//--------

//	std::list<Row*>::iterator iter = m_vRows.begin();
//	std::list<Row*>::iterator iend = m_vRows.end();
//
//	while( iter != iend )
//	{
//		Row* pRow = *iter;
//
//		int nNumProperties = pRow->Size();
//
//		for( int i=0 ; i<nNumProperties ; i++ )
//		{
//			EProperty* pProperty = pRow->Get(i);
//
//			if( pProperty == m_pActiveProperty )
//			{
//				while( true )
//				{
//					i++;
//
//					EProperty* pNewActive = pRow->Get(i);
//
//					if( pNewActive == m_pActiveProperty )	//the same (there was probably only one..)
//					{
//						SetActiveProperty( NULL );
//						break;
//					}
//
//					if( pNewActive->IsSelectable() )
//					{
//						SetActiveProperty( pNewActive );
//						break;
//					}
//				}
//			}
//		}
//
//		iter++;
//	}

//--------


}

BOOL EGridCtrl::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message == WM_KEYDOWN )
	{
//		TRACE("EPropListCtrl::PreTranslateMessage WM_KEYDOWN: ");

		bool bControl = (GetKeyState(VK_CONTROL)&0x80000000) != 0;
		bool bShift   = (GetKeyState(VK_SHIFT)  &0x80000000) != 0;

		if( pMsg->wParam == VK_RETURN )
		{
			OnDoPropClose();
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE)
		{
			m_bApplyOnKillFocus = false;
			OnDoPropClose();
			return TRUE;
		}
		else if( pMsg->wParam == VK_TAB )
		{
			if(bShift)
			{
				StepPropertySelection( -1 , 0 );
			}
			else
			{
				StepPropertySelection( 1 , 0 );
			}

			return TRUE;
		}
		else if( pMsg->wParam == VK_UP )
		{
			StepPropertySelection( 0 , -1 );
			return TRUE;
		}
		else if( pMsg->wParam == VK_DOWN )
		{
			StepPropertySelection( 0 , 1 );
			return TRUE;
		}
		
	}

	return EPropWnd::PreTranslateMessage( pMsg );
}

EProperty* EGridCtrl::GetProperty( int nCol , int nRow )
{
	return m_ppPropertyMap[ nCol + nRow * m_vCols.size() ];
}

EGridCtrl::Column* EGridCtrl::GetColumnAtIndex( int nColumn )
{
	std::list<Column*>::iterator coliter = m_vCols.begin();
	std::list<Column*>::iterator coliend = m_vCols.end();

	int x = 0;

	Column* pCol = NULL;

	while( coliter != coliend )
	{
		pCol = *coliter;

		if( x==nColumn )
		{
			break;
		}

		coliter++;
		x++;
	}

	return pCol;
}

int EGridCtrl::CalculateColumnWidth( int nColumn , bool bIncludeHeader )
{
	int nWidth = 0;

	int nNumRows = m_vRows.size();

	CDC* pDC = GetDC();
	pDC->SelectObject( &m_Font );

	for( int y=0 ; y<nNumRows ; y++ )
	{
		EProperty* pProperty = GetProperty(nColumn,y);

		if( pProperty!=NULL )
		{
			int w = pProperty->GetPreferedWidth(pDC);
			if( w > nWidth ) nWidth = w;
		}
	}

	if( bIncludeHeader )
	{
		Column* pCol = GetColumnAtIndex(nColumn);
		int m = EIconTextButtonProperty::s_nMargin;
		int w = m + pDC->GetTextExtent(pCol->m_sName).cx + m;
		if( w > nWidth ) nWidth = w;
	}

	ReleaseDC(pDC);

	return nWidth;
}

void EGridCtrl::OnContextMenu( CWnd* pWnd, CPoint pos )
{
	CMenu m;
	m.CreatePopupMenu();
	m.AppendMenu( MF_STRING , 130 , "Column Width: Total" );
	m.AppendMenu( MF_STRING , 131 , "Column Width: Data" );
//	m.AppendMenu( MF_STRING , 132 , "Column Width: Fit" );
	m.TrackPopupMenu( TPM_CENTERALIGN , pos.x , pos.y , this );

}

void EGridCtrl::OnMenuItemSelected(UINT nID)
{
//	int nNumCols = m_vCols.size();
//	int nNumRows = m_vRows.size();

	CDC* pDC = GetDC();
	pDC->SelectObject( &m_Font );

	if( (nID==130) || (nID==131) )
	{
		std::list<Column*>::iterator coliter = m_vCols.begin();
		std::list<Column*>::iterator coliend = m_vCols.end();

		int x = 0;

		while( coliter != coliend )
		{
			Column* pColumn = *coliter;
			int w = CalculateColumnWidth( x , nID==130 );
			pColumn->m_nWidth = w;
			coliter++;
			x++;
		}
	}
//	else if( nID==132 )
//	{
//		int nOptimal = 0;
//		int nIndex = 0;
//		std::list<Column*>::iterator coliter = m_vCols.begin();
//		std::list<Column*>::iterator coliend = m_vCols.end();
//		while( coliter != coliend )
//		{
//			Column* pColumn = *coliter;
//			pColumn->m_nWidth = CalculateColumnWidth( nIndex , true );
//			nOptimal += pColumn->m_nWidth;
//			nIndex++;
//			coliter++;
//		}
//
//		CRect cr;
//		GetClientRect(&cr);
//		int w = cr.Width();
//
//		coliter = m_vCols.begin();
//		coliend = m_vCols.end();
//		int xnext = 0;
//		while( coliter != coliend )
//		{
//			Column* pColumn = *coliter;
//
//			pColumn->m_nLeft = xnext;
//			int xnext = w * (pColumn->m_nLeft + pColumn->m_nWidth) / nOptimal;
//
//			pColumn->m_nWidth = (xnext - pColumn->m_nLeft);
//
//			coliter++;
//		}
//
//	}

	ReleaseDC(pDC);

	RefreshColumnLefts( m_ViewportOrg.x );
	Invalidate();
}

