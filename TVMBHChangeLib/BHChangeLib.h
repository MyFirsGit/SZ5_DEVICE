#pragma once
#include "BHLibExp.h"


// BHCHANGE Command
#define BH_CHANGE_GET_STATUS		0x01	// ȡ״̬
#define BH_CHANGE_INIT				0x02	// ��ʼ��
#define BH_CHANGE_COUNT				0x03	// ����

#define BH_CHANGE_TRANSPORTATION	0x05	// ���͸��˿�
#define BH_CHANGE_RETRIEVAL			0x06	// ���͵�������

#define BH_CHANGE_COUNT_TRAN_FRONT	0x09	// ���Ҳ����͵�ǰ��
#define BH_CHANGE_COUNT_TRAN_REAR	0x0A	// ���Ҳ����͵����

#define BH_CHANGE_DEVICE_INFO		0x0E	// �豸��Ϣ��ѯ��д��
#define BH_CHANGE_DIAGNOSIS			0x0F	// ���
#define BH_CHANGE_CANCEL			0x10	// �жϲ���
#define BH_CHANGE_CHECK_COUNT		0x11	// ���Ҳ��Զ������α������
#define BH_CHANGE_SENSOR_DETECTION	0x14	// ���������

// CONTROL Command
#define BH_CTRL_REQUEST				0x60	// ��������
#define BH_CTRL_RESPONSE			0xE0	// ��������
#define BH_CTRL_ERR_RESPONSE		0xF0	// ����Ӧ��
#define BH_CTRL_END_OF_COMMOND		0x1C	// ���������־

// BH�������Ӳ����ṹ
typedef struct _mim_bh_connect_param_
{
	DWORD ComPort;			// ���ں�
	DWORD Speed;			// ������		
	DWORD MaxSendCnt;		// ���Ͷ�����Ϣ�������ֵ
	DWORD MaxSendBuffSize;	// �������ݻ�������󳤶�
	DWORD MaxRecvCnt;		// ���ն�����Ϣ�������ֵ
	DWORD MaxRecvBuffSize;	// �������ݻ�������󳤶�
} BHCHANGETRS_CONNECT_PARAM, *LPBHCHANGETRS_CONNECT_PARAM;

#define	MAX_BUFFER_SIZE	3000

//===========================================================
//Note: BHTRS = Media Issue Machine Transaction
//===========================================================
#define	BHTRS_ST_SNDQUE	0x00000001L	// ���д��ڴ�����������
#define	BHTRS_ST_RCVQUE	0x00000002L	// ���д��ڴ�����������
#define	BHTRS_ST_SNDEXE	0x00000004L	// ����æ�����ڷ�������
#define	BHTRS_ST_RCVEXE	0x00000008L	// ����æ�����ڽ�������
#define	BHTRS_ST_SNDERR	0x00000010L	// ����ʧ��
#define	BHTRS_ST_RCVERR	0x00000020L	// ����ʧ�� 
#define	BHTRS_ST_RCVOVR	0x00000080L	// ���ջ������

#define	BHTRS_ST_DTR	0x00010000L	///< DTR
#define	BHTRS_ST_DSR	0x00020000L	///< DSR
#define	BHTRS_ST_RTS	0x00040000L	///< RTS
#define	BHTRS_ST_CTS	0x00080000L	///< CTS

#define STATUS_ID_EXECUTE		0	///< Status of library, such as: sending, receiving
#define STATUS_ID_SEND_QUEUE	1	///< The remain data in the sending queue
#define STATUS_ID_RECV_QUEUE	2	///< Received data in the receiving queue

#define STATUS_PHASE_1			0	///< Waiting for DLE ENQ
#define STATUS_PHASE_2			1	///< Waiting for DLE STX
#define STATUS_PHASE_3			2	///< Waiting for DLE ETX
#define STATUS_PHASE_4			3	///< Waiting for DLE ETX
#define STATUS_PHASE_5			4	///< Waiting for DLE ACK (after send DLE ENQ)
#define STATUS_PHASE_6			5	///< Waiting for DLE ACk (after send DLE STX .. DLE ETX BCC)

typedef DWORD BHTRS_RESULT;
typedef DWORD BHTRS_HANDLE;        // BH���Ӷ�����

#define	BHTRS_RES_OK				0L	// �����ɹ�

//typedef enum _bh_err_detail_ {
#define    BH_SET_PARAM_ERR            1	// �趨ֵ�������쳣
#define    BH_FUNC_PARAM_ERR           2	// ���������쳣
#define    BH_SEQUENCE_ERR	           3	// ��������˳���쳣
#define    BH_FUNC_ERR_RETURN	       4	// ���������쳣
#define    BH_LOGIC_ERR	               5	// �߼��쳣
#define    BH_OBHER_ERR                6    // FTP��ش���          

#define    BHTRS_RES_WINERR            1001 // ϵͳ����

#define    BHTRS_ERR_HANDLE            1003 // The handle is invalid.
#define    BHTRS_RES_ERR_BHREAD        1100 // Could not create thread. Window error.
#define    BHTRS_RES_ERR_TIMEOUT       1101 // Timeout. Could not finished task in time.
#define    BHTRS_RES_RESET             1102 // Operation was terminate because of reseting.
#define    BHTRS_RES_STRANGE           1103 // Unrecognizable response.
#define    BHTRS_RECV_EMPTY            1104 // There is no data to receive.

#define    BHTRS_HTDL_ERR_FULL         1201 // Could not open connection because the conneciton pool is full
#define    BHTRS_HTDL_ERR_OPNTO        1202 // The operation could not finish in time.
#define    BHTRS_HTDL_ERR_WORKNG       1203 // Could not allocate workspace, out of memory.
#define    BHTRS_HTDL_ERR_SQUENG       1204 // Could not allocate the queue, out of memory.
#define    BHTRS_HTDL_ERR_RBUFNG       1206 // Could not allocate buffer for receiving data, out of memory.
#define    BHTRS_HTDL_ERR_SENDFAIL     1207 // Sending error. Unknow reason.
#define    BHTRS_HTDL_ERR_PORT         1208 // Could not open the specified serial port.
#define    BHTRS_HTDL_ERR_CTSOFF       1209 // Serial port does not support CTS (clear-to-send) event
#define    BHTRS_HTDL_ERR_SQUE         1210 // ���Ͷ�����
#define    BHTRS_HTDL_ERR_RQUE         1211 // Too many items are queueing to be received

#define    BHTRS_MTX_ERR_ENQUIRE       1300 // ENQ����ʧ�ܣ����ܷ��ͱ���
#define    BHTRS_MTX_ERR_SEND          1301 // ����������Ȼ����ʧ��

#define    BHTRS_RFID_A_SUM_ERR        1500 // Ʊ��A��RFID����SUMֵ�쳣
#define    BHTRS_RFID_B_SUM_ERR        1501 // Ʊ��B��RFID����SUMֵ�쳣
#define    BHTRS_RFID_A_B_SUM_ERR      1502 // Ʊ��A��B��RFID����SUMֵ�쳣

#define    BHTRS_MSG_EMPTY			   1350 // The message is empty now.
#define    BHTRS_MSG_OVRFLW	           1351 // The buffer is too small to store message.
#define    BHTRS_CMD_INVALID	       1352 // Unsupported command.
#define    BHTRS_CMD_PARAM	           1400 // Invalid parameter.
#define    BHTRS_CMD_REJECT	           1401  // Too many commands are queuing, could not send command.
//}BH_CHANGE_ERR_DETAIL;

#define SWAPWORD(wValue) (WORD)((wValue << 8) | (wValue >> 8))

// ��BHͨ������
DLLIMP BHTRS_RESULT BHTrs_Open(OUT BHTRS_HANDLE* pHandle,IN  void* pParam);

// �ر�BHͨ������
DLLIMP BHTRS_RESULT BHTrs_Close(OUT BHTRS_HANDLE Handle);

// ���������BH
DLLIMP BHTRS_RESULT BHTrs_Send(IN  BHTRS_HANDLE Handle,IN  DWORD nSendLength,IN  void* pBuffer,OUT DWORD* pOutLength);

// ����BH��������
DLLIMP BHTRS_RESULT BHTrs_Recv(IN BHTRS_HANDLE Handle,IN DWORD nBuffLength,OUT void* pBuffer,OUT DWORD* pRecvLength);

// ����BHͨ�Ŵ���
DLLIMP BHTRS_RESULT BHTrs_Reset(IN BHTRS_HANDLE Handle,IN BOOL bSendClear,IN BOOL bRecvClear);

// ��ȡ��ǰͨ��״̬
DLLIMP BHTRS_RESULT BHTrs_GetStat(IN BHTRS_HANDLE Handle,OUT DWORD* pStatus);

// ��ʾ��������Ϣ
DLLIMP void BHTrs_ShowLastError(HWND hParent);

// ��ʼ��BH��־�ļ�
DLLIMP void BHChangeTrs_Initialize(const char* tracePath);

// ��ӡBH��־
DLLIMP void BHTrs_Trace(const char* lpszFormat, ...);

// ���BH��־�ļ�
DLLIMP void BHTrs_Clear();

// �ر���־
DLLIMP void BHChange_CloseLog();



