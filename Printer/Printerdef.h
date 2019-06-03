
const int  PRINT_MOVEDOWN_ROWNUM = 0x08;   //打印机打印完毕走纸行数
const int  Printer_WAIT_TIME = 30000;     //打印机等待时间
const int  BOX_WAIT_TIME = 5000;     //打开钱箱等待时间

typedef enum _err_detail_Printer {
	Printer_SET_PARAM_ERR   = 1,	      // 设定值：参数异常
	Printer_FUNC_PARAM_ERR  = 2,	      // 函数参数异常
	Printer_SEQUENCE_ERR	= 3,	      // 函数调用顺序异常
	Printer_FUNC_ERR_RETURN	= 4,	      // 函数返回异常
	Printer_LOGIC_ERR	    = 5,	      // 逻辑异常
	Printer_OTHER_ERR	    = 6,	      // 异常发生

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
	ERROR_Printer_CLOSE = 213 ,           //关闭时　关闭打印机串口错误
} ERR_DETAIL_Printer;

static const unsigned long  COMMAND_Printer_CONNECT = 0x5101  ; //打开串口
static const unsigned long  COMMAND_Printer_PRINT = 0x5102  ;   //打印命令
static const unsigned long  COMMAND_Printer_CLOSE = 0x5103  ;   //关闭打印机
static const unsigned long  COMMAND_Printer_OPENBOX = 0x5104  ;  //关闭钱箱
