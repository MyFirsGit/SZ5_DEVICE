#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // ��������
extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // ���������������
extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // ��������������Գ�ʱ
extern int mtxAcceptAck(HTDLWORK *pWork, TMATRIX *pMatrix);          // ����ACK
extern int mtxEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);            // ����ENQ
extern int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);       // ����ENQ������3��
extern int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);     // ����ENQ������3�γ�ʱ
//extern int mtxRecvFrameSeq(HTDLWORK *pWork, TMATRIX *pMatrix);       // ����R-SEQ
//extern int mtxRecvBlockNo(HTDLWORK *pWork, TMATRIX *pMatrix);        // ����R-BLK
extern int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix);             // ������ջ���
extern int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix);			 // ����һ���ֽ�
extern int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix);		 // ���ճ�ʱ����
extern int mtxCheckBcc(HTDLWORK *pWork, TMATRIX *pMatrix);           // BCC��֤����
extern int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix);              // ���⴦��
extern int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix);            // ͨ��Э�������
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
		0,							// ���У��ȴ�����ENQ��ȴ�����ENQ
		5000,						// �ȴ�����DLE��ACK��NAK��
		5000,						// �ȴ�����DLE(STX)
		5000,						// �ȴ�����Length
		5000,						// �ȴ�����Data
		5000,						//.�ȴ�����DLE��ETX��
		5000,						// �ȴ�����CRC����У�飩
		20000,						// ������ϵȴ�����DLE��ACK��NAK��
	},
	{								///< 19200 BPS
		0,							// ���У��ȴ�����ENQ��ȴ�����ENQ
		0,							// �ȴ�����DLE��ACK��NAK��
		0,							// �ȴ�����DLE(STX)
		0,							// �ȴ�����Length
		0,							// �ȴ�����Data
		0,							//.�ȴ�����DLE��ETX��
		0,							// �ȴ�����CRC����У�飩
		0,							// ������ϵȴ�����DLE��ACK��NAK��
	}
};

//
// HTDL MATRIX
//
TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT] = {
	//--------------------------------------------------------------------------*
	//   Phase 1, ���У��ȴ�����ENQ��ȴ�����ENQ
	//--------------------------------------------------------------------------*
	{
		// Phase 1.1  ����ENQ
		{
			PHASE_S1,
			EVENT_REQSEND,
			mtxEnquire,				// ����ENQ
			{PHASE_S1,PHASE_S2}		// �ȴ�DLE��ACK��NAK��
		},

		// Phase 1.2  �յ�DLE
		{
			PHASE_S1,
			EVENT_DLE,
			mtxDummy,				// ������
			{PHASE_S1,PHASE_S1}		// ׼������ENQ
		},

		// Phase 1.3  �յ�ENQ
		{
			PHASE_S1,
			EVENT_ENQ,
			mtxSendACK,				// ����ACK
			{PHASE_S1,PHASE_S3}		// �ȴ�����DLE(STX)
		},

		// Phase 1.4  �յ�STX
		{
			PHASE_S1,
			EVENT_STX,
			mtxDummy,				// ������
			{PHASE_S1,PHASE_S1}		// ����
		},

		// Phase 1.5  �յ�ETX
		{
			PHASE_S1,
			EVENT_ETX,
			mtxDummy,				// ������
			{PHASE_S1,PHASE_S1}		// ����
		},

		// Phase 1.6  �յ�ACK
		{
			PHASE_S1,
			EVENT_ACK,
			mtxDummy,				// ������
			{PHASE_S1,PHASE_S1}		// ����
		},

		// Phase 1.7  �յ�NAK
		{
			PHASE_S1,
			EVENT_NAK,
			mtxDummy,				// ������
			{PHASE_S1,PHASE_S1}		// ����
		},

		// Phase 1.8  �յ�����
		{
			PHASE_S1,
			EVENT_CHR,
			mtxDummy,				// ������
			{PHASE_S1,PHASE_S1}		// ����
		},

		// Phase 1.9  ��ʱ
		{
			PHASE_S1,
			EVENT_TIMEOUT,
			mtxDummy,				// ������
			{PHASE_S1,PHASE_S1}		// ����
		}
	},
	
	//--------------------------------------------------------------------------*
	//   Phase 2, �ȴ�����DLE��ACK��NAK��
	//--------------------------------------------------------------------------*
	{
		// Phase 2.1  ����ENQ
		{
			PHASE_S2,
			EVENT_REQSEND,
			mtxDummy,				// ������
			{PHASE_S2,PHASE_S2}		// �ȴ�ACK��NAK
		},

		// Phase 2.2  �յ�DLE
		{
			PHASE_S2,
			EVENT_DLE,
			mtxDummy,				// ������
			{PHASE_S2,PHASE_S2}		// ׼������ACK��NAK
		},

		// Phase 2.3  �յ�ENQ
		{
			PHASE_S2,
			EVENT_ENQ,
			mtxSendACK,				// ��ʱ���յ�ENQ���ȴ����������(�п���ͬʱ����ENQ�������ò�)
			{PHASE_S2,PHASE_S3}		// �ȴ�����DLE(STX)
		},

		// Phase 2.4  �յ�STX
		{
			PHASE_S2,
			EVENT_STX,
			mtxDummy,				// ������
			{PHASE_S2,PHASE_S2}		// �����ȴ�ACK��NAK
		},

		// Phase 2.5  �յ�ETX
		{
			PHASE_S2,
			EVENT_ETX,
			mtxDummy,				// ������
			{PHASE_S2,PHASE_S2}		// �����ȴ�ACK��NAK
		},

		// Phase 2.6  �յ�ACK
		{
			PHASE_S2,
			EVENT_ACK,
			mtxSend,				// �ȵ���ACK���ͷ�������
			{PHASE_S1,PHASE_S8}		// ����������˽������״̬������ȴ�ACK
		},

		// Phase 2.7  �յ�NAK
		{
			PHASE_S2,
			EVENT_NAK,
			mtxRetryEnquire,		// ����������
			{PHASE_S1,PHASE_S2}		// ���������������״̬
		},

		// Phase 2.8  �յ�����
		{
			PHASE_S2,
			EVENT_CHR,
			mtxDummy,				// ������
			{PHASE_S2,PHASE_S2}		// �����ȴ�ACK��NAK
		},

		// Phase 2.9  ��ʱ
		{
			PHASE_S2,
			EVENT_TIMEOUT,
			mtxTimeOutEnquire,		// ��ʱ��������
			{PHASE_S1,PHASE_S2}		// ������ʱ���������״̬
		}
	},
	//--------------------------------------------------------------------------*
	//   Phase 3, �ȴ�����DLE(STX)
	//--------------------------------------------------------------------------*
	{
		// Phase 3.1  ����ENQ
		{
			PHASE_S3,
			EVENT_REQSEND,
			mtxDummy,				// ������
			{PHASE_S3,PHASE_S3}		// �����ȴ�����DLE��STX��
		},

		// Phase 3.2  �յ�DLE
		{
			PHASE_S3,
			EVENT_DLE,
			mtxDummy,				// ������
			{PHASE_S3,PHASE_S3}		// �����ȴ�����DLE��STX��
		},

		// Phase 3.3  �յ�ENQ
		{
			PHASE_S3,
			EVENT_ENQ,
			mtxSendACK,				// ����ACK
			{PHASE_S3,PHASE_S3}		// �����ȴ�����DLE��STX��
		},

		// Phase 3.4  �յ�STX
		{
			PHASE_S3,
			EVENT_STX,
			mtxReRecv,				// ��ս��ջ���
			{PHASE_S3,PHASE_S4}		// ת�����ճ��ȵ�λ��
		},

		// Phase 3.5  �յ�ETX
		{
			PHASE_S3,
			EVENT_ETX,
			mtxDummy,				// ������
			{PHASE_S3,PHASE_S1}		// ����
		},

		// Phase 3.6  �յ�ACK
		{
			PHASE_S3,
			EVENT_ACK,
			mtxDummy,				// ������
			{PHASE_S3,PHASE_S1}		// ����
		},

		// Phase 3.7  �յ�NAK
		{
			PHASE_S3,
			EVENT_NAK,
			mtxDummy,				// ������
			{PHASE_S3,PHASE_S1}		// ����
		},

		// Phase 3.8  �յ�����
		{
			PHASE_S3,
			EVENT_CHR,
			mtxDummy,				// ������
			{PHASE_S3,PHASE_S3}		// �����ȴ�����DLE��STX��
		},

		// Phase 3.9  ��ʱ
		{
			PHASE_S3,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// ��ʱ����
			{PHASE_S1,PHASE_S1}		// ����
		}
	},
	//--------------------------------------------------------------------------*
	//   Phase 4, �ȴ�����Length
	//--------------------------------------------------------------------------*
	{
		// Phase 4.1  ����ENQ
		{
			PHASE_S4,
			EVENT_REQSEND,
			mtxDummy,				// ������
			{PHASE_S4,PHASE_S4}		// ��������Length
		},

		// Phase 4.2  �յ�DLE
		{
			PHASE_S4,
			EVENT_DLE,
			mtxDummy,				// ������
			{PHASE_S4,PHASE_S4}		// ��������Length
		},

		// Phase 4.3  �յ�ENQ
		{
			PHASE_S4,
			EVENT_ENQ,
			mtxDummy,				// ������
			{PHASE_S4,PHASE_S4}		// ��������Length
		},

		// Phase 4.4  �յ�STX
		{
			PHASE_S4,
			EVENT_STX,
			mtxDummy,				// ������
			{PHASE_S4,PHASE_S4}		// ��������Length
		},

		// Phase 4.5  �յ�ETX
		{
			PHASE_S4,
			EVENT_ETX,
			mtxDummy,				// ������
			{PHASE_S4,PHASE_S4}		// ��������Length
		},

		// Phase 4.6  �յ�ACK
		{
			PHASE_S4,
			EVENT_ACK,
			mtxDummy,				// ������
			{PHASE_S4,PHASE_S4}		// ��������Length
		},

		// Phase 4.7  �յ�NAK
		{
			PHASE_S4,
			EVENT_NAK,
			mtxDummy,				// ������
			{PHASE_S4,PHASE_S4}		// ��������Length
		},

		// Phase 4.8  �յ�����
		{
			PHASE_S4,
			EVENT_CHR,
			mtxRecvDataLen,			// �������ݳ���
			{PHASE_S4,PHASE_S5}		// �ɹ�ת����������
		},

		// Phase 4.9  ��ʱ
		{
			PHASE_S4,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// ���ճ�ʱ
			{PHASE_S1,PHASE_S3}		// ת������DLE��STX��
		}
	},
	//--------------------------------------------------------------------------*
	//   Phase 5, �ȴ�����Data
	//--------------------------------------------------------------------------*
	{
		// Phase 5.1  ����ENQ
		{
			PHASE_S5,
			EVENT_REQSEND,
			mtxDummy,				// ������
			{PHASE_S5,PHASE_S5}		// ������������
		},

		// Phase 5.2  �յ�DLE
		{
			PHASE_S5,
			EVENT_DLE,
			mtxDummy,				// ������
			{PHASE_S5,PHASE_S5}		// ������������
		},

		// Phase 5.3  �յ�ENQ
		{
			PHASE_S5,
			EVENT_ENQ,
			mtxDummy,				// ������
			{PHASE_S5,PHASE_S5}		// ������������
		},

		// Phase 5.4  �յ�STX
		{
			PHASE_S5,
			EVENT_STX,
			mtxDummy,				// ������
			{PHASE_S5,PHASE_S5}		// ������������
		},

		// Phase 5.5  �յ�ETX
		{
			PHASE_S5,
			EVENT_ETX,
			mtxDummy,				// ������
			{PHASE_S5,PHASE_S5}		// ������������
		},

		// Phase 5.6  �յ�ACK
		{
			PHASE_S5,
			EVENT_ACK,
			mtxDummy,				// ������
			{PHASE_S5,PHASE_S5}		// ������������
		},

		// Phase 5.7  �յ�NAK
		{
			PHASE_S5,
			EVENT_NAK,
			mtxDummy,				// ������
			{PHASE_S5,PHASE_S5}		// ������������
		},

		// Phase 5.8  �յ�����
		{
			PHASE_S5,
			EVENT_CHR,
			mtxRecvContext,			// �������ݣ�One by one��
			{PHASE_S5,PHASE_S6}		// ���ݽ��������ת��PHASE 6�������������
		},

		// Phase 5.9  ��ʱ
		{
			PHASE_S5,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// ��ʱ����
			{PHASE_S1,PHASE_S3}		// ת��S3�������ȴ�DLE��STX��
		}
	},
	//--------------------------------------------------------------------------*
	//   Phase 6, �ȴ�����DLE��ETX��
	//--------------------------------------------------------------------------*
	{
		// Phase 6.1  ����ENQ
		{
			PHASE_S6,
			EVENT_REQSEND,
			mtxDummy,				// ������
			{PHASE_S6,PHASE_S6}		// �����ȴ�DLE��ETX��
		},

		// Phase 6.2  �յ�DLE
		{
			PHASE_S6,
			EVENT_DLE,
			mtxOnChar,				// ������ն���
			{PHASE_S6,PHASE_S6}		// ׼�����գ�ETX��
		},

		// Phase 6.3  �յ�ENQ
		{
			PHASE_S6,
			EVENT_ENQ,
			mtxDummy,				// ������
			{PHASE_S6,PHASE_S6}		// �����ȴ�DLE��ETX��
		},

		// Phase 6.4  �յ�STX
		{
			PHASE_S6,
			EVENT_STX,
			mtxReRecv,				// ���½���
			{PHASE_S6,PHASE_S4}		// ת�����ճ���
		},

		// Phase 6.5  �յ�ETX
		{
			PHASE_S6,
			EVENT_ETX,
			mtxOnChar,				// ������ն��У����ڼ���CRC
			{PHASE_S6,PHASE_S7}		// ����S7��׼������CRC
		},

		// Phase 6.6  �յ�ACK
		{
			PHASE_S6,
			EVENT_ACK,
			mtxDummy,				// ������
			{PHASE_S6,PHASE_S6}		// �����ȴ�DLE��ETX��
		},

		// Phase 6.7  �յ�NAK
		{
			PHASE_S6,
			EVENT_NAK,
			mtxDummy,				// ������
			{PHASE_S6,PHASE_S6}		// �����ȴ�DLE��ETX��
		},

		// Phase 6.8  �յ�����
		{
			PHASE_S6,
			EVENT_CHR,
			mtxDummy,				// ������
			{PHASE_S6,PHASE_S6}		// �����ȴ�DLE��ETX��
		},

		// Phase 6.9  ��ʱ
		{
			PHASE_S6,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// ��ʱ����
			{PHASE_S1,PHASE_S3}		// ���µȴ�����
		}
	},

	//--------------------------------------------------------------------------*
	//   Phase 7, �ȴ�����CRC����У�飩
	//--------------------------------------------------------------------------*
	{
		// Phase 7.1  ����ENQ
		{
			PHASE_S7,
			EVENT_REQSEND,
			mtxDummy,				// ������
			{PHASE_S7,PHASE_S7}		// �����ȴ�CRC
		},

		// Phase 7.2  �յ�DLE
		{
			PHASE_S7,
			EVENT_DLE,
			mtxDummy,				// ������
			{PHASE_S7,PHASE_S7}		// �����ȴ�CRC
		},

		// Phase 7.3  �յ�ENQ
		{
			PHASE_S7,
			EVENT_ENQ,
			mtxDummy,				// ������
			{PHASE_S7,PHASE_S7}		// �����ȴ�CRC
		},

		// Phase 7.4  �յ�STX
		{
			PHASE_S7,
			EVENT_STX,
			mtxDummy,				// ������
			{PHASE_S7,PHASE_S7}		// �����ȴ�CRC
		},

		// Phase 7.5  �յ�ETX
		{
			PHASE_S7,
			EVENT_ETX,
			mtxDummy,				// ������
			{PHASE_S7,PHASE_S7}		// �����ȴ�CRC
		},

		// Phase 7.6  �յ�ACK
		{
			PHASE_S7,
			EVENT_ACK,
			mtxDummy,				// ������
			{PHASE_S7,PHASE_S7}		// �����ȴ�CRC
		},

		// Phase 7.7  �յ�NAK
		{
			PHASE_S7,
			EVENT_NAK,
			mtxDummy,				// ������
			{PHASE_S7,PHASE_S7}		// �����ȴ�CRC
		},

		// Phase 7.8  �յ�����
		{
			PHASE_S7,
			EVENT_CHR,
			mtxCheckCRC,			// CRCУ��
			{PHASE_S7,PHASE_S1}		// �������ղ�У�飬���ؿ���
		},

		// Phase 7.9  ��ʱ
		{
			PHASE_S7,
			EVENT_TIMEOUT,
			mtxTimeOutRecv,			// ��ʱ����
			{PHASE_S1,PHASE_S1}		// ����
		}
	},

	//--------------------------------------------------------------------------*
	//   Phase 8, ������ϵȴ�����DLE��ACK��NAK��
	//--------------------------------------------------------------------------*
	{
		// Phase 8.1  ����ENQ
		{
			PHASE_S8,
			EVENT_REQSEND,
			mtxDummy,					// ������
			{PHASE_S8,PHASE_S8}			// �����ȴ�DLE��ACK��NAK��
		},

		// Phase 8.2  �յ�DLE
		{
			PHASE_S8,
			EVENT_DLE,
			mtxDummy,					// ������
			{PHASE_S8,PHASE_S8}			// �����ȴ�DLE(ACK��NAK)
		},

		// Phase 8.3  �յ�ENQ
		{
			PHASE_S8,
			EVENT_ENQ,
			mtxDummy,					// ������
			{PHASE_S1,PHASE_S1}			// �����ȴ�DLE��ACK��NAK��
		},

		// Phase 8.4  �յ�STX
		{
			PHASE_S8,
			EVENT_STX,
			mtxDummy,					// ������
			{PHASE_S8,PHASE_S8}			// �����ȴ�
		},

		// Phase 8.5  �յ�ETX
		{
			PHASE_S8,
			EVENT_ETX,
			mtxDummy,					// ������
			{PHASE_S8,PHASE_S8}			// �����ȴ�
		},

		// Phase 8.6  �յ�ACK
		{
			PHASE_S8,
			EVENT_ACK,
			mtxSend,					// ����Ƿ�������ݵķ���
			{PHASE_S1,PHASE_S8}			// ������ɽ������
		},

		// Phase 8.7  �յ�NAK
		{
			PHASE_S8,
			EVENT_NAK,
			mtxRetrySend,				// �ط�
			{PHASE_S1,PHASE_S8}			// ���л�ȴ�ACK
		},

		// Phase 8.8  �յ�����
		{
			PHASE_S8,
			EVENT_CHR,
			mtxDummy,					// ������
			{PHASE_S8,PHASE_S8}			// �����ȴ�
		},

		// Phase 8.9  ��ʱ
		{
			PHASE_S8,
			EVENT_TIMEOUT,
			mtxRetrySend,				// ����һ��
			{PHASE_S1,PHASE_S8}			// ���л��ٴη���
		}
	}
};

//--------------------------------------------------------------------------*
// end of file