#include "StdAfx.h"
#include "InnerException.h"
#include "LogException.h"
#include "log.h"
#include "Sync.h"
#include "logthread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CLog::s_path = _T("");       // Trace目录
CLogThread* CLog::s_LogThread = NULL;
//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CString fileNamePrefix       文件名前缀（包括最后的字符"_"）
@param      (i)bool concurrence             是否并行

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLog::CLog(CString fileNamePrefix)
{
    m_fileNamePrefix = fileNamePrefix;      // 文件名前缀
    m_fileName = _T("");
	m_file = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLog::~CLog(void)
{
    try {
       delete m_file;
	   m_file = NULL;
    }
    catch (...) {
        // do nothing
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初期处理

@param      (i)CString path         Trace目录

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLog::Initialize(CString path)
{
    s_path = path;
	//s_LogThread = (CLogThread*)(CUtilThread::StartThread(RUNTIME_CLASS(CLogThread),"CLogThread"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写日志

@param      (i)CString data                  一行Trace数据(不包括操作时间部分)

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLog::WriteData(CString& data)
{
        // 操作时间
        SYSTEMTIME dtCurTime;
        ::GetLocalTime(&dtCurTime);
        CString strPrefix = _T("");
		strPrefix.Format(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d.%.3d   threadID %d"),dtCurTime.wYear,dtCurTime.wMonth,dtCurTime.wDay,
			dtCurTime.wHour,dtCurTime.wMinute,dtCurTime.wSecond,dtCurTime.wMilliseconds,GetCurrentThreadId());
		//if(s_LogThread!=NULL)
		//{
		//	pair<CLog*,CString>* writeLogTask = new pair<CLog*,CString>(this,strPrefix + ","  + data);
		//	s_LogThread->PostThreadMessage(WM_USER_LOG_ENQUEUE,NULL,(LPARAM)writeLogTask);
		//}
		WriteDataImp(strPrefix + _T(",")  + data);

		
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     写日志实现 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CLog::WriteDataImp(CString& data)
{
	try {
		// 路径未设置，则抛出异常
		if (s_path == _T("")) {
			return;
		}
		//SYNC(LOGFILE,_T("FILE_SYNC_LOG_THREAD"));
		// 文件未打开，则打开
		/*if (m_file == NULL) {
			OpenFile();
		}*/
		// 当前业务日期与当前文件名中的日期不同，则关闭原文件，打开新文件
		CString strCurrentDateFileName = GetCurrentDateFileName();
		SYNC(LOGFILE,strCurrentDateFileName);
		if (strCurrentDateFileName != m_fileName) {
			delete m_file;
			m_file = NULL;
			m_fileName = strCurrentDateFileName;
			m_file = new CXFile(strCurrentDateFileName);
		}
		// 写文件
		CXFile::ERR_DETAITL ret = m_file->WriteFile(data);

		if (CXFile::FILE_OPERATION_OK != ret){
			throw CLogException(CLogException::FILE_WRITE_FAIL,_T(__FILE__),__LINE__);
		}
	}    
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取文件名

@param      无

@retval     CString strFileName     文件名

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CLog::GetCurrentDateFileName()
{
    if (s_path == _T("")) {
        throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__);
    }
    
	// 操作时间
	SYSTEMTIME dtCurTime;
	::GetLocalTime(&dtCurTime);
    CString strDate;
	strDate.Format(_T("%.4d%.2d%.2d"),dtCurTime.wYear,dtCurTime.wMonth,dtCurTime.wDay);

    CString fileName = s_path + m_fileNamePrefix + strDate + CLOG_FILE_EXT_CHAR + CLOG_FILE_KIND;

    return fileName;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      打开文件
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CLog::OpenFile()
//{
//    try {
//        if (m_file != NULL) {
//            CloseFile();
//        }
//
//        m_fileName = GetFileName();
//        m_file = new CXFile(m_fileName);
//    }
//    catch(CSysException&) {
//        throw;
//    }
//    catch (...) {
//        throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__, m_fileName);
//    }
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      关闭文件
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CLog::CloseFile()
//{
//    try {
//        delete m_file;
//        m_file = NULL;
//        m_fileName = _T("");
//    }
//    catch(CSysException&) {
//        throw;
//    }
//    catch (...) {
//        throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__, m_fileName);
//    }
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      关闭日志

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLog::CloseLog(){	
	try {
		delete m_file;
		m_file = NULL;
		m_fileName = _T("");
	}
	catch(CSysException&) {
		//throw;
	}
	catch (...) {
		//throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__, m_fileName);
	}
}