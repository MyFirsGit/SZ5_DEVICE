#include "stdafx.h"
#include "CompositGroupBox.h"
#include "XButton.h"
#include "gedit.h"

const static UINT DEFAULT_HEIGHT_CATPION = 50;
const static UINT DEFAULT_HEIGHT_ROW	= 35;
const static UINT DEFAULT_HEIGHT_BUTTON = 55;
const static UINT DEFAULT_WIDTH_COL = 400;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数，从CGroupBox继承

@param      (i)UIINFO& uIInfo     当前控件的样式

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCompositGroupBox::CCompositGroupBox(UIINFO& uiInfo)
:CGroupBox(uiInfo)
{
	//m_pLblCaption = NULL;
	m_pTbl = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCompositGroupBox::~CCompositGroupBox()
{
	DeleteControls();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得组合框内容信息

@param      无

@retval     COMPOSITINFO

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COMPOSITINFO CCompositGroupBox::GetCompositInfo()
{
	return m_Info;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置组合框内容信息

@param      (i)COMPOSITINFO& info组合框内容信息

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCompositGroupBox::SetCompositInfo(COMPOSITINFO& info)
{
	m_UIInfo.m_Size.cy = DEFAULT_HEIGHT_CATPION;
	for(int i=0;i<info.rowsCount;i++)
	{
		UINT rowHeight = DEFAULT_HEIGHT_ROW;
		for(int j=0;j<info.colsCount;j++)
		{
			if(info.controlsInfo[pair<UINT,UINT>(i,j)].type == CONTROLTYPE::BUTTON)
			{
				rowHeight = DEFAULT_HEIGHT_BUTTON;
				break;
			}
		}
		m_UIInfo.m_Size.cy+=rowHeight;
	}
	//+ DEFAULT_HEIGHT_ROW * info.rowsCount;
	
	m_Info = info;
	BuildControls(info);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      创建一个组合框

@param      void

@retval     BOOL
								TRUE 成功
								FALSE	失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCompositGroupBox::Create()
{
	BOOL success = __super::Create();
	if(!success)
	{
		return success;
	}
	/*if(m_pLblCaption!=NULL){
		success = m_pLblCaption->Create();
		if(!success)
		{
			return success;
		}
	}*/
	if(m_pTbl!=NULL)
	{
		success = m_pTbl->Create();
		if(!success)
		{
			return success;
		}
	}
	for(map<pair<UINT,UINT>,CWnd*>::iterator iter=m_Controls.begin();iter!=m_Controls.end();iter++)
	{
		CWnd* control = iter->second;
		if(control)
		{
			CXButton* button = TryConvert<CXButton>(control);
			if(button!=NULL)
			{
				if(!button->Create())
				{
					success = false;
				}
			}
			CGEdit* edit = TryConvert<CGEdit>(control);
			if(edit!=NULL)
			{
				if(!edit->Create())
				{
					success = false;	
				}
			}
			CLable* lable = TryConvert<CLable>(control);
			if(lable!=NULL)
			{
				if(!lable->Create())
				{
					success = false;
				}
			}
		}
	}
	return success;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得标题标签Lable

@param      void

@retval     CLable* 标签

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//CLable* CCompositGroupBox::GetCaption()
//{
//	return m_pLblCaption;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      按列设置表格中控件的样式

@param      UIINFO& uiInfo 控件样式
@param	 UINT colIndex 列索引(从0开始计算)

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCompositGroupBox::SetContentControlUIByCol(UIINFO& uiInfo,UINT colIndex)
{
	ASSERT(colIndex<m_Info.colsCount);
	for(map<pair<UINT,UINT>,CWnd*>::iterator iter=m_Controls.begin();iter!=m_Controls.end();iter++)
	{
		pair<UINT,UINT> controlLocation = iter->first;
		if(controlLocation.second==colIndex)
		{
			CWnd* control = iter->second;
			CXButton* button = TryConvert<CXButton>(control);
			if(button!=NULL)
			{
				button->SetUIInfo(uiInfo);
			}
			CGEdit* edit = TryConvert<CGEdit>(control);
			if(edit!=NULL)
			{
				edit->SetUIInfo(uiInfo);
			}
			CLable* lable = TryConvert<CLable>(control);
			if(lable!=NULL)
			{
				lable->SetUIInfo(uiInfo);
			}
		}
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      按行设置表格中控件的样式

@param      UIINFO& uiInfo 控件样式
@param	 UINT rowIndex 行索引(从0开始计算)

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCompositGroupBox::SetContentControlUIByRow(UIINFO& uiInfo,UINT rowIndex)
{
	ASSERT(rowIndex<m_Info.rowsCount);
	for(map<pair<UINT,UINT>,CWnd*>::iterator iter=m_Controls.begin();iter!=m_Controls.end();iter++)
	{
		pair<UINT,UINT> controlLocation = iter->first;
		if(controlLocation.first==rowIndex)
		{
			CWnd* control = iter->second;
			CXButton* button = TryConvert<CXButton>(control);
			if(button!=NULL)
			{
				button->SetUIInfo(uiInfo);
			}
			CGEdit* edit = TryConvert<CGEdit>(control);
			if(edit!=NULL)
			{
				edit->SetUIInfo(uiInfo);
			}
			CLable* lable = TryConvert<CLable>(control);
			if(lable!=NULL)
			{
				lable->SetUIInfo(uiInfo);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      按指定行和指定列设置表格中控件的样式

@param      UIINFO& uiInfo 控件样式
@param	 UINT rowIndex 行索引(从0开始计算)
@param	 UINT colIndex 列索引(从0开始计算)
@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCompositGroupBox::SetContentControlUI(UIINFO& uiInfo,UINT rowIndex,UINT colIndex)
{
	ASSERT(colIndex<m_Info.colsCount);
	ASSERT(rowIndex<m_Info.rowsCount);
	CWnd* control = m_Controls[pair<UINT,UINT>(rowIndex,colIndex)];
	if(control!=NULL)
	{
		CXButton* button = TryConvert<CXButton>(control);
		if(button!=NULL)
		{
			button->SetUIInfo(uiInfo);
		}
		CGEdit* edit = TryConvert<CGEdit>(control);
		if(edit!=NULL)
		{
			edit->SetUIInfo(uiInfo);
		}
		CLable* lable = TryConvert<CLable>(control);
		if(lable!=NULL)
		{
			lable->SetUIInfo(uiInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得组合框中的内容表格

@param      void
@retval     CTable* 内容表格

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTable* CCompositGroupBox::GetContentTable()
{
	return m_pTbl;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      构建组合框中的子控件

@param      COMPOSITINFO& info 组合框内容信息
@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCompositGroupBox::BuildControls(COMPOSITINFO& info)
{
	DeleteControls();
	
	//UIINFO captionUIInfo;
	//captionUIInfo.m_DockStyle = DOCKSTYLE::TOP;
	//captionUIInfo.m_Size.cy = DEFAULT_HEIGHT_CATPION;
	//captionUIInfo.m_pParentWnd = this;
	//captionUIInfo.m_Location.y = 15;
	//captionUIInfo.m_Margins.cxLeftWidth = 15;
	//captionUIInfo.m_Font.lfWeight = FW_HEAVY * 1.2;

	//m_pLblCaption = new CLable(captionUIInfo,info.caption);
	GetCaption()->SetText(info.caption);

	UIINFO tblUIInfo;
	tblUIInfo.m_pParentWnd = this;
	tblUIInfo.m_DockStyle = DOCKSTYLE::BOTTOM;
	tblUIInfo.m_Size.cy = m_UIInfo.m_Size.cy - DEFAULT_HEIGHT_CATPION;
	
	
	m_pTbl = new CTable(tblUIInfo,info.rowsCount,info.colsCount);

	for(map<pair<UINT,UINT>,CUSTOMCONTROLINFO>::iterator iter=info.controlsInfo.begin();iter!=info.controlsInfo.end();iter++)
	{
		pair<UINT,UINT> controlLocation = iter->first;
		CUSTOMCONTROLINFO controlInfo = iter->second;
		
			CWnd* p_OldControl = m_Controls[controlLocation];
			if(p_OldControl!=NULL)
			{
				delete p_OldControl;
				p_OldControl = NULL;
			}

			CWnd* pControl = NULL;
			UIINFO controlUIInfo;
			controlUIInfo.m_pParentWnd = m_pTbl->Cell(controlLocation.first,controlLocation.second);
			controlUIInfo.m_Location.x = 10;
			controlUIInfo.m_DockStyle = controlLocation.second % 2 != 0 ? LEFT : RIGHT;

			switch(controlInfo.type)
			{
				case CONTROLTYPE::BUTTON:
					{
						BTN_INFO buttonInfo;
						buttonInfo.btnID = 0;
						buttonInfo.isSelected = FALSE;
						buttonInfo.text = controlInfo.text;

						controlUIInfo.m_Size.cy = 40;
						controlUIInfo.m_Size.cx = 100;
						m_pTbl->SetRowHeight(DEFAULT_HEIGHT_BUTTON,controlLocation.first);
						pControl = new CXButton(controlUIInfo,buttonInfo);
						break;
					}
				case CONTROLTYPE::EDIT:
					{
						controlUIInfo.m_Size.cy = 22;
						controlUIInfo.m_Size.cx = 100;
						controlUIInfo.m_BorderWidth = 1;
						
						pControl = new CGEdit(controlUIInfo);						
						break;
					}
				case CONTROLTYPE::LABEL:
					{
						pControl = new CLable(controlUIInfo,controlInfo.text);
						break;
					}
				default:
					{
						pControl = new CLable(controlUIInfo,controlInfo.text);
						break;
					}
					
			}
			m_Controls[controlLocation] = pControl;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除子控件

@param      void
@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCompositGroupBox::DeleteControls()
{
	/*delete m_pLblCaption;
	m_pLblCaption = NULL;*/

	delete m_pTbl;
	m_pTbl = NULL;

	for(map<pair<UINT,UINT>,CWnd*>::iterator iter=m_Controls.begin();iter!=m_Controls.end();iter++)
	{
		CWnd* control = iter->second;
		if(control)
		{
			delete control;
			control = NULL;
		}
	}
	m_Controls.clear();
	if(IsWindow(m_hWnd))
	{
		this->Invalidate();
	}
}
