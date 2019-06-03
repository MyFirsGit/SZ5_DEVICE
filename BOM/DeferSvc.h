#pragma once
#include "BOMForeService.h"
#include "WaitingOfficeDlg.h"
#include "ReceptionDlg.h"

/**
@brief      激活业务Service
*/
class CDeferSvc :	public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT = 1,
		DIALOG_GROUP_DEFER_NOT,
		DIALOG_GROUP_BASE,
		DIALOG_GROUP_FINISH
	} DIALOG_GROUP;
	typedef struct _tagDataModel
	{
		RW_READ_CARD_RESPONSE readCardResponse;
		TicketType_t ticketType;
		CAccTicketParam::TICKET_TYPE ticketTypeDetail;
		ProductCategory_t cardCategory;
		int balance;
		_DATE_TIME orignalValidate;
		int deferDay;
		_DATE_TIME newValidate;
		long paid;								  //已付金额
		long due;								  //应付金额
		long change;						  //找零
		CCPUCard* pCPUCard;
		CULCard* pULCard;
		RW_CMD_DEFER deferInput;
		_tagDataModel()
		{
			ticketType = 0;
			deferDay = 0; 
			paid = 0;
			due =0;
			change = 0;
			pCPUCard = NULL;
			pULCard = NULL;
		}
	} DataModel;

	CDeferSvc();
	~CDeferSvc();
	long DoDefer();
	DataModel& GetDataModel();
	virtual void NotifyDataModelChanged();
protected:
	void OnReadCard(UINT,CRWReadCard*);
	 long BlockCard(UINT resultCode,RW_BUSSINESS_RESPONSE response);
	 void OnError(CString strGuideInfo);
private:
	DataModel m_DataModel;
	void PrinterPrint();
};
