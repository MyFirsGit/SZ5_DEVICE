#pragma once
#include "Service.h"
#include "CoreExp.h"

/**
@brief Service Helper
*/

class  CORE_API CServiceHelper
{
public:
    CServiceHelper(CService& service);                      // 构造函数
    virtual ~CServiceHelper();                              // 析构函数

protected:
    CService& m_service;                                    // service
};
