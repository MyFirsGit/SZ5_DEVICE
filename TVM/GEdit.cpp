#include "stdafx.h"
#include "GEdit.h"
#include "MessageIDDefinition.h"
#include "controlmanager.h"


BEGIN_MESSAGE_MAP(CGEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

delegate<void(CGEdit*,CString)> CGEdit::EditTextChanged;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGEdit::CGEdit(CWnd* pParentWnd, const RECT& rect, bool isLeftAlign)
:CEdit()
,m_scope(DIGIT_CHAR)
{
	m_pBag = new CBag();
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER |WS_TABSTOP|ES_AUTOHSCROLL;
	if (! isLeftAlign) {
		dwStyle |= ES_RIGHT;
	}
	m_UIInfo.m_BackColor = WHITE;
	CEdit::Create(dwStyle, rect, pParentWnd, 0);
}

CGEdit::CGEdit(UIINFO& uiInfo):CEdit(),m_scope(DIGIT_CHAR)
{
	m_pBag = new CBag();
	m_UIInfo = uiInfo;
	m_UIInfo.m_BackColor = WHITE;
}

UIINFO CGEdit::GetUIInfo()
{
	return m_UIInfo;
}

void CGEdit::SetUIInfo(UIINFO& uiInfo)
{
	m_UIInfo = uiInfo;
}

CBag* CGEdit::GetBag()
{
	return m_pBag;
}

BOOL CGEdit::Create()
{
	DWORD dwStyle = WS_CHILD | WS_VISIBLE  | ES_UPPERCASE|WS_TABSTOP |ES_AUTOHSCROLL;
	if(m_UIInfo.m_BorderWidth>0)
	{
		dwStyle |= WS_BORDER;
	}
	return CEdit::Create(dwStyle, CRect(m_UIInfo.m_Location,m_UIInfo.m_Size), m_UIInfo.m_pParentWnd, 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGEdit::~CGEdit()
{
	theControl_MGR.UnRegisterControl(this);
	delete m_pBag;
	m_pBag = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
HBRUSH CGEdit::CtlColor(CDC* pDC,  UINT nCtlColor)
{
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(10,10,10));
	}
	CBrush brush(RGB(0,0,0));
	return (HBRUSH)brush;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到焦点响应函数

@param      (i)UINT nFlags
@param      (i)CPoint point

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGEdit::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);
	this->SetSel(-1);
	GetParent()->GetParent()->SendMessage(WM_EDIT_SETFOCUSE, (WPARAM)this);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      失去焦点响应函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CGEdit::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);
	if(m_scope!=CASH)
	{
		return;
	}
	CString strText;
	this->GetWindowText(strText);
	if(strText == _T(""))
	{
		return;
	}
	int dotPos = strText.Find(_T("."));
	if(dotPos==-1)//没有小数点
	{
		strText=strText+_T(".00");
	}
	else
	{
		int decimalLength = strText.GetLength() - (dotPos+1);
		for(int i=2;i>decimalLength;i--)
		{
			strText = strText+_T("0");
		}
	}
	this->SetWindowText(strText);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      字符输入响应函数

@param      (i)UINT nChar
@param      (i)UINT nRepCnt
@param      (i)UINT nFlags

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_CLEAR)
	{
		this->SetWindowText(_T(""));
		GetParent()->SendMessage(WM_EDIT_CHANGE,nChar,(LPARAM)this);
		EditTextChanged.Invoke(this,_T(""));
	}
	else if (IsValidChar(nChar)) {
		__super::OnChar(nChar, nRepCnt, nFlags);
		GetParent()->SendMessage(WM_EDIT_CHANGE,nChar,(LPARAM)this);
		CString text;
		GetWindowText(text);
		EditTextChanged.Invoke(this,text);
	}
	else {
		Beep(200, 100);
	}

}

int  CGEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(-1== __super::OnCreate(lpCreateStruct))
	{
		return -1;
	}
	theControl_MGR.RegisterControl(this);
	return 0;
}
void	CGEdit::OnDestroy()
{
	theControl_MGR.UnRegisterControl(this);
	__super::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取编辑区所属的Dialog

@param      无

@retval     CBaseDlg*   按钮所属的Dialog

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBaseDlg* CGEdit::GetParentDlg()
{
	CWnd* parent = NULL;

	if(!IsWindow(m_hWnd))
	{
		return NULL;
	}
	parent = GetParent();
	while(parent!=NULL && !parent->IsKindOf(RUNTIME_CLASS(CBaseDlg)))
	{
		parent = parent->GetParent();
	}
	return (CBaseDlg*)parent;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置允许输入的范围

@param      (i)CHAR_SCOPE scope

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGEdit::SetScope(CHAR_SCOPE scope)
{
	m_scope = scope;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断输入的符号是否是合法字母（a-f, A-F, x, X）

@param      (i)UINT nChar   输入字母的符号

@retval     bool enable  true:是合法字母; false:不是合法字母

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CGEdit::IsValidChar(UINT nChar)
{
	if (nChar == VK_BACK || nChar == VK_CLEAR) {
		return true;
	}

	bool result = false;

	switch (m_scope) {
	case DIGIT_CHAR:     // 10进制字符
		if (_istdigit(nChar)) {
			return true;
		}
		break;
	case DIGIT_DOT_CHAR:
		if (_istdigit(nChar)||nChar == '.') {
			return true;
		}
		break;

	case HEX_CHAR:       // 16进制字符
		if (_istdigit(nChar)
			|| (nChar >= 'a' && nChar <= 'f')
			|| (nChar >= 'A' && nChar <= 'F')) {

				return true;
		}
		break;
	case HEX_DOT_CHAR:       // 16进制字符
		if (_istdigit(nChar)|| nChar=='.'
			|| (nChar >= 'a' && nChar <= 'f')
			|| (nChar >= 'A' && nChar <= 'F')) {

				return true;
		}
		break;
	case CASH:
		{
			CString strText;
			this->GetWindowText(strText);
			if(strText.GetLength()==0&&nChar == '.')
			{
				return false;
			}
			if(strText == _T("0") && nChar!='.')
			{
				return false;
			}

			int dotPos = strText.Find(_T("."));
			if(dotPos==-1)//没有小数点
			{
				if(_istdigit(nChar) && (GetLimitText() -3)<=strText.GetLength())
				{
					return false;
				}
				if (_istdigit(nChar)||nChar == '.') {
					return true;
				}
			}
			else
			{
				int decimalLength = strText.GetLength() - (dotPos+1);
				if(decimalLength < 2)
				{
					if(_istdigit(nChar))
					{
						return true;
					}
				}
			}
		}
		break;
	case NAME:
		{
			if (!_istdigit(nChar)&&!_istpunct(nChar)&&!_istcntrl(nChar)) {
					return true;
			}			
			return false;
		}
		break;
	case NUMANDCHAR:
		{
			if (_istalnum(nChar)) {
				return true;
			}			
			return false;
		}
		break;
	case ALL_CHAR:       // 所有字符
		return true;
	}

	return result;
}

BOOL CGEdit::IsEnabled()
{
	return IsWindowEnabled();
}
void CGEdit::SetEnabled(BOOL enabled)
{
	EnableWindow(enabled);
	Invalidate();
}
