#include "stdafx.h"
#include "ExchangeReceptionDlg.h"
#include "ExceptionMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CExchangeReceptionDlg,CReceptionDlg)

BEGIN_MESSAGE_MAP(CExchangeReceptionDlg, CReceptionDlg)
	ON_WM_ACTIVATE()
    ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CExchangeReceptionDlg::CExchangeReceptionDlg(CService* pService)
	: CReceptionDlg(CExchangeReceptionDlg::IDD, pService)
{
   m_BusinessInfo->detailList.resize(LABEL_GROUP_EDIT_ROWS_5);     
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CExchangeReceptionDlg::~CExchangeReceptionDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeReceptionDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
 
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
int CExchangeReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
    try{
        if (-1 == __super::OnCreate(lpCreateStruct ,BUSINESS_AREA)){
            return -1;
        }
		
    }
    catch (CSysException e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(pExchangeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
	try{	
		// ����
		if (pExchangeSvc->GetCurDialogGroupID()==CExchangeSvc::DIALOG_GROUP_EXCHANGE_NOT) {
			//�����������	
			HideCompoment(BUSINESS_AREA);
		}
		// ����
		else if(pExchangeSvc->GetCurDialogGroupID()==CExchangeSvc::DIALOG_GROUP_FINISH)
		{
			m_BusinessInfo->detailList = pExchangeSvc->GetReceptionInfo();
			m_BusinessArea->Invalidate();
		}
		else
		{
			m_BusinessInfo->detailList = pExchangeSvc->GetReceptionInfo();
			m_BusinessArea->Invalidate();
		}
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pExchangeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}


}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ���

@param      (i)ulong ulCompomentFlag           �����־

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeReceptionDlg::ShowCompoment(ULONG ulCompomentFlag)
{
	CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
	m_BusinessInfo->detailList = pExchangeSvc->GetReceptionInfo();
	__super::ShowCompoment(ulCompomentFlag);
}
