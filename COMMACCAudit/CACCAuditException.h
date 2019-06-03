#pragma once
#include "afx.h"
#include "SysException.h"
#include "coreexp.h"
#include "ACCAuditExp.h"
/**
@brief	�Ĵ������ݴ����쳣��
*/

class ACCAUDIT_DECLSPEC CARException : public CSysException
{
	//    DECLARE_DYNAMIC(CARException);
public:
	static const int MODULE_ID = 10122;     // ģ���

	typedef enum _detail_error_code {
		FUNC_PARAM_ERR					= 1,	    // ���������쳣

		FILE_READ_FAIL					= 101,	    // AR����ļ���ȡ�쳣
		FILE_WRITE_FAIL					= 102,	    // AR����ļ�д���쳣
		FILE_OPEN_FAIL					= 103,	    // ��AR����ļ��쳣
 		FILE_RESCUE_FAIL				= 104,	    // ���ػ�����AR����ļ�������
 		FILE_RECOVER_OVERFAIL    		= 105,		// ���ػ�AR����ļ��ָ��쳣
		
		FOLDER_NOT_EXIST			= 201,	    // Ŀ¼������
		FOLDER_CREATE_FAIL 			= 202,	    // �½�Ŀ¼�쳣

		KEY_ERR = 301,											//����ļ�ֵ

		MALLOC_FAIL			     		= 401		    // �����ڴ�ʧ��
	} DETAIL_ERROR_CODE;

	CARException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage =_T(""));
	~CARException(void);

};
