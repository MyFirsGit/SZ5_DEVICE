#include "stdafx.h"
#include "LogHelper.h"
#include "FTPHelper.h"
#include "InfoZip.h"
#include<atlconv.h>

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

//////////////////////////////////////////////////////////////////////////
/*
@brief      打包日志文件

@param (o) CString &uploadFileName 打包后的文件名
@param (i) vector<_DATE> logDates需要打包的日志日期

@retval    long 1 成功 

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CLogHelper::PrepareUploadLogFile(CString &uploadFileName,vector<_DATE> logDates)
{
	uploadFileName = _T("");
	bool hasLog = false;
	_DATE_TIME currentTime = ComGetCurTime();
	uploadFileName.Format(_T("Log.%s.9001.%s.LOG")
		,theMAINTENANCE_INFO.GetCurrentDevice().ToString()
		,currentTime.ToString(_T("%04d%02d%02d%02d%02d%02d"))
		);
	uploadFileName = theTVM_INFO.GetLogPath()+uploadFileName;

	CXFile uploadLogFile(uploadFileName);

	std::map<CString,CString> singleLogFileNames;

	//通用日志
	singleLogFileNames[FILTER_LOGIN_LOGOFF_LOG]		= FILTER_LOGIN_LOGOFF_LOG;    // 登录登出日志文件
	singleLogFileNames[FILTER_STAFFOPERATION_LOG]	= FILTER_STAFFOPERATION_LOG;  // 站员操作日志文件
	singleLogFileNames[FILTER_EXCEPTION_LOG]        = FILTER_EXCEPTION_LOG;		  // 异常日志文件
	singleLogFileNames[FILTER_SC_CLIENT_LOG]		= FILTER_SC_CLIENT_LOG;       // SC客户端日志文件
	singleLogFileNames[FILTER_SC_CLIENT_DATA]		= FILTER_SC_CLIENT_DATA;      // SC通信数据日志文件
	singleLogFileNames[FILTER_TXN_DATA]				= FILTER_TXN_DATA;			  // 数据存储日志文件
	singleLogFileNames[FILTER_SC_SVC_LOG]			= FILTER_SC_SVC_LOG;          // SC服务日志文件
	singleLogFileNames[FILTER_DB_SQL_LOG]			= FILTER_DB_SQL_LOG;		  // 数据库SQL操作日志文件
	singleLogFileNames[FILTER_NETWORK_DB_LOG]		= FILTER_NETWORK_DB_LOG;	  // 网络数据库操作日志文件
	singleLogFileNames[FILTER_DEBUG_LOG]			= FILTER_DEBUG_LOG;           // DEBUG日志文件
	singleLogFileNames[FILTER_CCS_CLIENT_LOG]	    = FILTER_CCS_CLIENT_LOG;	  // CCS客户端日志文件
	singleLogFileNames[FILTER_CCS_CLIENT_DATA]		= FILTER_CCS_CLIENT_DATA;     // CCS通信数据日志文件
	singleLogFileNames[FILTER_CCS_SVC_LOG]			= FILTER_CCS_SVC_LOG;		  // CSS服务日志文件
	singleLogFileNames[FILTER_KEYBOARD_LOG]			= FILTER_KEYBOARD_LOG;		  // TVM维护键盘日志文件
	//硬件模块
	singleLogFileNames[FILTER_RECEIPT_PRINTER_LOG]	= FILTER_RECEIPT_PRINTER_LOG; // 凭证打印机日志文件
	singleLogFileNames[FILTER_DESKTOP_RW_LOG]		= FILTER_DESKTOP_RW_LOG;      // 桌面读写器日志文件
	singleLogFileNames[FILTER_RECHARGE_RW_LOG]		= FILTER_RECHARGE_RW_LOG;     // 充值用读写器日志文件
	singleLogFileNames[FILTER_TOKEN_RW_LOG]			= FILTER_TOKEN_RW_LOG;		  // Token处理模块用读写器日志文件
	singleLogFileNames[FILTER_CARD_RW_LOG]			= FILTER_CARD_RW_LOG;		  // 票卡处理模块用读写器日志文件
	//singleLogFileNames[FILTER_CARD_RECYCLE_RW_LOG]	  = FILTER_CARD_RECYCLE_RW_LOG;			// 票卡回收处理模块用读写器日志文件!!
	//singleLogFileNames[FILTER_TOKEN_RECYCLE_RW_LOG]     = FILTER_TOKEN_RECYCLE_RW_LOG;		// Token回收处理模块用读写器日志文件!!
	singleLogFileNames[FILTER_COIN_ADD_BOX1_RFID_LOG]     = FILTER_COIN_ADD_BOX1_RFID_LOG;		// 硬币补充箱1RFID日志文件
	singleLogFileNames[FILTER_COIN_ADD_BOX2_RFID_LOG]     = FILTER_COIN_ADD_BOX2_RFID_LOG;		// 硬币补充箱2RFID日志文件
	singleLogFileNames[FILTER_COIN_RECYCLE_BOX1_RFID_LOG] = FILTER_COIN_RECYCLE_BOX1_RFID_LOG;	// 硬币回收箱1RFID日志
	singleLogFileNames[FILTER_COIN_RECYCLE_BOX2_RFID_LOG] = FILTER_COIN_RECYCLE_BOX2_RFID_LOG;	// 硬币回收箱2RFID日志

	//模块日志
	singleLogFileNames[FILTER_CARD_MODULE_LOG]			  = FILTER_CARD_MODULE_LOG;				// 票卡处理模块日志文件
	singleLogFileNames[FILTER_TOKEN_MODULE_LOG]			  = FILTER_TOKEN_MODULE_LOG;			// Token处理模块日志文件
	singleLogFileNames[FILTER_BANKCARD_MODULE_LOG]		  = FILTER_BANKCARD_MODULE_LOG;			// 纸币处理模块日志文件
	singleLogFileNames[FILTER_COIN_MODULE_LOG]			  = FILTER_COIN_MODULE_LOG;				// 硬币处理模块日志文件
	singleLogFileNames[FILTER_PIN_PAD_LOG]				  = FILTER_PIN_PAD_LOG;					// 密码键盘模块日志文件
	singleLogFileNames[FILTER_BANKCARD_MODULE_LOG]		  = FILTER_BANKCARD_MODULE_LOG;			// 银行卡处理模块日志文件
	singleLogFileNames[FILTER_IO_MODULE_LOG]			  = FILTER_IO_MODULE_LOG;				// IO模块日志文件
	singleLogFileNames[FILTER_STATUS_DISPLAY_LOG]		  = FILTER_STATUS_DISPLAY_LOG;			// 状态显示屏日志文件
	singleLogFileNames[FILTER_CARD_RECYCLE_MODULELOG]     = FILTER_CARD_RECYCLE_MODULELOG;		// 票卡回收模块日志文件
	singleLogFileNames[FILTER_TOKEN_RECYCLE_MODULE_LOG]	  = FILTER_TOKEN_RECYCLE_MODULE_LOG;	// Token回收模块日志文件
	singleLogFileNames[FILTER_CARD_PRINTER_LOG]			  = FILTER_CARD_PRINTER_LOG;			// 票卡打印机日志文件
	singleLogFileNames[FILTER_MAGAZINE_MODULE_LOG]		  = FILTER_MAGAZINE_MODULE_LOG;			// 票箱模块日志文件
	singleLogFileNames[FILTER_DCR4000_LOG]			      = FILTER_DCR4000_LOG;					// DCR4000日志文件



	for(vector<_DATE>::iterator iterDate = logDates.begin();iterDate!=logDates.end();iterDate++)
	{
		_DATE dateOfLog = *iterDate;
		CString strDate = _T("");
		strDate.Format(_T("%04d%02d%02d"),dateOfLog.wYear, dateOfLog.biMonth, dateOfLog.biDay);
		CString strSeparator=_T("**************************************************************");
		for(std::map<CString,CString>::iterator iter = singleLogFileNames.begin();iter!=singleLogFileNames.end();iter++)
		{
			CString singleLogFileName =  theTVM_INFO.GetLogPath()+iter->first +strDate+_T(".LOG");
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
				CString fileName = _T("TestFtp");
				uploadLogFile.WriteFile(fileName);
			}
		}
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     上传日志文件 

@param (i) _DATE dateOfLog 需要上传哪天的日志

@retval     bool 成功或失败

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CLogHelper::UploadLogFile(_DATE dateOfLog)
{
	CString uploadFileFullPath = _T("");
	vector<_DATE> logDates;
	logDates.push_back(dateOfLog);
	CLogHelper::PrepareUploadLogFile(uploadFileFullPath,logDates);
	vector<CString> uploadFiles;
	uploadFiles.push_back(uploadFileFullPath);
	CString sUploadFtpPath = theTVM_SETTING.GetFileUploadFTPDir();
	bool success = theFTP_HELPER.UploadFiles(uploadFiles,sUploadFtpPath) == 1;
	DeleteFile(uploadFileFullPath);
	return success;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     压缩并上传日志文件 

@param      none

@retval     bool 成功或失败

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CLogHelper::PackAndUploadLog(){
	// 取所有的日志
	CString logPath = theTVM_INFO.GetLogPath();
	CString strDest;
	//CString strTemp;
	bool bRet = true;
	vector<CString> vecFilePath;
	vecFilePath.clear();
	//strTemp.Format(_T("%sTemp\\"),logPath);//Like:  D:\\TVM_Data\\log\\Temp\\

	CFileFind finder;
	BOOL bFinder = finder.FindFile(logPath + _T("*.log"));
	while(bFinder){
		bFinder = finder.FindNextFile();
		//CString strFileName = finder.GetFileName();
		if(finder.IsDots() || finder.IsDirectory()){
			continue;
		}
		else {
			CString strFilePathName = finder.GetFilePath();
			//if(ComCheckAndCreateDirectory(strTemp)){
			//	if(::CopyFile(strFilePathName,strTemp + finder.GetFileName(),FALSE)){
			//		strFilePathName = strTemp + finder.GetFileName();
					if(!strFilePathName.IsEmpty()){
						vecFilePath.push_back(strFilePathName);
					}
			//	}
			//}
		}
	}
	finder.Close();
	USES_CONVERSION;
	// 将日志打包
	int nCount = vecFilePath.size();
	char** chPathCount = new char* [nCount];
	for(int i = 0;i<nCount;i++){
		chPathCount[i] = T2A(vecFilePath[i]);
	}
	
	// 	strDest.Format(_T("Log.%s.9001.%s.LOG"),theMAINTENANCE_INFO.GetCurrentDevice().ToString(), ComGetCurTime().ToString(_T("%04d%02d%02d%02d%02d%02d")));// 压缩包格式：Log.01011101.20150513101010
	strDest.Format(_T("Log.%s.9001.%s.ZIP"),theMAINTENANCE_INFO.GetCurrentDevice().ToString(), ComGetCurTime().ToString(_T("%04d%02d%02d%02d%02d%02d")));// 压缩包格式：Log.01011101.20150513101010

	CInfoZip zip;
	// 初始化压缩日志
	if(zip.InitializeZip()){
		// 添加文件并压缩
		//if(zip.AddFiles(T2A(logPath + strDest),chPathCount,nCount)){
		// 			// 删除临时拷贝的文件
		// 			//::RemoveDirectory(strTemp);
		// 			bRet = true;
		// 		}
		if(zip.AddFilesEx((logPath + _T("*.log")),(logPath + strDest))){
			// 删除临时拷贝的文件
			//::RemoveDirectory(strTemp);
			bRet = true;
			CString strDestNew = logPath + strDest;
			strDestNew.Replace(_T("ZIP"),_T("LOG"));
			CString strDestOld = logPath + strDest;
			CFile::Rename(strDestOld,strDestNew);
			strDest.Replace(_T("ZIP"),_T("LOG"));
		}
		else{
			bRet = false;
		}
		zip.FinalizeZip();

		// 释放资源
		delete []chPathCount;
		chPathCount = NULL;
		if(!bRet)return false;
	}
	else{
		return	false;
	}

	CString strZipLogPath,strBackupZipLogPath;
	strZipLogPath.Format(_T("%sLogZipBackup\\Unsend\\"),logPath);
	strBackupZipLogPath.Format(_T("%sLogZipBackup\\History\\"),logPath);
	CString sUploadFtpPath = theTVM_SETTING.GetFileUploadFTPDir();

	// 开启日志
	theDEBUG_LOG->SetWriteDebug(true);

	if(ComCheckAndCreateDirectory(strZipLogPath) && ComCheckAndCreateDirectory(strBackupZipLogPath)){
		for(int tryCount = 0;tryCount < 3; tryCount++){
			// 移动压缩日志到备份目录,并且上传
			if(::MoveFile(logPath + strDest,strZipLogPath + strDest)){
				for (vector<CString>::iterator iter = vecFilePath.begin();iter!=vecFilePath.end();iter++)
				{
					DeleteFile(*iter);
				}
				// 上传尝试三次
				for(int trySend = 0; trySend < 3; trySend++){
					// 打开FTP连接
					if(theFTP_HELPER.FTPConnect()){
						int nSended = 0;
						// 遍历未送目录是否有没有上传的日志
						//vector<CString> uploadFiles;
						CFileFind itFinder;
						bool bisFind = itFinder.FindFile(strZipLogPath + _T("Log*.*"));
						while(bisFind){
							bisFind = itFinder.FindNextFile();
							if(itFinder.IsDots() || itFinder.IsDirectory()){
								continue;
							}
							else{
								CString strPathName = itFinder.GetFilePath();
								// 上传文件：成功则移动到已经上传目录，否则不改变状态
								if(theFTP_HELPER.FTPUploadFile(strPathName,sUploadFtpPath)){
									::MoveFile(strPathName,strBackupZipLogPath + itFinder.GetFileName());
									nSended++;
								}
							}
						}
						theFTP_HELPER.FTPClose();
						if(nSended>0){
							//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_DEBUG_UPLOAD);
						}
						itFinder.Close();
						// 上传完成
						break;
					}
					else continue;
				}
				break;
			}
			else continue;
		}
		
	}
	return true;
}