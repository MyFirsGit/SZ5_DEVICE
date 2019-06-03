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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CFTPHelper::CFTPHelper(void)
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
CFTPHelper::~CFTPHelper(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      下载参数文件

@param      (i)vector<CString> vecParamPathFile FTP文件路径及文件名

@retval     无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CFTPHelper::DownloadParam(vector<CString> vecParamPathFile)
{
	int counts = vecParamPathFile.size();
	CString msg = _T("");
	msg.Format(_T("%d"),counts);
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("DownloadParam"),__LINE__,_T("<< Download File Count:")+msg);
	try{
		m_vecDownloadFiles = vecParamPathFile;

		// 取参数下载目录
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		m_localPath = CString(szRunPath) + PM_UPDATE_DIR;
		
		// 连接FTP
		if(FTPConnect()){		
			vector<CString>::iterator iter = m_vecDownloadFiles.begin();
			while (iter != m_vecDownloadFiles.end()) {
				// 下载成功
				if(FTPDownloadFile((*iter),m_localPath) != 0){
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("文件：") + (*iter) + _T(" 下载失败。"));		
				}
				iter ++;
			} 
			FTPClose();
		}
		else{
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("FTP服务器连接失败。"));	
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
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("DownloadParam"),__LINE__,_T(">>"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      下载程序文件

@param      (i)vector<CString> vecParamPathFile FTP文件路径及文件名

@retval     无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CFTPHelper::DownloadProgm(CString csDownloadFiles,CString slocalPath)
{
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("DownloadProgm"),__LINE__,_T("<<")+csDownloadFiles);
	try{
		// 取参数下载目录
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		m_localPath = slocalPath;

		// 连接FTP
		if(FTPConnect()){		
			if(FTPDownloadFile(csDownloadFiles,m_localPath) != 0){	
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("文件：") + csDownloadFiles + _T(" 下载失败。"));
			}
			FTPClose();
		}
		else{
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("FTP服务器连接失败。"));	
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("DownloadProgm"),__LINE__,_T(">>")+csDownloadFiles);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    通过FTP下载指定的文件为指定的文件名  

@param  vector<pair<CString,CString>> downLoadFiles 需要下载的文件名与下载后的文件名对列表
@param  deleteOldFolder 是否删除原来目录中的文件

@retval  无   

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CFTPHelper::DownloadFiles(vector<pair<CString,CString>> downLoadFiles,bool deleteOldFolder)
{
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("DownloadFiles"),__LINE__,_T("<<"));
	try{
		
		// 连接FTP
		if(FTPConnect()){		
			vector<pair<CString,CString>>::iterator downLoadFile = downLoadFiles.begin();
			while(downLoadFile!=downLoadFiles.end())
			{
				if(deleteOldFolder)
				{
					DeleteFolder((*downLoadFile).second);
				}
				// 下载成功
				if(FTPDownloadFile( (*downLoadFile).first,(*downLoadFile).second )  == 0){					
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("文件：") + ((*downLoadFile).first) + _T(" 下载失败。"));					
				}	
				downLoadFile ++;
			}
		
			FTPClose();
		}	
		else{
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("FTP服务器连接失败。"));	
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
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("DownloadFiles"),__LINE__,_T(">>"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上传文件

@param      (i)vector<CString> vecLocalPathFile 本地文件路径及文件名
@param      (i)CString remotePath				FTP上传路径

@retval     无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
long CFTPHelper::UploadFiles(vector<CString> vecLocalPathFile,CString remotePath)
{
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("UploadFiles"),__LINE__,_T("<<"));
	long successCount = 0;
	try{
		m_vecUploadFiles = vecLocalPathFile;
		m_remotePath = remotePath;

		// 连接FTP
		if(FTPConnect()){		
			vector<CString>::iterator iter = m_vecUploadFiles.begin();
			while (iter != m_vecUploadFiles.end()) {	
				// 下载成功
				if(!FTPUploadFile((*iter),m_remotePath)){					
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("文件：") + (*iter) + _T(" 上传失败。"));					
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
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,(long)0,_T(__FILE__),__LINE__,_T("FTP服务器连接失败。"));	
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
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("UploadFiles"),__LINE__,_T(">>"));
	return successCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      FTP下载文件

@param      (i)CString sRemoteFileName FTP文件路径及文件名
@param      (i)CString sUpdateDir　    本地下载目录

@retval     long  \n      0:成功 ;非0 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::FTPDownloadFile(CString sRemoteFileName, CString sLocalDir)
{
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("FTPDownloadFile"),__LINE__,sRemoteFileName + _T("<<"));
	BOOL success = TRUE;
	try{
		if(sRemoteFileName.GetLength() == 0 || sLocalDir.GetLength() == 0 ){
			throw CFtpException(CFtpException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("FTPDownloadFile()")+_tl(TXT_PARAMETER_EXCEPTION));
		}
		CFTPDownLoad ftpDownload;

		CString sFileName = sRemoteFileName;
		int nPos = sFileName.ReverseFind('/');
		if (nPos != -1){
			sFileName = sFileName.Mid(nPos+1);
		}
		CString sLocalFileName = sLocalDir + _T("\\") + sFileName;
		// Try three times!
		for(size_t i = 0;i < 3;i++){
			success = ftpDownload.DownLoadFile(sRemoteFileName,sLocalFileName);
			if(success)break;
		}		
		if(!success){
			throw CFtpException(CFtpException::FTP_DOWNLOAD_ERR, _T(__FILE__), __LINE__, _tl(TXT_DOWNLOAD_ERROR));			
		}
		theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("FTPDownloadFile"),__LINE__,sRemoteFileName + _T(">>"));		
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
@brief      上传FTP文件

@param      (i)CString sFileName　     上传的文件名
@param      (i)CString sUploadFolder   上传目录

@retval     long  \n    0:成功 ;非0 失败
@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::FTPUploadFile(CString sFileName, CString sUploadFolder)
{
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("FTPDownloadFile"),__LINE__,sFileName + _T("<<"));
	if(sFileName.GetLength() == 0 || sUploadFolder.GetLength() == 0 ){
		throw CFtpException(CFtpException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("FTPUploadFile()")+_tl(TXT_PARAMETER_EXCEPTION));
	}
	CString uploadFileName = sUploadFolder + GetFileNameFromFullPath(sFileName);
	CFTPUpLoad ftpUpload;
	bool success = false;
	for(size_t i = 0;i<3;i++){
		success = ftpUpload.UploadFile(sFileName,uploadFileName);
		if(success)break;
	}	
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("FTPDownloadFile"),__LINE__,sFileName + _T(">>"));
    return success;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得FTP连接

@param      none

@retval     bool  true:成功 false:失败

@exception   CFtpException
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::FTPConnect()
{
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("FTPConnect"),__LINE__,_T("<<"));
    CString sHostID = theMAINTENANCE_INFO.GetFtpHostID();
    int iPort       = theMAINTENANCE_INFO.GetFtpPort();
    CString sUserID = theMAINTENANCE_INFO.GetFtpUserID();
    CString sPwd    = theMAINTENANCE_INFO.GetFtpPwd();
	CFTPConnect ftpConnect(sHostID, sUserID, sPwd, iPort);
	long errCode = ftpConnect.Execute();
	if(errCode != 0){	
		CString sErrorCode = _T("");
		sErrorCode.Format(_T("%d"),errCode);
		throw CFtpException(CFtpException::FTP_CONNECT_ERR, _T(__FILE__), __LINE__, _tl(TXT_FTP_CONNECTION_FAILED) + sErrorCode);			
	}
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("FTPConnect"),__LINE__,_T(">>"));
	return !errCode;	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      关闭FTP连接

@param      none

@retval     bool  true:成功 false:失败

@exception   CFtpException
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::FTPClose()
{
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("FTPClose"),__LINE__,_T("<<"));
	CFTPClose ftpClose;
	long errCode = ftpClose.Execute();
	if(errCode != 0){	
		CString sErrorCode = _T("");
		sErrorCode.Format(_T("%d"),errCode);
		throw CFtpException(CFtpException::FTP_CLOSE_ERR, _T(__FILE__), __LINE__, _tl(TXT_FTP_CLOSE_ERROR) + sErrorCode);			
	}		
	theFTPHelper_LOG->WriteData(_T("CFTPHelper"),_T("FTPClose"),__LINE__,_T(">>"));
	return !errCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解除FTP异常

@param      无

@retval     bool 解除是否成功

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFTPHelper::RepairException()
{
	bool bSuccess = FTPConnect();
	if (!bSuccess){
		return false;
	}
	CString uploadFileRemoteDir   = theTVM_SETTING.GetFileUploadFTPDir();
	CString uploadFileLocalPath  = theTVM_INFO.GetLogPath() + STR_TEST_FTP_FILE_NAME;
	CXFile testFtpFile(uploadFileLocalPath);
	CString fileName = _T("TestFtp");
	testFtpFile.WriteFile(fileName);
	testFtpFile.Close();
	FTPUploadFile(uploadFileLocalPath,uploadFileRemoteDir);
	DeleteFile(uploadFileLocalPath);

	CString downloadFileRemotePath = theTVM_SETTING.GetFileUploadFTPDir() + STR_TEST_FTP_FILE_NAME;
	CString downloadFileLocalDir =theTVM_INFO.GetLogPath() ;
	FTPDownloadFile(downloadFileRemotePath,downloadFileLocalDir);
	DeleteFile(downloadFileLocalDir+STR_TEST_FTP_FILE_NAME);
	FTPClose();
	return true;
}