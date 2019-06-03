#include "StdAfx.h"
#include "BRHCommands.h"
#include "Sync.h"
#include "DebugLog.h"
#include "TvmSetting.h"
#define theBRH_TRACE CFileLog::GetInstance(_T("BRH_"))
CBRHCommands gBRHCmd;
/************************ Defines and typedefs *******************************/
#define BNR_CASHTAKEN_TIME_OUT_IN_MS (30000)
#define BNR_DEFAULT_OPERATION_TIME_OUT_IN_MS (5000)
#define BNR_OPEN_OPERATION_TIME_OUT_IN_MS    (1000)
#define BNR_RESET_OPERATION_TIME_OUT_IN_MS   (60000)
#define BNR_CASHIN_OPERATION_TIME_OUT_IN_MS  (300000)

static T_BnrXfsResult ocResult;
static HANDLE listenerEvent = NULL;			// 操作完成事件
static HANDLE cashInStartEvent = 
	CreateEvent(NULL, TRUE, FALSE, NULL);	// 根据此事件，判断模块是否处于接收状态下

static T_XfsCashOrder cashOrder;
static T_XfsCashUnit cashUnit;

static T_Amount cashInAmount = 0;

static bool		bChangeSuccess = false;
static bool		bUserTakenCash = false;
static bool		bIsAccepting = false;
static bool		bIsReadyToAccept = false;

//static bool isCashInStart = false;
//static bool isDisable = false;
//static bool isDispense = false;
//static bool isEmptyMessage = false;
//static bool isCashUnitMessage = false;
//static bool isEject = false;
//static bool is_CDR_CASHUNIT_CHANGED = false;
//static bool is_XFS_O_CDR_CASH_IN = false;
//static bool is_Cashin_Abnormal = false; 
//static bool is_XFS_I_CDR_INPUT_REFUSED = false;
//static bool is_P_WAIT_STATUS_CASH_TAKEN_ERROR =false;
//static bool is_StartWaitCashin = false;
// 箱子信息
map<UINT,CASH_BOX_INFO> mapCashBoxInfo;

long checkAndResetEvent()
{
	if (!listenerEvent){
		listenerEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	if (!listenerEvent){
		// print errno
		CString sLogString = _T("");
		sLogString.Format(_T("checkAndResetEvent: listenEvent---errno = 0x%02x"),GetLastError());
		theBRH_TRACE->WriteData( _T("[Error]") + sLogString,NULL,0);
		return 1;
	}

	ResetEvent(listenerEvent);
	ZeroMemory(&cashOrder, sizeof(T_XfsCashOrder));

	return 0;
}

static void CALLTYPE onOperationComplete(LONG32 identificationId, LONG32 operationId, LONG32 result, LONG32 extendedResult, void *data)
{
	ZeroMemory(&cashOrder, sizeof(T_XfsCashOrder));
	ocResult = result;
	switch (operationId) {
	case XFS_O_CDR_RESET:
		SetEvent(listenerEvent);
		break;
	case XFS_O_CDR_EMPTY:
		if(data != NULL)
			memcpy(&cashOrder, data, sizeof(T_XfsCashOrder));
		SetEvent(listenerEvent); 
		break;
	case XFS_O_CDR_CASH_IN_START:
		SetEvent(listenerEvent);
		SetEvent(cashInStartEvent);
		break;           
	case XFS_O_CDR_CASH_IN:
		if(data != NULL)
			memcpy(&cashOrder, data, sizeof(T_XfsCashOrder));
		SetEvent(listenerEvent);
		break;
	case XFS_O_CDR_CASH_IN_END:
		if(data != NULL)
			memcpy(&cashOrder, data, sizeof(T_XfsCashOrder));
		SetEvent(listenerEvent);
		break;
	case XFS_O_CDR_CASH_IN_ROLLBACK:
		if(data != NULL)
			memcpy(&cashOrder, data, sizeof(T_XfsCashOrder));
		SetEvent(listenerEvent);    	
		break;
	case BNRXFS_O_CDR_EJECT:
		if(data != NULL)
			memcpy(&cashOrder, data, sizeof(T_XfsCashOrder));
		SetEvent(listenerEvent);       	   
		break; 
	case XFS_O_CDR_DISPENSE:
		if(data != NULL)
			memcpy(&cashOrder, data, sizeof(T_XfsCashOrder));
		SetEvent(listenerEvent);
		break;
	case XFS_O_CDR_PRESENT:
		if(data != NULL)
			memcpy(&cashOrder, data, sizeof(T_XfsCashOrder));
		SetEvent(listenerEvent);
		break;
	case XFS_O_CDR_REJECT:
		if(data != NULL)
			memcpy(&cashOrder, data, sizeof(T_XfsCashOrder)); 
		SetEvent(listenerEvent);
		break;
	case XFS_O_CDR_RETRACT:
		if(data != NULL)
			memcpy(&cashOrder, data, sizeof(T_XfsCashOrder)); 
		SetEvent(listenerEvent);
		break;
	case XFS_O_CDR_DENOMINATE:
		SetEvent(listenerEvent);
		break;
	case XFS_O_BNR_SELF_TEST:
		SetEvent(listenerEvent);   	    
		break;
	case BNRXFS_O_CDR_PARK:
		SetEvent(listenerEvent);   	    
		break;
	case XFS_O_CDR_UPDATE_CASH_UNIT:
		SetEvent(listenerEvent);      	
		break;
	case XFS_O_CDR_QUERY_CASH_UNIT:
		SetEvent(listenerEvent); 
		break;
	case XFS_O_CDR_QUERY_DENOMINATIONS:
		SetEvent(listenerEvent);      	
		break;
	case XFS_O_CDR_UPDATE_DENOMINATIONS:
		SetEvent(listenerEvent); 
		break;
	}//switch
}


static void CALLTYPE onStatusOccured(LONG32 status, LONG32 result, LONG32 extendedResult, void *data)
{

	switch(status){
	case XFS_S_CDR_CASH_AVAILABLE:
		{
			bChangeSuccess = true;
		}
		break;
	case XFS_S_CDR_CASH_TAKEN:
		{
			bUserTakenCash = true;
		}
		break;
	default:
		break;
	}
}


static void CALLTYPE onIntermediateOccured(LONG32 identificationId, LONG32 operationId, LONG32 reason, void *data)
{
	//switch(operationId){
	//case XFS_O_CDR_EMPTY:
	//case XFS_O_CDR_CASH_IN_START:
	//case XFS_O_CDR_CASH_IN:
	//case XFS_O_CDR_CASH_IN_END:
	//case XFS_O_CDR_CASH_IN_ROLLBACK:
	//	break;
	//}

	//switch(reason){
	//case XFS_I_CDR_INPUT_REFUSED:
	//case XFS_S_CDR_CASH_AVAILABLE:
	//case XFS_S_CDR_CASH_TAKEN:
	//	break;
	//}

	//switch(operationId){    
	//case XFS_O_CDR_CASH_IN:
	//	switch(reason){
	//	case XFS_I_CDR_INPUT_REFUSED:
	//		//is_XFS_I_CDR_INPUT_REFUSED = true;
	//		SetEvent(listenerEvent);
	//		//Sleep(100);
	//		break;
	//	}
	//	break;
	//}
}



CBRHCommands::CBRHCommands()
{

}

CBRHCommands::~CBRHCommands()
{
	
}

T_BnrXfsResult CBRHCommands::Open()												// 打开
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("OPEN"));

	sLogString.Format(_T("Open: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_Open(onOperationComplete, onStatusOccured, onIntermediateOccured);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, BNR_OPEN_OPERATION_TIME_OUT_IN_MS);
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Open: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Close()											// 关闭
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("CLOSE"));

	sLogString.Format(_T("Close: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_Close();
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, BNR_OPEN_OPERATION_TIME_OUT_IN_MS);
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Close: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Reset()												// 复位
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("RESET"));

	sLogString.Format(_T("Reset: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_Reset();
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		//WaitForSingleObject(listenerEvent, BNR_RESET_OPERATION_TIME_OUT_IN_MS);		//lichao:这里有必要等待设备状态变化吗
		//ResetEvent(listenerEvent);
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_RESET_OPERATION_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Reset: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Reboot()												// 重启
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("REBOOT"));

	sLogString.Format(_T("Reboot: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_Reboot();
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, BNR_RESET_OPERATION_TIME_OUT_IN_MS + 30000);
	}

	sLogString = _T("");
	sLogString.Format(_T("Reboot: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::GetStatus(T_XfsCdrStatus * status)		// 获取状态
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("GET_STATUS"));

	sLogString.Format(_T("GetStatus: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_GetStatus(status);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		//WaitForSingleObject(listenerEvent, BNR_OPEN_OPERATION_TIME_OUT_IN_MS);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("GetStatus: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::GetSystemStatus(T_SystemStatusOverview * status)		// 获取详细状态
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("GET_SYSTEM_STATUS"));

	sLogString.Format(_T("GetSystemStatus: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_GetSystemStatus(status);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		//WaitForSingleObject(listenerEvent, BNR_OPEN_OPERATION_TIME_OUT_IN_MS);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("GetSystemStatus: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Cancel()							// 取消操作
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("CANCEL"));

	sLogString.Format(_T("Cancel: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result = 0;
	if (bIsReadyToAccept){//准备接收，但接收命令还没开始执行
		bIsReadyToAccept = false;
		DWORD waitRet = WaitForSingleObject(cashInStartEvent, BNR_CASHIN_OPERATION_TIME_OUT_IN_MS);
		// 主要起一个等待同步作用
		if (WAIT_OBJECT_0!=waitRet){
			sLogString.Format(_T("Wait cashInStartEvent failed: %d"), waitRet);
			theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);
		}
		Sleep(300);//等待cashIn命令执行
	}

	if (bIsAccepting){
		result  = bnr_Cancel();
		if (BXR_NO_ERROR == result){
			//Wait if an OperationComplete event for previous operation has to be received.
			WaitForSingleObject(listenerEvent, BNR_DEFAULT_OPERATION_TIME_OUT_IN_MS);
			result = 0;
		}
		else{
			result = (-1)*result;
		}
	}
	else{
		sLogString = _T("Cancel: BRH is not accepting, so skip!");
		theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);
	}
	sLogString = _T("");
	sLogString.Format(_T("Cancel: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::GetCapabilities(T_XfsCapabilities *capabilities)	// 获取性能		
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("GET_CAPABILITIES"));

	sLogString.Format(_T("GetCapabilities: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_GetCapabilities(capabilities);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, 200);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("GetCapabilities: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::SetCapabilities(T_XfsCapabilities *capabilities)	// 设置性能		
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("SET_CAPABILITIES"));

	sLogString.Format(_T("SetCapabilities: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_SetCapabilities(capabilities);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, 500);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("SetCapabilities: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::SetDateTime(time_t dateTime)				// 设置时间	
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("SET_DATETIME"));

	sLogString.Format(_T("SetDateTime: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_SetDateTime(dateTime);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		//WaitForSingleObject(listenerEvent, 500);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("SetDateTime: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::GetTransactionStatus(T_TransactionStatus *status)	// 获取通道状态		
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("GET_TRANSACTIONSTATUS"));

	sLogString.Format(_T("GetTransactionStatus: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_GetTransactionStatus(status);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		//WaitForSingleObject(listenerEvent, 500);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("GetTransactionStatus: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Empty(char * pcuName, BOOL toFloat)				// 清空循环找零箱或补币箱
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("EMPTY"));

	sLogString.Format(_T("Empty: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	sLogString.Format(_T("pcuName = %s, toFloat = %d: "), pcuName, toFloat);
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	T_BnrXfsResult result  = bnr_Empty(pcuName, toFloat);
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_CASHIN_OPERATION_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Empty: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}


T_BnrXfsResult CBRHCommands::Reject(T_Amount *amount)											// 清空暂存中的钞票到循环找零箱或钱箱
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("REJECT"));

	sLogString.Format(_T("Reject: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_Reject();
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_CASHIN_OPERATION_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				*amount = cashOrder.denomination.amount;
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Reject: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::SelfTest()										// 硬件自检（测试模式下使用）
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("SELF_TEST"));

	sLogString.Format(_T("SelfTest: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_SelfTest();
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, BNR_DEFAULT_OPERATION_TIME_OUT_IN_MS);
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("SelfTest: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Park()											// 预备所有模块，使可卸下
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("PARK"));

	sLogString.Format(_T("Park: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_Park();
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_CASHIN_OPERATION_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Park: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands:: CancelWaitingCashTaken()							// 取消等待用户取走钞票
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("CANCEL_WAIT"));

	sLogString.Format(_T("CancelWaitingCashTaken: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_CancelWaitingCashTaken();
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, BNR_CASHTAKEN_TIME_OUT_IN_MS);
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("CancelWaitingCashTaken: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::CashInStart()								// 开始接收钞票
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("CASH_IN_START"));

	sLogString.Format(_T("CashInStart: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	ResetEvent(cashInStartEvent);
	bIsReadyToAccept = true;
	T_BnrXfsResult result  = bnr_CashInStart();
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_DEFAULT_OPERATION_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("CashInStart: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::CashIn(T_Amount *amount)	// 接受钞票，移动到暂存
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("CASH_IN"));

	sLogString.Format(_T("CashIn: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_CashIn(NULL, NULL);
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		bIsAccepting = true;
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_CASHIN_OPERATION_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				*amount = cashOrder.denomination.amount;
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	bIsAccepting = false;
	sLogString = _T("");
	sLogString.Format(_T("amount=%d"),*amount);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	sLogString.Format(_T("CashIn: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::CashInRollback(T_Amount *amount)									// 退出本次接受的所有钞票
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("CASH_IN_ROOLBACK"));

	sLogString.Format(_T("CashInRollback: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_CashInRollback();
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_CASHTAKEN_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				*amount = cashOrder.denomination.amount;
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("CashInRollback: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::CashInEnd(T_Amount *amount)									// 结束接受钞票
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("CASH_IN_END"));

	sLogString.Format(_T("CashInEnd: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_CashInEnd();
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_DEFAULT_OPERATION_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				*amount = cashOrder.denomination.amount;
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("CashInEnd: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Eject(T_Amount *amount)									// 退钞（用于钞票未识别）
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("EJECT"));

	sLogString.Format(_T("Eject: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_Eject();
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_CASHTAKEN_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				*amount = cashOrder.denomination.amount;
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Eject: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Present(T_Amount *amount)									// 开始现金找零
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("PRESENT"));

	sLogString.Format(_T("Present: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_Present();
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_CASHTAKEN_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				*amount = cashOrder.denomination.amount;
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Present: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Dispense(T_XfsDispenseRequest* dispenseRequest)	// 找零
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("DISPENSE"));

	sLogString.Format(_T("Dispense: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	bChangeSuccess = false;
	bUserTakenCash = false;

	T_BnrXfsResult result  = bnr_Dispense(dispenseRequest);
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_CASHTAKEN_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;

			// 找零成功,不在等待了
			if(bChangeSuccess){
				if(!bUserTakenCash)
					bnr_CancelWaitingCashTaken();
				result = 0;
			}
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Dispense: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::Retract(T_Amount *amount)											// 回收找零的钞票
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("RETRACT"));

	sLogString.Format(_T("Retract: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_Retract();
	if (result>0){
		//Wait if an OperationComplete event for previous operation has to be received.
		DWORD waitRet = WaitForSingleObject(listenerEvent, BNR_CASHTAKEN_TIME_OUT_IN_MS);
		if (waitRet == WAIT_OBJECT_0){
			if( ocResult == XFS_RC_SUCCESSFUL){
				sLogString.Format(_T("WAIT_OBJECT_0: success"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				*amount = cashOrder.denomination.amount;
				result = 0;
			}
			else{
				sLogString.Format(_T("WAIT_OBJECT_0: failed"));
				theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
				result = ocResult;
			}
		}
		else if(waitRet == WAIT_TIMEOUT){
			sLogString.Format(_T("WAIT_TIMEOUT: "));
			theBRH_TRACE->WriteData(_T("<")+sLogString,NULL,0);
			result = BRH_ERR_TIMEOUT;
		}
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("Retract: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::QueryCashUnit(T_XfsCashUnit * cashUnit)	// 获取现金单元信息		
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("QUERY_CASHUNIT"));

	sLogString.Format(_T("QueryCashUnit: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_QueryCashUnit(cashUnit);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, 200);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("QueryCashUnit: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::UpdateCashUnit(T_XfsCashUnit * cashUnit)	// 更新现金单元信息		
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("UPDATE_CASHUNIT"));

	sLogString.Format(_T("UpdateCashUnit: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_UpdateCashUnit(cashUnit->transportCount, &(cashUnit->logicalCashUnitList), &(cashUnit->physicalCashUnitList));
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, BNR_DEFAULT_OPERATION_TIME_OUT_IN_MS);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("UpdateCashUnit: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}
// 配置现金单元信息
T_BnrXfsResult CBRHCommands::ConfigureCashUnit(UINT32 transportCount,T_LogicalCashUnitList * logicalCashUnitList, T_PhysicalCashUnitList * physicalCashUnitList)
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("CONFIG_CASHUNIT"));

	sLogString.Format(_T("ConfigureCashUnit: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_ConfigureCashUnit(transportCount,logicalCashUnitList,physicalCashUnitList);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, BNR_DEFAULT_OPERATION_TIME_OUT_IN_MS);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("ConfigureCashUnit: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::QueryDenominations(T_DenominationList * denominationList)	 // 获取面值		
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("QUERY_DENOMINATIONS"));

	sLogString.Format(_T("QueryDenominations: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_QueryDenominations(denominationList);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, 200);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("QueryDenominations: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

T_BnrXfsResult CBRHCommands::UpdateDenominations(T_DenominationList * denominationList)	// 更新面值		
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("UPDATE_DENOMINATIONS"));

	sLogString.Format(_T("UpdateDenominations: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = bnr_UpdateDenominations(denominationList);
	if (BXR_NO_ERROR == result){
		//Wait if an OperationComplete event for previous operation has to be received.
		WaitForSingleObject(listenerEvent, 200);//lichao
		result = 0;
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("UpdateDenominations: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

// 更新模块面额(只针对纸币找零箱)
T_BnrXfsResult CBRHCommands::SetCashType(T_ModuleId id, T_XfsCashType* cashType)	
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("SET_CASH_TYPE"));

	sLogString.Format(_T("SetCashType: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = module_SetCashType(id,cashType);
	if (BXR_NO_ERROR == result){
		WaitForSingleObject(listenerEvent, BNR_DEFAULT_OPERATION_TIME_OUT_IN_MS);
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("SetCashType: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

// 获取模块面额
T_BnrXfsResult CBRHCommands::GetCashType(T_ModuleId id, T_XfsCashType* cashType)	
{
	CString sLogString(_T(""));
	SYNC(BRH,_T("GET_CASH_TYPE"));

	sLogString.Format(_T("GetCashType: "));
	theBRH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (checkAndResetEvent()){
		return BRH_ERR_EVENT_FAILURE;// 事件创建错误
	}

	T_BnrXfsResult result  = module_GetCashType(id,cashType);
	if (BXR_NO_ERROR == result){
		WaitForSingleObject(listenerEvent, BNR_DEFAULT_OPERATION_TIME_OUT_IN_MS);
	}
	else{
		result = (-1)*result;
	}

	sLogString = _T("");
	sLogString.Format(_T("GetCashType: ReturnCode = 0x%02x"),result);
	theBRH_TRACE->WriteData( _T("<") + sLogString,NULL,0);

	return result;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取结果

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CBRHCommands::GetCashOrder(T_XfsCashOrder* pCashOrder){
	if(NULL != pCashOrder){
		*pCashOrder = cashOrder;
		return true;
	}

	return false;
}


int BNC_open(unsigned int uiCommPort, unsigned int uiBaudRate)
{
	T_BnrXfsResult result;
	result = gBRHCmd.Open();
	if(BXR_NO_ERROR != result){
		return 1;
	}
	return 0;
}

int  BNC_close()
{
	T_BnrXfsResult result;
	result = gBRHCmd.Close();
	if(BXR_NO_ERROR != result){
		return 1;
	}
	return 0;
}

int BNC_Reset(int iMode, int iRetryTimes, tBncInitNumInfo *pNumInfo,tBncDevReturn * p_psStatus)
{
	T_BnrXfsResult result;
	for(int i = 0; i < iRetryTimes ; i++){
		result = gBRHCmd.Reset();
		p_psStatus->iStdErrorCode = result;

		if(BXR_NO_ERROR != result){
			return result;
		}
	}
	return result;
}

int BNC_GetStatus(tBncDevReturn * p_psStatus)
{
	T_XfsCdrStatus  status;
	T_BnrXfsResult result;
	result = gBRHCmd.GetStatus(&status);

	p_psStatus->uiProviderId = 0x00;
	p_psStatus->iType = status.deviceStatus;
	p_psStatus->iStdErrorCode = result;
	memset(p_psStatus->acDevReturn,0x00,128);
	memset(p_psStatus->acReserve,0x00,128);
	return result;
}
int BNC_GetSysDevStatus(tBncSysDevStatus* p_psSysStatus)
{
	T_BnrXfsResult result;
	T_SystemStatusOverview systemStatus;
	result = gBRHCmd.GetSystemStatus(&systemStatus);
	for(int sz=0; sz<systemStatus.size; sz++){
		// print log
		if(systemStatus.moduleStatusOverviews[sz].cashbox.id == CASHBOX_CLASS){
			if(systemStatus.moduleStatusOverviews[sz].cashbox.errorCode == CEC_MODULE_MISSING){
				p_psSysStatus->bIsCollectBoxInstalled = false;
			}
			else
				p_psSysStatus->bIsCollectBoxInstalled = true;
		}
		else if(systemStatus.moduleStatusOverviews[sz].loader.id == (LOADER_CLASS+1)){
			if(systemStatus.moduleStatusOverviews[sz].loader.errorCode == MODEC_MISSING_MODULE){
				p_psSysStatus->bIsChangeBoxInstalled = false;
			}
			else
			{
				p_psSysStatus->bIsChangeBoxInstalled = true;
			}
		}
	}
	return result;
}
// 弃用
int BNC_GetAuditData(tBncAuditCashNum *pBncAuditCashNum, tBncDevReturn *pDevStatus)
{
	return 0;
}

int BNC_GetBillBoxInfo(tBncBillBoxInfo *pBillBoxInfo, tBncDevReturn *pDevStatus)
{
	T_BnrXfsResult result;
	T_XfsCashUnit *cashUnit = new T_XfsCashUnit;
	cashUnit->logicalCashUnitList.maxSize = NBMAXLCU;
	cashUnit->physicalCashUnitList.maxSize =  NBMAXPCU;

	result = gBRHCmd.QueryCashUnit(cashUnit);
	pDevStatus->iStdErrorCode = result;
	if (result == 0)
	{
		int nIndex = 0;
		for(int i = 0;i < cashUnit->logicalCashUnitList.size;i++){
			T_XfsLogicalCashUnit LogicalCashUnit = cashUnit->logicalCashUnitList.items[i];
			CASH_BOX_INFO boxInfo;

			boxInfo.cashType = LogicalCashUnit.cashType;
			boxInfo.logicalNumber = LogicalCashUnit.number;
			boxInfo.TotalCount    = LogicalCashUnit.count;
			memcpy(boxInfo.PhysicalName,LogicalCashUnit.physicalCashUnit->name,SIZE_OF_PHYSICAL_NAME);

			mapCashBoxInfo[boxInfo.logicalNumber] = boxInfo;
		}
		for(int i = 0 ;i < cashUnit->physicalCashUnitList.size ; i++){
			T_XfsPhysicalCashUnit& CashUnit = cashUnit->physicalCashUnitList.items[i];

			T_XfsPcuName* pBoxName = &(CashUnit.name);
			if (strcmp((char*)pBoxName,BNR_MODULE_CASH_BOX_NAME) == 0 ){//回收箱
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxNo = 0;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxType = 0;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillValue = 0;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillNum = CashUnit.count;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillMaxNum = CashUnit.threshold.full;
				pBillBoxInfo->pCashBoxValueInfo[nIndex++].uiBoxStatus = CashUnit.status;
			}else if (strcmp((char*)pBoxName,BNR_MODULE_LOADER_BOX_NAME) == 0){ //5 元
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxNo = 1;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxType = 2;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillValue = theTVM_SETTING.GetBNRChangeBoxAmount()*100;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillNum = CashUnit.count;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillMaxNum = CashUnit.threshold.full;//???
				pBillBoxInfo->pCashBoxValueInfo[nIndex++].uiBoxStatus = CashUnit.status;
			}else if (strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0){// 1元
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxNo = 2;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxType = 1;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillValue = theTVM_SETTING.GetBNRRecycleBoxAAmount()*100;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillNum = CashUnit.count;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillMaxNum = CashUnit.threshold.full;//???
				pBillBoxInfo->pCashBoxValueInfo[nIndex++].uiBoxStatus = CashUnit.status;
			}else if (strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0){// 1元
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxNo = 3;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxType = 1;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillValue = theTVM_SETTING.GetBNRRecycleBoxBAmount()*100;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillNum = CashUnit.count;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillMaxNum = CashUnit.threshold.full;//???
				pBillBoxInfo->pCashBoxValueInfo[nIndex++].uiBoxStatus = CashUnit.status;
			}else if (strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0){// 5元
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxNo = 4;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxType = 2;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillValue = theTVM_SETTING.GetBNRRecycleBoxCAmount()*100;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillNum = CashUnit.count;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillMaxNum = CashUnit.threshold.full;//???
				pBillBoxInfo->pCashBoxValueInfo[nIndex++].uiBoxStatus = CashUnit.status;
			}else if (strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0){// 5元
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxNo = 5;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBoxType = 2;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillValue = theTVM_SETTING.GetBNRRecycleBoxDAmount()*100;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillNum = CashUnit.count;
				pBillBoxInfo->pCashBoxValueInfo[nIndex].uiBillMaxNum = CashUnit.threshold.full;//???
				pBillBoxInfo->pCashBoxValueInfo[nIndex++].uiBoxStatus = CashUnit.status;
			}
		}
	}
	delete cashUnit;
	cashUnit = NULL;
	return result;
}

int BNC_SetBillBoxInfo(UINT uiSlotID, tBncBillBoxInfo *pBillBoxInfo, tBncDevReturn *pDevStatus)
{
	T_BnrXfsResult result;
	T_XfsCashUnit  cashUnit;
	//T_BnrXfsResult UpdateCashUnit(T_XfsCashUnit * cashUnit);
	cashUnit.logicalCashUnitList.items[uiSlotID].count = pBillBoxInfo->pCashBoxValueInfo[0].uiBillNum;
	result = gBRHCmd.UpdateCashUnit(&cashUnit);
	pDevStatus->iStdErrorCode = result;

	return result;
}

int BNC_SetPrevalue(tBncSetInhibitList *p_psBuyTicketSetInhibitList, tBncSetInhibitList *p_psRechargeSetInhibitList, 
						tBncDevReturn * p_psStatus){
	T_BnrXfsResult result;
	T_DenominationList listQueryDenominations;
	listQueryDenominations.maxSize = MAX_NR_OF_DENOMINATION;
	result = gBRHCmd.QueryDenominations(&listQueryDenominations);
	BRH_FACE_ACEPTED_TABLE table;
	// 设置可接收的面额信息
	for(int i = 0 ;i < p_psBuyTicketSetInhibitList->iNumOfItems ; i++){
		switch(p_psBuyTicketSetInhibitList->psBncCashInfo[i].iValue){
		case 100:
			table.bIsBanknote1Yuan = p_psBuyTicketSetInhibitList->psBncCashInfo[i].bAccepted;
			break;
		case 500:
			table.bIsBanknote5Yuan = p_psBuyTicketSetInhibitList->psBncCashInfo[i].bAccepted;
			break;
		case 1000:
			table.bIsBanknote10Yuan = p_psBuyTicketSetInhibitList->psBncCashInfo[i].bAccepted;
			break;
		case 2000:
			table.bIsBanknote20Yuan = p_psBuyTicketSetInhibitList->psBncCashInfo[i].bAccepted;
			break;
		case 5000:
			table.bIsBanknote50Yuan = p_psBuyTicketSetInhibitList->psBncCashInfo[i].bAccepted;
			break;
		case 10000:
			table.bIsBanknote100Yuan = p_psBuyTicketSetInhibitList->psBncCashInfo[i].bAccepted;
			break;
		default:
			break;
		}
	}

	if(BXR_NO_ERROR != result){
		return result;
	}
	// 设置可接收的面额信息
	for(int i = 0 ;i < listQueryDenominations.size ; i++){
		T_XfsDenominationInfo& item = listQueryDenominations.items[i];
		tBncSetInhibitItem& BncItem = p_psBuyTicketSetInhibitList->psBncCashInfo[i];
		switch(item.cashType.value){
		case 100:
			item.enableDenomination = table.bIsBanknote1Yuan;
			break;
		case 500:
			item.enableDenomination = table.bIsBanknote5Yuan;
			break;
		case 1000:
			item.enableDenomination =  table.bIsBanknote10Yuan;
			break;
		case 2000:
			item.enableDenomination = table.bIsBanknote20Yuan;
			break;
		case 5000:
			item.enableDenomination =  table.bIsBanknote50Yuan;
			break;
		case 10000:
			item.enableDenomination =  table.bIsBanknote100Yuan;
			break;
		default:
			break;
		}
	}
	// 更新可接收面额信息
	result = gBRHCmd.UpdateDenominations(&listQueryDenominations);
	p_psStatus->iStdErrorCode = result;

	if(BXR_NO_ERROR != result){

		return result;
	}
	return result;
}

int BNC_Start(tBncDevReturn * p_psStatus)
{
	T_BnrXfsResult result;
	result = gBRHCmd.CashInStart();
	p_psStatus->iStdErrorCode = result;

	p_psStatus->iStdErrorCode = result;

	return result;
}

int  BNC_GetCashInfo(tBncCashInfo *p_psCashInfo, BYTE& p_byOperation,tBncDevReturn *p_psStatus)
{
	T_BnrXfsResult result;
	T_Amount amount;

	result = gBRHCmd.CashIn(&amount);
	if (p_psCashInfo)
	{
		p_psCashInfo->iNumOfItems = 1;
		if (p_psCashInfo->psBncCashInfo != NULL)
		{
			delete p_psCashInfo->psBncCashInfo;
			p_psCashInfo->psBncCashInfo = NULL;
		}
		p_psCashInfo->psBncCashInfo = new tBncCashItem;
		p_psCashInfo->psBncCashInfo->iCount = 1;
		p_psCashInfo->psBncCashInfo->iValue = amount;
	}
	p_psStatus->iStdErrorCode = result;

	return result;
}

int BNC_Stop(tBncEnCashInfo *p_psCashInf,tBncDevReturn * p_psStatus)
{
	T_BnrXfsResult result;
	T_Amount amount;
	result = gBRHCmd.CashInEnd(&amount);
	p_psStatus->iStdErrorCode = result;
	if(BRH_NO_ERROR == result){
		T_XfsCashOrder m_cashOrder;
		if(gBRHCmd.GetCashOrder(&m_cashOrder)){

			for(int i = 0; i< m_cashOrder.denomination.size ; i++){
				T_XfsDenominationItem item = m_cashOrder.denomination.items[i];

				map<UINT,CASH_BOX_INFO>::iterator it = mapCashBoxInfo.find(item.unit);
				// 逻辑编号对应
				if(it != mapCashBoxInfo.end()){
					// 看钱进入到哪个箱子
					T_XfsPcuName* pBoxName = &(it->second.PhysicalName);
					if(strcmp((char*)pBoxName,BNR_MODULE_CASH_BOX_NAME) == 0 ){// 回收箱
						switch(it->second.cashType.value){
							// 可能投入两种版本的纸币，得累加计数 王峰 20180125
						case 100:
							p_psCashInf->wCashBox_CNY1_IntoCount += item.count;
							break;
						case 500:
							p_psCashInf->wCashBox_CNY5_IntoCount += item.count;
							break;
						case 1000:
							p_psCashInf->wCashBox_CNY10_IntoCount += item.count;
							break;
						case 2000:
							p_psCashInf->wCashBox_CNY20_IntoCount += item.count;
							break;
						case 5000:
							p_psCashInf->wCashBox_CNY50_IntoCount += item.count;
							break;
						case 10000:
							p_psCashInf->wCashBox_CNY100_IntoCount += item.count;
							break;
						}
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0 ){// 循环鼓A
						p_psCashInf->wRecyclerAIntoCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0 ){// 循环鼓B
						p_psCashInf->wRecyclerBIntoCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0 ){// 循环鼓C
						p_psCashInf->wRecyclerCIntoCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0 ){// 循环鼓D
						p_psCashInf->wRecyclerDIntoCount = item.count;
					}
				}
			}
		}
	}

	return result;
}

int BNC_Refund(tBncDevReturn *p_psStatus)
{
	T_BnrXfsResult result;
	T_Amount amount;
	result = gBRHCmd.CashInRollback(&amount);
	p_psStatus->iStdErrorCode = result;

	return result;
}

int BNC_ChangeBill(tBncChangeNum *pChangeNum, tBncDevReturn *pDevStatus)
{
	T_BnrXfsResult result;
	T_XfsDispenseRequest dispenseRequest;
	memset(&dispenseRequest, 0, sizeof(dispenseRequest));
	char currency [4] = {"CNY"};
	strcpy(dispenseRequest.currency.currencyCode, currency);
	dispenseRequest.mixNumber = XFS_C_CDR_MIX_ALGORITHM;
	dispenseRequest.denomination.amount = pChangeNum->uiAmount;
	result = gBRHCmd.Dispense(&dispenseRequest);
	if (result == BRH_NO_ERROR)
	{
		T_XfsCashOrder order;

		if(gBRHCmd.GetCashOrder(&order)){
			pChangeNum->uiAmount = order.denomination.amount;
			memset(pChangeNum->uiOutNumber,0,6);
			for(int i = 0; i < order.denomination.size;i++){
				T_XfsDenominationItem item = order.denomination.items[i];
				map<UINT,CASH_BOX_INFO>::iterator it = mapCashBoxInfo.find(item.unit);
				// 逻辑编号对应
				if(it != mapCashBoxInfo.end()){
					// 看钱从哪个箱子出来
					T_XfsPcuName* pBoxName = &(it->second.PhysicalName);
					if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0)
					{
						pChangeNum->uiOutNumber[0] = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0)
					{
						pChangeNum->uiOutNumber[1] = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0)
					{
						pChangeNum->uiOutNumber[2] = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0)
					{
						pChangeNum->uiOutNumber[3] = item.count;
					}
					else if (strcmp((char*)pBoxName,BNR_MODULE_LOADER_BOX_NAME) == 0)
					{
						pChangeNum->uiOutNumber[4] = item.count;
					}
				}
			}
		}
	}
	pDevStatus->iStdErrorCode = result;

	return result;
}
void RecordResult(char* pcuName,T_XfsCashOrder order,tBncChangeNum* pBoxInfo)
{
	pBoxInfo->uiAmount += order.denomination.amount;

	for(int i = 0; i < order.denomination.size;i++){
		T_XfsDenominationItem item = order.denomination.items[i];

		map<UINT,CASH_BOX_INFO>::iterator it = mapCashBoxInfo.find(item.unit);
		// 逻辑编号对应
		if(it != mapCashBoxInfo.end()){
			// 看钱从哪个箱子出来
			switch(it->second.cashType.value){
			case 100:
				{
					if(strcmp(pcuName,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0)
					{
						pBoxInfo->uiOutNumber[0] = item.count;
					}
					else if(strcmp(pcuName,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0)
					{
						pBoxInfo->uiOutNumber[1] = item.count;
					}
				}
				break;
			case 500:
				{
					if(strcmp(pcuName,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0)
					{
						pBoxInfo->uiOutNumber[2] = item.count;
					}
					else if(strcmp(pcuName,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0)
					{
						pBoxInfo->uiOutNumber[3] = item.count;
					}
					else if (strcmp(pcuName,BNR_MODULE_LOADER_BOX_NAME) == 0)
					{
						pBoxInfo->uiOutNumber[4] = item.count;
					}

				}
				break;
			}
		}
	}
}
int BNC_Empty(tBncChangeNum* pBoxInfo)
{
	BOOL toFloat = FALSE;
	long lCompleteCode = gBRHCmd.Empty((char*)BNR_MODULE_RECYCLER_BOX_A_NAME, toFloat);

	if(lCompleteCode == BRH_NO_ERROR){
		T_XfsCashOrder order;
		if(gBRHCmd.GetCashOrder(&order)){
			RecordResult((char*)BNR_MODULE_RECYCLER_BOX_A_NAME,order,pBoxInfo);
		}
	}
	lCompleteCode = gBRHCmd.Empty((char*)BNR_MODULE_RECYCLER_BOX_B_NAME, toFloat);

	if(lCompleteCode == BRH_NO_ERROR){
		T_XfsCashOrder order;
		if(gBRHCmd.GetCashOrder(&order)){
			RecordResult((char*)BNR_MODULE_RECYCLER_BOX_B_NAME,order,pBoxInfo);
		}
	}
	lCompleteCode = gBRHCmd.Empty((char*)BNR_MODULE_RECYCLER_BOX_C_NAME, toFloat);

	if(lCompleteCode == BRH_NO_ERROR){
		T_XfsCashOrder order;
		if(gBRHCmd.GetCashOrder(&order)){
			RecordResult((char*)BNR_MODULE_RECYCLER_BOX_C_NAME,order,pBoxInfo);
		}
	}
	lCompleteCode = gBRHCmd.Empty((char*)BNR_MODULE_RECYCLER_BOX_D_NAME, toFloat);

	if(lCompleteCode == BRH_NO_ERROR){
		T_XfsCashOrder order;
		if(gBRHCmd.GetCashOrder(&order)){
			RecordResult((char*)BNR_MODULE_RECYCLER_BOX_D_NAME,order,pBoxInfo);
		}
	}
	lCompleteCode = gBRHCmd.Empty((char*)BNR_MODULE_LOADER_BOX_NAME, toFloat);

	if(lCompleteCode == BRH_NO_ERROR){
		T_XfsCashOrder order;
		if(gBRHCmd.GetCashOrder(&order)){
			RecordResult((char*)BNR_MODULE_LOADER_BOX_NAME,order,pBoxInfo);
		}
	}
	return lCompleteCode;
}

int BNC_Cancel(tBncDevReturn *p_psStatus)
{
	T_BnrXfsResult result;
	result = gBRHCmd.Cancel();
	p_psStatus->iStdErrorCode = result;

	return result;
}

int BNC_GetBoxID(char * p_BoxID, tBncDevReturn * p_psStatus)
{
	return 0;
}
int  BNC_Encash(tBncDevReturn *p_psStatus)
{
	return 0;
}
int BNC_Init(tBncInitNumInfo *pNumInfo, tBncDevReturn * pDevStatus)
{
	return 0;
}

int BNC_GetVersion(char* pVersion, tBncDevReturn * p_psStatus)
{
	return 0;
}