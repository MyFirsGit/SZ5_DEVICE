#pragma once
#include "Table.h"
#include "GStatic.h"
#include "GroupBox.h"
#include "Lable.h"

enum CONTROLTYPE
{
	LABEL,
	EDIT,
	BUTTON
};

typedef struct _CustomControlInfo
{
	CONTROLTYPE type;
	CString text;
	_CustomControlInfo()
	{
		type = LABEL;
		text = _T("");
	}

}CUSTOMCONTROLINFO,*PCUSTOMCONTROLINFO ;



typedef struct _CompositInfo
{
	CString caption;
	UINT rowsCount;
	UINT colsCount;
	map<pair<UINT,UINT>,CUSTOMCONTROLINFO> controlsInfo;

	_CompositInfo()
	{
		caption = "";
		rowsCount =1;
		colsCount = 1;

	}
	void ClearControlInfo()
	{
		controlsInfo.clear();
	}
	void SetControlInfo(CUSTOMCONTROLINFO* controlInfos,UINT count, UINT startIndex=0)
	{
		for(unsigned int i=0;i<count && (i+startIndex)<rowsCount*colsCount;i++)
		{
			UINT rowIndex = (i+startIndex)/colsCount;
			UINT colIndex = (i+startIndex)%colsCount;
			controlsInfo[pair<UINT,UINT>(rowIndex,colIndex)] = controlInfos[i];
		}
	}
	void SetControlInfo(CONTROLTYPE* controlTypes, CString* controlTexts, UINT count, UINT startIndex=0)
	{
		for(unsigned int i=0;i<count && (i+startIndex)<rowsCount*colsCount;i++)
		{
			CUSTOMCONTROLINFO controlInfo;
			controlInfo.text =controlTexts[i];
			controlInfo.type = controlTypes[i];
			UINT rowIndex = (i+startIndex)/colsCount;
			UINT colIndex = (i+startIndex)%colsCount;
			controlsInfo[pair<UINT,UINT>(rowIndex,colIndex)] = controlInfo;
		}
	}
	void SetControlInfo(vector<CONTROLTYPE> controlTypes,UINT startIndex=0)
	{
		for(unsigned int i=0;i<controlTypes.size() && (i+startIndex)<rowsCount*colsCount;i++)
		{
			CUSTOMCONTROLINFO controlInfo;
			controlInfo.text = _T("");
			controlInfo.type = controlTypes[i];
			UINT rowIndex = (i+startIndex)/colsCount;
			UINT colIndex = (i+startIndex)%colsCount;
			controlsInfo[pair<UINT,UINT>(rowIndex,colIndex)] = controlInfo;
		}
	}

	void SetControlInfo(vector<CUSTOMCONTROLINFO> controlInfos,UINT startIndex=0)
	{
		for(unsigned int i=0;i<controlInfos.size() && (i+startIndex)<rowsCount*colsCount;i++)
		{
			
			UINT rowIndex = (i+startIndex)/colsCount;
			UINT colIndex = (i+startIndex)%colsCount;
			controlsInfo[pair<UINT,UINT>(rowIndex,colIndex)] = controlInfos[i];
		}
	}

	void SetControlInfo(CONTROLTYPE controlType)
	{
		for(unsigned int rowIndex=0;rowIndex<rowsCount;rowIndex++)
		{
			for(unsigned int colIndex=0;colIndex<colsCount;colIndex++)
			{
				CUSTOMCONTROLINFO controlInfo;
				controlInfo.text = _T("");
				controlInfo.type = controlType;
				controlsInfo[pair<UINT,UINT>(rowIndex,colIndex)] = controlInfo;
			}
		}
	}

	void SetControlInfo(CUSTOMCONTROLINFO controlInfo,int rowIndex,int colIndex)
	{
		ASSERT(colIndex<colsCount);
		ASSERT(rowIndex<rowsCount);
		controlsInfo[pair<UINT,UINT>(rowIndex,colIndex)] = controlInfo;
	}

	void SetControlInfo(CONTROLTYPE controlType,int rowIndex,int colIndex)
	{
		ASSERT(colIndex<colsCount);
		ASSERT(rowIndex<rowsCount);
		CUSTOMCONTROLINFO controlInfo;
		controlInfo.text = _T("");
		controlInfo.type = controlType;
		controlsInfo[pair<UINT,UINT>(rowIndex,colIndex)] = controlInfo;
	}

	void SetControlInfoByCol(UINT colIndex,CONTROLTYPE controlType)
	{
		ASSERT(colIndex<colsCount);
		for(unsigned int rowIndex=0;rowIndex<rowsCount;rowIndex++)
		{
			CUSTOMCONTROLINFO controlInfo;
			controlInfo.text = _T("");
			controlInfo.type = controlType;
			controlsInfo[pair<UINT,UINT>(rowIndex,colIndex)] = controlInfo;
		}
	}
	void SetControlInfoByRow(UINT rowIndex,CONTROLTYPE controlType)
	{
		ASSERT(rowIndex<rowsCount);
		for(unsigned int colIndex=0;colIndex<colsCount;colIndex++)
		{
			CUSTOMCONTROLINFO controlInfo;
			controlInfo.text = _T("");
			controlInfo.type = controlType;
			controlsInfo[pair<UINT,UINT>(rowIndex,colIndex)] = controlInfo;
		}
	}

} COMPOSITINFO,*PCOMPOSITINFO;



class CCompositGroupBox:public CGroupBox
{
	
public:

	CCompositGroupBox(UIINFO& uiInfo);

	virtual ~CCompositGroupBox();

	COMPOSITINFO GetCompositInfo();

	void SetCompositInfo(COMPOSITINFO& info);

	virtual BOOL Create();

	//CLable* GetCaption();

	CTable* GetContentTable();

	template<typename T>
	T* GetControl(UINT rowIndex,UINT colIndex)
	{
		return TryConvert<T>(m_Controls[pair<UINT,UINT>(rowIndex,colIndex)]);
	}

	template<typename T>
	void GetControls(vector<T*>& controls)
	{
		for(map<pair<UINT,UINT>, CWnd*>::iterator controlIter = m_Controls.begin();controlIter!=m_Controls.end();controlIter++)
		{
			T* c = TryConvert<T>(controlIter->second);
			if(c!=NULL)
			{
				controls.push_back(c);
			}
		}
	}

	void SetContentControlUIByCol(UIINFO& uiInfo,UINT colIndex);

	void SetContentControlUIByRow(UIINFO& uiInfo,UINT rowIndex);

	void SetContentControlUI(UIINFO& uiInfo,UINT rowIndex,UINT colIndex);

private:
	COMPOSITINFO m_Info;

	//CLable * m_pLblCaption;	//标题
	CTable * m_pTbl;		//内容表格

	map<pair<UINT,UINT>, CWnd*> m_Controls; //控件

	void DeleteControls();

	void BuildControls(COMPOSITINFO& info);


	template<typename T>
	T* TryConvert(CWnd* pWnd)
	{
		T* pT = NULL;
		if(pWnd->IsKindOf(RUNTIME_CLASS(T)))
		{
			pT = reinterpret_cast<T*>(pWnd);
		}
		return pT;
	}
};