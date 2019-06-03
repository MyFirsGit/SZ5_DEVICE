#pragma once

#include "SCAPI.h"
#include "CTLOGCountInfo.h"


// BOMaudit��־
typedef enum _audit_flag
{
	OPERATER_LOGOUT  = 0x01,     // ����Ա�ǳ�
	OPERATER_DAY     = 0x02,     // ����Ա����
	END_OF_DAY       = 0x03      // �ս� 
}AUDIT_FLAG;

typedef enum _msg_code_transation
{
	MSG_TRANS_BOM_AR = 0x2402,//6.2.1.2BOM��Ʊ������ݶ���
	MSG_TRANS_BOM_REFOUND_TVM = 0x5001,//6.3.1BOM��TVM�����˿�
	MSG_TRANS_RECYLE_INVALID_CARD = 0x5005,//6.3.5��Ʊ���ս���
	MSG_TRANS_BOM_NONBUSINESS_INCOME = 0x5007,//6.3.5BOM��Ӫҵ����֧
	MSG_TRANS_NONIMMIDATELY_SURRENDER_APPLY = 0x0234,
	MSG_TRANS_NONIMMIDATELY_SURRENDER = 0x0235,
	MSG_TRANS_SIGNCARD_APPLY = 0x0237,
	MSG_TRANS_SIGNCARD_MODIFY = 0x0238,
	MSG_TRANS_REPORT_LOSS = 0x0239,
	MSG_TRANS_CANCEL_REPORT_LOSS = 0x0240,
	MSG_TRANS_ADMINISTRATION_PROCESS = 0x0241,
	MSG_TRANS_AUDIT = 0x0460,
} MSG_CODE_TRANS;

// ҵ�������������
typedef  enum _msg_code_bussiness{
	MSG_BUSSINESS_SOFTANDPROGRAM_UPDATE			 = 0x5102,   // 6.4.2  �����ͳ������
	MSG_BUSSINESS_SOFTANDPROGRAM_DOWNLOAD		 = 0x5103,   // 6.4.3  �����ͳ�������ҵ������
	MSG_BUSSINESS_DEVICE_IMPORTANT_STATUS_CHANGE = 0x5104,   // 6.4.4  �豸��Ҫ״̬�ı�	
	MSG_BUSSINESS_ISAM_LOGIN_LOGOUT              = 0x5105,   // 6.4.5  ISAMǩ��/ǩ��
	MSG_BUSSINESS_OPERATING_LOG					 = 0x5106,	 // 6.4.6  ������־
	MSG_BUSSINESS_DEVICE_ERROR					 = 0x5107,   // 6.4.7  �豸��������
	MSG_BUSSINESS_DEVICE_REGISTER				 = 0x5108,   // 6.4.8  �豸�Ĵ�������
	MSG_BUSSINESS_TICKETBOX_CHANGE				 = 0x510C,	 // 6.4.10 Ʊ�����
	MSG_BUSSINESS_DEVICE_COMPONENT				 = 0x5114,	 // 6.4.11 �豸��������
	MSG_BUSSINESS_ACCOUNT_LOCK					 = 0x5201,   // 6.5.1  �˻�����
	MSG_BUSSINESS_UDSN_RESET					 = 0x5204,   // 6.5.2  �豸��ˮ����
	MSG_BUSSINESS_DEVICE_COMPONENT_CHANGE		 = 0x5205,   // 6.5.3  ��������
	MSG_BUSSINESS_MAGAZINE_INFO					 = 0x5207,   // 6.5.4  Ʊ���汨��
	MSG_BUSSINESS_BOM_AUDIT                      = 0x520B,   // 6.5.8  BOM�������
	MSG_BUSSINESS_PACKAGE_AUDIT					 = 0x520D,	 // 6.5.10 �������������	

	MSG_BUSSINESS_NONIMMIDATELY_SURRENDER_APPLY = 0x0234,
	MSG_BUSSINESS_NONIMMIDATELY_SURRENDER_APPLY_QUERY = 0x0244,
	MSG_BUSSINESS_NONIMMIDATELY_SURRENDER = 0x0235,
	MSG_BUSSINESS_SIGNCARD_APPLY_QUERY = 0x0246
}MSG_CODE_BUSSINESS;

// ��������---�������ݴ���
typedef  enum _msg_code_control_command{
	MSG_SPECIFY_BLOCKNO_DATA               = 0x5201,   // 6.7.1.1 �ϴ�ָ���������������
	MSG_SPECIFY_TIME_DATA                  = 0x5202,   // 6.7.1.2 �ϴ�ָ��ʱ����������
	MSG_DEBUG_DATA                         = 0x5203,   // 6.7.1.3 �ϴ��豸��������
	MSG_OPERATION_DATA                     = 0x5204,   // 6.7.1.4 ��Ӫģʽ��������
	MSG_DEVICE_CONTROL_COMMAND             = 0x5205,   // 6.7.1.5 �豸���п�������
	MSG_24HOUR_OPERATION_DATA              = 0x5206,   // 6.7.1.6 24Сʱ��Ӫ��������
	MSG_DELAY_OPERATION_DATA               = 0x5207,   // 6.7.1.7 �ӳ���Ӫʱ������
	MSG_FORCE_TIME_SYNC                    = 0x5208,   // 6.7.1.8 ǿ��ʱ��ͬ������
	MSG_ISSUE_RESTRICT                     = 0x520A,   // 6.7.1.9 ��Ʊ��������
	MSG_UPLOAD_PARAMETER_ID           = 0x520C,   // 6.7.1.10 �ϴ������ͳ���汾
	MSG_UPDATE_PARAMETER                   = 0x520D,   // 6.7.1.11 �����ͳ����������
	MSG_FORCE_LOGOUT                       = 0x5211,   // 6.7.1.12 ǿ���˳���¼
	MSG_CHANGE_PARAMETER_ID           = 0x5212,   // 6.7.1.13 �����ͳ���汾�л�����
	MSG_REQUEST_DEVICE_STATUS              = 0x5213,   // 6.7.1.14 �豸״̬��ѯ
	MSG_AUTORUN_PARAMETER                  = 0x5215    // 6.7.1.16 �Զ����в������ÿ���
}MSG_CODE_CONTROL_COMMAND;

// ��������---�������ݴ���
typedef  enum _msg_code_control_request{
	MSG_REQUEST_LOGIN_OR_LOGOUT            = 0x5101,   // 6.7.2.1 ��¼�ǳ�����
	MSG_REQUEST_REFORM_PASSWORD            = 0x5103,   // 6.7.2.3 �����޸�����
	MSG_REQUEST_SEND_IMIDIATLY_PARAMETER   = 0x5107,   // 6.7.2.6 ��ʱ��Ч�����·�����
	MSG_REQUEST_CONNECT_AUTH               = 0x5109,    // 6.7.2.8 ������֤����
}MSG_CODE_CONTROL_REQUEST;

// ��������---֪ͨ���ݴ���
typedef  enum _msg_code_control_notic{
	MSG_ACCOUNT_UNLOCK_NOTIC               = 0x5502,		// 6.7.3.2 �˻�����֪ͨ
	MSG_FTP_CONFIG_NOTIC                   = 0x550C,				// 6.7.3.11 FTP������Ϣ֪ͨ
	MSG_OP_END_NOTIC                       = 0x550E,						// 6.7.3.13 ��������֪ͨ
	MSG_SIGNCARD_APPLY_RESULT = 0x0247,				
	MSG_NONIMMIDATELY_SURRENDER_APPLY_RESULT  = 0x0245,
}MSG_CODE_CONTROL_NOTIC;

// ��������---�������ݴ���
typedef  enum _msg_code_control_reply{
	MSG_REQUEST_LOGIN_OR_LOGOUT_REPLY      = 0x5101,   // 6.7.4.1 ��¼�ǳ�������
	MSG_MSG_REFORM_PASSWORD_REPLY          = 0x5103,   // 6.7.4.3 �����޸�������
	MSG_REQUEST_IMIDIATLY_PARAMETER_REPLY  = 0x5107,   // 6.7.4.4 ��ʱ��Ч�����·�������
	MSG_REQUEST_CONNECT_AUTH_REPLY         = 0x5109,   // 6.7.4.6 ������֤������
	MSG_PARAMETER_ID_DATA             = 0x5401    // 6.7.4.8 �豸�����ͳ���汾����
}MSG_CODE_CONTROL_REPLY;

// ״̬���ݴ���
typedef  enum _msg_code_status{
	MSG_BOM_ALL_STATUS_DATA                = 0x3101,   // 6.6.2 BOM����״̬����
	MSG_BOM_STATUS_CHANGE_DATA             = 0x3102,   // 6.6.7 �豸���״̬����
	MSG_BOM_ERROR_NOTIC                    = 0x3107    // 6.6.8 �豸����״̬����
}MSG_CODE_STATUS;

#define LEN_AFC_SYS_HEADER 10
#define  LEN_TXN_DATATYPE 17
#define LEN_PACKAGE_HEADER 28
//6.3AFC�ڲ�����
//6.3.1BOM��TVM�˿�
#define  LEN_TXN_BOM_REFUND_TVM (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+19)
//6.3.2�Ǽ�ʱ�˿�����
#define  LEN_TXN_DELAY_REFUND (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+89)
//6.3.5��Ʊ���ս���
#define  LEN_TXN_INVALID_TICKET_RECYCLE (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+16)
//6.3.6BOM��Ӫҵ����֧
#define  LEN_TXN_BOM_NONBUSINESS_INCOME (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+84)
#define LEN_TXN_SYSCOM_HDR 42
#define LEN_TXN_TICKETCOM_HDR 17
#define LEN_TXN_OPERATORCOM_HDR  4
#define LEN_TXN_FINCOM_HDR 13
#define LEN_TXN_SERCOM_HDR  5
#define LEN_TXN_NONIMMIDATELY_SURRENDER (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR +LEN_TXN_FINCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 15 +LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_APPLY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 375+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_MODIFY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 375+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_REPORT_LOSS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 42+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_CANCEL_REPORT_LOSS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 42+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_ADMINISTRATION_PROCESS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_FINCOM_HDR + LEN_TXN_OPERATORCOM_HDR+ 22+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 105 + LEN_TXN_SERCOM_HDR)
#define LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY_QUERY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 31 + LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_APPLY_QUERY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 21 + LEN_TXN_SERCOM_HDR)

#define LEN_BUSINESS_HEADER 32
#define INX_BUSINESS_DATATYPE (LEN_BUSINESS_HEADER-7)
#define INX_BUSINESS_HEADER_LENGTH (LEN_BUSINESS_HEADER-5)
#define LEN_BUSINESS_HEADER_DATATYPE (LEN_BUSINESS_HEADER-5)
//6.4ҵ������
//6.4.2�����ͳ������
#define LEN_BUS_PARAMETER_SOFT_UPDATE_BASE (LEN_BUSINESS_HEADER+2)
//#6.4.3�����ͳ�������ҵ������
#define LEN_BUS_PARAMETER_SOFT_DOWNLOAD_BASE (LEN_BUSINESS_HEADER+1)
//6.4.4�豸��Ҫ״̬�ı�
#define LEN_BUS_DEVICE_IMPORTANT_STATUS_CHANG (LEN_BUSINESS_HEADER+4)
//6.4.5ISAMǩ��ǩ��
#define LEN_BUS_ISAM_LOGIN_LOGOUT (LEN_BUSINESS_HEADER+16)
//6.4.6������־
#define LEN_BUS_OPERATING_LOG (LEN_BUSINESS_HEADER + 68)
//6.4.7�豸��������
#define LEN_BUS_DEVICE_ERROR (LEN_BUSINESS_HEADER + 13)
//6.4.8�豸�Ĵ�������
#define LEN_BUS_DEVICE_REGISTER_BASE (LEN_BUSINESS_HEADER + 3)
#define LEN_BUS_DEVICE_REGISTER (LEN_BUSINESS_HEADER + 3 + 6 * 314)
//6.4.10Ʊ�����
#define LEN_BUS_TICKETBOX_CHANGE (LEN_BUSINESS_HEADER + 18)
//6.4.11�豸��������
#define COUNT_DEVICE_COMPONET 3
#define LEN_BUS_DEVICE_COMPONENT (LEN_BUSINESS_HEADER + 2 + 7*COUNT_DEVICE_COMPONET)

//6.5�¼�����
//6.5.1�˻�����
#define LEN_EVN_ACCOUNT_LOCK (LEN_BUSINESS_HEADER)
//6.5.3�豸��ˮ������
#define LEN_EVN_UDSN_RESET (LEN_BUSINESS_HEADER + 2 + 1*9)
//6.5.4��������
#define LEN_EVN_DEVICE_COMPONENT_CHANGE (LEN_BUSINESS_HEADER+13)
//6.5.5Ʊ���汨��
#define LEN_EVN_TICKETBOX_INFO_BASE  (LEN_BUSINESS_HEADER + 3)
#define LEN_EVN_TICKETBOX_INFO (LEN_BUSINESS_HEADER + 17)



//6.6״̬����
#define LEN_STATUS_HEADER 11
#define LEN_STATUS_SINGLE_MODE (LEN_STATUS_HEADER + 2 + 4 * 1) // ״̬���ݳ���(����ģʽ/24Сʱ��Ӫ/�ӳ�ģʽ
#define LEN_STATUS_THREE (LEN_STATUS_HEADER + 2 + 4*3) // ����ģʽ + 24Сʱ��Ӫ + �ӳ�ģʽ
#define LEN_STATUS_FULL (LEN_STATUS_HEADER + 2 + 4 * 24 )  // ȫ״̬����
#define LEN_STATUS_HAS_ERROR (LEN_STATUS_HEADER + 7) // �쳣״̬���ĳ���
#define LEN_STATUS_CHANGE (LEN_STATUS_HEADER+4) //6.6.6�豸״̬�������

#define LEN_REQ_HEADER 2
//6.7.2��������
//6.7.2.1��½�ǳ�
#define LEN_REQ_LOGINLOGOFF  (LEN_REQ_HEADER+12)
//6.7.2.3�����޸�
#define LEN_REQ_CHANGE_PASSWORD (LEN_REQ_HEADER+17)
//6.7.2.5�����޸�
//6.7.2.6��ʱ��Ч�����·�����
#define LEN_REQ_SEND_IMMIDIATLY_PARAMETER (LEN_REQ_HEADER+6)
//6.7.2.8������֤����
#define  LEN_REQ_CONNECT_AUTH (LEN_REQ_HEADER+11)

//6.7.3֪ͨ����
//6.7.3.13��������֪ͨ
#define  LEN_NOTICE_OPERATION_END 4

//6.7.4��������
#define LEN_REPLY_PARAMETER_VERSION (10+15*40)

// ����������
typedef enum _transfer_error{
	ERR_PROTOCOL_IDENTIFY        = 0x0001,   // Э���ʶ����
	ERR_PROTOCOL_VERSION         = 0x0002,   // Э��汾�Ŵ���
	ERR_DATA_FORMAT_VERSION      = 0x0003,   // ���ݸ�ʽ�汾�Ŵ���
	ERR_PROTOCOL_COMMAND         = 0x0004,   // Э���������
	ERR_DATA_TYPE                = 0x0005,   // �������ͱ������
	ERR_CRC_TYPE                 = 0x0006,   // У�����㷨����
	ERR_CRC_DATA                 = 0x0007,   // У�������
	ERR_BLOCK_DATA               = 0x0008,   // �ְ����ݴ���
	ERR_ECRYPT                   = 0x0009,   // �����㷨����
	ERR_KEY_VERSION              = 0x000A,   // ��Կ�汾����
	ERR_SAME_DATAD               = 0x000B,   // �ظ���Ϣ
	ERR_PACKAT_LENGTH            = 0x00FD,   // ���峤�ȴ���
	ERR_OTHER                    = 0x00FE    // ��������
}TRANSFER_ERROR;

// ��������
typedef enum _operation_code{
	OPERATION_LOGIN          = 0x1001, // ��¼
	OPERATION_LOGOUT         = 0x1002, // �ǳ�
	OPERATION_INPUT_PARAM    = 0x1101, // ��������ͳ���
	OPERATION_OUTPUT_PARAM   = 0x1102, // ���������ͳ���
	OPERATION_INPUT_DEAL     = 0x1103, // ���뽻������
	OPERATION_OUTPUT_DEAL    = 0x1104, // ������������
	OPERATION_INPUT_OPER     = 0x1105, // ����ҵ������
	OPERATION_OUTPUT_OPER    = 0x1106, // ����ҵ������
	OPERATION_INPUT_DEBUG    = 0x1107, // �����������
	OPERATION_OUTPUT_DEBUG   = 0x1108, // ������������
	OPERATION_FORCE_TIME     = 0x1411, // ǿ��ʱ��ͬ��
	OPERATION_MANUAL_TIME    = 0x1412, // �ֹ�ʱ��ͬ��
	OPERATION_24HOUR_DELAY   = 0x1701, // 24Сʱ��Ӫ���ӳ���Ӫ����
	OPERATION_DEBUG_UPLOAD   = 0x1702, // ������־�ϴ�
	OPERATION_PARAM_EDIT     = 0x1A01, // �����༭(�༭���������·���
	OPERATION_MODE_SET       = 0x1901, // ģʽ�趨���˹������豸ģʽ��
	OPERATION_MODE_NOTIC     = 0x1902, // ģʽ֪ͨ�趨
	OPERATION_TIME_WARNING   = 0xA101, // ʱ��ͬ������
	OPERATION_TIME_ERROR     = 0xA102, // ʱ��ͬ���쳣
	OPERATION_CONNECT_STATUS = 0xA103  // ����״̬
}OPERATION_CODE;

// ��������
typedef enum _operation_level{
	OPERATION_ONE_LEVEL   = 0x01, // ��ͨ����
	OPERATION_TWO_LEVEL   = 0x02, // ������Ϣ 
	OPERATION_THREE_LEVEL = 0x03, // ��Ҫ����
	OPERATION_FOUR_LEVEL  = 0x04, // ������Ϣ
	OPERATION_FIVE_LEVEL  = 0x05  // Σ�ղ���
}OPERATION_LEVEL;



// Ʊ��λ��
typedef enum _magazine_palace{
	MAGAZINEA  = 0x01,  // AƱ��
	MAGAZINEB  = 0x02,   // BƱ��
	MAGAZINEC  = 0x03   // ��Ʊ��
}MAGAZINE_PALACE;

// Ʊ������
typedef enum _magazine_type{
	MAGAZINE_NORMAL = 0x01, // ����Ʊ��	0x01
	MAGAZINE_CYCLE  = 0X02  // ��Ʊ��	0x02
}_MAGAZINE_TYPE;

// Ʊ��״̬
typedef enum _sc_magazine_status{
	VALID_ATTACH   = 0x01, // ������װ��
	INVALID_ATTCH  = 0x02, // �Ƿ���װ��
	VALID_DETACH   = 0x03, // ����ж�£�
	INVALID_DETACH = 0x04  // �Ƿ�ж��
}SC_MAGAZINE_STATUS;


// �豸����״̬����
typedef enum{
	IN_SERVICE    = 0x00, // �豸��������
	WARN_SERVICE  = 0x01, // �豸�������
	OUTOF_SERVICE = 0x02, // �豸��ͣ����
	ERROR_SERVICE = 0x03, // �豸���Ϲر�
	MANT_SERVICE  = 0x04, // �豸ά�޹ر�
	SLEEP_SERVICE = 0x05, // �豸˯��ģʽ
	STOP_SERVICE  = 0x06, // �豸ֹͣ������Ӫ������
	EMER_SERVICE  = 0x07  // �豸��������
}SERVICE_STATUS;

// ����ģʽ 
typedef enum{
	RUNING       = 0x00, // ����������
	CLOSEING     = 0x01, // ҵ��������
	START_RUNING = 0x02, // ��������
	SHUTDOWNING  = 0x03, // �ػ���
	WARNING      = 0x04  // �豸����
}RUN_STATUS;

// ����ģʽ
typedef enum{
	ISSUE_MODE        = 0x01, // BOM��Ʊģʽ
	ADJUST_MODE       = 0x02, // BOM��Ʊģʽ
	ISSUE_ADJUST_MODE = 0x03  // BOM�۲�Ʊģʽ
}WORK_MODE;

// ͨѶ״̬
typedef enum{
	DISCONNECT = 0x00,// ͨѶ�ж�
	CONNECTED  = 0x01,// ͨѶ����
	CONNECTING = 0x02 // ͨѶ�ָ���
}COMMUNICATION;

// ʱ�Ӳ���
typedef enum{
	VALID_TIME = 0x00, // ʱ������
	WARN_TIME  = 0x01, // ʱ�Ӿ���
	ERROR_TIME = 0x02  // ʱ�ӹ���
}TIME_STATUS;

// ��¼״̬
typedef enum{
	LOGIN   = 0x00,	// ���˵�¼	��¼�ɹ����״̬
	LOGOUT  = 0x01,	// ���˵�¼	�˳���¼���״̬
	LOCKED  = 0x02	// �˻�����	�˻��������״̬�����˵�¼�ɹ���״̬�ı�
}LOGIN_STATUS;

// Ʊ��״̬
typedef enum{
	STATUS_MAGAZINE_SET = 0x00,           // Ʊ�䰲װ
	STATUS_MAGAZINE_INVALIDE_ATACH = 0x01,// Ʊ��Ƿ���װ
	STATUS_MAGAZINE_UNSET = 0x02,         // Ʊ��δ��װ��ж�£�
}STATUS_MAGAZINE;

// Ʊ������״̬
typedef enum{
	MAG_NUM_NORMAL     = 0x00,// Ʊ������
	MAG_NUM_EMPTY      = 0x01,// Ʊ���
	MAG_NUM_NEAR_EMPTY = 0x02,// Ʊ�佫��
	MAG_NUM_FULL       = 0x03,// Ʊ����
	MAG_NUM_NEAR_FULL  = 0x04 // Ʊ�佫��
}MAGAZINE_NUM_STATUS;

// ��д��״̬
typedef enum{
	RW_NORMAL    = 0x00, // ��д������
	RW_ERROR     = 0x01, // ��д������
	RW_LOSS      = 0x02, // ��д����ʧ
	RW_SAM_ERROR = 0x03  // ��д��SAM����
}STATUS_RW;

// SAM����
typedef enum{
	YPT_ISAM = 0x01,// һƱͨISAM
	YPT_PSAM = 0x02,// һƱͨPSAM
	YKT_ISAM = 0x03,// һ��ͨISAM
	YKT_PSAM = 0x04	// һ��ͨPSAM
}SAM_TYPE_;

// SAMλ��
typedef enum{
	RW_YPT_ISAM = 0x01,// �ⲿ��д��һƱͨISAM
	RW_YPT_PSAM = 0x02,// �ⲿ��д��һƱͨPSAM
	RW_YKT_ISAM = 0x03,// �ⲿ��д��һ��ͨISAM	
	RW_YKT_PSAM = 0x04,// �ⲿ��д��һ��ͨPSAM
	TH_YPT_ISAM = 0x05,// �ڲ���д��һƱͨISAM
	TH_YPT_PSAM = 0x06,// �ڲ���д��һƱͨPSAM
	TH_YKT_ISAM = 0x07,// �ڲ���д��һ��ͨISAM
	TH_YKT_PSAM = 0x08,// �ڲ���д��һ��ͨPSAM
}SAM_POS_;

// SAM״̬
typedef enum{
	ISAM_UNAUTH = 0x00, // ISAM״̬δ��֤
	ISAM_AUTHED = 0x01  // ISAM״̬����֤
}SAM_STATUS;

// �豸�Զ�����״̬
typedef enum{
	AUTORUN_STOP  = 0x00, // �豸�Զ�����״̬ͣ��
	AUTORUN_START = 0x01  // �豸�Զ�����״̬����
}DEVICE_AUTORUN;

// ά����״̬
typedef enum{
	DOOR_NORMAL_OPEN  = 0x00, // ά����������
	DOOR_NORMAL_CLOSE = 0x01, // ά���������ر�
	DOOR_ILLEGLE_OPEN = 0x02, // ά���ŷǷ���
	DOOR_CLOSE_UNLOCK = 0x03  // ά���Źر�δ��
}DOOR_STATUS;

// ��ӡ��״̬
typedef enum{
	PRINTER_NORMAL   = 0x00,  // ��ӡ����������
	PRINTER_ERR      = 0x01,  // ��ӡ��ͨ�Ź���
	PRINTER_SHUTDOWN = 0x02,  // ��ӡ���ϵ�
	PRINTER_NOPAPER  = 0x03	  // ��ӡ��ȱֽ
}PRINTER_STATUS;

typedef enum{
	TH_MODULE_NORMAL   = 0x00, // ģ������
	TH_MODULE_ERR      = 0x01, // ģ�����
	TH_MODULE_SHUTDOWN = 0x02, // ģ�����ӶϿ�
	TH_MODULE_JAM      = 0x03  // ģ�鿨Ʊ
}TH_MODULE_STATUS;

typedef enum{
	OPENED  = 0x01,	// �Ŵ���
	CLOSED  = 0x02,	// �Źر���
	UNKNOWN = 0xFF	// δ֪
} DOORSTATUS;

typedef enum _box_no {
	TICKET_BOX1 = 1,                // Ʊ��A  
	TICKET_BOX2                     // Ʊ��B
} BOX_NO;

typedef enum 
{
	SRC_NOT_BUT_NEW_HAVE = 0,	//����Ʊ��֮ǰû��Ʊ�䣬֮��װ���µ�Ʊ��
	SRC_HAVE_BUT_NEW_NOT,		//����Ʊ��֮ǰ����Ʊ�䣬֮��û�а�װ���µ�Ʊ�䣬 ��ж��Ʊ�䣬δ��װ��Ʊ�䡣
	SRC_NEW_SAME,				//����Ʊ��֮ǰ����Ʊ�䣬֮��װ��Ʊ����ԭ����һ��, ��Ʊ��û�и���
	SRC_NEW_DIFFER,				//����Ʊ��֮ǰ����Ʊ�䣬֮��װ��Ʊ����ԭ���Ĳ�һ���� ��������Ʊ�䡣
	SRC_NEW_ALL_NOT,			//����ǰ�󶼲�����Ʊ�䡣
	SRC_NEW_DIFFER_NUM			//����ǰ��������һ��
} OP_BOX_STATUS;

typedef enum{
	UPLOAD_COMPLETE    = 0x0001, // �ļ��ϴ�����
	PARAMETER_COMPLETE = 0x0002, // �����������
	DAY_AUDIT_COMPLETE = 0x0003, // �մ�����ƽ���
	DATA_SYNC_COMPLETE = 0x0004  // ��λͬ���������
}NOTICE_TYPE;


typedef struct _tagBOM_STATUS_ID
{
	BYTE b1;
	BYTE b2;
	BYTE b3;
} BOM_STATUS_ID;

namespace BOM_STATUS_IDS
{
	const static _tagBOM_STATUS_ID SERVICE_STATUS = {0x01,0x01,0x01};// ����״̬
	const static _tagBOM_STATUS_ID  RUN_STATUS = {0x01,0x01,0x02};             // ����״̬
	const static _tagBOM_STATUS_ID   WORK_MODE = {0x01,0x01,0x03};              // ����ģʽ
	const static _tagBOM_STATUS_ID   RUN_MODE = {0x01,0x01,0x04};               // ��Ӫģʽ���μ�7.9��Ӫģʽ���롣
	const static _tagBOM_STATUS_ID   HOUR24MODE = {0x01,0x01,0x05};             // 24Сʱ��Ӫģʽ
	const static _tagBOM_STATUS_ID   DELAY_RUN_MODE = {0x01,0x01,0x06};         // �ӳ���Ӫʱ��
	const static _tagBOM_STATUS_ID   CONNECT_STATUS = {0x01,0x01,0x07};         // ����״̬
	const static _tagBOM_STATUS_ID   TIME_DELAY = {0x01,0x01,0x08};             // ʱ�Ӳ���
	const static _tagBOM_STATUS_ID   LOGIN_STATUS = {0x01,0x01,0x09};           // ��¼״̬
	const static _tagBOM_STATUS_ID   DEVICE_AUTO_RUN_STATUS = {0x01,0x01,0x0A}; // �豸�Զ�����״̬
	const static _tagBOM_STATUS_ID   MAGAZINEA_STATUS = {0x06,0x01,0x01};    	// Ʊ��1��װ״̬
	const static _tagBOM_STATUS_ID   MAGAZINEA_COUNT = {0x06,0x01,0x02};        // Ʊ��1����״̬
	const static _tagBOM_STATUS_ID   MAGAZINEB_STATUS = {0x06,0x02,0x01};    	// Ʊ��2��װ״̬
	const static _tagBOM_STATUS_ID   MAGAZINEB_COUNT = {0x06,0x02,0x02};        // Ʊ��2����״̬
	const static _tagBOM_STATUS_ID   CYCLEBOX_STATUS = {0x07,0x01,0x01};        // ��Ʊ�䰲װ״̬
	const static _tagBOM_STATUS_ID   CYCLEBOX_COUNT = {0x07,0x01,0x02};         // ��Ʊ������״̬
	const static _tagBOM_STATUS_ID   RW1_STATUS = {0x13,0x01,0x01};             // ��д��1״̬
	const static _tagBOM_STATUS_ID   RW2_STATUS = {0x13,0x02,0x01};             // ��д��2״̬
	const static _tagBOM_STATUS_ID   SAM1_STATUS = {0x14,0x02,0x01};            // SAM1����
	const static _tagBOM_STATUS_ID   SAM2_STATUS = {0x14,0x04,0x01};            // SAM2����
	const static _tagBOM_STATUS_ID   SAM3_STATUS = {0x14,0x08,0x01};            // SAM3����
	const static _tagBOM_STATUS_ID   ISAM1_STATUS = {0x14,0x02,0x02};           // ISAM1״̬
	const static _tagBOM_STATUS_ID   ISAM2_STATUS = {0x14,0x04,0x02};           // ISAM2״̬
	const static _tagBOM_STATUS_ID   ISAM3_STATUS = {0x14,0x08,0x02};           // ISAM3״̬
	const static _tagBOM_STATUS_ID   DOOR_STATUS = {0x1C,0x01,0x01};            // ά�����ۺ�״̬
	const static _tagBOM_STATUS_ID   PRINTER_STATUS = {0x15,0x01,0x01};         // ��ӡ��
	const static _tagBOM_STATUS_ID   TH_MODULE_STATUS = {0x20,0x01,0x01};       // TH��Ʊģ��
}


typedef struct _ticketcomhdr_t
{
	WORD issuerID;											//2BCD��Ʊ�����̱���
	BYTE ticketApplicationSerialNo[8];		//8HEXƱ��Ӧ�����к�
	BYTE ticketType;											//1BCDƱ������
	BYTE ticketSubType;									//1BCDƱ��������
	BYTE ticketStatus;										//1BCDƱ��״̬
	WORD ticketChargeTransSerialNo;		//2HEXƱ����ֵ�������
	WORD ticketConsumeTransSerialNo;	//2HEXƱ�����ѽ������
	_ticketcomhdr_t()
	{
		issuerID = 0;
		memset(ticketApplicationSerialNo,0,sizeof ticketApplicationSerialNo);
		ticketType = 0;
		ticketSubType = 0;
		ticketStatus = 0;
		ticketChargeTransSerialNo = 0;
		ticketConsumeTransSerialNo = 0;
	}
} TicketComHdr_t;

typedef struct _operatorcomhdr_t
{
	BYTE operatorID[3];				//3BCD����Ա����
	BYTE bomShiftID;					//BOM������
	_operatorcomhdr_t()
	{
		memset(operatorID,0,sizeof operatorID);
	}
} OperatorComHdr_t;

typedef struct _financecomhdr_t
{
	BYTE payMethod;	//֧����ʽ
	BYTE ticketNum[8];	//֧��Ʊ���߼�����
	long amount;		//���׷������
	_financecomhdr_t()
	{
		payMethod = 0x01;
		memset(ticketNum,0,sizeof ticketNum);
		amount = 0;
	}

} FinanceComHdr_t;


typedef struct _nonimmidatelysurrenderapply
{
	Sex_t gender;							//�ֿ����Ա�
	BYTE name[20];							//ASC�ֿ�������
	CERTIFICATE_TYPE certificateType;		//BCD֤������
	BYTE certificateID[20];					//ASC֤������
	BYTE billNum[10];						//���������վݱ��
	BYTE nouseReason;						//��ʹ��Ʊ��ԭ��
	BYTE applyType;							//�˿���������
	BYTE reason;							//Ʊ��Ǯ���˿�ԭ��
	BYTE suspendBillNum[10];				//��ʧ���ݱ��
	WORD cityCode;							//���д���
	WORD industryCode;						//��ҵ����
	TicketComHdr_t ticketComHdr;			//Ʊ��������
	_nonimmidatelysurrenderapply()
	{
		gender = SEX_MALE;
		memset(name,0,sizeof name);
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
		memset(billNum,0,sizeof billNum);
		nouseReason = 0;
		applyType = 0;
		reason = 0;
		memset(suspendBillNum,0,sizeof(suspendBillNum));
		cityCode = 0;
		industryCode = 0;
	}
}NonImmidatelySurrenderApply;


typedef struct _querynonimmidatelysurrenderapply
{
	CERTIFICATE_TYPE certificateType;		//BCD֤������
	BYTE certificateID[20];					//ASC֤������
	BYTE billNum[10];						//BCD�վݱ��
	WORD cityCode;							//���д���
	WORD industryCode;						//��ҵ����
	TicketComHdr_t ticketComHdr;			//Ʊ��������
	_querynonimmidatelysurrenderapply()
	{
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
		memset(billNum,0,sizeof billNum);
		cityCode = 0;
		industryCode = 0;
	}
}QueryNonImmidatelySurrenderApply;

typedef struct _nonimmidatelysurrender
{
	BYTE cardNum[8];				//Ʊ��Ӧ�����к�
	DWORD transAmount;				//���׷������
	BYTE refundReason;				//�˿�ԭ��
	DWORD depositAmount;			//��Ѻ���
	BYTE billNum[10];				//�վݺ�
	_nonimmidatelysurrender()
	{
		memset(cardNum,0,sizeof(cardNum));
		transAmount = 0;
		refundReason = 0;
		depositAmount = 0;
		memset(billNum,0,sizeof(billNum));
	}
}NonImmidatelySurrender;

typedef struct _signcardapply
{
	Sex_t gender;										//�ֿ����Ա�
	BYTE name[20];							//ASC�ֿ�������
	CERTIFICATE_TYPE certificateType;							//BCD֤������
	BYTE certificateID[20];							//ASC֤������
	_DATE_TIME cardValidity ;		//֤����Ч��
	BYTE phoneNumber[10];			//BCD�绰����
	BYTE email[60];							//ASCEmail
	BYTE address1[128];					//ASC��ַ1
	BYTE address2[128];					//ASC��ַ2

	_signcardapply()
	{
		gender = SEX_MALE;
		memset(name,0,sizeof name);
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
		memset(phoneNumber,0,sizeof phoneNumber);
		memset(email,0,sizeof email);
		memset(address1,0,sizeof address1);
		memset(address2,0,sizeof address2);
	}
} SignCardApply;

typedef SignCardApply SignCardModify;

typedef struct _querysigncardapply
{
	CERTIFICATE_TYPE certificateType;							//BCD֤������
	BYTE certificateID[20];							//ASC֤������
	_querysigncardapply()
	{
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
	}
} QuerySignCardApply;

typedef struct _reportloss
{
	BYTE gender;										//�ֿ����Ա�
	BYTE name[20];							//ASC�ֿ�������
	CERTIFICATE_TYPE certificateType;							//BCD֤������
	BYTE certificateID[20];							//ASC֤������
	_reportloss()
	{
		gender = 0x01;
		memset(name,0,sizeof name);
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
	}
} ReportLoss;

typedef ReportLoss CancelReportLoss;

typedef enum _administrationprocesstype
{
	TVM_STUCK_CASH = 50,				//TVM����
	TVM_STUCK_TICKET = 51,			//TVM��Ʊ
	TVM_INVALID_TICKET = 52,		//TVM������ЧƱ
	TVM_LESS_CHANGE = 53,				//TVM���һ�
	OTHER = 99											//���� 
}AdministrationProcessType;

typedef struct _administrationprocess
{
	AdministrationProcessType type;
	BYTE incomeOrOutcome; //01���룬02֧��
	BYTE memo[20];					//��ע
	_administrationprocess()
	{
		type = TVM_STUCK_CASH;
		incomeOrOutcome = 0x01;
		memset(memo,0,sizeof memo);
	}
}AdministrationProcess;

// ��ǰ��ЧCD�汾
typedef enum _cd_version{
	VERSION_FIRST = 1,
	VERSION_SECOND = 2
}CD_VERSION;

// 6.3.1	BOM��TVM�����˿�
typedef struct _bom_refund_tvm{
	TransComHdr_t transComHdr;
	BYTE bRefoundNo[15];	      // 15 1BIN+2BCD+1BIN+7BCD+4BIN �˿�ƾ֤���
	DWORD dwRefoundAmount;	      // 4 BIN BOM�˿���
	_bom_refund_tvm(){
		dwRefoundAmount = 0;
	};

}BOM_REFUND_TVM;


// 6.3.5	��Ʊ���ս���
typedef struct _invalid_ticket_recycletrade{
	TransComHdr_t transComHdr;
	DWORD cardSerialNo;			// 4 BIN �����к�
	BYTE cardPhysicalType;      // 1 BIN ����������
	DWORD issueId;			    // 4 BIN ��������
	WORD productType;			// 2 BIN ��Ʊ��Ʒ����
	BYTE reson;                 // 1 BIN �Ͽ�����ԭ��
	CCTLOGCountInfo::TICKETBOX_ID magazineId;     // 4 1BCD+1BIN+2BIN Ʊ��ID
	_invalid_ticket_recycletrade()
	{
		cardSerialNo = 0;
		cardPhysicalType = 0;
		issueId = 0;
	}
}INVALIDE_TICKET_RECYCLETRADE;

//6.3.6 BOM��Ӫҵ����֧
typedef struct _tagBOM_NONBUSINESS_INCOME
{
	TransComHdr_t transComHdr;
	BYTE baReason[20];			  //20 ASCII��֧ԭ��
	DWORD dwAmount;			//4 BIN ��֧��ת��ΪINTEL��
	BYTE baMemo[60];				 //60 ASCII��ע
};

// 6.3.6	�������۳�Ʊ����
typedef struct _recycle_issued_tickettrade{
	AfcSysHdr_t afcSysHdr;      // AFCϵͳ����ͷ
	DEVICE_ID deviceid;         // 4 BIN �豸ID
	DWORD dwAfcSerialNo;		// 4 BIN AFC������ˮ��	
	_DATE_TIME dateTime;		// 7 BCD ���ݲ���ʱ��
	BYTE bMsgType;		        // 1 BIN ��������
	BYTE bMsgSubType;           // 1 BIN ����������
	DWORD issueId;			    // 4 BIN ��������
	WORD productType;			// 2 BIN ��Ʊ��Ʒ����
	DWORD cardSerialNo;			// 4 BIN �����к�
	WORD lifecycle;             // 2 BIN ԭ���׵Ŀ���������
	DWORD udsnNo;				// 4 BIN ԭ���׵Ľ�����ˮ��
	DWORD tradeValue;			// 4 BIN ԭ���׽��
	BYTE  tradeType;			// 1 BIN ԭ���׵Ľ�������
	BYTE  tradeSubType;			// 1 BIN ԭ���׵Ľ���������
	_recycle_issued_tickettrade()
	{
		dwAfcSerialNo = 0;
		bMsgType = 0x50;
		bMsgSubType = 0x06;
		issueId  = 0;
		productType = 0;
		cardSerialNo = 0;
		lifecycle = 0;
		udsnNo = 0;
		tradeValue = 0;
		tradeType = 0;
		tradeSubType = 0;
	}
}RECYCLE_ISSUED_TICKETTRADE;

// ����Ա�����޸�����
typedef struct _change_staffpassword
{
	DWORD StaffID;        // ���޸������Ա��ID
	CString sOldPassword;    // ������
	CString sNewPassword;    // �޸ĺ�����
	BYTE ciperOldPwd[6];
	BYTE ciperNewPwd[6];
	_change_staffpassword()
	{
		StaffID = 0;
		sOldPassword = "";
		sNewPassword = "";
		memset(ciperOldPwd, 0, 6);
		memset(ciperNewPwd, 0, 6);
	}
}CHANGE_STAFFPASSWORD;

// �����ͳ�����£�ҵ�����ݣ�
typedef struct _parameter_update{
	WORD parameterId;  // �����ͳ���ID	2	BCD	����С�����˳������
	WORD providerId;   // �豸��Ӧ��ID 	2	BIN	����ʱ��Ҫת����INTEL�� �Գ�����Ч���Բ�����Ч��������ʱ��ȫ0
	WORD softwareNo;   // ������κ� 	2	BIN	����ʱ��Ҫת����INTEL�򡣶Գ�����Ч���Բ�����Ч��������ʱ��ȫ0
	BYTE versionType;  // �汾����	1	BIN	0x00��ʽ������ǰ�汾��0x01��ʽ���������汾��0x10���Բ�����ǰ�汾��	0x11���Բ��������汾
	DWORD oldVersion;  // �ɰ汾��	4	BIN	����ʱ��Ҫת����INTEL�� �л�ǰ�Ĳ���/�����ļ��İ汾��
	DWORD oldStartDate;// ��Ч����	4	BCD	�ɰ汾����Ч���ڣ�YYYYMMDD
	DWORD newVersion;  // �°汾��	4	BIN	����ʱ��Ҫת����INTEL�� �л������/�����ļ��İ汾��
	DWORD newStartDate;// ��Ч����	4	BCD	�°汾����Ч���ڣ�YYYYMMDD
	_parameter_update()
	{
		parameterId = 0;
		providerId = 0;
		softwareNo = 0;
		versionType = 0;
		oldVersion = 0;
		oldStartDate = 0;
		newVersion = 0;
		newStartDate = 0;
	}
}PARAMETER_UPDATE;

// �����ͳ������أ�ҵ�����ݣ�
typedef struct _parameter_download{
	WORD parameterId;    // 2 BCD �����ͳ���ID ����С�����˳������
	WORD providerId;     // 2 BIN �豸��Ӧ��ID
	WORD softwareNo;     // 2 BIN ������κ�
	BYTE versionType;    // 1 BIN �汾���� 0x00��ʽ������ǰ�汾��0x01��ʽ���������汾��0x10���Բ�����ǰ�汾��	0x11���Բ��������汾
	DWORD version;       // 4 BIN �汾
	_DATE_TIME DateTime; // 7 BCD ����ʱ��
	BYTE file[20];       // 20 ASCII �ļ�����
	BYTE result;         // 1 BIN ���ؽ�� 0x00���سɹ���0x01����ʧ�ܣ�0x02�޷�������0x03��������

	_parameter_download(){
		parameterId = 0;
		providerId = 0;
		softwareNo = 0;
		versionType = 0;
		version = 0;
		result = 0;
		memset(file, 0x00, 20);
	}
}PARAMETER_DOWNLOAD;


// ������־
typedef struct _operator_log{
	CString csUserID;
	OPERATION_CODE operCode;    // ��������	2  1BIN+1BIN
	OPERATION_LEVEL operLevel;	// ��������	1  BIN
	BYTE result;	            // �������	1  BIN
	BYTE operDetail[32];	    // ��������	32 ASCII
	BYTE operTip[32];	        // ��ע	    32 ASCII
	_operator_log(){
		csUserID = "";
		result  = 0;
		memset(operDetail, 0x00, 32);
		memset(operTip, 0x00, 32);
	}
}OPERATOR_LOG;

// �豸��������
typedef struct _device_errordata{
	DEVICE_COMPONENT partType;    // 1 BIN �豸��������
	BYTE  bDeviceType;            // 1 BIN ��������                          
	BYTE devicePartID[3];         // 3 BIN ����ID
	WORD deviceDeviderId;         // 2 BIN �豸��Ӧ��ID
	BYTE maintanCode[4];	          // 4 1BIN+1BIN+1BIN+1BIN ���ϴ���
	BYTE ecode[2];	                  // 2 1BIN+1BIN ά�޴���
	_device_errordata()
	{
		memset(devicePartID, 0, 3);
		memset(maintanCode, 0, 4);
		memset(ecode, 0, 2);
	}
}DEVICE_ERRORDATA;

// Ʊ�佻��
typedef struct _magazine_change{
	TICKETBOX_OPERATION_TYPE operType;             // 1 BIN ��������
	CCTLOGCountInfo::TICKETBOX_ID magazineId;              // 4 1BCD+1BIN+2BIN Ʊ��ID
	MAGAZINE_PALACE magazne;			 // 1 BIN Ʊ��λ��ID
	BYTE  cardType;						 // 1 BIN Ʊ����������
	DWORD providerId;					 // 4 BIN Ʊ��������ID
	WORD cardProductType;				 // 2 BIN ��Ʊ��Ʒ����
	BYTE preProperty;					 // 1 BIN Ԥ��ֵ����
	BYTE childproductId;                 // 1 BIN ������ƷID
	WORD CardCount;                      // 2 BIN Ʊ������
	SC_MAGAZINE_STATUS magazineStatus;      // 1 BIN Ʊ��״̬
	_magazine_change()
	{
		cardType = 0;
		providerId = 0;
		cardProductType = 0;
		preProperty = 0;
		childproductId = 0;
		CardCount = 0;
	}
}MAGAZINE_CHANGE;

// �������
typedef struct _component{
	BYTE  bDeviceType;   // 1 BIN �豸��������
	DWORD dwCompoentID;  // 3 BIN �������
}COMPONENT;
