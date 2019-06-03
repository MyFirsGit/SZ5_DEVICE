#pragma once
#include "afx.h"
#include "SysException.h"
#include "coreexp.h"
#include "ACCAuditExp.h"
/**
@brief	寄存器数据处理异常类
*/

class ACCAUDIT_DECLSPEC CARException : public CSysException
{
	//    DECLARE_DYNAMIC(CARException);
public:
	static const int MODULE_ID = 10122;     // 模块号

	typedef enum _detail_error_code {
		FUNC_PARAM_ERR					= 1,	    // 函数参数异常

		FILE_READ_FAIL					= 101,	    // AR审计文件读取异常
		FILE_WRITE_FAIL					= 102,	    // AR审计文件写入异常
		FILE_OPEN_FAIL					= 103,	    // 打开AR审计文件异常
 		FILE_RESCUE_FAIL				= 104,	    // 二重化备份AR审计文件不存在
 		FILE_RECOVER_OVERFAIL    		= 105,		// 二重化AR审计文件恢复异常
		
		FOLDER_NOT_EXIST			= 201,	    // 目录不存在
		FOLDER_CREATE_FAIL 			= 202,	    // 新建目录异常

		KEY_ERR = 301,											//错误的键值

		MALLOC_FAIL			     		= 401		    // 分配内存失败
	} DETAIL_ERROR_CODE;

	CARException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage =_T(""));
	~CARException(void);

};
