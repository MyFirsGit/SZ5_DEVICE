#include "stdafx.h"
#include "ExchangeBaseOfficeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//�ֽ���Ϣ��ʾ�༭���������
static CRect editCrash[] = {
	BASE_INFO_RECT(2,6),
};


IMPLEMENT_DYNAMIC(CExchangeBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CExchangeBaseOfficeDlg, COperationDlg)
    ON_WM_CREATE()
    ON_WM_ACTIVATE()
    ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeBaseOfficeDlg::CExchangeBaseOfficeDlg(CService* pService)
	: COperationDlg(CExchangeBaseOfficeDlg::IDD, pService)
{
	CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();

	// ��ֵ��Ϣ��ʾ����
	m_baseInfo->titleInfo.titleName = TITLE_CHARGE_INFO;                    // ����
	m_baseInfo->detailInfo.row = pExchangeSvc->GetExchangeInfoSize();         // ����
	m_baseInfo->detailInfo.labelGroup = pExchangeSvc->GetExchangeInfo();      // ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = 0;                               // �༭����Ŀ 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeBaseOfficeDlg::~CExchangeBaseOfficeDlg()
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
void CExchangeBaseOfficeDlg::OnDraw(CDC* pDC)
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
int CExchangeBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
    try{
        // ����������Ϣ����ȷ�ϼ�
        if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA| BTN_OK |  BASE_AREA )){
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
@brief      ��ť���º�Ĵ���

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CExchangeBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CExchangeSvc* pExchange = (CExchangeSvc*)GetService();
	pExchange->DoOldCardExchange();   
    return __super::XButtonClick(wParam,lParam);
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
void CExchangeBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CExchangeSvc* pExchange = (CExchangeSvc*)GetService();
	try{	
		// ����������
		if (pExchange->GetCurDialogGroupID()==CExchangeSvc::DIALOG_GROUP_EXCHANGE_NOT) {
			//�����������	
			m_vec_Right_2_BTN_INFO.clear();
			HideCompoment( BTN_OK|BASE_AREA|KEYBOARD_AREA );		
		}
		// ����
		else if(pExchange->GetCurDialogGroupID()==CExchangeSvc::DIALOG_GROUP_FINISH)
		{
			m_vec_Right_2_BTN_INFO.clear();
			HideCompoment( BTN_OK |KEYBOARD_AREA );
			m_baseInfoArea->Invalidate();
		}	
		theSERVICE_MGR.SetState(FINISH);
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pExchange->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	
}


