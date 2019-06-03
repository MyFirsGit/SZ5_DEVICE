#pragma once
#include "ServiceHelper.h"
#include "audithelper.h"

/**
@brief Logoff Helper
*/

class CLogoffHelper :
    public CServiceHelper
{
public:
    CLogoffHelper(CService& service);               // 构造函数
    ~CLogoffHelper();                               // 析构函数

    void Logoff();                                  // 登出

private:
};
