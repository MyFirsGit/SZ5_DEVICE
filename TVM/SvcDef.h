#include "util.h"
#include "UIDefinition.h"
#pragma once

//������ʾ�ó�������
//static const int MAX_ARRAY_LEN	= 9;            ///< ��ͨ��Ϣ�����������

// �Ӳ˵���ťλ��
//#define  SUBMENU_BTN1_POINT          CPoint(36,489)			///< �Ӳ˵���һ����ť���Ͻ�����
//#define  SUBMENU_BTN2_POINT          CPoint(143,489)		///< �Ӳ˵��ڶ�����ť���Ͻ�����
//#define  SUBMENU_BTN3_POINT          CPoint(250,489)			///< �Ӳ˵���������ť���Ͻ�����
//#define	SUBMENU_BTN4_POINT          CPoint(357,489)			///< �Ӳ˵����ĸ���ť���Ͻ�����
//#define  SUBMENU_BTN5_POINT          CPoint(464,489)			///< �Ӳ˵��������ť���Ͻ�����
//#define  PRINT_BTN_POINT             CPoint(571,489)			///< �Ӳ˵���ӡ��ť���Ͻ�����
//#define  BACK_BTN_POINT              CPoint(678,489)			///< �Ӳ˵����ذ�ť���Ͻ�����
//#define  OK_BTN_POINT               CPoint(785,489)			///< �Ӳ˵�ȷ����ť���Ͻ�����
//#define  STANDARD_BTN_POINT         CPoint(NORMAL_BUTTON_WIDTH,NORMAL_BUTTON_HEIGHT)
//#define   SUBMENU_BTN3_RECT           CRect(SUBMENU_BTN3_POINT,SUBMENU_BTN3_POINT+STANDARD_BTN_POINT)	///< �Ӳ˵���������ť��С
//#define   SUBMENU_BTN4_RECT           CRect(SUBMENU_BTN4_POINT,SUBMENU_BTN4_POINT+STANDARD_BTN_POINT)	///< �Ӳ˵����ĸ���ť��С
//#define   SUBMENU_BTN5_RECT           CRect(SUBMENU_BTN5_POINT,SUBMENU_BTN5_POINT+STANDARD_BTN_POINT)	///< �Ӳ˵��������ť��С
//
//static const int DATAINPUT_MSG_POINT_X           =   26;
//static const int DATAINPUT_DATATYPE_MSG_POINT_Y  =    8;
//static const int DATAINPUT_INPUTTYPE_MSG_POINT_Y =  172;
//
//static const int DEVICETEST_TESTTYPE_MSG_POINT_X     =   26;
//static const int DEVICETEST_TESTTYPE_MSG_POINT_Y     =    8;
//static const int DEVICETEST_TESTDETAIL_MSG_POINT_X   =   26;
//static const int DEVICETEST_TESTDETAIL_MSG_POINT_Y   =  264;
//
//static const int DEVICETEST_DETAILITEM_MSG_POINT_Y =  292;

// ����ģʽ
typedef	enum _service_mode_id{
	//0x1F00,			   // ��������
	//0x0000,			   // ��ͣ����
}SERVICE_MODE_ID;

// ҵ��ģʽ����
typedef	enum _business_mode_id{
	//0x1F00,			   // ����ҵ��ģʽ
	//0x1000,			   // ֻ��Ʊ
	//0x0800,			   // ֻ�ۿ�
	//0x0400	,			   // ֻ�۲�Ʒ
	//0x0200	,			   // ֻ��ֵ
	//0x0100,			   // 	ֻ��ѯ
	//0x0F00	,			   // ����Ʊ
	//0x1700,			   // 	���ۿ�
	//0x1B00	,			   // ���۲�Ʒ
	//0x1D00,			   // 	����ֵ
	//0x1E00	,			   // ����ѯ
	//0x1800,			   // 	��Ʊ+�ۿ�
	//0x1400	,			   // ��Ʊ+�۲�Ʒ
	//0x1200,			   // 	��Ʊ+��ֵ
	//0x1100	,			   // ��Ʊ+��ѯ
	//0x0C00,			   // 	�ۿ�+�۲�Ʒ
	//0x0A00,			   // 	�ۿ�+��ֵ
	//0x0900,			   // 	�ۿ�+��ѯ
	//0x0600,			   // 	�۲�Ʒ+��ֵ
	//0x0500,			   // 	�۲�Ʒ+��ѯ
	//0x0300	,			   // ��ֵ+��ѯ
	//0x1C00,			   // 	��Ʊ+�ۿ�+�۲�Ʒ
	//0x1600	,			   // 	��Ʊ+�ۿ�+��ֵ
	//0x1A00,			   // 		��Ʊ+�ۿ�+��ѯ
	//0x1900,			   // 		��Ʊ+�۲�Ʒ+��ֵ
	//0x1500,			   // 		��Ʊ+�۲�Ʒ+��ѯ
	//0x1300	,			   // 	��Ʊ+��ֵ+��ѯ
	//0x0E00	,			   // 	�ۿ�+�۲�Ʒ+��ֵ
	//0x0D00,			   // 		�ۿ�+�۲�Ʒ+��ѯ
	//0x0700	,			   // 	�۲�Ʒ+��ֵ+��ѯ
} BUSINESS_MODE_ID;

// ֧��ģʽ����
typedef	enum _payment_mode_id{
	//0x003C	�����տ�ģʽ
	//0x0000	���տ�ģʽ
	//0x0020	ֻ�ռ���
	//0x0010	ֻ��Ӳ��
	//0x0008	ֻ�����п�
	//0x0004	ֻ��EP
	//0x001C	���ռ���
	//0x002C	����Ӳ��
	//0x0034	�����y�п�
	//0x0038	����EP
	//0x000C	���լF��
	//0x0024	����Ӳ��/�y�п�
	//0x0028	����Ӳ��/EP
	//0x0014	���ռ���/�y�п�
	//0x0018	���ռ���/EP
	//0x0030	�����y�п�/EP
}PAYMENT_MODE_ID;

// ����ģʽ
typedef	enum _change_mode_id{
	//0x0002	��������
	//0x0000	�o����
}CHANGE_MODE_ID;

// ��ӡģʽ
typedef	enum _print_mode_id{
	//0x0001	�ɴ�ӡ
	//0x0000	�o��ӡ
}PRINT_MODE_ID;


// ����ID����
typedef enum  _audio_id{
	// ͨ������
	AUDIO_ID_WELCOME									= 4001,	//	��ӭ���٣���ѡ�����ϵİ�ť
	AUDIO_ID_THANKS										= 4002,	//	���׳ɹ���ллʹ��
	AUDIO_ID_CALLING_STAFF								= 4003,	//	����վԱ�У����Ժ�
	AUDIO_ID_INSERT_CASH								= 4004,	//	��Ͷ���ֽ�
	AUDIO_ID_INSERT_BANKCARD							= 4005,	//	��������п�
	AUDIO_ID_SWIPE_EP_CARD								= 4006,	//	��ˢ��ֵ��
	AUDIO_ID_SWIPE_CARD									= 4007,	//	��ˢ��
	AUDIO_ID_SWIPE_CARD_AGAIN							= 4008,	//	������ˢ��
	AUDIO_ID_INVALIDATE_CARD							= 4009,	//	�ÿ������ã��뵽���ڴ���
	AUDIO_ID_SWITCH_PAYMENT								= 4010,	//	�ÿ������ã���ѡ������֧����ʽ
	AUDIO_ID_INPUT_ERROR								= 4011,	//	�������ݲ��Ϸ�����ȷ�Ϻ���������
	AUDIO_ID_TRANSACTION_CANCELED						= 4012,	//	������ȡ����ллʹ��
	AUDIO_ID_TRANSACTION_FAILED							= 4013,	//	����ʧ�ܣ�����ϵ������Ա	
	AUDIO_ID__WAITINGTRANSACTION						= 4014,	//  ������,���Ժ�

	// ä��ר��
	AUDIO_ID_LANGUAGE_TYPE								= 4015,	//  ��������
	AUDIO_ID_BUSINESS_TYPE								= 4016,	//	ҵ������
	AUDIO_ID_TICKET_TYPE								= 4017,	//	��Ʊ����
	AUDIO_ID_ISSUE_METHOD								= 4018,	//	��Ʊ��ʽ
	AUDIO_ID_HOT_STATION								= 4019,	//	�ȵ㳵վ
	AUDIO_ID_TICKET_PRICE								= 4020,	//	��Ʊ����
	AUDIO_ID_START_SECTION								= 4021,	//  ��ʼ����
	AUDIO_ID_END_SECTION								= 4022,	//	Ŀ������
	AUDIO_ID_START_LINE									= 4023,	//	��ʼ��·
	AUDIO_ID_END_LINE									= 4024,	//	Ŀ����·
	AUDIO_ID_START_STATION								= 4025, //  ��ʼ��վ
	AUDIO_ID_END_STATION								= 4026,	//	Ŀ�ĳ�վ
	AUDIO_ID_ISSUE_QUANTITY								= 4027,	//  ��Ʊ����
	AUDIO_ID_PASSENGER_TYPE								= 4028,	//	�˿�����
	AUDIO_ID_PRODUCT_TYPE								= 4029,	//	��Ʒ����
	AUDIO_ID_TRANSACTION_INFO							= 4030,	//	������Ϣ
	AUDIO_ID_PAYMENT_METHOD								= 4031, //  ֧����ʽ
	AUDIO_ID_PAYMENT_INFO								= 4032,	//	֧����Ϣ
	AUDIO_ID_PAYMENT_PASSWORD							= 4033, //  ֧������
	AUDIO_ID_CHARGE_AMOUNT								= 4034,	//	��ֵ���
	AUDIO_ID_TRANSACTION_OK_CANCEL						= 4035, //	��ȷ�Ͻ�����Ϣ��Ȼ����ȷ�ϼ��������밴ȡ����
	AUDIO_ID_TICKET_PEIECE								= 4041, //	��

	AUDIO_ID_PLEASE_PRESS								= 4900,	//  �밴
	AUDIO_ID_PLEASE_SELECT								= 4901,	//	��ѡ��
	AUDIO_ID_PLEASE_INPUT								= 4902,	//	������
	AUDIO_ID_PLEASE_CONFIRM								= 4903,	//	��ȷ��
	AUDIO_ID_RANGE_1_TO_9								= 4904, //  ��Χ1��9
	AUDIO_ID_OK_OR_CANCEL								= 4905,	//	Ȼ����ȷ�ϼ��������밴ȡ����

	AUDIO_ID_CN											= 4001,	//	��ͨ��
	AUDIO_ID_CTN										= 4002,	//	����
	AUDIO_ID_EN											= 4003,	//	Ӣ��
	AUDIO_ID_PN											= 4004,	//	��������

	AUDIO_ID_BUY_TICKETS								= 4010, //	��Ʊ
	AUDIO_ID_BUY_CARDS									= 4011, //	����
	AUDIO_ID_RECHARGE									= 4012,	//	��ֵ
	AUDIO_ID_QUERY										= 4013, //	��ѯ

	AUDIO_ID_ONE_WAY_TICKET								= 4040,	//	����Ʊ
	AUDIO_ID_ROUND_TRIP_TICKET							= 4041, //	����Ʊ
	AUDIO_ID_STORED_VALUE_TICKET						= 4042, //	��ֵƱ
	AUDIO_ID_COUNT_TICKET								= 4043, //	�ƴ�Ʊ
	AUDIO_ID_PERIODICAL_TICKET							= 4044, //	����Ʊ

	AUDIO_ID_NUM_1										= 4060,	//	һ
	AUDIO_ID_NUM_2										= 4061,	//	��
	AUDIO_ID_NUM_3										= 4062,	//	��
	AUDIO_ID_NUM_4										= 4063,	//	��
	AUDIO_ID_NUM_5										= 4064,	//	��
	AUDIO_ID_NUM_6										= 4065,	//	��
	AUDIO_ID_NUM_7										= 4066,	//  ��
	AUDIO_ID_NUM_8										= 4067,	//	��
	AUDIO_ID_NUM_9										= 4068,	//	��
	AUDIO_ID_NUM_10										= 4069,	//	ʮ��ten��
	AUDIO_ID_NUM_11_EN									= 4070,	//	ʮһ(eleven)
	AUDIO_ID_NUM_12_EN									= 4071,	//	ʮ��(twelve)
	AUDIO_ID_NUM_3_EN									= 4072,	//	��(thir)
	AUDIO_ID_NUM_4_EN									= 4073,	//	��(four)
	AUDIO_ID_NUM_5_EN									= 4074,	//	��(fif)
	AUDIO_ID_NUM_TY										= 4075,	//	��ʮ(ty)
	AUDIO_ID_NUM_100									= 4076,	//	��
	AUDIO_ID_NUM_1000									= 4077,	//	ǧ
	AUDIO_ID_NUM_10000									= 4078,	//	��
	AUDIO_ID_NUM_AND									= 4079,	//	�ͣ�and��
	AUDIO_ID_NUM_POINT									= 4080, //	��
	AUDIO_ID_NUM_TEEN									= 4081, //	ʮ��teen��
	AUDIO_ID_NUM_MINUS									= 4082, //	��
	AUDIO_ID_NUM_0										= 4083, //	��

	AUDIO_ID_UNIT_YUAN									= 4020,	//	Ԫ
	AUDIO_ID_UNIT_JIAO									= 4021,	//	��
	AUDIO_ID_UNIT_FEN									= 4022,	//	��

	AUDIO_ID_TIME_YEAR									= 4400,	//	��
	AUDIO_ID_TIME_MONTH									= 4401,	//	��
	AUDIO_ID_TIME_DAY									= 4402,	//	��
	AUDIO_ID_TIME_HOUR									= 4403,	//	ʱ
	AUDIO_ID_TIME_MINUTE								= 4404,	//	��
	AUDIO_ID_TIME_SECOND								= 4405,	//	��
	AUDIO_ID_UNIT_TIMES									= 4406,	//	��

	AUDIO_ID_GUANZHA									= 4200, //	��բ
	AUDIO_ID_MACHANGDONG								= 4201, //	����
	AUDIO_ID_HEISHAHUANPARK								= 4203, //	��ɳ����԰

	AUDIO_ID_ADULT										= 4000, //	����

	AUDIO_ID_CASH_PAYMENT								= 4420, //	�ֽ�֧��
	AUDIO_ID_BANKCARD_PAYMENT							= 4421,	//	���п�֧��
	AUDIO_ID_STORED_VALUE_CARD_PAYMENT					= 4422, //	��ֵ��֧��

	AUDIO_ID_PAYMENT_DUE								= 4430,	//	Ӧ�����
	AUDIO_ID_PAYMENT_PAID								= 4431,	//	�Ѹ����
	AUDIO_ID_PAYMENT_CHANGE								= 4432,	//	������
	AUDIO_ID_PAYMENT_DEPOSIT							= 4433,	//	Ѻ��
	AUDIO_ID_PAYMENT_TAX								= 4434,	//	˰��
	AUDIO_ID_PAYMENT_TOTAL								= 4435,	//	�ϼƽ��
	AUDIO_ID_PAYMENT_RECHARGE							= 4436, //  ��ֵ���

	AUDIO_ID_HOT_SPOT_BUY_TICKET						= 4440,	//	�ȵ㹺Ʊ
	AUDIO_ID_PRICE_BUY_TICKET							= 4441, //	Ʊ�۹�Ʊ
	AUDIO_ID_STATION_BUY_TICKET							= 4442,	//	վ�㹺Ʊ

	// COMMON
	AUDIO_COMMON_PLEASE_SELECT_LANGUAGE					= 1015, //  ����ѡ��			��ͨ���밴1�������밴2��Ӣ���밴3�����������밴4
	AUDIO_COMMON_MSG_DEGRADE							= 1035, //  ������ʾ��			��ǰ��֧�ִ�ӡƾ֤�����㣬Ӳ��֧����ֽ��֧�����Ƿ����?�����밴ȷ�ϼ���ȡ���밴ȡ����
	AUDIO_COMMON_SELECT_BUSINESS						= 1016, //	ҵ��ѡ����ʾ��		��ѡ��ҵ�����ͣ�Ȼ����ȷ�ϼ�,ȡ���밴ȡ������������Ʊ�밴1�������밴2����ֵ�밴3
	AUDIO_COMMON_CALL_HELP_ON							= 1003, //  ��Ԯ��ť���£�		������Ԯ�У����Ժ󡭣������ԣ�
	AUDIO_COMMON_CALL_HELP_OFF							= 1036, //  ��Ԯ��ť����		��Ԯ��ȡ���������������
	AUDIO_COMMON_INSERT_MAX_AMOUNT						= 1037, //	�ֽ�Ͷ�볬�ޣ�		�ֽ�Ͷ�������Ѵﵽ������ơ���Ͷ����ֽ�99,999.99MOP�����˻ء����ú������ֽ�лл��
	AUDIO_COMMON_CHANGE_MAX_COUNT						= 1038, //  Ӳ�������������ޣ�	Ӳ�����㳬�^������ơ�	��Ͷ��ĬF��99,999.99MOP�����˻ء�	Ո�ú����ĬF���x�x��
	AUDIO_COMMON_CHANGE_NOT_ENOUGH						= 1039, //  ���㲻�㣺			������Ӳ�����㲻�㡣	��Ͷ��ĬF��99,999.99MOP�����˻ء�	Ո�ú����ĬF���x�x��
	AUDIO_COMMON_MSG_NOT_SUPPORT_BUSINESS				= 1049, //  ��ʾ��Ϣ��			�Բ��𣬵�ǰ��֧�ִ�ҵ��
	AUDIO_COMMON_MSG_NOT_SUPPORT_PAYMENT				= 1051, //  ��ʾ��Ϣ��			�Բ��𣬵�ǰ��֧�ִ�֧����ʽ!!!
	AUDIO_COMMON_MSG_NOT_SUPPORT_TICKET_TYPE			= 1052, //  ��ʾ��Ϣ��			�Բ��𣬵�ǰ��֧�ִ�Ʊ��!!!
	AUDIO_COMMON_MSG_INPUT_ERROR						= 1011,	//  ��ʾ��Ϣ��			������������������
	AUDIO_COMMON_MSG_TIMEOUT_CANCEL						= 1050, //  ��ʾ��Ϣ��			������ʱȡ�������ú������ֽ�лл��
	AUDIO_COMMON_MSG_INPUT_AN_ERROR_STATION				= 1058,	//  ��ʾ��Ϣ��			��վΪ��վ������������
	AUDIO_COMMON_MSG_NOT_SUPPORT_BLIND_SERVICE			= 1060,	//	��ʾ��Ϣ��			�Բ��𣬵�ǰ��֧��ä��ҵ��
	
	AUDIO_BUSINESS_SUCCESS								= 1002,	//  ���׳ɹ�
	AUDIO_BUSINESS_FAILED								= 1013,	//  ����ʧ�ܣ�����ϵ������Ա
	AUDIO_BUSINESS_CANCEL								= 1044,	//	����ȡ�����˿�
	AUDIO_BUSINESS_PRINT_SUCCESS						= 1045,	//	��ӡ���
	AUDIO_BUSINESS_CANCEL_NORMAL						= 1012,	//  ����ȡ��
	AUDIO_BUSINESS_DOING								= 1014,	//  ������...
	AUDIO_SELECT_PAYMENT								= 1031,	//	ѡ��֧����ʽ
	AUDIO_INSERT_CASH									= 1004,	//	Ͷ���ֽ�
	AUDIO_CONFORM_PAYINFO								= 1032,	//	֧��ȷ��
	AUDIO_INSERT_BANKCARD								= 1005,	//	�������п�
	AUDIO_INPUT_PASSWORD								= 1033,	//	�������п�����


	// ä�˹���	(CARD)
	AUDIO_CARD_SELECT_PRODUCT_TYPE						= 1017, //  Ʊ��ѡ��
	AUDIO_CARD_SELECT_SON_PRODUCT_TYPE_SVT				= 1029, //  ѡ���Ӳ�Ʒ:SVT
	AUDIO_CARD_SELECT_SON_PEODUCT_TYPE_MST				= 1040, //	ѡ���Ӳ�Ʒ:MST
	AUDIO_CARD_SELECT_SON_PEODUCT_TYPE_PST				= 1041, //	ѡ���Ӳ�Ʒ:PST
	AUDIO_CARD_SELECT_BEGIN_SECTION						= 1021, //  ѡ����ʼ����
	AUDIO_CARD_SELECT_END_SECTION						= 1022, //  ѡ��Ŀ������
	//AUDIO_CARD_SELECT_PIECE								= 4104, //  ѡ��������Ԥ����
	AUDIO_CARD_CONFIRM_BUSINESS_SVT						= 1030, //  ����ȷ��:SVT
	AUDIO_CARD_CONFIRM_BUSINESS_MST						= 1042,	//  ����ȷ��:MST
	AUDIO_CARD_CONFIRM_BUSINESS_PST						= 1043,	//  ����ȷ��:PST
	
	// ä�˹�Ʊ
	//AUDIO_TOKEN_SELECT_PRODUCT_TYPE						= 1018, //  Ʊ��ѡ��Ԥ����
	//AUDIO_TOKEN_SELECT_HOT_STATION						= 4201, //  ѡ���ȵ㳵վ��Ԥ����
	AUDIO_TOKEN_SELECT_END_STATION						= 1026, //  ѡ��Ŀ�ĳ�վ
	//AUDIO_TOKEN_SELECT_PIECE								= 4203, //  ѡ��������Ԥ����
	//AUDIO_TOKEN_SELECT_PAYMENT							= 4204, //  ѡ��֧����ʽ��Ԥ����
	AUDIO_TOKEN_CONFIRM_BUSINESS						= 1053, //  ����ȷ��!!!
	AUDIO_TOKEN_INSERT_BANKCARD							= 1005, //  �������п���Ԥ����
	AUDIO_TOKEN_INPUT_PASSWORD							= 1033, //  �������п����루Ԥ����
	// ä�˳�ֵ
	AUDIO_RECHARGE_FIRST_SWIPE_CARD						= 1007, //  ��һ��ˢ��  
	AUDIO_RECHARGE_FIRST_SWIPE_CARD_FOR_AVM				= 1061,	//  AVM��һ��ˢ����ʾ

	AUDIO_RECHARGE_SELECE_PRODUCT_TYPE					= 1054, //  ѡ���ֵƱ��!!!
	AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE				= 1055, //  ѡ���ֵ�Ӳ�Ʒ!!!
	AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_PST			= 1056,	//	ѡ���ֵ�Ӳ�Ʒ��PST��
	AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_MST			= 1057,	//	ѡ���ֵ�Ӳ�Ʒ��MST��
	//AUDIO_RECHARGE_SELECT_PAYMENT							= 1031, //  ѡ��֧����ʽ
	AUDIO_RECHARGE_CONFIRM_BUSINESS_SVT					= 1046, //  ����ȷ��:SVT
	AUDIO_RECHARGE_CONFIRM_BUSINESS_MST					= 1047, //  ����ȷ��:MST
	AUDIO_RECHARGE_CONFIRM_BUSINESS_PST					= 1048, //  ����ȷ��:PST
	//AUDIO_RECHARGE_INSERT_CASH							= 1004, //  Ͷ���ֽ�
	//AUDIO_RECHARGE_CONFIRM_PAYMENT						= 1032, //  ֧��ȷ��
	//AUDIO_RECHARGE_INSERT_BANKCARD						= 1005, //  �������п�
	//AUDIO_RECHARGE_INPUT_PASSWORD							= 1033, //  �������п�����
	AUDIO_RECHARGE_SECOND_SWIPE_CARD					= 1008, //  �ڶ���ˢ��
	//AUDIO_RECHARGE_CHARGING								= 1014, //  ��ֵ��...
	//AUDIO_RECHARGE_CHARGE_SUCCESS							= 4211, //  ��ֵ�ɹ�
	//AUDIO_RECHARGE_CHARGE_FAILED							= 1013, //  ��ֵʧ��
	//AUDIO_RECHARGE_CHARGE_CANCEL							= 1044, //  ��ֵȡ��
	//AUDIO_RECHARGE_PRINT_FINISHED							= 1045, //  ��ӡ���
	//AUDIO_RECHARGE_BUSINESS_CANCEL						= 1012, //  ����ȡ��
	AUDIO_RECHARGE_FAILED_READ_CARD						= 1009, //  ˢ��ʧ��
} AUDIO_ID ;

// Ӳ��ģ��ص�������
typedef enum COIN_MODULE_CMD_ID{
	COIN_MODULE_COMMAND_ID_READ_COIN_AMOUNT = 0x2000,		   // ��ȡ���ܽ������
	COIN_MODULE_COMMAND_ID_CHANGE			= 0x2001,		   // ��������
	COIN_MODULE_COMMAND_ID_ACCEPT			= 0x2002,		   // ����Ӳ������
	COIN_MODULE_COMMAND_ID_RETURN			= 0x2003		   // �˻�Ӳ������
};

// ֽ�ҽ���ģ��ص�������
typedef enum BANKNOTE_MODULE_CMD_ID{
	BANKNOTE_MODULE_COMMAND_ID_INSERT			= 0x3000,				// ʶ��ֽ������
	BANKNOTE_MODULE_COMMAND_ID_COLLECT			= 0x3001,				// ����ֽ�ң�ѹǮ�䣩����
	BANKNOTE_MODULE_COMMAND_ID_RETURN_ALL		= 0x3002,				// �˻ػ��������ݴ���ֽ������
	BANKNOTE_MODULE_COMMAND_ID_RETURN_BUFF		= 0x3003,				// �˻ػ�����ֽ������
	BANKNOTE_MODULE_COMMAND_ID_ACCEPT			= 0x3004					// ѹ�ݴ�
};

// ֽ������ģ��ص�����
typedef enum BANKNOTE_CHANGE_CALLBACK_ID{
	BANKNOTE_CHANGE_COMMAND_ID_SEND_OUT			= 0x6000,				// ֽ���ͳ�
};

// ���п�ģ��ص�������
typedef enum BANKCARD_MODULE_CMD_ID{
	BANKCARD_COMMAND_ID_READ_STATE		= 0x4000,               // ��ȡ״̬����
	BANKCARD_COMMAND_ID_RETURN			= 0x4001             // �˻����п�����
};

// PINPAD��ģ��ص�������
typedef enum PINPAD_CMD_ID{
	PINPAD_COMMAND_ID_READ_READ_STATE = 0x5000             // ��ȡ״̬
};

// �˿Ͳ忨�¼��ص�������
typedef enum CHARGE_INSERT_CARD_CMD_ID{
	CHARGE_INSERT_CARD_CMD_ID_STATUS  = 0x7000,				// ��ȡ������״̬
};

//// PINPAD��������
//typedef enum _pinpad_type_id{
//	KEY_0		= 0,			// ����0
//	KEY_1		= 1,			// ����1
//	KEY_2		= 2,			// ����2
//	KEY_3		= 3,			// ����3
//	KEY_4		= 4,			// ����4
//	KEY_5		= 5,			// ����5
//	KEY_6		= 6,			// ����6
//	KEY_7		= 7,			// ����7
//	KEY_8		= 8,			// ����8
//	KEY_9		= 9,			// ����9
//	KEY_CANCEL	= 10,			// ȡ����
//	KEY_ENTER	= 11,			// ȷ�ϼ�
//	KEY_CLEAR	= 12,			// �����
//	KEY_SPACE	= 13,			// �հ׼�
//	KEY_LEFT	= 14,			// �󰴼�
//	KEY_RIGHT	= 15,			// �Ұ���
//}PINPAD_TYPE;

// ä�˲���״̬����
typedef enum _blind_operator_status_{
	STATUS_BLIND_NORMAL					= 0x01,	// ����
	STATUS_BLIND_SELECT_LANGUAGE		= 0x02, // ����ѡ��
	STATUS_BLIND_GEDRADE_MSG			= 0x03, // �����ж�
	STATUS_BLIND_SELECT_BUSINESS		= 0x04, // ѡ��ҵ��
	STATUS_BLIND_CALL_HELP				= 0x05, // ��Ԯ��ť����
	STATUS_BLIND_CALL_HELP_END			= 0x06, // ��Ԯ��ť����
	STATUS_BLIND_INSERT_CASH_MAX		= 0x07, // Ͷ���ֽ���
	STATUS_BLIND_CHANGE_MAX_COUNT		= 0x08, // ������������
	STATUS_BLIND_CHANGE_NOT_ENOUGHT		= 0x09, // ���㲻��

	STATUS_BLIND_SELECT_PRODUCT			= 0x0A, // ѡ���Ʒ
	STATUS_BLIND_SELECT_SUB_PRODUCT		= 0x0B, // ѡ���Ӳ�Ʒ
	STATUS_BLIND_SELECT_BEGIN_SECTION	= 0x0C, // ѡ����ʼ����
	STATUS_BLIND_SELECT_END_SECTION		= 0x0D, // ѡ��Ŀ������
	STATUS_BLIND_SELECT_PRICE			= 0x0E, // ѡ������
	STATUS_BLIND_SELECT_PAYMENT			= 0x0F, // ѡ��֧����ʽ
	STATUS_BLIND_CONFIRM_BUSINESS		= 0x11, // ȷ�Ͻ�����Ϣ
	STATUS_BLIND_INSERT_CASH			= 0x12, // Ͷ���ֽ�
	STATUS_BLIND_CONFIRM_PAYMENT		= 0x13, // ȷ��֧����Ϣ
	STATUS_BLIND_INSERT_BANKCARD		= 0x14, // �������п�
	STATUS_BLIND_INPUT_PASSWORD			= 0x15, // �������п�����
	STATUS_BLIND_ISSUING				= 0x16, // ������...
	STATUS_BLIND_BUSINESS_SUCCESS		= 0x17, // ���׳ɹ�
	STATUS_BLIND_BUSINESS_FAILED		= 0x18, // ����ʧ��
	STATUS_BLIND_BUSINESS_CANCEL		= 0x19, // ����ȡ��
	STATUS_BLIND_PRINT_SUCCESS			= 0x1A, // ��ӡ�ɹ�
	STATUS_BLIND_CANCEL					= 0x1B, // ȡ������
	STATUS_BLIND_FIRST_SWIPE_CARD		= 0x1C, // ��һ��ˢ��
	STATUS_BLIND_SECOND_SWIPE_CARD		= 0x1D, // �ڶ���ˢ��
	STATUS_BLIND_SELECT_END_STATION		= 0x1E, // ѡ��Ŀ�ĳ�վ
	STATUS_BLIND_SELECT_HOT_STATION		= 0x1F, // ѡ���ȵ㳵վ
	STATUS_BLIND_OPERATION_OUT_OF_TIME	= 0x20,	// ������ʱ
	STATUS_BLIND_READ_CARD_FAILED		= 0x21,	// ����ʧ��

	// ��ǰҵ��״̬
	STATUS_BLIND_BUY_TOKEN				= 0x30,	// ��Ʊ
	STATUS_BLIND_BUY_CARD				= 0x31,	// ����
	STATUS_BLIND_RECHARGE				= 0x32,	// ��ֵ
	STATUS_BLIND_BUY_PRODUCT			= 0x33,	// ����Ʒ

	STATUS_BLIND_ON_RETURN				= 0x34,
}BLIND_OPERATION_STATUS;

/*
	ƾ�����Ͷ���
*/
typedef enum _receipt_print_type_{
	PRINT_TYPE_NONE,					// ��
	PRINT_TYPE_BALANCE,					// ����
	PRINT_TYPE_CH_CHANGE_INSTALL,		// Ӳ�������䰲װ
	PRINT_TYPE_CH_CHANGE_UNINSTALL,		// Ӳ��������ж��
	PRINT_TYPE_CH_COLLBOX_INSTALL,		// Ӳ�һ����䰲װ
	PRINT_TYPE_CH_COLLBOX_UNINSTALL,	// Ӳ�һ�����ж��
	PRINT_TYPE_CH_COLLECTION,			// Ӳ�һ���
	PRINT_TYPE_CH_SUPPLY,				// Ӳ�Ҳ���
	PRINT_TYPE_BH_INSTALL,				// ֽ�һ����䰲װ
	PRINT_TYPE_BH_UNINSTALL,			// ֽ�һ�����ж��
	PRINT_TYPE_BH_CHANGE_INSTALL,		// ֽ�������䰲װ
	PRINT_TYPE_BH_CHANGE_UNINSTALL,		// ֽ��������ж��
	PRINT_TYPE_TH_INSTALL,				// Ʊ�䰲װ
	PRINT_TYPE_TH_UNINSTALL,			// Ʊ��ж��
	PRINT_TYPE_ISSUE_ERROR,				// ��Ʊ���ϵ�
	PRINT_TYPE_CHARGE_ERROR,			// ��ֵ��Ԥ����
}RECEIPT_PRINT_TYPE;