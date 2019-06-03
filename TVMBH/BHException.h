#pragma once
#include "afx.h"
#include "SysException.h"
#include "BHexp.h"

/**
 @brief		ֽ�ҽ���ģ���쳣��
*/

class BH_API CBHException :
	public CSysException
{
public:
	static const int MODULE_ID = 11702;     // ģ���

	typedef enum _detail_error_code {
		SET_PARAM_ERR				= 1,	// �趨ֵ�������쳣
		FUNC_PARAM_ERR				= 2,	// ���������쳣
		SEQUENCE_ERR				= 3,	// ��������˳���쳣
		FUNC_ERR_RETURN				= 4,	// ���������쳣
		LOGIC_ERR					= 5,	// �߼��쳣
		OTHER_ERR					= 6,	// �쳣����

		ERROR_BH_OTHER				= 10,	// �����쳣
		ERROR_BH_OPEN_FAILED		= 11,	// ��ʱ	���ڴ�ʧ��
		ERROR_BH_ALREADY_OPENED		= 12,	// ��ʱ	�����ظ���
		ERROR_BH_CLOSE_FAILED		= 13,	// �ر�ʱ	���ڹر�ʧ��
		ERROR_BH_ALREADY_CLOSED		= 14,	// �ر�ʱ	�����ظ��ر�
		ERROR_BH_NOT_OPEN			= 15,	// ͨ��ʱ	����δ��
		ERROR_BH_CMD				= 16,	// ͨ��ʱ	�����쳣
		ERROR_BH_SEND				= 17,	// ͨ��ʱ	�����쳣
		ERROR_BH_RECEIVE			= 18,	// ͨ��ʱ	�����쳣
		ERROR_BH_DESERIALIZE		= 19,	// ͨ��ʱ	�����ظ���Ϣ�쳣
		ERROR_BH_RECEIVE_TIMEOUT	= 20,	// ͨ��ʱ	���ճ�ʱ
		ERROR_BH_SEQUNCE_ERR		= 231,	// ����ʱ�����
	} DETAIL_ERROR_CODE;

    CBHException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CBHException();

private:
};
