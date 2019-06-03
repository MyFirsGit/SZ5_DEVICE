#pragma once
#include "Log.h"
#include "SvcDef.h"

#define theLOGIN_LOGOFF    CLOGLoginAndLogoff::GetInstance()            ///< CLOGLoginAndLogoff的唯一接口

/**
  @brief 登录登出日志trace
*/

class  CLOGLoginAndLogoff : 
    public CLog
{
public:
    //登录区分
    typedef enum _loginout_type
    {
        LOG_IN = 1,                     // 登录
        LOG_OFF = 2                     // 登出
    } LOGINOUT_TYPE;



    static CLOGLoginAndLogoff&  GetInstance()
    {
        return theInstance;
    }

    void WriteData(LOGINOUT_TYPE nOperationDifference,CString  iUserID,STAFF_AUTHORITY nAuthority); 

private:
	static CLOGLoginAndLogoff theInstance;

	CLOGLoginAndLogoff(void);
	~CLOGLoginAndLogoff(void);
	CLOGLoginAndLogoff(const CLOGLoginAndLogoff&);
	CLOGLoginAndLogoff& operator=(const CLOGLoginAndLogoff&); 
};
