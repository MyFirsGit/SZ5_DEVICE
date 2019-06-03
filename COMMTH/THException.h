#pragma once
#include "afx.h"
#include "SysException.h"
#include "THCOMLIBExp.h"

/**
@brief	�ⲿ��д�������쳣��
*/

class THCOMLIB_DECLSPEC CTHException : public CSysException
{
public:
    static const int MODULE_ID = 12402; // ģ���

    typedef enum _detail_error_code {
        SET_PARAM_ERR           = 1,	// �趨ֵ�������쳣
        FUNC_PARAM_ERR          = 2,	// ���������쳣
        SEQUENCE_ERR	        = 3,	// ��������˳���쳣
        FUNC_ERR_RETURN	        = 4,	// ���������쳣
        LOGIC_ERR	            = 5,	// �߼��쳣
        OTHER_ERR	            = 6,	// �쳣����
    } DETAIL_ERROR_CODE;

    CTHException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CTHException(void);

private:
};