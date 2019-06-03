#include "stdafx.h"
#include "ExchangeSvc.h"
#include "ExchangeBackupDlg.h"
#include "ExceptionMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//�ֽ���Ϣ��ʾ�༭���������
static CRect editCrash[] = {
	BASE_INFO_RECT(2,6),
};

IMPLEMENT_DYNAMIC(CExchangeBackupDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CExchangeBackupDlg, COperationDlg)
    ON_WM_CREATE()
    ON_WM_ACTIVATE()
	ON_MESSAGE(WM_XGROUP,XGroupClick)
    ON_MESSAGE(WM_XBUTTON,XButtonClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeBackupDlg::CExchangeBackupDlg(CService* pService)
: COperationDlg(CExchangeBackupDlg::IDD, pService)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
// ��ֵ��Ϣ��ʾ����
	m_baseInfo->titleInfo.titleName = TITLE_EXCHANGE_INFO;                    // ����
	m_baseInfo->detailInfo.row = pExchangeSvc->GetExchangeInfoSize();         // ����
	m_baseInfo->detailInfo.labelGroup = pExchangeSvc->GetExchangeInfo();      // ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = 0;                               // �༭����Ŀ

	// �ֽ���Ϣ��ʾ����
	//m_cashInfo->titleInfo.titleName = TITLE_CASH_INFO;                      // ����
	//m_cashInfo->detailInfo.row = pExchangeSvc->GetCrashInfoSize();             // ����
	//m_cashInfo->detailInfo.labelGroup = pExchangeSvc->GetCrashInfo();          // ��ǩ���֣�2�У�
	//m_cashInfo->detailInfo.editGroup.cnt = 1;                                // �༭����Ŀ
	//m_cashInfo->detailInfo.editGroup.editRect = editCrash;                   // �༭������ 

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeBackupDlg::~CExchangeBackupDlg()
{
   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC �豸������

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeBackupDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CExchangeBackupDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
    try{
        // ����������Ϣ���򡢷��ؼ���ȷ�ϼ�
        if(-1 == __super::OnCreate(lpCreateStruct, BASE_AREA | CASH_AREA | BTN_BACK | BTN_OK)){
            return -1;
        }
    }
    catch (CSysException e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        //theEXCEPTION_MGR.ProcessException(CInnerException(pExchangeSvc->GetServiceID(), CInnerException::OTHER_ERR, __FILE__, __LINE__));
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ť���º�Ĵ���

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CExchangeBackupDlg::XButtonClick(WPARAM wParam,LPARAM lParam)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
    CString sMsg = (LPCTSTR)lParam;
    try {
       
    }
    catch (CSysException e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
//        theEXCEPTION_MGR.ProcessException(CInnerException(pExchangeSvc->GetServiceID(), CInnerException::OTHER_ERR, __FILE__, __LINE__));
    }

    // �����л�
    return __super::XButtonClick(wParam,lParam);   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ּ��̻س�����ť���º�Ĵ���

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CExchangeBackupDlg::XGroupClick(WPARAM wParam,LPARAM lParam)
{
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeBackupDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���cash�༭��������Ƿ���ȷ

@param      none

@retval     bool		\n
TRUE:��ȷ  FALSE:����ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CExchangeBackupDlg::CheckInput()
{
	bool ret = false;
	
	return ret;
}

