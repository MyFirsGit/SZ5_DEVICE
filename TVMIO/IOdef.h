#pragma once

#define MAX_RSP_LEN							256   // ���ջ�����󳤶�
#define MAX_SEND_ITEMS_COUNT		10		 // ���Ͷ������Ԫ�ظ���
#define MAX_RECEIVE_ITEMS_COUNT	10		 // ���ն������Ԫ�ظ���

// ״̬��Ӧbitλ
//��һ�ֽ�
#define IO_STS_BACKDOOR						0x01//���ŵ�λ
#define IO_STS_FRONTDOOR					0x04//ǰ�ŵ�λ
#define IO_STS_BANKNOTE_SENSOR		        0x10//ֽ��ģ�鵽λ
#define IO_STS_TOKEN_COLBOX_SENSOR			0x20//Token�����䵽λ
#define IO_STS_T_U_SENSOR					0x40//T-U Sensor
#define IO_STS_TOKEN_INVBOX_SENSOR			0x80//Token��Ʊ�䵽λ
//�ڶ��ֽ���ʱ����
#define IO_STS_DETECT_RIGHT					0x02//
#define IO_STS_DETECT_LEFT					0x01
#define IO_STS_ASSISTENT_BTN				0x04
#define IO_STS_SHOCK_SENSOR_RIGHT           0x08  
#define IO_STS_SHOCK_SENSOR_LEFT		    0x00 //����
//�����ֽ�
#define IO_STS_B_U_SENSOR					0x02//B-USensor
#define IO_STS_PERSON_NEAR_SENSOR			0x04//����ӽ���Ӧ

// IO������붨��
typedef enum _io_command_code {
	IO_COMMAND_CODE_INIT									= 0x4C,   // L	��ʼ��������I/O��
	IO_COMMAND_CODE_GET_STATUS						= 0x49,   // I		ȡ����ڡ������״̬
	IO_COMMAND_CODE_SET_OUTPUT_ON_OFF		= 0x4F,   // O	���õ�·���״̬��������
	IO_COMMAND_CODE_SET_OUTPUT_FLASH			= 0x52,   // R	���õ�·���״̬����˸��
	IO_COMMAND_CODE_SET_12OUTPUT					= 0x50,   // P	����12·���״̬�����������˸��
	IO_COMMAND_CODE_GET_VERSION					= 0x56,   // V	ȡ���ʳ���汾��
	IO_COMMAND_CODE_IO_BOARD_TYPE				= 0x54    // T	ȡIO���ͺ�
} IO_COMMAND_CODE;

// IO���������峤�ȶ���
typedef enum _io_command_len {
	IO_COMMAND_LEN_INIT								= 0x01,   // L   ��ʼ��������I/O�������
	IO_COMMAND_LEN_GET_STATUS					= 0x01,   // I	ȡ����ڡ������״̬�����
	IO_COMMAND_LEN_SET_OUTPUT_ON_OFF	= 0x03,   // O	���õ�·���״̬�������������
	IO_COMMAND_LEN_SET_OUTPUT_FLASH		= 0x04,   // R	���õ�·���״̬����˸�������
	IO_COMMAND_LEN_SET_12OUTPUT				= 0x03,   // P	����12·���״̬�����������˸�������
	IO_COMMAND_LEN_GET_VERSION					= 0x01,   // V	ȡ���ʳ���汾�������
	IO_COMMAND_LEN_IO_BOARD_TYPE				= 0x01    // T	ȡIO���ͺ������
} IO_COMMAND_LEN;

// IO����˿ںŶ���
typedef enum _io_output_port_no {
	IO_OUTPUT_PORT_NO_CH_INDICATOR										= 0x31,	  // Ͷ�ҿ�ָʾ��
	IO_OUTPUT_PORT_NO_RETURN_CUP_INDICATOR								= 0x32,   // ��Ʊ��ָʾ��
	IO_OUTPUT_PORT_NO_RETURN_CUP_LIGHT									= 0x33,   // ��Ʊ��������
	IO_OUTPUT_PORT_NO_CARDFFLE_INDICATOR								= 0x35,	  // ��ֵ��ָʾ��
	IO_OUTPUT_PORT_NO_BAFFLE_B											= 0x39,	  // B�����
	IO_OUTPUT_PORT_NO_OPERATION_LIGHT									= 0x41,	  // ����������
	IO_OUTPUT_PORT_NO_BAFFLE_T											= 0x42,	  // T�����
	IO_OUTPUT_PORT_NO_ALARM												= 0x43,   // ������
	IO_OUTPUT_PORT_NO_HELP_BTN_LIGHT									= 0x00,	  // ��Ԯ��ť��(ͣ��)
} IO_OUTPUT_PORT_NO;

// IO����źŶ���
typedef enum _io_output_value {
	IO_OUTPUT_ON								= 0x31,   // �������1��
	IO_OUTPUT_OFF								= 0x30,   // ����أ�0��
	IO_OUTPUT_LIGHT_SLOW						= 0x73,	  // ������˸
	IO_OUTPUT_LIGHT_MID							= 0x6D,	  // ������˸
	IO_OUTPUT_LIGHT_FAST						= 0x66,	  // ������˸
	IO_OUTPUT_LIGHT_STOP						= 0x4F,	  // ��·ֹͣ��˸
} IO_OUTPUT_VALUE;

// IO״̬�붨��
typedef enum _io_state_value {
	IO_STATE_SUCCEED								= 0x73,    // �ɹ�
	IO_STATE_WARNING								= 0x77,    // ����
	IO_STATE_ERROR									= 0x65    // ����
} IO_STATE_VALUE;

