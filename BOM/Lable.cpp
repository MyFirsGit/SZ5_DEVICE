#include "stdafx.h"
#include "Lable.h"

IMPLEMENT_DYNAMIC(CLable,CPanel)

BEGIN_MESSAGE_MAP(CLable,CPanel)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      Label���캯��

@param      (i)UIINFO& uIInfo ���ؼ���ʽ
@param      (i)CString text ��ʾ������

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLable::CLable(UIINFO& uIInfo,CString text)
:CPanel(uIInfo)
{
	this->m_OnPaintFunc = NULL;
	this->m_Text = text;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      CLable���캯��

@param      void

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLable::~CLable()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ��Label

@param      void

@retval     BOOL
						TRUE �ɹ�
						FALSE	ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CLable::Create()
{
	this->m_UIInfo.m_Size = GetTextSize();
	return __super::Create();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������������������������С

@param      void

@retval     CSize ��С

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSize CLable::GetTextSize()
{
	CPaintDC dc(m_UIInfo.m_pParentWnd);
	CFont cFont;
	cFont.CreateFontIndirect(&m_UIInfo.m_Font);
	CFont* pOldFont=dc.SelectObject(&cFont);
	CSize size = dc.GetTextExtent(m_Text);
	dc.SelectObject(pOldFont);
	size.cx += (m_UIInfo.m_Margins.cxLeftWidth+m_UIInfo.m_Margins.cxRightWidth+m_UIInfo.m_BorderWidth*2+m_UIInfo.m_Paddings.cxLeftWidth+m_UIInfo.m_Paddings.cxRightWidth);
	size.cy	+= (m_UIInfo.m_Margins.cyTopHeight+m_UIInfo.m_Margins.cyBottomHeight+m_UIInfo.m_BorderWidth*2+m_UIInfo.m_Paddings.cyTopHeight+m_UIInfo.m_Paddings.cyBottomHeight);
	return size;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ƺ���

@param      void

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLable::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC = CDC::FromHandle(dc);

	CFont cFont;
	cFont.CreateFontIndirect(&m_UIInfo.m_Font);

	pDC->SetTextColor(m_UIInfo.m_ForeColor);
	CFont* pOldFont = pDC->SelectObject(&cFont);
	pDC->SetBkMode(TRANSPARENT);

	if(m_OnPaintFunc)
	{
		m_OnPaintFunc->Invoke(pDC,this);
	}
	else
	{
		CRect contentRect = GetContentRect();
		CSize textSize = dc.GetTextExtent(m_Text);

		CPoint pointStartText = CPoint(contentRect.left,contentRect.top);
		switch(m_UIInfo.m_DockStyle)
		{
		case LEFT:
			{
				break;
			}
		case TOP:
			{
				break;
			}
		case RIGHT:
			{
				pointStartText = CPoint(contentRect.right- textSize.cx,contentRect.top);
				break;
			}
		case BOTTOM:
			{
				pointStartText = CPoint(contentRect.left,contentRect.bottom - textSize.cy);
				break;
			}
		case FILL:
			{
				break;
			}
		default:
			{
				break;
			}
		}

		pDC->TextOut(pointStartText.x,pointStartText.y,m_Text);
	}

	pDC->SelectObject(pOldFont);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ������

@param      void

@retval     CString

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CLable::GetText()
{
	return m_Text;
}		

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     CString

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLable::SetText(CString text)
{
	m_Text = text;
	UIINFO uiInfo = m_UIInfo;
	if(IsWindow(m_hWnd))
	{
		uiInfo.m_Size = GetTextSize();
	}
	SetUIInfo(uiInfo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ÿؼ���ʽ�����ظ��ຯ��

@param      UIINFO& uIInfo �ؼ���ʽ

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLable::SetUIInfo(UIINFO& uIInfo)
{
	if(IsWindow(m_hWnd))
	{
		uIInfo.m_Size = GetTextSize();
	}
	__super::SetUIInfo(uIInfo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���û��ƺ���ʱ�Ļص��������ṩһ���ӿڸ����������л���

@param      Func<void (CDC*,CLable*)> * pPaintFunc �ص�����ָ��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLable::SetPaintFunc(Func<void (CDC*,CLable*)> * pPaintFunc)
{
	m_OnPaintFunc = pPaintFunc;
}