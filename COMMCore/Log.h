#pragma once

#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "coreexp.h"

/**
@brief 日志trace
*/

#define CLOG_FILE_EXT_CHAR							_T(".")
#define CLOG_FILE_KIND								_T("log")

class CLogThread;

class CORE_API CLog
{
public:
    CLog(CString fileNamePrefix);  // 构造函数
    virtual ~CLog(void);                                     // 析构函数

    static void Initialize(CString path);                              // 设置目录

protected:
    CString GetCurrentDateFileName();                                          // 取文件名
    //void OpenFile();                                                // 打开文件
    //void CloseFile();                                               // 关闭文件
    void WriteData(CString& data);                                   // 写日志
	void CloseLog();												 // 关闭日志
private:

	static CLogThread* s_LogThread;
    static CString s_path;                                          // Trace目录
    CString m_fileNamePrefix;                                       // 文件名前缀（包括最后的字符"_"）
    CString m_fileName;                                             // 文件全名
    CXFile* m_file;                                                 // 文件对象
	void WriteDataImp(CString& data);
friend class CLogThread;
};
