#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // 发送命令
extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送命令，三次重试
extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // 发送命令，三次重试超时
extern int mtxAcceptAck(HTDLWORK *pWork, TMATRIX *pMatrix);          // 接收ACK
//extern int mtxEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);            // 发送ENQ
//extern int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);       // 发送ENQ，重试3次
//extern int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);     // 发送ENQ，重试3次超时
//extern int mtxRecvFrameSeq(HTDLWORK *pWork, TMATRIX *pMatrix);       // 接收R-SEQ
//extern int mtxRecvBlockNo(HTDLWORK *pWork, TMATRIX *pMatrix);        // 接收R-BLK
extern int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix);             // 清除接收缓存
extern int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix);			 // 接收一个字节
extern int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix);		 // 接收超时处理
extern int mtxCheckBcc(HTDLWORK *pWork, TMATRIX *pMatrix);           // BCC验证处理
extern int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix);              // 虚拟处理
extern int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix);            // 通信协议错误处理
extern int checkSendTimeout(HTDLWORK *pWork, TMATRIX *pMatrix);		 // 检查发送超时时的状态
extern int checkSendPaddingCmd(HTDLWORK *pWork, TMATRIX *pMatrix);	 // 检查发送过程中进入忙碌状态

///***************************************************************************
/// HTDL Time monitor
///***************************************************************************
DWORD wiTimeOut[2][MAX_PHASE] =
{
	{							///< 9600 BPS
		10000,					///< Phase 1, 空闲，等待命令发送事件/检查发送是否完成
		3000,					///< Phase 2, 命令发送完毕，等待ACK / NAK
		20000,					///< Phase 3, 等待空闲应答码ON
		20000,					///< Phase 4, 有应答命令，等待STX
		3000,					///< Phase 5, 等待ETX
		2000,					///< Phase 6, 等待BCC
	},
	{							///< 19200 BPS
		10000,					///< Phase 1, 空闲，等待命令发送事件/检查发送是否完成
		0,						///< Phase 2, 命令发送完毕，等待ACK / NAK
		0,						///< Phase 3, 等待空闲应答码ON
		0,						///< Phase 4, 有应答命令，等待STX
		0,						///< Phase 5, 等待ETX
		0,						///< Phase 6, 等待BCC
	}
};

//
// HTDL MATRIX
//
TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT] = {

	//--------------------------------------------------------------------------*
	//  Phase 1, 空闲，等待命令发送事件
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S1,
			EVENT_REQSEND,					// 发送请求
			mtxSend,								// 发送命令
			{ PHASE_S1, PHASE_S2 }		// 等待ACK
		},
		{
			PHASE_S1,
			EVENT_STX,						// 收到STX
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }	// 等待命令发送
		},
		{
			PHASE_S1,
			EVENT_CHR,						// 收到数据
			mtxDummy,	 					// 不处理
			{ PHASE_S1, PHASE_S1 }	// 等待命令发送
		},
		{
			PHASE_S1,
			EVENT_ETX,						// 收到ETX
			mtxDummy,	 					// 不处理
			{ PHASE_S1, PHASE_S1 }	// 等待命令发送
		},
		{
			PHASE_S1,
			EVENT_ACK,						// 收到ACK
			mtxDummy,	 					// 不处理
			{ PHASE_S1, PHASE_S1 }	// 等待命令发送
		},
		{
			PHASE_S1,
			EVENT_NAK,						// 收到NAK
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }	// 等待ACK
		},
		{
			PHASE_S1,
			EVENT_ON,						// 收到ON
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }	// 等待ACK
		},
		{
			PHASE_S1,
			EVENT_OFF,						// 收到OFF
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }	// 等待ACK
		},
		{
			PHASE_S1,
			EVENT_TIMEOUT,					// 超时
			checkSendTimeout,				// 处理发送超时
			{ PHASE_S1, PHASE_S1 }		// 等待命令发送
		}
	},

	//--------------------------------------------------------------------------*
	//  Phase 2, 命令发送完毕，等待ACK / NAK
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S2,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,							// 不处理
			{ PHASE_S2, PHASE_S2 }		// 等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_STX,						// 收到STX
			mtxDummy,						// 不处理
			{ PHASE_S2, PHASE_S2 }	// 等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_CHR,						// 收到数据
			mtxDummy,	 					// 不处理
			{ PHASE_S2, PHASE_S2 }	// 等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_ETX,						// 收到ETX
			mtxDummy,	 					// 不处理
			{ PHASE_S2, PHASE_S2 }	// 等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_ACK,						// 收到ACK
			mtxAcceptAck,	 				// 接收ACK
			{ PHASE_S4, PHASE_S1 }	// 需要反馈则等待STX，不需反馈到空闲
		},
		{
			PHASE_S2,
			EVENT_NAK,						// 收到NCK
			mtxRetrySend,					// 重发
			{ PHASE_S1, PHASE_S2 }	// 等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_ON,						// 收到ON
			mtxDummy,						// 不处理
			{ PHASE_S2, PHASE_S2 }	// 等待ACK
		},
		{
			PHASE_S2,
			EVENT_OFF,						// 收到OFF
			mtxDummy,						// 不处理
			{ PHASE_S2, PHASE_S3 }	// 等待空闲应答码ON
		},
		{
			PHASE_S2,
			EVENT_TIMEOUT,				// 超时
			mtxTimeOutSend,				// 重发
			{ PHASE_S1, PHASE_S2 }	// 等待ACK/NAK
		}
	},

	//--------------------------------------------------------------------------*
	//  Phase 3, 等待空闲应答码ON
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S3,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,							// 不处理
			{ PHASE_S3, PHASE_S3 }		// 等待空闲ON
		},
		{
			PHASE_S3,
			EVENT_STX,						// 收到STX
			mtxDummy,						// 不处理
			{ PHASE_S3, PHASE_S3 }	// 等待空闲ON
		},
		{
			PHASE_S3,
			EVENT_CHR,						// 收到数据
			mtxDummy,	 					// 不处理
			{ PHASE_S3, PHASE_S3 }	//  等待空闲ON
		},
		{
			PHASE_S3,
			EVENT_ETX,						// 收到ETX
			mtxDummy,	 					// 不处理
			{ PHASE_S3, PHASE_S3 }	// 等待ACK/NAK
		},
		{
			PHASE_S3,
			EVENT_ACK,						// 收到ACK
			mtxDummy,	 					// 不处理
			{ PHASE_S3, PHASE_S3 }	// 需要反馈则等待STX，不需反馈到空闲
		},
		{
			PHASE_S3,
			EVENT_NAK,						// 收到NAK
			mtxDummy,						// 不处理
			{ PHASE_S3, PHASE_S3 }	// 等待ACK/NAK
		},
		{
			PHASE_S3,
			EVENT_ON,						// 收到ON
			checkSendPaddingCmd,			// 直接转到空闲
			{ PHASE_S1, PHASE_S2 }	// 等待发送/接收ACK（系统忙碌结束/重新发送命令）
		},
		{
			PHASE_S3,
			EVENT_OFF,						// 收到OFF
			mtxDummy,						// 不处理
			{ PHASE_S3, PHASE_S3 }	// 等待空闲应答码ON
		},
		{
			PHASE_S3,
			EVENT_TIMEOUT,				// 超时
			mtxDummy,					// 直接转到空闲
			{ PHASE_S1, PHASE_S1 }	// 等待发送
		}
	},


//--------------------------------------------------------------------------*
//  Phase 4,有应答命令等待STX
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S4,
			EVENT_REQSEND,					// 发送数据
			mtxDummy,							// 不处理
			{ PHASE_S4, PHASE_S4 }		// 等待STX
		},
		{
			PHASE_S4,
			EVENT_STX,						// 收到STX
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S5 }	// 等待ETX
		},
		{
			PHASE_S4,
			EVENT_CHR,						// 收到MSG
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }	// 等待STX
		},
		{
			PHASE_S4,
			EVENT_ETX,						// 收到ETX
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }	// 等待STX
		},
		{
			PHASE_S4,
			EVENT_ACK,						// 收到ACK
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }	// 等待STX
		},
		{
			PHASE_S4,
			EVENT_NAK,						// 收到NAK
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }	// 等待STX
		},
		{
			PHASE_S4,
			EVENT_ON,						// 收到ON
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }	// 等待ACK
		},
		{
			PHASE_S4,
			EVENT_OFF,						// 收到OFF
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }	// 等待空闲应答码ON
		},
		{
			PHASE_S4,
			EVENT_TIMEOUT,					// 处理超时
			mtxTimeOutSend,					// 重试发送
			{ PHASE_S1, PHASE_S4 }		// 等待STX
		}
	},

//--------------------------------------------------------------------------*
//  Phase S5: 等待ETX
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S5,
			EVENT_REQSEND,					// 发送数据
			mtxDummy,							// 不处理
			{ PHASE_S5, PHASE_S5 }		// 等待ETX
		},
		{
			PHASE_S5,
			EVENT_STX,						// 收到STX
			mtxReRecv,						// 重新接收
			{ PHASE_S5, PHASE_S5 }	// 等待ETX
		},
		{
			PHASE_S5,
			EVENT_CHR,						// 收到数据
			mtxOnChar,		 				// 保存数据
			{ PHASE_S5, PHASE_S5 }	// 等待ETX
		},
		{
			PHASE_S5,
			EVENT_ETX,						// 收到ETX
			mtxDummy,						// 不处理
			{ PHASE_S5, PHASE_S6 }	// 等待BCC
		},
		{
			PHASE_S5,
			EVENT_ACK,						// 收到ACK
			mtxProtErr,						// 时序错误
			{ PHASE_S1, PHASE_S1 }	// 空闲
		},
		{
			PHASE_S5,
			EVENT_NAK,						// 收到NAK
			mtxProtErr,						// 时序错误
			{ PHASE_S1, PHASE_S1 }	// 空闲
		},
		{
			PHASE_S5,
			EVENT_ON,						// 收到ON
			mtxProtErr,						// 时序错误
			{ PHASE_S5, PHASE_S5 }	// 空闲
		},
		{
			PHASE_S5,
			EVENT_OFF,						// 收到OFF
			mtxProtErr,						// 时序错误
			{ PHASE_S5, PHASE_S5 }	// 空闲
		},
		{
			PHASE_S5,
			EVENT_TIMEOUT,					// 超时
			mtxTimeOutRecv,					// 接收超时处理
			{ PHASE_S1, PHASE_S1 }		// 空闲
		}
	},

//--------------------------------------------------------------------------*
//  Phase 6, 等待BCC
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S6,
			EVENT_REQSEND,					// 发送数据
			mtxDummy,							// 不处理
			{ PHASE_S6, PHASE_S6 }		// 等待BCC
		},
		{
			PHASE_S6,
			EVENT_STX,						// 收到STX
			mtxReRecv,						// 重新接收
			{ PHASE_S5, PHASE_S5 }	// 等待ETX
		},
		{
			PHASE_S6,
			EVENT_CHR,						// 收到数据
			mtxCheckBcc,	 				// 检查BCC
			{ PHASE_S4, PHASE_S1 }	// < S2 (FAIL or Wait next chunk) or S1 (Done)
		},
		{
			PHASE_S6,
			EVENT_ETX,						// 收到ETX
			mtxReRecv,						// 重新接收
			{ PHASE_S6, PHASE_S4 }	// 等待STX
		},
		{
			PHASE_S6,
			EVENT_ACK,						// 收到ACK
			mtxProtErr,						// 时序错误
			{ PHASE_S1, PHASE_S1 }	// 空闲
		},
		{
			PHASE_S6,
			EVENT_NAK,						// 收到ACK
			mtxProtErr,						// 时序错误
			{ PHASE_S1, PHASE_S1 }	//  空闲
		},
		{
			PHASE_S6,
			EVENT_TIMEOUT,					// 超时
			mtxTimeOutRecv,					// 超时处理
			{ PHASE_S1, PHASE_S1 }		// 空闲
		}
	}
};

//--------------------------------------------------------------------------*
// end of file