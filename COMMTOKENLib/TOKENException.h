#pragma once
#include "afx.h"
#include "SysException.h"
#include "TOKENLIBExp.h"

/**
@brief	外部读写器软件异常类
*/

class TOKENCOMLIB_DECLSPEC CTOKENException : public CSysException
{
public:
    static const int MODULE_ID = 12502; // 模块号

    typedef enum _detail_error_code {
        SET_PARAM_ERR           = 1,	// 设定值：参数异常
        FUNC_PARAM_ERR          = 2,	// 函数参数异常
        SEQUENCE_ERR	        = 3,	// 函数调用顺序异常
        FUNC_ERR_RETURN	        = 4,	// 函数返回异常
        LOGIC_ERR	            = 5,	// 逻辑异常
        OTHER_ERR	            = 6,	// 异常发生
    } DETAIL_ERROR_CODE;

    CTOKENException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CTOKENException(void);

private:
};
