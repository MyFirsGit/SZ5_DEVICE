#include <afxtempl.h>
#include <vector>
#include <map>
#include "commondefine.h"

using namespace std;

#ifndef _SCAPI_H_
#define _SCAPI_H_

//ʱ�䶨��

#define RECONNECT_INTERVAL                  60
//������Դ���
#define MAX_RETRY						3
//�����Ϣ����
#define RECEIVED_BUFF_SIZE 8*1024
//���� 
#define SC_CONNECTION_OFF				0
#define SC_SOCKET_ON							1
#define SC_CONNECTION_ON				2

#define SC_MSG_SEND_TO_SC								0x6100
#define SC_MSG_CONNECT                                     0x6101 //����ʱ��
#define SC_MSG_CLOSEOPERATION					0x6102 //������Ӫʱ��
#define SC_MSG_RECONNECT								0x6103 //����
#define SC_MSG_ENQLINK										0x6104 //����
#define SC_MSG_DATAMSG                                     0x6105 //��λ��������
#define SC_MSG_DOWNLOAD_IMMIDIATE_PARAMETER  0x6107 //�������ؼ�ʱ����

#define SC_MSG_SEND_ERR									0x6108
#define SC_MSG_SEND_FULLSTATUS				0x6109
#define SC_MSG_SEND_DEVICE_ERR									0x6108
#define SC_MSG_SEND_DEVICE_FULLSTATUS				0x6109
#define SC_MSG_STARTOPERATION				0x6110				//��ʼ��Ӫʱ��
#define WM_SC_CLOSE_COMPLETE  (SP_USER + 105)

// �����ֵ
enum {
	SC_ERR_CREATE_SOCKET		    = 0x7001,	/// Could not create Socket
	SC_ERR_SOCKET_OPEN			    = 0x7002,	/// The connection to socket server could not be opened
	SC_ERR_SEND_BIND_REQUEST	    = 0x7003,	/// Failed to send Bind Request message
	SC_ERR_SEND_UNBIND_REQUEST	    = 0x7004,	/// Failed to send Unbind Request message	        
	SC_ERR_EXCEED_LIMIT				= 0x7005,   /// Length of sent data is out of limit             
	SC_ERR_WRONG_RESPONSE_ID		= 0x7006,                                                       
	SC_ERR_NOT_FINISH_CMD		    = 0x7010,                                                       
	SC_ERR_NOT_RESPONSE				= 0x7011,	// CPS doesn't response
	SC_ERR_PARSE_DATA			    = 0x7012,
	SC_ERR_GENERIC_NACK				= 0x7013,	// CPS receive 
	SC_ERR_CERTIFICATE_FAIL			= 0x7014,	// ��֤ʧ��
	SC_ERR_GET_SESSIONKEY_FAIL		= 0x7015,	// ��ȡTH��Կʧ��
	SC_FTP_IN_CONNECT				= 0x7016,	// ����������FTP����
	SC_ERR_RSP_DATA					= 0x7017,   // �յ���response��������
	SC_ERR_INVALID_SEQUENCE = 0x7018,// ���������
	SC_ERR_INTERUPTED = 0x7019,
	SC_ERR_INVLID_CONTENT = 0x7020,
};


#define LEN_START_TAG 1
#define LEN_END_TAG 1
#define LEN_CMD_HEADER 38
#define LEN_NTID	16
#define LEN_CRC 4
#define LEN_START_TAG_CMD_HEADER  (LEN_START_TAG+LEN_CMD_HEADER)
#define LEN_START_TAG_CMD_HEADER_END_TAG (LEN_START_TAG+LEN_CMD_HEADER+LEN_END_TAG)
#define LEN_CRC_END_TAG (LEN_CRC+LEN_END_TAG)
#define LEN_PACKAGE_LEAST (LEN_START_TAG+LEN_CMD_HEADER+LEN_END_TAG)

#define  OFFSET_CRC_FLAG 37
#define  OFFSET_PACKAGE_COUNT 31
#define  OFFSET_PACKAGE_INDEX 32
#define  OFFSET_PACKAGE_LENGTH 1

#define  START_TAG                 0xF0  // ��ʼ���
#define END_TAG                    0xFF   // �������

typedef enum _transfer_command_code{ 
	CMD_CONNECT_REQUEST       = 0x01,  // ������֤����
	CMD_CONNECT_REQUEST_REPLY = 0x02,  // ����������
	CMD_DATA_TRANSFER         = 0x03,  // ���ݴ���
	CMD_DATA_TRANSFER_REPLY   = 0xFD,  // ���ݴ��ݷ���
	CMD_ENQUIRE_LINK          = 0x08,  // ���Ӵ��ȷ��
	CMD_ENQUIRE_LINK_REPLY    = 0xFC,  // ���Ӵ���
	CMD_GENERIC_ACK           = 0xFB,  // ��Ϣ��ȷӦ��
	CMD_GENERIC_NACK          = 0x00,  // ��Ϣ����Ӧ��
}TRANSFER_COMMAND_CODE;

// ��������
typedef enum _transfer_data_type{
	PROTOCAL_DATA    = 0x00, // Э������
	RESERVE_DATA     = 0x01, // Ԥ������
	TRANSACTION_DATA = 0x02, // ��������
	OPER_DATA        = 0x03, // ҵ������
	CONTROL_DATA     = 0x04, // ��������
	STATUS_DATA      = 0x05  // ״̬����
}TRANSFER_DATA_TYPE;

// Error Reason
typedef enum _error_reason
{
	INVALID_PDU_LENGTH		= 0x01,	// command length + body length
	INVALID_MSG_LENGTH		= 0x02,
	UNKNOWN_COMMAND_ID		= 0x03,
	UNKNOWN_MSG_ID			= 0x04,
	MISTAKE_MSG_ID			= 0x05,
	EXECUTE_ERROR			= 0x06,
	ABNORMAL_DEFINITION		= 0x07,
	CRC_CHECK_ERROR			= 0x09,
	INVALID_DATA			= 0x0A,
	KEY_ERROR               = 0x0B
}ERROR_REASON;

// SC NACKԭ�����
typedef enum _sc_nack_code{
	PROTOCAL_INVALID        = 0x01, // Э���ʶ����	0x0001
	PROTOCAL_VER_INVALID    = 0x02, // Э��汾�Ŵ���	0x0002
	DATA_FORMATVER_INVALIDE = 0x03, // ���ݸ�ʽ�汾�Ŵ���	0x0003
	PROTOCAL_CMD_INVALIDE   = 0x04, // Э���������	0x0004
	DATA_TYPE_INVALID       = 0x05, // �������ͱ������	0x0005
	CRC_ALG_INVALID         = 0x06, // У�����㷨����	0x0006
	CRC_DATA_INVALIE        = 0x07, // У�������	0x0007
	BLOCK_DATA_INVALIDE     = 0x08, // �ְ����ݴ���	0x0008
	ENCRYPT_ALG_INVALIDE    = 0x09, // �����㷨����	0x0009
	KEY_VER_INVALIDE        = 0x0A, // ��Կ�汾����	0x000A
	SAME_MSG_ERR            = 0x0B, // �ظ���Ϣ	0x000B
	MSG_LEN_INVALIDE        = 0xFD, // ���峤�ȴ���	0x00FD
	OTHER_ERR               = 0xFE  // ��������	0x00FE
}SC_NACK_CODE;




// NETWORK_TRANS_ID  ��ϢΨһ��ʶ     16 BIN   
typedef struct _network_trans_id{
	BYTE bCmdID;                   // 1 BIN  ����Э������
	_DATE_TIME dtTimeStamp;        // 7 BCD ʱ���
	WORD wMsgNo;                   // 2 BIN ��Ϣ���к�
	DEVICE_ID lDeviceID;           // 4 1BIN+2BCD+1BIN �豸ID
	NW_TRANS_ID_GRP scGroup;       // 2 BIN �豸������
	_network_trans_id(){
		bCmdID = 0;
		wMsgNo = 0;
	}
	void Serialize(LPBYTE lpByte)
	{
		*lpByte=bCmdID;
		lpByte++;
		Time2BCD(dtTimeStamp,lpByte);
		lpByte+=7;
		*lpByte = LOBYTE(wMsgNo);
		lpByte++;
		*lpByte=HIBYTE(wMsgNo);
		lpByte++;
		lDeviceID.Serialize(lpByte);
		lpByte+=sizeof(lDeviceID);
		scGroup.Serialize(lpByte);
		lpByte+=sizeof(scGroup);
	}
	void Deserialize(LPBYTE lpByte)
	{
		bCmdID = *lpByte;
		lpByte++;
		dtTimeStamp = BCD2Time((char*)lpByte);
		lpByte+=7;
		wMsgNo = MAKEWORD(*lpByte,*(lpByte+1));
		lpByte+=2;
		lDeviceID.Deserialize(lpByte);
		lpByte+=sizeof(lDeviceID);
		scGroup.Deserialize(lpByte);
		lpByte+=sizeof(scGroup);
	}
}NETWORK_TRANS_ID;

// CMD_HEADER ��Ϣ��������ͷ          38 BIN
typedef struct _cmd_header 
{
	DWORD lMsgLen;                  // 4 BIN ��Ϣ����
	DWORD lProtoclIdentify;         // 4 BIN Э���ʶ
	BYTE bProtoclVersion;           // 1 BIN Э��汾
	BYTE bDataFormatVer;            // 1 BIN ���ݸ�ʽ�汾
	BYTE bDataTransType;            // 1 BIN ���ݴ�������
	BYTE bReserved1;                // 1 BIN Ԥ��
	NETWORK_TRANS_ID nwTransId;     // 16 BIN ��ϢΨһ��ʶ
	WORD wLifeCycle;                // 2 BIN �Ự����
	BYTE bBlockCnt;                 // 1 BIN �ְ�����
	BYTE bBlockNo;                  // 1 BIN �ְ����
	BYTE bFrontHeader;              // 1 BIN ǰ�ô���ͷ��ʾ
	BYTE bCipherflag;               // 1 BIN �����㷨
	BYTE bCipherAddLen;             // 1 BIN �����㷨��λ����
	BYTE bCipherVer;                // 1 BIN ��Կ�汾
	BYTE bCrcType;                  // 1 BIN У�����㷨
	BYTE bReserved2;                // 1 BIN Ԥ��

	_cmd_header()
	{
		lMsgLen = 0x00;
		lProtoclIdentify = 0x01;
		bProtoclVersion = 0x01;
		bDataFormatVer = 0x01;
		bDataTransType = 0x00;
		bReserved1 = 0xFF;
		wLifeCycle = 0x00;
		bBlockCnt = 0x01;
		bBlockNo = 0x01;
		bFrontHeader = 0x00;
		bCipherflag = 0x00;
		bCipherAddLen = 0x00;
		bCipherVer = 0x00;
		bCrcType = 0x01;
		bReserved2 = 0xFF;
	}

	void Serialize(LPBYTE lpByte)
	{
		memcpy(lpByte,&lMsgLen,sizeof(lMsgLen));
		lpByte+=sizeof(lMsgLen);
		memcpy(lpByte,&lProtoclIdentify,sizeof(lProtoclIdentify));
		lpByte+=sizeof(lProtoclIdentify);
		*lpByte = bProtoclVersion;
		lpByte+=sizeof(bProtoclVersion);
		*lpByte=bDataFormatVer;
		lpByte+=sizeof(bDataFormatVer);
		*lpByte = bDataTransType;
		lpByte+=sizeof(bDataTransType);
		*lpByte = bReserved1;
		lpByte+=sizeof(bReserved1);
		nwTransId.Serialize(lpByte);
		lpByte+=sizeof(nwTransId);
		*lpByte=LOBYTE(wLifeCycle);
		*(lpByte+1) = HIBYTE(wLifeCycle);
		lpByte+=sizeof(wLifeCycle);
		*lpByte = bBlockCnt;
		lpByte+=sizeof(bBlockCnt);
		*lpByte = bBlockNo;
		lpByte+=sizeof(bBlockNo);
		*lpByte = bFrontHeader;
		lpByte+=sizeof(bFrontHeader);
		*lpByte = bCipherflag;
		lpByte+=sizeof(bCipherflag);
		*lpByte = bCipherAddLen;
		lpByte+=sizeof(bCipherAddLen);
		*lpByte = bCipherVer;
		lpByte+=sizeof(bCipherVer);
		*lpByte = bCrcType;
		lpByte+=sizeof(bCrcType);
		*lpByte = bReserved2;
		lpByte+=sizeof(bReserved2);
	}

	void Deserialize(LPBYTE lpByte)
	{
		memcpy(&lMsgLen,lpByte,sizeof(lMsgLen));
		lpByte+=sizeof(lMsgLen);
		memcpy(&lProtoclIdentify,lpByte,sizeof(lProtoclIdentify));
		lpByte+=sizeof(lProtoclIdentify);
		bProtoclVersion=*lpByte;
		lpByte+=sizeof(bProtoclVersion);
		bDataFormatVer=*lpByte;
		lpByte+=sizeof(bDataFormatVer);
		bDataTransType=*lpByte ;
		lpByte+=sizeof(bDataTransType);
		bReserved1=*lpByte;
		lpByte+=sizeof(bReserved1);
		nwTransId.Deserialize(lpByte);
		lpByte+=sizeof(nwTransId);
		wLifeCycle = MAKEWORD(*lpByte,*(lpByte+1));
		lpByte+=sizeof(wLifeCycle);
		bBlockCnt=*lpByte;
		lpByte+=sizeof(bBlockCnt);
		bBlockNo=*lpByte;
		lpByte+=sizeof(bBlockNo);
		bFrontHeader=*lpByte;
		lpByte+=sizeof(bFrontHeader);
		bCipherflag=*lpByte;
		lpByte+=sizeof(bCipherflag);
		bCipherAddLen=*lpByte;
		lpByte+=sizeof(bCipherAddLen);
		bCipherVer=*lpByte;
		lpByte+=sizeof(bCipherVer);
		bCrcType=*lpByte;
		lpByte+=sizeof(bCrcType);
		bReserved2=*lpByte;
		lpByte+=sizeof(bReserved2);
	}

}CMD_HEADER;

#endif