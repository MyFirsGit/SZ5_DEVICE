#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // 发送命令
//extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送命令，三次重试
//extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // 发送命令，三次重试超时
//extern int mtxAcceptEnq(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送ACK
//extern int mtxEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);            // 发送ENQ
//extern int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);       // 发送ENQ，重试3次
//extern int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);     // 发送ENQ，重试3次超时
//extern int mtxRecvFrameSeq(HTDLWORK *pWork, TMATRIX *pMatrix);       // 接收R-SEQ
//extern int mtxRecvBlockNo(HTDLWORK *pWork, TMATRIX *pMatrix);        // 接收R-BLK
extern int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix);             // 清除接收缓存
extern int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix);			 // 接收一个字节
extern int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix);		 // 接收超时处理
//extern int mtxCheckBcc(HTDLWORK *pWork, TMATRIX *pMatrix);           // BCC验证处理
extern int mtxRecvComplete(HTDLWORK *pWork, TMATRIX *pMatrix);    // 接收完成处理
extern int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix);              // 虚拟处理
extern int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix);            // 通信协议错误处理
extern int mtxRecvLenth(HTDLWORK *pWork, TMATRIX *pMatrix);          // 收到长度处理



///***************************************************************************
/// HTDL Time monitor
///***************************************************************************
DWORD wiTimeOut[2][MAX_PHASE] =
{
	// 9600 BPS
	{								
		0,								// S1 等待STX  wait for STX
		0,								// S2 等待LEN  wait for LEN
		30000,							// S3 等待ETX wait for ETX
	},
	// 19200 BPS
	{								
		0,								// S1 等待STX  wait for STX
		0,								// S2 等待LEN  wait for LEN
		6000,							// S3 等待 ETX wait for ETX
	}
};

//
// HTDL MATRIX
//
TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT] = {

//--------------------------------------------------------------------------*
//  Phase S1: 等待STX状态
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S1,
			EVENT_REQSEND,					// 发送数据
			mtxSend,	 					// Send command
			{ PHASE_S1, PHASE_S1 }		    // 等待STX
		},
		{
			PHASE_S1,
			EVENT_STX,						// 收到STX
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S2 }			// 等待ENQ
		},
		{
			PHASE_S1,
			EVENT_CHR,						// 收到数据
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }			// 等待ENQ
		},
		{
			PHASE_S1,
			EVENT_ETX,							// 收到ETX
			mtxDummy,							// 不处理
			{ PHASE_S1, PHASE_S1 }		// 等待ENQ
		},
		{
			PHASE_S1,
			EVENT_TIMEOUT,					// 处理超时
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }			// 等待ENQ
		},
	},

	//--------------------------------------------------------------------------*
	//  Phase S2: 等待LEN状态
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S2,
			EVENT_REQSEND,						// 发送数据
			mtxDummy,								// 不处理
			{ PHASE_S2, PHASE_S2 }			// 等待LEN
		},
		{
			PHASE_S2,
			EVENT_STX,								// 收到STX
			mtxReRecv,								// 清除接收缓存
			{ PHASE_S2, PHASE_S2 }			// 等待SEQ
		},
		{
			PHASE_S2,
			EVENT_CHR,								// 收到MSG
			mtxRecvLenth,						// 保存长度
			{ PHASE_S3, PHASE_S3 }			// 等待ETX
		},
		{
			PHASE_S2,
			EVENT_ETX,								// 收到ETX
			mtxDummy,								// 不处理
			{ PHASE_S1, PHASE_S1 }			// 等待STX
		},
		{
			PHASE_S2,
			EVENT_TIMEOUT,					// 处理超时
			mtxTimeOutRecv,					// 超时处理
			{ PHASE_S1, PHASE_S1 }		// 等待ENQ
		}
		},

//--------------------------------------------------------------------------*
//  Phase S3: 等待ETX状态
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S3,
			EVENT_REQSEND,						// 发送数据
			mtxDummy,							// 不处理
			{ PHASE_S3, PHASE_S3 }				// 等待ETX
		},
		{
			PHASE_S3,
			EVENT_STX,							// 收到STX
			mtxOnChar,							// 处理接收数据
			{ PHASE_S3, PHASE_S3 }				// 继续等待ETX
		},
		{
			PHASE_S3,
			EVENT_CHR,							// 收到MSG
			mtxOnChar,							// 不处理
			{ PHASE_S3, PHASE_S3 }				// 等待ETX
		},
		{
			PHASE_S3,
			EVENT_ETX,							// 收到ETX
			mtxRecvComplete,					// 接收完成处理
			{ PHASE_S1, PHASE_S3 }				// 成功：等待STX 失败：等待ETX 
		},
		{
			PHASE_S1,
			EVENT_TIMEOUT,					// 处理超时
			mtxTimeOutRecv,					// 超时处理
			{ PHASE_S1, PHASE_S1 }			// 等待STX
		}
	}
};

//--------------------------------------------------------------------------*
// end of file