#include "stdafx.h"
#include "TVMForeService.h"
#include "MessageIDDefinition.h"
#include "ExceptionMgr.h"
#include "InnerException.h"
#include "CursorMgr.h"
#include "CTCoinCountInfo.h"
#include "CTBanknoteCountInfo.h"
#include "LogoffHelper.h"

BEGIN_MESSAGE_MAP(CTVMForeService,CForeService)
	ON_COMMAND_COMPLETE(RW_COMMAND_ID_READ_CARD,OnReadCardEx)
	ON_COMMAND_COMPLETE(COIN_MODULE_COMMAND_ID_READ_COIN_AMOUNT,OnCoinInsertedEx)
	ON_COMMAND_COMPLETE(COIN_MODULE_COMMAND_ID_CHANGE,OnCoinChangedEx)
	ON_COMMAND_COMPLETE(COIN_MODULE_COMMAND_ID_ACCEPT,OnCoinReceivedEx)
	ON_COMMAND_COMPLETE(COIN_MODULE_COMMAND_ID_RETURN,OnCoinReturnedEx)
	ON_COMMAND_COMPLETE(BANKNOTE_MODULE_COMMAND_ID_INSERT,OnBanknoteInsertedEx)
	ON_COMMAND_COMPLETE(BANKNOTE_MODULE_COMMAND_ID_RETURN_ALL,OnBanknoteReturnedAllEx)
	ON_COMMAND_COMPLETE(BANKNOTE_MODULE_COMMAND_ID_RETURN_BUFF,OnBanknoteReturnedBuffEx)
	ON_COMMAND_COMPLETE(BANKNOTE_MODULE_COMMAND_ID_COLLECT,OnBanknoteCollectEx)
	ON_COMMAND_COMPLETE(BANKNOTE_CHANGE_COMMAND_ID_SEND_OUT,OnBanknoteChangeSendOutEx)
	ON_COMMAND_COMPLETE(CHARGE_INSERT_CARD_CMD_ID_STATUS,OnChargeCardInsertEx)
	//ON_COMMAND_COMPLETE(BANKCARD_COMMAND_ID_READ_STATE,OnBankcardInsertedEx)
	//ON_COMMAND_COMPLETE(BANKCARD_COMMAND_ID_RETURN,OnBankcardReturnedEx)
	//ON_COMMAND_COMPLETE(PINPAD_COMMAND_ID_READ_READ_STATE,OnPinPadPressedEx)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_DOT,OnKeyboardDot)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_CLEAR,OnKeyboardClear)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_ESC,OnKeyboardEsc)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F1,OnKeyboardF1)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F2,OnKeyboardF2)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F4,OnKeyboardF4)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F5,OnKeyboardF5)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F6,OnKeyboardF6)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief          构造函数

@param      (i)SERVICE_ID serviceID              当前serviceID
@param      (i)SERVICE_ID parentServiceID    父serviceID

@retval        none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMForeService::CTVMForeService(SERVICE_ID serviceID, SERVICE_ID parentServiceID) :CForeService(serviceID,parentServiceID)
{
	m_isSecondary = false;
	 m_support_page = false;
	 m_per_page = 0;
	 m_total_page = 0;
	 m_current_page = 0;

	 // 初始化临界区对象
	 ::InitializeCriticalSection(&m_critical_Section);

	 m_pDialogFlow = new CTVMDialogFlow(*this);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief			析构函数

@param		none

@retval			none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMForeService::~CTVMForeService()
{
	delete m_pDialogFlow;
	m_pDialogFlow = NULL;

	// 删除临界区对象
	::DeleteCriticalSection(&m_critical_Section);

	// 维护框架设置业务状态
	if (theSERVICE_MGR.GetState() == MAINTENANCE || theSERVICE_MGR.GetState() == WAIT_MAINTENANCE|| theSERVICE_MGR.GetState() == INIT)
	{
		theAPP_SESSION.SetKeyboard(KEY_RETURN);		// 禁用维护键盘
		theAPP_SESSION.SetMaxInputLength(0);		// 重置输入长度
		theAPP_SESSION.SetGuideWhenDelete(_T(""));	// 重置删除文言
		theAPP_SESSION.DeleteAllInput();			// 删除所有输入内容
		theAPP_SESSION.AppendMenuLevel(_T(""));		// 清空菜单导航的追加级别
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service启动后处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnStart()
{
	try {
		theCURSOR_MGR.EndWaitCursor();
		if(NULL == m_pDialogFlow){
			return;
		}
		m_pDialogFlow->ShowFirstDialogGroup();
		// 隐藏父Service的Dialog
		if (GetServiceID() != MAIN_SVC && GetParentServiceID() != MAIN_SVC) {
			if(NULL != GetParent())((CTVMForeService*)GetParent())->HideCurDialogGroup();
		}

		// 维护框架设置业务状态
		if (theSERVICE_MGR.GetState() == MAINTENANCE || theSERVICE_MGR.GetState() == INIT || theSERVICE_MGR.GetState() == WAIT_MAINTENANCE)
		{
			theAPP_SESSION.SetMenuLevel((SERVICE_ID)GetServiceID());
		}
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
@brief      Service停止前处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::OnStop()
{
	try {
		// 隐藏父Service的当前Dialog组
		if (GetServiceID() != MAIN_SVC && GetParentServiceID() != MAIN_SVC) {
			if(NULL != GetParent())((CTVMForeService*)GetParent())->ShowCurDialogGroup();
		}
		
		if(NULL != m_pDialogFlow)
			m_pDialogFlow->HideAllDialog();

		return true;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CTVMDialogFlow* CTVMForeService::GetDialogFlow()
{
	return (CTVMDialogFlow*)m_pDialogFlow;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      处理画面跳转按钮消息

@param      (i)const char* pButtonMsg 按钮按下发送的附加消息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::DoDialogFlow(const TCHAR* pButtonMsg)
{
	if (m_pDialogFlow->IsFlowButton(pButtonMsg)) {
		m_pDialogFlow->ShowNextDialogGroup(pButtonMsg);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当前Dialog组编号

@param      无

@retval     UINT    当前Dialog组编号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CTVMForeService::GetCurDialogGroupID()
{
	return m_pDialogFlow->GetCurDialogGroupID();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
COperationDlg* CTVMForeService::GetCurOperationDlg()
{
	return GetDialogFlow()->GetCurOperationDlg();
}												 

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CReceptionDlg* CTVMForeService::GetCurReceptionDlg()
{
	return GetDialogFlow()->GetCurReceptionDlg();
}												 

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取前一组Dialog组编号

@param      无

@retval     UINT    前一组Dialog组编号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CTVMForeService::GetPreDialogGroupID()
{
	return m_pDialogFlow->GetPreDialogGroupID();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示当前Dialog组

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::ShowCurDialogGroup()
{
	m_pDialogFlow->ShowDialogGroup(GetCurDialogGroupID());
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏当前Dialog组

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::HideCurDialogGroup()
{
	m_pDialogFlow->HideDialogGroup(GetCurDialogGroupID());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     服务中数据对象发生变化，通知窗体更新 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::NotifyDataModelChanged()
{
	((CTVMDialogFlow*)m_pDialogFlow)->NotifyCurrentDialogGroupUpdate();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBusy(bool busy)
{
	if(busy) {
		//theCURSOR_MGR.BeginWaitCursor();
	}else
	{
		//theCURSOR_MGR.EndWaitCursor();
	}

	COperationDlg* pOperationDlg = GetDialogFlow()->GetCurOperationDlg();
	if (pOperationDlg != NULL) {
		pOperationDlg->OnServiceBusy(busy);
	}
	CReceptionDlg* pReceptionDlg = GetDialogFlow()->GetCurReceptionDlg();
	if (pReceptionDlg != NULL) {
		pReceptionDlg->OnServiceBusy(busy);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读卡回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnReadCardEx(UINT result, CCommand * command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
	{
		theSERVICE_MGR.SetState(RECEPT);	
	}else
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CRWReadCard* rwReadCard = dynamic_cast<CRWReadCard*>(command);
	if(RECHARGERW_HELPER->IsExceptionCode(result))
	{
		theEXCEPTION_MGR.ProcessException(CRWHardwareException(TVM_CARD_RW,result, _T(__FILE__), __LINE__));
		//return;
	}
	if(rwReadCard!=NULL)
	{	
		OnReadCard(result,rwReadCard);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币投入回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinInsertedEx(UINT result, CCommand * command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
	{
		theSERVICE_MGR.SetState(RECEPT);	
	}else
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CCHReadReceivs* chReadReceivs = dynamic_cast<CCHReadReceivs*>(command);
	//if(COIN_HELPER->IsExceptionCode(result))
	//{
	//	theEXCEPTION_MGR.ProcessException(CRWHardwareException(TVM_RECHARGE_RW,result, _T(__FILE__), __LINE__));
	//	return;
	//}
	if(chReadReceivs!=NULL)
	{	
		OnCoinInserted(result,chReadReceivs);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币找零回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinChangedEx(UINT res,CCommand* command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
	{
		theSERVICE_MGR.SetState(RECEPT);	
	}else
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CCHChange* chChange = dynamic_cast<CCHChange*>(command);
	//if(COIN_HELPER->IsExceptionCode(result))
	//{
	//	theEXCEPTION_MGR.ProcessException(CRWHardwareException(TVM_RECHARGE_RW,res, _T(__FILE__), __LINE__));
	//	return;
	//}
	if(chChange!=NULL)
	{	
		OnCoinChanged(res,chChange);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币收纳回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinReceivedEx(UINT res,CCommand* command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
	{
		//theSERVICE_MGR.SetState(RECEPT);	
	}else
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CCHAccept* chAccept = dynamic_cast<CCHAccept*>(command);
	//if(COIN_HELPER->IsExceptionCode(result))
	//{
	//	theEXCEPTION_MGR.ProcessException(CRWHardwareException(TVM_RECHARGE_RW,res, _T(__FILE__), __LINE__));
	//	return;
	//}
	if(chAccept!=NULL)
	{	
		OnCoinReceived(res,chAccept);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币退出回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinReturnedEx(UINT res,CCommand* command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
	{
		theSERVICE_MGR.SetState(RECEPT);	
	}else
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CCHRefund* chCancel = dynamic_cast<CCHRefund*>(command);
	//if(COIN_HELPER->IsExceptionCode(result))
	//{
	//	theEXCEPTION_MGR.ProcessException(CRWHardwareException(TVM_RECHARGE_RW,res, _T(__FILE__), __LINE__));
	//	return;
	//}
	if(chCancel!=NULL)
	{	
		OnCoinReturned(res,chCancel);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币投入回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteInsertedEx(UINT res,CCommand* command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
	{
		theSERVICE_MGR.SetState(RECEPT);	
	}else
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CBRHOpenAndValidate* BHOpenAndValidate = dynamic_cast<CBRHOpenAndValidate*>(command);

	// 如果是手动取消，进入回调函数阶段
	/*if(res & DONE_PHASE_ACT_ABORT ){
		goto DONE_PHASE_ABORD;
	}*/

	/*if(res != E_NO_ERROR){
		theEXCEPTION_MGR.ProcessException(CBHHardwareException(res, _T(__FILE__), __LINE__));
		return;
	}*/

//goto 跳转标记
//DONE_PHASE_ABORD:
	if(BHOpenAndValidate!=NULL)
	{	
		OnBanknoteInserted(res,BHOpenAndValidate);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币退出回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteReturnedAllEx(UINT res,CCommand* command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
	{
		theSERVICE_MGR.SetState(RECEPT);	
	}else
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CBRHCashInRollback* bhReturnAll = dynamic_cast<CBRHCashInRollback*>(command);
	if(RECHARGERW_HELPER->IsExceptionCode(res)){
		theEXCEPTION_MGR.ProcessException(CRWHardwareException(TVM_RECHARGE_RW,res, _T(__FILE__), __LINE__));
		return;
	}
	if(bhReturnAll!=NULL)
	{	
		OnBanknoteReturned(res,bhReturnAll);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      缓冲区纸币退出回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteReturnedBuffEx(UINT res,CCommand* command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
	{
		theSERVICE_MGR.SetState(RECEPT);	
	}else
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CBRHCashInRollback* bhReturnAll = dynamic_cast<CBRHCashInRollback*>(command);
	if(RECHARGERW_HELPER->IsExceptionCode(res)){
		theEXCEPTION_MGR.ProcessException(CRWHardwareException(TVM_RECHARGE_RW,res, _T(__FILE__), __LINE__));
		return;
	}
	if(bhReturnAll!=NULL)
	{	
		OnBanknoteReturned(res,bhReturnAll);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币压回收箱回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteCollectEx(UINT res,CCommand* command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE){
		//theSERVICE_MGR.SetState(RECEPT);	
	}else{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CBRHCashInEnd* bhCollect = dynamic_cast<CBRHCashInEnd*>(command);
	if(RECHARGERW_HELPER->IsExceptionCode(res)){
		theEXCEPTION_MGR.ProcessException(CRWHardwareException(TVM_RECHARGE_RW,res, _T(__FILE__), __LINE__));
		return;
	}
	if(bhCollect!=NULL){	
		OnBanknoteCollect(res,bhCollect);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币压回收箱回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteChangeSendOutEx(UINT res,CCommand* command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE){
		//theSERVICE_MGR.SetState(RECEPT);	
	}else{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CBRHDispenseRequest* bhChangeBill = dynamic_cast<CBRHDispenseRequest*>(command);
	
	if(bhChangeBill != NULL){	
		OnBanknoteChangeSendOut(res,bhChangeBill);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      银行卡插入回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CTVMForeService::OnBankcardInsertedEx(UINT uiResult,CCommand* command)
//{
//	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
//	{
//		theSERVICE_MGR.SetState(RECEPT);
//	}else
//	{
//		theSERVICE_MGR.SetForeServiceBusy(false);
//	}
//	CTVMBankCardIsCardInsert* pCardIsInsert= dynamic_cast<CTVMBankCardIsCardInsert*>(command);	
//	if(pCardIsInsert!=NULL)
//	{	
//		OnBankcardInserted(uiResult,pCardIsInsert);		
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      银行卡退出回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CTVMForeService::OnBankcardReturnedEx(UINT res,CCommand*)
//{
//
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      密码键盘按下回调函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CTVMForeService::OnPinPadPressedEx(UINT uiResult,CCommand* command)
//{
//	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
//	{
//		theSERVICE_MGR.SetState(RECEPT);	
//	}else
//	{
//		theSERVICE_MGR.SetForeServiceBusy(false);
//	}
//	CTVMPinPadStartInput* pInput= dynamic_cast<CTVMPinPadStartInput*>(command);
//	if(pInput!=NULL)
//	{	
//		OnPinPadPressed(uiResult,pInput);		
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief   读卡实现函数   

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnReadCard(UINT res, CRWReadCard *)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      接收到硬币回调函数

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinInserted(UINT res,CCHReadReceivs* chReadReceiveAmount)
{
	try{
	//	// 计算投入总额
	//	CH_RSP_ACCEPT_INSERT_INFO chAmountInfo;
	//	chReadReceiveAmount->GetCHResponse(&chAmountInfo);
	//	GetCoinAcceptAmount(chAmountInfo);
	//	
	//	// 更新界面金额信息显示信息
	//	NotifyDataModelChanged();

	//	// 写入数据库相关信息
	//	RenewDbWhileCoinInsert(chAmountInfo);

	//	CAfcTVMOperationParam::BILLBOX_PARAM billboxParam;
	//	theAFC_TVM_OPERATION.GetBillboxParam(billboxParam);

	//	// 判断投入硬币数量是否超限。如果是，关硬币口。
	//	if (GetCoinAcceptCount() >= billboxParam.acceptCoinMaxCnt && issuePayDetailInfo.isCionOpen) {
	//		COIN_HELPER->CH_EndReceive();
	//		issuePayDetailInfo.isCionOpen = false;
	//	}

	//	
	//	// 以下三个操作只会发生一个。已按优先级排列。
	//	// 判断投入金额是否足够
	//	if (IsReceiveEnoughMoney()){
	//		// 如果无需找零 或者 (系统允许找零 并且 判断找零箱是否可以找零 并且 找零数量不超过最大找零数量限制)
	//		if (issuePayDetailInfo.changeAmount == 0
	//			|| ((theTVM_STATUS_MGR.GetChangeMode()&DEVICE_CONTRL_CODE_NO_BILL_CHANGE) == DEVICE_CONTRL_CODE_NO_BILL_CHANGE
	//			&& CalculateChangeCount() 
	//			&& issuePayDetailInfo.ChangeHopper1Count+issuePayDetailInfo.ChangeHopper2Count
	//			   +issuePayDetailInfo.ChangeStockCount <= billboxParam.chargeCoinMaxCnt)) {
	//				   // 关闭硬币口和纸币口，并进入下一步
	//				   if (issuePayDetailInfo.isCionOpen) {
	//					   COIN_HELPER->CH_EndReceive();
	//					   issuePayDetailInfo.isCionOpen = false;
	//				   }
	//				   if (issuePayDetailInfo.isBankNoteOpen) {
	//					   BANKNOTE_HELPER->StopValidation();
	//					   issuePayDetailInfo.isBankNoteOpen = false;
	//				   }
	//				   OnAcceptMoneyOK();
	//		}
	//		// 否则进入找零数量超限，退所有的钱。
	//		else {
	//			OnOverChangeCountLimit();
	//		}
	//	}
	//	// 如果硬币和纸币均达到最大数量，但是总额却不够，退所有的钱。
	//	//IsBothCoinBanknoteMaxCount定义在ChargeSvc和IssueSvc里，因为纸币数量上限函数对于两种情形不同
	//	else if (IsBothCoinBanknoteMaxCount()) {
	//		OnOverMoneyCountLimit();
	//	}
	//	// 继续收钱
	//	else{
	//		COIN_HELPER->Chp_Get_Insert_Info();
	//	}
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币找零回调函数

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinChanged(UINT res,CCHChange* chChange)
{
	//if (res==0){
	//	issuePayDetailInfo.isCoinCharged = true;

	//	// 从硬件读取各票箱数据
	//	CH_COMMON_RSP* m_response;
	//	chChange->GetCHResponse(m_response);

	//	// 读硬币HOPPER箱A的计数信息，更新，并重新写入。
	//	CTCoinCountInfo::COIN_BOX_INFO& boxInfo = theCOIN_COUNT.GetCoinHopperboxAInfo();
	//	boxInfo.ulCurCount = m_response->coin_info.hopper1;
	//	if(!theCOIN_COUNT.SetCoinHopperboxAInfo(boxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("HOPPER箱A的计数信息写入失败"));
	//	}

	//	// 读硬币HOPPER箱B的计数信息，更新，并重新写入。
	//	boxInfo = theCOIN_COUNT.GetCoinHopperboxBInfo();
	//	boxInfo.ulCurCount = m_response->coin_info.hopper2;
	//	if(!theCOIN_COUNT.SetCoinHopperboxBInfo(boxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("HOPPER箱B的计数信息写入失败"));
	//	}

	//	// 读硬币循环HOPPER箱的计数信息，更新，并重新写入。
	//	boxInfo = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//	boxInfo.ulCurCount = m_response->coin_info.stock1;
	//	if(!theCOIN_COUNT.SetCoinHopperboxCInfo(boxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("循环找零HOPPER箱的计数信息写入失败"));
	//	}

	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      压硬币到回收箱和找零箱   

@param      UINT res,CCHAccept*

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinReceived(UINT res,CCHAccept* chAccept)
{
	//if (res==0){
	//	issuePayDetailInfo.isCionCollected = true;
	//	OnReceiveMoneyOK();

	//	// 从硬件读取各票箱数据
	//	CH_COMMON_RSP* m_response;
	//	chAccept->GetCHResponse(m_response);

	//	// 读硬币循环HOPPER箱的计数信息，更新，并重新写入。
	//	CTCoinCountInfo::COIN_BOX_INFO& boxInfo = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//	boxInfo.ulCurCount = m_response->coin_info.stock1;
	//	if(!theCOIN_COUNT.SetCoinHopperboxCInfo(boxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("循环找零HOPPER箱的计数信息写入失败"));
	//	}

	//	// 读硬币回收箱A的计数信息，更新，并重新写入。
	//	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO& collectBoxInfo = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	//	collectBoxInfo.ulCount_1yuan = m_response->coin_info.collect1_1Num;
	//	collectBoxInfo.ulCount_2yuan = 0; 
	//	collectBoxInfo.ulCount_5yuan = 0;
	//	collectBoxInfo.ulCount_5jiao = 0;
	//	if(!theCOIN_COUNT.SetCollectionCoinboxAInfo(collectBoxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱A的计数信息写入失败"));
	//	}

	//	// 读硬币回收箱B的计数信息，更新，并重新写入。
	//	collectBoxInfo = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	//	collectBoxInfo.ulCount_1yuan = m_response->coin_info.collect2_1Num;
	//	collectBoxInfo.ulCount_2yuan = m_response->coin_info.collect2_2Num;
	//	collectBoxInfo.ulCount_5yuan = m_response->coin_info.collect2_5Num;
	//	collectBoxInfo.ulCount_5jiao = m_response->coin_info.collect2_05Num;
	//	if(!theCOIN_COUNT.SetCollectionCoinboxBInfo(collectBoxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱B的计数信息写入失败"));
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      退币回调函数 

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinReturned(UINT res,CCHRefund*)
{
	if (res == 0){
		issuePayDetailInfo.CHReceiveMoney = 0;
		issuePayDetailInfo.PayCoinInfo.clear();
		OnReturnCashOK();
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      接收纸币回调函数

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteInserted(UINT resultCode,CBRHOpenAndValidate* bhReadReceiveAmount)
{
	try{
		// 计算投入总额
		BRH_STATUS bh_status;
		bhReadReceiveAmount->GetResponse(&bh_status);
		GetBankNoteAcceptAmount(bh_status);

		// 更新界面金额信息显示信息
		NotifyDataModelChanged();

		// 写入数据库相关信息
		RenewDbWhileBanknoteInsert(bh_status);

		CAfcTVMOperationParam::BILLBOX_PARAM billboxParam;
		theAFC_TVM_OPERATION.GetBillboxParam(billboxParam);

		// 判断投入纸币数量是否超限。如果是，关纸币口。
		// IsBanknoteMaxCount定义在ChargeSvc和IssueSvc里，因为纸币数量上限函数对于两种情形不同
		if (IsBanknoteMaxCount() && issuePayDetailInfo.isBankNoteOpen) {
			BANKNOTE_HELPER->StopValidation();
			issuePayDetailInfo.isBankNoteOpen = false;
		}

		// 以下四个操作只会发生一个。已按优先级排列。
		// 如果纸币金额超限，退所有的钱，显示相应结束页面。
		// IsBanknoteMaxAmount定义在ChargeSvc和IssueSvc里，因为纸币数量上限函数对于两种情形不同
		if (IsBanknoteMaxAmount()) {
			OnOverMoneyAmountLimit();
		}
		// 判断投入金额是否足够
		else if (IsReceiveEnoughMoney()){
			// 如果无需找零 或者 (系统允许找零 并且 判断找零箱是否可以找零 并且 找零数量不超过最大找零数量限制)
			if (issuePayDetailInfo.changeAmount == 0
				((theTVM_STATUS_MGR.GetChangeMode()&DEVICE_CONTRL_CODE_NO_BILL_CHANGE) == DEVICE_CONTRL_CODE_NO_BILL_CHANGE
				&& CalculateChangeCount() 
				&& issuePayDetailInfo.ChangeHopper1Count+issuePayDetailInfo.ChangeHopper2Count
				+issuePayDetailInfo.ChangeStockCount <= billboxParam.chargeCoinMaxCnt)) {
					// 关闭硬币口和纸币口，并进入下一步
					if (issuePayDetailInfo.isCionOpen) {
						COIN_HELPER->CH_StopInsertInfo();
						COIN_HELPER->CH_EndReceive();
						issuePayDetailInfo.isCionOpen = false;
					}
					if (issuePayDetailInfo.isBankNoteOpen) {
						BANKNOTE_HELPER->StopValidation();
						issuePayDetailInfo.isBankNoteOpen = false;
					}
					OnAcceptMoneyOK();
			}
			// 否则进入找零数量超限，退所有的钱。
			else {
				OnOverChangeCountLimit();
			}
		}
		// 如果硬币和纸币均达到最大数量，但是总额却不够，退所有的钱。
		// IsBothCoinBanknoteMaxCount定义在ChargeSvc和IssueSvc里，因为纸币数量上限函数对于两种情形不同
		else if (IsBothCoinBanknoteMaxCount()) {
			OnOverMoneyCountLimit();
		}
		// 继续收钱
		else {
			//BANKNOTE_HELPER->OpenAndValidateAsync();
			//BANKNOTE_HELPER->HoldAsync(this);
		}
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}		
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币退币   

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteReturned(UINT res,CBRHCashInRollback*)
{
	if (res == 0){
		issuePayDetailInfo.BHReceiveMoney = 0;
		issuePayDetailInfo.PayCoinInfo.clear();
		if (issuePayDetailInfo.CHReceiveMoney==0&&issuePayDetailInfo.BHReceiveMoney==0){
			OnReturnCashOK();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币压回收箱实现函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteCollect(UINT res,CBRHCashInEnd*)
{
	if (res==0){
		issuePayDetailInfo.isBankNoteCollected = true;
		OnReceiveMoneyOK();

		// 读纸币钱箱的计数信息的计数信息，更新，并重新写入。
		/*CTBanknoteCountInfo::BANKNOTE_BOX_INFO& boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
		boxInfo.ulCount_1 += issuePayDetailInfo.PayBankNoteInfo[Banknote1Yuan];
		boxInfo.ulCount_5 += issuePayDetailInfo.PayBankNoteInfo[Banknote5Yuan];
		boxInfo.ulCount_10 += issuePayDetailInfo.PayBankNoteInfo[Banknote10Yuan];
		boxInfo.ulCount_20 += issuePayDetailInfo.PayBankNoteInfo[Banknote20Yuan];
		boxInfo.ulCount_50 += issuePayDetailInfo.PayBankNoteInfo[Banknote50Yuan];
		boxInfo.ulCount_100 += issuePayDetailInfo.PayBankNoteInfo[Banknote100Yuan];
		if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币钱箱的计数信息写入失败"));
		}*/
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币找零回调函数

@param      UINT res;						执行结果
			CBHChangeSendOutBill* command;	结果指针

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteChangeSendOut(UINT res,CBRHDispenseRequest* command){
	// 子类实现
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      银行卡插入实现函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CTVMForeService::OnBankcardInserted(UINT res,CCommand*)
//{
//
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      银行卡退出实现函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CTVMForeService::OnBankcardReturned(UINT res,CCommand*)
//{
//
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      密码键盘按下实现函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CTVMForeService::OnPinPadPressed(UINT res,CTVMPinPadStartInput*)
//{
//
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [0~9] 键按下消息（数字）

@param      (i)WPARAM wParam  数字键值，例如：数字键7，值为7
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/)
{
	theAPP_SESSION.AppendOneNumToGuide(wParam);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [.] 键按下消息（小数点）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardDot(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	theAPP_SESSION.AppendOneDotToGuide();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Clear] 键按下消息（删除）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	bool empty = !theAPP_SESSION.DeleteOneCharFromGuide();
	if (empty)
	{
		CString guidemsg = theAPP_SESSION.GetPrefixGuide();
		if(guidemsg.IsEmpty())
		{
			if (theAPP_SESSION.GetMaintainMenu() == SERVICE_ID::ONE_KEY_SETTLEMENT_SVC
				|| theAPP_SESSION.GetMaintainMenu() == SERVICE_ID::COIN_BOX_MANAGE_SVC
				|| theAPP_SESSION.GetMaintainMenu() == SERVICE_ID::BANKNOTE_BOX_MANAGE_SVC)
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
			}
			else{
				guidemsg = theAPP_SESSION.GetGuideWhenDelete();

				if (guidemsg != _T(""))
				{
					theAPP_SESSION.ShowOfficeGuide(guidemsg);
				}
				else
				{
					CTVMDialogFlow* flow = GetDialogFlow();
					theAPP_SESSION.ShowOfficeGuide(flow->GetCurrentGuideMsg());
				}

			}
		}
		else{
			theAPP_SESSION.SetPrefixToGuide(guidemsg);
		}
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Enter] 键按下消息（执行）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Esc] 键按下消息（主菜单）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	UINT id = theSERVICE_MGR.GetCurService()->GetServiceID();
	switch (id)
	{
	case WAIT_MAINTENANCE_SVC:
		break;
	case LOGIN_SVC:
		//theSERVICE_MGR.StartService(WAIT_MAINTENANCE_SVC);
		break;
	case MAINTENANCE_SVC:
		{
			CLogoffHelper logoffHelper(*this);
			logoffHelper.Logoff();	
		}
		break;
	case ISSUE_SVC:
	case CHARGE_SVC:
		break;
	default:
		theSERVICE_MGR.StartService(MAINTENANCE_SVC);
	}

	theAPP_SESSION.AppendMenuLevel(_T(""));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F1] 键按下消息（未使用）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF1(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F2/Alt] 键按下消息（保存）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F4] 键按下消息（未使用）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF4(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F5] 键按下消息（上一页）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_support_page)
	{
		if (--m_current_page < 1)
			m_current_page = 1;
		NotifyDataModelChanged();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F6] 键按下消息（下一页）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_support_page)
	{
		if (++m_current_page > m_total_page)
			m_current_page = m_total_page;
		NotifyDataModelChanged();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置翻页信息

@param      (i)int per		每页条数
@param      (i)int total	总计页数
@param      (i)int current  当前页码

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::SetPageInfo(int total, int current /* = 1 */, int per /* = 9 */)
{
	m_per_page = per;
	m_total_page = total;
	m_current_page = current;
	m_support_page = true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取每页条数

@param      无

@retval     int

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMForeService::GetPerPage() const
{
	return m_per_page;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置总计页数

@param      无

@retval     int

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMForeService::GetTotalPage() const
{
	return m_total_page;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置当前页数

@param      无

@retval     int

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMForeService::GetCurrentPage() const
{
	return m_current_page;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当前页的前缀条数

@param      无

@retval     int

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMForeService::GetPagePrefix()
{
	return m_per_page * (m_current_page - 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开始收现金

@param      无

@retval     无

@exception  CSysException& e
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::StartReceiveCash()
{
	try{		
		// 当前模式支持接受硬币则开始接收硬币
		if (theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_COIN){			
			COIN_HELPER->CH_StartReceive(this);  
			issuePayDetailInfo.isCionOpen = true;
		}
		// 当前模式支持接受纸币则开始接收纸币
		if (theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			//BANKNOTE_HELPER->OpenAndValidateAsync();
			issuePayDetailInfo.isBankNoteOpen = true;
		}
		theAudio_HELPER.PlayAudio(AUDIO_ID_INSERT_CASH);
		return true;		
	}
	catch (CSysException& e){
		throw e;
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		throw e;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开始读储值卡

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::StartReadEPCard()
{
	try{	
		theSERVICE_MGR.SetState(ISSUEING);
		RECHARGERW_HELPER->ReadCard((SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID(),true);	
		theAudio_HELPER.PlayAudio(AUDIO_ID_SWIPE_EP_CARD);
		issuePayDetailInfo.isWaitingEPCard = true;
	}
	catch (CSysException& e){
		throw e;
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		throw e;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开始接收银行卡

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::StartReceiveBankCard()
{
	try{	
		theSERVICE_MGR.SetState(ISSUEING);
		//BANKCARD_HELPER->TurnOnInsertTrigger(this);
		theAudio_HELPER.PlayAudio(AUDIO_ID_INSERT_BANKCARD);
		issuePayDetailInfo.isBankCardOpen = true;
	}
	catch (CSysException& e){
		throw e;
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		throw e;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      压钞和回收

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::StartAcceptCashMoney()
{
	try{	
		theSERVICE_MGR.SetState(ISSUEING);
		bool res = false;
		// 硬币回收
		COIN_HELPER->CH_AcceptAsync(this);   
		// 纸币回收		
		if (issuePayDetailInfo.BHReceiveMoney>0){
			BANKNOTE_HELPER->Collect();
		}

		//if (issuePayDetailInfo.isCionOpen){
		//	// 关闭硬币口
		//	COIN_HELPER->CH_EndReceive();
		//	issuePayDetailInfo.isCionOpen = false;
		//	// 关口后，硬币模块需要再读一次暂存区。
		//	//??double recvAmount = 0;
		//	//CH_RSP_ACCEPT_INSERT_INFO chAmountInfo;
		//	//COIN_HELPER->Chp_Get_Insert_Info(chAmountInfo);
		//	//issuePayDetailInfo.PayCoinInfo[Coin1MOP] = chAmountInfo.insert_info.escrow1;
		//	//recvAmount += 100*chAmountInfo.insert_info.escrow1;
		//	//issuePayDetailInfo.PayCoinInfo[Coin2MOP] = chAmountInfo.insert_info.escrow2_2;
		//	//recvAmount += 200*chAmountInfo.insert_info.escrow2_2;
		//	//issuePayDetailInfo.PayCoinInfo[Coin5MOP] = chAmountInfo.insert_info.escrow2_5;
		//	//recvAmount += 500*chAmountInfo.insert_info.escrow2_5;
		//	//issuePayDetailInfo.PayCoinInfo[Coin50AVOS] = chAmountInfo.insert_info.escrow2_05;
		//	//recvAmount += 50*chAmountInfo.insert_info.escrow2_05;
		//	//issuePayDetailInfo.CHReceiveMoney = recvAmount; 
		//	// 硬币回收
		//	COIN_HELPER->CH_Accept();   
		//}
		//// 当前模式支持接受纸币则关闭纸币口
		//if (issuePayDetailInfo.isBankNoteOpen){
		//	BANKNOTE_HELPER->StopValidation();
		//	issuePayDetailInfo.isBankNoteOpen = false;
		//	// 纸币回收		
		//	if (issuePayDetailInfo.BHReceiveMoney>0){
		//		BANKNOTE_HELPER->Collect();
		//	}			
		//}		
	}
	catch (CSysException& e){
		throw e;
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		throw e;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::StartChange()
{
	theSERVICE_MGR.SetState(ISSUEING);
	try{	
		// 硬币找零
		//long res = COIN_HELPER->CH_Change(issuePayDetailInfo.ChangeHopper1Count, issuePayDetailInfo.ChangeHopper2Count, issuePayDetailInfo.ChangeStockCount,rsp); 
		return /*res==0?true:*/false;
	}
	catch (CSysException& e){
		throw e;
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		throw e;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      压钞和回收

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnError(CSysException e)
{
	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      退币（纸币硬币缓存区）

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::StartReturnCash()
{
	try{	
		theSERVICE_MGR.SetState(ISSUEING);
		// 当有接收到硬币时
		if (issuePayDetailInfo.CHReceiveMoney>0){
			// 硬币退币
			COIN_HELPER->CH_RefundAsync(this); 
		}
		// 当有接收到纸币时
		if (issuePayDetailInfo.BHReceiveMoney>0){
			// 纸币退币
			BANKNOTE_HELPER->ReturnAll();   
			issuePayDetailInfo.BHReceiveMoney = 0;
			issuePayDetailInfo.PayBankNoteInfo.clear();
		}
	}
	catch (CSysException& e){
		throw e;
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		throw e;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭所有硬件

@param      (i)CString errorMsg  错误信息文言
@param      (i)bool bContinue    是否继续读卡标记

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::CloseAllDevice()
{
	// 关硬币口
	if (issuePayDetailInfo.isCionOpen){
		COIN_HELPER->CH_StopInsertInfo();
		COIN_HELPER->CH_EndReceive();
		issuePayDetailInfo.isCionOpen = false;
	}
	// 关纸币口
	if (issuePayDetailInfo.isBankNoteOpen){
		BANKNOTE_HELPER->StopValidation();
		issuePayDetailInfo.isBankNoteOpen = false;
	}
	// 停止读储值卡
	if (issuePayDetailInfo.isWaitingEPCard){
		RECHARGERW_HELPER->StopReadCard();
		issuePayDetailInfo.isWaitingEPCard = false;
	}

	// 停止读银行卡，退出银行卡
	if (issuePayDetailInfo.isBankCardOpen) {
		issuePayDetailInfo.isBankCardOpen = false;
		//BANKCARD_HELPER->
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取硬币接受金额

@param      无

@retval     硬币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMForeService::GetCoinAcceptAmount(CH_RSP_ACCEPT_INSERT_INFO chAmountInfo)
{
	// this function has two effects:
	// (1) renew issuePayDetailInfo.
	// (2) return a value of overall amount.
	// as all the calculation comes from "chAmountInfo", this function can be run multiple
	// times without error.
	DOUBLE recvAmount = 0;
	issuePayDetailInfo.PayCoinInfo.clear();
	issuePayDetailInfo.PayCoinInfo[Coin1Yuan] = chAmountInfo.insert_info.escrow1;
	recvAmount += 100*chAmountInfo.insert_info.escrow1;
	/*issuePayDetailInfo.PayCoinInfo[Coin2MOP] = chAmountInfo.insert_info.escrow2_2;
	recvAmount += 200*chAmountInfo.insert_info.escrow2_2;
	issuePayDetailInfo.PayCoinInfo[Coin5MOP] = chAmountInfo.insert_info.escrow2_5;
	recvAmount += 500*chAmountInfo.insert_info.escrow2_5;*/
	issuePayDetailInfo.PayCoinInfo[CoinHalfYuan] = chAmountInfo.insert_info.escrow2_05;
	recvAmount += 50*chAmountInfo.insert_info.escrow2_05;
	issuePayDetailInfo.CHReceiveMoney = (DWORD)recvAmount; 
	return issuePayDetailInfo.CHReceiveMoney;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币接受金额

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMForeService::GetBankNoteAcceptAmount(BRH_STATUS bh_status)
{
	// this function has two effects:
	// (1) renew issuePayDetailInfo as a counting info.
	// (2) return a value of overall amount.
	// since "bh_status" only give the information of the very last banknote, 
	// this function can be run ONLY ONCE after inserting one single bill.
	int recvAmount = 0;
	//switch (bh_status.bh_faceValue.faceValue){
	//case Banknote10MOP:
	//	recvAmount = 1000;
	//	break;
	//case Banknote20MOP:
	//	recvAmount = 2000;
	//	break;
	//case Banknote50MOP:
	//	recvAmount = 5000;
	//	break;
	//case Banknote100MOP:
	//	recvAmount = 10000;
	//	break;
	//case Banknote500MOP:
	//	recvAmount = 50000;
	//	break;
	//case Banknote1000MOP:
	//	recvAmount = 100000;
	//	break;
	//default:
	//	// 系统中不接收的面值
	//	recvAmount = 0;
	//}
	//issuePayDetailInfo.PayBankNoteInfo[bh_status.bh_faceValue.faceValue] += 1;
	//issuePayDetailInfo.BHReceiveMoney += recvAmount; 
	//return issuePayDetailInfo.BHReceiveMoney;
	return recvAmount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      计算找零枚数

@param      无

@retval     true 有足够的硬币可成功找零
			false 硬币不够成功找零

@exception  从changeAmount计算，并存入ChangeStockCount/ChangeHopper1Count/ChangeHopper2Count
			此函数依赖币种与金额的对应，采用0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP
			具体参照TVM.INI和Common/Source Files/TVMInfo.cpp，并体现在TransCoinValue中
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::CalculateChangeCount()
{
	//DWORD remainAmount = issuePayDetailInfo.changeAmount;

	//int stockType = theTVM_INFO.GetCHStockMoneyType();
	//int hopper1Type = theTVM_INFO.GetCHHopper1Type();
	//int hopper2Type = theTVM_INFO.GetCHHopper2Type();

	//// 依赖币种与金额的对印，假设币种数字大的金额高。“0：无效”的情形会报错，并返回Value 0。
	//DWORD stockValue = TransCoinValue(stockType);
	//DWORD hopper1Value = TransCoinValue(hopper1Type);
	//DWORD hopper2Value = TransCoinValue(hopper2Type);

	////为0直接返回错误。
	//if (0 == stockValue || 0 == hopper1Value || 0 == hopper2Value)
	//{
	//	return false;
	//}
	//CTCoinCountInfo::COIN_BOX_INFO& stockInfo = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//CTCoinCountInfo::COIN_BOX_INFO& hopper1Info = theCOIN_COUNT.GetCoinHopperboxAInfo();
	//CTCoinCountInfo::COIN_BOX_INFO& hopper2Info = theCOIN_COUNT.GetCoinHopperboxBInfo();

	//// 所有情形均已考虑。由于循环找零箱优先级高，所以是>=。两个大hopper顺序无所谓，所以就随便假设了hopper1优先级比较高。
	//if (stockValue >= hopper1Value && hopper1Value >= hopper2Value) {
	//	issuePayDetailInfo.ChangeStockCount = min((DWORD)stockInfo.ulCurCount, remainAmount/stockValue);
	//	remainAmount -= stockValue*issuePayDetailInfo.ChangeStockCount;

	//	issuePayDetailInfo.ChangeHopper1Count = min((DWORD)hopper1Info.ulCurCount, remainAmount/hopper1Value);
	//	remainAmount -= hopper1Value*issuePayDetailInfo.ChangeHopper1Count;

	//	issuePayDetailInfo.ChangeHopper2Count = min((DWORD)hopper2Info.ulCurCount, remainAmount/hopper2Value);
	//	remainAmount -= hopper2Value*issuePayDetailInfo.ChangeHopper2Count;
	//}
	//else if (stockValue>= hopper2Value && hopper2Value > hopper1Value ) {
	//	issuePayDetailInfo.ChangeStockCount = min((DWORD)stockInfo.ulCurCount, remainAmount/stockValue);
	//	remainAmount -= stockValue*issuePayDetailInfo.ChangeStockCount;
	//		
	//	issuePayDetailInfo.ChangeHopper2Count = min((DWORD)hopper2Info.ulCurCount, remainAmount/hopper2Value);
	//	remainAmount -= hopper2Value*issuePayDetailInfo.ChangeHopper2Count;

	//	issuePayDetailInfo.ChangeHopper1Count = min((DWORD)hopper1Info.ulCurCount, remainAmount/hopper1Value);
	//	remainAmount -= hopper1Value*issuePayDetailInfo.ChangeHopper1Count;
	//}
	//else if (hopper1Value > stockValue && stockValue >= hopper2Value ) {
	//	issuePayDetailInfo.ChangeHopper1Count = min((DWORD)hopper1Info.ulCurCount, remainAmount/hopper1Value);
	//	remainAmount -= hopper1Value*issuePayDetailInfo.ChangeHopper1Count;

	//	issuePayDetailInfo.ChangeStockCount = min((DWORD)stockInfo.ulCurCount, remainAmount/stockValue);
	//	remainAmount -= stockValue*issuePayDetailInfo.ChangeStockCount;

	//	issuePayDetailInfo.ChangeHopper2Count = min((DWORD)hopper2Info.ulCurCount, remainAmount/hopper2Value);
	//	remainAmount -= hopper2Value*issuePayDetailInfo.ChangeHopper2Count;
	//}
	//else if (hopper2Value > stockValue && stockValue >= hopper1Value ) {
	//	issuePayDetailInfo.ChangeHopper2Count = min((DWORD)hopper2Info.ulCurCount, remainAmount/hopper2Value);
	//	remainAmount -= hopper2Value*issuePayDetailInfo.ChangeHopper2Count;

	//	issuePayDetailInfo.ChangeStockCount = min((DWORD)stockInfo.ulCurCount, remainAmount/stockValue);
	//	remainAmount -= stockValue*issuePayDetailInfo.ChangeStockCount;

	//	issuePayDetailInfo.ChangeHopper1Count = min((DWORD)hopper1Info.ulCurCount, remainAmount/hopper1Value);
	//	remainAmount -= hopper1Value*issuePayDetailInfo.ChangeHopper1Count;
	//}
	//else if (hopper1Value >= hopper2Value && hopper2Value > stockValue ) {
	//	issuePayDetailInfo.ChangeHopper1Count = min((DWORD)hopper1Info.ulCurCount, remainAmount/hopper1Value);
	//	remainAmount -= hopper1Value*issuePayDetailInfo.ChangeHopper1Count;

	//	issuePayDetailInfo.ChangeHopper2Count = min((DWORD)hopper2Info.ulCurCount, remainAmount/hopper2Value);
	//	remainAmount -= hopper2Value*issuePayDetailInfo.ChangeHopper2Count;

	//	issuePayDetailInfo.ChangeStockCount = min((DWORD)stockInfo.ulCurCount, remainAmount/stockValue);
	//	remainAmount -= stockValue*issuePayDetailInfo.ChangeStockCount;
	//}
	//else if (hopper2Value > hopper1Value && hopper1Value > stockValue ) {
	//	issuePayDetailInfo.ChangeHopper2Count = min((DWORD)hopper2Info.ulCurCount, remainAmount/hopper2Value);
	//	remainAmount -= hopper2Value*issuePayDetailInfo.ChangeHopper2Count;

	//	issuePayDetailInfo.ChangeHopper1Count = min((DWORD)hopper1Info.ulCurCount, remainAmount/hopper1Value);
	//	remainAmount -= hopper1Value*issuePayDetailInfo.ChangeHopper1Count;

	//	issuePayDetailInfo.ChangeStockCount = min((DWORD)stockInfo.ulCurCount, remainAmount/stockValue);
	//	remainAmount -= stockValue*issuePayDetailInfo.ChangeStockCount;
	//}

	//if (remainAmount <= 0) {
	//	return true;
	//}
	//else{
		return false;
	//}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      获取硬币接受个数

@param      无

@retval     硬币接受个数 DWARD

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CTVMForeService::GetCoinAcceptCount()
{
	DWORD recvCount = 0;
	recvCount += issuePayDetailInfo.PayCoinInfo[CoinHalfYuan];
	recvCount += issuePayDetailInfo.PayCoinInfo[Coin1Yuan];
	/*recvCount += issuePayDetailInfo.PayCoinInfo[Coin5MOP];
	recvCount += issuePayDetailInfo.PayCoinInfo[Coin50AVOS];*/
	return recvCount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币接受张数

@param      无

@retval     纸币接受个数 DWORD

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CTVMForeService::GetBankNoteAcceptCount()
{
	DWORD recvCount = 0;
	recvCount += issuePayDetailInfo.PayBankNoteInfo[Banknote1Yuan];
	recvCount += issuePayDetailInfo.PayBankNoteInfo[Banknote5Yuan];
	recvCount += issuePayDetailInfo.PayBankNoteInfo[Banknote10Yuan];
	recvCount += issuePayDetailInfo.PayBankNoteInfo[Banknote20Yuan];
	recvCount += issuePayDetailInfo.PayBankNoteInfo[Banknote50Yuan];
	recvCount += issuePayDetailInfo.PayBankNoteInfo[Banknote100Yuan];
	return recvCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新数据库关于投入金额的相关信息，供OnCoinInserted用

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

//void CTVMForeService::RenewDbWhileCoinInsert(CH_RSP_ACCEPT_INSERT_INFO chAmountInfo) {
//
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新数据库关于投入金额的相关信息，供OnBanknoteInserted用

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

void CTVMForeService::RenewDbWhileBanknoteInsert(BRH_STATUS bh_status) {

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否硬币纸币都达到最大数量，供OnCoinInserted和OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

bool CTVMForeService::IsBothCoinBanknoteMaxCount() {
	// since it is a virtual function, the return value doesn't really matter.
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否纸币达到最大数量，供OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

bool CTVMForeService::IsBanknoteMaxCount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否纸币都达到最大金额，供OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

bool CTVMForeService::IsBanknoteMaxAmount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      现金数量超限操作

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnOverMoneyCountLimit()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      现金金额超限操作

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnOverMoneyAmountLimit()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零数量超限操作

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnOverChangeCountLimit()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      接收到硬币回调函数

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::IsReceiveEnoughMoney()
{
	try{
		bool res = false;
		EnterCriticalSection(&m_critical_Section);
		issuePayDetailInfo.changeAmount = issuePayDetailInfo.CHReceiveMoney + issuePayDetailInfo.BHReceiveMoney - issuePayDetailInfo.dueAmount;
		if (static_cast<long>(issuePayDetailInfo.changeAmount) >= 0) {
			res = true;
		}
		LeaveCriticalSection(&m_critical_Section);
		return res;
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      退币完成

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnReturnCashOK()
{
	if (issuePayDetailInfo.CHReceiveMoney==0&&issuePayDetailInfo.BHReceiveMoney==0){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      现金接收完成

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

void CTVMForeService::OnAcceptMoneyOK()
{

}
//////////////////////////////////////////////////////////////////////////
/*
@brief      现金收纳完成

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnReceiveMoneyOK()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析硬币面值代码 BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CTVMForeService::TransCoinType(int value) {
	switch (value) {
	case 1: return CoinHalfYuan;
	case 2: return Coin1Yuan;
	/*case 3: return Coin2MOP;
	case 4: return Coin5MOP;*/
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币箱币种为无效，但是countInfo记录数量非零"));
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析纸币面值代码 BIN	1：1元，2：5元，3：10元，4：20元，5：50元，6：100元

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CTVMForeService::TransBanknoteType(int value) {
	switch (value) {
	case 1: return Banknote1Yuan;
	case 5: return Banknote5Yuan;
	case 10: return Banknote10Yuan;
	case 20: return Banknote20Yuan;
	case 50: return Banknote50Yuan;
	case 100: return Banknote100Yuan;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币箱币种为无效，但是countInfo记录数量非零"));
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析硬币面值代码 BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CTVMForeService::TransCoinValue(int value) {
	switch (value) {
	case 1: return 50;
	case 2: return 100;
	//case 3: return 200;
	//case 4: return 500;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币箱币种为无效，但是countInfo记录数量非零"));
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析纸币面值代码(纸币面额转换 1：1元，5：5元，10：10元，20：20元，50：50元，100：100元)

@param      int type; 面值代码

@retval     DWORD 解析过的面额(单位：分)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CTVMForeService::TransBankNoteValue(int value){
	switch(value){
	case 1:return 100;
	case 5:return 500;
	case 10:return 1000;
	case 20:return 2000;
	case 50:return 5000;
	case 100:return 10000;
	default:
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币箱币种为无效，但是countInfo记录数量非零"));
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析硬币面值代码 BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMForeService::ShowCoinValue (BYTE value) {
	switch (value) {
	case 1: return _T("50")+_tl(AVOS_UNIT);
	case 2: return _T("1")+_tl(MONEY_UNIT);
	case 3: return _T("2")+_tl(MONEY_UNIT);
	case 4: return _T("5")+_tl(MONEY_UNIT);
	default: return _T("0.00")+_tl(MONEY_UNIT);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		将面值转为CString格式

@param      BankNoteAndCoinType_t& value

@retval     CString格式的面值

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMForeService::CashValueToString(BankNoteAndCoinType_t value)
{
	CString strValue;
	switch(value)
	{
	case Coin1Yuan:strValue = _T("1元");break;
	case CoinHalfYuan:strValue = _T("5角");break;
	case Banknote1Yuan:strValue = _T("  1元");break;
	case Banknote5Yuan:strValue = _T("  5元");break;
	case Banknote10Yuan:strValue = _T(" 10元");break;
	case Banknote20Yuan:strValue = _T(" 20元");break;
	case Banknote50Yuan:strValue = _T(" 50元");break;
	case Banknote100Yuan:strValue = _T("100元");break;
	default: break;
	}
	return strValue;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      乘客插卡回调函数

@param      UINT uiResult	   (返回结果)
	
@param      CCommand* pCommand (返回命令)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnChargeCardInsertEx(UINT uiResult,CCommand* pCommand){
	if(theSERVICE_MGR.GetState() != MAINTENANCE){

	}
	else{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}

	//CIOCardPosMonitor* pIOCardPosMonitor = dynamic_cast<CIOCardPosMonitor*>(pCommand);

	//if(NULL != pIOCardPosMonitor){
	//	//OnChargeInsertCard(uiResult,pIOCardPosMonitor);
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      充值乘客插卡回调函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CTVMForeService::OnChargeInsertCard(UINT,CIOCardPosMonitor*){
//
//}