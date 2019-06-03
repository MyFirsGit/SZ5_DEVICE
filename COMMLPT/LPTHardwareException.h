#pragma once
#include "afx.h"
#include "SysException.h"
#include "LPTExp.h"

/**
@brief	打印机异常类
*/

class LPT_API CLPTHardwareException :
	public CSysException
{
public:
	static const int MODULE_ID = 29994;// 模块号

	typedef enum _detail_error_code {
		SET_PARAM_ERR               = 1,	// 设定值：参数异常
		FUNC_PARAM_ERR              = 2,	// 函数参数异常
		SEQUENCE_ERR	            = 3,	// 函数调用顺序异常
		FUNC_ERR_RETURN	            = 4,	// 函数返回异常
		LOGIC_ERR	                = 5,	// 逻辑异常
		OTHER_ERR	                = 6,	// 异常发生
	} DETAIL_ERROR_CODE;

	CLPTHardwareException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CLPTHardwareException(void);
};
