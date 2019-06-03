#include "stdafx.h"
#include "RefundQueryOfficeDlg.h"
#include "GuideInfo.h"
#include "RefundApplySvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//�˿��ѯ��Ϣ��ʾ�༭���������
static CRect editQuery[] = {
	BASE_INFO_RECT(1,10),
	BASE_INFO_RECT(3,20),
};

IMPLEMENT_DYNAMIC(CRefundQueryOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CRefundQueryOfficeDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnCertificateTypeBtnClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundQueryOfficeDlg::CRefundQueryOfficeDlg(CService* pService) 
	: COperationDlg(CRefundQueryOfficeDlg::IDD,pService)
{
	InitQueryInfo();
	// ���˲�ѯ��Ϣ��ʾ����
	m_baseInfo->titleInfo.titleName = TITLE_REFUND_INFO;                    // ����
	m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_3;					// ����
	m_baseInfo->detailInfo.labelGroup = m_QueryInfo;    // ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = 2;                               // �༭����Ŀ
	m_baseInfo->detailInfo.editGroup.editRect = editQuery;                  // �༭������ 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundQueryOfficeDlg::~CRefundQueryOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
int CRefundQueryOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK | BASE_AREA)){
			return -1;
		}
		// ���ñ༭������
		GetEdit(0)->SetLimitText(9);
		GetEdit(0)->SetScope(CGEdit::DIGIT_CHAR);
		GetEdit(1)->SetLimitText(20);
		GetEdit(1)->SetScope(CGEdit::NUMANDCHAR);
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

void CRefundQueryOfficeDlg::InitQueryInfo()
{
	// ��Ʊ���
	m_QueryInfo[0].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ֤������
	m_QueryInfo[1].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_TYPE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ֤�����
	m_QueryInfo[2].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_NUM) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ok��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundQueryOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try {
		// �Ϸ��Լ��
		if (!CheckInputStatus()){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_INPUT_ERROR_ECT));
			return FALSE;
		}
		//���˲�ѯ��Ϣȷ��
		CString msg(_T("�������˲�ѯ��Ϣ���Ƿ�ȷ�ϣ�"));
		INT_PTR confirmResult = CMessageBox::Show(TITLE_REFUND_INFO,msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		if(confirmResult == IDCANCEL){
			return FALSE;
		}
		else if (confirmResult == IDOK){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUNDING_QUERY));
			theSERVICE_MGR.SetState(ISSUEING);
			CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
			dataModel.strTicketNum = GetEditText(0);
			dataModel.strCertificateNumber = GetEditText(1);
			// ��ѯ����״̬
			BYTE status = pRefundApplySvc->QueryRefundApplyResult();
			if(status == 0xFF){
				pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_QUERY_FINISH_DLG);
				// ��ʾ������Ϣ
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_QUERY_ERROR));
			}
			// �����ת����������
			else if(status == 0)
			{
				pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_QUERY_FINISH_DLG);
			}
			// ���ͨ��ת���˿����
			else if(status == 1){
				pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_BASE_APPLY_DLG);
			}
		}
	}
	catch (CSysException& e){
		pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_QUERY_FINISH_DLG);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theSERVICE_MGR.SetState(FINISH);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...){
		CInnerException e = CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		pRefundApplySvc->DoDialogFlow(WM_TO_REFUND_QUERY_FINISH_DLG);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theSERVICE_MGR.SetState(FINISH);
		theEXCEPTION_MGR.ProcessException(e);
	}
	return __super::XButtonClick(wParam,lParam);
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
void CRefundQueryOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_QUERY)
		{
			// ��ʼ��֤�����Ͱ�ť
			CString strCertificateMsg[] = {TXT_CERTIFICATE_ID_CARD_MAINLAND,TXT_CERTIFICATE_STUDENT,TXT_CERTIFICATE_MILITARY,TXT_CERTIFICATE_FOREIGNER};
			int strCertificateNo[] = {CERTIFICATE_ID_CARD_MAINLAND,CERTIFICATE_STUDENT_CARD,CERTIFICATE_MILITARY_OFFICER,CERTIFICATE_POST_CARD};
			for (int i = 0;i < 4;i++){
				BTN_INFO pCertificateBtnInfo;
				pCertificateBtnInfo.btnID = strCertificateNo[i];
				pCertificateBtnInfo.text = strCertificateMsg[i];
				if(pCertificateBtnInfo.text.GetLength()>16)
				{
					pCertificateBtnInfo.text.Insert(12,'\n');
				}
				m_vec_Left_2_BTN_INFO.push_back(pCertificateBtnInfo);
			}
			m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
			m_Left_2_Group->SetSelectType(CButtonGroup::SIGLE);
			m_Left_2_Group->ClickFirstAvailButton();
		}
		else if(pRefundApplySvc->GetCurDialogGroupID() == CRefundApplySvc::DIALOG_GROUP_QUERY_FINISH)
		{
			HideCompoment(BTN_OK|BASE_AREA|KEYBOARD_AREA);
			m_Left_2_Group->ShowButtons(false);
		}
		theSERVICE_MGR.SetState(FINISH);
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֤�����Ͱ�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundQueryOfficeDlg::OnCertificateTypeBtnClick(WPARAM wParam,LPARAM lParam)
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		dataModel.certificateType = (CERTIFICATE_TYPE)btnInfo->btnID;
		pRefundApplySvc->NotifyDataModelChanged();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

void CRefundQueryOfficeDlg::UpdateUI()
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	try{
		CRefundApplySvc::Model& dataModel = pRefundApplySvc->GetDataModel();
		switch(dataModel.certificateType)
		{
		case CERTIFICATE_TYPE::CERTIFICATE_ID_CARD:
			m_QueryInfo[1].nameRight = TXT_CERTIFICATE_ID_CARD_MAINLAND;
			break;
		case CERTIFICATE_TYPE::CERTIFICATE_STUDENT_CARD:
			m_QueryInfo[1].nameRight = TXT_CERTIFICATE_STUDENT;
			break;
		case CERTIFICATE_TYPE::CERTIFICATE_MILITARY_OFFICER:
			m_QueryInfo[1].nameRight = TXT_CERTIFICATE_MILITARY;
			break;
		case CERTIFICATE_TYPE::CERTIFICATE_POST_CARD:
			m_QueryInfo[1].nameRight = TXT_CERTIFICATE_FOREIGNER;
			break;
		default:
			m_QueryInfo[1].nameRight = TXT_UNKNOW_STATUS;
			break;
		}
		m_baseInfoArea->Invalidate();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���������Ϣ

@param      ��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CRefundQueryOfficeDlg::CheckInputStatus()
{
	CRefundApplySvc* pRefundApplySvc = (CRefundApplySvc*)GetService();
	CRefundApplySvc::Model & dataModel = pRefundApplySvc->GetDataModel();

	if(GetEditText(0).IsEmpty() && GetEditText(1).IsEmpty())
	{
		SetEditFocus(0);
		return false;
	}
	if(!GetEditText(1).IsEmpty())
	{
		if (!CheckIDCard(GetEditText(1)))
		{
			SetEditFocus(1);
			return false;
		}
	}
	return true;
}
