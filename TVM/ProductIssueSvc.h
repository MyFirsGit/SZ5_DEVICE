#pragma once

#include "PassengerForeSvc.h"
#include "ReceptionStaticInfo.h"

/**
@brief      充值业务Service
*/
class CProductIssueSvc : public CPassengerForeSvc
{
     DECLARE_MESSAGE_MAP()

public:

	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT = 1,
		DIALOG_GROUP_INIT,
		DIALOG_GROUP_CHOOSE_LOCATION,
		DIALOG_GROUP_CHOOSE_PAYMENT,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR
	} DIALOG_GROUP;

	// SVC数据模型
	struct DataModel : public CommonModel
	{
		BYTE SalableProductCount;					// 可售产品数量
		BYTE SalableProductType;					// 可售产品类型
	};

	CProductIssueSvc();
	virtual ~CProductIssueSvc();

	DataModel& GetDataModel();

	map<WORD,CString> GetAvailableProductNames();
	map<WORD,CString> GetAvailableSubproductNames();
	map<DWORD,CString> GetAvailableLocationNames();

	// 业务流相关顺序流程
	long CalculatePrice();								// 获取产品票价
	void DlgNextPage();
	bool DoCardIssueProduct();							// TPU加新产品处理（储值/定期/计次)
	void PrinterPrintReceipt();

	bool IsBothCoinBanknoteMaxCount();
	bool IsBanknoteMaxCount();
	bool IsBanknoteMaxAmount();
	bool IsCoinMaxCount();
//	bool POSPayment();												// 银行卡支付
//	bool POSCancle();												// 银行卡冲正
//	bool IssueProductCard();										// 添加产品流程
	void ReturnFinishDlg();											// 返回到完成界面

protected:

	// 在这个类的构造函数中，让*m_pDataModel和PassengerForeSvc里的*m_pCommonModel指向同一个struct。
	// 这样当调用这里的method时，使用*m_pDataModel，继承的更多struct变量在这里可视。
	// 在使用PassengerForeSvc里的method时，通过*m_pCommonModel则仍可操作DataModel中CommonModel的部分。
	DataModel *m_pDataModel;

	// 业务流相关顺序流程
	long SaveCardInfo(LPRW_READ_CARD_RESPONSE);			// 保存车票数据到DataModel
	void OnSuccessFinish();
	void SaveSCData();									// 保存SC数据
	void WriteDBData(Data::TransactionRecord &dbInfo);	// 写入数据库数据（保存需在不同情形下特别执行）
	void FlowToFinishDlg();								// 供OnSuccessFinish使用的一个函数封装（父类调用）
	void FlowToErrorDlg();								// 供OnErrorFinish，OnCancel和各种超限使用的一个函数封装（父类调用）

	// 业务流相关异常流程
	void PrinterPrintErrorReceipt();
	bool ConfirmCard(LPRW_READ_CARD_RESPONSE);			//判断刷卡信息是否一致（父类调用）

private:

	// 其他
	void FilterSalableProductList(vector<CAccTicketParam::TICKET_TYPE>& vecProduct); // 筛选可以充值的产品列表
	bool IsCoinChageMaxCount();
	COIN_ACCEPTABLE GetAcceptCoinFaceTable();						// 获取允许接收硬币面额信息
	BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList* table = NULL);					// 获取允许接收纸币面额信息
	BOOL m_IsTheSameCard;
	vector<CACCCard::ACCCARD_PRODUCT_INFO> m_vecReadCard; 
	BOOL isSubProductExist(WORD productType,WORD subProductType);	// 判断产品ProductType中是否存在subProduct产品
	friend class CProductIssueReceptionDlg;
protected:
	virtual void OnStart();
};
