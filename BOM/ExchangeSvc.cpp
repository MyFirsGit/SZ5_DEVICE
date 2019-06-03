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
@brief      构造函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeSvc::CExchangeSvc()
      :CBOMForeService(EXCHANGE_SVC)
{
    // 初始化成员变量
	pCpuCard =  new CCPUCard;	                                                     // cpu卡信息结构体
	pUlCard = new CULCard;                                                        // ul卡信息结构体
	m_ExchangeInfoSize = 0;                                               // 站员画面显示信息行数  
	m_TicketType = 0;                                                       // 车票子类型     
	m_isOldCard = true;                                                        // 旧卡标志
	m_TicketType=0;                                                       // 车票子类型     
	m_strOldNum="";
	m_strNewNum="";
	m_strDeposit="";
	m_ExchangeInfoSize = InitExchangeInfo();

    // 设置操作员画面
    m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
    m_pDialogFlow->AddDialog(IDD_00501_EXCHANGE_BASE_DLG, new CExchangeBaseOfficeDlg(this));
    m_pDialogFlow->AddDialog(IDD_00502_EXCHANGE_BACKUP_DLG, new CExchangeBackupDlg(this));

    // 设置乘客画面
    m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));
    m_pDialogFlow->AddDialog(IDD_10501_EXCHANGE_DLG, new CExchangeReceptionDlg(this));

    // 设置画面分组
    // 等待读卡画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_PUT_CARD));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_10002_WELCOME_DLG);

    // 替换基本画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_CONFIRM), 
                                                     theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_CONFIRM_PASSENGER), 
                                                     theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_CONFIRM_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_00501_EXCHANGE_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_10501_EXCHANGE_DLG);

    // 替换结束画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_FINISH),
                                                       theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_FINISH_PASSENGER), 
                                                       theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_FINISH_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_00501_EXCHANGE_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_10501_EXCHANGE_DLG);

    //// 替换备份画面
    //m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BACKUP, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_BACKUP),
    //                                                   theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_CONFIRM_PASSENGER), 
    //                                                   theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_CONFIRM_PASSENGER));
    //m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BACKUP,IDD_00502_EXCHANGE_BACKUP_DLG);
    //m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BACKUP,IDD_10501_EXCHANGE_DLG);

    //// 替换备份结束画面
    //m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BACKUP_FINISH, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_FINISH), 
    //                                                          theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_FINISH_PASSENGER), 
    //                                                          theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_FINISH_PASSENGER));
    //m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BACKUP_FINISH,IDD_00502_EXCHANGE_BACKUP_DLG);
    //m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BACKUP_FINISH,IDD_10501_EXCHANGE_DLG);

    // 替换错误结束画面
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_EXCHANGE_NOT, theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_NOT), 
                                                              theGUIDE_INFO.GetPassengerMessageCn(GUIDE_EXCHANGE_NOT_PASSENGER), 
                                                              theGUIDE_INFO.GetPassengerMessageEn(GUIDE_EXCHANGE_NOT_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_EXCHANGE_NOT,IDD_00501_EXCHANGE_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_EXCHANGE_NOT,IDD_10501_EXCHANGE_DLG);

    // 设置画面流转
    m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_EXCHANGE_BASE_DLG, DIALOG_GROUP_BASE);
    m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_TO_EXCHANGE_NOT_DLG,DIALOG_GROUP_EXCHANGE_NOT);

    m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_EXCHANGE_FINISH_DLG,DIALOG_GROUP_FINISH);
    m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_EXCHANGE_WAIT_DLG,  DIALOG_GROUP_WAIT);
    m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_TO_EXCHANGE_NOT_DLG,DIALOG_GROUP_EXCHANGE_NOT);

    //m_pDialogFlow->AddFlow(DIALOG_GROUP_BACKUP, WM_TO_EXCHANGE_NOT_DLG,DIALOG_GROUP_EXCHANGE_NOT);
    //m_pDialogFlow->AddFlow(DIALOG_GROUP_BACKUP, WM_TO_EXCHANGE_FINISH_DLG,DIALOG_GROUP_BACKUP_FINISH);
    //m_pDialogFlow->AddFlow(DIALOG_GROUP_BACKUP, WM_TO_EXCHANGE_WAIT_DLG,DIALOG_GROUP_WAIT);
    //


    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);          // 跳转至替换等待画面
 
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置读新卡

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
@brief      析构函数

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
@brief      初始化车票基本信息

@param      void
@retval     void
@exception  void
*/
//////////////////////////////////////////////////////////////////////////
UINT CExchangeSvc::InitExchangeInfo()
{
	// 票面卡号
	UINT i = 0;
	m_ExchangeInfo[i++].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 车票种类
	m_ExchangeInfo[i++].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 有效期
	m_ExchangeInfo[i++].nameLeft = add_wsp(CString(TXT_TICKET_VALIDATE_DATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 卡内余额 	
	m_ExchangeInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_ExchangeInfo[i++].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	return i;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置乘客画面信息

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
UINT CExchangeSvc::SetReceptionInfo()
{
	// 车票编号
	m_TicketInfoGroup.clear();
	MSG_NAME_GROUP msg_tmp;
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_NUMBER,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_NUMBER_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(m_cardInfo.cardNum,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	m_TicketInfoGroup.push_back(msg_tmp);

	// 票卡类型
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(m_cardInfo.cardNameCn,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(m_cardInfo.cardNameEn,MAX_CASH_RIGHT_LEN);
	m_TicketInfoGroup.push_back(msg_tmp);
	// 有效期
	msg_tmp.nameLeft.nameChinese = add_wsp(TXT_TICKET_VALIDATE_DATE,MAX_SBC_CASE_LEN);
	msg_tmp.nameLeft.nameEnglish = add_wsp(TXT_TICKET_VALIDATE_DATE_EN,MAX_SBC_CASE_LEN);
	msg_tmp.nameRight.nameChinese = add_hsp_right(m_cardInfo.vilidateTime,MAX_CASH_RIGHT_LEN);
	msg_tmp.nameRight.nameEnglish = add_hsp_right(_T(""),MAX_CASH_RIGHT_LEN);
	m_TicketInfoGroup.push_back(msg_tmp);
	// 票内余额
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
@brief      返回乘客画面信息

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
@brief      设置站员画面

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeSvc::SetBaseDlg()
{
	try{
		// 票面卡号
		UINT i = 0;
		m_ExchangeInfo[i++].nameRight = m_cardInfo.cardNum;
		// 车票种类名称
		m_ExchangeInfo[i++].nameRight = m_cardInfo.cardNameCn;
		// 有效期
		m_ExchangeInfo[i++].nameRight = m_cardInfo.vilidateTime;
		// 卡内余额 	
		if(m_category==MULTIRIDE) {
			m_ExchangeInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_RIDES) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		}else{
			m_ExchangeInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		}
		m_ExchangeInfo[i++].nameRight = m_cardInfo.balance;
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得车票基本信息的行数

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
@brief      获得车票基本信息

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
@brief     读取车票信息

@param      (i)  UINT errCode              错误代码
@param      (i)  CRWReadCard* rwReadCard   读卡命令

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
			// 黑名单卡
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
			// 操作取消
			else if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD){	
				guideInfo = theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL);
			}
			// 其他错误
			else{
				// 旧卡
				if(m_isOldCard){	
					// 转到不可画面
					theSERVICE_MGR.SetState(FINISH);
					DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG); 	
					guideInfo = m_pRWHelper->GetTpuErrorInfo(cardRspInfo.DetailErrorCode);
				}
				// 新卡
				else{
					StartReadCard(guideInfo);
				}			
			}
			theAPP_SESSION.ShowOfficeGuide(guideInfo);		
		}		
		// 读卡成功
		if (RW_RSP_OK == errCode) {
			// 获得读卡信息
			if (SaveCardInfo(cardRspInfo))
			{
				// 设置站员画面
				SetBaseDlg();
				// 设置乘客画面
				SetReceptionInfo();
				if(m_isOldCard){
					// 跳转至基本画面
					DoDialogFlow(WM_TO_EXCHANGE_BASE_DLG); 	
					guideInfo =  theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_CONFIRM); 
				}
				// 新卡
				else{
					DoNewCardExchange();
				}
			}			
		}
		if(guideInfo != ""){
			theAPP_SESSION.ShowOfficeGuide(guideInfo);	
		}
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
@brief      保存卡数据

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CExchangeSvc::SaveCardInfo(RW_READ_CARD_RESPONSE& RWReadCardResponse)
{
	// UL票卡
	if(RWReadCardResponse.CardPHType == ULTRALIGHT) {
		// 解析UL卡数据
		pUlCard->GetCardInfo(RWReadCardResponse.CardInfo);
		// 票卡主类型
		m_TicketType = pUlCard->applicationData.CardAppMainType;
		// 卡逻辑编号
		m_cardInfo.cardNum.Format(_T("%.9u"),pUlCard->applicationData.CardLogicalNumber);
		if (m_isOldCard)
		{
			m_strOldNum = m_cardInfo.cardNum;
		}else
		{
			m_strNewNum = m_cardInfo.cardNum;
		}
		// 票种名称
		CAccTicketParam::TICKET_TYPE ticketTypeDetail;
		theACC_TICKET.GetTicketInfo(m_TicketType,ticketTypeDetail);
		m_cardInfo.cardNameCn = ticketTypeDetail.ticketTypeName_cn;
		m_cardInfo.cardNameEn = ticketTypeDetail.ticketTypeName_en;
		m_cardInfo.vilidateTime = pUlCard->applicationData.ValidEndTime.ToString(_T("%.4d年%.2d月%.2d日"));	

		// 产品种类
		theACC_TICKET.GetTicketCategry(m_TicketType,m_category);
		// 计次
		if(m_category==MULTIRIDE) {
			m_cardInfo.balance.Format(_T("%d次"),pUlCard->applicationData.Balance);
			m_strDeposit.Format(_T("%d次"),pUlCard->applicationData.Deposit);
		}
		// 钱包 
		else if(m_category==PURSE){
			m_cardInfo.balance = ComFormatAmount(pUlCard->applicationData.Balance)+MONEY_UNIT;
			m_strDeposit = ComFormatAmount(pUlCard->applicationData.Deposit)+MONEY_UNIT;
		}
		// 定期
		else if(m_category==PERIOD){
			m_cardInfo.balance = TXT_NOTHING;
		}
		else{
			// 跳转至基本画面
			DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG); 	
			return false;
		}
		
	}
	// CPU卡
	else if (RWReadCardResponse.CardPHType == CPU_TICKET){		
		// 解析UL卡数据
		pCpuCard->GetCardInfo(RWReadCardResponse.CardInfoLen,RWReadCardResponse.CardInfo);
		// 票卡主类型
		m_TicketType = pCpuCard->applicationData.CardAppMainType;
		// 卡逻辑编号
		m_cardInfo.cardNum.Format(_T("%.9u"),pCpuCard->applicationData.CardLogicalNumber);
		if (m_isOldCard)
		{
			m_strOldNum = m_cardInfo.cardNum;
		}else
		{
			m_strNewNum = m_cardInfo.cardNum;
		}
		m_cardInfo.vilidateTime = pCpuCard->applicationData.ValidDate.ToString(_T("%.4d年%.2d月%.2d日"));	
		// 票种名称
		CAccTicketParam::TICKET_TYPE ticketTypeDetail;
		theACC_TICKET.GetTicketInfo(m_TicketType,ticketTypeDetail);
		m_cardInfo.cardNameCn = ticketTypeDetail.ticketTypeName_cn;
		m_cardInfo.cardNameEn = ticketTypeDetail.ticketTypeName_en;
		// 产品种类
		theACC_TICKET.GetTicketCategry(m_TicketType,m_category);
		// 计次
		if(m_category==MULTIRIDE) {
			m_cardInfo.balance.Format(_T("%d次"),pCpuCard->applicationData.Balance);
			m_strDeposit.Format(_T("%d次"),pCpuCard->applicationData.Deposit);
		}
		// 钱包 
		else if(m_category==PERIOD){
			m_cardInfo.balance = ComFormatAmount(pCpuCard->applicationData.Balance)+MONEY_UNIT;
			m_strDeposit = ComFormatAmount(pCpuCard->applicationData.Deposit)+MONEY_UNIT;
		}
		// 定期
		else if(m_category==PERIOD){
			m_cardInfo.balance = TXT_NOTHING;
		}
		else{
			// 跳转至基本画面
			DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG); 	
			return false;
		}
	}
	else
	{
		// 跳转至基本画面
		DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG); 	
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      一卡通充值处理(储值) 

@param      无

@retval  	0:处理成功  非零:失败信息

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeSvc::DoOldCardExchange()
{
	try{
		//写卡
		RW_CMD_REPLACE_CARD replaceInput;
		replaceInput.Serialnum = theCOUNT_INFO.GetUDSNForDisplay();
		replaceInput.TranDateTime = ComGetCurTime();
		
		RW_BUSSINESS_RESPONSE response;
		long errCode = m_pRWHelper->ReplaceOldCard(replaceInput,response);
		// 写卡成功
		if(RW_RSP_OK == errCode){
			// 更新交易流水号
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	

			// 更新UD数据
			if(response.UDlen != 0){
				m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
			}

			// 更新AR数据
			if(response.ARLen != 0){
				m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,m_TicketType);
			}

			// sc审计
			SaveSCData();

			m_isOldCard = false;
			// 读新卡
			StartReadCard();			
		}
		else{
			// 跳转至错误画面
			DoDialogFlow(WM_TO_EXCHANGE_NOT_DLG);   
			theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode));
		}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      一卡通充值处理

@param      无

@retval  	long         错误信息
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CExchangeSvc::DoNewCardExchange()
{
	try{
		//写卡
		RW_CMD_REPLACE_CARD replaceInput;
		replaceInput.Serialnum = theCOUNT_INFO.GetUDSNForDisplay() ;
		replaceInput.TranDateTime = ComGetCurTime();

		RW_BUSSINESS_RESPONSE response;
		long errCode = m_pRWHelper->ReplaceNewCard(replaceInput,response);
		// 写卡成功
		if(RW_RSP_OK == errCode){
			// 更新交易流水号
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	

			// 更新UD数据
			if(response.UDlen != 0){
				m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
			}

			// 更新AR数据
			if(response.ARLen != 0){
				m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,m_TicketType);
			}

			// sc审计
			SaveSCData();
			
			//打印
			//if (!IsPrinterException())
			//{
				PrinterPrint();  
			//}
			
			// 跳转至结束画面
			DoDialogFlow(WM_TO_EXCHANGE_FINISH_DLG);    
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_EXCHANGE_FINISH)); 
		}
		else{
			// 继续读新卡
			StartReadCard(m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode));
		}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      保存一卡通sc数据

@param      无

@retval  	long         错误信息
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
@brief      判断打印机是否正常

@param      无

@retval     bool true:  false:异常正常

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//bool CExchangeSvc::IsPrinterException() 
//{
//	return m_pPRTHelper->IsPrinterHasException();
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印数据、打开钱箱

@param      无

@retval     无

@exception  无
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
			
				// 计次
				if(m_category==MULTIRIDE) {
					line.Replace(_T("卡内余额"),_T("卡内余次"));
				}
				// 定期 
				else if(m_category==PERIOD){
					if (line.Find(_T("卡内余额")))
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
