#include "stdafx.h"
#include "TVMFailureRefundBaseOfficeDlg.h"
#include "TVMFailureRefundSvc.h"
#include "CTTVMRefund.h"
#include "guideinfo.h"
#include "TranscationTranslateSvc.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ����˿������һ��ͨ����ֵ������λ5.80����Ϊ2���ֽڼ���õ�(Ԫ)
static const int MAX_REFUND_MONEY  = 655;

// �༭��λ��
static CRect editFailure[] = 
{
	BASE_INFO_RECT(1,8),
	BASE_INFO_RECT(2,10),
	BASE_INFO_RECT(3,7),
};

IMPLEMENT_DYNAMIC(CTVMFailureRefundBaseOfficeDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CTVMFailureRefundBaseOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     void 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMFailureRefundBaseOfficeDlg::CTVMFailureRefundBaseOfficeDlg(CService* pService) :
COperationDlg(CTVMFailureRefundBaseOfficeDlg::IDD, pService)
{
	CTVMFailureRefundSvc* pTVMFailureRefundSvc = (CTVMFailureRefundSvc*)GetService();
	m_baseInfo->titleInfo.titleName = TITLE_FAILURE_TITLENAME;									// ����
	m_baseInfo->detailInfo.row = FAILURE_INFO_SIZE;												// ����
	m_baseInfo->detailInfo.labelGroup = pTVMFailureRefundSvc->GetFailureInfo();					// ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = EDIT_COUNT;											// �༭�����
	m_baseInfo->detailInfo.editGroup.editRect = editFailure;									// �༭��λ��

	m_strTransNum = _T("");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMFailureRefundBaseOfficeDlg::~CTVMFailureRefundBaseOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int \n
0:�ɹ�  -1:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMFailureRefundBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ����������Ϣ���򣬷��ؼ���ȷ�ϼ�
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA|BASE_AREA /*|  BTN_RESET*/ | BTN_OK)){
		return -1;
	}
	
	// �豸������8λ
	GetEdit(0)->SetLimitText(8);
	GetEdit(0)->SetScope(CGEdit::HEX_CHAR);

	// �˿�ƾ�������10λ
	GetEdit(1)->SetLimitText(10);

	// �˿������3λ
	GetEdit(2)->SetLimitText(6);
	GetEdit(2)->SetScope(CGEdit::CASH);
	// ���ñ༭�򽹵�
	SetEditFocus(0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CTVMFailureRefundBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CString sMsg = (LPCTSTR)lParam;

	CTVMFailureRefundSvc* pTVMFailureRefundSvc = (CTVMFailureRefundSvc*)GetService();

	try{
			if (CheckValid()){
				// ��Ǯ��
				pTVMFailureRefundSvc->GetPRTHelper()->PrinterOpenBox();

				int i=1,j=0;
				CString strDeviceNo = _T("");
				CString strNo = _T("");
				CString strLine=_T("");

				// �����豸���
				strDeviceNo = GetEditText(0);
				pTVMFailureRefundSvc->SetFailureInfo(i++,strDeviceNo);
			
				// �����˿�ƾ����
				strNo = GetEditText(1);
				pTVMFailureRefundSvc->SetFailureInfo(i++,strNo);
				DWORD iNo = _ttoi64(strNo);

				// �����˿���
				m_baseInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(ComGetAmount(GetEditText(2))) + MONEY_UNIT;
				// ˢ�´���
				ShowCompoment(BASE_AREA);

				// ˢ�³˿ͻ��������Ϣ
				pTVMFailureRefundSvc->SetRefundMoney(ComGetAmount(GetEditText(2)));

				// ������
				BYTE bitNum[15]={0};
				
				// �����豸��
				USES_CONVERSION;
				ComfrHEX(T2A(strDeviceNo),bitNum,4);
				//memcpy(bitNum,,4);
				// ����ʱ��
				memset(bitNum+4,0,7);
				// �����豸AFC��ˮ��
		
				memcpy(bitNum+4+7,&iNo,4);
				DWORD dwRefundAmount = ComGetAmount(GetEditText(2));

				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertBomRefundTVMMsg(bitNum,dwRefundAmount);

				// �����������
				SCAuditKey purchaseKey;
				purchaseKey.serviceID = TVMFAILURE_REFUND_SVC;
				purchaseKey.cardIssuancer = ACC;
				purchaseKey.ticketType = TICKET_SINGLE_RIDE;
				
				SCAuditValue purchaseValue;
				purchaseValue.transCount = 1;
				purchaseValue.transAmount = 0-dwRefundAmount;
				theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);

				// һ����ϸ����
				//pTVMFailureRefundSvc->DoDetailInfo(atoi(GetEditText(2)));

				// ��ӡ��Ʊ	
				pTVMFailureRefundSvc->PrintServiceInfo(strDeviceNo,strNo);

				// �������б༭�����ּ���ȷ�ϣ����أ���ʱ�˿�Ǽ�ʱ�˿ť
				HideCompoment(BTN_OK|BTN_RESET|EDIT_BOX);

				// Ĭ��ʱ���ת��Ĭ��ҵ��
				theSERVICE_MGR.SetState(FINISH);

				// ��ʾBOM���ԣ�TVM�����˿�ɹ�
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_FINISH));
			}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pTVMFailureRefundSvc->GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ð�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CTVMFailureRefundBaseOfficeDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	// ���öԻ�������
	SetEditText(0,_T(""));
	SetEditText(1,_T(""));
	SetEditText(2,_T(""));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ի����ʼ��

@param      ��

@retval     BOOL  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTVMFailureRefundBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���������Ч��

@param      ��

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMFailureRefundBaseOfficeDlg::CheckValid()
{
	CString strTicketNo = GetEditText(1);
	CString strDeviceNo = GetEditText(0);
	CString strMoney = GetEditText(2);
	

	// ����豸����Ƿ�Ϊ��
	if (_T("") == strDeviceNo){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_PLEASE_INPUT_DEVICE_NO));
		return false;
	}

	// ����豸����Ƿ�Ϊ8λ
	if (strDeviceNo.GetLength() != 8){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_DEVICE_MUST_BE_NUMBER));
		return false;
	}

	// ���ƾ�����Ƿ�Ϊ��
	if (_T("") == strTicketNo || strTicketNo.GetLength() != 10){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_PLEASE_INPUT_TICKET_NO));
		return false;
	}

	__int64  iNo = _ttoi64(strTicketNo);
	if(iNo > 0xFFFFFFFF)
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TICKET_MUST_BE_NUMBER));
		return false;
	}

	// ������Ƿ�Ϊ��
	if (_T("") == strMoney){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_PLEASE_INPUT_MONEY));
		return false;
	}

	// ����Ƿ����˿�
	// ����ׯ�ߴ���copy���������ڲ���ʵ�ֹ��ܣ�ע�͵�

	/*CString sTVMErrorID = _T("");
	CString sErrorNum = _T("");
	sErrorNum.Format("%08d", atoi(strTicketNo));
	sTVMErrorID = strDeviceNo + sErrorNum;

	CCTVMRefund::TVM_ERROR_TICKET_ID ErrId;
	ErrId.csTVMErrorSierialNo = sTVMErrorID;

	
	if (theTVMRefund.CheckIsTVMTicket(strTicketNo) == false){
		vector<CCTVMRefund::TVM_ERROR_TICKET_ID> vecErrorID;
		vecErrorID.push_back(ErrId);
		theTVMRefund.UpdateTVMTicketInfo(vecErrorID);
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TVMFAILURE_ERROR));
		return false;
	}*/
	return true; 
}


