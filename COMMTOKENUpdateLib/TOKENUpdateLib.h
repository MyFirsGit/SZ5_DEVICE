#pragma once

#define	TOKENTRS_RES_OK				0L	// 命令应答成功
//
typedef enum _th_err_detail {
	TOKEN_SET_PARAM_ERR            = 1,	// 设定值：参数异常
	TOKEN_FUNC_PARAM_ERR           = 2,	// 函数参数异常
	TOKEN_SEQUENCE_ERR	            = 3,	// 函数调用顺序异常
	TOKEN_FUNC_ERR_RETURN	        = 4,	// 函数返回异常
	TOKEN_LOGIC_ERR	            = 5,	// 逻辑异常
	TOKEN_OTHER_ERR                = 6,    // 其他相关错误 

	TOKENTRS_RES_WINERR           = 1001, // 系统错误
	TOKENTRS_ERR_HANDLE           = 1002, // 串口打开失败
	TOKENTRS_OPEN_FILE_FAIL		= 1003, // 打开文件失败
	TOKENTRS_READ_FILE_FAIL		= 1004, // 读取文件失败
	TOKENTRS_CMD_EXECUTE_FAIL		= 1005, // 命令执行失败
	TOKENTRS_RECV_EMPTY           = 1006, // 没有接收到数据.

}TOKEN_ERR_DETAIL;

