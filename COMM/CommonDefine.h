#pragma once
#include "Util.h"

// �����ļ�������
#define BOM_INI_FILE_NAME							_T("BOM.INI")
#define INI_FILE											_T(".INI")
#define MAINTENANCE_INI_FILE_NAME				_T("Maintenance.INI")
#define FUNCTION_INI_FILE_NAME						_T("Functioninfo.INI")
#define VERSION_INI_FILE_NAME						_T("Versioninfo.INI")
#define FARE_TABLE_INI_FILE_NAME					_T("FareTableInfo.ini")

// ��־�ļ�������
// ͨ����־�ļ�������
static const TCHAR* const FILTER_LOGIN_LOGOFF_LOG			= _T("LoginAndLogoff_");	// ��¼�ǳ���־�ļ���ǰ׺
static const TCHAR* const FILTER_STAFFOPERATION_LOG			= _T("PassengerOperation_");	// �˿Ͳ�����־�ļ���ǰ׺
static const TCHAR* const FILTER_EXCEPTION_LOG				= _T("Exception_");			// �쳣��־�ļ���ǰ׺
static const TCHAR* const FILTER_SC_CLIENT_LOG				= _T("SC_Client_");			// SC�ͻ�����־�ļ���ǰ׺
static const TCHAR* const FILTER_SC_CLIENT_DATA				= _T("SC_Data_");			// SCͨ��������־�ļ���ǰ׺
static const TCHAR* const FILTER_SC_SVC_LOG					= _T("SC_Service");			// SC������־�ļ���ǰ׺
static const TCHAR* const FILTER_TXN_DATA					= _T("Txn_Data_");			// ���ݴ洢��־�ļ���ǰ׺
static const TCHAR* const FILTER_DEBUG_LOG					= _T("Debug_");				// DEBUG��־�ļ���ǰ׺
static const TCHAR* const FILTER_DB_SQL_LOG					= _T("DB_SQL_");			// ���ݿ�SQL������־�ļ���ǰ׺
static const TCHAR* const FILTER_NETWORK_DB_LOG				= _T("Network_DB_");		// �������ݿ������־�ļ���ǰ׺
static const TCHAR* const FILTER_QRCODE_LOG					= _T("QRCode_");			// ��ά�������־�ļ���ǰ׺
static const TCHAR* const FILTER_CCS_CLIENT_LOG				= _T("CCS_Client_");		// CCS�ͻ�����־�ļ���ǰ׺
static const TCHAR* const FILTER_CCS_CLIENT_DATA			= _T("CCS_Data_");			// CCSͨ��������־�ļ���ǰ׺
static const TCHAR* const FILTER_CCS_SVC_LOG				= _T("CCS_Service_");		// CSS������־�ļ���ǰ׺
static const TCHAR* const FILTER_KEYBOARD_LOG				= _T("Keyboard_");			// TVMά��������־�ļ���ǰ׺
static const TCHAR* const FILTER_OPERATION_LOG				= _T("PassengerFore_Debug_");//TVM�˿͵�����־�ļ���ǰ׺�����������ã�����Ҫ��ɾ��������
static const TCHAR* const FILTER_FTP_HELPER_LOG				= _T("FTP_HELPER_");		// FTP������־��ǰ׺
static const TCHAR* const FILTER_MONITOR_CONTROL_LOG= _T("MonitorControl_");//TVM��ط��������־
static const TCHAR* const FILTER_MONITOR_RECHARGE_RW_LOG= _T("RechargeRWMonitor_");//TVM ��ֵ��д�������־
static const TCHAR* const FILTER_MONITOR_COIN_MODULE_LOG= _T("CoinModuleMonitor_");//Ӳ��ģ������־
static const TCHAR* const FILTER_MONITOR_BANKNOTE_MODULE_LOG= _T("BanknoteCollectionModuleMonitor_");//ֽ�ҽ���ģ������־
static const TCHAR* const FILTER_STAFF_PRINT_HISTORY		= _T("Staff\\History");
static const TCHAR* const FILTER_STAFF_PRINT_UNPRINT		= _T("Staff\\UnPrint");
static const TCHAR* const FILTER_PASSENGER_PRINT_HISTORY	= _T("Passenger\\History");
static const TCHAR* const FILTER_PASSENGER_PRINT_UNPRINT	= _T("Passenger\\UnPrint");
static const TCHAR* const FILTER_AUDIT_LOG					= _T("Audit\\");
static const TCHAR* const FILTER_LOG_ZIP_BACKUP_HISTORY		= _T("LogZipBackup\\History");
static const TCHAR* const FILTER_WEBAPI_LOG					= _T("WebServiceApi_");
static const TCHAR* const FILTER_QR_ORDER_IMAGES_RUN_PRE	= _T("QR_ORDER_INFO_");	// ��ǰ������ά����Ϣ
static const TCHAR* const FILTER_UPS_LOG					= _T("UPSLog_");					// UPS��־�ļ���ǰ׺

// Ӳ��ģ����־�ļ�������
static const TCHAR* const FILTER_RECEIPT_PRINTER_LOG		= _T("Receipt_Printer_");			// ƾ֤��ӡ����־�ļ���ǰ׺
static const TCHAR* const FILTER_DESKTOP_RW_LOG				= _T("Desktop_RW_");				// �����д����־�ļ���ǰ׺
static const TCHAR* const FILTER_TOKEN_RW_LOG					= _T("Token_RW_");					// Token����ģ���ö�д����־�ļ���ǰ׺
static const TCHAR* const FILTER_CARD_RW_LOG					= _T("Card_RW_");					// Ʊ������ģ���ö�д����־�ļ���ǰ׺
static const TCHAR* const FILTER_RECHARGE_RW_LOG			= _T("Recharge_RW_");				// ��ֵ�ö�д����־�ļ���ǰ׺
static const TCHAR* const FILTER_CARD_RECYCLE_RW_LOG	= _T("Card_Recycle_RW_");		// Ʊ�����մ���ģ���ö�д����־�ļ���ǰ׺
static const TCHAR* const FILTER_TOKEN_RECYCLE_RW_LOG	= _T("Token_Recycle_RW_");		// Token���մ���ģ���ö�д����־�ļ���ǰ׺
static const TCHAR* const FILTER_ENTRY_RW_LOG					= _T("Entry_RW_");					// ��վˢ����д����־�ļ���ǰ׺
static const TCHAR* const FILTER_EXIT_RW_LOG						= _T("Exit_RW_");						// ��վˢ����д����־�ļ���ǰ׺
static const TCHAR* const FILTER_TOKEN_ES_RW1_LOG			= _T("Token_ES_RW1_");			// TokenʽES��д��1��־�ļ���ǰ׺
static const TCHAR* const FILTER_TOKEN_ES_RW2_LOG			= _T("Token_ES_RW2_");			// TokenʽES��д��2��־�ļ���ǰ׺
static const TCHAR* const FILTER_TOKEN_ES_RW3_LOG			= _T("Token_ES_RW3_");			// TokenʽES��д��3��־�ļ���ǰ׺
static const TCHAR* const FILTER_CARD_ES_RW_LOG				= _T("Card_ES_RW_");				// ��ʽES�ö�д����־�ļ���ǰ׺
static const TCHAR* const FILTER_PM_RW_LOG						= _T("PM_RW");						// ֤����ӡ���ö�д����־�ļ���ǰ׺

static const TCHAR* const FILTER_ES_CARD_BOX1_RFID_LOG						= _T("ES_Card_Box1_RFID_");					// ESƱ��1RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_ES_CARD_BOX2_RFID_LOG						= _T("ES_Card_Box2_RFID_");					// ESƱ��2RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_ES_CARD_BOX3_RFID_LOG						= _T("ES_Card_Box3_RFID_");					// ESƱ��3RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_ES_CARD_BOX4_RFID_LOG						= _T("ES_Card_Box4_RFID_");					// ESƱ��4RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_ES_CARD_BOX5_RFID_LOG						= _T("ES_Card_Box5_RFID_");					// ES Ʊ��5RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_ES_TOKEN_BOX1_RFID_LOG					= _T("ES_Token_Box1_RFID_");					// ES Token��1RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_ES_TOKEN_BOX2_RFID_LOG					= _T("ES_Token_Box2_RFID_");					// ES Token��2RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_ES_TOKEN_BOX3_RFID_LOG					= _T("ES_Token_Box3_RFID_");					// ES Token��3RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_COIN_ADD_BOX1_RFID_LOG					= _T("Coin_Add_Box1_RFID_");					// Ӳ�Ҳ�����1RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_COIN_ADD_BOX2_RFID_LOG					= _T("Coin_Add_Box2_RFID_");					// Ӳ�Ҳ�����2RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_COIN_RECYCLE_BOX1_RFID_LOG			= _T("Coin_Recycle_Box1_RFID_");			// Ӳ�һ�����1RFID��־
static const TCHAR* const FILTER_COIN_RECYCLE_BOX2_RFID_LOG			= _T("Coin_Recycle_Box2_RFID_");			// Ӳ�һ�����2RFID��־
static const TCHAR* const FILTER_BANKNOTE_CHANGE_BOX1_RFID_LOG	= _T("Banknote_Change_Box1_RFID_");		// ֽ��������1RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_BANKNOTE_CHANGE_BOX2_RFID_LOG	= _T("Banknote_Change_Box2_RFID_");		// ֽ��������2RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_BANKNOTE_WASTE_BOX_RFID_LOG		= _T("Banknote_Waste_Box_RFID_");			// ֽ�ҷϱ���RFID��־�ļ���ǰ׺
static const TCHAR* const FILTER_BANKNOTE_RECYCLE_BOX_RFID_LOG		= _T("Banknote_Recycle_Box_RFID_");		// ֽ�һ�����RFID��־

static const TCHAR* const FILTER_CARD_MODULE_LOG					= _T("Card_Module_");							// Ʊ������ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_TOKEN_MODULE_LOG				= _T("Token_Module_");						// Token����ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_BANKNOTE_MODULE_LOG			= _T("Banknote_Module_");					// ֽ�Ҵ���ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_COIN_MODULE_LOG					= _T("Coin_Module_");							// Ӳ�Ҵ���ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_PIN_PAD_LOG  							= _T("Pin_Pad_");									// �������ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_BANKCARD_MODULE_LOG			= _T("Bankcard_Module_");					// ���п�����ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_IO_MODULE_LOG						= _T("IO_Module_");								// IOģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_STATUS_DISPLAY_LOG				= _T("Status_Diaplay_");								// IOģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_UPS_MODULE_LOG					= _T("UPS_Module_");							// UPSģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_GATE_MODULE_LOG					= _T("Gate_Module_");							// բ��ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_CARD_RECYCLE_MODULELOG	= _T("Card_Recycle_Module_");				// Ʊ������ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_TOKEN_RECYCLE_MODULE_LOG	= _T("Token_Recycle_Module_");			// Token����ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_CARD_PRINTER_LOG					= _T("Card_Printer_");							// Ʊ����ӡ����־�ļ���ǰ׺
static const TCHAR* const FILTER_SCANER_LOG								= _T("Scaner_");									// ɨ������־�ļ���ǰ׺
static const TCHAR* const FILTER_LASERPRINTER_LOG					= _T("Laser_Printer_");							// �����ӡ����־�ļ���ǰ׺
static const TCHAR* const FILTER_CAMERA_LOG								= _T("Camera_");									// ����ͷ��־�ļ���ǰ׺
static const TCHAR* const FILTER_TOKEN_ES_LOG							= _T("Token_ES_");								// TokenʽES��־�ļ���ǰ׺
static const TCHAR* const FILTER_CARD_ES_LOG								= _T("Card_ES_");									// ��ʽES��־�ļ���ǰ׺
static const TCHAR* const FILTER_PM_MODULE_LOG						= _T("PM_Module");								// PMģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_MAGAZINE_MODULE_LOG     	= _T("Magazine_Module_");					// Ʊ��ģ����־�ļ���ǰ׺
static const TCHAR* const FILTER_DCR4000_LOG							= _T("logDCR4000Comm_");					// DCR4000��־�ļ���ǰ׺

// Ŀ¼�ṹ����
// ͨ��Ŀ¼����
static const TCHAR* const LOG_DIR									= _T("Log\\");						// ��־�ļ�Ŀ¼
static const TCHAR* const DATABASE_DIR								= _T("Database\\");					// ���ݿ��ļ�Ŀ¼
static const TCHAR* const MC_UPDATE_DIR								= _T("Download\\MC\\");				// ���Ʋ��������Ŀ¼
static const TCHAR* const PM_UPDATE_DIR								= _T("Download\\PM\\");				// ��������Ŀ¼
static const TCHAR* const TEMPLATE_UPDATE_DIR						= _T("Download\\TEMPLATE\\");		// �վ�ģ�����Ŀ¼
static const TCHAR* const GUI_UPDATE_DIR							= _T("Download\\GUI\\");			// GUI�ļ�����Ŀ¼
static const TCHAR* const STATION_MAP_UPDATE_DIR					= _T("Download\\STATION_MAP\\");	// STATION_MAP�ļ�����Ŀ¼
static const TCHAR* const VOICE_UPDATE_DIR							= _T("Download\\VOICE\\");			// �����ļ�����Ŀ¼
static const TCHAR* const MC_DOWNLOAD_DIR							= _T("PG\\MC\\");					// ���Ʋ���������Ŀ¼
static const TCHAR* const TPU_MAIN_DOWNLOAD_DIR						= _T("PG\\TPU-MAIN\\");				// TPU���س�������Ŀ¼
static const TCHAR* const TPU_APP_DOWNLOAD_DIR						= _T("PG\\TPU-APP\\");				// TPUӦ�ò���������Ŀ¼
static const TCHAR* const GUI_DOWNLOAD_DIR							= _T("PG\\GUI\\");					// GUI�ļ�����Ŀ¼
static const TCHAR* const STATION_MAP_DOWNLOAD_DIR					= _T("PG\\STATION_MAP\\");			// ��·վ���������Ŀ¼
static const TCHAR* const TH_DOWNLOAD_DIR							= _T("PG\\TH\\");					// TH�ļ�����Ŀ¼
static const TCHAR* const PURCHASE_DIR								= _T("SCAudit\\");					// �����ļ�Ŀ¼
static const TCHAR* const UPGRADE_DATA_DESC_FILE_NAME				= _T("upgrade.description");		// �����ļ�˵���ļ�
static const TCHAR* const GUI_CURRENT_DIR							= _T("res\\gui\\");					// GUI����Ŀ¼
static const TCHAR* const TEMPLATE_CURRENT_DIR						= _T("Template\\");					// TEMPLATE����Ŀ¼
static const TCHAR* const VOICE_CURRNT_DIR							= _T("res\\tts\\audio\\");			// Voice����Ŀ¼ 

// ���ݺͱ��������ļ�·��
static const TCHAR* const MASTER_DIR		= _T("master\\"); // master�ļ�Ŀ¼

// TVM��
static const TCHAR* const TVM_CARD_RW_UPDATE_DIR		= _T("Download\\CardRW\\");		// TVM Ʊ����д������Ŀ¼
static const TCHAR* const TVM_TOKEN_RW_UPDATE_DIR		= _T("Download\\TokenRW\\");	// TVM Token��д������Ŀ¼
static const TCHAR* const TVM_CHARGE_RW_UPDATE_DIR		= _T("Download\\ChargeRW\\");	// TVM ���ö�д������Ŀ¼	
static const TCHAR* const DATABASE_FILE_NAME_OF_TVM     = _T("TVM.db");					// TVM ���ݿ��ļ�
static const TCHAR* const TVM_TH_UPDATE_DIR				= _T("Download\\TH\\");			// TVM ����ģ�����Ŀ¼
// ES��
static const TCHAR* const PHOTO_MAIN_DIR								= _T("Picture\\photo\\");				// ͼƬ�ļ�Ŀ¼
static const TCHAR* const PHOTO_TEMP_DIR								= _T("Picture\\temp\\");				// ��ʱͼƬ�ļ�Ŀ¼
static const TCHAR* const CRPT_TEMP_DIR_FILE_NAME				= _T("Picture\\printerTmp.prn");	// Ʊ����ӡ����ӡ�ļ���ʱĿ¼
static const TCHAR* const PM_TEST_FRONTGOUND					= _T("res\\test card-Front.bmp");	// PM��ӡ��������ͼƬ
static const TCHAR* const PM_TEST_BACKGOUND						= _T("res\\test card-Bakct.bmp");	// PM��ӡ���Ա���ͼƬ

static const TCHAR* const TC_UPDATE_DIR									= _T("Download\\TC\\");				// TH�������Ŀ¼
static const TCHAR* const IC_UPDATE_DIR									= _T("Download\\IC\\");				// �ⲿRW�������Ŀ¼
static const TCHAR* const TW_UPDATE_DIR									= _T("Download\\TW\\");				// �ڲ�RW�������Ŀ¼
static const TCHAR* const PM_DOWNLOAD_DIR							= _T("Download\\PM\\");				// ��������Ŀ¼

// ��־�ļ���
static const TCHAR* const NOTE_LOGIN_LOGOFF_LOG			= _T("��¼�ǳ���־");
static const TCHAR* const NOTE_STAFFOPERATION_LOG		= _T("վԱ������־");
static const TCHAR* const NOTE_EXCEPTION_LOG			= _T("�쳣��־");
static const TCHAR* const NOTE_SC_CLIENT_LOG			= _T("SC�ͻ�����־");
static const TCHAR* const NOTE_SC_CLIENT_DATA			= _T("SC�շ�����");
static const TCHAR* const NOTE_SC_SVC_LOG				= _T("SC������־");
static const TCHAR* const NOTE_TXN_DATA					= _T("��������");
static const TCHAR* const NOTE_DT_RW_LOG				= _T("�ⲿ��д����־");
static const TCHAR* const NOTE_DT_TW_LOG				= _T("�ڲ���д����־");
static const TCHAR* const NOTE_TH_LOG					= _T("TH�豸ͨ����־");
static const TCHAR* const NOTE_PRT_LOG					= _T("��ӡ����־");
static const TCHAR* const NOTE_DEBUG_LOG				= _T("������־");
static const TCHAR* const NOTE_DCR4000_LOG				= _T("DCR4000��־");

static const TCHAR* const FILTER_RW_LOG					= _T("RW_");						// RW��־�ļ���ǰ׺
static const TCHAR* const FILTER_TW_LOG					= _T("TW_");						// TW��־�ļ���ǰ׺
static const TCHAR* const FILTER_TH_LOG					= _T("ThTrace_");					// TH��־�ļ���ǰ׺
static const TCHAR* const FILTER_BRH_LOG                = _T("BrhTrace_");                  // TH��־�ļ���ǰ׺  20180531
static const TCHAR* const FILTER_TOKEN_LOG				= _T("TokenTrace_");				// Token��־�ļ���ǰ׺
static const TCHAR* const FILTER_PRT_LOG				= _T("PRT_");                       // ƾ֤��ӡ��
static const TCHAR* const FILTER_DIO_LOG				= _T("DioTrace_");					// IO��־�ļ���ǰ׺


static const TCHAR* const BOM_TRACE_DIR					= _T("log\\");						// ��־�ļ�Ŀ¼
static const TCHAR* const BOM_MC_UPDATE_DIR				= _T("Download\\MC\\");				// ���Ʋ��������Ŀ¼
static const TCHAR* const BOM_TC_UPDATE_DIR				= _T("Download\\TC\\");				// TH�������Ŀ¼
static const TCHAR* const BOM_IC_UPDATE_DIR				= _T("Download\\IC\\");				// �ⲿRW�������Ŀ¼
static const TCHAR* const BOM_TW_UPDATE_DIR				= _T("Download\\TW\\");				// �ڲ�RW�������Ŀ¼
static const TCHAR* const BOM_PM_UPDATE_DIR				= _T("Download\\PM\\");				// ��������Ŀ¼
static const TCHAR* const BOM_MC_DOWNLOAD_DIR			= _T("PG\\MC\\");					// ���Ʋ���������Ŀ¼
static const TCHAR* const BOM_TPU_MAIN_DOWNLOAD_DIR		= _T("PG\\TPU-MAIN\\");				// TPU���س�������Ŀ¼
static const TCHAR* const BOM_TPU_APP_DOWNLOAD_DIR		= _T("PG\\TPU-APP\\");				// TPUӦ�ò���������Ŀ¼
static const TCHAR* const BOM_TH_APP_DOWNLOAD_DIR		= _T("PG\\TH-APP\\");				// THӦ�ò���������Ŀ¼
static const TCHAR* const BOM_PM_DOWNLOAD_DIR			= _T("Download\\PM\\");				// ��������Ŀ¼
static const TCHAR* const BOM_PURCHASE_DIR				= _T("SCAudit\\");					// �����ļ�Ŀ¼

static const TCHAR* const TCM_TRACE_DIR					= _T("log\\");						// ��־�ļ�Ŀ¼
static const TCHAR* const TCM_MC_UPDATE_DIR				= _T("Download\\MC\\");				// ���Ʋ��������Ŀ¼
static const TCHAR* const TCM_IC_UPDATE_DIR				= _T("Download\\IC\\");				// �ⲿRW�������Ŀ¼
static const TCHAR* const TCM_PM_UPDATE_DIR				= _T("Download\\PM\\");				// ��������Ŀ¼
static const TCHAR* const TCM_MC_DOWNLOAD_DIR			= _T("PG\\MC\\");					// ���Ʋ���������Ŀ¼
static const TCHAR* const TCM_TPU_MAIN_DOWNLOAD_DIR		= _T("PG\\TPU-MAIN\\");				// TPU���س�������Ŀ¼
static const TCHAR* const TCM_TPU_APP_DOWNLOAD_DIR		= _T("PG\\TPU-APP\\");				// TPUӦ�ò���������Ŀ¼
static const TCHAR* const TCM_PM_DOWNLOAD_DIR			= _T("Download\\PM\\");				// ��������Ŀ¼
static const TCHAR* const TCM_PURCHASE_DIR				= _T("SCAudit\\");					// �����ļ�Ŀ¼

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// ES ����
//////////stati const TCHAR* const PHOTO_MAIN_DIR                = _T("picture\\photo");			   // ͼƬ�ļ�Ŀ¼
//////////static const TCHAR* const PHOTO_TEMP_DIR                = _T("picture\\temp");			   // ��ʱͼƬ�ļ�Ŀ¼

//////////static const TCHAR* const PM_TEST_FRONTGOUND			= _T("card background\\test card-Front.bmp");	// PM��ӡ��������ͼƬ
//////////static const TCHAR* const PM_TEST_BACKGOUND				= _T("card background\\test card-Bakct.bmp");	// PM��ӡ���Ա���ͼƬ

static const TCHAR* const DATABASE_FILE_NAME         = _T("ES.db");
static const TCHAR* const ES_MC_UPDATE_DIR			 = _T("Download\\MC\\");               // ���Ʋ��������Ŀ¼
static const TCHAR* const ES_RW1_UPDATE_DIR			 = _T("Download\\ESRW1\\");            // ES��д��1����Ŀ¼
static const TCHAR* const ES_RW2_UPDATE_DIR			 = _T("Download\\ESRW2\\");            // ES��д��2����Ŀ¼
static const TCHAR* const ES_RW3_UPDATE_DIR			 = _T("Download\\ESRW3\\");            // ES��д��3����Ŀ¼
static const TCHAR* const PM_RW_UPDATE_DIR			 = _T("Download\\PMRW\\");             // PM��д������Ŀ¼
static const TCHAR* const CARD_RW_UPDATE_DIR         = _T("Download\\CARDRW\\");           // ��ʽ��д������Ŀ¼
static const TCHAR* const ES_PM_UPDATE_DIR			 = _T("Download\\PM\\");               // ��������Ŀ¼
static const TCHAR* const ES_BLANK_CARD_LIST_DIR     = _T("blankcardlist\\");					// �׿��嵥���·��
static const TCHAR* const ES_MC_DOWNLOAD_DIR		 = _T("PG\\MC\\");					   // ���Ʋ���������Ŀ¼
static const TCHAR* const ES_TPU_MAIN_DOWNLOAD_DIR	 = _T("PG\\TPU-MAIN\\");			   // TPU���س�������Ŀ¼
static const TCHAR* const ES_TPU_APP_DOWNLOAD_DIR	 = _T("PG\\TPU-APP\\");				   // TPUӦ�ò���������Ŀ¼
static const TCHAR* const ES_PM_DOWNLOAD_DIR		 = _T("Download\\PM\\");			   // ��������Ŀ¼
//////////static const TCHAR* const UPGRADE_DATA_DESC_FILE_NAME = _T("upgrade.description");		   // �����ļ�˵���ļ�



////////////////////���峣����ö��///////////////////////////////
// ϵͳ��ֵ�ж��������
typedef void*               CardBlacklistStruct_t;      // ������������
typedef void*               ProductBlacklistStruct_t;   // ��Ʊ����������
typedef void*               PayloadDetailsStruct_t;     // �嵥��Ч��������
typedef void*               SamBlacklistStruct_t;       // ����SAM ��ʧ�Ŀ�ʼ�ͽ���ʱ��
typedef void*               FeedbackIdStruct_t;         // ��������������������ֺ͵ƹ�ָʾ��ע�⣺��������ṹ�����в�����������Ч�ġ�
typedef void*               SoundIdStruct_t;            // �趨�豸�������ԡ�
typedef void*               TimeZoneInfoStruct_t;       // ʱ����Ϣ��
typedef void*               PayloadDetailsStruct_t;     // �嵥��Ч��������
typedef void*               SamBlacklistStruct_t;       // ����SAM ��ʧ�Ŀ�ʼ�ͽ���ʱ��
typedef void*               SoundIdStruct_t;            // �趨�豸�������ԡ�
typedef void*               FareCodeId_t;               // δ����

// ���Ļ�������
typedef char                Byte_t;
typedef unsigned long       DateC19_t;                  // ������1799��12��31������0=��δ�趨����1=1800��1��1��
typedef unsigned short      DateC20_t;                  // ������1899��12��31������0=��δ�趨����1=1900��1��1��
typedef char                ExceptionID_t[3];
typedef short               S16_t;
typedef long                S32_t;
typedef __int64             S64_t;         
typedef char                S8_t;         	
typedef unsigned short      U16_t;        	
typedef unsigned long	    U32_t;        
typedef unsigned __int64    U64_t;                  
typedef unsigned char	    U8_t;         
typedef U16_t               Time_Days_t;   	
typedef S32_t       	    Time_t;     

// ϵͳ��������
typedef U8_t                ActionSequenceNumber_t;     // ��������Ķ�����ֵ���������Եĺ���ȡ������ϵͳ��ʹ�á�
typedef char                Address_t[41];              // ��ʾһ������Ľֵ���ַ��
typedef U32_t               ApplicationSerialNumber_t;  // Ʊ���ϱ�ʶӦ�õĺ��롣
typedef U32_t               AuditRegisterValue_t;       // ��������ּ�ڴ洢�������ͻ��ۼ�����������ݵ�ֵ.
typedef U16_t               CalendarId_t;               // ������ʶ��
typedef U16_t               CardBatchNumber_t;          // Ʊ��������Ʊ����������ʱд��Ʊ���ϵı�š�
typedef char                CardholderName_t[61];       // ASCII �ַ������ڱ�ʶ�ֿ��˵ı�š�	
typedef char                CardholderNum_t[19];   	    // �ֿ��˵�Ψһ��ʶ��
typedef U32_t               CardholderSerialNumber_t;   // ��ʶ�ֿ��ˡ����ֵΪ0xFFFFFFFF �����ʾ������δ���˻���
typedef char                CardholderTitle_t[5];       // �ֿ��˵�ͷ�Σ���������Ůʿ�����ڵȡ�
typedef U32_t               CardIssuerId_t;             // ����ISO/IEC 7812 ��ע�Ῠ������: 000,000 �� 999,999
typedef U32_t               CardSerialNumber_t;         // �Զ�����ֵ��ʶʵ��Ʊ����       
typedef char	            CartonId_t[10];             // ��ʶ����һ��Ʊ����Ʊ���ASCII �ַ�
typedef U32_t               CdAttributeId_t;            // �ж�BEI-00040 ��˵��CD ���ԡ�
typedef char                CdAttributeName_t[51];      // CD���Ե�����
typedef U16_t               CdPayloadDataVersion_t;     // ������ָ��Ч�����ڵ����ݣ������ݷ����ı�ʱ���ӡ�
typedef U16_t               CdPayloadFormatVersion_t;   // ��Ч���صĽṹ�汾,����Ч���ط����ṹ�Ըı䣨�����ԡ�ɾ�������ԡ����ҵĸ��Ӽ���ʱ����
typedef char                CdPayloadName_t[12];        // Ψһ��CD ��Ч���ص����ƣ�12 �ַ���
typedef U16_t               CdSetVersion_t;             // CD���İ汾�� 
typedef char                Char8_t;                    // ����һ��ϵͳʹ�õĲ������ŵ��ַ����͡�    	
typedef U8_t                CounterEight_t;             // ����һ����;�İ�λ��������
typedef U16_t               CounterSixteen_t;   	    // ͨ�õ�16 λ������
typedef U16_t               DayTypeId_t;                  // ��ʶ�������ͣ�����ĩ�����գ��ȣ�������Ŀ�ڿ����õ�λ�ֶΡ�
typedef U8_t                DeviceFeedbackId_t;         // �����豸��Ƶ�ͣ�����Ƶ����� 
typedef U32_t               DeviceId_t;                 // �˺�����AFC ϵͳ��Ψһ��ʶ�豸�������Ͱ������м������ͣ�HardwareType_t��ռ������Ч��16 λ��HardwareSequenceNumber_t��ռ�������Ч��16 λ��  	
typedef U16_t               Duration_t;   	            // ͨ����λ��ֵ����ʾ����ʱ�䡣DurationUnit_t ռ������Ч��4 λ��DurationValue_t ռ�������Ч��12 λ��
typedef U32_t               DurationValue_t;            // ָ��ĳ����ĳ����ڼ䣨���磬��Ʒ�ĳ����ڣ���
typedef char                EmailAddress_t[129];        // �������ʼ���ַ��ʹ������
typedef U32_t               Fare_t;                     // ����ָ��һ������ֵ��
typedef U16_t               FareCode_t;                 // �����ڷ��ñ���ȷ��ĳ�еķ��ô��롣 
typedef U16_t               FareCodeTableId_t;          // ��ʶ��Ҫ���ڷ��ü���ķ��ô����	
typedef U16_t               FarePatternId_t;   	        // ��ʶ����ģʽ��
typedef U8_t                FareSetId_t;                // �������ñ�ʶ����
typedef U16_t               FareTableId_t;              // ���ñ��Ψһ��ʶ��
typedef U16_t               GraphicId_t;                // ���ڱ�ʶƱ���ϵ�ͼ����һ�����
typedef U16_t               HardwareSequenceNumber_t;   // ͨ��ΪӲ�������˳��ű�ʶAFC ϵͳ�����õ�Ӳ����
typedef U16_t               JourneyType_t;              // ��ʶ������ڻ����������������漰����ڡ� 
typedef U16_t               KeyVersion_t;               // ����ȷ���ļ���ȫ����Կ�İ汾��
typedef U16_t               KeyNumber_t;                // Ϊ����ϵͳʹ�ö���������Կ�롣��ʶĳ����ȫ����Կ��������汾����
typedef U16_t               LifeCycleCounter_t;         // ��ʶƱ����ѭ���������Ķ�����ֵ��  	
typedef U32_t               LocationCode_t;             // ��ʶλ�ô���Ķ�����ֵ��
typedef U32_t               LocationNumber_t;           // ͨ�����ͺʹ����ʶλ�á���LocationType_t�������ڡ�LocationNumber_t��������Ч�ֽ��С�
typedef U32_t               StationNumber_t;            // ͨ�����ͺʹ����ʶλ�á���LocationType_t�������ڡ�StationNumber_t��������Ч�ֽ��С�
typedef U16_t               NameRepresentationType_t;   // ���ڰ������ڱ�������(NameEncodingType_t)�������ض��仯(NameVariantType_t)����Ϣ��
typedef U32_t               ParticipantId_t;            // ��ʶ�����еĲ����ߡ� 
typedef U8_t                PassengerType_t;            // ���Ƕ����ܷ�����ˡ��������ķ��ࡣ	
typedef char                Password_t[16];   	        // ����һ���������Null Ϊ�ս����ASCII �ַ���
typedef char                PasswordClue_t[21];         // �������������ASCII �ַ����˿������������������������ǵ����롣
typedef U16_t               Percent_t;                  // ȷ����ȷ��2 ��С��λ�İٷֱ�(0..100)��
typedef U32_t               ProductIssuerId_t;          // ��Ʒ�����̱�ʶ
typedef U16_t               ProductSerialNumber_t;      // ����һ����Ʊ����Ӧ���б�ʶ��Ʒ�ĺ��롣
typedef U16_t               ProductType_t;              // ������ֵ��ȷ���˱������������׵Ĳ�Ʒ����Щ���Ƕ��ض�����Ŀ���Ե�
typedef U16_t               ProductTypeVariant_t;       // ��ʶ�ض�ProductType_t�ı���
typedef U16_t               Ptsn_t;                     // ������ֵ��һ����Ʒ����ض�˳�����
typedef U16_t               ReceiptNum_t;               // һ����;���վݺ��롣 
typedef U8_t                RefundLevel_t;              // ������ʾĳ�����ڵ��˿����Ľ��г̶ȵ�һ��������ֵ��
typedef U8_t                Rides_t;                    // �������� �����û��ڳ��������Ĳ�Ʒ
typedef U8_t                RoleType_t;                 // �������Ա�Ľ�ɫ������������о١���ֵ��CD ���塣
typedef U32_t               SamId_t;                    // �������飨��SAM����Ψһ��ʶ
typedef U32_t               ShiftSequenceNumber_t;      // ��ʶ�ض�����Ա���ְ�ĺ��룬��SAM ά����
typedef char                StaffCodeNumber_t[10];      // ��ʾԱ��������
typedef char                StaffId_t[10];              // ASCII �ַ�����ĳ��Ӧ�ù�Ӧ�̷�Χ�ڱ�ʶ��ĳ��������Ա����ʱ����Ӧ�ù�Ӧ��ͨ���Ƿ���Ӧ�̣���
typedef char                TelephoneNumber_t[21];      // ����绰���루һ����;����ASCII �ַ���
typedef U8_t                TimeCode_t;                 // ʱ�����޵Ĳο���ͨ��CD ���ͣ� 
typedef U8_t                TxnMac_t[8];                // �йؽ��׵İ�ȫ��Ϣ��
typedef U8_t                UD_Subtype_t;               // ������Դ��Դ�������б�ʶ��Ϣ��
typedef U8_t                UD_Type_t;                  // ȷ������Դ�ڵ����Դ��
typedef U32_t               UDSNum_t;                   // �����������кš�
typedef U16_t               ValidityDistance_t;         // ��Ʒ��Ч�ĵ�����ǧ�׵���Ŀ����Ч�Ե�λ�ɲ�Ʒ������ȷ����
typedef S32_t               ValueOneUnit_t;             // ���л�����С��λ��ʾ�Ļ��Ҽ�ֵ��
typedef U16_t               Version_t;                  // ������ֵ��д��Ʊ������Կ�ġ�SecurityKeyTable���İ汾
typedef U8_t                Years_t;                    // ����ֵ�ļ�������

/////////////////////////////// ����ΪƱ�����״���������ݶ���///////////////////////////
//�̶���������
typedef unsigned long        KeyTableVersionNumber_t;                // ���ؽ�����ģ������һ����Կ��İ汾���롣
typedef unsigned long        SoftwareVersion_t;                      // ����İ汾���롣
typedef unsigned short       SystemReferenceId_t;                    // BinaryValue ������ֵ
// ���Ͷ���
typedef Time_t              TimeT;                      // ����/ʱ�䡣��ʾ��1970 ��1 ��1 �� 00��00 �����������Χ��ԼΪ68 �꣩��0 = δָ���� 
// 1 = 1970 ��1 ��1 ��00��01��ע����time_t ��ͬ����������������δ��ǵ�
typedef unsigned long       TimeM26_t;                  // ����/ʱ�䡣��ʾ��2006 ��1 ��1 �� 00��00 ��ķ���������Χ��ԼΪ32 �꣩��0 = δָ���� 
// 1 = 2006 ��1 ��1 ��00��01��
typedef unsigned short      CardBaseDateTime_t;     // ����Ultralight ����ʼ��ʱ�Ŀ�ʼ������/ʱ�����£�
													// 0= 2006 ��1 ��1 �� 00��00
													// 1= 2006 ��2 ��1 �� 00��00
													// 2= 2006 ��3 ��1 �� 00��00
typedef unsigned short      TimeOfDayMinutes_t;         // ����������ҹ��ʼ�ķ���������Χ��0..1440
typedef unsigned short      DateC26_t;                  // ���ڡ���ʾ��2006 ��1 ��1 ����ķ���������Χ��ԼΪ179 �꣩��0 = δָ����1 =2006 ��1 ��2 �ա�
typedef unsigned short      CardLocation_t;             // �洢�ڿ���ӳ���λ�ô��롣ͨ��һ��ӳ���ӳ�䵽StationNumber_t���ñ�������4094 ��λ�ã�0 = ������0xFFF = δָ����
typedef unsigned char       CardParticipant_t;          // �洢�ڿ���ӳ��Ĳ��뷽ID��ͨ��һ��ӳ���ӳ�䵽ParticipantId_t���ñ�������253�����뷽��0 = ������0xFF = δָ����
typedef unsigned long       DeviceMode_t;               // ˵����ǰ��Ӫģʽ���豸ģʽ�ı��룬���У�
														// 1  = ����ģʽ
														// 2  = �ر�ģʽ
														// 3  = �г�����ģʽ
														// 4  = ��վ���ģʽ
														// 5  = ��վ���ģʽ
														// 6  = ��������ģʽ
														// 7  = ���� �� ʱ�����ģʽ
														// 8  = ���� �� �������ģʽ
														// 9  = ���� �� �������ģʽ
														// 10 = ���� �� ʱ�� �� �������ģʽ
														// 11 = ���� �� ʱ�� �� �������ģʽ
														// 12 = ���� �� ���� �� �������ģʽ
														// 13 = ���� �� ʱ�� �� ���� �� �������ģʽ
														// 14 = ��վ �� ʱ�����ģʽ
														// 15 = ��վ �� �������ģʽ
														// 16 = ��վ �� �������ģʽ
														// 17 = ��վ �� ʱ�� �� �������ģʽ
														// 18 = ��վ �� ʱ�� �� �������ģʽ
														// 19 = ��վ �� ���� ���������ģʽ
														// 20 = ��վ �� ʱ�� �� ���� ���������ģʽ
														// 21 = ��վ �� ʱ�����ģʽ
														// 22 = ��վ �� �������ģʽ
														// 23 = ��վ �� �������ģʽ
														// 24 = ��վ �� ʱ�� �� �������ģʽ
														// 25 = ��վ �� ʱ�� �� �������ģʽ
														// 26 = ��վ �� ���� ���������ģʽ
														// 27 = ��վ �� ʱ�� �� ���� ���������ģʽ
														// 28 = ��վ �� ��վ ��ʱ�����ģʽ
														// 29 = ��վ �� ��վ ���������ģʽ
														// 30 = ��վ �� ��վ ���������ģʽ
														// 31 = ��վ �� ��վ ��ʱ�� �� �������ģʽ
														// 32 = ��վ �� ��վ ��ʱ�� �� �������ģʽ
														// 33 = ��վ �� ��վ ������ ���������ģʽ
														// 34 = ��վ �� ��վ ��ʱ�� �� ���� ���������ģʽ
typedef U8_t                CardPaymentMethod_t;
typedef U8_t                LogTransactionType_t;
typedef U8_t                JourneyStatus_t;
typedef bool                CardLanguageId_t;
typedef U8_t                LiteCardStatus_t;
typedef U8_t                S1kCardStatus_t;
typedef U8_t                S1kProductStatus_t;
typedef unsigned char       ValidityDurationType_t;     // �涨һ���ƴ�/���ڲ�Ʒ��ValidityDuration �Ƿ����������Сʱ�����߷������洢��0.1=������2 =��������3 =Сʱ����4��������5��������6������������    

// ������־����
static const unsigned char  LOG_TRANSACTION_TYPE_MULTIRIDE_USE_ENTRY            = 17;
static const unsigned char  LOG_TRANSACTION_TYPE_MULTIRIDE_USE_EXIT             = 18;
static const unsigned char  LOG_TRANSACTION_TYPE_PASS_USE_ENTRY                 = 17;
static const unsigned char  LOG_TRANSACTION_TYPE_PASS_USE_EXIT                  = 18;

// ����ʱ��
static const unsigned long  DATE_C_19_MAXIMUM                                   = 0xFFFFFFFF;
static const unsigned char  DATE_C_19_UNSPECIFIED                               = 0;
static const unsigned char  MINS_255                                            = 0xFF;
static const unsigned short DATE_C_20_MAXIMUM                                   = 0xFFFF;
static const unsigned char  TIME_T_UNSPECIFIED                                  = 0;
static const unsigned char  DATE_C_20_UNSPECIFIED                               = 0;
static const unsigned char  TIME_M_26_UNSPECIFIED                               = 0;
// δʵ��
static const unsigned short DAYS_BETWEEN_DATEC20_AND_DATEC26                    = 38717;

//��Ҫ
static const short          NUMBER_OF_AUDIT_REGISTERS                           = 300;
static const unsigned long  UD_FORMAT_VERSION                                   = 0x00020000;
static const unsigned long  LOCATION_NUMBER_UNSPECIFIED                         = 0x00000000;
static const unsigned long  VALUEONEUNIT_UNSPECIFIED                            = 0xFFFFFFFF;
static const unsigned char  PASSENGER_TYPE_UNSPECIFIED                          = 0xFF;
static const unsigned char  RIDES_UNSPECIFIED                                   = 0xFF;
static const unsigned short DURATION_UNSPECIFIED                                = 0xFFFF;

//��Ч���о�
static const unsigned char  VALIDITY_DURATION_UNIT_MIN                          = 0;
static const unsigned char  VALIDITY_DURATION_UNIT_HOUR                         = 1;
static const unsigned char  VALIDITY_DURATION_UNIT_DAY                          = 2;
static const unsigned char  VALIDITY_DURATION_UNIT_CALENDAR_MONTH               = 3;
static const unsigned char  VALIDITY_DURATION_UNIT_UNSPECIFIED                  = 0xFF;

//Ʊ���о�
static const unsigned short CARD_VERSION_UNINITIALISED                          = 0;
static const unsigned short APPLICATION_SERIAL_NUMBER                           = 1;
static const unsigned short APPLICATION_STATUS_CODE_NOT_BLOCKED                 = 1;
static const unsigned char  AUTOLOAD_PRODUCT_ID_UNSPECIFIED                     = 0xFF;
static const unsigned short MIN_TRANSACTION_SEQUENCE_NUMBER                     = 0;
static const unsigned short REFUND_LEVEL_UNSPECIFIED                            = 0;
static const unsigned short RECEIPT_NUMBER_UNSPECIFIED                          = 0;
static const TCHAR* const    CARDHOLDER_NUMBER_UNSPECIFIED                       = _T("000000000000000000");
static const TCHAR* const    CARDHOLDER_NAME_UNSPECIFIED                         = _T("");
static const unsigned short MOC_INDUSTRY_CODE                                   = 5320;
static const unsigned char  CARD_LIFECYCLE_INITIAL_VALUE                        = 0;
static const unsigned short MAX_CARD_BASE_DATE_TIME                             = 511;

//�г̲�ƷID �о�
static const unsigned char  JOURNEY_PRODUCT_ID_PURSE                            = 0;
static const unsigned char  JOURNEY_PRODUCT_ID_PASS_MULTIRIDE                   = 1;
static const unsigned char  JOURNEY_PRODUCT_ID_UNSPECIFIED                      = 0xFF;

// Ʊ���ó�״̬����
static const unsigned char  JOURNEY_NONE                                        = 0;
static const unsigned char  JOURNEY_CHECKED_IN                                  = 1;
static const unsigned char  JOURNEY_CHECKED_OUT                                 = 2;
static const unsigned char  JOURNEY_CHECKED_OUT_TRAIN_FAULT                     = 3;
static const unsigned char  JOURNEY_CHECKED_IN_COMPENSATION_FARE_PAID           = 4;

// ���۳���
static const unsigned char  MAX_NUMBER_PRODUCTS_FOR_CARD_ENQUIRY                = 10;
static const unsigned char  REFUND_LEVEL_PROCESS_STARTED                        = 1;
static const unsigned char  MAX_NUMBER_PRODUCT_HISTORY_FOR_CARD_ENQUIRY         = 10;

// ��������
static const U16_t KEY_VERSION_UNSPECIFIED                                      = 0xFFFF;
static const U16_t KEY_NUMBER_UNSPECIFIED                                       = 0xFFFF;
static const U32_t UNSPECIFIED_CARD_PARTICIPANT                                 = 0xFFFFFFFF;
static const U16_t LIFE_CYCLE_COUNTER_UNSPECIFIED                               = 0xFFFF;
static const U32_t CARD_SERIAL_NUMBER_UNSPECIFIED                               = 0xFFFFFFFF;
static const DateC20_t      UNSPECIFIED_DATEC20                                 = 0xFFFF;
static const U32_t          UDSNUM_UNSPECIFIED                                  = 0xFFFFFFFF;

// CD�汾��Ч���ж�ֵ
static const unsigned long UNAVALIBLE_VERSION     = 0xFFFFFFFF;


///////////////////////////////// �����￪ʼ��ϵͳ�������Ķ���///////////////////////////////////////////

// �����̱��붨��
typedef enum _tagISSUANCER_CODE{
	ACC = 0x04,		// ACC
	ECT = 0x01		// YKT
} ISSUANCER_CODE;
// ҵ��״̬
typedef enum _service_state {
	INIT						= 0x00,         // ��ʼ�У�ϵͳ��ʼ����
	OUT_OF						= 0x01,         // ��ֹ�� ����Ԯʱ��
	READY						= 0x02,         // �ȴ��� ���ȴ��˿Ͳ��������Ա������
	RECEPT						= 0x03,         // ������ ���˿Ͳ�����
	COUNTING					= 0x04,			// ������  ��⵽���ֽ�Ͷ��ʱ��
	ISSUEING					= 0x05,         // �豸�����У���Ʊ�����е�Ӳ������ִ�е�ʱ��
	CANCELING					= 0x06,			// ������ ���û�ȡ�������������ʱʱ��
	MAINTENANCE					= 0x07,         // ά���� ������Ա��¼��
	ABNORMAL					= 0x08,         // �쳣�� ��TVM�����쳣��
	REST						= 0x09,         // ������ ����沥��ʱ��
	FINISH						= 0x0A,         // ����	  ����Ʊ���ֵ��ɺ�
	WAIT_MAINTENANCE			= 0x0B,         // �ȴ�ά���У�ά���ſ�����δ��¼��
} SERVICE_STATE;

// �豸����
typedef enum _afc_device_type{
	DEVICE_TYPE_TVM					=	0x16,		// �Զ���Ʊ�� 
	DEVICE_TYPE_TOM					=	0x14,		// ���Զ���Ʊ��
	DEVICE_TYPE_AVM					=	0x19,		// �Զ���ֵ��
	DEVICE_TYPE_VM					=	0x04,		// ������Զ���Ʊ��
	DEVICE_TOM						=	DEVICE_TYPE_TOM,
	DEVICE_VM						=	DEVICE_TYPE_VM,	//Ϊ�˼���֣����Ŀ
	DEVICE_TYPE_AG					=	0x1F,		// �Զ���Ʊ��
	DEVICE_TYPE_HT					=	0x23,		// �ֳּ�Ʊ��
	DEVICE_TYPE_ES					=	0x3D,		// ����ּ��
	DEVICE_TYPE_PM					=	0x3E,		// ���Ի���ӡ��
	DEVICE_TYPE_CCS_SERVER			=	0x62,		// ��·���ķ�����
	DEVICE_TYPE_SC_SERVER			=	0x5d,		// ��վ���ķ�����
	DEVICE_TYPE_CCS_WS				=	0x81,		// ��·���Ĺ���վ
	DEVICE_TYPE_SC_WS				= 0x82,		// ��վ���Ĺ���վ
	DEVICE_TYPE_ROUTE				=	0x51,		// ·����
	DEVICE_TYPE_SWITCH				=	0x52,		// ������
	DEVICE_TYPE_UNKNOW				=	0xFF		// δ����
}AFC_DEVICE_TYPE;

// �豸������
typedef enum _device_subtype{
	DEVICE_SUBTYPE_AGM_NORMAL		=	0x01,	// ͨ��բ��
	DEVICE_SUBTYPE_AGM_WIDE			=	0x02,	// ��ͨ��բ��
	
	DEVICE_SUBTYPE_ES_CARD			=	0x01,	// ��ʽES
	DEVICE_SUBTYPE_ES_TOKEN			=	0x02,	// TOKENʽES
	DEVICE_SUBTYPE_PM				=	0x04,	// PM
	DEVICE_SUBTYPE_ES_CARD_PM		=	0x05,	// ��ʽES & PM
	DEVICE_SUBTYPE_ES_TOKEN_PM		=	0x06,	// TOKENʽES & PM
	DEVICE_SUBTYPE_UNKNOW			=	0x00 	// δ����
}DEVICE_SUBTYPE;

//  ���Ա��붨�壨 ISO 639-2 ��
typedef enum _languageType{  
	MODE_UNKNOWN						= 0,		//�����Լ�
	LANGUAGE_ID_CHINESE				= 2245,	// ��������
	LANGUAGE_ID_ENGLISH				= 3751,	// Ӣ��
	LANGUAGE_ID_PORTUGUESE			= 11224,// ��������
	LANGUAGE_ID_CANTONESE			= 17451,// ����
	LANGUAGE_ID_UNSPECIFIED			= 65535,	// δ����
	CHINESE=LANGUAGE_ID_CHINESE,
	ENGLISH=LANGUAGE_ID_ENGLISH,
	PORTUGUESE=LANGUAGE_ID_PORTUGUESE,
	TRADITIONAL_CHINESE=LANGUAGE_ID_CANTONESE
}LANGUAGETYPE_T;

// ���ͽ���ģʽ����
typedef enum _send_mode{
	MODE_SEND		= 1,
	MODE_RECEIVE    = 2
}SEND_MODE;


// Ʊ��ָ������
typedef	enum Magazine_type
{
	MAGAZINE_NONE =	0x0000,			   // ��ָ��Ʊ��
	MAGAZINE_A	  =	0x0001,			   // Ʊ��A��Ʊ
	MAGAZINE_B	  =	0x0002,			   // Ʊ��B��Ʊ
	MAGAZINE_R	  =	0x0003,			   // ������
} MAGAZINE_TYPE;

// acc��Ʊ״̬
//typedef enum acc_ticket_statue
//{
//	INIT_T = 1,			        //��ʼ��
//	SALE_T = 2,					//������ 
//	ENTER_T = 3,				//�ѽ�վ
//	ENTER_UPDATE_T = 4,			//��վ�����
//	EXIT_T = 5,					//�ѳ�վ 
//	EXIT_UPDATE_T= 6,			//��վ�����
//	REFUND_T = 7,				//���˿�
//	OVER_T = 8,					//��ע 
//	UNKNOW_T = 9,				//δ֪
//}ACC_TICKET_STATUE;
// ECT��Ʊ״̬
//typedef enum _ect_ticket_statue
//{
//	UNUSETICKET = 1,			        //δ����
//	USE = 2,					//���� 
//	STOP = 3,	    			//ͣ��
//	BLACKET = 4,				//��������
//	RECYCLED = 5,				//���� 
//	
//}ECT_TICKET_STATUE;

// ��֧���Ͷ���
typedef enum business_type{		
	BUSINESS_TYPE_NONE = 0,        // Ĭ��
	BUSINESS_TYPE_INCOME = 1,	   // ����
	BUSINESS_TYPE_EXPENSE = 2,     // ֧��
}BUSINESS_TYPE;	

// ���ݵ����������Ͷ���
typedef	enum _export_datatype
{
	PAM_TYPE = 0x01,
	TXN_TYPE = 0x02,
	BUS_TYPE = 0x03,
	UNKNOW_TYPE   = 0xFF //δ֪
} EXPORT_DATA_TYPE;

// 1.ϵͳ������� �Ķ������
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// �����￪ʼ�ǳ�Ʊ����صĶ���



//////////////////////////////////////////////////////////////////////////
// �����￪ʼ��	3.IC R/W I/Fʽ���� �Ķ���
// SAM�۱��
typedef	enum _slot_id
{
	SLOT1 =	0x00,									// һ��ͨPSAM����
	SLOT2 =	0x01,									// һ��ͨISAM����
	SLOT3 =	0x02,									// ����PSAM����
	SLOT4 =	0x03									// ����ISAM����
} SLOT_ID;

// SAM���ඨ��
typedef enum _sam_type {
	RW_ACC_ISAM = 0x01,
	RW_ACC_PSAM = 0x02,
	TH_ACC_ISAM = 0x03,
	TH_ACC_PSAM = 0x04,
	RW_ECT_ISAM = 0x05,
	RW_ECT_PSAM = 0x06,
	TH_ECT_ISAM = 0x07,
	TH_ECT_PSAM = 0x08
}SAM_TYPE;

// SIM ID��Ϣ
typedef struct _sam_id_info{
	BYTE rw_acc_psam_id[6];
	BYTE rw_acc_isam_id[6];
	BYTE rw_ect_psam_id[6];
	BYTE rw_ect_isam_id[6];
	BYTE th_acc_psam_id[6];
	BYTE th_acc_isam_id[6];
	BYTE th_ect_psam_id[6];
	BYTE th_ect_isam_id[6];

	_sam_id_info(){
		memset(rw_acc_psam_id, 0, 6);
		memset(rw_acc_isam_id, 0, 6);
		memset(rw_ect_psam_id, 0, 6);
		memset(rw_ect_isam_id, 0, 6);
		memset(th_acc_psam_id, 0, 6);
		memset(th_acc_isam_id, 0, 6);
		memset(th_ect_psam_id, 0, 6);
		memset(th_ect_isam_id, 0, 6);
	}
}SAM_ID_INFO;

// �豸��������
typedef enum _device_component{
	BOM_DEVICE_APPLI		 = 0x20, // BOM���ز�	    0x20
	BOM_DEVICE_CUSTOMER_LCD  = 0x21, // BOM�˿���ʾ��	0x21
	BOM_DEVICE_OFFICER_LCD	 = 0x22, // BOM��Ʊ��ʾ��	0x22
	BOM_DEVICE_TICKET_MODULE = 0x23, // BOM��Ʊ����ģ��	0x23
	BOM_DEVICE_PRINTER       = 0x24, // BOM���ݴ�ӡ��	0x24
	BOM_DEVICE_CASH_BOX      = 0x25, // BOM�ֽ�ģ��	    0x25
	BOM_DEVICE_OUT_TPU       = 0x26, // BOM����TPU	    0x26
	BOM_DEVICE_INNER_TPU     = 0x27, // BOM�ڲ�TPU	    0x27
	BOM_DEVICE_CAMERA        = 0x28, // BOM����ͷ       0x28
	BOM_DEVICE_CARDPRINTER   = 0x29, // BOMƱ����ӡ��   0x29
	BOM_DEVICE_SCANNER       = 0x2A, // BOMɨ����       0x28
	BOM_DEVICE_LASERPRINTER  = 0x2B, // BOM�����ӡ��   0x29

	TCM_DEVICE_APPLI		 = 0x20, // TCM���ز�	    0x20
	TCM_DEVICE_CUSTOMER_LCD  = 0x21, // TCM�˿���ʾ��	0x21
	TCM_DEVICE_OFFICER_LCD	 = 0x22, // TCM��Ʊ��ʾ��	0x22
	TCM_DEVICE_TICKET_MODULE = 0x23, // TCM��Ʊ����ģ��	0x23
	TCM_DEVICE_PRINTER       = 0x24, // TCM���ݴ�ӡ��	0x24
	TCM_DEVICE_CASH_BOX      = 0x25, // TCM�ֽ�ģ��	    0x25
	TCM_DEVICE_OUT_TPU       = 0x26, // TCM����TPU	    0x26
	TCM_DEVICE_INNER_TPU     = 0x27, // TCM�ڲ�TPU	    0x27
	TCM_DEVICE_CAMERA        = 0x28, // TCM����ͷ       0x28
	TCM_DEVICE_CARDPRINTER   = 0x29, // TCMƱ����ӡ��   0x29
	TCM_DEVICE_SCANNER       = 0x2A, // TCMɨ����       0x28
	TCM_DEVICE_LASERPRINTER  = 0x2B, // TCM�����ӡ��   0x29

	ACC_PSAM_RW				 = 0xA1, // �ⲿ��д��һƱͨPSAM
	ACC_ISAM_RW				 = 0xA2, // �ⲿ��д��һƱͨISAM
	ECT_PSAM_RW				 = 0xA3, // �ⲿ��д��һ��ͨPSAM
	ECT_ISAM_RW				 = 0xA4, // �ⲿ��д��һ��ͨISAM
	DEVICE_COMPOENT_UNKNOW = 0XFF
}DEVICE_COMPONENT;
// ���½���
typedef enum _update_suggest{
	NO_UPDATE = 0x00,                // �ó�Ʊ����Ҫ��Ʊ
	ENTER_STATION_UPDATE_81 =  0x81,   // ��վ����(��վ20�����ڽ�վ)
	ENTER_STATION_UPDATE_82 =  0x82,           // �뷢����Ʊ
	ENTER_STATION_UPDATE_83 = 0x83,    // ��վ����(��վ20�������վ)
	ENTER_STATION_UPDATE_84 = 0x84,    // ��վ����(�Ǳ�վ��վ)
	ENTER_STATION_UPDATE_85 = 0x85,      // ���ӳٳ�վ(����ģʽ)
	ENTER_STATION_UPDATE_86 =  0x86,   // ��վ����(����ģʽ��վ)
	ENTER_STATION_UPDATE_87 =  0x87,   // ��վ����(�г�����ģʽ����)
	ENTER_STATION_UPDATE_88 =  0x88,   // ��վ����(�г�����ģʽ��վ)
	OUT_STATION_UPDATE_21 = 0x21,    // ���ӳٽ�վ���޽�վ��־��Ĭ�Ϸ��۳�վΪ��վ��վ
	OUT_STATION_UPDATE_22 = 0x22,    // ���ӳٽ�վ���޽�վ��־����Ҫ���ճ˿�����������Ա�ж�ѡ���վ��վ���Գ�Ʊд���վ��
	OUT_STATION_UPDATE_23 = 0x23,    // ���ӳٽ�վ(��վ���ģʽ)
	OUT_STATION_UPDATE_24 = 0x24,        // ��ʱ����
	OUT_STATION_UPDATE_25 = 0x25,    // ���ӳٽ�վ
	OUT_STATION_UPDATE_26 = 0x26,        // ���̲�Ʊ
	OUT_STATION_UPDATE_27 = 0x27,        // ���̲�Ʊ->���г�վƱ
	OUT_STATION_UPDATE_28 = 0x28, // ����/��ʱ��Ʊ
	OUT_STATION_UPDATE_29 = 0x29,      // ���г�վƱ
	OUT_STATION_UPDATE_2A =  0x2A            // ���г�վƱ
}UPDATE_SUGGEST;
typedef	enum test_no
{
	TEST_NUM_LED                = 0x1100,	// // LED test
	TEST_NUM_MOTOR				= 0x1300,	// Motor test
	TEST_NUM_RESET				= 0x1400,	// Reset test
	TEST_NUM_RW					= 0x1600,	// R/W test
	TEST_NUM_CONT_CSC_ISSUE		= 0x1700,	// Continuous CSC issue	test
	TEST_NUM_SENSOR_SWITCH		= 0x2000,	// Sensor/Switch Test
	TEST_NUM_UNKNOWN			= 0xFFFF	// unknown
} TEST_NO;

// 3.IC	R/W	I/Fʽ���� �Ķ������
//////////////////////////////////////////////////////////////////////////

// Ʊ����ӡ��״̬����
typedef enum _pmStatus{
	PM_STATUS_UNKNOW  = 0x00, // δ֪״̬
	PM_STATUS_OFF	  = 0x50, // �ر�״̬
	PM_STATUS_ON	  = 0xD0, // �ϵ�״̬
	PM_STATUS_READY   = 0x18  // ����״̬
}PM_STATUS;

//////////////////////////////////////////////////////////////////////////
// �����￪ʼ��	4.CPS I/Fʽ����	�Ķ���
// 5.1��վ�豸״̬TK_TRANS_EntryEQState
//typedef	enum _TK_TRANS_EntryEQState
//{
//	NO_WORK	  =	0x00,									// ������
//	NORMAL	  =	0x01,									// ��ʽ
//	TEST	  =	0x02,									// ����
//	MAINTAIN  =	0x03,									// ά��
//	NO_USE    = 0xFF
//} TK_TRANS_EntryEQState;


// ���ڻ�����
typedef	enum _init_detail_error {
	INI_ERROR						= 0x00000001,			// ���ڻ�INI�ļ�����
	HOME_FOLDER_ERROR				= 0x00000002,			// ����������Ŀ¼����
	BACKUP_FOLDER_ERROR				= 0x00000004,			// �������ݱ���Ŀ¼����
	OPERATION_PARAMETERA_ERROR		= 0x00000008,			// Operation parameter A����
	DEVICE_TXNDATA_ERROR					= 0x00000010,			// ���ڻ��������ݴ���
	DEVICE_MASTER_ERROR					= 0x00000020,			// �ǲ�����master����
	INIT_TRACE_ERROR				= 0x00000040,			// ���ڻ�trace�д�
	INIT_DATACOUNT_ERROR			= 0x00000080,			// ���ڻ����ƴ���
	NTP_ERROR						= 0x00000100,			// NTPͬ������ʱ�����
	PARAMETER_ERROR					= 0x00000200,			// �������󣨲����ǲ�����master��
	VFARE_ERROR						= 0x00000400,			// Ʊ�۱����
	INIT_RW_ERROR					= 0x00000800,			// ���Ӷ�д������
	INIT_TH_ERROR					= 0x00001000,			// ��Ⲣ��ʼ����Ʊ������
	INIT_PRINT_ERROR				= 0x00002000,			// ���ڻ���ӡ������
	INIT_CAMERA_ERROR		        = 0x00003000,           // ���ڻ�����ͷ����
	BOX_CHANGE_ERROR				= 0x00004000,			// Ʊ�����������
	ADD_NEW_DATACOUNT_ERROR			= 0x00008000,			// ����һ�������ڼ����ļ�
	ACC_ACCREDIT_ERROR				= 0x00010000,			// ACCһƱͨISAM��֤����
	OVER_TIME_DATA_ERROR			= 0x00020000,			// ���ڳ�������δ�����ݴ���
	BMAC_ACCREDIT_ERROR				= 0x00040000,			// һ��ͨ��֤����
	INI_DATABASE_ERROR				= 0x00080000,			// ���ݿ��ʼ������
	INIT_STATUS_DISPLAY_ERROR				= 0x00100000,			// ״̬��ʾ����ʼ������
} INIT_DETAIL_ERROR;

// 5.69	//	operator login status
typedef	enum _login_off_flag
{
	LOG_IN  = 0x00,				  // login
	LOG_OFF = 0x01				  // logoff
} LOGIN_OFF_FLAG;


// 5.86	Magazine data
typedef	enum _operation_type
{
	ATTACH			= 0x01,				  // ATTACH
	DETACH			= 0x02,				  // DETACH
	UNUSE			= 0xFF				  // unuse
} TICKETBOX_OPERATION_TYPE;
								  

// 5.14Magazine	1 status
typedef	enum _magazine1_status
{
	MAG_NORMAL	        = 0x00,		// normal
	MAG_NEAR_EMPTY      = 0x01,		// near	empty
	MAG_EMPTY		    = 0x02		// empty
} MAGAZINE1_STATUS;

// �豸��Ӧ��ID
static const WORD  FOUNDER_ID = 0x0011;

// Collection_Box
typedef	enum _collection_box
{
	LESS_FULL			  =	0x00,		  //  less than	nearly full
	NEARLY_FULL			  =	0x03,		  //  nearly full		 
	IS_FULL				  =	0x04		  //  full					   
} COLLECTION_BOX;											  

// IN_OFF_SVC
typedef	enum _in_off_sev
{
	IN_SVC				  =	0x00,		   //  In service
	IN_SVC_WARNING		  =	0x01,		   //  In service (with	warning)						 
	OUT_SVC_NORMAL		  =	0x02,		   //  Out of service (under normal	conditions)			 
	OUT_SVC_ERROR		  =	0x03,		   //  Out of service (under error conditions)			 
	OUT_SVC_MAINTENANCE	  =	0x04		   //  Out of service (under maintenance)					 
} IN_OFF_SVC;

// Close_Start_flag
typedef	enum _close_start_flag
{
	CS_NORMAL 	            = 0x00,			// Normal
	CS_CLOSE			    = 0x01,			// Closing sequence
	CS_RESTART				= 0x02,			// Restart sequence
	CS_SHUTDOWN			    = 0x03,				// shut	down
	CS_EXIT				= 0x04,			//�˳����򲻹ػ�
	CS_EXITBOM				=CS_EXIT,
	CS_EXITTCM				= CS_EXIT			//�˳�TCM���򲻹ػ�
} CLOSE_START_FLAG;

// �汾�ĵ����ݽṹ
typedef struct _SC_VER_INFO
{
	TCHAR type[10];                  // TH��ʽ
	TCHAR version[2];                // TH�汾
	ULONG sum;                      // SUMֵ

	_SC_VER_INFO()
	{
		memset(type, 0x00, 10);
		memset(version, 0x00, 2);
		sum = 0;
	}
} SC_VER_INFO, *LPSC_VER_INFO;	//֣���¼�

// THģʽ����
typedef	enum 
{
	TH_MODE_NORMAL		= 0x00,				// ����ģʽ
	TH_MODE_ABNORMAL	= 0x01				// ����ģʽ
} TH_MODE;

// TH�豸����״̬����
typedef	enum set_mode
{
	SET		= 0x01,				  // ����
	UN_SET	= 0x00				  // δ����
} SET_MODE;

//Is_Authentication
typedef	enum _is_authentication
{
	AUTHENTICATED		 = 0x01,			//	Authenticated
	UNAUTHENTICATED		 = 0x02				//	Unauthenticated
} IS_AUTHENTICATION;
// 4.CPS I/Fʽ���� �Ķ������
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �����￪ʼ��	5.��Ʊencodeʽ���� �Ķ���


// 5.��Ʊencodeʽ���� �Ķ������
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �����￪ʼ��	6.һ��ͨʽ���� �Ķ���
// 5.7 Card_Trans_Status



// �����˿���ʶ
typedef	enum _refund_type
{ 
	REFUND_CARD     = 0x01,				// �˿�
	REFUND_MONEY    = 0x03				// ����
} REFUND_TYPE;


// �豸����״̬
typedef	enum _equip_status
{
	STATUS_NORMAL =	0x00,			// ����״̬
	NORMAL_TEST	  =	0x01,			// �ϴ������豸���������β����豸����
	TEST_NORMAL	  =	0x10,			// �ϴβ����豸���������������豸����
	TEST_TEST	  =	0x11			// �ϴβ����豸���������β����豸����	
} EQUIP_STATUS;


// һ��ͨ��������
typedef	enum _ykt_trans_type
{
	//YKT_TRTYPE_FREE	                = 0x00,             // ���ۿ�
	//YKT_TRTYPE_ISSUE	            = 0x01,             // �ۿ�
	//YKT_TRTYPE_CHARGE	            = 0x02,             // ��ֵ
	//YKT_TRTYPE_CONSUME	            = 0x06,             // ����
	//YKT_TRTYPE_UPDATE	            = 0x07,             // �޸�
	//YKT_TRTYPE_ADJUST	            = 0x08,             // ��Ʊ
	//YKT_TRTYPE_CASH	                = 0x09,             // �ֽ�
	//YKT_TRTYPE_BLOCK	            = 0x0A,             // ����
	//YKT_TRTYPE_REFUND	            = 0x0E,             // ����
	//YKT_TRTYPE_SURRENDER	        = 0x0F,             // �˿�
	//YKT_TRTYPE_EXCHANGE             = 0x10,             // ����
	//YKT_TRTYPE_VALUE_SHIFT1         = 0x11,             // ����
	//YKT_TRTYPE_VALUE_SHIFT2         = 0x12,             // ������
	//YKT_TRTYPE_INNER_DISCOUNT       = 0x16,             // �����Żݽ���(��ҵ��)
	//YKT_TRTYPE_DISCOUNT             = 0x26              // �����Żݽ���(��ҵ��)
} YKT_TRANS_TYPE;

//// һƱͨƱ״̬
//typedef	enum _ypt_ticket_status
//{
//	NORMAL_TICKET	 = 0x00,				// �����˳��೵Ʊ
//	TEST_MAINTENENCE = 0x01					// ά�������೵Ʊ
//} YPT_TICKET_STATUS;
// 6.һ��ͨʽ���� �Ķ������


//////////////////////////////////////////////////////////////////////////
////����Ʊ��ĳ��Ӧ�õĴ���״̬��
//typedef enum _processingStatus_t{  
//	PROCESSING_STATUS_RECEIVED                  = 1,        // �Ѿ��յ�Ӧ�á�
//	PROCESSING_STATUS_APPROVED                  = 2,        // �Ѿ���׼Ӧ�á�
//	PROCESSING_STATUS_REJECTED                  = 3,        // �Ѿ��ܾ�Ӧ�á�
//	PROCESSING_STATUS_UNSPECIFIED               = 255       // ���趨ֵ��
//}ProcessingStatus_t;
//
////ȷ����Щ��ͨ���������Բ�Ʒ��ȡ�Ĳ�����
//typedef enum _productActionCode_t{  
//	PRODUCT_ACTION_CODE_BLOCK                   = 1,        // ������Ʒ��
//	PRODUCT_ACTION_CODE_UNBLOCK                 = 2,        // ������Ʒ��
//	PRODUCT_ACTION_CODE_UNSPECIFIED             = 15        // δָ���Ķ�������
//}ProductActionCode_t;

////ĳ��Ŀ��Ʊ���ϱ����ߵ�ԭ��
//typedef enum _removalReason_t{  
//	REMOVAL_REASON_INVALID                   = 1,       // ��Ч���������ɡ�
//	REMOVAL_REASON_GARBAGE_COLLECTED         = 2,       // ����Ʒ�ӿ��ϱ�����ʱ�����ͽ���TxnProductDelete_t�� UD
//	REMOVAL_REASON_UNSPECIFIED               = 255      // δָ����
//}RemovalReason_t;


//////////////////////////////////////////////////////////////////////////
// �����￪ʼ��	7.���� �Ķ���


//����ͳ��
typedef	enum _SALESTAT_KIND
{
	CURRENT_DATE = 1,	  //ȫ��
	STAFF_ONCE	 = 2	  //����
} SALESTAT_KIND;

// 7.���� �Ķ������

typedef enum _status_type{
	STATUS_TYPE_FULL = 0x00,
	STATUS_TYPE_THREESTATUS = 0x01,
}STATUS_TYPE;

// ����˵��Ʊ��/Ӧ��/��Ʒ�ʻ���״̬��
typedef enum _accountState_t{
	ACCOUNT_STATE_UNINITIALISED         = 1,     // δ��ʼ����
	ACCOUNT_STATE_INITIALISED           = 2,     // �ʻ�����ʼ����
	ACCOUNT_STATE_ISSUED                = 3,     // �ʻ��ѱ����С�
	ACCOUNT_STATE_REPLACED              = 4,     // �ѷ����滻��
	ACCOUNT_STATE_REFUND_IN_PROGRESS    = 5,     // ���ڶ��ʻ������˿
	ACCOUNT_STATE_REFUNDED              = 6,     // �ʻ��ѱ��˿
	ACCOUNT_STATE_CLOSED_OFF            = 7,     // �ʻ����رա�
	ACCOUNT_STATE_REVERSED              = 8,     // �ʻ����ع���
	ACCOUNT_STATE_UNSPECIFIED           = 255    // δ����ֵ��
} AccountState_t;

//����M1Ʊ����״̬
typedef enum _cardStatusCode_t_M1{    
	CARD_STATUS_CODE_NOT_BLOCKED                    = 1,    // �˿�δ��������
	CARD_STATUS_CODE_BLOCKED_OTHER                  = 2,    // �˿��ѱ���������������δ������ԭ��
	CARD_STATUS_CODE_BLOCKED_SURRENDERED_CARD       = 3,    // �˿�����������Ϊ�ֿ����˿���բ������Ʊ����
	//PRODUCT_STATUS_CODE_BLOCKED_HOTLISTED_SAM       = 4,    // ��Ʒ��Ϊ�ں�����SAM ��ʹ�ö�������
	CARD_STATUS_CODE_BLOCKED_FAULTY_CARD            = 5,    // ����������
	CARD_STATUS_CODE_BLOCKED_NOT_ISSUED             = 6,    // �˿��ѱ���������Ϊ����δ�����С�
	//PRODUCT_STATUS_CODE_BLOCKED_BAD_AUTOLOAD_DEBT   = 8,    // ��Ʒ����������Ϊ���л��ʡ�
	//PRODUCT_STATUS_CODE_BLOCKED_BANK_ACCOUNT_CLOSED = 9,    // ��Ʒ����������Ϊ�����ʻ��ѹرա�
	//PRODUCT_STATUS_CODE_BLOCKED_PRODUCT_OVER_LIMIT  = 10,   // ��Ʒ����������Ϊ���ڡ�
	CARD_STATUS_CODE_BLOCKED_LOST_STOLEN            = 11,   // Ʊ������ʧ����������������
	CARD_STATUS_CODE_BLOCKED_CARD_BATCH             = 12,   // ����������
	CARD_STATUS_CODE_BLOCKED_OVER_TIME              = 13,   // ��Ϊ�������ڼ����������������ֵ����������
	CARD_STATUS_CODE_BLOCKED_INACTIVE               = 14,   // ��Ϊ12 ����δ�ã��ÿ���������
	CARD_STATUS_CODE_UNSPECIFIED                    = 15,    // ���趨ֵ��
	CARD_STATUS_CODE_INITIALISED_NOT_YET_ISSUED     = 110    // �˿�û�г�ʼ��
}CardStatusCode_t_M1;

typedef enum _productStatus_t_M1
{
	PRODUCT_STATUS_CODE_NOT_BLOCKED                 = 1,    //��Ʒδ������
	PRODUCT_STATUS_CODE_BLOCKED_OTHER               = 2,    //��Ʒ��δ����������ԭ������
	PRODUCT_STATUS_CODE_BLOCKED_HOTLISTED_SAM       = 4,    //��Ʒ�����������������SAM���豸��ʹ�ö�����
	PRODUCT_STATUS_CODE_BLOCKED_BAD_AUTOLOAD_DEBT   = 8,    //��Ʒ���ʱ�����
	PRODUCT_STATUS_CODE_BLOCKED_BANK_ACCOUNT_CLOSED = 9,    //��Ʒ����������Ϊ�����ʻ��ѹر�
	PRODUCT_STATUS_CODE_BLOCKED_PRODUCT_OVER_LIMIT  = 10,   //��Ʒ����������Ϊ����
	PRODUCT_STATUS_CODE_UNSPECIFIED                 = 15    //���趨ֵ

}ProductStatus_t_M1;
//����ULƱ����״̬
typedef enum _cardStatusCode_t_UL{
	CARD_STATUS_CODE_UNSEND					    	= 0,    // δ��
	CARD_STATUS_CODE_SEND					        = 3,    // ����
	CARD_STATUS_RECYCLE		                        = 7,    // ����	
	CARD_STATUS_BLACKLIST_BLOCK                      = 9,    // ����������
}CardStatusCode_t_UL;

// ��Ƭ���� 0x01��һ��ͨCPU��Ƭ��0x02��һ��ͨM1��Ƭ��
//			0x11��һƱͨCPU��Ƭ��0x12��һƱͨUL���� 0x13��һƱͨM1��Ƭ
typedef enum _cardType_t{
	CARD_TYPE_ACC_UNDEFINE       = 0xFF,
	CARD_TYPE_ACC_UL             = 0x00,
	CARD_TYPE_ACC_M1             = 0x01, 
	CARD_TYPE_ACC_M4			 = 0x02,
	CARD_TYPE_ACC_CPU            = 0x04,
	CARD_TYPE_ECT_M1,       
	CARD_TYPE_ECT_CPU,     
	//������Ƭ���ඨ��
	CARD_TYPE_SY_ECT			= 0x01,//һ��ͨ
	CARD_TYPE_SY_ECT_CPU		= 0x02,//һ��ͨCPU��
	CARD_TYPE_SY_EPT			= 0x11,//һƱͨ
	CARD_TYPE_SY_EPT_UL			= 0x12,//һƱͨUL��
	CARD_TYPE_SY_EPT_CPU		= 0x13,//һƱͨCPU��
}CardType_t;

// ������
typedef enum _cardholderFeeType_t{
	CARDHOLDER_FEE_TYPE_REPLACEMENT      = 2,     // Fee TCHARged for replacement.
	CARDHOLDER_FEE_TYPE_REFUND           = 5,     // Fee TCHARged for a refund.
	CARDHOLDER_FEE_TYPE_PERSONALISATION  = 13,    // Fee TCHARged for personalisation.
	CARDHOLDER_FEE_TYPE_UNSPECIFIED      = 255    // No value set.���趨ֵ
}CardholderFeeType_t;

typedef enum _cdPayloadId_t{     
	CD_PAYLOAD_ID_SYSTEM_PARAMS                      = 1,       // �������з����ṩ�̶��ԣ���������ϵͳ��Χ�ڲ���ֵ�����������豸���͵Ĳ�����CD ʵ�塣
	CD_PAYLOAD_ID_SERVICE_PROVIDER_PARAMS            = 2,       // �����������ֵ������ض������ṩ�̽������Զ���Ĳ�����CD ʵ�壬����ֵ���������ڴ˷����ṩ�̵������豸���͡�
	CD_PAYLOAD_ID_CARD_ACTIONLIST                    = 6,       // �����Ը���Ʊ��������ȡ�ж��������������������CD ʵ�壬�����ڽ�����ֹ����ʹ�õ�Ʊ����Χ��
	CD_PAYLOAD_ID_PRODUCT_ACTIONLIST                 = 8,       // �����ڰ���Ʊ����Ʒϸ�ڵĲ�Ʒ��������CD ʵ�壬�Ҵ�Ʊ����ƷҪ��Ʊ���ӿ��豸��ȡ�ر��ж���
	CD_PAYLOAD_ID_SAM_HOTLIST                        = 9,       // CD ʵ�����Ҫ��Ʊ���ӿ��豸��ȡ�ر��ж��ļ���ģ���ʶ���嵥���ض�����£������г���ʧ���ȡ�ļ���ģ�飬�˼���ģ���п�������ƭ�ֶζԳ˿�Ʊ����Ӽ�ֵ��
	CD_PAYLOAD_ID_CALENDARS                          = 17,      // ͨ��ָ���ο����ڣ�����ȷ���ض���������������������CD ʵ�塣
	CD_PAYLOAD_ID_FARE_TABLES                        = 18,      // ���ñ��ǻ��ҷ��ü�ֵ����Ҫ���ַ������ǽ���������ڷ��ò������б���ȡ�Ľ��Ļ��Ҽ�ֵ��
	CD_PAYLOAD_ID_FARE_PATTERNS                      = 19,      // ����ģʽ�ǻ��ҷ��ü�ֵ������û��Ʒ�۸�����г˿����������Ե���Ҫ��ʽ��
	CD_PAYLOAD_ID_FARE_CODES                         = 20,      // ���ô����������׼�������ļ���ת����������ȷ����ط��ñ�ķ��ü�ֵ�ķ��ô��롣
	CD_PAYLOAD_ID_DAY_TYPES                          = 47,      // ʱ���CD Ŀ���ṩʶ��һ���о�����ҵ���������ʱ��μ��ϵĻ��ơ�
	CD_PAYLOAD_ID_LOCATION_NAMES                     = 63,      // ����Ч���ذ���һ��λ��������ֱ��
	CD_PAYLOAD_ID_PRODUCT_TYPE_PARAMS                = 64 ,     // ͨ������������ڱ�Ʊ���ϵĶ������ԣ���CD��Ч������չ��Ʊ��ģ��
	CD_PAYLOAD_ID_PARTICIPANT_PARAMS                 = 73,      // ��CD Ŀ��������뷽���еĲ�����
	CD_PAYLOAD_ID_NAMES                              = 75,      // ��CD ʵ�����������ʾĿ�ĵĸ����о����͵����ơ�
	CD_PAYLOAD_ID_MIFARE_PARAMS                      = 83,      // ��Ʊ��ģ��ʹ�ã����ڰ�������Ʊ���Ĵ���
	CD_PAYLOAD_ID_ATTRIBUTE_DEFINITIONS              = 84,      // ����CdAttributeId_t�Ķ��塣
	CD_PAYLOAD_ID_HIGH_SECURITY_BLACKLIST            = 85,      // ����֪ͨ��վ������Ա�ڱ�����·��ϵͳ�д������⿨��
	CD_PAYLOAD_ID_LOCATIONS                          = 86,      // ��ʶλ�úͿ�����֮�йص����ԡ�
	CD_PAYLOAD_ID_DEVICE_PARAMS                      = 87,      // ����������LCS �豸�Ĳ���
	CD_PAYLOAD_ID_MANIFEST                           = 88,      // ��LCS CD XML �ļ��У�������������������Ч���ݵı�Ҫ��Ϣ��
	CD_PAYLOAD_ID_BUSINESS_RULES                     = 89,      // ����Ч�������ڰ����������AFCϵͳҪ��
	CD_PAYLOAD_ID_TIME_CODES                         = 90,      // �����ѱ�ʶ��һ����ض�ʱ�Σ�����߷��ڣ��������ʱ����롣
	CD_PAYLOAD_ID_PASSENGER_TYPE_PARAMS              = 91,      // ����ÿ���������ĸ�����Ϣ��
	CD_PAYLOAD_ID_CARD_BATCH_WITHDRAW_LIST           = 92       // ����CardBatchNumber_t��Ʊ���Ļ������ڴ���ͨ��ɾ��Ʊ����
}CdPayloadId_t;

//ȷ���豸״̬��
typedef enum _deviceStatus_t{  
	DEVICE_STATUS_IN_SERVICE            = 1,     // �豸ͨ��SAM ��װ���ԣ�׼��������״̬��
	DEVICE_STATUS_OUT_OF_SERVICE        = 2,     // ��״̬��ʾ�豸������������������ⷢ�����°���豸�ر�ʱ��
	DEVICE_STATUS_UNSPECIFIED           = 255    // δ�趨ֵ��
}DeviceStatus_t;

//�������ָ��ѷ�����
typedef enum _paymentMethod_t{  
	PAYMENT_METHOD_CASH							= 1,		// �ֽ�
	PAYMENT_METHOD_ACC_CARD						= 2,		// ACC��ֵ��
	PAYMENT_METHOD_YKT_CARD						= 3,		// YKT��ֵ��
	PAYMENT_METHOD_CRIDIT_CARD					= 4,		// ���ÿ�֧��
	PAYMENT_METHOD_WITH_NET						= 5,		// ����֧��
	PAYMENT_METHOD_WITH_ZHIFUBAO				= 6,		// ɨ��֧����20171208,֧������΢�ź�һ��
	PAYMENT_METHOD_WITH_WEIXIN					= 7,		// ΢��֧��(����Ч)
	PAYMENT_METHOD_BY_CARD						= 252,		// ��ֵ������Ч��
	PAYMENT_METHOD_BANK_CARD					= 253,		// ���п�����Ч��
	PAYMENT_METHOD_NONE                         = 254,      // ���趨ֵ
	PAYMENT_METHOD_UNSPECIFIED					= 255		// ���趨ֵ
}PaymentMethod_t;

//Ʊ����������ԭ��
typedef enum _replacementReason_t{  
	REPLACEMENT_REASON_CARD_FAULTY                          = 1,    // ����Ʊ��
	REPLACEMENT_REASON_NEW_PERSONALISED_CARD_AVAILABLE      = 3,    // ���µġ����˻���Ʊ����
	REPLACEMENT_REASON_CARD_EXPIRED                         = 4,    // Ʊ���Ѿ����ڡ�
	REPLACEMENT_REASON_CARD_STOLEN                          = 5,    // ���ڿ������ԣ�����
	REPLACEMENT_REASON_UNSPECIFIED                          = 255   // ���趨ֵ��
}ReplacementReason_t;

//���ױ�������ԭ��
typedef enum _reversalReasonCode_t{  
	REVERSAL_REASON_CODE_INVALID             = 1,       // ��ȡ����Ϊ��Ч��
	REVERSAL_REASON_CODE_UNSPECIFIED         = 255      // ���趨ֵ��
}ReversalReasonCode_t;



//ָ����ϸ�������������λͼ��־��
typedef enum _sortingFlags_t{  
	SORTING_FLAGS_UNSORTED_EXACT_MATCH           = 0x00,     // ��ʾ������Ҫ����������������û�в����������
	SORTING_FLAGS_ASCENDING_EXACT_MATCH          = 0x01,     // ��ʾ�����ܽ��ж�����������
	SORTING_FLAGS_ASCENDING_BEST_FIT_HIGH        = 0x02,     // �������ʺϵ�ֵ�����ṩ��������ֵ.
	SORTING_FLAGS_ASCENDING_BEST_FIT_LOW         = 0x04      // ��������ʵ�ֵС���ṩ��������ֵ
}SortingFlags_t;

//ȷ���˻ؿ��Ŀ���ԭ��
typedef enum _surrenderReason_t{  
	SURRENDER_REASON_CARDHOLDER_REQUEST                 = 1,    // ���Գֿ��˵�����
	SURRENDER_REASON_FAULTY_CARD                        = 2,    // Ʊ���й��ϡ�
	SURRENDER_REASON_EXPIRED_CARD                       = 3,    // Ʊ���ѹ��ڡ�
	SURRENDER_REASON_PREVIOUSLY_BLOCKED                 = 4,    // Ʊ����������
	SURRENDER_REASON_CARD_PRINTING_PROBLEM              = 5,    // Ʊ������ӡˢ��
	SURRENDER_REASON_AUTOMATICALLY_EXPIRED_CARD         = 6,    // Ʊ���Զ����ڡ�
	SURRENDER_REASON_AUTOMATICALLY_EXPIRED_PRODUCT      = 7,    // Ʊ���ϵ�ĳ����Ʒ�Զ����ڡ�
	SURRENDER_REASON_TICKET_EXCHANGE                    = 8,    // �����˳�Ʊ������
	SURRENDER_REASON_UNSPECIFIED                        = 255   // ���趨ֵ��
}SurrenderReason_t;

//�о�ȷ�ϵ�ǰƱ���״̬��ͨ�����ڶ�һ����Ʊ�ı��档
typedef enum _ticketStockStatus_t{  
	TICKET_STOCK_STATUS_OK                      = 1,    // ����״̬��
	TICKET_STOCK_STATUS_CARD_BATCH_WITHDRAWAL   = 2,    // ��ʾ�������ա�
	TICKET_STOCK_STATUS_DAMAGED                 = 3,    // ��ʾ��Ʊ��
	TICKET_STOCK_STATUS_MAX_LIFECYCLE_REACHED   = 4,    // ��ʾ�ó�Ʊ�ﵽ�����������������
	TICKET_STOCK_STATUS_FAULTY                  = 5,    // ��ʾ�ó�Ʊ�й��ϡ�
	TICKET_STOCK_STATUS_OTHER                   = 254,  // ����״̬ԭ�򡣡�
	TICKET_STOCK_STATUS_UNSPECIFIED             = 255   // ˵��δ�ṩ��Ʊ״̬��ԭ��
} TicketStockStatus_t;

//��ʶ���׵�״̬��
typedef enum _transactionStat_t{  
	TRANSACTION_STAT_CARD_UPDATE_NOT_CONFIRMED          = 0x01 ,    // ������δ��ȷ��ʱ�����λ����Ϊ��1�����������ѱ�ȷ��ʱ�����λ����Ϊ��0����
	// �������з�Ʊ�����ף�����Ʊ���޹صĽ��ף������λ����Ϊ��0��
	// δȷ�ϵĽ�����ָ���豸��ָ��Ʊ��һ����뽻�׵��豸���޷�ȷ���Ƿ����н������ݶ���д��Ʊ���С�
	// �����Ʊ����ɽ�������д��󣬵��������豸����֮ǰ��Ʊ���ͱ���RF ��������,��������ͷ����ˡ�
	TRANSACTION_STAT_TEST_CARD                          = 0x02,     // ��ʹ�ò��Կ�ʱ����Ϊ��1������δʹ�ò��Կ�ʱ����Ϊ��0�������û��ʹ��Ʊ����Ҳ����Ϊ��0����
}TransactionStat_t;


// ��ˮ������ԭ��
typedef enum _udsn_reset_reason
{
	UNDIFINED     = 0x00, // δ����
	MAXVALUERESET = 0x01, // �������ֵ���Զ����ã� 
	MAINTANRESET  = 0x02  // ά������
}UDSN_RESET_REASON;

// ��ˮ������
typedef enum _udsn_type{
	ACC_UDSN = 0x01, // ACC������ˮ�ţ�
	ECT_UDSN = 0x02, // һ��ͨ������ˮ�ţ�
	AFC_UDSN = 0x03, // AFC������ˮ�ţ�
	UNDIFINE = 0x00  // δ����
}UDSN_TYPE;

// ��ˮ������
typedef struct _udsn_reset{
	UDSN_TYPE udsn_type;  // 1 BIN �豸��ˮ������
	DWORD beforeResetNo;  // 4 BIN ����ǰ�豸��ˮ��
	DWORD afterResetNo;   // 4 BIN ���ú��豸��ˮ��
}UDSN_RESET;



//�������������
typedef enum _counter_type{
	COUNTER_TYPE_EVENT = 0x00,
	COUNTER_TYPE_TRANS_ACC = 0x01,
	COUNTER_TYPE_TRANS_ECT = 0x02,
	COUNTER_TYPE_TRANS_AFC = 0x03
} COUNTER_TYPE;

//������Ʊ��ʽ������TPU�ӿ��ĵ���
typedef enum _issue_sale_mode
{
	BY_UNDEFINE = 0x00,				// δ����
	BY_PRICE = 0x01,				// ��ʾ����Ʊ������Ʊ
	BY_BEGIN_END_STATION = 0x02,	// ��ʾ������ֹվ����Ʊ
} ISSUE_SALE_MODE;

typedef enum _price_mode
{
	BY_CONSUME_PRICE = 0x01,		// ����Ʊ��
	BY_SALE_PRICE = 0x02,			// ����Ʊ��
} PRICE_MODE;

// ҵ��״̬
//typedef enum _service_state {
//	INIT        = 0x00,         // ��ʼ��
//	OUT_OF      = 0x01,         // ��ֹ��
//	READY       = 0x02,         // �ȴ���
//	RECEPT      = 0x03,         // ������
//	ISSUEING    = 0x05,         // �豸������
//	MAINTENANCE = 0x07,         // ά����
//	ABNORMAL    = 0x08,         // �쳣��
//	REST        = 0x09,         // ��Ϣ��
//	FINISH      = 0x0A,         // ����
//} SERVICE_STATE;

// ��·���룬���õ��ֽڱ��롣
//typedef U8_t  LineID_t;
// ��վ���룬�������ֽڱ��룬���ֽ�Ϊ��·���룬���ֽ�Ϊ��·�ڳ�վ�������롣
//typedef U16_t StationID_t;

////Ʊ�ֻ������ͣ�������ࣩ
//enum TicketFamily_t{
//	TicketFamily_SJT      = 1,/*����Ʊ  */
//	TicketFamily_SV       = 2,/*�������мƴ�Ʊ  */
//	TicketFamily_YktCsc   = 3 /*һ��ͨ��ֵƱ */
//};

typedef struct _NW_TRANS_ID_GRP
{
	BYTE biGrpNum;        // ���
	BYTE biEquipNum;      // ���ڱ��

	_NW_TRANS_ID_GRP() {
		biGrpNum   = 0;
		biEquipNum = 0;
	}
	void Serialize(LPBYTE lpByte)
	{
		*lpByte=biGrpNum;
		lpByte++;
		*lpByte = biEquipNum;
		lpByte++;
	}
	void Deserialize(LPBYTE lpByte)
	{
		biGrpNum = *lpByte;
		lpByte++;
		biEquipNum = *lpByte;
		lpByte++;
	}
}NW_TRANS_ID_GRP;

// �豸ID
typedef struct _device_id{
	BYTE bAfcDeviceType;           // 1 BCD �豸����
	BYTE bLineId;        // 1 BCD ��·ID
	BYTE bStationId;            // 1 BCD ��վID
	BYTE bDeviceSeqNoInStation; // 1 BIN ��վ���豸���
	_device_id(){
		bLineId = 0;
		bStationId = 0;
		bAfcDeviceType = 0;
		bDeviceSeqNoInStation = 0;
	}
	void Serialize(LPBYTE lpByte)
	{
		*lpByte = bAfcDeviceType;
		lpByte++;
		*lpByte = bLineId;
		lpByte++;
		*lpByte = bStationId;
		lpByte++;
		int2BCD(bDeviceSeqNoInStation, (char*)lpByte, 1);
		lpByte++;
	}
	void Deserialize(LPBYTE lpByte)
	{
		bAfcDeviceType = *lpByte;
		lpByte++;

		bLineId = *lpByte;
		lpByte++;
		bStationId = *lpByte;
		lpByte++;
		bDeviceSeqNoInStation = BCD2int((char*)lpByte, 1);
		lpByte++;
	}
	CString ToString()
	{
		CString s;
		s.Format(_T("%.2X%.2X%.2X%.2d"),bAfcDeviceType,bLineId,bStationId,bDeviceSeqNoInStation);
		return s;
	}
}DEVICE_ID;

typedef struct _accdeviceid_t
{
	BYTE lineCode;				//BCD��·����
	WORD stationCode;			//2BCDվ�����
	BYTE deviceType;			//1BCD ACC�豸����
	WORD wSerialNo;					//2BCD�豸���
	CString ToString()
	{
		CString s;
		s.Format(_T("%.2X%.2X%.2X%.2X%.4d"),lineCode,HIBYTE(stationCode),LOBYTE(stationCode),deviceType,wSerialNo);
		return s;
	}
	void Serialize(LPBYTE lpMsg)
	{
		*lpMsg = lineCode;
		lpMsg+=1;
		*lpMsg = HIBYTE(stationCode);
		lpMsg++;
		*lpMsg = LOBYTE(stationCode);
		lpMsg++;
		*lpMsg = deviceType;
		lpMsg+=1;
		int2BCD(wSerialNo,(char*)lpMsg,2);
		lpMsg+=2;
	}
}AccDeviceID_t;
//Ʊ����ϸ����,����Ʊ���� 
typedef U8_t  TicketType_t;

//Ʊ����ϸ����,����Ʊ���� 
typedef enum _ProductType
{
	SJT				= 0x62,		// ����Ʊ��0100-����Ʊ&Ԥ��ֵ����Ʊ&��վƱ��0103-�����Ʊ��0104-���ѳ�վƱ��	
	SVT				= 0x02,		// ��ֵƱ��0200-��ͨ��ֵƱ��0202-���˴�ֵƱ��0203-���ֵƱ��0204-������ֵƱ��
	MST				= 0x03,		// �ƴ�Ʊ��0300-��ͨ�˴�Ʊ��0305-�޴γ˴�Ʊ��
	PST				= 0x04,		// ����Ʊ��0400-һ��Ʊ��0401-����Ʊ��0402-����Ʊ��
	NMT				= 0x05,		// ���Ʊ��0500-���Ʊ��0501-�������Ʊ��
	EPT				= 0x07,		// Ա��Ʊ��0700-EP1��0701-EP2��0703-EP34��
	TET				= 0x0C,		// ����Ʊ��0C00-����Ʊ��
	UNKNOWPRODUCT	= 0xFF,	// δ����
}ProductType;
// ��Ʊ������루BOM��
typedef	enum _ticket_kind {
	TICKET_SINGLE_RIDE = 98,				//����Ʊ
	TICKET_PRE_MAKE = 97,					//Ԥ��Ʊ
	TICKET_ROUND = 96,							//����Ʊ
	TICKET_WELFARE = 95,							//����Ʊ
	TICKET_BAGGAGE = 94,							//����Ʊ
	TICKET_EXIT_ONLY = 93,					//��վƱ

	TICKET_ONE_DAY = 5,						//һ��Ʊ

	TICKET_STORE_VALUE = 51,			//��ͨ��ֵƱ
	TICKET_STORE_VALUE_NAMED = 52,//������ͨ��ֵƱ
	TICKET_STUDENT = 53,						//ѧ��Ʊ
	TICKET_OLD_STORE_VALUE = 4,	//���괢ֵƱ
	TICKET_YKT_STORE_VALUE = 1,		// һ��ͨ��ֵƱ:
	TICKET_OLD_FREE =7,							//�������Ʊ

	TICKET_COMMEMORATIC_STORE_VALUE = 37,	//��ֵ����Ʊ

	TICKET_STORE_MULTIRIDE = 31,			//��ͨ�ƴ�Ʊ��CPU��
	TICKET_COMMEMORATIC_MULTIRIDE = 40,//����ƴ�Ʊ

	TICKET_FIX_PERIOD = 34,				//��ͨ����Ʊ
	TICKET_COMMEMORATIC_PERIOD = 15,	//���ڼ���Ʊ��UL��


	TICKET_EMPLOYEE_3 = 8,				//Ա��ƱEP3
	TICKET_EMPLOYEE_2 = 9,				//Ա��ƱEP2
	TICKET_EMPLOYEE_1 = 10,				//Ա��ƱEP1
	TICKET_COMMEMORATIC_PURSE = 65,				//���ֵ��
	TICKET_STATION_WORK = 89,		//��վ����Ʊ

	TICKET_FOREIGN_PERSON_1 = 81,			//���ƱFP1
	TICKET_FOREIGN_PERSON_2 = 82,			//���ƱFP2
	TICKET_FOREIGN_PERSON_3 = 83,			//���ƱFP3

	TICKET_PAPER			= 254,			//����ֽƱ

} TICKET_KIND;
//Ʊ���߼���
typedef U64_t TicketLogicalID_t;
// ��·���룬���õ��ֽڱ��롣
typedef U8_t  LineID_t;
// ��վ���룬�������ֽڱ��룬���ֽ�Ϊ��·���룬���ֽ�Ϊ��·�ڳ�վ�������롣
typedef U16_t StationID_t;

// ����������
typedef	enum _card_medium_type
{
	RESERVE		= 0xFF,									// Ԥ��
	ULTRALIGHT	= 0x00,									// Ultralight
	MIFARE1K	= 0x01,									// Mifare1K
	MIFARE4K    = 0x02,									// Mifare 4K
	CPU_TICKET	= 0x04,									// CPUƱ��

	CARD_MEDIUM_TYPE_ULTRALIGHT		= CARD_TYPE_SY_EPT_UL/*ULTRALIGHT*/, // Mifare Ultralight
	CARD_MEDIUM_TYPE_MIFARE1K		= MIFARE1K, // Mifare One 1K��
	CARD_MEDIUM_TYPE_MIFARE4K		= MIFARE4K, // Mifare One 4K��
	CARD_MEDIUM_TYPE_CPU			= CARD_TYPE_SY_EPT_CPU/*CPU_TICKET*/ // CPU��
} CARD_MEDIUM_TYPE;
//�ÿͷ��༶��  
typedef enum _classificationLevel_t{  
	CLASSIFICATION_LEVEL_NORMOL                     = 0,        // ��ͨ
	CLASSIFICATION_LEVEL_VIP                        = 1,        // VIP
	CLASSIFICATION_LEVEL_UNSPECIFIED                = 2         // δ�趨ֵ��
}ClassificationLevel_t;

//ϵͳ��������ز�Ʒ������
typedef enum _productCategory_t{
	//UNDEFINE = 0x00,		    //δ����
	//PURSE = 0x01,				//Ǯ��
	//PERIOD = 0x02,				//����
	//MULTIRIDE = 0x03,			//�ƴ�	

	UNDEFINE = 0x00,		    //δ����
	PURSE = 0x01,				//��ֵ��Ǯ��
	MULTIRIDE = 0x02,			//�˴�Ǯ��
	PERIOD = 0x03,				//����Ǯ��
	PURSE_FIRST = 0x04,			//��ֵǮ�����ȣ���ֵǮ���ͳ˴�Ǯ��˫��Ʒ��
	MULTIRIDE_FIRST = 0x05,		//�˴�Ǯ�����ȣ���ֵǮ���ͳ˴�Ǯ��˫��Ʒ��	
}ProductCategory_t;

// ACCƱ������ģʽ 
typedef enum _accCard_test_status
{
	FORMAL_CARD = 0x00,					//0:����ʹ��
	TEST_CARD = 0x01,				    //1:���Ի���ѵ
} ACC_CARD_TEST_STATUS;

typedef enum _acc_Card_active_status
{
	UNACTIVED = 0x00,					//0:�״γ���֧��ʱ���� 
	ACTIVED   = 0x01,					//1:�Ѽ���
}ACC_CARD_ACTIVE_STATUS;

//�ó�״̬
typedef	enum _acc_card_journey_status
{
	//TICKET_UNUSED				    = 0x00,				   //  ��Ʒδʹ��	
	//UNUSED=TICKET_UNUSED				    ,
	//INSTATION						= 0x01,				   //  ������վ																 
	//OUTSTATION						= 0x02,				   //  ������վ	
	//BEFOREOUT_IN					= 0x03,				   //  �ϴ��г�����ģʽ��վ
	//STATUS_ADJUST_TIMEOUT           = 0x04,                //  �ϴβ�Ʊ��־

	TICKET_INIT						= 2,				   //  ��ʼ��	
	TICKET_SJT_ISSUE				= 3,				   //  ����Ʊ����
	TICKET_WRITE_OFF				= 4,				   //  ע��																 
	TICKET_REENCODE					= 5,				   //  �ر���
	YKT_DISABLED_UPDATE_FOR_IN		= 10,				   //  һ��ͨ��ä����վ����				
	YKT_DISABLED_UPDATE_FOR_OUT		= 11,				   //  һ��ͨ��ä����վ����
	TICKET_UPDATE_FOR_IN			= 12,				   //  ���£�Ϊ�˽�բ��
	TICKET_UPDATE_FOR_OUT           = 13,				   //  ���£�Ϊ�˳�բ��
	TICKET_UPDATE_FOR_OTHER			= 14,				   //  ��������
	TICKET_IN						= 15,				   //  ��բ				
	TICKET_OUT						= 16,				   //  ��բ
	YKT_DISABLED_IN					= 17,				   //  һ��ͨ��ä����բ				
	YKT_DISABLED_OUT				= 18,				   //  һ��ͨ��ä����բ
	REFOUND_TIKCET		            = 21,                  //  �˿�
	TICKET_ES_ASSIGNMENT            = 31,                  //  ��E/SԤ��ֵ
	TICKET_ASSIGNMENT_ZERO			= 32,                  //  Ԥ��ֵ����
	TICKET_ET_FOR_EXIST				= 33,                  //  ��վƱ��Ϊ�˳�բ��
	TICKET_BEFOREOUT_IN				= 34,	               //  �г�����ʱ��բ

} ACC_CARD_JOURNEY_STATUS;

// ���Ի���λ��־
typedef enum _personal_area_flag{
	PERSONAL_AREA_UNKNOWN			= 0,	// δ֪����
	PERSONAL_AREA_PASSAGER_TYPE		= 1,	// �˿�����
	PERSONAL_AREA_PASSAGER_NAME		= 2,	// ����
	PERSONAL_AREA_PASSAGER_GENDER	= 3,	// �Ա�
	PERSONAL_AREA_PASSAGER_BIRTHDAY	= 4,	// ����
	PERSONAL_AREA_CERTIFICATE_TYPE	= 5,	// ֤������
	PERSONAL_AREA_CERTIFICATE_ID	= 6,	// ֤�����
	PERSONAL_AREA_PASSAGER_PHOTO	= 7,	// Ա����Ƭ
	PERSONAL_AREA_FOREGOUND			= 8,	// ���汳��
	PERSONAL_AREA_BACKGOUND			= 9,	// ���汳��
} PERSONAL_AREA_FLAG;

// ���Ի���λ��Ϣ
typedef struct _personal_area{
	PERSONAL_AREA_FLAG flag;		// ��λ��־
	CString txtAreaContent;			// ��λ����
	CString txtFont;				// �ı�����
	int nSize;						// �ı���С
	COLORREF color;					// �ı���ɫ
	int x;							// ������
	int y;							// ������

	_personal_area()
	{
		flag = PERSONAL_AREA_UNKNOWN;
		txtAreaContent = _T("");
		txtFont = _T("");
		nSize = 0;
		color = 0;
		x = 0;
		y = 0;
	}
} PERSONAL_AREA;

// ���Ի�����
typedef enum _personalize_type{
	TYPE_PERSONAL = 0,			// ���Ի�����
	TYPE_PERSONAL_UPDATE = 1,	// ���Ի���������
} PERSONALIZE_TYPE;

// �Ա��ʶ
typedef enum _sex_type{  
	SEX_MALE  = 1,			// ��
	SEX_FEMALE = 2,			// Ů
	SEX_UNSPECIFIED = 255	// ���趨ֵ��
} Sex_t;

// ֤������
typedef	enum _certificate_type{
	CERTIFICATE_ID_CARD_MAINLAND = 1,	// ���֤����½��
	CERTIFICATE_ID_CARD_MACAO = 2,		// ���֤�����ţ�
	CERTIFICATE_ID_CARD_HONGKONG = 3,	// ���֤����ۣ�
	CERTIFICATE_ID_CARD = 0,           // 0�����֤ 
	CERTIFICATE_MILITARY_OFFICER = 1,   // 1������֤ 
	CERTIFICATE_POST_CARD = 2,          // 2������ 
	CERTIFICATE_STUDENT_CARD = 3,       // 3��ѧ��֤
	CERTIFICATE_UNSPECIFIED = 255,      // δ����
} CERTIFICATE_TYPE;

typedef enum _payed_method{
	BY_CASH  = 0x01,                   // �ֽ�֧����Ʊ���
	BY_CARD  = 0x03                    // ���ڿ۳���Ʊ���             
}_PAYED_METHOD;

//�˿����ɣ�ͬʱ�����ڱ��˻�黹��Ʊ����
typedef enum _refundReason_t{  
	REFUND_REASON_TICKET_UNUSED_FULL_REFUND_LESS_HANDLING_CHARGE                =1,     // ��δʹ�õĳ�Ʊ��ȥ�����Ѻ�ȫ���˿�
	REFUND_REASON_TICKET_USED_COMPLEMENTARY_FULL_REFUND                         =2,     // ��ʹ�ù��ĳ�Ʊ����ȡ�κη���ȫ���˿�
	REFUND_REASON_TICKET_PARTLY_USED_PROPORTIONAL_REFUND_LESS_HANDLING_CHARGE   =3,     // ��ʹ�ù��ĳ�Ʊ��ȥ�����Ѻ�ȫ���˿�
	REFUND_REASON_UNSPECIFIED                                                   =255    // δָ����
}RefundReason_t;
typedef enum _adjust_type
{
	ADJUST_KIND = 0x01,						//��Ʊ
	UPDATE_KIND = 0x02,						//����
}ADJUST_TYPE;


// ��������
typedef enum _transaction_type{
	UNDEFINED           = 0,		// û��
	CARD_KEYS_UPDATE	= 1,		// ����Կ����	
	ISSUE				= 2,		// ����
	DELETE1		        = 3,		// ɾ��
	ADD_VALUE			= 4,		// ��ֵ
	USE					= 5,		// ʹ��
	REFUND				= 6,		// �˿�
	REBATE				= 7,		// �ۿ�
	SUBCHARGE			= 8,		// ����
	BLOCK				= 9,		// ����
	UNBLOCK				= 10,		// ����
	AUTOLOAD_ENABLE		= 11,		// �Զ���ֵ������Ч
	AUTOLOAD_DISABLE	= 12,		// �Զ���ֵ����ʧЧ
	AUTOLOAD_UPDATE		= 13,		// �Զ���ֵ����
	REVERSE_ADD_VALUE	= 14,		// ��ֵ��������
	PERSONALISE			= 15,		// ���˻�
	USE_ON_ENTRY		= 16,		// ��վʹ��
	USE_ON_EXIT			= 17,		// ��վʹ��
	CONSUME				= 31,		// ����
}TRANSACTION_TYPE;
// RW�ص�������
typedef enum RW_CMD_ID{
	RW_COMMAND_ID_READ_CARD = 0x1000,               // ��������
};

// ����ID����ӳ���¼��
typedef struct _language_map{
	WORD languageID;
	CString languageName;
	_language_map()
	{
		languageID = 0;
		languageName = "";
	}
} LANGUAGE_MAP;

// �豸ID
typedef struct _TPU_device_id{
	BYTE bAfcDeviceType;        // 1 BIN AFC�豸����
	WORD wAfcStationID;         // 2 BCD AFC��վID 
	BYTE bDeviceSeqNoInStation; // 1 BIN ��վ���豸���
	_TPU_device_id(){
		bAfcDeviceType = 0;
		wAfcStationID = 0;
		bDeviceSeqNoInStation = 0;
	}
	void Serialize(LPBYTE lpByte)
	{
		
		*lpByte = HIBYTE(wAfcStationID);
		lpByte++;
		*lpByte = LOBYTE(wAfcStationID);
		lpByte++;
		*lpByte=bAfcDeviceType;
		lpByte++;
		*lpByte = bDeviceSeqNoInStation;
		lpByte++;
	}
	void Deserialize(LPBYTE lpByte)
	{
		
		wAfcStationID = MAKEWORD(*(lpByte+1),*lpByte);
		lpByte+=2;
		bAfcDeviceType = *lpByte;
		lpByte++;
		bDeviceSeqNoInStation = *lpByte;
		lpByte++;
	}
	CString ToString()
	{
		CString s;
		s.Format(_T("%.2X%.2X%.2d%.2X"),HIBYTE(wAfcStationID),LOBYTE(wAfcStationID),bAfcDeviceType,bDeviceSeqNoInStation);
		return s;
	}
}TPU_DEVICE_ID;
typedef enum _tagApplyType
{
	APPLY_TYPE_GOOD_CARD = 0,					// �ÿ�����
	APPLY_TYPE_BAD_CARD = 1,					// ��������
	APPLY_TYPE_SUSPEND = 2,						// ��ʧ����
}RefundApplyType;
typedef enum _tagRefundReasonType
{
	REFUND_REASON_PASSAGER = 0,					// �˿�
	REFUND_REASON_SUBWAY = 1,					// ����
}RefundReasonType;

typedef enum _tagRefundApplyReasonType
{
	APPLY_REASON_BAD_CARD = 0,					// Ʊ����
	APPLY_REASON_LARGE_MONEY = 1,				// ��
	APPLY_REASON_OTHER = 2,						// ����
}RefundApplyReasonType;

//////////// ��������
//////////static const U16_t KEY_VERSION_UNSPECIFIED                                      = 0xFFFF;
//////////static const U16_t KEY_NUMBER_UNSPECIFIED                                       = 0xFFFF;
//////////static const U32_t UNSPECIFIED_CARD_PARTICIPANT                                 = 0xFFFFFFFF;
//////////static const U16_t LIFE_CYCLE_COUNTER_UNSPECIFIED                               = 0xFFFF;
//////////static const U32_t CARD_SERIAL_NUMBER_UNSPECIFIED                               = 0xFFFFFFFF;
//////////static const DateC20_t      UNSPECIFIED_DATEC20                                 = 0xFFFF;
//////////static const U32_t          UDSNUM_UNSPECIFIED                                  = 0xFFFFFFFF;
//////////// CD�汾��Ч���ж�ֵ
//////////static const unsigned long UNAVALIBLE_VERSION     = 0xFFFFFFFF;

// �������ԱID�����볤��
static const int STAFFID_LEN   = 6;
static const int STAFFPWD_LEN   = 6;

// վԱȨ��
typedef enum _staff_authority
{
	NONE_AUTHORITY      = 0,            // ��Ȩ��
	COMMON_AUTHORITY    = 1,            // ��ͨҵ��Ȩ��
	MAINTAIN_AUTHORITY  = 2,            // ά��ҵ��Ȩ��
	ALL_AUTHORITY       = 3,            // ȫ��ҵ��Ȩ�ޣ���ͨ����ͨ��
} STAFF_AUTHORITY;

typedef enum _ForceLogoutReason
{
	FORCE_LOGOUT_SERVER_REQUEST = 0x01,	//��λ��ǿ�Ƶǳ�����
	FORCE_LOGOUT_DEVICE_CONTROL = 0x02,//�豸���п���ǿ�Ƶǳ�
	FORCE_LOGOUT_EMERGENCY_MODE = 0x03, //����ģʽǿ�Ƶǳ�
	FORCE_LOGOUT_NEWPARAMETER = 0x04,		//�²�������������
	FORCE_LOGOUT_DEVICE_CONTROL_SHUTDOWN = 0x05,		//�豸���п��ƹػ�����
} FORCELOGOUTREASON;

// Ʊ��״̬
typedef enum _ticket_status{
	TICTKET_STATUS_UNINITIALIZED	= 0x00,	// 0:δ��ʼ������Ϊ0ʱ���Ŷα���Ч��
	TICTKET_STATUS_INITIALIZED		= 0x01,	// 1:�ѳ�ʼ��
}TICTKET_STATUS;

typedef enum _authorization_param{
	taskSetting = 0x3D010101,
	taskManagement = 0x3D010201,
	templateSetting = 0x3D010301,
	templateManagement = 0x3D010401,
	otherService = 0x3D010601,
	managementServive = 0x3D020101,
} AUTHORIZATION_PARAM;

// �豸ģʽ����
typedef enum _device_mode{
	TEST_MODE		= 0x00,		// ����ģʽ
	NORMAL_MODE     = 0x01		// ����ģʽ
}DEVICE_MODE;

// ��ʧ���
typedef enum _suspend_and_resmue_type{
	BUSINESS_SUSPEND_TYPE = 1,					// ��ʧ
	BUSINESS_RESMUE_TYPE = 2,					// ���
	BUSINESS_SUSPEND_RESMUE_UNSPECIFIED = 255,	// δ֪����
} SUSPEND_RESMUE_TYPE;

// �������ͣ�ACC M1K��
typedef enum _acc_m1k_transaction_type{
	ACC_M1K_TRANS_TYPE_ISSUE_TICKET			= 1,  // �ۿ�
	ACC_M1K_TRANS_TYPE_ISSUE_EXIT			= 3,  // ���۳�վƱ
	ACC_M1K_TRANS_TYPE_ISSUE_LOVE			= 4,  // ����Ʊ
	ACC_M1K_TRANS_TYPE_ISSUE_BAGGAGE		= 5,  // ��������Ʊ
	ACC_M1K_TRANS_TYPE_INITIALIZATION		= 8,  // ��ʼ��
	ACC_M1K_TRANS_TYPE_PREEASSIGNMENT		= 9,  // Ԥ��ֵ
	ACC_M1K_TRANS_TYPE_CASH_RECHARGE		= 11, // �ֽ��ֵ
	ACC_M1K_TRANS_TYPE_ADJUST_TICKET		= 15, // ��Ʊ
	ACC_M1K_TRANS_TYPE_SUBWAY_CONSUME_START = 21, // ����������ʼ
	ACC_M1K_TRANS_TYPE_SUBWAY_CONSUME_END	= 22, // �������ѽ���
	ACC_M1K_TRANS_TYPE_SVT_BUY_TICKET		= 41, // ��ֵ����Ʊ
	ACC_M1K_TRANS_TYPE_FOLLOW_ENTER			= 51, // ���ٽ�վ
	ACC_M1K_TRANS_TYPE_FOLLOW_EXIT			= 52, // ���ٳ�վ
	ACC_M1K_TRANS_TYPE_CANCELLATION			= 66, // ע��
	ACC_M1K_TRANS_TYPE_COUNTERACT			= 67, // ����
	ACC_M1K_TRANS_TYPE_INSTANT_REFUND_CARD	= 71, // ��ʱ�˿�
	ACC_M1K_TRANS_TYPE_LOCK					= 77, // ����
	ACC_M1K_TRANS_TYPE_UNLOCK				= 78, // ����
	ACC_M1K_TRANS_TYPE_REVERSE				= 79, // ����
	ACC_M1K_TRANS_TYPE_OVERTIME				= 83, // ������ʱ����
	ACC_M1K_TRANS_TYPE_UNEXIT				= 84, // �޳�վ����
	ACC_M1K_TRANS_TYPE_UNENTER				= 89, // ��������վ����
	ACC_M1K_TRANS_TYPE_EXTENSION_VALIDITY	= 90, // ��Ч�ڸ���
	ACC_M1K_TRANS_TYPE_ADMINISTRATIVE_FINE	= 91, // ��������
	ACC_M1K_TRANS_TYPE_ADMINISTRATIVE_PAY	= 92, // ����֧��
	ACC_M1K_TRANS_TYPE_ACTIVATION			= 96, // ����
	ACC_M1K_TRANS_TYPE_ISSUE_GROUP			= 97, // ������Ʊ
}ACC_M1K_TRANSACTION_TYPE;

// ���Ի�ҵ������
typedef enum _personal_business_type{
	BUSINESS_PERSONAL_TYPE = 0,			// ���Ի�����
	BUSINESS_PERSONAL_UPDATE_TYPE = 1,	// ���Ի���������
	BUSINESS_UNSPECIFIED = 255,			// δ֪����
} PERSONAL_BUSINESS_TYPE;


// RFIDģʽ����
typedef	enum{
	RFID_MODE_READONLY		= 0x00,				// ֻ��ģʽ
	RFID_MODE_WRITABLE	= 0x01				    // ��дģʽ
} RFID_MODE;

// �������ͣ�ACC CPU��
typedef enum _acc_cpu_transaction_type{
	ACC_CPU_TRANS_TYPE_RECHARGE				= 2, // ��ֵ
	ACC_CPU_TRANS_TYPE_CONSUME				= 6, // ����
	ACC_CPU_TRANS_TYPE_COMPLEX_CONSUME		= 9, // ��������
}ACC_CPU_TRANSACTION_TYPE;

// Ʊ��״̬
enum {
	MAGAZINEBOX_COUNT = 12,			// Ʊ��״̬��	           
	MAGASENSOR_COUNT = 12,			// ������״̬��
	MAGAALERM_COUNT = 12,			// ����״̬��
	MAGAVALID_COUNT = 5,			// ��Ч״̬��
};

// �˿�����
typedef enum _passenger_type{
	PASSENGER_UNDEFINED			= 0x00,		// Ԥ��
	PASSENGER_ADULT				= 0x01,		// ����
	PASSENGER_CHILD				= 0x02,		// ��ͯ
	PASSENGER_OLD_PEOPLE		= 0x03,		// ������
	PASSENGER_STUDENT			= 0x04,		// ѧ��
	PASSENGER_SOLDIER			= 0x05,		// ����
	PASSENGER_DISABLED_PEOPLE	= 0x06,		// ������
}PASSENGER_TYPE;

//////////////////////////////////////////////////////////////////////////


////////////////////////��д����ض���////////////////////////////
//////////// SAM�۱��
//////////typedef	enum _slot_id{
//////////	SLOT1 =	0x00,									// һ��ͨPSAM����
//////////	SLOT2 =	0x01,									// һ��ͨISAM����
//////////	SLOT3 =	0x02,									// ����PSAM����
//////////	SLOT4 =	0x03									// ����ISAM����
//////////} SLOT_ID;

//////////// SAM���ඨ��
//////////typedef enum _sam_type {
//////////	RW_ACC_ISAM = 0x01,
//////////	RW_ACC_PSAM = 0x02,
//////////	TH_ACC_ISAM = 0x03,
//////////	TH_ACC_PSAM = 0x04,
//////////	RW_ECT_ISAM = 0x05,
//////////	RW_ECT_PSAM = 0x06,
//////////	TH_ECT_ISAM = 0x07,
//////////	TH_ECT_PSAM = 0x08
//////////}SAM_TYPE;

// ��д������
typedef enum _rw_type{
	BOM_DESKTOP_RW					= 0x01,    // BOM�����д��
	BOM_CARD_RW						= 0x02,    // BOMƱ��ģ���ö�д��
	BOM_TOKEN_RW						= 0x03,    // BOM Tokenģ���ö�д��
	TVM_RECHARGE_RW				= 0x04,    // TVM��ֵ�ö�д��
	TVM_CARD_RW						= 0x05,    // TVMƱ��ģ���ö�д��
	TVM_TOKEN_RW						= 0x06,    // TVM Tokenģ���ö�д��
	AGM_CARD_RECYCLE_RW		= 0x07,    // AGM Ʊ�����ն�д��
	AGM_TOKEN_RECYCLE_RW		= 0x08,    // AGM Token���ն�д��
	AGM_ENTRY_RW						= 0x09,    // AGM��վˢ����д��
	AGM_EXIT_RW							= 0x10,    // AGM��վˢ����д��
	ES_TOKEN_RW1						= 0x11,	// Token ES��д��1
	ES_TOKEN_RW2						= 0x12,	// Token ES��д��2
	ES_TOKEN_RW3						= 0x13,	// Token ES��д��3
	ES_CARD_RW							= 0x14,	// ES Card��д��
	PM_RW									= 0x15,	// PM��д��
	TCM_RW									= 0x16 	// TCM��д��
}RW_TYPE;

typedef enum _rw_update_flag{
	SUCCEEDED    = 0x00, // �ɹ�
	FAILURE      = 0x01  // ʧ��
}RW_UPDATE_FLAG;


// ����������
typedef	enum _es_card_medium_type
{
	TICKET_MEDIUM_TYPE_RESERVE	= 0xFF,									// Ԥ��
	TICKET_MEDIUM_TYPE_CARD		= 0x01,									// Card
	TICKET_MEDIUM_TYPE_TOKEN    = 0x02,									// Token
} TICKET_MEDIUM_TYPE;

typedef enum _sortType_t{
	ID_SORT_TYPE_SATYPE					               = 0 , //��Կ�汾               
	ID_SORT_TYPE_CARD_VER				               = 1 , //�����κ�               
	ID_SORT_TYPE_CARD_NUM				               = 2 , //�߼�����               
	ID_SORT_TYPE_MEDIA_TYPE							   = 3 , //��������               
	ID_SORT_TYPE_PRO_TYPE				               = 4 , //��Ʒ����               
	ID_SORT_TYPE_PRO_CARATARY						   = 5 , //��Ʒ���               
	ID_SORT_TYPE_TEST_MODE 							   = 6 , //����ģʽ               
	ID_SORT_TYPE_INI_MONTH					           = 7 , //��ʼ����               
	//ID_SORT_TYPE_CARD_COMPANY						   = 8 , //��Ƭ��װ��             
	ID_SORT_TYPE_PRE_ISSUE_AMOUNT                      = 8 , //Ԥ�����               
	ID_SORT_TYPE_PRE_ISSUE                             = 9, //Ԥ������               
	ID_SORT_TYPE_CARD_STATUE                           = 10, //��״̬                 
}SortType_t;

typedef	enum _ulCard_trac_type
{
	MONTHPAK_STROE    = 0x01,                                     // "��ƱǮ�� Ȧ��";
	PUBPAK_STROE      = 0x02,                                     // "����Ǯ�� Ȧ��";
	MONTHPAK_CONSUMER = 0x05,                                     // "��ƱǮ�� ����";
	PUBPAK_CONSUMER   = 0x06,                                     // "����Ǯ�� ����";
	COM_CONSUMER      = 0x09                                      // "��������";

} ULCARD_TRAC_TYPE;

typedef	enum _cpuCard_trac_type
{
	LOACLPAK_CONSUMER = 0x01,                                     // "����Ǯ������";
	UPDATE_TRAC       = 0x62,                                     // "�����ͨ���½���";
	REFOUND_TRAC      = 0x8A,                                     // "�˿�";
	LUCK_TRAC         = 0x99,                                     // "��������";
	ENTRE_TRAC        = 0x41,                                     // "�����ͨ��վ";
	OUT_TRAC          = 0x81,                                     // "�����ͨ��վ";
	CHARGE_TRAC       = 0x88,                                     // "��ֵ";
} _CPUCARD_TRAC_TYPE;


// ��Ӫģʽ����
typedef enum _operationalMode_t{
	//NORMAL_SERVICE_MODE				= 0x8300,			// ��������ģʽ
	//EMERGENCY_MODE					= 0x8301,			// ��������ģʽ
	//ENTRY_EXIT_STATION_FREE_MODE	= 0x8302,			// ��վ/��վ�������ģʽ
	//RIDE_TIME_FREE_MODE				= 0x8308,			// �˳�ʱ�����ģʽ
	//TICKET_DATE_FREE_MODE			= 0x8304,			// ��Ʊ�������ģʽ
	//TRAIN_SERVICE_DISRUPTION_MODE	= 0x8310,			// �г�����ģʽ
	//FARE_FREE_MODE					= 0x8320,			// �������ģʽ
	////FREE_ENTRY_STATION_MODE     	= 0x8340,			// ��վ���ģʽ

	// TVM
	NORMAL_SERVICE_MODE				= 0x8300,			// ��������ģʽ
	TRAIN_SERVICE_DISRUPTION_MODE	= 0x8301,			// �г�����ģʽ
	ENTRY_EXIT_STATION_FREE_MODE	= 0x8302,			// ��վ/��վ�������ģʽ��TVM������
	TICKET_DATE_FREE_MODE			= 0x8304,			// ��Ʊ�������ģʽ��TVM������
	FARE_FREE_MODE					= 0x8308,			// �������ģʽ��TVM������
	TIME_FREE_MODE					= 0x8310,			// ʱ�����ģʽ��TVM������
	EMERGENCY_MODE					= 0x8320,			// ��������ģʽ
	//BOM
	DEVICE_MODE_NORMAL = 1,									// Normal����
	DEVICE_MODE_CLOSED = 2,									// Closed�ر�
	DEVICE_MODE_TRAIN_FAULT = 3,							// Train fault�г�����
	DEVICE_MODE_CHECK_FREE_ENTRY = 4,						// Check-free entry��վ���
	DEVICE_MODE_CHECK_FREE_EXIT = 5,						// Check-free exit��վ���
	DEVICE_MODE_EMERGENCY_EXIT = 6,							// Emergency exit����ģʽ
	DEVICE_MODE_NORMAL_CHECK_FREE_TIME = 7,					// Normal + Check-free time����+ʱ�����
	DEVICE_MODE_NORMAL_CHECK_FREE_DATE = 8,					// Normal + Check-free date����+�������
	DEVICE_MODE_NORMAL_CHECK_FREE_FARE = 9,					// Normal + Check-free fare����+�������
	DEVICE_MODE_NORMAL_CHECK_FREE_TIME_DATE = 10,			// Normal + Check-free time + date����+ʱ�����
	DEVICE_MODE_NORMAL_CHECK_FREE_TIME_FARE = 11,			// Normal + Check-free time + fare����+ʱ�����+�������
	DEVICE_MODE_NORMAL_CHECK_FREE_DATE_FARE = 12,			// Normal + Check-free date + fare����+�������+�������
	DEVICE_MODE_NORMAL_CHECK_FREE_TIME_DATE_FARE = 13,		// Normal + Check-free time + date + fare����+ʱ�����+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_TIME = 14,					// Check-free entry + time��վ���+ʱ�����
	DEVICE_MODE_CHECK_FREE_ENTRY_DATE = 15,					// Check-free entry + date��վ���+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_FARE = 16,					// Check-free entry + fare��վ���+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_TIME_DATE = 17,			// Check-free entry + time + date��վ���+ʱ�����+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_TIME_FARE = 18,			// Check-free entry + time + fare��վ���+ʱ�����+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_DATE_FARE = 19,			// Check-free entry + date + fare��վ���+�������+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_TIME_DATE_FARE = 20,		// Check-free entry + time + date + fare��վ���+ʱ�����+�������+�������
	DEVICE_MODE_CHECK_FREE_EXIT_TIME = 21,					// Check-free exit + time��վ���+ʱ�����
	DEVICE_MODE_CHECK_FREE_EXIT_DATE = 22,					// Check-free exit + date��վ���+�������
	DEVICE_MODE_CHECK_FREE_EXIT_FARE = 23,					// Check-free exit + fare��վ���+�������
	DEVICE_MODE_CHECK_FREE_EXIT_TIME_DATE = 24,				// Check-free exit + time + date��վ���+ʱ�����+�������
	DEVICE_MODE_CHECK_FREE_EXIT_TIME_FARE = 25,				// Check-free exit + time + fare��վ���+ʱ�����+�������
	DEVICE_MODE_CHECK_FREE_EXIT_DATE_FARE = 26,				// Check-free exit + date + fare��վ���+�������+�������
	DEVICE_MODE_CHECK_FREE_EXIT_TIME_DATE_FARE = 27,		// Check-free exit + time + date + fare��վ���+ʱ�����+�������+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME = 28,			//Check-free entry + exit + time��վ���+ʱ�����+��վ���
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_DATE = 29,			//Check-free entry + exit + date��վ���+��վ���+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_FARE = 30,			//Check-free entry + exit + fare��վ���+��վ���+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_DATE = 31,		//Check-free entry + exit + time + date��վ���+��վ���+ʱ�����+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_FARE = 32,		//Check-free entry + exit + time + fare��վ���+��վ���+ʱ�����+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_DATE_FARE = 33,		//Check-free entry + exit + date + fare��վ���+��վ���+�������+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_DATE_FARE = 34,	//Check-free entry + exit + time + date + fare��վ���+��վ���+ʱ�����+�������+�������
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT = 35					//Check-free entry + exit��վ���+��վ���	
}OperationalMode_t;


// վԱ��Ϣ
typedef struct _user_info
{
	CString sUserID;                    // �û�ID
	STAFF_AUTHORITY allAuthority;       // �û�ȫ��Ȩ��
	STAFF_AUTHORITY curAuthority;       // �û���ǰȨ��
	_DATE_TIME loginTime;               // ��¼ʱ��
	CString sPwd;
	_user_info() {                      // ���캯��
		Initialize();
	}

	void Initialize() {                 // ���ڻ�
		sUserID = _T("");
		allAuthority = NONE_AUTHORITY;
		curAuthority = NONE_AUTHORITY;
		sPwd = _T("");
	}
} USER_INFO, *LPUSER_INFO;

// Ʊ���������Ϣ
typedef struct _ticket_no_range
{
	DWORD nBeginNo;
	DWORD nEndNo;
	_ticket_no_range()
	{
		nBeginNo = 0;
		nEndNo = 0;
	}
} TICKET_NO_RANGE;

// �������ù�궨λ
typedef enum _task_control_focus 
{
	FOCUS_TASK_NAME_EDIT		= 1,
	FOCUS_PLAN_QUANTITY_EDIT	= 2,
	FOCUS_BEGIN_NO_EDIT			= 3,
	FOCUS_END_NO_EDIT			= 4,
	FOCUS_AMOUNT_EDIT			= 5,
	FOCUS_VALID_RIDES_EDIT		= 6
} TASK_CONTROL_FOCUS;

// ��������
typedef enum _task_type {
	TASK_TYPE_UNKNOW = 0,		// δ֪����
	TASK_TYPE_INITIALIZATION,	// ��ʼ������
	TASK_TYPE_PRE_ISSUE,		// Ԥ��ֵ����
	TASK_TYPE_RE_ENCODE,		// �ر�������
	TASK_TYPE_SORT,				// �ּ�����
	TASK_TYPE_PERSONALIZE,		// ���Ի�����
	TASK_TYPE_CANCEL,           // ע�������ƶ�
} TASK_TYPE;

// ģ������
typedef enum _template_type {
	TEMPLATE_TYPE_UNKNOW = 0,		// δ֪����
	TEMPLATE_TYPE_SORT = 10,		// �ּ�ģ��
	TEMPLATE_TYPE_PERSONALIZE = 11,	// ���Ի�ģ��
} TEMPLATE_TYPE;

// ��д����������
typedef enum _rw_operation_type {	
	OPERATION_TYPE_UNKNOW = 0,					// 0��δ֪��������
	OPERATION_TYPE_INITIALIZATION = 1,			// 1����ʼ��
	OPERATION_TYPE_PRE_ISSUE,					// 2��Ԥ��ֵ
	OPERATION_TYPE_REENCODE,					// 3���ر���
	OPERATION_TYPE_INITIALIZATION_REENCODE,		// 4����ʼ��+�ر���
	OPERATION_TYPE_REENCODE_PRE_ISSUE,			// 5���ر���+Ԥ��ֵ
	OPERATION_TYPE_CANCEL,						// 6��ע����Ԥ����
}RW_OPERATION_TYPE;

// ES2Ʊ�����
typedef enum _es_magazine_type{
	ES_MAGAZINE_NORMAL =0,	// ����Ʊ��
	ES_MAGAZINE_WASTE,		// ��Ʊ��
}ES_MAGAZINE_TYPE;

// ES2Ʊ����
typedef enum _es_magazine_number{
	ES_MAGAZINE_ALL = 0,			// ����Ʊ��
	ES_MAGAZINE_1 = 1,				// Ʊ��1
	ES_MAGAZINE_2 = 2,				// Ʊ��2
	ES_MAGAZINE_3 = 3,				// Ʊ��3
	ES_MAGAZINE_4 = 4,				// Ʊ��4
	ES_MAGAZINE_5 = 5,				// Ʊ��5
}ES_MAGAZINE_NO;

// ES��������λ
typedef enum _es2_sensor_site{	
	ES2_SENSOR_SITE_SEND_CARD = 0,	// ��λ0��������λ
	ES2_SENSOR_SITE_RW1,			// ��λ1����д��1��λ
	ES2_SENSOR_SITE_RW2,			// ��λ2����д��2��λ���ݲ�ʹ�ã�
	ES2_SENSOR_SITE_MAGAZINE1,		// ��λ3��Ʊ��1��λ
	ES2_SENSOR_SITE_MAGAZINE2,		// ��λ4��Ʊ��2��λ
	ES2_SENSOR_SITE_MAGAZINE3,		// ��λ5��Ʊ��3��λ
	ES2_SENSOR_SITE_MAGAZINE4,		// ��λ6��Ʊ��4��λ
	ES2_SENSOR_SITE_MAGAZINE5,		// ��λ7��Ʊ��5��λ
}ES2_SENSOR_SITE;

//////////// ���Ի���λ��־
//////////typedef enum _personal_area_flag{
//////////	PERSONAL_AREA_UNKNOWN		= 0,	// δ֪����
//////////	PERSONAL_AREA_PERSON_ID		= 1,	// Ա�����
//////////	PERSONAL_AREA_PERSON_NAME	= 2,	// Ա������
//////////	PERSONAL_AREA_PERSON_BIRTHDAY= 3,	// Ա������
//////////	PERSONAL_AREA_PAPER_TYPE	= 4,	// ֤������
//////////	PERSONAL_AREA_PAPER_ID		= 5,	// ֤�����
//////////	PERSONAL_AREA_COMPANY_NAME	= 6,	// ��˾����
//////////	PERSONAL_AREA_FOREGOUND		= 7,	// ���汳��
//////////	PERSONAL_AREA_BACKGOUND		= 8,	// ���汳��
//////////	PERSONAL_AREA_PERSON_PHOTO	= 9		// Ա����Ƭ
//////////} PERSONAL_AREA_FLAG;

//////////// ���Ի���λ��Ϣ
//////////typedef struct _personal_area{
//////////	PERSONAL_AREA_FLAG flag;		// ��λ��־
//////////	CString txtAreaContent;			// ��λ����
//////////	CString txtFont;				// �ı�����
//////////	int nSize;						// �ı���С
//////////	COLORREF color;					// �ı���ɫ
//////////	int x;							// ������
//////////	int y;							// ������
//////////
//////////	_personal_area()
//////////	{
//////////		flag = PERSONAL_AREA_UNKNOWN;
//////////		txtAreaContent = _T("");
//////////		txtFont = _T("");
//////////		nSize = 0;
//////////		color = 0;
//////////		x = 0;
//////////		y = 0;
//////////	}
//////////} PERSONAL_AREA;

// Ա��֤������
typedef enum _person_paper_type{
	PAPER_IDENTITY_CARD_MAINLAND = 1,		// ���֤(��½)
	PAPER_IDENTITY_CARD_MACAU = 2,			// ���֤(����)
	PAPER_IDENTITY_CARD_HONGKONG = 3,		// ���֤(���)
	PAPER_UNKNOW		= 4					// ����
} PERSON_PAPER_TYPE;

// ����״̬
typedef  enum _task_status {	

	TASK_STATUS_UNKONW		= 0x00,	// δ֪״̬
	TASK_STATUS_UNEXCUTE	= 0x01,	// δִ��
	TASK_STATUS_UNCOMPLETE	= 0x02,	// δ���
	TASK_STATUS_COMPLETED	= 0x03,	// �����
}TaskStatus;



// �����������
typedef  enum _task_end_type {		
	TASK_END_NORMAL			= 1,	// ������������
	TASK_END_UNNORMAL		= 2,	// �����쳣����
}TASK_END_TYPE;

// �ּ�ģ���м���������
typedef  enum _sort_compute_type {	
	COMPUTE_TYPE_EQUAL			= 1,	// ����
	COMPUTE_TYPE_GREATER_THAN	= 2,	// ����
	COMPUTE_TYPE_LESS_THAN		= 3,	// С��
	COMPUTE_TYPE_NO_EQUAL		= 4,	// ������
	COMPUTE_TYPE_NOT_LESS_THAN	= 5,	// ���ڵ���
	COMPUTE_TYPE_NOT_MORE_THAN	= 6,	// С�ڵ���	
}SORT_COMPUTE_TYPE;

// ESģ��
typedef enum _ES_module_number{		
	ES_MODULE_NUMBER_0 = 0,				// ȫ��ģ��
	ES_MODULE_NUMBER_1 = 1,				// ģ��1
	ES_MODULE_NUMBER_2 = 2,				// ģ��2
	ES_MODULE_NUMBER_3 = 3,				// ģ��3
} ES_MODULE_NUMBER;

// �ּ���Ϣ�ṹ��
typedef struct _sort_info{	
	SortType_t sortType;			// �ּ�������
	SORT_COMPUTE_TYPE computeType;	// �ּ�����
	CString computeValus;			// �ּ�����ѡ��ֵ
	_sort_info()
	{
		sortType = ID_SORT_TYPE_CARD_VER;
		computeValus = _T("");
		computeType = COMPUTE_TYPE_EQUAL;			
	}
}SORT_INFO;

#define WM_USER_TASK_END (WM_USER+5000)					// ���������Ϣ

// ���Զ�Ӧ���ݽṹ��
typedef struct _language_content{
	WORD languageType;			// �����������
	CString txtContent;			// ��Ӧ����	
	_language_content()
	{
		languageType = 0;
		txtContent = _T("");
	}
} LANGUAGE_CONTENT;



// Ʊ��״̬
typedef enum _ticket_logic_status{		
	TICKET_STATUS_UNKNOW = 0,			// δ֪״̬
	TICKET_STATUS_NORMAL = 1,			// ����
	TICKET_STATUS_ERROR = 2,			// �쳣
}TICKET_LOGIC_STATUS;

#define  NORMAL_MAGAZINE_NO_MAX_COUNT	4	// ����Ʊ������
#define  MAGAZINE_MAX_COUNT				5	// Ʊ��������
#define  RE_TRY_COUNT					3	// ����������
#define  BOX_BUFFER_LEN					5	// Ʊ����Ϣ���鳤��

// �޵�ͼ��ӡ������Ϣ
typedef struct _print_text_info {
	CString txtContent;		// �ı�����
	bool bBold;				// �Ƿ����
	int nSize;				// �����С
	int x;					// ������
	int y;					// ������
	_print_text_info()
	{
		txtContent = _T("");
		bBold = false;
		nSize = 0;
		x = 0;
		y = 0;
	}
} PRINT_TEXT_INFO;


// Ʊ��״̬
typedef enum _magazine_operation_type{	
	MAGAZINE_OPERATION_UNKNOW = 0,			// δ֪
	MAGAZINE_OPERATION_UNINSTALL = 1,		// ж��
	MAGAZINE_OPERATION_INSTALL = 2,			// ��װ
}MAGAZINE_OPERATION_TYPE;

// �豸��������
typedef enum _device_operation_type
{
	DEVICE_RUN = 0x01,				// ����
	DEVICE_SUSPEND = 0x02,			// ��ͣ
	DEVICE_CONTINUE = 0x03,			// ����
	DEVICE_END = 0x04,				// ����
	DEVICE_CHANGEMagazine = 0x05,	// ����Ʊ��
	DEVICE_RESERVE = 0x06			// Ԥ��
}DEVICE_OPERATION_TYPE;


// ���ӱ�ǩ���
typedef enum _rfid_no{
	RFID = 0,
	RFID_1 = 1,
	RFID_2 = 2,
	RFID_3 = 3,
	RFID_4 = 4,
} RFID_NO;

typedef struct _rfid_param{	
	HANDLE	hCommHandle;	// ���ھ��	
	RFID_NO rfidId;			// ���ӱ�ǩ���
	CString rfidName;		// ���ӱ�ǩ����
	_rfid_param()
	{	
		hCommHandle = INVALID_HANDLE_VALUE;
		rfidId = RFID;
		rfidName = _T("RFID");
	}
}RFID_PARAM;



// RFID���ݽṹ
typedef struct _SC_RFID_DATA
{
	BYTE byBoxID[4];					// 		0	Ʊ����	4	1BIN+1BIN+2BIN	���2�ֽڰ�Intel��洢 ,��̬��������
	//----------����Ϊ��̬��������----------------------------
	BYTE byOperatorID[3];				// 		0	����ԱID	3	BCD	
	BYTE byDeviceID[4];					// 		3	�豸ID	1BIN+2BCD+1BIN	
	BYTE byBoxLocaStatusBeforeOperate;	// 		7	Ʊ��λ��״̬	1	BIN	�μ�ע1.
	BYTE byBoxLocaStatusAfterOperate;	// 		8	������Ʊ��״̬	1	BIN	�μ�ע2
	BYTE byTicketIssuerID[4];			// 		9	Ʊ��������ID	4	BIN	��Intel��洢
	BYTE byTicketPHType;				// 		13	Ʊ����������	1	BIN	
	short sTicketProductType;			// 		14	��Ʊ��Ʒ����	2	BIN	��Intel��洢
	BYTE byAttributeOfPrepWrite;		// 		16	Ԥ��ֵ����	1	BIN	�μ�ע3
	BYTE bySeqOfDerivativeProduct;		// 		17	������Ʒ���	1	BIN	�μ�ע4
	short sTicketNumber;				// 		18	Ʊ������	2	BIN	Ʊ���ڵ�Ʊ����������Intel��洢
	BYTE byStationCode[2];				// 		20	��վ����	2	BCD	
	BYTE byPosition;					// 		22	��װλ��	1	BIN	����Ʊ�����豸�еİ�װλ��
	BYTE byBlockFlag[4];				// 		23	��������	4	BIN	��֮���������д���־����Intel��洢
	BYTE byOperateTime[7];				// 		27	������ʱ��	7	BCD	RFID���һ��д��ʱ��
	BYTE byCrc[2];						// 		34	У���ֶ�	2	BIN	�Կ�A��0��34�ֽڵ�CRC16У��ֵ,��Intel��洢��У���ֵ��0xFFFF��
	BYTE byReserve[12];					// 		35	Ԥ���ֶ�	12	BIN	

	_SC_RFID_DATA(){
		Initialize();
	}

	void Initialize(){
		memset(byBoxID,0xFF,sizeof(byBoxID));
		memset(byOperatorID,0,sizeof(byOperatorID));
		memset(byDeviceID,0,sizeof(byDeviceID));
		memset(&byBoxLocaStatusBeforeOperate,0,sizeof(byBoxLocaStatusBeforeOperate));
		memset(&byBoxLocaStatusAfterOperate,0,sizeof(byBoxLocaStatusAfterOperate));
		memset(byTicketIssuerID,0,sizeof(byTicketIssuerID));
		memset(&byTicketPHType,0,sizeof(byTicketPHType));
		memset(&sTicketProductType,0,sizeof(sTicketProductType));
		memset(&byAttributeOfPrepWrite,0,sizeof(byAttributeOfPrepWrite));
		memset(&bySeqOfDerivativeProduct,0,sizeof(bySeqOfDerivativeProduct));
		memset(&sTicketNumber,0,sizeof(sTicketNumber));
		memset(byStationCode,0,sizeof(byStationCode));
		memset(&byPosition,0,sizeof(byPosition));
		memset(byBlockFlag,0,sizeof(byBlockFlag));
		memset(byOperateTime,0,sizeof(byOperateTime));
		memset(byCrc,0,sizeof(byCrc));
		memset(byReserve,0,sizeof(byReserve));
	}
} SC_RFID_DATA, * LPSC_RFID_DATA;


//////////// Ʊ����ӡ��״̬����
//////////typedef enum _pmStatus{
//////////	PM_STATUS_UNKNOW  = 0x00, // δ֪״̬
//////////	PM_STATUS_OFF	  = 0x50, // �ر�״̬
//////////	PM_STATUS_ON	  = 0xD0, // �ϵ�״̬
//////////	PM_STATUS_READY   = 0x18  // ����״̬
//////////}PM_STATUS;


// Ԥ��ֵ��ʽ
typedef enum _pre_issue_mothed{
	PREISSUE_MOTHED_PRE_ISSUE			= 0, // Ԥ��ֵ
	PREISSUE_MOTHED_REENCODE_PRE_ISSUE	= 1, // �ر���+Ԥ��ֵ
}PRE_ISSUE_MOTHED;


// �ر��뷽ʽ
typedef enum _re_encode_mothed{
	PREISSUE_MOTHED_REENCODE				= 0, // �ر���
	PREISSUE_MOTHED_INITIALIZATION_REENCODE	= 1, // ��ʼ��+�ر���
}RE_ENCODE_MOTHED;


// �豸�ṩ��
typedef enum _device_provider{
	DEVICE_PROVIDER_DAT     = 0x000F, // DAT
	DEVICE_PROVIDER_FOUNDER = 0x0011  // ����
}DEVICE_PROVIDER;

// �����汾����
typedef enum _parameter_ver_type
{
	NOMAL_CUR     = 0x00,  // ��ʽ������ǰ�汾
	NORMAL_FUTURE = 0x01,  // ��ʽ���������汾
	TEST_CUR      = 0x10,  // ���Բ�����ǰ�汾
	TEST_FUTURE   = 0x11   // ���Բ��������汾
}PARAMETER_VER_TYPE;

// ��¼�ǳ�����
typedef enum _longin_type{
	LOGIN_ON  = 0x00,
	LOGIN_OFF = 0x01
}LOGIN_TYPE;

// AR����ԭ��
typedef enum _ar_reson{
	AR_IN_SERVICE    = 0x01,  //��Ӫ��ʼ
	AR_OUTOF_SERVICE = 0x02,  // ��Ӫ����
	AR_TIME_ON       = 0x03   // ����ָ��ʱ����
}AR_REASON;

// �������ͱ��붨��
typedef enum _banknoteAndCoinType_t{
	VALUE_UNKNOWN	= 0x00,
	CoinHalfYuan	= 0x10,
	Coin1Yuan		= 0x11,
	
	Banknote1Yuan	= 0x31,
	Banknote5Yuan	= 0x33,
	Banknote10Yuan	= 0x34,
	Banknote20Yuan	= 0x35,
	Banknote50Yuan	= 0x36,
	Banknote100Yuan	= 0x37,
}BankNoteAndCoinType_t;

// Ǯ�����ඨ��
typedef enum _box_type_def_{
	UNDEFINE_BOX,
	COIN_HOPPER_1,
	COIN_HOPPER_2,
	COIN_CHANGE_BOX_1,
	COIN_CHANGE_BOX_2,
	COIN_COLL_BOX_1_1,
	COIN_COLL_BOX_1_5,
	COIN_COLL_BOX_2,
	BNR_CASH_BOX,
	BNR_CHANGE_BOX,
	BNR_RECYCLER_BOX_3,
	BNR_RECYCLER_BOX_4,
	BNR_RECYCLER_BOX_5,
	BNR_RECYCLER_BOX_6,
}MONEY_BOX_TYPE;


//--------------------------------------������ͨ��Э��ͷ���ݶ���-----------------------------------------//
// AfcSysHdr_t AFCϵͳר��ͷ�ṹ�嶨�� 10 BIN
typedef struct _afcsyshdr_t 
{
	BYTE OperatorID[3];             // 3 BCD ����ԱID
	_DATE DateTime;                 // 4 BCD ����ʱ��
	WORD DataLen;                   // 2 BIN ��Ϣ����
	BYTE Reserve;                   // 1 BIN Ԥ��
	_afcsyshdr_t(){
		memset(OperatorID, 0x00, sizeof OperatorID);
		DataLen = 0;
		Reserve = 0;
	}
}AfcSysHdr_t;

// SysComHdr_t ϵͳ����ͷ�ṹ�嶨�� 56 BIN
typedef struct _syscomhdr_t {
	_DATE_TIME transTime;				//����ʱ��
	WORD resourceProviderID;			//��Դ�ṩ��ID
	BYTE transType;								//��������
	BYTE transSubType;						//����������
	WORD cityCode;								//���д���
	WORD industryCode;						//��ҵ����
	BYTE lineCode;								//��·����
	WORD stationCode;							//վ�����
	BYTE deviceType;							//�豸����
	AccDeviceID_t deviceID;				//�豸����
	DWORD transSerialNo;					//�������к�
	BYTE dataType;								//��������
	WORD parameterVersion;				//��������
	BYTE samID[6];								//SAM���ն˻����
	DWORD samTransSerialNo;			//SAM���������к�
	_syscomhdr_t()
	{
		memset(samID,0,sizeof samID);
	}
}SysComHdr_t;

// SysCardCom_t Ʊ��������ṹ�嶨�� 20 BIN
typedef struct _syscardcom_t {
	DWORD cardIssuerId;				// 4 BIN ����Ʊ���ķ����˵�ΨһID
	DWORD cardSerialNumber;         // 4 BIN Ʊ�������
	DWORD cardType;                 // 4 BIN Ʊ��������
	DWORD cardLifeCycleCount;       // 4 BIN Ʊ���ĵ�ǰʹ�����ڼ���
	DWORD cardActionSequenceNumber; // 4 BIN ���������кţ�����У�
	_syscardcom_t(){
		cardIssuerId = 0;
		cardSerialNumber = 0;
		cardType = 0;
		cardLifeCycleCount = 0;
		cardActionSequenceNumber = 0;
	}
}SysCardCom_t;

// SysCardholderCom_t �ֿ��˹���ͷ�ṹ�嶨�� 16 BIN
typedef struct _syscardholdercom_t {
	DWORD cardholderSerialNum;		// 4 BIN ���ض������˷�Χ��ʶ��ֿ��˵�ר�ú���
	DWORD cardholderIssuerId;		// 4 BIN �ֿ��˵ķ����ˡ����̶�ΪACC=0xffffffff��
	DWORD companyId;				// 4 BIN ACC ���幫˾��ʶ������Ա��Ʊ���ԣ�����ָ������Ӫ�̱��롣
	//		���ǲ��뷽�ı�ʶ�롣
	//		0..255 = ΪACC �Լ���Ӫ��Ԥ����
	//		256..65535 = ������˾���ã��ӿ���ȡ�ã�
	DWORD classificationLevel;		// 4 BIN �˿ͼ���1:VIP;255:δ��
	_syscardholdercom_t(){
		cardholderSerialNum = 0;
		cardholderIssuerId = 0;
		companyId = 0;
		classificationLevel = 0;
	}
}SysCardholderCom_t;

// SysSecurityHdr_t ��ȫͷ�ṹ�嶨�� 12 BIN
typedef struct _syssecurityhdr_t {
	BYTE txnMac[4];					// 4 BIN MAC
	BYTE keyVersion;				// 1 BIN ������������MAC ����Կ�汾
	_syssecurityhdr_t(){
		memset(txnMac, 0x00, sizeof txnMac);
		keyVersion = 0;
	}
}SysSecurityHdr_t;

// �ڲ��������ݹ�����ṹ�嶨��	17 BIN
typedef struct _transComHdr_t
{
	DEVICE_ID deviceid;				// 1BIN+2BCD+1BIN �豸ID
	DWORD dwAfcSerialNo;			// 4 BIN AFC������ˮ��
	_DATE_TIME dtTransDataTime;		// 7 BCD ���ײ���ʱ��
	BYTE bMsgType;					// 1 BIN ��������
	BYTE bMsgSubType;				// 1 BIN ����������
	_transComHdr_t(){
		dwAfcSerialNo = 0;
		bMsgType = 0;
		bMsgSubType = 0;
	}
} TransComHdr_t;

// ҵ������ר��ͷ 32 BIN
typedef struct _opercomhdr_t{
	BYTE operatorID[3];				// 3 BCD ����ԱID
	_DATE dataTime;					// 4 BCD ��������
	BYTE localType;					// 1 BIN λ������
	WORD localAreaID;				// 2 BCD λ��ID
	DEVICE_ID deviceID;				// 4 1BIN+2BCD+1BIN �豸ID
	DWORD afcSeqNo;					// 4 BIN AFC������ˮ��
	_DATE_TIME operTime;			// 7 BCD ҵ�����ݲ���ʱ��
	BYTE msgType;					// 1 BIN ��������
	BYTE msgSubType;				// 1 BIN ����������
	WORD msgLen;					// 2 BIN ��Ϣ����
	//BYTE reserve[3];				// 3 Ԥ��
	_opercomhdr_t(){
		memset(operatorID, 0x00, sizeof operatorID);
		localType = 0;
		localAreaID = 0;
		afcSeqNo = 0;
		msgType = 0;
		msgSubType = 0;
		msgLen = 0;
		//memset(reserve, 0x00, sizeof reserve);
	}
}OperComHdr_t;

// ״̬����ͷ 11 BIN
typedef struct _statusComHdr_t
{
	WORD wStatusCode;				// 2 BIN ����������
	WORD wStationCode;				// 2 BCD ��վID
	DEVICE_ID deviceID;				// 4 1BIN+2BCD+1BIN �豸ID
	char operatorID[3];				// 3 BCD ����ԱID
	_statusComHdr_t(){
		wStatusCode = 0;
		wStationCode = 0;
		memset(operatorID,0x00,sizeof operatorID);
	}
} StatusComHdr_t;

//--------------------------------------������ͨ��Э��ͷ���ݶ���-----------------------------------------//

// Ԥ��ֵ����
typedef enum _pre_pay_method_{
	PRE_PAY_CARD	= 0x01,		// Ԥ��ֵ��
	YKT_EMPTY_CARD	= 0x02,		// һ��ͨ�׿�
	ACC_INIT_CARD	= 0x03,		// ACC��ʼ����
}PRE_PAY_METHOD;

// ���߶���
typedef enum _AntennaMark__mothod_{
	ANTENNA_MARK_ALL	= 0x01,		// ˫����(A��B)ͬʱѰ��
	ANTENNA_MARK_A		= 0x02,		// ֻ��A����Ѱ��
	ANTENNA_MARK_B		= 0x03,		// ֻ��B����Ѱ��
	ANTENNA_MARK_C		= 0x04,		// ֻ��C����Ѱ������TVM��ά���Ŷ���ʹ�ã�
}ANTENNA_MARK_MOTHOD;
