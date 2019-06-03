#include "stdafx.h"
#include "CounteractSvc.h"
#include "CounteractConfirmOfficeDlg.h"
#include "waitingofficedlg.h"
#include "tickethelper.h"
#include "guideinfo.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCounteractSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCounteractSvc::CCounteractSvc()
    :CBOMForeService(COUNTERACT_SVC)
{

	// 初始化画面基本信息区
	m_counteractConfirmInfoSize = InitCounteractConfirmInfo();

	m_pDialogFlow->AddDialog(IDD_01102_COUNTERACT_CONFIRM_DLG, new CCounteractConfirmOfficeDlg(this));
	
	// 设置乘客画面

	// 设置画面分组
    // 等待读卡画面
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT, theGUIDE_INFO.GetOperationMessage(GUIDE_CANCEL_PUT_CARD));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_01102_COUNTERACT_CONFIRM_DLG);
	
    // 确认画面
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CONFIRM, theGUIDE_INFO.GetOperationMessage(GUIDE_CANCEL_CONFIRM));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CONFIRM, IDD_01102_COUNTERACT_CONFIRM_DLG);
	
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR,_T(""));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_01102_COUNTERACT_CONFIRM_DLG);


	// 设置画面流转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_COUNTERACT_WAIT_TO_CONFIRM, DIALOG_GROUP_CONFIRM);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_COUNTERACT_WAIT_TO_ERROR, DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CONFIRM,WM_COUNTERACT_CONFIRM_TO_ERROR,DIALOG_GROUP_ERROR);
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCounteractSvc::~CCounteractSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		启动抵消操作    

@param		无    

@retval		无   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::OnStart()
{
	__super::OnStart();
	StartReadCard();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::OnOK()
{
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_CONFIRM)
	{
		Counteract();
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		抵消操作      

@param		无     

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::Counteract()
{
	RW_CMD_COUNTERACT rwCmdCounteract;
	rwCmdCounteract.CardType = m_RWReadCardResponse.CardPHType;
	rwCmdCounteract.ProductyType = m_ULCard.applicationData.CardAppMainType;
	rwCmdCounteract.ReasonCode = 0x01;
	rwCmdCounteract.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
	rwCmdCounteract.TranDateTime = ComGetCurTime();
	RW_BUSSINESS_RESPONSE rwBusinessResponse;
	long counteractResult = m_pRWHelper->Counteract(rwCmdCounteract,rwBusinessResponse);
	if(counteractResult!=RW_RSP_OK)
	{
		theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(rwBusinessResponse.DetailErrorCode));
		m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_CONFIRM_TO_ERROR);
		//theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(rwBusinessResponse.DetailErrorCode));
		return;
	}
	theCOUNT_INFO.UpdateUDSN(rwBusinessResponse.SerialAddnum);
	if(rwBusinessResponse.UDlen>0)
	{
		m_pRWHelper->SaveRWUDData(rwBusinessResponse.UDlen,rwBusinessResponse.UDData);
	}
	if(rwBusinessResponse.ARLen>0)
	{
		m_pRWHelper->SaveRWARData(rwBusinessResponse.ARLen,rwBusinessResponse.ARData,m_ULCard.applicationData.CardAppMainType);
	}
	
	SaveBusinessData(&rwBusinessResponse);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_CANCEL_CARD_NOT_FOUND));
	m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_CONFIRM_TO_ERROR);
	theSERVICE_MGR.SetState(FINISH);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		保存业务数据      

@param      (i)LPRW_BUSSINESS_RESPONSE lpRWBusinessResponse		业务读写器反馈信息

@retval		无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::SaveBusinessData(LPRW_BUSSINESS_RESPONSE lpRWBusinessResponse)
{
	

	SCAuditKey purchaseKey;
	purchaseKey.serviceID = COUNTERACT_SVC;
	purchaseKey.cardIssuancer = ACC;
	purchaseKey.ticketType = TICKET_SINGLE_RIDE;

	SCAuditValue purchaseValue;
	purchaseValue.transCount = 1;
	purchaseValue.transAmount = 0-m_ULCard.applicationData.Balance;


	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化确认画面车票信息数据构成

@param      none

@retval     画面Label编号

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
UINT CCounteractSvc::InitCounteractConfirmInfo()
{
	int i = 0;
	// 车票编号
	m_counteractConfirmInfo[i++].nameLeft = add_wsp(TXT_TICKET_NUMBER,MAX_SBC_CASE_LEN);
	// 车票种类
	m_counteractConfirmInfo[i++].nameLeft = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
	// 有效期
	m_counteractConfirmInfo[i++].nameLeft = add_wsp(TXT_TICKET_VALIDATE_DATE,MAX_SBC_CASE_LEN);
	// 票内余额
	m_counteractConfirmInfo[i++].nameLeft = add_wsp(TXT_TICKET_AMOUNT,MAX_SBC_CASE_LEN);
	return i;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新抵消确认信息

@param      (o)CULCard & ulCard		UL卡信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::UpdateCounteractConfirmInfo(CULCard & ulCard)
{
	int i = 0;
	// 车票编号
	m_counteractConfirmInfo[i++].nameRight.Format(_T("%.9u."),ulCard.applicationData.CardLogicalNumber);
	// 车票种类
	CAccTicketParam::TICKET_TYPE ticketTypeDetail;
	theACC_TICKET.GetTicketInfo(ulCard.applicationData.CardAppMainType,ticketTypeDetail);
	m_counteractConfirmInfo[i++].nameRight = ticketTypeDetail.ticketTypeName_cn;
	// 有效期
	m_counteractConfirmInfo[i++].nameRight = ulCard.applicationData.ValidEndTime.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
	// 票内余额
	m_counteractConfirmInfo[i++].nameRight = ComFormatAmount(ulCard.applicationData.Balance) + MONEY_UNIT;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		获得抵消确认信息大小      

@param      无

@retval     抵消确认信息大小

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CCounteractSvc::GetCounteractConfirmInfoSize()
{
	return m_counteractConfirmInfoSize;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得确认画面车票信息

@param      无

@retval     PLABEL_GROUP

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP CCounteractSvc::GetCounteractConfirmInfo()
{
	return m_counteractConfirmInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		读取卡片信息    

@param      (i)UINT  resultCode			错误码
			(i)CRWReadCard * pRWReadCard	卡片读写器指针

@retval     none

@exception  卡片读写异常
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::OnReadCard(UINT resultCode, CRWReadCard * pRWReadCard)
{
	CMessageBox::End();
	auto_ptr<RW_READ_CARD_RESPONSE> pRWReadCardResponse(new RW_READ_CARD_RESPONSE);
	pRWReadCard->GetRWResponse(pRWReadCardResponse.get());
	if(resultCode!=RW_RSP_OK)
	{
		if(pRWReadCardResponse->DetailErrorCode != RW_ERROR_CARD_NO_CARD)
		{
			OnCardError(m_pRWHelper->GetTpuErrorInfo(resultCode));
			return;
		}
		OnCardError(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL));
		return;
	}
	try
	{
		memcpy(&m_RWReadCardResponse,pRWReadCardResponse.get(),sizeof(RW_READ_CARD_RESPONSE));
		m_ULCard.GetCardInfo(pRWReadCardResponse->CardInfo);
		if(!thePRE_TK.CheckIsPreTicket(m_ULCard.applicationData.CardLogicalNumber))
		{
			OnCardError(theGUIDE_INFO.GetOperationMessage(GUIDE_CANCEL_NOT_PRE_SELL));
			return;
		}
		UpdateCounteractConfirmInfo(m_ULCard);
		m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_WAIT_TO_CONFIRM);
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		开始读卡  

@param		无

@retval		无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::StartReadCard()
{
	theSERVICE_MGR.SetState(ISSUEING);
	m_pRWHelper->ReadCard(true);
	INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)this->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
	if(ret == IDCANCEL)
	{
		m_pRWHelper->StopReadCard();
		theSERVICE_MGR.SetState(FINISH);
		//m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_WAIT_TO_ERROR);
		//// 显示向导标题
		theAPP_SESSION.ShowAllGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CANCEL));	
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		读卡错误响应函数    

@param      (i)CString errorMsg		错误信息

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::OnCardError(CString errorMsg)
{
	theSERVICE_MGR.SetState(FINISH);
	m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_WAIT_TO_ERROR);
	theAPP_SESSION.ShowOfficeGuide(errorMsg);
}