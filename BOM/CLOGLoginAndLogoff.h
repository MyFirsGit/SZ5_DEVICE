#pragma once
#include "Log.h"
#include "SvcDef.h"

#define theLOGIN_LOGOFF    CLOGLoginAndLogoff::GetInstance()            ///< CLOGLoginAndLogoff��Ψһ�ӿ�

/**
  @brief ��¼�ǳ���־trace
*/

class  CLOGLoginAndLogoff : 
    public CLog
{
public:
    //��¼����
    typedef enum _loginout_type
    {
        LOG_IN = 1,                     // ��¼
        LOG_OFF = 2                     // �ǳ�
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
