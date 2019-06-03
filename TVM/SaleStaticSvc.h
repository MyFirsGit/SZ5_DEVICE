#pragma once
#include "TVMForeService.h"
#include "DataHelper.h"
#define NULL_STR _T("")	// 空字符串
/**
@brief 维护业务Service
*/
class CSaleStaticSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CSaleStaticSvc();					// 构造函数
	virtual ~CSaleStaticSvc();			// 析构函数

	typedef enum _dialog_group {
		DIALOG_GROUP_OVERVIEW	= 1,
		DIALOG_GROUP_DETAIL
	} DIALOG_GROUP;

	// 销售统计数据模型
	typedef struct _sale_statci_data_model_{
		/******************统计主界面*************************/
		CString strDateFrom;	// 开始统计时间
		CString strDateTo;		// 结束统计时间

		DWORD dwFailedAmount;		// 故障金额
		DWORD dwFailedCount;		// 故障笔数
		DWORD dwFailedTotalAmount;	// 故障总金额

		DWORD dwSingleTicketAmount;	 // 单程票金额
		DWORD dwSingleTicketTrsCount;// 单程票交易笔数
		DWORD dwSingleTicketCount;	 // 单程票售票张数

		DWORD dwSaleTotalAmount;	// 售票小计：金额
		DWORD dwSaleTotalTrsCount;	// 售票小计：笔数
		DWORD dwSaleTotalCount;		// 售票小计：张数


		DWORD dwRechargeAmount;		// 充值金额
		DWORD dwRechargeTrsCount;	// 充值笔数

		DWORD dwRechargeTotalAmount;	// 充值小计：金额
		DWORD dwRechargeTotalTraCount;	// 充值小计：笔数

		DWORD dwNetIncomAmount;			// 净收入（需要加上故障金额）

		/***************统计现金明细界面**********************/

		// 接收纸币
		DWORD dwRecvBHFace1Count;		// 面额：1
		DWORD dwRecvBHFace5Count;		// 面额：5
		DWORD dwRecvBHFace10Count;		// 面额：10
		DWORD dwRecvBHFace20Count;		// 面额：20
		DWORD dwRecvBHFace50Count;		// 面额：50
		DWORD dwRecvBHFace100Count;		// 面额：100

		DWORD dwRecvBHTotalCount;		// 纸币接收小计：张数
		DWORD dwRecvBHTotalAmount;		// 纸币接收小计：金额

		// 接收硬币
		DWORD dwRecvCHFace05Count;		// 面额：5角
		DWORD dwRecvCHFace1Count;		// 面额：1元

		DWORD dwRecvCHTotalCount;		// 硬币接收小计：枚数
		DWORD dwRecvCHTotalAmount;		// 硬币接收小计：金额
		
		// 硬币找零
		DWORD dwChangeCHFace05Count;	// 面额：5角
		DWORD dwChangeCHFace1Count;		// 面额：1元

		DWORD dwChangeCHTotalCount;		// 硬币找零小计：枚数
		DWORD dwChangeCHTotalAmount;	// 硬币找零小计：金额
		
		// 纸币找零
		DWORD dwChangeBHFace1Count;		// 面额：1
		DWORD dwChangeBHFace5Count;		// 面额：5
		DWORD dwChangeBHFace10Count;	// 面额：10
		DWORD dwChangeBHFace20Count;	// 面额：20
		DWORD dwChangeBHFace50Count;	// 面额：50
		DWORD dwChangeBHFace100Count;	// 面额：100

		DWORD dwChangeBHTotalCount;		// 纸币找零小计：张数
		DWORD dwChangeBHTotalAmount;	// 纸币找零小计：金额


		// 初始化
		_sale_statci_data_model_(){

			strDateFrom				= _T("");
			strDateTo				= _T("");

			dwFailedAmount			= 0;
			dwFailedCount			= 0;
			dwFailedTotalAmount		= 0;

			dwSingleTicketAmount	= 0;
			dwSingleTicketTrsCount	= 0;
			dwSingleTicketCount		= 0;

			dwSaleTotalAmount		= 0;
			dwSaleTotalTrsCount		= 0;
			dwSaleTotalCount		= 0;


			dwRechargeAmount		= 0;
			dwRechargeTrsCount		= 0;

			dwRechargeTotalAmount	= 0;
			dwRechargeTotalTraCount	= 0;

			dwNetIncomAmount		= 0;
			dwRecvBHFace1Count		= 0;
			dwRecvBHFace5Count		= 0;
			dwRecvBHFace10Count		= 0;
			dwRecvBHFace20Count		= 0;
			dwRecvBHFace50Count		= 0;
			dwRecvBHFace100Count	= 0;

			dwRecvBHTotalCount		= 0;
			dwRecvBHTotalAmount		= 0;

			dwRecvCHFace05Count		= 0;
			dwRecvCHFace1Count		= 0;

			dwRecvCHTotalCount		= 0;
			dwRecvCHTotalAmount		= 0;

			
			dwChangeCHFace05Count	= 0;
			dwChangeCHFace1Count	= 0;

			dwChangeCHTotalCount	= 0;
			dwChangeCHTotalAmount	= 0;

			
			dwChangeBHFace1Count	= 0;
			dwChangeBHFace5Count	= 0;
			dwChangeBHFace10Count	= 0;
			dwChangeBHFace20Count	= 0;
			dwChangeBHFace50Count	= 0;
			dwChangeBHFace100Count	= 0;

			dwChangeBHTotalCount	= 0;
			dwChangeBHTotalAmount	= 0;
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();

protected:
	virtual void OnStart();                                                         // Service启动后处理
private:
	LRESULT OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F5] 键按下消息
	LRESULT OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F6] 键按下消息
	virtual LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);		// 处理 [Enter] 键按下消息（执行）
	virtual LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);			// 处理 [F3/Ctrl] 键按下消息（返回）
	Model m_DataModel;
private:
	void InitSaleStaticOverView(vector<TransactionRecord>& vec_transInfo);	// 初始化销售统计一览主界面
};