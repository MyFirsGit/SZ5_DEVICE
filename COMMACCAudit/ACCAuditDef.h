#pragma once



#define AR_MANAGE_ORIGINAL_FILE_LEN					4*3						    // �ļ���ʼ����
#define AR_MANAGE_DEVICE_TYPE_LEN					4							// �豸���ͳ���
#define AR_MANAGE_REVOLVE_TIMES_LEN					4							// ѭ����������
#define AR_MANAGE_REVOLVE_BODY_LEN                  40							// ѭ���峤��
#define AR_MANAGE_REVOLVE_TIMES_POS                 8							// ѭ��������ʼλ��
#define AR_MANAGE_TICKET_TYPE_LEN					4							// ��Ʊ���ͳ���
#define AR_MANAGE_TICKET_POSITION_POS			    12							// ��Ʊ���Ϳ�ʼλ��
#define AR_MANAGE_UPDATE_MESSAGE_LEN				4							// �������ݳ���
#define AR_MANAGE_UPDATE_MESSAGE_POS				12							// �������ݿ�ʼλ��

#define AR_MANAGE_TAG_LEN											4
#define AR_MANAGE_COMM_HEAD_LEN                     24							//���ͨ�ñ���ͷ����


static const TCHAR* const AR_MANAGE_FOLDER			= _T("AR\\");					// ����ļ�Ŀ¼��
static const TCHAR* const AR_MANAGE_FILE_NAME		= _T("DeviceAR.dat");			// ����ļ���
static const TCHAR* const AR_MANAGE_ACC_FILE_NAME	= _T("ACCAR.dat");
static const TCHAR* const AR_MANAGE_SERIAL_NAME		= _T("SERIAL.dat");				// ������ˮ���ݴ��ļ�
static const TCHAR* const AR_MANAGE_DEV_NAME		= _T("DeviceReg.dat");			// �豸�Ĵ�����¼��
static const TCHAR* const AR_MANAGE_BOM_FILE_NAME = _T("BomAR.dat");		       // ����ļ���

static const int AR_ERROR_MSG_MAX_LEN  = 261;		// �쳣���������

typedef enum registerId{
		LUCKCOUNT     =5,	//�ۻ���������
		SELLCOUNT     =6,	//�ۻ���������
		SELLAMOUNT    =7,	//�ۻ����۽��֣�
		REFOUNDCOUNT  =8,	//�ۻ���Ʊ����
		REFOUNDAMOUNT =9,	//�ۻ���Ʊ���֣�
		UPDATACOUNT   =10,	//�ۻ����´���
		UPDATAAMOUNT  =11,	//�ۻ����½��֣�
		CHARGECOUNT   =12,	//�ۻ���ֵ����
		CHARGEAMOUNT  =13	//�ۻ���ֵ���֣�
}REGISTERID;
// �豸���ͱ���
typedef enum _ar_reason
{
	AR_BUSINESS_DAY_START = 0x02,		//��Ӫ�տ�ʼ
	AR_BUSINESS_DAY_END = 0x01,			//��Ӫ�ս���
	AR_BUSINESS_DAY_SHUTDOWN = 0x03,	//��Ӫ��û�����ػ�
} DEVICE_AR_REASON;