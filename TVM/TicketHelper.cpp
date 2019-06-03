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
ISSUANCER_CODE CTicketHelper::GetCardIssuancer(WORD ticketType)
{
	return ACC;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得发售票卡的物理类型

@param      (i)ProductType& ticketType 车票种类

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
@brief      取得发售票卡的产品种类

@param      (i)ProductType& ticketType 车票种类

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
@brief      取得发售票卡的种类

@param      (i)ProductType& ticketType 车票种类

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
@brief      取得ACC卡测试标志名称

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
		return _T("正式卡");
	case TEST_CARD:
		return _T("测试卡");
	default:
		return _T("未知");
	}
	return _T("未知");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得ACC UL票卡状态名称

@param      (i)CardStatusCode_t_UL ulticketStatus  UL票卡状态编码
@param      (i)LANGUAGETYPE_T language			   语言类型编码	

@retval     UL票卡状态名称

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//CString CTicketHelper::GetACCULCardBlockStatusName(CardStatusCode_t_UL ulticketStatus/*,LANGUAGETYPE_T languageTypeId */)
//{
//	// 取得票卡状态编码对应的文言代码
//	LANGUAGE_MSG msgCode = TXT_RETENTION;
//	switch(ulticketStatus)	{
//		// 0-预留
//		case CARD_STATUS_CODE_RETENTION:
//			msgCode = TXT_RETENTION;
//			break;
//		// 1-未锁定
//		case CARD_STATUS_CODE_NOT_BLOCK:
//			msgCode = TXT_NOT_BLOCK;
//			break;
//		// 2-已锁定
//		case CARD_STATUS_CODE_BLOCK:
//			msgCode = TXT_BLOCING;
//			break;
//		// 3-已锁定（废弃或已回收）
//		case CARD_STATUS_CODE_BLOCKED_SCRAP_RECOVER_CARD:		
//			msgCode = TXT_BLOCKED_SCRAP_RECOVER_CARD;
//			break;
//		default:
//			msgCode = TXT_RETENTION;
//	}
//	// 取得指定语言的文言	
//	return _tl(msgCode);	
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得ACC M1K票卡状态名称

@param      (i)CardStatusCode_t_M1 m1kticketStatus  M1K票卡状态编码
@param      (i)LANGUAGETYPE_T languageTypeId			   语言类型编码	

@retval     M1K票卡状态名称

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetACCM1KCardBlockStatusName(CardStatusCode_t_M1 m1kticketStatus,LANGUAGETYPE_T languageTypeId )
{
	// 取得票卡状态编码对应的文言代码
	LANGUAGE_MSG msgCode = TXT_NO_SET;
	switch(m1kticketStatus){	
		// 未锁定
		case CARD_STATUS_CODE_NOT_BLOCKED:
			msgCode = TXT_NOT_LOCKED;
			break;
		// 已锁定（其他）
		case CARD_STATUS_CODE_BLOCKED_OTHER:
			msgCode = TXT_UNEXPLAINED;
			break;
		// 已锁定（退卡或已回收）
		case CARD_STATUS_CODE_BLOCKED_SURRENDERED_CARD:
			msgCode = TXT_RECYCLING_OR_BACKCARD_LOCK;
			break;
		//// 已锁定（SAM黑名单）
		//case PRODUCT_STATUS_CODE_BLOCKED_HOTLISTED_SAM:
		//	msgCode = TXT_BLACK_LOCK;
		//	break;
		// 已锁定（坏卡）
		case CARD_STATUS_CODE_BLOCKED_FAULTY_CARD:
			msgCode = TXT_BAD_CARD_LOCK;
			break;
		// 已锁定（未发行）
		case CARD_STATUS_CODE_BLOCKED_NOT_ISSUED:
			msgCode = TXT_UNISSUED_LOCK;
			break;
		// 已锁定（银行坏账）
		/*case PRODUCT_STATUS_CODE_BLOCKED_BAD_AUTOLOAD_DEBT:
			msgCode = TXT_BANKS_BAD_LOCK;
			break;*/
		// 已锁定（银行账户注销）
			/*	case PRODUCT_STATUS_CODE_BLOCKED_BANK_ACCOUNT_CLOSED:
			msgCode = TXT_BANK_CLOSE_LOCK;
			break;*/
		// 已锁定（已过期）
		/*case PRODUCT_STATUS_CODE_BLOCKED_PRODUCT_OVER_LIMIT:
			msgCode = TXT_PRODUCT_EXPIRED_LOCK;
			break;*/
		// 已锁定（遗失）
		case CARD_STATUS_CODE_BLOCKED_LOST_STOLEN:
			msgCode = TXT_LOST_CARD_LOCK;
			break;
		// 已锁定（批次锁定）
		case CARD_STATUS_CODE_BLOCKED_CARD_BATCH:
			msgCode = TXT_BATCH_LOCK;
			break;
		// 已锁定（长期未使用）
		case CARD_STATUS_CODE_BLOCKED_INACTIVE:
			msgCode = TXT_NOT_USED_LOCK;
			break;
		// 无设定值
		case CARD_STATUS_CODE_UNSPECIFIED:
			msgCode = TXT_UNDEFINE;
			break;
		default:
			msgCode = TXT_UNDEFINE;
	}
	// 取得指定语言的文言	

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
@brief      取得ACC卡激活状态名称

@param      ACC_CARD_ACTIVE_STATUS activeStatus 激活状态

@retval     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetACCCardActiveStatusName(ACC_CARD_ACTIVE_STATUS activeStatus)
{
	// ACC_CARD_ACTIVE_STATUS
	switch(activeStatus)
	{
		// 激活
	case ACTIVED:
		return _tl(TXT_ACTIVATION);
		// 未激活
	case UNACTIVED:
		return _tl(TXT_NOT_ACTIVATION);
	default:
		return _tl(TXT_UNDEFINE);
	}
	return _tl(TXT_UNDEFINE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得ACC卡旅程状态名称

@param      (i)ACC_CARD_JOURNEY_STATUS journeyStatus 旅程状态编码
@param      (i)LANGUAGETYPE_T languageTypeId		 语言种类编码

@retval     ACC卡旅程状态名称

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetACCCardJourneyStatusName(ACC_CARD_JOURNEY_STATUS journeyStatus,LANGUAGETYPE_T languageTypeId )
{
	// 取得票卡状态编码对应的文言代码
	LANGUAGE_MSG msgCode = TXT_UNDEFINE;
	switch(journeyStatus){
		// 初始化
		case TICKET_INIT:
			msgCode = TXT_TICKET_INIT;	
			break;
		// 单程票发售
		case TICKET_SJT_ISSUE:
			msgCode = TXT_TICKET_SJT_ISSUE;	
			break;
		// 注销
		case TICKET_WRITE_OFF:
			msgCode = TXT_TICKET_WRITE_OFF;	
			break;
		// 重编码
		case TICKET_REENCODE:
			msgCode = TXT_TICKET_REENCODE;	
			break;
		// 更新（为了进闸）
		case TICKET_UPDATE_FOR_IN:
			msgCode = TXT_TICKET_UPDATE_FOR_IN;	
			break;
		// 更新（为了出闸）
		case TICKET_UPDATE_FOR_OUT:
			msgCode = TXT_TICKET_UPDATE_FOR_OUT;	
			break;
		// 其它更新
		case TICKET_UPDATE_FOR_OTHER:
			msgCode = TXT_TICKET_UPDATE_FOR_OTHER;	
			break;
		// 进闸
		case TICKET_IN:
			msgCode = TXT_TICKET_IN;	
			break;
		// 出闸
		case TICKET_OUT:
			msgCode = TXT_TICKET_OUT;	
			break;
		// 在E/S预赋值
		case TICKET_ES_ASSIGNMENT:
			msgCode = TXT_TICKET_ES_ASSIGNMENT;	
			break;
		// 预赋值清零
		case TICKET_ASSIGNMENT_ZERO:
			msgCode = TXT_TICKET_ASSIGNMENT_ZERO;	
			break;
		// 出站票（为了出闸）
		case TICKET_ET_FOR_EXIST:
			msgCode = TXT_TICKET_ET_FOR_EXIST;	
			break;
		// 列车故障时出闸
		case TICKET_BEFOREOUT_IN:
			msgCode = TXT_TICKET_BEFOREOUT_IN;	
			break;
			// 一卡通残盲卡进站更新
		case YKT_DISABLED_UPDATE_FOR_IN:
			msgCode = TXT_YKT_DISABLED_UPDATE_FOR_IN;	
			break;
			// 一卡通残盲卡出站更新
		case YKT_DISABLED_UPDATE_FOR_OUT:
			msgCode = TXT_YKT_DISABLED_UPDATE_FOR_OUT;	
			break;
			// 一卡通残盲卡进闸
		case YKT_DISABLED_IN:
			msgCode = TXT_YKT_DISABLED_IN;	
			break;
			// 一卡通残盲卡出闸
		case YKT_DISABLED_OUT:
			msgCode = TXT_YKT_DISABLED_OUT;	
			break;
			// 退卡
		case REFOUND_TIKCET:
			msgCode = TXT_REFOUND_TIKCET;	
			break;
		default:
			msgCode = TXT_UNDEFINE;	
	}
	// 取得指定语言的文言	
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
@brief      获取证件类型名称：如身份證（大陸）

@param      (i)CERTIFICATE_TYPE certifateType 证件类型
@param      (i) LANGUAGETYPE_T language 语言种类

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCertificateTypeName(CERTIFICATE_TYPE certificateType, LANGUAGETYPE_T language)
{
	// 取得票卡状态编码对应的文言代码
	LANGUAGE_MSG msgCode = TXT_UNDEFINE;
	switch (certificateType)
	{
	// 身份证（大陆）
	case CERTIFICATE_ID_CARD_MAINLAND:
		msgCode = TXT_ID_CARD_MAINLAND;
		break;
	// 身份证（澳门）
	case CERTIFICATE_ID_CARD_MACAO:
		msgCode = TXT_ID_CARD_MACAO;
		break;
	// 身份证（香港）
	case CERTIFICATE_ID_CARD_HONGKONG:
		msgCode = TXT_ID_CARD_HONGKONG;
		break;
	default:
		return _tl(TXT_UNDEFINE);
	}
	// 取得指定语言的文言	
	return theLANGUAGE_INFO.GetLanguageInfo(msgCode,language);	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得交易类型

@param      (i)TRANSACTION_TYPE TransType 交易类型

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetTransactionType(TRANSACTION_TYPE TransType,BYTE media_type, ISSUANCER_CODE issue_Code)
{
	CString msg_code(_T(""));

	// M1K卡
	if ((MIFARE1K == media_type||MIFARE4K == media_type)&&issue_Code ==  ACC)
	{
		switch (TransType)
		{
		case ACC_M1K_TRANS_TYPE_ISSUE_TICKET:
			msg_code = _T("售票");
			break;
		case ACC_M1K_TRANS_TYPE_ISSUE_EXIT:
			msg_code = _T("发售出站票");
			break;
		case ACC_M1K_TRANS_TYPE_ISSUE_LOVE:
			msg_code = _T("发售爱心票");
			break;
		case ACC_M1K_TRANS_TYPE_ISSUE_BAGGAGE:
			msg_code = _T("发售行李票");
			break;
		case ACC_M1K_TRANS_TYPE_INITIALIZATION:
			msg_code = _T("初始化");
			break;
		case ACC_M1K_TRANS_TYPE_PREEASSIGNMENT:
			msg_code = _T("预付值");
			break;
		case ACC_M1K_TRANS_TYPE_CASH_RECHARGE:
			msg_code = _T("现金充值");
			break;
		case ACC_M1K_TRANS_TYPE_ADJUST_TICKET:
			msg_code = _T("补票");
			break;
		case ACC_M1K_TRANS_TYPE_SUBWAY_CONSUME_START:
			msg_code = _T("地铁消费起始");
			break;
		case ACC_M1K_TRANS_TYPE_SUBWAY_CONSUME_END:
			msg_code = _T("地铁消费结束");
			break;
		case ACC_M1K_TRANS_TYPE_SVT_BUY_TICKET:
			msg_code = _T("储值卡购票");
			break;
		case ACC_M1K_TRANS_TYPE_FOLLOW_ENTER:
			msg_code = _T("跟踪进站");
			break;
		case ACC_M1K_TRANS_TYPE_FOLLOW_EXIT:
			msg_code = _T("跟踪出站");
			break;
		case ACC_M1K_TRANS_TYPE_CANCELLATION:
			msg_code = _T("注销");
			break;
		case ACC_M1K_TRANS_TYPE_COUNTERACT:
			msg_code = _T("抵消");
			break;
		case ACC_M1K_TRANS_TYPE_INSTANT_REFUND_CARD:
			msg_code = _T("即时退卡");
			break;
		case ACC_M1K_TRANS_TYPE_LOCK:
			msg_code = _T("锁卡");
			break;
		case ACC_M1K_TRANS_TYPE_UNLOCK:
			msg_code = _T("解锁");
			break;
		case ACC_M1K_TRANS_TYPE_REVERSE:
			msg_code = _T("冲正");
			break;
		case ACC_M1K_TRANS_TYPE_OVERTIME:
			msg_code = _T("滞留超时处理");
			break;
		case ACC_M1K_TRANS_TYPE_UNEXIT:
			msg_code = _T("无出站处理");
			break;
		case ACC_M1K_TRANS_TYPE_UNENTER:
			msg_code = _T("地铁无入站处理");
			break;
		case ACC_M1K_TRANS_TYPE_EXTENSION_VALIDITY:
			msg_code = _T("有效期更改");
			break;
		case ACC_M1K_TRANS_TYPE_ADMINISTRATIVE_FINE:
			msg_code = _T("行政罚款");
			break;
		case ACC_M1K_TRANS_TYPE_ADMINISTRATIVE_PAY:
			msg_code = _T("行政支付");
			break;
		case ACC_M1K_TRANS_TYPE_ACTIVATION:
			msg_code = _T("激活");
			break;
		case ACC_M1K_TRANS_TYPE_ISSUE_GROUP:
			msg_code = _T("售团体票");
			break;
		}
	}
	// CPU卡
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
@brief      取得补票类型

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
	//	return theLANGUAGE_INFO.GetLanguageInfo((GUIDE_ADJUST_ISSUE_OUT),language); // 购买出站票??
	//case PAIDAREA_OVERRIDE:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_RIDE),language);// 超程补票  
	//case PAIDAREA_OVERTIME:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_TIME),language);// 超时补票 
	//case PAIDAREA_DELAY_ENTRY:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_ENTRY_DALY),language);// 补延迟进站 
	//case PAIDAREA_EXIT_THEN_ENTRY:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_EXIT_THEN_ENTRY),language);// 先补出站后补进站 
	//case PAIDAREA_OVERTIMERIDE:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_RIDE_TIME),language);// 超程补票 超时补票 
	//case PAIDAREA_OVERRIDE_AGAIN:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_RIDE_AGAIN),language);// 补票后超程 
	//case PAIDAREA_OVERTIME_AGAIN:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_TIME_AGAIN),language);// 补票后超时 
	//case PAIDAREA_OVERTIMERIDE_AGAIN:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_OVER_RIDE_TIME_AGAIN),language);// 补票后超程 补票后超时 
	//case UNPAIDAREA_DELAY_EXIT:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_EXIT_DALY),language);// 补延迟出站 
	//case UNPAIDAREA_FREE_ENTRY:
	//	return theLANGUAGE_INFO.GetLanguageInfo((TXT_ADJUST_FREE),language);// 免费进站（走紧急门）
	//case UNPAIDAREA_BUY_NEW_CARD:
	//	return theLANGUAGE_INFO.GetLanguageInfo((GUIDE_ADJUST_ISSUE_NEW),language);// 购买新票
	//case UNKNOE:
	default:
		return _tl(TXT_UNDEFINE);
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      取得补票类型

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
@brief      取得产品种类

@param      ProductCategory_t category 产品种类

@retval     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetCategoryName(ProductCategory_t category)
{
	switch(category)
	{
	case UNDEFINE:
		return _tl(TXT_UNDEFINED);
		// 钱包
	case PURSE:
		return _tl(TXT_PURES);
		// 计次
	case MULTIRIDE:
		return _tl(TXT_MULTIRDE);
		// 定期
	case PERIOD:
		return _tl(TXT_PERIOD);
	default:
		return _tl(TXT_UNDEFINED);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得支付方式信息

@param      (i)WORD serviceType, 业务ID
@param      (i)WORD cardPHType   卡物理类型

@retval     map<WORD,CString>  所有可用支付方式ID及名称

@exception  无
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
@brief      获取个性化业务类型名称

@param      (i)PERSONAL_BUSINESS_TYPE businessType 个性化业务类型

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CTicketHelper::GetPersonalizeTypeName(PERSONALIZE_TYPE businessType, LANGUAGETYPE_T language)
{
	LANGUAGE_MSG msgCode = TXT_UNDEFINE;
	switch (businessType)
	{
		// 个性化
	//case TYPE_PERSONAL:
	//	msgCode = TXT_BUSINESS_PERSONAL_TYPE;
	//	break;
	//	// 个性化更新
	//case TYPE_PERSONAL_UPDATE:
	//	msgCode = TXT_BUSINESS_PERSONAL_UPDATE_TYPE;
	//	break;
	default:
		return _tl(TXT_UNDEFINE);
	}
	// 取得指定语言的文言	
	return theLANGUAGE_INFO.GetLanguageInfo(msgCode,language);	
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      获取个性化业务类型名称

@param      (i)PERSONAL_BUSINESS_TYPE businessType 个性化业务类型

@retval     CString

@exception  无
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
@brief      获取有效期间显示文言

@param      (i)WORD passengerGender 

@retval     CString

@exception  无
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
@brief      获取隐藏后的证件编号

@param      (i)CERTIFICATE_TYPE certifateType 证件类型
@param      (i)CString certificateNumber 证件编号

@retval     CString

@exception  无
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
@brief   获取销售票价   

@param		short productType 产品类型
@param		short productVariantType 产品子类型
@param		StationNumber_t fromStation 起始车站
@param		StationNumber_t toStation 结束车站

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
@brief   获取消费票价   

@param		short productType 产品类型
@param		short productVariantType 产品子类型
@param		StationNumber_t fromStation 起始车站
@param		StationNumber_t toStation 结束车站

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