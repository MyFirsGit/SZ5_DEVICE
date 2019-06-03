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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTicketHelper::CTicketHelper(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief      ȡ�÷���Ʊ������������

@param      (i)TicketType_t& ticketType ��Ʊ����

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
@brief      ȡ��Ʊ�����۵ķ�ʽ�����ڲ������Ǵ��ⲿ��

@param      (i)TicketType_t& ticketType ��Ʊ����

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CTicketHelper::IssueWay CTicketHelper::GetIssueWay(TicketType_t& ticketType)
{
	// �ڲ����ۣ�����Ʊ | ����Ʊ | һ��Ʊ | ��վƱ��
	TicketType_t byInnerTpuTickets[] = {TICKET_WELFARE,TICKET_SINGLE_RIDE,TICKET_ROUND,TICKET_ONE_DAY,TICKET_EXIT_ONLY};
	if(CValidation::IsInRange(ticketType,byInnerTpuTickets)){
		// TH����ģʽ
		if(theSETTING.GetTHMode() == TH_MODE_ABNORMAL){
			return BY_INNER_CONVERTTO_OUTER_TPU;
		}
			// TH����ģʽ
		else
		{
			return BY_INNER_TPU;
		}
		
	}
	// �ⲿ���ۣ�����Ʊ�֣�
	else{
		return BY_OUTER_TPU;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ��Ǽ���Ʊ��δʹ�ã�

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
@brief      �ܷ��ֽ�����Ʊ

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
@brief      Ʊ�Ƿ��Ǽ���Ʊ

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
@brief      Ʊ���Ƿ�Ϊ�ǻ����ഢֵƱ

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
@brief      ��ȡ��Ƭ����

@param      (i)BYTE		cardPHType		��Ƭ��������

@retval     BYTE	��Ƭ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BYTE CTicketHelper::GetCardType(BYTE cardPHType)
{
	BYTE cardType;

	//0x01��һ��ͨS50/S70��Ƭ��0x02��һ��ͨCPU����0x11��һƱͨS50/S70��Ƭ��0x12��һƱͨUL����0x13��һƱͨCPU����
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
@brief      ȡ��CPUƱ��״̬����

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
		return _T("δ��");
	case CCPUCard::PRECHARGED:
		return _T("Ԥ��ֵ");
	case CCPUCard::ISSUED:
		return _T("�ѷ���");
	case CCPUCard::RECYCLED:
		return _T("����");
	case CCPUCard::BLACK_LOCKED:
		return _T("����������");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��CPU������״̬��־

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
		return _T("δ����");
	default:
		return _T("������");
	}
	return _T("������");
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��CPU������״̬��־

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
		return _T("������");
	case CCPUCard::UNLOCKED:
		return _T("δ����");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��CPU������״̬��־

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
		return _T("�Ѽ���");
	case CCPUCard::UNACTIVED:
		return _T("δ����");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��CPU������״̬����

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
		return _T("��ʽ��");
	case CCPUCard::INVALID_CARD:
		return _T("��Ч��");
	case CCPUCard::TEST_CARD:
		return _T("���Կ�");
	}
	return _T("���Կ�");
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��CPU����ͨ��־����

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
		return _T("���ؿ�");
	case CCPUCard::OTHER_PLACE_CARD:
		return _T("��ؿ�");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��CPU���ó�״̬����

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
		return _T("�ѳ�վ");
	//case CCPUCard::ENTERED_STATION:
	//	return _T("�ѽ�վ";
	//case CCPUCard::EXITED_STATION:
	//	return _T("�ѳ�վ";
	default:
		return _T("�ѽ�վ");
	}
	return _T("δ֪");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��UL�����Ա�־����

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
		return _T("��ʽ��");
	case CULCard::INVALID_CARD:
		return _T("��Ч��");
	case CULCard::TEST_CARD:
		return _T("���Կ�");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��UL��Ʊ��״̬����

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
		return _T("�ѷ���");
	case CCPUCard::UNISSUE:
		return _T("δ����");
	case CCPUCard::RECYCLED:
		return _T("�ѻ���");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��UL������״̬����

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
		return _T("�Ѽ���");
	case CULCard::UNACTIVED:
		return _T("δ����");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��UL���ó�״̬����

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
		return _T("δ��վ");
	case CULCard::ENTERED_STATION:
		return _T("�ѽ�վ");
	case CULCard::EXITED_STATION:
		return _T("�ѳ�վ");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ü�����֤����������

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
		return _T("���֤");
	//case CERTIFICATE_STUDENT_CARD:
	//	return _T("ѧ��֤");
	//case CERTIFICATE_MILITARY_OFFICER:
	//	return _T("����֤");
	//case CERTIFICATE_POST_CARD:
	//	return _T("��������þ���֤");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ����THƱ��״̬��Ʊ����� 

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
