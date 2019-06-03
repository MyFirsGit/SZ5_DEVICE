#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // 发送命令
extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送命令，三次重试
extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // 发送命令，三次重试超时
extern int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix);             // 清除接收缓存
extern int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix);			 // 接收一个字节
extern int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix);		 // 接收超时处理
extern int mtxCalcCrc16(HTDLWORK *pWork, TMATRIX *pMatrix);          // CRC16验证处理
extern int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix);              // 虚拟处理
extern int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix);            // 通信协议错误处理


///***************************************************************************
/// HTDL Time monitor
///***************************************************************************
DWORD wiTimeOut[2][MAX_PHASE] =
{
	{								///< 9600 BPS
		0,								///< Phase 1, 空闲，等待命令发送事件
		5000,						///< Phase 2, 等待STX
		128000						///< Phase 3, 等待ETX
	},
	{								///< 19200 BPS
		0,								///< Phase 1, 空闲，等待命令发送事件
		10000,						///< Phase 2, 等待STX
		128000						///< Phase 3, 等待ETX
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
			{ PHASE_S1, PHASE_S1 }		// 等待ACK
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
			EVENT_TIMEOUT,					// 超时
			mtxDummy,							// 不处理
			{ PHASE_S1, PHASE_S1 }		// 等待命令发送
		}
	},

	//--------------------------------------------------------------------------*
	//  Phase 2, 命令发送完毕，等待接受STX
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
			{ PHASE_S2, PHASE_S3 }	// 等待ACK/NAK
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
			PHASE_S1,
			EVENT_TIMEOUT,				// 超时
			mtxTimeOutSend,				// 重发
			{ PHASE_S2, PHASE_S2 }	// 等待ACK/NAK
		}
	},
//--------------------------------------------------------------------------*
//  Phase 3, 前面已经收到STX ,遇到一个STX之后就表示可以接收数据了
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S3,
			EVENT_REQSEND,					// 发送数据
			mtxDummy,							// 不处理
			{ PHASE_S3, PHASE_S3 }		// 等待STX
		},
		{
			PHASE_S3,
			EVENT_STX,						// 收到STX
			mtxDummy,						// 不处理
			{ PHASE_S3, PHASE_S3 }	// 等待DLE-ETX
		},
		{
			PHASE_S3,
			EVENT_CHR,						// 收到MSG
			mtxOnChar,						// 不处理
			{ PHASE_S3, PHASE_S3 }	// 等待STX
		},
		{
			PHASE_S3,
			EVENT_ETX,						// 收到ETX
			mtxCalcCrc16,					// 不处理
			{ PHASE_S1, PHASE_S1 }	// 等待STX
		},
		{
			PHASE_S3,
			EVENT_TIMEOUT,					// 处理超时
			mtxDummy,					// 重发ENQ
			{ PHASE_S3, PHASE_S3 }		// 等待STX
		}
	}
};

//--------------------------------------------------------------------------*
// end of file