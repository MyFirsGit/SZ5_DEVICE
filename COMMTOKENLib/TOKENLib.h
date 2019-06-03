#pragma once
#include "stdafx.h"
#include "TOKENLibExp.h"

#define	MAX_BUFFER_SIZE	3000

//===========================================================
//Note: TOKENTRS = Media Issue Machine Transaction
//===========================================================
#define	TOKENTRS_ST_SNDQUE	0x00000001L	// 队列存在待发送数据项
#define	TOKENTRS_ST_RCVQUE	0x00000002L	// 队列存在待接收数据项
#define	TOKENTRS_ST_SNDEXE	0x00000004L	// 服务忙，正在发送数据
#define	TOKENTRS_ST_RCVEXE	0x00000008L	// 服务忙，正在接收数据
#define	TOKENTRS_ST_SNDERR	0x00000010L	// 发送失败
#define	TOKENTRS_ST_RCVERR	0x00000020L	// 接收失败 
#define	TOKENTRS_ST_RCVOVR	0x00000080L	// 接收缓存溢出

#define	TOKENTRS_ST_DTR	0x00010000L	///< DTR
#define	TOKENTRS_ST_DSR	0x00020000L	///< DSR
#define	TOKENTRS_ST_RTS	0x00040000L	///< RTS
#define	TOKENTRS_ST_CTS	0x00080000L	///< CTS

typedef DWORD TOKENTRS_RESULT;
typedef DWORD TOKENTRS_HANDLE;        // 连接对象句柄

#define	TOKENTRS_RES_OK				0L	// 命令应答成功

//
typedef enum _token_err_detail {
	TOKEN_SET_PARAM_ERR            = 1,	// 设定值：参数异常
	TOKEN_FUNC_PARAM_ERR           = 2,	// 函数参数异常
	TOKEN_SEQUENCE_ERR	            = 3,	// 函数调用顺序异常
	TOKEN_FUNC_ERR_RETURN	        = 4,	// 函数返回异常
	TOKEN_LOGIC_ERR	            = 5,	// 逻辑异常
	TOKEN_OTHER_ERR                = 6,    // 其他相关错误          

	TOKENTRS_RES_WINERR           = 1001, // 系统错误

	TOKENTRS_ERR_HANDLE           = 1003, // The handle is invalid.
	TOKENTRS_RES_ERR_THREAD       = 1100, // Could not create thread. Window error.
	TOKENTRS_RES_ERR_TIMEOUT      = 1101, // Timeout. Could not finished task in time.
	TOKENTRS_RES_RESET            = 1102, // Operation was terminate because of reseting.
	TOKENTRS_RES_STRANGE          = 1103, // Unrecognizable response.
	TOKENTRS_RECV_EMPTY           = 1104, // There is no data to receive.
	TOKENTRS_RECV_ERROR           = 1105, // There is no data to receive.


	TOKENTRS_HTDL_ERR_FULL        = 1201, // Could not open connection because the conneciton pool is full
	TOKENTRS_HTDL_ERR_OPNTO       = 1202, // The operation could not finish in time.
	TOKENTRS_HTDL_ERR_WORKNG      = 1203, // Could not allocate workspace, out of memory.
	TOKENTRS_HTDL_ERR_SQUENG      = 1204, // Could not allocate the queue, out of memory.
	TOKENTRS_HTDL_ERR_RBUFNG      = 1206, // Could not allocate buffer for receiving data, out of memory.
	TOKENTRS_HTDL_ERR_SENDFAIL    = 1207, // Sending error. Unknow reason.
	TOKENTRS_HTDL_ERR_PORT        = 1208, // Could not open the specified serial port.
	TOKENTRS_HTDL_ERR_CTSOFF      = 1209, // Serial port does not support CTS (clear-to-send) event
	TOKENTRS_HTDL_ERR_SQUE        = 1210, // 发送队列满
	TOKENTRS_HTDL_ERR_RQUE        = 1211, // Too many items are queueing to be received

	TOKENTRS_MTX_ERR_SEND         = 1301, // 三次重试仍然发送失败

	TOKENTRS_MSG_EMPTY	        = 1350, // The message is empty now.
	TOKENTRS_MSG_OVRFLW	        = 1351, // The buffer is too small to store message.
	TOKENTRS_CMD_INVALID	    = 1352, // Unsupported command.
	TOKENTRS_CMD_PARAM	        = 1400, // Invalid parameter.
	TOKENTRS_CMD_REJECT	        = 1401  // Too many commands are queuing, could not send command.

}TOKEN_ERR_DETAIL;

