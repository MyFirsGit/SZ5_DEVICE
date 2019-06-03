#pragma once
#include "Log.h"
#include "coreexp.h"
#include <map>

/**
@brief �˿ͽ������
*/
#define theOPRATION_LOG CFileLog::GetInstance(FILTER_OPERATION_LOG)
#define LOG(Msg,...) theOPRATION_LOG->WriteData(_T(""),CString(__FUNCTION__), __LINE__,_T(Msg),__VA_ARGS__)

/*
@brief FTP������־��
*/
#define theFTPHelper_LOG CFileLog::GetInstance(FILTER_FTP_HELPER_LOG)

/**
@brief Debug��trace
*/
#define theDEBUG_LOG  CFileLog::GetInstance(_T("Debug_"))
using namespace std;

class CORE_API CFileLog : 
	public CLog
{
public:
	static CFileLog* GetInstance(CString logPrefix);

	void SetWriteDebug(bool writeDebug);                // �����Ƿ��¼Debug��Ϣ
	void WriteData(CString className, CString methodName, int line, CString format, ...);
	void WriteData(CString prefix,LPBYTE lpData,UINT length,CString format = _T("%02x"));
	void WriteData(int line, CString prefix,LPBYTE lpData,UINT length,CString format = _T("%02x"));
	static void ReleaseDebugLog();
private:
	static  CFileLog theInstance;
	bool    m_writeDebug;                               // �Ƿ��¼Debug��Ϣ

	CFileLog(CString logPrefix);
	~CFileLog(void);
	map<CString,CFileLog*> mapLog;
};
