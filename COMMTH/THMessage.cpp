#include "StdAfx.h"
#include "THMessage.h"
//#include "BomInfo.h"
#include "THException.h"
#include "THHardwareException.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
typedef enum TH_CMD_SENSOR
{
	SENSOR01_ID = 61000,	// 61000 (0:正常;1:触发) 票卡出口检测传感器
	SENSOR02_ID = 61001,    // 61001 (0:正常;1:触发) 读写区检测传感器
	SENSOR07_ID = 61006,    // 61006 (0:到位;1:移除) 票箱 A 到位检测传感器
	SENSOR08_ID = 61007,    // 61007 (0:打开;1:关闭) 票箱 A 顶盖检测传感器（塑料票箱未使用）
	SENSOR09_ID = 61008,    // 61008 (0:正常;1:将空) 票箱 A 将空传感器   
	SENSOR10_ID = 61009,    // 61009 (0:正常;1:已空) 票箱 A 空传感器
	SENSOR13_ID = 61012,    // 61012 (0:到位;1:移除) 票箱 B 到位检测传感器
	SENSOR14_ID = 61013,    // 61013 (0:打开;1:关闭) 票箱 B 顶盖检测传感器（塑料票箱未使用） 
	SENSOR15_ID = 61014,    // 61014 (0:正常;1:将空) 票箱 B 将空传感器
	SENSOR16_ID = 61015,    // 61015 (0:正常;1:已空) 票箱 B 空传感器
	SENSOR19_ID = 61018,    // 61018 (0:到位;1:移除) 废票箱到位检测传感器
	SENSOR20_ID = 61019,    // 61019 (0:打开;1:关闭) 废票箱顶盖检测传感器（塑料票箱未使用）
	SENSOR21_ID = 61020,    // 61020 (0:正常;1:已满) 废票箱满检测传感器
};

typedef enum _tem_th_rsp_code {
	TH_RSP_OK = 0x0000,    // 操作成功
	TH_RSP_NG = 0x0001,    // 操作失败
};
//////////////////////////////////////////////////////////////////////////
/**
@brief      CTHCommands构造函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHCommands::CTHCommands()
{
	hInst = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CTHCommands析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHCommands::~CTHCommands()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      处理反馈代码

@param      long lReturnCode    反馈代码         

@retval     long    成功:0; 失败:错误应答码         

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommands::DealReturnCode(long lReturnCode, tTIMDevReturn* pDevReturn)
{
	long lRet = TIM_ERR_CODE_NONE;
	if (TIM_ERR_CODE_NONE != lReturnCode) {
		lRet = (long)pDevReturn->uiErrorCode;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommands::ExecuteCommand()
{
	return TH_RSP_OK;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     long     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommands::IsValidCommand()
{
	return SP_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHCommOpen::CTHCommOpen(TH_CMD_COMM_OPEN &command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHCommOpen::~CTHCommOpen()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommOpen::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommOpen::ExecuteCommand()
{
	long lCompleteCode = TIM_CommOpen(m_command.uiCommPort, m_command.uiBaudRate,_T("D:\\LOG\\"),_T("TIM_"));
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_INIT cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHInit::CTHInit(TH_CMD_INIT_RESET command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHInit::~CTHInit(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHInit::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHInit::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_Init(m_command, &m_uiRetractNum,&tModuleStatus, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_INIT* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHInit::GetTHResponse(UINT* uiRetractNum)
{
	try {
		if(uiRetractNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiRetractNum = m_uiRetractNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_RESET cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHReset::CTHReset( TH_CMD_INIT_RESET command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHReset::~CTHReset(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHReset::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHReset::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_Reset(m_command, &m_uiRetractNum, &tModuleStatus, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_RESET* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHReset::GetTHResponse(UINT* uiRetractNum)
{
	try {
		if(uiRetractNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiRetractNum = m_uiRetractNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHGetVersion::CTHGetVersion(void) : CTHCommands()
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
CTHGetVersion::~CTHGetVersion(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetVersion::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetVersion::ExecuteCommand()
{
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_GetVersion(m_response.cFirmWareVersion, m_response.cDriverVersion,&tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_GET_VERSION* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetVersion::GetTHResponse(TH_RSP_GET_VERSION* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response->cDriverVersion, m_response.cDriverVersion, sizeof(m_response.cDriverVersion));
			memcpy(response->cFirmWareVersion, m_response.cFirmWareVersion, sizeof(m_response.cFirmWareVersion));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHGetDevStatus::CTHGetDevStatus(void) : CTHCommands()
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
CTHGetDevStatus::~CTHGetDevStatus(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetDevStatus::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetDevStatus::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	//TH_RSP_GET_DEV_STATUS tDevStatus;

	long lCompleteCode = TIM_GetDevStatus(&tModuleStatus, &tDevReturn);
	if (TH_RSP_OK == lCompleteCode) {

		// 返回模块的状态信息
		m_response.bCardInReadArea       = tModuleStatus.bCardInReadArea;
		// 票箱状态
		m_response.cCassetteAStatus      = tModuleStatus.cCassetteAStatus;
		m_response.cCassetteBStatus      = tModuleStatus.cCassetteBStatus;
		m_response.cRejectCassetteStatus = tModuleStatus.cRejectCassetteStatus;
		// 票箱票卡数量
		m_response.uiCassetteACardNum    = tModuleStatus.tCardNum.uiCassetteACardNum;
		m_response.uiCassetteBCardNum    = tModuleStatus.tCardNum.uiCassetteBCardNum;
		m_response.uiRejectCassetteNum   = tModuleStatus.tCardNum.uiRejectCassetteNum;
		// 返回状态信息
		m_response.uiType                = tDevReturn.uiType;
		m_response.uiErrorType           = tDevReturn.uiErrorType;
		m_response.uiErrorCode           = tDevReturn.uiErrorCode;

		// 传感器状态(0:正常  1:故障  3:未遮挡  4:遮挡  0xff:不存在该传感器)
		int nCount = (int)tModuleStatus.uiSensorCount;
		for (int i = 0; i < nCount; i++) {
			UINT uiSensorID = tModuleStatus.tSensorType[i].uiSensotID;
			BYTE btSensorStatus = tModuleStatus.tSensorType[i].btSensorStatus;
			switch (uiSensorID)
			{
				//case SENSOR02_ID:m_response.cRwAreaDetect = btSensorStatus;       break;						// 61001 (0:正常;1:触发) 读写区检测传感器
				//case SENSOR07_ID:m_response.bBoxAArrive = (btSensorStatus == 0x04 ? true: false);break;			// 61012 (0:到位;1:移除) 票箱 B 到位检测传感器
				//case SENSOR08_ID:m_response.bBoxACoverArrive = (btSensorStatus == 0x03 ? true : false); break;	// 61013 (0:打开;1:关闭) 票箱 B 顶盖检测传感器（塑料票箱未使用）
				//case SENSOR09_ID:m_response.bBoxANearlyEmpty = btSensorStatus;    break;						// 61014 (0:正常;1:将空) 票箱 B 将空传感器
				//case SENSOR10_ID:m_response.bBoxAEmpty =  (btSensorStatus == 0x04 ? true : false);break;		// 61015 (0:正常;1:已空) 票箱 B 空传感器
				//case SENSOR13_ID:m_response.bBoxBArrive = (btSensorStatus == 0x04 ? true : false);break;		// 61006 (0:到位;1:移除) 票箱 A 到位检测传感器
				//case SENSOR14_ID:m_response.bBoxBCoverArrive = (btSensorStatus == 0x03 ? true : false); break;	// 61007 (0:打开;1:关闭) 票箱 A 顶盖检测传感器（塑料票箱未使用）
				//case SENSOR15_ID:m_response.bBoxBNearlyEmpty = btSensorStatus;    break;						// 61008 (0:正常;1:将空) 票箱 A 将空传感器
				//case SENSOR16_ID:m_response.bBoxBEmpty =  (btSensorStatus == 0x04 ? true : false);	break;		// 61009 (0:正常;1:已空) 票箱 A 空传感器

				//case SENSOR19_ID:m_response.bBoxCArrive = (btSensorStatus == 0x04 ? true : false);break;		// 61018 (0:到位;1:移除) 废票箱到位检测传感器
				//case SENSOR20_ID:m_response.bBoxCCoverArrive = true/*(btSensorStatus == 0x03 ? true : false)*/;break;	// 61019 (0:打开;1:关闭) 废票箱顶盖检测传感器（塑料票箱未使用）没有顶盖
				//case SENSOR21_ID:m_response.bBoxCFull =  (btSensorStatus == 0x04 ? true : false);break;			// 61020 (0:正常;1:已满) 废票箱满检测传感器

			case CODE_SENSOR53_ID:m_response.cRwAreaDetect = (btSensorStatus==0?true:false);break;			// 41006 (0:正常;1:触发) 读写区检测传感器
			case CODE_SENSOR56_ID:m_response.bBoxAArrive = (btSensorStatus == 0?true:false);break;			// 41009 (0:到位;1:移除) 票箱 A 到位检测传感器
			case CODE_SENSOR74_ID:m_response.bBoxACoverArrive = (btSensorStatus==1?true:false);break;		// 41027 (0:打开;1:关闭) 票箱 A 顶盖检测传感器（塑料票箱未使用）
			case CODE_SENSOR55_ID:m_response.bBoxANearlyEmpty = (btSensorStatus==1?true:false);break;		// 41008 (0:正常;1:将空) 票箱 A 将空传感器
			case CODE_SENSOR57_ID:m_response.bBoxAEmpty =(btSensorStatus==1?true:false);break;				// 41010 (0:正常;1:已空) 票箱 A 空传感器
			case CODE_SENSOR62_ID:m_response.bBoxBArrive = (btSensorStatus==0?true:false);break;			// 41015 (0:到位;1:移除) 票箱 B 到位检测传感器
			case CODE_SENSOR75_ID:m_response.bBoxBCoverArrive = (btSensorStatus==1?true:false);break;		// 41028 (0:打开;1:关闭) 票箱 B 顶盖检测传感器（塑料票箱未使用）
			case CODE_SENSOR61_ID:m_response.bBoxBNearlyEmpty = (btSensorStatus==1?true:false);break;		// 41014 (0:正常;1:将空) 票箱 B 将空传感器
			case CODE_SENSOR63_ID:m_response.bBoxBEmpty =  (btSensorStatus==1?true:false);break;			// 41016 (0:正常;1:已空) 票箱 B 空传感器

			case CODE_SENSOR71_ID:m_response.bBoxCArrive = (btSensorStatus == 0?true:false);break;		// 41024 (0:到位;1:移除) 废票箱到位检测传感器
			case CODE_SENSOR76_ID:m_response.bBoxCCoverArrive = true/*(btSensorStatus==0?true:false)*/;break;	// 41029 (0:打开;1:关闭) 废票箱顶盖检测传感器（塑料票箱未使用）
			case CODE_SENSOR72_ID:m_response.bBoxCFull = (btSensorStatus==1?true:false);break;			// 41025 (0:正常;1:已满) 废票箱满检测传感器
				default:break;
			}
		}
	}
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_GET_DEV_STATUS* response    

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetDevStatus::GetTHResponse(TH_RSP_GET_DEV_STATUS* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			// 传感器状态(0:正常  1:故障  3:未遮挡  4:遮挡  0xff:不存在该传感器)
			response->bCardInReadArea       = m_response.bCardInReadArea      ;
			response->cCassetteAStatus      = m_response.cCassetteAStatus     ;
			response->cCassetteBStatus      = m_response.cCassetteBStatus     ;
			response->cRejectCassetteStatus = m_response.cRejectCassetteStatus;
			response->uiCassetteACardNum    = m_response.uiCassetteACardNum   ;
			response->uiCassetteBCardNum    = m_response.uiCassetteBCardNum   ;
			response->uiRejectCassetteNum   = m_response.uiRejectCassetteNum  ;
			response->cRwAreaDetect         = m_response.cRwAreaDetect        ;
			response->bBoxAArrive           = m_response.bBoxAArrive          ;
			response->bBoxACoverArrive      = m_response.bBoxACoverArrive     ;
			response->bBoxANearlyEmpty      = m_response.bBoxANearlyEmpty     ;
			response->bBoxAEmpty            = m_response.bBoxAEmpty           ;
			response->bBoxBArrive           = m_response.bBoxBArrive          ;
			response->bBoxBCoverArrive      = m_response.bBoxBCoverArrive     ;
			response->bBoxBNearlyEmpty      = m_response.bBoxBNearlyEmpty     ;
			response->bBoxBEmpty            = m_response.bBoxBEmpty           ;
			response->bBoxCArrive           = m_response.bBoxCArrive          ;
			response->bBoxCCoverArrive      = m_response.bBoxCCoverArrive     ;
			response->bBoxCFull             = m_response.bBoxCFull            ;
			response->uiType                = m_response.uiType               ;
			response->uiErrorType           = m_response.uiErrorType          ;
			response->uiErrorCode           = m_response.uiErrorCode          ;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_CARD_OUT cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHCardOut::CTHCardOut(TH_CMD_CARD_OUT command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHCardOut::~CTHCardOut(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHCardOut::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHCardOut::ExecuteCommand()
{
	long lCompleteCode = TIM_CardOut((char)m_command, &m_tModuleStatus, &m_uiTransportNum,&m_tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_CARD_OUT* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHCardOut::GetTHResponse(UINT* uiTransportNum)
{
	try {
		if(uiTransportNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiTransportNum = m_uiTransportNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHSendCard::CTHSendCard(void) : CTHCommands()
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
CTHSendCard::~CTHSendCard(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHSendCard::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHSendCard::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_SendCard(&tModuleStatus, &m_uiTransportNum, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_SEND_CARD* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHSendCard::GetTHResponse(UINT* uiTransportNum)
{
	try {
		if(uiTransportNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiTransportNum = m_uiTransportNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHRetractCard::CTHRetractCard(void) : CTHCommands()
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
CTHRetractCard::~CTHRetractCard(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHRetractCard::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHRetractCard::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_RetractCard(&tModuleStatus, &m_uiRetractNum, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_RETRACT_CARD* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHRetractCard::GetTHResponse(UINT* uiRetractNum)
{
	try {
		if(uiRetractNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiRetractNum = m_uiRetractNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHGetAuditData::CTHGetAuditData(void) : CTHCommands()
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
CTHGetAuditData::~CTHGetAuditData(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetAuditData::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetAuditData::ExecuteCommand()
{
	long lCompleteCode = TIM_GetAuditData(&m_tAuditCardNum, &m_tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_GET_AUDIT_DATA* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetAuditData::GetTHResponse(tTIMAuditCardNum* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response,&m_tAuditCardNum,sizeof(tTIMAuditCardNum));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_GET_COUNT_INFO cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHGetTicketBoxCountInfo::CTHGetTicketBoxCountInfo(TH_BOX_TYPE command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHGetTicketBoxCountInfo::~CTHGetTicketBoxCountInfo(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetTicketBoxCountInfo::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetTicketBoxCountInfo::ExecuteCommand()
{
	//long lCompleteCode = GetTicketBoxCountInfo(m_command, &m_response);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_GET_COUNT_INFO* response    

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetTicketBoxCountInfo::GetTHResponse(tTIMCardNum* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			response->uiCassetteACardNum = m_response.uiCassetteACardNum;
			response->uiCassetteBCardNum = m_response.uiCassetteBCardNum;
			response->uiRejectCassetteNum = m_response.uiRejectCassetteNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_SET_COUNT_INFO cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHSetTicketBoxCountInfo::CTHSetTicketBoxCountInfo(TH_BOX_TYPE iBoxID,tTIMCardNum* iCardNum) : CTHCommands()
{
	m_boxID = iBoxID;
	m_cardNum.uiCassetteACardNum = iCardNum->uiCassetteACardNum;
	m_cardNum.uiCassetteBCardNum = iCardNum->uiCassetteBCardNum;
	m_cardNum.uiRejectCassetteNum= iCardNum->uiRejectCassetteNum;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHSetTicketBoxCountInfo::~CTHSetTicketBoxCountInfo(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHSetTicketBoxCountInfo::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

long CTHSetTicketBoxCountInfo::ExecuteCommand()
{
	long lCompleteCode = TIM_SetTicketBoxCountInfo(m_boxID, &m_cardNum,&m_DevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_SET_COUNT_INFO* response  

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHSetTicketBoxCountInfo::GetTHResponse(tTIMDevReturn* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			response = &m_DevReturn;
			/*response->uiCassetteACardNum = m_response.uiCassetteACardNum;
			response->uiCassetteBCardNum = m_response.uiCassetteBCardNum;
			response->uiRejectCassetteNum = m_response.uiRejectCassetteNum;*/
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHGetSN::CTHGetSN(void) : CTHCommands()
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
CTHGetSN::~CTHGetSN(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetSN::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetSN::ExecuteCommand()
{
	long lCompleteCode = TIM_GetSN(m_response.cModuleSN, &m_DevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TH_RSP_GET_SN* response 

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetSN::GetTHResponse(TH_RSP_GET_SN* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response->cModuleSN, m_response.cModuleSN, sizeof(response->cModuleSN));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHCommClose::CTHCommClose() : CTHCommands()
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
CTHCommClose::~CTHCommClose()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommClose::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommClose::ExecuteCommand()
{
	long lCompleteCode = TIM_CommClose();
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_SET_WORK_MODE cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHSetWorkMode::CTHSetWorkMode(TH_CMD_SET_WORK_MODE command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHSetWorkMode::~CTHSetWorkMode(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHSetWorkMode::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHSetWorkMode::ExecuteCommand()
{
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_SetWorkMode((char)m_command, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_TEST_SETTING cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSetting::CTHTestSetting(TH_CMD_TEST_SETTING command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSetting::~CTHTestSetting(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHTestSetting::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHTestSetting::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_TestSetting((char)m_command, &tModuleStatus, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHReadRFIDInfo::CTHReadRFIDInfo(BYTE bTicketBoxNo){
	m_bTicketBoxNo = bTicketBoxNo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHReadRFIDInfo::~CTHReadRFIDInfo(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读取票箱RFID执行命令

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHReadRFIDInfo::ExecuteCommand(){

	long lRet = TIM_GetTicketBoxRFIDInfo((char)m_bTicketBoxNo,&m_RfidInfo,&m_TimDevReturn);
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取读取RFID的反馈

@param      tTIMRFIDInfo* rspRfid

@retval     long lRet;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHReadRFIDInfo::GetTHResponse(tTIMRFIDInfo* rspRfid){
	try{
		if(rspRfid == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(rspRfid,&m_RfidInfo,sizeof(tTIMRFIDInfo));
		}
			
		return 0;
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      写RFID构造函数

@param      BYTE bTicketBoxNo 箱子类型
			tTIMRFIDInfo& pTimRfidInfo RFID信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHWriteRFIDInfo::CTHWriteRFIDInfo(BYTE bTicketBoxNo,tTIMRFIDInfo* pTimRfidInfo){
	m_bTicketBoxNo = bTicketBoxNo;
	m_pRfidInfo = pTimRfidInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHWriteRFIDInfo::~CTHWriteRFIDInfo(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      写RFID执行命令函数

@param      无

@retval     long 反馈结果

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHWriteRFIDInfo::ExecuteCommand(){
	long lRet = ThWriteRfidInfo();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      写RFID执行函数

@param      无

@retval     long 反馈结果

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHWriteRFIDInfo::ThWriteRfidInfo(){
	return TIM_SetTicketBoxRFIDInfo((char)m_bTicketBoxNo,m_pRfidInfo,&m_TimDevReturn);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		票箱更换构造函数

@param      BYTE bTicketBoxNo 更换票箱编号
			BYTE bBoxMode	  更换票箱类型（卸下/安装）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHReplaceTicketBox::CTHReplaceTicketBox(BYTE bTicketBoxNo,BYTE bBoxMode){
	m_bTicketBoxNo = bTicketBoxNo;
	m_bBoxMode = bBoxMode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      票箱更换析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHReplaceTicketBox::~CTHReplaceTicketBox(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      票箱更换执行命令

@param      无

@retval     long 执行结果

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHReplaceTicketBox::ExecuteCommand(){
	long lRet = ReplaceTicketBox();

	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      票箱更换执行函数

@param      无

@retval     long 执行结果

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTHReplaceTicketBox::ReplaceTicketBox(){
	return TIM_ReplaceTBox((char)m_bTicketBoxNo,m_bBoxMode,&m_TimDevReturn);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH更新构造函数

@param      WORD port,WORD speed, char* pFilePath

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
THUpdateCommand::THUpdateCommand(DWORD port,DWORD speed,DWORD updateSpeed, char* pFilePath){
	m_dPort = port;
	m_dSpeed = speed;
	m_sFilePath = pFilePath;
	m_dUpdateSpeed = updateSpeed;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH更新析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
THUpdateCommand::~THUpdateCommand(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH更新执行函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long THUpdateCommand::ExecuteCommand(){
	long lRet = THDownload(m_dPort,m_dSpeed,m_dUpdateSpeed,m_sFilePath);
	this->OnComplete(lRet);
	return lRet;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      TH更新构造函数

@param      WORD port,WORD speed, char* pFilePath

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
THTrayContral::THTrayContral(int nTrayType, int nTrayCmd){
	m_nTrayType = nTrayType;
	m_nTrayCmd = nTrayCmd;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH更新析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
THTrayContral::~THTrayContral(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH更新执行函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long THTrayContral::ExecuteCommand(){
	//tTIMDevReturn ret;
	//long lRet = TIM_TrayContral(m_nTrayType,m_nTrayCmd,&ret);
	//this->OnComplete(lRet);
	//return lRet;
	return 0;
}