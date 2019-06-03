#pragma once
#include "afx.h"
#include "SysException.h"
#include "ftpexp.h"

/**
@brief	FTP�쳣��
*/

class FTP_API CFtpException : public CSysException
{
public:
    static const int MODULE_ID = 10102;     // ģ���

    typedef enum _detail_error_code {
        SET_PARAM_ERR           = 1,		// �趨ֵ�������쳣
        FUNC_PARAM_ERR          = 2,		// ���������쳣
        SEQUENCE_ERR	        = 3,		// ��������˳���쳣
        FUNC_ERR_RETURN	        = 4,		// ���������쳣
        LOGIC_ERR	            = 5,		// �߼��쳣
        OTHER_ERR	            = 6,		// �쳣����
		FTP_CONNECT_ERR			= 101,		// FTP�����쳣
		//FTP_USERNO_ERR			= 102,      // �Ƿ���FTP�û���
		//FTP_PASSWORD_ERR		= 103,      // �Ƿ���FTP����
		//FTP_URL_ERR				= 104,      // �޷����͵�FTP URL
		//FTP_FILE_NOTEXIST_ERR	= 105,      // �ϴ��ļ�������
		//FTP_NOTCONNECT_ERR		= 106,      // FTPδ����
		FTP_CLOSE_ERR			= 107,      // FTP Close����
		FTP_DOWNLOAD_ERR		= 108,      // FTP���ش���
		FTP_UPLOAD_ERR			= 109      // FTP���ش���
    } DETAIL_ERROR_CODE;


    CFtpException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CFtpException(void);

private:
};
