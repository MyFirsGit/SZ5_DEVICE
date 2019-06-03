#include "stdafx.h"
#include "TicketHelper.h"
#include "Validation.h"
#include "Util.h"
#include "CommonDefine.h"
#include "CPUCardInfo.h"
#include "tvmsetting.h"
#include "LanguageInfo.h"
#include "FunctionInfo.h"


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
ISSUANCER_CODE CTicketHelper::GetCardIssuancer(WORD ticketType)
{
	return ACC;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�÷���Ʊ������������

@param      (i)ProductType& ticketType ��Ʊ����

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CARD_MEDIUM_TYPE CTicketHelper::GetMediumType(CardType_t cardType)
{
	CARD_MEDIUM_TYPE tmpMediumType = RESERVE;
	switch(cardType)
	{
// 	case CARD_TYPE_MACAUPSSS_M1:
// 		tmpMediumType = MIFARE1K;
// 		break;
// 	case CARD_TYPE_MACAUPSSS_CPU:
// 		tmpMediumType = CPU_TICKET;
// 		break;
	case CARD_TYPE_SY_EPT_UL:
		tmpMediumType = ULTRALIGHT;
		break;
	case CARD_TYPE_ACC_M1:
		tmpMediumType = MIFARE1K;
		break;
	default:
		tmpMediumType = ULTRALIGHT;
		break;
	}
	return tmpMediumType;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�÷���Ʊ���Ĳ�Ʒ����

@param      (i)ProductType& ticketType ��Ʊ����

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
ProductCategory_t CTicketHelper::GetProductCategory(WORD& ticketType)
{
	ProductCategory_t tmpCardCategory = UNDEFINE;
	tmpCardCategory = (ProductCategory_t)theACC_PRODUCT.GetTicketCategry(ticketType);
	return tmpCardCategory;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�÷���Ʊ��������

@param      (i)ProductType& ticketType ��Ʊ����

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CardType_t CTicketHelper::GetCardType(WORD ticketType)
{
	CardType_t tmpCardType = CARD_TYPE_ACC_M1;
	switch(ticketType)
	{
	case SJT:
		tmpCardType = CARD_TYPE_SY_EPT_UL;
		break;
	default:		
		break;
	}
	return tmpCardType;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ACC�����Ա�־����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetACCCardTestStatusName(BYTE testStatus)
{
	switch(testStatus)
	{
	case FORMAL_CARD:
		return _T("��ʽ��");
	case TEST_CARD:
		return _T("���Կ�");
	default:
		return _T("δ֪");
	}
	return _T("δ֪");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ACC ULƱ��״̬����

@param      (i)CardStatusCode_t_UL ulticketStatus  ULƱ��״̬����
@param      (i)LANGUAGETYPE_T language			   �������ͱ���	

@retval     ULƱ��״̬����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//CString CTicketHelper::GetACCULCardBlockStatusName(CardStatusCode_t_UL ulticketStatus/*,LANGUAGETYPE_T languageTypeId */)
//{
//	// ȡ��Ʊ��״̬�����Ӧ�����Դ���
//	LANGUAGE_MSG msgCode = TXT_RETENTION;
//	switch(ulticketStatus)	{
//		// 0-Ԥ��
//		case CARD_STATUS_CODE_RETENTION:
//			msgCode = TXT_RETENTION;
//			break;
//		// 1-δ����
//		case CARD_STATUS_CODE_NOT_BLOCK:
//			msgCode = TXT_NOT_BLOCK;
//			break;
//		// 2-������
//		case CARD_STATUS_CODE_BLOCK:
//			msgCode = TXT_BLOCING;
//			break;
//		// 3-���������������ѻ��գ�
//		case CARD_STATUS_CODE_BLOCKED_SCRAP_RECOVER_CARD:		
//			msgCode = TXT_BLOCKED_SCRAP_RECOVER_CARD;
//			break;
//		default:
//			msgCode = TXT_RETENTION;
//	}
//	// ȡ��ָ�����Ե�����	
//	return _tl(msgCode);	
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ACC M1KƱ��״̬����

@param      (i)CardStatusCode_t_M1 m1kticketStatus  M1KƱ��״̬����
@param      (i)LANGUAGETYPE_T languageTypeId			   �������ͱ���	

@retval     M1KƱ��״̬����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetACCM1KCardBlockStatusName(CardStatusCode_t_M1 m1kticketStatus,LANGUAGETYPE_T languageTypeId )
{
	// ȡ��Ʊ��״̬�����Ӧ�����Դ���
	LANGUAGE_MSG msgCode = TXT_NO_SET;
	switch(m1kticketStatus){	
		// δ����
		case CARD_STATUS_CODE_NOT_BLOCKED:
			msgCode = TXT_NOT_LOCKED;
			break;
		// ��������������
		case CARD_STATUS_CODE_BLOCKED_OTHER:
			msgCode = TXT_UNEXPLAINED;
			break;
		// ���������˿����ѻ��գ�
		case CARD_STATUS_CODE_BLOCKED_SURRENDERED_CARD:
			msgCode = TXT_RECYCLING_OR_BACKCARD_LOCK;
			break;
		//// ��������SAM��������
		//case PRODUCT_STATUS_CODE_BLOCKED_HOTLISTED_SAM:
		//	msgCode = TXT_BLACK_LOCK;
		//	break;
		// ��������������
		case CARD_STATUS_CODE_BLOCKED_FAULTY_CARD:
			msgCode = TXT_BAD_CARD_LOCK;
			break;
		// ��������δ���У�
		case CARD_STATUS_CODE_BLOCKED_NOT_ISSUED:
			msgCode = TXT_UNISSUED_LOCK;
			break;
		// �����������л��ˣ�
		/*case PRODUCT_STATUS_CODE_BLOCKED_BAD_AUTOLOAD_DEBT:
			msgCode = TXT_BANKS_BAD_LOCK;
			break;*/
		// �������������˻�ע����
			/*	case PRODUCT_STATUS_CODE_BLOCKED_BANK_ACCOUNT_CLOSED:
			msgCode = TXT_BANK_CLOSE_LOCK;
			break;*/
		// ���������ѹ��ڣ�
		/*case PRODUCT_STATUS_CODE_BLOCKED_PRODUCT_OVER_LIMIT:
			msgCode = TXT_PRODUCT_EXPIRED_LOCK;
			break;*/
		// ����������ʧ��
		case CARD_STATUS_CODE_BLOCKED_LOST_STOLEN:
			msgCode = TXT_LOST_CARD_LOCK;
			break;
		// ������������������
		case CARD_STATUS_CODE_BLOCKED_CARD_BATCH:
			msgCode = TXT_BATCH_LOCK;
			break;
		// ������������δʹ�ã�
		case CARD_STATUS_CODE_BLOCKED_INACTIVE:
			msgCode = TXT_NOT_USED_LOCK;
			break;
		// ���趨ֵ
		case CARD_STATUS_CODE_UNSPECIFIED:
			msgCode = TXT_UNDEFINE;
			break;
		default:
			msgCode = TXT_UNDEFINE;
	}
	// ȡ��ָ�����Ե�����	

	if (languageTypeId==LANGUAGE_ID_CHINESE){
		return _cn(msgCode);
	}
	else if(languageTypeId==LANGUAGE_ID_ENGLISH){
		return _en(msgCode);
	}		

	return _tl(msgCode); 
}


CString CTicketHelper::GetAccM1KCardProductStatusName(ProductStatus_t_M1 m1kProductStatus)
{
	LANGUAGE_MSG msgCode = TXT_NO_SET;
	switch ( m1kProductStatus)
	{
	case PRODUCT_STATUS_CODE_NOT_BLOCKED:
		msgCode = TXT_PRODUCT_NOT_CLOCKED;
		break;
	case PRODUCT_STATUS_CODE_BLOCKED_OTHER:
		msgCode = TXT_PRODUCT_BLOCKED_OTHER;
		break;
	case PRODUCT_STATUS_CODE_BLOCKED_HOTLISTED_SAM:
		msgCode = TXT_PRODUCT_HOTLISTED_SAM;
		break;
	case PRODUCT_STATUS_CODE_BLOCKED_BAD_AUTOLOAD_DEBT:
		msgCode = TXT_PRODUCT_BAD_AUTOLOAD_DEBT;
		break;
	case PRODUCT_STATUS_CODE_BLOCKED_BANK_ACCOUNT_CLOSED:
		msgCode = TXT_PRODUCT_BANK_ACCOUNT_LIMIT;
		break;
	case PRODUCT_STATUS_CODE_BLOCKED_PRODUCT_OVER_LIMIT:
		msgCode = TXT_PRODUCT_OVER_LIMIT;
		break;
	case PRODUCT_STATUS_CODE_UNSPECIFIED:
		msgCode = TXT_UNDEFINE;
		break;
	default:
		msgCode = TXT_UNDEFINE;
		break;
	}
	return _tl(msgCode);	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ACC������״̬����

@param      ACC_CARD_ACTIVE_STATUS activeStatus ����״̬

@retval     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetACCCardActiveStatusName(ACC_CARD_ACTIVE_STATUS activeStatus)
{
	// ACC_CARD_ACTIVE_STATUS
	switch(activeStatus)
	{
		// ����
	case ACTIVED:
		return _tl(TXT_ACTIVATION);
		// δ����
	case UNACTIVED:
		return _tl(TXT_NOT_ACTIVATION);
	default:
		return _tl(TXT_UNDEFINE);
	}
	return _tl(TXT_UNDEFINE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ACC���ó�״̬����

@param      (i)ACC_CARD_JOURNEY_STATUS journeyStatus �ó�״̬����
@param      (i)LANGUAGETYPE_T languageTypeId		 �����������

@retval     ACC���ó�״̬����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetACCCardJourneyStatusName(ACC_CARD_JOURNEY_STATUS journeyStatus,LANGUAGETYPE_T languageTypeId )
{
	// ȡ��Ʊ��״̬�����Ӧ�����Դ���
	LANGUAGE_MSG msgCode = TXT_UNDEFINE;
	switch(journeyStatus){
		// ��ʼ��
		case TICKET_INIT:
			msgCode = TXT_TICKET_INIT;	
			break;
		// ����Ʊ����
		case TICKET_SJT_ISSUE:
			msgCode = TXT_TICKET_SJT_ISSUE;	
			break;
		// ע��
		case TICKET_WRITE_OFF:
			msgCode = TXT_TICKET_WRITE_OFF;	
			break;
		// �ر���
		case TICKET_REENCODE:
			msgCode = TXT_TICKET_REENCODE;	
			break;
		// ���£�Ϊ�˽�բ��
		case TICKET_UPDATE_FOR_IN:
			msgCode = TXT_TICKET_UPDATE_FOR_IN;	
			break;
		// ���£�Ϊ�˳�բ��
		case TICKET_UPDATE_FOR_OUT:
			msgCode = TXT_TICKET_UPDATE_FOR_OUT;	
			break;
		// ��������
		case TICKET_UPDATE_FOR_OTHER:
			msgCode = TXT_TICKET_UPDATE_FOR_OTHER;	
			break;
		// ��բ
		case TICKET_IN:
			msgCode = TXT_TICKET_IN;	
			break;
		// ��բ
		case TICKET_OUT:
			msgCode = TXT_TICKET_OUT;	
			break;
		// ��E/SԤ��ֵ
		case TICKET_ES_ASSIGNMENT:
			msgCode = TXT_TICKET_ES_ASSIGNMENT;	
			break;
		// Ԥ��ֵ����
		case TICKET_ASSIGNMENT_ZERO:
			msgCode = TXT_TICKET_ASSIGNMENT_ZERO;	
			break;
		// ��վƱ��Ϊ�˳�բ��
		case TICKET_ET_FOR_EXIST:
			msgCode = TXT_TICKET_ET_FOR_EXIST;	
			break;
		// �г�����ʱ��բ
		case TICKET_BEFOREOUT_IN:
			msgCode = TXT_TICKET_BEFOREOUT_IN;	
			break;
			// һ��ͨ��ä����վ����
		case YKT_DISABLED_UPDATE_FOR_IN:
			msgCode = TXT_YKT_DISABLED_UPDATE_FOR_IN;	
			break;
			// һ��ͨ��ä����վ����
		case YKT_DISABLED_UPDATE_FOR_OUT:
			msgCode = TXT_YKT_DISABLED_UPDATE_FOR_OUT;	
			break;
			// һ��ͨ��ä����բ
		case YKT_DISABLED_IN:
			msgCode = TXT_YKT_DISABLED_IN;	
			break;
			// һ��ͨ��ä����բ
		case YKT_DISABLED_OUT:
			msgCode = TXT_YKT_DISABLED_OUT;	
			break;
			// �˿�
		case REFOUND_TIKCET:
			msgCode = TXT_REFOUND_TIKCET;	
			break;
		default:
			msgCode = TXT_UNDEFINE;	
	}
	// ȡ��ָ�����Ե�����	
	if (languageTypeId==LANGUAGE_ID_CHINESE){
		return _cn(msgCode);
	}
	else if(languageTypeId==LANGUAGE_ID_ENGLISH){
		return _en(msgCode);
	}		

	return _tl(msgCode);	
	 
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ֤���������ƣ�������C����ꑣ�

@param      (i)CERTIFICATE_TYPE certifateType ֤������
@param      (i) LANGUAGETYPE_T language ��������

@retval     CString

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCertificateTypeName(CERTIFICATE_TYPE certificateType, LANGUAGETYPE_T language)
{
	// ȡ��Ʊ��״̬�����Ӧ�����Դ���
	LANGUAGE_MSG msgCode = TXT_UNDEFINE;
	switch (certificateType)
	{
	// ���֤����½��
	case CERTIFICATE_ID_CARD_MAINLAND:
		msgCode = TXT_ID_CARD_MAINLAND;
		break;
	// ���֤�����ţ�
	case CERTIFICATE_ID_CARD_MACAO:
		msgCode = TXT_ID_CARD_MACAO;
		break;
	// ���֤����ۣ�
	case CERTIFICATE_ID_CARD_HONGKONG:
		msgCode = TXT_ID_CARD_HONGKONG;
		break;
	default:
		return _tl(TXT_UNDEFINE);
	}
	// ȡ��ָ�����Ե�����	
	return theLANGUAGE_INFO.GetLanguageInfo(msgCode,language);	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ý�������

@param      (i)TRANSACTION_TYPE TransType ��������

@retval     CString

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetTransactionType(TRANSACTION_TYPE TransType,BYTE media_type, ISSUANCER_CODE issue_Code)
{
	CString msg_code(_T(""));

	// M1K��
	if ((MIFARE1K == media_type||MIFARE4K == media_type)&&issue_Code ==  ACC)
	{
		switch (TransType)
		{
		case ACC_M1K_TRANS_TYPE_ISSUE_TICKET:
			msg_code = _T("��Ʊ");
			break;
		case ACC_M1K_TRANS_TYPE_ISSUE_EXIT:
			msg_code = _T("���۳�վƱ");
			break;
		case ACC_M1K_TRANS_TYPE_ISSUE_LOVE:
			msg_code = _T("���۰���Ʊ");
			break;
		case ACC_M1K_TRANS_TYPE_ISSUE_BAGGAGE:
			msg_code = _T("��������Ʊ");
			break;
		case ACC_M1K_TRANS_TYPE_INITIALIZATION:
			msg_code = _T("��ʼ��");
			break;
		case ACC_M1K_TRANS_TYPE_PREEASSIGNMENT:
			msg_code = _T("Ԥ��ֵ");
			break;
		case ACC_M1K_TRANS_TYPE_CASH_RECHARGE:
			msg_code = _T("�ֽ��ֵ");
			break;
		case ACC_M1K_TRANS_TYPE_ADJUST_TICKET:
			msg_code = _T("��Ʊ");
			break;
		case ACC_M1K_TRANS_TYPE_SUBWAY_CONSUME_START:
			msg_code = _T("����������ʼ");
			break;
		case ACC_M1K_TRANS_TYPE_SUBWAY_CONSUME_END:
			msg_code = _T("�������ѽ���");
			break;
		case ACC_M1K_TRANS_TYPE_SVT_BUY_TICKET:
			msg_code = _T("��ֵ����Ʊ");
			break;
		case ACC_M1K_TRANS_TYPE_FOLLOW_ENTER:
			msg_code = _T("���ٽ�վ");
			break;
		case ACC_M1K_TRANS_TYPE_FOLLOW_EXIT:
			msg_code = _T("���ٳ�վ");
			break;
		case ACC_M1K_TRANS_TYPE_CANCELLATION:
			msg_code = _T("ע��");
			break;
		case ACC_M1K_TRANS_TYPE_COUNTERACT:
			msg_code = _T("����");
			break;
		case ACC_M1K_TRANS_TYPE_INSTANT_REFUND_CARD:
			msg_code = _T("��ʱ�˿�");
			break;
		case ACC_M1K_TRANS_TYPE_LOCK:
			msg_code = _T("����");
			break;
		case ACC_M1K_TRANS_TYPE_UNLOCK:
			msg_code = _T("����");
			break;
		case ACC_M1K_TRANS_TYPE_REVERSE:
			msg_code = _T("����");
			break;
		case ACC_M1K_TRANS_TYPE_OVERTIME:
			msg_code = _T("������ʱ����");
			break;
		case ACC_M1K_TRANS_TYPE_UNEXIT:
			msg_code = _T("�޳�վ����");
			break;
		case ACC_M1K_TRANS_TYPE_UNENTER:
			msg_code = _T("��������վ����");
			break;
		case ACC_M1K_TRANS_TYPE_EXTENSION_VALIDITY:
			msg_code = _T("��Ч�ڸ���");
			break;
		case ACC_M1K_TRANS_TYPE_ADMINISTRATIVE_FINE:
			msg_code = _T("��������");
			break;
		case ACC_M1K_TRANS_TYPE_ADMINISTRATIVE_PAY:
			msg_code = _T("����֧��");
			break;
		case ACC_M1K_TRANS_TYPE_ACTIVATION:
			msg_code = _T("����");
			break;
		case ACC_M1K_TRANS_TYPE_ISSUE_GROUP:
			msg_code = _T("������Ʊ");
			break;
		}
	}
	// CPU��
	else if (media_type == CPU_TICKET&&issue_Code ==  ACC)
	{
		switch (TransType)
		{
		case ACC_CPU_TRANS_TYPE_RECHARGE:
			msg_code = _tl(TXT_ANALYZE_TRADE_TYPE_CHARGE);
			break;
		case ACC_CPU_TRANS_TYPE_CONSUME:
			msg_code = _tl(TXT_ANALYZE_TRADE_TYPE_CONSUME);
			break;
		case ACC_CPU_TRANS_TYPE_COMPLEX_CONSUME:
			msg_code = _tl(TXT_ANALYZE_TRADE_TYPE_MIX_CONSUME);
			break;
		}
	}
	return msg_code;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ò�Ʊ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetAdjustTypeName(BYTE adjustType ,LANGUAGETYPE_T language )
{
	if (language == LANGUAGE_ID_UNSPECIFIED){
		language = theMAINTENANCE_INFO.GetLanguageMode();;
	}
	switch(adjustType)
	{
	//case NO_UPDATE:
	//	return theLANGUAGE_INFO.GetLanguageInfo((GUIDE_ADJUST_NOT),language);
	//case PAIDAREA_BUY_EXIT_CARD:
	//	return theLANGUAGE_INFO.GetLanguageInfo((GUIDE_ADJUST_ISSUE_OUT),language); // �����վƱ??
	//case PAIDAREA_OVERRIDE:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_RIDE),language);// ���̲�Ʊ  
	//case PAIDAREA_OVERTIME:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_TIME),language);// ��ʱ��Ʊ 
	//case PAIDAREA_DELAY_ENTRY:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_ENTRY_DALY),language);// ���ӳٽ�վ 
	//case PAIDAREA_EXIT_THEN_ENTRY:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_EXIT_THEN_ENTRY),language);// �Ȳ���վ�󲹽�վ 
	//case PAIDAREA_OVERTIMERIDE:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_RIDE_TIME),language);// ���̲�Ʊ ��ʱ��Ʊ 
	//case PAIDAREA_OVERRIDE_AGAIN:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_RIDE_AGAIN),language);// ��Ʊ�󳬳� 
	//case PAIDAREA_OVERTIME_AGAIN:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_TIME_AGAIN),language);// ��Ʊ��ʱ 
	//case PAIDAREA_OVERTIMERIDE_AGAIN:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_RIDE_TIME_AGAIN),language);// ��Ʊ�󳬳� ��Ʊ��ʱ 
	//case UNPAIDAREA_DELAY_EXIT:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_EXIT_DALY),language);// ���ӳٳ�վ 
	//case UNPAIDAREA_FREE_ENTRY:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_FREE),language);// ��ѽ�վ���߽����ţ�
	//case UNPAIDAREA_BUY_NEW_CARD:
	//	return theLANGUAGE_INFO.GetLanguageInfo((GUIDE_ADJUST_ISSUE_NEW),language);// ������Ʊ
	//case UNKNOE:
	default:
		return _tl(TXT_UNDEFINE);
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ò�Ʊ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetAdjustPayTypeName(BYTE adjustPayType ,LANGUAGETYPE_T language )
{
	if (language == LANGUAGE_ID_UNSPECIFIED){
		language = theMAINTENANCE_INFO.GetLanguageMode();;
	}
	switch(adjustPayType)
	{
	//case BY_CARD:
	//	return theLANGUAGE_INFO.GetLanguageInfo((GUIDE_ADJUST_PAY_BY_CARD),language);
	//case BY_CASH:
	//	return theLANGUAGE_INFO.GetLanguageInfo((GUIDE_ADJUST_PAY_BY_CASH),language); 
	//default:
		return _tl(TXT_UNDEFINE);
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ò�Ʒ����

@param      ProductCategory_t category ��Ʒ����

@retval     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCategoryName(ProductCategory_t category)
{
	switch(category)
	{
	case UNDEFINE:
		return _tl(TXT_UNDEFINED);
		// Ǯ��
	case PURSE:
		return _tl(TXT_PURES);
		// �ƴ�
	case MULTIRIDE:
		return _tl(TXT_MULTIRDE);
		// ����
	case PERIOD:
		return _tl(TXT_PERIOD);
	default:
		return _tl(TXT_UNDEFINED);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��֧����ʽ��Ϣ

@param      (i)WORD serviceType, ҵ��ID
@param      (i)WORD cardPHType   ����������

@retval     map<WORD,CString>  ���п���֧����ʽID������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
map<WORD,CString> CTicketHelper::GetAvailablePayTypeList(WORD serviceType,CardType_t cardPHType)
{
	/*s_AvailablePayTypeList.clear();
	bool bIsTvm = theMAINTENANCE_INFO.GetDeviceType() == DEVICE_TYPE_TVM ? true:false;
	if(s_AvailablePayTypeList.empty()){
		map<WORD,CString> allAvailablePayTypeList;
		theACC_BUSINESS.GetPayTypeParam(theAPP_SESSION.GetCurrentClientLanguageType(),allAvailablePayTypeList);		
		map<WORD,CString>::iterator it;
		for (it = allAvailablePayTypeList.begin();it != allAvailablePayTypeList.end();it++){		
			if (theFunction_INFO.IsPayTypeEnable(serviceType,cardPHType,it->first)){
				switch(it->first){
				case PAYMENT_METHOD_CASH:
					if ((((theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN) && bIsTvm
						&& (!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)))
						|| ((theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE
						&& (!theEXCEPTION_MGR.HasException(CBHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CBHException::MODULE_ID))))
						s_AvailablePayTypeList[it->first] = it->second;
					break;
				case PAYMENT_METHOD_EFT:
					if(((theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_EP) == DEVICE_CONTRL_CODE_PAYMENT_EP)
						&& (!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)))
						s_AvailablePayTypeList[it->first] = it->second;
					break;
				case PAYMENT_METHOD_BANK_CARD:
					if(((theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_BANKCARD) == DEVICE_CONTRL_CODE_PAYMENT_BANKCARD)
						&& (!theEXCEPTION_MGR.HasException(CBankCardException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CBankHardException::MODULE_ID)))
						s_AvailablePayTypeList[it->first] = it->second;
					break;
				case PAYMENT_METHOD_CREDIT_CARD:
					if((theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD == DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD)
						&&(!theEXCEPTION_MGR.HasException(CBankCardException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CBankHardException::MODULE_ID))){
							s_AvailablePayTypeList[it->first] = it->second;
					}
					break;
				default:
					break;
				}
			}			
		}		
	}*/
	return s_AvailablePayTypeList;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ���Ի�ҵ����������

@param      (i)PERSONAL_BUSINESS_TYPE businessType ���Ի�ҵ������

@retval     CString

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetPersonalizeTypeName(PERSONALIZE_TYPE businessType, LANGUAGETYPE_T language)
{
	LANGUAGE_MSG msgCode = TXT_UNDEFINE;
	switch (businessType)
	{
		// ���Ի�
	//case TYPE_PERSONAL:
	//	msgCode = TXT_BUSINESS_PERSONAL_TYPE;
	//	break;
	//	// ���Ի�����
	//case TYPE_PERSONAL_UPDATE:
	//	msgCode = TXT_BUSINESS_PERSONAL_UPDATE_TYPE;
	//	break;
	default:
		return _tl(TXT_UNDEFINE);
	}
	// ȡ��ָ�����Ե�����	
	return theLANGUAGE_INFO.GetLanguageInfo(msgCode,language);	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ���Ի�ҵ����������

@param      (i)PERSONAL_BUSINESS_TYPE businessType ���Ի�ҵ������

@retval     CString

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetPassengerGender(Sex_t passengerGender)
{
	switch(passengerGender)
	{
	case SEX_MALE:
		return _tl(TXT_MALE);
	case SEX_FEMALE:
		return _tl(TXT_FEMALE);
	case SEX_UNSPECIFIED:
		return _tl(TXT_UNDEFINE);
	default:
		return _tl(TXT_UNDEFINE);
		
	}		
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��Ч�ڼ���ʾ����

@param      (i)WORD passengerGender 

@retval     CString

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetDurationText(WORD ValidityPeriod)
{
	int validityPeriod = GetDurationValue(ValidityPeriod);
	CString strValidityPeriod = _T("");

	switch(GetDurationType(ValidityPeriod)){
		case DURATION_UNIT_SECOND:
			strValidityPeriod.Format(_T("%d"),validityPeriod);
			strValidityPeriod += _tl(SECOND_CNT_UNIT);
			break;
		case DURATION_UNIT_MINUTE:
			strValidityPeriod.Format(_T("%d"),validityPeriod);
			strValidityPeriod += _tl(MINUTE_CNT_UNIT);
			break;
		case DURATION_UNIT_HOUR:
			strValidityPeriod.Format(_T("%d"),validityPeriod);
			strValidityPeriod += _tl(HOUR_CNT_UNIT);
			break;
		case DURATION_UNIT_DAY:
			strValidityPeriod.Format(_T("%d"),validityPeriod);
			strValidityPeriod += _tl(DAY_CNT_UNIT);
			break;
		case DURATION_UNIT_WEEK:
			strValidityPeriod.Format(_T("%d"),validityPeriod);
			strValidityPeriod += _tl(WEEK_CNT_UNIT);
			break;
		case DURATION_UNIT_CALENDAR_MONTH:
			strValidityPeriod.Format(_T("%d"),validityPeriod);
			strValidityPeriod += _tl(MONTH_CNT_UNIT);
			break;
		case DURATION_UNIT_YEAR:
			strValidityPeriod.Format(_T("%d"),validityPeriod);
			strValidityPeriod += _tl(YEAR_CNT_UNIT);
			break;
		default:
			return _tl(TXT_UNDEFINE);
	}	
	return strValidityPeriod;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ���غ��֤�����

@param      (i)CERTIFICATE_TYPE certifateType ֤������
@param      (i)CString certificateNumber ֤�����

@retval     CString

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetHideCertificateID(CERTIFICATE_TYPE certificateType, CString certificateID)
{
	CString newID = certificateID;

	switch (certificateType)
	{
	case CERTIFICATE_ID_CARD_MAINLAND:
		for (int i=0; i<4; ++i) {
			newID.SetAt(10+i, '*');
		}
		break;
	case CERTIFICATE_ID_CARD_MACAO:
		for (int i=0; i<3; ++i) {
			newID.SetAt(4+i, '*');
		}
		break;
	case CERTIFICATE_ID_CARD_HONGKONG:
		for (int i=0; i<3; ++i) {
			newID.SetAt(4+i, '*');
		}
		break;
	}

	return newID;	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ��ȡ����Ʊ��   

@param		short productType ��Ʒ����
@param		short productVariantType ��Ʒ������
@param		StationNumber_t fromStation ��ʼ��վ
@param		StationNumber_t toStation ������վ

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Fare_t CTicketHelper::GetSalePrice(short productType, short productVariantType,PassengerType_t passengerType,  StationNumber_t fromStation,StationNumber_t toStation)
{
	Fare_t fare;
	return fare;
	/*CAccProductParam::SUB_PRODUCT subProdut;
	bool getSubProductSuccess = theACC_PRODUCT.GetSubProduct(productType,productVariantType,subProdut);
	if(!getSubProductSuccess)
	{
		return 0;
	}
	return theACC_SALES.GetSalePrice(subProdut.salesTableId,subProdut.salesPatternId,productType,productVariantType,passengerType,subProdut.salesCodeTableId,fromStation,toStation);*/
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ��ȡ����Ʊ��   

@param		short productType ��Ʒ����
@param		short productVariantType ��Ʒ������
@param		StationNumber_t fromStation ��ʼ��վ
@param		StationNumber_t toStation ������վ

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Fare_t CTicketHelper::GetPursePrice(short productType, _DATE_TIME dateTime,PassengerType_t passengerType,  StationNumber_t fromStation,StationNumber_t toStation)
{
	Fare_t fare;
	return fare;
	/*CAccProductParam::PRODUCT_INFO product;
	bool getProductSuccess = theACC_PRODUCT.GetProduct(productType,product);
	if(!getProductSuccess)
	{
		return 0;
	}
	TimeCode_t timeCode = theACC_CALANDER.GetTimeCode(product.calendarId,dateTime);
	return theACC_SHOP.GetPucharsePrice(product.fareCodeTableId,product.farePatternId,timeCode,passengerType,product.fareCodeTableId,fromStation,toStation);*/
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

	CAfcTVMOperationParam::MAGAZINE_PARAM magazineParam;
	theAFC_TVM_OPERATION.GetMagazineParam(magazineParam);
	MAGAZINE_NUM_STATUS ticketBoxAStatus = MAG_NUM_EMPTY;
	if(ticketboxAInfo.ulCurCount< magazineParam.TicketEmptyValue)
	{
		ticketBoxAStatus = MAG_NUM_EMPTY;
	}
	else if (ticketboxAInfo.ulCurCount< magazineParam.TicketNearEmptyValue)
	{
		ticketBoxAStatus = MAG_NUM_NEAR_EMPTY;
	}
	else
	{
		ticketBoxAStatus = MAG_NUM_NORMAL;
	}
	/*if(theBOM_STATUS_MGR.GetMagANumStatus() != ticketBoxAStatus)
	{
	theBOM_STATUS_MGR.SetMagANumStatus(ticketBoxAStatus);
	}*/
	MAGAZINE_NUM_STATUS ticketBoxBStatus = MAG_NUM_EMPTY;
	if(ticketboxBInfo.ulCurCount< magazineParam.TicketEmptyValue)
	{
		ticketBoxBStatus = MAG_NUM_EMPTY;
	}
	else if (ticketboxBInfo.ulCurCount< magazineParam.TicketNearEmptyValue)
	{
		ticketBoxBStatus = MAG_NUM_NEAR_EMPTY;
	}
	else
	{
		ticketBoxBStatus = MAG_NUM_NORMAL;
	}
	/*if(theBOM_STATUS_MGR.GetMagBNumStatus() != ticketBoxBStatus)
	{
		theBOM_STATUS_MGR.SetMagBNumStatus(ticketBoxBStatus);
	}*/
	MAGAZINE_NUM_STATUS collectionBoxStatus = MAG_NUM_NORMAL;
	if(ticketboxCInfo.ulCurCount>magazineParam.TicketFullValue)
	{
		collectionBoxStatus = MAG_NUM_FULL;
	}
	else if(ticketboxCInfo.ulCurCount>magazineParam.TicketNearFullValue)
	{
		collectionBoxStatus = MAG_NUM_NEAR_FULL;
	}
	else
	{
		collectionBoxStatus = MAG_NUM_NORMAL;
	}
	/*if(theBOM_STATUS_MGR.GetCycleBoxNumStatus() != collectionBoxStatus)
	{
		theBOM_STATUS_MGR.SetCycleBoxNumStatus(collectionBoxStatus);
	}*/
}