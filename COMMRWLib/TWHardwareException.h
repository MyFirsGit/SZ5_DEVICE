#pragma once
#include "afx.h"
#include "SysException.h"
#include "RWCOMLIBExp.h"
/**
@brief	内部读写器硬件异常类
*/

class RWCOMLIB_DECLSPEC CTWHardwareException :
    public CSysException
{
public:
    static const int MODULE_ID = 29995;     // 模块号

    CTWHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CTWHardwareException(void);

};
