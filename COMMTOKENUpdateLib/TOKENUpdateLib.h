#pragma once

#define	TOKENTRS_RES_OK				0L	// ����Ӧ��ɹ�
//
typedef enum _th_err_detail {
	TOKEN_SET_PARAM_ERR            = 1,	// �趨ֵ�������쳣
	TOKEN_FUNC_PARAM_ERR           = 2,	// ���������쳣
	TOKEN_SEQUENCE_ERR	            = 3,	// ��������˳���쳣
	TOKEN_FUNC_ERR_RETURN	        = 4,	// ���������쳣
	TOKEN_LOGIC_ERR	            = 5,	// �߼��쳣
	TOKEN_OTHER_ERR                = 6,    // ������ش��� 

	TOKENTRS_RES_WINERR           = 1001, // ϵͳ����
	TOKENTRS_ERR_HANDLE           = 1002, // ���ڴ�ʧ��
	TOKENTRS_OPEN_FILE_FAIL		= 1003, // ���ļ�ʧ��
	TOKENTRS_READ_FILE_FAIL		= 1004, // ��ȡ�ļ�ʧ��
	TOKENTRS_CMD_EXECUTE_FAIL		= 1005, // ����ִ��ʧ��
	TOKENTRS_RECV_EMPTY           = 1006, // û�н��յ�����.

}TOKEN_ERR_DETAIL;

