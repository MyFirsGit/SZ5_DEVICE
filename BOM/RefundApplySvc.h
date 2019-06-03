#include "BOMForeService.h"
#include "ReceptionStaticInfo.h"
#pragma once

static const TCHAR* const WM_TO_REFUND_APPLY_DLG				=  _T("WM_TO_REFUND_APPLY_DLG");                // 退款申请画面
static const TCHAR* const WM_TO_REFUND_APPLY_NOT_DLG			=  _T("WM_TO_REFUND_APPLY_NOT_DLG");			// 申请不可画面
static const TCHAR* const WM_TO_REFUND_APPLY_FINISH_DLG      =  _T("WM_TO_REFUND_APPLY_FINISH_DLG");			// 申请完成画面
static const TCHAR* const WM_TO_REFUND_BASE_APPLY_DLG		=  _T("WM_TO_REFUND_BASE_APPLY_DLG");                 // 退款基本画面
static const TCHAR* const WM_TO_REFUND_FINISH_APPLY_DLG		=  _T("WM_TO_REFUND_FINISH_APPLY_DLG");				// 退款完成画面
static const TCHAR* const WM_TO_REFUND_QUERY_FINISH_DLG		=  _T("WM_TO_REFUND_QUERY_FINISH_DLG");				//查询完成画面

class CRefundApplySvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	typedef enum _dialog_group{
		DIALOG_GROUP_WAIT = 1,                       // 等待读卡画面
		DIALOG_GROUP_APPLY,                          // 退款申请画面
		DIALOG_GROUP_APPLY_NOT,						 // 申请不可画面
		DIALOG_GROUP_APPLY_FINISH,                   // 申请结束画面
		DIALOG_GROUP_QUERY,							 // 退款申请查询画面组
		DIALOG_GROUP_QUERY_FINISH,					 // 退款申请查询完成画面组
		DIALOG_GROUP_BASE,                           // 退款基本画面
		DIALOG_GROUP_FINISH,                         // 退款结束画面
	} DIALOG_GROUP;
	


	typedef struct _tagModel
	{
		bool isQuerySvc;													// 是否点击的查询

		BYTE cardPHType;
		CString strTicketNum;													// 票面卡号
		CString strTicketType_CN;                                                // 票卡中文名
		CString strTicketType_EN;                                                // 票卡英文名
		CString strValidDate;                                                    // 票卡有效期
		long ticketBalance;													// 卡内余额
		long ticketDeposit;													// 押金
		long refundBalance;
		long refundDeposit;
		CString strUserName;														// 姓名
		Sex_t sexType;														// 性别
		CERTIFICATE_TYPE certificateType;										// 证件类型
		CString strCertificateNumber;											// 证件号码
		WORD cityCode;														//城市代码
		WORD industryCode;													//行业代码
		TicketComHdr_t ticketComHdr;											//票卡公共域

		_DATE_TIME transTime;													// 交易时间	
		RefundApplyType applyType;														// 申请类型
		RefundReasonType refundReason;													// 退款原因
		RefundApplyReasonType applyReason;														// 申请原因
		BYTE cardType;														// 卡片种类
		BYTE cardAppType;														// 车票产品种类
		CString strSuspendBillNum;												// 挂失单据号
		CString strApplyBillNum;													// 申退收据号

		_tagModel(){

			strTicketNum = "";
			strTicketType_CN = "";
			strTicketType_EN = "";
			strValidDate = "";

			ticketBalance = 0;
			ticketDeposit = 0;
			refundBalance = 0;
			refundDeposit = 0;
			sexType = SEX_MALE;
			certificateType = CERTIFICATE_ID_CARD_MAINLAND;
			
			applyType = APPLY_TYPE_GOOD_CARD;
			refundReason = REFUND_REASON_PASSAGER;
			applyReason = APPLY_REASON_BAD_CARD;
			cardType = 0;
			cardAppType = 0;
			strSuspendBillNum = "";
			strApplyBillNum = "";
		}
	}Model;

	CRefundApplySvc();
	~CRefundApplySvc();
	void SetIsQuery(bool isQuery);
	long RefundApply();														// 退款申请
	BYTE QueryRefundApplyResult();											// 查询退款申请结果
	long Refund();                                                         // 退款
	CString GetTpuErrorInfo(int errCode);									// 获得TPU出错信息
	void PrinterPrint(bool isApply);                                        // 打印
	bool CheckOperatorIdAndPwd(int nOperatorID,CString sPwd);				// 验证操作员ID和密码
	
	Model& GetDataModel();

protected:
	void OnStart();
	
private:
	void OnReadCard(UINT errCode, CRWReadCard* rwReadCard);
	long BlockCard(RW_BUSSINESS_RESPONSE& response);
	void SaveSCData(char flag);                                                      // 保存SC数据

	Model m_DataModel;
};