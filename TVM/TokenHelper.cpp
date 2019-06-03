#pragma once
#include "StdAfx.h"
#include "TOKENHelper.h"
#include "TOKENException.h"
#include "TOKENHardwareException.h"
#include "TicketHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef TOKENIS_FILE
static TCHAR TOKENIS_FILE[] = _T(__FILE__);
#endif

//@brief TOKEN Helper

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENHelper::CTOKENHelper(CService& service):CServiceHelper(service)
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
CTOKENHelper::~CTOKENHelper(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TOKEN建立连接命令

@param      无

@retval     无

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENHelper::Connect()
{
	try {
		// 未连接
		if(!theAPP_SESSION.IsTOKENModuleConnected()) {
			CommOpen();
			// 设置已连接
			theAPP_SESSION.SetIsTOKENModuleConnected(true);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...){
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TOKEN断开连接

@param      无

@retval     无

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENHelper::Disconnect()
{
	try {
		// 已连接
		if(theAPP_SESSION.IsTOKENModuleConnected()) {
			CommClose();
			// 设置未连接
			theAPP_SESSION.SetIsTOKENModuleConnected(false);
		}		
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化出票机

@param       无 

@retval      CTCMException

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENHelper::Initialize()
{

	try {
		TOKEN_RSP_GET_DEV_STATUS th_rsp_get_dev_status;
		UINT uiRetractNum = 0;

		// 打开串口连接出票机
		Connect();

		//// 硬件故障
		//if(FAULT_HARDWARE == th_rsp_get_dev_status.uiErrorType){
		//	// 复位命令
		//	Reset(TOKEN_CMD_RESET::CMD_RESET_FAREBOX, &uiRetractNum);
		//}

		UINT ulReturn = 0;
		Init(&ulReturn);

		// 获取状态
		GetDevStatus(&th_rsp_get_dev_status);

		//// 设置模块工作模式
		//SetWorkMode(TOKEN_CMD_SET_WORK_MODE::CMD_WORK_MODE_NORMAL);

		// 设置版本
		if(!theEXCEPTION_MGR.HasException(CTOKENHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTOKENException::MODULE_ID))
		{
			// 记录TOKEN硬件版本
			TOKEN_RSP_GET_VERSION th_rsp_get_version;
			GetVersion(&th_rsp_get_version);

			//DWORD dwVersion = th_rsp_get_version.ucFwVer
			//	
			//	((th_rsp_get_version.cFirmWareVersion[8] - 48) & 0x0F)*1000 + 
			//	((th_rsp_get_version.cFirmWareVersion[10] - 48) & 0x0F) *100 +
			//	((th_rsp_get_version.cFirmWareVersion[12] - 48) & 0x0F) *10 +
			//	((th_rsp_get_version.cFirmWareVersion[13] - 48) & 0x0F);
			//// TODO 串口数据格式转换
			//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TOKEN_APP,dwVersion);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       修复出票机

@param       无 

@retval      bool   true:成功; false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTOKENHelper::Repare()
{
	bool result = false;
	TOKEN_RSP_GET_DEV_STATUS th_rsp_get_dev_status;
	UINT uiRetractNum = 0;
	try {

		// 停止工作
		//SetWorkMode(TOKEN_CMD_SET_WORK_MODE::CMD_WORK_MODE_MAINTENANCE);

		// 断开出票机
		Disconnect();

		// 连接出票机
		Connect();

		UINT ulReturn = 0;
		Init(&ulReturn);

		GetDevStatus(&th_rsp_get_dev_status);

		// 设置版本
		if(!theEXCEPTION_MGR.HasException(CTOKENHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTOKENException::MODULE_ID))
		{
			// 记录TOKEN硬件版本
			TOKEN_RSP_GET_VERSION th_rsp_get_version;
			GetVersion(&th_rsp_get_version);
			//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TOKEN_APP,thInfo.TOKENVersion);
		}

		result = true;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
	return result;
}

// 测试出票机 
void CTOKENHelper::Test()
{
	bool result = false;
	UINT uiTransportNum = 0;
	UINT uiRetractNum = 0;
	try {
		// 获取票卡
		CardOut(TOKEN_BOX_A, &uiTransportNum);
		// 送出票卡
		SendCard(&uiTransportNum);
		// 获取票卡
		CardOut(TOKEN_BOX_A, &uiTransportNum);
		// 回收票卡
		RetractCard(&uiRetractNum);

		result = true;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
	//return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       检查票箱到位状态

@param       (o)TOKEN_RSP_GET_DEV_STATUS& response    读取设备状态参数结构
@param       (o)bool& isMagazineAExist             票箱A到位状态
@param       (o)bool& isMagazineBExist             票箱B到位状态
@param       (o)bool& isMagazineCExist             票箱C到位状态

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENHelper::CheckTicketBoxExist(TOKEN_RSP_GET_DEV_STATUS* response, bool& isMagazineAExist,bool& isMagazineBExist,bool& isMagazineCExist,bool& isMagazineDExist)
{
	long lErrCode = TIM_FAULT_CMD;
	do {
		// 读取设备状态
		lErrCode = GetDevStatus(response);
		if (TIM_FAULT_NORMAL != lErrCode){
			throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
		}

		isMagazineAExist = false;
		isMagazineBExist = false;
		isMagazineCExist = false;
		isMagazineDExist = false;
		IO_RSP_STATUS_INFO ioStatus;
		if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
			try{
				IO_HELPER->GetIOStatus(ioStatus);
				isMagazineCExist = ioStatus.isTokenInvBoxReady;
				isMagazineDExist = ioStatus.isTokenColBoxReady;
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...){
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}
		isMagazineAExist = response->bBoxAExist;
		isMagazineBExist = response->bBoxBExist;

	} while (0);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      是否为需要抛异常的ID

@param      (i)long lErrCode      故障代码

@retval     bool    true:异常的错误代码; false:不抛异常的

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTOKENHelper::IsExceptionCode(long lErrCode)
{
	if (lErrCode!=TIM_ERR_CODE_NONE){
		return true;
	}
	return false;


	//bool bRet = false;

	//switch(lErrCode) {
	//case FAULT_CMD					 :
	//	//FAULT_CMD
	//case ERR_CODE_INVALID_CMD        : // invalid command
	//case ERR_CODE_PARAM_ERR          : // parameter error
	//case ERR_CODE_UPLOAD_DATA_ERR    : // the active upload data timeout
	//case ERR_CODE_RECIEVE_TIMEOUT    : // recieve timeout
	//case ERR_CODE_COMCONFIGURE       : // com open error
	//case ERR_CODE_TO_NORMALMD        : // maintenance mode recieve normal mode command
	//case ERR_CODE_TO_MAINTMD         : // normal mode recieve maintenance mode command
	//case ERR_CODE_RECIEVE            : // recieve error
	//	//FAULT_HARDWARE
	//case ERR_CODE_EXIT_SEN           : // exit detect sensor error
	//case ERR_CODE_CSC_SEN            : // csc area sensor error
	//case ERR_CODE_A_ENTRY_SEN        : // A entrance Channel sensor status
	//case ERR_CODE_B_ENTRY_SEN        : // B entrance Channel sensor status
	//case ERR_CODE_SENSOR5            : // sensor5 error
	//case ERR_CODE_CHG_DIRE_SEN       : // change direction sensor status
	//case ERR_CODE_BOXB_TICKET_DE_SEN : // case box B ticket to anti detect sensor
	//case ERR_CODE_A_AVL_SEN          : // case box A available detection sensor
	//case ERR_CODE_A_COVER_SEN        : // case box A cover status sensor
	//case ERR_CODE_A_AE_SEN           : // case box A about empty detection sensor
	//case ERR_CODE_A_EM_SEN           : // case box A empty detection sensor
	//case ERR_CODE_A_MOTOR_UP_SEN     : // case box A motor up sensor status
	//case ERR_CODE_A_MOTOR_DW_SEN     : // case box A motor down sensor status
	//case ERR_CODE_B_AVL_SEN          : // case box B available detection sensor
	//case ERR_CODE_B_COVER_SEN        : // case box B cover sensor
	//case ERR_CODE_B_AE_SEN           : // case box B about empty detection sensor
	//case ERR_CODE_B_EM_SEN           : // case box B empty detection sensor
	//case ERR_CODE_B_MOTOR_UP_SEN     : // case box B motor up sensor status
	//case ERR_CODE_B_MOTOR_DW_SEN     : // case box B motor down sensor status
	//case ERR_CODE_FARE_AVL_SEN       : // fare box available detection sensor
	//case ERR_CODE_FARE_COVER_SEN     : // fare box Cover detection sensor
	//case ERR_CODE_FARE_FULL_SEN      : // fare box full detection sensor
	//case ERR_CODE_FARE_RETURN_SEN    : // fare box return ticket sensor
	//case ERR_CODE_DIG1_LO1_SEN       : // dig card wheel 1 load 1 sensor status
	//case ERR_CODE_DIG1_LO2_SEN       : // dig card wheel 1 load 2 sensor status
	//case ERR_CODE_DIG2_LO1_SEN       : // dig card wheel 2 load 1 sensor status
	//case ERR_CODE_DIG2_LO2_SEN       : // dig card wheel 2 load 2 sensor status
	//case ERR_CODE_SETBLOCKA          : // set block A failure
	//case ERR_CODE_READBLOCKA         : // read block A failure
	//case ERR_CODE_READBOXID          : // read box id failure
	//case ERR_CODE_SETBOXID           : // set box id failure
	//case ERR_CODE_READRFIDFAILURE    : // two block of RFID couldn't read
	//case ERR_CODE_SETBLOCKB          : // set block B failure
	//case ERR_CODE_READBLOCKB         : // read block B failure
	//case ERR_CODE_A_UP_DW_SEN        : // case box A motor up or down sensor status
	//case ERR_CODE_B_UP_DW_SEN        : // case box B motor up or down sensor status
	//case ERR_CODE_READ_SN            : // read sn failure
	//case ERR_CODE_WRITE_SN           : // write sn failure
	//	//FAULT_CHANNEL
	//case ERR_CODE_CASEBOXA_JAM       : // case box A dig ticket failed
	//case ERR_CODE_CASEBOXB_JAM       : // case box B dig ticket failed
	//case ERR_CODE_BOXA_ANTN_JAM      : // jam in from case boxA to ANTN
	//case ERR_CODE_BOXB_ANTN_JAM      : // jam in from case boxB to ANTN
	//case ERR_CODE_TICK_SELL_JAM      : // ticket sell jam
	//case ERR_CODE_FAREBOX_JAM        : // jam in form ANTN to farebox
	//case ERR_CODE_NOCARD_IN_ANTNAREA : // No card in antenna area
	//case ERR_CODE_CARD_IN_ANTNAREA   : // one card in antenna area
	//	//FAULT_CARDBOX
	//case ERR_CODE_CASEBOXABF_NA      : // case box A,B,farebox not available
	//case ERR_CODE_FAREBOX_COVER      : // fare box cover not opened
	//case ERR_CODE_CASEBOXA_COVER     : // case box A cover error
	//case ERR_CODE_CASEBOXB_COVER     : // case box B cover error
	//case ERR_CODE_CASEBOXA_AE        : // case box A about empty
	//case ERR_CODE_CASEBOXB_AE        : // case box B about empty
	//case ERR_CODE_CASEBOXAB_AE       : // case box A and case box B about empty
	//case ERR_CODE_CASEBOXA_EM        : // case box A empty
	//case ERR_CODE_CASEBOXB_EM        : // case box B empty
	//case ERR_CODE_FAREBOX_FU         : // fare box full
	//case ERR_CODE_CASEBOXAB_EM       : // case box A and B empty
	//case ERR_CODE_CASEBOXA_NA        : // case box A not available
	//case ERR_CODE_CASEBOXB_NA        : // case box B not available
	//case ERR_CODE_FAREBOX_NA         : // fare box not available
	//case ERR_CODE_CASEBOXAB_NA       : // case box A and B not available                                                                    
	//	bRet = true;
	//	break;
	//default:
	//	break;
	//}
	//return bRet;
}

// 雷格特API封装
//////////////////////////////////////////////////////////////////////////
/**
@brief      通讯建立

@param      无

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::CommOpen()
{
	TOKEN_CMD_COMM_OPEN cmd_comm_open;
	cmd_comm_open.uiCommPort = theTVM_SETTING.GetTokenComPort();
	cmd_comm_open.uiBaudRate = theTVM_SETTING.GetTokenBaudRate();
	CTOKENCommOpen cTOKENCommOpen(cmd_comm_open);
	long lErrCode = cTOKENCommOpen.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭通讯

@param      无

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::CommClose()
{
	CTOKENCommClose cTOKENCommClose;
	long lErrCode = cTOKENCommClose.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置模块工作模式
//
//@param      (i)TOKEN_CMD_SET_WORK_MODE command      输入参数
//
//@retval     long    0:成功; 其他:错误代码
//
//@exception  CTCMException
//*/
////////////////////////////////////////////////////////////////////////////
//long CTOKENHelper::SetWorkMode(TOKEN_CMD_SET_WORK_MODE command)
//{
//	CTOKENSetWorkMode cTOKENSetWorkMode(command);
//	long lErrCode = cTOKENSetWorkMode.ExecuteCommand();
//	if (IsExceptionCode(lErrCode)) {
//		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
//	}
//	return lErrCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取票卡

@param      (i)TOKEN_CMD_CARD_OUT command      输入参数
@param      (o)TOKEN_RSP_CARD_OUT& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::CardOut(TOKEN_BOX_NO boxNo, UINT* uiTransportNum)
{
	CTOKENCardOut cTOKENCardOut(boxNo);
	long lErrCode = cTOKENCardOut.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENCardOut.GetTOKENResponse(uiTransportNum);
	}
	// 审计数据
	if(boxNo == CMD_CARD_OUT_BOX_A)
		theDeviceRegAR.AddThScratchWheelACount();
	else
		theDeviceRegAR.AddThScratchWheelBCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      送出票卡

@param      (o)TOKEN_RSP_SEND_CARD& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::SendCard(UINT* uiTransportNum)
{
	CTOKENSendCard cTOKENSendCard;
	long lErrCode = cTOKENSendCard.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENSendCard.GetTOKENResponse(uiTransportNum);
	}
	// 审计数据
	theDeviceRegAR.AddThSendOutMotorCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      回收票卡

@param      (o)TOKEN_RSP_RETRACT_CARD& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::RetractCard(UINT* uiRetractNum)
{
	CTOKENRetractCard cTOKENRetractCard;
	long lErrCode = cTOKENRetractCard.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENRetractCard.GetTOKENResponse(uiRetractNum);
	}
	// 审计数据
	theDeviceRegAR.AddThLinkElecFlipCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清理票卡

@param      (i)TOKEN_BOX_NO boxNo    票箱编号

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::CleanOut(TOKEN_BOX_NO boxNo)
{
	CTOKENCleanOut cTOKENCleanOut(boxNo);
	long lErrCode = cTOKENCleanOut.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	// 审计数据
	theDeviceRegAR.AddThLinkElecFlipCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取设备状态

@param      (o)TOKEN_RSP_GET_DEV_STATUS& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::GetDevStatus(TOKEN_RSP_GET_DEV_STATUS* response)
{
	CTOKENGetDevStatus cTOKENGetDevStatus;
	long lErrCode = cTOKENGetDevStatus.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
		// 如果获取状态出错，复位。复位后，设备还必须重新初始化。
		//Reset();
		//UINT retractNum = 0;
		//Init(&retractNum);
	}
	else{
		cTOKENGetDevStatus.GetTOKENResponse(response);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      模块复位

@param      (i)TOKEN_CMD_RESET command      输入参数
@param      (o)TOKEN_RSP_RESET& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::Reset()
{
	CTOKENReset cTOKENReset;
	long lErrCode = cTOKENReset.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	//if (ERR_CODE_NONE == lErrCode) {
	//	cTOKENReset.GetTOKENResponse(uiRetractNum);
	//}
	theDeviceRegAR.AddThResetCount();
	// 等待复位硬件动作30秒完成，否则无法通信
	Sleep(33000);
	return lErrCode;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置票箱数量信息
//
//@param      (i)TOKEN_CMD_SET_COUNT_INFO inBoxID      输入参数
//@param      (i)TOKEN_RSP_SET_COUNT_INFO& inCardNum   输入参数
//
//@retval     long    0:成功; 其他:错误代码
//
//@exception  CTCMException
//*/
////////////////////////////////////////////////////////////////////////////
//long CTOKENHelper::SetTicketBoxCountInfo(TOKEN_CMD_SET_COUNT_INFO inBoxID, tTIMCardNum* inCardNum)
//{
//	CTOKENSetTicketBoxCountInfo cTOKENSetTicketBoxCountInfo(inBoxID,inCardNum);
//	long lErrCode = cTOKENSetTicketBoxCountInfo.ExecuteCommand();
//	if (IsExceptionCode(lErrCode)) {
//		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
//	}
//	tTIMDevReturn DevReturn;
//	if (ERR_CODE_NONE == lErrCode) {
//		cTOKENSetTicketBoxCountInfo.GetTOKENResponse(&DevReturn);
//	}
//	return lErrCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      读取票箱数量信息
//
//@param      (i)TOKEN_CMD_GET_COUNT_INFO command      输入参数
//@param      (o)TOKEN_RSP_GET_COUNT_INFO& response    输出参数
//
//@retval     long    0:成功; 其他:错误代码
//
//@exception  CTCMException
//*/
////////////////////////////////////////////////////////////////////////////
//long CTOKENHelper::GetTicketBoxCountInfo(TOKEN_CMD_GET_COUNT_INFO command, tTIMCardNum* response)
//{
//	CTOKENGetTicketBoxCountInfo cTOKENGetTicketBoxCountInfo(command);
//	long lErrCode = cTOKENGetTicketBoxCountInfo.ExecuteCommand();
//	if (IsExceptionCode(lErrCode)) {
//		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
//	}
//	if (ERR_CODE_NONE == lErrCode) {
//		cTOKENGetTicketBoxCountInfo.GetTOKENResponse(response);
//	}
//	return lErrCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      模块初始化

@param      (i)TOKEN_CMD_INIT command      输入参数
@param      (o)TOKEN_RSP_INIT& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::Init(UINT* uiRetractNum)
{
	// 20171124，lichao：实际硬件通道几乎不可能卡票，因此不做清理。清理反而会将暂存区的备票清到回收箱。
	TOKEN_CMD_INIT command = CMD_INIT_NOT_CLEAN/*CMD_INIT_CLEAN*/;
	CTOKENInit cTOKENInit(command);
	long lErrCode = cTOKENInit.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENInit.GetTOKENResponse(uiRetractNum);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取设备驱动版本信息

@param      (o)TOKEN_RSP_GET_VERSION& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::GetVersion(TOKEN_RSP_GET_VERSION* response)
{
	CTOKENGetVersion cTOKENGetVersion;
	long lErrCode = cTOKENGetVersion.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENGetVersion.GetTOKENResponse(response);
	}
	return lErrCode;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置模块测试
//
//@param      (i)TOKEN_CMD_TEST_SETTING command      输入参数
//@param      (o)TOKEN_RSP_TEST_SETTING& response    输出参数
//
//@retval     long    0:成功; 其他:错误代码
//
//@exception  CTCMException
//*/
////////////////////////////////////////////////////////////////////////////
//long CTOKENHelper::TestSetting(TOKEN_CMD_TEST_SETTING command)
//{
//	CTOKENTestSetting CTOKENTestSetting(command);
//	long lErrCode = CTOKENTestSetting.ExecuteCommand();
//	if (IsExceptionCode(lErrCode)) {
//		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
//	}
//	return lErrCode;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读取RFID

@param      IN TOKEN_CMD_SET_COUNT_INFO boxType(箱子类型)
			OUT tTIMRFIDInfo* pTimRfidInfo(输出RFID信息)

@retval     bool true成功 false失败;

@exception  无
*/
////////////////////////////////////////////////////////////////////////
bool CTOKENHelper::ReadTicketBoxRFID(TOKEN_READ_WRITE_RFID* pTimRfidInfo)
{
	CTOKENReadRFIDInfo readRfid(pTimRfidInfo->ucRfidNo);
	long lRet = readRfid.ExecuteCommand();

	if(IsExceptionCode(lRet)){
		throw CTOKENHardwareException(lRet,_T(__FILE__),__LINE__);
	}
	
	if (TIM_ERR_CODE_NONE == lRet) {
		readRfid.GetTOKENResponse(pTimRfidInfo);
	}	

	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      写票箱RFID

@param      无

@retval     无

@exception  无
*/
////////////////////////////////////////////////////////////////////////
bool CTOKENHelper::WriteTicketBoxRFID(TOKEN_READ_WRITE_RFID* pTimRfidInfo)
{
	CTOKENWriteRFIDInfo writeRfid(pTimRfidInfo);
	long lRet = writeRfid.ExecuteCommand();
	if(IsExceptionCode(lRet)){
		throw CTOKENHardwareException(lRet,_T(__FILE__),__LINE__);
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      更换箱子

@param      IN SC_REPLACE_BOX_CMD_INFO& rpsBoxInfo

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//bool CTOKENHelper::ReplaceTicketBox(IN SC_REPLACE_BOX_CMD_INFO& rpsBoxInfo)
//{
	//// 硬件动作(卸载)
	//if(rpsBoxInfo.actionType == TOKEN_UNINSTALL){
	//	try{
	//		if(rpsBoxInfo.boxId == TOKEN_BOX_A){
	//			TestSetting(CMD_TEST_A_TRAY_DOWN);
	//		}
	//		else if(rpsBoxInfo.boxId == TOKEN_BOX_B){
	//			TestSetting(CMD_TEST_B_TRAY_DOWN);
	//		}
	//	}
	//	catch(...){
	//		return false;
	//	}
	//}

	//// 安装
	//if(rpsBoxInfo.actionType == TOKEN_INSTALL){
	//	try{
	//		if(rpsBoxInfo.boxId == TOKEN_BOX_A){
	//			TestSetting(CMD_TEST_A_TRAY_UP);
	//		}
	//		else if(rpsBoxInfo.boxId == TOKEN_BOX_B){
	//			TestSetting(CMD_TEST_B_TRAY_UP);
	//		}
	//	}
	//	catch(...){
	//		return false;
	//	}
	//}

//	// 审计数据
//	if(rpsBoxInfo.boxId == TOKEN_BOX_A)
//		theDeviceRegAR.AddThBoxAUpDownCount();
//	else
//		theDeviceRegAR.AddThBoxBUpDownCount();
//
//	/*if(lRet != 0){
//	return false;
//	}*/
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发卡模块更新

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
 int CTOKENHelper::UpdateProgram(){
	int iRet = 0;
	try
	{
		//CString fileName = (CString)STR_TVM_TOKEN_APP;
		//vector<CString> updateFiles;

		//CString sAppPath;
		//GetAppPath(sAppPath);
		//CString updateFileFolder = sAppPath + TVM_TOKEN_UPDATE_DIR;//BOM_TC_UPDATE_DIR;


		//SearchFile(updateFileFolder + fileName + _T("*.*"),updateFiles);
		//if(updateFiles.size()<=0)
		//{
		//	return -2;
		//}

		//for(int i=0;i<updateFiles.size();i++)
		//{
		//	CString updateFile = updateFiles[i];
		//	string strFilePath = (LPCSTR)CStringA(updateFile);
		//	SetFileAttributes(updateFile,FILE_ATTRIBUTE_NORMAL);
		//	CString upgradeVersionStr = thePARAM_HELPER.GetProgVerFromProgName(updateFile);	// 获取升级的TOKEN版本
		//	DWORD upgradeVersion = _ttoi(upgradeVersionStr);

		//	// 版本无效
		//	if(UNAVALIBLE_VERSION == upgradeVersion){
		//		DeleteFile(updateFile);
		//		return -3;
		//	}

		//	// 读出bin文件的信息
		//	/*VARIABLE_DATA data;
		//	{
		//	CXFile file(updateFile);
		//	file.ReadBinFile(data);
		//	}
		//	DeleteFile(updateFile);*/

		//	theAPP_SESSION.SetHasProgUpdate(true);
		//	TOKENUpdateCommand cmdUpdate(theSETTING.GetTOKENComPort(),theSETTING.GetTOKENBaudRate(),theSETTING.GetTOKENUpadateBaudRate(),(char*)strFilePath.c_str());
		//	if (RW_RSP_OK == cmdUpdate.Execute()) {
		//		DeleteFile(updateFile);
		//		//theSETTING.SetRWUpdateFlag(0,true);
		//		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TOKEN_APP,upgradeVersion);//更新本地版本
		//	}
		//	else{
		//		//CVersionInfo::PROG_TYPE progType;
		//		//if(isMain){
		//		//	progType = CVersionInfo::RW_MAIN;
		//		//}
		//		//else{
		//		//	progType = CVersionInfo::RW_APP;
		//		//}
		//		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,progType,currentVersion);//更新失败，保留TPU当前实际版本。
		//		iRet = -1;
		//	}
		//	// 更新主控后等到120秒方可进行应用部程序更新
		//	/*if(isMain){
		//		Sleep(120*1000);
		//	}*/
		//}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return iRet;
}

 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      尝试修复出票机

 @param      无

 @retval     bool true成功 false失败

 @exception  无
 */
 //////////////////////////////////////////////////////////////////////////
 bool CTOKENHelper::TryRepareTOKEN(){

	 try{
		 Repare();
	 }
	 catch(CSysException& e){
		 theEXCEPTION_MGR.ProcessException(e);
		 return false;
	 }
	 catch(...){
		 return false;
	 }
	 return true;
 }


 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      取线路箱子唯一编号

 @param      无

 @retval     无

 @exception  无
 */
 //////////////////////////////////////////////////////////////////////////
 //bool CTOKENHelper::GetStaticBoxId(IN TOKEN_BOX_NO boxId,OUT tTIMRFIDInfo& rfidInfo){

	// rfidInfo.ucBoxID[0] = theMAINTENANCE_INFO.GetDeviceType();
	// rfidInfo.ucBoxID[1] = 0x01;
	// rfidInfo.ucBoxID[2] = Dec2Hex(theMAINTENANCE_INFO.GetStationCode());
	// BYTE bNum = theMAINTENANCE_INFO.GetEquNumberInGroup();
	// CString strNum(_T(""));

	// switch(boxId){
	// case TOKEN_BOX_A:
	//	 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-2);
	//	 rfidInfo.ucBoxID[3] = 2 * bNum - 2/*_tcstoul(strNum,NULL,16)*/;
	//	 break;
	// case TOKEN_BOX_B:
	//	 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-1);
	//	 rfidInfo.ucBoxID[3] = 2 * bNum - 1/*_tcstoul(strNum,NULL,16)*/;
	//	 break;
	// case TOKEN_BOX_C:
	//	 rfidInfo.ucBoxID[1] = 0x02;
	//	 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-2);
	//	 rfidInfo.ucBoxID[3] = 2 * bNum/*_tcstoul(strNum,NULL,16)*/;
	//	 break;
	// default:
	//	 return false;
	// }

	// return true;
 //}


 long CTOKENHelper::TokenCyclerRfidInit()
 {
	long lRet = 0;

	// 断开连接
	CTCRfidClose rfidCloseCmd;
	lRet = rfidCloseCmd.ExecuteCommand();

	// 连接回收箱RFID读写器
	CTCRfidConnect rfidCommand(theTVM_SETTING.GetTokenCyclerComPort(), theTVM_SETTING.GetTokenCyclerBaudRate());
	lRet = rfidCommand.ExecuteCommand();
	//if(IsExceptionCode(lRet)){
	// throw CTOKENHardwareException(lRet,_T(__FILE__),__LINE__);
	//}
	if (lRet){
		return lRet;
	}

	// 读写前准备
	CTCRfidInit rfidInitCommand;
	lRet = rfidInitCommand.ExecuteCommand();
	//if(IsExceptionCode(lRet)){
	// throw CTOKENHardwareException(lRet,_T(__FILE__),__LINE__);
	//}
	if (lRet){
		return lRet;
	}

	return 0;
 }


 long CTOKENHelper::TokenCyclerRfidRead(TOKEN_READ_WRITE_RFID* pTimRfidInfo)
 {
	long lRet = 0;
	lRet = TokenCyclerRfidInit();
	if (lRet){
		return lRet;
	}

	CTCRfidRead rfidCommand;
	lRet = rfidCommand.ExecuteCommand();
	if (lRet==0){
		rfidCommand.GetResponse(pTimRfidInfo->rfidData.uBoxNo);
	}

	return 0;
 }


 long CTOKENHelper::TokenCyclerRfidWrite(TOKEN_READ_WRITE_RFID* pTimRfidInfo)
 {
	 long lRet = 0;
	 lRet = TokenCyclerRfidInit();
	 if (lRet){
		 return lRet;
	 }

	 CTCRfidWrite rfidCommand(pTimRfidInfo->rfidData.uBoxNo);
	 lRet = rfidCommand.ExecuteCommand();

	 return lRet;
 }


 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      取TOKEN箱A的RFID

 @param      无

 @retval     CString strRFID;箱子的RFID

 @exception  无
 */
 //////////////////////////////////////////////////////////////////////////
 CString CTOKENHelper::ReadTokenBoxARFID()
 {
	CString strRFID(_T(""));

	try{
		USES_CONVERSION;
		TOKEN_READ_WRITE_RFID readTokenData;
		readTokenData.ucRfidNo = 0x04;
		if(ReadTicketBoxRFID(&readTokenData)){
			CString strTemp;
			strRFID.Format(_T("%s"), A2T((const char*)readTokenData.rfidData.uBoxNo));
			return strRFID;
		}

		return strRFID;
	 }
	 catch(...){
		return strRFID;
	 }
 }


 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      取TOKEN箱B的RFID

 @param      无

 @retval     CString strRFID;箱子的RFID 

 @exception  无
 */
 //////////////////////////////////////////////////////////////////////////
 CString CTOKENHelper::ReadTokenBoxBRFID()
 {
	 CString strRFID(_T(""));

	 try{
		 USES_CONVERSION;
		 TOKEN_READ_WRITE_RFID readTokenData;
		 readTokenData.ucRfidNo = 0x03;
		 if(ReadTicketBoxRFID(&readTokenData)){
			 CString strTemp;
			 strRFID.Format(_T("%s"), A2T((const char*)readTokenData.rfidData.uBoxNo));
			 return strRFID;
		 }

		 return strRFID;
	 }
	 catch(...){
		 return strRFID;
	 }
 }

 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      取废票箱RFID

 @param      无

 @retval     CString strRFID；箱子的ID

 @exception  无
 */
 //////////////////////////////////////////////////////////////////////////
 CString CTOKENHelper::ReadInvalidBoxRFID()
 {
	 CString strRFID(_T(""));

	 try{
		 strRFID.Format(_T("%02d03%04d"),theMAINTENANCE_INFO.GetLineCode(),1);
		 return strRFID;
	 }
	 catch(...){
		 return strRFID;
	 }
 }


  //////////////////////////////////////////////////////////////////////////
 /*
 @brief      取TOKEN回收箱的RFID

 @param      无

 @retval     CString strRFID;箱子的RFID 

 @exception  无
 */
 //////////////////////////////////////////////////////////////////////////
 CString CTOKENHelper::ReadTokenBoxCollectRFID()
 {
	 CString strRFID(_T(""));

	 try{
		 USES_CONVERSION;
		 TOKEN_READ_WRITE_RFID sTimRfidInfo;
		 if(TokenCyclerRfidRead(&sTimRfidInfo)==0){
			 CString strTemp;
			 strRFID.Format(_T("%s"), A2T((const char*)sTimRfidInfo.rfidData.uBoxNo));
			 return strRFID;
		 }

		 return strRFID;
	 }
	 catch(...){
		 return strRFID;
	 }
 }