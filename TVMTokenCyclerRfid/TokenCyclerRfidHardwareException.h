#pragma once
#include "afx.h"
#include "SysException.h"
#include "TokenCyclerRfidExp.h"

/**
 @brief		token回收箱RFID模块异常类
*/

class CS_API CTokenCyclerRfidHardwareException :
	public CSysException
{
public:
	static const int MODULE_ID = 11802;     // 模块号（注意，这里定义的异常类，实际使用中不抛出异常，仅仅是定义错误码）

	typedef enum hardware_err_code {
		CS_ERR_NOT_INIT					= 6500,						// 自定义，未初始化错误
		// 以下是硬件模块的错误码
		//mifare error												// check write Error(下面）
		CTCRfid_MI_OK					= 0,						// CTCRfid_MIS_CHK_OK          0
		CTCRfid_MI_NOTAGERR				= 1,						// CTCRfid_MIS_CHK_FAILED      1
		CTCRfid_MIS_CHK_COMPERR			= 2,						// CTCRfid_MIS_CHK_COMPERR	   2
		CTCRfid_MI_EMPTY				= 3,
		CTCRfid_MI_AUTHERR				= 4,
		CTCRfid_MI_PARITYERR			= 5,
		CTCRfid_MI_CODEERR				= 6,
		CTCRfid_MI_SERNRERR				= 8,
		CTCRfid_MI_NOTAUTHERR			= 10,
		CTCRfid_MI_BITCOUNTERR			= 11,
		CTCRfid_MI_BYTECOUNTERR			= 12,
		CTCRfid_MI_TRANSERR				= 14,
		CTCRfid_MI_WRITEERR				= 15,
		CTCRfid_MI_INCRERR				= 16,
		CTCRfid_MI_DECRERR				= 17,
		CTCRfid_MI_READERR				= 18,
		CTCRfid_MI_QUIT					= 30,
	}HARDWARE_ERR;	

    CTokenCyclerRfidHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CTokenCyclerRfidHardwareException();

};
