// RefreshSvc.cpp : 实现文件

#include "StdAfx.h"
#include "Refreshsvc.h"
#include "RefreshBaseOfficeDlg.h"
#include "RefreshReceptionDlg.h"
#include "WelcomeReceptionDlg.h"
#include "guideinfo.h"
#include "RWDef.h"
#include "TicketHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRefreshSvc, CBOMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefreshSvc::CRefreshSvc()
    :CBOMForeService(REFRESH_SVC)
{
	// 设置操作员画面
    m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_00701_REFRESH_BASE_DLG, new CRefreshBaseOfficeDlg(this));

	// 设置乘客画面
	m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));
	m_pDialogFlow->AddDialog(IDD_10701_REFRESH_DLG, new CRefreshReceptionDlg(this));
	
	// 设置画面分组
    // 等待读卡画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT, theGUIDE_INFO.GetOperationMessage(GUIDE_REFRESH_PUT_CARD));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_10002_WELCOME_DLG);

    // 激活不可画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_REFRESH_NOT,_T(""),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFRESH_NOT_PASSENGER),
                                                              theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFRESH_NOT_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REFRESH_NOT, IDD_00701_REFRESH_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REFRESH_NOT, IDD_10701_REFRESH_DLG);

    // 激活基本画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_REFRESH_CONFIRM_PRESS_OKBTN),theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFRESH_CONFIRM_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFRESH_CONFIRM_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_00701_REFRESH_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_10701_REFRESH_DLG);

    // 激活结束画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, theGUIDE_INFO.GetOperationMessage(GUIDE_REFRESH_FINISH),
                                theGUIDE_INFO.GetPassengerMessageCn(GUIDE_REFRESH_FINISH_PASSENGER),
                                theGUIDE_INFO.GetPassengerMessageEn(GUIDE_REFRESH_FINISH_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_00701_REFRESH_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_10701_REFRESH_DLG);

    // 画面流转
    m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_BASE_DLG, DIALOG_GROUP_BASE);
    m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_REFRESH_NOT_DLG, DIALOG_GROUP_REFRESH_NOT);
    m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_REFRESH_FINISH_DLG, DIALOG_GROUP_FINISH);
    m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_REFRESH_FAILURE_DLG, DIALOG_GROUP_REFRESH_NOT);
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
CRefreshSvc::~CRefreshSvc(void)
{
	//if(m_DataModel.pULCard!=NULL)
	//{
	//	delete m_DataModel.pULCard;
	//	m_DataModel.pULCard = NULL;
	//}
	//if(m_DataModel.pCPUCard != NULL)
	//{
	//	delete m_DataModel.pCPUCard;
	//	m_DataModel.pCPUCard = NULL;
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CRefreshSvc::OnReadCard(UINT errCode, CRWReadCard * pRWReadCard)
{
	try{
		CMessageBox::End();
		RW_READ_CARD_RESPONSE &cardRspInfo = m_DataModel.readCardResponse;
		pRWReadCard->GetRWResponse(&cardRspInfo);  
		if(RW_RSP_OK != errCode)
		{
			// 到充值不可画面
			DoDialogFlow(WM_TO_REFRESH_NOT_DLG); 		
			theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(cardRspInfo.DetailErrorCode));
			theSERVICE_MGR.SetState(FINISH);
			return;
		}
		if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD)
		{
			DoDialogFlow(WM_TO_REFRESH_NOT_DLG); 	
			theAPP_SESSION.ShowAllGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CANCEL));	
			return;
		}
		if (ULTRALIGHT == cardRspInfo.CardPHType)
		{
			CULCard ulCard;
			ulCard.GetCardInfo(cardRspInfo.CardInfo);
			m_DataModel.ticketType =  ulCard.applicationData.CardAppMainType;
			m_DataModel.orignalValidate = ulCard.applicationData.ValidEndTime;
			m_DataModel.balance = ulCard.applicationData.Balance;
		}
		if(CPU_TICKET == cardRspInfo.CardPHType)
		{
			CCPUCard cpuCard;
			cpuCard.GetCardInfo(cardRspInfo.CardInfoLen,cardRspInfo.CardInfo);
			m_DataModel.ticketType = cpuCard.applicationData.CardAppMainType;
			_DATE &validDate =cpuCard.applicationData.ValidDate;
			m_DataModel.orignalValidate = _DATE_TIME(validDate.wYear,validDate.biMonth,validDate.biDay,23,59,59);
			m_DataModel.balance = cpuCard.applicationData.Balance;
		}
		theACC_TICKET.GetTicketInfo(m_DataModel.ticketType,m_DataModel.ticketTypeDetail);
		theACC_TICKET.GetTicketCategry(m_DataModel.ticketType,m_DataModel.cardCategory);
		// 跳转至充值基本画面
		DoDialogFlow(WM_TO_BASE_DLG); 	
	}
	// 异常捕获
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      激活处理

@param      无

@retval  	0:处理成功  非零:失败信息

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CRefreshSvc::DoRefresh()
{
	try {
		INT_PTR  result = CMessageBox::Show(_T("车票信息"),m_DataModel.ticketTypeDetail.ticketTypeName_cn + _T("激活，是否确认？")
			,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		if(result== IDCANCEL)
		{
			return 0;
		}
		RW_CMD_ACTIVE rerfeshInput;
		RW_BUSSINESS_RESPONSE response;
		long errCode = m_pRWHelper->Refresh(rerfeshInput,response);
		// 写卡成功
		if(RW_RSP_OK == errCode){
			DoDialogFlow(WM_TO_REFRESH_FINISH_DLG);
		}
		else{
			DoDialogFlow(WM_TO_REFRESH_FAILURE_DLG);
			theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode));
		}
		return errCode;
	}
	// 异常捕获
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
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
CRefreshSvc::DataModel& CRefreshSvc::GetDataModel()
{
	return m_DataModel;
}
