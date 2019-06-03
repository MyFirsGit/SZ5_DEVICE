#pragma once
#include "afx.h"
#include "SysException.h"
//#include "printerexp.h"

/**
@brief	打印机异常类
*/

class CPrinterException :
    public CSysException
{
public:
    static const int MODULE_ID = 12101;     // 模块号

    typedef enum _detail_error_code {
        SET_PARAM_ERR               = 1,	// 设定值：参数异常
        FUNC_PARAM_ERR              = 2,	// 函数参数异常
        SEQUENCE_ERR	            = 3,	// 函数调用顺序异常
        FUNC_ERR_RETURN	            = 4,	// 函数返回异常
        LOGIC_ERR	                = 5,	// 逻辑异常
        OTHER_ERR	                = 6,	// 异常发生

		ERROR_Printer_LOAD_SDK =  200 ,       //加载SDK错误
		ERROR_Printer_OPEN_COM =  201 ,       //打印机连接错误
		ERROR_Printer_CLOSE_COM = 202 ,       //打印机关闭错误
		ERROR_Printer_SETTIME = 203 ,         //设置打印超时
		ERROR_Printer_GETSTATE = 204 ,        //取打印机dcb错误
		ERROR_Printer_SETSTATE = 205 ,        //设置打印机dcb错误
		ERROR_Printer_BAUD_INVALID = 206 ,    //波特率设置不合法
		ERROR_Printer_OVERLAPPED_EVENT = 207, //生成OVERLAPPED事件失败
		ERROR_Printer_NOTOPEN = 208 ,         //未打开打印机错误
		ERROR_Printer_READFILE = 209 ,        //从打印机读数据错误
		ERROR_Printer_WRITEFILE = 210 ,       //向打印机写数据错误
		ERROR_Printer_WAIT_ABANDONED = 211,   //遇到错误放弃
		ERROR_Printer_WAIT_TIMEOUT = 212,     //打印时超时
		ERROR_Printer_KNIFE_ERROR = 213,     // 打印机切刀错误
		ERROR_Printer_PARAMTER_ERROR = 214,  // 参数错误
		ERROR_Printer_OTHER_ERROR = 215,     // 打印机其他错误
		ERROR_Printer_NO_PAPER = 216,		 // 打印机纸尽
		ERROR_Printer_LESS_PAPER = 217,		 // 打印机纸将尽
		ERROR_Printer_HARDWARE = 218,		 // 打印机硬件错误

    } DETAIL_ERROR_CODE;

    CPrinterException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CPrinterException(void);

private:
};
