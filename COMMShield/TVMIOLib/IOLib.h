#pragma once
#include "IOLibExp.h"

// �������Ӳ����ṹ
typedef struct _io_connect_param
{
	DWORD ComPort;				// ���ں�
	DWORD Speed;						// ������		
	DWORD MaxSendCnt;			// ���Ͷ�����Ϣ�������ֵ
	DWORD MaxSendBuffSize;	// �������ݻ�������󳤶�
	DWORD MaxRecvCnt;			// ���ն�����Ϣ�������ֵ
	DWORD MaxRecvBuffSize;	// �������ݻ�������󳤶�
} IOTRS_CONNECT_PARAM, *LPIOTRS_CONNECT_PARAM;

#define	MAX_BUFFER_SIZE	3000

//===========================================================
//Note: IOTRS = Media Issue Machine Transaction
//===========================================================
#define	IOTRS_ST_SNDQUE	0x00000001L	// ���д��ڴ�����������
#define	IOTRS_ST_RCVQUE	0x00000002L	// ���д��ڴ�����������
#define	IOTRS_ST_SNDEXE		0x00000004L	// ����æ�����ڷ�������
#define	IOTRS_ST_RCVEXE		0x00000008L	// ����æ�����ڽ�������
#define	IOTRS_ST_SNDERR	0x00000010L	// ����ʧ��
#define	IOTRS_ST_RCVERR		0x00000020L	// ����ʧ�� 
#define	IOTRS_ST_RCVOVR	0x00000080L	// ���ջ������

#define	IOTRS_ST_DTR	0x00010000L	//< DTR
#define	IOTRS_ST_DSR	0x00020000L	//< DSR
#define	IOTRS_ST_RTS	0x00040000L	//< RTS
#define	IOTRS_ST_CTS	0x00080000L	//< CTS

#define STATUS_ID_EXECUTE		0	//< Status of library, such as: sending, receiving
#define STATUS_ID_SEND_QUEUE	1	//< The remain data in the sending queue
#define STATUS_ID_RECV_QUEUE	2	//< Received data in the receiving queue

#define STATUS_PHASE_1			0	///< Waiting for DLE ENQ
#define STATUS_PHASE_2			1	///< Waiting for DLE STX
#define STATUS_PHASE_3			2	///< Waiting for DLE ETX
#define STATUS_PHASE_4			3	///< Waiting for DLE ETX
#define STATUS_PHASE_5			4	///< Waiting for DLE ACK (after send DLE ENQ)
#define STATUS_PHASE_6			5	///< Waiting for DLE ACk (after send DLE STX .. DLE ETX BCC)

typedef DWORD IOTRS_RESULT;
typedef DWORD IOTRS_HANDLE;        // ���Ӷ�����

#define	IOTRS_RES_OK				0L			// �����ɹ�

typedef enum _io_err_detail {
    SET_PARAM_ERR				= 1,	// �趨ֵ�������쳣
    FUNC_PARAM_ERR         = 2,	// ���������쳣
    SEQUENCE_ERR	            = 3,	// ��������˳���쳣
    FUNC_ERR_RETURN	        = 4,	// ���������쳣
    LOGIC_ERR						= 5,	// �߼��쳣
    //OTHER_ERR						= 6,  // ��������          

    IOTRS_RES_WINERR           = 1001, // ϵͳ����

    IOTRS_ERR_HANDLE           = 1003, // The handle is invalid.
    IOTRS_RES_ERR_THREAD       = 1100, // Could not create thread. Window error.
    IOTRS_RES_ERR_TIMEOUT      = 1101, // Timeout. Could not finished task in time.
    IOTRS_RES_RESET            = 1102, // Operation was terminate because of reseting.
    IOTRS_RES_STRANGE          = 1103, // Unrecognizable response.
    IOTRS_RECV_EMPTY           = 1104, // There is no data to receive.

    IOTRS_HTDL_ERR_FULL        = 1201, // Could not open connection because the conneciton pool is full
    IOTRS_HTDL_ERR_OPNTO       = 1202, // The operation could not finish in time.
    IOTRS_HTDL_ERR_WORKNG      = 1203, // Could not allocate workspace, out of memory.
    IOTRS_HTDL_ERR_SQUENG      = 1204, // Could not allocate the queue, out of memory.
    IOTRS_HTDL_ERR_RBUFNG      = 1206, // Could not allocate buffer for receiving data, out of memory.
    IOTRS_HTDL_ERR_SENDFAIL    = 1207, // Sending error. Unknow reason.
    IOTRS_HTDL_ERR_PORT        = 1208, // Could not open the specified serial port.
    IOTRS_HTDL_ERR_CTSOFF      = 1209, // Serial port does not support CTS (clear-to-send) event
    IOTRS_HTDL_ERR_SQUE        = 1210, // ���Ͷ�����
    IOTRS_HTDL_ERR_RQUE        = 1211, // Too many items are queueing to be received

    IOTRS_MTX_ERR_ENQUIRE      = 1300, // ENQ����ʧ�ܣ����ܷ��ͱ���
    IOTRS_MTX_ERR_SEND         = 1301, // ����������Ȼ����ʧ��

    IOTRS_RFID_A_SUM_ERR       = 1500, // Ʊ��A��RFID����SUMֵ�쳣
    IOTRS_RFID_B_SUM_ERR       = 1501, // Ʊ��B��RFID����SUMֵ�쳣
    IOTRS_RFID_A_B_SUM_ERR     = 1502, // Ʊ��A��B��RFID����SUMֵ�쳣

    IOTRS_MSG_EMPTY	        = 1350, // The message is empty now.
    IOTRS_MSG_OVRFLW	        = 1351, // The buffer is too small to store message.
    IOTRS_CMD_INVALID	        = 1352, // Unsupported command.
    IOTRS_CMD_PARAM	        = 1400, // Invalid parameter.
    IOTRS_CMD_REJECT	        = 1401  // Too many commands are queuing, could not send command.
}IO_ERR_DETAIL;

#define SWAPWORD(wValue) (WORD)((wValue << 8) | (wValue >> 8))

// ��ͨ������
DLLIMP IOTRS_RESULT IOTrs_Open(OUT IOTRS_HANDLE* pHandle,IN  void* pParam);

// �ر�ͨ������
DLLIMP IOTRS_RESULT IOTrs_Close(OUT IOTRS_HANDLE Handle);

// ������������
DLLIMP IOTRS_RESULT IOTrs_Send(IN  IOTRS_HANDLE Handle,IN  DWORD nSendLength,IN  void* pBuffer,OUT DWORD* pOutLength);

// ���շ�������
DLLIMP IOTRS_RESULT IOTrs_Recv(IN IOTRS_HANDLE Handle,IN DWORD nBuffLength,OUT void* pBuffer,OUT DWORD* pRecvLength);

// ����ͨ�Ŵ���
DLLIMP IOTRS_RESULT IOTrs_Reset(IN IOTRS_HANDLE Handle,IN BOOL bSendClear,IN BOOL bRecvClear);

// ��ȡ��ǰͨ��״̬
DLLIMP IOTRS_RESULT IOTrs_GetStat(IN IOTRS_HANDLE Handle,OUT DWORD* pStatus);

// ��ʾ��������Ϣ
DLLIMP void IOTrs_ShowLastError(HWND hParent);

// ��ӡ��־
DLLIMP void IOTrs_Trace(const char* lpszFormat, ...);

// �����־�ļ�
DLLIMP void IOTrs_Clear();

// ��ʼ����־�ļ�
DLLIMP void IOTrs_Initialize(const char* tracePath);

// �ر�IO��־����
DLLIMP void ReleaseIOLog();
