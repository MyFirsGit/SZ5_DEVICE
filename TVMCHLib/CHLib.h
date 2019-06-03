#pragma once
#include "CHLibExp.h"

// CH建立连接参数结构
typedef struct _mim_ch_connect_param
{
	DWORD ComPort;			// 串口号
	DWORD Speed;			// 波特率		
	DWORD MaxSendCnt;		// 发送队列消息数量最大值
	DWORD MaxSendBuffSize;	// 发送数据缓存区最大长度
	DWORD MaxRecvCnt;		// 接收队列消息数量最大值
	DWORD MaxRecvBuffSize;	// 接收数据缓存区最大长度
} CHTRS_CONNECT_PARAM, *LPCHTRS_CONNECT_PARAM;



#define	MAX_BUFFER_SIZE	3000

// 超时
#define TIME_OUT_20000			20000	// 超时时间20S
#define TIME_OUT_30000			30000	// 超时时间30S
#define TIME_OUT_60000			60000	// 超时时间60S
#define TIME_OUT_300000			300000	// 超时时间300S
//===========================================================
//Note: CHTRS = Media Issue Machine Transaction
//===========================================================
#define	CHTRS_ST_SNDQUE	0x00000001L	// 队列存在待发送数据项
#define	CHTRS_ST_RCVQUE	0x00000002L	// 队列存在待接收数据项
#define	CHTRS_ST_SNDEXE	0x00000004L	// 服务忙，正在发送数据
#define	CHTRS_ST_RCVEXE	0x00000008L	// 服务忙，正在接收数据
#define	CHTRS_ST_SNDERR	0x00000010L	// 发送失败
#define	CHTRS_ST_RCVERR	0x00000020L	// 接收失败 
#define	CHTRS_ST_RCVOVR	0x00000080L	// 接收缓存溢出

#define	CHTRS_ST_DTR	0x00010000L	///< DTR
#define	CHTRS_ST_DSR	0x00020000L	///< DSR
#define	CHTRS_ST_RTS	0x00040000L	///< RTS
#define	CHTRS_ST_CTS	0x00080000L	///< CTS

#define STATUS_ID_EXECUTE		0	///< Status of library, such as: sending, receiving
#define STATUS_ID_SEND_QUEUE	1	///< The remain data in the sending queue
#define STATUS_ID_RECV_QUEUE	2	///< Received data in the receiving queue

#define STATUS_PHASE_1			0	///< Waiting for DLE ENQ
#define STATUS_PHASE_2			1	///< Waiting for DLE STX
#define STATUS_PHASE_3			2	///< Waiting for DLE ETX
#define STATUS_PHASE_4			3	///< Waiting for DLE ETX
#define STATUS_PHASE_5			4	///< Waiting for DLE ACK (after send DLE ENQ)
#define STATUS_PHASE_6			5	///< Waiting for DLE ACk (after send DLE STX .. DLE ETX BCC)

typedef DWORD CHTRS_RESULT;
typedef DWORD CHTRS_HANDLE;        // CH连接对象句柄

#define	CHTRS_RES_OK				0L	// 操作成功

typedef enum _ch_err_detail {
    CH_SET_PARAM_ERR            = 1,	// 设定值：参数异常
    CH_FUNC_PARAM_ERR           = 2,	// 函数参数异常
    CH_SEQUENCE_ERR	            = 3,	// 函数调用顺序异常
    CH_FUNC_ERR_RETURN	        = 4,	// 函数返回异常
    CH_LOGIC_ERR	            = 5,	// 逻辑异常
    CH_OTHER_ERR                = 6,    // FTP相关错误   


    CHTRS_RES_WINERR           = 1001, // 系统错误

    CHTRS_ERR_HANDLE           = 1003, // The handle is invalid.
    CHTRS_RES_ERR_CHREAD       = 1100, // Could not create thread. Window error.
    CHTRS_RES_ERR_TIMEOUT      = 1101, // Timeout. Could not finished task in time.
    CHTRS_RES_RESET            = 1102, // Operation was terminate because of reseting.
    CHTRS_RES_STRANGE          = 1103, // Unrecognizable response.
    CHTRS_RECV_EMPTY           = 1104, // There is no data to receive.

    CHTRS_HTDL_ERR_FULL        = 1201, // Could not open connection because the conneciton pool is full
    CHTRS_HTDL_ERR_OPNTO       = 1202, // The operation could not finish in time.
    CHTRS_HTDL_ERR_WORKNG      = 1203, // Could not allocate workspace, out of memory.
    CHTRS_HTDL_ERR_SQUENG      = 1204, // Could not allocate the queue, out of memory.
    CHTRS_HTDL_ERR_RBUFNG      = 1206, // Could not allocate buffer for receiving data, out of memory.
    CHTRS_HTDL_ERR_SENDFAIL    = 1207, // Sending error. Unknow reason.
    CHTRS_HTDL_ERR_PORT        = 1208, // Could not open the specified serial port.
    CHTRS_HTDL_ERR_CTSOFF      = 1209, // Serial port does not support CTS (clear-to-send) event
    CHTRS_HTDL_ERR_SQUE        = 1210, // 发送队列满
    CHTRS_HTDL_ERR_RQUE        = 1211, // Too many items are queueing to be received

    CHTRS_MTX_ERR_ENQUIRE      = 1300, // ENQ发送失败，不能发送报文
    CHTRS_MTX_ERR_SEND         = 1301, // 三次重试仍然发送失败

    CHTRS_RFID_A_SUM_ERR       = 1500, // 票箱A的RFID数据SUM值异常
    CHTRS_RFID_B_SUM_ERR       = 1501, // 票箱B的RFID数据SUM值异常
    CHTRS_RFID_A_B_SUM_ERR     = 1502, // 票箱A和B的RFID数据SUM值异常

    CHTRS_MSG_EMPTY	        = 1350, // The message is empty now.
    CHTRS_MSG_OVRFLW	        = 1351, // The buffer is too small to store message.
    CHTRS_CMD_INVALID	        = 1352, // Unsupported command.
    CHTRS_CMD_PARAM	        = 1400, // Invalid parameter.
    CHTRS_CMD_REJECT	        = 1401  // Too many commands are queuing, could not send command.
}CH_ERR_DETAIL;

#define SWAPWORD(wValue) (WORD)((wValue << 8) | (wValue >> 8))

// 打开CH通信连接
DLLIMP CHTRS_RESULT CHTrs_Open(OUT CHTRS_HANDLE* pHandle,IN  void* pParam);

// 关闭CH通信连接
DLLIMP CHTRS_RESULT CHTrs_Close(OUT CHTRS_HANDLE Handle);

// 发送命令给CH
DLLIMP CHTRS_RESULT CHTrs_Send(IN  CHTRS_HANDLE Handle,IN  DWORD nSendLength,IN  void* pBuffer,OUT DWORD* pOutLength);

// 接收CH反馈数据
DLLIMP CHTRS_RESULT CHTrs_Recv(IN CHTRS_HANDLE Handle,IN DWORD nBuffLength,OUT void* pBuffer,OUT DWORD* pRecvLength);

// 重置CH通信处理
DLLIMP CHTRS_RESULT CHTrs_Reset(IN CHTRS_HANDLE Handle,IN BOOL bSendClear,IN BOOL bRecvClear);

// 获取当前通信状态
DLLIMP CHTRS_RESULT CHTrs_GetStat(IN CHTRS_HANDLE Handle,OUT DWORD* pStatus);

// 显示最后错误信息
DLLIMP void CHTrs_ShowLastError(HWND hParent);

// 打印CH日志
DLLIMP void CHTrs_Trace(const char* lpszFormat, ...);

// 清空CH日志文件
DLLIMP void CHTrs_Clear();

// 初始化CH日志文件
DLLIMP void CHTrs_Initialize(const char* tracePath);

// 关闭日志连接
DLLIMP void ReleaseCHLog();
