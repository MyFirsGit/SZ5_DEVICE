#pragma once
#include "DllExp.h"
#include "mtxfunc.h"

#ifndef _connect_param
#define _connect_param
// CH�������Ӳ����ṹ
typedef struct _connect_param
{
	DWORD ComPort;			// ���ں�
	DWORD Speed;			// ������		
	DWORD MaxSendCnt;		// ���Ͷ�����Ϣ�������ֵ
	DWORD MaxSendBuffSize;	// �������ݻ�������󳤶�
	DWORD MaxRecvCnt;		// ���ն�����Ϣ�������ֵ
	DWORD MaxRecvBuffSize;	// �������ݻ�������󳤶�
} CONNECT_PARAM, *LPCONNECT_PARAM;
#endif

#define	MAX_BUFFER_SIZE	3000

//===========================================================
//Note: CHTRS = Media Issue Machine Transaction
//===========================================================
#define	 ST_SNDQUE	0x00000001L	// ���д��ڴ�����������
#define	 ST_RCVQUE	0x00000002L	// ���д��ڴ�����������
#define	 ST_SNDEXE	0x00000004L	// ����æ�����ڷ�������
#define	 ST_RCVEXE	0x00000008L	// ����æ�����ڽ�������
#define	 ST_SNDERR	0x00000010L	// ����ʧ��
#define	 ST_RCVERR	0x00000020L	// ����ʧ�� 
#define	 ST_RCVOVR	0x00000080L	// ���ջ������

#define	 ST_DTR	0x00010000L	///< DTR
#define	 ST_DSR	0x00020000L	///< DSR
#define	 ST_RTS	0x00040000L	///< RTS
#define	 ST_CTS	0x00080000L	///< CTS

#define STATUS_ID_EXECUTE		0	///< Status of library, such as: sending, receiving
#define STATUS_ID_SEND_QUEUE	1	///< The remain data in the sending queue
#define STATUS_ID_RECV_QUEUE	2	///< Received data in the receiving queue

#define STATUS_PHASE_1			0	///< Waiting for DLE ENQ
#define STATUS_PHASE_2			1	///< Waiting for DLE STX
#define STATUS_PHASE_3			2	///< Waiting for DLE ETX
#define STATUS_PHASE_4			3	///< Waiting for DLE ETX
#define STATUS_PHASE_5			4	///< Waiting for DLE ACK (after send DLE ENQ)
#define STATUS_PHASE_6			5	///< Waiting for DLE ACk (after send DLE STX .. DLE ETX BCC)

typedef DWORD COMM_RESULT;
typedef DWORD COMM_HANDLE;        // CH���Ӷ�����

#define	 RES_OK				0L	// �����ɹ�
#define	 RES_FAIL			1L	// �����ɹ�

#ifndef _err_detail
#define _err_detail
typedef enum _err_detail {
    ERR_WINERR           = 1001, // ϵͳ����

    ERR_HANDLE           = 1003, // The handle is invalid.
    ERR_CHREAD       = 1100, // Could not create thread. Window error.
    ERR_TIMEOUT      = 1101, // Timeout. Could not finished task in time.
    ERR_RESET            = 1102, // Operation was terminate because of reseting.
    ERR_STRANGE          = 1103, // Unrecognizable response.
    ERR_RECV_EMPTY           = 1104, // There is no data to receive.

    HTDL_ERR_FULL        = 1201, // Could not open connection because the conneciton pool is full
    HTDL_ERR_OPNTO       = 1202, // The operation could not finish in time.
    HTDL_ERR_WORKNG      = 1203, // Could not allocate workspace, out of memory.
    HTDL_ERR_SQUENG      = 1204, // Could not allocate the queue, out of memory.
    HTDL_ERR_RBUFNG      = 1206, // Could not allocate buffer for receiving data, out of memory.
    HTDL_ERR_SENDFAIL    = 1207, // Sending error. Unknow reason.
    HTDL_ERR_PORT        = 1208, // Could not open the specified serial port.
    HTDL_ERR_CTSOFF      = 1209, // Serial port does not support CTS (clear-to-send) event
    HTDL_ERR_SQUE        = 1210, // ���Ͷ�����
    HTDL_ERR_RQUE        = 1211, // Too many items are queueing to be received

    MTX_ERR_ENQUIRE      = 1300, // ENQ����ʧ�ܣ����ܷ��ͱ���
    MTX_ERR_SEND         = 1301, // ����������Ȼ����ʧ��

    RFID_A_SUM_ERR       = 1500, // Ʊ��A��RFID����SUMֵ�쳣
    RFID_B_SUM_ERR       = 1501, // Ʊ��B��RFID����SUMֵ�쳣
    RFID_A_B_SUM_ERR     = 1502, // Ʊ��A��B��RFID����SUMֵ�쳣

    MSG_EMPTY	        = 1350, // The message is empty now.
    MSG_OVRFLW	        = 1351, // The buffer is too small to store message.
    CMD_INVALID	        = 1352, // Unsupported command.
    CMD_PARAM	        = 1400, // Invalid parameter.
    CMD_REJECT	        = 1401  // Too many commands are queuing, could not send command.
}ERR_DETAIL;
#endif

#define SWAPWORD(wValue) (WORD)((wValue << 8) | (wValue >> 8))

// ��CHͨ������
DLLIMP COMM_RESULT COMM_Open(OUT COMM_HANDLE* pHandle,IN  void* pParam);

// �ر�CHͨ������
DLLIMP COMM_RESULT COMM_Close(OUT COMM_HANDLE Handle);

// ���������CH
DLLIMP COMM_RESULT COMM_Send(IN  COMM_HANDLE Handle,IN  DWORD nSendLength,IN  void* pBuffer,OUT DWORD* pOutLength);

// ����CH��������
DLLIMP COMM_RESULT COMM_Recv(IN COMM_HANDLE Handle,IN DWORD nBuffLength,OUT void* pBuffer,OUT DWORD* pRecvLength);

// ����CHͨ�Ŵ���
DLLIMP COMM_RESULT COMM_Reset(IN COMM_HANDLE Handle,IN BOOL bSendClear,IN BOOL bRecvClear);

// ��ȡ��ǰͨ��״̬
DLLIMP COMM_RESULT COMM_GetStat(IN COMM_HANDLE Handle,OUT DWORD* pStatus);

// ��ʼ����־
DLLIMP void CTSPTrs_Initialize(const char* tracePath);

// ��ӡ��־
DLLIMP void CTSPTrs_Trace(const char* lpszFormat,...);

// �����־�ļ�
DLLIMP void CTSPTrs_Clear();

// �ر���־����
DLLIMP void CTSPTrs_Close();