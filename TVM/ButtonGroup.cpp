#include "stdafx.h"
#include "ButtonGroup.h"
#include "MessageIDDefinition.h"

const static TCHAR* MSG_BUTTONGROUP_NEXTPAGE = _T("MSG_BUTTONGROUP_NEXTPAGE");
const static TCHAR* MSG_BUTTONGROUP_PRVIOUSPAGE = _T("MSG_BUTTONGROUP_PRVIOUSPAGE");

const static TCHAR* TXT_BUTTONGROUP_NEXTPAGE = _tl(TXT_BTN_GROUP_NEXT);
const static TCHAR* TXT_BUTTONGROUP_PRVIOUSPAGE = _tl(TXT_BTN_GROUP_PREVIOUS);


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
CButtonGroup::CButtonGroup(UIINFO& uIInfo,UINT rows,UINT cols,UINT ID, SELECTTYPE selectType,BUTTON_TYPE btnType,UIINFO& btnUIInfo,CString titleName,int groupType,int noticeWidth)
:CTable(uIInfo,rows,cols,titleName==_T("--")?0:GBUTTON_TITLE_HEIGHT,noticeWidth)
{
	m_GroupType = groupType;
	m_uIInfo = uIInfo;
	btnUIInfo.m_Font = GUEST_FUNC_BTN_FONT/*theGUI_INFO.GetBtnFont()*/;
	m_BtnUIInfo = btnUIInfo;
	m_ID = ID;
	m_SelectType = selectType;
	m_ShowCount = 0;
	m_ShowStartIndex = 0;
	m_ShowButtons = true;
	m_titleName = titleName;
	m_btnType = btnType;
	// 加载btn背景图片
	CString bmppath;
	CString apppath;
	GetAppPath(apppath);
	bmppath = apppath + TVM_IMAGE_FUNC_BTN_BKG;
	m_Logo.Load(bmppath); 
	// 创建按钮
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
	m_nNextIndex = 0;
	m_nPreviousIndex = 0;
	if(vecButtonInfo.size() > m_Rows*m_Cols)
	{
		ASSERT(m_Rows*m_Cols >= 3);
	}
	m_vecButtonInfo = vecButtonInfo;
	m_vecEnableButtonInfo.clear();
	if(IsWindow(this->m_hWnd))
	{
			BindButton(true);
			if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
			{
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
			uiInfo.m_DockStyle = uiInfo.m_DockStyle == NONE?FILL:uiInfo.m_DockStyle;
			uiInfo.m_Margins = m_uIInfo.m_Margins;
			uiInfo.m_Visible = false;
			uiInfo.m_pParentWnd = Cell(i,j);
			uiInfo.m_Font = theGUI_INFO.GetBtnFont();
			BTN_INFO btnInfo;
			btnInfo.isVisible = false;
			btnInfo.type = m_btnType;

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
	// 如果通常按钮组，则处理方式延续之前的
	if(m_GroupType==0){
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
			m_nPreviousIndex = previousIndex;
			m_vecFixedButton[previousIndex]->SetText(_tl(TXT_PAGE_UP));
			m_vecFixedButton[previousIndex]->SetMsg(MSG_BUTTONGROUP_PRVIOUSPAGE);
		}
		if(m_ShowNext)
		{
			int nextIndex = m_ShowCount + (m_ShowPrevious ? 1 : 0);
			m_nNextIndex = nextIndex;
			m_vecFixedButton[nextIndex]->SetText(_tl(TXT_PAGE_DOWN));
			m_vecFixedButton[nextIndex]->SetMsg(MSG_BUTTONGROUP_NEXTPAGE);
		}
	}
	// 特殊按钮组，需要单独处理（固定首位各有一个固定的按钮项，且固定有上一页和下一页按钮）
	else{
		// 去掉首尾的固定按钮
		int a = m_vecButtonInfo.size();
		if(a==0) return;

		int autoShowCount = a - 2;// 减去首尾的两个
		int infactShowCount = m_ShowCount-4;
		infactShowCount = infactShowCount<0?0:infactShowCount;
		m_ShowPrevious = true;
		m_ShowNext = true;
		int capacity = m_Cols * m_Rows;


		if(isNext) {
			m_ShowPreCount = infactShowCount;
			m_ShowStartIndex +=  infactShowCount;
		}
		else{
			m_ShowStartIndex -= m_ShowPreCount;
			if(m_ShowStartIndex < (capacity - 4)){
				m_ShowStartIndex = 0;
			}
		}

		int leftDynamicButtonCount = autoShowCount - m_ShowStartIndex + 4;
		m_ShowCount = capacity>leftDynamicButtonCount?leftDynamicButtonCount:capacity;//显示元素的个数
	
		for(int index=0;index<m_ShowCount;index++)
		{
				m_vecFixedButton[index]->SetEnabled(true);
				m_vecFixedButton[index]->SetSelect(false);
			if (index==0){
				BTN_INFO& btnInfo = m_vecButtonInfo[0];
				m_vecFixedButton[index]->SetID(btnInfo.btnID);
				m_vecFixedButton[index]->SetText(btnInfo.text);
				m_vecFixedButton[index]->SetMsg(btnInfo.btnMsg);
				m_vecFixedButton[index]->SetShortCutKey(btnInfo.shortCutKey);
				if(m_vecFixedButton[index]->IsSelected()!=btnInfo.isSelected){
					m_vecFixedButton[index]->SetSelect(btnInfo.isSelected);
				}
			}
			else if (index==1){
				if (m_ShowStartIndex==0){
					m_nPreviousIndex = index;
					m_vecFixedButton[index]->SetEnabled(false);
					m_ShowNext = false;//实际这里标识是否置灰
				}
				else{
					m_vecFixedButton[index]->SetEnabled(true);
					m_ShowNext = true;//实际这里标识是否置灰
				}
				m_vecFixedButton[index]->SetText(_tl(TXT_PAGE_UP));
				m_vecFixedButton[index]->SetMsg(MSG_BUTTONGROUP_PRVIOUSPAGE);
			}
			else if (index==m_ShowCount-2){
				m_nNextIndex = index;
				if (leftDynamicButtonCount<=capacity){
					m_vecFixedButton[index]->SetEnabled(false);
					m_ShowPrevious = false;//实际这里标识是否置灰
				}
				else{
					m_vecFixedButton[index]->SetEnabled(true);
					m_ShowPrevious = true;//实际这里标识是否置灰
				}
				m_vecFixedButton[index]->SetText(_tl(TXT_PAGE_DOWN));
				m_vecFixedButton[index]->SetMsg(MSG_BUTTONGROUP_NEXTPAGE);
			}
			else if (index==m_ShowCount-1){
				BTN_INFO& btnInfo = m_vecButtonInfo[m_vecButtonInfo.size()-1];
				m_vecFixedButton[index]->SetID(btnInfo.btnID);
				m_vecFixedButton[index]->SetText(btnInfo.text);
				m_vecFixedButton[index]->SetMsg(btnInfo.btnMsg);
				m_vecFixedButton[index]->SetShortCutKey(btnInfo.shortCutKey);
				/*if(m_vecFixedButton[index]->IsSelected()!=btnInfo.isSelected)*/{
					m_vecFixedButton[index]->SetSelect(btnInfo.isSelected);
				}
			}
			else{
				BTN_INFO& btnInfo = m_vecButtonInfo[index-1+m_ShowStartIndex];
				m_vecFixedButton[index]->SetID(btnInfo.btnID);
				m_vecFixedButton[index]->SetText(btnInfo.text);
				m_vecFixedButton[index]->SetMsg(btnInfo.btnMsg);
				m_vecFixedButton[index]->SetShortCutKey(btnInfo.shortCutKey);
				/*if(m_vecFixedButton[index]->IsSelected()!=btnInfo.isSelected)*/{
					m_vecFixedButton[index]->SetSelect(btnInfo.isSelected);
				}
			}
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
	//__super::OnPaint();

	CPaintDC dc(this); // device context for painting
	// 绘制边框
	if(m_uIInfo.m_BorderWidth!=0){
		CPen pen(PS_SOLID, m_uIInfo.m_BorderWidth, m_uIInfo.m_BorderColor);
		CPen* pOldPen = (CPen*)dc.SelectObject(&pen);
		dc.Rectangle(2, 2, m_uIInfo.m_Size.cx-4, m_uIInfo.m_Size.cy-4);
		dc.SelectObject(pOldPen);
		
	}

	if(!m_ShowButtons)
	{
		for (int index=0;index<m_vecFixedButton.size();index++)
		{
			m_vecFixedButton[index]->SetVisible(false);
		}
		this->ShowWindow(false);
		return;
	}
	else{
		this->ShowWindow(true);
	}

	if(m_titleName!=_T("--")){
		//// 1. 绘制容器矩形
		//CPen pen(PS_SOLID, 1, BLUE/*m_uIInfo.m_BorderColor*/);
		//CBrush brush(BLUE);
		//CPen* pOldPen = (CPen*)dc.SelectObject(&pen);
		//CBrush* pOldBrush = (CBrush*)dc.SelectObject(&brush);
		//dc.Rectangle(3, 3, m_uIInfo.m_Size.cx - 4, GBUTTON_TITLE_HEIGHT + 2);
		//dc.SelectObject(pOldPen);
		//dc.SelectObject(pOldBrush);
		//
		//// 2. 绘制logo
		//CString bmppath;
		//CString apppath;
		//GetAppPath(apppath);
		//bmppath = apppath + TVM_IMAGE_LOGO;
		//CImage m_Logo;
		//m_Logo.Load(bmppath); 
		//if(!m_Logo.IsNull()) ConvertPng(&m_Logo);
		//if (!m_Logo.IsNull()){
		//	RECT logoRect;
		//	logoRect.left = 2;
		//	logoRect.top = 2;
		//	logoRect.right = m_Logo.GetWidth() + 2;
		//	logoRect.bottom = GBUTTON_TITLE_HEIGHT/*m_Logo.GetHeight()*/ - 2;
		//	m_Logo.Draw(dc.m_hDC, logoRect);
		//}

		dc.SetBkMode(TRANSPARENT);
		// 绘制背景图片
		if(!m_Logo.IsNull()) ConvertPng(&m_Logo);
		if (!m_Logo.IsNull()){
			//RECT logoRect;
			//logoRect.left = 2;
			//logoRect.top = 2;
			//logoRect.right = m_uIInfo.m_Size.cx-4-1/*m_Logo.GetWidth() - 1*/;
			//logoRect.bottom = GBUTTON_TITLE_HEIGHT/*m_Logo.GetHeight()*/;
			//m_Logo.Draw(dc.m_hDC, logoRect);
		}
		// 3. 绘制文字
		if (theAPP_SESSION.GetCurrentClientLanguageType() != LANGUAGETYPE_T::PORTUGUESE)
		{
			dc.SetTextColor(GUEST_FUNC_BTN_TITLE_FONT_COLOR/*ORANGE*/);
			DWORD dwFontSize = theMAINTENANCE_INFO.GetLanguageMode()== CHINESE?45:30;
			HFONT hTitleFont=::CreateFont(dwFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
				CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
				DEFAULT_PITCH|FF_SWISS, GUEST_FUNC_BTN_TITLE_FONT_NAME/*guideFountName*/);

			HFONT hOldFont=(HFONT)::SelectObject(dc.m_hDC,hTitleFont);
			//dc.TextOut(2*DEFAULT_GROUPBOX_MARGINS.cxLeftWidth, 2*DEFAULT_GROUPBOX_MARGINS.cyTopHeight, m_titleName);
			if (m_titleName.Find(_T("^")) == -1)
			{
				dc.TextOut(30, 5+2*DEFAULT_GROUPBOX_MARGINS.cyTopHeight, m_titleName);
			}
			else
			{
				int pos = m_titleName.Find(_T("^"));
				dc.TextOut(30, 5+2*DEFAULT_GROUPBOX_MARGINS.cyTopHeight, m_titleName.Left(pos));
				dc.TextOut(30, 5+2*DEFAULT_GROUPBOX_MARGINS.cyTopHeight+50, m_titleName.Mid(pos+1));
			}
			::SelectObject(dc.m_hDC,hOldFont);
			::DeleteObject(hTitleFont);
		}
		else
		{
			CFont vertFont;
			CRect rect;
			GetClientRect(rect);
			CStringArray* vecText = SplitCString(m_titleName,_T(" "));
			//字体
			vertFont.CreateFont(30, 0, -900, 900, FW_BOLD,
				0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH, _T("Arial"));
			CFont *pOldFont = dc.SelectObject(&vertFont);
			COLORREF oldColor = dc.GetTextColor();
			dc.SetTextColor(GUEST_STATE_STATION_FONT_COLOR);  //文本颜色
			dc.SetBkMode(TRANSPARENT);
			int nWeight = 0;
			int it = 0;
			while(it < vecText->GetSize())
			{
				CString szCnt = vecText->GetAt(it);
				if (m_ID == TOP_1_GROUP_ID)
				{
					dc.TextOut(40+nWeight, 5+2*DEFAULT_GROUPBOX_MARGINS.cyTopHeight, szCnt,szCnt.GetLength()); //文本起始位置
				}
				else
				{
					dc.TextOut(60+nWeight, 5+2*DEFAULT_GROUPBOX_MARGINS.cyTopHeight, szCnt,szCnt.GetLength()); //文本起始位置

				}
				it++;
				nWeight += 30;
			}
			dc.SetTextColor(oldColor); 
			dc.SelectObject(pOldFont); 

		}
		
	}

	if (m_GroupType==0){
		for(int index=0;index<m_ShowCount;index++){
			BTN_INFO& btnInfo = m_vecButtonInfo[index+m_ShowStartIndex];
			if(m_vecFixedButton[index]->IsVisible()!=btnInfo.isVisible){
				m_vecFixedButton[index]->SetVisible(btnInfo.isVisible);
			}
			if(m_vecFixedButton[index]->IsEnabled()!=btnInfo.isEnable){
				m_vecFixedButton[index]->SetEnabled(btnInfo.isEnable);
			}
		}
		if(m_ShowPrevious){
			int previousIndex = m_ShowCount;
			m_vecFixedButton[previousIndex]->SetEnabled(true);
			m_vecFixedButton[previousIndex]->SetVisible(true);
			m_vecFixedButton[previousIndex]->SetSelect(false);
		}
		if(m_ShowNext){
			int nextIndex = m_ShowCount + (m_ShowPrevious ? 1 : 0);
			m_vecFixedButton[nextIndex]->SetEnabled(true);
			m_vecFixedButton[nextIndex]->SetVisible(true);
			m_vecFixedButton[nextIndex]->SetSelect(false);
		}
		int unusedButtonIndex = m_ShowCount + (m_ShowPrevious ? 1 : 0)+(m_ShowNext ? 1 : 0);
		for(int i=unusedButtonIndex;i<m_Cols*m_Rows;i++){
			m_vecFixedButton[i]->SetVisible(false);
		}
		if(!IsWindowEnabled()){
			for (int index=0;index<m_vecFixedButton.size();index++){
				m_vecFixedButton[index]->SetEnabled(false);
			}
			return;
		}
	}
	else{
		for(int index=0;index<m_ShowCount;index++){

			if (index>1&&index<m_ShowCount-2){
				BTN_INFO& btnInfo = m_vecButtonInfo[index-1+m_ShowStartIndex];
				if(m_vecFixedButton[index]->IsVisible()!=btnInfo.isVisible){
					m_vecFixedButton[index]->SetVisible(btnInfo.isVisible);
				}
				if(m_vecFixedButton[index]->IsEnabled()!=btnInfo.isEnable){
					m_vecFixedButton[index]->SetEnabled(btnInfo.isEnable);
				}
				else if(m_vecFixedButton[index]->IsSelected()!=btnInfo.isSelected){
					m_vecFixedButton[index]->SetSelect(btnInfo.isSelected);
				}
			}
			else{
					m_vecFixedButton[index]->SetVisible(true);
					m_vecFixedButton[index]->SetEnabled(true);
					if (index==1){
						m_vecFixedButton[index]->SetEnabled(m_ShowNext);
					}
					else if(index==m_ShowCount-2){
						m_vecFixedButton[index]->SetEnabled(m_ShowPrevious);
					}
					else if(index==m_ShowCount-1){
						m_vecFixedButton[index]->SetSelect(false);
					}
			}

		}

		int unusedButtonIndex = m_ShowCount;
		for(int i=unusedButtonIndex;i<m_Cols*m_Rows;i++){
			m_vecFixedButton[i]->SetVisible(false);
		}
		if(!IsWindowEnabled()){
			for (int index=0;index<m_vecFixedButton.size();index++){
				m_vecFixedButton[index]->SetEnabled(false);
			}
			return;
		}
	}
	__super::OnPaint();
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CButtonGroup::SetButtonTitle(CString titleName)
{
	m_titleName = titleName;
	if(IsWindow(this->m_hWnd))
	{
		//this->Invalidate();
		if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
		{
			//this->UpdateWindow();
			if(m_ShowButtons){
				this->ShowWindow(true);
			}
		}
		//Invalidate();
		CPoint ptTopLeft(2*DEFAULT_GROUPBOX_MARGINS.cxLeftWidth, 2*DEFAULT_GROUPBOX_MARGINS.cyTopHeight);
		CPoint ptBottomRight(m_ActualSize.cx,m_ActualSize.cy);
		CRect rcClient(ptTopLeft,ptBottomRight);
		this->InvalidateRect(rcClient);
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
	this->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置按钮选中而不触发消息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CButtonGroup::SetSelection(CString msg){
	for (int i = 0; i < m_vecButtonInfo.size(); i++)
	{
		BTN_INFO& btnInfo = m_vecButtonInfo[i];
		if(btnInfo.btnMsg == msg)
		{
			if(!btnInfo.isSelected)
			{
				btnInfo.isSelected = true;
				m_vecFixedButton[i]->SetSelect(true);
			}
		}
	}
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
BOOL CButtonGroup::EnableButtonGroup(bool enabled)
{
	if (enabled){
		if (m_vecEnableButtonInfo.size()>0){
			m_vecButtonInfo = m_vecEnableButtonInfo;//设置置灰前的按钮信息
		}

	}
	else{
		m_vecEnableButtonInfo = m_vecButtonInfo;//保存置灰前的按钮信息
		for (int i=0;i<m_vecButtonInfo.size();i++){
			m_vecButtonInfo[i].isEnable = false;
		}
	}
	this->EnableWindow(enabled);
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
/*
@brief      通过消息点击按钮

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL CButtonGroup::ClickButton(CString msg)
{
	for (int i=0; i<m_vecButtonInfo.size(); i++)
	{
		BTN_INFO& btnInfo = m_vecButtonInfo[i];
		if(btnInfo.btnMsg == msg)
		{
			if(!btnInfo.isEnable || !btnInfo.isVisible)
			{
				return FALSE;
			}
			LRESULT result = m_vecFixedButton[i-m_ShowStartIndex]->OnXButtonClick(NULL,NULL);
			return result;
		}
	}

	throw CInnerException(CInnerException::MODULE_ID,CInnerException::FUNC_ERR_RETURN,_T(__FILE__),__LINE__);
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

vector<CXButton*>& CButtonGroup::getBtns()
{
	return m_vecFixedButton;
}

UIINFO& CButtonGroup::getUIInfo()
{
	return m_uIInfo;
}

vector<BTN_INFO>& CButtonGroup::getBtnsInfo()
{
	return m_vecButtonInfo;
}

vector<BTN_INFO>& CButtonGroup::getSelectBtnsInfo()
{
	return m_vecSelectedButtonInfo;
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
	//if (msg== _tl(TXT_ISSUE_HOME))
	//{
	//	m_ShowCount = 0;
	//	m_ShowStartIndex = 0;
	//	m_ShowPreCount =0;
	//	m_ShowNext = false;
	//	m_ShowPrevious = false;
	//	m_nNextIndex = 0;
	//	m_nPreviousIndex = 0;
	//	BindButton(true);

	//	BTN_INFO& btnInfo = FindBtnInfo(ID,msg);
	//	GetParentDlg()->PostMessage(WM_XGROUP,m_ID,(LPARAM)&btnInfo);
	//	return -1;//阻止冒泡，不传到上一层。		
	//}

	//if (msg== _tl(TXT_ALL_LINES)/*||_tl(TXT_ISSUE_HOME)*/)
	//{
	//	int index=m_ShowCount-1;
	//		BTN_INFO& btnInfo1 = m_vecButtonInfo[m_vecButtonInfo.size()-1];
	//		//m_vecFixedButton[index]->SetID(btnInfo.btnID);
	//		//m_vecFixedButton[index]->SetText(btnInfo.text);
	//		//m_vecFixedButton[index]->SetMsg(btnInfo.btnMsg);
	//		//m_vecFixedButton[index]->SetShortCutKey(btnInfo.shortCutKey);
	//		/*if(m_vecFixedButton[index]->IsSelected()!=btnInfo.isSelected)*/{
	//			m_vecFixedButton[index]->SetSelect(false);
	//		}

	//	BTN_INFO& btnInfo = FindBtnInfo(ID,msg);
	//	GetParentDlg()->PostMessage(WM_XGROUP,m_ID,(LPARAM)&btnInfo);
	//	return -1;//阻止冒泡，不传到上一层。
	//}
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
				if (m_GroupType==0){
					for(int i=0;i<m_vecButtonInfo.size();i++){
						BTN_INFO& btnInfo = m_vecButtonInfo[i];
						if(btnInfo.isSelected){
							btnInfo.isSelected = false;
							if(i>=m_ShowStartIndex && i<m_ShowStartIndex+m_ShowCount){
								m_vecFixedButton[i-m_ShowStartIndex]->SetSelect(false);
							}
						}
						if(btnInfo.btnID == ID && btnInfo.btnMsg == msg){
							btnInfo.isSelected = true;
							if(i>=m_ShowStartIndex && i<m_ShowStartIndex+m_ShowCount){
								m_vecFixedButton[i-m_ShowStartIndex]->SetSelect(true);
							}
						}
					}
				}
				else{
					if (msg==_T("0")){//地图还原按钮
							//m_ShowCount = 0;
							//m_ShowStartIndex = 0;
							//m_ShowPreCount =0;
							//m_ShowNext = false;
							//m_ShowPrevious = false;
							//m_nNextIndex = 0;
							//m_nPreviousIndex = 0;
							//BindButton(true);
							//this->Invalidate();

							BTN_INFO& btnInfo = FindBtnInfo(ID,msg);
							GetParentDlg()->PostMessage(WM_XGROUP,m_ID,(LPARAM)&btnInfo);
							return -1;//阻止冒泡，不传到上一层。
					}



					int selIndex = 0;
					for(int i=0;i<m_ShowCount;i++){
						if(m_vecFixedButton[i]->GetID() == ID &&m_vecFixedButton[i]->GetMsg() == msg){
							selIndex = i;
						}
					}
	
					if (selIndex==0){
						for (int i=0;i<m_vecButtonInfo.size();i++){
							BTN_INFO& btnInfo = m_vecButtonInfo[i];
							btnInfo.isSelected = false;
						}
						for(int i=0;i<m_ShowCount;i++){
							m_vecFixedButton[i]->SetSelect(false);
						}
						BTN_INFO& btnInfo = m_vecButtonInfo[0];
						btnInfo.isSelected = true;
						m_vecFixedButton[0]->SetSelect(true);
					}
					else if (selIndex==m_ShowCount-1){
						m_vecFixedButton[selIndex]->SetSelect(false);
					}
					else{
						for (int i=0;i<m_vecButtonInfo.size();i++){
							BTN_INFO& btnInfo = m_vecButtonInfo[i];
							btnInfo.isSelected = false;
						}
						for(int i=0;i<m_ShowCount;i++){
							m_vecFixedButton[i]->SetSelect(false);
						}
						BTN_INFO& btnInfo = m_vecButtonInfo[m_ShowStartIndex+selIndex-1];
						btnInfo.isSelected = true;
						m_vecFixedButton[selIndex]->SetSelect(true);	
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
	throw CInnerException(CInnerException::MODULE_ID,CInnerException::ERROR_BUTTON_NOT_FOUND,_T(__FILE__),__LINE__);
}