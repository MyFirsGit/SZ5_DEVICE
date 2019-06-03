#include "stdafx.h"
#include "LogHelper.h"
#include "FTPHelper.h"
#include "InfoZip.h"
#include<atlconv.h>

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLogHelper::CLogHelper(CService &service):CServiceHelper(service)
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLogHelper::~CLogHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����־�ļ�

@param (o) CString &uploadFileName �������ļ���
@param (i) vector<_DATE> logDates��Ҫ�������־����

@retval    long 1 �ɹ� 

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

	//ͨ����־
	singleLogFileNames[FILTER_LOGIN_LOGOFF_LOG]		= FILTER_LOGIN_LOGOFF_LOG;    // ��¼�ǳ���־�ļ�
	singleLogFileNames[FILTER_STAFFOPERATION_LOG]	= FILTER_STAFFOPERATION_LOG;  // վԱ������־�ļ�
	singleLogFileNames[FILTER_EXCEPTION_LOG]        = FILTER_EXCEPTION_LOG;		  // �쳣��־�ļ�
	singleLogFileNames[FILTER_SC_CLIENT_LOG]		= FILTER_SC_CLIENT_LOG;       // SC�ͻ�����־�ļ�
	singleLogFileNames[FILTER_SC_CLIENT_DATA]		= FILTER_SC_CLIENT_DATA;      // SCͨ��������־�ļ�
	singleLogFileNames[FILTER_TXN_DATA]				= FILTER_TXN_DATA;			  // ���ݴ洢��־�ļ�
	singleLogFileNames[FILTER_SC_SVC_LOG]			= FILTER_SC_SVC_LOG;          // SC������־�ļ�
	singleLogFileNames[FILTER_DB_SQL_LOG]			= FILTER_DB_SQL_LOG;		  // ���ݿ�SQL������־�ļ�
	singleLogFileNames[FILTER_NETWORK_DB_LOG]		= FILTER_NETWORK_DB_LOG;	  // �������ݿ������־�ļ�
	singleLogFileNames[FILTER_DEBUG_LOG]			= FILTER_DEBUG_LOG;           // DEBUG��־�ļ�
	singleLogFileNames[FILTER_CCS_CLIENT_LOG]	    = FILTER_CCS_CLIENT_LOG;	  // CCS�ͻ�����־�ļ�
	singleLogFileNames[FILTER_CCS_CLIENT_DATA]		= FILTER_CCS_CLIENT_DATA;     // CCSͨ��������־�ļ�
	singleLogFileNames[FILTER_CCS_SVC_LOG]			= FILTER_CCS_SVC_LOG;		  // CSS������־�ļ�
	singleLogFileNames[FILTER_KEYBOARD_LOG]			= FILTER_KEYBOARD_LOG;		  // TVMά��������־�ļ�
	//Ӳ��ģ��
	singleLogFileNames[FILTER_RECEIPT_PRINTER_LOG]	= FILTER_RECEIPT_PRINTER_LOG; // ƾ֤��ӡ����־�ļ�
	singleLogFileNames[FILTER_DESKTOP_RW_LOG]		= FILTER_DESKTOP_RW_LOG;      // �����д����־�ļ�
	singleLogFileNames[FILTER_RECHARGE_RW_LOG]		= FILTER_RECHARGE_RW_LOG;     // ��ֵ�ö�д����־�ļ�
	singleLogFileNames[FILTER_TOKEN_RW_LOG]			= FILTER_TOKEN_RW_LOG;		  // Token����ģ���ö�д����־�ļ�
	singleLogFileNames[FILTER_CARD_RW_LOG]			= FILTER_CARD_RW_LOG;		  // Ʊ������ģ���ö�д����־�ļ�
	//singleLogFileNames[FILTER_CARD_RECYCLE_RW_LOG]	  = FILTER_CARD_RECYCLE_RW_LOG;			// Ʊ�����մ���ģ���ö�д����־�ļ�!!
	//singleLogFileNames[FILTER_TOKEN_RECYCLE_RW_LOG]     = FILTER_TOKEN_RECYCLE_RW_LOG;		// Token���մ���ģ���ö�д����־�ļ�!!
	singleLogFileNames[FILTER_COIN_ADD_BOX1_RFID_LOG]     = FILTER_COIN_ADD_BOX1_RFID_LOG;		// Ӳ�Ҳ�����1RFID��־�ļ�
	singleLogFileNames[FILTER_COIN_ADD_BOX2_RFID_LOG]     = FILTER_COIN_ADD_BOX2_RFID_LOG;		// Ӳ�Ҳ�����2RFID��־�ļ�
	singleLogFileNames[FILTER_COIN_RECYCLE_BOX1_RFID_LOG] = FILTER_COIN_RECYCLE_BOX1_RFID_LOG;	// Ӳ�һ�����1RFID��־
	singleLogFileNames[FILTER_COIN_RECYCLE_BOX2_RFID_LOG] = FILTER_COIN_RECYCLE_BOX2_RFID_LOG;	// Ӳ�һ�����2RFID��־

	//ģ����־
	singleLogFileNames[FILTER_CARD_MODULE_LOG]			  = FILTER_CARD_MODULE_LOG;				// Ʊ������ģ����־�ļ�
	singleLogFileNames[FILTER_TOKEN_MODULE_LOG]			  = FILTER_TOKEN_MODULE_LOG;			// Token����ģ����־�ļ�
	singleLogFileNames[FILTER_BANKCARD_MODULE_LOG]		  = FILTER_BANKCARD_MODULE_LOG;			// ֽ�Ҵ���ģ����־�ļ�
	singleLogFileNames[FILTER_COIN_MODULE_LOG]			  = FILTER_COIN_MODULE_LOG;				// Ӳ�Ҵ���ģ����־�ļ�
	singleLogFileNames[FILTER_PIN_PAD_LOG]				  = FILTER_PIN_PAD_LOG;					// �������ģ����־�ļ�
	singleLogFileNames[FILTER_BANKCARD_MODULE_LOG]		  = FILTER_BANKCARD_MODULE_LOG;			// ���п�����ģ����־�ļ�
	singleLogFileNames[FILTER_IO_MODULE_LOG]			  = FILTER_IO_MODULE_LOG;				// IOģ����־�ļ�
	singleLogFileNames[FILTER_STATUS_DISPLAY_LOG]		  = FILTER_STATUS_DISPLAY_LOG;			// ״̬��ʾ����־�ļ�
	singleLogFileNames[FILTER_CARD_RECYCLE_MODULELOG]     = FILTER_CARD_RECYCLE_MODULELOG;		// Ʊ������ģ����־�ļ�
	singleLogFileNames[FILTER_TOKEN_RECYCLE_MODULE_LOG]	  = FILTER_TOKEN_RECYCLE_MODULE_LOG;	// Token����ģ����־�ļ�
	singleLogFileNames[FILTER_CARD_PRINTER_LOG]			  = FILTER_CARD_PRINTER_LOG;			// Ʊ����ӡ����־�ļ�
	singleLogFileNames[FILTER_MAGAZINE_MODULE_LOG]		  = FILTER_MAGAZINE_MODULE_LOG;			// Ʊ��ģ����־�ļ�
	singleLogFileNames[FILTER_DCR4000_LOG]			      = FILTER_DCR4000_LOG;					// DCR4000��־�ļ�



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
@brief     �ϴ���־�ļ� 

@param (i) _DATE dateOfLog ��Ҫ�ϴ��������־

@retval     bool �ɹ���ʧ��

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
@brief     ѹ�����ϴ���־�ļ� 

@param      none

@retval     bool �ɹ���ʧ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CLogHelper::PackAndUploadLog(){
	// ȡ���е���־
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
	// ����־���
	int nCount = vecFilePath.size();
	char** chPathCount = new char* [nCount];
	for(int i = 0;i<nCount;i++){
		chPathCount[i] = T2A(vecFilePath[i]);
	}
	
	// 	strDest.Format(_T("Log.%s.9001.%s.LOG"),theMAINTENANCE_INFO.GetCurrentDevice().ToString(), ComGetCurTime().ToString(_T("%04d%02d%02d%02d%02d%02d")));// ѹ������ʽ��Log.01011101.20150513101010
	strDest.Format(_T("Log.%s.9001.%s.ZIP"),theMAINTENANCE_INFO.GetCurrentDevice().ToString(), ComGetCurTime().ToString(_T("%04d%02d%02d%02d%02d%02d")));// ѹ������ʽ��Log.01011101.20150513101010

	CInfoZip zip;
	// ��ʼ��ѹ����־
	if(zip.InitializeZip()){
		// ����ļ���ѹ��
		//if(zip.AddFiles(T2A(logPath + strDest),chPathCount,nCount)){
		// 			// ɾ����ʱ�������ļ�
		// 			//::RemoveDirectory(strTemp);
		// 			bRet = true;
		// 		}
		if(zip.AddFilesEx((logPath + _T("*.log")),(logPath + strDest))){
			// ɾ����ʱ�������ļ�
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

		// �ͷ���Դ
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

	// ������־
	theDEBUG_LOG->SetWriteDebug(true);

	if(ComCheckAndCreateDirectory(strZipLogPath) && ComCheckAndCreateDirectory(strBackupZipLogPath)){
		for(int tryCount = 0;tryCount < 3; tryCount++){
			// �ƶ�ѹ����־������Ŀ¼,�����ϴ�
			if(::MoveFile(logPath + strDest,strZipLogPath + strDest)){
				for (vector<CString>::iterator iter = vecFilePath.begin();iter!=vecFilePath.end();iter++)
				{
					DeleteFile(*iter);
				}
				// �ϴ���������
				for(int trySend = 0; trySend < 3; trySend++){
					// ��FTP����
					if(theFTP_HELPER.FTPConnect()){
						int nSended = 0;
						// ����δ��Ŀ¼�Ƿ���û���ϴ�����־
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
								// �ϴ��ļ����ɹ����ƶ����Ѿ��ϴ�Ŀ¼�����򲻸ı�״̬
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
						// �ϴ����
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