#pragma once
#include "afx.h"
#include "SysException.h"
#include "BHexp.h"
#include "BHChangeLib.h"

/**
 @brief		纸币找零模块异常类
*/

class BH_API CBHChangeException : public CSysException
{
public:
	static const int MODULE_ID = 13202;     // 模块号

	typedef enum _detail_error_code {
		SET_PARAM_ERR				= 1001,	// 设定值：参数异常
		FUNC_PARAM_ERR				= 1002,	// 函数参数异常
		SEQUENCE_ERR				= 1003,	// 函数调用顺序异常
		FUNC_ERR_RETURN				= 1004,	// 函数返回异常
		LOGIC_ERR					= 1005,	// 逻辑异常
		OTHER_ERR					= 1006,	// 异常发生

		ERROR_BH_OTHER				= 1010,	// 其它异常
		ERROR_BH_OPEN_FAILED		= 1011,	// 打开时	串口打开失败
		ERROR_BH_ALREADY_OPENED		= 1012,	// 打开时	串口重复打开
		ERROR_BH_CLOSE_FAILED		= 1013,	// 关闭时	串口关闭失败
		ERROR_BH_ALREADY_CLOSED		= 1014,	// 关闭时	串口重复关闭
		ERROR_BH_NOT_OPEN			= 1015,	// 通信时	串口未打开
		ERROR_BH_CMD				= 1016,	// 通信时	命令异常
		ERROR_BH_SEND				= 1017,	// 通信时	送信异常
		ERROR_BH_RECEIVE			= 1018,	// 通信时	接收异常
		ERROR_BH_DESERIALIZE		= 1019,	// 通信时	解析回复信息异常
		ERROR_BH_RECEIVE_TIMEOUT	= 1020,	// 通信时	接收超时
		ERROR_BH_SEQUNCE_ERR		= 1231,	// 命令时序错误
	} DETAIL_ERROR_CODE;

    CBHChangeException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CBHChangeException();

private:
};
