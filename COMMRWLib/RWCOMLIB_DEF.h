#ifndef __RWCOMLIBAPI_H__
#define __RWCOMLIBAPI_H__

#pragma once

#include "CommonDefine.h"
#include "RWException.h"

static const UINT RW_MSG_NO_RESPONSE = WM_USER + 201;

static const unsigned long RW_ERROR_CARD_NO_CARD = 0x0069;

//static const unsigned long RW_ERROR_CARD_NO_CARD = 0x00100001;
static const unsigned long RW_ERROR_NOT_AVALUEABLE_CARD_TYPE = 0x0050003D;

typedef enum _rw_rsp_code {
	RW_RSP_ACC_CARD_BLACKLIST_BLOCKED   = 0xA1000000,			// һƱͨ������Ʊ��������

	//RW_RSP_ACC_CARD_BLACKLIST   = 0xA1,			// ??һƱͨ������Ʊ����Ҫ����
	//RW_RSP_ECT_CARD_BLACKLIST   = 0xA3,			// ??һ��ͨ������Ʊ����Ҫ����
/*-------------------------------------����Ϊ�ɰ� ���ִ�ȷ��----------------------------------------------------*/
	RW_RSP_CLOSE_ERROR					= 0xFF11,		// �ر�ͨѶ����
	RW_RSP_SEND_ERROR					= 0xFF12,		// ���ʹ���
	RW_RSP_RECEIVE_TIMEOUT				= 0xFF13,		// ���ճ�ʱ
	RW_RSP_RECEIVE_ERROR				= 0xFF14,		// ���մ���
	RW_RSP_DOWNLOAD_PARA_ERR			= 0xFF15,		// ���ز�������
	RW_RSP_DOWNLOAD_SERIAL_ERR			= 0xFF16,		// �������кŴ���
	RW_RSP_SEND_WAIT					= 0xFF17,		// ����ͣ��ȴ�
	RW_RSP_COMMAND_CANCLED				= 0xFF18,		// ����ȡ��
	RW_RSP_OPEN_PORT_ERR				= 0xFF19,		// �򿪶˿ڴ���
	RW_RSP_DEVICE_ERR					= 0xFF20,		// ��������豸���Ͳ���ȷʱ
	RW_RSP_PACKETS_ERR					= 0xFF21,		// ���񲻵����е����ݰ�
	RW_RSP_GET_DATA_ERR					= 0xFF22,		// ����ȡ��������ʱ
	RW_RSP_OUTPUT_BUFFER_ERR			= 0xFF23,		// ���ӿں����ṩ���ݵ�������治��ʱ
	RW_RSP_NO_DATA_RETURN				= 0xFF24,		// ��û������Ҫ����ʱ
	RW_RSP_MEMORY_ERR					= 0xFF25,		// �����ڴ������
	RW_RSP_FILE_ERR						= 0xFF26,		// �ļ�����
	RW_RSP_UPLOAD_FINAL_TRANSACTION		= 0xFF31,		// ���ϴ����һ����
	RW_RSP_PRELIMINARY_INSPECTION		= 0xFF32,		// �豸��Ҫ����(ֻ��CPU������)
	RW_RSP_INITIALl_INSPECTION_END		= 0xFF33,		// �������(ֻ��M1������)
	RW_RSP_CARD_ERR						= 0xFF34,		// ��Ƭ����
	RW_RSP_POINTER_ERR					= 0xFF35,		// ָ�����
	RW_RSP_TIMEOUT_HANDLING				= 0xFF36,		// ��ʱ����
	RW_RSP_INSUFFICIENT_LENGTH_DATA		= 0xFF37,		// ���յ����ݳ��Ȳ���
	RW_RSP_CONNECT_SERVER_FAIL			= 0xFF38,		// ���ӷ�����ʧ��
	RW_RSP_PARAMETER_ERR				= 0xFF39,		// ������������
	/*0xFF3A=$$$$
	0xFF3B=$$$$
	0xFF3C=$$$$
	0xFF3D=$$$$
	0xFF3E=$$$$
	0xFF3F=$$$$*/
	RW_RSP_COMMAND_ERR					= 0xFF40,		// ������Ӧ���ļ��е����������
	RW_RSP_COMMAND_ERROR				= 0xFF50,		// ������Ӧ���ļ��е����������
	RW_RSP_MAC2_LEN_ERR					= 0xFF52,		// ������������ص�MAC2���ȴ���(���Ȳ�����4)
	RW_RSP_CARD_TYPE_ERR				= 0xFF53,		// �����������ص�"��Ƭ����"����
	RW_RSP_NEED_MODIFY_FILE_NUM			= 0xFF54,		// �����������ص�"���޸��ļ��ĸ���"����
	RW_RSP_SEND_DATA_ERR				= 0xFF61,		// ����������������ʧ��
	RW_RSP_SERVICE_GET_DATA_ERR			= 0xFF62,		// �����������ϻ�ȡ����ʧ��
	RW_RSP_TPU_INCORRECT_ERR			= 0xFF64,		// �����ۿ�(��ֵ)ʱ,��TPU�ж�ȡ����Կ���Ͳ���ȷ
	RW_RSP_DATA_LESS_THAN_32			= 0xff71,		// ���յ��Ķ�����Ϣ�������ݳ���С��32
	RW_RSP_DATA_LESS_32CARD_DATA		= 0xff72,		// ���յ��Ķ�����Ϣ��������С��32+�����ݳ���
	RW_RSP_DATA_LESS_32_CARD_LOG_LEN	= 0xff73,		// ���յ��Ķ�����Ϣ��������С��32+�����ݳ���+��־����
	RW_RSP_LESS_THAN_37					= 0xff74,		// ���յ�����Ϣ����С��37
	RW_RSP_LESS_THAN_37_UD				= 0xff75,		// ���յ�����Ϣ����С��37+UD���ݳ���
	RW_RSP_LESS_THAN_37_UD_AR			= 0xff76,		// ���յ�����Ϣ����С��37+UD���ݳ���+AR���ݳ���
	RW_RSP_CARDLEN_LESS_THAN_CARDSIZE	= 0xff77,		// ���͸�TPU��CardlenС��Cardsize
	RW_RSP_RECEIVED_PACKET_LESS_CARD	= 0xff78,		// ���յ����ݰ�����С�ڿ����ݳ���+������Ϣ����
	RW_RSP_CYCLE_POWER_TPU				= 0xFFF4,		// ��TPU�ϵ�����
	RW_RSP_OK							= 0x0000,		// �����ɹ�
	RW_RSP_OPERATION_FAILED				= 0x0001,		// ����ʧ��
	RW_RSP_RESULTS_UNCERTAIN			= 0x0002,		// ���������ȷ��
	RW_RSP_READ_CARD_FAILED				= 0x0057,		// ����ʧ��
	RW_RSP_WRITE_CARD_FAILED			= 0x0058,		// д��ʧ��
	RW_RSP_BLACK_CARD					= 0x0059,		// ��������
	RW_RSP_CARD_DATA_NOT_LEGITIMATE		= 0x005a,		// ��Ƭ���ݲ��Ϸ������д���������ҵ�������󣬷����̴���������Ͳ�ƥ�䣩
	RW_RSP_CARD_STATUS_NOT_LEGITIMATE	= 0x005b,		// ��Ƭ״̬���Ϸ�
	RW_RSP_PURSE_BEYOND_LIMIT			= 0x005c,		// Ǯ����Խ����
	RW_RSP_CARD_NOT_VALID				= 0x005d,		// ��Ƭ������Ч��
	RW_RSP_ENTER_OUT_STATION_ERR		= 0x005e,		// ����վ�������
	RW_RSP_BALANCE_INSUFFICIENT			= 0x005f,		// ��Ƭ����
	RW_RSP_SITE_LIMIT					= 0x0060,		// վ������
	RW_RSP_TIMES_LIMIT					= 0x0061,		// ��Ƭ�ճ�����������
	RW_RSP_CARD_TIMEOUT					= 0x0062,		// ��Ƭ��ʱ
	RW_RSP_CARD_BEYONG_JOURNEY			= 0x0063,		// ��Ƭ����
	RW_RSP_RW_NOT_SUPPORT				= 0x0064,		// ��д����֧�ִ�ҵ��
	RW_RSP_SAM_ERR						= 0x0065,		// sam������
	RW_RSP_DOWNLOAD_FILE_FAILED			= 0x0066,		// �����ļ�ʧ��
	RW_RSP_CLOCK_OPERATION_FAILED		= 0x0067,		// ʱ�Ӳ���ʧ��
	RW_RSP_START_RW_FAILS				= 0x0068,		// ������д��ʧ��
	RW_RSP_FIND_CARD_FAILED				= 0x0069,		// �ҿ�ʧ��
	RW_RSP_FILE_NOT_EXIST				= 0x006a,		// �����ڴ��ļ�
	RW_RSP_GET_REGISTER_DATA_FAILED		= 0x006b,		// ��ȡ�Ĵ�������ʧ��
	RW_RSP_COSTING_FAILURE				= 0x006c,		// �������ʧ��
	RW_RSP_SLE_SEND_NOT_LEGITIMATE		= 0x006d,		// SLE���͵��������ݲ��Ϸ�
	RW_RSP_RW_NOT_INITIALIZED			= 0x006e,		// ��д��δ��ʼ��
	RW_RSP_DATA_NOT_EXIST				= 0x006f,		// ���ݲ�����
	RW_RSP_FARE_FILE_EXCEPTIONS			= 0x0070,		// Ʊ���ļ��쳣
	RW_RSP_OPERATIONAL_PARM_EXCEPTIONS	= 0x0071,		// ��Ӫ���Ʋ����쳣

	//RW_RSP_OK               = 0x00,         // �ɹ�
	RW_RSP_BLACKCARD        = 0x01,			// ����������
	RW_RSP_ENOUGH           = 0x04,         // ����
	RW_RSP_READERROR        = 0x05,         // ����ʧ��
	RW_RSP_WRITEERROR       = 0x06,         // д��ʧ��
	RW_RSP_STATUSERROR      = 0x07,         // TPU״̬�Ƿ�
	RW_RSP_FINDERROR		= 0x10,         // Ѱ�������
	RW_RSP_M1KAUTHENERROR	= 0x20,			// M1����֤����

	RW_RSP_SAMERR           = 0x30,         // SAM�������������
	RW_RSP_BMACPSAMERR      = 0x31,         // һ��ͨPSAM����֤����
	RW_RSP_BMACISAMERR      = 0x32,         // һ��ͨISAM����֤����
	RW_RSP_ACCPSAMERR       = 0x33,         // ACC PSAM����֤����
	RW_RSP_ACCISAMERR       = 0x34,         // ACC ISAM����֤����

	RW_RSP_COMERR			= 0x40,         // ͨѶ�ӿ������

	RW_RSP_ACCERR			= 0x50,         // һƱͨҵ���������
	RW_RSP_ECTERR			= 0x60,         // һ��ͨҵ���������

	RW_RSP_PARAMERR			= 0x70,			// ���ò��������
	RW_RSP_UDERR			= 0x80,			// UD���������
	RW_RSP_NOT_AVALIABLE_STATION = 0x88,	// Ŀ�س�վ����Ӫ
	RW_RSP_HARDWAREERR		= 0x90,			// Ӳ�������

	RW_RSP_ACC_CARD_BLACKLIST   = 0xA1,			// һƱͨ������Ʊ����Ҫ����
	RW_RSP_ECT_CARD_BLACKLIST   = 0xA3,			// һ��ͨ������Ʊ����Ҫ����
	RW_RSP_UNKNOWERR			= 0xf0,			// δ�������

	RW_RSP_ISSUE_INIT_CHECK	= 0x33,			// һ��ͨ�ۿ���Ҫ����
	RW_RSP_CHARGE_OVERTIME	= 0xB1,			// һ��ͨ��ֵ��Ҫ��ʱ����
	RW_RSP_CHARGE_DIFF_CARD  = 0xB2         // ��ʱ������ͬһ�ſ�
};

// ���ܴ���
typedef enum FUNCTION_CODE{
	FUNCTION_CODE_ENTER				= 0x01,//��վ��
	FUNCTION_CODE_EXIT				= 0x02,//��վ��
	FUNCTION_CODE_ISSUE				= 0x03,//�ۿ�;
	FUNCTION_CODE_ADJUST			= 0x04,//��Ʊ;
	FUNCTION_CODE_CHARGE			= 0x05,//��ֵ;
	FUNCTION_CODE_REFUND			= 0x06,//�˿�����;
	FUNCTION_CODE_EXCHANGE_OLD		= 0x07,//�滻�ɿ���
	FUNCTION_CODE_EXCHANGE_NEW		= 0x08,//�滻�¿���0
	FUNCTION_CODE_REFRESH_AUTO		= 0x09,//���
	FUNCTION_CODE_DEFER				= 0x10,//���ڣ�
	FUNCTION_CODE_REPORT_LOST		= 0x11,//��ʧ��
	FUNCTION_CODE_CONTERACT			= 0x12,//������
	FUNCTION_CODE_LOST_CANCEL		= 0x13,//������
	FUNCTION_CODE_REFUND_MONEY		= 0x14,//���ʣ�
	FUNCTION_CODE_MAINTENANCE_DOOR	= 0x5F,//TVM��ά���Ŷ���
	FUNCTION_CODE_SVT_ISSUE			= 0x6F,//TVM��ֵ����Ʊ
	FUNCTION_CODE_OTHER				= 0xFF //����ҵ��	
};


// ��������
typedef enum PARAM_TPU_CODE{
	TICKET_PRICE_PARA  = 0x08,    //Ʊ�۲���
	BLACK_LIST_PARA    = 0x10,    //������������
	RE_APP_PARA        = 0x11,    //Ӧ�ó����ļ�
	SERV_CONTROL_PARA  = 0x14,    //��Ӫ���Ʋ����ļ�
	MODE_HISTORY_PARA  = 0x15,    //ģʽ����
	STATION_NUM_PARA   = 0x17,    //��Ӫ�����
};

static const unsigned long RW_MSG_CARD_INFO_LEN = 512;      // ������Ϣ�ĳ���
static const unsigned long RW_MSG_LOG_INFO_LEN  = 512;     // ����־��Ϣ�ĳ���

static const unsigned long RW_SIM_M1_CARD_TYPE = 1;		// ģ��ECT��  
// ͨ�������ļ�������
static const TCHAR* const TPU_INFO_FILE_NAME				= _T("C:\\RWSimData\\TpuInfoDataFile.dat");				// Tpu��Ϣ�����ļ�
static const TCHAR* const PARAM_DATA_FILE_NAME				= _T("C:\\RWSimData\\ParamVersionDataFile.dat");		// �����汾�����ļ�
static const TCHAR* const UD_DATA_FILE_NAME					= _T("C:\\RWSimData\\UDDataFile.dat");					// UD�����ļ�

// һ��ͨ�����ļ�������
static const TCHAR* const M1_CARD_FILE_NAME				= _T("C:\\RWSimData\\M1\\M1CardDataFile.dat");			// M1Ʊ�������ļ�
static const TCHAR* const ACC_M1_CHARGE_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\ChargeUDDataFile.dat");		// ��ֵUD�����ļ�
static const TCHAR* const ACC_M1_CHARGE_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\ChargeARDataFile.dat");		// ��ֵAR�����ļ�
static const TCHAR* const ACC_M1_ISSUE_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\IssueUDDataFile.dat");			// ��ƱUD�����ļ�
static const TCHAR* const ACC_M1_ISSUE_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\IssueARDataFile.dat");			// ��ƱAR�����ļ�
static const TCHAR* const ACC_M1_BLOCK_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\BlockUDDataFile.dat");			// ����UD�����ļ�
static const TCHAR* const ACC_M1_BLOCK_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\BlockARDataFile.dat");			// ����AR�����ļ�
static const TCHAR* const ACC_M1_ADJUST_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\AdjustUDDataFile.dat");		// ��ƱUD�����ļ�
static const TCHAR* const ACC_M1_ADJUST_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\AdjustARDataFile.dat");		// ��ƱAR�����ļ�

// ACC�����ļ�������
static const TCHAR* const UL_CARD_FILE_NAME					= _T("C:\\RWSimData\\UL\\ULCardDataFile.dat");			// ULƱ�������ļ�
static const TCHAR* const ACC_ISSUE_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\IssueUDDataFile.dat");			// ��ƱUD�����ļ�
static const TCHAR* const ACC_ISSUE_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\IssueARDataFile.dat");			// ��ƱAR�����ļ�
static const TCHAR* const ACC_ADJUST_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\AdjustUDDataFile.dat");		// ��ƱUD�����ļ�
static const TCHAR* const ACC_ADJUST_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\AdjustARDataFile.dat");		// ��ƱAR�����ļ�
static const TCHAR* const ACC_REFUND_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\RefundUDDataFile.dat");		// �˿�UD�����ļ�
static const TCHAR* const ACC_REFUND_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\RefundARDataFile.dat");		// �˿�AR�����ļ�
static const TCHAR* const ACC_BLOCK_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\BlockUDDataFile.dat");			// ����UD�����ļ�
static const TCHAR* const ACC_BLOCK_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\BlockARDataFile.dat");			// ����AR�����ļ�
static const TCHAR* const ACC_COUNTERACT_UD_DATA_FILE_NAME	= _T("C:\\RWSimData\\UL\\CounteractUDDataFile.dat");	// ����UD�����ļ�
static const TCHAR* const ACC_COUNTERACT_AR_DATA_FILE_NAME	= _T("C:\\RWSimData\\UL\\CounteractARDataFile.dat");	// ����AR�����ļ�

#endif