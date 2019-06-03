#ifndef	__COMMDEF_H_INCLUDED_
#define	__COMMDEF_H_INCLUDED_

#if	_MSC_VER > 1000
#pragma	once
#endif	// _MSC_VER	> 1000

#endif	// __COMMDEF_H_INCLUDED_

//static const TCHAR* const BOM_PM_UPDATE_DIR		            = _T("Download\\PM\\");             // ��������Ŀ¼



// master�ļ���
// �����汾��master�ļ���
static const TCHAR* const SOFTVER_FILE_NAME						= _T("SoftwareVersion.mst");    // �汾������Ϣ
static const TCHAR* const COUNT_INFO_FILE_NAME					= _T("CountInfo.mst");          // ������Ϣ
static const TCHAR* const AUDIT_INFO_FILE_NAME					= _T("AuditInfo.mst");          // ��Ȩ��Ϣ
static const TCHAR* const PRESELL_TICKET_FILE_NAME				= _T("PreTicketInfo.mst");      // Ԥ��Ʊ��Ϣ
static const TCHAR* const REGISTER_INFO_FILE_NAME	            = _T("Register.mst"); 		    // �Ĵ���������Ϣ
static const TCHAR* const PSD_ERROR_INFO_FILE_NAME	            = _T("PasswordError.mst"); 	    // ��������Ա����Ϣ
//static const TCHAR* const EXCHANGE_BACKUP_FILE_NAME	            = _T("ExchangeBackup.mst");     // �������ݿ���Ϣ
//static const TCHAR* const ECT_DATA_FILE_NAME	                    = _T("ECTData.mst");            // һ��ͨ���ױ�������
static const TCHAR* const DEVICE_INFO_FILE_NAME	                = _T("DeviceInfo.mst");         // �豸��Ϣ
static const TCHAR* const REFUND_APPLY_INFO_FILE_NAME	        = _T("RefundApplyInfo.mst");    // �Ǽ�ʱ�˿�������Ϣ
static const TCHAR* const NAMEDCARD_APPLY_INFO_FILE_NAME	        = _T("NamedCardApplyInfo.mst"); // ������������Ϣ
// �����汾��master��·��
static const TCHAR* const MASTER_CURRENT_DIR                     = _T("current\\");              // ��ǰ�汾�ļ�·��
static const TCHAR* const MASTER_FUTURE_DIR                      = _T("future\\");               // �����汾�ļ�·��

// �����ļ���
static const TCHAR* const PARAM_PREFIX							= _T("Para."); 						// �����ļ�ǰ׺
static const TCHAR* const ACC_PARAM_ID_0101						= _T("Para.0101.");					// ACC��Ӫ����
static const TCHAR* const ACC_PARAM_ID_0102						= _T("Para.0102.");					// ACC�������������Ѳ���
static const TCHAR* const ACC_PARAM_ID_0103						= _T("Para.0103.");					// ACC��ʱ�������ò���
static const TCHAR* const ACC_PARAM_ID_0201						= _T("Para.0201.");					// ACC�豸���Ʋ���
static const TCHAR* const ACC_PARAM_ID_0301						= _T("Para.0301.");					// ACC��վλ�ò���
static const TCHAR* const ACC_PARAM_ID_0302						= _T("Para.0302.");					// ACC�Ʒ�վ�����
static const TCHAR* const ACC_PARAM_ID_0303						= _T("Para.0303.");					// ACC���β���
static const TCHAR* const ACC_PARAM_ID_0401						= _T("Para.0401.");					// ACCƱ�����Բ���
static const TCHAR* const ACC_PARAM_ID_0601						= _T("Para.0601.");					// ACC����Ʊ�۱����
static const TCHAR* const ACC_PARAM_ID_0602						= _T("Para.0602.");					// ACCƱ�ۼ�������
static const TCHAR* const ACC_PARAM_ID_0603					    = _T("Para.0603.");					// ACCƱ�۵������Ա����
static const TCHAR* const ACC_PARAM_ID_0604						= _T("Para.0604.");					// ACCʱ�両�������
static const TCHAR* const ACC_PARAM_ID_0605						= _T("Para.0605.");					// ACCʱ�����ͱ����
static const TCHAR* const ACC_PARAM_ID_0606						= _T("Para.0606.");					// ACCʱ��������ϸ����
static const TCHAR* const ACC_PARAM_ID_0607						= _T("Para.0607.");					// ACC�ۿ��ʱ����
static const TCHAR* const ACC_PARAM_ID_0608						= _T("Para.0608.");					// ACCͣ�˳�վ��
static const TCHAR* const ACC_PARAM_ID_0801						= _T("Para.0801.");					// ACC����������
static const TCHAR* const ACC_PARAM_ID_0802				    	= _T("Para.0802.");					// ACC��ؿ�/���д�����ղ���
static const TCHAR* const ACC_PARAM_ID_0701						= _T("Para.0701.");					// ACC��������������
static const TCHAR* const ACC_PARAM_ID_0702						= _T("Para.0702.");					// ACC���κ���������
static const TCHAR* const ACC_PARAM_ID_0501						= _T("Para.0501.");					// ACCƱ�������̲���

static const TCHAR* const ECT_PARAM_ID_4011						= _T("Para.4011.");					// ECT����������

static const TCHAR* const AFC_PARAM_ID_1002						= _T("Para.1002.");					// AFCģʽ��������
static const TCHAR* const AFC_PARAM_ID_1005						= _T("Para.1005.");					// AFC�豸��������
static const TCHAR* const AFC_PARAM_ID_1006						= _T("Para.1006.");					// AFC�豸����Ա��Ϣ
static const TCHAR* const AFC_PARAM_ID_1007						= _T("Para.1007.");					// AFC�豸Ȩ�޲���
static const TCHAR* const AFC_PARAM_ID_1011						= _T("Para.1011.");					// AFC TVM��Ӫ����
static const TCHAR* const AFC_PARAM_ID_1089						= _T("Para.1089.");					// AFC ��Ӫʱ�����
static const TCHAR* const AFC_PARAM_ID_1021						= _T("Para.1021.");					// AFC BOM��Ӫ����



// �����汾���ͱ���
typedef	enum _param_version_type
{
	CUR_VER  = 0x00,//��ʽ��ǰ��	
	FUT_VER	 = 0x01,//��ʽ������	
	CUR_TEST = 0x10,//��ǰ���԰�
	FUT_TEST = 0x11,//�������԰�
	VER_TYPE_UNKNOW   = 0xFF //δ֪��	
} PARAM_VERSION_TYPE_CODE;

// BOMϵͳ���ܱ���
typedef	enum _sys_function_code
{
	SYS_FUNCTION_ISSUE				= 0x14010101,	// ��Ʊ
	SYS_FUNCTION_ADJUST				= 0x14010201,	// ��Ʊ
	SYS_FUNCTION_CHARGE				= 0x14010301,	// ��ֵ
	SYS_FUNCTION_REFUND				= 0x14010401,	// �˿�
	SYS_FUNCTION_ANALYZE			= 0x14010701,	// ��ѯ	
	SYS_FUNCTION_ACTIVATION			= 0x14010801,	// ����
	SYS_FUNCTION_DEFER				= 0x14010901,	// ����
	SYS_FUNCTION_OTHER				= 0x14010A01,	// ����

	SYS_FUNCTION_REFUND_BY_METRO = 0x14010901,//����ԭ���˿�
	SYS_FUNCTION_EXCHANGE		= 0x14010501,//����
	SYS_FUNCTION_REFRESH			= 0x14010601,//����

	SYS_FUNCTION_SYSTEM_SET			= 0x14020101,	// ϵͳ����	
	SYS_FUNCTION_DATA_MANAGE		= 0x14020201,	// ���ݹ���	
	SYS_FUNCTION_HARDWARE_TEST		= 0x14020301,	// Ӳ������	
	SYS_FUNCTION_REMOVEEXCEPTION	= 0x14020401,	// �쳣���

	// TVM ���ܴ��붨��
	SYS_TVM_FUNCTION_MODE_SET				= 0x02010401,// ģʽ�趨
	SYS_TVM_FUNCTION_CASH_SALE_INFO			= 0x02010301,// ����һ��������ͳ�ơ��豸״̬
	SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE		= 0x02010501,// ֽ�������Ӳ�������
	SYS_TVM_FUNCTION_CARD_BOX_MANAGE		= 0x02010601,// Ʊ�����
	SYS_TVM_FUNCTION_BALANCE_MANAGE			= 0x02010701,// ���㴦��һ���ջ���ƾ֤����
	SYS_TVM_FUNCTION_DATA_MANAGE			= 0x02010801,// ���ݹ���
	SYS_TVM_FUNCTION_SYSTEM_HARDWARE_TEST	= 0x02010901,// ϵͳ�趨��Ӳ���Լ�
} SYS_FUNCTION_CODE;

// ACC����һ��
typedef struct _version_acc{
	DWORD lBusinessParam;					// ACC��Ӫ����
	DWORD lServerFeeParam;					// ACC�������������Ѳ���
	DWORD lOverTimeAdjuestParam;			// ACC��ʱ�������ò���
	DWORD lDeviceControlParam;				// ACC�豸���Ʋ���
	DWORD lStationParam;					// ACC��վλ�ò���
	DWORD lPayStationParam;					// ACC�Ʒ�վ�����
	DWORD lCardAttributeParam;				// ACCƱ�����Բ���
	DWORD lBasePriceParam;					// ACC����Ʊ�۱����
	DWORD lPrieLevelParam;					// ACCƱ�ۼ�������
	DWORD lPriceAdjuestParam;				// ACCƱ�۵������Ա����
	DWORD lTimeFloatParam;					// ACCʱ�両�������
	DWORD lTimeTypeParam;					// ACCʱ�����ͱ����
	DWORD lTimeDetailParam;					// ACCʱ��������ϸ����
	DWORD lDiscountParam;					// ACC�ۿ��ʱ����
	DWORD lNotServStationParam;				// ACCͣ�˳�վ��
	DWORD lWriteListParam;					// ACC����������
	DWORD lOtherCityCardParam;				// ACC��ؿ�/���д�����ղ���
	DWORD lSingleBlackListParam;			// ACC��������������
	DWORD lSectionBlackListParam;			// ACC���κ���������
	DWORD lSectionParam;					// ACC���β���
	DWORD lissueCompanyParam;				// ACCƱ�������̲���


	_version_acc(){
		lBusinessParam= 0xFFFFFFFF;					// ACC��Ӫ����
		lServerFeeParam= 0xFFFFFFFF;					// ACC�������������Ѳ���
		lOverTimeAdjuestParam= 0xFFFFFFFF;			    // ACC��ʱ�������ò���
		lDeviceControlParam= 0xFFFFFFFF;				// ACC�豸���Ʋ���
		lStationParam= 0xFFFFFFFF;					    // ACC��վλ�ò���
		lPayStationParam= 0xFFFFFFFF;					// ACC�Ʒ�վ�����
		lCardAttributeParam= 0xFFFFFFFF;				// ACCƱ�����Բ���
		lBasePriceParam= 0xFFFFFFFF;					// ACC����Ʊ�۱����
		lPrieLevelParam= 0xFFFFFFFF;					// ACCƱ�ۼ�������
		lPriceAdjuestParam= 0xFFFFFFFF;				// ACCƱ�۵������Ա����
		lTimeFloatParam= 0xFFFFFFFF;					// ACCʱ�両�������
		lTimeTypeParam= 0xFFFFFFFF;					// ACCʱ�����ͱ����
		lTimeDetailParam= 0xFFFFFFFF;					// ACCʱ��������ϸ����
		lDiscountParam= 0xFFFFFFFF;					// ACC�ۿ��ʱ����
		lNotServStationParam= 0xFFFFFFFF;				// ACCͣ�˳�վ��
		lWriteListParam= 0xFFFFFFFF;					// ACC����������
		lOtherCityCardParam= 0xFFFFFFFF;				// ACC��ؿ�/���д�����ղ���
		lSingleBlackListParam= 0xFFFFFFFF;			    // ACC��������������
		lSectionBlackListParam= 0xFFFFFFFF;		  	// ACC���κ���������
		lSectionParam= 0xFFFFFFFF;						// ACC���β���
		lissueCompanyParam= 0xFFFFFFFF;				// ACCƱ��������
	}
}VERSION_ACC;

 //һ��ͨ�汾
typedef struct _version_ect{
	DWORD lBlacklistParam;  // һ��ͨ������������
	_version_ect(){
		lBlacklistParam = 0xFFFFFFFF;
	}
}VERSION_ECT;

// AFCϵͳ����
typedef struct _version_afc{
	DWORD lAccessLevelParam;   // �豸Ȩ�޲�����
	DWORD lOperationParam;  // ��Ӫ������
	DWORD lDeviceCommonParam;  // �豸����������
	DWORD lStaffPwdParam;      // ����Ա������
	DWORD lDeviceRunTimeParam; // �豸����ʱ�����
	//DWORD lOpratorCodeTable;   // ��Ӫ�̴���ӳ���
	DWORD lModeHistoryParam;   // ģʽ����
	_version_afc(){
		lAccessLevelParam = 0xFFFFFFFF;
		lOperationParam = 0xFFFFFFFF;
		lDeviceCommonParam = 0xFFFFFFFF;
		lStaffPwdParam = 0xFFFFFFFF;
		//lDeviceRunTimeParam = 0xFFFFFFFF;
		lModeHistoryParam = 0xFFFFFFFF;
	}
}VERSION_AFC;

// ����������ID
typedef enum _parameter_id{
	AFC_MODEHISTORY_ID           = 0x1002,  // ģʽ����
	AFC_DEVICECOMMONPARAM_ID     = 0x1005,  // �豸��������
	AFC_STAFFPWDPARAM_ID         = 0x1006,  // �豸����Ա��Ϣ
	AFC_ACCESSLEVELPARAM_ID      = 0x1007,  // �豸Ȩ�޲���
	AFC_TVMOPERATIONPARAM_ID     = 0x1011,  // TVM��Ӫ����
	AFC_TCMOPERATIONPARAM_ID     = 0x1041,  // TCM��Ӫ����
	AFC_BOMOPERATIONPARAM_ID     = 0x1021,  // BOM��Ӫ����
	AFC_DEVICERUNTIMEPARAM_ID    = 0x1089,  // �豸����ʱ�����

	ACC_BUSINESS_ID						= 0x0101,					// ACC��Ӫ����
	ACC_SERVICEFEE_ID					= 0x0102,					// ACC�������������Ѳ���
	ACC_OVERTIME_UPDATE_ID				= 0x0103,					// ACC��ʱ�������ò���
	ACC_DEVICE_ID						= 0x0201,					// ACC�豸���Ʋ���
	ACC_STATION_ID						= 0x0301,					// ACC��վλ�ò���
	ACC_CHARGE_STATION_ID				= 0x0302,					// ACC�Ʒ�վ�����
	ACC_SECTION_ID						= 0x0303,					// ACC���β���
	ACC_CARDATTRIBUTE_ID				= 0x0401,					// ACCƱ�����Բ���
	ACC_ISSUE_COMPANY_ID			    = 0x0501,					// ACCƱ�������̲���
	ACC_BASE_PRICE_TABLE_ID				= 0x0601,					// ACC����Ʊ�۱����
	ACC_PRICE_LEVEL_TABLE_ID			= 0x0602,					// ACCƱ�ۼ�������
	ACC_PRICE_ADJUEST_TABLE_ID			= 0x0603,					// ACCƱ�۵������Ա����
	ACC_TIME_FLOATING_TABLE_ID			= 0x0604,					// ACCʱ�両�������
	ACC_TIME_TYPE_TABLE_ID				= 0x0605,					// ACCʱ�����ͱ����
	ACC_TTIME_TYPE_DETAIL_ID			= 0x0606,					// ACCʱ��������ϸ����
	ACC_DISSCOUNT_TABLE_ID				= 0x0607,					// ACC�ۿ��ʱ����
	ACC_NOT_SERVICE_STATION_ID			= 0x0608,					// ACCͣ�˳�վ��
	ACC_SINGLE_BLACK_LIST_ID			= 0x0701,					// ACC��������������
	ACC_SECTION_BLACK_LIST_ID			= 0x0702,					// ACC���κ���������
	ACC_WRITE_LIST_ID					= 0x0801,					// ACC����������
	ACC_OTHER_CITY_CARD_ID				= 0x0802,					// ACC��ؿ�/���д�����ղ���

	AFC_TVMPROGRAM_ID				= 0x4101,	// TVMӦ�ó���
	AFC_BOMPROGRAM_ID				= 0x4102,   // BOMӦ�ó���
	AFC_TCMPROGRAM_ID				= 0x4104,   // TCMӦ�ó���(EQM)
	AFC_TPUPROGRAM_ID				= 0x4109,   // TPU����
	//AFC_TVMGUI_ID					= ACC_STATION_MAP_ID,// GUI���� 
	//AFC_TPUMAINPROGRAM_ID        = 0x4110,// TPU Main����
	AFC_THPROGRAM_ID				= 0x4110, // THӦ�ó���(BOM)
	AFC_TVMTHPROG_ID				= 0x4111, // THӦ�ó���TVM��
	PARAM_UNKNOW					= 0x0000   
}PARAMETER_ID;

// �����汾��master��·��
static const TCHAR* const PARAM_ACC_CURRENT_DIR                     = _T("parameters\\current\\");     // ACC��ǰ�����ļ�·��
static const TCHAR* const PARAM_ACC_FUTURE_DIR                      = _T("parameters\\future\\");      // ACC���������ļ�·��
static const TCHAR* const PARAM_ACC_BACKUP_DIR                      = _T("parameters\\backup\\");      // ACC��ǰ�����ļ�·��
static const TCHAR* const PARAM_ECT_CURRENT_DIR					   = _T("parameters\\current\\");     // ECT��ǰ�����ļ�·��
static const TCHAR* const PARAM_ECT_FUTURE_DIR					   = _T("parameters\\future\\");      // ECT���������ļ�·��
static const TCHAR* const PARAM_ECT_BACKUP_DIR					   = _T("parameters\\backup\\");      // ECT���������ļ�·��
static const TCHAR* const PARAM_AFC_CURRENT_DIR					   = _T("parameters\\current\\");     // AFC��ǰ�����ļ�·��
static const TCHAR* const PARAM_AFC_FUTURE_DIR					   = _T("parameters\\future\\");      // AFC���������ļ�·��
static const TCHAR* const PARAM_AFC_BACKUP_DIR					   = _T("parameters\\backup\\");      // AFC���������ļ�·��

// �ļ�·��
static const TCHAR* const DEVICE_MASTER_DIR         = _T("master\\");                   // master�ļ�Ŀ¼
static const TCHAR* const DEVICE_MESSAGE_DIR        = _T("messages\\");                  // message�ļ�Ŀ¼

// master�ļ���
// �����汾��master�ļ���
static const TCHAR* const PEKG_AUDIT_FILE_NAME					= _T("PekgAuditInfo.mst");      // �����������Ϣ

#define BOM_INI_SECTION_APPLICATION					_T("APPLICATION")		// BOM.ini���Section

#define BOM_INI_KEY_DATAPATH						_T("DataPath")   		// ������Ŀ¼������·����
#define BOM_INI_KEY_DATAPATH_BKP					_T("BackupPath ")		// ��������Ŀ¼������·����




// �����°���� ϵͳ��������


// 
//typedef enum _cardholderFeeType_t{
//	CARDHOLDER_FEE_TYPE_REPLACEMENT      = 2,     // Fee TCHARged for replacement.
//	CARDHOLDER_FEE_TYPE_REFUND           = 5,     // Fee TCHARged for a refund.
//	CARDHOLDER_FEE_TYPE_PERSONALISATION  = 13,    // Fee TCHARged for personalisation.
//	CARDHOLDER_FEE_TYPE_UNSPECIFIED      = 255    // No value set.���趨ֵ
//}CardholderFeeType_t;

