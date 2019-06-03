#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "RWMessage.h"
#include "VersionInfo.h"
#include "PrinterHelper.h"
#include "SCAuditData.h"
#include "DataHelper.h"

/**
@brief  打印审计信息类
*/
class CAuditHelper : public CServiceHelper	
{
public:

	typedef struct _product_trans_data					//票卡操作数据
	{
		ProductType		product;		// 产品类型
		int				nCount;			// 数量
		long			lAmount;		// 金额
		_product_trans_data()
		{
			product	= UNKNOWPRODUCT;
			nCount	= 0;
			lAmount = 0;
		}
	}PRODUCT_TRANS_DATA , *PPRODUCT_TRANS_DATA;

	typedef struct	_CASH_DATA							//现金交易数据
	{
		BankNoteAndCoinType_t	value;  //面值
		int						nCount; //数量
		long					lAmount;//金额
		_CASH_DATA()
		{
			value	= VALUE_UNKNOWN;
			nCount	= 0;
			lAmount = 0;
		}
	}CASH_DATA, *PCASH_DATA;


	typedef enum _cash_box_type
	{
		COMMON_BOX,
		RECYCLE_CHANGE_BOX,
		CHANGE_BOX_A,
		CHANGE_BOX_B,
		CYCLE_BOX_A,
		CYCLE_BOX_B,
		COLLECTION_BOX_A,
		COLLECTION_BOX_B,
	}CASH_BOX_TYPE;

	typedef struct _CASH_BOX_DATA
	{
		CASH_BOX_TYPE type;				//箱子类型(普通箱，循环找零...)
		CString			nBoxID;			//箱子ID
		BankNoteAndCoinType_t value;	//面值
		int			amount;				//数量
		_CASH_BOX_DATA()
		{
			type = COMMON_BOX;
			nBoxID = _T("--------");
			value = VALUE_UNKNOWN;
			amount = 0;
		}

	}CASH_BOX_DATA, *PCASH_BOX_DATA;
	
	typedef enum _change_box_operation_type_{
		INSTALL_CHANGE_BOX		= 1,// 安装操作
		UNINSTALL_CHANGE_BOX	= 2,// 卸载操作
	}CHANGE_BOX_OPERATION_TYPE;

	typedef struct _FAULT_TRANS_DATA	//故障交易
	{
		int			nFaultCardCount;	//发卡失败
		long		lFaultCardAmount;
		int			nFaultTokenCount;	//发Token失败
		long		lFaultTokenAmount;
		int			nFaultChangeCount;	//找零故障
		long		lFaultChangeAmount;
		int			nFaultDCPayCount;	//信用卡支付故障
		long		lFaultDCPayAmount;
		int			nFaultBCPayCount;	//银行卡支付故障
		long		lFaultBCPayAmount;
		int			nFaultEPCount;		//EP支付故障
		long		lFaultEPAmount;
		int			nFaultCount;		//故障合计
		long		lFaultAmount;
		_FAULT_TRANS_DATA()
		{
			nFaultCardCount = 0;	//发卡失败
			lFaultCardAmount = 0;
			nFaultTokenCount = 0;	//发Token失败
			lFaultTokenAmount = 0;
			nFaultChangeCount = 0;	//找零故障
			lFaultChangeAmount = 0;
			nFaultDCPayCount = 0;	//信用卡支付故障
			lFaultDCPayAmount = 0;
			nFaultBCPayCount = 0;	//银行卡支付故障
			lFaultBCPayAmount = 0;
			nFaultEPCount = 0;		//EP支付故障
			lFaultEPAmount = 0;
			nFaultCount = 0;		//故障合计
			lFaultAmount = 0;
		}
	}FAULT_TRANS_DATA;

	typedef enum _PayMent								// 支付方式
	{
		CASH							= 1,
		EP								= 2,
		BANK_CARD				= 5,
		CREDIT_CARD			= 7
	}PAYMENT_TYPE;
	typedef enum _transaction_type						// 交易类型
	{
		CARD_ISSUE			=  2,
		/*CARD_ISSUE			=  2,
		PRODUCT_ISSUE		=  3,*/
		RECHARGE			=  4,
	}TRANSACTION_TYPE;
	
	typedef enum _exceion_trans
	{
		//TOKEN_FAILURE		= 1,
		CARD_FAILURE		= 1,
		COIN_CHANGE_FAILURE	= 3,
		NOTE_CHANGE_FAILURE	= 4,
		CREDIT_FAILURE		= 5,
		BANK_FAILURE		= 6,
		EP_FAILURE			= 7,
		WRITE_CARD_FAILED   = 10,
	}EXCEPTION_TRANS;
	typedef struct _balance_data
	{
		_DATE_TIME	dtOriginTime;		//开始时间
		_DATE_TIME	dtCurrentTime;		//操作时间
		/*	结构解释：
				map<ProductType, pair<int, int>>
				map<产品类型名称， pair<交易数量，交易金额>> 

				vector<CASH_DATA>
				vector<<面值,数量，金额>>

				vector<CASH_BOX_DATA>
				vector<<箱子ID，面值，数量>>

				map<EXCEPTION_TRANS, pair<int, int>> mapException;
				map<故障类型名称, pair<交易数量，交易金额>>
		*/
		
		map<ProductType, pair<int, int>> mapIssue;			//充值交易数据
		map<ProductType, pair<int, int>> mapRecharge;		//售产品交易数据
		map<ProductType, pair<int, int>> mapIssuePdt;

		map<ProductType, pair<int, int>> mapCashIssue;		//现金售票交易数据	
		map<ProductType, pair<int, int>> mapCashRecharge;	//现金充值交易数据
		map<ProductType, pair<int, int>> mapCashProdIssue;	//现金售产品交易数据

		vector<CASH_DATA> vecCoinReceived;					//硬币接收
		vector<CASH_DATA> vecNotesReceived;					//纸币接受
		vector<CASH_DATA> vecCoinReleased;					//硬币找零
		map<BankNoteAndCoinType_t,pair<int,int>> mapBanknoteChange;	// 纸币找零

		vector<CASH_BOX_DATA> vecCoinAdded;					//硬币补充（不用了）
		map<BankNoteAndCoinType_t,int>	mapCoinAdded;		//硬币补充
		vector<CASH_BOX_DATA> vecBanknoteAdded;				//纸币找零箱补充（不用了）
		map<BankNoteAndCoinType_t,int>  mapBanknotAdded;	//纸币找零箱补充
		vector<CASH_BOX_DATA> vecBanknoteChangeInfo;		//纸币找零明细
		vector<CASH_BOX_DATA> vecCoinCollection;			//硬币回收
		vector<CASH_DATA> vecCashCollection;				//纸币回收
		vector<CASH_DATA> vecCoinCollectionBox;				//硬币回收（整个审计时间段）
		
		map<ProductType, pair<int, int>> mapDCIssue;		//信用卡售卡
		map<ProductType, pair<int, int>> mapDCRecharge;		//信用卡充值
		map<ProductType, pair<int, int>> mapDCIssuePdt;		//信用卡售产品

		map<ProductType, pair<int, int>> mapBCIssue;		//银行卡售卡
		map<ProductType, pair<int, int>> mapBCRecharge;		//银行卡充值
		map<ProductType, pair<int, int>> mapBCIssuePdt;		//银行卡售产品

		int			nCashAcceptSum;
		int			nCashChangeSum;
		int			nBnChangeCount;		// 纸币找零数量

		int			nEpPayCount;		// 售票数量
		long		lEpPayAmount;		// 售票金额

		int			nSumSale;			// 销售金额
		int			nQRSumSale;			// 取票金额
		int			nCashSum;			// 现金交易金额
		int			nCreditSum;			// 银行卡交易金额
		int			nBankSum;			// 信用卡交易金额

		int			nNoteCollection;	// 纸币回收
		int			nCoinCollection;	// 硬币回收

		// 纸币存留
		vector<CASH_BOX_DATA> vec_BanknoteRecyclerReservInfo;
		vector<CASH_BOX_DATA> vec_BanknoteChangeReservInfo;
		vector<CASH_DATA> vec_BanknoteCollectionReservInfo;

		// 硬币留存
		vector<CASH_BOX_DATA> vec_CoinReservInfo;

		map<EXCEPTION_TRANS, pair<int, int>> mapException;	//故障交易
		int nSumExceptionCount, nSumExceptionAmount;
		_DATE_TIME	dtOperationTime;	// 操作时间
		int		nOperatorID;			// 操作员ID
		CString	station_name_CN;		// 车站名（中）
		CString	station_name_EN;		// 车站名（英）
		CString	station_name_PN;		// 车站名（葡）
		int		nDeviceID;				// 操作员ID

		_balance_data()
		{	
			nCashAcceptSum = 0;
			nCashChangeSum = 0;
			nBnChangeCount  = 0;
			nCashSum		= 0;
			nCreditSum		= 0;
			nBankSum		= 0;
			nEpPayCount		= 0;
			lEpPayAmount	= 0;
			nOperatorID		= 0;
			nNoteCollection = 0;
			nCoinCollection = 0;
			nSumSale		= 0;
			nQRSumSale		= 0;
			mapBanknoteChange.clear();
			mapIssue.clear();
			mapRecharge.clear();
			mapIssuePdt.clear();

			mapCashIssue.clear();
			mapCashRecharge.clear();
			mapCashProdIssue.clear();

			vecCoinReceived.clear();
			vecNotesReceived.clear();
			vecCoinReleased.clear();

			vecCoinAdded.clear();
			vecCoinCollection.clear();
			vecCashCollection.clear();
			vecCoinCollectionBox.clear();

			mapDCIssue.clear();
			mapDCRecharge.clear();
			mapDCIssuePdt.clear();

			mapBCIssue.clear();
			mapBCRecharge.clear();
			mapBCIssuePdt.clear();


			station_name_CN = _T("");
			station_name_EN = _T("");
			station_name_PN = _T("");
			nSumExceptionCount = 0;
			nSumExceptionAmount = 0;
		}

	}BALANCE_DATA;

	CAuditHelper(CService& service);													// 构造函数
	~CAuditHelper();																	// 析构函数

	BALANCE_DATA LoadBalanceData();														// 获取结算数据
	int PrintBalanceBill(bool bIsPrint = true);											// 业结数据模板化,业结单据打印处理
private:
	CString  GetProductName(ProductType prodtype);										// 产品名称	转	字符串
	CString  CashValueToString(BankNoteAndCoinType_t& value);							// 现金面值	转	字符串（方便显示）
	int		 FormatCashToINT(BankNoteAndCoinType_t cash);								// 现金面值	转	整形(方便运算)
	int DeviceType;																		// 设备类型
	pair<CString, CString> GetSum(map<ProductType, pair<int, int>> mapData);			// 容器求和
	pair<CString, CString> GetSum( vector<CASH_DATA>& vec);							// 容器求和
	pair<CString, CString> GetSum( vector<CASH_BOX_DATA>& vec);						// 容器求和

	DWORD TransTypeToValue(int value);													// 转换硬币类型为值
	void SendTVMAuditData();															// 发送审计数据到服务器
	PurchaseData m_curPurchase;
	BALANCE_DATA m_BalanceData;
	CString FromInt(int);
	BankNoteAndCoinType_t TransCoinType(int value);										// 币值转换
	BankNoteAndCoinType_t TransBanknoteType(int value);									// 转换纸币类型
	//void PrintAuditReport(SALESTAT_KIND kind, PurchaseData& purchaseData);               //打印审计信息
	//CString GetPrintLineInfo(CString strTitle,int number,int amount);
	//CString GetPrintLineInfo(unsigned short ticketID,int number,int amount,bool isTotal,bool isYKT,bool isAdjust = false);
	//void PrintTicketInfo(CStringArray& printTicketTexts, map<WORD,SCAuditValue>& summary);
	//CString ComPrintLineFormatForTemplate(CString& PrintLine);


};
