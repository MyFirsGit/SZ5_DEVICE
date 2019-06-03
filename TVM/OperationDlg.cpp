#include "StdAfx.h"
#include "OperationDlg.h"
#include "ForeService.h"
#include "bitmapmanager.h"
#include "InnerException.h"
#include "ExceptionMgr.h"
#include "AppSession.h"
#include "GuideDlg.h"
#include "MessageIDDefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COperationDlg, CBaseDlg)
BEGIN_MESSAGE_MAP(COperationDlg, CBaseDlg)
	ON_WM_ACTIVATE()
    ON_MESSAGE(WM_EDIT_SETFOCUSE, OnEditSetFocus)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)UINT ID             �Ի���ID
@param      (i)CService* pService  �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COperationDlg::COperationDlg(UINT ID, CService* pService, CRect rect)
	: CBaseDlg(ID, theAPP_SESSION.GetChildView(), rect, GRAY, pService)
{
    Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)UINT ID              �Ի���ID
@param      (i)CRect position       λ�ô�С
@param      (i)COLORREF bkColor     ����ɫ
@param      (i)CService* pService   �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COperationDlg::COperationDlg(UINT ID, CRect rect, COLORREF bkColor, CService* pService)
	: CBaseDlg(ID, theAPP_SESSION.GetChildView(), rect, bkColor, pService)
{
    Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ڻ�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::Initialize()
{
	m_baseInfoArea = NULL;

	m_baseInfo = new GSTATIC_INFO;
	m_baseInfo->point = LEFTUP_AREA_POINT;						// ������Ϣ�������Ͻ�����
	m_baseInfo->titleInfo.titleName = _T("");					// �������ƣ�TVM��ʹ�ã�
	m_baseInfo->titleInfo.color = BLACK;						// ������ɫ��TVM��ʹ�ã�
	m_baseInfo->detailInfo.color = BLACK;						// ������ɫ
	m_baseInfo->detailInfo.row = 15;							// ��������
	m_baseInfo->detailInfo.editGroup.cnt = 0;                   // �༭�����
	m_baseInfo->detailInfo.editGroup.isLeftAlign = true;		// �Ƿ������
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COperationDlg::~COperationDlg(void)
{
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;

	delete m_baseInfo;
	m_baseInfo = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct ���ڳ�ʼ������
@param      (i)ulong ulCompomentFlag ���������־

@retval     int  0:�����ɹ�  -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int COperationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulCompomentFlag)
{
    try {
		theGuideDlg.ShowWindow(ulCompomentFlag & GUIDE_AREA ? SW_SHOW : SW_HIDE);
	    if (__super::OnCreate(lpCreateStruct) == -1) {
		    return -1;
	    }
		if(ulCompomentFlag & BASE_AREA) {
			m_baseInfoArea = new CGStatic(this, m_baseInfo);
		}

        // �༭���б����������������б༭��
        if (m_baseInfoArea != NULL && m_baseInfo->detailInfo.editGroup.cnt > 0) {  
            for (int i = 0; i < m_baseInfo->detailInfo.editGroup.cnt; i++) {
                m_editArray.push_back(m_baseInfoArea->GetEdit(i));
			}
        }
		if (m_editArray.size() > 0) {
			SetEditFocus(0);	// �������뽹�㵽�༭��
		}
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	
	SetEditFocus(m_curEditIndex);	

	if(NULL!=m_baseInfoArea)
	{
		if(IsWindow(m_baseInfoArea->m_hWnd))
		{
			m_baseInfoArea->SetStyle();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��������

@param      ��

@retval     PGSTATIC_INFO

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
PGSTATIC_INFO COperationDlg::GetBaseInfo()
{
	return m_baseInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ������������

@param      ��

@retval     CGStatic*

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGStatic* COperationDlg::GetBaseArea()
{
	return m_baseInfoArea;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������

@param      (i)CDC* pDC �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::OnDraw(CDC* pDC)
{
    SetEditFocus(m_curEditIndex);	// �������뽹�㵽�༭��

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �༭�����ݸı����Ϣ��Ӧ����

@param      (i)WPARAM wParam  ��Ϣ��Ϣ������༭��
@param      (i)LPARAM lParam  ��������

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT COperationDlg::OnEditSetFocus(WPARAM wParam, LPARAM lParam)
{
    CGEdit* pEdit = (CGEdit*)wParam;
    for (UINT i = 0; i < m_editArray.size(); i++) {
        if (pEdit == m_editArray[i]) {
			m_curEditIndex = i;
            break;
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
void COperationDlg::OnServiceBusy(bool busy)
{
	// ��ά��æµʱֹͣ�Զ��ǳ���ʱ��(����æµʱ���Զ��ǳ�ʱ�����0�ſ�����������ʱ��)
	DWORD dwOutOfTime = theAFC_TVM_OPERATION.GetAutoLogoffTime();
	if (dwOutOfTime > 0)
	{
		busy ?theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StopTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF):theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF,dwOutOfTime);
	}
	else
	{
		if (busy)
		{
			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StopTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF);
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ���

@param      (i)ulong ulCompomentFlag �����־

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::ShowCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������

@param      (i)ulong ulCompomentFlag �����־

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::HideCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ/�������

@param      (i)ulong ulCompomentFlag �����־
@param      (i)int nCmdShow ����(SW_SHOW/SW_HIDE)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow)
{
    // ������������
    if (ulCompomentFlag & BASE_AREA)
	{
        ShowHideCompoment(m_baseInfoArea, nCmdShow);
    }

    // �༭��
    if (ulCompomentFlag & EDIT_BOX)
	{
        for (UINT i = 0; i < m_editArray.size(); i++)
		{
            ShowHideCompoment(m_editArray[i], nCmdShow);
        }
        SetEditFocus(0);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ/�������

@param      (i)CWnd *pCompoment �������
@param      (i)int nCmdShow ����(SW_SHOW/SW_HIDE)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::ShowHideCompoment(CWnd* pCompoment, int nCmdShow)
{
    if (pCompoment == NULL) 
	{
		return;
	}

	CRect rect;
    pCompoment->ShowWindow(nCmdShow);
    GetCompomentRect(pCompoment, &rect);
    InvalidateRect(&rect, TRUE);
    UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������ڵ�����

@param      (i)CWnd *pCompoment �������
@param      (o)CRect* lpRect ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::GetCompomentRect(CWnd *pCompoment, CRect* lpRect)
{
    pCompoment->GetWindowRect(lpRect);
    ScreenToClient(lpRect);									// ת��������ڵ�ǰOperationDlg��λ��
    lpRect->InflateRect(BORDER_MAX_SIZE, BORDER_MAX_SIZE);	// ���ϱ߿�Ĵ�С
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ñ༭�򽹵�

@param      (i)int index �༭����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::SetEditFocus(int index)
{
    if (index >= 0 && index < m_editArray.size())
	{
        m_editArray[index]->SetFocus();
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ý���ı༭����

@param      ��

@retval     int ��ý���ı༭����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int COperationDlg::GetEditFocusID()
{
	return m_curEditIndex;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ñ༭���ı�

@param      (i)int index �༭����
@param      (i)CString text �༭������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::SetEditText(int index, CString text)
{
	ASSERT(index >= 0 && index < m_editArray.size());
    m_editArray[index]->SetWindowText(text);
    SetEditFocus(m_curEditIndex);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�༭���ı�

@param      (i)int index �༭����

@retval     CString �༭���ı�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString COperationDlg::GetEditText(int index)
{
	ASSERT(index >= 0 && index < m_editArray.size());
    CString text;
    m_editArray[index]->GetWindowText(text);
    return text;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�༭�����

@param      (i)int index �༭����

@retval     CGEdit* �༭�����, index��Чʱ����NULL

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGEdit* COperationDlg::GetEdit(int index)
{
	ASSERT(index >= 0 && index < m_editArray.size());
	return m_editArray[index];
}
