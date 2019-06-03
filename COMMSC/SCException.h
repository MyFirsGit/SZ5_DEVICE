#pragma once
#include "afx.h"
#include "SysException.h"
#include "scexp.h"

/**
@brief	��λͨ���쳣��
*/

class SC_API CSCException :
    public CSysException
{
public:
    static const int MODULE_ID = 10101;     // ģ���

	typedef enum _detail_error_code {
		SET_PARAM_ERR					= 1,	// �趨ֵ�������쳣
		FUNC_PARAM_ERR					= 2,	// ���������쳣
		SEQUENCE_ERR					= 3,	// ��������˳���쳣
		FUNC_ERR_RETURN					= 4,	// ���������쳣
		LOGIC_ERR						= 5,	// �߼��쳣
		OTHER_ERR						= 6,	// �쳣����
		CONNECTION_ERROR				= 101,  // ���Ӵ���
		NO_BIND_ERROR					= 102,  // δ����
		SEND_ERROR						= 103,  // ���������쳣
		RECEIVE_ERROR					= 104,  // ���������쳣
		DO_ERROR						= 105,  // ��������
		REC_ADDRESS_ERROR				= 107,  // ���ն˵�ַû�б�ָ��
		MES_LEN_ERROR					= 108,  // ��Ϣ�����쳣
		NET_DOWN_ERROR					= 109,  // �������
		NET_REACH_ERROR					= 110,  // ���ܵ�������
		NO_ADDRESS_ERROR				= 111,  // û�пյĻ�����
		SOCKET_CONNECT_ERROR			= 112,  // socket��������
		SOCKET_SEND_ERROR				= 113,  // socket�򿪺�������
		BIND_TIMEOUT_ERROR				= 114,  // ���ӳ�ʱ
		USER_TOO_BIG_ERROR				= 115,  // �û�����(BSD���Q)
		WIN_VER_ERROR					= 116,  // WinSock.dll�İ汾���ڷ�Χ��
		SERVICE_ERROR					= 117,  // SERVICE���ɼ�
		POST_SUB_ERROR					= 118,  // POST���ɼ���SUB�쳣
		OTHER_ERROR						= 119,  // ����WinSock ERROR        
		TIME_SYNCHRONIZE_ERROR			= 200,  // ʱ��ͬ������
		REFRESH_VERSION_ERROR			= 201,  // CD�汾�л�ʧ��
		NOT_CONNECT_SERVER_OUT_OF_TIME	= 202,	// �������ʱ�����
	} DETAIL_ERROR_CODE;


    CSCException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CSCException(void);

private:
};
