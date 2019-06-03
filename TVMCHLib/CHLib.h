#pragma once
#include "CHLibExp.h"

// CH�������Ӳ����ṹ
typedef struct _mim_ch_connect_param
{
	DWORD ComPort;			// ���ں�
	DWORD Speed;			// ������		
	DWORD MaxSendCnt;		// ���Ͷ�����Ϣ�������ֵ
	DWORD MaxSendBuffSize;	// �������ݻ�������󳤶�
	DWORD MaxRecvCnt;		// ���ն�����Ϣ�������ֵ
	DWORD MaxRecvBuffSize;	// �������ݻ�������󳤶�
} CHTRS_CONNECT_PARAM, *LPCHTRS_CONNECT_PARAM;



#define	MAX_BUFFER_SIZE	3000

// ��ʱ
#define TIME_OUT_20000			20000	// ��ʱʱ��20S
#define TIME_OUT_30000			30000	// ��ʱʱ��30S
#define TIME_OUT_60000			60000	// ��ʱʱ��60S
#define TIME_OUT_300000			300000	// ��ʱʱ��300S
//===========================================================
//Note: CHTRS = Media Issue Machine Transaction
//===========================================================
#define	CHTRS_ST_SNDQUE	0x00000001L	// ���д��ڴ�����������
#define	CHTRS_ST_RCVQUE	0x00000002L	// ���д��ڴ�����������
#define	CHTRS_ST_SNDEXE	0x00000004L	// ����æ�����ڷ�������
#define	CHTRS_ST_RCVEXE	0x00000008L	// ����æ�����ڽ�������
#define	CHTRS_ST_SNDERR	0x00000010L	// ����ʧ��
#define	CHTRS_ST_RCVERR	0x00000020L	// ����ʧ�� 
#define	CHTRS_ST_RCVOVR	0x00000080L	// ���ջ������

#define	CHTRS_ST_DTR	0x00010000L	///< DTR
#define	CHTRS_ST_DSR	0x00020000L	///< DSR
#define	CHTRS_ST_RTS	0x00040000L	///< RTS
#define	CHTRS_ST_CTS	0x00080000L	///< CTS

#define STATUS_ID_EXECUTE		0	///< Status of library, such as: sending, receiving
#define STATUS_ID_SEND_QUEUE	1	///< The remain data in the sending queue
#define STATUS_ID_RECV_QUEUE	2	///< Received data in the receiving queue

#define STATUS_PHASE_1			0	///< Waiting for DLE ENQ
#define STATUS_PHASE_2			1	///< Waiting for DLE STX
#define STATUS_PHASE_3			2	///< Waiting for DLE ETX
#define STATUS_PHASE_4			3	///< Waiting for DLE ETX
#define STATUS_PHASE_5			4	///< Waiting for DLE ACK (after send DLE ENQ)
#define STATUS_PHASE_6			5	///< Waiting for DLE ACk (after send DLE STX .. DLE ETX BCC)

typedef DWORD CHTRS_RESULT;
typedef DWORD CHTRS_HANDLE;        // CH���Ӷ�����

#define	CHTRS_RES_OK				0L	// �����ɹ�

typedef enum _ch_err_detail {
    CH_SET_PARAM_ERR            = 1,	// �趨ֵ�������쳣
    CH_FUNC_PARAM_ERR           = 2,	// ���������쳣
    CH_SEQUENCE_ERR	            = 3,	// ��������˳���쳣
    CH_FUNC_ERR_RETURN	        = 4,	// ���������쳣
    CH_LOGIC_ERR	            = 5,	// �߼��쳣
    CH_OTHER_ERR                = 6,    // FTP��ش���   


    CHTRS_RES_WINERR           = 1001, // ϵͳ����

    CHTRS_ERR_HANDLE           = 1003, // The handle is invalid.
    CHTRS_RES_ERR_CHREAD       = 1100, // Could not create thread. Window error.
    CHTRS_RES_ERR_TIMEOUT      = 1101, // Timeout. Could not finished task in time.
    CHTRS_RES_RESET            = 1102, // Operation was terminate because of reseting.
    CHTRS_RES_STRANGE          = 1103, // Unrecognizable response.
    CHTRS_RECV_EMPTY           = 1104, // There is no data to receive.

    CHTRS_HTDL_ERR_FULL        = 1201, // Could not open connection because the conneciton pool is full
    CHTRS_HTDL_ERR_OPNTO       = 1202, // The operation could not finish in time.
    CHTRS_HTDL_ERR_WORKNG      = 1203, // Could not allocate workspace, out of memory.
    CHTRS_HTDL_ERR_SQUENG      = 1204, // Could not allocate the queue, out of memory.
    CHTRS_HTDL_ERR_RBUFNG      = 1206, // Could not allocate buffer for receiving data, out of memory.
    CHTRS_HTDL_ERR_SENDFAIL    = 1207, // Sending error. Unknow reason.
    CHTRS_HTDL_ERR_PORT        = 1208, // Could not open the specified serial port.
    CHTRS_HTDL_ERR_CTSOFF      = 1209, // Serial port does not support CTS (clear-to-send) event
    CHTRS_HTDL_ERR_SQUE        = 1210, // ���Ͷ�����
    CHTRS_HTDL_ERR_RQUE        = 1211, // Too many items are queueing to be received

    CHTRS_MTX_ERR_ENQUIRE      = 1300, // ENQ����ʧ�ܣ����ܷ��ͱ���
    CHTRS_MTX_ERR_SEND         = 1301, // ����������Ȼ����ʧ��

    CHTRS_RFID_A_SUM_ERR       = 1500, // Ʊ��A��RFID����SUMֵ�쳣
    CHTRS_RFID_B_SUM_ERR       = 1501, // Ʊ��B��RFID����SUMֵ�쳣
    CHTRS_RFID_A_B_SUM_ERR     = 1502, // Ʊ��A��B��RFID����SUMֵ�쳣

    CHTRS_MSG_EMPTY	        = 1350, // The message is empty now.
    CHTRS_MSG_OVRFLW	        = 1351, // The buffer is too small to store message.
    CHTRS_CMD_INVALID	        = 1352, // Unsupported command.
    CHTRS_CMD_PARAM	        = 1400, // Invalid parameter.
    CHTRS_CMD_REJECT	        = 1401  // Too many commands are queuing, could not send command.
}CH_ERR_DETAIL;

#define SWAPWORD(wValue) (WORD)((wValue << 8) | (wValue >> 8))

// ��CHͨ������
DLLIMP CHTRS_RESULT CHTrs_Open(OUT CHTRS_HANDLE* pHandle,IN  void* pParam);

// �ر�CHͨ������
DLLIMP CHTRS_RESULT CHTrs_Close(OUT CHTRS_HANDLE Handle);

// ���������CH
DLLIMP CHTRS_RESULT CHTrs_Send(IN  CHTRS_HANDLE Handle,IN  DWORD nSendLength,IN  void* pBuffer,OUT DWORD* pOutLength);

// ����CH��������
DLLIMP CHTRS_RESULT CHTrs_Recv(IN CHTRS_HANDLE Handle,IN DWORD nBuffLength,OUT void* pBuffer,OUT DWORD* pRecvLength);

// ����CHͨ�Ŵ���
DLLIMP CHTRS_RESULT CHTrs_Reset(IN CHTRS_HANDLE Handle,IN BOOL bSendClear,IN BOOL bRecvClear);

// ��ȡ��ǰͨ��״̬
DLLIMP CHTRS_RESULT CHTrs_GetStat(IN CHTRS_HANDLE Handle,OUT DWORD* pStatus);

// ��ʾ��������Ϣ
DLLIMP void CHTrs_ShowLastError(HWND hParent);

// ��ӡCH��־
DLLIMP void CHTrs_Trace(const char* lpszFormat, ...);

// ���CH��־�ļ�
DLLIMP void CHTrs_Clear();

// ��ʼ��CH��־�ļ�
DLLIMP void CHTrs_Initialize(const char* tracePath);

// �ر���־����
DLLIMP void ReleaseCHLog();
