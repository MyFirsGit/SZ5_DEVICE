#pragma once

#define	MIMTRS_RES_OK				0L	// ����Ӧ��ɹ�
//
typedef enum _th_err_detail {
	TH_SET_PARAM_ERR            = 1,	// �趨ֵ�������쳣
	TH_FUNC_PARAM_ERR           = 2,	// ���������쳣
	TH_SEQUENCE_ERR	            = 3,	// ��������˳���쳣
	TH_FUNC_ERR_RETURN	        = 4,	// ���������쳣
	TH_LOGIC_ERR	            = 5,	// �߼��쳣
	TH_OTHER_ERR                = 6,    // ������ش��� 

	MIMTRS_RES_WINERR           = 1001, // ϵͳ����
	MIMTRS_ERR_HANDLE           = 1002, // ���ڴ�ʧ��
	MIMTRS_OPEN_FILE_FAIL		= 1003, // ���ļ�ʧ��
	MIMTRS_READ_FILE_FAIL		= 1004, // ��ȡ�ļ�ʧ��
	MIMTRS_CMD_EXECUTE_FAIL		= 1005, // ����ִ��ʧ��
	MIMTRS_RECV_EMPTY           = 1006, // û�н��յ�����.

}TH_ERR_DETAIL;

