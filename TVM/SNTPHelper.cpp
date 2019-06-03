#include "stdafx.h"
#include "SNTPHelper.h"
#include "csntpexception.h"
#include "CAfcDeviceCommonParam.h"
#include "MessageBox.h"
#include "tvmStatusMgr.h"
#include "ServiceMgr.h"
#include "BusinessTranslateSvc.h"

CSNTPHelper CSNTPHelper::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSNTPHelper::CSNTPHelper(void)
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
CSNTPHelper::~CSNTPHelper(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSNTPHelper::ForceTimeSynchronize()
{
	::TimeSynchronize();
	//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_FORCE_TIME);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSNTPHelper::TimeSynchronize(bool isInit)
{

	CAfcDeviceCommonParam::COMMUNICATION comunication;
	theAFC_COMMON.GetCommunication(comunication);			// 获得通讯参数
	int nRet = ::TimeSynchronize(comunication.ntpAlarmValue,comunication.ntpMalfunctionValue);

	if(nRet == NTP_SUCCESSED || nRet == NTP_SUCCESSED_AND_ALARM){
		if(!isInit){
			try{
				_DATE_TIME datetime;
				_DATE_TIME currentDateTime =ComGetCurTime();
				if(!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID))
				{
					//RECHARGERW_HELPER->SetTime(currentDateTime,datetime);
				}
				if(!theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID))
				{
					CARDRW_HELPER->SetTime(currentDateTime,datetime);
				}
				if(!theEXCEPTION_MGR.HasException(CRWException::TVM_TOKEN_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID))
				{
					//TOKENRW_HELPER->SetTime(currentDateTime,datetime);
				}
			}
			catch(CSysException& cboException){
				nRet = NTP_ERROR_OTHER;
			}
		}
	}
	TIME_STATUS timeStatus;
	switch(nRet)
	{
	case NTP_SUCCESSED:
		timeStatus = VALID_TIME;
		break;
	case NTP_SUCCESSED_AND_ALARM:
		timeStatus = WARN_TIME;
		break;
	default:
		timeStatus = ERROR_TIME;
		break;
	}
	theTVM_STATUS_MGR.SetTimeSynStatus(timeStatus);
	if(nRet!=NTP_SUCCESSED)
	{
		OPERATOR_LOG operLog;
		operLog.result = 0x00;
		CString operationDesc;
		switch(nRet)
		{
		case NTP_SUCCESSED_AND_ALARM:
			operLog.operCode = OPERATION_TIME_WARNING;
			operLog.operLevel = OPERATION_TWO_LEVEL;
			operLog.result = 0x01;
			operationDesc = _opl(TXT_CLOCK_ALARM);
			break;
		case NTP_TIME_DIFF_FAULT:
			operLog.operCode = OPERATION_TIME_ERROR;
			operLog.operLevel = OPERATION_FOUR_LEVEL;
			operLog.result = 0x00;
			operationDesc = _opl(TXT_CLOCK_DIFFERENCE_FAULT);
			break;
		default:
			operLog.operCode = OPERATION_TIME_ERROR;
			operLog.operLevel = OPERATION_FOUR_LEVEL;
			operLog.result = 0x00;
			operationDesc = _opl(TXT_CLOCK_OTHER_FAULT);
			break;
		}
		//memcpy(operLog.operDetail,operationDesc.GetBuffer(),operationDesc.GetLength());
		USES_CONVERSION;
		char *pContent = T2A(operationDesc);
		strncpy((char*)operLog.operDetail,pContent,32);

		// 发送设备操作日志		
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operLog);
		if (nRet == NTP_TIME_DIFF_FAULT)
		{
			CString errorCode(_T(""));
			errorCode.Format(_T("%d"),nRet);
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,operationDesc+ errorCode);	

		}

		// 同步时间故障异常
		
		if(nRet == NTP_SUCCESSED_AND_ALARM){
			//CMessageBox::Show(_tl(TXT_CLOCK_SYN),_tl(TXT_CLOCK_SYN_WARNING),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		}
		else if(nRet == NTP_TIME_DIFF_FAULT){
			throw CSNTPSException(CSNTPSException::TIME_SYNCHRONIZE_ERROR,_T(__FILE__),__LINE__,_tl(TXT_CLOCK_SYN_FAULT_VALUE));
		}
		else
		{
			//throw CSNTPSException(CSNTPSException::TIME_SYNCHRONIZE_OTHER_ERROR,_T(__FILE__),__LINE__,_T("时钟同步错误"));
		}
	}
}

