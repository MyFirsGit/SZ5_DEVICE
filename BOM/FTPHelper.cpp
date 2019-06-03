#include "StdAfx.h"
#include "FTPHelper.h"
#include "FTPCommand.h"
#include "ParameterDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

static const TCHAR* const STR_TEST_FTP_FILE_NAME = _T("TestFtp.txt");
CFTPHelper CFTPHelper::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CFTPHelper::CFTPHelper(void)
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
CFTPHelper::~CFTPHelper(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ز����ļ�

@param      (i)vector<CString> vecParamPathFile FTP�ļ�·�����ļ���

@retval     ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CFTPHelper::DownloadParam(vector<CString> vecParamPathFile)
{
	int counts = vecParamPathFile.size();
	CString msg = _T("");
	msg.Format(_T("%d"),counts);
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("DownloadParam"),__LINE__,_T("<< Download File Count:")+msg);
	try{
		m_vecDownloadFiles = vecParamPathFile;

		// ȡ��������Ŀ¼
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		m_localPath = CString(szRunPath) + BOM_PM_UPDATE_DIR;
		
		// ����FTP
		if(FTPConnect()){		
			vector<CString>::iterator iter = m_vecDownloadFiles.begin();
			while (iter != m_vecDownloadFiles.end()) {
				// ���سɹ�
				if(FTPDownloadFile((*iter),m_localPath) != 0){
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("�ļ���" )+ (*iter) + _T(" ����ʧ�ܡ�"));		
				}
				iter ++;
			} 
			FTPClose();
		}
		else{
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("FTP����������ʧ�ܡ�"));	
		}
	}
	catch (CSysException& e) {
		FTPClose();
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		FTPClose();
		theEXCEPTION_MGR.ProcessException(CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("DownloadParam"),__LINE__,_T(">>"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���س����ļ�

@param      (i)vector<CString> vecParamPathFile FTP�ļ�·�����ļ���

@retval     ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CFTPHelper::DownloadProgm(CString csDownloadFiles,CString slocalPath)
{
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("DownloadProgm"),__LINE__,_T("<<")+csDownloadFiles);
	try{
		// ȡ��������Ŀ¼
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		m_localPath = slocalPath;

		// ����FTP
		if(FTPConnect()){		
			if(FTPDownloadFile(csDownloadFiles,m_localPath) != 0){	
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("�ļ���") + csDownloadFiles + _T(" ����ʧ�ܡ�"));
			}
			FTPClose();
		}
		else{
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("FTP����������ʧ�ܡ�"));	
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("DownloadProgm"),__LINE__,_T(">>")+csDownloadFiles);
}

void CFTPHelper::DownloadFiles(vector<pair<CString,CString>> downLoadFiles,bool deleteOldFolder)
{
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("DownloadFiles"),__LINE__,_T("<<"));
	try{
		
		// ����FTP
		if(FTPConnect()){		
			vector<pair<CString,CString>>::iterator downLoadFile = downLoadFiles.begin();
			while(downLoadFile!=downLoadFiles.end())
			{
				if(deleteOldFolder)
				{
					DeleteFolder((*downLoadFile).second);
				}
				// ���سɹ�
				if(FTPDownloadFile( (*downLoadFile).first,(*downLoadFile).second )  == 0){					
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("�ļ���") + ((*downLoadFile).first) + _T(" ����ʧ�ܡ�"));					
				}	
				downLoadFile ++;
			}
		
			FTPClose();
		}	
		else{
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("FTP����������ʧ�ܡ�"));	
		}
	}
	catch (CSysException& e) {
		FTPClose();
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		FTPClose();
		theEXCEPTION_MGR.ProcessException(CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("DownloadFiles"),__LINE__,_T(">>"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ϴ��ļ�

@param      (i)vector<CString> vecLocalPathFile �����ļ�·�����ļ���
@param      (i)CString remotePath				FTP�ϴ�·��

@retval     ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
long CFTPHelper::UploadFiles(vector<CString> vecLocalPathFile,CString remotePath)
{
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("UploadFiles"),__LINE__,_T("<<"));
	long successCount = 0;
	try{
		m_vecUploadFiles = vecLocalPathFile;
		m_remotePath = remotePath;

		// ����FTP
		if(FTPConnect()){		
			vector<CString>::iterator iter = m_vecUploadFiles.begin();
			while (iter != m_vecUploadFiles.end()) {	
				// ���سɹ�
				if(!FTPUploadFile((*iter),m_remotePath)){					
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("�ļ���") + (*iter) + _T(" �ϴ�ʧ�ܡ�"));					
				}
				else
				{
					successCount++;
				}
				iter ++;
			} 
			FTPClose();
		}	
		else{
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("FTP����������ʧ�ܡ�"));	
		}
	}
	catch (CSysException& e) {
		FTPClose();
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		FTPClose();
		theEXCEPTION_MGR.ProcessException(CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("UploadFiles"),__LINE__,_T(">>"));
	return successCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      FTP�����ļ�

@param      (i)CString sRemoteFileName FTP�ļ�·�����ļ���
@param      (i)CString sUpdateDir��    ��������Ŀ¼

@retval     long  \n      0:�ɹ� ;��0 ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::FTPDownloadFile(CString sRemoteFileName, CString sLocalDir)
{
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("FTPDownloadFile"),__LINE__,sRemoteFileName + _T("<<"));
	BOOL success = TRUE;
	try{
		if(sRemoteFileName.GetLength() == 0 || sLocalDir.GetLength() == 0 ){
			throw CFtpException(CFtpException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("FTPDownloadFile()�����쳣"));
		}
		CFTPDownLoad ftpDownload;

		CString sFileName = sRemoteFileName;
		int nPos = sFileName.ReverseFind('/');
		if (nPos != -1){
			sFileName = sFileName.Mid(nPos+1);
		}
		CString sLocalFileName = sLocalDir + _T("\\") + sFileName;
		success = ftpDownload.DownLoadFile(sRemoteFileName,sLocalFileName);
		if(!success){
			throw CFtpException(CFtpException::FTP_DOWNLOAD_ERR, _T(__FILE__), __LINE__, _T("FTP���ش���" ));			
		}
		theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("FTPDownloadFile"),__LINE__,sRemoteFileName + _T(">>"));		
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return success;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ϴ�FTP�ļ�

@param      (i)CString sFileName��     �ϴ����ļ���
@param      (i)CString sUploadFolder   �ϴ�Ŀ¼

@retval     long  \n    0:�ɹ� ;��0 ʧ��
@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::FTPUploadFile(CString sFileName, CString sUploadFolder)
{
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("FTPDownloadFile"),__LINE__,sFileName + _T("<<"));
	if(sFileName.GetLength() == 0 || sUploadFolder.GetLength() == 0 ){
		throw CFtpException(CFtpException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("FTPUploadFile()�����쳣"));
	}
	CString uploadFileName = sUploadFolder + GetFileNameFromFullPath(sFileName);
	CFTPUpLoad ftpUpload;
	bool success = ftpUpload.UploadFile(sFileName,uploadFileName);
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("FTPDownloadFile"),__LINE__,sFileName + _T(">>"));
    return success;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��FTP����

@param      none

@retval     bool  true:�ɹ� false:ʧ��

@exception   CFtpException
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::FTPConnect()
{
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("FTPConnect"),__LINE__,_T("<<"));
    CString sHostID = theMAINTENANCE_INFO.GetFtpHostID();
    int iPort       = theMAINTENANCE_INFO.GetFtpPort();
    CString sUserID = theMAINTENANCE_INFO.GetFtpUserID();
    CString sPwd    = theMAINTENANCE_INFO.GetFtpPwd();
	CFTPConnect ftpConnect(sHostID, sUserID, sPwd, iPort);
	long errCode = ftpConnect.Execute();
	if(errCode != 0){	
		CString sErrorCode = _T("");
		sErrorCode.Format(_T("%d"),errCode);
		throw CFtpException(CFtpException::FTP_CONNECT_ERR, _T(__FILE__), __LINE__, _T("FTP����ʧ�ܣ�" + sErrorCode));			
	}
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("FTPConnect"),__LINE__,_T(">>"));
	return !errCode;	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ر�FTP����

@param      none

@retval     bool  true:�ɹ� false:ʧ��

@exception   CFtpException
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::FTPClose()
{
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("FTPClose"),__LINE__,_T("<<"));
	CFTPClose ftpClose;
	long errCode = ftpClose.Execute();
	if(errCode != 0){	
		CString sErrorCode = _T("");
		sErrorCode.Format(_T("%d"),errCode);
		throw CFtpException(CFtpException::FTP_CLOSE_ERR, _T(__FILE__), __LINE__, _T("FTP�رմ���") + sErrorCode);			
	}		
	theDEBUG_LOG->WriteData(_T("CFTPHelper"),_T("FTPClose"),__LINE__,_T(">>"));
	return !errCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::RepairException()
{
	bool bSuccess = FTPConnect();
	if (!bSuccess){
		return false;
	}
	CString uploadFileRemoteDir   = theSETTING.GetFileUploadFTPDir();
	CString uploadFileLocalPath  = theBOM_INFO.GetLogPath() + STR_TEST_FTP_FILE_NAME;
	CXFile testFtpFile(uploadFileLocalPath);
	CString strContent = _T("TestFtp");
	testFtpFile.WriteFile(strContent);
	testFtpFile.Close();
	FTPUploadFile(uploadFileLocalPath,uploadFileRemoteDir);
	DeleteFile(uploadFileLocalPath);

	CString downloadFileRemotePath = theSETTING.GetFileUploadFTPDir() + STR_TEST_FTP_FILE_NAME;
	CString downloadFileLocalDir =theBOM_INFO.GetLogPath() ;
	FTPDownloadFile(downloadFileRemotePath,downloadFileLocalDir);
	DeleteFile(downloadFileLocalDir+STR_TEST_FTP_FILE_NAME);
	FTPClose();
	return true;
}