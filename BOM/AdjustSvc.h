#pragma once
#include "bomforeservice.h"
#include "ReceptionStaticInfo.h"
#include "CommonDefine.h"

using namespace std;

static const TCHAR* const WM_ADJUST_WAIT_TO_INIT = _T("WM_ADJUST_WAIT_TO_INIT");
static const TCHAR* const WM_ADJUST_INIT_TO_FINISH	= _T("WM_ADJUST_INIT_TO_FINISH");
static const TCHAR* const WM_ADJUST_WAIT_TO_FINISHT	= _T("WM_ADJUST_WAIT_TO_FINISHT");
static const TCHAR* const WM_WAIT_TO_ERROR = _T("WM_WAIT_TO_ERROR");
static const TCHAR* const WM_INI_TO_ERROR = _T("WM_INI_TO_ERROR");
static const TCHAR* const STR_ADJUST_INFO = _T("补票信息");


class CAdjustSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	// 画面分组ID
	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT=1,
		DIALOG_GROUP_INIT,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR,
	} DIALOG_GROUP;
	typedef struct _tagModel
	{
		bool isPaidArea;					//是否是付费区

		TicketType_t ticketType;			//票卡种类
		CString strTicketTypeName;			//车票种类文言
		CString strTicketTypeEnName;		//车票种类英文文言
		ProductCategory_t ticketCategory;		//票卡归类
		CARD_MEDIUM_TYPE cardPHType;		//票卡物理类型
		CString strTicketNumber;			//票卡号
		BYTE journeyStatus;					//旅程状态
		WORD saleStation;					//售卖车站
		_DATE_TIME saleTime;				//售卖时间
		WORD enterStation;					//进站车站
		_DATE_TIME enterDateTime;			//进站时间
		WORD exitStation;					//出站车站
		_DATE_TIME exitDateTime;			//出站时间
		long balance;						//票卡余额
		
		long adjustAmount;					//补票金额
		long actualAdjustAmount;			//实际补票金额
		BYTE adjustCode;					//补票建议代码
		CString strAdjustGuide;				//补票建议文言
		CString strAdjustGuideOfReceipt;		//乘客补票建议文言
		CString strAdjustEnGuideOfReceipt;		//乘客补票英文建议文言
		_PAYED_METHOD payMethod;				//补票方式
		BYTE overTimeReason;					//超时时，超时原因 0：未超时， 1：乘客原因，2：地铁原因

		bool canSelectStation;					//是否可以选择站点
		int	 availablePayMethod;				//可以选择补票方式 1，现金，2，卡内，3，现金和卡内

		long due;							//应收
		long paid;							//实收
		long change;						//找零

		_tagModel()
		{
			isPaidArea = true;
			ticketType = TICKET_SINGLE_RIDE;
			strTicketTypeName = "";	
			strTicketTypeEnName = "";
			ticketCategory = PURSE;	
			cardPHType = ULTRALIGHT;
			strTicketNumber="";
			journeyStatus = 0;
			saleStation=0;			
			enterStation=0;	
			exitStation=0;
			balance=0;
			adjustAmount=0;		
			actualAdjustAmount = 0;
			adjustCode=0;					
			strAdjustGuide="";
			strAdjustGuideOfReceipt="";
			strAdjustEnGuideOfReceipt="";
			payMethod=BY_CASH;				
			overTimeReason = 0;

			canSelectStation = false;					
			availablePayMethod = 0x01;

			due=0;							//应收
			paid=0;							//实收
			change=0;						//找零
		}

	}Model;
	void SetIsPaidArea(bool isPaidArea);
	void OnOK();
	Model& GetDataModel();
	void NotifyDataModelChanged();
	CAdjustSvc();
	~CAdjustSvc();

protected:
	void OnStart();
	void OnReadCard(UINT,CRWReadCard*);
	long BlockCard(UINT resultCode,RW_BUSSINESS_RESPONSE response);
	void PrinterPrint();

private:
	
	bool IsAdjustCodeValid();
	bool IsNeedAdust();

	void OnCardError(CString errorMsg);
	void Adjust();
	bool SaveCardInfo(LPRW_READ_CARD_RESPONSE);
	void SaveBusinessData(LPRW_BUSSINESS_RESPONSE);

	
	Model m_DataModel;

};
