#include "stdafx.h"
#include "ExchangeSvc.h"
#include "ExchangeBaseOfficeDlg.h"
#include "ExchangeBackupDlg.h"
#include "ExchangeReceptionDlg.h"
#include "ServiceDefinition.h"
#include "WaitingOfficeDlg.h"
#include "BOMForeService.h"
#include "guideinfo.h"
#include "WelcomeReceptionDlg.h"
#include "TemplateHelper.h"
#include "Service.h"
#include "ReceptionDlg.h"
#include "RWDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CExchangeSvc, CBOMForeService)
  
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeSvc::CExchangeSvc()
      :CBOMForeService(EXCHANGE_SVC)
{
    // ��ʼ����Ա����
	pCpuCard =  new CCPUCard;	                                                     // cpu����Ϣ�ṹ��
	pUlCard = new CULCard;                                                        // ul����Ϣ�ṹ��
	m_ExchangeInfoSize = 0;                                               // վԱ������ʾ��Ϣ����  
	m_TicketType = 0;                                                       // ��Ʊ������     
	m_isOldCard = true;                                                        // �ɿ���־
	m_TicketType=0;                                                       // ��Ʊ������     
	m_strOldNum="";
	m_strNewNum="";
	m_strDeposit="";
	m_ExchangeInfoSize = InitExchangeInfo();

    // ���ò���Ա����
    m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
    m_pDialogFlow->AddDialog(IDD_00501_EXCHANGE_BASE_DLG, new CExchangeBaseOfficeDlg(this));
    m_pDialogFlow->AddDialog(IDD_00502_EXCHANGE_BACKUP_DLG, new CExchangeBackupDlg(this));

    // ���ó˿ͻ���
    m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));
    m_pDialogFlow->AddDialog(IDD_10501_EXCHANGE_DLG, new CExchangeReceptionDlg(this));

    // ���û������
    // �ȴ���������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_PUT_CARD));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_10002_WELCOME_DLG);

    // �滻��������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_CONFIRM), 
                                                     theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_CONFIRM_PASSENGER), 
                                                     theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_CONFIRM_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_00501_EXCHANGE_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_10501_EXCHANGE_DLG);

    // �滻��������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_FINISH),
                                                       theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_FINISH_PASSENGER), 
                                                       theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_FINISH_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_00501_EXCHANGE_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_10501_EXCHANGE_DLG);

    //// �滻���ݻ���
    //m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BACKUP, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_BACKUP),
    //                                                   theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_CONFIRM_PASSENGER), 
    //                                                   theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_CONFIRM_PASSENGER));
    //m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BACKUP,IDD_00502_EXCHANGE_BACKUP_DLG);
    //m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BACKUP,IDD_10501_EXCHANGE_DLG);

    //// �滻���ݽ�������
    //m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BACKUP_FINISH, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_FINISH), 
    //                                                          theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_FINISH_PASSENGER), 
    //                                                          theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_FINISH_PASSENGER));
    //m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BACKUP_FINISH,IDD_00502_EXCHANGE_BACKUP_DLG);
    //m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BACKUP_FINISH,IDD_10501_EXCHANGE_DLG);

    // �滻�����������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_EXCHANGE_NOT, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_NOT), 
                                                              theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_NOT_PASSENGER), 
                                                              theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_NOT_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_EXCHANGE_NOT,IDD_00501_EXCHANGE_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_EXCHANGE_NOT,IDD_10501_EXCHANGE_DLG);

    // ���û�����ת
    m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_EXCHANGE_BASE_DLG, DIALOG_GROUP_BASE);
    m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_EXCHANGE_NOT_DLG,DIALOG_GROUP_EXCHANGE_NOT);

    m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_EXCHANGE_FINISH_DLG,DIALOG_GROUP_FINISH);
    m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_EXCHANGE_WAIT_DLG,  DIALOG_GROUP_WAIT);
    m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_EXCHANGE_NOT_DLG,DIALOG_GROUP_EXCHANGE_NOT);

    //m_pDialogFlow->AddFlow(DIALOG_GROUP_BACKUP, WM_TO_EXCHANGE_NOT_DLG,DIALOG_GROUP_EXCHANGE_NOT);
    //m_pDialogFlow->AddFlow(DIALOG_GROUP_BACKUP, WM_TO_EXCHANGE_FINISH_DLG,DIALOG_GROUP_BACKUP_FINISH);
    //m_pDialogFlow->AddFlow(DIALOG_GROUP_BACKUP, WM_TO_EXCHANGE_WAIT_DLG,DIALOG_GROUP_WAIT);
    //


    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);          // ��ת���滻�ȴ�����
 
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ö��¿�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeSvc::StartReadCard(CString guidInfo)
{
	RW_READ_CARD_REQUEST cmdReadCard;
	cmdReadCard.FunctionCode = FUNCTION_CODE_EXCHANGE_NEW;
	cmdReadCard.IsCheck = true;
	cmdReadCard.IsReadRecords = false;
	cmdReadCard.IsPaidArea = false;
	if(guidInfo == ""){
		INT_PTR ret =CMessageBox::Show(_T(""),EXCHANGE_NEWCARD_DIALOG,CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
	}
	else{
		guidInfo += EXCHANGE_NEWCARD_DIALOG2;
		INT_PTR ret =CMessageBox::Show(_T(""),guidInfo,CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
	}
	m_pRWHelper->ReadCard(cmdReadCard);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeSvc::~CExchangeSvc()
{
	delete pUlCard;
	pUlCard = NULL;
	delete pCpuCard;
	pCpuCard = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ����Ʊ������Ϣ

@param      void
@retval     void
@exception  void
*/
//////////////////////////////////////////////////////////////////////////
UINT CExchangeSvc::InitExchangeInfo()
{
	// Ʊ�濨��
	UINT i = 0;
	m_ExchangeInfo[i++].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ��Ʊ����
	m_ExchangeInfo[i++].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ��Ч��
	m_ExchangeInfo[i++].nameLeft = add_wsp(CString(TXT_TICKET_VALIDATE_DATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ������� 	
	m_ExchangeInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_ExchangeInfo[i++].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	return i;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ó˿ͻ�����Ϣ

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
UINT CExchangeSvc::SetReceptionInfo()
{
	// ��Ʊ���
	m_TicketInfoGroup.clear();
	MSG_NAME_GROUP msg_tmp;
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_NUMBER,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_NUMBER_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(m_cardInfo.cardNum,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	m_TicketInfoGroup.push_back(msg_tmp);

	// Ʊ������
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(m_cardInfo.cardNameCn,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(m_cardInfo.cardNameEn,MAX_CASH_RIGHT_LEN);
	m_TicketInfoGroup.push_back(msg_tmp);
	// ��Ч��
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_VALIDATE_DATE,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_VALIDATE_DATE_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(m_cardInfo.vilidateTime,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	m_TicketInfoGroup.push_back(msg_tmp);
	// Ʊ�����
	if(m_category==MULTIRIDE) {
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_RIDES,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
	}else{
		msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_BALANCE,MAX_SBC_CASE_LEN);
		msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
	}
	msg_tmp.nameRight.nameChinese = add_hsp_right(m_cardInfo.balance,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	m_TicketInfoGroup.push_back(msg_tmp);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���س˿ͻ�����Ϣ

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
vector<MSG_NAME_GROUP>& CExchangeSvc::GetReceptionInfo()
{
	return	m_TicketInfoGroup;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����վԱ����

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeSvc::SetBaseDlg()
{
	try{
		// Ʊ�濨��
		UINT i = 0;
		m_ExchangeInfo[i++].nameRight = m_cardInfo.cardNum;
		// ��Ʊ��������
		m_ExchangeInfo[i++].nameRight = m_cardInfo.cardNameCn;
		// ��Ч��
		m_ExchangeInfo[i++].nameRight = m_cardInfo.vilidateTime;
		// ������� 	
		if(m_category==MULTIRIDE) {
			m_ExchangeInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_RIDES) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		}else{
			m_ExchangeInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		}
		m_ExchangeInfo[i++].nameRight = m_cardInfo.balance;
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ó�Ʊ������Ϣ������

@param      void
@retval     void
@exception  void
*/
//////////////////////////////////////////////////////////////////////////
UINT CExchangeSvc::GetExchangeInfoSize()
{
	return m_ExchangeInfoSize;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ó�Ʊ������Ϣ

@param      void
@retval     void
@exception  void
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP CExchangeSvc::GetExchangeInfo()
{
	return m_ExchangeInfo;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ȡ��Ʊ��Ϣ

@param      (i)  UINT errCode              �������
@param      (i)  CRWReadCard* rwReadCard   ��������

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeSvc::OnReadCard(UINT errCode, CRWReadCard* rwReadCard)
{
	try{
		CMessageBox::End();
		RW_READ_CARD_RESPONSE cardRspInfo ;
		rwReadCard->GetRWResponse(&cardRspInfo);  
		CString guideInfo =_T("");
		if(RW_RSP_OK != errCode){
			// ��������
			if(errCode == RW_RSP_ECT_CARD_BLACKLIST){
				RW_BUSSINESS_RESPONSE response;
				errCode = BlockCard(response); 
				if (errCode != RW_RSP_OK){
					guideInfo = m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode);				
				} 
				else{
					guideInfo =theGUIDE_INFO.GetPassengerMessageCn(GUIDE_BLACK);				
				}
			} 
			// ����ȡ��
			else if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD){	
				guideInfo = theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL);
			}
			// ��������
			else{
				// �ɿ�
				if(m_isOldCard){	
					// ת�����ɻ���
					theSERVICE_MGR.SetState(FINISH);
					DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG); 	
					guideInfo = m_pRWHelper->GetTpuErrorInfo(cardRspInfo.DetailErrorCode);
				}
				// �¿�
				else{
					StartReadCard(guideInfo);
				}			
			}
			theAPP_SESSION.ShowOfficeGuide(guideInfo);		
		}		
		// �����ɹ�
		if (RW_RSP_OK == errCode) {
			// ��ö�����Ϣ
			if (SaveCardInfo(cardRspInfo))
			{
				// ����վԱ����
				SetBaseDlg();
				// ���ó˿ͻ���
				SetReceptionInfo();
				if(m_isOldCard){
					// ��ת����������
					DoDialogFlow(WM_TO_EXCHANGE_BASE_DLG); 	
					guideInfo =  theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_CONFIRM); 
				}
				// �¿�
				else{
					DoNewCardExchange();
				}
			}			
		}
		if(guideInfo != ""){
			theAPP_SESSION.ShowOfficeGuide(guideInfo);	
		}
	}
	// �쳣����
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CExchangeSvc::BlockCard(RW_BUSSINESS_RESPONSE response)
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
/*
@brief      ���濨����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CExchangeSvc::SaveCardInfo(RW_READ_CARD_RESPONSE& RWReadCardResponse)
{
	// ULƱ��
	if(RWReadCardResponse.CardPHType == ULTRALIGHT) {
		// ����UL������
		pUlCard->GetCardInfo(RWReadCardResponse.CardInfo);
		// Ʊ��������
		m_TicketType = pUlCard->applicationData.CardAppMainType;
		// ���߼����
		m_cardInfo.cardNum.Format(_T("%.9u"),pUlCard->applicationData.CardLogicalNumber);
		if (m_isOldCard)
		{
			m_strOldNum = m_cardInfo.cardNum;
		}else
		{
			m_strNewNum = m_cardInfo.cardNum;
		}
		// Ʊ������
		CAccTicketParam::TICKET_TYPE ticketTypeDetail;
		theACC_TICKET.GetTicketInfo(m_TicketType,ticketTypeDetail);
		m_cardInfo.cardNameCn = ticketTypeDetail.ticketTypeName_cn;
		m_cardInfo.cardNameEn = ticketTypeDetail.ticketTypeName_en;
		m_cardInfo.vilidateTime = pUlCard->applicationData.ValidEndTime.ToString(_T("%.4d��%.2d��%.2d��"));	

		// ��Ʒ����
		theACC_TICKET.GetTicketCategry(m_TicketType,m_category);
		// �ƴ�
		if(m_category==MULTIRIDE) {
			m_cardInfo.balance.Format(_T("%d��"),pUlCard->applicationData.Balance);
			m_strDeposit.Format(_T("%d��"),pUlCard->applicationData.Deposit);
		}
		// Ǯ�� 
		else if(m_category==PURSE){
			m_cardInfo.balance = ComFormatAmount(pUlCard->applicationData.Balance)+MONEY_UNIT;
			m_strDeposit = ComFormatAmount(pUlCard->applicationData.Deposit)+MONEY_UNIT;
		}
		// ����
		else if(m_category==PERIOD){
			m_cardInfo.balance = TXT_NOTHING;
		}
		else{
			// ��ת����������
			DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG); 	
			return false;
		}
		
	}
	// CPU��
	else if (RWReadCardResponse.CardPHType == CPU_TICKET){		
		// ����UL������
		pCpuCard->GetCardInfo(RWReadCardResponse.CardInfoLen,RWReadCardResponse.CardInfo);
		// Ʊ��������
		m_TicketType = pCpuCard->applicationData.CardAppMainType;
		// ���߼����
		m_cardInfo.cardNum.Format(_T("%.9u"),pCpuCard->applicationData.CardLogicalNumber);
		if (m_isOldCard)
		{
			m_strOldNum = m_cardInfo.cardNum;
		}else
		{
			m_strNewNum = m_cardInfo.cardNum;
		}
		m_cardInfo.vilidateTime = pCpuCard->applicationData.ValidDate.ToString(_T("%.4d��%.2d��%.2d��"));	
		// Ʊ������
		CAccTicketParam::TICKET_TYPE ticketTypeDetail;
		theACC_TICKET.GetTicketInfo(m_TicketType,ticketTypeDetail);
		m_cardInfo.cardNameCn = ticketTypeDetail.ticketTypeName_cn;
		m_cardInfo.cardNameEn = ticketTypeDetail.ticketTypeName_en;
		// ��Ʒ����
		theACC_TICKET.GetTicketCategry(m_TicketType,m_category);
		// �ƴ�
		if(m_category==MULTIRIDE) {
			m_cardInfo.balance.Format(_T("%d��"),pCpuCard->applicationData.Balance);
			m_strDeposit.Format(_T("%d��"),pCpuCard->applicationData.Deposit);
		}
		// Ǯ�� 
		else if(m_category==PERIOD){
			m_cardInfo.balance = ComFormatAmount(pCpuCard->applicationData.Balance)+MONEY_UNIT;
			m_strDeposit = ComFormatAmount(pCpuCard->applicationData.Deposit)+MONEY_UNIT;
		}
		// ����
		else if(m_category==PERIOD){
			m_cardInfo.balance = TXT_NOTHING;
		}
		else{
			// ��ת����������
			DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG); 	
			return false;
		}
	}
	else
	{
		// ��ת����������
		DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG); 	
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      һ��ͨ��ֵ����(��ֵ) 

@param      ��

@retval  	0:����ɹ�  ����:ʧ����Ϣ

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeSvc::DoOldCardExchange()
{
	try{
		//д��
		RW_CMD_REPLACE_CARD replaceInput;
		replaceInput.Serialnum = theCOUNT_INFO.GetUDSNForDisplay();
		replaceInput.TranDateTime = ComGetCurTime();
		
		RW_BUSSINESS_RESPONSE response;
		long errCode = m_pRWHelper->ReplaceOldCard(replaceInput,response);
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
				m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,m_TicketType);
			}

			// sc���
			SaveSCData();

			m_isOldCard = false;
			// ���¿�
			StartReadCard();			
		}
		else{
			// ��ת��������
			DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG);   
			theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode));
		}
	}
	// �쳣����
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

@param      ��

@retval  	long         ������Ϣ
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CExchangeSvc::DoNewCardExchange()
{
	try{
		//д��
		RW_CMD_REPLACE_CARD replaceInput;
		replaceInput.Serialnum = theCOUNT_INFO.GetUDSNForDisplay() ;
		replaceInput.TranDateTime = ComGetCurTime();

		RW_BUSSINESS_RESPONSE response;
		long errCode = m_pRWHelper->ReplaceNewCard(replaceInput,response);
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
				m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,m_TicketType);
			}

			// sc���
			SaveSCData();
			
			//��ӡ
			//if (!IsPrinterException())
			//{
				PrinterPrint();  
			//}
			
			// ��ת����������
			DoDialogFlow(WM_TO_EXCHANGE_FINISH_DLG);    
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_FINISH)); 
		}
		else{
			// �������¿�
			StartReadCard(m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode));
		}
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ��ͨsc����

@param      ��

@retval  	long         ������Ϣ
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeSvc::SaveSCData()
{
	SCAuditKey purchaseKey;
	purchaseKey.serviceID = EXCHANGE_SVC;
	purchaseKey.cardIssuancer = ACC;
	purchaseKey.ticketType = m_TicketType;

	SCAuditValue purchaseValue;
	purchaseValue.transCount = 1;
	purchaseValue.recyleCardNum = 1;
	purchaseValue.cardCostAmount = 0;
	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �жϴ�ӡ���Ƿ�����

@param      ��

@retval     bool true:  false:�쳣����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//bool CExchangeSvc::IsPrinterException() 
//{
//	return m_pPRTHelper->IsPrinterHasException();
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ���ݡ���Ǯ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeSvc::PrinterPrint()
{
	try{
		CString templateName= _T("EXCHANGE.template");
		CStringArray printArr;
		for(int i=0;i<theBOM_INFO.GetReceiptPrintTimes(_T("EXCHANGE"));i++)
		{
			CStringArray printTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
			_DATE_TIME transDateTime = ComGetCurTime();
			for(int i=0;i<printTexts.GetCount();i++)
			{
				CString& line = printTexts.ElementAt(i);
				line.Replace(_T("{USER_ID}"),theAPP_SESSION.GetUserInfo().sUserID);
				line.Replace(_T("{OLD_NUM}"),m_strOldNum);
				line.Replace(_T("{NEW_NUM}"),m_strNewNum);
				line.Replace(_T("{TICKET_TYPE}"),m_cardInfo.cardNameCn);
				line.Replace(_T("{STATION_NAME}"),theAPP_SESSION.GetStationNameCN());
				line.Replace(_T("{DEVICE_CODE}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());
				line.Replace(_T("{DEPOSIE}"),m_strDeposit);			
				line.Replace(_T("{BALANCE}"),m_cardInfo.balance);			
				line.Replace(_T("{VALID_DATE}"),m_cardInfo.vilidateTime);
				line.Replace(_T("{TRANS_TIME}"),transDateTime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
			
				// �ƴ�
				if(m_category==MULTIRIDE) {
					line.Replace(_T("�������"),_T("�������"));
				}
				// ���� 
				else if(m_category==PERIOD){
					if (line.Find(_T("�������")))
					{
						line.Empty();
					}
				}
				CStringArray printLine;
				ComPrintLineFormatForTemplate(line,printLine);
				printArr.Append(printLine);
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
