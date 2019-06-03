#include "StdAfx.h"
#include "Sync.h"
#include "CoinModuleHelper.h"
#include "CTCoinCountInfo.h"
#include "CoinShutterCommand.h"
#include "IOModuleHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**
@brief  硬币模块控制类实现
*/


// 硬币异常处理
#define INTERNAL_ERROR(errCode) \
{if (IsExceptionCode(errCode)) {\
        throw CCHException(errCode, _T(__FILE__), __LINE__);\
	}else\
	{throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);}}

// 硬币接收口错误异常
//#define SHUTTER_ERROR(errCode)\
//{if(IsShutterExceptionCode(errCode)){\
//	throw CCHShutterException(errCode,_T(__FILE__),__LINE__);\
//}else{\
//	throw CCHShutterHardwareException(errCode,_T(__FILE__),__LINE__);}}
//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleHelper::CCoinModuleHelper():
m_nSortTrayRunCount(0)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleHelper::~CCoinModuleHelper(void)
{
}


//////////////////////////////////////////////////////////////////////////
BOOL CCoinModuleHelper::IsExceptionCode(long errCode)
{
	bool ret = false;

	switch(errCode) {
	case    CH_SET_PARAM_ERR       : // 设定值：参数异常
	case    CH_FUNC_PARAM_ERR      : // 函数参数异常
	case    CH_SEQUENCE_ERR	       : // 函数调用顺序异常
	case    CH_FUNC_ERR_RETURN	   : // 函数返回异常
	case    CH_LOGIC_ERR	       : // 逻辑异常
	case    CH_OTHER_ERR           : // FTP相关错误   
	case    CHTRS_RES_WINERR       : // 系统错误
	case	CCHException::ERROR_CH_SEQUNCE_ERR   :		// 命令时序错误
	case	CCHException::ERROR_CH_SEND		     :		// 发送数据错误
	case	CCHException::ERROR_CH_RECEIVE	     :		// 接收数据错误
	case    CCHException::ERROR_CH_RECEIVE_TIMEOUT:		// 接收数据错误
	case    CHTRS_ERR_HANDLE       : // The handle is invalid.
	case    CHTRS_RES_ERR_CHREAD   : // Could not create thread. Window error.
	case    CHTRS_RES_ERR_TIMEOUT  : // Timeout. Could not finished task in time.
	case    CHTRS_RES_RESET        : // Operation was terminate because of reseting.
	case    CHTRS_RES_STRANGE      : // Unrecognizable response.
	case    CHTRS_RECV_EMPTY       : // There is no data to receive.
	case    CHTRS_HTDL_ERR_FULL    : // Could not open connection because the conneciton pool is full
	case    CHTRS_HTDL_ERR_OPNTO   : // The operation could not finish in time.
	case    CHTRS_HTDL_ERR_WORKNG  : // Could not allocate workspace, out of memory.
	case    CHTRS_HTDL_ERR_SQUENG  : // Could not allocate the queue, out of memory.
	case    CHTRS_HTDL_ERR_RBUFNG  : // Could not allocate buffer for receiving data, out of memory.
	case    CHTRS_RFID_A_SUM_ERR   : // 票箱A的RFID数据SUM值异常
	case    CHTRS_RFID_B_SUM_ERR   : // 票箱B的RFID数据SUM值异常
	case    CHTRS_RFID_A_B_SUM_ERR : // 票箱A和B的RFID数据SUM值异常
	case    CHTRS_MSG_EMPTY	       : // The message is empty now.
	case    CHTRS_MSG_OVRFLW	   : // The buffer is too small to store message.
	case    CHTRS_CMD_INVALID	   : // Unsupported command.
	case    CHTRS_CMD_PARAM	       : // Invalid parameter.
	case    CHTRS_CMD_REJECT	   : // Too many commands are queuing, could not send command.
		ret = true;
		break;
	//Hardware Error!
	//case	CCHException::ERROR_CH_OPEN_COM      :      //打开时　无法打开串口错误
	//case	CCHException::ERROR_CH_CLOSE_COM     :      //打开时　关闭串口错误
	//case	CCHException::ERROR_CH_OPEN_ERROR    :      //打开时　打开错误
	//case	CCHException::ERROR_CH_WAIT_TIMEOUT  :      //打印时  超时
	//case	CCHException::ERROR_CH_CLOSE         :      //关闭时　关闭串口错误
	case    CHTRS_HTDL_ERR_SENDFAIL: // Sending error. Unknow reason.
	case    CHTRS_HTDL_ERR_PORT    : // Could not open the specified serial port.
	case    CHTRS_HTDL_ERR_CTSOFF  : // Serial port does not support CTS (clear-to-send) event
	case    CHTRS_HTDL_ERR_SQUE    : // 发送队列满
	case    CHTRS_HTDL_ERR_RQUE    : // Too many items are queueing to be received
	case    CHTRS_MTX_ERR_ENQUIRE  : // ENQ发送失败，不能发送报文
	case    CHTRS_MTX_ERR_SEND     : // 三次重试仍然发送失败
		ret = false;
		break;
	default:
		break;
	}

	return ret;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       与硬币模块建立连接

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Open()
{
	try {
		long errCode = 0;
		// 未连接
		if(!theAPP_SESSION.IsCHConnected())
		{
			// 硬币模块复位
			int port = theTVM_SETTING.GetCHComPort();
			int baud = theTVM_SETTING.GetCHBaudRate();
			CCHConnect cmd(port,baud,0);
			errCode = cmd.ExecuteCommand();
			//if (errCode == E_NO_CH_ERROR) 
			//{			
			//	errCode = CH_Init();
			//}
			if (E_NO_CH_ERROR != errCode){
				throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
			}
			CCSConnect shuter(theTVM_SETTING.GetCSHComPort(),theTVM_SETTING.GetCSHBaudRate());
			errCode = shuter.ExecuteCommand();
			if (E_NO_CH_ERROR != errCode)
			{
				throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
			}
			theAPP_SESSION.SetIsCHConnected(true);

		}
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       与硬币模块断开连接

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Close()
{
	try {
		long errCode = 0;
		// 已连接
		if(theAPP_SESSION.IsCHConnected()){
			CCSClose csCommand;
			errCode = csCommand.ExecuteCommand();
		
			// 做断开连接操作
			CCHClose command;
			errCode = command.ExecuteCommand();
			if (E_NO_CH_ERROR != errCode){
				throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
			}
			theAPP_SESSION.SetIsCHConnected(false);
		}
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       硬币模块初始化

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Init()
{
	try {
		long errCode = 0;

		CCHInit command(0x00);
		errCode = command.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}
		CCSInit shutInit;
		errCode = shutInit.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      CH打开投币口

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CoinShutter_Open()
{
	try {                     
		IO_HELPER->OpenCoinShutter();
		return 0;
	}
	catch(CSysException&) {
		throw CIOException(CIOException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       获取状态

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_GetStatus(tCHADevStatus* pStatus)
{
	try {
		long errCode = 0;

		CCHGetStatus command;
		errCode = command.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}
		command.GetResponse(pStatus);

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化硬币模块

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_InitModule()
{
	try {
		long errCode = 0;

		errCode = CH_Open();
		errCode = CH_Init();
		//tCHADevStatus status;
		//errCode = CH_GetStatus(&status);
		errCode = CH_SetCoinNum();

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       异常修复

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Repare()
{
	try {
		long errCode = 0;

		errCode = CH_Close();
		errCode = CH_InitModule();
		tCHADevStatus status;
		errCode = CH_GetStatus(&status);
		
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       设置硬币模块各部件的硬币数(初始化时及补币时调用，否则Hopper中无数，无法找零）

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_SetCoinNum()
{
	try {
		long errCode = 0;

		CTCoinCountInfo::COIN_CHANGE_BOX_INFO addBoxA = theCOIN_COUNT.GetChangeCoinboxAInfo();
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO addBoxB = theCOIN_COUNT.GetChangeCoinboxBInfo();
		CTCoinCountInfo::COIN_BOX_INFO hopperA = theCOIN_COUNT.GetCoinHopperboxAInfo();
		CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectA = theCOIN_COUNT.GetCollectionCoinboxAInfo();	
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectB = theCOIN_COUNT.GetCollectionCoinboxBInfo();	
		
		tCoinBoxInfo coinInfo;
		coinInfo.OneY_inHopper = hopperA.ulCurCount;						// HopperA中数量
		coinInfo.HalfY_inHopper = hopperB.ulCurCount;						// HopperB中数量
		coinInfo.OneY_inRecover = collectA.ulCount_1yuan;					// 回收箱1元数量
		coinInfo.OneY_preparation = addBoxA.ulCount;
		coinInfo.HalfY_preparation = addBoxB.ulCount;
		CAfcTVMOperationParam::BILLBOX_PARAM parm;
		theAFC_TVM_OPERATION.GetBillboxParam(parm);
		coinInfo.OneY_maxthreshold_inRecover = parm.coinCollectionAlreadyFullCoinCnt;
		CCHSetCoinNum command(coinInfo);
		errCode = command.ExecuteCommand();
		
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       开始接收硬币

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_StartReceive()
{
	try {
		long errCode = 0;
		// 1. 打开投币口指示灯
		IO_HELPER->OpenCHIndicator();
		// 2. 打开硬币闸门
		errCode = CH_OpenShutter(); 
		if (E_NO_CH_ERROR == errCode){
			// 3. 开始接收
			CCHStartReceive command;
			errCode = command.ExecuteCommand();
		}

		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       开始接收硬币(异步）

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_StartReceive(CService* pCallbackSvc)
{
	try {
		long errCode = 0;
		// 1. 打开投币口指示灯
		IO_HELPER->OpenCHIndicator();
		// 2. 打开投币口
		errCode = CH_OpenShutter(); 
		if (E_NO_CH_ERROR == errCode){		
			// 3. 开始接收
			CCHStartReceive command;
			errCode = command.ExecuteCommand();
		}
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		// 2. 异步读取接收金额
		CH_GetInsertInfoAsync(pCallbackSvc);

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       开始接收硬币(异步）

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_StartReceive(CService* pCallbackSvc, COIN_ACCEPTABLE& coinFaceTable)
{
	try {
		long errCode = 0;
		// 异常安全检查
		if(pCallbackSvc == NULL){
			throw CCHHardwareException(CCHHardwareException::ERROR_CH_BEGIN_INSERT_COMMOND_ERR,_T(__FILE__),__LINE__);
		}

		// 设定参数检查
		if(!coinFaceTable.CH_COIN_ALLOW_05 && !coinFaceTable.CH_COIN_ALLOW_1){// 无效的接收列表
			throw CCHHardwareException(CCHHardwareException::ERROR_CH_BEGIN_INSERT_COMMOND_ERR,_T(__FILE__),__LINE__);
		}
		// 设置拒收金额
		CCHSetRejectType rejectCommand(coinFaceTable);
		errCode = rejectCommand.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		// 1. 打开投币口指示灯
		IO_HELPER->OpenCHIndicator();
		// 2. 打开投币口
		errCode = CH_OpenShutter(); 
		if (E_NO_CH_ERROR == errCode){		
			// 3. 开始接收
			CCHStartReceive revCommand;
			errCode = revCommand.ExecuteCommand();
			if (E_NO_CH_ERROR != errCode){
				throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
			}

		}
		else{
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		// 3. 异步读取接收金额
		CH_GetInsertInfoAsync(pCallbackSvc);

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH读取硬币金额（异步调用）
//
//@param      CService* pCallbackSvc 回调指针
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_GetInsertInfoAsync(CService* pCallbackSvc)
{
	CCHReadReceivs * pChCmd = new CCHReadReceivs();
	pChCmd->SetAutoDeleted(TRUE);
	pChCmd->SetService(pCallbackSvc);
	pChCmd->SetNeedCallbackService(true);
	pChCmd->SetCommandID(COIN_MODULE_COMMAND_ID_READ_COIN_AMOUNT);
	CH_EnableInsert();
	g_pCHControl->DoCommand(pChCmd,SP_EXECMODE_QUEUE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH读取硬币金额（同步调用）
//
//@param      CH_SYSTEM_STATUS_RSP& rsp  命令反馈
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_GetInsertInfo(tCHACoinValueInfo& rsp)
{
	/*
	try {
		long errCode = 0;

		CCHGetStatus readstatus;

		errCode = readstatus.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}		
		readstatus.GetResponse(&rsp);

		//// 硬币箱硬件计数同步到软件计数
		//SyncCoinTemporaryInfoToMasterFile(coin_info);//lichao

		m_nSortTrayRunCount = rsp.nNumOfEscrow1 + rsp.nNumOfEscrow2;

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	*/
	{
		try {
			long errCode = 1;

			{
				CCHReadCoinNum receive;
				errCode = receive.ExecuteCommand();
				if (E_NO_CH_ERROR != errCode) {
					INTERNAL_ERROR(errCode);
				}
				receive.GetResponse(&rsp);
			}
			return errCode;
		}
		catch (CSysException) {
			throw;
		}
		catch (...){
			throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       结束接收硬币

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_EndReceive()
{
	try {
		long errCode = 0;
		// 1. 打开投币口指示灯
		IO_HELPER->CloseCHIndicator();
		// 2. 关闭投币口
		errCode = CH_CloseShutter();
		// 这里不判断关投币口的结果。即使失败，也要结束接收硬币。
		// 3. 结束接收
		CCHEndReceive command;
		errCode = command.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}
		

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH结束监控进币信息
//
//@param      无
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_StopInsertInfo()
{
	try {
		long errCode = 0;
		CHStopReadRV stopMonitor;
		errCode = stopMonitor.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      退回接收的硬币（同步调用）
//
//@param      无
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Refund()
{
	try {
		long errCode = 0;
		CCHRefund Cancel(m_nSortTrayRunCount);
		errCode = Cancel.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		// 审计数据
		theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
		m_nSortTrayRunCount = 0;

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      退回接收的硬币（异步调用）
//
//@param     CService* pCallbackSvc 回调指针
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_RefundAsync(CService* pCallbackSvc)
{
	CCHRefund * pChCmd = new CCHRefund(m_nSortTrayRunCount);
	pChCmd->SetAutoDeleted(TRUE);
	pChCmd->SetService(pCallbackSvc);
	pChCmd->SetNeedCallbackService(true);
	pChCmd->SetCommandID(COIN_MODULE_COMMAND_ID_RETURN);
	g_pCHControl->DoCommand(pChCmd,SP_EXECMODE_QUEUE);

	// 审计数据
	theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
	m_nSortTrayRunCount = 0;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH收纳硬币（同步调用）
//
//@param      (o)CH_COMMON_RSP& rsp 通用命令反馈
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Accept(bool bHopperAccept)
{
	try {
		long errCode = 0;
		//CCHAccept pChCmd(bHopperAccept);
		//errCode = pChCmd.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
		m_nSortTrayRunCount = 0;
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH收纳（异步调用）
//
//@param      (i)CService* pCallbackSvc 
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_AcceptAsync(CService* pCallbackSvc, bool bHopperAccept)
{
	CCHAccept * pChCmd = new CCHAccept();
	pChCmd->SetAutoDeleted(TRUE);
	pChCmd->SetService(pCallbackSvc);
	pChCmd->SetNeedCallbackService(true);
	pChCmd->SetCommandID(COIN_MODULE_COMMAND_ID_ACCEPT);
	g_pCHControl->DoCommand(pChCmd,SP_EXECMODE_QUEUE);

	// 审计数据
	theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
	m_nSortTrayRunCount = 0;

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH硬币找零（同步调用）
//
//@param      
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Change(WORD num_OneY, WORD num_HalfY, tCHAChangeNum* rsp)
{
	try {
		long errCode = 0;
		CCHChange pChCmd(num_OneY, num_HalfY);
		errCode = pChCmd.ExecuteCommand();
		pChCmd.GetResponse(rsp);// 不管成功与失败，都取结果。防止找零不足失败，但找出了硬币。
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
		m_nSortTrayRunCount = 0;
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH收纳（异步调用）
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_ChangeAsync(CService* pCallbackSvc,WORD num_OneY, WORD num_HalfY)
{
	CCHChange * pChCmd = new CCHChange(num_OneY, num_HalfY);
	pChCmd->SetAutoDeleted(TRUE);
	pChCmd->SetService(pCallbackSvc);
	pChCmd->SetNeedCallbackService(true);
	pChCmd->SetCommandID(COIN_MODULE_COMMAND_ID_CHANGE);
	g_pCHControl->DoCommand(pChCmd,SP_EXECMODE_QUEUE);

	// 审计数据
	theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
	m_nSortTrayRunCount = 0;
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      清币到回收箱
//
//@param      void
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_ClearCoin(tCoinBoxInfo *tBoxInfo)
{
	try {
		long errCode = 0;
		CCHClearCoin pChCmd;
		errCode = pChCmd.ExecuteCommand();
		pChCmd.GetResponse(tBoxInfo);
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
		m_nSortTrayRunCount = 0;
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      读RFID
//
//@param      void
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
CString CCoinModuleHelper::CH_ReadRfidData(int boxNo)
{
	try {
		long errCode = 0;
		USES_CONVERSION;
		//CCHReadRfid pChCmd(boxNo);
		char RfidData[8] = {0};
		//errCode = pChCmd.ExecuteCommand();
		//pChCmd.GetResponse((BYTE*)RfidData);
		if (E_NO_CH_ERROR != errCode){
			return CString(_T(""));
		}
		
		return A2T(RfidData);
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      读RFID
//
//@param      void
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_WriteRfidData(int boxNo, BYTE* rfidData)
{
	try {
		long errCode = 0;
		// 箱子编号：0x01 = 加币箱2  0x02 = 加币箱1  0x03 = 回收箱1  0x04 = 回收箱2
		if (boxNo!=0x03){
			return 1;
		}

		//CCHWriteRfid pChCmd(boxNo, rfidData);
		//errCode = pChCmd.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			
		}
		
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}



//////////////////////////////////////////////////////////////////////////////
//**
//@brief      打开投币口
//
//@param      void
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_OpenShutter()
{
	try {
		long errCode = 0;
		CCSOpenShutter pChCmd;
		errCode = pChCmd.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      关闭投币口
//
//@param      void
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_CloseShutter()
{
	try {
		long errCode = 0;
		CCSCloseShutter pChCmd;
		errCode = pChCmd.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      修复投币口
//
//@param      void
//
//@retval     long    错误代码
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_RepareShutter()
{
	try {
		long errCode = 0;
		errCode = CH_CloseShutter();
		errCode = CH_OpenShutter();

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}