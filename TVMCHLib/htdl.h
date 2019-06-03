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
#define	MAX_PHASE	14			// ͨ��״̬����
#define PHASE_S1		0				// Phase 1, ����,�ȴ����ݷ���
#define PHASE_S2		1				// Phase 2, ���ݷ�����ϣ��ȴ�ACK / NAKӦ��
#define PHASE_S3		2				// Phase 3, ��������ѯ����������ϣ��ȴ�ENQӦ��
#define PHASE_S4		3				// Phase 4, �ȴ�У��ENQӦ���е�����a��ģ�����ͶԷ��ֵ�ַ��
#define PHASE_S5		4				// Phase 5, �ȴ�У��ENQӦ���е�����b�����ֵ�ַ�ʹ��ʹ�����
#define PHASE_S6		5				// Phase 6, �ȴ�У��ENQӦ���е�����c��c1���ı��ߴ����ݣ�
#define PHASE_S7		6				// Phase 7, �ȴ�У��ENQӦ���е�����c2���ı��ߴ����ݣ�
#define PHASE_S8		7				// Phase 8, �ȴ�����ENQӦ���е��ı�����
#define PHASE_S9		8				// Phase 9, �ȴ�����ENQӦ���е�BCC��λ1
#define PHASE_S10		9				// Phase 10, �ȴ�����ENQӦ���е�BCC��λ2��BCCȷ��
#define PHASE_S11		10				// Phase 11, ACK������ϣ��ȴ�����EOT
#define PHASE_S12		11				// Phase 12, NAK������ϣ��ٴεȴ���������
#define PHASE_S13		12				// Phase 13, �ȴ�����ENQӦ���е����ݽ��մ���
#define PHASE_S14		13				// Phase 14, EOT������ϣ��ȴ�����EOT

// ͨ���¼�ID����
#define MAX_EVENT		8			// All availables events				
#define EVENT_REQSEND	0			//
#define EVENT_ENQ		1			// 
#define EVENT_CHR		2			//
#define EVENT_ACK		3			// 
#define EVENT_NAK		4			//
#define EVENT_EOT		5			//
#define EVENT_TIMEOUT	6			// 
#define EVENT_QUERY		7			// ������ѯ�¼�
#define EVENT_ERROR		8			// 
#define EVENT_NONE		0xFFFF		// 

// ͨ�ſ����붨��
#define DLE				0x10		// ������·ת���
#define STX				0x02		// ���Ŀ�ʼ�����
#define ETX				0x03		// ���Ľ��������
#define ENQ				0x05		// ����ѯ��
#define ACK				0x06		// ����ѯ�ʵĿ϶�Ӧ����
#define NAK				0x15		// ����ѯ�ʵķ�Ӧ����
#define EOT				0x04		// ͨ����ֹ��־

// ��λ->��Ԫ
// a 
#define INQUIRY			0x82		// ��ѯ
#define SEND_DATA2		0xC2		// �������ݣ�����2��
#define SEND_DATA3		0xD2		// �������ݣ�����3��
// b
#define SEND_B			0x01		// ����ʱB��ֵ

//  ��Ԫ->��λ
// a
#define INQUIRY_RSP		0x10		// ��ѯ����
#define RECV_DATA2		0x40		// �������ݿ飨����2��
#define RECV_DATA3		0x50		// �������ݿ飨����3��
// b
#define RECV_B			0x21		// ����ʱB��ֵ

// ���ֵ����
#define MAX_MSG_LEN		1152//256			// ���ݿ����󳤶�
#define MAX_RECVBUFF    1152//2*263			// DLE, STX, Seq, BlockNo, 2*MsgSize, DLE, ETX, BCC (4 + 256 + 3)
#define MAX_SENDBUFF    2*263		// DLE, STX, Seq, BlockNo, 2*MsgSize, DLE, ETX, BCC (4 + 256 + 3)
#define MAX_RETRYSEND	3			// ������Դ���

// ����
#define COMMAND_UNDEFINE		0x0000	// δ֪
#define COMMAND_SET_PARAMETER	0x0006	// �����趨
#define COMMAND_RESET			0x0007	// ��λ
#define COMMAND_READ_STATUS		0x000C	// ״̬��ȡ
#define COMMAND_LOG_DATA		0x000D	// LOG DataҪ��
#define COMMAND_DATA_LIGHT		0x0002	// Data Light
#define COMMAND_PDL				0x0003	// PDL ����
#define COMMAND_SET_INSERT		0x0020	// Ͷ�����
#define COMMAND_FORBID_INSERT	0x0021	// Ͷ���ֹ
#define COMMAND_TRAS_BEGIN_END	0x0030	// ���׿�ʼ������
#define COMMAND_TEMP_RETURN		0x0031	// �ݴ��˳�
#define COMMAND_COUNT_OUT		0x0032	// ���Ҽ���
#define COMMAND_TRANSFOR_OUT	0x0033	// Ӳ�Ҵ���
#define COMMAND_COLLOCTION		0x0040	// ����
#define COMMAND_ABORT			0x0044	// ֹͣ����



#pragma pack(push) // �������״̬
#pragma pack(1)   // �趨Ϊ1�ֽڶ���

// ����������ṹ
typedef struct send_item_tag
{
	HANDLE				hEvent;			// ��������¼�
	CHTRS_RESULT*		pResult;		// �������
	DWORD				nSendLength;	// ׼�����͵��ֽ���
	BYTE*				pCommand;		// ׼�����͵�����ָ��
	BYTE*				pSending;		// ���ݷ��͵���ʼλ��ָ��
	WORD				wLastSendingLen;// ���һ�η��͵������ֽڳ���
	DWORD*				pOutLength;		// �ѷ��������ֽڳ���
	WORD				wCommand;		// ����
    send_item_tag()
    {
        hEvent = NULL;
        pResult = NULL;
        nSendLength = 0;
        pCommand = NULL;
        pSending = NULL;
        wLastSendingLen = 0;
        pOutLength = NULL;
		wCommand = COMMAND_UNDEFINE;
    }
} SENDITEM, *PSENDITEM;

// ����������ṹ
typedef struct recv_item_tag
{
	HANDLE				hEvent;			// ��������¼�
	CHTRS_RESULT*		pResult;		// �������
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
	WORD RecvCnt;						// ���յ����ݳ��� Number of received byte
	BYTE *RecvPtr;						// Writing position of the buffer
	BYTE RecvBuff[MAX_RECVBUFF];		// ���ݽ��ջ��� Temporary buffer storing receiving frame
	BYTE c;								// �����յ��ֽ�
	WORD wNeedRecvCnt;					// Э������Ҫ���յ��ֽ�
	//BYTE SeqRecv;						// �������к�
	//BYTE LastSeqRecv;					// ��һ�εĽ������к�
	//BYTE BlockNumRecv;					// ���տ���� 0x00:�޷ָ�� 0x80:��һ�� 0x81:�ڶ��� ....0x0N:���һ��
	//BYTE LastBlockNumRecv;				// Last Block Number of receiving
    //--------------------------------------------------------
    
	//--------------------Sending Buffer----------------------
	BYTE SendBuff[MAX_SENDBUFF];		// �������ݻ���(һ�����ݿ�)
	WORD SendLength;					// �������ݵĳ���
	//BYTE SeqSend;						// �������к�(0x01-0xFF)
	//BYTE BlockNumSend;					// ���Ϳ���� 0x00:�޷ָ�� 0x80:��һ�� 0x81:�ڶ��� ....0x0N:���һ��
	//--------------------------------------------------------

	bool ReqSend;						// 
	bool Dle;							//
	
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

	CHTRS_RESULT		nError;			//

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
        SendLength = 0;					
// 		SeqSend = 0;						
// 		BlockNumSend = 0;					
        ReqSend = false;					
        Dle = false;						
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

