#pragma once
#include "commonexp.h"
#include "Log.h"


#define theEXCEPTION_LOG  CExceptionLog::GetInstance()        ///< CExceptionLog的唯一接口

/**
  @brief 异常日志trace
*/

class COMMON_API  CExceptionLog : 
    public CLog
{
public:
    // 异常级别
    typedef enum _exception_level
    {
        LEVEL_WARNING,		    // 警告
        LEVEL_ERROR             // 错误
    } EXCEPTION_LEVEL;

    static CExceptionLog&  GetInstance();
    

	void WriteData(EXCEPTION_LEVEL exceptionLevel, long lMTCCode, CString strFileName, long lRowNum, CString strError);  //写日志
	void ReleaseExceptionLog();
private:
	static  CExceptionLog theInstance;

    CExceptionLog(void);
	~CExceptionLog(void);
	CExceptionLog(const CExceptionLog&);
	CExceptionLog& operator=(const CExceptionLog&); 
};
