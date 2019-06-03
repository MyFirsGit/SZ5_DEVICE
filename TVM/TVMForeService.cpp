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
@brief          ���캯��

@param      (i)SERVICE_ID serviceID              ��ǰserviceID
@param      (i)SERVICE_ID parentServiceID    ��serviceID

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

	 // ��ʼ���ٽ�������
	 ::InitializeCriticalSection(&m_critical_Section);

	 m_pDialogFlow = new CTVMDialogFlow(*this);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief			��������

@param		none

@retval			none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMForeService::~CTVMForeService()
{
	delete m_pDialogFlow;
	m_pDialogFlow = NULL;

	// ɾ���ٽ�������
	::DeleteCriticalSection(&m_critical_Section);

	// ά���������ҵ��״̬
	if (theSERVICE_MGR.GetState() == MAINTENANCE || theSERVICE_MGR.GetState() == WAIT_MAINTENANCE|| theSERVICE_MGR.GetState() == INIT)
	{
		theAPP_SESSION.SetKeyboard(KEY_RETURN);		// ����ά������
		theAPP_SESSION.SetMaxInputLength(0);		// �������볤��
		theAPP_SESSION.SetGuideWhenDelete(_T(""));	// ����ɾ������
		theAPP_SESSION.DeleteAllInput();			// ɾ��������������
		theAPP_SESSION.AppendMenuLevel(_T(""));		// ��ղ˵�������׷�Ӽ���
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

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
		// ���ظ�Service��Dialog
		if (GetServiceID() != MAIN_SVC && GetParentServiceID() != MAIN_SVC) {
			if(NULL != GetParent())((CTVMForeService*)GetParent())->HideCurDialogGroup();
		}

		// ά���������ҵ��״̬
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
@brief      Serviceֹͣǰ����

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::OnStop()
{
	try {
		// ���ظ�Service�ĵ�ǰDialog��
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
@brief      ��������ת��ť��Ϣ

@param      (i)const char* pButtonMsg ��ť���·��͵ĸ�����Ϣ

@retval     ��

@exception  ��
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
@brief      ��ȡ��ǰDialog����

@param      ��

@retval     UINT    ��ǰDialog����

@exception  ��
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
@brief      ��ȡǰһ��Dialog����

@param      ��

@retval     UINT    ǰһ��Dialog����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CTVMForeService::GetPreDialogGroupID()
{
	return m_pDialogFlow->GetPreDialogGroupID();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ��ǰDialog��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::ShowCurDialogGroup()
{
	m_pDialogFlow->ShowDialogGroup(GetCurDialogGroupID());
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ص�ǰDialog��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::HideCurDialogGroup()
{
	m_pDialogFlow->HideDialogGroup(GetCurDialogGroupID());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���������ݶ������仯��֪ͨ������� 

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
@brief      �����ص�����

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
@brief      Ӳ��Ͷ��ص�����

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
@brief      Ӳ������ص�����

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
@brief      Ӳ�����ɻص�����

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
@brief      Ӳ���˳��ص�����

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
@brief      ֽ��Ͷ��ص�����

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

	// ������ֶ�ȡ��������ص������׶�
	/*if(res & DONE_PHASE_ACT_ABORT ){
		goto DONE_PHASE_ABORD;
	}*/

	/*if(res != E_NO_ERROR){
		theEXCEPTION_MGR.ProcessException(CBHHardwareException(res, _T(__FILE__), __LINE__));
		return;
	}*/

//goto ��ת���
//DONE_PHASE_ABORD:
	if(BHOpenAndValidate!=NULL)
	{	
		OnBanknoteInserted(res,BHOpenAndValidate);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ���˳��ص�����

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
@brief      ������ֽ���˳��ص�����

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
@brief      ֽ��ѹ������ص�����

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
@brief      ֽ��ѹ������ص�����

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
@brief      ���п�����ص�����

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
@brief      ���п��˳��ص�����

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
@brief      ������̰��»ص�����

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
@brief   ����ʵ�ֺ���   

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnReadCard(UINT res, CRWReadCard *)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���յ�Ӳ�һص�����

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinInserted(UINT res,CCHReadReceivs* chReadReceiveAmount)
{
	try{
	//	// ����Ͷ���ܶ�
	//	CH_RSP_ACCEPT_INSERT_INFO chAmountInfo;
	//	chReadReceiveAmount->GetCHResponse(&chAmountInfo);
	//	GetCoinAcceptAmount(chAmountInfo);
	//	
	//	// ���½�������Ϣ��ʾ��Ϣ
	//	NotifyDataModelChanged();

	//	// д�����ݿ������Ϣ
	//	RenewDbWhileCoinInsert(chAmountInfo);

	//	CAfcTVMOperationParam::BILLBOX_PARAM billboxParam;
	//	theAFC_TVM_OPERATION.GetBillboxParam(billboxParam);

	//	// �ж�Ͷ��Ӳ�������Ƿ��ޡ�����ǣ���Ӳ�ҿڡ�
	//	if (GetCoinAcceptCount() >= billboxParam.acceptCoinMaxCnt && issuePayDetailInfo.isCionOpen) {
	//		COIN_HELPER->CH_EndReceive();
	//		issuePayDetailInfo.isCionOpen = false;
	//	}

	//	
	//	// ������������ֻ�ᷢ��һ�����Ѱ����ȼ����С�
	//	// �ж�Ͷ�����Ƿ��㹻
	//	if (IsReceiveEnoughMoney()){
	//		// ����������� ���� (ϵͳ�������� ���� �ж��������Ƿ�������� ���� �����������������������������)
	//		if (issuePayDetailInfo.changeAmount == 0
	//			|| ((theTVM_STATUS_MGR.GetChangeMode()&DEVICE_CONTRL_CODE_NO_BILL_CHANGE) == DEVICE_CONTRL_CODE_NO_BILL_CHANGE
	//			&& CalculateChangeCount() 
	//			&& issuePayDetailInfo.ChangeHopper1Count+issuePayDetailInfo.ChangeHopper2Count
	//			   +issuePayDetailInfo.ChangeStockCount <= billboxParam.chargeCoinMaxCnt)) {
	//				   // �ر�Ӳ�ҿں�ֽ�ҿڣ���������һ��
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
	//		// ������������������ޣ������е�Ǯ��
	//		else {
	//			OnOverChangeCountLimit();
	//		}
	//	}
	//	// ���Ӳ�Һ�ֽ�Ҿ��ﵽ��������������ܶ�ȴ�����������е�Ǯ��
	//	//IsBothCoinBanknoteMaxCount������ChargeSvc��IssueSvc���Ϊֽ���������޺��������������β�ͬ
	//	else if (IsBothCoinBanknoteMaxCount()) {
	//		OnOverMoneyCountLimit();
	//	}
	//	// ������Ǯ
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
@brief      Ӳ������ص�����

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinChanged(UINT res,CCHChange* chChange)
{
	//if (res==0){
	//	issuePayDetailInfo.isCoinCharged = true;

	//	// ��Ӳ����ȡ��Ʊ������
	//	CH_COMMON_RSP* m_response;
	//	chChange->GetCHResponse(m_response);

	//	// ��Ӳ��HOPPER��A�ļ�����Ϣ�����£�������д�롣
	//	CTCoinCountInfo::COIN_BOX_INFO& boxInfo = theCOIN_COUNT.GetCoinHopperboxAInfo();
	//	boxInfo.ulCurCount = m_response->coin_info.hopper1;
	//	if(!theCOIN_COUNT.SetCoinHopperboxAInfo(boxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("HOPPER��A�ļ�����Ϣд��ʧ��"));
	//	}

	//	// ��Ӳ��HOPPER��B�ļ�����Ϣ�����£�������д�롣
	//	boxInfo = theCOIN_COUNT.GetCoinHopperboxBInfo();
	//	boxInfo.ulCurCount = m_response->coin_info.hopper2;
	//	if(!theCOIN_COUNT.SetCoinHopperboxBInfo(boxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("HOPPER��B�ļ�����Ϣд��ʧ��"));
	//	}

	//	// ��Ӳ��ѭ��HOPPER��ļ�����Ϣ�����£�������д�롣
	//	boxInfo = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//	boxInfo.ulCurCount = m_response->coin_info.stock1;
	//	if(!theCOIN_COUNT.SetCoinHopperboxCInfo(boxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ѭ������HOPPER��ļ�����Ϣд��ʧ��"));
	//	}

	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ѹӲ�ҵ��������������   

@param      UINT res,CCHAccept*

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnCoinReceived(UINT res,CCHAccept* chAccept)
{
	//if (res==0){
	//	issuePayDetailInfo.isCionCollected = true;
	//	OnReceiveMoneyOK();

	//	// ��Ӳ����ȡ��Ʊ������
	//	CH_COMMON_RSP* m_response;
	//	chAccept->GetCHResponse(m_response);

	//	// ��Ӳ��ѭ��HOPPER��ļ�����Ϣ�����£�������д�롣
	//	CTCoinCountInfo::COIN_BOX_INFO& boxInfo = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//	boxInfo.ulCurCount = m_response->coin_info.stock1;
	//	if(!theCOIN_COUNT.SetCoinHopperboxCInfo(boxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ѭ������HOPPER��ļ�����Ϣд��ʧ��"));
	//	}

	//	// ��Ӳ�һ�����A�ļ�����Ϣ�����£�������д�롣
	//	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO& collectBoxInfo = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	//	collectBoxInfo.ulCount_1yuan = m_response->coin_info.collect1_1Num;
	//	collectBoxInfo.ulCount_2yuan = 0; 
	//	collectBoxInfo.ulCount_5yuan = 0;
	//	collectBoxInfo.ulCount_5jiao = 0;
	//	if(!theCOIN_COUNT.SetCollectionCoinboxAInfo(collectBoxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("Ӳ�һ�����A�ļ�����Ϣд��ʧ��"));
	//	}

	//	// ��Ӳ�һ�����B�ļ�����Ϣ�����£�������д�롣
	//	collectBoxInfo = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	//	collectBoxInfo.ulCount_1yuan = m_response->coin_info.collect2_1Num;
	//	collectBoxInfo.ulCount_2yuan = m_response->coin_info.collect2_2Num;
	//	collectBoxInfo.ulCount_5yuan = m_response->coin_info.collect2_5Num;
	//	collectBoxInfo.ulCount_5jiao = m_response->coin_info.collect2_05Num;
	//	if(!theCOIN_COUNT.SetCollectionCoinboxBInfo(collectBoxInfo)) {
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("Ӳ�һ�����B�ļ�����Ϣд��ʧ��"));
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �˱һص����� 

@param      CRWReadCard * ��������

@retval     ��

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
@brief      ����ֽ�һص�����

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteInserted(UINT resultCode,CBRHOpenAndValidate* bhReadReceiveAmount)
{
	try{
		// ����Ͷ���ܶ�
		BRH_STATUS bh_status;
		bhReadReceiveAmount->GetResponse(&bh_status);
		GetBankNoteAcceptAmount(bh_status);

		// ���½�������Ϣ��ʾ��Ϣ
		NotifyDataModelChanged();

		// д�����ݿ������Ϣ
		RenewDbWhileBanknoteInsert(bh_status);

		CAfcTVMOperationParam::BILLBOX_PARAM billboxParam;
		theAFC_TVM_OPERATION.GetBillboxParam(billboxParam);

		// �ж�Ͷ��ֽ�������Ƿ��ޡ�����ǣ���ֽ�ҿڡ�
		// IsBanknoteMaxCount������ChargeSvc��IssueSvc���Ϊֽ���������޺��������������β�ͬ
		if (IsBanknoteMaxCount() && issuePayDetailInfo.isBankNoteOpen) {
			BANKNOTE_HELPER->StopValidation();
			issuePayDetailInfo.isBankNoteOpen = false;
		}

		// �����ĸ�����ֻ�ᷢ��һ�����Ѱ����ȼ����С�
		// ���ֽ�ҽ��ޣ������е�Ǯ����ʾ��Ӧ����ҳ�档
		// IsBanknoteMaxAmount������ChargeSvc��IssueSvc���Ϊֽ���������޺��������������β�ͬ
		if (IsBanknoteMaxAmount()) {
			OnOverMoneyAmountLimit();
		}
		// �ж�Ͷ�����Ƿ��㹻
		else if (IsReceiveEnoughMoney()){
			// ����������� ���� (ϵͳ�������� ���� �ж��������Ƿ�������� ���� �����������������������������)
			if (issuePayDetailInfo.changeAmount == 0
				((theTVM_STATUS_MGR.GetChangeMode()&DEVICE_CONTRL_CODE_NO_BILL_CHANGE) == DEVICE_CONTRL_CODE_NO_BILL_CHANGE
				&& CalculateChangeCount() 
				&& issuePayDetailInfo.ChangeHopper1Count+issuePayDetailInfo.ChangeHopper2Count
				+issuePayDetailInfo.ChangeStockCount <= billboxParam.chargeCoinMaxCnt)) {
					// �ر�Ӳ�ҿں�ֽ�ҿڣ���������һ��
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
			// ������������������ޣ������е�Ǯ��
			else {
				OnOverChangeCountLimit();
			}
		}
		// ���Ӳ�Һ�ֽ�Ҿ��ﵽ��������������ܶ�ȴ�����������е�Ǯ��
		// IsBothCoinBanknoteMaxCount������ChargeSvc��IssueSvc���Ϊֽ���������޺��������������β�ͬ
		else if (IsBothCoinBanknoteMaxCount()) {
			OnOverMoneyCountLimit();
		}
		// ������Ǯ
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
@brief      ֽ���˱�   

@param      CRWReadCard * ��������

@retval     ��

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
@brief      ֽ��ѹ������ʵ�ֺ���

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

		// ��ֽ��Ǯ��ļ�����Ϣ�ļ�����Ϣ�����£�������д�롣
		/*CTBanknoteCountInfo::BANKNOTE_BOX_INFO& boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
		boxInfo.ulCount_1 += issuePayDetailInfo.PayBankNoteInfo[Banknote1Yuan];
		boxInfo.ulCount_5 += issuePayDetailInfo.PayBankNoteInfo[Banknote5Yuan];
		boxInfo.ulCount_10 += issuePayDetailInfo.PayBankNoteInfo[Banknote10Yuan];
		boxInfo.ulCount_20 += issuePayDetailInfo.PayBankNoteInfo[Banknote20Yuan];
		boxInfo.ulCount_50 += issuePayDetailInfo.PayBankNoteInfo[Banknote50Yuan];
		boxInfo.ulCount_100 += issuePayDetailInfo.PayBankNoteInfo[Banknote100Yuan];
		if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ��Ǯ��ļ�����Ϣд��ʧ��"));
		}*/
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ������ص�����

@param      UINT res;						ִ�н��
			CBHChangeSendOutBill* command;	���ָ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnBanknoteChangeSendOut(UINT res,CBRHDispenseRequest* command){
	// ����ʵ��
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���п�����ʵ�ֺ���

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
@brief      ���п��˳�ʵ�ֺ���

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
@brief      ������̰���ʵ�ֺ���

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
@brief      ���� [0~9] ��������Ϣ�����֣�

@param      (i)WPARAM wParam  ���ּ�ֵ�����磺���ּ�7��ֵΪ7
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/)
{
	theAPP_SESSION.AppendOneNumToGuide(wParam);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [.] ��������Ϣ��С���㣩

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardDot(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	theAPP_SESSION.AppendOneDotToGuide();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Clear] ��������Ϣ��ɾ����

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      ���� [Enter] ��������Ϣ��ִ�У�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Esc] ��������Ϣ�����˵���

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      ���� [F1] ��������Ϣ��δʹ�ã�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF1(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F2/Alt] ��������Ϣ�����棩

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F4] ��������Ϣ��δʹ�ã�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTVMForeService::OnKeyboardF4(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F5] ��������Ϣ����һҳ��

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      ���� [F6] ��������Ϣ����һҳ��

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      ���÷�ҳ��Ϣ

@param      (i)int per		ÿҳ����
@param      (i)int total	�ܼ�ҳ��
@param      (i)int current  ��ǰҳ��

@retval     ��

@exception  ��
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
@brief      ��ȡÿҳ����

@param      ��

@retval     int

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMForeService::GetPerPage() const
{
	return m_per_page;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ܼ�ҳ��

@param      ��

@retval     int

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMForeService::GetTotalPage() const
{
	return m_total_page;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ�ǰҳ��

@param      ��

@retval     int

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMForeService::GetCurrentPage() const
{
	return m_current_page;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰҳ��ǰ׺����

@param      ��

@retval     int

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMForeService::GetPagePrefix()
{
	return m_per_page * (m_current_page - 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ���ֽ�

@param      ��

@retval     ��

@exception  CSysException& e
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::StartReceiveCash()
{
	try{		
		// ��ǰģʽ֧�ֽ���Ӳ����ʼ����Ӳ��
		if (theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_COIN){			
			COIN_HELPER->CH_StartReceive(this);  
			issuePayDetailInfo.isCionOpen = true;
		}
		// ��ǰģʽ֧�ֽ���ֽ����ʼ����ֽ��
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
@brief      ��ʼ����ֵ��

@param      ��

@retval     ��

@exception  ��
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
@brief      ��ʼ�������п�

@param      ��

@retval     ��

@exception  ��
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
@brief      ѹ���ͻ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::StartAcceptCashMoney()
{
	try{	
		theSERVICE_MGR.SetState(ISSUEING);
		bool res = false;
		// Ӳ�һ���
		COIN_HELPER->CH_AcceptAsync(this);   
		// ֽ�һ���		
		if (issuePayDetailInfo.BHReceiveMoney>0){
			BANKNOTE_HELPER->Collect();
		}

		//if (issuePayDetailInfo.isCionOpen){
		//	// �ر�Ӳ�ҿ�
		//	COIN_HELPER->CH_EndReceive();
		//	issuePayDetailInfo.isCionOpen = false;
		//	// �ؿں�Ӳ��ģ����Ҫ�ٶ�һ���ݴ�����
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
		//	// Ӳ�һ���
		//	COIN_HELPER->CH_Accept();   
		//}
		//// ��ǰģʽ֧�ֽ���ֽ����ر�ֽ�ҿ�
		//if (issuePayDetailInfo.isBankNoteOpen){
		//	BANKNOTE_HELPER->StopValidation();
		//	issuePayDetailInfo.isBankNoteOpen = false;
		//	// ֽ�һ���		
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
@brief      ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::StartChange()
{
	theSERVICE_MGR.SetState(ISSUEING);
	try{	
		// Ӳ������
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
@brief      ѹ���ͻ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnError(CSysException e)
{
	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �˱ң�ֽ��Ӳ�һ�������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::StartReturnCash()
{
	try{	
		theSERVICE_MGR.SetState(ISSUEING);
		// ���н��յ�Ӳ��ʱ
		if (issuePayDetailInfo.CHReceiveMoney>0){
			// Ӳ���˱�
			COIN_HELPER->CH_RefundAsync(this); 
		}
		// ���н��յ�ֽ��ʱ
		if (issuePayDetailInfo.BHReceiveMoney>0){
			// ֽ���˱�
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
@brief      �ر�����Ӳ��

@param      (i)CString errorMsg  ������Ϣ����
@param      (i)bool bContinue    �Ƿ�����������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::CloseAllDevice()
{
	// ��Ӳ�ҿ�
	if (issuePayDetailInfo.isCionOpen){
		COIN_HELPER->CH_StopInsertInfo();
		COIN_HELPER->CH_EndReceive();
		issuePayDetailInfo.isCionOpen = false;
	}
	// ��ֽ�ҿ�
	if (issuePayDetailInfo.isBankNoteOpen){
		BANKNOTE_HELPER->StopValidation();
		issuePayDetailInfo.isBankNoteOpen = false;
	}
	// ֹͣ����ֵ��
	if (issuePayDetailInfo.isWaitingEPCard){
		RECHARGERW_HELPER->StopReadCard();
		issuePayDetailInfo.isWaitingEPCard = false;
	}

	// ֹͣ�����п����˳����п�
	if (issuePayDetailInfo.isBankCardOpen) {
		issuePayDetailInfo.isBankCardOpen = false;
		//BANKCARD_HELPER->
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡӲ�ҽ��ܽ��

@param      ��

@retval     Ӳ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
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
@brief      ��ȡֽ�ҽ��ܽ��

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
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
	//	// ϵͳ�в����յ���ֵ
	//	recvAmount = 0;
	//}
	//issuePayDetailInfo.PayBankNoteInfo[bh_status.bh_faceValue.faceValue] += 1;
	//issuePayDetailInfo.BHReceiveMoney += recvAmount; 
	//return issuePayDetailInfo.BHReceiveMoney;
	return recvAmount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ö��

@param      ��

@retval     true ���㹻��Ӳ�ҿɳɹ�����
			false Ӳ�Ҳ����ɹ�����

@exception  ��changeAmount���㣬������ChangeStockCount/ChangeHopper1Count/ChangeHopper2Count
			�˺���������������Ķ�Ӧ������0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP
			�������TVM.INI��Common/Source Files/TVMInfo.cpp����������TransCoinValue��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMForeService::CalculateChangeCount()
{
	//DWORD remainAmount = issuePayDetailInfo.changeAmount;

	//int stockType = theTVM_INFO.GetCHStockMoneyType();
	//int hopper1Type = theTVM_INFO.GetCHHopper1Type();
	//int hopper2Type = theTVM_INFO.GetCHHopper2Type();

	//// ������������Ķ�ӡ������������ִ�Ľ��ߡ���0����Ч�������λᱨ��������Value 0��
	//DWORD stockValue = TransCoinValue(stockType);
	//DWORD hopper1Value = TransCoinValue(hopper1Type);
	//DWORD hopper2Value = TransCoinValue(hopper2Type);

	////Ϊ0ֱ�ӷ��ش���
	//if (0 == stockValue || 0 == hopper1Value || 0 == hopper2Value)
	//{
	//	return false;
	//}
	//CTCoinCountInfo::COIN_BOX_INFO& stockInfo = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//CTCoinCountInfo::COIN_BOX_INFO& hopper1Info = theCOIN_COUNT.GetCoinHopperboxAInfo();
	//CTCoinCountInfo::COIN_BOX_INFO& hopper2Info = theCOIN_COUNT.GetCoinHopperboxBInfo();

	//// �������ξ��ѿ��ǡ�����ѭ�����������ȼ��ߣ�������>=��������hopper˳������ν�����Ծ���������hopper1���ȼ��Ƚϸߡ�
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
@brief      ��ȡӲ�ҽ��ܸ���

@param      ��

@retval     Ӳ�ҽ��ܸ��� DWARD

@exception  ��
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
@brief      ��ȡֽ�ҽ�������

@param      ��

@retval     ֽ�ҽ��ܸ��� DWORD

@exception  ��
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
@brief      �������ݿ����Ͷ����������Ϣ����OnCoinInserted��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

//void CTVMForeService::RenewDbWhileCoinInsert(CH_RSP_ACCEPT_INSERT_INFO chAmountInfo) {
//
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ݿ����Ͷ����������Ϣ����OnBanknoteInserted��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

void CTVMForeService::RenewDbWhileBanknoteInsert(BRH_STATUS bh_status) {

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�Ӳ��ֽ�Ҷ��ﵽ�����������OnCoinInserted��OnBanknoteInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

bool CTVMForeService::IsBothCoinBanknoteMaxCount() {
	// since it is a virtual function, the return value doesn't really matter.
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�ֽ�Ҵﵽ�����������OnBanknoteInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

bool CTVMForeService::IsBanknoteMaxCount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�ֽ�Ҷ��ﵽ������OnBanknoteInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

bool CTVMForeService::IsBanknoteMaxAmount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ֽ��������޲���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnOverMoneyCountLimit()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ֽ���޲���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnOverMoneyAmountLimit()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����������޲���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnOverChangeCountLimit()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���յ�Ӳ�һص�����

@param      CRWReadCard * ��������

@retval     ��

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
@brief      �˱����

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnReturnCashOK()
{
	if (issuePayDetailInfo.CHReceiveMoney==0&&issuePayDetailInfo.BHReceiveMoney==0){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ֽ�������

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

void CTVMForeService::OnAcceptMoneyOK()
{

}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �ֽ��������

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMForeService::OnReceiveMoneyOK()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ӳ����ֵ���� BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CTVMForeService::TransCoinType(int value) {
	switch (value) {
	case 1: return CoinHalfYuan;
	case 2: return Coin1Yuan;
	/*case 3: return Coin2MOP;
	case 4: return Coin5MOP;*/
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("Ӳ�������Ϊ��Ч������countInfo��¼��������"));
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ����ֵ���� BIN	1��1Ԫ��2��5Ԫ��3��10Ԫ��4��20Ԫ��5��50Ԫ��6��100Ԫ

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
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
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ�������Ϊ��Ч������countInfo��¼��������"));
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ӳ����ֵ���� BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DWORD CTVMForeService::TransCoinValue(int value) {
	switch (value) {
	case 1: return 50;
	case 2: return 100;
	//case 3: return 200;
	//case 4: return 500;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("Ӳ�������Ϊ��Ч������countInfo��¼��������"));
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ����ֵ����(ֽ�����ת�� 1��1Ԫ��5��5Ԫ��10��10Ԫ��20��20Ԫ��50��50Ԫ��100��100Ԫ)

@param      int type; ��ֵ����

@retval     DWORD �����������(��λ����)

@exception  ��
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
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ�������Ϊ��Ч������countInfo��¼��������"));
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ӳ����ֵ���� BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
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
@brief		����ֵתΪCString��ʽ

@param      BankNoteAndCoinType_t& value

@retval     CString��ʽ����ֵ

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMForeService::CashValueToString(BankNoteAndCoinType_t value)
{
	CString strValue;
	switch(value)
	{
	case Coin1Yuan:strValue = _T("1Ԫ");break;
	case CoinHalfYuan:strValue = _T("5��");break;
	case Banknote1Yuan:strValue = _T("  1Ԫ");break;
	case Banknote5Yuan:strValue = _T("  5Ԫ");break;
	case Banknote10Yuan:strValue = _T(" 10Ԫ");break;
	case Banknote20Yuan:strValue = _T(" 20Ԫ");break;
	case Banknote50Yuan:strValue = _T(" 50Ԫ");break;
	case Banknote100Yuan:strValue = _T("100Ԫ");break;
	default: break;
	}
	return strValue;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �˿Ͳ忨�ص�����

@param      UINT uiResult	   (���ؽ��)
	
@param      CCommand* pCommand (��������)

@retval     ��

@exception  ��
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
@brief      ��ֵ�˿Ͳ忨�ص�����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CTVMForeService::OnChargeInsertCard(UINT,CIOCardPosMonitor*){
//
//}