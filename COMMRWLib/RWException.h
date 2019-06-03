#pragma once
#include "afx.h"
#include "SysException.h"
#include "RWCOMLIBExp.h"

/**
@brief	�ⲿ��д������쳣��
*/

class RWCOMLIB_DECLSPEC CRWException : public CSysException
{
public:
    static const int MODULE_ID = 10310; // ģ���
//
//    typedef enum _detail_error_code {
//        SET_PARAM_ERR           = 1,	// �趨ֵ�������쳣
//        FUNC_PARAM_ERR          = 2,	// ���������쳣
//        SEQUENCE_ERR	        = 3,	// ��������˳���쳣
//        FUNC_ERR_RETURN	        = 4,	// ���������쳣
//        LOGIC_ERR	            = 5,	// �߼��쳣
//        OTHER_ERR	            = 6,	// �쳣����
//    } DETAIL_ERROR_CODE;
//
//    CRWException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
//    ~CRWException(void);
//
//private:

	public:
	static const int BOM_DESKTOP_RW_MODULE_ID					= 11102;    // BOM�����д��
	static const int BOM_CARD_RW_MODULE_ID						= 11104;    // BOMƱ��ģ���ö�д��
	static const int BOM_TOKEN_RW_MODULE_ID						= 11106;    // BOM Tokenģ���ö�д��
	static const int TVM_RECHARGE_RW_MODULE_ID					= 11108;    // TVM��ֵ�ö�д��
	static const int TVM_CARD_RW_MODULE_ID						= 11110;    // TVMƱ��ģ���ö�д��
	static const int TVM_TOKEN_RW_MODULE_ID						= 11112;    // TVM Tokenģ���ö�д��
	static const int ES_TOKEN_RW1_MODULE_ID						= 11114;	// Token ES��д��1
	static const int ES_TOKEN_RW2_MODULE_ID						= 11116;	// Token ES��д��2
	static const int ES_TOKEN_RW3_MODULE_ID						= 11118;	// Token ES��д��3
	static const int ES_CARD_RW_MODULE_ID						= 11120;	// Token ES��д��3
	static const int PM_RW_MODULE_ID							= 11122;	// PM��д��
	static const int TCM_RW_MODULE_ID							= 11124;	// TCM��д��
	static const int AGM_CARD_RECYCLE_RW_MODULE_ID				= 10370;    // AGM Ʊ�����ն�д��
	static const int AGM_TOKEN_RECYCLE_RW_MODULE_ID				= 10380;    // AGM Token���ն�д��
	static const int AGM_ENTRY_RW_MODULE_ID						= 10390;    // AGM��վˢ����д��
	static const int AGM_EXIT_RW_MODULE_ID						= 10400;    // AGM��վˢ����д��


    typedef enum _detail_error_code {
        SET_PARAM_ERR           = 1,	// �趨ֵ�������쳣
        FUNC_PARAM_ERR          = 2,	// ���������쳣
        SEQUENCE_ERR	        = 3,	// ��������˳���쳣
        FUNC_ERR_RETURN	        = 4,	// ���������쳣
        LOGIC_ERR	            = 5,	// �߼��쳣
        OTHER_ERR	            = 6,	// �쳣����
		PARAM_NOT_EXIST			= 7,	// �����ļ�������
    } DETAIL_ERROR_CODE;

    CRWException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	CRWException(RW_TYPE rwType,long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CRWException(void);
	static long GetModuleID(RW_TYPE rwType);
};
