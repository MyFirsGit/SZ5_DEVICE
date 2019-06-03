#pragma once
#include "afx.h"
#include "SysException.h"
#include "RWCOMLIBExp.h"
/**
@brief	Íâ²¿¶ÁÐ´Æ÷Ó²¼þÒì³£Àà
*/

class RWCOMLIB_DECLSPEC CRWHardwareException :
    public CSysException
{
public:
    static const int MODULE_ID = 29993;     // Ä£¿éºÅ
//
//    CRWHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
//    ~CRWHardwareException(void);
//
//private:

	public:
	static const int BOM_DESKTOP_RW_MODULE_ID					= 11101;    // BOM×ÀÃæ¶ÁÐ´Æ÷
	static const int BOM_CARD_RW_MODULE_ID						= 11103;    // BOMÆ±¿¨Ä£¿éÓÃ¶ÁÐ´Æ÷
	static const int BOM_TOKEN_RW_MODULE_ID						= 11105;    // BOM TokenÄ£¿éÓÃ¶ÁÐ´Æ÷
	static const int TVM_RECHARGE_RW_MODULE_ID					= 11107;    // TVM³äÖµÓÃ¶ÁÐ´Æ÷
	static const int TVM_CARD_RW_MODULE_ID						= 11109;    // TVMÆ±¿¨Ä£¿éÓÃ¶ÁÐ´Æ÷
	static const int TVM_TOKEN_RW_MODULE_ID						= 11111;    // TVM TokenÄ£¿éÓÃ¶ÁÐ´Æ÷
	static const int ES_TOKEN_RW1_MODULE_ID						= 11113;	// Token ES¶ÁÐ´Æ÷1
	static const int ES_TOKEN_RW2_MODULE_ID						= 11115;	// Token ES¶ÁÐ´Æ÷2
	static const int ES_TOKEN_RW3_MODULE_ID						= 11117;	// Token ES¶ÁÐ´Æ÷3
	static const int ES_CARD_RW_MODULE_ID						= 11123;	// Token ES¶ÁÐ´Æ÷3
	static const int PM_RW_MODULE_ID							= 11121;	// PM¶ÁÐ´Æ÷
	static const int TCM_RW_MODULE_ID							= 11150;	// TCM¶ÁÐ´Æ÷
	static const int AGM_CARD_RECYCLE_RW_MODULE_ID				= 10370;    // AGM Æ±¿¨»ØÊÕ¶ÁÐ´Æ÷
	static const int AGM_TOKEN_RECYCLE_RW_MODULE_ID				= 10380;    // AGM Token»ØÊÕ¶ÁÐ´Æ÷
	static const int AGM_ENTRY_RW_MODULE_ID						= 10390;    // AGM½øÕ¾Ë¢¿¨¶ÁÐ´Æ÷
	static const int AGM_EXIT_RW_MODULE_ID						= 10400;    // AGM³öÕ¾Ë¢¿¨¶ÁÐ´Æ÷

    CRWHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	CRWHardwareException(RW_TYPE rwType,long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CRWHardwareException(void);

private:
	static long GetModuleID(RW_TYPE rwType);
};
