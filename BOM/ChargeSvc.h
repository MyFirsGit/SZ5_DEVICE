#include "bomforeservice.h"
#include "ReceptionStaticInfo.h"

#pragma once

static const TCHAR* const WM_TO_CHARGE_BASE_DLG       =  _T("WM_TO_CHARGE_BASE_DLG");                           // 充值基本画面
static const TCHAR* const WM_TO_CHARGE_NOT_DLG		 =  _T("WM_TO_CHARGE_NOT_DLG");                            // 充值不能画面
static const TCHAR* const WM_TO_CHARGE_FINISH_DLG     =  _T("WM_TO_CHARGE_FINISH_DLG");
static const TCHAR* const WM_BASE_TO_CHARGE_NOT_DLG   =  _T("WM_BASE_TO_CHARGE_NOT_DLG");

/**
@brief      充值业务Service
*/
class CChargeSvc :
    public CBOMForeService
{
    DECLARE_MESSAGE_MAP()

public:
	CChargeSvc();
	~CChargeSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT = 1,
		DIALOG_GROUP_BASE,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_CHARGE_NOT
	} DIALOG_GROUP;
	
	typedef struct _tagModel
	{
		TicketType_t ticketType;		//票卡种类
		ProductCategory_t ticketCategory;	//票卡归类
		CARD_MEDIUM_TYPE cardPHType;	//票卡物理类型
		BYTE cardType;					//
		CString strTicketNumber;		//车票编号
		CString strTicketName;			//车票名称
		CString strTicketEnName;		//车票英文名称
		_DATE_TIME validDate;			//有效期
		long balance;					//票卡余额
		long deposit;					//押金
		long chargeValue;				//充值金额
		long valueAfterCharge;			//充值后金额
		long due;						//应付
		long paid;						//实收
		long change;					//找零

		_DATE_TIME transTime;

		_tagModel()
		{
			strTicketNumber = "";
			strTicketName = "";
			strTicketEnName = "";
			balance = 0;
			deposit = 0;
			chargeValue = 0;
			valueAfterCharge = 0;
			due = 0;
			paid = 0;
			change = 0;
		}

	}Model;

	long DoCardCharge();                                                    // CPU充值处理
	long ChargeOverTime(bool bcheck);                                       // 一卡通充值充值超时处理
	Model& GetDataModel();

	void NotifyDataModelChanged();
	void OnError(CString strMessage);
	CString GetTpuErrorInfo(int errCode);
private:
	
	void OnReadCard(UINT errCode, CRWReadCard* rwReadCard);
	bool SaveCardInfo(LPRW_READ_CARD_RESPONSE);
	long BlockCard(RW_BUSSINESS_RESPONSE response);
	long DoCardPurseCharge();                                               // CPU充值处理(储值/计次)
	void SaveSCData();                                                      // 保存SC审计数据
	void PrinterPrint();                                                    // 打开钱箱并打印信息
	

	Model m_DataModel;														
};
