#pragma once
#include "afx.h"
#include "SysException.h"
#include "IOExp.h"

/**
@brief	TVM IO软件异常类
*/
class IO_API CIOException :	public CSysException
{
public:
	static const int MODULE_ID = 11902;// 模块号

	typedef enum _detail_error_code {
		SET_PARAM_ERR					= 1,	// 设定值：参数异常
		FUNC_PARAM_ERR				= 2,	// 函数参数异常
		SEQUENCE_ERR					= 3,	// 函数调用顺序异常
		FUNC_ERR_RETURN	            = 4,	// 函数返回异常
		LOGIC_ERR							= 5,	// 逻辑异常
		OTHER_ERR							= 6,	// 异常发生

		ERROR_IO_SEQUNCE_ERR			= 201,				// 命令时序错误
		ERROR_IO_OPEN						= 202,				// 打开错误
		ERROR_IO_CLOSE						= 203,				// 关闭错误
		ERROR_IO_SEND						= 204,				// 发送数据错误
		ERROR_IO_RECEIVE					= 205,				// 接收数据错误
		ERROR_IO_GETSTATE				= 206,				// 获取状态错误
		ERROR_IO_RECEIVE_TIMEOUT   	= 207				// 接收数据超时
	} DETAIL_ERROR_CODE;

	CIOException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CIOException(void);
};
