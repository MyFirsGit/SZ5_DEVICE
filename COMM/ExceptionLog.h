#pragma once
#include "commonexp.h"
#include "Log.h"


#define theEXCEPTION_LOG  CExceptionLog::GetInstance()        ///< CExceptionLog��Ψһ�ӿ�

/**
  @brief �쳣��־trace
*/

class COMMON_API  CExceptionLog : 
    public CLog
{
public:
    // �쳣����
    typedef enum _exception_level
    {
        LEVEL_WARNING,		    // ����
        LEVEL_ERROR             // ����
    } EXCEPTION_LEVEL;

    static CExceptionLog&  GetInstance();
    

	void WriteData(EXCEPTION_LEVEL exceptionLevel, long lMTCCode, CString strFileName, long lRowNum, CString strError);  //д��־
	void ReleaseExceptionLog();
private:
	static  CExceptionLog theInstance;

    CExceptionLog(void);
	~CExceptionLog(void);
	CExceptionLog(const CExceptionLog&);
	CExceptionLog& operator=(const CExceptionLog&); 
};
