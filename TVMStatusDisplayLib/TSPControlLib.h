#pragma once
#include "DllExp.h"
#include "mtxfunc.h"

#ifndef _connect_param
#define _connect_param
// CH建立连接参数结构
typedef struct _connect_param
{
	DWORD ComPort;			// 串口号
	DWORD Speed;			// 波特率		
	DWORD MaxSendCnt;		// 发送队列消息数量最大值
	DWORD MaxSendBuffSize;	// 发送数据缓存区最大长度
	DWORD MaxRecvCnt;		// 接收队列消息数量最大值
	DWORD MaxRecvBuffSize;	// 接收数据缓存区最大长度
} CONNECT_PARAM, *LPCONNECT_PARAM;
#endif

#define	MAX_BUFFER_SIZE	3000

//===========================================================
//Note: CHTRS = Media Issue Machine Transaction
//===========================================================
#define	 ST_SNDQUE	0x00000001L	// 队列存在待发送数据项
#define	 ST_RCVQUE	0x00000002L	// 队列存在待接收数据项
#define	 ST_SNDEXE	0x00000004L	// 服务忙，正在发送数据
#define	 ST_RCVEXE	0x00000008L	// 服务忙，正在接收数据
#define	 ST_SNDERR	0x00000010L	// 发送失败
#define	 ST_RCVERR	0x00000020L	// 接收失败 
#define	 ST_RCVOVR	0x00000080L	// 接收缓存溢出

#define	 ST_DTR	0x00010000L	///< DTR
#define	 ST_DSR	0x00020000L	///< DSR
#define	 ST_RTS	0x00040000L	///< RTS
#define	 ST_CTS	0x00080000L	///< CTS

#define STATUS_ID_EXECUTE		0	///< Status of library, such as: sending, receiving
#define STATUS_ID_SEND_QUEUE	1	///< The remain data in the sending queue
#define STATUS_ID_RECV_QUEUE	2	///< Received data in the receiving queue

#define STATUS_PHASE_1			0	///< Waiting for DLE ENQ
#define STATUS_PHASE_2			1	///< Waiting for DLE STX
#define STATUS_PHASE_3			2	///< Waiting for DLE ETX
#define STATUS_PHASE_4			3	///< Waiting for DLE ETX
#define STATUS_PHASE_5			4	///< Waiting for DLE ACK (after send DLE ENQ)
#define STATUS_PHASE_6			5	///< Waiting for DLE ACk (after send DLE STX .. DLE ETX BCC)

typedef DWORD COMM_RESULT;
typedef DWORD COMM_HANDLE;        // CH连接对象句柄

#define	 RES_OK				0L	// 操作成功
#define	 RES_FAIL			1L	// 操作成功

#ifndef _err_detail
#define _err_detail
typedef enum _err_detail {
    ERR_WINERR           = 1001, // 系统错误

    ERR_HANDLE           = 1003, // The handle is invalid.
    ERR_CHREAD       = 1100, // Could not create thread. Window error.
    ERR_TIMEOUT      = 1101, // Timeout. Could not finished task in time.
    ERR_RESET            = 1102, // Operation was terminate because of reseting.
    ERR_STRANGE          = 1103, // Unrecognizable response.
    ERR_RECV_EMPTY           = 1104, // There is no data to receive.

    HTDL_ERR_FULL        = 1201, // Could not open connection because the conneciton pool is full
    HTDL_ERR_OPNTO       = 1202, // The operation could not finish in time.
    HTDL_ERR_WORKNG      = 1203, // Could not allocate workspace, out of memory.
    HTDL_ERR_SQUENG      = 1204, // Could not allocate the queue, out of memory.
    HTDL_ERR_RBUFNG      = 1206, // Could not allocate buffer for receiving data, out of memory.
    HTDL_ERR_SENDFAIL    = 1207, // Sending error. Unknow reason.
    HTDL_ERR_PORT        = 1208, // Could not open the specified serial port.
    HTDL_ERR_CTSOFF      = 1209, // Serial port does not support CTS (clear-to-send) event
    HTDL_ERR_SQUE        = 1210, // 发送队列满
    HTDL_ERR_RQUE        = 1211, // Too many items are queueing to be received

    MTX_ERR_ENQUIRE      = 1300, // ENQ发送失败，不能发送报文
    MTX_ERR_SEND         = 1301, // 三次重试仍然发送失败

    RFID_A_SUM_ERR       = 1500, // 票箱A的RFID数据SUM值异常
    RFID_B_SUM_ERR       = 1501, // 票箱B的RFID数据SUM值异常
    RFID_A_B_SUM_ERR     = 1502, // 票箱A和B的RFID数据SUM值异常

    MSG_EMPTY	        = 1350, // The message is empty now.
    MSG_OVRFLW	        = 1351, // The buffer is too small to store message.
    CMD_INVALID	        = 1352, // Unsupported command.
    CMD_PARAM	        = 1400, // Invalid parameter.
    CMD_REJECT	        = 1401  // Too many commands are queuing, could not send command.
}ERR_DETAIL;
#endif

#define SWAPWORD(wValue) (WORD)((wValue << 8) | (wValue >> 8))

// 打开CH通信连接
DLLIMP COMM_RESULT COMM_Open(OUT COMM_HANDLE* pHandle,IN  void* pParam);

// 关闭CH通信连接
DLLIMP COMM_RESULT COMM_Close(OUT COMM_HANDLE Handle);

// 发送命令给CH
DLLIMP COMM_RESULT COMM_Send(IN  COMM_HANDLE Handle,IN  DWORD nSendLength,IN  void* pBuffer,OUT DWORD* pOutLength);

// 接收CH反馈数据
DLLIMP COMM_RESULT COMM_Recv(IN COMM_HANDLE Handle,IN DWORD nBuffLength,OUT void* pBuffer,OUT DWORD* pRecvLength);

// 重置CH通信处理
DLLIMP COMM_RESULT COMM_Reset(IN COMM_HANDLE Handle,IN BOOL bSendClear,IN BOOL bRecvClear);

// 获取当前通信状态
DLLIMP COMM_RESULT COMM_GetStat(IN COMM_HANDLE Handle,OUT DWORD* pStatus);

// 初始化日志
DLLIMP void CTSPTrs_Initialize(const char* tracePath);

// 打印日志
DLLIMP void CTSPTrs_Trace(const char* lpszFormat,...);

// 清空日志文件
DLLIMP void CTSPTrs_Clear();

// 关闭日志连接
DLLIMP void CTSPTrs_Close();