#pragma once
#include "Service.h"
#include "CoreExp.h"

/**
@brief Service Helper
*/

class  CORE_API CServiceHelper
{
public:
    CServiceHelper(CService& service);                      // ���캯��
    virtual ~CServiceHelper();                              // ��������

protected:
    CService& m_service;                                    // service
};
