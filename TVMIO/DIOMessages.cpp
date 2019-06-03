#include "StdAfx.h"
#include "DIOMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDIOCommOpen::CDIOCommOpen(int nNum, tSerialConfigInfo &command) : CCommand()
{
	m_command = command;
	m_Num = nNum;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDIOCommOpen::~CDIOCommOpen()
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
long CDIOCommOpen::IsValidCommand()
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
long CDIOCommOpen::ExecuteCommand(void)
{
	long lCompleteCode = DIO_Open(m_Num,&m_command);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDIOCommClose::CDIOCommClose() : CCommand()
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
CDIOCommClose::~CDIOCommClose()
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
long CDIOCommClose::IsValidCommand()
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
long CDIOCommClose::ExecuteCommand()
{
	long lCompleteCode = DIO_Close();
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDIOCommInit::CDIOCommInit() : CCommand()
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
CDIOCommInit::~CDIOCommInit()
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
long CDIOCommInit::IsValidCommand()
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
long CDIOCommInit::ExecuteCommand()
{
	long lCompleteCode = DIO_Init(m_DioReturn);
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
void CDIOCommInit::GetIOResponse(STDioDevReturn* response)
{
	try {
		if(response == NULL){
			throw CIOException(CIOException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response, &m_DioReturn, sizeof(m_DioReturn));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CIOException(CIOException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
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
CDIOSetLampSignal::CDIOSetLampSignal(IO_SIGNAL_TYPE usLampType, DIO_LAMP_COLOR ucLampColor,DIO_OPER_TYPE ucOperType, unsigned char ucBlinkTimes) : CCommand()
{
	m_DioLampConfig.usLampType = usLampType;
	m_DioLampConfig.ucLampColor = ucLampColor;
	m_DioLampConfig.ucOperType = ucOperType;
	m_DioLampConfig.ucBlinkTimes = ucBlinkTimes;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDIOSetLampSignal::~CDIOSetLampSignal()
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
long CDIOSetLampSignal::IsValidCommand()
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
long CDIOSetLampSignal::ExecuteCommand()
{
	unsigned char ucLampCnt = 1;
	long lCompleteCode = DIO_SetLampSignal(&m_DioLampConfig,ucLampCnt,m_DioReturn);
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
void CDIOSetLampSignal::GetIOResponse(STDioDevReturn* response)
{
	try {
		if(response == NULL){
			throw CIOException(CIOException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response, &m_DioReturn, sizeof(m_DioReturn));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CIOException(CIOException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
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
CDIOSetSwitchSignal::CDIOSetSwitchSignal(IO_SIGNAL_TYPE usSignalType,DIO_OPER_TYPE ucOperType,UCHAR ucSignalCnt) : CCommand()
{
	m_DioSignalConfig.usSignalType = usSignalType;
	m_DioSignalConfig.ucOperType = ucOperType;
	m_ucSignalCnt = ucSignalCnt;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDIOSetSwitchSignal::~CDIOSetSwitchSignal()
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
long CDIOSetSwitchSignal::IsValidCommand()
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
long CDIOSetSwitchSignal::ExecuteCommand()
{
	long lCompleteCode = DIO_SetSwitchSignal(&m_DioSignalConfig,m_ucSignalCnt,m_DioReturn);
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
void CDIOSetSwitchSignal::GetIOResponse(STDioDevReturn* response)
{
	try {
		if(response == NULL){
			throw CIOException(CIOException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response, &m_DioReturn, sizeof(m_DioReturn));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CIOException(CIOException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
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
CDIOCommGetStatus::CDIOCommGetStatus() : CCommand()
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
CDIOCommGetStatus::~CDIOCommGetStatus()
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
long CDIOCommGetStatus::IsValidCommand()
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
long CDIOCommGetStatus::ExecuteCommand()
{
	unsigned char ucLampCnt = 0;
	STDioSignalConfig DioSignalConfig[50] = {0};
	memset(&DioSignalConfig,0x00,sizeof(STDioSignalConfig)* 50);
	STDioDevReturn stDevRes= {0};
	long lCompleteCode = DIO_GetSwitchSignal(DioSignalConfig,ucLampCnt,stDevRes);
	if (IO_RSP_OK == lCompleteCode){
		for(int n = 0; n < ucLampCnt; n++)
		{
			switch (DioSignalConfig[n].usSignalType)
			{
			case IO_SIGNAL_HUMAN_SENSOR1:
				m_status.isLeftPersonActive = (bool)DioSignalConfig[n].ucOperType;
				break;
			case IO_SIGNAL_HUMAN_SENSOR2:
				m_status.isRightPersonActive = (bool)DioSignalConfig[n].ucOperType;
				break;
			case IO_SIGNAL_RECHARGE_INPLACE:
				m_status.isRechargeCardReady = ((bool)DioSignalConfig[n].ucOperType == 0?true:false);
				break;
			case IO_SIGNAL_HELPBUTTON:
				m_status.isAssittentButtonPressed = (bool)DioSignalConfig[n].ucOperType;
				break;
			case IO_SIGNAL_BH_RECEIVING_INPLACE:
				m_status.isBanknoteModeReady = (bool)DioSignalConfig[n].ucOperType;
				break;
			case IO_SIGNAL_CH_INPLACE:
				m_status.isCoinModeReady = (bool)DioSignalConfig[n].ucOperType;
				break;
			case IO_SIGNAL_TH_INPLACE:
				m_status.isCardModeReady = (bool)DioSignalConfig[n].ucOperType;
				break;
			case IO_SIGNAL_PASSENGER_PRINT_INLPACE:
				m_status.isPrintModeReady = (bool)DioSignalConfig[n].ucOperType;
				break;
			case IO_SIGNAL_MAINTENANCE_DOOR1:
				m_status.isFrontDoorOpen = (bool)DioSignalConfig[n].ucOperType;
				break;
			case IO_SIGNAL_MAINTENANCE_DOOR2:
			case IO_SIGNAL_MAINTENANCE_DOOR3:
				{
					m_status.isBackDoorOpen = (bool)DioSignalConfig[n].ucOperType;
					//m_status.isBackDoorOpen = false;//方便调试wangfeng
				}
				break;
			case IO_SIGNAL_COIN_COLECT_BOX_READY:
				m_status.isCoinboxReady = (bool)DioSignalConfig[n].ucOperType;
			default:
				break;
			}
		}
	}
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
void CDIOCommGetStatus::GetIOResponse(IO_RSP_STATUS_INFO* response)
{
	try {
		if(response == NULL){
			throw CIOException(CIOException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response, &m_status, sizeof(m_status));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CIOException(CIOException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}
CDIOGetVersion::CDIOGetVersion(void):CCommand()
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
CDIOGetVersion::~CDIOGetVersion(void)
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
long CDIOGetVersion::IsValidCommand()
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
long CDIOGetVersion::ExecuteCommand()
{
	STDioDevReturn tDevReturn;
	long lCompleteCode = DIO_GetVersion(m_DioVerInfo ,m_DioReturn);
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
void CDIOGetVersion::GetDIOResponse(STDioVerInfo* response)
{
	try {
		if(response == NULL){
			throw CIOException(CIOException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response, &m_DioVerInfo, sizeof(STDioVerInfo));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CIOException(CIOException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}
