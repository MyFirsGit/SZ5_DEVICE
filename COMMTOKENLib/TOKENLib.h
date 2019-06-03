#pragma once
#include "stdafx.h"
#include "TOKENLibExp.h"

#define	MAX_BUFFER_SIZE	3000

//===========================================================
//Note: TOKENTRS = Media Issue Machine Transaction
//===========================================================
#define	TOKENTRS_ST_SNDQUE	0x00000001L	// ���д��ڴ�����������
#define	TOKENTRS_ST_RCVQUE	0x00000002L	// ���д��ڴ�����������
#define	TOKENTRS_ST_SNDEXE	0x00000004L	// ����æ�����ڷ�������
#define	TOKENTRS_ST_RCVEXE	0x00000008L	// ����æ�����ڽ�������
#define	TOKENTRS_ST_SNDERR	0x00000010L	// ����ʧ��
#define	TOKENTRS_ST_RCVERR	0x00000020L	// ����ʧ�� 
#define	TOKENTRS_ST_RCVOVR	0x00000080L	// ���ջ������

#define	TOKENTRS_ST_DTR	0x00010000L	///< DTR
#define	TOKENTRS_ST_DSR	0x00020000L	///< DSR
#define	TOKENTRS_ST_RTS	0x00040000L	///< RTS
#define	TOKENTRS_ST_CTS	0x00080000L	///< CTS

typedef DWORD TOKENTRS_RESULT;
typedef DWORD TOKENTRS_HANDLE;        // ���Ӷ�����

#define	TOKENTRS_RES_OK				0L	// ����Ӧ��ɹ�

//
typedef enum _token_err_detail {
	TOKEN_SET_PARAM_ERR            = 1,	// �趨ֵ�������쳣
	TOKEN_FUNC_PARAM_ERR           = 2,	// ���������쳣
	TOKEN_SEQUENCE_ERR	            = 3,	// ��������˳���쳣
	TOKEN_FUNC_ERR_RETURN	        = 4,	// ���������쳣
	TOKEN_LOGIC_ERR	            = 5,	// �߼��쳣
	TOKEN_OTHER_ERR                = 6,    // ������ش���          

	TOKENTRS_RES_WINERR           = 1001, // ϵͳ����

	TOKENTRS_ERR_HANDLE           = 1003, // The handle is invalid.
	TOKENTRS_RES_ERR_THREAD       = 1100, // Could not create thread. Window error.
	TOKENTRS_RES_ERR_TIMEOUT      = 1101, // Timeout. Could not finished task in time.
	TOKENTRS_RES_RESET            = 1102, // Operation was terminate because of reseting.
	TOKENTRS_RES_STRANGE          = 1103, // Unrecognizable response.
	TOKENTRS_RECV_EMPTY           = 1104, // There is no data to receive.
	TOKENTRS_RECV_ERROR           = 1105, // There is no data to receive.


	TOKENTRS_HTDL_ERR_FULL        = 1201, // Could not open connection because the conneciton pool is full
	TOKENTRS_HTDL_ERR_OPNTO       = 1202, // The operation could not finish in time.
	TOKENTRS_HTDL_ERR_WORKNG      = 1203, // Could not allocate workspace, out of memory.
	TOKENTRS_HTDL_ERR_SQUENG      = 1204, // Could not allocate the queue, out of memory.
	TOKENTRS_HTDL_ERR_RBUFNG      = 1206, // Could not allocate buffer for receiving data, out of memory.
	TOKENTRS_HTDL_ERR_SENDFAIL    = 1207, // Sending error. Unknow reason.
	TOKENTRS_HTDL_ERR_PORT        = 1208, // Could not open the specified serial port.
	TOKENTRS_HTDL_ERR_CTSOFF      = 1209, // Serial port does not support CTS (clear-to-send) event
	TOKENTRS_HTDL_ERR_SQUE        = 1210, // ���Ͷ�����
	TOKENTRS_HTDL_ERR_RQUE        = 1211, // Too many items are queueing to be received

	TOKENTRS_MTX_ERR_SEND         = 1301, // ����������Ȼ����ʧ��

	TOKENTRS_MSG_EMPTY	        = 1350, // The message is empty now.
	TOKENTRS_MSG_OVRFLW	        = 1351, // The buffer is too small to store message.
	TOKENTRS_CMD_INVALID	    = 1352, // Unsupported command.
	TOKENTRS_CMD_PARAM	        = 1400, // Invalid parameter.
	TOKENTRS_CMD_REJECT	        = 1401  // Too many commands are queuing, could not send command.

}TOKEN_ERR_DETAIL;

