#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // ��������
extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // ���������������
extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // ��������������Գ�ʱ
extern int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix);             // ������ջ���
extern int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix);			 // ����һ���ֽ�
extern int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix);		 // ���ճ�ʱ����
extern int mtxCalcCrc16(HTDLWORK *pWork, TMATRIX *pMatrix);          // CRC16��֤����
extern int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix);              // ���⴦��
extern int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix);            // ͨ��Э�������


///***************************************************************************
/// HTDL Time monitor
///***************************************************************************
DWORD wiTimeOut[2][MAX_PHASE] =
{
	{								///< 9600 BPS
		0,								///< Phase 1, ���У��ȴ�������¼�
		5000,						///< Phase 2, �ȴ�STX
		128000						///< Phase 3, �ȴ�ETX
	},
	{								///< 19200 BPS
		0,								///< Phase 1, ���У��ȴ�������¼�
		10000,						///< Phase 2, �ȴ�STX
		128000						///< Phase 3, �ȴ�ETX
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
			{ PHASE_S1, PHASE_S1 }		// �ȴ�ACK
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
			EVENT_TIMEOUT,					// ��ʱ
			mtxDummy,							// ������
			{ PHASE_S1, PHASE_S1 }		// �ȴ������
		}
	},

	//--------------------------------------------------------------------------*
	//  Phase 2, �������ϣ��ȴ�����STX
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
			{ PHASE_S2, PHASE_S3 }	// �ȴ�ACK/NAK
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
			PHASE_S1,
			EVENT_TIMEOUT,				// ��ʱ
			mtxTimeOutSend,				// �ط�
			{ PHASE_S2, PHASE_S2 }	// �ȴ�ACK/NAK
		}
	},
//--------------------------------------------------------------------------*
//  Phase 3, ǰ���Ѿ��յ�STX ,����һ��STX֮��ͱ�ʾ���Խ���������
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S3,
			EVENT_REQSEND,					// ��������
			mtxDummy,							// ������
			{ PHASE_S3, PHASE_S3 }		// �ȴ�STX
		},
		{
			PHASE_S3,
			EVENT_STX,						// �յ�STX
			mtxDummy,						// ������
			{ PHASE_S3, PHASE_S3 }	// �ȴ�DLE-ETX
		},
		{
			PHASE_S3,
			EVENT_CHR,						// �յ�MSG
			mtxOnChar,						// ������
			{ PHASE_S3, PHASE_S3 }	// �ȴ�STX
		},
		{
			PHASE_S3,
			EVENT_ETX,						// �յ�ETX
			mtxCalcCrc16,					// ������
			{ PHASE_S1, PHASE_S1 }	// �ȴ�STX
		},
		{
			PHASE_S3,
			EVENT_TIMEOUT,					// ����ʱ
			mtxDummy,					// �ط�ENQ
			{ PHASE_S3, PHASE_S3 }		// �ȴ�STX
		}
	}
};

//--------------------------------------------------------------------------*
// end of file