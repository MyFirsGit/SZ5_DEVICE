#include "StdAfx.h"
#include "ChargeSvc.h"
#include "ServiceDefinition.h"
#include "Service.h"
#include "chargebaseofficedlg.h"
#include "waitingofficedlg.h"
#include "WelcomeReceptionDlg.h"
#include "chargereceptiondlg.h"
#include "BOMForeService.h"
#include "Util.h"
#include "GuideInfo.h"
#include "TicketHelper.h"
#include "TemplateHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CChargeSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CChargeSvc::CChargeSvc()
    :CBOMForeService(CHARGE_SVC)
{
	// ����վԱ����
	m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_00302_CHARGE_STAFF_DLG, new CChargeBaseOfficeDlg(this));

	// ���ó˿ͻ���
	m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));
	m_pDialogFlow->AddDialog(IDD_10301_CHARGE_DLG, new CChargeReceptionDlg(this));

	// ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT, theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_PUT_CARD));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_10002_WELCOME_DLG);

	// ��ֵ��������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_SVTICKET),
		theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CHARGE_CONFIRM_PASSENGER),
		theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CHARGE_CONFIRM_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_00302_CHARGE_STAFF_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_10301_CHARGE_DLG);

	// ��ֵ��������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_FINISH),
		theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CHARGE_FINISH_PASSENGER),
		theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CHARGE_FINISH_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_00302_CHARGE_STAFF_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_10301_CHARGE_DLG);

	// ��ֵ���ɻ���  
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CHARGE_NOT,_T(""));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHARGE_NOT,IDD_00302_CHARGE_STAFF_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHARGE_NOT,IDD_10301_CHARGE_DLG);

	//������ת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_CHARGE_BASE_DLG, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_CHARGE_NOT_DLG, DIALOG_GROUP_CHARGE_NOT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_CHARGE_FINISH_DLG, DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_BASE_TO_CHARGE_NOT_DLG,DIALOG_GROUP_CHARGE_NOT);
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
CChargeSvc::~CChargeSvc(void)
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ����Ʊ������Ϣ

@param      none

@retval     UNIT	��Ʊ��Ϣ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CChargeSvc::Model& CChargeSvc::GetDataModel()
{
	return m_DataModel;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���TPU������Ϣ

@param      (i)int	errCode	�������

@retval     CString

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CChargeSvc::GetTpuErrorInfo(int errCode)
{
	return m_pRWHelper->GetTpuErrorInfo(errCode);
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
void CChargeSvc::OnReadCard(UINT errCode, CRWReadCard* rwReadCard)
{
	try{
		CMessageBox::End();
		RW_READ_CARD_RESPONSE cardRspInfo ;
		rwReadCard->GetRWResponse(&cardRspInfo);
		m_DataModel.cardPHType = (CARD_MEDIUM_TYPE)cardRspInfo.CardPHType;
		// ����ʧ��
		if(errCode == RW_RSP_ACC_CARD_BLACKLIST || errCode == RW_RSP_ECT_CARD_BLACKLIST){
			// ������Ʊ��
			RW_BUSSINESS_RESPONSE response;
			errCode = BlockCard(response); 
			CString guideInfo =errCode == RW_RSP_OK ? theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_BLACK_CARD) : m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode);
			// ����ֵ���ɻ���
			DoDialogFlow(WM_TO_CHARGE_NOT_DLG);
			theAPP_SESSION.ShowAllGuide(guideInfo,theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CHARGE_NOT_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CHARGE_NOT_PASSENGER));	
			return;
		}
		// �޿�����Ĭ�Ͻ���
		if(errCode!=RW_RSP_OK)
		{
			DoDialogFlow(WM_TO_CHARGE_NOT_DLG);
			if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD)
			{
				theAPP_SESSION.ShowAllGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CANCEL));	
			}
			else
			{
				theAPP_SESSION.ShowAllGuide(m_pRWHelper->GetTpuErrorInfo(cardRspInfo.DetailErrorCode),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CHARGE_NOT_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CHARGE_NOT_PASSENGER));
			}
			return;
		}
		bool saveCardInfoSuccess = SaveCardInfo(&cardRspInfo);
		if(!saveCardInfoSuccess)
		{
			return;
		}
		if(m_DataModel.ticketCategory == PERIOD)
		{
			OnError(_T("���죩��Ʊ���ܽ��г�ֵҵ��"));
			return;
		}
		DoDialogFlow(WM_TO_CHARGE_BASE_DLG);
		NotifyDataModelChanged();
	}
	catch (CSysException& e){
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

bool CChargeSvc::SaveCardInfo(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	m_DataModel.cardType = theTICKET_HELPER.GetCardType(pReadCardResponse->CardPHType);
	if(m_DataModel.cardPHType == ULTRALIGHT)
	{
		CULCard ulCard;
		ulCard.GetCardInfo(pReadCardResponse->CardInfo);
		m_DataModel.ticketType = ulCard.applicationData.CardAppMainType;
		m_DataModel.strTicketNumber.Format(_T("%09u"),ulCard.applicationData.CardLogicalNumber);
		theACC_TICKET.GetTicketTypeName(ulCard.applicationData.CardAppMainType,m_DataModel.strTicketName,m_DataModel.strTicketEnName);
		m_DataModel.validDate = ulCard.applicationData.ValidStartTime;
		m_DataModel.deposit = ulCard.applicationData.Deposit;
		m_DataModel.balance = ulCard.applicationData.Balance;
		theACC_TICKET.GetTicketCategry(m_DataModel.ticketType,m_DataModel.ticketCategory);
	}
	else if(m_DataModel.cardPHType == CPU_TICKET)
	{
		CCPUCard cpuCard;
		cpuCard.GetCardInfo(pReadCardResponse->CardInfoLen,pReadCardResponse->CardInfo);
		m_DataModel.ticketType = cpuCard.applicationData.CardAppMainType;
		m_DataModel.strTicketNumber.Format(_T("%09u"),cpuCard.applicationData.CardLogicalNumber);
		theACC_TICKET.GetTicketTypeName(cpuCard.applicationData.CardAppMainType,m_DataModel.strTicketName,m_DataModel.strTicketEnName);
		_DATE cpuValidDate = cpuCard.applicationData.ValidDate;
		m_DataModel.validDate = _DATE_TIME(cpuValidDate.wYear,cpuValidDate.biMonth,cpuValidDate.biDay,0,0,0);
		m_DataModel.deposit = cpuCard.applicationData.Deposit;
		m_DataModel.balance = cpuCard.applicationData.Balance;
		theACC_TICKET.GetTicketCategry(m_DataModel.ticketType,m_DataModel.ticketCategory);
	}
	else
	{
		OnError(_T("����ʶ��ĳ�Ʊ��������"));
		return false;
	}
	return true;
}

void CChargeSvc::OnError(CString strMessage)
{
	theSERVICE_MGR.SetState(FINISH);
	if(GetCurDialogGroupID() == DIALOG_GROUP_WAIT)
	{
		DoDialogFlow(WM_TO_CHARGE_NOT_DLG);
	}
	else if(GetCurDialogGroupID() == DIALOG_GROUP_BASE)
	{
		DoDialogFlow(WM_BASE_TO_CHARGE_NOT_DLG);
	}
	theAPP_SESSION.ShowOfficeGuide(strMessage);
}

void CChargeSvc::NotifyDataModelChanged()
{
	if(m_DataModel.paid != 0){
		m_DataModel.change = m_DataModel.paid - m_DataModel.due;
	}
	else{
		m_DataModel.change = 0;
	}
	m_DataModel.valueAfterCharge = m_DataModel.balance + m_DataModel.chargeValue;
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����

@param      (i)RW_BUSSINESS_RESPONSE	response	������������

@retval     long	�������

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::BlockCard(RW_BUSSINESS_RESPONSE response)
{
	UINT resultCode;
	RW_CMD_BLOCK rw_cmd_block;
	rw_cmd_block.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
	rw_cmd_block.TranDateTime = ComGetCurTime();
	resultCode = m_pRWHelper->Block(rw_cmd_block,response);
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


//////////////////////////////////////////////////////////////////////////
/**
@brief      CPU��ֵ����(��ֵ/�ƴ�) 

@param      none

@retval  	0:����ɹ�  ����:ʧ����Ϣ

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::DoCardPurseCharge()
{
	try{
		// CPUд��
		RW_CMD_CHARGE chargeInput;
		chargeInput.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
		chargeInput.TranDateTime = ComGetCurTime();
		chargeInput.CardType = m_DataModel.cardType;				// ��Ƭ����
		chargeInput.PaymentMethod = BY_CASH;						// �ֽ�֧��
		chargeInput.ChargeType = m_DataModel.ticketCategory;		// ��ֵ����
		chargeInput.ChargeAmount = m_DataModel.due;					// ��ֵ���
		chargeInput.ChargeCount = m_DataModel.chargeValue;			// ��ֵ����(�ƴο���)
		// ��ֵ
		RW_BUSSINESS_RESPONSE response;
		long errCode = m_pRWHelper->Charge(chargeInput,response);
		// д���ɹ�
		if(RW_RSP_OK == errCode){			
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
			SaveSCData();
			PrinterPrint();	
		}
		else{
			return response.DetailErrorCode;
		}
		return errCode;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      һ��ͨ��ֵ����

@param      none

@retval  	long	������Ϣ

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::DoCardCharge()
{
	m_DataModel.transTime = ComGetCurTime();	
	long ret = DoCardPurseCharge();
	if(ret == RW_RSP_OK){
		DoDialogFlow(WM_TO_CHARGE_FINISH_DLG);    
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      һ��ͨ��ֵ��ֵ��ʱ����

@param      (i)bool	bcheck	�Ƿ��ǽ��г�ʱ����

@retval  	long	������Ϣ

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::ChargeOverTime(bool bcheck)
{
	RW_CMD_CHARGEOVERTIME chargeOverTime;
    chargeOverTime.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
	chargeOverTime.byDo = bcheck;
	chargeOverTime.TranDateTime = ComGetCurTime();
	RW_BUSSINESS_RESPONSE response; 
	long res = m_pRWHelper->ChargeOverTimeProcess(chargeOverTime,response);
	if (res == RW_RSP_OK){
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
		SaveSCData();
		PrinterPrint();
		DoDialogFlow(WM_TO_CHARGE_FINISH_DLG);
	}
	else{
		return response.DetailErrorCode;
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ��ͨ��ֵsc����

@param      none

@retval  	long	������Ϣ

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::SaveSCData()
{
	SCAuditKey purchaseKey;
	purchaseKey.serviceID = CHARGE_SVC;
	purchaseKey.cardIssuancer =  theTICKET_HELPER.GetCardIssuancer(m_DataModel.ticketType);
	purchaseKey.ticketType = m_DataModel.ticketType;

	SCAuditValue purchaseValue;
	purchaseValue.transCount = 1;
	purchaseValue.transAmount = m_DataModel.due;
	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ���ݡ���Ǯ��

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::PrinterPrint()
{
	try{

		CString templateName= _T("CHARGE.template");
		CStringArray printArr;
		for(int printIndex=0;printIndex<theBOM_INFO.GetReceiptPrintTimes(_T("CHARGE"));printIndex++)
		{
			CStringArray printTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
			const int hspCount = 20;
			int i = 0;
			while(i < printTexts.GetCount())
			{
				CString& line = printTexts.ElementAt(i);
				line.Replace(_T("{CARD_NUM}"),m_DataModel.strTicketNumber);
				line.Replace(_T("{TICKET_TYPE}"),m_DataModel.strTicketName);
				line.Replace(_T("{VALID_DATE}"),m_DataModel.validDate.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
				if(m_DataModel.ticketCategory == PURSE){
					line.Replace(_T("{BEFORE_CHARGE_AMOUNT}"),ComFormatAmount(m_DataModel.balance) + MONEY_UNIT);
					line.Replace(_T("{CHARGE_AMOUNT}"),ComFormatAmount(m_DataModel.chargeValue) + MONEY_UNIT);
					line.Replace(_T("{AFTER_CHARGE_AMOUNT}"),ComFormatAmount(m_DataModel.valueAfterCharge) + MONEY_UNIT);
					if(line.Find(_T("{BEFORE_CHARGE_TIMES}")) >=0)
					{
						printTexts.RemoveAt(i);
						continue;
					}
					if(line.Find(_T("{CHARGE_TIMES}")) >=0)
					{
						printTexts.RemoveAt(i);
						continue;
					}
					if(line.Find(_T("{AFTER_CHARGE_TIMES}")) >=0)
					{
						printTexts.RemoveAt(i);
						continue;
					}
					if(line.Find(_T("{TRANS_AMOUNT}")) >=0)
					{
						printTexts.RemoveAt(i);
						continue;
					}
				}
				else if(m_DataModel.ticketCategory == MULTIRIDE){
					if(line.Find(_T("{BEFORE_CHARGE_AMOUNT}")) >=0)
					{
						printTexts.RemoveAt(i);
						continue;
					}
					if(line.Find(_T("{CHARGE_AMOUNT}") )>=0)
					{
						printTexts.RemoveAt(i);
						continue;
					}
					if(line.Find(_T("{AFTER_CHARGE_AMOUNT}") )>=0)
					{
						printTexts.RemoveAt(i);
						continue;
					}
					line.Replace(_T("{BEFORE_CHARGE_TIMES}"),ComFormatCount(m_DataModel.balance) + TIMES_UNIT);
					line.Replace(_T("{CHARGE_TIMES}"),ComFormatCount(m_DataModel.chargeValue) + TIMES_UNIT);
					line.Replace(_T("{AFTER_CHARGE_TIMES}"),ComFormatCount(m_DataModel.valueAfterCharge) + TIMES_UNIT);
					line.Replace(_T("{TRANS_AMOUNT}"),ComFormatAmount(m_DataModel.due) + MONEY_UNIT);
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
		//			m_pPRTHelper->PrinterPrint(&printTexts,true);
		m_pPRTHelper->SaveToChargePrintFile(&printArr,true);

	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}