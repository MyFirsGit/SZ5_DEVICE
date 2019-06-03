#include "stdafx.h"
#include "IssueSvc.h"
#include "IssueACCDlg.h"
#include "IssueACCReceptionDlg.h"
#include "WaitingOfficeDlg.h"
#include "tickethelper.h"
#include "CAccTicketParam.h"
#include "issueresultdlg.h"
#include "WelcomeReceptionDlg.h"
#include "guideinfo.h"
#include <basetsd.h>
#include "TemplateHelper.h"
#include "setting.h"
// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_WELCOME_TO_INIT = _T("WM_WELCOME_TO_INIT");
static const TCHAR* const WM_WAIT_TO_INIT = _T("WM_WAIT_TO_INIT");
static const TCHAR* const WM_INIT_TO_FINISH = _T("WM_INIT_TO_FINISH");
static const TCHAR* const WM_INIT_TO_ERROR = _T("WM_INIT_TO_ERROR");
static const TCHAR* const WM_WAIT_TO_FINISH = _T("WM_WAIT_TO_FINISH");
static const TCHAR* const WM_WAIT_TO_ERROR = _T("WM_WAIT_TO_ERROR");

BEGIN_MESSAGE_MAP(CIssueSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CIssueSvc::CIssueSvc():CBOMForeService(ISSUE_SVC)
{	
	m_IssuedTickets.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CIssueSvc::~CIssueSvc()
{
	delete m_DataModel.pCPUCard;
	m_DataModel.pCPUCard = NULL;
	delete m_DataModel.pULCard;
	m_DataModel.pULCard = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ʱ��Ӧ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::OnStart()
{
	//m_pDialogFlow->AddDialog(IDD_00101_ISSUE_INIT_DLG,  new CIssueACCDlg(this));
	//m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WELCOME, theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_NEW_CARD_SELET_INFO));
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WELCOME, IDD_00101_ISSUE_INIT_DLG);
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WELCOME, IDD_10002_WELCOME_DLG);

	m_pDialogFlow->AddDialog(IDD_00101_ISSUE_INIT_DLG,  new CIssueACCDlg(this));
	m_pDialogFlow->AddDialog(IDD_10101_ISSUE_DLG, new CIssueACCReceptionDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_INIT, theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_NEW_CARD_SELET_INFO),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_BASE_CONFIRM_UL),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_BASE_CONFIRM_UL));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INIT, IDD_00101_ISSUE_INIT_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INIT,IDD_10101_ISSUE_DLG);

	m_pDialogFlow->AddDialog(IDD_00103_ISSUE_RESULT_DLG,new CIssueResultDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH,theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_FINISH),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ISSUE_FINISH_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ISSUE_FINISH_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_00103_ISSUE_RESULT_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_10101_ISSUE_DLG);
	
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR,/*theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_ERROR)*/_T(""),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ISSUE_ERROR_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ISSUE_ERROR_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR,IDD_00103_ISSUE_RESULT_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR,IDD_10101_ISSUE_DLG);

	m_pDialogFlow->AddFlow(DIALOG_GROUP_WELCOME,WM_WELCOME_TO_INIT,DIALOG_GROUP_INIT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_FINISH,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_ERROR,DIALOG_GROUP_ERROR);

	// �ⲿ����Ʊ��
	if(m_DataModel.issueWay == CTicketHelper::BY_OUTER_TPU){
		m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
		m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));
		m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT,theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_PUT_CARD));
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
		m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_10002_WELCOME_DLG);
		m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_INIT,DIALOG_GROUP_INIT);
		m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_FINISH,DIALOG_GROUP_FINISH);
		m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_ERROR,DIALOG_GROUP_ERROR);
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
	}
	// �ڲ���д������Ʊ��
	else{
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_INIT);
	}

	__super::OnStart();

}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ʊ������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetTicketType(TicketType_t& ticketType)
{
	m_DataModel.ticketType = ticketType;
	m_DataModel.rwIssueRequest.ProductType = ticketType;
	m_DataModel.ticketPHType = theTICKET_HELPER.GetMediumType(ticketType);
	m_DataModel.issueWay = theTICKET_HELPER.GetIssueWay(ticketType);
	theACC_TICKET.GetTicketInfo(ticketType,m_DataModel.ticketTypeDetail);
	theACC_TICKET.GetTicketCategry(ticketType,m_DataModel.cardCategory);
	m_DataModel.deposit = m_DataModel.ticketTypeDetail.deposit;
	// ��վƱ
	if(ticketType == TICKET_EXIT_ONLY){
		m_DataModel.rwIssueRequest.SaleMode = BY_PRICE;
		m_DataModel.rwIssueRequest.EndStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
	}
	// ����Ʊ��
	else{
		m_DataModel.rwIssueRequest.EndStation = 0;
	}
	
	if(m_DataModel.issueWay == CTicketHelper::BY_INNER_TPU ||  m_DataModel.issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
		m_DataModel.rwIssueRequest.BegainStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����Ƿ���Ԥ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetIsPreSell(bool isPresell)
{
	m_DataModel.isPresell = isPresell;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��Service������ģ��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CIssueSvc::Model& CIssueSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ģ�͸ı��¼�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::NotifyDataModelChanged()
{
	if(GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_WELCOME){
		return;
	}
	TicketType_t ticketType = m_DataModel.ticketType;
	if(m_DataModel.issueWay == CTicketHelper::BY_INNER_TPU || m_DataModel.issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
		m_DataModel.due = m_DataModel.rwIssueRequest.SaleAmount * m_DataModel.piece;
		m_DataModel.dueOfSuccess = m_DataModel.rwIssueRequest.SaleAmount * m_DataModel.issueSuccessPiece;
	}
	if(m_DataModel.issueWay == CTicketHelper::BY_OUTER_TPU)
	{
		m_DataModel.due = (m_DataModel.rwIssueRequest.SaleAmount + m_DataModel.deposit)* m_DataModel.piece;
		m_DataModel.dueOfSuccess = (m_DataModel.rwIssueRequest.SaleAmount + m_DataModel.deposit)*m_DataModel.issueSuccessPiece;
	}
	if(m_DataModel.paid != 0){
		m_DataModel.change = m_DataModel.paid - m_DataModel.due;
		m_DataModel.changeOfSuccess = m_DataModel.paid - m_DataModel.dueOfSuccess;
	}
	else{
		m_DataModel.change = 0;
		m_DataModel.changeOfSuccess = 0;
	}
	__super::NotifyDataModelChanged();
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::OnOK()
{
	try{		
		CString confirmMessage=_T("");
		confirmMessage.Format(_T("��Ʊ%d��,�ܼ�%sԪ���Ƿ�ȷ��"),m_DataModel.piece,ComFormatAmount(m_DataModel.due));
		INT_PTR confirmResult = CMessageBox::Show(STR_ISSUE_INFO,confirmMessage,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		if (confirmResult == IDCANCEL){
			return;
		}
		if(m_DataModel.issueWay == CTicketHelper::BY_INNER_TPU && m_LPTHelper->GetTicketboxDoorStatus()!=DOORSTATUS::CLOSED)
		{
			CMessageBox::Show(STR_ISSUE_INFO,_T("���ȹرճ�Ʊ����"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
			return;
		}
		theSERVICE_MGR.SetState(ISSUEING);
		m_DataModel.issueSuccessPiece = 0;
		//TH���ˣ�ʹ���ⲿ��д������
		//if(m_DataModel.issueWay == CTicketHelper::BY_INNER_TPU && theSETTING.GetTHMode()==TH_MODE_ABNORMAL){			
		//	StartReadCard();
		//	return;
		//}
		// ʹ���ڲ���д������
		if(m_DataModel.issueWay == CTicketHelper::BY_INNER_TPU ){
			if((theCOUNT_INFO.GetTicketboxAInfo().ulCurCount+theCOUNT_INFO.GetTicketboxBInfo().ulCurCount)<m_DataModel.piece)
			{
				confirmMessage.Format(_T("Ʊ���г�Ʊ��������%d�ţ��Ƿ������"),m_DataModel.piece);
				INT_PTR confirmResult = CMessageBox::Show(STR_ISSUE_INFO,confirmMessage,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
				if (confirmResult == IDCANCEL){
					theSERVICE_MGR.SetState(RECEPT);
					return;
				}
			}
			while(m_DataModel.issueSuccessPiece < m_DataModel.piece){
				CString strCurrentIssue;
				strCurrentIssue.Format(_T("��%d�ţ���%d��"),m_DataModel.issueSuccessPiece+1,m_DataModel.piece);
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUING) + strCurrentIssue);
				theAPP_SESSION.ShowReceptionGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ISSUING_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ISSUING_PASSENGER));
				bool success =   TWIssueOneCard();
				if(success)
				{
					m_DataModel.issueSuccessPiece++;
					// �����ֽ���Ϣ
					NotifyDataModelChanged();
				}
				else
				{
					//SyncTicketCountToTH();
					// ��Ǯ��
					m_pPRTHelper->PrinterOpenBox();
					return;
				}
			}
			//SyncTicketCountToTH();
			// ��Ǯ��
			m_pPRTHelper->PrinterOpenBox();
			theSERVICE_MGR.SetState(FINISH);
			m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_FINISH);
			return;
		}
		// �ⲿ��д������TH����Ʊ��δ������
		else if( m_DataModel.issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
			StartReadCard();
		}
		// �ⲿ��д�����ۣ��Ѿ��ڿ�ʼʱ��ɶ�����������CPU����
		else{
			bool success = RWIssueOneCard();
			if(success){
				m_DataModel.issueSuccessPiece ++;
				// �����ֽ���Ϣ
				NotifyDataModelChanged();
			}
			else{
				
				return;
			}
			//if (!m_pPRTHelper->IsPrinterHasException()){
				PrinterPrint(); //��ӡ���
			////}
			theSERVICE_MGR.SetState(FINISH);
			m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_FINISH);
		}
		
	}
	catch (CSysException& e)
	{
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
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
long CIssueSvc::IssueCheck(bool checkDo)
{
	RW_CMD_SETCHECK chargeOverTime;
	chargeOverTime.byDo = checkDo;
	chargeOverTime.TranDateTime = ComGetCurTime();
	RW_CMD_RESP_SETCHECK response; 
	long initCheckResult = m_pRWHelper->IssueInitCheckProcess(chargeOverTime,response);
	if(initCheckResult != RW_RSP_OK)
	{
		return response.DetailErrCode;
	}
	PrinterPrint();	
	theSERVICE_MGR.SetState(FINISH);
	m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_FINISH); 
	return RW_RSP_OK;

}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CIssueSvc::BlockCard()
{
	UINT resultCode;
	RW_CMD_BLOCK rw_cmd_block;
	rw_cmd_block.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
	rw_cmd_block.TranDateTime = ComGetCurTime();
	RW_BUSSINESS_RESPONSE response;
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
@brief      ������Ʊ��TPU��Ϣ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetIssueParameter()
{
	RW_ISSUE_REQUEST& rwIssueRequest =  m_DataModel.rwIssueRequest;
	TicketType_t ticketType = m_DataModel.ticketType;
	rwIssueRequest.PaymentMethod=BY_CASH;
	rwIssueRequest.SerialNo =  theCOUNT_INFO.GetUDSNForDisplay();
	rwIssueRequest.TranDateTime = ComGetCurTime();
	rwIssueRequest.CardType = theTICKET_HELPER.GetCardType(m_DataModel.rwReadCardResponse.CardPHType);
	rwIssueRequest.ProductyCategory =m_DataModel.cardCategory;
	rwIssueRequest.ProductType = ticketType;
	rwIssueRequest.ProductSubType = 0;
	rwIssueRequest.PassengerType = 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  �������Ĵ���    

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::OnFindCard(RW_READ_CARD_RESPONSE& rwReadCardResponse)
{
	if(m_DataModel.ticketPHType != RESERVE && m_DataModel.ticketPHType != rwReadCardResponse.CardPHType){
		OnError(_T("�����Ʊ������"));
		return false;
	}
	delete m_DataModel.pCPUCard;
	m_DataModel.pCPUCard = NULL;
	delete m_DataModel.pULCard;
	m_DataModel.pULCard = NULL;
	if(ULTRALIGHT == rwReadCardResponse.CardPHType){
		m_DataModel.pULCard = new CULCard;
		m_DataModel.pULCard->GetCardInfo(rwReadCardResponse.CardInfo);
	}
	else if(CPU_TICKET == rwReadCardResponse.CardPHType){
		m_DataModel.pCPUCard = new CCPUCard;
		m_DataModel.pCPUCard->GetCardInfo(rwReadCardResponse.CardInfoLen,rwReadCardResponse.CardInfo);
	}
	else{
		OnError(_T("�������������"));
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ҵ������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SaveBusinessData(RW_BUSSINESS_RESPONSE& rwBussinessResponse)
{
	RW_ISSUE_REQUEST &rwIssueRequest = m_DataModel.rwIssueRequest;
	theCOUNT_INFO.UpdateUDSN(rwBussinessResponse.SerialAddnum);
	DWORD ticketID = 0;
	if(m_DataModel.pULCard!=NULL)ticketID=m_DataModel.pULCard->applicationData.CardLogicalNumber;
	if(m_DataModel.pCPUCard!=NULL)ticketID=m_DataModel.pCPUCard->applicationData.CardLogicalNumber;
	if(m_DataModel.isPresell)
	{
		vector<CCTLOGPresellTicketInfo::PRESELL_TICKET_ID> vecTicketID;
		CCTLOGPresellTicketInfo::PRESELL_TICKET_ID preTicketID;
		preTicketID.presellId = ticketID;
		vecTicketID.push_back(preTicketID);
		thePRE_TK.UpdatePresellTicketInfo(vecTicketID);
	}
	m_IssuedTickets.push_back(ticketID);
	if(rwBussinessResponse.UDlen>0)
	{
		m_pRWHelper->SaveRWUDData(rwBussinessResponse.UDlen,rwBussinessResponse.UDData);
	}
	if(rwBussinessResponse.ARLen>0)
	{
		m_pRWHelper->SaveRWARData(rwBussinessResponse.ARLen,rwBussinessResponse.ARData,m_DataModel.ticketType);
	}
	
	if(m_DataModel.ticketType!=TICKET_YKT_STORE_VALUE)
	{
		SCAuditKey purchaseKey;
		purchaseKey.serviceID = ISSUE_SVC;
		purchaseKey.ticketType = m_DataModel.ticketType;
		purchaseKey.cardIssuancer = theTICKET_HELPER.GetCardIssuancer(m_DataModel.ticketType);

		SCAuditValue purchaseValue;
		purchaseValue.transCount = 1;
		purchaseValue.transAmount = m_DataModel.rwIssueRequest.SaleAmount;
		purchaseValue.cardCostAmount =0;
		purchaseValue.depositAmount =m_DataModel.deposit;
		purchaseValue.recyleCardNum = 0;

		theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
	}
	else
	{
		SCAuditKey purchaseKey;
		purchaseKey.serviceID = ISSUE_SVC;
		purchaseKey.ticketType = m_DataModel.ticketType;
		purchaseKey.cardIssuancer = theTICKET_HELPER.GetCardIssuancer(m_DataModel.ticketType);

		SCAuditValue purchaseValue;
		purchaseValue.transCount = 1;
		purchaseValue.transAmount = 0;
		purchaseValue.cardCostAmount =0;
		purchaseValue.depositAmount =m_DataModel.deposit;
		purchaseValue.recyleCardNum = 0;

		theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);

		SCAuditKey chargePurchaseKey;
		chargePurchaseKey.serviceID = CHARGE_SVC;
		chargePurchaseKey.ticketType = m_DataModel.ticketType;
		chargePurchaseKey.cardIssuancer = theTICKET_HELPER.GetCardIssuancer(m_DataModel.ticketType);

		SCAuditValue chargePurchaseValue;
		chargePurchaseValue.transCount = 1;
		chargePurchaseValue.transAmount = m_DataModel.rwIssueRequest.SaleAmount;
		chargePurchaseValue.cardCostAmount =0;
		chargePurchaseValue.depositAmount =0;
		chargePurchaseValue.recyleCardNum = 0;

		theSCAudit_MGR.AddLoginPurchase(chargePurchaseKey,chargePurchaseValue);
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
bool CIssueSvc::RWIssueOneCard()
{
	SetIssueParameter();
	RW_BUSSINESS_RESPONSE rwBusinessResponse;
	long saleResult =   m_pRWHelper->Issue(m_DataModel.rwIssueRequest,rwBusinessResponse) ;
	if(saleResult!=RW_RSP_OK){
		OnError( m_pRWHelper->GetTpuErrorInfo(rwBusinessResponse.DetailErrorCode));
		return false;
	}
	SaveBusinessData(rwBusinessResponse);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::StartReadCard()
{
	m_pRWHelper->ReadCard(true);
	theSERVICE_MGR.SetState(ISSUEING);
	INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)this->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
	if(ret == IDCANCEL){
		m_pRWHelper->StopReadCard();
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����Ļص�����

@param      UINT resultCode �������
@param      CRWReadCard* rwReadCardCommand ��������

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::OnReadCard(UINT resultCode,CRWReadCard* rwReadCardCommand)
{
	try{
		CMessageBox::End();
		if(GetCurDialogGroupID() == DIALOG_GROUP_FINISH || GetCurDialogGroupID() == DIALOG_GROUP_ERROR)
		{
			return;
		}
		rwReadCardCommand->GetRWResponse(&m_DataModel.rwReadCardResponse);
		// ������Ʊ��
		if(resultCode == RW_RSP_ACC_CARD_BLACKLIST){
			long blockCardResult = BlockCard(); 
			CString tips = blockCardResult == RW_RSP_OK ? CString("����������������" ): m_pRWHelper->GetTpuErrorInfo(resultCode);
			OnError(tips);
		}
		// �Ǻ�����Ʊ��
		else{		
			// ����ʧ��
			if(resultCode!=RW_RSP_OK){
				// ��������
				if(m_DataModel.rwReadCardResponse.DetailErrorCode != RW_ERROR_CARD_NO_CARD){
					OnError(m_pRWHelper->GetTpuErrorInfo(m_DataModel.rwReadCardResponse.DetailErrorCode));
				}
				// ��������ȡ��
				else{
					OnError(_T(""));
					theAPP_SESSION.ShowAllGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CANCEL));	
				}
			}
			// �����ɹ�
			else{
				// ����Ʊ�����ݳɹ�
				if(OnFindCard(m_DataModel.rwReadCardResponse)){
					// �ⲿ��д���ۿ�
					if(m_DataModel.issueWay == CTicketHelper::BY_OUTER_TPU || m_DataModel.issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
						// ULƱ��
						if(m_DataModel.rwReadCardResponse.CardPHType == ULTRALIGHT){
							long ticketNumber = 0;
							//if(m_DataModel.pCPUCard!=NULL){
							//	ticketNumber = m_DataModel.pCPUCard->applicationData.CardLogicalNumber;
							//}
							//else if(m_DataModel.pULCard!=NULL){
								ticketNumber = m_DataModel.pULCard->applicationData.CardLogicalNumber;
							//}
							// Ʊ��������
							if(find(m_IssuedTickets.begin(),m_IssuedTickets.end(),ticketNumber) != m_IssuedTickets.end()){
								CMessageBox::Show(theAPP_SESSION.GetServiceNameCH(ISSUE_SVC),_T("�˿��Ѿ����ۣ��뻻һ�ſ��������ۣ�"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_3);
								StartReadCard();								
							}
							else{
								// �ۿ��ɹ�
								if(RWIssueOneCard()){
									// ��Ʊ�ɹ�������1
									m_DataModel.issueSuccessPiece +=1;
									// �����ֽ���Ϣ
									NotifyDataModelChanged();
									// �������
									if(m_DataModel.issueSuccessPiece == m_DataModel.piece){
										m_IssuedTickets.clear();
										// ��Ǯ��
										m_pPRTHelper->PrinterOpenBox();
										theSERVICE_MGR.SetState(FINISH);
										m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_FINISH);										
									}
									// δ�������
									else{
										CString strPreviouseSuccessIssue;
										strPreviouseSuccessIssue.Format(_T("��%d�ŷ��۳ɹ���"),m_DataModel.issueSuccessPiece);
										CMessageBox::Show(theAPP_SESSION.GetServiceNameCH(ISSUE_SVC),strPreviouseSuccessIssue,CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_3);
										CString strCurrentIssue;
										strCurrentIssue.Format(_T("��%d�ţ���%d��"),m_DataModel.issueSuccessPiece+1,m_DataModel.piece);
										theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUING) + strCurrentIssue);
										StartReadCard();										
									}
								}								
							}							
						}
						// CPUƱ��
						else{
							if(m_DataModel.pCPUCard != NULL && m_DataModel.pCPUCard->applicationData.CardAppMainType!=m_DataModel.ticketType){
								OnError(_T("����Ŀ����ͣ�"));
								return;
							}
							DoDialogFlow(WM_WAIT_TO_INIT);
							// �����ֽ���Ϣ
							NotifyDataModelChanged();
						}
					}
				}
			}
		}		
	}
	catch (CSysException& e){
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ڲ���д����һ�ſ�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::TWIssueOneCard()
{
	theSERVICE_MGR.SetState(ISSUEING);
	CAfcBomOperationParam::TPU_PARAM tpuParam;
	theAFC_BOM_OPERATION.GetTpuParam(tpuParam);

	int recycleCardCount = 0;
	int deleteBoxACount = 0;
	int deleteBoxBCount = 0;
	int maxAllowErrorCount =  tpuParam.maxWriteErrorCnt == 0 ? 0x7FFFFFFF : tpuParam.maxWriteErrorCnt;
	while(recycleCardCount<maxAllowErrorCount)
	{
		//1. 
		CCTLOGCountInfo::TICKETBOX_INFO& ticketboxAInfo = theCOUNT_INFO.GetTicketboxAInfo();
		CCTLOGCountInfo::TICKETBOX_INFO& ticketboxBInfo = theCOUNT_INFO.GetTicketboxBInfo();
		//LOG("--(1)---A״̬��%x��Ʊ��:%d-B״̬��%x��Ʊ��:%d---",
		//	theBOM_STATUS_MGR.GetMagANumStatus(),ticketboxAInfo.ulCurCount,
		//	theBOM_STATUS_MGR.GetMagBNumStatus(),ticketboxBInfo.ulCurCount);
		CAfcBomOperationParam::MAGAZINE_PARAM temParam;
		theAFC_BOM_OPERATION.GetMagazineParam(temParam);
		// A
		if (ticketboxAInfo.ulCurCount <= temParam.willEmptyValue && ticketboxAInfo.ulCurCount > temParam.haveEmptyValue && theBOM_STATUS_MGR.GetMagANumStatus() != MAG_NUM_NEAR_EMPTY)
		{
			theBOM_STATUS_MGR.SetMagANumStatus(MAG_NUM_NEAR_EMPTY);
			//LOG("--(2)---A���գ�Ʊ��:%d---",ticketboxAInfo.ulCurCount);
		} 
		else if (ticketboxAInfo.ulCurCount <= temParam.haveEmptyValue && theBOM_STATUS_MGR.GetMagANumStatus() != MAG_NUM_EMPTY)
		{
			theBOM_STATUS_MGR.SetMagANumStatus(MAG_NUM_EMPTY);
			//LOG("--(3)---A�գ�Ʊ��:%d---",ticketboxAInfo.ulCurCount);
		} 

		// B
		if (ticketboxBInfo.ulCurCount <= temParam.haveEmptyValue && ticketboxBInfo.ulCurCount > temParam.haveEmptyValue && theBOM_STATUS_MGR.GetMagBNumStatus() != MAG_NUM_NEAR_EMPTY)
		{
			theBOM_STATUS_MGR.SetMagBNumStatus(MAG_NUM_NEAR_EMPTY);
			//LOG("--(4)---B���գ�Ʊ��:%d---",ticketboxBInfo.ulCurCount);
		} 
		else if (ticketboxBInfo.ulCurCount <= temParam.haveEmptyValue && theBOM_STATUS_MGR.GetMagBNumStatus() != MAG_NUM_EMPTY)
		{
			theBOM_STATUS_MGR.SetMagBNumStatus(MAG_NUM_EMPTY);
			//LOG("--(5)---B�գ�Ʊ��:%d---",ticketboxBInfo.ulCurCount);
		} 

		TH_CMD_CARD_OUT boxNo = CMD_CARD_OUT_BOX_A;
		UINT uiTransNum = 0;
		if(/*ticketboxAInfo.ulCurCount != 0*/theBOM_STATUS_MGR.GetMagANumStatus() != MAG_NUM_EMPTY){
			boxNo = CMD_CARD_OUT_BOX_A;
		}
		else
		{
			if(/*ticketboxBInfo.ulCurCount != 0*/theBOM_STATUS_MGR.GetMagBNumStatus() != MAG_NUM_EMPTY){
				boxNo = CMD_CARD_OUT_BOX_B;
			}
			else{
				OnError(_T("Ʊ���ѿ�"));
				return false;
			}
		}
		//LOG("--(6)---Ʊ���:%d---",boxNo);

		long lRet = 0;
		try{
			//LOG("��ȡƱ����ʼ");
			long lRet = m_pTHHelper->CardOut(boxNo, &uiTransNum);
			//LOG("��ȡƱ������");
		}
		catch(CSysException& e){
			//LOG("��Ʊ���쳣������");
			theEXCEPTION_MGR.WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
			// Ʊ��1�ѿ�(A)
			if(e.GetDetailID() == CTHHardwareException::TH_ERROR_TICKET_BOX1_EMPTY){
				//	ticketboxAInfo.ulCurCount = 0;
				//	theCOUNT_INFO.SetTicketboxAInfo(ticketboxAInfo);
				theBOM_STATUS_MGR.SetMagANumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
				//LOG("--(7)---A�������գ�Ʊ��:%d---",ticketboxAInfo.ulCurCount);
				continue;
			}
			// Ʊ��2�ѿ�

			if(e.GetDetailID() == CTHHardwareException::TH_ERROR_TICKET_BOX2_EMPTY){
				//ticketboxBInfo.ulCurCount = 0;
				//theCOUNT_INFO.SetTicketboxBInfo(ticketboxBInfo);
				theBOM_STATUS_MGR.SetMagBNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
				//LOG("--(7)---B�������գ�Ʊ��:%d---",ticketboxBInfo.ulCurCount);
				continue;
			}
			else{
				// �����쳣���������쳣��Ϣ
				OnError(_T("����ʧ��"));
				return false;
			}
		}
		catch(...){
			//throw;
			OnError(_T("����ʧ��"));
			return false;
		}

		// ���Գ�Ʊ
		if(lRet != 0){
			++recycleCardCount;
			continue;
		}
		else
		{
			if (boxNo == CMD_CARD_OUT_BOX_A)
			{
				deleteBoxACount = 1;
			}
			else
			{
				deleteBoxBCount = 1;
			}
		}

		RW_BUSSINESS_RESPONSE saleCardResponse;
		long twSaleCardResult = -1;
		//LOG("��д����ʼ");
		twSaleCardResult = TWSaleCardFunc(saleCardResponse);
		//LOG("��д������");
		if(twSaleCardResult!=RW_RSP_OK){		
			// ����Ʊ��
			UINT uiRetractBoxNum = 0;
			//LOG("--(8)---����Ʊ����ʼ");
			m_pTHHelper->RetractCard(&uiRetractBoxNum);// ����Ʊ��
			//LOG("����Ʊ������");
			recycleCardCount ++;
			theTICKET_HELPER.UpdateTHCount(deleteBoxACount,deleteBoxBCount,1);                //���»��ճ�Ʊ����			
			if(recycleCardCount == maxAllowErrorCount){		
				OnError(_T("TH�ۼƴ��������������ֵ"));
				return false;
			}
			continue;
		}

		// ���潻�׼����
		SaveBusinessData(saleCardResponse);

		// �ų�Ʊ��
		//bool isRecycle = false;
		//SC_RSP_ISSUE_PROCESS releaseCardResponse;
		//m_pTHHelper->THIssueCard(releaseCardResponse,isRecycle);
		UINT uiRetractBoxNum = 0;
		//LOG("--(8)---�ų�Ʊ����ʼ");
		m_pTHHelper->SendCard(&uiRetractBoxNum);// ��Ʊ
		//LOG("�ų�Ʊ������");
		theTICKET_HELPER.UpdateTHCount(deleteBoxACount,deleteBoxBCount,0);                //���»��ճ�Ʊ����
		return true;
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
long CIssueSvc::TWSaleCardFunc(RW_BUSSINESS_RESPONSE& rwBusinessResponse)
{
	long issueResult = RW_RSP_OK;
	for(int saleCardRetryTimes = 0 ; saleCardRetryTimes<3;saleCardRetryTimes++)
	{
		long readCardResult  = RW_RSP_OK;
		for(int i=0;i<30;i++)
		{
			readCardResult = m_pTWHelper->TryReadCard(m_DataModel.rwReadCardResponse);
			if(readCardResult == RW_RSP_OK)
			{
				bool getCardInfoSuccess =OnFindCard(m_DataModel.rwReadCardResponse);
				if(!getCardInfoSuccess)
				{
					return RW_RSP_ACCERR;
				}
				break;
			}
		}
		if(readCardResult !=RW_RSP_OK )
		{
			return readCardResult;
		}
		SetIssueParameter();
		issueResult = m_pTWHelper->Issue(m_DataModel.rwIssueRequest,rwBusinessResponse);
		if(issueResult == RW_RSP_OK)
		{
			return issueResult;
		}
	}
	return issueResult;	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ����THƱ��״̬��Ʊ����� 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::UpdateTHCount(BYTE boxACount,BYTE boxBCount,BYTE collectionCount)
{
	CCTLOGCountInfo::TICKETBOX_INFO& ticketboxAInfo = theCOUNT_INFO.GetTicketboxAInfo();
	CCTLOGCountInfo::TICKETBOX_INFO& ticketboxBInfo = theCOUNT_INFO.GetTicketboxBInfo();
	CCTLOGCountInfo::TICKETBOX_INFO& ticketboxCInfo = theCOUNT_INFO.GetTicketboxCInfo();

	ticketboxAInfo.ulCurCount -= boxACount;
	ticketboxBInfo.ulCurCount -= boxBCount;
	ticketboxCInfo.ulCurCount += collectionCount;
	theCOUNT_INFO.SetTicketboxAInfo(ticketboxAInfo);
	theCOUNT_INFO.SetTicketboxBInfo(ticketboxBInfo);
	theCOUNT_INFO.SetTicketboxCInfo(ticketboxCInfo);

	CAfcBomOperationParam::MAGAZINE_PARAM magazineParam;
	theAFC_BOM_OPERATION.GetMagazineParam(magazineParam);
	MAGAZINE_NUM_STATUS ticketBoxAStatus = MAG_NUM_EMPTY;
	if(ticketboxAInfo.ulCurCount< magazineParam.haveEmptyValue)
	{
		ticketBoxAStatus = MAG_NUM_EMPTY;
	}
	else if (ticketboxAInfo.ulCurCount< magazineParam.willEmptyValue)
	{
		ticketBoxAStatus = MAG_NUM_NEAR_EMPTY;
	}
	else
	{
		ticketBoxAStatus = MAG_NUM_NORMAL;
	}
	if(theBOM_STATUS_MGR.GetMagANumStatus() != ticketBoxAStatus)
	{
		theBOM_STATUS_MGR.SetMagANumStatus(ticketBoxAStatus);
	}
	MAGAZINE_NUM_STATUS ticketBoxBStatus = MAG_NUM_EMPTY;
	if(ticketboxBInfo.ulCurCount< magazineParam.haveEmptyValue)
	{
		ticketBoxBStatus = MAG_NUM_EMPTY;
	}
	else if (ticketboxBInfo.ulCurCount< magazineParam.willEmptyValue)
	{
		ticketBoxBStatus = MAG_NUM_NEAR_EMPTY;
	}
	else
	{
		ticketBoxBStatus = MAG_NUM_NORMAL;
	}
	if(theBOM_STATUS_MGR.GetMagBNumStatus() != ticketBoxBStatus)
	{
		theBOM_STATUS_MGR.SetMagBNumStatus(ticketBoxBStatus);
	}
	MAGAZINE_NUM_STATUS collectionBoxStatus = MAG_NUM_NORMAL;
	if(ticketboxCInfo.ulCurCount>magazineParam.haveInvalidFullValue)
	{
		collectionBoxStatus = MAG_NUM_FULL;
	}
	else if(ticketboxCInfo.ulCurCount>magazineParam.willInvalidFullValue)
	{
		collectionBoxStatus = MAG_NUM_NEAR_FULL;
	}
	else
	{
		collectionBoxStatus = MAG_NUM_NORMAL;
	}
	if(theBOM_STATUS_MGR.GetCycleBoxNumStatus() != collectionBoxStatus)
	{
		theBOM_STATUS_MGR.SetCycleBoxNumStatus(collectionBoxStatus);
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
void CIssueSvc::OnError(CString errorMsg)
{
	m_IssuedTickets.clear();
	theSERVICE_MGR.SetState(FINISH);
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_WAIT){
		m_pDialogFlow->ShowNextDialogGroup(WM_WAIT_TO_ERROR);
	}
	else if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_INIT){
		m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_ERROR);
	}
	errorMsg.Replace(_T("���죩"),_T(""));
	errorMsg.Replace(_T("���ڣ�"),_T(""));
	errorMsg=_T( "���죩") +errorMsg;
	theAPP_SESSION.ShowOfficeGuide(errorMsg);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ͬ������Ʊ�������TH   

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SyncTicketCountToTH()
{
	//SC_TICKET_DATA ticketData;
	//ticketData.wMagazineANumber = theCOUNT_INFO.GetTicketboxAInfo().ulCurCount;
	//ticketData.wMagazineBNumber = theCOUNT_INFO.GetTicketboxBInfo().ulCurCount;
	//ticketData.wCollectionBoxNumber = 0 /*theCOUNT_INFO.GetCollectionCurCount()*/;//��Ʊд�㣬ʹ��Ӳ����֪����
	//m_pTHHelper->TicketDataWriteCMD(ticketData);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ���ݡ���Ǯ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::PrinterPrint()
{
	try{
		CString templateName= _T("ISSUE.template");
		CStringArray printArr;
		for(int printIndex=0;printIndex<theBOM_INFO.GetReceiptPrintTimes(_T("ISSUE"));printIndex++)
		{
			CStringArray printTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);

			CString strCardNum = _T("");
			if(m_DataModel.pCPUCard!=NULL)
			{
				strCardNum.Format(_T("%09u"),m_DataModel.pCPUCard->applicationData.CardLogicalNumber);
			}
			else if(m_DataModel.pULCard!=NULL)
			{
				strCardNum.Format(_T("%09u"),m_DataModel.pULCard->applicationData.CardLogicalNumber);
			}
			long balance = 0;
			if(m_DataModel.pCPUCard!=NULL){
				 balance = m_DataModel.pCPUCard->applicationData.Balance;
			}
			else if(m_DataModel.pULCard!=NULL){
				balance = m_DataModel.pULCard->applicationData.Balance;
			}
			CString strValidDate = _T("");
			if(m_DataModel.pCPUCard!=NULL)
			{
				strValidDate = m_DataModel.pCPUCard->applicationData.ValidDate.ToString(_T("%.4d-%.2d-%.2d"));
			}
			else if(m_DataModel.pULCard!=NULL)
			{
				strValidDate = m_DataModel.pULCard->applicationData.ValidEndTime.ToString(_T("%.4d-%.2d-%.2d"));
			}
			const int hspCount = 20;
			int i=0;
			while(i<printTexts.GetCount())
			{
				CString& line = printTexts.ElementAt(i);
				line.Replace(_T("{CARD_NUM}"),strCardNum);
				line.Replace(_T("{TICKET_TYPE}"),m_DataModel.ticketTypeDetail.ticketTypeName_cn);
				line.Replace(_T("{TICKET_DEPOSIT}"),ComFormatAmount(m_DataModel.ticketTypeDetail.deposit));
				if(m_DataModel.cardCategory == MULTIRIDE)
				{
					line.Replace(_T("{VALID_RIDES}"),ComFormatCount(balance));
				}
				else if(line.Find(_T("{VALID_RIDES}")) >=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				if(m_DataModel.cardCategory == PERIOD)
				{
					line.Replace(_T("{VALID_DATE}"),strValidDate);
				}
				else if(line.Find(_T("{VALID_DATE}"))>=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				line.Replace(_T("{ISSUE_AMOUNT}"),ComFormatAmount(m_DataModel.rwIssueRequest.SaleAmount));
				line.Replace(_T("{TRANS_TIME}"),m_DataModel.rwIssueRequest.TranDateTime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
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


CStringArray CIssueSvc::s_AvailablePieceList;

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
const  CStringArray& CIssueSvc::GetAvailablePieceList()
{
	if(s_AvailablePieceList.GetCount() == 0)
	{
		CString strAvailablePieceList = _T("1,2,3,4,5,10,20,50,100");
		GetSplitStrArray(strAvailablePieceList,_T(","),s_AvailablePieceList);
	}
	return s_AvailablePieceList;
}

CStringArray CIssueSvc::s_AvailableChargeValueList;
//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ÿɳ�ֵ�Ľ��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
const CStringArray& CIssueSvc::GetAvailableChargeValueList()
{
	if(s_AvailableChargeValueList.GetCount() == 0)
	{
		CString strAvailableChargeValueList = _T("50,100,150,200,250,300");
		GetSplitStrArray(strAvailableChargeValueList,_T(","),s_AvailableChargeValueList);
	}
	return s_AvailableChargeValueList;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool  CIssueSvc::GetMaxPrice(long& ticketPrice)
{
	RW_CMD_TICKET_PRICE rw_ticket_price;
	rw_ticket_price.CardType = m_DataModel.ticketType;
	rw_ticket_price.BegainStation[1] = theMAINTENANCE_INFO.GetLineCode();
	rw_ticket_price.BegainStation[0] = theMAINTENANCE_INFO.GetStationCode();
	rw_ticket_price.EndStation[0] = 0xFF;
	rw_ticket_price.EndStation[1] = 0xFF;
	rw_ticket_price.Time = ComGetCurTime();
	RW_CMD_TICKET_PRICE_RES res;
	long getPriceResult = -1;
	if(!theEXCEPTION_MGR.HasException(CRWException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID))
	{
		getPriceResult = m_pRWHelper->GetTicketPrice(rw_ticket_price,res);
	}
	else if(!theEXCEPTION_MGR.HasException(CTWException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID))
	{
		getPriceResult = m_pTWHelper->GetTicketPrice(rw_ticket_price,res);
	}
	if(getPriceResult!=RW_RSP_OK)
	{		
		theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(res.DetailErrorCode));
		return false;
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_NEW_CARD_SELET_INFO));
		ticketPrice = res.TicketPrice;
		return true;
	}	
}
