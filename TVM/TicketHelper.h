#pragma once
#include "CommonDefine.h"

#define  theTICKET_HELPER CTicketHelper::GetInstance()

class CTicketHelper
{
public:
	static CTicketHelper&  GetInstance(){
		return theInstance;
	}

	ProductCategory_t GetProductCategory(WORD& ticketType);		// ȡ�÷���Ʊ���Ĳ�Ʒ����
	CardType_t GetCardType(WORD ticketType);					// ȡ�÷���Ʊ��������
	CARD_MEDIUM_TYPE GetMediumType(CardType_t cardType);		// ȡ�÷���Ʊ������������
	ISSUANCER_CODE GetCardIssuancer(WORD ticketType);

	CString GetACCCardTestStatusName(BYTE testStatus);			// ȡ��ACC�����Ա�־����
	//CString GetACCULCardBlockStatusName(CardStatusCode_t_UL ulticketStatus/*,LANGUAGETYPE_T language */);       // ȡ��ACC ULƱ��״̬����
	CString GetACCM1KCardBlockStatusName(CardStatusCode_t_M1 m1kticketStatus, LANGUAGETYPE_T languageTypeId);// ȡ��ACC M1KƱ��״̬����
	CString GetAccM1KCardProductStatusName(ProductStatus_t_M1 m1kProductStatus/*, LANGUAGETYPE_T language*/);       // ȡACC M1kƱ����Ʒ״̬����
	CString GetACCCardActiveStatusName(ACC_CARD_ACTIVE_STATUS activeStatus);									// ȡ��ACC������״̬����
	CString GetACCCardJourneyStatusName(ACC_CARD_JOURNEY_STATUS journeyStatus,LANGUAGETYPE_T language);// ȡ��ACC���ó�״̬����
	CString GetTransactionType(TRANSACTION_TYPE TransType,BYTE media_type, ISSUANCER_CODE issue_Code);														// ȡ�ý�������
	CString GetPersonalizeTypeName(PERSONALIZE_TYPE businessType, LANGUAGETYPE_T language);						// ��ȡ���Ի���������
	CString GetCertificateTypeName(CERTIFICATE_TYPE certificateType, LANGUAGETYPE_T language);					// ��ȡ֤���������ƣ�������C����ꑣ�
	CString GetAdjustTypeName(BYTE adjustType,LANGUAGETYPE_T language = LANGUAGE_ID_UNSPECIFIED);							// ȡ�ò�Ʊ����
	CString GetAdjustPayTypeName(BYTE adjustPayType ,LANGUAGETYPE_T language = LANGUAGE_ID_UNSPECIFIED);		            // ȡ�ò�Ʊ֧������
	CString GetCategoryName(ProductCategory_t categor);															// ��Ʒ����
	CString GetPassengerGender(Sex_t passengerGender);														// ���Ի�����
	map<WORD,CString> GetAvailablePayTypeList(WORD serviceType,CardType_t cardPHType);								// ȡ��֧����ʽ��Ϣ

	CString GetDurationText(WORD ValidityPeriod);																// ��ȡ��Ч�ڼ���ʾ����

	CString GetHideCertificateID(CERTIFICATE_TYPE certificateType, CString certificateID);						// ��ȡ���غ��֤�����
	//��ȡ����Ʊ��
	Fare_t GetSalePrice(short productType, short productVariantType,PassengerType_t passengerType,  StationNumber_t fromStation,StationNumber_t toStation);
	//��ȡ���ѱ��
	Fare_t GetPursePrice(short productType, _DATE_TIME dateTime,PassengerType_t passengerType,  StationNumber_t fromStation,StationNumber_t toStation);

	void UpdateTHCount(BYTE boxACount,BYTE boxBCount,BYTE collectionCount);
private:

	// ����,��������
	CTicketHelper(void);
	~CTicketHelper();
	CTicketHelper(const CTicketHelper&);
	CTicketHelper& operator=(const CTicketHelper&);

	map<WORD,CString> s_AvailablePayTypeList;

	static  CTicketHelper theInstance;   // �����Ψһ�ӿ�
};