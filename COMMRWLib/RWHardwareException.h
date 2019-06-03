#pragma once
#include "afx.h"
#include "SysException.h"
#include "RWCOMLIBExp.h"
/**
@brief	�ⲿ��д��Ӳ���쳣��
*/

class RWCOMLIB_DECLSPEC CRWHardwareException :
    public CSysException
{
public:
    static const int MODULE_ID = 29993;     // ģ���
//
//    CRWHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
//    ~CRWHardwareException(void);
//
//private:

	public:
	static const int BOM_DESKTOP_RW_MODULE_ID					= 11101;    // BOM�����д��
	static const int BOM_CARD_RW_MODULE_ID						= 11103;    // BOMƱ��ģ���ö�д��
	static const int BOM_TOKEN_RW_MODULE_ID						= 11105;    // BOM Tokenģ���ö�д��
	static const int TVM_RECHARGE_RW_MODULE_ID					= 11107;    // TVM��ֵ�ö�д��
	static const int TVM_CARD_RW_MODULE_ID						= 11109;    // TVMƱ��ģ���ö�д��
	static const int TVM_TOKEN_RW_MODULE_ID						= 11111;    // TVM Tokenģ���ö�д��
	static const int ES_TOKEN_RW1_MODULE_ID						= 11113;	// Token ES��д��1
	static const int ES_TOKEN_RW2_MODULE_ID						= 11115;	// Token ES��д��2
	static const int ES_TOKEN_RW3_MODULE_ID						= 11117;	// Token ES��д��3
	static const int ES_CARD_RW_MODULE_ID						= 11123;	// Token ES��д��3
	static const int PM_RW_MODULE_ID							= 11121;	// PM��д��
	static const int TCM_RW_MODULE_ID							= 11150;	// TCM��д��
	static const int AGM_CARD_RECYCLE_RW_MODULE_ID				= 10370;    // AGM Ʊ�����ն�д��
	static const int AGM_TOKEN_RECYCLE_RW_MODULE_ID				= 10380;    // AGM Token���ն�д��
	static const int AGM_ENTRY_RW_MODULE_ID						= 10390;    // AGM��վˢ����д��
	static const int AGM_EXIT_RW_MODULE_ID						= 10400;    // AGM��վˢ����д��

    CRWHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	CRWHardwareException(RW_TYPE rwType,long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CRWHardwareException(void);

private:
	static long GetModuleID(RW_TYPE rwType);
};
