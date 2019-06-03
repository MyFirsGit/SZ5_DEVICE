#pragma once
#include "BHLibExp.h"


// BHCHANGE Command
#define BH_CHANGE_GET_STATUS		0x01	// 取状态
#define BH_CHANGE_INIT				0x02	// 初始化
#define BH_CHANGE_COUNT				0x03	// 出币

#define BH_CHANGE_TRANSPORTATION	0x05	// 传送给乘客
#define BH_CHANGE_RETRIEVAL			0x06	// 传送到回收箱

#define BH_CHANGE_COUNT_TRAN_FRONT	0x09	// 出币并传送到前端
#define BH_CHANGE_COUNT_TRAN_REAR	0x0A	// 出币并传送到后端

#define BH_CHANGE_DEVICE_INFO		0x0E	// 设备信息查询或写入
#define BH_CHANGE_DIAGNOSIS			0x0F	// 诊断
#define BH_CHANGE_CANCEL			0x10	// 中断操作
#define BH_CHANGE_CHECK_COUNT		0x11	// 出币并自动检测真伪或破损
#define BH_CHANGE_SENSOR_DETECTION	0x14	// 传感器检测

// CONTROL Command
#define BH_CTRL_REQUEST				0x60	// 请求命令
#define BH_CTRL_RESPONSE			0xE0	// 反馈命令
#define BH_CTRL_ERR_RESPONSE		0xF0	// 错误应答
#define BH_CTRL_END_OF_COMMOND		0x1C	// 命令结束标志

// BH建立连接参数结构
typedef struct _mim_bh_connect_param_
{
	DWORD ComPort;			// 串口号
	DWORD Speed;			// 波特率		
	DWORD MaxSendCnt;		// 发送队列消息数量最大值
	DWORD MaxSendBuffSize;	// 发送数据缓存区最大长度
	DWORD MaxRecvCnt;		// 接收队列消息数量最大值
	DWORD MaxRecvBuffSize;	// 接收数据缓存区最大长度
} BHCHANGETRS_CONNECT_PARAM, *LPBHCHANGETRS_CONNECT_PARAM;

#define	MAX_BUFFER_SIZE	3000

//===========================================================
//Note: BHTRS = Media Issue Machine Transaction
//===========================================================
#define	BHTRS_ST_SNDQUE	0x00000001L	// 队列存在待发送数据项
#define	BHTRS_ST_RCVQUE	0x00000002L	// 队列存在待接收数据项
#define	BHTRS_ST_SNDEXE	0x00000004L	// 服务忙，正在发送数据
#define	BHTRS_ST_RCVEXE	0x00000008L	// 服务忙，正在接收数据
#define	BHTRS_ST_SNDERR	0x00000010L	// 发送失败
#define	BHTRS_ST_RCVERR	0x00000020L	// 接收失败 
#define	BHTRS_ST_RCVOVR	0x00000080L	// 接收缓存溢出

#define	BHTRS_ST_DTR	0x00010000L	///< DTR
#define	BHTRS_ST_DSR	0x00020000L	///< DSR
#define	BHTRS_ST_RTS	0x00040000L	///< RTS
#define	BHTRS_ST_CTS	0x00080000L	///< CTS

#define STATUS_ID_EXECUTE		0	///< Status of library, such as: sending, receiving
#define STATUS_ID_SEND_QUEUE	1	///< The remain data in the sending queue
#define STATUS_ID_RECV_QUEUE	2	///< Received data in the receiving queue

#define STATUS_PHASE_1			0	///< Waiting for DLE ENQ
#define STATUS_PHASE_2			1	///< Waiting for DLE STX
#define STATUS_PHASE_3			2	///< Waiting for DLE ETX
#define STATUS_PHASE_4			3	///< Waiting for DLE ETX
#define STATUS_PHASE_5			4	///< Waiting for DLE ACK (after send DLE ENQ)
#define STATUS_PHASE_6			5	///< Waiting for DLE ACk (after send DLE STX .. DLE ETX BCC)

typedef DWORD BHTRS_RESULT;
typedef DWORD BHTRS_HANDLE;        // BH连接对象句柄

#define	BHTRS_RES_OK				0L	// 操作成功

//typedef enum _bh_err_detail_ {
#define    BH_SET_PARAM_ERR            1	// 设定值：参数异常
#define    BH_FUNC_PARAM_ERR           2	// 函数参数异常
#define    BH_SEQUENCE_ERR	           3	// 函数调用顺序异常
#define    BH_FUNC_ERR_RETURN	       4	// 函数返回异常
#define    BH_LOGIC_ERR	               5	// 逻辑异常
#define    BH_OBHER_ERR                6    // FTP相关错误          

#define    BHTRS_RES_WINERR            1001 // 系统错误

#define    BHTRS_ERR_HANDLE            1003 // The handle is invalid.
#define    BHTRS_RES_ERR_BHREAD        1100 // Could not create thread. Window error.
#define    BHTRS_RES_ERR_TIMEOUT       1101 // Timeout. Could not finished task in time.
#define    BHTRS_RES_RESET             1102 // Operation was terminate because of reseting.
#define    BHTRS_RES_STRANGE           1103 // Unrecognizable response.
#define    BHTRS_RECV_EMPTY            1104 // There is no data to receive.

#define    BHTRS_HTDL_ERR_FULL         1201 // Could not open connection because the conneciton pool is full
#define    BHTRS_HTDL_ERR_OPNTO        1202 // The operation could not finish in time.
#define    BHTRS_HTDL_ERR_WORKNG       1203 // Could not allocate workspace, out of memory.
#define    BHTRS_HTDL_ERR_SQUENG       1204 // Could not allocate the queue, out of memory.
#define    BHTRS_HTDL_ERR_RBUFNG       1206 // Could not allocate buffer for receiving data, out of memory.
#define    BHTRS_HTDL_ERR_SENDFAIL     1207 // Sending error. Unknow reason.
#define    BHTRS_HTDL_ERR_PORT         1208 // Could not open the specified serial port.
#define    BHTRS_HTDL_ERR_CTSOFF       1209 // Serial port does not support CTS (clear-to-send) event
#define    BHTRS_HTDL_ERR_SQUE         1210 // 发送队列满
#define    BHTRS_HTDL_ERR_RQUE         1211 // Too many items are queueing to be received

#define    BHTRS_MTX_ERR_ENQUIRE       1300 // ENQ发送失败，不能发送报文
#define    BHTRS_MTX_ERR_SEND          1301 // 三次重试仍然发送失败

#define    BHTRS_RFID_A_SUM_ERR        1500 // 票箱A的RFID数据SUM值异常
#define    BHTRS_RFID_B_SUM_ERR        1501 // 票箱B的RFID数据SUM值异常
#define    BHTRS_RFID_A_B_SUM_ERR      1502 // 票箱A和B的RFID数据SUM值异常

#define    BHTRS_MSG_EMPTY			   1350 // The message is empty now.
#define    BHTRS_MSG_OVRFLW	           1351 // The buffer is too small to store message.
#define    BHTRS_CMD_INVALID	       1352 // Unsupported command.
#define    BHTRS_CMD_PARAM	           1400 // Invalid parameter.
#define    BHTRS_CMD_REJECT	           1401  // Too many commands are queuing, could not send command.
//}BH_CHANGE_ERR_DETAIL;

#define SWAPWORD(wValue) (WORD)((wValue << 8) | (wValue >> 8))

// 打开BH通信连接
DLLIMP BHTRS_RESULT BHTrs_Open(OUT BHTRS_HANDLE* pHandle,IN  void* pParam);

// 关闭BH通信连接
DLLIMP BHTRS_RESULT BHTrs_Close(OUT BHTRS_HANDLE Handle);

// 发送命令给BH
DLLIMP BHTRS_RESULT BHTrs_Send(IN  BHTRS_HANDLE Handle,IN  DWORD nSendLength,IN  void* pBuffer,OUT DWORD* pOutLength);

// 接收BH反馈数据
DLLIMP BHTRS_RESULT BHTrs_Recv(IN BHTRS_HANDLE Handle,IN DWORD nBuffLength,OUT void* pBuffer,OUT DWORD* pRecvLength);

// 重置BH通信处理
DLLIMP BHTRS_RESULT BHTrs_Reset(IN BHTRS_HANDLE Handle,IN BOOL bSendClear,IN BOOL bRecvClear);

// 获取当前通信状态
DLLIMP BHTRS_RESULT BHTrs_GetStat(IN BHTRS_HANDLE Handle,OUT DWORD* pStatus);

// 显示最后错误信息
DLLIMP void BHTrs_ShowLastError(HWND hParent);

// 初始化BH日志文件
DLLIMP void BHChangeTrs_Initialize(const char* tracePath);

// 打印BH日志
DLLIMP void BHTrs_Trace(const char* lpszFormat, ...);

// 清空BH日志文件
DLLIMP void BHTrs_Clear();

// 关闭日志
DLLIMP void BHChange_CloseLog();



