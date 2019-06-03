#include <afxtempl.h>
#include <vector>
#include <map>
#include "commondefine.h"

using namespace std;

#ifndef _SCAPI_H_
#define _SCAPI_H_

//时间定义

#define RECONNECT_INTERVAL                  60
//最大重试次数
#define MAX_RETRY						3
//最大消息长度
#define RECEIVED_BUFF_SIZE 8*1024
//连接 
#define SC_CONNECTION_OFF				0
#define SC_SOCKET_ON							1
#define SC_CONNECTION_ON				2

#define SC_MSG_SEND_TO_SC								0x6100
#define SC_MSG_CONNECT                                     0x6101 //开机时序
#define SC_MSG_CLOSEOPERATION					0x6102 //结束运营时序
#define SC_MSG_RECONNECT								0x6103 //重连
#define SC_MSG_ENQLINK										0x6104 //心跳
#define SC_MSG_DATAMSG                                     0x6105 //上位请求数据
#define SC_MSG_DOWNLOAD_IMMIDIATE_PARAMETER  0x6107 //请求下载即时参数

#define SC_MSG_SEND_ERR									0x6108
#define SC_MSG_SEND_FULLSTATUS				0x6109
#define SC_MSG_SEND_DEVICE_ERR									0x6108
#define SC_MSG_SEND_DEVICE_FULLSTATUS				0x6109
#define SC_MSG_STARTOPERATION				0x6110				//开始运营时序
#define WM_SC_CLOSE_COMPLETE  (SP_USER + 105)

// 命令返回值
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
	SC_ERR_CERTIFICATE_FAIL			= 0x7014,	// 认证失败
	SC_ERR_GET_SESSIONKEY_FAIL		= 0x7015,	// 获取TH密钥失败
	SC_FTP_IN_CONNECT				= 0x7016,	// 开机序列中FTP下载
	SC_ERR_RSP_DATA					= 0x7017,   // 收到的response解析错误
	SC_ERR_INVALID_SEQUENCE = 0x7018,// 错误的序列
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

#define  START_TAG                 0xF0  // 开始标记
#define END_TAG                    0xFF   // 结束标记

typedef enum _transfer_command_code{ 
	CMD_CONNECT_REQUEST       = 0x01,  // 连接认证请求
	CMD_CONNECT_REQUEST_REPLY = 0x02,  // 连接请求反馈
	CMD_DATA_TRANSFER         = 0x03,  // 数据传递
	CMD_DATA_TRANSFER_REPLY   = 0xFD,  // 数据传递反馈
	CMD_ENQUIRE_LINK          = 0x08,  // 连接存活确认
	CMD_ENQUIRE_LINK_REPLY    = 0xFC,  // 连接存活反馈
	CMD_GENERIC_ACK           = 0xFB,  // 消息正确应答
	CMD_GENERIC_NACK          = 0x00,  // 消息错误应答
}TRANSFER_COMMAND_CODE;

// 数据类型
typedef enum _transfer_data_type{
	PROTOCAL_DATA    = 0x00, // 协议数据
	RESERVE_DATA     = 0x01, // 预留数据
	TRANSACTION_DATA = 0x02, // 交易数据
	OPER_DATA        = 0x03, // 业务数据
	CONTROL_DATA     = 0x04, // 控制数据
	STATUS_DATA      = 0x05  // 状态数据
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

// SC NACK原因代码
typedef enum _sc_nack_code{
	PROTOCAL_INVALID        = 0x01, // 协议标识错误	0x0001
	PROTOCAL_VER_INVALID    = 0x02, // 协议版本号错误	0x0002
	DATA_FORMATVER_INVALIDE = 0x03, // 数据格式版本号错误	0x0003
	PROTOCAL_CMD_INVALIDE   = 0x04, // 协议命令错误	0x0004
	DATA_TYPE_INVALID       = 0x05, // 数据类型编码错误	0x0005
	CRC_ALG_INVALID         = 0x06, // 校验码算法错误	0x0006
	CRC_DATA_INVALIE        = 0x07, // 校验码错误	0x0007
	BLOCK_DATA_INVALIDE     = 0x08, // 分包数据错误	0x0008
	ENCRYPT_ALG_INVALIDE    = 0x09, // 加密算法错误	0x0009
	KEY_VER_INVALIDE        = 0x0A, // 密钥版本错误	0x000A
	SAME_MSG_ERR            = 0x0B, // 重复消息	0x000B
	MSG_LEN_INVALIDE        = 0xFD, // 包体长度错误	0x00FD
	OTHER_ERR               = 0xFE  // 其它错误	0x00FE
}SC_NACK_CODE;




// NETWORK_TRANS_ID  消息唯一标识     16 BIN   
typedef struct _network_trans_id{
	BYTE bCmdID;                   // 1 BIN  传输协议命令
	_DATE_TIME dtTimeStamp;        // 7 BCD 时间戳
	WORD wMsgNo;                   // 2 BIN 消息序列号
	DEVICE_ID lDeviceID;           // 4 1BIN+2BCD+1BIN 设备ID
	NW_TRANS_ID_GRP scGroup;       // 2 BIN 设备分组编号
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

// CMD_HEADER 消息传输数据头          38 BIN
typedef struct _cmd_header 
{
	DWORD lMsgLen;                  // 4 BIN 消息长度
	DWORD lProtoclIdentify;         // 4 BIN 协议标识
	BYTE bProtoclVersion;           // 1 BIN 协议版本
	BYTE bDataFormatVer;            // 1 BIN 数据格式版本
	BYTE bDataTransType;            // 1 BIN 数据传输类型
	BYTE bReserved1;                // 1 BIN 预留
	NETWORK_TRANS_ID nwTransId;     // 16 BIN 消息唯一标识
	WORD wLifeCycle;                // 2 BIN 会话周期
	BYTE bBlockCnt;                 // 1 BIN 分包总数
	BYTE bBlockNo;                  // 1 BIN 分包序号
	BYTE bFrontHeader;              // 1 BIN 前置处理头表示
	BYTE bCipherflag;               // 1 BIN 加密算法
	BYTE bCipherAddLen;             // 1 BIN 加密算法补位长度
	BYTE bCipherVer;                // 1 BIN 密钥版本
	BYTE bCrcType;                  // 1 BIN 校验码算法
	BYTE bReserved2;                // 1 BIN 预留

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