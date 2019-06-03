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
@brief      构造函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundSvc::CRefundSvc()
    :CBOMForeService(REFUND_BASE_SVC)
{
    
    // 设置操作员画面
    m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
    m_pDialogFlow->AddDialog(IDD_00602_REFUND_BASE_DLG, new CRefundBaseOfficeDlg(this));
   
    // 设置乘客画面
    m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(this));
    m_pDialogFlow->AddDialog(IDD_10601_REFUND_DLG, new CRefundReceptionDlg(this));

    // 设置画面分组
    // 等待读卡画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT, theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_PUT_CARD));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT,IDD_WAITING_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT,IDD_10002_WELCOME_DLG);

    // 退款基本画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_SELECT_MTD_CONFIRM),
                                                    theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_CONFIRM_PASSENGER), 
                                                    theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_CONFIRM_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE,IDD_00602_REFUND_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE,IDD_10601_REFUND_DLG);

	// 退款不可画面  
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_REFUND_NOT,/*theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_NOT)*/_T(""),
														theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_NOT_PASSENGER),
														theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_NOT_PASSENGER));	
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REFUND_NOT,IDD_00602_REFUND_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REFUND_NOT,IDD_10601_REFUND_DLG);

	// 退款完成画面
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH,theGUIDE_INFO.GetOperationMessage(GUIDE_REFUND_FINISH),
													theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUND_FINISH_PASSENGER), 
													theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUND_FINISH_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_00602_REFUND_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_10601_REFUND_DLG);

	//画面流转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_TO_REFUND_BASE_DLG,DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_TO_REFUND_NOT_DLG,DIALOG_GROUP_REFUND_NOT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_TO_REFUND_FINISH_DLG,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_REFUND_NOT_DLG,DIALOG_GROUP_REFUND_NOT);
   
    // 设置初始画面组
    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

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
@brief      获取服务的数据模型

@param      none

@retval     PLABEL_GROUP	站员画面基本信息

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundSvc::Model& CRefundSvc::GetDataModel()
{
	return m_DataModel;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获得车票信息

@param      (i)  UINT errCode              错误代码
@param      (i)  CRWReadCard* rwReadCard   读卡命令

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
				guideInfo = _T("黑名单卡，已锁定！");				
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
		// 设置卡片种类
		m_DataModel.cardType = theTICKET_HELPER.GetCardType(cardRspInfo.CardPHType);
		// 退款金额
		m_DataModel.refundValue = cardRspInfo.RechargeProduct[0].RefoundMoney;
		if(cardRspInfo.CardPHType == ULTRALIGHT){
			// 获得读卡信息 			    
			m_DataModel.pULCard->GetCardInfo(cardRspInfo.CardInfo);
			// 车票产品种类
			m_DataModel.cardAppType = m_DataModel.pULCard->applicationData.CardAppMainType;
			// 车票编号
			m_DataModel.strTicketNum.Format(_T("%09u"),m_DataModel.pULCard->applicationData.CardLogicalNumber);
			// 票卡名称
			theACC_TICKET.GetTicketTypeName(m_DataModel.pULCard->applicationData.CardAppMainType,m_DataModel.strTicketType_CN,m_DataModel.strTicketType_EN);
			// 有效期
			m_DataModel.strValidDate = m_DataModel.pULCard->applicationData.ValidEndTime.ToString(_T("%.4d年%.2d月%.2d日%.2d:%.2d"));
			// 卡内余额
			m_DataModel.ticketBalance = m_DataModel.pULCard->applicationData.Balance;
			// 押金
			m_DataModel.deposit = 0;
			m_DataModel.refundValue = m_DataModel.ticketBalance;
			m_DataModel.refundDeposit = 0;
			
			// 车票产品类型
			theACC_TICKET.GetTicketCategry(m_DataModel.pULCard->applicationData.CardAppMainType,m_DataModel.cardCategory);
			// 跳转至即时退款画面
			DoDialogFlow(WM_TO_REFUND_BASE_DLG);
		}
		else if(theBOM_INFO.IsEnableCPUImmidatelyRefund() && cardRspInfo.CardPHType == CPU_TICKET){
			
			// 获得读卡信息 			    
			m_DataModel.pCPUCard->GetCardInfo(cardRspInfo.CardInfoLen,cardRspInfo.CardInfo);

			// 余额小于0时不退款
			if(m_DataModel.pCPUCard->applicationData.Balance>0){
				// 车票产品种类
				m_DataModel.cardAppType = m_DataModel.pCPUCard->applicationData.CardAppMainType;
				// 车票编号
				m_DataModel.strTicketNum.Format(_T("%09u"),m_DataModel.pCPUCard->applicationData.CardLogicalNumber);
				// 票卡名称
				theACC_TICKET.GetTicketTypeName(m_DataModel.pCPUCard->applicationData.CardAppMainType,m_DataModel.strTicketType_CN,m_DataModel.strTicketType_EN);
				// 有效期
				m_DataModel.strValidDate = m_DataModel.pCPUCard->applicationData.ValidDate.ToString(_T("%.4d年%.2d月%.2d日"));
				// 卡内余额
				m_DataModel.ticketBalance = m_DataModel.pCPUCard->applicationData.Balance;
				// 押金
				m_DataModel.deposit = m_DataModel.pCPUCard->applicationData.Deposit;
				m_DataModel.refundDeposit = 0 ;
				m_DataModel.refundValue = m_DataModel.ticketBalance;
				// 车票产品类型
				theACC_TICKET.GetTicketCategry(m_DataModel.pCPUCard->applicationData.CardAppMainType,m_DataModel.cardCategory);
				// 跳转至即时退款画面
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
	// 异常捕获
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
@brief      锁卡

@param      (i)RW_BUSSINESS_RESPONSE	response	锁卡反馈数据

@retval     long	错误代码

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


void CRefundSvc::OnOK()
{
	try {
		// 退款确认
		CString msg(""),sPrice("");
		sPrice = ComFormatAmount(m_DataModel.refundDeposit +m_DataModel.refundValue );
		msg = _T("退款金额：") + sPrice + _T("元，是否确认？");
		INT_PTR confirmResult = CMessageBox::Show(TITLE_REFUND_INFO,msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		// 取消
		if(confirmResult == IDCANCEL){
			return;
		}
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_REFUNDING));
		theAPP_SESSION.ShowReceptionGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFUNDING_PASSENGER), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFUNDING_PASSENGER));
		theSERVICE_MGR.SetState(ISSUEING);
		// 退款处理
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

		// 更新交易流水号
		// 更新交易流水号
		theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);
		// 更新UD数据
		if(response.UDlen != 0){
			m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
		}
		// 更新AR数据
		if(response.ARLen != 0){
			m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,m_DataModel.cardType);
		}
		// sc审计
		SaveSCData();

		if(m_DataModel.cardPHType == CPU_TICKET) // CPU卡，补发一条退卡申请交易
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

			// 票卡公共域
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
@brief      保存一卡通充值sc数据

@param      none

@retval  	long         错误信息

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
	purchaseValue.transAmount = 0-m_DataModel.refundValue;			//退款金额
	purchaseValue.recyleCardNum = 1;
	purchaseValue.depositAmount = 0-m_DataModel.refundDeposit;		//退款押金

	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      验证操作员ID和密码

@param      int nOperatorID 操作员ID
@param      CString sPwd 密码

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
	if(thePWD_ERR.IsStuffIDExist(nOperatorID))	// ID被锁定
	{
		return false;
	}
	if(sPwd != currentStaff.staffPWD)	// 密码不正确
	{
		return false;
	}
	
	return true;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      打印数据、打开钱箱

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