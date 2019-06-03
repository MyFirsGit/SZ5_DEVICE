#pragma once
#include "tvmforeservice.h"
#include "tpuerrorinfo.h"
#include "SCAuditManager.h"
#include "AudioHelper.h"
#include "DataHelper.h"
#include "TransactionInfo.h" 


/**
@brief      乘客业务Service
*/
class CPassengerForeSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:

	typedef enum _timers_id{
		TIMER_COUNTING_TO_RETURN				= 1,	// 计数中到返回中的计时器，用于正在投币超时。
		TIMERS_OUT_OF_REPLAY					= 2,	// 重复播放上一句语音计时器ID(盲人业务)
		TIMERS_OPERATION_OUT_OF_TIME			= 3,	// 盲人操作超时时间计时器ID(盲人业务)
		TIMERS_COUNTING_BEFORE_ISSUE			= 4,	// 开始购票到开始出票之间的计时器（针对武汉业务，新增）
		TIMERS_WEB_QUERY_ORDER					= 5,	// 订单查询定时器
		TIMERS_WAIT_CARD_INSERT					= 6,	// 等待储值卡插入
	}TIMERS;

	typedef enum _finish_type {
		DEFAULT = 0,
		PASSAGE_CANCEL,						// 乘客取消
		OVER_MONEY_AMOUNT_LIMIT,			// 投入金额超限
		OVER_MONEY_COUNT_LIMIT,				// 投入数量超限
		OVER_CHANGE_COUNT_LIMIT,			// 找零数量超限
		OVER_CHANGE_AMOUNT_LIMIT,			// 找零不足
		SERVICE_ERROR_CANCEL,				// 业务逻辑错误取消
		EXCEPTION_ERROR,					// 异常取消
		INSERT_OUT_OF_TIME,					// 投入超时取消
		NOT_ENOUGH_TICKETS_FOR_BOOKING,     // 票卡不足，无法售票
		TRANSACTION_STATE_FAILED,			// 交易失败！
		RUNNING_OUT_OF_TIME,				// 交易超时
		CHANGE_FALIED_RETURN,				// 找零失败结束
		QR_ORDER_GENERATE_FAILED,			// 二维码订单生成失败
	} FINISH_TYPE;

	// 硬币接收状态
	typedef enum CoinAcceptState{
		COIN_ACCEPT_STATE_NONE 					= 0,			// 无接收
		COIN_ACCEPT_STATE_INSERTED 				= 1,			// 已投入
		COIN_ACCEPT_STATE_COLLECTED 			= 2,			// 已收纳
		COIN_ACCEPT_STATE_RETURNED 				= 3,			// 已退出
		COIN_ACCEPT_STATE_RETURN_FAILD			= 4,			// 退币失败
		COIN_ACCEPT_STATE_ERROR					= 5,			// 交易失败
		COIN_ACCEPT_STATE_COLLECTED_FAILD		= 6,			// 回收失败
	}COIN_ACCEPT_STATE;

	// 纸币接收状态
	typedef enum BanknoteAcceptState{
		BANKNOTE_ACCEPT_STATE_NONE 				= 0,			// 无接收
		BANKNOTE_ACCEPT_STATE_INSERTED 			= 1,			// 已投入
		BANKNOTE_ACCEPT_STATE_COLLECTED 		= 2,			// 已收纳
		BANKNOTE_ACCEPT_STATE_RETURNED 			= 3,			// 已退出
		BANKNOTE_ACCEPT_STATE_RETURN_FAILED		= 4,			// 退币失败
		BANKNOTE_ACCEPT_STATE_ERROR				= 5,			// 交易失败
		BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED	= 6,			// 回收失败
	}BANKNOTE_ACCEPT_STATE;

	// 硬币找零状态
	typedef enum CoinChangeState{
		COIN_CHANGE_STATE_NONE 					= 0,			// 找零操作前
		COIN_CHANGE_STATE_SUCCESS 				= 1,			// 找零成功
		COIN_CHANGE_STATE_FAIL 					= 2,			// 找零失败
		COIN_CHANGE_STATE_NA					= 3,			// 无需找零
	}COIN_CHANGE_STATE;

	// 纸币找零状态
	typedef enum BanknoteChangeState{
		BANKNOTE_CHANGE_STATE_NONE 				= 0,			// 无找零
		BANKNOTE_CHANGE_STATE_SUCCESS 			= 1,			// 找零成功
		BANKNOTE_CHANGE_STATE_FAIL 				= 2,			// 找零失败
	}BANKNOTE_CHANGE_STATE;

	// 支付状态（银行卡和EP支付时用）
	typedef enum PaymentState{
		PAYMENT_STATE_NONE 						= 0,			// 无支付
		PAYMENT_STATE_SUCCESS 					= 1,			// 支付成功
		PAYMENT_STATE_FAIL 						= 2,			// 支付失败
	}PAYMENT_STATE;

	// 打印状态
	typedef enum FailurePrintState{
		FAILURE_PRINT_STATE_NONE 				= 0,			// 无打印
		FAILURE_PRINT_STATE_SUCCESS 			= 1,			// 打印成功
		FAILURE_PRINT_STATE_FAIL 				= 2,			// 打印失败
	}FAILURE_PRINT_STATE;

	// 交易状态 
	typedef enum TransactionState{
		TRANSACTION_STATE_NONE 					= 0,			// 无交易
		TRANSACTION_STATE_SUCCESS 				= 1,			// 交易正常结束
		TRANSACTION_STATE_FAIL 					= 2,			// 交易故障结束
		TRANSACTION_STATE_CANCEL_SUCCESS		= 3,			// 取消正常结束（预留）
		TRANSACTION_STATE_CANCEL_FAIL			= 4,			// 取消异常结束（预留）
	}TRANSACTION_STATE;

	// 交易异常
	typedef enum TransactionException{
		//TRANSACTION_EXCEPTION_TOKEN_MOUDOL	  = 1,			// Token异常
		TRANSACTION_EXCEPTION_CARD_MOUDOL	  = 1,			// Card 异常
		TRANSACTION_EXCEPTION_RELEASE_COIN	  = 3,			// 硬币找零异常
		TRANSACTION_EXCEPTION_RELEASE_BANKNOTE= 4,			// 纸币找零异常
		TRANSACTION_EXCEPTION_CREDIT		  = 5,			// 信用卡异常
		TRANSACTION_EXCEPTION_BANKCARD		  = 6,			// 储蓄卡异常
		TRANSACTION_EXCEPTION_EP		      = 7,			// EP支付异常
		TRANSACTION_EXCEPTION_RETURN_COIN     = 8,			// 硬币放出异常
		TRANSACTION_EXCEPTION_RETURN_BANKNOTE = 9,			// 纸币放出异常
		TRANSACTION_EXCEPTION_WRITE_CARD_FAILED = 10,		// 票卡编码错误（充值，添加产品用）
		TRANSACTION_EXCEPTION_COIN_INSERT		= 11,		// 硬币投入异常
		TRANSACTION_EXCEPTION_COIN_COLLECTION	= 12,		// 硬币收纳异常
		TRANSACTION_EXCEPTION_BANKNOTE_INSERT	= 13,		// 纸币投入异常
		TRANSACTION_EXCEPTION_BANKNOTE_COLLECTION = 14,		// 纸币收纳异常
	}TRANSACTION_EXCEPTION;

	// 模块忙碌状态
	typedef enum _device_status_{
		DEVICE_ALL_OK				= 0x00,					// 所有设备为空闲状态
		DEVICE_COIN_BUSY			= 0x01,					// 硬币模块正在处理
		DEVICE_BANKNOTE_BUSY		= 0x02,					// 纸币模块正在处理
		DEVICE_BANKNOTE_CHANGE_BUSY = 0x04,					// 纸币找零模块正在处理
		DEVICE_CARD_BUSY			= 0x08,					// 发卡模块正在处理中
	};
	// 票卡信息记录体
	typedef struct _tagMediumInfo
	{
		CString cardLogicalNumber;							// 票卡逻辑编号（CACCCard）
		CARD_MEDIUM_TYPE mediumType;						// 介质物理类型
		BYTE cardIssuer;									// 卡片发行商
		CardType_t cardPHType;								// 票卡物理类型（0x11：一票通M1卡；0x12：一票通UL卡）
		long lCardDeposit;									// 票卡押金
		long lCardFee;										// 票卡成本费
		PassengerType_t passengerType;						// 票卡乘客类型

		_tagMediumInfo () {
			cardLogicalNumber = _T("");
			mediumType = MIFARE1K;
			cardPHType = CARD_TYPE_ACC_M1;
			lCardDeposit = 0;
			lCardFee = 0;
			passengerType = 0x01; // 成人 // should use "PASSENGER_TYPE_UNSPECIFIED"
		}
	} MEDIUM_INFO;

	// 产品信息记录体
	typedef struct _tagProductInfo
	{
		ProductCategory_t productCategory;					// 产品种类（钱包、定期、计次）
		ProductType_t productType;							// 产品类型（SVT、PST、MST）
		ProductTypeVariant_t productSubType;				// 产品子类型（如计次票的30次、50次、100次）
		LocationNumber_t beginLocation;						// 起始站点或区段
		LocationNumber_t endLocation;						// 目的站点或区段
		WORD useArea;										// 使用区段
		long lBalanceAmount;								// SVT金额，PST不使用
		long lBalanceRides;									// MST次数，PST不使用
		Duration_t lValidatePeriod;							// PST时长信息
		_DATE_TIME dtValidateDateTime;						// 产品有效期结束时间
		long lProductDeposit;								// 产品押金
		long lProductSaleAmount;							// 产品销售单价
		int  nProductPiece;									// 产品数量（售票）
		int  nProductIssuedPiece;							// 已售产品数量（售票）
		_tagProductInfo () {
			productCategory = UNDEFINE;
			productType = static_cast<ProductType_t>(UNKNOWPRODUCT);
			productSubType = 0;
			beginLocation = LOCATION_NUMBER_UNSPECIFIED;
			endLocation = LOCATION_NUMBER_UNSPECIFIED;
			lBalanceAmount = 0;
			lBalanceRides = 0;
			useArea = 0;
			lValidatePeriod = static_cast<Duration_t>(0);
			dtValidateDateTime = _date_time();
			lProductDeposit = 0;
			lProductSaleAmount = 0;
			nProductPiece = 0;
			nProductIssuedPiece = 0;
		}

		// PRODUCT_INFO中lBalanceAmount和lBalanceRides和lValidatePeriod为产品购买后的信息。

		// 对于售票和添加产品，由于原先剩余次数为0，lBalanceAmount和lBalanceRides和lValidatePeriod等同于
		// 单个可售产品的信息，故为对应产品类型的参数信息。

		// 对于充值业务，lBalanceAmount和lBalanceRides为产品充值前/后的绝对信息，充值额度另行用变量给出。
		// lValidatePeriod因为无法从票卡中读出，故并不使用。

	} PRODUCT_INFO;

	// 支付信息记录体
	typedef struct _tagPaymentInfo
	{
		PaymentMethod_t paymentMethod;						// 支付方式
		long lPriceAmount;									// 税前金额
		long lTaxAmount;									// 税费金额
		long lDueAmount;									// 应收金额
		long lPaidAmount;									// 实收金额
		long lChangeAmount;									// 找零金额
		long lReturnAmount;									// 退款金额
		long lNotChangeAmount;								// 未找零金额

		// 硬币支付信息
		long lCHReceiveTotalAmount;							// 硬币支付总金额
		long lCHReceiveTotalCount;							// 硬币支付总个数
		map<BankNoteAndCoinType_t,long> PayCoinInfo;		// 硬币支付详情

		// 纸币支付信息
		long lBHReceiveTotalAmount;							// 纸币支付总金额
		long lBHReceiveTotalCount;							// 纸币支付总个数
		map<BankNoteAndCoinType_t,long> PayBankNoteInfo;	// 纸币支付详情

		// 银行卡支付
		BYTE BankCardNum[10];								// 银行卡号	
		long lBankReceiveTotalAmount;						// 银行卡支付总金额

		// EP支付
		BYTE EPCardNum[8];									// EP卡号
		long lEPReceiveTotalAmount;							// EP支付总金额

		// 找零信息
		//DWORD dwChangeStockCount;							// 循环找零箱找零枚数
		//DWORD dwChangeHopper1Count;						// 找零箱1找零枚数
		//DWORD dwChangeHopper2Count;						// 找零箱2找零枚数

		DWORD dwChangeCoinOneYuanCount;						// 硬币找零一元个数
		DWORD dwChangeCoinHalfYuanCount;					// 硬币找零五角个数
		DWORD dwRecycleBNRBoxACount;						// 纸币循环找零A箱找零张数（根据配置确定面额）
		DWORD dwRecycleBNRBoxBCount;						// 纸币循环找零B箱找零张数（根据配置确定面额）
		DWORD dwRecycleBNRBoxCCount;						// 纸币循环找零C箱找零张数（根据配置确定面额）
		DWORD dwRecycleBNRBoxDCount;						// 纸币循环找零D箱找零张数（根据配置确定面额）

		DWORD dwChangeBNRBoxCount;							// 纸币找零箱找零张数（根据配置确定面额）

		DWORD dwChangeBHAmout;								// 纸币找零金额
		map<MONEY_BOX_TYPE,long> ChargeCoinBanknoteInfo;	// 找零支付详情

		DWORD dwCoinChangeErrorAmount;						// 硬币找零失败金额
		DWORD dwBanknoteChangeErrorAmount;					// 纸币找零失败金额

		map<MONEY_BOX_TYPE,long> CoinSaveDetail;			// 硬币收纳信息

		CH_COMMON_RSP rspCoinModule;						// 硬币信息

		DWORD dwBanknoteChangeRetryCount;					// 纸币找零重试次数
		_tagPaymentInfo () {
			paymentMethod = PAYMENT_METHOD_CASH;			// 默认现金支付
			lPriceAmount = 0;
			lTaxAmount = 0;
			lDueAmount = 0;
			lPaidAmount = 0;
			lChangeAmount = 0;
			lReturnAmount = 0;
			lNotChangeAmount = 0;

			lCHReceiveTotalAmount = 0;
			lCHReceiveTotalCount = 0;
			PayCoinInfo.clear();

			lBHReceiveTotalAmount = 0;
			lBHReceiveTotalCount = 0;
			PayBankNoteInfo.clear();

			memset(BankCardNum, 0, 10);
			lBankReceiveTotalAmount = 0;

			memset(EPCardNum, 0, 8);
			lEPReceiveTotalAmount = 0;

			lEPReceiveTotalAmount = 0;

			/*dwChangeStockCount = 0;
			dwChangeHopper1Count = 0;
			dwChangeHopper2Count = 0;*/
			dwChangeCoinHalfYuanCount = 0;
			dwChangeCoinOneYuanCount = 0;
			/*dwChangeBHBoxACount = 0;
			dwChangeBHBoxBCount = 0;*/
			dwRecycleBNRBoxACount = 0;
			dwRecycleBNRBoxBCount = 0;
			dwRecycleBNRBoxCCount = 0;
			dwRecycleBNRBoxDCount = 0;
			dwChangeBNRBoxCount = 0;

			dwChangeBHAmout = 0;
			ChargeCoinBanknoteInfo.clear();
			CoinSaveDetail.clear();
			dwCoinChangeErrorAmount = 0;
			dwBanknoteChangeErrorAmount = 0;
			dwBanknoteChangeRetryCount = 0;
			//rspCoinModule.coin_info = CH_COMMON_RSP_COININFO();

		}

		// 纸币找零数量总和
		DWORD BNRChangeCount(){
			return dwRecycleBNRBoxACount + dwRecycleBNRBoxBCount + dwRecycleBNRBoxCCount + dwRecycleBNRBoxDCount + dwChangeBNRBoxCount;
		}
	} PAYMENT_INFO;

	// 操作信息记录体
	typedef struct _tagOperationInfo
	{
		FINISH_TYPE finishType;         					// 业务结束原因
		int readCardCount;									// 读卡次数计数
		bool isCoinSlotOpen;								// 硬币口是否打开
		bool isBankNoteSlotOpen;							// 纸币口是否打开
		bool isBankCardSlotOpen;							// 银行卡口是否打开
		bool isWaitingEPCard;								// 是否在读EP卡
		bool isFinishedReceveMoney;							// 是否完成接收钱
		//bool isFailedIssueCard;								// 是否售卡失败
		//bool isSuccessIssueCard;							    // 售卡是否成功
		bool isBeginBuyProduct;								// 是否开始买产品
		bool isBanknotesMaxCount;							// 是否纸币接收数量达到最大值
		bool isCoinMaxCount;								// 是否硬币接收数量达到最大值
		//bool isRechargeSuccess;								// 充值是否成功
		bool isFinishedJob;									// 是否完成前台业务？
		COIN_ACCEPT_STATE coinAcceptState; 					// 硬币收纳状态
		BANKNOTE_ACCEPT_STATE banknoteAcceptState; 			// 纸币收纳状态
		COIN_CHANGE_STATE coinChangeState; 					// 硬币找零状态
		BANKNOTE_CHANGE_STATE banknoteChangeState; 			// 纸币找零状态
		PAYMENT_STATE  paymentState;						// 支付状态（银行卡和EP支付时用） 
		FAILURE_PRINT_STATE failurePrintState;				// 故障单打印状态
		map<TRANSACTION_EXCEPTION,long> mapException;		// 故障交易种类
		vector<CSysException>	m_vecException;				// 异常记录

		_tagOperationInfo () {
			finishType				= DEFAULT;
			readCardCount			= 0;
			isCoinSlotOpen			= false;
			isBankNoteSlotOpen		= false;
			isBankCardSlotOpen		= false;
			isWaitingEPCard			= false;
			isFinishedReceveMoney	= false;
			//isFailedIssueCard		= false;
			//isSuccessIssueCard		= false;
			isBeginBuyProduct		= false;
			isCoinMaxCount			= false;
			isBanknotesMaxCount		= false;
			//isRechargeSuccess		= false;
			isFinishedJob			= false;
			coinAcceptState = COIN_ACCEPT_STATE_NONE;
			banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;
			coinChangeState = COIN_CHANGE_STATE_NONE;
			banknoteChangeState = BANKNOTE_CHANGE_STATE_NONE;
			paymentState = PAYMENT_STATE_NONE;
			failurePrintState = FAILURE_PRINT_STATE_NONE;
			mapException.clear();
			m_vecException.clear();
		}
	} OPERATION_INFO;

	// 找零优先级结构定义
	typedef struct _change_type_value_{
		MONEY_BOX_TYPE	BoxType;			// 箱子类型
		DWORD			dwValue;			// 面额
		long			nCurCount;			// 此箱子的当前数量
		long			nMoneyCount;		// 此箱最大找零数量
		_change_type_value_(){
			BoxType = UNDEFINE_BOX;
			dwValue = 0;
			nCurCount = 0;
			nMoneyCount = 0;
		}

		_change_type_value_(MONEY_BOX_TYPE BoxType, DWORD dwValue, long nCurCount, long nMoneyCount){
			this->BoxType = BoxType;
			this->dwValue = dwValue;
			this->nCurCount = nCurCount;
			this->nMoneyCount = nMoneyCount;
		}

		bool operator<(const _change_type_value_& other){
			return BoxType<=other.BoxType;
		}

	}CHANGE_VALUE;

	struct CommonModel
	{
		_DATE_TIME transTime;							// 交易时间
		DWORD dwUDSN;         							// 首笔交易UDSN
		DWORD dwTransactionID;							// AFC计数
		MEDIUM_INFO mediumInfo;							// 票卡信息
		MEDIUM_INFO mediumCardInfo;						// 票卡信息(用于存储储值卡购票业务中储值卡信息）
		PRODUCT_INFO productCardInfo;					// 产品信息(用于存储储值卡购票业务中储值卡产品信息）
		TicketInfo* actrualTicketInfo;					// 出票信息（用于存储二维码购票业务中发售的票卡信息）
		PRODUCT_INFO productInfo;						// 产品信息
		PAYMENT_INFO paymentInfo;						// 支付信息
		OPERATION_INFO operationInfo;					// 操作信息
		bool			isInsertToDatabase;				// 是否已经插入库
		long lDeviceStatus;								// 硬件模块状态

		// 初始化
		CommonModel(){
			transTime = _date_time();
			dwUDSN = 0;
			dwTransactionID = 0;
			mediumInfo = MEDIUM_INFO();
			productInfo = PRODUCT_INFO();
			mediumCardInfo = MEDIUM_INFO();
			productCardInfo = PRODUCT_INFO();
			actrualTicketInfo = NULL;
			paymentInfo = PAYMENT_INFO();
			operationInfo = OPERATION_INFO();
			isInsertToDatabase = false;
			lDeviceStatus = 0;
		}
	};

	CPassengerForeSvc(SERVICE_ID serviceID, SERVICE_ID parentServiceID = MAIN_SVC);
	virtual ~CPassengerForeSvc();

	CommonModel& GetCommonModel();

	virtual void NotifyDataModelChanged();

	virtual long CalculatePrice();			// 获取产品票价（这里只包含从TPU计算的部分。特别情形需在具体Svc中具体写出）
	virtual void StartReceiveMoney();				// 开始收钱
	void AcceptMoney();						// 收纳现金

	void OnCancel();
	void OnServiceErrorCancel();

protected:

	CommonModel *m_pCommonModel;
	int  m_nReadedCardCount;											// 当前读卡错误次数
	bool bIsConfirmReadCard;											// 判断是第一次读卡，还是第二次确认读卡
	bool m_bIsPassengerCanceled;										// 判断乘客是否按下取消键只响应一次
	bool m_bIsFinishedWriteData;										// 判断是否写入审计数据？
	bool m_bNeedClickOkButton;											// 是否需要点击按钮继续发票？		
	virtual void OnReadCard(UINT,CRWReadCard*);
	virtual void OnCoinInserted(UINT,CCHReadReceivs*);
	virtual void OnCoinChanged(UINT,CCHChange*);
	virtual void OnCoinReceived(UINT,CCHAccept*);
	virtual void OnCoinReturned(UINT,CCHRefund*);
	virtual void OnBanknoteInserted(UINT,CBRHOpenAndValidate*);
	virtual void OnBanknoteCollect(UINT,CBRHCashInEnd*);
	virtual void OnBanknoteReturned(UINT,CBRHCashInRollback*);
	virtual void OnBanknoteChangeSendOut(UINT,CBRHDispenseRequest*);		// 纸币找零回调函数
	//virtual void OnChargeInsertCard(UINT,CIOCardPosMonitor*);				// 充值乘客插入卡回调函数

	// 业务流相关顺序流程
	virtual long SaveCardInfo(LPRW_READ_CARD_RESPONSE);						// 保存车票数据到DataModel
	virtual bool ConfirmCard(LPRW_READ_CARD_RESPONSE);						// 确认卡信息
	//void OnSuccessFinish();
	virtual void SaveSCData(); // 有具体业务相关的SaveSCData在具体业务中写出。
	virtual void WriteDBData(Data::TransactionRecord &dbInfo); // 有具体业务相关的WriteDBData在具体业务中写出。
	virtual void FlowToFinishDlg(); // 供OnSuccessFinish使用的一个函数封装
	virtual long FinishEPPayment(LPRW_READ_CARD_RESPONSE);
	virtual void FlowToDefaultDlg();					// 转到默认业务
	// 业务流相关异常流程
	void OnError(CSysException e);											// 错误处理，硬件/系统抛出
	void OnError(CString errMsg, DWORD dwReceptMsg=GUIDE_CHARGE_FAILURE);	// 错误处理，Svc代码抛出
	//void OnErrorFinish();
	bool StartReturnCash();	// 退币
	void CloseAllDevice();	// 关闭所有硬件
	//void OnReturnCashOK();  // 退币成功
	void OnReturnFinish();	// 完成
	virtual void FlowToErrorDlg(); // 供OnErrorFinish，OnCancel和各种超限使用的一个函数封装

	virtual void PrinterPrintReceipt();
	virtual void PrinterPrintErrorReceipt();

	bool IsReceiveEnoughMoney();								// 是否完成支付

	// 纸币金额超限相关函数。之所以放在具体Svc层，因为从theAFC_TVM_OPERATION取来的纸币限制，对售票和找零不同
	virtual bool IsBothCoinBanknoteMaxCount();
	virtual bool IsBanknoteMaxCount();
	virtual bool IsBanknoteMaxAmount();
	virtual bool IsCoinMaxCount();
	virtual bool IsCoinChageMaxCount();									// 找零数量是否超限？
	virtual bool IsBHChangeMaxCount();									// 纸币找零数量是否超限？
	virtual bool IsBHChangeMaxAmout();									// 纸币找零金额是否超限？

	virtual COIN_ACCEPTABLE GetAcceptCoinFaceTable() = 0;				// 获取可接收的硬币的面额
	virtual BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList* table = NULL) = 0;			// 获取可接收的纸币的面额

	// 菜单栏支付按钮
	// 计时器，子类需要用到
	virtual void OnTimer(UINT nEventID);								// 计数器,计数中到返回中的计时器响应函数
	virtual void UpdateReceptDlg(CString strTxt){}						// 更新乘客界面倒计时提示
public:
	void ResetMoneyNeedTimer();											// 重置接受纸币或硬币定时器

	void EndRecvTimer();													// 停止计数中的计时器（针对武汉业务，新增）
	void InsertRecvStartTimer();											// 开启出票前返回计时器（针对武汉业务，新增）
private:

	//?? 下面这些硬件驱动函数，未来最好返回值都是void（目前不行，因为和TVMForeService矛盾）。
	//真的硬件有些返回错误代码，有些没有。最好根据错误代码调用onError即可。
	long BlockCard();					// 黑名单锁卡
	void StartReceiveCoin();			// 开始接受现金（纸币，硬币）
	void StartReceiveBanknote();
	void StartPayUsingEPCard();			// 开始用储值卡支付
	void OnAcceptMoneyOK();				// 投币金额足够后被OnCoinInserted，OnBanknoteInserted，OnBankcardInserted或者EP卡支付成功的函数调用。
	void StartAcceptCash();				// 关口和压钞
	void OnReceiveMoneyOK();			// 压钞后，被OnCoinReceived和OnBanknoteCollect调用
	//bool StartChange();					// 找零 //!! 这个需要保留返回值，因为成功了要去OnSuccessfulFinish

	// 郑州新加接口，纸币找零及硬币找零分开
	void StartCHChange();				// 开始硬币找零，需要等待硬币压到回收箱后才能够找零
	void StartBHChange();				// 开始纸币找零，不依赖于其他硬件条件，可以自己独立工作。


	void RenewCoinReceivedAmount(tCHACoinValueInfo chAmountInfo);				// 更新硬币接受金额信息
	void RenewBanknoteReceivedAmount(tBncCashInfo& bh_status);			// 更新纸币接受金额信息
	void RenewCoinReceivedCount(tCHACoinValueInfo chAmountInfo);					// 更新硬币接受个数信息
	void RenewBanknoteReceivedCount(tBncCashInfo& bh_status);			// 更新纸币接受个数信息
	bool CalculateChangeCount();											// 从changeAmount计算changeCount
	void EndInsertTimer();													// 停止计数中的计时器
	void InsertMoneyNeedStartTimer();										// 开启投钱计数到返回计时器
	void ReturnWhenCoinInserted();											// 当硬币投入足够数量为达到要求时，退还所收的硬币及纸币。
	void ReturnWhenBanknotesInserted();										// 当纸币投入数量或总额足够多时，退还所有纸币及硬币。

	string GetFailedReason();

private:
	int m_timerInterval;


// 武汉11号线，修改添加：
public:
	// 从配置文件取票价列表
	vector<int> GetTicketAmountTable();
	vector<int> m_vecTicketAmount;
	// 从配置文件取售票张数列表
	vector<int> GetTicketCountTable();
	vector<int> m_vecTicketCount;
	// 循环读卡相关
	CString m_CardLogicalNumber;// 充值业务初次读到的合法卡号
	bool m_bReaderBad;			// 是否读写器发生异常
	CString m_strReadCardRsp;	// 读卡反馈提示信息
	static DWORD WINAPI DoCircleReadCard(LPVOID lpParameter);			//创建循环读卡线程  
	long StartCircleReadCard();
	long StopCircleReadCard();
	virtual long EmptyCardInfo();
	long StopReceiveMoney();

	// 二维码购票相关
	void EndQueryOrderTimer();											// 停止订单查询计时器
	void InsertQueryOrderTimer();										// 开启订单查询计时器

	long StartWebThread();
	static DWORD WINAPI WebCommunicateFunction(LPVOID lpParameter);
	void RequestOrder();
	void QueryOrder();
	void ConfirmOrder();
	void CancelOrder();

	// 硬币接收相关
	bool m_bCoinHopperAccept;

	void OnErrorFinish();
	void OnSuccessFinish();

	volatile bool m_bStopCircleRead;
	volatile bool m_bCircleReading;
	RW_READ_CARD_RESPONSE m_readResponse;
	volatile bool m_bRechargeConfirm;
	volatile bool m_bRechargeOverlimit;
private:
	bool m_bExitWebThread;
	HANDLE m_hWebEvent;
	int m_nCurWebOper;
	bool m_bWebPaySuccess;
public:
	typedef enum enum_pay_type{
		PAY_WITH_NORMAL		= 1,	// 正常支付方式（现金）
		PAY_WITH_SVT		= 2,	// 储值票支付购票
		PAY_WITH_WEIXIN		= 3,	// 微信支付
		PAY_WITH_ZHIFUBAO	= 4,	// 支付宝支付
	}PAY_TYPE;

	int m_ePayType;											// 购票方式
};
