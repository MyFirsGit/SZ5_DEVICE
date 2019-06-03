#include "stdafx.h"
#include "RefundApplySvc.h"
#include "RefundApplyOfficeDlg.h"
#include "ARefundBaseOfficeDlg.h"
#include "ARefundReceptionDlg.h"
#include "WaitingOfficeDlg.h"
#include "WelcomeReceptionDlg.h"
#include "GuideInfo.h"
#include "TemplateHelper.h"
#include "TicketHelper.h"
#include "RefundQueryOfficeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRefundApplySvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundApplySvc::CRefundApplySvc()
	:CBOMForeService(REFUND_APPLY_SVC)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundApplySvc::~CRefundApplySvc()
{
}


void CRefundApplySvc::OnStart()
{
	// ���ò���Ա����
	if(m_DataModel.isQuerySvc)
	{
		m_pDialogFlow->AddDialog(IDD_00602_REFUND_QUERY_DLG,new CRefundQueryOfficeDlg(this));
	}
	else
	{
		m_pDialogFlow->AddDialog(IDD_WAITING_DLG,new CWaitingOfficeDlg(this));
		m_pDialogFlow->AddDialog(IDD_00602_REFUND_APPLY_DLG,new CRefundApplyOfficeDlg(this));
	}
	m_pDialogFlow->AddDialog(IDD_00602_REFUND_BASE_DLG,new CARefundBaseOfficeDlg(this));

	// ���ó˿ͻ���
	m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG,new CWelcomeReceptionDlg(this));
	m_pDialogFlow->AddDialog(IDD_10601_REFUND_DLG,new CARefundReceptionDlg(this));

	// ���û������
	if(m_DataModel.isQuerySvc)
	{
		m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_QUERY,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_SELECT_MTD_CONFIRM));
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_QUERY,IDD_00602_REFUND_QUERY_DLG);
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_QUERY,IDD_10002_WELCOME_DLG);

		m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_QUERY_FINISH,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_QUERY_ERROR));
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_QUERY_FINISH,IDD_00602_REFUND_QUERY_DLG);
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_QUERY_FINISH,IDD_10002_WELCOME_DLG);
	}
	else
	{
		// �ȴ���������
		m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_PUT_CARD));
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT,IDD_WAITING_DLG);
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT,IDD_10002_WELCOME_DLG);

		// �˿����뻭��
		m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_APPLY,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_SELECT_MTD_CONFIRM));
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_APPLY,IDD_00602_REFUND_APPLY_DLG);
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_APPLY,IDD_10002_WELCOME_DLG);

		// ���벻�ɻ���  
		m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_APPLY_NOT,/*theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_APPLY_NOT)*/_T(""));	
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_APPLY_NOT,IDD_00602_REFUND_APPLY_DLG);
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_APPLY_NOT,IDD_10002_WELCOME_DLG);

		// ������ɻ���
		m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_APPLY_FINISH,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_FINISH));
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_APPLY_FINISH,IDD_00602_REFUND_APPLY_DLG);
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_APPLY_FINISH,IDD_10002_WELCOME_DLG);
	}

	// �˿��������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_SELECT_MTD_CONFIRM),
		theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_CONFIRM_PASSENGER), 
		theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_CONFIRM_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE,IDD_00602_REFUND_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE,IDD_10601_REFUND_DLG);

	// �˿���ɻ���
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_FINISH),
		theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_FINISH_PASSENGER), 
		theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_FINISH_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_00602_REFUND_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_10601_REFUND_DLG);
	if(m_DataModel.isQuerySvc)
	{
		//������ת
		m_pDialogFlow->AddFlow(DIALOG_GROUP_QUERY,WM_TO_REFUND_QUERY_FINISH_DLG,DIALOG_GROUP_QUERY_FINISH);
		m_pDialogFlow->AddFlow(DIALOG_GROUP_QUERY,WM_TO_REFUND_BASE_APPLY_DLG,DIALOG_GROUP_BASE);
		m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_TO_REFUND_FINISH_APPLY_DLG,DIALOG_GROUP_FINISH);
		// ���ó�ʼ������
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_QUERY);
	}
	else
	{
		//������ת
		m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_TO_REFUND_APPLY_DLG,DIALOG_GROUP_APPLY);
		m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_TO_REFUND_APPLY_NOT_DLG,DIALOG_GROUP_APPLY_NOT);
		m_pDialogFlow->AddFlow(DIALOG_GROUP_APPLY,WM_TO_REFUND_APPLY_FINISH_DLG,DIALOG_GROUP_APPLY_FINISH);
		m_pDialogFlow->AddFlow(DIALOG_GROUP_APPLY,WM_TO_REFUND_BASE_APPLY_DLG,DIALOG_GROUP_BASE);
		m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_TO_REFUND_FINISH_APPLY_DLG,DIALOG_GROUP_FINISH);
		// ���ó�ʼ������
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
	}
	__super::OnStart();
}

void CRefundApplySvc::SetIsQuery(bool isQuery)
{
	m_DataModel.isQuerySvc = isQuery;
}


CRefundApplySvc::Model& CRefundApplySvc::GetDataModel()
{
	return m_DataModel;
}




//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ó�Ʊ��Ϣ

@param      (i)  UINT errCode              �������
@param      (i)  CRWReadCard* rwReadCard   ��������

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplySvc::OnReadCard(UINT errCode, CRWReadCard* rwReadCard)
{
	try{
		CMessageBox::End();
		RW_READ_CARD_RESPONSE cardRspInfo ;
		rwReadCard->GetRWResponse(&cardRspInfo);
		m_DataModel.cardPHType = cardRspInfo.CardPHType;
		if(errCode == RW_RSP_ACC_CARD_BLACKLIST || errCode == RW_RSP_ECT_CARD_BLACKLIST) //��������
		{
			RW_BUSSINESS_RESPONSE response;
			errCode = BlockCard(response); 
			CString guideInfo = errCode == RW_RSP_OK ? _T("������������������") :  m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode);
			DoDialogFlow(WM_TO_REFUND_APPLY_NOT_DLG);
			theAPP_SESSION.ShowOfficeGuide(guideInfo);	
		}
		else if(RW_RSP_OK != errCode)//����ʧ�ܣ�����Ϊ����
		{
			if(cardRspInfo.DetailErrorCode != RW_ERROR_CARD_NO_CARD)
			{
				CString guideInfo = errCode == RW_RSP_OK ? _T("������������������") :  m_pRWHelper->GetTpuErrorInfo(cardRspInfo.DetailErrorCode);
				DoDialogFlow(WM_TO_REFUND_APPLY_NOT_DLG);
				theAPP_SESSION.ShowOfficeGuide(guideInfo);
			}
			else
			{
				// ��ת���˿����뻭��
				m_DataModel.applyType = APPLY_TYPE_BAD_CARD;
				DoDialogFlow(WM_TO_REFUND_APPLY_DLG);
			}
		}
		else if(cardRspInfo.CardPHType == ULTRALIGHT)//UL�����ܷǼ�ʱ�˿���
		{
			DoDialogFlow(WM_TO_REFUND_APPLY_NOT_DLG);
			theAPP_SESSION.ShowOfficeGuide(_T("�ó�Ʊ���ܽ��зǼ�ʱ�˿�"));
		}
		else if(cardRspInfo.CardPHType == CPU_TICKET)
		{
			m_DataModel.applyType = APPLY_TYPE_GOOD_CARD;// ��״̬
			// ���ÿ�Ƭ����
			m_DataModel.cardType = theTICKET_HELPER.GetCardType(cardRspInfo.CardPHType);
			CCPUCard cpuCard;
			// ��ö�����Ϣ 			    
			cpuCard.GetCardInfo(cardRspInfo.CardInfoLen,cardRspInfo.CardInfo);
			// ��Ʊ��Ʒ����
			m_DataModel.cardAppType = cpuCard.applicationData.CardAppMainType;
			// ��Ʊ���
			m_DataModel.strTicketNum.Format(_T("%09u"),cpuCard.applicationData.CardLogicalNumber);
			// Ʊ������
			theACC_TICKET.GetTicketTypeName(cpuCard.applicationData.CardAppMainType,m_DataModel.strTicketType_CN,m_DataModel.strTicketType_EN);
			// ��Ч��
			m_DataModel.strValidDate = cpuCard.applicationData.ValidDate.ToString(_T("%.4d��%.2d��%.2d��"));
			// ����
			m_DataModel.strUserName = cpuCard.personalData.Name;
			// �Ա�
			m_DataModel.sexType = (Sex_t)cpuCard.personalData.Sex;
			// ֤������
			m_DataModel.certificateType = (CERTIFICATE_TYPE)cpuCard.personalData.CredentialsType;
			// ֤�����
			m_DataModel.strCertificateNumber = cpuCard.personalData.CredentialsNumber;
			// ���д���
			
			m_DataModel.cityCode = Hex2Dec(cpuCard.applicationData.CityCode)/*BCD2int((char*)&cpuCard.applicationData.CityCode,4)*/;
			// ��ҵ����
			m_DataModel.industryCode = Hex2Dec(cpuCard.applicationData.IndustryCode)/*BCD2int((char*)&cpuCard.applicationData.IndustryCode,4)*/;
			// Ʊ��������
			m_DataModel.ticketComHdr.issuerID = cpuCard.applicationData.PublisherCode == 0 ? 1: cpuCard.applicationData.PublisherCode;
			ComConvertStrToArrEight(m_DataModel.strTicketNum,m_DataModel.ticketComHdr.ticketApplicationSerialNo);
			m_DataModel.ticketComHdr.ticketType = theACC_TICKET.GetTicketMainType(cpuCard.applicationData.CardAppMainType);//
			m_DataModel.ticketComHdr.ticketSubType = cpuCard.applicationData.CardAppMainType;
			m_DataModel.ticketComHdr.ticketStatus = cpuCard.applicationData.TicketStatus;
			m_DataModel.ticketBalance = cpuCard.applicationData.Balance;
			m_DataModel.ticketDeposit = cpuCard.applicationData.Deposit;
			// ��ת���˿����뻭��
			DoDialogFlow(WM_TO_REFUND_APPLY_DLG);
		}	
	}
	// �쳣����
	catch (CSysException& e) {
		DoDialogFlow(WM_TO_REFUND_APPLY_NOT_DLG);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		DoDialogFlow(WM_TO_REFUND_APPLY_NOT_DLG);
		CInnerException e(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����

@param      (i)RW_BUSSINESS_RESPONSE	response	������������

@retval     long	�������

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRefundApplySvc::BlockCard(RW_BUSSINESS_RESPONSE& response)
{
	RW_CMD_BLOCK rw_cmd_block;
	rw_cmd_block.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
	rw_cmd_block.TranDateTime = ComGetCurTime();
	long resultCode = m_pRWHelper->Block(rw_cmd_block,response);
	// �����ɹ�
	if(RW_RSP_OK == resultCode)
	{
		theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	
		if(response.UDlen != 0)
		{
			m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
		}
		if(response.ARLen != 0){
			m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,response.TranResult.CardType);
		}
	}
	return resultCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���TPU������Ϣ

@param      none

@retval     CString	TPU������Ϣ

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CRefundApplySvc::GetTpuErrorInfo(int errCode)
{
	CString strError;
	if(m_DataModel.applyType == APPLY_TYPE_GOOD_CARD){
		strError = m_pRWHelper->GetTpuErrorInfo(errCode);
	}
	else{
		strError = "���죩���ͽ�������ʧ��";
	}
	return strError;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˿�

@param      none

@retval     long	�˿�״̬

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRefundApplySvc::Refund()
{
	long errorCode = 0;
	if(m_DataModel.applyType == APPLY_TYPE_GOOD_CARD){
		RW_CMD_REFUND refund;
		m_DataModel.transTime = ComGetCurTime();
		refund.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
		refund.TranDateTime = m_DataModel.transTime;
		refund.CardPHType = m_DataModel.cardType;
		refund.CardAppType = m_DataModel.cardAppType;
		refund.ReasonCode = m_DataModel.refundReason;
		refund.RefundAmount = m_DataModel.refundBalance;
		refund.DepositAmount = m_DataModel.refundDeposit;
		RW_BUSSINESS_RESPONSE response;
		errorCode = m_pRWHelper->Refund(refund,response);
		if(RW_RSP_OK == errorCode){
			// ���½�����ˮ��
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	
			// ����UD����
			if(response.UDlen != 0){
				m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
			}
			// ����AR����
			if(response.ARLen != 0){
				m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,m_DataModel.cardType);
			}
			// SC���
			SaveSCData(2);
			PrinterPrint(false);
		}
		else{
			return response.DetailErrorCode==0 ? errorCode : response.DetailErrorCode;//û�д�����ϸ����ʱֱ�ӷ�����ֵ
		}
	}
	else{
		NonImmidatelySurrender refundInfo;
		m_DataModel.transTime = ComGetCurTime();
		ComConvertStrToArrEight(m_DataModel.strTicketNum,refundInfo.cardNum);
		refundInfo.transAmount = m_DataModel.ticketBalance;
		refundInfo.refundReason = m_DataModel.refundReason;
		refundInfo.depositAmount = m_DataModel.ticketDeposit;
		errorCode = theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendNonImmediatelySurrender(refundInfo);
		if(errorCode == SP_SUCCESS){
			SaveSCData(2);
			PrinterPrint(false);
		}
	}
	return errorCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ѯ�˿�������

@param      none

@retval     BYTE		��ѯ״̬	00������ˣ�01������ˣ�02�����ϣ�03���Ѵ���04�����˿�


@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BYTE CRefundApplySvc::QueryRefundApplyResult()
{
	BYTE checkStatus = 0;
	try{
		CRefundApplyResult::REFUND_APP_RESULT refund_app_result;
		refund_app_result.certificateType = m_DataModel.certificateType;
		memcpy(refund_app_result.certificateID,m_DataModel.strCertificateNumber.GetBuffer(),m_DataModel.strCertificateNumber.GetLength());
		memcpy(refund_app_result.ticketApplicationSerialNo,m_DataModel.ticketComHdr.ticketApplicationSerialNo,sizeof(refund_app_result.ticketApplicationSerialNo));
		// ��ѯ�������˽��
		bool isChecked = theREFUND_APPLY_RRSULT.GetTheApplyResult(refund_app_result);
		// ���ؼ�¼������
		if(!isChecked){
			QueryNonImmidatelySurrenderApply queryInfo;
			queryInfo.certificateType = m_DataModel.certificateType;
			memcpy(queryInfo.certificateID,m_DataModel.strCertificateNumber.GetBuffer(),m_DataModel.strCertificateNumber.GetLength());
			memcpy(queryInfo.billNum,m_DataModel.strApplyBillNum.GetBuffer(),m_DataModel.strApplyBillNum.GetLength());
			if(m_DataModel.applyType == APPLY_TYPE_GOOD_CARD){
				queryInfo.cityCode = m_DataModel.cityCode;
				queryInfo.industryCode = m_DataModel.industryCode;
				queryInfo.ticketComHdr = m_DataModel.ticketComHdr;
			}
			else{
				queryInfo.cityCode = 1100;
				queryInfo.industryCode = 0003;
				queryInfo.ticketComHdr.issuerID = 0001;
				ComConvertStrToArrEight(m_DataModel.strTicketNum,queryInfo.ticketComHdr.ticketApplicationSerialNo);
			}
			int count = 0;
			long result = theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendQueryNonImmidatelySurrenderApply(queryInfo);
			if(result != SP_SUCCESS){
				return 0xFF;
			}
			do{
				if(count == 2){
					break;
				}				
				Sleep(60*1000);
				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendQueryNonImmidatelySurrenderApply(queryInfo);
				count++;				
			}while(!theREFUND_APPLY_RRSULT.GetTheApplyResult(refund_app_result));	// �����
		}
		BYTE emptyCertificateID[20]={0};
		if(memcmp(emptyCertificateID,refund_app_result.certificateID,sizeof(emptyCertificateID)) !=0)
		{
			m_DataModel.certificateType = refund_app_result.certificateType;
			memcpy(m_DataModel.strCertificateNumber.GetBuffer(),refund_app_result.certificateID,sizeof(refund_app_result.certificateID));
		}
		BYTE emptyTicketNo[8]={0};
		if(memcmp(emptyTicketNo,refund_app_result.ticketApplicationSerialNo,sizeof(refund_app_result.ticketApplicationSerialNo))!=0)
		{
			memcpy(m_DataModel.ticketComHdr.ticketApplicationSerialNo,refund_app_result.ticketApplicationSerialNo,sizeof(refund_app_result.ticketApplicationSerialNo));
			int ticketNum = ComMakeLong(*(m_DataModel.ticketComHdr.ticketApplicationSerialNo+4),*(m_DataModel.ticketComHdr.ticketApplicationSerialNo+5),*(m_DataModel.ticketComHdr.ticketApplicationSerialNo+6),*(m_DataModel.ticketComHdr.ticketApplicationSerialNo+7));
			m_DataModel.strTicketNum.Format(_T("%09u"),ticketNum);
		}
		m_DataModel.refundBalance =refund_app_result.Balance;
		m_DataModel.ticketDeposit = m_DataModel.refundDeposit =refund_app_result.Deposit;
		SaveSCData(1);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return checkStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˿�����

@param      none

@retval     bool		�˿�����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRefundApplySvc::RefundApply()
{
	NonImmidatelySurrenderApply applyInfo;
	m_DataModel.transTime = ComGetCurTime();
	applyInfo.gender = m_DataModel.sexType;
	memcpy(applyInfo.name,m_DataModel.strUserName.GetBuffer(),m_DataModel.strUserName.GetLength());
	applyInfo.certificateType = m_DataModel.certificateType;
	memcpy(applyInfo.certificateID,m_DataModel.strCertificateNumber.GetBuffer(),m_DataModel.strCertificateNumber.GetLength());
	m_DataModel.strApplyBillNum.Format(_T("%d"),theCOUNT_INFO.GetUDSNForDisplay());
	memcpy(applyInfo.billNum,m_DataModel.strApplyBillNum.GetBuffer(),m_DataModel.strApplyBillNum.GetLength());
	applyInfo.nouseReason = m_DataModel.applyReason;
	applyInfo.applyType = m_DataModel.applyType;
	applyInfo.reason = m_DataModel.refundReason;
	memcpy(applyInfo.suspendBillNum,m_DataModel.strSuspendBillNum.GetBuffer(),m_DataModel.strSuspendBillNum.GetLength());
	if(m_DataModel.applyType == APPLY_TYPE_GOOD_CARD){
		applyInfo.cityCode = m_DataModel.cityCode;
		applyInfo.industryCode = m_DataModel.industryCode;
		applyInfo.ticketComHdr = m_DataModel.ticketComHdr;
	}
	else{
		applyInfo.cityCode = 1100;
		applyInfo.industryCode = 0003;
		applyInfo.ticketComHdr.issuerID = 0001;
		ComConvertStrToArrEight(m_DataModel.strTicketNum,applyInfo.ticketComHdr.ticketApplicationSerialNo);
	}
	SaveSCData(0);
	return theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendNonImmidatelySurrenderApply(applyInfo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��֤����ԱID������

@param      int nOperatorID ����ԱID
@param      CString sPwd ����

@retval     bool

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CRefundApplySvc::CheckOperatorIdAndPwd(int nOperatorID,CString sPwd)
{
	typedef CAfcStaffPwdParam::STAFF STAFF_t;
	vector<STAFF_t> vecStaff = theAFC_STAFF.GetStaffParam();
	STAFF_t currentStaff;
	bool hasFound=false;
	for(int i=0;i<vecStaff.size();i++)
	{
		if(vecStaff.at(i).staffID == nOperatorID)
		{
			currentStaff = vecStaff.at(i);
			hasFound = true;
			break;
		}
	}
	if(!hasFound)
	{
		return false;
	}
	if(thePWD_ERR.IsStuffIDExist(nOperatorID))	// ID������
	{
		return false;
	}
	if(sPwd != currentStaff.staffPWD)	// ���벻��ȷ
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ��ͨ��ֵsc����

@param      none

@retval  	long         ������Ϣ

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplySvc::SaveSCData(char flag)
{
	SCAuditKey purchaseKey;
	purchaseKey.serviceID = flag == 2 ? REFUND_SVC : REFUND_APPLY_SVC;
	purchaseKey.cardIssuancer = theTICKET_HELPER.GetCardIssuancer(m_DataModel.cardAppType);
	purchaseKey.ticketType = m_DataModel.cardAppType;
	purchaseKey.flag = flag;
	SCAuditValue purchaseValue;
	purchaseValue.transCount = 1;
	if(flag == 2)
	{
		purchaseValue.transAmount = 0-m_DataModel.refundBalance;
		purchaseValue.depositAmount = 0-m_DataModel.refundDeposit;
		purchaseValue.recyleCardNum = 1;
	}
	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ���ݡ���Ǯ��

@param      none

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundApplySvc::PrinterPrint(bool isApply)
{
	CString templateName= _T("REFUND.template");
	CStringArray printArr;
	for(int printIndex=0;printIndex<theBOM_INFO.GetReceiptPrintTimes(_T("REFUND"));printIndex++)
	{
		CStringArray printTexts;
		theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
		const int hspCount = 20;
		int i = 0;
		while(i < printTexts.GetCount())
		{
			CString& line = printTexts.ElementAt(i);
			if(!isApply){
				line.Replace(_T("{REFUND_CARD_TITLE}"),_T(""));
				line.Replace(_T("{CARD_DEPOSIT}"),m_DataModel.refundDeposit <= 0 ? TXT_UNDEFINE : ComFormatAmount(m_DataModel.refundDeposit) + MONEY_UNIT);
				line.Replace(_T("{CARD_BALANCE}"),m_DataModel.refundBalance <= 0 ? TXT_UNDEFINE : ComFormatAmount(m_DataModel.refundBalance) + MONEY_UNIT);
				line.Replace(_T("{REFUND_AMOUNT}"),ComFormatAmount(m_DataModel.refundBalance+m_DataModel.refundDeposit) + MONEY_UNIT);
				if(line.Find(_T("{REFUND_APPLY_TITLE}")) >=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				if(line.Find(_T("{BILL_NUM}")) >=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
			}
			else{
				line.Replace(_T("{REFUND_APPLY_TITLE}"),_T(""));
				line.Replace(_T("{BILL_NUM}"),m_DataModel.strApplyBillNum);
				if(line.Find(_T("{REFUND_CARD_TITLE}")) >=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				if(line.Find(_T("{CARD_DEPOSIT}")) >=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				if(line.Find(_T("{CARD_BALANCE}")) >=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				if(line.Find(_T("{REFUND_AMOUNT}")) >=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
			}
			if(m_DataModel.strTicketNum.IsEmpty() && line.Find(_T("{CARD_NUM}"))>=0)
			{
				printTexts.RemoveAt(i);
				continue;
			}
			else
			{
				line.Replace(_T("{CARD_NUM}"),m_DataModel.strTicketNum);
			}
			if(m_DataModel.strTicketType_CN.IsEmpty() && line.Find(_T("{TICKET_TYPE}"))>=0)
			{
				printTexts.RemoveAt(i);
				continue;
			}
			else
			{
				line.Replace(_T("{TICKET_TYPE}"),m_DataModel.strTicketType_CN);
			}
			if(m_DataModel.strValidDate.IsEmpty() && line.Find(_T("{VALID_DATE}"))>=0)
			{
				printTexts.RemoveAt(i);
				continue;
			}
			else
			{
				line.Replace(_T("{VALID_DATE}"), m_DataModel.strValidDate);
			}
			if(m_DataModel.strCertificateNumber.IsEmpty() && (line.Find(_T("{CERTIFICATE_TYPE}"))>=0 || line.Find(_T("{CERTIFICATE_NUM}"))>=0))
			{
				printTexts.RemoveAt(i);
				continue;
			}
			else
			{
				line.Replace(_T("{CERTIFICATE_NUM}"),m_DataModel.strCertificateNumber);
				CString strCertificateType = _T("");
				switch(m_DataModel.certificateType)
				{
				case CERTIFICATE_TYPE::CERTIFICATE_ID_CARD:
					strCertificateType = TXT_CERTIFICATE_ID_CARD_MAINLAND;
					break;
				case CERTIFICATE_TYPE::CERTIFICATE_STUDENT_CARD:
					strCertificateType = TXT_CERTIFICATE_STUDENT;
					break;
				case CERTIFICATE_TYPE::CERTIFICATE_MILITARY_OFFICER:
					strCertificateType = TXT_CERTIFICATE_MILITARY;
					break;
				case CERTIFICATE_TYPE::CERTIFICATE_POST_CARD:
					strCertificateType = TXT_CERTIFICATE_FOREIGNER;
					break;
				default:
					break;
				}
				line.Replace(_T("{CERTIFICATE_TYPE}"),strCertificateType);
			}
			if(m_DataModel.strSuspendBillNum.IsEmpty() && line.Find(_T("{SUSPEND_BILL_NUM}"))>=0)
			{
				printTexts.RemoveAt(i);
				continue;
			}
			else
			{
				line.Replace(_T("{SUSPEND_BILL_NUM}"),m_DataModel.strSuspendBillNum);
			}
			line.Replace(_T("{TRANS_TIME}"),m_DataModel.transTime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
			line.Replace(_T("{STATION_NAME}"),theAPP_SESSION.GetStationNameCN());
			line.Replace(_T("{DEVICE_CODE}"),theMAINTENANCE_INFO.GetCurrentACCDevice().ToString());
			line.Replace(_T("{USER_ID}"),theAPP_SESSION.GetUserInfo().sUserID);
			CStringArray printLine;
			ComPrintLineFormatForTemplate(line,printLine);
			printArr.Append(printLine);
			i++;
		}
	}
	if(!isApply){
		m_pPRTHelper->PrinterPrint(&printArr,true);
	}
	else{
		m_pPRTHelper->PrinterPrint(&printArr,false);
	}

}