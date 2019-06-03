#pragma once
#include "afx.h"
#include "SysException.h"
#include "BHexp.h"
/**
@brief	BHӲ���쳣��
*/
class BH_API CBHHardwareException : public CSysException
{

public:
	static const int MODULE_ID = 11701;     // ģ���

	typedef enum _error_banknote_module_{
		ERROR_BH_MODULE_NOT_STANDBY				= 200,	// ֽ��ģ��δ��λ
		ERROR_BH_MODULE_SAFE_DOOR_OPEN			= 201,	// ֽ��ģ���ֽ���δ�ر�
		ERROR_BH_MODULE_OUTER_SHUTTER_EXCEPTION = 202,	// ֽ�ҳ��ҿ����쳣
		ERROR_BH_MODULE_EXCEPTION				= 500,	// ֽ��ģ���쳣
		ERROR_BH_MODULE_COLL_BOX_NOT_INSTALL	= 501,	// ֽ��ģ�������δ��װ
		ERROR_BH_MODULE_WORK_INVALID			= 502,	// ֽ��ģ���쳣����2��
		ERROR_BH_CLEAR_MONEY_FAILED				= 503,	// ֽ������Ǯʧ��
	}ERROR_BANKNOTE_DETAIL;

	CBHHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CBHHardwareException(void);

private:
};