#pragma once
#include "afx.h"
#include "SysException.h"
#include "RWCOMLIBExp.h"
/**
@brief	�ڲ���д��Ӳ���쳣��
*/

class RWCOMLIB_DECLSPEC CTWHardwareException :
    public CSysException
{
public:
    static const int MODULE_ID = 29995;     // ģ���

    CTWHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CTWHardwareException(void);

};
