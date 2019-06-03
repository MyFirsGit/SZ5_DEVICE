#pragma once
#include "afx.h"
#include "SysException.h"
#include "coreexp.h"

/**
@brief	文件处理异常类
*/

class CORE_API CLogException : public CSysException
{
//    DECLARE_DYNAMIC(LogException);
public:
    static const int MODULE_ID = 10108;     // 模块号

    typedef enum _detail_error_code {
        SET_PARAM_ERR					= 1,	    // 设定值：参数异常
        FUNC_PARAM_ERR					= 2,	    // 函数参数异常
        SEQUENCE_ERR					= 3,	    // 函数调用顺序异常
        FUNC_ERR_RETURN					= 4,	    // 函数返回异常
        LOGIC_ERR						= 5,	    // 逻辑异常
        OTHER_ERR						= 6,	    // 异常发生

        FILE_NOT_EXIST					= 101,	    // 文件不存在
        FILE_READ_FAIL					= 102,	    // 文件读取异常
        FILE_WRITE_FAIL					= 103,	    // 文件写入异常
        FILE_OPEN_FAIL					= 104,	    // 文件打开异常
        FILE_DEL_FAIL					= 105,	    // 文件删除异常
		FILE_LEN_FAIL					= 106,	    // 文件长度异常
		FILE_SAM_ERR					= 107,	    // 文件格式错误（sum值异常）
		FILE_CRC_ERR					= 108,      // 文件格式错误（crc值异常）
		FILE_RESCUE_FAIL				= 109,	    // 两重化文件恢复异常
		FILE_RECOVER_OVERMAXTIMES		= 199,		// 两重化文件恢复次数异常
		FILE_RECOVER_1					= 305,	    // 两重化文件自动恢复(SDD2->SDD1)
		FILE_RECOVER_2					= 306,	    // 两重化文件自动恢复(SDD1->SDD2)

        CTLOG_ERR_SALEDATA_NOT_SEND	    = 701,		// 存在期限未送的数据
	} DETAIL_ERROR_CODE;

    CLogException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CLogException(void);

private:
};
