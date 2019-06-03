#include "common.h"

extern int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix);               // ��������
extern int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix);          // ���������������
extern int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix);        // ��������������Գ�ʱ
//extern int mtxAcceptEnq(HTDLWORK *pWork, TMATRIX *pMatrix);          // ����ACK
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
//------------add by lichao
extern int mtxSndACK(HTDLWORK *pWork, TMATRIX *pMatrix);               // ��������
extern int mtxRetrySndACK(HTDLWORK *pWork, TMATRIX *pMatrix);          // ���������������
extern int mtxSndNAK(HTDLWORK *pWork, TMATRIX *pMatrix);               // ��������
extern int mtxRetrySndNAK(HTDLWORK *pWork, TMATRIX *pMatrix);          // ���������������
extern int mtxSndEOT(HTDLWORK *pWork, TMATRIX *pMatrix);               // ��������
extern int mtxRetrySndEOT(HTDLWORK *pWork, TMATRIX *pMatrix);          // ���������������
extern int mtxCheckDataA(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxCheckDataB(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxCheckDataC1(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxCheckDataC2(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxRecvContext(HTDLWORK *pWork, TMATRIX *pMatrix);			// 
extern int mtxRecvBCC(HTDLWORK *pWork, TMATRIX *pMatrix);				// 
extern int mtxCheckBCC(HTDLWORK *pWork, TMATRIX *pMatrix);				// 

extern int mtxSndEnquire(HTDLWORK *pWork, TMATRIX *pMatrix);            //���Ͳ�ѯ��

///***************************************************************************
/// HTDL Time monitor
///***************************************************************************
#define T1 5000					// add by lichao
#define T3 2000
#define T4 8000

DWORD wiTimeOut[2][MAX_PHASE] =
{
	{							///< 38400 BPS
		T1,						///< Phase 1, ���У��ȴ�������ʱ
		T1,						///< Phase 2, ���ݷ�����ϣ��ȴ�ACK / NAK
		T4,						///< Phase 3, ENQ������ϣ��ȴ�ENQӦ��
		T3,						///< Phase 4, �ȴ�У��ENQӦ���е�����a��ģ�����ͶԷ��ֵ�ַ��
		T3,						///< Phase 5, �ȴ�У��ENQӦ���е�����b�����ֵ�ַ�ʹ��ʹ�����
		T3,						///< Phase 6, �ȴ�У��ENQӦ���е�����c��c1���ı��ߴ����ݣ�
		T3,						///< Phase 7, �ȴ�У��ENQӦ���е�����c2���ı��ߴ����ݣ�
		T1,						///< Phase 8, �ȴ�����ENQӦ���е��ı�����
		T3,						///< Phase 9, �ȴ�����ENQӦ���е�BCC��λ1
		T1,						///< Phase 10, �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��
		T1,						///< Phase 11, ACK������ϣ��ȴ�����EOT
		T1,						///< Phase 12, NAK������ϣ��ȴ�����EOT
		T3,						///< Phase 13, �ȴ�����ENQӦ���е����ݽ��մ���
		T1,						///< Phase 14, EOT������ϣ��ȴ�����EOT
	},
	{							///< 19200 BPS
		0,						///< Phase 1, ���У��ȴ�������ʱ
		0,						///< Phase 2, ���ݷ�����ϣ��ȴ�ACK / NAK
		0,						///< Phase 3, ENQ������ϣ��ȴ�ENQӦ��
		0,						///< Phase 4, �ȴ�У��ENQӦ���е�����a��ģ�����ͶԷ��ֵ�ַ��
		0,						///< Phase 5, �ȴ�У��ENQӦ���е�����b�����ֵ�ַ�ʹ��ʹ�����
		0,						///< Phase 6, �ȴ�У��ENQӦ���е�����c��c1���ı��ߴ����ݣ�
		0,						///< Phase 7, �ȴ�У��ENQӦ���е�����c2���ı��ߴ����ݣ�
		0,						///< Phase 8, �ȴ�����ENQӦ���е��ı�����
		0,						///< Phase 9, �ȴ�����ENQӦ���е�BCC��λ1
		0,						///< Phase 10, �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��
		0,						///< Phase 11, ACK������ϣ��ȴ�����EOT
		0,						///< Phase 12, NAK������ϣ��ȴ�����EOT
		0,						///< Phase 13, �ȴ�����ENQӦ���е����ݽ��մ���
		0,						///< Phase 14, EOT������ϣ��ȴ�����EOT
	}
};

#define	MAX_PHASE	14			// ͨ��״̬����
#define PHASE_S1		0				// Phase 1, ����,�ȴ����ݷ���
#define PHASE_S2		1				// Phase 2, ���ݷ�����ϣ��ȴ�ACK / NAKӦ��
#define PHASE_S3		2				// Phase 3, ENQ��������ϣ��ȴ�ENQӦ��
#define PHASE_S4		3				// Phase 4, �ȴ�У��ENQӦ���е�����a��ģ�����ͶԷ��ֵ�ַ��
#define PHASE_S5		4				// Phase 5, �ȴ�У��ENQӦ���е�����b�����ֵ�ַ�ʹ��ʹ�����
#define PHASE_S6		5				// Phase 6, �ȴ�У��ENQӦ���е�����c��c1���ı��ߴ����ݣ�
#define PHASE_S7		6				// Phase 7, �ȴ�У��ENQӦ���е�����c2���ı��ߴ����ݣ�
#define PHASE_S8		7				// Phase 8, �ȴ�����ENQӦ���е��ı�����
#define PHASE_S9		8				// Phase 9, �ȴ�����ENQӦ���е�BCC��λ1
#define PHASE_S10		9				// Phase 10, �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��
#define PHASE_S11		10				// Phase 11, ACK������ϣ��ȴ�����EOT
#define PHASE_S12		11				// Phase 12, NAK���ͺ���ٷ������ݵȴ�
#define PHASE_S13		12				// Phase 13, �ȴ�����ENQӦ���е����ݽ��մ���
#define PHASE_S14		13				// Phase 14, EOT������ϣ��ȴ�����EOT
//
// HTDL MATRIX
//
TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT] = {

	//--------------------------------------------------------------------------*
	//  Phase 1, ���У��ȴ����ݷ��ͻ�������ʱ
	//--------------------------------------------------------------------------*
	{
		{
			PHASE_S1,
			EVENT_REQSEND,					// ������������
			mtxSend,						// ��������
			{ PHASE_S1, PHASE_S2 }			// �ȴ�ACK/NAK		
		},
		{
			PHASE_S1,
			EVENT_ENQ,						// �յ�ENQ����ѯӦ������ݣ�
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }			// �ȴ����ݷ��ͻ�������ʱ
		},
		{
			PHASE_S1,
			EVENT_CHR,						// �յ�����
			mtxDummy,	 					// ������
			{ PHASE_S1, PHASE_S1 }			// �ȴ����ݷ��ͻ�������ʱ
		},
		{
			PHASE_S1,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,	 					// ������
			{ PHASE_S1, PHASE_S1 }			// �ȴ����ݷ��ͻ�������ʱ
		},
		{
			PHASE_S1,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }			// �ȴ����ݷ��ͻ�������ʱ
		},
		{
			PHASE_S1,
			EVENT_EOT,						// �յ�EOT
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }			// �ȴ����ݷ��ͻ�������ʱ
		},
		{
			PHASE_S1,
			EVENT_TIMEOUT,					// ��ʱ
			mtxDummy,				        // ������
			{ PHASE_S1, PHASE_S1 }			// 
		},
		{
			PHASE_S1,
			EVENT_QUERY,					// ������ѯ
			mtxRetryEnquire,		        // ��������
			{ PHASE_S1, PHASE_S3 }			// �ȴ����ݷ��ͻ��ѯ��Ӧ��ȴ�
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 2, ���ݷ�����ϣ��ȴ�ACK / NAKӦ��
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S2,
			EVENT_REQSEND,					// ������������
			mtxDummy,						// ������
			{ PHASE_S2, PHASE_S2 }			// �ȴ�ACK��NAK		
		},
		{
			PHASE_S2,
			EVENT_ENQ,						// �յ�ENQ
			mtxRetrySend,					// �ط�����
			{ PHASE_S1, PHASE_S2 }			// �ص����л�ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_CHR,						// �յ�����
			mtxRetrySend,					// �ط�����
			{ PHASE_S1, PHASE_S2 }			// �ص����л�ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_ACK,						// �յ�ACK
			mtxSndEOT,	 					// ����EOT
			{ PHASE_S2, PHASE_S14 }			// EOT������ϣ��ȴ�����EOT
		},
		{
			PHASE_S2,
			EVENT_NAK,						// �յ�NAK
			mtxRetrySend,					// �ط�����
			{ PHASE_S1, PHASE_S2 }			// �ص����л�ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_EOT,						// �յ�EOT
			mtxRetrySend,					// �ط�����
			{ PHASE_S1, PHASE_S2 }			// �ص����л�ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_TIMEOUT,					// ��ʱ���ȴ�ACK/NAK)
			mtxRetrySend,					// �ط�����
			{ PHASE_S1, PHASE_S2 }			// �ص����л�ȴ�ACK/NAK
		},
		{
			PHASE_S2,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S2 }			// �ص����л�ȴ�ACK/NAK
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 3, ��������ѯ����������ϣ��ȴ�ENQӦ��
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S3,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S3, PHASE_S3 }			// �ȴ�ENQӦ��		
		},
		{
			PHASE_S3,
			EVENT_ENQ,						// �յ�ENQ
			mtxDummy,						// ��������ֱ�ӽ���ȴ�����ENQ������a��״̬
			{ PHASE_S3, PHASE_S4 }			// �ȴ�����ENQ������a
		},
		{
			PHASE_S3,
			EVENT_CHR,						// �յ�����
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�ENQӦ��
		},
		{
			PHASE_S3,
			EVENT_ACK,						// �յ�ACK
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�ENQӦ��
		},
		{
			PHASE_S3,
			EVENT_NAK,						// �յ�NAK
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�ENQӦ��
		},
		{
			PHASE_S3,
			EVENT_EOT,						// �յ�EOT
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�ENQӦ��
		},
		{
			PHASE_S3,
			EVENT_TIMEOUT,					// ��ʱ���ȴ�����ENQ����Ӧ���ȴ�ACK/NAK��ȴ�����EOT)
			mtxTimeOutEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�ENQӦ��
		},
		{
			PHASE_S3,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S3, PHASE_S3 }			// �ص����л�ȴ�ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 4, �ȴ�У��ENQӦ���е�����a��ģ�����ͶԷ��ֵ�ַ)
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S4,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }			// �ȴ�У��ENQӦ���е�����a	
		},
		{
			PHASE_S4,
			EVENT_ENQ,						// �յ�ENQ
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }			// �����ȴ�����a����
		},
		{
			PHASE_S4,
			EVENT_CHR,						// �յ�����
			mtxCheckDataA,	 				// У���Ƿ���������a��Ҫ��
			{ PHASE_S13, PHASE_S5 }			// ���ݴ������ȴ�У��ENQӦ���е�����b
		},
		{
			PHASE_S4,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,	 					// ������
			{ PHASE_S4, PHASE_S4 }			// �����ȴ�����a����
		},
		{
			PHASE_S4,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }			// �����ȴ�����a����
		},
		{
			PHASE_S4,
			EVENT_EOT,						// �յ�EOT
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }			// �����ȴ�����a����
		},
		{
			PHASE_S4,
			EVENT_TIMEOUT,					// ��ʱ���ȴ�����ENQ����Ӧ���е�����a)
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�����ENQӦ��
		},
		{
			PHASE_S4,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S4, PHASE_S4 }			// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 5, �ȴ�У��ENQӦ���е�����b�����ֵ�ַ�ʹ��ʹ�����
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S5,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S5, PHASE_S5 }			// �ȴ�У��ENQӦ���е�����b	
		},
		{
			PHASE_S5,
			EVENT_ENQ,						// �յ�ENQ
			mtxDummy,	 					// ������
			{ PHASE_S5, PHASE_S5 }			// �ȴ�У��ENQӦ���е�����b
		},
		{
			PHASE_S5,
			EVENT_CHR,						// �յ�����
			mtxCheckDataB,	 				// У��ENQӦ���е�����b
			{ PHASE_S13, PHASE_S6 }			// ���ݴ������ȴ�У���ı��ߴ�c/c1
		},
		{
			PHASE_S5,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,	 					// ������
			{ PHASE_S5, PHASE_S5 }			// �ȴ�У��ENQӦ���е�����b
		},
		{
			PHASE_S5,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,	 					// ������
			{ PHASE_S5, PHASE_S5 }			// �ȴ�У��ENQӦ���е�����b
		},
		{
			PHASE_S5,
			EVENT_EOT,						// �յ�EOT
			mtxDummy,	 					// ������
			{ PHASE_S5, PHASE_S5 }			// �ȴ�У��ENQӦ���е�����b 
		},
		{
			PHASE_S5,
			EVENT_TIMEOUT,					// ��ʱ
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�����ENQӦ��
		},
		{
			PHASE_S5,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S5, PHASE_S5 }			// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 6, �ȴ�У��ENQӦ���е�����c��c1���ı��ߴ����ݣ�
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S6,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S6, PHASE_S6 }			// �ȴ�У��ENQӦ���е�����c��c1	
		},
		{
			PHASE_S6,
			EVENT_ENQ,						// �յ�ENQ
			mtxCheckDataC1,	 				// ENQӦ���е��ı��ߴ�����c��c1ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�У��ENQӦ���е��ı��ߴ�����c2���ı�����
		},
		{
			PHASE_S6,
			EVENT_CHR,						// �յ�����
			mtxCheckDataC1,	 				// ENQӦ���е��ı��ߴ�����c��c1ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�У��ENQӦ���е��ı��ߴ�����c2���ı�����
		},
		{
			PHASE_S6,
			EVENT_ACK,						// �յ�ACK
			mtxCheckDataC1,	 				// ENQӦ���е��ı��ߴ�����c��c1ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�У��ENQӦ���е��ı��ߴ�����c2���ı�����
		},
		{
			PHASE_S6,
			EVENT_NAK,						// �յ�NAK
			mtxCheckDataC1,	 				// ENQӦ���е��ı��ߴ�����c��c1ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�У��ENQӦ���е��ı��ߴ�����c2���ı�����
		},
		{
			PHASE_S6,
			EVENT_EOT,						// �յ�EOT
			mtxCheckDataC1,	 				// ENQӦ���е��ı��ߴ�����c��c1ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�У��ENQӦ���е��ı��ߴ�����c2���ı�����
		},
		{
			PHASE_S6,
			EVENT_TIMEOUT,					// ��ʱ
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�����ENQӦ�� 
		},
		{
			PHASE_S6,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S6, PHASE_S6 }			// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 7, �ȴ�У��ENQӦ���е�����c2���ı��ߴ����ݣ�
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S7,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S7, PHASE_S7 }			// �ȴ�У��ENQӦ���е�����c2	
		},
		{
			PHASE_S7,
			EVENT_ENQ,						// �յ�ENQ
			mtxCheckDataC2,	 				// ENQӦ���е��ı��ߴ�����c2ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�����ENQӦ�������е��ı����ݻ����ݴ�����
		},
		{
			PHASE_S7,
			EVENT_CHR,						// �յ�����
			mtxCheckDataC2,	 				// ENQӦ���е��ı��ߴ�����c2ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�����ENQӦ�������е��ı����ݻ����ݴ�����
		},
		{
			PHASE_S7,
			EVENT_ACK,						// �յ�ACK
			mtxCheckDataC2,	 				// ENQӦ���е��ı��ߴ�����c2ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�����ENQӦ�������е��ı����ݻ����ݴ�����
		},
		{
			PHASE_S7,
			EVENT_NAK,						// �յ�NAK
			mtxCheckDataC2,	 				// ENQӦ���е��ı��ߴ�����c2ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�����ENQӦ�������е��ı����ݻ����ݴ�����
		},
		{
			PHASE_S7,
			EVENT_EOT,						// �յ�EOT
			mtxCheckDataC2,	 				// ENQӦ���е��ı��ߴ�����c2ȷ��
			{ PHASE_S8, PHASE_S13 }			// �ȴ�����ENQӦ�������е��ı����ݻ����ݴ�����
		},
		{
			PHASE_S7,
			EVENT_TIMEOUT,					// ��ʱ
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�����ENQӦ�� 
		},
		{
			PHASE_S7,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S7, PHASE_S7 }			// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 8, �ȴ�����ENQӦ���е��ı�����
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S8,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S8, PHASE_S8 }			// �ȴ�����ENQӦ���е��ı�����	
		},
		{
			PHASE_S8,
			EVENT_ENQ,						// �յ�ENQ
			mtxDummy,	 					// ������
			{ PHASE_S8, PHASE_S8 }			// �ȴ�����ENQӦ���е��ı�����
		},
		{
			PHASE_S8,
			EVENT_CHR,						// �յ�����
			mtxRecvContext,	 				// ����ENQӦ���е��ı�����
			{ PHASE_S8, PHASE_S9 }			// �������ջ�ȴ�BCCȷ��
		},
		{
			PHASE_S8,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,	 					// ������
			{ PHASE_S8, PHASE_S8 }			// �ȴ�����ENQӦ���е��ı�����
		},
		{
			PHASE_S8,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,	 					// ������
			{ PHASE_S8, PHASE_S8 }			// �ȴ�����ENQӦ���е��ı�����
		},
		{
			PHASE_S8,
			EVENT_EOT,						// �յ�EOT
			mtxDummy,	 					// ������
			{ PHASE_S8, PHASE_S8 }			// �ȴ�����ENQӦ���е��ı�����
		},
		{
			PHASE_S8,
			EVENT_TIMEOUT,					// ��ʱ
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�����ENQӦ�� 
		},
		{
			PHASE_S8,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S8, PHASE_S8 }			// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 9, �ȴ�����ENQӦ���е�BCC��λ1
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S9,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S9, PHASE_S9 }			// �ȴ�����ENQӦ���е�BCC��λ1	
		},
		{
			PHASE_S9,
			EVENT_ENQ,						// �յ�ENQ
			mtxDummy,	 					// ������
			{ PHASE_S9, PHASE_S9 }			// �ȴ�����ENQӦ���е�BCC��λ1
		},
		{
			PHASE_S9,
			EVENT_CHR,						// �յ�����
			mtxRecvBCC,	 					// ����BCC��һλ
			{ PHASE_S10, PHASE_S10 }		// �ȴ�����BCC�ڶ�λ��BCCȷ��
		},
		{
			PHASE_S9,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,	 					// ������
			{ PHASE_S9, PHASE_S9 }			// �ȴ�����ENQӦ���е�BCC��λ1
		},
		{
			PHASE_S9,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,	 					// ������
			{ PHASE_S9, PHASE_S9 }			// �ȴ�����ENQӦ���е�BCC��λ1
		},
		{
			PHASE_S9,
			EVENT_EOT,						// �յ�EOT
			mtxDummy,	 					// ������
			{ PHASE_S9, PHASE_S9 }			// �ȴ�����ENQӦ���е�BCC��λ1
		},
		{
			PHASE_S9,
			EVENT_TIMEOUT,					// ��ʱ
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�����ENQӦ�� 
		},
		{
			PHASE_S9,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S9, PHASE_S9 }			// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 10, �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S10,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S10, PHASE_S10 }		// �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��	
		},
		{
			PHASE_S10,
			EVENT_ENQ,						// �յ�ENQ
			mtxDummy,	 					// ������
			{ PHASE_S10, PHASE_S10 }		// �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��
		},
		{
			PHASE_S10,
			EVENT_CHR,						// �յ�����
			mtxCheckBCC,	 				// ����ENQӦ���е�BCC��λ2��BCCȷ��
			{ PHASE_S1, PHASE_S11 }			// �ص����л�ȴ�EOT
		},
		{
			PHASE_S10,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,	 					// ������
			{ PHASE_S10, PHASE_S10 }		// �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��
		},
		{
			PHASE_S10,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,	 					// ������
			{ PHASE_S10, PHASE_S10 }		// �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��
		},
		{
			PHASE_S10,
			EVENT_EOT,						// �յ�EOT
			mtxDummy,	 					// ������
			{ PHASE_S10, PHASE_S10 }		// �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��
		},
		{
			PHASE_S10,
			EVENT_TIMEOUT,					// ��ʱ���ȴ�����ENQ����Ӧ���ȴ�ACK/NAK��ȴ�����EOT)
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�����ENQӦ��  
		},
		{
			PHASE_S10,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S10, PHASE_S10 }		// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 11, ACK������ϣ��ȴ�����EOT
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S11,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S11, PHASE_S11 }		// ACK������ϣ��ȴ�����EOT	
		},
		{
			PHASE_S11,
			EVENT_ENQ,						// �յ�ENQ
			mtxRetrySndACK,					// �ط�ACK
			{ PHASE_S1, PHASE_S11 }		// �ȴ�����EOT
		},
		{
			PHASE_S11,
			EVENT_CHR,						// �յ�����
			mtxRetrySndACK,					// �ط�ACK
			{ PHASE_S1, PHASE_S11 }		// �ȴ�����EOT
		},
		{
			PHASE_S11,
			EVENT_ACK,						// �յ�ACK
			mtxRetrySndACK,					// �ط�ACK
			{ PHASE_S1, PHASE_S11 }		// �ȴ�����EOT
		},
		{
			PHASE_S11,
			EVENT_NAK,						// �յ�NAK
			mtxRetrySndACK,					// �ط�ACK
			{ PHASE_S1, PHASE_S11 }		// �ȴ�����EOT
		},
		{
			PHASE_S11,
			EVENT_EOT,						// �յ�EOT
			mtxSndEOT,						// ����EOT
			{ PHASE_S1, PHASE_S1 }			// �ص�����
		},
		{
			PHASE_S11,
			EVENT_TIMEOUT,					// ��ʱ
			mtxRetrySndACK,					// �ط�ACK
			{ PHASE_S1, PHASE_S11 }		// �ȴ�����EOT
		},
		{
			PHASE_S11,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S11, PHASE_S11 }		// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 12, NAK���ͺ���ٷ������ݵȴ�
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S12,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S12, PHASE_S12 }		// NAK���ͺ���ٷ������ݵȴ�	
		},
		{
			PHASE_S12,
			EVENT_ENQ,						// �յ�ENQ
			mtxDummy,						// �ط�NAK
			{ PHASE_S1, PHASE_S4 }			// �ص����л�ȴ�NAT���ͺ�����ݽ���
		},
		{
			PHASE_S12,
			EVENT_CHR,						// �յ�����
			mtxRetrySndNAK,					// �ط�NAK
			{ PHASE_S1, PHASE_S12 }			// �ص����л�ȴ�NAT���ͺ�����ݽ���
		},
		{
			PHASE_S12,
			EVENT_ACK,						// �յ�ACK
			mtxRetrySndNAK,					// �ط�NAK
			{ PHASE_S1, PHASE_S12 }			// �ص����л�ȴ�NAT���ͺ�����ݽ���
		},
		{
			PHASE_S12,
			EVENT_NAK,						// �յ�NAK
			mtxRetrySndNAK,					// �ط�NAK
			{ PHASE_S1, PHASE_S12 }			// �ص����л�ȴ�NAT���ͺ�����ݽ���
		},
		{
			PHASE_S12,
			EVENT_EOT,						// �յ�EOT
			mtxRetrySndNAK,					// �ط�NAK
			{ PHASE_S1, PHASE_S12 }			// �ص����л�ȴ�NAT���ͺ�����ݽ���
		},
		{
			PHASE_S12,
			EVENT_TIMEOUT,					// ��ʱ
			mtxRetrySndNAK,					// �ط�NAK
			{ PHASE_S1, PHASE_S12 }			// �ص����л�ȴ�NAT���ͺ�����ݽ���
		},
		{
			PHASE_S12,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S12, PHASE_S12 }		// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 13, �ȴ�����ENQӦ���е����ݽ��մ���
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S13,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S13, PHASE_S13 }		// �ȴ�����ENQӦ���е����ݽ��մ���	
		},
		{
			PHASE_S13,
			EVENT_ENQ,						// �յ�ENQ
			mtxDummy,						// ������
			{ PHASE_S13, PHASE_S13 }		// �ȴ�����ENQӦ���е����ݽ��մ���
		},
		{
			PHASE_S13,
			EVENT_CHR,						// �յ�����
			mtxDummy,						// ������
			{ PHASE_S13, PHASE_S13 }		// �ȴ�����ENQӦ���е����ݽ��մ���
		},
		{
			PHASE_S13,
			EVENT_ACK,						// �յ�ACK
			mtxDummy,	 					// ������
			{ PHASE_S13, PHASE_S13 }		// �ȴ�����ENQӦ���е����ݽ��մ���
		},
		{
			PHASE_S13,
			EVENT_NAK,						// �յ�NAK
			mtxDummy,						// ������
			{ PHASE_S13, PHASE_S13 }		// �ȴ�����ENQӦ���е����ݽ��մ���
		},
		{
			PHASE_S13,
			EVENT_EOT,						// �յ�EOT
			mtxDummy,						// ������
			{ PHASE_S13, PHASE_S13 }		// �ȴ�����ENQӦ���е����ݽ��մ���
		},
		{
			PHASE_S13,
			EVENT_TIMEOUT,					// ��ʱ���ȴ�����ENQ����Ӧ���ȴ�ACK/NAK��ȴ�����EOT)
			mtxRetryEnquire,				// �ط�ENQ
			{ PHASE_S1, PHASE_S3 }			// �ص����л�ȴ�����ENQӦ�� 
		},
		{
			PHASE_S13,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S13, PHASE_S13 }		// �ص����л�ȴ�����ENQӦ��
		}
	},
		//--------------------------------------------------------------------------*
		//  Phase 14, EOT������ϣ��ȴ�����EOT
		//--------------------------------------------------------------------------*
	{
		{
			PHASE_S14,
			EVENT_REQSEND,					// ��������
			mtxDummy,						// ������
			{ PHASE_S14, PHASE_S14 }		// EOT������ϣ��ȴ�����EOT
		},
		{
			PHASE_S14,
			EVENT_ENQ,						// �յ�ENQ
			mtxRetrySndEOT,					// �ط�EOT 
			{ PHASE_S1, PHASE_S14 }			// �ص����л�ȴ�����EOT
		},
		{
			PHASE_S14,
			EVENT_CHR,						// �յ�����
			mtxRetrySndEOT,	 			    // ������
			{ PHASE_S14, PHASE_S14 }		// EOT������ϣ��ȴ�����EOT
		},
		{
			PHASE_S14,
			EVENT_ACK,						// �յ�ACK
			mtxRetrySndEOT,					// �ط�EOT 
			{ PHASE_S1, PHASE_S14 }			// �ص����л�ȴ�����EOT
		},
		{
			PHASE_S14,
			EVENT_NAK,						// �յ�NAK
			mtxRetrySndEOT,					// �ط�EOT 
			{ PHASE_S1, PHASE_S14 }			// �ص����л�ȴ�����EOT
		},
		{
			PHASE_S14,
			EVENT_EOT,						// �յ�EOT
			mtxDummy,						// ������
			{ PHASE_S1, PHASE_S1 }			// ����
		},
		{
			PHASE_S14,
			EVENT_TIMEOUT,					// ��ʱ���ȴ�����EOT)
			mtxRetrySndEOT,					// �ط�EOT 
			{ PHASE_S1, PHASE_S14 }			// �ص����л�ȴ�����EOT
		},
		{
			PHASE_S14,
			EVENT_QUERY,					// ������ѯ
			mtxDummy,						// ������
			{ PHASE_S14, PHASE_S14 }		// �ص����л�ȴ�����ENQӦ��
		}
	}
};

//--------------------------------------------------------------------------------------------
//	end of file