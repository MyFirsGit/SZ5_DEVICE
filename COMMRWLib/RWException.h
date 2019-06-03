#pragma once
#include "afx.h"
#include "SysException.h"
#include "RWCOMLIBExp.h"

/**
@brief	外部读写器软件异常类
*/

class RWCOMLIB_DECLSPEC CRWException : public CSysException
{
public:
    static const int MODULE_ID = 10310; // 模块号
//
//    typedef enum _detail_error_code {
//        SET_PARAM_ERR           = 1,	// 设定值：参数异常
//        FUNC_PARAM_ERR          = 2,	// 函数参数异常
//        SEQUENCE_ERR	        = 3,	// 函数调用顺序异常
//        FUNC_ERR_RETURN	        = 4,	// 函数返回异常
//        LOGIC_ERR	            = 5,	// 逻辑异常
//        OTHER_ERR	            = 6,	// 异常发生
//    } DETAIL_ERROR_CODE;
//
//    CRWException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
//    ~CRWException(void);
//
//private:

	public:
	static const int BOM_DESKTOP_RW_MODULE_ID					= 11102;    // BOM桌面读写器
	static const int BOM_CARD_RW_MODULE_ID						= 11104;    // BOM票卡模块用读写器
	static const int BOM_TOKEN_RW_MODULE_ID						= 11106;    // BOM Token模块用读写器
	static const int TVM_RECHARGE_RW_MODULE_ID					= 11108;    // TVM充值用读写器
	static const int TVM_CARD_RW_MODULE_ID						= 11110;    // TVM票卡模块用读写器
	static const int TVM_TOKEN_RW_MODULE_ID						= 11112;    // TVM Token模块用读写器
	static const int ES_TOKEN_RW1_MODULE_ID						= 11114;	// Token ES读写器1
	static const int ES_TOKEN_RW2_MODULE_ID						= 11116;	// Token ES读写器2
	static const int ES_TOKEN_RW3_MODULE_ID						= 11118;	// Token ES读写器3
	static const int ES_CARD_RW_MODULE_ID						= 11120;	// Token ES读写器3
	static const int PM_RW_MODULE_ID							= 11122;	// PM读写器
	static const int TCM_RW_MODULE_ID							= 11124;	// TCM读写器
	static const int AGM_CARD_RECYCLE_RW_MODULE_ID				= 10370;    // AGM 票卡回收读写器
	static const int AGM_TOKEN_RECYCLE_RW_MODULE_ID				= 10380;    // AGM Token回收读写器
	static const int AGM_ENTRY_RW_MODULE_ID						= 10390;    // AGM进站刷卡读写器
	static const int AGM_EXIT_RW_MODULE_ID						= 10400;    // AGM出站刷卡读写器


    typedef enum _detail_error_code {
        SET_PARAM_ERR           = 1,	// 设定值：参数异常
        FUNC_PARAM_ERR          = 2,	// 函数参数异常
        SEQUENCE_ERR	        = 3,	// 函数调用顺序异常
        FUNC_ERR_RETURN	        = 4,	// 函数返回异常
        LOGIC_ERR	            = 5,	// 逻辑异常
        OTHER_ERR	            = 6,	// 异常发生
		PARAM_NOT_EXIST			= 7,	// 参数文件不存在
    } DETAIL_ERROR_CODE;

    CRWException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	CRWException(RW_TYPE rwType,long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CRWException(void);
	static long GetModuleID(RW_TYPE rwType);
};
