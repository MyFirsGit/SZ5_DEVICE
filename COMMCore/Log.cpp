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

CString CLog::s_path = _T("");       // TraceĿ¼
CLogThread* CLog::s_LogThread = NULL;
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CString fileNamePrefix       �ļ���ǰ׺�����������ַ�"_"��
@param      (i)bool concurrence             �Ƿ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLog::CLog(CString fileNamePrefix)
{
    m_fileNamePrefix = fileNamePrefix;      // �ļ���ǰ׺
    m_fileName = _T("");
	m_file = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief      ���ڴ���

@param      (i)CString path         TraceĿ¼

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLog::Initialize(CString path)
{
    s_path = path;
	//s_LogThread = (CLogThread*)(CUtilThread::StartThread(RUNTIME_CLASS(CLogThread),"CLogThread"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д��־

@param      (i)CString data                  һ��Trace����(����������ʱ�䲿��)

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CLog::WriteData(CString& data)
{
        // ����ʱ��
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
@brief     д��־ʵ�� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CLog::WriteDataImp(CString& data)
{
	try {
		// ·��δ���ã����׳��쳣
		if (s_path == _T("")) {
			return;
		}
		//SYNC(LOGFILE,_T("FILE_SYNC_LOG_THREAD"));
		// �ļ�δ�򿪣����
		/*if (m_file == NULL) {
			OpenFile();
		}*/
		// ��ǰҵ�������뵱ǰ�ļ����е����ڲ�ͬ����ر�ԭ�ļ��������ļ�
		CString strCurrentDateFileName = GetCurrentDateFileName();
		SYNC(LOGFILE,strCurrentDateFileName);
		if (strCurrentDateFileName != m_fileName) {
			delete m_file;
			m_file = NULL;
			m_fileName = strCurrentDateFileName;
			m_file = new CXFile(strCurrentDateFileName);
		}
		// д�ļ�
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
@brief      ȡ�ļ���

@param      ��

@retval     CString strFileName     �ļ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CLog::GetCurrentDateFileName()
{
    if (s_path == _T("")) {
        throw CLogException(CLogException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__);
    }
    
	// ����ʱ��
	SYSTEMTIME dtCurTime;
	::GetLocalTime(&dtCurTime);
    CString strDate;
	strDate.Format(_T("%.4d%.2d%.2d"),dtCurTime.wYear,dtCurTime.wMonth,dtCurTime.wDay);

    CString fileName = s_path + m_fileNamePrefix + strDate + CLOG_FILE_EXT_CHAR + CLOG_FILE_KIND;

    return fileName;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���ļ�
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
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
//@brief      �ر��ļ�
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
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
@brief      �ر���־

@param      ��

@retval     ��

@exception  ��
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