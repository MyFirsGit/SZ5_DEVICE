#pragma once
#include "afx.h"
#include "SysException.h"
#include "CHexp.h"

/**
 @brief		硬币接收模块异常类
*/

class CH_API CCHException :
	public CSysException
{
public:
	static const int MODULE_ID = 11802;     // 模块号

	typedef enum _detail_error_code {
		SET_PARAM_ERR				= 1,	// 设定值：参数异常
		FUNC_PARAM_ERR				= 2,	// 函数参数异常
		SEQUENCE_ERR				= 3,	// 函数调用顺序异常
		FUNC_ERR_RETURN				= 4,	// 函数返回异常
		LOGIC_ERR					= 5,	// 逻辑异常
		OTHER_ERR					= 6,	// 异常发生

		ERROR_CH_OTHER				= 10,	// 其它异常
		ERROR_CH_OPEN_FAILED		= 11,	// 打开时	串口打开失败
		ERROR_CH_ALREADY_OPENED		= 12,	// 打开时	串口重复打开
		ERROR_CH_CLOSE_FAILED		= 13,	// 关闭时	串口关闭失败
		ERROR_CH_ALREADY_CLOSED		= 14,	// 关闭时	串口重复关闭
		ERROR_CH_NOT_OPEN			= 15,	// 通信时	串口未打开
		ERROR_CH_CMD				= 16,	// 通信时	命令异常
		ERROR_CH_SEND				= 17,	// 通信时	送信异常
		ERROR_CH_RECEIVE			= 18,	// 通信时	接收异常
		ERROR_CH_DESERIALIZE		= 19,	// 通信时	解析回复信息异常
		ERROR_CH_RECEIVE_TIMEOUT	= 20,	// 通信时	接收超时
		ERROR_CH_SEQUNCE_ERR		= 231,	// 命令时序错误
		ERROR_CH_STOP_READ_AMOUNT	= 232,	// 停止读取金额
	} DETAIL_ERROR_CODE;

    CCHException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CCHException();

private:
};
