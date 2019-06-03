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
@brief      ���캯������CGroupBox�̳�

@param      (i)UIINFO& uIInfo     ��ǰ�ؼ�����ʽ

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCompositGroupBox::~CCompositGroupBox()
{
	DeleteControls();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ����Ͽ�������Ϣ

@param      ��

@retval     COMPOSITINFO

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COMPOSITINFO CCompositGroupBox::GetCompositInfo()
{
	return m_Info;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ͽ�������Ϣ

@param      (i)COMPOSITINFO& info��Ͽ�������Ϣ

@retval     void

@exception  ��
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
@brief      ����һ����Ͽ�

@param      void

@retval     BOOL
								TRUE �ɹ�
								FALSE	ʧ��

@exception  ��
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
@brief      ȡ�ñ����ǩLable

@param      void

@retval     CLable* ��ǩ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//CLable* CCompositGroupBox::GetCaption()
//{
//	return m_pLblCaption;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ñ���пؼ�����ʽ

@param      UIINFO& uiInfo �ؼ���ʽ
@param	 UINT colIndex ������(��0��ʼ����)

@retval     void

@exception  ��
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
@brief      �������ñ���пؼ�����ʽ

@param      UIINFO& uiInfo �ؼ���ʽ
@param	 UINT rowIndex ������(��0��ʼ����)

@retval     void

@exception  ��
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
@brief      ��ָ���к�ָ�������ñ���пؼ�����ʽ

@param      UIINFO& uiInfo �ؼ���ʽ
@param	 UINT rowIndex ������(��0��ʼ����)
@param	 UINT colIndex ������(��0��ʼ����)
@retval     void

@exception  ��
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
@brief      ȡ����Ͽ��е����ݱ��

@param      void
@retval     CTable* ���ݱ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTable* CCompositGroupBox::GetContentTable()
{
	return m_pTbl;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ͽ��е��ӿؼ�

@param      COMPOSITINFO& info ��Ͽ�������Ϣ
@retval     void

@exception  ��
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
@brief      ɾ���ӿؼ�

@param      void
@retval     void

@exception  ��
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
