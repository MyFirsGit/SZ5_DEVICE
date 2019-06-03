#pragma once
#include "afx.h"
#include "SysException.h"
#include "LPTExp.h"

/**
@brief	��ӡ���쳣��
*/

class LPT_API CLPTHardwareException :
	public CSysException
{
public:
	static const int MODULE_ID = 29994;// ģ���

	typedef enum _detail_error_code {
		SET_PARAM_ERR               = 1,	// �趨ֵ�������쳣
		FUNC_PARAM_ERR              = 2,	// ���������쳣
		SEQUENCE_ERR	            = 3,	// ��������˳���쳣
		FUNC_ERR_RETURN	            = 4,	// ���������쳣
		LOGIC_ERR	                = 5,	// �߼��쳣
		OTHER_ERR	                = 6,	// �쳣����
	} DETAIL_ERROR_CODE;

	CLPTHardwareException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CLPTHardwareException(void);
};
