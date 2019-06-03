#include "stdafx.h"
#include "Table.h"

IMPLEMENT_DYNAMIC(CCell,CPanel)

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ԫ���캯��

@param      (i)UIINFO& uIInfo     ��ǰ�ؼ�����ʽ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCell::CCell(UIINFO& uIInfo)
:CPanel(uIInfo)
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ԫ�����캯��

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCell::~CCell()
{
	
}


IMPLEMENT_DYNAMIC(CTable,CPanel)

//////////////////////////////////////////////////////////////////////////
/**
@brief      Table���캯��

@param      (i)UIINFO& uIInfo     ��ǰ�ؼ�����ʽ
@param      (i)UINT rows          ��Ҫ����Table������
@param      (i)UINT cols          ��Ҫ����Table������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTable::CTable(
			   UIINFO& uIInfo
			   ,UINT rows
			   ,UINT cols
			   ,int titleHeigh
			   ,int noticeWidth
			   )
:CPanel(uIInfo)
{

	ASSERT(rows>0 && cols>0);
	
	m_Cols = cols;
	m_Rows = rows;
	m_CellCutFunc = NULL;
	m_titleHeigh = titleHeigh;
	m_noticeWidth = noticeWidth; 
	for (int rowIndex=0;rowIndex<rows;rowIndex++)
	{
		for(int colIndex=0;colIndex<cols;colIndex++)
		{
			UIINFO cellUIInfo;
			cellUIInfo.m_pParentWnd = this;
			CCell* cell = new CCell(cellUIInfo);
			pair<int,int> cellIndex(rowIndex,colIndex);
			cells[cellIndex] = cell;
		}

	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Table���캯��

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTable::~CTable()
{
	for(map<pair<UINT,UINT>, CCell*>::iterator iter = cells.begin() ;iter!=cells.end();iter++)
	{
		CCell* cell = iter->second;
		if(cell)
		{
			delete cell;
			cell = NULL;
		}
	}
	cells.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��ָ�����ж�Ӧ�ĵ�Ԫ���У��д�0��ʼ������

@param      (i)UINT row ����������0��ʼ������
@param      (i)UINT col ����������0��ʼ������

@retval     CCell* ָ���ĵ�Ԫ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCell* CTable::Cell(UINT row,UINT col)
{
	
	return cells[pair<UINT,UINT>(row,col)];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ָ���еĿ��

@param      (i)UINT width ���
@param      (i)UINT colIndex ����������0��ʼ������

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTable::SetColWidth(UINT width,UINT colIndex)
{
	ASSERT(colIndex<m_Cols);
	m_mapColWidths[colIndex] = width;
	if(IsWindow(m_hWnd))
	{
		this->DestroyWindow();
		this->Create();
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ָ�����еĿ��

@param      (i)UINT* width ָ���еĿ������
@param      (i)UINT  count ָ���е�����
@param      (i)UINT startColIndex ָ�������еĿ�ʼ����

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTable::SetColWidth(UINT* widths,UINT count,UINT startColIndex)
{
	for(int i=0;i<count && (i+startColIndex)<m_Rows;i++)
	{
		m_mapColWidths[i+startColIndex] = widths[i];
	}
	if(IsWindow(m_hWnd))
	{
		this->DestroyWindow();
		this->Create();
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ָ���еĸ߶�

@param      (i)UINT height �߶�
@param      (i)UINT rowIndex ����������0��ʼ������

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTable::SetRowHeight(UINT height,UINT rowIndex)
{
	m_mapRowHeights[rowIndex] = height;
	if(IsWindow(m_hWnd))
	{
		this->DestroyWindow();
		this->Create();
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ָ�����еĸ߶�

@param      (i)UINT* heights ָ���еĸ߶�����
@param      (i)UINT  count ָ���е�����
@param      (i)UINT startRowIndex ָ�������еĿ�ʼ����

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTable::SetRowHeight(UINT* heights,UINT count,UINT startRowIndex)
{
	for(int i=0;i<count && (i+startRowIndex)<m_Rows;i++)
	{
		m_mapRowHeights[i+startRowIndex] = heights[i];
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ñ��ؼ�

@param      void

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTable::Create()
{
	BOOL success = TRUE;
	success = __super::Create();
	CalculateCellRect();
	for(map<pair<UINT,UINT>, CCell*>::iterator iter=cells.begin();iter!=cells.end();iter++)
	{
		CCell* cell = iter->second;
		if(cell)
		{
			if(!cell->Create())
			{
				success = FALSE;
			}
		}
	}
	return success;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ñ�񻮷ֵ�Ԫ��ʱ�Ļص��������ṩһ���ӿڹ������߸��ĵ�Ԫ���λ�ü���С��

@param      Func<void (const CTable *,UINT,UINT,CPoint*,CSize*)> * cellCutFunc �ص�����ָ��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTable::SetCellCutFunc(Func<void (const CTable *,UINT,UINT,CPoint*,CSize*)> * cellCutFunc)
{
	m_CellCutFunc = cellCutFunc;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ÿ����Ԫ���λ�ü���С�����δ�����иߺ��п�����ݸ߶ȺͿ��ƽ�������ÿ����Ԫ��

@param      void

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTable::CalculateCellRect()
{
	CRect contentRect = GetContentRect();
	
	// Ԥ����Ŧ���������
	contentRect.top += m_titleHeigh;
	// Ԥ����ͼ��ʾ����
	contentRect.left += m_noticeWidth;
	CPoint contentLeftTop = CPoint(contentRect.left,contentRect.top);

	map<UINT,UINT> map_ColWidths;
	for(int i=0;i<m_Cols;i++)
	{
		map_ColWidths[i] = 0;
	}
	UINT totalSetWidth = 0;
	for(map<UINT,UINT>::iterator colWidthIter = m_mapColWidths.begin(); colWidthIter!=m_mapColWidths.end();colWidthIter ++)
	{
		map_ColWidths[colWidthIter->first] = colWidthIter->second;
		totalSetWidth += colWidthIter->second;
	}
	ASSERT(totalSetWidth<=contentRect.Width());
	if(m_Cols - m_mapColWidths.size()>0)
	{
		UINT unitOfWidth = (contentRect.Width() - totalSetWidth) / (m_Cols - m_mapColWidths.size());
		for(map<UINT,UINT>::iterator colWidthIter = map_ColWidths.begin(); colWidthIter!=map_ColWidths.end();colWidthIter ++)
		{
			if(colWidthIter->second==0)//δ�趨�Ļ������趨һ��Ĭ��ֵ
			{
				colWidthIter->second = unitOfWidth;
			}
		}
	}
	map<UINT,UINT> map_RowHeights;
	for(int i=0; i<m_Rows; i++)
	{
		map_RowHeights[i] = 0;
	}
	UINT totalSetHeight = 0;
	for(map<UINT,UINT>::iterator rowHeightIter = m_mapRowHeights.begin(); rowHeightIter!=m_mapRowHeights.end();rowHeightIter ++)
	{
		map_RowHeights[rowHeightIter->first] = rowHeightIter->second;
		totalSetHeight += rowHeightIter->second;
	}
	ASSERT(totalSetHeight<=contentRect.Height());
	if(m_Rows > m_mapRowHeights.size())
	{
		UINT unitOfHeight = (contentRect.Height() - totalSetHeight) / (m_Rows - m_mapRowHeights.size());
		for(map<UINT,UINT>::iterator rowHeightIter = map_RowHeights.begin(); rowHeightIter!=map_RowHeights.end();rowHeightIter ++)
		{
			if(rowHeightIter->second==0)//δ�趨�Ļ������趨һ��Ĭ��ֵ
			{
				rowHeightIter->second = unitOfHeight;
			}
		}
	}
	UINT startY = 0;
	for (int rowIndex=0;rowIndex<m_Rows;rowIndex++)
	{
		UINT startX = 0;
		UINT height = map_RowHeights[rowIndex];
		for(int colIndex=0;colIndex<m_Cols;colIndex++)
		{
			UINT width = map_ColWidths[colIndex];
			CPoint cellLoction = contentLeftTop + CPoint(startX,startY );
			CSize size = CSize(width, height);
			if(m_CellCutFunc)
			{
				m_CellCutFunc->Invoke(this,rowIndex,colIndex,&cellLoction,&size);
			}
			pair<int,int> cellIndex(rowIndex,colIndex);
			UIINFO cellUIInfo = cells[cellIndex]->GetUIInfo();
			cellUIInfo.m_Location = cellLoction;
			cellUIInfo.m_Size = size;
			cellUIInfo.m_pParentWnd = this;
			
			cells[cellIndex]->SetUIInfo(cellUIInfo);
			startX+=width;
		}
		startY+=height;
	}
}

