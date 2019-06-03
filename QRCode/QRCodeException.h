#pragma once
#include "afx.h"
#include "SysException.h"
#include "QRCodeexp.h"

/**
@brief	����ͷ�쳣��
*/

class QRCode_API CQRCodeException :
    public CSysException
{
public:
    static const int MODULE_ID = 11010;     // ģ���

    typedef enum _detail_error_code {
        SET_PARAM_ERR               = 1,	// �趨ֵ�������쳣
        FUNC_PARAM_ERR              = 2,	// ���������쳣
        SEQUENCE_ERR	            = 3,	// ��������˳���쳣
        FUNC_ERR_RETURN	            = 4,	// ���������쳣
        LOGIC_ERR	                = 5,	// �߼��쳣
        OTHER_ERR	                = 6,	// �쳣����
		
		QRCODE_FILENAME_ERROR = 101,		// �ļ�������
		QRCODE_CREATE_FILE_ERROR = 102,		// �����ļ�ʧ��
		QRCODE_BUFFER_LENGTH_LESS,			// �������ݵĻ���������̫С
		QRCODE_VERSION_ERROR,				// �汾������Χ
		QRCODE_OUT_OF_IMAGE,				// ���뷽ʽ���ҳ�����ͼ��
		QRCODE_INVALID_NUMBER,				// ��Ч�����ݼ��ģʽ
		QRCODE_VERSION_RECOVERY,			// �汾��Ϣ����ʧ��
		QRCODE_FORMATE_RECOVERY,			// ��ʽ��Ϣ����ʧ��
		QRCODE_OUT_OF_SAMPLING,				// ����ͼ���������
		QRCODE_NOT_SUPPORT_CHARACTERSET,	// ��֧�ֵ��ַ���
	} DETAIL_ERROR_CODE;

    CQRCodeException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CQRCodeException(void);

private:
};
