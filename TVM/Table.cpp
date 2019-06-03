#include "stdafx.h"
#include "Table.h"

IMPLEMENT_DYNAMIC(CCell,CPanel)

//////////////////////////////////////////////////////////////////////////
/**
@brief      单元格构造函数

@param      (i)UIINFO& uIInfo     当前控件的样式

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCell::CCell(UIINFO& uIInfo)
:CPanel(uIInfo)
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      单元格析造函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCell::~CCell()
{
	
}


IMPLEMENT_DYNAMIC(CTable,CPanel)

//////////////////////////////////////////////////////////////////////////
/**
@brief      Table构造函数

@param      (i)UIINFO& uIInfo     当前控件的样式
@param      (i)UINT rows          需要创建Table的行数
@param      (i)UINT cols          需要创建Table的列数

@retval     无

@exception  无
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
@brief      Table析造函数

@param      void

@retval     无

@exception  无
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
@brief      取得指定行列对应的单元格（行，列从0开始计数）

@param      (i)UINT row 行索引（从0开始计数）
@param      (i)UINT col 列索引（从0开始计数）

@retval     CCell* 指定的单元格

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCell* CTable::Cell(UINT row,UINT col)
{
	
	return cells[pair<UINT,UINT>(row,col)];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置指定列的宽度

@param      (i)UINT width 宽度
@param      (i)UINT colIndex 列索引（从0开始计数）

@retval     void

@exception  无
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
@brief      设置指定几列的宽度

@param      (i)UINT* width 指定列的宽度数组
@param      (i)UINT  count 指定列的数量
@param      (i)UINT startColIndex 指定所有列的开始索引

@retval     void

@exception  无
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
@brief      设置指定行的高度

@param      (i)UINT height 高度
@param      (i)UINT rowIndex 行索引（从0开始计数）

@retval     void

@exception  无
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
@brief      设置指定几行的高度

@param      (i)UINT* heights 指定行的高度数组
@param      (i)UINT  count 指定行的数量
@param      (i)UINT startRowIndex 指定所有行的开始索引

@retval     void

@exception  无
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
@brief      设置表格控件

@param      void

@retval     void

@exception  无
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
@brief      设置表格划分单元格时的回调函数（提供一个接口供调用者更改单元格的位置及大小）

@param      Func<void (const CTable *,UINT,UINT,CPoint*,CSize*)> * cellCutFunc 回调函数指针

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTable::SetCellCutFunc(Func<void (const CTable *,UINT,UINT,CPoint*,CSize*)> * cellCutFunc)
{
	m_CellCutFunc = cellCutFunc;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      计算每个单元格的位置及大小（如果未设置行高和列宽则根据高度和宽度平均分配给每个单元格）

@param      void

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTable::CalculateCellRect()
{
	CRect contentRect = GetContentRect();
	
	// 预留按纽组标题区域
	contentRect.top += m_titleHeigh;
	// 预留底图提示区域
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
			if(colWidthIter->second==0)//未设定的话，则设定一个默认值
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
			if(rowHeightIter->second==0)//未设定的话，则设定一个默认值
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

