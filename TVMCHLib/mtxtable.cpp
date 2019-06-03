#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // 发送命令
extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送命令，三次重试
extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // 发送命令，三次重试超时
//extern int mtxAcceptEnq(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送ACK
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
//------------add by lichao
extern int mtxSndACK(HTDLWORK *pWork, TMATRIX *pMatrix);               // 发送命令
extern int mtxRetrySndACK(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送命令，三次重试
extern int mtxSndNAK(HTDLWORK *pWork, TMATRIX *pMatrix);               // 发送命令
extern int mtxRetrySndNAK(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送命令，三次重试
extern int mtxSndEOT(HTDLWORK *pWork, TMATRIX *pMatrix);               // 发送命令
extern int mtxRetrySndEOT(HTDLWORK *pWork, TMATRIX *pMatrix);          // 发送命令，三次重试
extern int mtxCheckDataA(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxCheckDataB(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxCheckDataC1(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxCheckDataC2(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxRecvContext(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxRecvBCC(HTDLWORK *pWork, TMATRIX *pMatrix);				// 
extern int mtxCheckBCC(HTDLWORK *pWork, TMATRIX *pMatrix);				// 

extern int mtxSndEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);            //发送查询块

///***************************************************************************
/// HTDL Time monitor
///***************************************************************************
#define T1 5000					// add by lichao
#define T3 2000
#define T4 8000

DWORD wiTimeOut[2][MAX_PHASE] =
{
	{							///< 38400 BPS
		T1,						///< Phase 1, 空闲，等待心跳超时
		T1,						///< Phase 2, 数据发送完毕，等待ACK / NAK
		T4,						///< Phase 3, ENQ发送完毕，等待ENQ应答
		T3,						///< Phase 4, 等待校验ENQ应答中的数据a（模块类别和对方局地址）
		T3,						///< Phase 5, 等待校验ENQ应答中的数据b（主局地址和传送次数）
		T3,						///< Phase 6, 等待校验ENQ应答中的数据c或c1（文本尺寸数据）
		T3,						///< Phase 7, 等待校验ENQ应答中的数据c2（文本尺寸数据）
		T1,						///< Phase 8, 等待接收ENQ应答中的文本数据
		T3,						///< Phase 9, 等待接收ENQ应答中的BCC的位1
		T1,						///< Phase 10, 等待接收ENQ应答中的BCC的位2，BCC确认
		T1,						///< Phase 11, ACK发送完毕，等待接收EOT
		T1,						///< Phase 12, NAK发送完毕，等待接收EOT
		T3,						///< Phase 13, 等待处理ENQ应答中的数据接收错误
		T1,						///< Phase 14, EOT发送完毕，等待接收EOT
	},
	{							///< 19200 BPS
		0,						///< Phase 1, 空闲，等待心跳超时
		0,						///< Phase 2, 数据发送完毕，等待ACK / NAK
		0,						///< Phase 3, ENQ发送完毕，等待ENQ应答
		0,						///< Phase 4, 等待校验ENQ应答中的数据a（模块类别和对方局地址）
		0,						///< Phase 5, 等待校验ENQ应答中的数据b（主局地址和传送次数）
		0,						///< Phase 6, 等待校验ENQ应答中的数据c或c1（文本尺寸数据）
		0,						///< Phase 7, 等待校验ENQ应答中的数据c2（文本尺寸数据）
		0,						///< Phase 8, 等待接收ENQ应答中的文本数据
		0,						///< Phase 9, 等待接收ENQ应答中的BCC的位1
		0,						///< Phase 10, 等待接收ENQ应答中的BCC的位2，BCC确认
		0,						///< Phase 11, ACK发送完毕，等待接收EOT
		0,						///< Phase 12, NAK发送完毕，等待接收EOT
		0,						///< Phase 13, 等待处理ENQ应答中的数据接收错误
		0,						///< Phase 14, EOT发送完毕，等待接收EOT
	}
};

#define	MAX_PHASE	14			// 通信状态数量
#define PHASE_S1		0				// Phase 1, 空闲,等待数据发送
#define PHASE_S2		1				// Phase 2, 数据发送完毕，等待ACK / NAK应答
#define PHASE_S3		2				// Phase 3, ENQ请求发送完毕，等待ENQ应答
#define PHASE_S4		3				// Phase 4, 等待校验ENQ应答中的数据a（模块类别和对方局地址）
#define PHASE_S5		4				// Phase 5, 等待校验ENQ应答中的数据b（主局地址和传送次数）
#define PHASE_S6		5				// Phase 6, 等待校验ENQ应答中的数据c或c1（文本尺寸数据）
#define PHASE_S7		6				// Phase 7, 等待校验ENQ应答中的数据c2（文本尺寸数据）
#define PHASE_S8		7				// Phase 8, 等待接收ENQ应答中的文本数据
#define PHASE_S9		8				// Phase 9, 等待接收ENQ应答中的BCC的位1
#define PHASE_S10		9				// Phase 10, 等待接收ENQ应答中的BCC的位2，BCC确认
#define PHASE_S11		10				// Phase 11, ACK发送完毕，等待接收EOT
#define PHASE_S12		11				// Phase 12, NAK发送后的再发送数据等待
#define PHASE_S13		12				// Phase 13, 等待处理ENQ应答中的数据接收错误
#define PHASE_S14		13				// Phase 14, EOT发送完毕，等待接收EOT
//
// HTDL MATRIX
//
TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT] = {

	//--------------------------------------------------------------------------*
	//  Phase 1, 空闲，等待数据发送或心跳超时
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S1,
			EVENT_REQSEND,					// 发送数据请求
			mtxSend,						// 发送数据
			{ PHASE_S1, PHASE_S2 }			// 等待ACK/NAK		
		},
		{
			PHASE_S1,
			EVENT_ENQ,						// 收到ENQ（查询应答或数据）
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }			// 等待数据发送或心跳超时
		},
		{
			PHASE_S1,
			EVENT_CHR,						// 收到数据
			mtxDummy,	 					// 不处理
			{ PHASE_S1, PHASE_S1 }			// 等待数据发送或心跳超时
		},
		{
			PHASE_S1,
			EVENT_ACK,						// 收到ACK
			mtxDummy,	 					// 不处理
			{ PHASE_S1, PHASE_S1 }			// 等待数据发送或心跳超时
		},
		{
			PHASE_S1,
			EVENT_NAK,						// 收到NAK
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }			// 等待数据发送或心跳超时
		},
		{
			PHASE_S1,
			EVENT_EOT,						// 收到EOT
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }			// 等待数据发送或心跳超时
		},
		{
			PHASE_S1,
			EVENT_TIMEOUT,					// 超时
			mtxDummy,				        // 不处理
			{ PHASE_S1, PHASE_S1 }			// 
		},
		{
			PHASE_S1,
			EVENT_QUERY,					// 心跳查询
			mtxRetryEnquire,		        // 发送心跳
			{ PHASE_S1, PHASE_S3 }			// 等待数据发送或查询后应答等待
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 2, 数据发送完毕，等待ACK / NAK应答
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S2,
			EVENT_REQSEND,					// 发送数据请求
			mtxDummy,						// 不处理
			{ PHASE_S2, PHASE_S2 }			// 等待ACK或NAK		
		},
		{
			PHASE_S2,
			EVENT_ENQ,						// 收到ENQ
			mtxRetrySend,					// 重发数据
			{ PHASE_S1, PHASE_S2 }			// 回到空闲或等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_CHR,						// 收到数据
			mtxRetrySend,					// 重发数据
			{ PHASE_S1, PHASE_S2 }			// 回到空闲或等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_ACK,						// 收到ACK
			mtxSndEOT,	 					// 发送EOT
			{ PHASE_S2, PHASE_S14 }			// EOT发送完毕，等待接收EOT
		},
		{
			PHASE_S2,
			EVENT_NAK,						// 收到NAK
			mtxRetrySend,					// 重发数据
			{ PHASE_S1, PHASE_S2 }			// 回到空闲或等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_EOT,						// 收到EOT
			mtxRetrySend,					// 重发数据
			{ PHASE_S1, PHASE_S2 }			// 回到空闲或等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_TIMEOUT,					// 超时（等待ACK/NAK)
			mtxRetrySend,					// 重发数据
			{ PHASE_S1, PHASE_S2 }			// 回到空闲或等待ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S2 }			// 回到空闲或等待ACK/NAK
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 3, 心跳（查询）请求发送完毕，等待ENQ应答
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S3,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S3, PHASE_S3 }			// 等待ENQ应答		
		},
		{
			PHASE_S3,
			EVENT_ENQ,						// 收到ENQ
			mtxDummy,						// 不做处理，直接进入等待接收ENQ中数据a的状态
			{ PHASE_S3, PHASE_S4 }			// 等待接收ENQ中数据a
		},
		{
			PHASE_S3,
			EVENT_CHR,						// 收到数据
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待ENQ应答
		},
		{
			PHASE_S3,
			EVENT_ACK,						// 收到ACK
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待ENQ应答
		},
		{
			PHASE_S3,
			EVENT_NAK,						// 收到NAK
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待ENQ应答
		},
		{
			PHASE_S3,
			EVENT_EOT,						// 收到EOT
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待ENQ应答
		},
		{
			PHASE_S3,
			EVENT_TIMEOUT,					// 超时（等待接收ENQ接收应答或等待ACK/NAK或等待接收EOT)
			mtxTimeOutEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待ENQ应答
		},
		{
			PHASE_S3,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S3, PHASE_S3 }			// 回到空闲或等待ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 4, 等待校验ENQ应答中的数据a（模块类别和对方局地址)
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S4,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }			// 等待校验ENQ应答中的数据a	
		},
		{
			PHASE_S4,
			EVENT_ENQ,						// 收到ENQ
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }			// 继续等待接收a数据
		},
		{
			PHASE_S4,
			EVENT_CHR,						// 收到数据
			mtxCheckDataA,	 				// 校验是否满足数据a的要求
			{ PHASE_S13, PHASE_S5 }			// 数据错误处理或等待校验ENQ应答中的数据b
		},
		{
			PHASE_S4,
			EVENT_ACK,						// 收到ACK
			mtxDummy,	 					// 不处理
			{ PHASE_S4, PHASE_S4 }			// 继续等待接收a数据
		},
		{
			PHASE_S4,
			EVENT_NAK,						// 收到NAK
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }			// 继续等待接收a数据
		},
		{
			PHASE_S4,
			EVENT_EOT,						// 收到EOT
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }			// 继续等待接收a数据
		},
		{
			PHASE_S4,
			EVENT_TIMEOUT,					// 超时（等待接收ENQ接收应答中的数据a)
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待接收ENQ应答
		},
		{
			PHASE_S4,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S4, PHASE_S4 }			// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 5, 等待校验ENQ应答中的数据b（主局地址和传送次数）
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S5,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S5, PHASE_S5 }			// 等待校验ENQ应答中的数据b	
		},
		{
			PHASE_S5,
			EVENT_ENQ,						// 收到ENQ
			mtxDummy,	 					// 不处理
			{ PHASE_S5, PHASE_S5 }			// 等待校验ENQ应答中的数据b
		},
		{
			PHASE_S5,
			EVENT_CHR,						// 收到数据
			mtxCheckDataB,	 				// 校验ENQ应答中的数据b
			{ PHASE_S13, PHASE_S6 }			// 数据错误处理或等待校验文本尺寸c/c1
		},
		{
			PHASE_S5,
			EVENT_ACK,						// 收到ACK
			mtxDummy,	 					// 不处理
			{ PHASE_S5, PHASE_S5 }			// 等待校验ENQ应答中的数据b
		},
		{
			PHASE_S5,
			EVENT_NAK,						// 收到NAK
			mtxDummy,	 					// 不处理
			{ PHASE_S5, PHASE_S5 }			// 等待校验ENQ应答中的数据b
		},
		{
			PHASE_S5,
			EVENT_EOT,						// 收到EOT
			mtxDummy,	 					// 不处理
			{ PHASE_S5, PHASE_S5 }			// 等待校验ENQ应答中的数据b 
		},
		{
			PHASE_S5,
			EVENT_TIMEOUT,					// 超时
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待接收ENQ应答
		},
		{
			PHASE_S5,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S5, PHASE_S5 }			// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 6, 等待校验ENQ应答中的数据c或c1（文本尺寸数据）
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S6,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S6, PHASE_S6 }			// 等待校验ENQ应答中的数据c或c1	
		},
		{
			PHASE_S6,
			EVENT_ENQ,						// 收到ENQ
			mtxCheckDataC1,	 				// ENQ应答中的文本尺寸数据c或c1确认
			{ PHASE_S8, PHASE_S13 }			// 等待校验ENQ应答中的文本尺寸数据c2或文本内容
		},
		{
			PHASE_S6,
			EVENT_CHR,						// 收到数据
			mtxCheckDataC1,	 				// ENQ应答中的文本尺寸数据c或c1确认
			{ PHASE_S8, PHASE_S13 }			// 等待校验ENQ应答中的文本尺寸数据c2或文本内容
		},
		{
			PHASE_S6,
			EVENT_ACK,						// 收到ACK
			mtxCheckDataC1,	 				// ENQ应答中的文本尺寸数据c或c1确认
			{ PHASE_S8, PHASE_S13 }			// 等待校验ENQ应答中的文本尺寸数据c2或文本内容
		},
		{
			PHASE_S6,
			EVENT_NAK,						// 收到NAK
			mtxCheckDataC1,	 				// ENQ应答中的文本尺寸数据c或c1确认
			{ PHASE_S8, PHASE_S13 }			// 等待校验ENQ应答中的文本尺寸数据c2或文本内容
		},
		{
			PHASE_S6,
			EVENT_EOT,						// 收到EOT
			mtxCheckDataC1,	 				// ENQ应答中的文本尺寸数据c或c1确认
			{ PHASE_S8, PHASE_S13 }			// 等待校验ENQ应答中的文本尺寸数据c2或文本内容
		},
		{
			PHASE_S6,
			EVENT_TIMEOUT,					// 超时
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待接收ENQ应答 
		},
		{
			PHASE_S6,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S6, PHASE_S6 }			// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 7, 等待校验ENQ应答中的数据c2（文本尺寸数据）
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S7,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S7, PHASE_S7 }			// 等待校验ENQ应答中的数据c2	
		},
		{
			PHASE_S7,
			EVENT_ENQ,						// 收到ENQ
			mtxCheckDataC2,	 				// ENQ应答中的文本尺寸数据c2确认
			{ PHASE_S8, PHASE_S13 }			// 等待接收ENQ应答数据中的文本内容或数据错误处理
		},
		{
			PHASE_S7,
			EVENT_CHR,						// 收到数据
			mtxCheckDataC2,	 				// ENQ应答中的文本尺寸数据c2确认
			{ PHASE_S8, PHASE_S13 }			// 等待接收ENQ应答数据中的文本内容或数据错误处理
		},
		{
			PHASE_S7,
			EVENT_ACK,						// 收到ACK
			mtxCheckDataC2,	 				// ENQ应答中的文本尺寸数据c2确认
			{ PHASE_S8, PHASE_S13 }			// 等待接收ENQ应答数据中的文本内容或数据错误处理
		},
		{
			PHASE_S7,
			EVENT_NAK,						// 收到NAK
			mtxCheckDataC2,	 				// ENQ应答中的文本尺寸数据c2确认
			{ PHASE_S8, PHASE_S13 }			// 等待接收ENQ应答数据中的文本内容或数据错误处理
		},
		{
			PHASE_S7,
			EVENT_EOT,						// 收到EOT
			mtxCheckDataC2,	 				// ENQ应答中的文本尺寸数据c2确认
			{ PHASE_S8, PHASE_S13 }			// 等待接收ENQ应答数据中的文本内容或数据错误处理
		},
		{
			PHASE_S7,
			EVENT_TIMEOUT,					// 超时
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待接收ENQ应答 
		},
		{
			PHASE_S7,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S7, PHASE_S7 }			// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 8, 等待接收ENQ应答中的文本数据
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S8,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S8, PHASE_S8 }			// 等待接收ENQ应答中的文本数据	
		},
		{
			PHASE_S8,
			EVENT_ENQ,						// 收到ENQ
			mtxDummy,	 					// 不处理
			{ PHASE_S8, PHASE_S8 }			// 等待接收ENQ应答中的文本数据
		},
		{
			PHASE_S8,
			EVENT_CHR,						// 收到数据
			mtxRecvContext,	 				// 接收ENQ应答中的文本数据
			{ PHASE_S8, PHASE_S9 }			// 继续接收或等待BCC确认
		},
		{
			PHASE_S8,
			EVENT_ACK,						// 收到ACK
			mtxDummy,	 					// 不处理
			{ PHASE_S8, PHASE_S8 }			// 等待接收ENQ应答中的文本数据
		},
		{
			PHASE_S8,
			EVENT_NAK,						// 收到NAK
			mtxDummy,	 					// 不处理
			{ PHASE_S8, PHASE_S8 }			// 等待接收ENQ应答中的文本数据
		},
		{
			PHASE_S8,
			EVENT_EOT,						// 收到EOT
			mtxDummy,	 					// 不处理
			{ PHASE_S8, PHASE_S8 }			// 等待接收ENQ应答中的文本数据
		},
		{
			PHASE_S8,
			EVENT_TIMEOUT,					// 超时
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待接收ENQ应答 
		},
		{
			PHASE_S8,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S8, PHASE_S8 }			// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 9, 等待接收ENQ应答中的BCC的位1
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S9,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S9, PHASE_S9 }			// 等待接收ENQ应答中的BCC的位1	
		},
		{
			PHASE_S9,
			EVENT_ENQ,						// 收到ENQ
			mtxDummy,	 					// 不处理
			{ PHASE_S9, PHASE_S9 }			// 等待接收ENQ应答中的BCC的位1
		},
		{
			PHASE_S9,
			EVENT_CHR,						// 收到数据
			mtxRecvBCC,	 					// 接收BCC第一位
			{ PHASE_S10, PHASE_S10 }		// 等待接收BCC第二位和BCC确认
		},
		{
			PHASE_S9,
			EVENT_ACK,						// 收到ACK
			mtxDummy,	 					// 不处理
			{ PHASE_S9, PHASE_S9 }			// 等待接收ENQ应答中的BCC的位1
		},
		{
			PHASE_S9,
			EVENT_NAK,						// 收到NAK
			mtxDummy,	 					// 不处理
			{ PHASE_S9, PHASE_S9 }			// 等待接收ENQ应答中的BCC的位1
		},
		{
			PHASE_S9,
			EVENT_EOT,						// 收到EOT
			mtxDummy,	 					// 不处理
			{ PHASE_S9, PHASE_S9 }			// 等待接收ENQ应答中的BCC的位1
		},
		{
			PHASE_S9,
			EVENT_TIMEOUT,					// 超时
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待接收ENQ应答 
		},
		{
			PHASE_S9,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S9, PHASE_S9 }			// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 10, 等待接收ENQ应答中的BCC的位2，BCC确认
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S10,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S10, PHASE_S10 }		// 等待接收ENQ应答中的BCC的位2，BCC确认	
		},
		{
			PHASE_S10,
			EVENT_ENQ,						// 收到ENQ
			mtxDummy,	 					// 不处理
			{ PHASE_S10, PHASE_S10 }		// 等待接收ENQ应答中的BCC的位2，BCC确认
		},
		{
			PHASE_S10,
			EVENT_CHR,						// 收到数据
			mtxCheckBCC,	 				// 接收ENQ应答中的BCC的位2，BCC确认
			{ PHASE_S1, PHASE_S11 }			// 回到空闲或等待EOT
		},
		{
			PHASE_S10,
			EVENT_ACK,						// 收到ACK
			mtxDummy,	 					// 不处理
			{ PHASE_S10, PHASE_S10 }		// 等待接收ENQ应答中的BCC的位2，BCC确认
		},
		{
			PHASE_S10,
			EVENT_NAK,						// 收到NAK
			mtxDummy,	 					// 不处理
			{ PHASE_S10, PHASE_S10 }		// 等待接收ENQ应答中的BCC的位2，BCC确认
		},
		{
			PHASE_S10,
			EVENT_EOT,						// 收到EOT
			mtxDummy,	 					// 不处理
			{ PHASE_S10, PHASE_S10 }		// 等待接收ENQ应答中的BCC的位2，BCC确认
		},
		{
			PHASE_S10,
			EVENT_TIMEOUT,					// 超时（等待接收ENQ接收应答或等待ACK/NAK或等待接收EOT)
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待接收ENQ应答  
		},
		{
			PHASE_S10,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S10, PHASE_S10 }		// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 11, ACK发送完毕，等待接收EOT
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S11,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S11, PHASE_S11 }		// ACK发送完毕，等待接收EOT	
		},
		{
			PHASE_S11,
			EVENT_ENQ,						// 收到ENQ
			mtxRetrySndACK,					// 重发ACK
			{ PHASE_S1, PHASE_S11 }		// 等待接收EOT
		},
		{
			PHASE_S11,
			EVENT_CHR,						// 收到数据
			mtxRetrySndACK,					// 重发ACK
			{ PHASE_S1, PHASE_S11 }		// 等待接收EOT
		},
		{
			PHASE_S11,
			EVENT_ACK,						// 收到ACK
			mtxRetrySndACK,					// 重发ACK
			{ PHASE_S1, PHASE_S11 }		// 等待接收EOT
		},
		{
			PHASE_S11,
			EVENT_NAK,						// 收到NAK
			mtxRetrySndACK,					// 重发ACK
			{ PHASE_S1, PHASE_S11 }		// 等待接收EOT
		},
		{
			PHASE_S11,
			EVENT_EOT,						// 收到EOT
			mtxSndEOT,						// 发送EOT
			{ PHASE_S1, PHASE_S1 }			// 回到空闲
		},
		{
			PHASE_S11,
			EVENT_TIMEOUT,					// 超时
			mtxRetrySndACK,					// 重发ACK
			{ PHASE_S1, PHASE_S11 }		// 等待接收EOT
		},
		{
			PHASE_S11,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S11, PHASE_S11 }		// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 12, NAK发送后的再发送数据等待
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S12,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S12, PHASE_S12 }		// NAK发送后的再发送数据等待	
		},
		{
			PHASE_S12,
			EVENT_ENQ,						// 收到ENQ
			mtxDummy,						// 重发NAK
			{ PHASE_S1, PHASE_S4 }			// 回到空闲或等待NAT发送后的数据接收
		},
		{
			PHASE_S12,
			EVENT_CHR,						// 收到数据
			mtxRetrySndNAK,					// 重发NAK
			{ PHASE_S1, PHASE_S12 }			// 回到空闲或等待NAT发送后的数据接收
		},
		{
			PHASE_S12,
			EVENT_ACK,						// 收到ACK
			mtxRetrySndNAK,					// 重发NAK
			{ PHASE_S1, PHASE_S12 }			// 回到空闲或等待NAT发送后的数据接收
		},
		{
			PHASE_S12,
			EVENT_NAK,						// 收到NAK
			mtxRetrySndNAK,					// 重发NAK
			{ PHASE_S1, PHASE_S12 }			// 回到空闲或等待NAT发送后的数据接收
		},
		{
			PHASE_S12,
			EVENT_EOT,						// 收到EOT
			mtxRetrySndNAK,					// 重发NAK
			{ PHASE_S1, PHASE_S12 }			// 回到空闲或等待NAT发送后的数据接收
		},
		{
			PHASE_S12,
			EVENT_TIMEOUT,					// 超时
			mtxRetrySndNAK,					// 重发NAK
			{ PHASE_S1, PHASE_S12 }			// 回到空闲或等待NAT发送后的数据接收
		},
		{
			PHASE_S12,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S12, PHASE_S12 }		// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 13, 等待处理ENQ应答中的数据接收错误
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S13,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S13, PHASE_S13 }		// 等待处理ENQ应答中的数据接收错误	
		},
		{
			PHASE_S13,
			EVENT_ENQ,						// 收到ENQ
			mtxDummy,						// 不处理
			{ PHASE_S13, PHASE_S13 }		// 等待处理ENQ应答中的数据接收错误
		},
		{
			PHASE_S13,
			EVENT_CHR,						// 收到数据
			mtxDummy,						// 不处理
			{ PHASE_S13, PHASE_S13 }		// 等待处理ENQ应答中的数据接收错误
		},
		{
			PHASE_S13,
			EVENT_ACK,						// 收到ACK
			mtxDummy,	 					// 不处理
			{ PHASE_S13, PHASE_S13 }		// 等待处理ENQ应答中的数据接收错误
		},
		{
			PHASE_S13,
			EVENT_NAK,						// 收到NAK
			mtxDummy,						// 不处理
			{ PHASE_S13, PHASE_S13 }		// 等待处理ENQ应答中的数据接收错误
		},
		{
			PHASE_S13,
			EVENT_EOT,						// 收到EOT
			mtxDummy,						// 不处理
			{ PHASE_S13, PHASE_S13 }		// 等待处理ENQ应答中的数据接收错误
		},
		{
			PHASE_S13,
			EVENT_TIMEOUT,					// 超时（等待接收ENQ接收应答或等待ACK/NAK或等待接收EOT)
			mtxRetryEnquire,				// 重发ENQ
			{ PHASE_S1, PHASE_S3 }			// 回到空闲或等待接收ENQ应答 
		},
		{
			PHASE_S13,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S13, PHASE_S13 }		// 回到空闲或等待接收ENQ应答
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 14, EOT发送完毕，等待接收EOT
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S14,
			EVENT_REQSEND,					// 发送请求
			mtxDummy,						// 不处理
			{ PHASE_S14, PHASE_S14 }		// EOT发送完毕，等待接收EOT
		},
		{
			PHASE_S14,
			EVENT_ENQ,						// 收到ENQ
			mtxRetrySndEOT,					// 重发EOT 
			{ PHASE_S1, PHASE_S14 }			// 回到空闲或等待接收EOT
		},
		{
			PHASE_S14,
			EVENT_CHR,						// 收到数据
			mtxRetrySndEOT,	 			    // 不处理
			{ PHASE_S14, PHASE_S14 }		// EOT发送完毕，等待接收EOT
		},
		{
			PHASE_S14,
			EVENT_ACK,						// 收到ACK
			mtxRetrySndEOT,					// 重发EOT 
			{ PHASE_S1, PHASE_S14 }			// 回到空闲或等待接收EOT
		},
		{
			PHASE_S14,
			EVENT_NAK,						// 收到NAK
			mtxRetrySndEOT,					// 重发EOT 
			{ PHASE_S1, PHASE_S14 }			// 回到空闲或等待接收EOT
		},
		{
			PHASE_S14,
			EVENT_EOT,						// 收到EOT
			mtxDummy,						// 不处理
			{ PHASE_S1, PHASE_S1 }			// 空闲
		},
		{
			PHASE_S14,
			EVENT_TIMEOUT,					// 超时（等待接收EOT)
			mtxRetrySndEOT,					// 重发EOT 
			{ PHASE_S1, PHASE_S14 }			// 回到空闲或等待接收EOT
		},
		{
			PHASE_S14,
			EVENT_QUERY,					// 心跳查询
			mtxDummy,						// 不处理
			{ PHASE_S14, PHASE_S14 }		// 回到空闲或等待接收ENQ应答
		}
	}
};

//--------------------------------------------------------------------------------------------
//	end of file