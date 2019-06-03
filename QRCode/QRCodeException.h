#pragma once
#include "afx.h"
#include "SysException.h"
#include "QRCodeexp.h"

/**
@brief	摄像头异常类
*/

class QRCode_API CQRCodeException :
    public CSysException
{
public:
    static const int MODULE_ID = 11010;     // 模块号

    typedef enum _detail_error_code {
        SET_PARAM_ERR               = 1,	// 设定值：参数异常
        FUNC_PARAM_ERR              = 2,	// 函数参数异常
        SEQUENCE_ERR	            = 3,	// 函数调用顺序异常
        FUNC_ERR_RETURN	            = 4,	// 函数返回异常
        LOGIC_ERR	                = 5,	// 逻辑异常
        OTHER_ERR	                = 6,	// 异常发生
		
		QRCODE_FILENAME_ERROR = 101,		// 文件名错误
		QRCODE_CREATE_FILE_ERROR = 102,		// 创建文件失败
		QRCODE_BUFFER_LENGTH_LESS,			// 接受数据的缓冲区长度太小
		QRCODE_VERSION_ERROR,				// 版本超出范围
		QRCODE_OUT_OF_IMAGE,				// 对齐方式查找超过了图像
		QRCODE_INVALID_NUMBER,				// 无效的数据检测模式
		QRCODE_VERSION_RECOVERY,			// 版本信息纠错失败
		QRCODE_FORMATE_RECOVERY,			// 格式信息纠错失败
		QRCODE_OUT_OF_SAMPLING,				// 超出图像采样网格
		QRCODE_NOT_SUPPORT_CHARACTERSET,	// 不支持的字符集
	} DETAIL_ERROR_CODE;

    CQRCodeException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CQRCodeException(void);

private:
};
