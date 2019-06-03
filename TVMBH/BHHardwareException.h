#pragma once
#include "afx.h"
#include "SysException.h"
#include "BHexp.h"
/**
@brief	BH硬件异常类
*/
class BH_API CBHHardwareException : public CSysException
{

public:
	static const int MODULE_ID = 11701;     // 模块号

	typedef enum _error_banknote_module_{
		ERROR_BH_MODULE_NOT_STANDBY				= 200,	// 纸币模块未到位
		ERROR_BH_MODULE_SAFE_DOOR_OPEN			= 201,	// 纸币模块现金门未关闭
		ERROR_BH_MODULE_OUTER_SHUTTER_EXCEPTION = 202,	// 纸币出币口有异常
		ERROR_BH_MODULE_EXCEPTION				= 500,	// 纸币模块异常
		ERROR_BH_MODULE_COLL_BOX_NOT_INSTALL	= 501,	// 纸币模块回收箱未安装
		ERROR_BH_MODULE_WORK_INVALID			= 502,	// 纸币模块异常（！2）
		ERROR_BH_CLEAR_MONEY_FAILED				= 503,	// 纸币清理钱失败
	}ERROR_BANKNOTE_DETAIL;

	CBHHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CBHHardwareException(void);

private:
};