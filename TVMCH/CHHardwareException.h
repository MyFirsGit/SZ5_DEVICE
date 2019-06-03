#pragma once
#include "afx.h"
#include "SysException.h"
#include "CHexp.h"
#include "CAS002DevDef.h"
/**
@brief	CHӲ���쳣��
*/
class CH_API CCHHardwareException : public CSysException
{

public:
	static const int MODULE_ID = 11801;     // ģ���
	typedef enum hardware_err_code {
		// �Զ���Ӳ������
		ERROR_CH_COLLECT_BOX_A_NOT_OPEN			= 57,	// Ӳ�һ����䲻��λ
		//ERROR_CH_COLLECT_BOX_B_NOT_OPEN			= 58,	// Ӳ�һ�����B�ǰ�δ��
		ERROR_CH_MODULE_NOT_STANDBY				= 200,	// Ӳ��ģ��û�е�λ
		ERROR_CH_CHANGE_BOX_A_NOT_INSTALL		= 201,	// ������Aû�а�װ
		ERROR_CH_CHANGE_BOX_B_NOT_INSTALL		= 202,	// ������Bû�а�װ
		ERROR_CH_CHANGE_CHANGE_FAILED			= 203,	// ����ʧ��
		ERROR_CH_BEGIN_INSERT_COMMOND_ERR		= 300,	// ��ʼ����Ӳ���������Ӳ�ҽ����б�Ϊ�գ�

		// �����Դ���
		CH_ERR_BASE						= S_CAS_BASE,					// �������ֵ
		CH_ERR_WARNNING_BASE			= W_CAS_BASE,					// 17005 �������ֵ
		CH_ERR_HOPPER1_COUNT			= W_CAS_MHOPPER1_COUNT_ERROR,	// 17006 Hopper1�����������						(���ʧ��)
		CH_ERR_HOPPER2_COUNT			= W_CAS_MHOPPER2_COUNT_ERROR,	// 17007 Hopper2�����������						(���ʧ��)
		CH_ERR_HOPPER_COUNT				= W_CAS_MHOPPER_COUNT_ERROR,	// 17008 ��Hopper�����������						(���ʧ��)

		// ���ش���
		CH_ERR_ERROR_BASE				= E_CAS_BASE,					// ���ش���ֵ
		CH_ERR_COMMAND					= E_CAS_COMMAND_ERROR,			// 17011 ��Ч����
		CH_ERR_NO_CHANGE				= E_CAS_NO_CHANGE,				// 17012 ���㣬����ʧ��							(�����������㹦��)
		CH_ERR_ESCROW1_MIDDLE			= E_CAS_ESCROW1_MIDDLE,			// 17013 �ݴ���1�Ҳ����м�λ��						(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ESCROW1_LEFT				= E_CAS_ESCROW1_LEFT,			// 17014 �ݴ���1�Ҳ������λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ESCROW1_RIGHT			= E_CAS_ESCROW1_RIGHT,			// 17015 �ݴ���1�Ҳ����ұ�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ESCROW2_MIDDLE			= E_CAS_ESCROW2_MIDDLE,			// 17016 �ݴ���2�Ҳ����м�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ESCROW2_LEFT				= E_CAS_ESCROW2_LEFT,			// 17017 �ݴ���2�Ҳ������λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ESCROW2_RIGHT			= E_CAS_ESCROW2_RIGHT,			// 17018 �ݴ���2�Ҳ����ұ�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ROUTER1_LEFT				= E_CAS_ROUTER1_LEFT,			// 17019 ������1�Ҳ������λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ROUTER1_RIGHT			= E_CAS_ROUTER1_RIGHT,			// 17020 ������1�Ҳ����ұ�λ��  					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ROUTER2_LEFT				= E_CAS_ROUTER2_LEFT,			// 17021 ������2�Ҳ������λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ROUTER2_RIGHT			= E_CAS_ROUTER2_RIGHT,			// 17022 ������2�Ҳ����ұ�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ROUTER1_MIDDLE			= E_CAS_ESCROW1_MIDDLE,			// 17023 ������1�Ҳ����м�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_ROUTER2_MIDDLE			= E_CAS_ESCROW2_MIDDLE,			// 17024 ������2�Ҳ����м�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
		CH_ERR_HOPPER1_DISP				= E_CAS_MHOPPER1_DISP,			// 17025 A������������ʧ��       					(����ͣ��ά���������������㹦��)
		CH_ERR_HOPPER1_JAM				= E_CAS_MHOPPER1_JAM,			// 17026 A������������           					(����ͣ��ά���������������㹦��)
		CH_ERR_HOPPER2_DISP				= E_CAS_MHOPPER2_DISP,			// 17027 B������������ʧ��       					(����ͣ��ά���������������㹦��)
		CH_ERR_HOPPER2_JAM				= E_CAS_MHOPPER2_JAM,			// 17028 B������������           					(����ͣ��ά���������������㹦��)
		CH_ERR_ACCEPT_FAULT				= E_CAS_COINACCEPTOR_FAULT,		// 17033 Ӳ�ҽ���������          					(����ͣ��ά�������������ձҹ���)

		CH_ERR_IBUTTON1					= E_CAS_IBUTTON1,				// 17034 �ӱ���1iButton����/RFID����  				(����ͣ��ά��)
		CH_ERR_IBUTTON2					= E_CAS_IBUTTON2,				// 17035 �ӱ���2iButton����/RFID����  				(����ͣ��ά��)
		CH_ERR_WRITE_FLASH				= E_CAS_WRITE_FLASH,			// 17036 ���FLASHʧ��           					(����ͣ��ά��)
		CH_ERR_FILE_NOTFINISH			= E_CAS_FILE_NOTFINISH,			// 17037 �ļ�δ������            					(����ͣ��ά��)
		CH_ERR_WRITE_LOG				= E_CAS_WRITE_LOG,				// 17038 дLogʧ��               					(Ԥ��)
		CH_ERR_READ_LOG					= E_CAS_READ_LOG,				// 17039 ��Logʧ��               					(Ԥ��)
		CH_ERR_CLEAN_FAIL				= E_CAS_CLRCOIN_FAIL,			// 17040 ���ʧ��                					(����ͣ��ά������Ӱ������ͽ���Ӳ�ҹ���)
		CH_ERR_CAM_JAM					= E_CAS_CAM_JAM,				// 17041 ͹�ֶ�ת                					(����ͣ��ά��)
		CH_ERR_CAM_MOTOR				= E_CAS_CAM_MOTOR,				// 17042 ͹��������            					(����ͣ��ά��)
		CH_ERR_COININ_JAM				= E_CAS_COININ_JAM,				// 17043 Ͷ�ҿڿ���              					(����ͣ��ά�������������ձҹ���)
		CH_ERR_EJECT_SIGNAL				= E_CAS_EJECT_SIGNAL,			// 17044 Ӳ�ҽ������˱��ź��쳣  					(����ͣ��ά�������������ձҹ���)
		CH_ERR_ESCROW1_SENSOR			= E_CAS_ESCROW1_SENSOR,			// 17045 �ݴ���1����������       					(����ͣ��ά��)
		CH_ERR_SCROW2_SENSOR			= E_CAS_ESCROW2_SENSOR,			// 17046 �ݴ���2����������       					(����ͣ��ά��)
		CH_ERR_ROUTER1_SENSOR			= E_CAS_ROUTER1_SENSOR,			// 17047 ������1����������       					(����ͣ��ά��)
		CH_ERR_ROUTER2_SENSOR			= E_CAS_ROUTER2_SENSOR,			// 17048 ������2����������       					(����ͣ��ά��)
		CH_ERR_EEPROM					= E_CAS_EEPROM,					// 17049 EEPROM����              					(����ͣ��ά��)
		CH_ERR_COINACCEPTOR_TIMEOUT		= E_CAS_COINACCEPTOR_TIMEOUT,	// 17050 Ӳ�ҽ�������ʱ          					(����ͣ��ά�������������ձҹ���)
		CH_ERR_COINACCEPTOR_DATA		= E_CAS_COINACCEPTOR_DATA,		// 17051 Ӳ�ҽ������������ݴ�    					(����ͣ��ά�������������ձҹ���)
		CH_ERR_DEV_BUSY					= E_CAS_DEV_BUSY,				// 17052 �豸æ                  
		CH_ERR_IBUTTON_ERROR			= E_CAS_IBUTTON_ERROR,			// 17053 Ǯ��iButton����/RFID����					(����ͣ��ά��)
		CH_ERR_COINOUT_JAM				= E_CAS_COINOUT_JAM,			// 17054 Ͷ�ҳ��ڿ���            					(����ͣ��ά�������������ձҹ���)
		CH_ERR_MODULE_ERROR				= E_CAS_MODULE_ERROR,			// 17055 ģ��λ���쳣          						(����ͣ��ά��)
		CH_ERR_HOPPER1_POS				= E_CAS_HOPPER1_POS,			// 17056 HOPPER1����λ         						(����ͣ��ά��)
		CH_ERR_HOPPER2_POS				= E_CAS_HOPPER2_POS,			// 17057 HOPPER2����λ           					(����ͣ��ά��)
		CH_ERR_COINBOX_POS				= E_CAS_COINBOX_POS,			// 17058 Ǯ�䲻��λ              					(����ͣ��ά��)
		CH_ERR_MODULE_POS				= E_CAS_MODULE_POS,				// 17059 ģ�鲻��λ              					(����ͣ��ά��)
		CH_ERR_IMAGE_ERROR				= E_CAS_IMAGE_ERROR,			// 17060 ͼ��ʶ��ģ�����        					(����ͣ��ά��)

		CH_ERR_RFID_REQUEST				= E_CAS_RFID_REQUEST,			// 17061 ��Ӧ������Ч����/��ģ��δ����				(����ҵ������������ϵȼ�)
		CH_ERR_RFID_AUTHEN				= E_CAS_RFID_AUTHEN,			// 17062 ��Ӧ������֤ʧ��							(����ҵ������������ϵȼ�)
		CH_ERR_RFID_PARAMETER			= E_CAS_RFID_PARAMETER,			// 17063 ��дRFIDʱ������							(����ҵ������������ϵȼ�)
		CH_ERR_READ_FILE				= E_CAS_READ_FILE,				// 17066 ���ļ�ʧ��									(����ͣ��ά��)
		CH_ERR_FILE_LENGTH				= E_CAS_FILE_LENGTH,			// 17067 �ļ�̫��									(����ͣ��ά��)
		CH_ERR_OPEN_FILE				= E_CAS_OPEN_FILE,				// 17068 �ļ���ʧ��								(����ͣ��ά��)
		CH_ERR_UPDATEDATA_EMPTY			= E_CAS_UPDATEDATA_EMPTY,		// 17069 �޷�ִ�и��£��޸�������
		CH_ERR_COINBOXFULL				= E_CAS_COINBOXFULL,			// 17073 Ӳ�һ���Ǯ����
		CH_ERR_VOLTAGE_LACK				= E_CAS_VOLTAGE_LACK,			// 17074 ��ѹ���� 
		CH_ERR_HOPPER1_INTERVENE		= E_CAS_MHOPPER1_INTERVENE,		// 17075 ������1���ҹ����д�����������(��ǰ���׳��Ҽ�����׼)
		CH_ERR_HOPPER1_LACK				= E_CAS_MHOPPER1_LACK,			// 17076 ������1���ҹ����п����ٳ��� (��ǰ���׳��Ҽ�����׼) 
		CH_ERR_HOPPER2_INTERVENE		= E_CAS_MHOPPER2_INTERVENE,		// 17077 ������2���ҹ����д����������� (��ǰ���׳��Ҽ�����׼)
		CH_ERR_HOPPER2_LACK				= E_CAS_MHOPPER2_LACK,			// 17078 ������2���ҹ����п����ٳ��� (��ǰ���׳��Ҽ�����׼)

		CH_ERR_UNSUPPORT				= E_CAS_UNSUPPORT,				// 17079 �豸��֧�ֵĹ���							(ĳЩ�ӿڽ�����ض��ͺţ����ڲ�֧�ֵ��ͺŻ᷵�ش˴���)
		CH_ERR_RTRBOX2_RFID				= E_CAS_RTRBOX2_RFID,			// 17081 ������2 iButton/RFID ����
		CH_ERR_GATE_OPEN				= E_CAS_GATE_OPEN,				// 17082 բ�Ŵ�ʧ��
		CH_ERR_GATE_CLOSE				= E_CAS_GATE_CLOSE,				// 17083 բ�Źر�ʧ��
		CH_ERR_UNKNOWN					= E_CAS_UNKNOWN,				// 17090 δ֪��
		CH_ERR_PARAM_ERROR				= E_CAS_PARAM_ERROR,			// 17091 ��������ȷ 
		CH_ERR_DEV_CFG					= E_CAS_DEV_CFG,				// 17092 ��д�豸���ô�                
		CH_ERR_COMM_CFG					= E_CAS_COMM_CFG,				// 17093 �������ô� 
		CH_ERR_RECEIVE_NAK				= E_CAS_RECEIVE_NAK,			// 17094 �յ�NAK,���������д�    
		CH_ERR_REVDATA_ERROR			= E_CAS_REVDATA_ERROR,			// 17095 ���յ�����������        
		CH_ERR_SENDENQ_ERROR			= E_CAS_SENDENQ_ERROR,			// 17096 ����ENQ����             
		CH_ERR_SENDCMD_ERROR			= E_CAS_SENDCMD_ERROR,			// 17097 ���������              
		CH_ERR_WAITACK_TIMEOUT			= E_CAS_WAITACK_TIMEOUT,		// 17098 �ȴ�ACK��ʱ             
		CH_ERR_WAITRSP_TIMEOUT			= E_CAS_WAITRSP_TIMEOUT,		// 17099 �ȴ���Ӧ��ʱ    


		// ע�⣺
		// ����Ӳ��բ��ģ��Ƚϼ򵥣��Һ�Ӳ��ģ�����ϵ�ǳ����У����Ӳ��բ�Ų�����helper��ֱ����Ӳ��ģ��helper��ʹ�ã���Ӳ��բ�ŵĴ�����ϲ���Ӳ��ģ���У�����Ӳ��ģ�����
		// ������Ӳ��բ��ģ��Ĵ����롣
		CS_ERR_NOT_INIT					= 6500,		// �Զ��壬δ��ʼ������
		CS_ERR_SENSOR					= 6540,		// 6540  �ŷ�բ��Sensor����
		CS_ERR_OPEN_FAIL				= 6541,		// 6541  ����ʧ�ܣ��Ŵ��ڹص�״̬
		CS_ERR_OPEN_JAM					= 6542,		// 6542  ����ʧ�ܣ��Ŵ��ڰ뿪������
		CS_ERR_CLOSE_FAIL				= 6543,		// 6543  ����ʧ�ܣ���ﲻת���Ŵ��ڿ���״̬
		CS_ERR_CLOSE_JAM				= 6544,		// 6544  ����ʧ�ܣ��Ŵ��ڰ뿪����
		CS_ERR_DOOR_JAM					= 6545,		// 6545  ����ʧ�ܣ��ű����ﶥס��ȫ����
		CS_ERR_COM						= 6546,		// 6546  COM�������
		CS_ERR_INVALID_DATA				= 6547,		// 6547  ������Ч������
		CS_ERR_ABNORMAL					= 6548,		// 6548  բ�Ŵ����쳣״̬
		CS_ERR_TIMEOUT					= 6599,		// 6599  ��ʱ����

	}HARDWARE_ERR;	//������ϸ���ο��ĵ�

	CCHHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CCHHardwareException(void);

private:
};