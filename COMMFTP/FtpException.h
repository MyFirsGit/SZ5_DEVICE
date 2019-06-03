#pragma once
#include "afx.h"
#include "SysException.h"
#include "ftpexp.h"

/**
@brief	FTP异常类
*/

class FTP_API CFtpException : public CSysException
{
public:
    static const int MODULE_ID = 10102;     // 模块号

    typedef enum _detail_error_code {
        SET_PARAM_ERR           = 1,		// 设定值：参数异常
        FUNC_PARAM_ERR          = 2,		// 函数参数异常
        SEQUENCE_ERR	        = 3,		// 函数调用顺序异常
        FUNC_ERR_RETURN	        = 4,		// 函数返回异常
        LOGIC_ERR	            = 5,		// 逻辑异常
        OTHER_ERR	            = 6,		// 异常发生
		FTP_CONNECT_ERR			= 101,		// FTP连接异常
		//FTP_USERNO_ERR			= 102,      // 非法的FTP用户名
		//FTP_PASSWORD_ERR		= 103,      // 非法的FTP密码
		//FTP_URL_ERR				= 104,      // 无法解释的FTP URL
		//FTP_FILE_NOTEXIST_ERR	= 105,      // 上传文件不存在
		//FTP_NOTCONNECT_ERR		= 106,      // FTP未连接
		FTP_CLOSE_ERR			= 107,      // FTP Close错误
		FTP_DOWNLOAD_ERR		= 108,      // FTP下载错误
		FTP_UPLOAD_ERR			= 109      // FTP上载错误
    } DETAIL_ERROR_CODE;


    CFtpException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CFtpException(void);

private:
};
