#pragma once
#include "afx.h"
#include "SysException.h"
#include "StatusDisplayexp.h"

/**
@brief	״̬�����쳣��
*/

class STATUS_DISPLAY_API CStatusDisplayException :  public CSysException
{
public:
    static const int MODULE_ID = 12001;     // ģ���

    typedef enum _detail_error_code {
        SET_PARAM_ERR               = 1,	// �趨ֵ�������쳣
        FUNC_PARAM_ERR              = 2,	// ���������쳣
        SEQUENCE_ERR	            = 3,	// ��������˳���쳣
        FUNC_ERR_RETURN	            = 4,	// ���������쳣
        LOGIC_ERR	                = 5,	// �߼��쳣
        OTHER_ERR	                = 6,	// �쳣����
		//=============================================================================		
    } DETAIL_ERROR_CODE;

    CStatusDisplayException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CStatusDisplayException(void);

private:
};
