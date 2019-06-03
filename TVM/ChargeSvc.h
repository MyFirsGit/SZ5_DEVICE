#pragma once

#include "PassengerForeSvc.h"
#include "ReceptionStaticInfo.h"

static const TCHAR* const WM_WAIT_TO_INIT = _T("WM_WAIT_TO_INIT");
static const TCHAR* const WM_WAIT_TO_ERROR = _T("WM_WAIT_TO_ERROR");
static const TCHAR* const WM_INIT_TO_WAIT = _T("WM_INIT_TO_WAIT");
static const TCHAR* const WM_INIT_TO_FINISH = _T("WM_INIT_TO_FINISH");
static const TCHAR* const WM_INIT_TO_ERROR = _T("WM_INIT_TO_ERROR");

/**
@brief      充值业务Service
*/
class CChargeSvc : public CPassengerForeSvc
{
     DECLARE_MESSAGE_MAP()

public:

	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT = 1,
		DIALOG_GROUP_INIT,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR
	} DIALOG_GROUP;

	//读卡状态
	typedef enum _readcard_info
	{
		NONE_READ = 0,
		NOT_COMPARE,
		COMPARE,
		ABORD,			// 终止读卡
	}READCARD;

	// 可充值的产品类型
	typedef enum _available_recharge_type{
		TYPE_NULL = 0,
		TYPE_SVT  = 1,
		TYPE_MST  = 2,
		TYPE_PST  = 4,
	}AVAILABLE_RECHARGE; 

	// SVC数据模型
	struct DataModel : public CommonModel
	{
		// 在充值模型中，产品类型对应的产品信息记于此。票卡中的剩余量记于PRODUCT_INFO中lBalanceAmount和lAmount。
		// PRODUCT_INFO中lValidatePeriod（因没有对应信息从票卡读出）不使用。
		long lAddAmount;					// SVT充值净增加额度，PST不使用
		long lAddRides;						// MST充值净增加次数，PST不使用
		Duration_t lValidatePeriod;			// PST时长
		

		vector<PRODUCT_INFO> vecTicketProduct;	// 车票内可充值子产品记录体
		PRODUCT_INFO oldProductInfo;			// 被充值产品充值前状态
		unsigned long lAvailableRechargeProduct;// 卡内可充值的种类
		map<CString,bool> mapAvailableProduct;// 卡内可充值的所有子产品map<主产品类型乘以产品子类型,true>假设子产品类型不大于10
	};

	CChargeSvc();
	virtual ~CChargeSvc();

	DataModel& GetDataModel();

	bool GetCurrentProductInfo(WORD productType, WORD productSubType, PRODUCT_INFO& ticketProduct); // 获取卡内产品信息

	// 业务相关顺序流程
	long CalculatePrice();							// 获取产品票价	
	long DoCardCharge();							// TPU充值处理(储值/定期/计次)
	bool ConfirmCard(LPRW_READ_CARD_RESPONSE);		// 确认第二次刷卡是否是刚才的卡
	void PrinterPrintReceipt();
	CString GetAddProductTime(Duration_t date);     // 对应转换充值天数：30天
	
//	bool POSPayment();												// 银行卡支付
//	bool POSCancle();												// 银行卡冲正
//	bool ChargeCard();												// 充值流程
	bool IsBanknoteMaxCount();
	bool IsCoinMaxCount();
	void PlayAudioInDlg(DWORD eventID);								// 在界面中播放语音
	COIN_ACCEPTABLE GetAcceptCoinFaceTable();						// 获取允许接收硬币面额信息
	BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList* table = NULL);					// 获取允许接收纸币面额信息
	void ReturnFinishDlg();											// 返回完成界面
	CString GetCurrentUnitOfProduct();

	void NotifyDataModelChanged();
	void UpdateReceptDlg(CString strTxt);						// 更新乘客界面倒计时提示

	void FlowToFinishDlg();								
	void FlowToErrorDlg();								
	void FlowToDefaultDlg();

	virtual void OnError(CSysException e);
protected:

	// 在这个类的构造函数中，让*m_pDataModel和PassengerForeSvc里的*m_pCommonModel指向同一个struct。
	// 这样当调用这里的method时，使用*m_pDataModel，继承的更多struct变量在这里可视。
	// 在使用PassengerForeSvc里的method时，通过*m_pCommonModel则仍可操作DataModel中CommonModel的部分。
	DataModel *m_pDataModel;

	// 业务相关顺序流程
	long SaveCardInfo(LPRW_READ_CARD_RESPONSE); // 保存车票数据到DataModel
	void SaveSCData();
	void WriteDBData(Data::TransactionRecord &dbInfo);
	//void FlowToFinishDlg(); // 供OnSuccessFinish使用的一个函数封装
	//void FlowToErrorDlg(); // 供OnErrorFinish，OnCancel和各种超限使用的一个函数封装

	//void OnChargeInsertCard(UINT,CIOCardPosMonitor*);					// 乘客插入卡后的回调函数

	// 业务流相关异常流程
	void PrinterPrintErrorReceipt();

protected:
	virtual void OnStart();
	
private:

	// 纸币金额超限相关函数。之所以放在具体Svc层，因为从theAFC_TVM_OPERATION取来的纸币限制，对售票和找零不同
	bool IsBothCoinBanknoteMaxCount();	
	bool IsBanknoteMaxAmount();
	bool IsCoinChageMaxCount();										// 判断找零是否达到最大数量？
	// 纸币找零数量判断
	bool IsBHChangeMaxCount();
	bool IsBHChangeMaxAmout();

	READCARD  m_enumIsReadedCard;									// 判断是否读过卡
	
	// 盲人购票功能
	void OnTimer(UINT timerID);										// 计时器
	CChargeAudioHelper* m_pChargeAudioHelper;						// 盲人购票语言播放指针
	void	PlayEventAudio(DWORD eventID,bool bIsStopRepeat = false);	// 封装语言播报接口，启动计时器用
	//void	OnPinPadPressed(UINT,CTVMPinPadStartInput*);				// 密码键盘回调函数
//	void    TurnOnPinPadAndTimer(bool bIsStartPinPad = true,bool bIsStartOutOfTimer = true);	// 启动密码键盘监控及操作超时计时器
								// 在界面中播放语音
	DWORD m_dwLastAudio;											// 上一句播放的语音ID
	BLIND_OPERATION_STATUS m_operation_status;
	bool    m_bIsPinPadStoped;										// 密码键盘是否关闭
	vector<int> m_vPassengerInput;									// 乘客的连续输入
	map<int,DWORD> m_KeyValue;										// 当前对应语句的键值对应值
	int m_VoicePlayTime;

	void InitDataModel();

public:
	long EmptyCardInfo();
};
