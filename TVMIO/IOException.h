#pragma once
#include "afx.h"
#include "SysException.h"
#include "IOExp.h"

/**
@brief	TVM IO����쳣��
*/
class IO_API CIOException :	public CSysException
{
public:
	static const int MODULE_ID = 11902;// ģ���

	typedef enum _detail_error_code {
		SET_PARAM_ERR					= 1,	// �趨ֵ�������쳣
		FUNC_PARAM_ERR				= 2,	// ���������쳣
		SEQUENCE_ERR					= 3,	// ��������˳���쳣
		FUNC_ERR_RETURN	            = 4,	// ���������쳣
		LOGIC_ERR							= 5,	// �߼��쳣
		OTHER_ERR							= 6,	// �쳣����

		ERROR_IO_SEQUNCE_ERR			= 201,				// ����ʱ�����
		ERROR_IO_OPEN						= 202,				// �򿪴���
		ERROR_IO_CLOSE						= 203,				// �رմ���
		ERROR_IO_SEND						= 204,				// �������ݴ���
		ERROR_IO_RECEIVE					= 205,				// �������ݴ���
		ERROR_IO_GETSTATE				= 206,				// ��ȡ״̬����
		ERROR_IO_RECEIVE_TIMEOUT   	= 207				// �������ݳ�ʱ
	} DETAIL_ERROR_CODE;

	CIOException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CIOException(void);
};
