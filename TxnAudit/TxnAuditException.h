#pragma once
#include "afx.h"
#include "SysException.h"
#include "coreexp.h"
#include "TxnAuditExp.h"
/**
@brief	参数文件处理异常类
*/

class TXNAUDIT_DECLSPEC CTxnAuditException : public CSysException
{
	//    DECLARE_DYNAMIC(CTxnAuditException);
public:
	static const int MODULE_ID = 10920;     // 模块号

	typedef enum _detail_error_code {
		SET_PARAM_ERR	            = 1,	    // 设定值：参数异常
		FUNC_PARAM_ERR				= 2,	    // 函数参数异常
		SEQUENCE_ERR				= 3,	    // 函数调用顺序异常
		FUNC_ERR_RETURN				= 4,	    // 函数返回异常
		LOGIC_ERR					= 5,	    // 逻辑异常
		OTHER_ERR					= 6,	    // 异常发生


		FILE_USING_ERR				= 100,	    // 文件被锁定
		FILE_NOT_EXIST				= 101,	    // 文件不存在
		FILE_READ_FAIL				= 102,	    // 文件读取异常
		FILE_WRITE_FAIL				= 103,	    // 文件写入异常
		FILE_OPEN_FAIL				= 104,	    // 文件打开异常
		FILE_DELETE_FAIL			= 105,	    // 文件删除异常
		FILE_LENGTH_FAIL		    = 106,	    // 文件长度异常
		FILE_CRC_ERR				= 107,	    // 文件格式错误（CRC32值异常）
		FILE_CREATE_FAIL			= 108,	    // 文件新建异常
		FILE_RESCUE_FAIL			= 109,	    // 两重化文件恢复异常
		FILE_RECOVER_OVERMAXTIMES	= 199,		// 两重化文件恢复次数异常
		FILE_RECOVER_1				= 305,	    // 两重化文件自动恢复(SDD2->SDD1)
		FILE_RECOVER_2				= 306,	    // 两重化文件自动恢复(SDD1->SDD2)

		FOLDER_NOT_EXIST			= 401,	    // 目录不存在
		FOLDER_CREATE_FAIL 			= 402,	    // 新建目录异常

		INVALID_DATE_ERR			= 501,		// 日期格式不合法
		NOT_SEND_FILE_ERR			= 510,	    // 集计文件不是未送文件
		FILE_NAME_INVALID				= 520,   // 文件名不合法

		DATA_NOT_SAME_ERR 			= 601,	    // 设定送上为标志时传入数据与保存数据不一致
		RENAME_FILE_FAIL				= 602,		// 命令名文件失败

		SALEDATA_NOT_SEND_TOO_MORE	= 701,		// 未送数据过多

		MALLOC_FAIL			     		= 401		    // 分配内存失败
	} DETAIL_ERROR_CODE;

	CTxnAuditException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage =_T(""));
	~CTxnAuditException(void);

};
