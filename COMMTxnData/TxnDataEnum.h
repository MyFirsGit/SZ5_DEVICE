#pragma once

// ������������
typedef enum _txn_message_kind
{
	ACC_TRANSACTION = 1,	// ACC��������
	ECT_TRANSACTION = 2,	// ECT��������
	AFC_TRANSACTION = 3		// AFC��������
}TXN_MESSAGE_KIND;

// ���ݰ�����
typedef enum _data_pekg_kind
{
	UN_DEF	 = 0x00,	// δ�������ݣ�
	ACC_PEKG = 0x01,	// ACC�������ݰ���
	ECT_PEKG = 0x02,	// һ��ͨ�������ݰ���
	AFC_PEKG = 0x03,	// �ڲ��������ݰ���
	BUS_PEKG = 0x04,	// ҵ�����ݰ���
	EVN_PEKG = 0x05 	// �¼����ݰ�
}DATA_PEKG_KIND;


// �������ͱ���
typedef	enum _data_type
{
	ACC_TRANS = 1,//ACC��������	
	AFC_TRANS = 2,//AFC��������	
	ECT_TRANS = 3,//ECT��������
	BUS_DATA  = 4,//ҵ������
	EVEN_DATA = 5,//�¼�����
	LOG_DATA  = 6,//��־����
	UNKNOW_DATA = 7 //δ֪����
} DATA_TYPE_CODE;

static const int ERROR_MSG_MAX_LEN  = 261;		// �쳣���������

static const int PEKG_ID_LEN		= 28;							// ���ݰ�ID����

static const TCHAR* const MESSAGES_DIR =  _T("messages\\");
static const TCHAR* const UNSEND_FILE_PATH		=  _T("unsend\\");
static const TCHAR* const ECT_TXN_FILE_PATH		=  _T("messages\\transaction\\ect\\");
static const TCHAR* const ACC_TXN_FILE_PATH		=  _T("messages\\transaction\\acc\\");
static const TCHAR* const AFC_TXN_FILE_PATH		=  _T("messages\\transaction\\afc\\");
static const TCHAR* const ACC_SENDED_FILE_PATH	=  _T("messages\\transaction\\acc\\history\\");
static const TCHAR* const ACC_UNTAC_FOLDER =  _T("messages\\transaction\\acc\\untac\\");
static const TCHAR* const AFC_SENDED_FILE_PATH	=  _T("messages\\transaction\\afc\\history\\");
static const TCHAR* const ECT_SENDED_FILE_PATH	=  _T("messages\\transaction\\ect\\history\\");
static const TCHAR* const BUS_TXN_FILE_PATH		=  _T("messages\\bussiness\\");
static const TCHAR* const BUS_SENDED_FILE_PATH	=  _T("messages\\bussiness\\history\\");
static const TCHAR* const EVN_TXN_FILE_PATH		=  _T("messages\\event\\");
static const TCHAR* const EVN_SENDED_FILE_PATH	=  _T("messages\\event\\history\\");

static const TCHAR* const CURRENT_ACC_FILE_NAME	 =  _T("currentAccPekg");
static const TCHAR* const CURRENT_ECT_FILE_NAME	 =  _T("currentEctPekg");
static const TCHAR* const CURRENT_AFC_FILE_NAME	 =  _T("currentAfcPekg");
static const TCHAR* const CURRENT_BUS_FILE_NAME	 =  _T("currentBusPekg");
static const TCHAR* const CURRENT_EVN_FILE_NAME	 =  _T("currentEvnPekg");
static const TCHAR* const PROTECT_FILE_NAME =  _T("unsendpackage.dll");


