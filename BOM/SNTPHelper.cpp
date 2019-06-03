#include "stdafx.h"
#include "SNTPHelper.h"
#include "csntpexception.h"
#include "CAfcDeviceCommonParam.h"
#include "MessageBox.h"
#include "BomStatusMgr.h"
#include "ServiceMgr.h"
#include "BusinessTranslateSvc.h"

CSNTPHelper CSNTPHelper::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSNTPHelper::CSNTPHelper(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_FORCE_TIME);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSNTPHelper::TimeSynchronize()
{

	CAfcDeviceCommonParam::COMMUNICATION comunication;
	theAFC_COMMON.GetCommunication(comunication);			// ���ͨѶ����
	int nRet = ::TimeSynchronize(comunication.ntpAlarmValue,comunication.ntpMalfunctionValue);

	if(nRet == NTP_SUCCESSED || nRet == NTP_SUCCESSED_AND_ALARM){
		try{
			_DATE_TIME datetime;
			_DATE_TIME currentDateTime =ComGetCurTime();
			if(!theEXCEPTION_MGR.HasException(CRWException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID))
			{
				CRWHelper rwHelper(*(theSERVICE_MGR.GetService(MAIN_SVC)));
				rwHelper.SetTime(currentDateTime,datetime);
			}
			if(!theEXCEPTION_MGR.HasException(CTWException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID))
			{
				CTWHelper twHelper(*(theSERVICE_MGR.GetService(MAIN_SVC)));
				twHelper.SetTime(currentDateTime,datetime);
			}

		}
		catch(CSysException& cboException){
			nRet = NTP_ERROR_OTHER;
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
	theBOM_STATUS_MGR.SetTimeSynStatus(timeStatus);
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
			operationDesc = _T("ʱ��ͬ������");
			break;
		case NTP_TIME_DIFF_FAULT:
			operLog.operCode = OPERATION_TIME_ERROR;
			operLog.operLevel = OPERATION_FOUR_LEVEL;
			operLog.result = 0x00;
			operationDesc = _T("ʱ��ͬ����ֵ�ﵽ����ֵ");
			break;
		default:
			operLog.operCode = OPERATION_TIME_ERROR;
			operLog.operLevel = OPERATION_FOUR_LEVEL;
			operLog.result = 0x00;
			operationDesc = _T("ʱ��ͬ����������");
			break;
		}
		memcpy(operLog.operDetail,operationDesc.GetBuffer(),operationDesc.GetLength());
		// �����豸������־		
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operLog);
		CString errorCode(_T(""));
		errorCode.Format(_T("%d"),nRet);
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,operationDesc+ errorCode);	

		// ͬ��ʱ������쳣
		
		if(nRet == NTP_SUCCESSED_AND_ALARM){
			CMessageBox::Show(_T("ʱ��ͬ��"),_T("ʱ��ͬ����ֵ����"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		}
		else if(nRet == NTP_TIME_DIFF_FAULT){
			throw CSNTPSException(CSNTPSException::TIME_SYNCHRONIZE_ERROR,_T(__FILE__),__LINE__,_T("ʱ��ͬ���ﵽ����ֵ"));
		}
		else
		{
			//throw CSNTPSException(CSNTPSException::TIME_SYNCHRONIZE_OTHER_ERROR,_T(__FILE__),__LINE__,"ʱ��ͬ������");
		}
	}
}

