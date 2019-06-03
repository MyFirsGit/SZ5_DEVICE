#pragma once
#include "CommonDefine.h"

#define  theTICKET_HELPER CTicketHelper::GetInstance()

class CTicketHelper
{
public:

	typedef enum _IssueWay
	{
		BY_INNER_TPU,
		BY_OUTER_TPU,
		BY_INNER_CONVERTTO_OUTER_TPU,
	}IssueWay;

	static CTicketHelper&  GetInstance(){
		return theInstance;
	}
	CARD_MEDIUM_TYPE GetMediumType(TicketType_t& ticketType);
	IssueWay GetIssueWay(TicketType_t& ticketType);
	bool IsCommemorate(TicketType_t& ticketType);
	bool CanSaleByCash(TicketType_t& ticketType);
	bool IsTicketNamed(TicketType_t& ticketType);
	bool IsTicketStoredValue(TicketType_t& ticketType);
	ISSUANCER_CODE GetCardIssuancer(TicketType_t& ticketType);

	BYTE GetCardType(BYTE cardPHType);
	BYTE GetTicketMainType(TicketType_t& ticketType);

	CString GetCPUCardTicketStatusName(BYTE status);
	CString GetCPUCardEnableStatusName(BYTE enableStatus);
	CString GetCPUCardLockStatusName(BYTE lockStatus);
	CString GetCPUCardActiveStatusName(BYTE activeStatus);
	CString GetCPUCardTestStatusName(BYTE testStatus);
	CString GetCPUCardExchangeStatusName(BYTE exchangeStatus);
	CString GetCPUCardJourneyStatusName(BYTE journeyStatus);

	CString GetULCardTestStatusName(BYTE testStatus);
	CString GetULCardTicketStatusName(BYTE ticketStatus);
	CString GetULCardActiveStatusName(BYTE activeStatus);
	CString GetULCardJourneyStatusName(BYTE journeyStatus);
	
	CString GetCertifateTypeName(BYTE certifateType);

	void UpdateTHCount(BYTE boxACount,BYTE boxBCount,BYTE collectionCount);

private:

	// 构造,析构函数
	CTicketHelper(void);
	~CTicketHelper();
	CTicketHelper(const CTicketHelper&);
	CTicketHelper& operator=(const CTicketHelper&);

	static  CTicketHelper theInstance;   // 该类的唯一接口
};