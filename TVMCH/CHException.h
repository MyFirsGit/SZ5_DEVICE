#pragma once
#include "afx.h"
#include "SysException.h"
#include "CHexp.h"

/**
 @brief		Ӳ�ҽ���ģ���쳣��
*/

class CH_API CCHException :
	public CSysException
{
public:
	static const int MODULE_ID = 11802;     // ģ���

	typedef enum _detail_error_code {
		SET_PARAM_ERR				= 1,	// �趨ֵ�������쳣
		FUNC_PARAM_ERR				= 2,	// ���������쳣
		SEQUENCE_ERR				= 3,	// ��������˳���쳣
		FUNC_ERR_RETURN				= 4,	// ���������쳣
		LOGIC_ERR					= 5,	// �߼��쳣
		OTHER_ERR					= 6,	// �쳣����

		ERROR_CH_OTHER				= 10,	// �����쳣
		ERROR_CH_OPEN_FAILED		= 11,	// ��ʱ	���ڴ�ʧ��
		ERROR_CH_ALREADY_OPENED		= 12,	// ��ʱ	�����ظ���
		ERROR_CH_CLOSE_FAILED		= 13,	// �ر�ʱ	���ڹر�ʧ��
		ERROR_CH_ALREADY_CLOSED		= 14,	// �ر�ʱ	�����ظ��ر�
		ERROR_CH_NOT_OPEN			= 15,	// ͨ��ʱ	����δ��
		ERROR_CH_CMD				= 16,	// ͨ��ʱ	�����쳣
		ERROR_CH_SEND				= 17,	// ͨ��ʱ	�����쳣
		ERROR_CH_RECEIVE			= 18,	// ͨ��ʱ	�����쳣
		ERROR_CH_DESERIALIZE		= 19,	// ͨ��ʱ	�����ظ���Ϣ�쳣
		ERROR_CH_RECEIVE_TIMEOUT	= 20,	// ͨ��ʱ	���ճ�ʱ
		ERROR_CH_SEQUNCE_ERR		= 231,	// ����ʱ�����
		ERROR_CH_STOP_READ_AMOUNT	= 232,	// ֹͣ��ȡ���
	} DETAIL_ERROR_CODE;

    CCHException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CCHException();

private:
};
