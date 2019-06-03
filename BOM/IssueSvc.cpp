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
// 检查内存泄漏
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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIssueSvc::CIssueSvc():CBOMForeService(ISSUE_SVC)
{	
	m_IssuedTickets.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
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
@brief      服务启动时响应函数

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

	// 外部发售票种
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
	// 内部读写器发售票种
	else{
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_INIT);
	}

	__super::OnStart();

}


//////////////////////////////////////////////////////////////////////////
/*
@brief      设置票卡类型

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
	// 出站票
	if(ticketType == TICKET_EXIT_ONLY){
		m_DataModel.rwIssueRequest.SaleMode = BY_PRICE;
		m_DataModel.rwIssueRequest.EndStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
	}
	// 其他票种
	else{
		m_DataModel.rwIssueRequest.EndStation = 0;
	}
	
	if(m_DataModel.issueWay == CTicketHelper::BY_INNER_TPU ||  m_DataModel.issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
		m_DataModel.rwIssueRequest.BegainStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置是否是预发售

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
@brief      取得Service的数据模型

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
@brief      引发数据模型改变事件

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
		confirmMessage.Format(_T("售票%d张,总价%s元，是否确认"),m_DataModel.piece,ComFormatAmount(m_DataModel.due));
		INT_PTR confirmResult = CMessageBox::Show(STR_ISSUE_INFO,confirmMessage,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		if (confirmResult == IDCANCEL){
			return;
		}
		if(m_DataModel.issueWay == CTicketHelper::BY_INNER_TPU && m_LPTHelper->GetTicketboxDoorStatus()!=DOORSTATUS::CLOSED)
		{
			CMessageBox::Show(STR_ISSUE_INFO,_T("请先关闭出票机门"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
			return;
		}
		theSERVICE_MGR.SetState(ISSUEING);
		m_DataModel.issueSuccessPiece = 0;
		//TH缩退，使用外部读写器发售
		//if(m_DataModel.issueWay == CTicketHelper::BY_INNER_TPU && theSETTING.GetTHMode()==TH_MODE_ABNORMAL){			
		//	StartReadCard();
		//	return;
		//}
		// 使用内部读写器发售
		if(m_DataModel.issueWay == CTicketHelper::BY_INNER_TPU ){
			if((theCOUNT_INFO.GetTicketboxAInfo().ulCurCount+theCOUNT_INFO.GetTicketboxBInfo().ulCurCount)<m_DataModel.piece)
			{
				confirmMessage.Format(_T("票箱中车票数量不足%d张，是否继续？"),m_DataModel.piece);
				INT_PTR confirmResult = CMessageBox::Show(STR_ISSUE_INFO,confirmMessage,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
				if (confirmResult == IDCANCEL){
					theSERVICE_MGR.SetState(RECEPT);
					return;
				}
			}
			while(m_DataModel.issueSuccessPiece < m_DataModel.piece){
				CString strCurrentIssue;
				strCurrentIssue.Format(_T("第%d张，共%d张"),m_DataModel.issueSuccessPiece+1,m_DataModel.piece);
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUING) + strCurrentIssue);
				theAPP_SESSION.ShowReceptionGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ISSUING_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ISSUING_PASSENGER));
				bool success =   TWIssueOneCard();
				if(success)
				{
					m_DataModel.issueSuccessPiece++;
					// 设置现金信息
					NotifyDataModelChanged();
				}
				else
				{
					//SyncTicketCountToTH();
					// 打开钱箱
					m_pPRTHelper->PrinterOpenBox();
					return;
				}
			}
			//SyncTicketCountToTH();
			// 打开钱箱
			m_pPRTHelper->PrinterOpenBox();
			theSERVICE_MGR.SetState(FINISH);
			m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_FINISH);
			return;
		}
		// 外部读写器发售TH缩退票（未读卡）
		else if( m_DataModel.issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
			StartReadCard();
		}
		// 外部读写器发售（已经在开始时完成读卡动作）（CPU卡）
		else{
			bool success = RWIssueOneCard();
			if(success){
				m_DataModel.issueSuccessPiece ++;
				// 设置现金信息
				NotifyDataModelChanged();
			}
			else{
				
				return;
			}
			//if (!m_pPRTHelper->IsPrinterHasException()){
				PrinterPrint(); //打印存根
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
	// 锁卡成功
	if(RW_RSP_OK == resultCode){
		// 更新交易流水号
		theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	
		// 更新UD数据
		if(response.UDlen != 0){
			m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
		}
		// 更新AR数据
		if(response.ARLen != 0){
			m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,response.TranResult.CardType);
		}
	}
	return resultCode;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      设置卖票的TPU信息

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
@brief  读到卡的处理    

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::OnFindCard(RW_READ_CARD_RESPONSE& rwReadCardResponse)
{
	if(m_DataModel.ticketPHType != RESERVE && m_DataModel.ticketPHType != rwReadCardResponse.CardPHType){
		OnError(_T("错误的票卡类型"));
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
		OnError(_T("错误的物理卡类型"));
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      保存业务数据

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
@brief      读卡的回调函数

@param      UINT resultCode 读卡结果
@param      CRWReadCard* rwReadCardCommand 读卡命令

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
		// 黑名单票卡
		if(resultCode == RW_RSP_ACC_CARD_BLACKLIST){
			long blockCardResult = BlockCard(); 
			CString tips = blockCardResult == RW_RSP_OK ? CString("黑名单卡，已锁！" ): m_pRWHelper->GetTpuErrorInfo(resultCode);
			OnError(tips);
		}
		// 非黑名单票卡
		else{		
			// 读卡失败
			if(resultCode!=RW_RSP_OK){
				// 读卡错误
				if(m_DataModel.rwReadCardResponse.DetailErrorCode != RW_ERROR_CARD_NO_CARD){
					OnError(m_pRWHelper->GetTpuErrorInfo(m_DataModel.rwReadCardResponse.DetailErrorCode));
				}
				// 读卡操作取消
				else{
					OnError(_T(""));
					theAPP_SESSION.ShowAllGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CANCEL));	
				}
			}
			// 读卡成功
			else{
				// 解析票卡数据成功
				if(OnFindCard(m_DataModel.rwReadCardResponse)){
					// 外部读写器售卡
					if(m_DataModel.issueWay == CTicketHelper::BY_OUTER_TPU || m_DataModel.issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
						// UL票卡
						if(m_DataModel.rwReadCardResponse.CardPHType == ULTRALIGHT){
							long ticketNumber = 0;
							//if(m_DataModel.pCPUCard!=NULL){
							//	ticketNumber = m_DataModel.pCPUCard->applicationData.CardLogicalNumber;
							//}
							//else if(m_DataModel.pULCard!=NULL){
								ticketNumber = m_DataModel.pULCard->applicationData.CardLogicalNumber;
							//}
							// 票卡已售卖
							if(find(m_IssuedTickets.begin(),m_IssuedTickets.end(),ticketNumber) != m_IssuedTickets.end()){
								CMessageBox::Show(theAPP_SESSION.GetServiceNameCH(ISSUE_SVC),_T("此卡已经发售，请换一张卡继续发售！"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_3);
								StartReadCard();								
							}
							else{
								// 售卡成功
								if(RWIssueOneCard()){
									// 售票成功张数加1
									m_DataModel.issueSuccessPiece +=1;
									// 设置现金信息
									NotifyDataModelChanged();
									// 发售完成
									if(m_DataModel.issueSuccessPiece == m_DataModel.piece){
										m_IssuedTickets.clear();
										// 打开钱箱
										m_pPRTHelper->PrinterOpenBox();
										theSERVICE_MGR.SetState(FINISH);
										m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_FINISH);										
									}
									// 未发售完成
									else{
										CString strPreviouseSuccessIssue;
										strPreviouseSuccessIssue.Format(_T("第%d张发售成功！"),m_DataModel.issueSuccessPiece);
										CMessageBox::Show(theAPP_SESSION.GetServiceNameCH(ISSUE_SVC),strPreviouseSuccessIssue,CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_3);
										CString strCurrentIssue;
										strCurrentIssue.Format(_T("第%d张，共%d张"),m_DataModel.issueSuccessPiece+1,m_DataModel.piece);
										theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUING) + strCurrentIssue);
										StartReadCard();										
									}
								}								
							}							
						}
						// CPU票卡
						else{
							if(m_DataModel.pCPUCard != NULL && m_DataModel.pCPUCard->applicationData.CardAppMainType!=m_DataModel.ticketType){
								OnError(_T("错误的卡类型！"));
								return;
							}
							DoDialogFlow(WM_WAIT_TO_INIT);
							// 设置现金信息
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
@brief      内部读写器卖一张卡

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
		//LOG("--(1)---A状态：%x，票数:%d-B状态：%x，票数:%d---",
		//	theBOM_STATUS_MGR.GetMagANumStatus(),ticketboxAInfo.ulCurCount,
		//	theBOM_STATUS_MGR.GetMagBNumStatus(),ticketboxBInfo.ulCurCount);
		CAfcBomOperationParam::MAGAZINE_PARAM temParam;
		theAFC_BOM_OPERATION.GetMagazineParam(temParam);
		// A
		if (ticketboxAInfo.ulCurCount <= temParam.willEmptyValue && ticketboxAInfo.ulCurCount > temParam.haveEmptyValue && theBOM_STATUS_MGR.GetMagANumStatus() != MAG_NUM_NEAR_EMPTY)
		{
			theBOM_STATUS_MGR.SetMagANumStatus(MAG_NUM_NEAR_EMPTY);
			//LOG("--(2)---A将空，票数:%d---",ticketboxAInfo.ulCurCount);
		} 
		else if (ticketboxAInfo.ulCurCount <= temParam.haveEmptyValue && theBOM_STATUS_MGR.GetMagANumStatus() != MAG_NUM_EMPTY)
		{
			theBOM_STATUS_MGR.SetMagANumStatus(MAG_NUM_EMPTY);
			//LOG("--(3)---A空，票数:%d---",ticketboxAInfo.ulCurCount);
		} 

		// B
		if (ticketboxBInfo.ulCurCount <= temParam.haveEmptyValue && ticketboxBInfo.ulCurCount > temParam.haveEmptyValue && theBOM_STATUS_MGR.GetMagBNumStatus() != MAG_NUM_NEAR_EMPTY)
		{
			theBOM_STATUS_MGR.SetMagBNumStatus(MAG_NUM_NEAR_EMPTY);
			//LOG("--(4)---B将空，票数:%d---",ticketboxBInfo.ulCurCount);
		} 
		else if (ticketboxBInfo.ulCurCount <= temParam.haveEmptyValue && theBOM_STATUS_MGR.GetMagBNumStatus() != MAG_NUM_EMPTY)
		{
			theBOM_STATUS_MGR.SetMagBNumStatus(MAG_NUM_EMPTY);
			//LOG("--(5)---B空，票数:%d---",ticketboxBInfo.ulCurCount);
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
				OnError(_T("票箱已空"));
				return false;
			}
		}
		//LOG("--(6)---票箱号:%d---",boxNo);

		long lRet = 0;
		try{
			//LOG("获取票卡开始");
			long lRet = m_pTHHelper->CardOut(boxNo, &uiTransNum);
			//LOG("获取票卡结束");
		}
		catch(CSysException& e){
			//LOG("出票机异常！！！");
			theEXCEPTION_MGR.WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
			// 票箱1已空(A)
			if(e.GetDetailID() == CTHHardwareException::TH_ERROR_TICKET_BOX1_EMPTY){
				//	ticketboxAInfo.ulCurCount = 0;
				//	theCOUNT_INFO.SetTicketboxAInfo(ticketboxAInfo);
				theBOM_STATUS_MGR.SetMagANumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
				//LOG("--(7)---A物理数空，票数:%d---",ticketboxAInfo.ulCurCount);
				continue;
			}
			// 票箱2已空

			if(e.GetDetailID() == CTHHardwareException::TH_ERROR_TICKET_BOX2_EMPTY){
				//ticketboxBInfo.ulCurCount = 0;
				//theCOUNT_INFO.SetTicketboxBInfo(ticketboxBInfo);
				theBOM_STATUS_MGR.SetMagBNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
				//LOG("--(7)---B物理数空，票数:%d---",ticketboxBInfo.ulCurCount);
				continue;
			}
			else{
				// 其他异常，报具体异常信息
				OnError(_T("发售失败"));
				return false;
			}
		}
		catch(...){
			//throw;
			OnError(_T("发售失败"));
			return false;
		}

		// 重试出票
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
		//LOG("读写卡开始");
		twSaleCardResult = TWSaleCardFunc(saleCardResponse);
		//LOG("读写卡结束");
		if(twSaleCardResult!=RW_RSP_OK){		
			// 回收票卡
			UINT uiRetractBoxNum = 0;
			//LOG("--(8)---回收票卡开始");
			m_pTHHelper->RetractCard(&uiRetractBoxNum);// 进废票箱
			//LOG("回收票卡结束");
			recycleCardCount ++;
			theTICKET_HELPER.UpdateTHCount(deleteBoxACount,deleteBoxBCount,1);                //更新回收车票计数			
			if(recycleCardCount == maxAllowErrorCount){		
				OnError(_T("TH累计错误次数超过允许值"));
				return false;
			}
			continue;
		}

		// 保存交易及审计
		SaveBusinessData(saleCardResponse);

		// 放出票卡
		//bool isRecycle = false;
		//SC_RSP_ISSUE_PROCESS releaseCardResponse;
		//m_pTHHelper->THIssueCard(releaseCardResponse,isRecycle);
		UINT uiRetractBoxNum = 0;
		//LOG("--(8)---放出票卡开始");
		m_pTHHelper->SendCard(&uiRetractBoxNum);// 出票
		//LOG("放出票卡结束");
		theTICKET_HELPER.UpdateTHCount(deleteBoxACount,deleteBoxBCount,0);                //更新回收车票计数
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
@brief     更新TH票箱状态，票箱计数 

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
	errorMsg.Replace(_T("（红）"),_T(""));
	errorMsg.Replace(_T("（黑）"),_T(""));
	errorMsg=_T( "（红）") +errorMsg;
	theAPP_SESSION.ShowOfficeGuide(errorMsg);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   同步本地票箱计数到TH   

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
	//ticketData.wCollectionBoxNumber = 0 /*theCOUNT_INFO.GetCollectionCurCount()*/;//废票写零，使用硬件感知报错
	//m_pTHHelper->TicketDataWriteCMD(ticketData);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印数据、打开钱箱

@param      无

@retval     无

@exception  无
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
@brief      取得可充值的金额

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
