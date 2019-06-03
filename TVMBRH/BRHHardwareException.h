#pragma once
#include "afx.h"
#include "SysException.h"
#include "BRHExp.h"
/**
@brief	BRH硬件异常类
*/
class BRH_API CBRHHardwareException : public CSysException
{

public:
	static const int MODULE_ID = 11701;     // 模块号
	typedef enum hardware_err_code {
		// XFS error （ From 1001 to 10000 ）
		//ERR_NOT_USED					= -1,			/**< Field not used. */ 
		//ERR_TIMEOUT						= 9999,			/** 自定义，异步操作超时. */ 
		ERR_ILLEGAL						= 1010,			/**< Illegal request. */
		ERR_NOHARDWARE					= 1011,			/**< The module is not connected to the BNR. */
		ERR_NOEXIST						= 1013,			/**< The requested item =reports or other data, does not exist. */
		ERR_FAILURE						= 1015,			/**< The operation failed. */
		ERR_BUSY						= 1017,			/**< The BNR is busy =moving or about to move bill,. */
		ERR_PARAMETER_INVALID			= 1018,			/**< One of the parameters given was invalid. */
		ERR_NOT_SUPPORTED				= 1022,			/**< The operation is not supported by this firmware. */
		ERR_CANCELLED					= 1021,			/**< The operation was cancelled. */
		ERR_FIRMWARE					= 1023,			/**< Error during firmware update or no runnable firmware in device. */
		ERR_HARDWAREERROR				= 1025,			/**< Error occurred that is hardware related. */
		// 自定义错误
		ERR_CASHBOX_NOT_INSTALLED		= 1500,			// 回收箱不在位
		ERR_CASHCHANGEBOX_NOT_INSTALLED	= 1501,			// 找零箱不在位
		ERR_CASHBOX_IGILL_INSTALL		= 1502,			// 回收箱非法安装
		ERR_CASHCHANGEBOX_IGILL_INSTALL	= 1503,			// 找零箱非法安装
		BANKNOTE_ERROR_MODULE_NOT_STANDBY = 1504,		// 纸币模块不到位
		/* ----Cash Module error---- */
		ERR_CDR_OFFSET                  = 6071,
		ERR_CDR_CASHIN_ACTIVE           = 6072,			/**< The device has already issued a cashInStart command. */
		ERR_CDR_CASH_DEVICE_ERROR       = 6073,			/**< A selected cash device caused an error. */
		ERR_CDR_CASH_UNIT_ERROR         = 6074,			/**< A selected cash unit caused an error. */
		ERR_CDR_INVALID_CASH_UNIT       = 6080,			/**< Invalid cash unit ID. */
		ERR_CDR_INVALID_CURRENCY        = 6081,			/**< The currency is not available = defined, in the BNR. */
		ERR_CDR_INVALID_MIXNUMBER       = 6085,			/**< The number refers to a defined and reserved mix algorithm. */
		ERR_CDR_NOT_DISPENSABLE         = 6087,			/**< The amount is not dispensable. */
		ERR_CDR_NO_BILLS                = 6088,			/**< There were no bills on the stacker to present.*/
		ERR_CDR_NO_CASHIN_STARTED       = 6089,			/**< CashInStart was not called. */
		ERR_CDR_TOO_MANY_BILLS          = 6092,			/**< The request would require too many bills to be dispensed. */
		BNRERR_CDR_USER_ERROR           = 2001,         /**< A person has stopped the device operation. i.e. manupulating the outlet flap or inserting a bill at the inlet when it is not expected. */
	
		BNR_USB_ERR_CONNECT				= 10000 + 0x0FFF,// 设备连接错误（自定义，用此错误码涵盖下面这四个错误）。
																			// USB_NO_SUCH_DEVICE
																			// USB_DEVICE_NOT_FOUND
																			// USB_DEVICE_REMOVED
																			// USB_DEVICE_LOCKED
		BNR_USB_ERR_OTHER				= 10000 + 0x0FFE,//	其他错误（自定义，用此错误码涵盖除了上面4个的所有USB错误）。

		// USB error （ From 10001 ）
		USB_CRC                     = 10000 + 0x01,
		USB_BTSTUFF                 = 10000 + 0x02,
		USB_DATA_TOGGLE_MISMATCH    = 10000 + 0x03,
		USB_STALL_PID               = 10000 + 0x04,
		USB_DEV_NOT_RESPONDING      = 10000 + 0x05,
		USB_PID_CHECK_FAILURE       = 10000 + 0x06,
		USB_UNEXPECTED_PID          = 10000 + 0x07,
		USB_DATA_OVERRUN            = 10000 + 0x08,
		USB_DATA_UNDERRUN           = 10000 + 0x09,
		USB_BUFFER_OVERRUN          = 10000 + 0x0C,
		USB_BUFFER_UNDERRUN         = 10000 + 0x0D,
		USB_NOT_ACCESSED            = 10000 + 0x0F,
		USB_FIFO                    = 10000 + 0x10,
		USB_ENDPOINT_HALTED         = 10000 + 0x30,
		USB_NO_MEMORY               = 10000 + 0x0100,
		USB_INVALID_URB_FUNCTION    = 10000 + 0x0101,
		USB_INVALID_PARAMETER       = 10000 + 0x0102,
		USB_ERROR_BUSY              = 10000 + 0x0103,
		USB_REQUEST_FAILED          = 10000 + 0x0104,
		USB_INVALID_PIPE_HANDLE     = 10000 + 0x0105,
		USB_NO_BANDWIDTH            = 10000 + 0x0106,
		USB_INTERNAL_HC_ERROR       = 10000 + 0x0107,
		USB_ERROR_SHORT_TRANSFER    = 10000 + 0x0108,
		USB_BAD_START_FRAME         = 10000 + 0x0109,
		USB_ISOCH_REQUEST_FAILED    = 10000 + 0x010A,
		USB_FRAME_CONTROL_OWNED     = 10000 + 0x010B,
		USB_FRAME_CONTROL_NOT_OWNED = 10000 + 0x010C,
		USB_CANCELED                = 10000 + 0x0200,
		USB_CANCELING               = 10000 + 0x0201,
		USB_ALREADY_CONFIGURED      = 10000 + 0x0202,  /**< Device is already configured. */
		USB_UNCONFIGURED            = 10000 + 0x0203,  /**< Device is unconfigured. */
		USB_NO_SUCH_DEVICE          = 10000 + 0x0204,  /**< The specified Device doesn't exists. */
		USB_DEVICE_NOT_FOUND        = 10000 + 0x0205,  /**< The specified Device not found in system. */
		USB_NOT_SUPPORTED           = 10000 + 0x0206,  /**< Operation isn't supported by Driver */
		USB_IO_PENDING              = 10000 + 0x0207,  /**< I/O operation is still in progress */
		USB_IO_TIMEOUT              = 10000 + 0x0208,  /**< Request timeout */
		USB_DEVICE_REMOVED          = 10000 + 0x0209,  /**< The USB device was removed = power off or disconnected,. */
		USB_PIPE_NOT_LINKED         = 10000 + 0x020A,  /**< Pipe not linked */
		USB_CONNECTED_PIPES         = 10000 + 0x020B,  /**< Device cannot be reconfigured because pipe connections already exist. */
		USB_DEVICE_LOCKED           = 10000 + 0x020C,  /**< The device is already locked by another application. */
	}HARDWARE_ERR;	//错误详细，参考文档

	CBRHHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CBRHHardwareException(void);

private:
};