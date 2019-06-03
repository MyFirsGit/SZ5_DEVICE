#include "StdAfx.h"
#include "AdjustSvc.h"
#include "waitingofficedlg.h"
#include "adjustbaseofficedlg.h"
#include "WelcomeReceptionDlg.h"
#include "AdjustReceptionDlg.h"
#include "TicketHelper.h"
#include "Validation.h"
#include "GuideInfo.h"
#include "CAccLineInfoParam.h"
#include "SCAuditManager.h"


// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAdjustSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CAdjustSvc::CAdjustSvc()
	:CBOMForeService(ADJUST_SVC)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CAdjustSvc::~CAdjustSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置付费区标志


@param      (i) 付费区标志

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAdjustSvc::SetIsPaidArea(bool isPaidArea)
{
	m_DataModel.isPaidArea = isPaidArea;

}



//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAdjustSvc::OnStart()
{
	m_pDialogFlow->AddDialog(IDD_00204_ADJUST_BASE_DLG,new CAdjustBaseOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(this));
	m_pDialogFlow->AddDialog(IDD_10201_ADJUST_DLG, new CAdjustReceptionDlg(this));
	if(m_DataModel.isPaidArea){
		m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this,_T("TRUE")));
	}
	else{
		m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
	}

	// 设置画面分组
	// 等待组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT,theGUIDE_INFO.GetOperationMessage(GUIDE_ADJUST_PUT_CARD));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_10002_WELCOME_DLG);


	// 基本画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_INIT,  theGUIDE_INFO.GetOperationMessage(GUIDE_ADJUST_CONFORM),
		theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ADJUST_CONFIRM_PASSENGER), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ADJUST_CONFIRM_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INIT, IDD_00204_ADJUST_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INIT, IDD_10201_ADJUST_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH,theGUIDE_INFO.GetOperationMessage(GUIDE_ADJUST_END),
		theGUIDE_INFO.GetPassengerMessageCn(m_DataModel.isPaidArea ? GUIDE_ADJUST_PAIDAREA_FINISH_PASSENGER:GUIDE_ADJUST_UNPAIDAREA_FINISH_PASSENGER)
		,theGUIDE_INFO.GetPassengerMessageEn(m_DataModel.isPaidArea ? GUIDE_ADJUST_PAIDAREA_FINISH_PASSENGER:GUIDE_ADJUST_UNPAIDAREA_FINISH_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_00204_ADJUST_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_10201_ADJUST_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR, /*theGUIDE_INFO.GetOperationMessage(GUIDE_ADJUST_NOT)*/_T(""),
		theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ADJUST_ERROR_PASSENGER), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ADJUST_ERROR_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR,IDD_00204_ADJUST_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR,IDD_10201_ADJUST_DLG);

	// 设置画面流转
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);

	// 等待组
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_ADJUST_WAIT_TO_INIT,DIALOG_GROUP_INIT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_ADJUST_WAIT_TO_FINISHT,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_ADJUST_INIT_TO_FINISH,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INI_TO_ERROR,DIALOG_GROUP_ERROR);
	__super::OnStart();

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
void CAdjustSvc::OnReadCard(UINT resultCode,CRWReadCard* pRWReadCard)
{
	try{
		CMessageBox::End();
		if(GetCurDialogGroupID() == DIALOG_GROUP_FINISH || GetCurDialogGroupID() == DIALOG_GROUP_ERROR)
		{
			return;
		}
		auto_ptr<RW_READ_CARD_RESPONSE> pRWReadCardResponse(new RW_READ_CARD_RESPONSE);
		pRWReadCard->GetRWResponse(pRWReadCardResponse.get());
		// 读卡失败
		m_DataModel.cardPHType =(CARD_MEDIUM_TYPE) pRWReadCardResponse->CardPHType;
		if(resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_ECT_CARD_BLACKLIST){
			// 黑名单卡
			RW_BUSSINESS_RESPONSE response;
			theSERVICE_MGR.SetState(ISSUEING);
			long errCode = BlockCard(resultCode,response); 
			theSERVICE_MGR.SetState(RECEPT);
			if(errCode != RW_RSP_OK){
				OnCardError(m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode));
			}	
			else{
				OnCardError(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_BLACK));
			}
			return;
		}
		if(resultCode != RW_RSP_OK)
		{
			CString errorMessage = pRWReadCardResponse->DetailErrorCode == RW_ERROR_CARD_NO_CARD ? theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL) : m_pRWHelper->GetTpuErrorInfo(pRWReadCardResponse->DetailErrorCode);
			OnCardError(errorMessage);
			return;
		}
		bool saveCardInfoSuccess = SaveCardInfo(pRWReadCardResponse.get());
		if(!saveCardInfoSuccess)
		{
			// 跳转至错误画面
			OnCardError(_T("车票数据错误"));	
			return;
		}
		if(!IsAdjustCodeValid())
		{
			OnCardError(theGUIDE_INFO.GetOperationMessage(GUIDE_ADJUST_CODE_ERROR));
			return;
		}
		if(!IsNeedAdust())
		{
			OnCardError(m_DataModel.strAdjustGuide);
			theAPP_SESSION.ShowReceptionGuide(m_DataModel.strAdjustGuideOfReceipt,m_DataModel.strAdjustEnGuideOfReceipt);
			return;
		}

		BYTE canSelectStationAdjustCodes[] = {OUT_STATION_UPDATE_22}; //22,25需要选择进站码
		m_DataModel.canSelectStation =  CValidation::IsInRange(m_DataModel.adjustCode,canSelectStationAdjustCodes);
		if(m_DataModel.adjustCode == OUT_STATION_UPDATE_21)//21出站更新，默认进站车站为发信车站
		{
			m_DataModel.enterStation = m_DataModel.saleStation;
			m_DataModel.enterDateTime =/* m_DataModel.saleTime*/ComGetCurTime();
		}
		if(m_DataModel.adjustCode == OUT_STATION_UPDATE_22)
		{
			m_DataModel.enterStation = 0;
			m_DataModel.enterDateTime = _DATE_TIME();
		}
		if(m_DataModel.ticketCategory == MULTIRIDE)
		{
			m_DataModel.availablePayMethod = 0x02;	//卡内补票
			m_DataModel.payMethod = BY_CARD;
		}
		else if(m_DataModel.ticketCategory == PERIOD)
		{
			m_DataModel.availablePayMethod = 0x01;	//现金补票
			m_DataModel.payMethod = BY_CASH;
		}
		else if(m_DataModel.ticketCategory == PURSE)
		{
			m_DataModel.availablePayMethod = 0x01;
			m_DataModel.payMethod = BY_CASH;
			if(m_DataModel.balance > m_DataModel.actualAdjustAmount && theACC_TICKET.IsNotCollectedTicket(m_DataModel.ticketType)){
				m_DataModel.availablePayMethod |= 0x02;
				m_DataModel.payMethod = BY_CARD;
			}
		}
		DoDialogFlow(WM_ADJUST_WAIT_TO_INIT);

	}
	catch (CSysException& e){
		OnCardError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		CInnerException e(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnCardError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      锁卡处理

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CAdjustSvc::BlockCard(UINT resultCode,RW_BUSSINESS_RESPONSE response)
{
	RW_CMD_BLOCK rw_cmd_block;
	rw_cmd_block.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
	rw_cmd_block.TranDateTime = ComGetCurTime();
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

bool CAdjustSvc::IsAdjustCodeValid()
{
	BYTE unNeedAdjustCodes[] = {NO_UPDATE,ENTER_STATION_UPDATE_82,OUT_STATION_UPDATE_2A,OUT_STATION_UPDATE_29};
	if(CValidation::IsInRange(m_DataModel.adjustCode,unNeedAdjustCodes))
	{
		return true;
	}
	BYTE paidAdjustCodes[] = {OUT_STATION_UPDATE_21,OUT_STATION_UPDATE_22,OUT_STATION_UPDATE_23,
		OUT_STATION_UPDATE_24,OUT_STATION_UPDATE_25,OUT_STATION_UPDATE_26,OUT_STATION_UPDATE_27,
		OUT_STATION_UPDATE_28};
	if(m_DataModel.isPaidArea)
	{
		return CValidation::IsInRange(m_DataModel.adjustCode,paidAdjustCodes);
	}

	BYTE notPaidAdjustCodes[] = {ENTER_STATION_UPDATE_81,ENTER_STATION_UPDATE_83,ENTER_STATION_UPDATE_84,ENTER_STATION_UPDATE_85,ENTER_STATION_UPDATE_86,ENTER_STATION_UPDATE_87,ENTER_STATION_UPDATE_88};
	if(!m_DataModel.isPaidArea)
	{
		return CValidation::IsInRange(m_DataModel.adjustCode,notPaidAdjustCodes);
	}
	return false;
}


bool CAdjustSvc::IsNeedAdust()
{
	BYTE unNeedAdjustCodes[] = {NO_UPDATE,ENTER_STATION_UPDATE_82,OUT_STATION_UPDATE_2A,OUT_STATION_UPDATE_29};
	return !CValidation::IsInRange(m_DataModel.adjustCode,unNeedAdjustCodes);
}



//////////////////////////////////////////////////////////////////////////
/*
@brief    保存读卡数据

@param      (i) LPRW_READ_CARD_RESPONSE pRWReadCardResponse 读卡数据

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CAdjustSvc::SaveCardInfo(LPRW_READ_CARD_RESPONSE pRWReadCardResponse)
{
	m_DataModel.adjustCode = m_DataModel.isPaidArea ? pRWReadCardResponse->PaidAreaAdjustCode : pRWReadCardResponse->UnPaidAreaAdjustCode;
	m_DataModel.adjustAmount = pRWReadCardResponse->AdjustAmount;
	m_DataModel.actualAdjustAmount = m_DataModel.adjustAmount;	//默认实际补票金额为建议补票金额
	m_DataModel.strAdjustGuide = theBOM_INFO.GetUpdateSuggestOfOperator(m_DataModel.adjustCode);
	m_DataModel.strAdjustGuideOfReceipt = theBOM_INFO.GetUpdateSuggestOfPassager(m_DataModel.adjustCode);
	m_DataModel.strAdjustEnGuideOfReceipt = theBOM_INFO.GetUpdateSuggestEn(m_DataModel.adjustCode);
	if(m_DataModel.cardPHType == ULTRALIGHT)
	{
		CULCard ulCard;
		ulCard.GetCardInfo(pRWReadCardResponse->CardInfo);
		m_DataModel.ticketType = ulCard.applicationData.CardAppMainType;
		m_DataModel.strTicketNumber.Format(_T("%.9u"),ulCard.applicationData.CardLogicalNumber);
		m_DataModel.balance = ulCard.applicationData.Balance;
		m_DataModel.enterStation = ulCard.journeyData.EnterStation;
		m_DataModel.enterDateTime = ulCard.journeyData.EnterTime;
		m_DataModel.exitStation = ulCard.journeyData.ExitStation;
		m_DataModel.exitDateTime = ulCard.journeyData.ExitTime;
		m_DataModel.journeyStatus = ulCard.journeyData.JourneyStatus;

		m_DataModel.saleStation = ulCard.applicationData.SaleStation;
		m_DataModel.saleTime = ulCard.applicationData.ValidStartTime;
	}
	else if(m_DataModel.cardPHType == CPU_TICKET)
	{
		CCPUCard cpuCard;
		cpuCard.GetCardInfo(pRWReadCardResponse->CardInfoLen,pRWReadCardResponse->CardInfo);
		m_DataModel.ticketType = cpuCard.applicationData.CardAppMainType;
		m_DataModel.strTicketNumber.Format(_T("%.9u"),cpuCard.applicationData.CardLogicalNumber);
		m_DataModel.balance = cpuCard.applicationData.Balance;
		m_DataModel.enterStation = cpuCard.journeyData.EnterStation;
		m_DataModel.enterDateTime = cpuCard.journeyData.EnterTime;
		m_DataModel.exitStation = cpuCard.journeyData.ExitStation;
		m_DataModel.exitDateTime = cpuCard.journeyData.ExitTime;
		m_DataModel.journeyStatus = cpuCard.journeyData.JourneyStatus;
	}
	else{
				
		return false;
	}
	theACC_TICKET.GetTicketCategry(m_DataModel.ticketType,m_DataModel.ticketCategory);
	theACC_TICKET.GetTicketTypeName(m_DataModel.ticketType,m_DataModel.strTicketTypeName,m_DataModel.strTicketTypeEnName);
	return true;


}


//////////////////////////////////////////////////////////////////////////
/*
@brief      响应确定按钮

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAdjustSvc::OnOK()
{
	// 补票处理 
	Adjust();		
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     补票处理 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAdjustSvc::Adjust()
{
	try{
		CString msg(_T(""));
		msg.Format(_T("补票金额：%s，是否确认"),m_DataModel.ticketCategory==MULTIRIDE ? ComFormatCount(m_DataModel.actualAdjustAmount)+TIMES_UNIT : ComFormatAmount(m_DataModel.actualAdjustAmount)+MONEY_UNIT);
		INT_PTR confirmResult = CMessageBox::Show(STR_ADJUST_INFO,msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);

		if (IDOK == confirmResult){
			theSERVICE_MGR.SetState(ISSUEING);
			RW_CMD_ADJUST rwCmdAdjust;
			rwCmdAdjust.SerialNo=theCOUNT_INFO.GetUDSNForDisplay();
			rwCmdAdjust.PaymentMethod = m_DataModel.payMethod;
			BYTE setEnterStationTime[] = {OUT_STATION_UPDATE_21,OUT_STATION_UPDATE_22};
			bool isSetEnterStationTimeAsTransTime =  CValidation::IsInRange(m_DataModel.adjustCode,setEnterStationTime);
			if(isSetEnterStationTimeAsTransTime)
			{
				rwCmdAdjust.TranDateTime = m_DataModel.enterDateTime;
			}
			else
			{
				rwCmdAdjust.TranDateTime = ComGetCurTime();
			}
			rwCmdAdjust.CardType = theTICKET_HELPER.GetCardType(m_DataModel.cardPHType);
			rwCmdAdjust.ProductType = m_DataModel.ticketType;
			rwCmdAdjust.AdjustArea = m_DataModel.isPaidArea ? 0x01:0x02;
			rwCmdAdjust.AdjustMethod= m_DataModel.adjustCode;
			rwCmdAdjust.AdjustAmount = m_DataModel.actualAdjustAmount;
			rwCmdAdjust.BegainStation = m_DataModel.enterStation;
			rwCmdAdjust.EndStation = m_DataModel.exitStation;

			RW_BUSSINESS_RESPONSE rwBusinessResponse;
			long issueResult = m_pRWHelper->Adjust(rwCmdAdjust,rwBusinessResponse);
			theSERVICE_MGR.SetState(RECEPT);
			if(issueResult!=RW_RSP_OK){
				OnCardError(m_pRWHelper->GetTpuErrorInfo(rwBusinessResponse.DetailErrorCode));
			}
			else{
				SaveBusinessData(&rwBusinessResponse);
				if(rwBusinessResponse.UDlen>0)
				{
					m_pRWHelper->SaveRWUDData(rwBusinessResponse.UDlen,rwBusinessResponse.UDData);
				}
				if(rwBusinessResponse.ARLen)
				{
					m_pRWHelper->SaveRWARData(rwBusinessResponse.ARLen,rwBusinessResponse.ARData,m_DataModel.ticketType);	
				}		
				if (m_DataModel.due>0)
				{
					m_pPRTHelper->PrinterOpenBox();
				}
				// 跳转画面
				m_pDialogFlow->ShowNextDialogGroup(WM_ADJUST_INIT_TO_FINISH);
			}
		}
	}
	catch (CSysException& e)
	{
		OnCardError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnCardError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      保存交易数据

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAdjustSvc::SaveBusinessData(LPRW_BUSSINESS_RESPONSE pRWBusinessResponse)
{

	theCOUNT_INFO.UpdateUDSN(pRWBusinessResponse->SerialAddnum);
	SCAuditKey purchaseKey;
	purchaseKey.serviceID = ADJUST_SVC;
	purchaseKey.cardIssuancer = theTICKET_HELPER.GetCardIssuancer(m_DataModel.ticketType);
	purchaseKey.ticketType = m_DataModel.ticketType;
	//purchaseKey.flag = m_DataModel.payMethod;
	
	//0115类交易补票代码
	BYTE adjustCodes_0115[]={OUT_STATION_UPDATE_24,OUT_STATION_UPDATE_26,OUT_STATION_UPDATE_28};	//超时，超乘，超时并且超乘
	bool isAdjust0115 = CValidation::IsInRange(m_DataModel.adjustCode,adjustCodes_0115);
	purchaseKey.flag =  isAdjust0115 ?  ADJUST_KIND :UPDATE_KIND;
	SCAuditValue purchaseValue;
	purchaseValue.transCount = m_DataModel.adjustCode == OUT_STATION_UPDATE_28 ? 2 : 1;	//超时并且超乘产生两条交易
	if(m_DataModel.payMethod == BY_CASH)//现金支付时，补票金额填在交易金额中
	{
		purchaseValue.transAmount = m_DataModel.actualAdjustAmount;
		purchaseValue.depositAmount = 0;
	}
	if(m_DataModel.payMethod == BY_CARD)//卡内扣款时，补票金额填在押金中，具体参见接口文档。
	{
		purchaseValue.transAmount = 0;
		purchaseValue.depositAmount = m_DataModel.actualAdjustAmount;
	}


	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
}


void CAdjustSvc::NotifyDataModelChanged()
{
	m_DataModel.due = m_DataModel.payMethod == BY_CASH ? m_DataModel.actualAdjustAmount : 0;
	if(m_DataModel.paid != 0){
		m_DataModel.change = m_DataModel.paid - m_DataModel.due;
	}
	else{
		m_DataModel.change = 0;
	}
	__super::NotifyDataModelChanged();
}

CAdjustSvc::Model& CAdjustSvc::GetDataModel()
{
	return m_DataModel;
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      出错处理

@param      CString errorMsg 错误信息

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAdjustSvc::OnCardError(CString errorMsg)
{
	theSERVICE_MGR.SetState(FINISH);
	if (GetDialogFlow()->GetCurDialogGroupID() == CAdjustSvc::DIALOG_GROUP_WAIT){
		m_pDialogFlow->ShowNextDialogGroup(WM_WAIT_TO_ERROR);
	}
	else if(GetDialogFlow()->GetCurDialogGroupID() == CAdjustSvc::DIALOG_GROUP_INIT){
		m_pDialogFlow->ShowNextDialogGroup(WM_INI_TO_ERROR);
	}	
	theAPP_SESSION.ShowOfficeGuide(errorMsg);
}
