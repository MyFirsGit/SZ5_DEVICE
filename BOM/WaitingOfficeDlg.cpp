#include "stdafx.h"
#include "BOM.h"
#include "WaitingOfficeDlg.h"
#include "bomforeservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CWaitingOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CWaitingOfficeDlg, COperationDlg)
    ON_WM_ACTIVATE()
    ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*   pService  ��ǰ��Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWaitingOfficeDlg::CWaitingOfficeDlg(CService* pService, CString strServiceName)
	: COperationDlg(CWaitingOfficeDlg::IDD, pService)
{
    m_strServiceName = strServiceName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWaitingOfficeDlg::~CWaitingOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
             0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CWaitingOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    return __super::OnCreate(lpCreateStruct, GUIDE_AREA);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CWaitingOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CBOMForeService* pService = (CBOMForeService*)GetService();

    try {
		theSERVICE_MGR.SetState(ISSUEING);
        // ������������
		if(m_strServiceName == "TRUE"){
			pService->GetRWHelper()->ReadCard(true);
		}
		else{
			pService->GetRWHelper()->ReadCard();
		}

        // ��ʾ�ȴ�������Ϣ��
		INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pService->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);

        if (IDCANCEL == ret) 
		{
            pService->GetRWHelper()->StopReadCard();    
        }
    }
    catch (CSysException& e) {
		theSERVICE_MGR.SetState(RECEPT);
       theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
		theSERVICE_MGR.SetState(RECEPT);
        theEXCEPTION_MGR.ProcessException(CInnerException(pService->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CWaitingOfficeDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);

    if ("" != m_strServiceName && m_strServiceName != _T("TRUE")) {
        DrawRoundRect(pDC, SUBMENU_BTN4_RECT, LINE_BORDER, BLACKGRAY, BLACKGRAY);
        DrawText(pDC, SUBMENU_BTN4_POINT, m_strServiceName, _T(""));
    }
}
