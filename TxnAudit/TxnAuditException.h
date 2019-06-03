#pragma once
#include "afx.h"
#include "SysException.h"
#include "coreexp.h"
#include "TxnAuditExp.h"
/**
@brief	�����ļ������쳣��
*/

class TXNAUDIT_DECLSPEC CTxnAuditException : public CSysException
{
	//    DECLARE_DYNAMIC(CTxnAuditException);
public:
	static const int MODULE_ID = 10920;     // ģ���

	typedef enum _detail_error_code {
		SET_PARAM_ERR	            = 1,	    // �趨ֵ�������쳣
		FUNC_PARAM_ERR				= 2,	    // ���������쳣
		SEQUENCE_ERR				= 3,	    // ��������˳���쳣
		FUNC_ERR_RETURN				= 4,	    // ���������쳣
		LOGIC_ERR					= 5,	    // �߼��쳣
		OTHER_ERR					= 6,	    // �쳣����


		FILE_USING_ERR				= 100,	    // �ļ�������
		FILE_NOT_EXIST				= 101,	    // �ļ�������
		FILE_READ_FAIL				= 102,	    // �ļ���ȡ�쳣
		FILE_WRITE_FAIL				= 103,	    // �ļ�д���쳣
		FILE_OPEN_FAIL				= 104,	    // �ļ����쳣
		FILE_DELETE_FAIL			= 105,	    // �ļ�ɾ���쳣
		FILE_LENGTH_FAIL		    = 106,	    // �ļ������쳣
		FILE_CRC_ERR				= 107,	    // �ļ���ʽ����CRC32ֵ�쳣��
		FILE_CREATE_FAIL			= 108,	    // �ļ��½��쳣
		FILE_RESCUE_FAIL			= 109,	    // ���ػ��ļ��ָ��쳣
		FILE_RECOVER_OVERMAXTIMES	= 199,		// ���ػ��ļ��ָ������쳣
		FILE_RECOVER_1				= 305,	    // ���ػ��ļ��Զ��ָ�(SDD2->SDD1)
		FILE_RECOVER_2				= 306,	    // ���ػ��ļ��Զ��ָ�(SDD1->SDD2)

		FOLDER_NOT_EXIST			= 401,	    // Ŀ¼������
		FOLDER_CREATE_FAIL 			= 402,	    // �½�Ŀ¼�쳣

		INVALID_DATE_ERR			= 501,		// ���ڸ�ʽ���Ϸ�
		NOT_SEND_FILE_ERR			= 510,	    // �����ļ�����δ���ļ�
		FILE_NAME_INVALID				= 520,   // �ļ������Ϸ�

		DATA_NOT_SAME_ERR 			= 601,	    // �趨����Ϊ��־ʱ���������뱣�����ݲ�һ��
		RENAME_FILE_FAIL				= 602,		// �������ļ�ʧ��

		SALEDATA_NOT_SEND_TOO_MORE	= 701,		// δ�����ݹ���

		MALLOC_FAIL			     		= 401		    // �����ڴ�ʧ��
	} DETAIL_ERROR_CODE;

	CTxnAuditException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage =_T(""));
	~CTxnAuditException(void);

};
