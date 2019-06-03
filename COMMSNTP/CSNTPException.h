#pragma once
#include "afx.h"
#include "SysException.h"
#include "SntpApi.h"

/**
@brief	ʱ��ͬ���쳣��
*/

class SNTP_API CSNTPSException :
	public CSysException
{
public:
	static const int MODULE_ID = 10107;     // ģ���

	typedef enum _detail_error_code {
		TIME_SYNCHRONIZE_ERROR      = 200,  // ʱ��ͬ������
		TIME_SYNCHRONIZE_OTHER_ERROR = 201,//ʱ��ͬ����������
	} DETAIL_ERROR_CODE;

	CSNTPSException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CSNTPSException(void);

};
