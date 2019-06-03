#include "stdafx.h"
#include "RefundSvc.h"
#include "RefundBaseOfficeDlg.h"
#include "RefundReceptionDlg.h"
#include "WaitingOfficeDlg.h"
#include "WelcomeReceptionDlg.h"
#include "GuideInfo.h"
#include "TicketHelper.h"
#include "TemplateHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRefundSvc, CBOMForeService)
    //ON_RW_READ_CARD()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundSvc::CRefundSvc()
    :CBOMForeService(REFUND_BASE_SVC)
{
    
    // ���ò���Ա����
    m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
    m_pDialogFlow->AddDialog(IDD_00602_REFUND_BASE_DLG, new CRefundBaseOfficeDlg(this));
   
    // ���ó˿ͻ���
    m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(this));
    m_pDialogFlow->AddDialog(IDD_10601_REFUND_DLG, new CRefundReceptionDlg(this));

    // ���û������
    // �ȴ���������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT, theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_PUT_CARD));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT,IDD_WAITING_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT,IDD_10002_WELCOME_DLG);

    // �˿��������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_SELECT_MTD_CONFIRM),
                                                    theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_CONFIRM_PASSENGER), 
                                                    theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_CONFIRM_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE,IDD_00602_REFUND_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE,IDD_10601_REFUND_DLG);

	// �˿�ɻ���  
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_REFUND_NOT,/*theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_NOT)*/_T(""),
														theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_NOT_PASSENGER),
														theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_NOT_PASSENGER));	
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REFUND_NOT,IDD_00602_REFUND_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REFUND_NOT,IDD_10601_REFUND_DLG);

	// �˿���ɻ���
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_FINISH),
													theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_FINISH_PASSENGER), 
													theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_FINISH_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_00602_REFUND_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_10601_REFUND_DLG);

	//������ת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_TO_REFUND_BASE_DLG,DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_TO_REFUND_NOT_DLG,DIALOG_GROUP_REFUND_NOT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_TO_REFUND_FINISH_DLG,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_REFUND_NOT_DLG,DIALOG_GROUP_REFUND_NOT);
   
    // ���ó�ʼ������
    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundSvc::~CRefundSvc()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ���������ģ��

@param      none

@retval     PLABEL_GROUP	վԱ���������Ϣ

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundSvc::Model& CRefundSvc::GetDataModel()
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
void CRefundSvc::OnReadCard(UINT errCode, CRWReadCard* rwReadCard)
{
	try{
		CMessageBox::End();
		RW_READ_CARD_RESPONSE cardRspInfo ;
		rwReadCard->GetRWResponse(&cardRspInfo);
		m_DataModel.cardPHType = cardRspInfo.CardPHType;
		if(errCode == RW_RSP_ACC_CARD_BLACKLIST || errCode == RW_RSP_ECT_CARD_BLACKLIST){
			CString guideInfo;
			RW_BUSSINESS_RESPONSE response;
			errCode = BlockCard(response);
			if (errCode != RW_RSP_OK){
				guideInfo = m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode);				
			}
			else{
				guideInfo = _T("������������������");				
			}
			OnError(guideInfo, theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_NOT_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_NOT_PASSENGER));	
			return;
		}
		if(errCode != RW_RSP_OK){
			if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD)
			{
				OnError(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CANCEL));
			}
			else{
				OnError(m_pRWHelper->GetTpuErrorInfo(cardRspInfo.DetailErrorCode), theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_NOT_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_NOT_PASSENGER));
			}
			return;
		}	
		// ���ÿ�Ƭ����
		m_DataModel.cardType = theTICKET_HELPER.GetCardType(cardRspInfo.CardPHType);
		// �˿���
		m_DataModel.refundValue = cardRspInfo.RechargeProduct[0].RefoundMoney;
		if(cardRspInfo.CardPHType == ULTRALIGHT){
			// ��ö�����Ϣ 			    
			m_DataModel.pULCard->GetCardInfo(cardRspInfo.CardInfo);
			// ��Ʊ��Ʒ����
			m_DataModel.cardAppType = m_DataModel.pULCard->applicationData.CardAppMainType;
			// ��Ʊ���
			m_DataModel.strTicketNum.Format(_T("%09u"),m_DataModel.pULCard->applicationData.CardLogicalNumber);
			// Ʊ������
			theACC_TICKET.GetTicketTypeName(m_DataModel.pULCard->applicationData.CardAppMainType,m_DataModel.strTicketType_CN,m_DataModel.strTicketType_EN);
			// ��Ч��
			m_DataModel.strValidDate = m_DataModel.pULCard->applicationData.ValidEndTime.ToString(_T("%.4d��%.2d��%.2d��%.2d:%.2d"));
			// �������
			m_DataModel.ticketBalance = m_DataModel.pULCard->applicationData.Balance;
			// Ѻ��
			m_DataModel.deposit = 0;
			m_DataModel.refundValue = m_DataModel.ticketBalance;
			m_DataModel.refundDeposit = 0;
			
			// ��Ʊ��Ʒ����
			theACC_TICKET.GetTicketCategry(m_DataModel.pULCard->applicationData.CardAppMainType,m_DataModel.cardCategory);
			// ��ת����ʱ�˿��
			DoDialogFlow(WM_TO_REFUND_BASE_DLG);
		}
		else if(theBOM_INFO.IsEnableCPUImmidatelyRefund() && cardRspInfo.CardPHType == CPU_TICKET){
			
			// ��ö�����Ϣ 			    
			m_DataModel.pCPUCard->GetCardInfo(cardRspInfo.CardInfoLen,cardRspInfo.CardInfo);

			// ���С��0ʱ���˿�
			if(m_DataModel.pCPUCard->applicationData.Balance>0){
				// ��Ʊ��Ʒ����
				m_DataModel.cardAppType = m_DataModel.pCPUCard->applicationData.CardAppMainType;
				// ��Ʊ���
				m_DataModel.strTicketNum.Format(_T("%09u"),m_DataModel.pCPUCard->applicationData.CardLogicalNumber);
				// Ʊ������
				theACC_TICKET.GetTicketTypeName(m_DataModel.pCPUCard->applicationData.CardAppMainType,m_DataModel.strTicketType_CN,m_DataModel.strTicketType_EN);
				// ��Ч��
				m_DataModel.strValidDate = m_DataModel.pCPUCard->applicationData.ValidDate.ToString(_T("%.4d��%.2d��%.2d��"));
				// �������
				m_DataModel.ticketBalance = m_DataModel.pCPUCard->applicationData.Balance;
				// Ѻ��
				m_DataModel.deposit = m_DataModel.pCPUCard->applicationData.Deposit;
				m_DataModel.refundDeposit = 0 ;
				m_DataModel.refundValue = m_DataModel.ticketBalance;
				// ��Ʊ��Ʒ����
				theACC_TICKET.GetTicketCategry(m_DataModel.pCPUCard->applicationData.CardAppMainType,m_DataModel.cardCategory);
				// ��ת����ʱ�˿��
				DoDialogFlow(WM_TO_REFUND_BASE_DLG);
			}
			else{
				OnError(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_CPU_NOT),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_CPU_NOT_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_CPU_NOT_PASSENGER));
			}
		}
		else{
			OnError(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_CPU_NOT),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_CPU_NOT_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_CPU_NOT_PASSENGER));
		}
	}
	// �쳣����
	catch (CSysException& e) {
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

void CRefundSvc::OnError(CString msg,CString receiptMsg,CString receiptENMsg)
{
	theSERVICE_MGR.SetState(FINISH);
	if(GetCurDialogGroupID() == DIALOG_GROUP_WAIT)
	{
		DoDialogFlow(WM_TO_REFUND_NOT_DLG);
	}
	else if(GetCurDialogGroupID() == DIALOG_GROUP_BASE)
	{
		DoDialogFlow(WM_BASE_TO_REFUND_NOT_DLG);
	}
	theAPP_SESSION.ShowAllGuide(msg,receiptMsg,receiptENMsg);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����

@param      (i)RW_BUSSINESS_RESPONSE	response	������������

@retval     long	�������

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRefundSvc::BlockCard(RW_BUSSINESS_RESPONSE respons)
{
	RW_CMD_BLOCK rw_cmd_block;
	rw_cmd_block.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
	rw_cmd_block.TranDateTime = ComGetCurTime();
	RW_BUSSINESS_RESPONSE response;
	long resultCode = m_pRWHelper->Block(rw_cmd_block,response);
	// �����ɹ�
	if(RW_RSP_OK == resultCode){
		// ���½�����ˮ��
		theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	
		// ����UD����
		if(response.UDlen != 0){
			m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
		}

		// ����AR����
		if(response.ARLen != 0){
			m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,response.TranResult.CardType);
		}
	}
	return resultCode;
}


void CRefundSvc::OnOK()
{
	try {
		// �˿�ȷ��
		CString msg(""),sPrice("");
		sPrice = ComFormatAmount(m_DataModel.refundDeposit +m_DataModel.refundValue );
		msg = _T("�˿��") + sPrice + _T("Ԫ���Ƿ�ȷ�ϣ�");
		INT_PTR confirmResult = CMessageBox::Show(TITLE_REFUND_INFO,msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		// ȡ��
		if(confirmResult == IDCANCEL){
			return;
		}
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUNDING));
		theAPP_SESSION.ShowReceptionGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUNDING_PASSENGER), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUNDING_PASSENGER));
		theSERVICE_MGR.SetState(ISSUEING);
		// �˿��
		RW_CMD_REFUND refund;
		refund.SerialNo =  theCOUNT_INFO.GetUDSNForDisplay();
		refund.TranDateTime = ComGetCurTime();
		m_DataModel.transTime = refund.TranDateTime;
		refund.CardPHType = m_DataModel.cardType;
		refund.CardAppType = m_DataModel.cardAppType;
		refund.ReasonCode = m_DataModel.refundReason;
		refund.RefundAmount = m_DataModel.refundValue;
		refund.DepositAmount = m_DataModel.refundDeposit;
		RW_BUSSINESS_RESPONSE response;
		long ret = m_pRWHelper->Refund(refund,response);
		if(ret!= RW_RSP_OK)
		{

			OnError(m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_ERROR_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_ERROR_PASSENGER));
			return;
		}

		// ���½�����ˮ��
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
		// sc���
		SaveSCData();

		if(m_DataModel.cardPHType == CPU_TICKET) // CPU��������һ���˿����뽻��
		{
			NonImmidatelySurrenderApply applyInfo;
			applyInfo.nouseReason =APPLY_REASON_OTHER;
			applyInfo.applyType = APPLY_TYPE_GOOD_CARD;
			applyInfo.reason = m_DataModel.refundReason;

			applyInfo.gender =(Sex_t)m_DataModel.pCPUCard->personalData.Sex;
			memcpy(applyInfo.name,m_DataModel.pCPUCard->personalData.Name.GetBuffer(),m_DataModel.pCPUCard->personalData.Name.GetLength());
			applyInfo.certificateType = (CERTIFICATE_TYPE)m_DataModel.pCPUCard->personalData.CredentialsType;
			memcpy(applyInfo.certificateID,m_DataModel.pCPUCard->personalData.CredentialsNumber.GetBuffer(),m_DataModel.pCPUCard->personalData.CredentialsNumber.GetLength());
			applyInfo.cityCode = Hex2Dec(m_DataModel.pCPUCard->applicationData.CityCode);
			applyInfo.industryCode = Hex2Dec(m_DataModel.pCPUCard->applicationData.IndustryCode);

			// Ʊ��������
			applyInfo.ticketComHdr.issuerID = m_DataModel.pCPUCard->applicationData.PublisherCode == 0 ? 1: m_DataModel.pCPUCard->applicationData.PublisherCode;
			ComConvertStrToArrEight(m_DataModel.strTicketNum,applyInfo.ticketComHdr.ticketApplicationSerialNo);
			applyInfo.ticketComHdr.ticketType = theACC_TICKET.GetTicketMainType(m_DataModel.pCPUCard->applicationData.CardAppMainType);//
			applyInfo.ticketComHdr.ticketSubType = m_DataModel.pCPUCard->applicationData.CardAppMainType;
			applyInfo.ticketComHdr.ticketStatus = m_DataModel.pCPUCard->applicationData.TicketStatus;

			theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendNonImmidatelySurrenderApply(applyInfo);
		}

		PrinterPrint();
		DoDialogFlow(WM_TO_REFUND_FINISH_DLG);

	}
	catch (CSysException& e) {
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theSERVICE_MGR.SetState(FINISH);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e = CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theSERVICE_MGR.SetState(FINISH);
		theEXCEPTION_MGR.ProcessException(e);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ��ͨ��ֵsc����

@param      none

@retval  	long         ������Ϣ

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRefundSvc::SaveSCData()
{
	SCAuditKey purchaseKey;
	purchaseKey.serviceID = REFUND_SVC;
	purchaseKey.cardIssuancer = theTICKET_HELPER.GetCardIssuancer(m_DataModel.cardAppType);
	purchaseKey.ticketType = m_DataModel.cardAppType;

	SCAuditValue purchaseValue;
	purchaseValue.transCount = 1;
	purchaseValue.transAmount = 0-m_DataModel.refundValue;			//�˿���
	purchaseValue.recyleCardNum = 1;
	purchaseValue.depositAmount = 0-m_DataModel.refundDeposit;		//�˿�Ѻ��

	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
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
bool CRefundSvc::CheckOperatorIdAndPwd(int nOperatorID,CString sPwd)
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
@brief      ��ӡ���ݡ���Ǯ��

@param      none

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundSvc::PrinterPrint()
{
	try{
		CString templateName= _T("IMMIDATE_REFUND.template");
		CStringArray printArr;
		for(int printIndex=0;printIndex<theBOM_INFO.GetReceiptPrintTimes(_T("IMMIDATE_REFUND"));printIndex++)
		{
			CStringArray printTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
			const int hspCount = 20;
			int i = 0;
			while(i < printTexts.GetCount())
			{
				CString& line = printTexts.ElementAt(i);
				line.Replace(_T("{REFUND_CARD_TITLE}"),_T(""));
				line.Replace(_T("{CARD_DEPOSIT}"),m_DataModel.deposit <= 0 ? TXT_UNDEFINE : ComFormatAmount(m_DataModel.deposit) + MONEY_UNIT);
				line.Replace(_T("{CARD_BALANCE}"),m_DataModel.ticketBalance <= 0 ? TXT_UNDEFINE : ComFormatAmount(m_DataModel.ticketBalance) + MONEY_UNIT);
				line.Replace(_T("{REFUND_AMOUNT}"),ComFormatAmount(m_DataModel.refundDeposit+m_DataModel.refundValue) + MONEY_UNIT);

				if(line.Find(_T("{BILL_NUM}")) >=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				

				line.Replace(_T("{CARD_NUM}"),m_DataModel.strTicketNum);
				line.Replace(_T("{TICKET_TYPE}"),m_DataModel.strTicketType_CN.IsEmpty() ? TXT_UNDEFINE : m_DataModel.strTicketType_CN);
				line.Replace(_T("{VALID_DATE}"),m_DataModel.strValidDate.IsEmpty() ? TXT_UNDEFINE : m_DataModel.strValidDate);
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
		m_pPRTHelper->PrinterPrint(&printArr,true);

	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}