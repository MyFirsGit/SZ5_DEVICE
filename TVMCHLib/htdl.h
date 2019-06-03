/*****************************************************************************/
/* FILE NAME     : htdl.h                                                    */
/* PROGRAM NAME  : Header file of comm server.                               */
/* FUNCTION      : 1.                                                        */
/*                                                                           */
/*****************************************************************************/

// 通信方式定义
#define	COM_PORT	0				// 串口通信
#define MAIL_SLOT	1				// 邮槽通信
#define NAME_PIPE	2				// 管道通信

// 通信阶段定义
#define	MAX_PHASE	14			// 通信状态数量
#define PHASE_S1		0				// Phase 1, 空闲,等待数据发送
#define PHASE_S2		1				// Phase 2, 数据发送完毕，等待ACK / NAK应答
#define PHASE_S3		2				// Phase 3, 心跳（查询）请求发送完毕，等待ENQ应答
#define PHASE_S4		3				// Phase 4, 等待校验ENQ应答中的数据a（模块类别和对方局地址）
#define PHASE_S5		4				// Phase 5, 等待校验ENQ应答中的数据b（主局地址和传送次数）
#define PHASE_S6		5				// Phase 6, 等待校验ENQ应答中的数据c或c1（文本尺寸数据）
#define PHASE_S7		6				// Phase 7, 等待校验ENQ应答中的数据c2（文本尺寸数据）
#define PHASE_S8		7				// Phase 8, 等待接收ENQ应答中的文本数据
#define PHASE_S9		8				// Phase 9, 等待接收ENQ应答中的BCC的位1
#define PHASE_S10		9				// Phase 10, 等待接收ENQ应答中的BCC的位2，BCC确认
#define PHASE_S11		10				// Phase 11, ACK发送完毕，等待接收EOT
#define PHASE_S12		11				// Phase 12, NAK发送完毕，再次等待接收数据
#define PHASE_S13		12				// Phase 13, 等待处理ENQ应答中的数据接收错误
#define PHASE_S14		13				// Phase 14, EOT发送完毕，等待接收EOT

// 通信事件ID定义
#define MAX_EVENT		8			// All availables events				
#define EVENT_REQSEND	0			//
#define EVENT_ENQ		1			// 
#define EVENT_CHR		2			//
#define EVENT_ACK		3			// 
#define EVENT_NAK		4			//
#define EVENT_EOT		5			//
#define EVENT_TIMEOUT	6			// 
#define EVENT_QUERY		7			// 心跳查询事件
#define EVENT_ERROR		8			// 
#define EVENT_NONE		0xFFFF		// 

// 通信控制码定义
#define DLE				0x10		// 数据链路转义符
#define STX				0x02		// 报文开始标记码
#define ETX				0x03		// 报文结束标记码
#define ENQ				0x05		// 送信询问
#define ACK				0x06		// 送信询问的肯定应答码
#define NAK				0x15		// 送信询问的否定应答码
#define EOT				0x04		// 通信中止标志

// 上位->单元
// a 
#define INQUIRY			0x82		// 查询
#define SEND_DATA2		0xC2		// 发送数据（类型2）
#define SEND_DATA3		0xD2		// 发送数据（类型3）
// b
#define SEND_B			0x01		// 发送时B的值

//  单元->上位
// a
#define INQUIRY_RSP		0x10		// 查询反馈
#define RECV_DATA2		0x40		// 接收数据块（类型2）
#define RECV_DATA3		0x50		// 接收数据块（类型3）
// b
#define RECV_B			0x21		// 接收时B的值

// 最大值定义
#define MAX_MSG_LEN		1152//256			// 数据块的最大长度
#define MAX_RECVBUFF    1152//2*263			// DLE, STX, Seq, BlockNo, 2*MsgSize, DLE, ETX, BCC (4 + 256 + 3)
#define MAX_SENDBUFF    2*263		// DLE, STX, Seq, BlockNo, 2*MsgSize, DLE, ETX, BCC (4 + 256 + 3)
#define MAX_RETRYSEND	3			// 最大重试次数

// 命令
#define COMMAND_UNDEFINE		0x0000	// 未知
#define COMMAND_SET_PARAMETER	0x0006	// 参数设定
#define COMMAND_RESET			0x0007	// 复位
#define COMMAND_READ_STATUS		0x000C	// 状态读取
#define COMMAND_LOG_DATA		0x000D	// LOG Data要求
#define COMMAND_DATA_LIGHT		0x0002	// Data Light
#define COMMAND_PDL				0x0003	// PDL 待机
#define COMMAND_SET_INSERT		0x0020	// 投入许可
#define COMMAND_FORBID_INSERT	0x0021	// 投入禁止
#define COMMAND_TRAS_BEGIN_END	0x0030	// 交易开始、结束
#define COMMAND_TEMP_RETURN		0x0031	// 暂存退出
#define COMMAND_COUNT_OUT		0x0032	// 出币计数
#define COMMAND_TRANSFOR_OUT	0x0033	// 硬币传送
#define COMMAND_COLLOCTION		0x0040	// 回收
#define COMMAND_ABORT			0x0044	// 停止回收



#pragma pack(push) // 保存对齐状态
#pragma pack(1)   // 设定为1字节对齐

// 发送数据项结构
typedef struct send_item_tag
{
	HANDLE				hEvent;			// 发送完成事件
	CHTRS_RESULT*		pResult;		// 操作结果
	DWORD				nSendLength;	// 准备发送的字节数
	BYTE*				pCommand;		// 准备发送的数据指针
	BYTE*				pSending;		// 数据发送的起始位置指针
	WORD				wLastSendingLen;// 最后一次发送的数据字节长度
	DWORD*				pOutLength;		// 已发送数据字节长度
	WORD				wCommand;		// 命令
    send_item_tag()
    {
        hEvent = NULL;
        pResult = NULL;
        nSendLength = 0;
        pCommand = NULL;
        pSending = NULL;
        wLastSendingLen = 0;
        pOutLength = NULL;
		wCommand = COMMAND_UNDEFINE;
    }
} SENDITEM, *PSENDITEM;

// 接收数据项结构
typedef struct recv_item_tag
{
	HANDLE				hEvent;			// 接收完成事件
	CHTRS_RESULT*		pResult;		// 操作结果
	DWORD				nBuffSize;		// 接收数据缓存的大小
	BYTE*				pBuffer;		// 接收数据缓存指针
	DWORD*				pRecvLength;	// 接收的数据长度
    recv_item_tag()
    {
        hEvent = NULL;
        pResult = NULL;
        nBuffSize = 0;
        pBuffer = NULL;
        pRecvLength = NULL;
    }
} RECVITEM, *PRECVITEM;

// 发送队列结构
typedef struct send_queue_tag
{
	int					nMax;			// 发送队列最大元素个数
	int					nCnt;			// 当前队列元素个数
	int					nWP;			// 元素写入游标
	int					nRP;			// 元素读取游标
	SENDITEM			Que[1];			// 发送数据项数组
    send_queue_tag()
	{
        nMax = 0;
        nCnt = 0;
        nWP  = 0;
        nRP  = 0;
        memset(Que,0x00,sizeof(SENDITEM)*1);
    }
} SEND_QUEUE, *PSEND_QUEUE;

// 接收队列结构
typedef struct recv_queue_tag
{
	int					nMax;			// 接收队列最大元素个数
	int					nCnt;			// 当前队列元素个数
	int					nWP;			///< Position of the writing slot in the queue
	int					nRP;			///< Position of the reading slot in the queue
	RECVITEM			Que[1];			// 接收数据项数组
    recv_queue_tag() {
        nMax = 0;
        nCnt = 0;
        nWP  = 0;
        nRP  = 0;
        memset(Que,0x00,sizeof(RECVITEM)*1);
    }
} RECV_QUEUE, *PRECV_QUEUE;

// 缓存数据项结构
typedef struct buff_item_tag
{
//	int					nWritten;		///< Number of written bytes
	int					nWP;			///< Position of the writing slot
	BYTE*				pData;			///< Pointer to a buffer storing data
	buff_item_tag() {
		nWP = 0;
		pData = NULL;
	}
} BUFFITEM, *PBUFFITEM;

// 数据缓存队列结构
typedef struct buff_queue_tag
{
	int					nMaxBuffSize;	// 数据缓存最大长度
	int					nMax;			// 队列最大元素个数
	int					nCnt;			// 当前队列元素个数
	int					nWP;			///< Position of the writing slot in the queue
	int					nRP;			///< Position of the reading slot in the queue
	BUFFITEM			Que[1];			///< Array of buffer item
    buff_queue_tag()
    {
        nMaxBuffSize = 0;
        nMax         = 0;
        nCnt         = 0;
        nWP          = 0;
        nRP          = 0;
        memset(Que,0x00,sizeof(BUFFITEM)*1);
    }
} BUFF_QUEUE, *PBUFF_QUEUE;

// 工作空间结构
typedef struct htdl_work_tag
{
	HANDLE hThread;						// 工作空间线程句柄
	HANDLE hSync;						// 协议线程启动同步事件
	HANDLE hClose;						// 协议线程关闭事件
	BYTE ComSpeed;						// 波特率分组(0:小于19200;1:大于19200)
	WORD Phase;							// 通信阶段
	WORD Event;							// 通信事件

    //--------------------Receiving Buffer--------------------
	WORD RecvCnt;						// 接收的数据长度 Number of received byte
	BYTE *RecvPtr;						// Writing position of the buffer
	BYTE RecvBuff[MAX_RECVBUFF];		// 数据接收缓存 Temporary buffer storing receiving frame
	BYTE c;								// 最后接收的字节
	WORD wNeedRecvCnt;					// 协议中需要接收的字节
	//BYTE SeqRecv;						// 接收序列号
	//BYTE LastSeqRecv;					// 上一次的接收序列号
	//BYTE BlockNumRecv;					// 接收块序号 0x00:无分割块 0x80:第一块 0x81:第二块 ....0x0N:最后一块
	//BYTE LastBlockNumRecv;				// Last Block Number of receiving
    //--------------------------------------------------------
    
	//--------------------Sending Buffer----------------------
	BYTE SendBuff[MAX_SENDBUFF];		// 发送数据缓存(一个数据块)
	WORD SendLength;					// 发送数据的长度
	//BYTE SeqSend;						// 发送序列号(0x01-0xFF)
	//BYTE BlockNumSend;					// 发送块序号 0x00:无分割块 0x80:第一块 0x81:第二块 ....0x0N:最后一块
	//--------------------------------------------------------

	bool ReqSend;						// 
	bool Dle;							//
	
	int RetryCnt;						// 重试次数

	HANDLE				hFinSend;		// 命令发送完成事件
	HANDLE				hFrameSend;		// UNUSED

	HANDLE				hMutex;			// 工作空间操作同步信号量
	HANDLE				hComm;			// Handle of the opened port

	DWORD				DrvStat;		// 工作空间的状态 Status of the workspace (driver)
	
	SENDITEM			SndItem;		// The item is going to be sent (get from queue)
	SEND_QUEUE*			pSndQue;		// 发送数据队列
	RECVITEM			RcvItem;		// The item is currently storing receiving 
	RECV_QUEUE*			pRcvQue;		// 接收数据队列

	BUFF_QUEUE*			pBuffQue;		//

	CHTRS_RESULT		nError;			//

	BOOL	bNewENQ;					// It's just received ENQ
	DWORD BaseTime;						//
	DWORD TimeOut;						//
	
	BYTE	bCommunicationMethod;		// 通信方式 0:串口 1:邮槽 2:管道 
	BYTE	SendSlotName[MAX_PATH];		//
	BYTE	ReceiveSlotName[MAX_PATH];	//
	HANDLE	hSimObject;						//
	BYTE	MailBuff[MAX_RECVBUFF];		//
    htdl_work_tag()
    {
        memset(SendBuff,0x00,MAX_SENDBUFF);		
        SendLength = 0;					
// 		SeqSend = 0;						
// 		BlockNumSend = 0;					
        ReqSend = false;					
        Dle = false;						
        RetryCnt = 0;						
        hFinSend = NULL;		
        hFrameSend = NULL;		
        hMutex = NULL;			
        hComm = NULL;			
        DrvStat = 0;		
        pSndQue = NULL;	
        pRcvQue = NULL;		
        pBuffQue = NULL;		
        nError = 0;		
        bNewENQ = false;					
        BaseTime = 0;						
        TimeOut = 0;						
        bCommunicationMethod = COM_PORT;
        memset(SendSlotName,0x00,MAX_PATH);
        memset(ReceiveSlotName,0x00,MAX_PATH);
        hSimObject = NULL;
        memset(MailBuff,0x00,MAX_RECVBUFF);
    }
}HTDLWORK, *PHTDLWORK;

// 通信阶段控制处理结构
struct matrix_tag 
{
	int phase;										// 通信阶段
	int event;										// 通信事件												
	int (*func)(HTDLWORK *, struct matrix_tag *);	// 事件处理函数
	int next[2];									// 下一个通信阶段 { FAIL, SUCCESS }
};

// 恢复对齐状态
#pragma pack(pop)  

// 自定义通信阶段控制处理结构类型
typedef struct matrix_tag TMATRIX;		

// 通信阶段控制处理结构数组定义
extern TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT];

// MATRIX of monitor time
extern DWORD wiTimeOut[2][MAX_PHASE];			

extern DWORD WINAPI HtdlThread(void* b);

// 从串口或邮槽读取数据
extern BOOL ReadHtdl(HANDLE hStream,BYTE *pBuffer,DWORD dwLength,DWORD *dwBytesRead,BYTE bCommunicationMethod,HANDLE hSimObject,BYTE *pMailBuffer);

// 更新服务状态
extern void UpdateStatus(HTDLWORK *pWork, DWORD dwStatus, BOOL bOn);

// 接收一个数据块
extern BOOL RecvData(IN HTDLWORK *pWork, IN BYTE *data, IN int cnt);

