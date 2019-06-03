#include "DllExp.h"

//LED屏幕日志打印类定义

class DLLIMP CTSPLog
{
public:
	CTSPLog();
	virtual ~CTSPLog();

    void Initialize(const char* tracePath);
	void OpenLog();
	void printf(const char *fmt, ... );
	void print(const char *text);
	void CloseLog();
    void ClearLog();
    void MakeFileName(char* fileName);
    void MakeFullName(char* strFullName);
  
private:
	HANDLE			 m_hFile;		// 日志文件句柄
	CRITICAL_SECTION m_hLock;		// lock for thread-safe access
	char			 m_strFileName[MAX_PATH];   // 日志文件名
    char			 m_strTracePath[MAX_PATH];  // 日志存放目录
    DWORD			 m_dwLineCount;
};

