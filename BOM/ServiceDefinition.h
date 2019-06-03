#pragma  once

// ��ҵ��Lib��ID
typedef	enum _service_id {
	UNKNOWN_SVC					= -1,		// δ��ҵ��

	// ��ҵ��ID��1 - 99��
	ISSUE_SVC					= 10,		// ��Ʊ
	CHARGE_SVC				= 20,		// ��ֵ
	//ADDTIMES_SVC				= 30,		// ���
	ADJUST_SVC					= 40,		// ��Ʊ
	ANALYZE_SVC					= 50,		// ����
	REFUND_SVC					= 60,		// �˿�
		REFUND_BASE_SVC			= 61,
		REFUND_APPLY_SVC		= 62,
		REFUND_QUERY_SVC		= 63,
//	SURRENDER_SVC				= 70,		// ��Ʊ
	EXCHANGE_SVC	            = 70,		// �滻
	PREISSUE_SVC				= 80,		// Ԥ��
	COUNTERACT_SVC				= 90,		// ����
	
	// ����ҵ��ID��100 - 200��
	OTHERS_SVC					= 99,		// ����
		TVMFAILURE_REFUND_SVC		= 100,		// TVM�����˿�
		NOBUSINESS_INCOME_SVC		= 110,		// ��Ӫҵ��֧
		REFRESH_SVC					= 120,		// ���
		SIGNCARD_APPLY_SVC			= 130,		// ����������
		SIGNCARD_REPORT_LOSS_SVC	= 140,		// ��������ʧ
		SIGNCARD_CANCEL_REPORT_LOSS_SVC = 150,	//���������
		CARD_SUSPEND_AND_RESUME_SVC = 160,		//��ʧ���
		TICKETBOX_EXCHANGE_SVC = 170,						//Ʊ�����
		ADMINISTRATION_PROCESS_SVC = 180,
		LOCK_SVC = 190,
		DEFER_SVC = 200,
		RECEIPT_PRINT_SVC = 210,						// ��ӡ����
	// ����ҵ��ID��300 - 499��
	ASSITANT_SVC				= 299,		// ����
		SALE_STATIC_SVC				= 300,		// ����ͳ��

	// ά��ҵ��ID��500 - 999��
	SYSTEM_SET_SVC				= 500,		// ϵͳ�趨
	STATION_SET_SVC				= 510,		// ��վ����
	COMMUNICATION_SET_SVC		= 520,		// ͨ�Ų�������
	MODE_SWITCH_SVC				= 530,		// ģʽ�л�����
	SYSTEM_TIME_SET_SVC			= 540,		// ϵͳʱ���趨
	UDSN_SET_SVC				= 550,		// UDSN�趨
	TH_MODE_SET_SVC				= 560,		// THģʽ�趨
	RFID_MODE_SET_SVC			= 570,		// RFIDģʽ�趨
	DATA_MANAGE_SVC				= 600,		// ���ݹ���
		DATA_INPUT_SVC			= 610,		// ���ݵ���
		DATA_OUTPUT_SVC			= 620,		// ���ݵ���
	DEVICE_TEST_SVC				= 700,		// Ӳ���Լ�
		OUTRW_TEST_SVC			= 710,		// ��д������
		PRINTER_TEST_SVC        = 720,		// ��ӡ������
		MONEYBOX_TEST_SVC		= 730,		// Ǯ�����
		NETWORK_TEST_SVC		= 740,		// �������
		UPS_INFO_SVC			= 750,		// UPS��Ϣ
		TH_TEST_SVC				= 760,		// ��Ʊ������
	REMOVE_EXCEPTION_SVC        = 800,		// �쳣���

	// ����������ڲ�����ҵ��ID��1000 -	9989��
	ROOT_SVC					= 1000,		// ��
	MAIN_SVC					= 1010,		// ����
	INIT_SVC					= 1050,		// ��ʼ��
	LOGIN_SVC					= 1060,		// ��¼
	LOGOFF_SVC					= 1070,		// �ǳ�
	END_SVC						= 1080,		// ҵ�����
	SHUTDOWN_SVC				= 1090,		// �ػ�
	EXIT_BOM_SVC				= 1100,		//�˳�BOM����

	// ������
	RESTART_SVC                 = 1180,     // ������

	// ���ڻ��쳣���
	REMOVE_INIT_EXCEPTION_SVC   = 1190,		// ���ڻ��쳣���
	
	SC_SVC						= 2000,
		TRANSMISSION_INTERVAL_SVC	= 2020,		// ���ڴ���
		BUSINESS_INTERVAL_SVC       = 2030,     // ҵ�����ݶ��ڴ���
		EVENT_MSG_SVC               = 2040,     // �¼����ݴ������
		STATUS_MGR_SVC              = 2070,     // ״̬�������
	MONITOR_SVC	= 3000,
		MONITOR_RW_SVC  = 3010, //�ⲿ��д�����
		MONITOR_TW_SVC  = 3020, //�ڲ���д�����
		MONITOR_TH_SVC  = 3030, //��Ʊ�����
		MONITOR_UPS_SVC = 3040, //UPS���
		MONITOR_LPT_SVC = 3050, //��Ʊ���Ž����
	CONFIRM_ALIVE_SVC = 4000,		//ȷ�ϴ�����

} SERVICE_ID;


