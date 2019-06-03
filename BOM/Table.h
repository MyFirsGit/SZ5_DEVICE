#pragma once
#include "Panel.h"
#include "delegate.h"
#include <vector>

class CTable;

class CCell:public CPanel
{
	DECLARE_DYNAMIC(CCell)
public:
	CCell(UIINFO& uIInfo);
	virtual ~CCell();

};

class CTable:public CPanel
{
	
public:
	DECLARE_DYNAMIC(CTable)
	CTable(
		UIINFO& uIInfo
		,UINT rows
		,UINT cols
		
		);
	
	CCell* Cell(UINT,UINT);

	virtual ~CTable();

	virtual BOOL Create();

	void SetColWidth(UINT width,UINT colIndex);

	void SetColWidth(UINT* widths,UINT count,UINT startColIndex = 0);

	void SetRowHeight(UINT height,UINT rowIndex);

	void SetRowHeight(UINT* heights,UINT count,UINT startRowIndex = 0);


	void SetCellCutFunc(Func<void (const CTable *,UINT,UINT,CPoint*,CSize*)> * cellCutFunc);
	

protected:
	void CalculateCellRect();
	map<pair<UINT,UINT> ,CCell*> cells;

	map<UINT,UINT> m_mapColWidths;
	map<UINT,UINT> m_mapRowHeights;

	UINT m_Rows;
	UINT m_Cols;
	
	Func<void (const CTable const *,UINT,UINT,CPoint*,CSize*)> * m_CellCutFunc;
};