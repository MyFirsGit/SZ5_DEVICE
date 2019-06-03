#pragma once
#include "afx.h"
#include "SysException.h"
#include "SntpApi.h"

/**
@brief	时钟同步异常类
*/

class SNTP_API CSNTPSException :
	public CSysException
{
public:
	static const int MODULE_ID = 10107;     // 模块号

	typedef enum _detail_error_code {
		TIME_SYNCHRONIZE_ERROR      = 200,  // 时钟同步故障
		TIME_SYNCHRONIZE_OTHER_ERROR = 201,//时钟同步其他错误
	} DETAIL_ERROR_CODE;

	CSNTPSException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CSNTPSException(void);

};
