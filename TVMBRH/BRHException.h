#pragma once
#include "afx.h"
#include "SysException.h"
#include "BRHExp.h"

// BRH�ײ������
//enum _enum_brh_op_err{
//	BRH_NO_ERROR						= 0,			/**< Returned by synchronious method when there is no error */
//	// API error �� From 1 to 1000 ��
//	BRH_API_NOT_OPENED					= 0x01,			/**< The library was not opened by a previous bnr_Open= , call */
//	BRH_API_FILE_NOT_FOUND				= 0x02,			/**< A file given in parameter can not be found */
//	BRH_API_NO_MEMORY					= 0x03,			/**< Insufficient memory available */
//	BRH_API_INVALID_PARAMETER			= 0x04,			/**< One of the parameter given in the function call is invalid */
//	BRH_API_INVALID_HANDLE				= 0x05,			/**< The API was not able to get a valid hande from Windows system. */
//	BRH_API_MISSING_DEPENDENCY			= 0x06,			/**< The API was not able to delay-load something it needs = e.g. a DLL,. */
//	BRH_API_ACCESS_DENIED				= 0x07,			/**< Acces of this command is denied. */
//
//	// XFS error �� From 1001 to 10000 ��
//	//BRH_ERR_NOT_USED					= -1,			/**< Field not used. */ 
//	BRH_ERR_TIMEOUT						= 9999,			/** �Զ��壬�첽������ʱ. */ 
//	BRH_ERR_ILLEGAL						= 1010,			/**< Illegal request. */
//	BRH_ERR_NOHARDWARE					= 1011,			/**< The module is not connected to the BNR. */
//	BRH_ERR_NOEXIST						= 1013,			/**< The requested item =reports or other data, does not exist. */
//	BRH_ERR_FAILURE						= 1015,			/**< The operation failed. */
//	BRH_ERR_BUSY						= 1017,			/**< The BNR is busy =moving or about to move bill,. */
//	BRH_ERR_PARAMETER_INVALID			= 1018,			/**< One of the parameters given was invalid. */
//	BRH_ERR_NOT_SUPPORTED				= 1022,			/**< The operation is not supported by this firmware. */
//	BRH_ERR_CANCELLED					= 1021,			/**< The operation was cancelled. */
//	BRH_ERR_FIRMWARE					= 1023,			/**< Error during firmware update or no runnable firmware in device. */
//	BRH_ERR_HARDWAREERROR				= 1025,			/**< Error occurred that is hardware related. */
//	/* ----Cash Module error---- */
//	BRH_ERR_CDR_OFFSET                  = 6071,
//	BRH_ERR_CDR_CASHIN_ACTIVE           = 6072,			/**< The device has already issued a cashInStart command. */
//	BRH_ERR_CDR_CASH_DEVICE_ERROR       = 6073,			/**< A selected cash device caused an error. */
//	BRH_ERR_CDR_CASH_UNIT_ERROR         = 6074,			/**< A selected cash unit caused an error. */
//	BRH_ERR_CDR_INVALID_CASH_UNIT       = 6080,			/**< Invalid cash unit ID. */
//	BRH_ERR_CDR_INVALID_CURRENCY        = 6081,			/**< The currency is not available = defined, in the BNR. */
//	BRH_ERR_CDR_INVALID_MIXNUMBER       = 6085,			/**< The number refers to a defined and reserved mix algorithm. */
//	BRH_ERR_CDR_NOT_DISPENSABLE         = 6087,			/**< The amount is not dispensable. */
//	BRH_ERR_CDR_NO_BILLS                = 6088,			/**< There were no bills on the stacker to present.*/
//	BRH_ERR_CDR_NO_CASHIN_STARTED       = 6089,			/**< CashInStart was not called. */
//	BRH_ERR_CDR_TOO_MANY_BILLS          = 6092,			/**< The request would require too many bills to be dispensed. */
//	BNRBRH_ERR_CDR_USER_ERROR           = 2001,         /**< A person has stopped the device operation. i.e. manupulating the outlet flap or inserting a bill at the inlet when it is not expected. */
//
//	// USB error �� From 10001 ��
//	BRH_USB_CRC                     = 10000 + 0x01,
//	BRH_USB_BTSTUFF                 = 10000 + 0x02,
//	BRH_USB_DATA_TOGGLE_MISMATCH    = 10000 + 0x03,
//	BRH_USB_STALL_PID               = 10000 + 0x04,
//	BRH_USB_DEV_NOT_RESPONDING      = 10000 + 0x05,
//	BRH_USB_PID_CHECK_FAILURE       = 10000 + 0x06,
//	BRH_USB_UNEXPECTED_PID          = 10000 + 0x07,
//	BRH_USB_DATA_OVERRUN            = 10000 + 0x08,
//	BRH_USB_DATA_UNDERRUN           = 10000 + 0x09,
//	BRH_USB_BUFFER_OVERRUN          = 10000 + 0x0C,
//	BRH_USB_BUFFER_UNDERRUN         = 10000 + 0x0D,
//	BRH_USB_NOT_ACCESSED            = 10000 + 0x0F,
//	BRH_USB_FIFO                    = 10000 + 0x10,
//	BRH_USB_ENDPOINT_HALTED         = 10000 + 0x30,
//	BRH_USB_NO_MEMORY               = 10000 + 0x0100,
//	BRH_USB_INVALID_URB_FUNCTION    = 10000 + 0x0101,
//	BRH_USB_INVALID_PARAMETER       = 10000 + 0x0102,
//	BRH_USB_ERROR_BUSY              = 10000 + 0x0103,
//	BRH_USB_REQUEST_FAILED          = 10000 + 0x0104,
//	BRH_USB_INVALID_PIPE_HANDLE     = 10000 + 0x0105,
//	BRH_USB_NO_BANDWIDTH            = 10000 + 0x0106,
//	BRH_USB_INTERNAL_HC_ERROR       = 10000 + 0x0107,
//	BRH_USB_ERROR_SHORT_TRANSFER    = 10000 + 0x0108,
//	BRH_USB_BAD_START_FRAME         = 10000 + 0x0109,
//	BRH_USB_ISOCH_REQUEST_FAILED    = 10000 + 0x010A,
//	BRH_USB_FRAME_CONTROL_OWNED     = 10000 + 0x010B,
//	BRH_USB_FRAME_CONTROL_NOT_OWNED = 10000 + 0x010C,
//	BRH_USB_CANCELED                = 10000 + 0x0200,
//	BRH_USB_CANCELING               = 10000 + 0x0201,
//	BRH_USB_ALREADY_CONFIGURED      = 10000 + 0x0202,  /**< Device is already configured. */
//	BRH_USB_UNCONFIGURED            = 10000 + 0x0203,  /**< Device is unconfigured. */
//	BRH_USB_NO_SUCH_DEVICE          = 10000 + 0x0204,  /**< The specified Device doesn't exists. */
//	BRH_USB_DEVICE_NOT_FOUND        = 10000 + 0x0205,  /**< The specified Device not found in system. */
//	BRH_USB_NOT_SUPPORTED           = 10000 + 0x0206,  /**< Operation isn't supported by Driver */
//	BRH_USB_IO_PENDING              = 10000 + 0x0207,  /**< I/O operation is still in progress */
//	BRH_USB_IO_TIMEOUT              = 10000 + 0x0208,  /**< Request timeout */
//	BRH_USB_DEVICE_REMOVED          = 10000 + 0x0209,  /**< The USB device was removed = power off or disconnected,. */
//	BRH_USB_PIPE_NOT_LINKED         = 10000 + 0x020A,  /**< Pipe not linked */
//	BRH_USB_CONNECTED_PIPES         = 10000 + 0x020B,  /**< Device cannot be reconfigured because pipe connections already exist. */
//	BRH_USB_DEVICE_LOCKED           = 10000 + 0x020C,  /**< The device is already locked by another application. */
//}ENUM_BRH_ERR;




/**
@brief	BRH�쳣��
*/

class BRH_API CBRHException : public CSysException
{
public:
	static const int MODULE_ID = 11702;     // ģ���	//lichao

	typedef enum _detail_error_code{
		// API error �� From 1 to 1000 ��
		API_NOT_OPENED					= 0x01,			/**< The library was not opened by a previous bnr_Open= , call */
		API_FILE_NOT_FOUND				= 0x02,			/**< A file given in parameter can not be found */
		API_NO_MEMORY					= 0x03,			/**< Insufficient memory available */
		API_INVALID_PARAMETER			= 0x04,			/**< One of the parameter given in the function call is invalid */
		API_INVALID_HANDLE				= 0x05,			/**< The API was not able to get a valid hande from Windows system. */
		API_MISSING_DEPENDENCY			= 0x06,			/**< The API was not able to delay-load something it needs = e.g. a DLL,. */
		API_ACCESS_DENIED				= 0x07,			/**< Acces of this command is denied. */
		ERR_OTHERS						= 9998,			// �Զ��壬��������
		ERR_TIMEOUT						= 9999,			/** �Զ��壬�첽������ʱ. */ 
	}DETAIL_ERROR_CODE;

	CBRHException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CBRHException(void);

private:
};
