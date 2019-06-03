#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // ��������
//extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // ���������������
//extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // ��������������Գ�ʱ
//extern int mtxAcceptEnq(HTDLWORK *pWork, TMATRIX *pMatrix);          // ����ACK
//extern int mtxEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);            // ����ENQ
//extern int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);       // ����ENQ������3��
//extern int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);     // ����ENQ������3�γ�ʱ
//extern int mtxRecvFrameSeq(HTDLWORK *pWork, TMATRIX *pMatrix);       // ����R-SEQ
//extern int mtxRecvBlockNo(HTDLWORK *pWork, TMATRIX *pMatrix);        // ����R-BLK
extern int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix);             // ������ջ���
extern int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix);			 // ����һ���ֽ�
extern int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix);		 // ���ճ�ʱ����
//extern int mtxCheckBcc(HTDLWORK *pWork, TMATRIX *pMatrix);           // BCC��֤����
extern int mtxRecvComplete(HTDLWORK *pWork, TMATRIX *pMatrix);    // ������ɴ���
extern int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix);              // ���⴦��
extern int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix);            // ͨ��Э�������
extern int mtxRecvLenth(HTDLWORK *pWork, TMATRIX *pMatrix);          // �յ����ȴ���



///***************************************************************************
/// HTDL Time monitor
///***************************************************************************
DWORD wiTimeOut[2][MAX_PHASE] =
{
	// 9600 BPS
	{								
		0,								// S1 �ȴ�STX  wait for STX
		0,								// S2 �ȴ�LEN  wait for LEN
		30000,							// S3 �ȴ�ETX wait for ETX
	},
	// 19200 BPS
	{								
		0,								// S1 �ȴ�STX  wait for STX
		0,								// S2 �ȴ�LEN  wait for LEN
		6000,							// S3 �ȴ� ETX wait for ETX
	}
};

//
// HTDL MATRIX
//
TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT] = {

//--------------------------------------------------------------------------*
//  Phase S1: �ȴ�STX״̬
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S1,
			EVENT_REQSEND,					// ��������
			mtxSend,	 					// Send command
			{ PHASE_S1, PHASE_S1 }		    // �ȴ�STX
		},
		{
			PHASE_S1,
			EVENT_STX,						// �յ�STX
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S2 }			// �ȴ�ENQ
		},
		{
			PHASE_S1,
			EVENT_CHR,						// �յ�����
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }			// �ȴ�ENQ
		},
		{
			PHASE_S1,
			EVENT_ETX,							// �յ�ETX
			mtxDummy,							// ������
			{ PHASE_S1, PHASE_S1 }		// �ȴ�ENQ
		},
		{
			PHASE_S1,
			EVENT_TIMEOUT,					// ����ʱ
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }			// �ȴ�ENQ
		},
	},

	//--------------------------------------------------------------------------*
	//  Phase S2: �ȴ�LEN״̬
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S2,
			EVENT_REQSEND,						// ��������
			mtxDummy,								// ������
			{ PHASE_S2, PHASE_S2 }			// �ȴ�LEN
		},
		{
			PHASE_S2,
			EVENT_STX,								// �յ�STX
			mtxReRecv,								// ������ջ���
			{ PHASE_S2, PHASE_S2 }			// �ȴ�SEQ
		},
		{
			PHASE_S2,
			EVENT_CHR,								// �յ�MSG
			mtxRecvLenth,						// ���泤��
			{ PHASE_S3, PHASE_S3 }			// �ȴ�ETX
		},
		{
			PHASE_S2,
			EVENT_ETX,								// �յ�ETX
			mtxDummy,								// ������
			{ PHASE_S1, PHASE_S1 }			// �ȴ�STX
		},
		{
			PHASE_S2,
			EVENT_TIMEOUT,					// ����ʱ
			mtxTimeOutRecv,					// ��ʱ����
			{ PHASE_S1, PHASE_S1 }		// �ȴ�ENQ
		}
		},

//--------------------------------------------------------------------------*
//  Phase S3: �ȴ�ETX״̬
//--------------------------------------------------------------------------*
	{
		{
			PHASE_S3,
			EVENT_REQSEND,						// ��������
			mtxDummy,							// ������
			{ PHASE_S3, PHASE_S3 }				// �ȴ�ETX
		},
		{
			PHASE_S3,
			EVENT_STX,							// �յ�STX
			mtxOnChar,							// �����������
			{ PHASE_S3, PHASE_S3 }				// �����ȴ�ETX
		},
		{
			PHASE_S3,
			EVENT_CHR,							// �յ�MSG
			mtxOnChar,							// ������
			{ PHASE_S3, PHASE_S3 }				// �ȴ�ETX
		},
		{
			PHASE_S3,
			EVENT_ETX,							// �յ�ETX
			mtxRecvComplete,					// ������ɴ���
			{ PHASE_S1, PHASE_S3 }				// �ɹ����ȴ�STX ʧ�ܣ��ȴ�ETX 
		},
		{
			PHASE_S1,
			EVENT_TIMEOUT,					// ����ʱ
			mtxTimeOutRecv,					// ��ʱ����
			{ PHASE_S1, PHASE_S1 }			// �ȴ�STX
		}
	}
};

//--------------------------------------------------------------------------*
// end of file