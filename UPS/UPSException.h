#pragma once
#include "afx.h"
#include "SysException.h"
#include "UPSExp.h"

/**
@brief	UPS异常类
*/

class UPS_API CUPSException :
	public CSysException
{
public:
	static const int MODULE_ID = 29997;// 模块号

	typedef enum _detail_error_code {
		ERROR_UPS_OPEN_COM			= 1,      //打开时  打开UPS电源串口错误
		ERROR_UPS_CLOSE_COM			=2,      //打开时  关闭UPS电源串口错误
		//ERROR_UPS_STATUS_ERROR		= 3,      //打开时  UPS状态错误
		//OTHER_ERR,
		ERROR_UPS_WRITE_COM,
		ERROR_UPS_READ_COM,
		ERROR_UPS_WAITING_DATA_OUTTIME,
		ERROR_UPS_HEADER_FLAG, 
		ERROR_UPS_END_FLAG,
		ERROR_UPS_OTHER,
	} DETAIL_ERROR_CODE;

	CUPSException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CUPSException(void);
};
