#pragma once
#include "afx.h"
#include "SysException.h"
#include "coreexp.h"

/**
@brief	�����ڲ��쳣��
*/

class CORE_API CInnerException :
    public CSysException
{
public:
	static const int MODULE_ID = 1000;      // ģ���
	typedef enum _detail_error_code {
	        SET_PARAM_ERR               = 1,	///< �趨ֵ�������쳣
			FUNC_PARAM_ERR              = 2,	///< ���������쳣
			SEQUENCE_ERR	            = 3,	///< ��������˳���쳣
			FUNC_ERR_RETURN	            = 4,	///< ���������쳣
			LOGIC_ERR	                = 5,	///< �߼��쳣
			OTHER_ERR	                = 6,	///< �쳣����
			ERROR_BUTTON_NOT_FOUND		= 7,	// < �����в����ڸð�ť
			
		    SYSTEM_ERROR				= 1000,	///< ϵͳ����
	} DETAIL_ERROR_CODE;

    CInnerException(long lModuleID, long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CInnerException();

};
