#pragma once
#include "afx.h"
#include "SysException.h"
#include "CoinShutterExp.h"
#include "OEDUShutterDevDef.h"

/**
 @brief		Ӳ�ҽ���ģ���쳣��
*/

class CS_API CCoinShutterHardwareException :
	public CSysException
{
public:
	static const int MODULE_ID = 11802;     // ģ���

	typedef enum hardware_err_code {
		CS_ERR_NOT_INIT					= 6500,						// �Զ��壬δ��ʼ������

		// ע�⣺����Ӳ��բ��ģ��Ƚϼ򵥣��Һ�Ӳ��ģ�����ϵ�ǳ����У����Ӳ��բ�Ų�����helper��ֱ����Ӳ��ģ��helper��ʹ�ã���Ӳ��բ�ŵĴ�����ϲ���Ӳ��ģ���У�����Ӳ��ģ�����

		//CS_ERR_SENSOR					= E_EDUSHUTTER_SENSORERR,	// 6540  �ŷ�բ��Sensor����
		//CS_ERR_OPEN_FAIL				= E_EDUSHUTTER_OPENFAIL,    // 6541  ����ʧ�ܣ��Ŵ��ڹص�״̬
		//CS_ERR_OPEN_JAM					= E_EDUSHUTTER_OPENJAM,		// 6542  ����ʧ�ܣ��Ŵ��ڰ뿪������
		//CS_ERR_CLOSE_FAIL				= E_EDUSHUTTER_CLOSEFAIL,   // 6543  ����ʧ�ܣ���ﲻת���Ŵ��ڿ���״̬
		//CS_ERR_CLOSE_JAM				= E_EDUSHUTTER_CLOSEJAM,	// 6544  ����ʧ�ܣ��Ŵ��ڰ뿪����
		//CS_ERR_DOOR_JAM					= E_EDUSHUTTER_DOORJAM,		// 6545  ����ʧ�ܣ��ű����ﶥס��ȫ����
		//CS_ERR_COM						= E_EDUSHUTTER_COMMERR,		// 6546  COM�������
		//CS_ERR_INVALID_DATA				= E_EDUSHUTTER_INVALIDDATA, // 6547  ������Ч������
		//CS_ERR_ABNORMAL					= E_EDUSHUTTER_ABNORMAL,	// 6548  բ�Ŵ����쳣״̬
		//CS_ERR_TIMEOUT					= E_EDUSHUTTER_TIMEOUT,		// 6599  ��ʱ����

	}HARDWARE_ERR;	

    CCoinShutterHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CCoinShutterHardwareException();

};
