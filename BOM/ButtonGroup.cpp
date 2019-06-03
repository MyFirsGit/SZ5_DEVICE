#include "stdafx.h"
#include "ButtonGroup.h"
#include "MessageIDDefinition.h"

const static TCHAR* MSG_BUTTONGROUP_NEXTPAGE = _T("MSG_BUTTONGROUP_NEXTPAGE");
const static TCHAR* MSG_BUTTONGROUP_PRVIOUSPAGE = _T("MSG_BUTTONGROUP_PRVIOUSPAGE");

const static TCHAR* TXT_BUTTONGROUP_NEXTPAGE = _T("下一组");
const static TCHAR* TXT_BUTTONGROUP_PRVIOUSPAGE = _T("上一组");


IMPLEMENT_DYNAMIC(CButtonGroup,CTable)

BEGIN_MESSAGE_MAP(CButtonGroup,CTable)
	ON_MESSAGE(WM_XBUTTON, XButtonClick)
	ON_WM_PAINT()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数，从Table继承

@param      (i)UIINFO& uIInfo     当前控件的样式
@param      (i)UINT rows          需要创建Table的行数
@param      (i)UINT cols          需要创建Table的列数
@param      (i)UINT ID            需要创建按钮组的ID（用于XGroup事件中）
@param      (i)SELECTTYPE& selectType 按钮组中按钮的选择类型（多选，单选，不可选中三种）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CButtonGroup::CButtonGroup(UIINFO& uIInfo,UINT rows,UINT cols,UINT ID, SELECTTYPE selectType,UIINFO& btnUIInfo)
:CTable(uIInfo,rows,cols)
{
	m_BtnUIInfo = btnUIInfo;
	m_ID = ID;
	m_SelectType = selectType;
	m_ShowCount = 0;
	m_ShowStartIndex = 0;
	m_ShowButtons = true;
	BuildFixedButton();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CButtonGroup::~CButtonGroup()
{
	DeleteFixedButton();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置按钮组中的按钮信息

@param      (i)BTN_INFO * arrButtonInfo 按钮信息数组
@param      (i)UINT count 按钮信息的数量
@param      (i)UIINFO& 按钮的样式（不设置时将采用默认设置）
@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::SetButtonInfos(vector<BTN_INFO>& vecButtonInfo)
{
	m_ShowCount = 0;
	m_ShowStartIndex = 0;
	m_ShowPreCount =0;
	m_ShowNext = false;
	m_ShowPrevious = false;
	if(vecButtonInfo.size() > m_Rows*m_Cols)
	{
		ASSERT(m_Rows*m_Cols >= 3);
	}
	m_vecButtonInfo = vecButtonInfo;
	if(IsWindow(this->m_hWnd))
	{
			BindButton(true);
			this->Invalidate();
			if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
			{
				this->UpdateWindow();
			}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::BuildFixedButton()
{
	DeleteFixedButton();
	for(int i=0;i<m_Rows;i++)
	{
		for(int j=0;j<m_Cols;j++)
		{
			UIINFO uiInfo=m_BtnUIInfo;
			uiInfo.m_DockStyle = FILL;
			MARGINS buttonMagins={2,2,2,2};
			uiInfo.m_Margins = buttonMagins;
			uiInfo.m_Visible = false;
			uiInfo.m_pParentWnd = Cell(i,j);
			BTN_INFO btnInfo;
			btnInfo.isVisible = false;

			CXButton* pFixButton = new CXButton(uiInfo,btnInfo);
			m_vecFixedButton.push_back(pFixButton);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::DeleteFixedButton()
{
	for (int i=0;i<m_vecFixedButton.size();i++)
	{
		CXButton* pButton = m_vecFixedButton[i];
		delete pButton;
	}
	m_vecFixedButton.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      创建按钮组

@param      (i)UINT nIDTemplate
@param      (i)CWnd*

@retval     bool\n
true: 创建成功，false: 创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CButtonGroup::Create()
{
	BOOL success =__super::Create();
	for (int i=0;i<m_vecFixedButton.size();i++)
	{
		m_vecFixedButton[i]->Create();
	}
	BindButton(true);
	return success;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      创建上一页表格的按钮

@param      void

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::BindButton( bool isNext)
{

	if(isNext) m_ShowPreCount = m_ShowCount;

	int capacity = m_Cols * m_Rows;//默认没有下一页，上一页
	if(isNext) m_ShowStartIndex +=  m_ShowCount;
	else 
	{
		m_ShowStartIndex -= m_ShowPreCount;
		if(m_ShowStartIndex < (capacity - 2))
		{
			m_ShowStartIndex = 0;
		}
	}

	int leftDynamicButtonCount = m_vecButtonInfo.size() - m_ShowStartIndex;
	if(m_ShowPrevious = m_ShowStartIndex>0) capacity--; //不是第一页，需要显示上一页
	if(m_ShowNext = capacity<leftDynamicButtonCount) capacity--;//下一页显示不完，需要显示下一组
	m_ShowCount = capacity>leftDynamicButtonCount?leftDynamicButtonCount:capacity;
	for(int index=0;index<m_ShowCount;index++)
	{
		BTN_INFO& btnInfo = m_vecButtonInfo[index+m_ShowStartIndex];
		m_vecFixedButton[index]->SetID(btnInfo.btnID);
		m_vecFixedButton[index]->SetText(btnInfo.text);
		m_vecFixedButton[index]->SetMsg(btnInfo.btnMsg);
		m_vecFixedButton[index]->SetShortCutKey(btnInfo.shortCutKey);
		if(m_vecFixedButton[index]->IsSelected()!=btnInfo.isSelected)
		{
			m_vecFixedButton[index]->SetSelect(btnInfo.isSelected);
		}
	}
	if(m_ShowPrevious)
	{
		int previousIndex = m_ShowCount;
		m_vecFixedButton[previousIndex]->SetText(TXT_BUTTONGROUP_PRVIOUSPAGE);
		m_vecFixedButton[previousIndex]->SetMsg(MSG_BUTTONGROUP_PRVIOUSPAGE);
	}
	if(m_ShowNext)
	{
		int nextIndex = m_ShowCount + (m_ShowPrevious ? 1 : 0);
		m_vecFixedButton[nextIndex]->SetText(TXT_BUTTONGROUP_NEXTPAGE);
		m_vecFixedButton[nextIndex]->SetMsg(MSG_BUTTONGROUP_NEXTPAGE);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::ShowButtons(bool showButtons)
{
	if(showButtons!=m_ShowButtons)
	{
		m_ShowButtons = showButtons;
		if(IsWindow(m_hWnd))
		{
			this->Invalidate(TRUE);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::OnPaint()
{
	__super::OnPaint();

	if(!m_ShowButtons)
	{
		for (int index=0;index<m_vecFixedButton.size();index++)
		{
			m_vecFixedButton[index]->SetVisible(false);
		}
		return;
	}
	for(int index=0;index<m_ShowCount;index++)
	{
		BTN_INFO& btnInfo = m_vecButtonInfo[index+m_ShowStartIndex];
		if(m_vecFixedButton[index]->IsVisible()!=btnInfo.isVisible)
		{
			m_vecFixedButton[index]->SetVisible(btnInfo.isVisible);
		}
		if(m_vecFixedButton[index]->IsEnabled()!=btnInfo.isEnable)
		{
			m_vecFixedButton[index]->SetEnabled(btnInfo.isEnable);
		}
	}
	if(m_ShowPrevious)
	{
		int previousIndex = m_ShowCount;
		m_vecFixedButton[previousIndex]->SetEnabled(true);
		m_vecFixedButton[previousIndex]->SetVisible(true);
		m_vecFixedButton[previousIndex]->SetSelect(false);
	}
	if(m_ShowNext)
	{
		int nextIndex = m_ShowCount + (m_ShowPrevious ? 1 : 0);
		m_vecFixedButton[nextIndex]->SetEnabled(true);
		m_vecFixedButton[nextIndex]->SetVisible(true);
		m_vecFixedButton[nextIndex]->SetSelect(false);
	}
	int unusedButtonIndex = m_ShowCount + (m_ShowPrevious ? 1 : 0)+(m_ShowNext ? 1 : 0);
	for(int i=unusedButtonIndex;i<m_Cols*m_Rows;i++)
	{
		m_vecFixedButton[i]->SetVisible(false);
	}
	if(!IsWindowEnabled())
	{
		for (int index=0;index<m_vecFixedButton.size();index++)
		{
			m_vecFixedButton[index]->SetEnabled(false);
		}
		return;
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::SetSelectType(CButtonGroup::SELECTTYPE selectType)
{
	this->m_SelectType = selectType;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CButtonGroup::SELECTTYPE CButtonGroup::GetSelectType()
{
	return this->m_SelectType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      所到所有选中的的按钮

@param    void  

@retval     vector<CXButton*>& buttons 

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const vector<BTN_INFO>&  CButtonGroup::GetSelectedBtnInfo()
{
	m_vecSelectedButtonInfo.clear();
	for (int i=0;i<m_vecButtonInfo.size();i++)
	{
		if(m_vecButtonInfo[i].isSelected)
		{
			m_vecSelectedButtonInfo.push_back(m_vecButtonInfo[i]);
		}
	}
	return m_vecSelectedButtonInfo;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      清除所有的选中状态

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::ClearSelection()
{
	for (int i=0;i<m_vecButtonInfo.size();i++)
	{
		if(m_vecButtonInfo[i].isSelected)
		{
			m_vecButtonInfo[i].isSelected = false;
			if(i>=m_ShowStartIndex)
			{
				m_vecFixedButton[i-m_ShowStartIndex]->SetSelect(false);
			}
			
		}
	}
	//this->Invalidate(false);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
vector<BTN_INFO>& CButtonGroup::GetBtnInfo()
{
	return m_vecButtonInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL CButtonGroup::EnableButton(UINT ID,CString msg,bool enabled)
{
	BTN_INFO& btnInfo = FindBtnInfo(ID,msg);
	btnInfo.isEnable = enabled;
	this->Invalidate(TRUE);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL CButtonGroup::EnableButton(int index,bool enabled)
{
	if(index+m_ShowStartIndex>=m_vecButtonInfo.size())
	{
		return FALSE;
	}
	BTN_INFO& btnInfo = m_vecButtonInfo[index+m_ShowStartIndex];
	btnInfo.isEnable = enabled;
	this->Invalidate(TRUE);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL CButtonGroup::ClickButton(int index)
{
	if(index+m_ShowStartIndex>=m_vecButtonInfo.size())
	{
		return FALSE;
	}
	BTN_INFO& btnInfo = m_vecButtonInfo[index+m_ShowStartIndex];
	if(!btnInfo.isEnable || !btnInfo.isVisible)
	{
		return FALSE;
	}
	LRESULT result =m_vecFixedButton[index]->OnXButtonClick(NULL,NULL);
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      点击第一个可用的按钮

@param      void

@retval     BOOL
							TRUE	点击成功
							FALSE	失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CButtonGroup::ClickFirstAvailButton()
{
	for (int i=0;i<(m_Cols * m_Rows );i++)
	{
		if(i+m_ShowStartIndex>=m_vecButtonInfo.size())
		{
			return NULL;
		}
		BTN_INFO& btnInfo = m_vecButtonInfo[i+m_ShowStartIndex];
		if(btnInfo.isEnable && btnInfo.isVisible)
		{
			LRESULT result =m_vecFixedButton[i]->OnXButtonClick(NULL,NULL);
			return result;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
const BTN_INFO* CButtonGroup::GetFirstAvailBtnInfo()
{
	for (int i=0;i<(m_Cols * m_Rows );i++)
	{
		if(i+m_ShowStartIndex>=m_vecButtonInfo.size())
		{
			return NULL;
		}
		const BTN_INFO& btnInfo = m_vecButtonInfo[i+m_ShowStartIndex];
		if(btnInfo.isEnable && btnInfo.isVisible)
		{
			return &btnInfo;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
  @brief      按钮点击事件

  @param      (i)WPARAM   wParam     消息设置参数          
  @param      (i)LPARAM   lParam     消息设置参数

  @retval     LRESULT   \n

  @exception 无 
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CButtonGroup::XButtonClick(WPARAM wParam, LPARAM lParam)
{
	UINT ID = static_cast<UINT>(wParam);
	CString msg = (LPCTSTR)lParam;
	if(msg == MSG_BUTTONGROUP_PRVIOUSPAGE)
	{
		BindButton(false);
		this->Invalidate();
		return -1;
	}
	if(msg == MSG_BUTTONGROUP_NEXTPAGE)
	{
		BindButton(true);
		this->Invalidate();
		return -1;
	}
	switch(m_SelectType)
	{
		case  NONE :
			{
				for(int i=0;i<m_vecButtonInfo.size();i++)
				{
					m_vecButtonInfo[i].isSelected = false;
				}
				for(int i=0;i<m_vecFixedButton.size();i++)
				{
					if(m_vecFixedButton[i]->IsSelected())
					{
						m_vecFixedButton[i]->SetSelect(false);
					}
				}
				m_vecSelectedButtonInfo.clear();
				break;
			}
		case SIGLE:
			{
				for(int i=0;i<m_vecButtonInfo.size();i++)
				{
					BTN_INFO& btnInfo = m_vecButtonInfo[i];
					if(btnInfo.isSelected)
					{
						btnInfo.isSelected = false;
						if(i>=m_ShowStartIndex && i<m_ShowStartIndex+m_ShowCount)
						{
							m_vecFixedButton[i-m_ShowStartIndex]->SetSelect(false);
						}
					}
					if(btnInfo.btnID == ID && btnInfo.btnMsg == msg)
					{
						btnInfo.isSelected = true;
						if(i>=m_ShowStartIndex && i<m_ShowStartIndex+m_ShowCount)
						{
							m_vecFixedButton[i-m_ShowStartIndex]->SetSelect(true);
						}
					}
				}
				break;
			}
		case MULTIPLE:
			{
				for(int i=0;i<m_vecButtonInfo.size();i++)
				{
					BTN_INFO& btnInfo = m_vecButtonInfo[i];
					if(btnInfo.btnID == ID && btnInfo.btnMsg == msg)
					{
						btnInfo.isSelected = !btnInfo.isSelected ;
						if(i>=m_ShowStartIndex && i<m_ShowStartIndex+m_ShowCount)
						{
							m_vecFixedButton[i-m_ShowStartIndex]->SetSelect(btnInfo.isSelected);
						}
					}
				}
				break;
			}
		default:
			ASSERT(false);
	}
	BTN_INFO& btnInfo = FindBtnInfo(ID,msg);
	GetParentDlg()->PostMessage(WM_XGROUP,m_ID,(LPARAM)&btnInfo);
	return -1;//阻止冒泡，不传到上一层。
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BTN_INFO& CButtonGroup::FindBtnInfo(UINT ID,CString msg)
{
	for (int i=0;i<m_vecButtonInfo.size();i++)
	{
		BTN_INFO& btnInfo = m_vecButtonInfo[i];
		if(btnInfo.btnID == ID && btnInfo.btnMsg == msg)
		{
			return btnInfo;
		}
	}
	throw CInnerException(CInnerException::MODULE_ID,CInnerException::FUNC_ERR_RETURN,_T(__FILE__),__LINE__);
}