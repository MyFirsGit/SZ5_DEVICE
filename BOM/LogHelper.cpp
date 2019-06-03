#include "stdafx.h"
#include "LogHelper.h"
#include "FTPHelper.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLogHelper::CLogHelper(CService &service):CServiceHelper(service)
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
CLogHelper::~CLogHelper()
{
}


long CLogHelper::PrepareUploadLogFile(CString &uploadFileName,vector<_DATE> logDates)
{
	uploadFileName = _T("");
	bool hasLog = false;
	_DATE_TIME currentTime = ComGetCurTime();
	uploadFileName.Format(_T("Log.%s.9001.%s.LOG")
		,theMAINTENANCE_INFO.GetCurrentDevice().ToString()
		,currentTime.ToString(_T("%04d%02d%02d%02d%02d%02d"))
		);
	uploadFileName = theBOM_INFO.GetLogPath()+uploadFileName;

	CXFile uploadLogFile(uploadFileName);

	std::map<CString,CString> singleLogFileNames;

	singleLogFileNames[FILTER_LOGIN_LOGOFF_LOG] = NOTE_LOGIN_LOGOFF_LOG;
	singleLogFileNames[FILTER_STAFFOPERATION_LOG] = NOTE_STAFFOPERATION_LOG;
	singleLogFileNames[FILTER_EXCEPTION_LOG] = NOTE_EXCEPTION_LOG;
	singleLogFileNames[FILTER_SC_CLIENT_LOG] = NOTE_SC_CLIENT_LOG;
	singleLogFileNames[FILTER_SC_CLIENT_DATA] = NOTE_SC_CLIENT_DATA;
	singleLogFileNames[FILTER_TXN_DATA] = NOTE_TXN_DATA;
	singleLogFileNames[FILTER_SC_SVC_LOG] = NOTE_SC_SVC_LOG;
	singleLogFileNames[FILTER_RW_LOG] = NOTE_DT_RW_LOG;
	singleLogFileNames[FILTER_TW_LOG] = NOTE_DT_TW_LOG;
	singleLogFileNames[FILTER_TH_LOG] = NOTE_TH_LOG;
	singleLogFileNames[FILTER_DEBUG_LOG] = NOTE_DEBUG_LOG;
	singleLogFileNames[FILTER_DCR4000_LOG] = NOTE_DCR4000_LOG;
	singleLogFileNames[FILTER_PRT_LOG] = NOTE_PRT_LOG;
	for(vector<_DATE>::iterator iterDate = logDates.begin();iterDate!=logDates.end();iterDate++)
	{
		_DATE dateOfLog = *iterDate;
		CString strDate = _T("");
		strDate.Format(_T("%04d%02d%02d"),dateOfLog.wYear, dateOfLog.biMonth, dateOfLog.biDay);
		CString strSeparator=_T("**************************************************************");
		for(std::map<CString,CString>::iterator iter = singleLogFileNames.begin();iter!=singleLogFileNames.end();iter++)
		{
			CString singleLogFileName =  theBOM_INFO.GetLogPath()+iter->first +strDate+_T(".LOG");
			if(ComCheckFileExist(singleLogFileName))
			{
				hasLog = true;
				BYTE buffer[10240]={0};
				uploadLogFile.WriteFile(strSeparator);
				uploadLogFile.WriteFile(iter->second);
				CXFile singleLogFile(singleLogFileName);
				UINT readCount = 0;
				while(readCount = singleLogFile.Read(buffer,10240))
				{
					uploadLogFile.Write(buffer,readCount);
				}
				uploadLogFile.WriteFile(_T("\r\n"));
			}
		}
	}
	return 1;
}


bool CLogHelper::UploadLogFile(_DATE dateOfLog)
{
	CString uploadFileFullPath = _T("");
	vector<_DATE> logDates;
	logDates.push_back(dateOfLog);
	CLogHelper::PrepareUploadLogFile(uploadFileFullPath,logDates);
	vector<CString> uploadFiles;
	uploadFiles.push_back(uploadFileFullPath);
	CString sUploadFtpPath = theSETTING.GetFileUploadFTPDir();
	bool success = theFTP_HELPER.UploadFiles(uploadFiles,sUploadFtpPath) == 1;
	DeleteFile(uploadFileFullPath);
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_DEBUG_UPLOAD);
	return success;
}