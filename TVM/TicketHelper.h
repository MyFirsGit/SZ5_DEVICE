#pragma once
#include "CommonDefine.h"

#define  theTICKET_HELPER CTicketHelper::GetInstance()

class CTicketHelper
{
public:
	static CTicketHelper&  GetInstance(){
		return theInstance;
	}

	ProductCategory_t GetProductCategory(WORD& ticketType);		// 取得发售票卡的产品种类
	CardType_t GetCardType(WORD ticketType);					// 取得发售票卡的种类
	CARD_MEDIUM_TYPE GetMediumType(CardType_t cardType);		// 取得发售票卡的物理类型
	ISSUANCER_CODE GetCardIssuancer(WORD ticketType);

	CString GetACCCardTestStatusName(BYTE testStatus);			// 取得ACC卡测试标志名称
	//CString GetACCULCardBlockStatusName(CardStatusCode_t_UL ulticketStatus/*,LANGUAGETYPE_T language */);       // 取得ACC UL票卡状态名称
	CString GetACCM1KCardBlockStatusName(CardStatusCode_t_M1 m1kticketStatus, LANGUAGETYPE_T languageTypeId);// 取得ACC M1K票卡状态名称
	CString GetAccM1KCardProductStatusName(ProductStatus_t_M1 m1kProductStatus/*, LANGUAGETYPE_T language*/);       // 取ACC M1k票卡产品状态名称
	CString GetACCCardActiveStatusName(ACC_CARD_ACTIVE_STATUS activeStatus);									// 取得ACC卡激活状态名称
	CString GetACCCardJourneyStatusName(ACC_CARD_JOURNEY_STATUS journeyStatus,LANGUAGETYPE_T language);// 取得ACC卡旅程状态名称
	CString GetTransactionType(TRANSACTION_TYPE TransType,BYTE media_type, ISSUANCER_CODE issue_Code);														// 取得交易类型
	CString GetPersonalizeTypeName(PERSONALIZE_TYPE businessType, LANGUAGETYPE_T language);						// 获取个性化类型名称
	CString GetCertificateTypeName(CERTIFICATE_TYPE certificateType, LANGUAGETYPE_T language);					// 获取证件类型名称：如身份證（大陸）
	CString GetAdjustTypeName(BYTE adjustType,LANGUAGETYPE_T language = LANGUAGE_ID_UNSPECIFIED);							// 取得补票类型
	CString GetAdjustPayTypeName(BYTE adjustPayType ,LANGUAGETYPE_T language = LANGUAGE_ID_UNSPECIFIED);		            // 取得补票支付类型
	CString GetCategoryName(ProductCategory_t categor);															// 产品种类
	CString GetPassengerGender(Sex_t passengerGender);														// 个性化名称
	map<WORD,CString> GetAvailablePayTypeList(WORD serviceType,CardType_t cardPHType);								// 取得支付方式信息

	CString GetDurationText(WORD ValidityPeriod);																// 获取有效期间显示文言

	CString GetHideCertificateID(CERTIFICATE_TYPE certificateType, CString certificateID);						// 获取隐藏后的证件编号
	//获取销售票价
	Fare_t GetSalePrice(short productType, short productVariantType,PassengerType_t passengerType,  StationNumber_t fromStation,StationNumber_t toStation);
	//获取消费标价
	Fare_t GetPursePrice(short productType, _DATE_TIME dateTime,PassengerType_t passengerType,  StationNumber_t fromStation,StationNumber_t toStation);

	void UpdateTHCount(BYTE boxACount,BYTE boxBCount,BYTE collectionCount);
private:

	// 构造,析构函数
	CTicketHelper(void);
	~CTicketHelper();
	CTicketHelper(const CTicketHelper&);
	CTicketHelper& operator=(const CTicketHelper&);

	map<WORD,CString> s_AvailablePayTypeList;

	static  CTicketHelper theInstance;   // 该类的唯一接口
};