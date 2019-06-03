#pragma once
#include "afx.h"
#include "SysException.h"
#include "coreexp.h"

/**
@brief	�ļ������쳣��
*/

class CORE_API CLogException : public CSysException
{
//    DECLARE_DYNAMIC(LogException);
public:
    static const int MODULE_ID = 10108;     // ģ���

    typedef enum _detail_error_code {
        SET_PARAM_ERR					= 1,	    // �趨ֵ�������쳣
        FUNC_PARAM_ERR					= 2,	    // ���������쳣
        SEQUENCE_ERR					= 3,	    // ��������˳���쳣
        FUNC_ERR_RETURN					= 4,	    // ���������쳣
        LOGIC_ERR						= 5,	    // �߼��쳣
        OTHER_ERR						= 6,	    // �쳣����

        FILE_NOT_EXIST					= 101,	    // �ļ�������
        FILE_READ_FAIL					= 102,	    // �ļ���ȡ�쳣
        FILE_WRITE_FAIL					= 103,	    // �ļ�д���쳣
        FILE_OPEN_FAIL					= 104,	    // �ļ����쳣
        FILE_DEL_FAIL					= 105,	    // �ļ�ɾ���쳣
		FILE_LEN_FAIL					= 106,	    // �ļ������쳣
		FILE_SAM_ERR					= 107,	    // �ļ���ʽ����sumֵ�쳣��
		FILE_CRC_ERR					= 108,      // �ļ���ʽ����crcֵ�쳣��
		FILE_RESCUE_FAIL				= 109,	    // ���ػ��ļ��ָ��쳣
		FILE_RECOVER_OVERMAXTIMES		= 199,		// ���ػ��ļ��ָ������쳣
		FILE_RECOVER_1					= 305,	    // ���ػ��ļ��Զ��ָ�(SDD2->SDD1)
		FILE_RECOVER_2					= 306,	    // ���ػ��ļ��Զ��ָ�(SDD1->SDD2)

        CTLOG_ERR_SALEDATA_NOT_SEND	    = 701,		// ��������δ�͵�����
	} DETAIL_ERROR_CODE;

    CLogException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CLogException(void);

private:
};
