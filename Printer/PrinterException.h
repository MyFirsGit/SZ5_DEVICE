#pragma once
#include "afx.h"
#include "sysException.h"
#include "printerexp.h"

/**
@brief	打印机异常类
*/

class PRINTER_API CPrinterException :
    public CSysException
{
public:
    static const int MODULE_ID = 10410;     // 模块号

    typedef enum _detail_error_code {
        SET_PARAM_ERR               = 1,	// 设定值：参数异常
        FUNC_PARAM_ERR              = 2,	// 函数参数异常
        SEQUENCE_ERR	            = 3,	// 函数调用顺序异常
        FUNC_ERR_RETURN	            = 4,	// 函数返回异常
        LOGIC_ERR	                = 5,	// 逻辑异常
        OTHER_ERR	                = 6,	// 异常发生

		ERROR_Printer_OPEN_COM =  201 ,       //打开时　无法打开打印机串口错误
		ERROR_Printer_CLOSE_COM = 202 ,       //打开时　关闭打印机器串口错误
		ERROR_Printer_OPEN_ERROR = 203 ,      //打开时　打开打印机错误
		ERROR_Printer_SETTIME = 204 ,         //打开时　设置打印超时错误
		ERROR_Printer_GETSTATE = 205 ,        //打开时　取打印机dcb错误
		ERROR_Printer_SETSTATE = 206 ,        //打开时　设置打印机dcb错误
		ERROR_Printer_BAUD_INVALID = 207 ,    //打开时　波特率设置不合法
		ERROR_Printer_OVERLAPPED_EVENT = 208, //打开时　生成OVERLAPPED事件失败
		ERROR_Printer_NOTOPEN = 209 ,         //打印时　未打开打印机错误
		ERROR_Printer_WRITEFILE = 210 ,       //打印时　向打印机写数据错误
		ERROR_Printer_WAIT_ABANDONED = 211,   //打印时  遇到错误放弃
		ERROR_Printer_WAIT_TIMEOUT = 212,     //打印时  超时
		ERROR_Printer_NO_PAPER = 213,         //打印时　打印机无纸
		ERROR_Printer_LESS_PAPER = 214,       //打印时　打印机纸将无
		ERROR_Printer_CLOSE = 215,            //关闭时　关闭打印机串口错误
    } DETAIL_ERROR_CODE;

    CPrinterException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CPrinterException(void);

private:
};
