#include "BOMForeService.h"
#include "ReceptionStaticInfo.h"
#pragma once

static const TCHAR* const WM_TO_REFUND_BASE_DLG				=  _T("WM_TO_REFUND_BASE_DLG");                           // 退款基本画面
static const TCHAR* const WM_TO_REFUND_NOT_DLG				=  _T("WM_TO_REFUND_NOT_DLG");                            // 退款不能画面
static const TCHAR* const WM_TO_REFUND_FINISH_DLG			=  _T("WM_TO_REFUND_FINISH_DLG");						  // 退款完成画面
static const TCHAR* const WM_BASE_TO_REFUND_NOT_DLG			=  _T("WM_BASE_TO_REFUND_NOT_DLG");

class CRefundSvc : public CBOMForeService
{
    DECLARE_MESSAGE_MAP()
public:
	typedef enum _dialog_group{
		DIALOG_GROUP_WAIT = 1,                       // 等待读卡画面
		DIALOG_GROUP_BASE,                           // 退款基本画面
		DIALOG_GROUP_REFUND_NOT,                     // 退款不可画面
		DIALOG_GROUP_FINISH,                         // 退款结束画面
	} DIALOG_GROUP;
	typedef struct _tagModel
	{
		BYTE refundReason;													// 退款原因
		BYTE cardPHType;
		BYTE cardType;														// 卡片种类
		BYTE cardAppType;														// 车票产品种类
		ProductCategory_t cardCategory;											// 卡片产品类型
		CString strTicketNum;													// 票面卡号
		CString strTicketType_CN;                                                // 票卡中文名
		CString strTicketType_EN;                                                // 票卡英文名
		CString strValidDate;                                                    // 票卡有效期
		long ticketBalance;													// 卡内余额
		long deposit;															// 押金
		long refundDeposit;													// 退款押金
		long refundValue;                                                      // 退款金额
		_DATE_TIME transTime;

		CCPUCard* pCPUCard;
		CULCard* pULCard;

		_tagModel()
		{
			refundReason = 0;
			cardPHType = 0;
			cardType = 0;
			cardAppType = 0;
			cardCategory = PURSE;
			strTicketNum = "";
			strTicketType_CN = "";
			strTicketType_EN = "";
			strValidDate = "";
			ticketBalance = 0;
			deposit = 0;
			refundDeposit = 0;
			refundValue = 0;
			pCPUCard = new CCPUCard;
			pULCard = new CULCard;
		}
		~_tagModel()
		{
			delete pCPUCard;
			pCPUCard = NULL;
			delete pULCard;
			pULCard = NULL;
		}

	}Model;
	CRefundSvc();
	~CRefundSvc();

	bool CheckOperatorIdAndPwd(int nOperatorID,CString sPwd);				// 验证操作员ID和密码
	
	Model& GetDataModel();
	
	void OnOK();
private:
	void OnReadCard(UINT errCode, CRWReadCard* rwReadCard);
	long BlockCard(RW_BUSSINESS_RESPONSE response);
	void SaveSCData();                                                      // 保存SC数据
	void OnError(CString msg,CString receiptMsg = _T(""),CString receiptENMsg = _T(""));
	void PrinterPrint();													// 打印
	Model m_DataModel;

	
};