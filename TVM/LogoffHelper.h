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
    CLogoffHelper(CService& service);               // ���캯��
    ~CLogoffHelper();                               // ��������

    void Logoff();                                  // �ǳ�

private:
};
