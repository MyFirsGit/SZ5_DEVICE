#include "stdafx.h"
#include "RefreshBaseOfficeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//��Ϣ��ʾ�༭���������
static CRect editCrash[] = {
	BASE_INFO_RECT(2,6),
};


IMPLEMENT_DYNAMIC(CRefreshBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CRefreshBaseOfficeDlg, COperationDlg)
	ON_WM_CREATE()
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
CRefreshBaseOfficeDlg::CRefreshBaseOfficeDlg(CService* pService)
	: COperationDlg(CRefreshBaseOfficeDlg::IDD,pService)
{
	// ��Ʊ����
	m_RefreshInfo[0].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ������� 	
	m_RefreshInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_RefreshInfo[1].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	// ��Ч��
	m_RefreshInfo[2].nameLeft = add_wsp(CString(TXT_TICKET_VALIDATE_DATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);

	// ������Ϣ��ʾ����
	m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_4;           // ����
	m_baseInfo->detailInfo.labelGroup = m_RefreshInfo;        // ��ǩ���֣�2�У�
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
CRefreshBaseOfficeDlg::~CRefreshBaseOfficeDlg()
{

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
int CRefreshBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CRefreshSvc* pRefreshSvc = (CRefreshSvc*)GetService();
    try{
        // ����������Ϣ���򡢷��ء�ȷ����ť
        if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK | BASE_AREA )){
            return -1;
        }
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(pRefreshSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������á����ذ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
            TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefreshBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CRefreshSvc* pRefreshSvc = (CRefreshSvc*)GetService();
	return pRefreshSvc->DoRefresh();
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ���»���

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CRefreshBaseOfficeDlg::UpdateUI()
{
	CRefreshSvc* pSvc = (CRefreshSvc*)GetService();
	try{	
		// ���ɼ���
		if (pSvc->GetCurDialogGroupID()==CRefreshSvc::DIALOG_GROUP_REFRESH_NOT) {
			HideCompoment(BTN_OK | BASE_AREA);	
			return;
		}
		CRefreshSvc::DataModel& model = pSvc->GetDataModel();
		m_RefreshInfo[0].nameRight =add_hsp( model.ticketTypeDetail.ticketTypeName_cn,MAX_CASH_RIGHT_LEN);
		if(model.cardCategory == PURSE)
		{
			m_RefreshInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_RefreshInfo[1].nameRight = add_hsp(ComFormatAmount(model.balance) + MONEY_UNIT,MAX_CASH_RIGHT_LEN);
		}
		else if(model.cardCategory == MULTIRIDE)
		{
			m_RefreshInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_RIDES) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_RefreshInfo[1].nameRight = add_hsp(ComFormatCount(model.balance) + TIMES_UNIT,MAX_CASH_RIGHT_LEN);
		}
		else if(model.cardCategory == PERIOD)
		{
			m_RefreshInfo[1].nameLeft = TXT_UNDEFINE;
			m_RefreshInfo[1].nameRight = TXT_UNDEFINE;
		}
		m_RefreshInfo[2].nameRight = add_wsp(model.orignalValidate.ToString(_T("%4d��%2d��%2d�� %2dʱ%2d��%2d��")),MAX_SBC_CASE_LEN);
		if(pSvc->GetCurDialogGroupID()==CRefreshSvc::DIALOG_GROUP_FINISH)
		{
			m_RefreshInfo[3].nameLeft = add_wsp(CString(TXT_NEW_VALIDATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_RefreshInfo[3].nameRight = add_wsp(model.newValidate.ToString(_T("%4d��%2d��%2d�� %2dʱ%2d��%2d��")),MAX_SBC_CASE_LEN);
			HideCompoment( BTN_OK  );
		}
		m_baseInfoArea->Invalidate();
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
