#include "StdAfx.h"
#include "BusinessTranslateSvc.h"
#include "CAfcDeviceCommonParam.h"
#include "headermanager.h"
#include "ctmessageaudit.h"
#include "scdatamsg.h"
#include "serverclientsvc.h"
#include "scmessagemakehelper.h"

#include "ServerClientSvc.h"
#include "tvmdef.h"
#include "CTLOGCountInfo.h"
#include "ACCAuditManager.h"
#include "scsvcresult.h"
#include "Sync.h"
#include <afxpriv.h>

/**
@brief 定期集计Service
*/

BEGIN_MESSAGE_MAP(CBusinessTranslateSvc, CBackService)
	ON_SERVICE_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBusinessTranslateSvc::CBusinessTranslateSvc()
:CBackService(BUSINESS_INTERVAL_SVC,ROOT_SVC)
{
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBusinessTranslateSvc::~CBusinessTranslateSvc(void)
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      启动业务计时器

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::StartBusinessIntervalTimer()
{
	// 大于24小时，不处理
	CAfcDeviceCommonParam::COMMUNICATION Communication;
	theAFC_COMMON.GetCommunication(Communication);
	DWORD registerTime = Communication.businessInterval*60;
	theDEBUG_LOG->WriteData(_T("CBusinessSvc"), _T("StartBusinessIntervalTimer"), __LINE__, _T("CBusinessSvc = 0x%x"),this);
	theDEBUG_LOG->WriteData(_T("CBusinessSvc"), _T("StartBusinessIntervalTimer"), __LINE__, _T("TxnIntervalTime = %d"),registerTime);
	if (registerTime == 0 || registerTime > 24*3600) {
		StartTimer(TIMER_REGISTER, 900);
	}
	else {
		StartTimer(TIMER_REGISTER, registerTime);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      关闭业务计时器

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::EndBusinessIntervalTimer()
{
	StopTimer(TIMER_REGISTER);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      定时器响应处理

@param      (i)UINT nTimerID  identifier of a timer

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::OnTimer(UINT nTimerID)
{
	if(nTimerID == TIMER_PACKAGE)
	{
		//组包
		theBUS_TXN_MGR.MakePekg(false);
	}
	if(nTimerID == TIMER_REGISTER)
	{
		EndBusinessIntervalTimer();
		try {
			theDEBUG_LOG->WriteData(_T("CBusinessSvc"), _T("OnTimer"), __LINE__, _T("UINT nTimerID = %x <-"),nTimerID);
			// 保存设备寄存器数据
			//InsertMsgDeviceRegisterData(AR_TIME_ON);
			SendBusinessData();
		}
		catch (CSysException& e) {
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch (...) {
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		}
		StartBusinessIntervalTimer();
	}
}

void CBusinessTranslateSvc::OnStart()
{
	__super::OnStart();
	StartTimer(TIMER_PACKAGE,theTVM_INFO.GetBusinessPackageInterval());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务停止时响应函数

@param      无

@retval     bool 是否允许停止。

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CBusinessTranslateSvc::OnStop()
{
	StopTimer(TIMER_PACKAGE);
	StopTimer(TIMER_REGISTER);
	return __super::OnStop();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送业务数据
@param      无
@retval     int    计数
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBusinessTranslateSvc::SendBusinessData()
{

	theBUS_TXN_MGR.MakePekg();
	if(!theAPP_SESSION.IsSCConnected())
	{
		return SCSVC_RESULT_NOT_CONNECTED;
	}
	SYNC(SENDDATA,_T("BUSINESS"));
	// 有未发送
	while(theBUS_TXN_MGR.HasUnsendData() > 0)
	{
		// 获取队列中的数据包
		PEKG_DATA dataPekg;		
		bool aquirePekgSuccess = theBUS_TXN_MGR.GetDataPekg(dataPekg);	
		if(!aquirePekgSuccess)
		{
			continue;
		}
		CMD_HEADER header = CHeaderManager::AquireHeader(OPER_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pBusDataMsg(new CSCDataMsg);
		pBusDataMsg->SetHeader(&header);
		pBusDataMsg->SetContent(dataPekg.lpData,dataPekg.nLen);
		long executeResult = theSERVICE_MGR.GetService(SC_SVC)->SendMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)pBusDataMsg.get());
		if (executeResult == SP_SUCCESS)
		{
			// 若发送成功设置数据包为已送
			theBUS_TXN_MGR.SetPekgSended(dataPekg.strPekgID);
			CTMessageAudit::PEKG_INFO PekgInfo;
			memcpy(PekgInfo.sPekgId, dataPekg.lpData+1, 14);
			memcpy(&PekgInfo.iRecordCnt,dataPekg.lpData+19, 2);				
			thePEKG_AUDIT.AddWriteFile(PekgInfo);
		}
		else
		{
			return executeResult;
		}
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    参数和程序更新
@param      
@retval     
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgUpdateParameterAndProgram(CParamHelper::VEC_VERSION_INFO& Vec_VersionInfo)   
{
	try
	{
		int iCount = Vec_VersionInfo.size();
		long lMsgLen = LEN_BUS_PARAMETER_SOFT_UPDATE_BASE + iCount*23;
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_SOFTANDPROGRAM_UPDATE,lMsgLen);
		LPBYTE lpMsg =  new BYTE[lMsgLen];
		theSCMessageMakeHelper.MakeUpdateParameterAndProgram(operComHdr, Vec_VersionInfo, lpMsg);
		theBUS_TXN_MGR.SaveTxnData(lMsgLen, lpMsg);
		delete[] lpMsg;
		lpMsg = NULL;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    参数和程序下载业务数据
@param    list<_parameter_download> list_parameter  下载参数和文件列表
LPBYTE					lpByte           
@retval     
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgDownloadParameterAndProgram(vector<ParameterAndSoftwareUpdate>& downloads)     
{
	try
	{
		int iCount = downloads.size();
		long lMsgLen = LEN_BUS_PARAMETER_SOFT_DOWNLOAD_BASE + 51*iCount;
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_SOFTANDPROGRAM_DOWNLOAD,lMsgLen);
		LPBYTE lpMsg = new BYTE[lMsgLen];
		theSCMessageMakeHelper.MakeDownloadParameterAndProgram(operComHdr, downloads,lpMsg);
		theBUS_TXN_MGR.SaveTxnData(lMsgLen, lpMsg);
		delete[] lpMsg;
		lpMsg = NULL;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    设备重要状态改变
@param    SERVICE_STATUS	status   服务状态
@retval     
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgImportantStatusChange(SERVICE_STATUS status)  
{
	try
	{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_DEVICE_IMPORTANT_STATUS_CHANGE,LEN_BUS_DEVICE_IMPORTANT_STATUS_CHANG);
		BYTE lpMsg[LEN_BUS_DEVICE_IMPORTANT_STATUS_CHANG]={0};
		theSCMessageMakeHelper.MakeImportantStatusChange(operComHdr,lpMsg,status);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_DEVICE_IMPORTANT_STATUS_CHANG, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief   ISAM签到/签退
@param   ISAM_AUTH  isam_auth  签到方式
BYTE       result     结果
@retval  无
*/
//////////////////////////////////////////////////////////////////////////
//void CBusinessTranslateSvc::InsertMsgIsamCheckIn(ISAM_AUTH isam_auth, BYTE result)		 
//{
//	try
//	{
//		OperComHdr_t operComHdr=theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_ISAM_LOGIN_LOGOUT,MSG_BUSSINESS_ISAM_LOGIN_LOGOUT_LEN);
//		BYTE lpMsg[MSG_BUSSINESS_ISAM_LOGIN_LOGOUT_LEN]={0};
//		//theSCMessageMakeHelper.MakeIsamCheckIn(operComHdr,lpMsg,isam_auth, result);
//		theBUS_TXN_MGR.SaveTxnData(MSG_BUSSINESS_ISAM_LOGIN_LOGOUT_LEN, lpMsg);
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief    操作日志
@param    OPERATOR_LOG  操作日志内容
@retval   无
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgOperationLog(OPERATOR_LOG operLog)					 
{
	try{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_OPERATING_LOG,LEN_BUS_OPERATING_LOG);
		BYTE lpMsg[LEN_BUS_OPERATING_LOG]={0};
		theSCMessageMakeHelper.MakeOperationLog(operComHdr, operLog, lpMsg);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_OPERATING_LOG, lpMsg);
		Sleep(50);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       生成操作日志数据

@param    (i)OPERATION_CODE operCode   操作类型代码
@param    (i)bool operationResult                操作结果 
@param    (i)CString strOperationContent    操作内容
@param    (i)CString strOperationComment 备注

@retval   无
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgOperationLog(OPERATION_CODE operCode,bool operResult,CString strOperComment)					 
{
	try{
		OPERATOR_LOG operatorLog;			// 操作日志信息结构体

		// 操作代码
		operatorLog.operCode = operCode;

		// 操作结果 
		operatorLog.result = (operResult == true)?0x01:0x00;

		// 操作员ID
		operatorLog.csUserID=theAPP_SESSION.GetUserInfo().sUserID;

		// 获取操作级别
		CString level = _T("");
		level =  theFunction_INFO.GetOperationLogLevelContent(operCode,3);			
		operatorLog.operLevel = (OPERATION_LEVEL)(_ttoi(level));
	
		// 获取内容
		CString content = theFunction_INFO.GetOperationLogLevelContent(operCode,2);
		USES_CONVERSION;
		char *pContent = T2A(content);
		strncpy((char*)operatorLog.operDetail,pContent,32);
		
		// 备注
		char *pOperComment = T2A(strOperComment);
		strncpy((char*)operatorLog.operDetail,pContent,32);
		
		InsertMsgOperationLog(operatorLog);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    设备故障数据

@param    (i)DEVICE_ERRORDATA& DeviceErrData

@retval   none
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgDeviceErrorData(DEVICE_ERRORDATA& DeviceErrData)					 
{
	try{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_DEVICE_ERROR,LEN_BUS_DEVICE_ERROR);
		BYTE lpMsg[LEN_BUS_DEVICE_ERROR] ={0};
		theSCMessageMakeHelper.MakeDeviceErrorData(operComHdr, DeviceErrData, lpMsg);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_DEVICE_ERROR, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     设备寄存器数据	
@param     PURCHASE_ACC_DATA PurchaseAccData
AR_RESON arReson
@retval     
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgDeviceRegisterData(AR_REASON arReson)                                    
{
	try{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_DEVICE_REGISTER,LEN_BUS_DEVICE_REGISTER);
		BYTE lpMsg[LEN_BUS_DEVICE_REGISTER]={0};
		DWORD dataLen = 0;
		theSCMessageMakeHelper.MakeDeviceRegisterData(operComHdr, theDeviceRegAR.GetDeviceRegAr()/*theACCAUDIT_MGR.GetCurrentAR()*/, arReson, lpMsg,dataLen);
		// 每次上传后需要清空设备寄存器数据
		theDeviceRegAR.ClearDeviceAR();

		theBUS_TXN_MGR.SaveTxnData(dataLen, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     票箱更换  	
@param     MAGAZINE_CHANGE magazine_change 票箱更换信息
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgMagazineExchange(MAGAZINE_CHANGE& magazine_change)                                  
{
	try{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_TICKETBOX_CHANGE,LEN_BUS_TICKETBOX_CHANGE);
		BYTE lpMsg[LEN_BUS_TICKETBOX_CHANGE]={0};
		theSCMessageMakeHelper.MakeMagazineExchange(operComHdr, magazine_change, lpMsg);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_TICKETBOX_CHANGE, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     钱箱更换电文  	 4.2.16
@param     CASHBOX_CHANGE cashbox_change 钱箱更换信息
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgBanknoteBoxExchange(CASHBOX_CHANGE& cashbox_change)
{
	try
	{
		//算出数据包的长度
		int nSize = cashbox_change.vecCash.size(); //vector中的每个元素长度为1BIN+2BIN
		DWORD dwLen_CASHBOX_CHANGE = LEN_BUSINESS_HEADER + 8 + 3 * nSize; //协议头 + 钱箱更换的数据长度
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_MONEYBOX_CHANGE, dwLen_CASHBOX_CHANGE);
		//BYTE lpMsg[dwLen_CASHBOX_CHANGE]={0};
		LPBYTE lpMsg = (LPBYTE)malloc(dwLen_CASHBOX_CHANGE);//由于数据纸币箱和硬币箱的币种数量不一样，导致数据包的长度不一样
		if (lpMsg == NULL)
		{
			//未分配成功
			return;
		}
		theSCMessageMakeHelper.MakeCashBoxExchange(operComHdr, cashbox_change, lpMsg);
		theBUS_TXN_MGR.SaveTxnData(dwLen_CASHBOX_CHANGE, lpMsg);
		free(lpMsg);
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{

	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief    生成设备部件构成数据  

@param    none  

@retval   none

*/
//////////////////////////////////////////////////////////////////////////
//void CBusinessTranslateSvc::InsertMsgDeviceComponentInfo()                                   
//{
//	try{
//		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_DEVICE_COMPONENT,LEN_BUS_DEVICE_COMPONENT);
//		BYTE lpMsg[LEN_BUS_DEVICE_COMPONENT]={0};
//		theSCMessageMakeHelper.MakeDeviceComponentInfo(operComHdr,lpMsg);
//		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_DEVICE_COMPONENT, lpMsg);
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TVM故障单业务数据

@param      (in)BUSINESS_FAILED_DATA inBusiness_data 数据体主要填充数据

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgBussnessFailedData(BUSINESS_FAILED_DATA inBusiness_data){
	try{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_FAILURE_REPORT,LEN_BUSINESS_FAILED_DATA);
		BYTE lpMsg[LEN_BUSINESS_FAILED_DATA] = {0};
		theSCMessageMakeHelper.MakeBusinessFaliedData(operComHdr,inBusiness_data,lpMsg);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUSINESS_FAILED_DATA,lpMsg);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}