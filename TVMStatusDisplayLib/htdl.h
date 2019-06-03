/*****************************************************************************/
/* FILE NAME     : htdl.h                                                    */
/* PROGRAM NAME  : Header file of comm server.                               */
/* FUNCTION      : 1.                                                        */
/*                                                                           */
/*****************************************************************************/

// ͨ�ŷ�ʽ����
#define	COM_PORT	0				// ����ͨ��
#define MAIL_SLOT	1				// �ʲ�ͨ��
#define NAME_PIPE	2				// �ܵ�ͨ��

// ͨ�Ž׶ζ���
#define	MAX_PHASE	3			// ͨ��״̬����
#define PHASE_S1		0				// Phase 1, ���У��ȴ�������¼�
#define PHASE_S2		1				// Phase 2, �ȴ�����STX
#define PHASE_S3		2				// Phase 3, �ȴ�����ETX

// ͨ���¼�ID����
#define MAX_EVENT		5			// All availables events				
#define EVENT_REQSEND	0			//
//#define EVENT_ENQ		1			// Ԥ��
#define EVENT_STX		1			// 
#define EVENT_CHR		2			//
#define EVENT_ETX		3			// 
#define EVENT_TIMEOUT	4			// 
#define EVENT_ERROR		5			// 
#define EVENT_NONE		0xFFFF		// 

// ͨ�ſ����붨��
#define STX				0xa5	//���ݿ�ʼ�ı�־
#define ETX				0x5a//���ݽ����ı�־

// ���ֵ����
#define MAX_MSG_LEN		1024			// ���ݿ����󳤶�
#define MAX_RECVBUFF    1024		// DLE, STX, Seq, BlockNo, 2*MsgSize, DLE, ETX, BCC (4 + 256 + 3)
#define MAX_SENDBUFF    1024		// DLE, STX, Seq, BlockNo, 2*MsgSize, DLE, ETX, BCC (4 + 256 + 3)
#define MAX_RETRYSEND	3			// ������Դ���


#pragma pack(push) // �������״̬
#pragma pack(1)   // �趨Ϊ1�ֽڶ���

// ����������ṹ
typedef struct send_item_tag
{
	HANDLE				hEvent;			// ��������¼�
	COMM_RESULT*		pResult;		// �������
	DWORD				nSendLength;	// ׼�����͵��ֽ���
	BYTE*				pCommand;		// ׼�����͵�����ָ��
	BYTE*				pSending;		// ���ݷ��͵���ʼλ��ָ��
	WORD				wLastSendingLen;// ���һ�η��͵������ֽڳ���
	DWORD*				pOutLength;		// �ѷ��������ֽڳ���
    send_item_tag()
    {
        hEvent = NULL;
        pResult = NULL;
        nSendLength = 0;
        pCommand = NULL;
        pSending = NULL;
        wLastSendingLen = 0;
        pOutLength = NULL;
    }
} SENDITEM, *PSENDITEM;

// ����������ṹ
typedef struct recv_item_tag
{
	HANDLE				hEvent;			// ��������¼�
	COMM_RESULT*		pResult;		// �������
	DWORD				nBuffSize;		// �������ݻ���Ĵ�С
	BYTE*				pBuffer;		// �������ݻ���ָ��
	DWORD*				pRecvLength;	// ���յ����ݳ���
    recv_item_tag()
    {
        hEvent = NULL;
        pResult = NULL;
        nBuffSize = 0;
        pBuffer = NULL;
        pRecvLength = NULL;
    }
} RECVITEM, *PRECVITEM;

// ���Ͷ��нṹ
typedef struct send_queue_tag
{
	int					nMax;			// ���Ͷ������Ԫ�ظ���
	int					nCnt;			// ��ǰ����Ԫ�ظ���
	int					nWP;			// Ԫ��д���α�
	int					nRP;			// Ԫ�ض�ȡ�α�
	SENDITEM			Que[1];			// ��������������
    send_queue_tag()
	{
        nMax = 0;
        nCnt = 0;
        nWP  = 0;
        nRP  = 0;
        memset(Que,0x00,sizeof(SENDITEM)*1);
    }
} SEND_QUEUE, *PSEND_QUEUE;

// ���ն��нṹ
typedef struct recv_queue_tag
{
	int					nMax;			// ���ն������Ԫ�ظ���
	int					nCnt;			// ��ǰ����Ԫ�ظ���
	int					nWP;			///< Position of the writing slot in the queue
	int					nRP;			///< Position of the reading slot in the queue
	RECVITEM			Que[1];			// ��������������
    recv_queue_tag() {
        nMax = 0;
        nCnt = 0;
        nWP  = 0;
        nRP  = 0;
        memset(Que,0x00,sizeof(RECVITEM)*1);
    }
} RECV_QUEUE, *PRECV_QUEUE;

// ����������ṹ
typedef struct buff_item_tag
{
//	int					nWritten;		///< Number of written bytes
	int					nWP;			///< Position of the writing slot
	BYTE*				pData;			///< Pointer to a buffer storing data
	buff_item_tag() {
		nWP = 0;
		pData = NULL;
	}
} BUFFITEM, *PBUFFITEM;

// ���ݻ�����нṹ
typedef struct buff_queue_tag
{
	int					nMaxBuffSize;	// ���ݻ�����󳤶�
	int					nMax;			// �������Ԫ�ظ���
	int					nCnt;			// ��ǰ����Ԫ�ظ���
	int					nWP;			///< Position of the writing slot in the queue
	int					nRP;			///< Position of the reading slot in the queue
	BUFFITEM			Que[1];			///< Array of buffer item
    buff_queue_tag()
    {
        nMaxBuffSize = 0;
        nMax         = 0;
        nCnt         = 0;
        nWP          = 0;
        nRP          = 0;
        memset(Que,0x00,sizeof(BUFFITEM)*1);
    }
} BUFF_QUEUE, *PBUFF_QUEUE;

// �����ռ�ṹ
typedef struct htdl_work_tag
{
	HANDLE hThread;						// �����ռ��߳̾��
	HANDLE hSync;						// Э���߳�����ͬ���¼�
	HANDLE hClose;						// Э���̹߳ر��¼�
	BYTE ComSpeed;						// �����ʷ���(0:С��19200;1:����19200)
	WORD Phase;							// ͨ�Ž׶�
	WORD Event;							// ͨ���¼�

    //--------------------Receiving Buffer--------------------
	UINT RecvCnt;						// ���յ����ݳ��� Number of received byte
	BYTE *RecvPtr;						// Writing position of the buffer
	BYTE RecvBuff[MAX_RECVBUFF];		// ���ݽ��ջ��� Temporary buffer storing receiving frame
	BYTE c;								// �����յ��ֽ�
	//BYTE SeqRecv;						// �������к�
	//BYTE LastSeqRecv;					// ��һ�εĽ������к�
	//BYTE BlockNumRecv;					// ���տ���� 0x00:�޷ָ�� 0x80:��һ�� 0x81:�ڶ��� ....0x0N:���һ��
	//BYTE LastBlockNumRecv;				// Last Block Number of receiving
    //--------------------------------------------------------
    
	//--------------------Sending Buffer----------------------
	BYTE SendBuff[MAX_SENDBUFF];		// �������ݻ���(һ�����ݿ�)
	WORD SendLength;					// �������ݵĳ���

	BYTE SendTemp[MAX_SENDBUFF];
	//BYTE SeqSend;						// �������к�(0x01-0xFF)
	//BYTE BlockNumSend;					// ���Ϳ���� 0x00:�޷ָ�� 0x80:��һ�� 0x81:�ڶ��� ....0x0N:���һ��
	//--------------------------------------------------------

	bool ReqSend;						// 
	
	int RetryCnt;						// ���Դ���

	HANDLE				hFinSend;		// ���������¼�
	HANDLE				hFrameSend;		// UNUSED

	HANDLE				hMutex;			// �����ռ����ͬ���ź���
	HANDLE				hComm;			// Handle of the opened port

	DWORD				DrvStat;		// �����ռ��״̬ Status of the workspace (driver)
	
	SENDITEM			SndItem;		// The item is going to be sent (get from queue)
	SEND_QUEUE*			pSndQue;		// �������ݶ���
	RECVITEM			RcvItem;		// The item is currently storing receiving 
	RECV_QUEUE*			pRcvQue;		// �������ݶ���

	BUFF_QUEUE*			pBuffQue;		//

	COMM_RESULT		nError;			//

	BOOL	bNewENQ;					// It's just received ENQ
	DWORD BaseTime;						//
	DWORD TimeOut;						//
	
	BYTE	bCommunicationMethod;		// ͨ�ŷ�ʽ 0:���� 1:�ʲ� 2:�ܵ� 
	BYTE	SendSlotName[MAX_PATH];		//
	BYTE	ReceiveSlotName[MAX_PATH];	//
	HANDLE	hSimObject;						//
	BYTE	MailBuff[MAX_RECVBUFF];		//
    htdl_work_tag()
    {
        memset(SendBuff,0x00,MAX_SENDBUFF);	
		memset(SendTemp,0x00,MAX_SENDBUFF);
        SendLength = 0;									
        ReqSend = false;										
        RetryCnt = 0;		
        hFinSend = NULL;		
        hFrameSend = NULL;		
        hMutex = NULL;			
        hComm = NULL;			
        DrvStat = 0;		
        pSndQue = NULL;	
        pRcvQue = NULL;		
        pBuffQue = NULL;		
        nError = 0;		
        bNewENQ = false;					
        BaseTime = 0;						
        TimeOut = 0;						
        bCommunicationMethod = COM_PORT;
        memset(SendSlotName,0x00,MAX_PATH);
        memset(ReceiveSlotName,0x00,MAX_PATH);
        hSimObject = NULL;
        memset(MailBuff,0x00,MAX_RECVBUFF);
    }
}HTDLWORK, *PHTDLWORK;

// ͨ�Ž׶ο��ƴ���ṹ
struct matrix_tag 
{
	int phase;										// ͨ�Ž׶�
	int event;										// ͨ���¼�												
	int (*func)(HTDLWORK *, struct matrix_tag *);	// �¼�������
	int next[2];									// ��һ��ͨ�Ž׶� { FAIL, SUCCESS }
};

// �ָ�����״̬
#pragma pack(pop)  

// �Զ���ͨ�Ž׶ο��ƴ���ṹ����
typedef struct matrix_tag TMATRIX;		

// ͨ�Ž׶ο��ƴ���ṹ���鶨��
extern TMATRIX tbl_matrix[MAX_PHASE][MAX_EVENT];

// MATRIX of monitor time
extern DWORD wiTimeOut[2][MAX_PHASE];			

extern DWORD WINAPI HtdlThread(void* b);

// �Ӵ��ڻ��ʲ۶�ȡ����
extern BOOL ReadHtdl(HANDLE hStream,BYTE *pBuffer,DWORD dwLength,DWORD *dwBytesRead,BYTE bCommunicationMethod,HANDLE hSimObject,BYTE *pMailBuffer);

// ���·���״̬
extern void UpdateStatus(HTDLWORK *pWork, DWORD dwStatus, BOOL bOn);

// ����һ�����ݿ�
extern BOOL RecvData(IN HTDLWORK *pWork, IN BYTE *data, IN int cnt);

