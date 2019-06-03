#define MAX_DEVICES		2     // 最大可连接设备数量

// 设备连接及通信控制结构
typedef struct device_tbl_tag 
{
	bool				bUsed;			// 设备连接是否已被使用  A flag indicates the device is being used.
	bool				bClose;			// 设备连接是否已经关闭  A flad indicate the service is closed.
	HANDLE				hSync;			// 服务线程启动同步事件
	HANDLE				hThread;		// 服务线程句柄
	CHTRS_RESULT		nResult;		// 命令执行结果
	HANDLE				hSend;			// Handle of an event used to send data
	HANDLE				hRecv;			// Handle of an event used to receive data
	HANDLE				hClose;			// 关闭连接事件
	CHTRS_CONNECT_PARAM	Parm;		// 设备连接参数结构
	HTDLWORK*			pWork;			// Pointer to a structure storing active transaction's information. Reffer @ref HTDLWORK.
} DEVICETBL, *PDEVICETBL;