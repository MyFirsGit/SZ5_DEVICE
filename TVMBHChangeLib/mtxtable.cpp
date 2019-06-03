#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // 发送命令
extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送命令，三次重试
extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // 发送命令，三次重试超时
extern int mtxAcceptAck(HTDLWORK *pWork, TMATRIX *pMatrix);          // 接收ACK
extern int mtxEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);            // 发送ENQ
extern int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);       // 发送ENQ，重试3次
extern int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);     // 发送ENQ，重试3次超时
//extern int mtxRecvFrameSeq(HTDLWORK *pWork, TMATRIX *pMatrix);       // 接收R-SEQ
//extern int mtxRecvBlockNo(HTDLWORK *pWork, TMATRIX *pMatrix);        // 接收R-BLK
extern int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix);             // 清除接收缓存
extern int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix);			 // 接收一个字节
extern int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix);		 // 接收超时处理
extern int mtxCheckBcc(HTDLWORK *pWork, TMATRIX *pMatrix);           // BCC验证处理
extern int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix);              // 虚拟处理
extern int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix);            // 通信协议错误处理
//extern int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);		
extern int mtxSendACK(HTDLWORK *pWork, TMATRIX *pMatrix);
extern int mtxSendNAK(HTDLWORK *pWork, TMATRIX *pMatrix);
extern int mtxRecvWrong(HTDLWORK *pWork, TMATRIX *pMatrix);
extern int mtxCheckCRC(HTDLWORK *pWork, TMATRIX *pMatrix);
extern int mtxRecvContext(HTDLWORK *pWork, TMATRIX *pMatrix);
extern int mtxRecvDataLen(HTDLWORK *pWork, TMATRIX *pMatrix);


///***************************************************************************
/// HTDL Time monitor
///***************************************************************************
DWORD wiTimeOut[2][MAX_PHASE] =
{
	{								///< 9600 BPS
		0,							// 空闲，等待接收ENQ或等待发送ENQ
		5000,						// 等待接收DLE（ACK、NAK）
		5000,						// 等待接收DLE(STX)
		5000,						// 等待接收Length
		5000,						// 等待接收Data
		5000,						//.等待接收DLE（ETX）
		5000,						// 等待接收CRC（并校验）
		20000,						// 发送完毕等待接收DLE（ACK、NAK）
	},
	{								///< 19200 BPS
		0,							// 空闲，等待接收ENQ或等待发送ENQ
		0,							// 等待接收DLE（ACK、NAK）
		0,							// 等待接收DLE(STX)
		0,							// 等待接收Length
		0,							// 等待接收Data
		0,							//.等待接收DLE（ETX）
		0,							// 等待接收CRC（并校验）
		0,							// 发送完毕等待接收DLE（ACK、NAK）
	}
};

//
// HTDL MATRIX
//
TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT] = {
	//--------------------------------------------------------------------------*
	//   Phase 1, 空闲，等待接收ENQ或等待发送ENQ
	//--------------------------------------------------------------------------*
	{
		// Phase 1.1  发送ENQ
		{
			PHASE_S1,
			EVENT_REQSEND,
			mtxEnquire,				// 发送ENQ
			{PHASE_S1,PHASE_S2}		// 等待DLE（ACK、NAK）
		},

		// Phase 1.2  收到DLE
		{
			PHASE_S1,
			EVENT_DLE,
			mtxDummy,				// 不处理
			{PHASE_S1,PHASE_S1}		// 准备接收ENQ
		},

		// Phase 1.3  收到ENQ
		{
			PHASE_S1,
			EVENT_ENQ,
			mtxSendACK,				// 发送ACK
			{PHASE_S1,PHASE_S3}		// 等待接收DLE(STX)
		},

		// Phase 1.4  收到STX
		{
			PHASE_S1,
			EVENT_STX,
			mtxDummy,				// 不处理
			{PHASE_S1,PHASE_S1}		// 空闲
		},

		// Phase 1.5  收到ETX
		{
			PHASE_S1,
			EVENT_ETX,
			mtxDummy,				// 不处理
			{PHASE_S1,PHASE_S1}		// 空闲
		},

		// Phase 1.6  收到ACK
		{
			PHASE_S1,
			EVENT_ACK,
			mtxDummy,				// 不处理
			{PHASE_S1,PHASE_S1}		// 空闲
		},

		// Phase 1.7  收到NAK
		{
			PHASE_S1,
			EVENT_NAK,
			mtxDummy,				// 不处理
			{PHASE_S1,PHASE_S1}		// 空闲
		},

		// Phase 1.8  收到数据
		{
			PHASE_S1,
			EVENT_CHR,
			mtxDummy,				// 不处理
			{PHASE_S1,PHASE_S1}		// 空闲
		},

		// Phase 1.9  超时
		{
			PHASE_S1,
			EVENT_TIMEOUT,
			mtxDummy,				// 不处理
			{PHASE_S1,PHASE_S1}		// 空闲
		}
	},
	
	//--------------------------------------------------------------------------*
	//   Phase 2, 等待接收DLE（ACK、NAK）
	//--------------------------------------------------------------------------*
	{
		// Phase 2.1  发送ENQ
		{
			PHASE_S2,
			EVENT_REQSEND,
			mtxDummy,				// 不处理
			{PHASE_S2,PHASE_S2}		// 等待ACK、NAK
		},

		// Phase 2.2  收到DLE
		{
			PHASE_S2,
			EVENT_DLE,
			mtxDummy,				// 不处理
			{PHASE_S2,PHASE_S2}		// 准备接收ACK、NAK
		},

		// Phase 2.3  收到ENQ
		{
			PHASE_S2,
			EVENT_ENQ,
			mtxSendACK,				// 此时接收到ENQ，先处理接收请求(有可能同时发出ENQ，主机让步)
			{PHASE_S2,PHASE_S3}		// 等待接收DLE(STX)
		},

		// Phase 2.4  收到STX
		{
			PHASE_S2,
			EVENT_STX,
			mtxDummy,				// 不处理
			{PHASE_S2,PHASE_S2}		// 继续等待ACK、NAK
		},

		// Phase 2.5  收到ETX
		{
			PHASE_S2,
			EVENT_ETX,
			mtxDummy,				// 不处理
			{PHASE_S2,PHASE_S2}		// 继续等待ACK、NAK
		},

		// Phase 2.6  收到ACK
		{
			PHASE_S2,
			EVENT_ACK,
			mtxSend,				// 等到了ACK，就发送数据
			{PHASE_S1,PHASE_S8}		// 如果发送完了进入空闲状态，否则等待ACK
		},

		// Phase 2.7  收到NAK
		{
			PHASE_S2,
			EVENT_NAK,
			mtxRetryEnquire,		// 重新请求发送
			{PHASE_S1,PHASE_S2}		// 连续出错，进入空闲状态
		},

		// Phase 2.8  收到数据
		{
			PHASE_S2,
			EVENT_CHR,
			mtxDummy,				// 不处理
			{PHASE_S2,PHASE_S2}		// 继续等待ACK、NAK
		},

		// Phase 2.9  超时
		{
			PHASE_S2,
			EVENT_TIMEOUT,
			mtxTimeOutEnquire,		// 超时重新请求
			{PHASE_S1,PHASE_S2}		// 连续超时，进入空闲状态
		}
	},
	//--------------------------------------------------------------------------*
	//   Phase 3, 等待接收DLE(STX)
	//--------------------------------------------------------------------------*
	{
		// Phase 3.1  发送ENQ
		{
			PHASE_S3,
			EVENT_REQSEND,
			mtxDummy,				// 不处理
			{PHASE_S3,PHASE_S3}		// 继续等待接收DLE（STX）
		},

		// Phase 3.2  收到DLE
		{
			PHASE_S3,
			EVENT_DLE,
			mtxDummy,				// 不处理
			{PHASE_S3,PHASE_S3}		// 继续等待接收DLE（STX）
		},

		// Phase 3.3  收到ENQ
		{
			PHASE_S3,
			EVENT_ENQ,
			mtxSendACK,				// 发送ACK
			{PHASE_S3,PHASE_S3}		// 继续等待接收DLE（STX）
		},

		// Phase 3.4  收到STX
		{
			PHASE_S3,
			EVENT_STX,
			mtxReRecv,				// 清空接收缓冲
			{PHASE_S3,PHASE_S4}		// 转到接收长度的位置
		},

		// Phase 3.5  收到ETX
		{
			PHASE_S3,
			EVENT_ETX,
			mtxDummy,				// 不处理
			{PHASE_S3,PHASE_S1}		// 空闲
		},

		// Phase 3.6  收到ACK
		{
			PHASE_S3,
			EVENT_ACK,
			mtxDummy,				// 不处理
			{PHASE_S3,PHASE_S1}		// 空闲
		},

		// Phase 3.7  收到NAK
		{
			PHASE_S3,
			EVENT_NAK,
			mtxDummy,				// 不处理
			{PHASE_S3,PHASE_S1}		// 空闲
		},

		// Phase 3.8  收到数据
		{
			PHASE_S3,
			EVENT_CHR,
			mtxDummy,				// 不处理
			{PHASE_S3,PHASE_S3}		// 继续等待接收DLE（STX）
		},

		// Phase 3.9  超时
		{
			PHASE_S3,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// 超时处理
			{PHASE_S1,PHASE_S1}		// 空闲
		}
	},
	//--------------------------------------------------------------------------*
	//   Phase 4, 等待接收Length
	//--------------------------------------------------------------------------*
	{
		// Phase 4.1  发送ENQ
		{
			PHASE_S4,
			EVENT_REQSEND,
			mtxDummy,				// 不处理
			{PHASE_S4,PHASE_S4}		// 继续接收Length
		},

		// Phase 4.2  收到DLE
		{
			PHASE_S4,
			EVENT_DLE,
			mtxDummy,				// 不处理
			{PHASE_S4,PHASE_S4}		// 继续接收Length
		},

		// Phase 4.3  收到ENQ
		{
			PHASE_S4,
			EVENT_ENQ,
			mtxDummy,				// 不处理
			{PHASE_S4,PHASE_S4}		// 继续接收Length
		},

		// Phase 4.4  收到STX
		{
			PHASE_S4,
			EVENT_STX,
			mtxDummy,				// 不处理
			{PHASE_S4,PHASE_S4}		// 继续接收Length
		},

		// Phase 4.5  收到ETX
		{
			PHASE_S4,
			EVENT_ETX,
			mtxDummy,				// 不处理
			{PHASE_S4,PHASE_S4}		// 继续接收Length
		},

		// Phase 4.6  收到ACK
		{
			PHASE_S4,
			EVENT_ACK,
			mtxDummy,				// 不处理
			{PHASE_S4,PHASE_S4}		// 继续接收Length
		},

		// Phase 4.7  收到NAK
		{
			PHASE_S4,
			EVENT_NAK,
			mtxDummy,				// 不处理
			{PHASE_S4,PHASE_S4}		// 继续接收Length
		},

		// Phase 4.8  收到数据
		{
			PHASE_S4,
			EVENT_CHR,
			mtxRecvDataLen,			// 接收数据长度
			{PHASE_S4,PHASE_S5}		// 成功转到接收数据
		},

		// Phase 4.9  超时
		{
			PHASE_S4,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// 接收超时
			{PHASE_S1,PHASE_S3}		// 转到接收DLE（STX）
		}
	},
	//--------------------------------------------------------------------------*
	//   Phase 5, 等待接收Data
	//--------------------------------------------------------------------------*
	{
		// Phase 5.1  发送ENQ
		{
			PHASE_S5,
			EVENT_REQSEND,
			mtxDummy,				// 不处理
			{PHASE_S5,PHASE_S5}		// 继续接收数据
		},

		// Phase 5.2  收到DLE
		{
			PHASE_S5,
			EVENT_DLE,
			mtxDummy,				// 不处理
			{PHASE_S5,PHASE_S5}		// 继续接收数据
		},

		// Phase 5.3  收到ENQ
		{
			PHASE_S5,
			EVENT_ENQ,
			mtxDummy,				// 不处理
			{PHASE_S5,PHASE_S5}		// 继续接收数据
		},

		// Phase 5.4  收到STX
		{
			PHASE_S5,
			EVENT_STX,
			mtxDummy,				// 不处理
			{PHASE_S5,PHASE_S5}		// 继续接收数据
		},

		// Phase 5.5  收到ETX
		{
			PHASE_S5,
			EVENT_ETX,
			mtxDummy,				// 不处理
			{PHASE_S5,PHASE_S5}		// 继续接收数据
		},

		// Phase 5.6  收到ACK
		{
			PHASE_S5,
			EVENT_ACK,
			mtxDummy,				// 不处理
			{PHASE_S5,PHASE_S5}		// 继续接收数据
		},

		// Phase 5.7  收到NAK
		{
			PHASE_S5,
			EVENT_NAK,
			mtxDummy,				// 不处理
			{PHASE_S5,PHASE_S5}		// 继续接收数据
		},

		// Phase 5.8  收到数据
		{
			PHASE_S5,
			EVENT_CHR,
			mtxRecvContext,			// 接收数据（One by one）
			{PHASE_S5,PHASE_S6}		// 数据接收完成跳转到PHASE 6，否则继续接收
		},

		// Phase 5.9  超时
		{
			PHASE_S5,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// 超时处理
			{PHASE_S1,PHASE_S3}		// 转到S3，继续等待DLE（STX）
		}
	},
	//--------------------------------------------------------------------------*
	//   Phase 6, 等待接收DLE（ETX）
	//--------------------------------------------------------------------------*
	{
		// Phase 6.1  发送ENQ
		{
			PHASE_S6,
			EVENT_REQSEND,
			mtxDummy,				// 不处理
			{PHASE_S6,PHASE_S6}		// 继续等待DLE（ETX）
		},

		// Phase 6.2  收到DLE
		{
			PHASE_S6,
			EVENT_DLE,
			mtxOnChar,				// 加入接收队列
			{PHASE_S6,PHASE_S6}		// 准备接收（ETX）
		},

		// Phase 6.3  收到ENQ
		{
			PHASE_S6,
			EVENT_ENQ,
			mtxDummy,				// 不处理
			{PHASE_S6,PHASE_S6}		// 继续等待DLE（ETX）
		},

		// Phase 6.4  收到STX
		{
			PHASE_S6,
			EVENT_STX,
			mtxReRecv,				// 重新接收
			{PHASE_S6,PHASE_S4}		// 转到接收长度
		},

		// Phase 6.5  收到ETX
		{
			PHASE_S6,
			EVENT_ETX,
			mtxOnChar,				// 接入接收队列，便于计算CRC
			{PHASE_S6,PHASE_S7}		// 传到S7，准备接收CRC
		},

		// Phase 6.6  收到ACK
		{
			PHASE_S6,
			EVENT_ACK,
			mtxDummy,				// 不处理
			{PHASE_S6,PHASE_S6}		// 继续等待DLE（ETX）
		},

		// Phase 6.7  收到NAK
		{
			PHASE_S6,
			EVENT_NAK,
			mtxDummy,				// 不处理
			{PHASE_S6,PHASE_S6}		// 继续等待DLE（ETX）
		},

		// Phase 6.8  收到数据
		{
			PHASE_S6,
			EVENT_CHR,
			mtxDummy,				// 不处理
			{PHASE_S6,PHASE_S6}		// 继续等待DLE（ETX）
		},

		// Phase 6.9  超时
		{
			PHASE_S6,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// 超时处理
			{PHASE_S1,PHASE_S3}		// 重新等待接收
		}
	},

	//--------------------------------------------------------------------------*
	//   Phase 7, 等待接收CRC（并校验）
	//--------------------------------------------------------------------------*
	{
		// Phase 7.1  发送ENQ
		{
			PHASE_S7,
			EVENT_REQSEND,
			mtxDummy,				// 不处理
			{PHASE_S7,PHASE_S7}		// 继续等待CRC
		},

		// Phase 7.2  收到DLE
		{
			PHASE_S7,
			EVENT_DLE,
			mtxDummy,				// 不处理
			{PHASE_S7,PHASE_S7}		// 继续等待CRC
		},

		// Phase 7.3  收到ENQ
		{
			PHASE_S7,
			EVENT_ENQ,
			mtxDummy,				// 不处理
			{PHASE_S7,PHASE_S7}		// 继续等待CRC
		},

		// Phase 7.4  收到STX
		{
			PHASE_S7,
			EVENT_STX,
			mtxDummy,				// 不处理
			{PHASE_S7,PHASE_S7}		// 继续等待CRC
		},

		// Phase 7.5  收到ETX
		{
			PHASE_S7,
			EVENT_ETX,
			mtxDummy,				// 不处理
			{PHASE_S7,PHASE_S7}		// 继续等待CRC
		},

		// Phase 7.6  收到ACK
		{
			PHASE_S7,
			EVENT_ACK,
			mtxDummy,				// 不处理
			{PHASE_S7,PHASE_S7}		// 继续等待CRC
		},

		// Phase 7.7  收到NAK
		{
			PHASE_S7,
			EVENT_NAK,
			mtxDummy,				// 不处理
			{PHASE_S7,PHASE_S7}		// 继续等待CRC
		},

		// Phase 7.8  收到数据
		{
			PHASE_S7,
			EVENT_CHR,
			mtxCheckCRC,			// CRC校验
			{PHASE_S7,PHASE_S1}		// 继续接收并校验，返回空闲
		},

		// Phase 7.9  超时
		{
			PHASE_S7,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// 超时处理
			{PHASE_S1,PHASE_S1}		// 空闲
		}
	},

	//--------------------------------------------------------------------------*
	//   Phase 8, 发送完毕等待接收DLE（ACK、NAK）
	//--------------------------------------------------------------------------*
	{
		// Phase 8.1  发送ENQ
		{
			PHASE_S8,
			EVENT_REQSEND,
			mtxDummy,					// 不处理
			{PHASE_S8,PHASE_S8}			// 继续等待DLE（ACK、NAK）
		},

		// Phase 8.2  收到DLE
		{
			PHASE_S8,
			EVENT_DLE,
			mtxDummy,					// 不处理
			{PHASE_S8,PHASE_S8}			// 继续等待DLE(ACK、NAK)
		},

		// Phase 8.3  收到ENQ
		{
			PHASE_S8,
			EVENT_ENQ,
			mtxDummy,					// 不处理
			{PHASE_S1,PHASE_S1}			// 继续等待DLE（ACK、NAK）
		},

		// Phase 8.4  收到STX
		{
			PHASE_S8,
			EVENT_STX,
			mtxDummy,					// 不处理
			{PHASE_S8,PHASE_S8}			// 继续等待
		},

		// Phase 8.5  收到ETX
		{
			PHASE_S8,
			EVENT_ETX,
			mtxDummy,					// 不处理
			{PHASE_S8,PHASE_S8}			// 继续等待
		},

		// Phase 8.6  收到ACK
		{
			PHASE_S8,
			EVENT_ACK,
			mtxSend,					// 检测是否完成数据的发送
			{PHASE_S1,PHASE_S8}			// 发送完成进入空闲
		},

		// Phase 8.7  收到NAK
		{
			PHASE_S8,
			EVENT_NAK,
			mtxRetrySend,				// 重发
			{PHASE_S1,PHASE_S8}			// 空闲或等待ACK
		},

		// Phase 8.8  收到数据
		{
			PHASE_S8,
			EVENT_CHR,
			mtxDummy,					// 不处理
			{PHASE_S8,PHASE_S8}			// 继续等待
		},

		// Phase 8.9  超时
		{
			PHASE_S8,
			EVENT_TIMEOUT,
			mtxRetrySend,				// 重试一次
			{PHASE_S1,PHASE_S8}			// 空闲或再次发送
		}
	}
};

//--------------------------------------------------------------------------*
// end of file