#pragma  once

// ��ҵ�񼰷���ID����
typedef	enum _service_id 
{
	UNKNOWN_SVC								= -1,	// δ��ҵ��

	// �˿�ҵ��ID��1 - 99��
	ISSUE_SVC								= 10,	// ��Ʊ
	BLIND_ISSUE_SVC							= 11,	// ä����Ʊ
	CHARGE_SVC								= 20,	// ��ֵ
	BLIND_CHARGE_SVC						= 21,	// ä�˳�ֵ
	ISSUE_PRODUCT_SVC						= 30,	// �۲�Ʒ
	BLIND_ISSUE_PRODUCT_SVC					= 31,	// ä���۲�Ʒ
	ANALYZE_SVC								= 40,	// ��ѯ
	ISSUE_USE_SVT_SVC						= 41,	// ��ֵƱ��Ʊ
	LANGUAGE_SWITH_SVC						= 42,	// �����л�
	CALL_HELP_SVC							= 50,	// ��Ԯ
	ADJUST_SVC								= 60,	// ��Ʊ��@#������
	REFUND_SVC								= 70,	// �˿@#������
	EXCHANGE_SVC							= 80,	// ������@#������
	STANDBY_SVC								= 90,	// ����
	SVT_ISSUE_SVC							= 91,	// ��ֵ����Ʊ
	// ά��ҵ��ID��100 - 999��
	MAINTENANCE_SVC							= 300,	// ά��ҵ��
	TRANSACTION_HISTORY_SVC					= 310,	// ����һ��
	DEVICE_STATUS_SVC						= 320,	// �豸״̬ 
	SALE_STATIC_SVC							= 330,	// ����ͳ��
	CARD_BOX_MANAGE_SVC						= 340,	// Card�����
	TOKEN_BOX_MANAGE_SVC					= 350,	// Token�����
	BANKNOTE_BOX_MANAGE_SVC					= 360,	// ֽ�������
	INSTALL_BANKNOTE_COLLECTION_BOX_SVC					=361,	// ֽ�һ����䰲װ
	REMOVE_BANKNOTE_COLLECTION_BOX_SVC					=362,	// ֽ�һ�����ж��
	INSTALL_BANKNOTE_CHANGE_INVALID_BOX_SVC				=363,	// ֽ�������䰲װ
	REMOVE_BANKNOTE_CHANGE_INVALID_BOX_SVC				=364,	// ֽ��������ж��
	BANKNOTE_BOX_RECYCLE_CHANGE_MONEY_SVC				=365,	// ֽ��ģ�����ֽ��

	COIN_BOX_MANAGE_SVC						= 370,	// Ӳ�������
		ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC	= 371,	// Ӳ��ר��������ӱ�
		ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC		= 372,	// Ӳ��ѭ��������
		ATTACH_COLLECTION_COIN_BOX_SVC					= 373,	// ��װӲ�һ�����
		DETACH_COLLECTION_COIN_BOX_SVC					= 374,	// ж��Ӳ�һ�����
		DETACH_ADDING_COIN_BOX_SVC						= 375,	// ж��Ӳ�Ҳ�����
		COLLECTION_COIN_SVC								= 376,	// ����Ӳ��
		INSTALL_COIN_CHANGE_BOX_SVC						= 377,	// ��װӲ��������
		REMOVE_COIN_CHANGE_BOX_SVC						= 378,	// ж��Ӳ��������


	OPERATION_CLOSE_SVC						= 380,	// ҵ�����
	RUN_MODE_SETTING_SVC					= 400,	// ����ģʽ�趨
		SERVICE_MODE_SETTING_SVC			= 401,	// ����ģʽ�趨
		PAYMENT_MODE_SETTING_SVC			= 402,	// ֧��ģʽ�趨
		CHANGE_MODE_SETTING_SVC				= 403,	// ����ģʽ�趨
		PRINT_MODE_SETTING_SVC				= 404,	// ��ӡģʽ�趨
	RECEIPT_REPRINT_SVC						= 410,	// ƾ֤����
	SYSTEM_SET_SVC							= 420,	// ϵͳ�趨
		STATION_SET_SVC						= 421,	// ��վ����
		COMMUNICATION_SET_SVC				= 422,	// ͨ�Ų�������
		SYSTEM_TIME_SET_SVC					= 423,	// ϵͳʱ���趨
		SWITCH_OFF_SVC						= 424,	// �����趨
		MODE_SWITCH_SVC						= 425,	// ģʽ�л����ã�@#������
		UDSN_SET_SVC						= 426,	// UDSN�趨��@#������
		RFID_MODE_SET_SVC					= 427,	// RFIDģʽ�趨��@#������
		SCREEN_BRIGHTNESS_SET				= 428,  // ��Ļ�����趨
		DEVICE_COMPORT_SET_SVC				= 429,	// ���ں��벨��������
	DATA_MANAGE_SVC							= 440,	// ���ݹ���
		DATA_INPUT_SVC						= 431,	// ���ݵ���
		DATA_OUTPUT_SVC						= 432,	// ���ݵ���
		VERSION_INFO_SVC					= 433,	// �汾��Ϣ
	DEVICE_TEST_SVC							= 700,	// Ӳ���Լ�
		RW_TEST_SVC							= 710,	// ��д������
		COIN_MODULE_TEST_SVC				= 720,	// Ӳ��ģ�����
		BANKNOTE_ACCEPT_MODULE_TEST_SVC		= 730,	// ֽ�ҽ���ģ�����
		BANKNOTE_CHANGE_MODULE_TEST_SVC		= 740,	// ֽ������ģ�����
		CARD_MODULE_TEST_SVC				= 750,	// ����ģ�����
		TOKEN_MODULE_TEST_SVC				= 760,	// Tokenģ�����
		RECEIPT_RINTER_TEST_SVC				= 770,	// ƾ֤��ӡ������
		PIN_PAD_TEST_SVC					= 780,	// �������ģ�����
		BANKCARD_MODULE_TEST_SVC			= 790,	// ���п�����ģ�����
		IO_MODULE_TEST_SVC					= 800,	// IOģ�����
		NETWORK_TEST_SVC					= 810,	// �������
		STATUS_DISPLAY_TEST_SVC				= 820,	// ״̬��ʾ�����ԣ�@#������
		UPS_TEST_SVC						= 830,	// UPS���ԣ�@#������
		ALL_DEVICE_STATUS_SVC				= 840,	// ����Ӳ��״̬��Ӳ���Լ죩
	REMOVE_EXCEPTION_SVC					= 900,	// �쳣���
	ONE_KEY_SETTLEMENT_SVC					= 910,	// һ���ջ�

	// ����������ڲ�����ҵ��ID��1000 -	1999��
	ROOT_SVC								= 1000,	// ��
	MAIN_SVC								= 1010,	// ����
	INIT_SVC								= 1050,	// ��ʼ��
	LOGIN_SVC								= 1060,	// ��¼
	LOGOFF_SVC								= 1070,	// �ǳ�
	END_SVC									= 1080,	// ҵ�����
	SHUTDOWN_RESTART_SVC					= 1090,	// �ػ�����ҵ��
		SHUTDOWN_SVC						= 1100,	// �ػ�
		RESTART_SVC							= 1110,	// ������
	EXIT_SVC								= 1120,	// �˳�����
	WAIT_MAINTENANCE_SVC					= 1130,	// �ȴ�ά������������ѡ��

	// ��̨����ID��2000 -	9999��
	SC_SVC									= 2000,
		TRANSMISSION_INTERVAL_SVC			= 2020,	// ���ڴ���
		BUSINESS_INTERVAL_SVC				= 2030,	// ҵ�����ݶ��ڴ���
		EVENT_MSG_SVC						= 2040,	// �¼����ݴ������
		STATUS_MGR_SVC						= 2070,	// ״̬�������
	MONITOR_SVC								= 3000,
		MONITOR_RECHARGE_RW_SVC				= 3010,	// ��ֵ��д��״̬���
		MONITOR_CARD_RW__SVC				= 3020,	// Ʊ����д��״̬���
		MONITOR_TOKEN_RW__SVC				= 3030,	// Token��д��״̬���
		MONITOR_CARD_ISSUE_MODULE_SVC		= 3040,	// ����ģ��״̬���
		MONITOR_UPS_SVC						= 3050,	// UPS���
		MONITOR_IO_SVC						= 3060,	// IO���
		MONITOR_TOKEN_MODULE_SVC			= 3070,	// TOKENģ����
		MONITOR_COIN_MODULE_SVC				= 3080,	// Ӳ��ģ����
		MONITOR_BANKNOTE_ACCEPT_MODULE_SVC	= 3090,	// ֽ�ҽ���ģ����
		MONITOR_BANKNOTE_CHANGE_MODULE_SVC	= 3100,	// ֽ������ģ����
		MONITOR_PINPAD_MODULE_SVC			= 3110,	// �������ģ����
		MONITOR_BANKCARD_MODULE_SVC			= 3120,	// ���п�����ģ����
		MONITOR_PRINTER_MODULE_SVC			= 3130,	// ��ӡ��ģ����
	CONFIRM_ALIVE_SVC						= 4000,	// ȷ�ϴ�����

} SERVICE_ID;


