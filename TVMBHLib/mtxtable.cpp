#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // ��������
extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // ���������������
extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // ��������������Գ�ʱ
extern int mtxAcceptAck(HTDLWORK *pWork, TMATRIX *pMatrix);          // ����ACK
//extern int mtxEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);            // ����ENQ
//extern int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);       // ����ENQ������3��
//extern int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);     // ����ENQ������3�γ�ʱ
//extern int mtxRecvFrameSeq(HTDLWORK *pWork, TMATRIX *pMatrix);       // ����R-SEQ
//extern int mtxRecvBlockNo(HTDLWORK *pWork, TMATRIX *pMatrix);        // ����R-BLK
extern int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix);             // ������ջ���
extern int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix);			 // ����һ���ֽ�
extern int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix);		 // ���ճ�ʱ����
extern int mtxCheckBcc(HTDLWORK *pWork, TMATRIX *pMatrix);           // BCC��֤����
extern int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix);              // ���⴦��
extern int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix);            // ͨ��Э�������
extern int checkSendTimeout(HTDLWORK *pWork, TMATRIX *pMatrix);		 // ��鷢�ͳ�ʱʱ��״̬
extern int checkSendPaddingCmd(HTDLWORK *pWork, TMATRIX *pMatrix);	 // ��鷢�͹����н���æµ״̬

///***************************************************************************
/// HTDL Time monitor
///***************************************************************************
DWORD wiTimeOut[2][MAX_PHASE] =
{
	{							///< 9600 BPS
		10000,					///< Phase 1, ���У��ȴ�������¼�/��鷢���Ƿ����
		3000,					///< Phase 2, �������ϣ��ȴ�ACK / NAK
		20000,					///< Phase 3, �ȴ�����Ӧ����ON
		20000,					///< Phase 4, ��Ӧ������ȴ�STX
		3000,					///< Phase 5, �ȴ�ETX
		2000,					///< Phase 6, �ȴ�BCC
	},
	{							///< 19200 BPS
		10000,					///< Phase 1, ���У��ȴ�������¼�/��鷢���Ƿ����
		0,						///< Phase 2, �������ϣ��ȴ�ACK / NAK
		0,						///< Phase 3, �ȴ�����Ӧ����ON
		0,						///< Phase 4, ��Ӧ������ȴ�STX
		0,						///< Phase 5, �ȴ�ETX
		0,						///< Phase 6, �ȴ�BCC
	}
};

//
// HTDL MATRIX
//
TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT] = {

	//--------------------------------------------------------------------------*
	//  Phase 1, ���У��ȴ�������¼�
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S1,
			EVENT_REQSEND,					// ��������
			mtxSend,								// ��������
			{ PHASE_S1, PHASE_S2 }		// �ȴ�ACK
		},
		{
			PHASE_S1,
			EVENT_STX,						// �յ�STX
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }	// �ȴ������
		},
		{
			PHASE_S1,
			EVENT_CHR,						// �յ�����
			mtxDummy,	 					// ������
			{ PHASE_S1, PHASE_S1 }	// �ȴ������
		},
		{
			PHASE_S1,
			EVENT_ETX,						// �յ�ETX
			mtxDummy,	 					// ������
			{ PHASE_S1, PHASE_S1 }	// �ȴ������
		},
		{
			PHASE_S1,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,	 					// ������
			{ PHASE_S1, PHASE_S1 }	// �ȴ������
		},
		{
			PHASE_S1,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }	// �ȴ�ACK
		},
		{
			PHASE_S1,
			EVENT_ON,						// �յ�ON
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }	// �ȴ�ACK
		},
		{
			PHASE_S1,
			EVENT_OFF,						// �յ�OFF
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }	// �ȴ�ACK
		},
		{
			PHASE_S1,
			EVENT_TIMEOUT,					// ��ʱ
			checkSendTimeout,				// �����ͳ�ʱ
			{ PHASE_S1, PHASE_S1 }		// �ȴ������
		}
	},

	//--------------------------------------------------------------------------*
	//  Phase 2, �������ϣ��ȴ�ACK / NAK
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S2,
			EVENT_REQSEND,					// ��������
			mtxDummy,							// ������
			{ PHASE_S2, PHASE_S2 }		// �ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_STX,						// �յ�STX
			mtxDummy,						// ������
			{ PHASE_S2, PHASE_S2 }	// �ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_CHR,						// �յ�����
			mtxDummy,	 					// ������
			{ PHASE_S2, PHASE_S2 }	// �ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_ETX,						// �յ�ETX
			mtxDummy,	 					// ������
			{ PHASE_S2, PHASE_S2 }	// �ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_ACK,						// �յ�ACK
			mtxAcceptAck,	 				// ����ACK
			{ PHASE_S4, PHASE_S1 }	// ��Ҫ������ȴ�STX�����跴��������
		},
		{
			PHASE_S2,
			EVENT_NAK,						// �յ�NCK
			mtxRetrySend,					// �ط�
			{ PHASE_S1, PHASE_S2 }	// �ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_ON,						// �յ�ON
			mtxDummy,						// ������
			{ PHASE_S2, PHASE_S2 }	// �ȴ�ACK
		},
		{
			PHASE_S2,
			EVENT_OFF,						// �յ�OFF
			mtxDummy,						// ������
			{ PHASE_S2, PHASE_S3 }	// �ȴ�����Ӧ����ON
		},
		{
			PHASE_S2,
			EVENT_TIMEOUT,				// ��ʱ
			mtxTimeOutSend,				// �ط�
			{ PHASE_S1, PHASE_S2 }	// �ȴ�ACK/NAK
		}
	},

	//--------------------------------------------------------------------------*
	//  Phase 3, �ȴ�����Ӧ����ON
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S3,
			EVENT_REQSEND,					// ��������
			mtxDummy,							// ������
			{ PHASE_S3, PHASE_S3 }		// �ȴ�����ON
		},
		{
			PHASE_S3,
			EVENT_STX,						// �յ�STX
			mtxDummy,						// ������
			{ PHASE_S3, PHASE_S3 }	// �ȴ�����ON
		},
		{
			PHASE_S3,
			EVENT_CHR,						// �յ�����
			mtxDummy,	 					// ������
			{ PHASE_S3, PHASE_S3 }	//  �ȴ�����ON
		},
		{
			PHASE_S3,
			EVENT_ETX,						// �յ�ETX
			mtxDummy,	 					// ������
			{ PHASE_S3, PHASE_S3 }	// �ȴ�ACK/NAK
		},
		{
			PHASE_S3,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,	 					// ������
			{ PHASE_S3, PHASE_S3 }	// ��Ҫ������ȴ�STX�����跴��������
		},
		{
			PHASE_S3,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,						// ������
			{ PHASE_S3, PHASE_S3 }	// �ȴ�ACK/NAK
		},
		{
			PHASE_S3,
			EVENT_ON,						// �յ�ON
			checkSendPaddingCmd,			// ֱ��ת������
			{ PHASE_S1, PHASE_S2 }	// �ȴ�����/����ACK��ϵͳæµ����/���·������
		},
		{
			PHASE_S3,
			EVENT_OFF,						// �յ�OFF
			mtxDummy,						// ������
			{ PHASE_S3, PHASE_S3 }	// �ȴ�����Ӧ����ON
		},
		{
			PHASE_S3,
			EVENT_TIMEOUT,				// ��ʱ
			mtxDummy,					// ֱ��ת������
			{ PHASE_S1, PHASE_S1 }	// �ȴ�����
		}
	},


//--------------------------------------------------------------------------*
//  Phase 4,��Ӧ������ȴ�STX
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S4,
			EVENT_REQSEND,					// ��������
			mtxDummy,							// ������
			{ PHASE_S4, PHASE_S4 }		// �ȴ�STX
		},
		{
			PHASE_S4,
			EVENT_STX,						// �յ�STX
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S5 }	// �ȴ�ETX
		},
		{
			PHASE_S4,
			EVENT_CHR,						// �յ�MSG
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }	// �ȴ�STX
		},
		{
			PHASE_S4,
			EVENT_ETX,						// �յ�ETX
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }	// �ȴ�STX
		},
		{
			PHASE_S4,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }	// �ȴ�STX
		},
		{
			PHASE_S4,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }	// �ȴ�STX
		},
		{
			PHASE_S4,
			EVENT_ON,						// �յ�ON
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }	// �ȴ�ACK
		},
		{
			PHASE_S4,
			EVENT_OFF,						// �յ�OFF
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }	// �ȴ�����Ӧ����ON
		},
		{
			PHASE_S4,
			EVENT_TIMEOUT,					// ����ʱ
			mtxTimeOutSend,					// ���Է���
			{ PHASE_S1, PHASE_S4 }		// �ȴ�STX
		}
	},

//--------------------------------------------------------------------------*
//  Phase S5: �ȴ�ETX
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S5,
			EVENT_REQSEND,					// ��������
			mtxDummy,							// ������
			{ PHASE_S5, PHASE_S5 }		// �ȴ�ETX
		},
		{
			PHASE_S5,
			EVENT_STX,						// �յ�STX
			mtxReRecv,						// ���½���
			{ PHASE_S5, PHASE_S5 }	// �ȴ�ETX
		},
		{
			PHASE_S5,
			EVENT_CHR,						// �յ�����
			mtxOnChar,		 				// ��������
			{ PHASE_S5, PHASE_S5 }	// �ȴ�ETX
		},
		{
			PHASE_S5,
			EVENT_ETX,						// �յ�ETX
			mtxDummy,						// ������
			{ PHASE_S5, PHASE_S6 }	// �ȴ�BCC
		},
		{
			PHASE_S5,
			EVENT_ACK,						// �յ�ACK
			mtxProtErr,						// ʱ�����
			{ PHASE_S1, PHASE_S1 }	// ����
		},
		{
			PHASE_S5,
			EVENT_NAK,						// �յ�NAK
			mtxProtErr,						// ʱ�����
			{ PHASE_S1, PHASE_S1 }	// ����
		},
		{
			PHASE_S5,
			EVENT_ON,						// �յ�ON
			mtxProtErr,						// ʱ�����
			{ PHASE_S5, PHASE_S5 }	// ����
		},
		{
			PHASE_S5,
			EVENT_OFF,						// �յ�OFF
			mtxProtErr,						// ʱ�����
			{ PHASE_S5, PHASE_S5 }	// ����
		},
		{
			PHASE_S5,
			EVENT_TIMEOUT,					// ��ʱ
			mtxTimeOutRecv,					// ���ճ�ʱ����
			{ PHASE_S1, PHASE_S1 }		// ����
		}
	},

//--------------------------------------------------------------------------*
//  Phase 6, �ȴ�BCC
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S6,
			EVENT_REQSEND,					// ��������
			mtxDummy,							// ������
			{ PHASE_S6, PHASE_S6 }		// �ȴ�BCC
		},
		{
			PHASE_S6,
			EVENT_STX,						// �յ�STX
			mtxReRecv,						// ���½���
			{ PHASE_S5, PHASE_S5 }	// �ȴ�ETX
		},
		{
			PHASE_S6,
			EVENT_CHR,						// �յ�����
			mtxCheckBcc,	 				// ���BCC
			{ PHASE_S4, PHASE_S1 }	// < S2 (FAIL or Wait next chunk) or S1 (Done)
		},
		{
			PHASE_S6,
			EVENT_ETX,						// �յ�ETX
			mtxReRecv,						// ���½���
			{ PHASE_S6, PHASE_S4 }	// �ȴ�STX
		},
		{
			PHASE_S6,
			EVENT_ACK,						// �յ�ACK
			mtxProtErr,						// ʱ�����
			{ PHASE_S1, PHASE_S1 }	// ����
		},
		{
			PHASE_S6,
			EVENT_NAK,						// �յ�ACK
			mtxProtErr,						// ʱ�����
			{ PHASE_S1, PHASE_S1 }	//  ����
		},
		{
			PHASE_S6,
			EVENT_TIMEOUT,					// ��ʱ
			mtxTimeOutRecv,					// ��ʱ����
			{ PHASE_S1, PHASE_S1 }		// ����
		}
	}
};

//--------------------------------------------------------------------------*
// end of file