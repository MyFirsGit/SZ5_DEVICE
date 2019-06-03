#include "stdafx.h"
#include "TicketHelper.h"
#include "Validation.h"
#include "Util.h"
#include "CommonDefine.h"
#include "CPUCardInfo.h"
#include "setting.h"


CTicketHelper CTicketHelper::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTicketHelper::CTicketHelper(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTicketHelper::~CTicketHelper(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
ISSUANCER_CODE CTicketHelper::GetCardIssuancer(TicketType_t &ticketType)
{
	return ACC;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得发售票卡的物理类型

@param      (i)TicketType_t& ticketType 车票种类

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CARD_MEDIUM_TYPE CTicketHelper::GetMediumType(TicketType_t& ticketType)
{
	CARD_MEDIUM_TYPE tmpMediumType = RESERVE;
	switch(ticketType)
	{
	case TICKET_SINGLE_RIDE:
	case TICKET_EXIT_ONLY:
	case TICKET_WELFARE:
		tmpMediumType = ULTRALIGHT;
		break;
	case TICKET_YKT_STORE_VALUE:
		tmpMediumType = CPU_TICKET;
		break;
	default:
		tmpMediumType = RESERVE;
		break;
	}
	return tmpMediumType;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得票卡发售的方式（从内部，还是从外部）

@param      (i)TicketType_t& ticketType 车票种类

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CTicketHelper::IssueWay CTicketHelper::GetIssueWay(TicketType_t& ticketType)
{
	// 内部发售（单程票 | 往返票 | 一日票 | 出站票）
	TicketType_t byInnerTpuTickets[] = {TICKET_WELFARE,TICKET_SINGLE_RIDE,TICKET_ROUND,TICKET_ONE_DAY,TICKET_EXIT_ONLY};
	if(CValidation::IsInRange(ticketType,byInnerTpuTickets)){
		// TH缩退模式
		if(theSETTING.GetTHMode() == TH_MODE_ABNORMAL){
			return BY_INNER_CONVERTTO_OUTER_TPU;
		}
			// TH正常模式
		else
		{
			return BY_INNER_TPU;
		}
		
	}
	// 外部发售（其他票种）
	else{
		return BY_OUTER_TPU;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否是纪念票（未使用）

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTicketHelper::IsCommemorate(TicketType_t& ticketType)
{
	TicketType_t commemortateTickets[] = {TICKET_COMMEMORATIC_MULTIRIDE,TICKET_COMMEMORATIC_PERIOD,TICKET_COMMEMORATIC_STORE_VALUE};
	return CValidation::IsInRange(ticketType,commemortateTickets);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      能否按现金售卖票

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTicketHelper::CanSaleByCash(TicketType_t& ticketType)
{
	//TicketType_t canSaleByCashTickets[] = {TICKET_SINGLE_RIDE,TICKET_COMMEMORATIC_PERIOD,TICKET_COMMEMORATIC_STORE_VALUE};
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      票是否是计名票

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTicketHelper::IsTicketNamed(TicketType_t& ticketType)
{
	return false;
	//TicketType_t namedTicets[] = {TICKET_COMMEMORATIC_MULTIRIDE,TICKET_COMMEMORATIC_PERIOD,TICKET_COMMEMORATIC_STORE_VALUE};
	//return CValidation::IsInRange(ticketType,commemortateTickets);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      票卡是否为非回收类储值票

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTicketHelper::IsTicketStoredValue(TicketType_t& ticketType)
{
	bool ret = false;
	if(theACC_TICKET.IsNotCollectedTicket(ticketType)){
		ProductCategory_t ticketCategory = PERIOD;	
		theACC_TICKET.GetTicketCategry(ticketType,ticketCategory);
		if(ticketCategory == PURSE){
			ret = true;
		}
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取卡片类型

@param      (i)BYTE		cardPHType		卡片物理类型

@retval     BYTE	卡片类型

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BYTE CTicketHelper::GetCardType(BYTE cardPHType)
{
	BYTE cardType;

	//0x01：一卡通S50/S70卡片；0x02：一卡通CPU卡；0x11：一票通S50/S70卡片；0x12：一票通UL卡；0x13：一票通CPU卡；
	switch(cardPHType)
	{
	case ULTRALIGHT:
		cardType = 0x12;
		break;
	case MIFARE1K:
		cardType = 0x11;
		break;
	case MIFARE4K:
		cardType = 0x11;
		break;
	case CPU_TICKET:
		cardType = 0x13;
		break;
	default:
		cardType = 0x00;
		break;
	}

	return cardType;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得CPU票卡状态名称

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCPUCardTicketStatusName(BYTE status)
{
	switch(status)
	{
	case CCPUCard::UNISSUE:
		return _T("未售");
	case CCPUCard::PRECHARGED:
		return _T("预充值");
	case CCPUCard::ISSUED:
		return _T("已发售");
	case CCPUCard::RECYCLED:
		return _T("回收");
	case CCPUCard::BLACK_LOCKED:
		return _T("黑名单锁定");
	default:
		return _T("未知");
	}
	return _T("未知");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得CPU卡启用状态标志

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCPUCardEnableStatusName(BYTE enableStatus)
{
	switch(enableStatus)
	{
	case CCPUCard::DISABLED:
		return _T("未启用");
	default:
		return _T("已启用");
	}
	return _T("已启用");
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得CPU卡锁卡状态标志

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCPUCardLockStatusName(BYTE lockStatus)
{
	switch(lockStatus)
	{
	case CCPUCard::LOCKED:
		return _T("已锁定");
	case CCPUCard::UNLOCKED:
		return _T("未锁定");
	default:
		return _T("未知");
	}
	return _T("未知");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得CPU卡激活状态标志

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCPUCardActiveStatusName(BYTE activeStatus)
{
	switch(activeStatus)
	{
	case CCPUCard::ACTIVED:
		return _T("已激活");
	case CCPUCard::UNACTIVED:
		return _T("未激活");
	default:
		return _T("未知");
	}
	return _T("未知");
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得CPU卡测试状态名称

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCPUCardTestStatusName(BYTE testStatus)
{
	switch(testStatus)
	{
	case CCPUCard::FORMAL_CARD:
		return _T("正式卡");
	case CCPUCard::INVALID_CARD:
		return _T("无效卡");
	case CCPUCard::TEST_CARD:
		return _T("测试卡");
	}
	return _T("测试卡");
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得CPU卡互通标志名称

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCPUCardExchangeStatusName(BYTE exchangeStatus)
{
	switch(exchangeStatus)
	{
	case CCPUCard::LOCAL_CARD:
		return _T("本地卡");
	case CCPUCard::OTHER_PLACE_CARD:
		return _T("异地卡");
	default:
		return _T("未知");
	}
	return _T("未知");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得CPU卡旅程状态名称

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCPUCardJourneyStatusName(BYTE journeyStatus)
{
	switch(journeyStatus)
	{
	case CCPUCard::UNENTER_STATION:
		return _T("已出站");
	//case CCPUCard::ENTERED_STATION:
	//	return _T("已进站";
	//case CCPUCard::EXITED_STATION:
	//	return _T("已出站";
	default:
		return _T("已进站");
	}
	return _T("未知");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得UL卡测试标志名称

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetULCardTestStatusName(BYTE testStatus)
{
	switch(testStatus)
	{
	case CULCard::FORMAL_CARD:
		return _T("正式卡");
	case CULCard::INVALID_CARD:
		return _T("无效卡");
	case CULCard::TEST_CARD:
		return _T("测试卡");
	default:
		return _T("未知");
	}
	return _T("未知");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得UL卡票卡状态名称

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetULCardTicketStatusName(BYTE ticketStatus)
{
	switch(ticketStatus)
	{
	case CULCard::ISSUED:
		return _T("已发售");
	case CCPUCard::UNISSUE:
		return _T("未发售");
	case CCPUCard::RECYCLED:
		return _T("已回收");
	default:
		return _T("未知");
	}
	return _T("未知");
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得UL卡激活状态名称

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetULCardActiveStatusName(BYTE activeStatus)
{
	switch(activeStatus)
	{
	case CULCard::ACTIVED:
		return _T("已激活");
	case CULCard::UNACTIVED:
		return _T("未激活");
	default:
		return _T("未知");
	}
	return _T("未知");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得UL卡旅程状态名称

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetULCardJourneyStatusName(BYTE journeyStatus)
{
	switch(journeyStatus)
	{
	case CULCard::UNENTER_STATION:
		return _T("未进站");
	case CULCard::ENTERED_STATION:
		return _T("已进站");
	case CULCard::EXITED_STATION:
		return _T("已出站");
	default:
		return _T("未知");
	}
	return _T("未知");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得记名卡证件类型名称

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCertifateTypeName(BYTE certifateType)
{
	switch(certifateType)
	{
	case CERTIFICATE_ID_CARD_MAINLAND:
		return _T("身份证");
	//case CERTIFICATE_STUDENT_CARD:
	//	return _T("学生证");
	//case CERTIFICATE_MILITARY_OFFICER:
	//	return _T("军官证");
	//case CERTIFICATE_POST_CARD:
	//	return _T("外国人永久居留证");
	default:
		return _T("未知");
	}
	return _T("未知");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     更新TH票箱状态，票箱计数 

@param     (i)BYTE boxACount
@param     (i)BYTE boxBCount
@param     (i)BYTE collectionCount

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTicketHelper::UpdateTHCount(BYTE boxACount,BYTE boxBCount,BYTE collectionCount)
{
	CCTLOGCountInfo::TICKETBOX_INFO& ticketboxAInfo = theCOUNT_INFO.GetTicketboxAInfo();
	CCTLOGCountInfo::TICKETBOX_INFO& ticketboxBInfo = theCOUNT_INFO.GetTicketboxBInfo();
	CCTLOGCountInfo::TICKETBOX_INFO& ticketboxCInfo = theCOUNT_INFO.GetTicketboxCInfo();

	ticketboxAInfo.ulCurCount -= boxACount;
	ticketboxBInfo.ulCurCount -= boxBCount;
	ticketboxCInfo.ulCurCount += collectionCount;
	theCOUNT_INFO.SetTicketboxAInfo(ticketboxAInfo);
	theCOUNT_INFO.SetTicketboxBInfo(ticketboxBInfo);
	theCOUNT_INFO.SetTicketboxCInfo(ticketboxCInfo);

	CAfcBomOperationParam::MAGAZINE_PARAM magazineParam;
	theAFC_BOM_OPERATION.GetMagazineParam(magazineParam);
	MAGAZINE_NUM_STATUS ticketBoxAStatus = MAG_NUM_EMPTY;
	if(ticketboxAInfo.ulCurCount< magazineParam.haveEmptyValue)
	{
		ticketBoxAStatus = MAG_NUM_EMPTY;
	}
	else if (ticketboxAInfo.ulCurCount< magazineParam.willEmptyValue)
	{
		ticketBoxAStatus = MAG_NUM_NEAR_EMPTY;
	}
	else
	{
		ticketBoxAStatus = MAG_NUM_NORMAL;
	}
	if(theBOM_STATUS_MGR.GetMagANumStatus() != ticketBoxAStatus)
	{
		theBOM_STATUS_MGR.SetMagANumStatus(ticketBoxAStatus);
	}
	MAGAZINE_NUM_STATUS ticketBoxBStatus = MAG_NUM_EMPTY;
	if(ticketboxBInfo.ulCurCount< magazineParam.haveEmptyValue)
	{
		ticketBoxBStatus = MAG_NUM_EMPTY;
	}
	else if (ticketboxBInfo.ulCurCount< magazineParam.willEmptyValue)
	{
		ticketBoxBStatus = MAG_NUM_NEAR_EMPTY;
	}
	else
	{
		ticketBoxBStatus = MAG_NUM_NORMAL;
	}
	if(theBOM_STATUS_MGR.GetMagBNumStatus() != ticketBoxBStatus)
	{
		theBOM_STATUS_MGR.SetMagBNumStatus(ticketBoxBStatus);
	}
	MAGAZINE_NUM_STATUS collectionBoxStatus = MAG_NUM_NORMAL;
	if(ticketboxCInfo.ulCurCount>magazineParam.haveInvalidFullValue)
	{
		collectionBoxStatus = MAG_NUM_FULL;
	}
	else if(ticketboxCInfo.ulCurCount>magazineParam.willInvalidFullValue)
	{
		collectionBoxStatus = MAG_NUM_NEAR_FULL;
	}
	else
	{
		collectionBoxStatus = MAG_NUM_NORMAL;
	}
	if(theBOM_STATUS_MGR.GetCycleBoxNumStatus() != collectionBoxStatus)
	{
		theBOM_STATUS_MGR.SetCycleBoxNumStatus(collectionBoxStatus);
	}
}
