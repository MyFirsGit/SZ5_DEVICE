#pragma once

#include "BOMForeService.h"
#include "TVMFailureRefundBaseOfficeDlg.h"
#include "ReceptionDlg.h"
#include "TVMFailureRefundReceptionDlg.h"

static const char			FAILURE_INFO_SIZE			=	3;			// 站员画面显示信息的行数
//static const char			MAX_RIGHT_LEN_7				=	7;			// 基本画面基本信息区右列最大缩进半角字符数为7
//static const char			MAX_RIGHT_LEN_5				=	5;			// 基本画面基本信息区右列最大缩进半角字符数为5
static const char			RECEPTION_INFO_SIZE			=	4;			// 基本信息区显示行数

/************************************************************************/
/*   5.80 BOM Refund TVM ticket                                         */
/************************************************************************/
typedef struct _brefundt_ticket 
{   
	CString TVM_ERROR_CODE;                 // TVM Error code
	_DATE_TIME ErrorDate;                   // Date and Time of the Error
	WORD Transaction_amount;                // Transaction_amount

	_brefundt_ticket(){
		TVM_ERROR_CODE = "";
		Transaction_amount = 0;
	}
}BREFUNDT_TICKET,*LPBREFUNDT_TRANS;


class CTVMFailureRefundSvc : public CBOMForeService
{
public:
	CTVMFailureRefundSvc();
	~CTVMFailureRefundSvc();

	void InitFailureInfo();												// 初始化故障信息数据构成
	PLABEL_GROUP GetFailureInfo();										// 取得故障信息
	void GetReceptionInfo(MSG_NAME_GROUP*&);							// 取得乘客画面信息
	void SetFailureInfo(unsigned char ,const CString&);					// 设置故障信息
	void SetRefundMoney(DWORD);											// 设置退款金额

	void PrintServiceInfo(CString strDeviceNo,CString strTicketNo);										// 打印方法
	void RefreshReceptionOperArea();									// 刷新乘客画面操作区
	void DoDetailInfo(int amount);												// 一件明细处理
	void SetFailureDate(_DATE_TIME);									// 设置故障发生日期
	void SetErrorCode(CString);											// 设置交易编号
	void SetReceptionOperInfo();										// 设置乘客画面信息
	void SetMoneyUnit();												// 设置金额单位

private:
	LABEL_GROUP m_failureInfo[FAILURE_INFO_SIZE];						// 画面显示的故障信息
	MSG_NAME_GROUP m_receptionOperInfo[RECEPTION_INFO_SIZE];			// 乘客画面基本数据	
	_DATE_TIME m_date;													// 故障发生日期
	CString m_errorCode;												// 交易编号
	DWORD m_refundMoney;												// 退款金额（分为单位）
	DWORD m_transactionNum;												// 打印凭证号

	CECTCard m_ECTCardInfo;	                                            // 一卡通信息结构体

	long m_ChargePrice;                                                 // 应收金额

	typedef enum _dialog_group{
		DIALOG_GROUP_BASE	= 1,
		DIALOG_GROUP_OVER
	} DIALOG_GROUP;

};
