#pragma once

#define	MIMTRS_RES_OK				0L	// 命令应答成功
//
typedef enum _th_err_detail {
	TH_SET_PARAM_ERR            = 1,	// 设定值：参数异常
	TH_FUNC_PARAM_ERR           = 2,	// 函数参数异常
	TH_SEQUENCE_ERR	            = 3,	// 函数调用顺序异常
	TH_FUNC_ERR_RETURN	        = 4,	// 函数返回异常
	TH_LOGIC_ERR	            = 5,	// 逻辑异常
	TH_OTHER_ERR                = 6,    // 其他相关错误 

	MIMTRS_RES_WINERR           = 1001, // 系统错误
	MIMTRS_ERR_HANDLE           = 1002, // 串口打开失败
	MIMTRS_OPEN_FILE_FAIL		= 1003, // 打开文件失败
	MIMTRS_READ_FILE_FAIL		= 1004, // 读取文件失败
	MIMTRS_CMD_EXECUTE_FAIL		= 1005, // 命令执行失败
	MIMTRS_RECV_EMPTY           = 1006, // 没有接收到数据.

}TH_ERR_DETAIL;

