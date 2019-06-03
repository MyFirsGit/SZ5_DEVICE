#pragma once
#include "StdAfx.h"
#include "THHelper.h"
//#include "THException.h"
#include "TicketHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#endif

//@brief TH Helper

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHHelper::CTHHelper(CService& service):CServiceHelper(service)
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
CTHHelper::~CTHHelper(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TH建立连接命令

@param      无

@retval     无

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHHelper::Connect()
{
	try {
		// 未连接
		if(!theAPP_SESSION.IsTHConnected()) {
			CommOpen();
			// 设置已连接
			theAPP_SESSION.SetIsTHConnected(true);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...){
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TH断开连接

@param      无

@retval     无

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHHelper::Disconnect()
{
	try {
		// 已连接
		if(theAPP_SESSION.IsTHConnected()) {
			// 管道通信不断开
			// 			if(theTCM_INFO.GetTHCommuMethod() != 2){
			CommClose();
			// 设置未连接
			theAPP_SESSION.SetIsTHConnected(false);
			// 			}
		}		
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
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
void CTHHelper::Initialize()
{

	try {
		TH_RSP_GET_DEV_STATUS th_rsp_get_dev_status;
		UINT uiRetractNum = 0;

		// 打开串口连接出票机
		Connect();

		// 获取状态
		GetDevStatus(&th_rsp_get_dev_status);
		// 硬件故障
		if(TIM_FAULT_HARDWARE == th_rsp_get_dev_status.uiErrorType){
			// 复位命令
			Reset(CMD_INIT_FAREBOX, &uiRetractNum);
		}

		UINT ulReturn = 0;
		Init(CMD_INIT_FAREBOX,&ulReturn);

		// 设置模块工作模式
		SetWorkMode(TH_CMD_SET_WORK_MODE::CMD_WORK_MODE_NORMAL);

		// 设置版本
		if(theSETTING.GetTHMode() == TH_MODE_NORMAL && !theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTHException::MODULE_ID))
		{
			// 记录TH硬件版本
			TH_RSP_GET_VERSION th_rsp_get_version;
			GetVersion(&th_rsp_get_version);

			DWORD dwVersion = ((th_rsp_get_version.cFirmWareVersion[8] - 48) & 0x0F)*1000 + 
				((th_rsp_get_version.cFirmWareVersion[10] - 48) & 0x0F) *100 +
				((th_rsp_get_version.cFirmWareVersion[12] - 48) & 0x0F) *10 +
				((th_rsp_get_version.cFirmWareVersion[13] - 48) & 0x0F);
			// TODO 串口数据格式转换
			theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TH_APP,dwVersion);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
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
bool CTHHelper::Repare()
{
	bool result = false;
	TH_RSP_GET_DEV_STATUS th_rsp_get_dev_status;
	UINT uiRetractNum = 0;
	try {

		// 停止工作
		//SetWorkMode(TH_CMD_SET_WORK_MODE::CMD_WORK_MODE_MAINTENANCE);

		// 断开出票机
		Disconnect();

		// 连接出票机
		Connect();

		GetDevStatus(&th_rsp_get_dev_status);
		// 硬件故障
		if(TIM_FAULT_HARDWARE == th_rsp_get_dev_status.uiErrorType){
			// 复位命令
			Reset(CMD_INIT_FAREBOX, &uiRetractNum);
		}

		UINT ulReturn = 0;
		Init(CMD_INIT_FAREBOX,&ulReturn);

		// 开始工作
		SetWorkMode(CMD_WORK_MODE_NORMAL);

		// 获取状态
		GetDevStatus(&th_rsp_get_dev_status);
		if(TIM_FAULT_HARDWARE == th_rsp_get_dev_status.uiErrorType){
			// 抛出硬件异常
			throw CTHHardwareException(CTHHardwareException::TH_HARDWERE_ERROR, _T(__FILE__), __LINE__);
		}

		// 设置版本
		if(theSETTING.GetTHMode() == TH_MODE_NORMAL && !theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTHException::MODULE_ID))
		{
			// 记录TH硬件版本
			TH_RSP_GET_VERSION th_rsp_get_version;
			GetVersion(&th_rsp_get_version);
			//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TH_APP,thInfo.THVersion);
		}

		result = true;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
	return result;
}

// 测试出票机 
void CTHHelper::Test()
{
	bool result = false;
	UINT uiTransportNum = 0;
	UINT uiRetractNum = 0;
	try {

		// 开始工作
		SetWorkMode(CMD_WORK_MODE_NORMAL);
		// 获取票卡
		CardOut(CMD_CARD_OUT_BOX_A, &uiTransportNum);
		// 送出票卡
		SendCard(&uiTransportNum);
		// 获取票卡
		CardOut(CMD_CARD_OUT_BOX_A, &uiTransportNum);
		// 回收票卡
		RetractCard(&uiRetractNum);

		result = true;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
	//return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       检查票箱到位状态

@param       (o)TH_RSP_GET_DEV_STATUS& response    读取设备状态参数结构
@param       (o)bool& isMagazineAExist             票箱A到位状态
@param       (o)bool& isMagazineBExist             票箱B到位状态
@param       (o)bool& isMagazineCExist             票箱C到位状态

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTHHelper::CheckTicketBoxExist(TH_RSP_GET_DEV_STATUS* response, bool& isMagazineAExist,bool& isMagazineBExist,bool& isMagazineCExist)
{
	long lErrCode = TIM_FAULT_CMD;
	do {
		// 读取设备状态
		lErrCode = GetDevStatus(response);
		if (TIM_FAULT_NORMAL != lErrCode) break;
		isMagazineAExist = false;
		if (response->bBoxAArrive) {
			isMagazineAExist = true;
		}
		isMagazineBExist = false;
		if (response->bBoxBArrive) {
			isMagazineBExist = true;
		}
		isMagazineCExist = false;
		if (response->bBoxCArrive) {
			isMagazineCExist = true;
		}
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
bool CTHHelper::IsExceptionCode(long lErrCode)
{
	bool bRet = false;

	switch(lErrCode) {
	case TIM_FAULT_CMD					 :
		//FAULT_CMD
	case TIM_ERR_CODE_INVALID_CMD        : // invalid command
	case TIM_ERR_CODE_INVALID_PARAM          : // parameter error
	case TIM_ERR_CODE_DEVICE_BUSY    : // the active upload data timeout
	case TIM_ERR_CODE_RECIEVE_TIMEOUT    : // recieve timeout
	case TIM_ERR_CODE_COMCONFIGURE       : // com open error
	case TIM_ERR_CODE_RECIEVE_ERR            : // recieve error                                                         
		bRet = true;
		break;
	default:
		break;
	}
	return bRet;
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
long CTHHelper::CommOpen()
{
	TH_CMD_COMM_OPEN cmd_comm_open;
	cmd_comm_open.uiCommPort = theBOM_INFO.GetTHCommPort();
	cmd_comm_open.uiBaudRate = theBOM_INFO.GetTHBaudRate();
	CTHCommOpen cTHCommOpen(cmd_comm_open);
	long lErrCode = cTHCommOpen.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
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
long CTHHelper::CommClose()
{
	CTHCommClose cTHCommClose;
	long lErrCode = cTHCommClose.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置模块工作模式

@param      (i)TH_CMD_SET_WORK_MODE command      输入参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::SetWorkMode(TH_CMD_SET_WORK_MODE command)
{
	CTHSetWorkMode cTHSetWorkMode(command);
	long lErrCode = cTHSetWorkMode.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取票卡

@param      (i)TH_CMD_CARD_OUT command      输入参数
@param      (o)TH_RSP_CARD_OUT& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::CardOut(TH_CMD_CARD_OUT command, UINT* uiTransportNum)
{
	CTHCardOut cTHCardOut(command);
	long lErrCode = cTHCardOut.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHCardOut.GetTHResponse(uiTransportNum);
	}
	//// 审计数据
	//if(command == CMD_CARD_OUT_BOX_A)
	//	theDeviceRegAR.AddThScratchWheelACount();
	//else
	//	theDeviceRegAR.AddThScratchWheelBCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      送出票卡

@param      (o)TH_RSP_SEND_CARD& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::SendCard(UINT* uiTransportNum)
{
	CTHSendCard cTHSendCard;
	long lErrCode = cTHSendCard.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHSendCard.GetTHResponse(uiTransportNum);
	}
	// 审计数据
	//theDeviceRegAR.AddThSendOutMotorCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      回收票卡

@param      (o)TH_RSP_RETRACT_CARD& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::RetractCard(UINT* uiRetractNum)
{
	CTHRetractCard cTHRetractCard;
	long lErrCode = cTHRetractCard.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHRetractCard.GetTHResponse(uiRetractNum);
	}
	// 审计数据
	//theDeviceRegAR.AddThLinkElecFlipCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取设备状态

@param      (o)TH_RSP_GET_DEV_STATUS& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::GetDevStatus(TH_RSP_GET_DEV_STATUS* response)
{
	CTHGetDevStatus cTHGetDevStatus;
	long lErrCode = cTHGetDevStatus.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHGetDevStatus.GetTHResponse(response);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      模块复位

@param      (i)TH_CMD_INIT_RESET command      输入参数
@param      (o)TH_RSP_RESET& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::Reset(TH_CMD_INIT_RESET command, UINT* uiRetractNum)
{
	CTHReset cTHReset(command);
	long lErrCode = cTHReset.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHReset.GetTHResponse(uiRetractNum);
	}
	//theDeviceRegAR.AddThResetCount();
	// 等待复位硬件动作30秒完成，否则无法通信
	Sleep(33000);
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置票箱数量信息

@param      (i)TH_CMD_SET_COUNT_INFO inBoxID      输入参数
@param      (i)TH_RSP_SET_COUNT_INFO& inCardNum   输入参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::SetTicketBoxCountInfo(TH_BOX_TYPE inBoxID, tTIMCardNum* inCardNum)
{
	CTHSetTicketBoxCountInfo cTHSetTicketBoxCountInfo(inBoxID,inCardNum);
	long lErrCode = cTHSetTicketBoxCountInfo.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	tTIMDevReturn DevReturn;
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHSetTicketBoxCountInfo.GetTHResponse(&DevReturn);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取票箱数量信息

@param      (i)TH_CMD_GET_COUNT_INFO command      输入参数
@param      (o)TH_RSP_GET_COUNT_INFO& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::GetTicketBoxCountInfo(TH_BOX_TYPE command, tTIMCardNum* response)
{
	CTHGetTicketBoxCountInfo cTHGetTicketBoxCountInfo(command);
	long lErrCode = cTHGetTicketBoxCountInfo.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHGetTicketBoxCountInfo.GetTHResponse(response);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      模块初始化

@param      (i)TH_CMD_INIT command      输入参数
@param      (o)TH_RSP_INIT& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::Init(TH_CMD_INIT_RESET command, UINT* uiRetractNum)
{
	CTHInit cTHInit(command);
	long lErrCode = cTHInit.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHInit.GetTHResponse(uiRetractNum);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取设备驱动版本信息

@param      (o)TH_RSP_GET_VERSION& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::GetVersion(TH_RSP_GET_VERSION* response)
{
	CTHGetVersion cTHGetVersion;
	long lErrCode = cTHGetVersion.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHGetVersion.GetTHResponse(response);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置模块测试

@param      (i)TH_CMD_TEST_SETTING command      输入参数
@param      (o)TH_RSP_TEST_SETTING& response    输出参数

@retval     long    0:成功; 其他:错误代码

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::TestSetting(TH_CMD_TEST_SETTING command)
{
	CTHTestSetting CTHTestSetting(command);
	long lErrCode = CTHTestSetting.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读取RFID

@param      IN TH_CMD_SET_COUNT_INFO boxType(箱子类型)
			OUT tTIMRFIDInfo* pTimRfidInfo(输出RFID信息)

@retval     bool true成功 false失败;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//bool CTHHelper::ReadTicketBoxRFID(IN TH_BOX_TYPE boxType,OUT tTIMRFIDInfo* pTimRfidInfo){
//	//CTHReadRFIDInfo thReadRfid(boxType);
//	//long lRet = thReadRfid.ExecuteCommand();
//
//	//if(IsExceptionCode(lRet)){
//	//	throw CTHHardwareException(lRet,_T(__FILE__),__LINE__);
//	//}
//	//
//	//// 确保反馈结果正确
//	//if(lRet != 0){
//	//	return false;
//	//}
//
//	//thReadRfid.GetTHResponse(pTimRfidInfo);
//
//	// lichao: 临时修改RFID
//	if (boxType==TH_BOX_A)
//	{
//		memcpy(pTimRfidInfo->ucBoxID, "1111", 4);
//	}
//	else if(boxType==TH_BOX_B){
//		memcpy(pTimRfidInfo->ucBoxID, "2222", 4);
//	}
//	
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      写票箱RFID

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//bool CTHHelper::WriteTicketBoxRFID(IN TH_BOX_TYPE boxType,OUT tTIMRFIDInfo* pTimRfidInfo)
//{
//	CTHWriteRFIDInfo writeRfid(boxType,pTimRfidInfo);
//	long lRet = writeRfid.ExecuteCommand();
//	if(IsExceptionCode(lRet)){
//		throw CTHHardwareException(lRet,_T(__FILE__),__LINE__);
//	}	
//	if(lRet != 0){
//		return false;
//	}
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      更换箱子

@param      IN SC_REPLACE_BOX_CMD_INFO& rpsBoxInfo

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTHHelper::ReplaceTicketBox(TH_REPLACE_BOX_CMD_INFO boxAction){
	//CTHReplaceTicketBox replaceBox(rpsBoxInfo.boxId,rpsBoxInfo.actionType);

	// lichao
	//long lRet = replaceBox.ExecuteCommand();
	//if(IsExceptionCode(lRet)){
	//	throw CTHHardwareException(lRet,_T(__FILE__),__LINE__);
	//}

	//// 硬件动作(卸载)
	//if(rpsBoxInfo.actionType == TH_UNINSTALL){
	//	try{
	//		if(rpsBoxInfo.boxId == TH_BOX_A){
	//			TestSetting(CMD_TEST_A_TRAY_DOWN);
	//		}
	//		else if(rpsBoxInfo.boxId == TH_BOX_B){
	//			TestSetting(CMD_TEST_B_TRAY_DOWN);
	//		}
	//	}
	//	catch(...){
	//		return false;
	//	}
	//}

	//// 安装
	//if(rpsBoxInfo.actionType == TH_INSTALL){
	//	try{
	//		if(rpsBoxInfo.boxId == TH_BOX_A){
	//			TestSetting(CMD_TEST_A_TRAY_UP);
	//		}
	//		else if(rpsBoxInfo.boxId == TH_BOX_B){
	//			TestSetting(CMD_TEST_B_TRAY_UP);
	//		}
	//	}
	//	catch(...){
	//		return false;
	//	}
	//}

	//// 审计数据
	//if(rpsBoxInfo.boxId == TH_BOX_A)
	//	theDeviceRegAR.AddThBoxAUpDownCount();
	//else
	//	theDeviceRegAR.AddThBoxBUpDownCount();

	///*if(lRet != 0){
	//return false;
	//}*/

	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发卡模块更新

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
 int CTHHelper::UpdateProgram(){
	int iRet = 0;
	try
	{
		//CString fileName = (CString)STR_TVM_TH_APP;
		//vector<CString> updateFiles;

		//CString sAppPath;
		//GetAppPath(sAppPath);
		//CString updateFileFolder = sAppPath + TVM_TH_UPDATE_DIR;//BOM_TC_UPDATE_DIR;


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
		//	CString upgradeVersionStr = thePARAM_HELPER.GetProgVerFromProgName(updateFile);	// 获取升级的TH版本
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
		//	THUpdateCommand cmdUpdate(theSETTING.GetTHComPort(),theSETTING.GetTHBaudRate(),theSETTING.GetTHUpadateBaudRate(),(char*)strFilePath.c_str());
		//	if (RW_RSP_OK == cmdUpdate.Execute()) {
		//		DeleteFile(updateFile);
		//		//theSETTING.SetRWUpdateFlag(0,true);
		//		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TH_APP,upgradeVersion);//更新本地版本
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
 bool CTHHelper::TryRepareTH(){

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
 bool CTHHelper::GetStaticBoxId(IN TH_BOX_TYPE boxId,OUT tTIMRFIDInfo& rfidInfo){

	 //rfidInfo.ucBoxID[0] = theMAINTENANCE_INFO.GetDeviceType();
	 rfidInfo.ucBoxID[1] = 0x01;
	 rfidInfo.ucBoxID[2] = Dec2Hex(theMAINTENANCE_INFO.GetStationCode());
	 BYTE bNum = theMAINTENANCE_INFO.GetMachineCode();
	 CString strNum(_T(""));

	 switch(boxId){
	 case TH_BOX_A:
		 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-2);
		 rfidInfo.ucBoxID[3] = 2 * bNum - 2/*_tcstoul(strNum,NULL,16)*/;
		 break;
	 case TH_BOX_B:
		 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-1);
		 rfidInfo.ucBoxID[3] = 2 * bNum - 1/*_tcstoul(strNum,NULL,16)*/;
		 break;
	 case TH_BOX_C:
		 rfidInfo.ucBoxID[1] = 0x02;
		 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-2);
		 rfidInfo.ucBoxID[3] = 2 * bNum/*_tcstoul(strNum,NULL,16)*/;
		 break;
	 default:
		 return false;
	 }

	 return true;
 }