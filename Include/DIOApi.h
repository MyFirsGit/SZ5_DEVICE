#ifndef _DIO_API_H
#define _DIO_API_H

#include "Apidef.h"

//��������
#define iTYPE_2    2
//�������
#define iERROR_CODE_DIO_0						0x0000	//OK
#define iERROR_CODE_DIO_OPEN_FAIL				0x2710	// 10000 ��DIOģ�����
#define iERROR_CODE_DIO_INIT_FAIL				0x2711	// 10001 ��ʼ��DIOģ�����
#define iERROR_CODE_DIO_RESET_FAIL				0x2712	// 10002 ��λDIOģ�����
#define iERROR_CODE_DIO_PARAM_LAMP				0x2713	// 10003 ��ʾ���źű���������
#define iERROR_CODE_DIO_CONFIG_LAMP				0x2714	// 10004 ���õ��źŷ�������
#define iERROR_CODE_DIO_PARAM_SIGNAL			0x2715	// 10005 �����źű���������
#define iERROR_CODE_DIO_CONFIG_SIGNAL			0x2716	// 10006 ���ÿ����źŷ�������
#define iERROR_CODE_DIO_GET_SIGNAL				0x2AF8	// 10007 ��ȡ�����źŷ�������

#pragma pack(push, 1)

typedef enum _dio_lamp_color{
	IO_LIGHT_RED	= 0x01,
	IO_LIGHT_GREEN	= 0x02,
	IO_LIGHT_BLUE	= 0x03,
	IO_LIGHT_YELLOW = 0x04,
	IO_LIGHT_WHITE	= 0x05,
	IO_LIGHT_UNKNOWN = 0xFF,
}DIO_LAMP_COLOR;

typedef enum _dio_oper_type{
	IO_CLOSE = 0x00,
	IO_OPEN	= 0x01,

}DIO_OPER_TYPE;


typedef enum _dio_signal_type{
	IO_SIGNAL_LOW						= 0x0001,
	IO_SIGNAL_EMERGENCY					= 0x0002,
	IO_SIGNAL_TPUA_RESET				= 0x0003,
	IO_SIGNAL_TPUB_RESET				= 0x0004,
	IO_SIGNAL_RECOVERY_RESET			= 0x0005,
	IO_SIGNAL_FLAP_RESET				= 0x0006,
	IO_SIGNAL_TH_RESET					= 0x0007,
	IO_SIGNAL_BH_RECEIVING_RESET		= 0x0008,
	IO_SIGNAL_BH_CHANGE_RESET			= 0x0009,
	IO_SIGNAL_CH_RESET					= 0x000A,
	IO_SIGNAL_MAINTENANCE_DOOR1			= 0x000B,
	IO_SIGNAL_MAINTENANCE_DOOR2			= 0x000C,
	IO_SIGNAL_MAINTENANCE_DOOR3			= 0x000D,
	IO_SIGNAL_MAINTENANCE_DOOR4			= 0x000E,
	IO_SIGNAL_MAINTENANCE_DOOR5			= 0x000F,
	IO_SIGNAL_MAINTENANCE_DOOR6			= 0x0010,
	IO_SIGNAL_MAINTENANCE_DOOR7			= 0x0011,
	IO_SIGNAL_MAINTENANCE_DOOR8			= 0x0012,
	IO_SIGNAL_BUZZER					= 0x0013,
	IO_SIGNAL_HUMAN_SENSOR1				= 0x3001,
	IO_SIGNAL_HUMAN_SENSOR2				= 0x3002,
	IO_SIGNAL_HUMAN_SENSOR3				= 0x3003,
	IO_SIGNAL_RECHARGE_OPEN				= 0x3004,
	IO_SIGNAL_RECHARGE_UNLOCK			= 0x3005,
	IO_SIGNAL_RECHARGE_INPLACE			= 0x3006,
	IO_SIGNAL_COIN_SLOT					= 0x3007,
	IO_SIGNAL_FAN_SWITCH				= 0x3008,
	IO_SIGNAL_HELPBUTTON				= 0x3009,
	IO_SIGNAL_SAFETY_DOOR_SWITCH		= 0x3010,
	IO_SIGNAL_SAFETY_DOOR_INPLACE		= 0x3011,
	IO_SIGNAL_BH_RECEIVING_SECURITY_DOOR = 0x3012,
	IO_SIGNAL_BH_CHANGE_SAFETY_DOOR		= 0x3013,
	IO_SIGNAL_CH_SAFETY_DOOR			= 0x3014,
	IO_SIGNAL_BH_RECEIVING_INPLACE		= 0x3015,
	IO_SIGNAL_BH_CHANGE_INPLACE			= 0x3016,
	IO_SIGNAL_CH_INPLACE				= 0x3017,
	IO_SIGNAL_TH_INPLACE				= 0x3018,
	IO_SIGNAL_PASSENGER_PRINT_INLPACE	= 0x3019,
	IO_SIGNAL_COIN_OPERATIING_LIGHT		= 0x3020,
	IO_SIGNAL_BH_RECEIVING_LIGHT		= 0x3021,
	IO_SIGNAL_RECHARGE_LIGHT			= 0x3022,
	IO_SIGNAL_PRINT_LIGHT				= 0x3023,
	IO_SIGNAL_PICKUPTICKET_INDICATOR	= 0x3024,
	IO_SIGNAL_BANKCARD_LIGHT			= 0x3025,
	IO_SIGNAL_COIN_COLECT_BOX_READY		= 0x3026,
	IO_SIGNAL_PICKUPTICKET_LITGHT		= 0x3027,
}IO_SIGNAL_TYPE;

// DIOģ��״̬�ṹ
struct STDioDevReturn {
	unsigned int uiProviderId;	// ģ�鳧�ұ�ʶ
	int iType;					// �������ͣ�0-������1-���棻2-����
	int iStdErrorCode;			//��׼�������
	int iErrorCode;				// �������
	char chErrorCode[128];   // Ӳ��������Ϣ
	char acReverse[128];   //Ԥ����Ϣ
};
// ��ʾ���ź����ýṹ��
struct STDioLampConfig {
	unsigned short usLampType;	// ��Ҫ���õ���ʾ������
	unsigned char ucLampColor;	// ��ʾ����ɫ:
								// 1�����(����/�����)
								// 2���̵�
								// 3������
								// 4���Ƶ�
								// 5���׵�
	unsigned char ucOperType;	// �������ͣ�0x00���رյƣ�0x01���򿪵�
	unsigned char ucBlinkTimes; // ��˸��������������Ϊ0x01ʱ��Ч����
								// 0x00������˸��
								// �� 0x00����˸����
} ;
// �����ź����ýṹ��
struct STDioSignalConfig {
	unsigned short usSignalType;  // �����ź�����
	unsigned char ucOperType;     // �������ͣ�
								  // 0x00���ر��ź�
								  // 0x01�����ź�
} ;
// �汾��Ϣ�ṹ��
struct STDioVerInfo {
	unsigned char pFirmwareVerInfo[ 32 ];  //�̼��汾��Ϣ, 32 �ֽڣ�������0x00
	unsigned char pDriverVerInfo[ 32 ];    //�����汾��Ϣ, 32 �ֽڣ�������0x00
	unsigned char pHardwareVerInfo[ 32 ];  //Ӳ���汾��Ϣ, 32 �ֽڣ�������0x00
} ;

// ά����DIO��Ϣ(0:ά���ſ�; 1:ά���Ź�)
struct STDioMaintainDoor {
	// ά��������
	unsigned char ucDoorNumber;
	// ά����DIO��Ϣ
	unsigned char ucDoorSignal[ 16 ];
};

// DIOģ�鴮�����ýṹ��
struct tSerialConfigInfo {
	int chSerialPort;	// ���ں�
	int nIoSpeed;		// ������
	int nStopbits;		// ֹͣλ
	int nDatabits;		// ����λ
	int nParity;		// У��λ
};
typedef enum _tem_io_rsp_code {
	IO_RSP_OK = 0x0000,    // �����ɹ�
	IO_RSP_NG = 0x0001,    // ����ʧ��
}IO_RSP_CODE;
// ȡIO״̬���ؽ������
typedef struct _io_rsp_status_info
{
	bool isFrontDoorOpen;									// ǰά�����Ƿ��
	bool isBackDoorOpen;									// ��ά�����Ƿ�λ
	bool isBanknoteModeReady;								// ֽ��ģ���Ƿ�λ
	bool isCoinModeReady;									// Ӳ��ģ���Ƿ�λ
	bool isTokenColBoxReady;								// Token�����䵽λ
	bool isTokenInvBoxReady;								// Token��Ʊ�䵽λ
	bool isLeftPersonActive;								// ����Ƿ�����
	bool isRightPersonActive;								// �Ҳ��Ƿ�����
	bool isLeftShockActive;									// ���񶯴������Ƿ񴥷�
	bool isRightShockActive;								// ���񶯴������Ƿ񴥷�
	bool isAssittentButtonPressed;							// ��Ԯ��ť�Ƿ���
	bool isCardModeReady;									// Ʊ��ģ���Ƿ�λ
	bool isCoinboxReady;									// Ӳ�һ������Ƿ�λ
	bool isPrintModeReady;									// �˿ʹ�ӡ���Ƿ�λ
	bool isRechargeCardReady;								// ��ֵ����λ
	_io_rsp_status_info()
	{
		isFrontDoorOpen = false;							// ǰά�����Ƿ��
		isBackDoorOpen = true;
		isBanknoteModeReady= true;							// ֽ��ģ���Ƿ�λ
		isCoinModeReady = true;
		isTokenColBoxReady = false;
		isTokenInvBoxReady = false;
		isLeftPersonActive= false;							// ����Ƿ�����
		isRightPersonActive= false;							// �Ҳ��Ƿ�����
		isLeftShockActive = false;							// ���񶯴������Ƿ񴥷�
		isRightShockActive= false;							// ���񶯴������Ƿ񴥷�
		isAssittentButtonPressed= false;					// ��Ԯ��ť�Ƿ���
		isCardModeReady= true;								// Ʊ��ģ���Ƿ�λ
		isCoinboxReady = true;
		isPrintModeReady = true;
		isRechargeCardReady = true;
	}
} IO_RSP_STATUS_INFO, *LPIO_RSP_IO_INFO;
#pragma pack(pop)

extern "C" {
	// >>>> ======= ��׼�ӿڶ��� =======
	/**
	* DIOģ���
	*  uiCommPort - ���ں�(�Ǵ��ڵ�DIOģ��������)
	*  uiBaudRate - ����ͨѶ�Ĳ�����(�Ǵ��ڵ�DIOģ��������)
	* ����ֵ:
	*  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	*/
	DLLIMP int DIO_Open( int nNum, tSerialConfigInfo* pSerialConfigInfo );
	/**
	 * ��ʼ��ģ�飬���ģ���Ƿ����
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
	DLLIMP int DIO_Init( STDioDevReturn& tReturn );
	/**
	 * DIOģ��ر�
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
	DLLIMP int DIO_Close();

	/**
	 * ������ʾ���ź�
	 *  pLampConfig - ��Ҫ���õĵ���Ϣ
	 *  ucLampCnt - ��Ҫ������ʾ�Ƶ�������tDioLampConfig�ĸ�����
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
	DLLIMP int DIO_SetLampSignal( STDioLampConfig* pLampConfig, unsigned char ucLampCnt, STDioDevReturn& tReturn );
	/**
	 * ������ʾ���ź�
	 *  pSignalConfig - ��Ҫ���õĿ����ź�
	 *  ucSignalCnt - ��Ҫ���õĿ����źŵ�������tDioSignalConfig�ĸ�����
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
	DLLIMP int DIO_SetSwitchSignal( STDioSignalConfig* pSignalConfig, unsigned char ucSignalCnt, STDioDevReturn& tReturn );
	/**
	 * ������ʾ���ź�
	 *  pSignalConfig - ��Ҫ���صĿ����ź����ýṹ����Ҫ�����߿��ٷ��������ڴ�ռ䣩
	 *  ucSignalCnt - ���ؿ����źŵ�����������tDioSignalConfig�ĸ����������������������������
						����ֵΪ0ʱ���������еģ����÷�Ӧ���ϵ���ǰΪpSignalConfig�����㹻�Ŀռ䣩
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
	DLLIMP int DIO_GetSwitchSignal( STDioSignalConfig* pSignalConfig, unsigned char& ucSignalCnt, STDioDevReturn& tReturn );
	/**
	 * ��ȡģ��̼��汾
	 *  tVersion - ģ��̼��汾��Ϣ
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
	DLLIMP int DIO_GetVersion( STDioVerInfo& tVersion, STDioDevReturn& tReturn );
	// <<<<
}



#endif
