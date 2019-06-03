// RefreshSvc.cpp : 实现文件

#include "StdAfx.h"
#include "WelcomeReceptionDlg.h"
#include "guideinfo.h"
#include "RWDef.h"
#include "defersvc.h"
#include "deferofficedlg.h"
#include "deferreceptiondlg.h"
#include "templatehelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_TO_BASE_DLG            = _T("WM_TO_BASE_DLG");              // 基本画面
static const TCHAR* const WM_TO_DEFER_NOT_DLG     = _T("WM_TO_DEFER_NOT_DLG");       // 激活不可画面
static const TCHAR* const WM_TO_DEFER_FINISH_DLG  = _T("WM_TO_DEFER_FINISH_DLG");    // 激活结束画面
static const TCHAR* const WM_TO_DEFER_FAILURE_DLG = _T("WM_TO_DEFER_FAILURE_DLG");   // 激活失败画面


BEGIN_MESSAGE_MAP(CDeferSvc, CBOMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CDeferSvc::CDeferSvc()
:CBOMForeService(DEFER_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_03101_DEFER_DLG, new CDeferOfficeDlg(this));

	// 设置乘客画面
	m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));
	m_pDialogFlow->AddDialog(IDD_11701_DEFER_DLG, new CDeferReceptionDlg(this));

	// 设置画面分组
	// 等待读卡画面
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT, theGUIDE_INFO.GetOperationMessage(GUIDE_DEFER_PUT_CARD));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_10002_WELCOME_DLG);

	// 激活不可画面
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DEFER_NOT,_T(""),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_DEFER_NOT_PASSENGER),
		theGUIDE_INFO.GetPassengerMessageEn(GUIDE_DEFER_NOT_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DEFER_NOT, IDD_03101_DEFER_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DEFER_NOT, IDD_11701_DEFER_DLG);

	// 激活基本画面
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_DEFER_CONFIRM_PRESS_OKBTN),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_DEFER_CONFIRM_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_DEFER_CONFIRM_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_03101_DEFER_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_11701_DEFER_DLG);

	// 激活结束画面
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, theGUIDE_INFO.GetOperationMessage(GUIDE_DEFER_FINISH),
		theGUIDE_INFO.GetPassengerMessageCn(GUIDE_DEFER_FINISH_PASSENGER),
		theGUIDE_INFO.GetPassengerMessageEn(GUIDE_DEFER_FINISH_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_03101_DEFER_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_11701_DEFER_DLG);

	// 画面流转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_BASE_DLG, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_DEFER_NOT_DLG, DIALOG_GROUP_DEFER_NOT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_DEFER_FINISH_DLG, DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_DEFER_FAILURE_DLG, DIALOG_GROUP_DEFER_NOT);
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
CDeferSvc::~CDeferSvc(void)
{
	delete m_DataModel.pCPUCard;
	m_DataModel.pCPUCard = NULL;
	delete m_DataModel.pULCard;
	m_DataModel.pULCard = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CDeferSvc::OnReadCard(UINT errCode, CRWReadCard * pRWReadCard)
{
	try{
		CMessageBox::End();
		RW_READ_CARD_RESPONSE &cardRspInfo = m_DataModel.readCardResponse;
		pRWReadCard->GetRWResponse(&cardRspInfo); 
		// 读卡失败
		if(errCode == RW_RSP_ACC_CARD_BLACKLIST || errCode == RW_RSP_ECT_CARD_BLACKLIST){
			// 黑名单票卡
			RW_BUSSINESS_RESPONSE response;
			errCode = BlockCard(errCode,response); 
			CString guideInfo =errCode == RW_RSP_OK ? theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_BLACK_CARD) : m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode);
			OnError(guideInfo);
			return;
		}
		if(RW_RSP_OK != errCode)
		{
			CString strGuideInfo = cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD ? theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL) : m_pRWHelper->GetTpuErrorInfo(cardRspInfo.DetailErrorCode);
			OnError(strGuideInfo);
			if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD)
			{
				theAPP_SESSION.ShowReceptionGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CANCEL));	
			}
			return;
		}
		delete m_DataModel.pCPUCard;
		m_DataModel.pCPUCard = NULL;
		delete m_DataModel.pULCard;
		m_DataModel.pULCard = NULL;
		if (ULTRALIGHT == cardRspInfo.CardPHType)
		{
			m_DataModel.pULCard = new CULCard;
			m_DataModel.pULCard->GetCardInfo(cardRspInfo.CardInfo);
			m_DataModel.ticketType =  m_DataModel.pULCard->applicationData.CardAppMainType;
			m_DataModel.orignalValidate = m_DataModel.pULCard->applicationData.ValidEndTime;
			m_DataModel.balance = m_DataModel.pULCard->applicationData.Balance;
		}
		if(CPU_TICKET == cardRspInfo.CardPHType)
		{
			m_DataModel.pCPUCard = new CCPUCard;
			m_DataModel.pCPUCard->GetCardInfo(cardRspInfo.CardInfoLen,cardRspInfo.CardInfo);
			m_DataModel.ticketType = m_DataModel.pCPUCard->applicationData.CardAppMainType;
			_DATE &validDate =m_DataModel.pCPUCard->applicationData.ValidDate;
			m_DataModel.orignalValidate = _DATE_TIME(validDate.wYear,validDate.biMonth,validDate.biDay,0,0,0);
			m_DataModel.balance = m_DataModel.pCPUCard->applicationData.Balance;
		}
		theACC_TICKET.GetTicketInfo(m_DataModel.ticketType,m_DataModel.ticketTypeDetail);
		theACC_TICKET.GetTicketCategry(m_DataModel.ticketType,m_DataModel.cardCategory);
		m_DataModel.deferDay = m_DataModel.ticketTypeDetail.extendTime;
		m_DataModel.due = m_DataModel.ticketType == TICKET_OLD_FREE ? 10*100 : 0; //老年免费票收10块钱，其他票卡不收费
		this->NotifyDataModelChanged();
		// 跳转至充值基本画面
		DoDialogFlow(WM_TO_BASE_DLG); 	
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

long CDeferSvc::BlockCard(UINT resultCode,RW_BUSSINESS_RESPONSE response)
{
	theSERVICE_MGR.SetState(ISSUEING);
	RW_CMD_BLOCK rw_cmd_block;
	if (resultCode == RW_RSP_ACC_CARD_BLACKLIST){
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
	} 
	theSERVICE_MGR.SetState(RECEPT);
	return resultCode;
}

void CDeferSvc::OnError(CString strGuideInfo)
{
	theSERVICE_MGR.SetState(FINISH);
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_WAIT)
	{
		DoDialogFlow(WM_TO_DEFER_NOT_DLG);
	}
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_BASE)
	{
		DoDialogFlow(WM_TO_DEFER_FAILURE_DLG);
	}
	theAPP_SESSION.ShowOfficeGuide(strGuideInfo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      激活处理

@param      无

@retval  	0:处理成功  非零:失败信息

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CDeferSvc::DoDefer()
{
	try {
		INT_PTR  result = CMessageBox::Show(_T("车票信息"),m_DataModel.ticketTypeDetail.ticketTypeName_cn + _T("延期，是否确认？")
			,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		if(result== IDCANCEL)
		{
			return 0;
		}
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_DEFER_WAIT));
		RW_CMD_DEFER &deferInput = m_DataModel.deferInput;
		deferInput.DeferTime = m_DataModel.deferDay;
		deferInput.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
		deferInput.ServiceFee = m_DataModel.due;
		deferInput.TranDateTime = ComGetCurTime();
		RW_BUSSINESS_RESPONSE  response;
		long errCode = m_pRWHelper->Defer(deferInput,response);
		// 写卡成功
		if(RW_RSP_OK == errCode){
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);
			if(response.UDlen>0)
			{
				m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
			}
			if(response.ARLen>0)
			{
				m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,m_DataModel.ticketType);	
			}

			SCAuditKey purchaseKey;
			purchaseKey.serviceID = DEFER_SVC;

			SCAuditValue purchaseValue;
			purchaseValue.transCount = 1;
			purchaseValue.transAmount = m_DataModel.due;
			purchaseValue.cardCostAmount =0;
			purchaseValue.depositAmount =0;
			purchaseValue.recyleCardNum = 0;
			purchaseValue.serviceFeeAmount =0;

			theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
			DoDialogFlow(WM_TO_DEFER_FINISH_DLG);
		}
		else{
			DoDialogFlow(WM_TO_DEFER_FAILURE_DLG);
			theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode));
		}
		return errCode;
	}
	// 异常捕获
	catch (CSysException & e) {
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获得业务模型

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CDeferSvc::DataModel& CDeferSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
 void  CDeferSvc::NotifyDataModelChanged()
 {
	 if(m_DataModel.paid!=0)
	 {
		 m_DataModel.change = m_DataModel.paid - m_DataModel.due;
	 }
	 else
	 {
		 m_DataModel.change = 0;
	 }
	 _DATE_TIME currentDate = ComGetCurTime();
	 currentDate.biHour = 0;
	 currentDate.biMinute = 0;
	 currentDate.biSecond = 0;
	 _DATE_TIME newValidateFrom = m_DataModel.orignalValidate>currentDate ? m_DataModel.orignalValidate : currentDate;
	 m_DataModel.newValidate = GetDateTimeAfter(newValidateFrom,m_DataModel.deferDay);
	 __super::NotifyDataModelChanged();
 }

//////////////////////////////////////////////////////////////////////////
/*
@brief      打印延期凭证

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
 void CDeferSvc::PrinterPrint()
 {
		 CString templateName= _T("DEFER.template");
		 CStringArray printArr;
		 for(int printIndex=0;printIndex<theBOM_INFO.GetReceiptPrintTimes(_T("DEFER"));printIndex++)
		 {
			 CStringArray printTexts;
			 theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);

			 CString strCardNum = _T("");
			 if(m_DataModel.pCPUCard!=NULL)
			 {
				 strCardNum.Format(_T("%.9u"),m_DataModel.pCPUCard->applicationData.CardLogicalNumber);
			 }
			 else if(m_DataModel.pULCard!=NULL)
			 {
				 strCardNum.Format(_T("%.9u"),m_DataModel.pULCard->applicationData.CardLogicalNumber);
			 }

			 const int hspCount = 20;
			 int i=0;
			 while(i<printTexts.GetCount())
			 {
				 CString& line = printTexts.ElementAt(i);
				 line.Replace(_T("{CARD_NUM}"),strCardNum);
				 line.Replace(_T("{TICKET_TYPE}"),m_DataModel.ticketTypeDetail.ticketTypeName_cn);
				 if(m_DataModel.cardCategory == MULTIRIDE)
				 {
					 line.Replace(_T("{TICKET_RIDES}"),ComFormatCount(m_DataModel.balance));
				 }
				 else if(line.Find(_T("{TICKET_RIDES}")) >=0)
				 {
					 printTexts.RemoveAt(i);
					 continue;
				 }
				 if(m_DataModel.cardCategory == PURSE)
				 {
					 line.Replace(_T("{TICKET_BALANCE}"),ComFormatAmount(m_DataModel.balance));
				 }
				 else if(line.Find(_T("{TICKET_BALANCE}"))>=0)
				 {
					 printTexts.RemoveAt(i);
					 continue;
				 }

				 line.Replace(_T("{OLD_VALID_DATE}"),m_DataModel.orignalValidate.ToString(_T("%.4d-%.2d-%.2d ")));
				 line.Replace(_T("{DEFER_DAYS}"),ComFormatCount(m_DataModel.deferDay));
				 line.Replace(_T("{NEW_VALID_DATE}"),m_DataModel.newValidate.ToString(_T("%.4d-%.2d-%.2d ")));
				 line.Replace(_T("{SERVICE_FEE}"),ComFormatAmount(m_DataModel.due));
				 line.Replace(_T("{TRANS_TIME}"),m_DataModel.deferInput.TranDateTime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
				 line.Replace(_T("{STATION_NAME}"),theAPP_SESSION.GetStationNameCN());
				 line.Replace(_T("{DEVICE_CODE}"),theMAINTENANCE_INFO.GetCurrentACCDevice().ToString());
				 line.Replace(_T("{USER_ID}"),theAPP_SESSION.GetUserInfo().sUserID);
				 i++;
				 CStringArray printLine;
				 ComPrintLineFormatForTemplate(line,printLine);
				 printArr.Append(printLine);
			 }
		 }
		 m_pPRTHelper->PrinterPrint(&printArr,true);

 }
