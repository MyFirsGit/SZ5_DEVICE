#pragma once
#include "afx.h"
#include "SysException.h"
#include "THCOMLIBExp.h"
/**
@brief	外部读写器硬件异常类
*/

class THCOMLIB_DECLSPEC CTHHardwareException :
    public CSysException
{
public:
    static const int MODULE_ID = 12401;     // 模块号

    CTHHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CTHHardwareException(void);

	typedef enum _detail_error_code {
		TH_HARDWERE_ERROR = 1000,
		TH_COMMOND_ERROR  = 1001,
		TH_STATUS_ERROR   = 1002,

		TH_CARD_MODULE_NOT_STANDBY = 1100,		// 票卡模块未到位
		TH_CARD_NOT_INSTALL		   = 1101,		// 卡箱未安装
		TH_WASTED_BOX_NOT_INSTALL  = 1102,		// 废票箱未安装
		TH_ILLGIAL_INSTALL_ERROR   = 1103,		// 箱子非法安装
		TH_WASTED_BOX_ALEADY_FULL  = 1104,		// 废票箱已满
		TH_CARD_BOX_EMPTY		   = 1105,		// 票箱A与B已空
		TH_ERROR_TICKET_BOX1_EMPTY = 52064,		// 指定票箱1，票箱1无票，导致不能发送
		TH_ERROR_TICKET_BOX2_EMPTY = 52065,		// 指定票箱2，票箱2无票，导致不能发送

		ERROR_TH_NOT_STANDBY			= 2000,		// 整机未到位
		ERROR_TH_TICKET_BOX_NOT_INSTALL = 2001,		// 票箱未到位异常
		ERROR_TH_SENSOR					= 2002,		// 传感器异常
		ERROR_TH_ELETROMAGNET			= 2003,		// 电磁铁异常
		ERROR_TH_CATCHCARD				= 2004,		// 走票故障
		ERROR_TH_MOTOR					= 2005,		// 电机故障
		ERROR_TH_BOX1_COVER_CLOSE		= 2006,		// 票箱1盖板未打开
		ERROR_TH_BOX2_COVER_CLOSE		= 2007,		// 票箱2盖板未打开
		ERROR_TH_WASTE_BOX_NOT_INSTALL	= 2008,		// 废票箱不在位
		ERROR_TH_WASTE_BOX_COVER_CLOSE	= 2009,		// 废票箱盖板未打开
		ERROR_TH_WASTE_BOX_FULL			= 2010,		// 废票箱满
		ERROR_TH_ILLGIAL_INSTALL		= 2011,		// 票箱或废票箱安装非法
		ERROR_TH_ILLGIAL_COUNT			= 2012,		// 票箱安装数量与实际不符
		ERROR_TH_SEND_CARD_OUT_FAILED	= 2013,		// 票卡发送失败
		ERROR_TH_CYCLE_CARD_FAILED		= 2014,		// 票卡进废票箱失败
		ERROR_TH_CARD_BOX_EMPTY			= 2015,		// 票箱已空了
		ERROR_TH_CARD_OUT_JAM			= 2016,		// 出票口卡票故障
	} DETAIL_ERROR_CODE;

private:
};
