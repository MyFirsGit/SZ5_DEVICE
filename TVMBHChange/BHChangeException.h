#pragma once
#include "afx.h"
#include "SysException.h"
#include "BHexp.h"
#include "BHChangeLib.h"

/**
 @brief		ֽ������ģ���쳣��
*/

class BH_API CBHChangeException : public CSysException
{
public:
	static const int MODULE_ID = 13202;     // ģ���

	typedef enum _detail_error_code {
		SET_PARAM_ERR				= 1001,	// �趨ֵ�������쳣
		FUNC_PARAM_ERR				= 1002,	// ���������쳣
		SEQUENCE_ERR				= 1003,	// ��������˳���쳣
		FUNC_ERR_RETURN				= 1004,	// ���������쳣
		LOGIC_ERR					= 1005,	// �߼��쳣
		OTHER_ERR					= 1006,	// �쳣����

		ERROR_BH_OTHER				= 1010,	// �����쳣
		ERROR_BH_OPEN_FAILED		= 1011,	// ��ʱ	���ڴ�ʧ��
		ERROR_BH_ALREADY_OPENED		= 1012,	// ��ʱ	�����ظ���
		ERROR_BH_CLOSE_FAILED		= 1013,	// �ر�ʱ	���ڹر�ʧ��
		ERROR_BH_ALREADY_CLOSED		= 1014,	// �ر�ʱ	�����ظ��ر�
		ERROR_BH_NOT_OPEN			= 1015,	// ͨ��ʱ	����δ��
		ERROR_BH_CMD				= 1016,	// ͨ��ʱ	�����쳣
		ERROR_BH_SEND				= 1017,	// ͨ��ʱ	�����쳣
		ERROR_BH_RECEIVE			= 1018,	// ͨ��ʱ	�����쳣
		ERROR_BH_DESERIALIZE		= 1019,	// ͨ��ʱ	�����ظ���Ϣ�쳣
		ERROR_BH_RECEIVE_TIMEOUT	= 1020,	// ͨ��ʱ	���ճ�ʱ
		ERROR_BH_SEQUNCE_ERR		= 1231,	// ����ʱ�����
	} DETAIL_ERROR_CODE;

    CBHChangeException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CBHChangeException();

private:
};
