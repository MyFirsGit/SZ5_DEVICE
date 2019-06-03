#include "stdafx.h"
#include "ButtonGroup.h"
#include "MessageIDDefinition.h"

const static TCHAR* MSG_BUTTONGROUP_NEXTPAGE = _T("MSG_BUTTONGROUP_NEXTPAGE");
const static TCHAR* MSG_BUTTONGROUP_PRVIOUSPAGE = _T("MSG_BUTTONGROUP_PRVIOUSPAGE");

const static TCHAR* TXT_BUTTONGROUP_NEXTPAGE = _T("��һ��");
const static TCHAR* TXT_BUTTONGROUP_PRVIOUSPAGE = _T("��һ��");


IMPLEMENT_DYNAMIC(CButtonGroup,CTable)

BEGIN_MESSAGE_MAP(CButtonGroup,CTable)
	ON_MESSAGE(WM_XBUTTON, XButtonClick)
	ON_WM_PAINT()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯������Table�̳�

@param      (i)UIINFO& uIInfo     ��ǰ�ؼ�����ʽ
@param      (i)UINT rows          ��Ҫ����Table������
@param      (i)UINT cols          ��Ҫ����Table������
@param      (i)UINT ID            ��Ҫ������ť���ID������XGroup�¼��У�
@param      (i)SELECTTYPE& selectType ��ť���а�ť��ѡ�����ͣ���ѡ����ѡ������ѡ�����֣�

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CButtonGroup::~CButtonGroup()
{
	DeleteFixedButton();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ð�ť���еİ�ť��Ϣ

@param      (i)BTN_INFO * arrButtonInfo ��ť��Ϣ����
@param      (i)UINT count ��ť��Ϣ������
@param      (i)UIINFO& ��ť����ʽ��������ʱ������Ĭ�����ã�
@retval     ��

@exception  ��
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
@brief      ������ť��

@param      (i)UINT nIDTemplate
@param      (i)CWnd*

@retval     bool\n
true: �����ɹ���false: ����ʧ��

@exception  ��
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
@brief      ������һҳ���İ�ť

@param      void

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::BindButton( bool isNext)
{

	if(isNext) m_ShowPreCount = m_ShowCount;

	int capacity = m_Cols * m_Rows;//Ĭ��û����һҳ����һҳ
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
	if(m_ShowPrevious = m_ShowStartIndex>0) capacity--; //���ǵ�һҳ����Ҫ��ʾ��һҳ
	if(m_ShowNext = capacity<leftDynamicButtonCount) capacity--;//��һҳ��ʾ���꣬��Ҫ��ʾ��һ��
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
@brief      ��������ѡ�еĵİ�ť

@param    void  

@retval     vector<CXButton*>& buttons 

@exception  ��
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
@brief      ������е�ѡ��״̬

@param      ��

@retval     ��

@exception  ��
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
@brief      �����һ�����õİ�ť

@param      void

@retval     BOOL
							TRUE	����ɹ�
							FALSE	ʧ��

@exception  ��
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
  @brief      ��ť����¼�

  @param      (i)WPARAM   wParam     ��Ϣ���ò���          
  @param      (i)LPARAM   lParam     ��Ϣ���ò���

  @retval     LRESULT   \n

  @exception �� 
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
	return -1;//��ֹð�ݣ���������һ�㡣
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