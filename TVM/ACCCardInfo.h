#pragma once
#include "StdAfx.h"
/**
@brief ACCƱ�����ݴ洢��                            
*/
#define CARD_PH_ID_LEN 7
#define CARD_CERTIFICATE_ID_LEN 9

class CACCCard
{
public:
	// ��Ʒ��Ϣ�ṹ��
	typedef struct _acccard_product_info
	{
		ProductCategory_t productCategory;		// ��Ʒ����
		WORD productType;						// ��Ʒ����
		WORD productSubType;					// ��Ʒ������
		BYTE productIssuerID;					// ��Ʒ������ID
		BYTE byProductStatus;					// ��Ʒ״̬  ��Ƭ״̬. 1��δ������2������ԭ��������3��Ʊ�����ջ��˿�������5������������6��δ���п�������11������������12������������13��������������������14������δʹ��������15��δ�趨��
		ACC_CARD_ACTIVE_STATUS CardActived;		// ��Ʒ�����־ ��Ʒ�Ƿ񼤻0��δ���1���Ѽ���
		_DATE_TIME ValidStartTime;				// ��Ʒ��Ч�ڿ�ʼʱ��, 7B, BCD
		_DATE_TIME ValidEndTime;				// ��Ʒ��Ч�ڽ���ʱ�� , 7B, BCD
		DWORD dwStartStation;					// ��Ʒ��Ч�˳����, ��վλ����Ϣ
		DWORD dwEndStation;						// ��Ʒ��Ч�˳��յ�, ��վλ����Ϣ
		WORD wArea;								// ��Ʒ��Чʹ������
		DWORD Balance;							// ��Ʒ����Ϊ����Ʊ������ֶ������塣
		short sSamID;							// ���ڱ�����ϴζԲ�Ʒ����/��ֵ��SAM ID
		DWORD nRemainRides;						// ��Σ���ǰ����Ӧ���˿�����Σ�
		_acccard_product_info()
		{
			productCategory = UNDEFINE;
			productType = UNKNOWPRODUCT;
			productSubType = 0;
			productIssuerID = 0;                                                                                                         
			byProductStatus = 0;
			CardActived = UNACTIVED;
			dwStartStation = 0;
			dwEndStation = 0;
			Balance = 0;
			sSamID = 0;
			nRemainRides = 0;
		}
	} ACCCARD_PRODUCT_INFO;

	//������Ϣ�ṹ��
	typedef struct _acc_Card_personalize_info
	{
		_DATE birthday;                                        // ����
		BYTE certificateCardNumber[CARD_CERTIFICATE_ID_LEN];   // ֤����
		BYTE certificateCardType;                              // ֤������
		TCHAR name[20];                                        // ����
		WORD companyID;                                        // ��˾
		ClassificationLevel_t userStatus;                      // �˿ͼ��� 0:��ͨ 1:��� 2:-7:����0:��ͨ 1:��� 2:-7:����	
		_acc_Card_personalize_info()
		{
			birthday;
			memset(certificateCardNumber,0,CARD_CERTIFICATE_ID_LEN);
			certificateCardType;
			memset(name,0,20);
			companyID = 0;
			userStatus = CLASSIFICATION_LEVEL_UNSPECIFIED;
		}
	} ACC_CARD_PERSONALIZE_INFO;

	// �ó���Ϣ�ṹ��
	typedef struct _card_journey_data {
		_DATE_TIME travelStartTime;				// �����ó̿�ʼʱ��, 7B, BCD
		DWORD dwTravelStartStation;				// �����ó̵���ʼվ��.�����ó̵���ʼվ��ĳ�վλ����Ϣ
		int nPaid;								// �Ѹ�����
		_DATE_TIME lastTransTime;				// �ϴν���ʱ��
		DWORD dwLastTransStation;				// �ϴν��׳�վ�ĳ�վλ����Ϣ
		ACC_CARD_JOURNEY_STATUS byTravelStatus;	// �ϴν����ó���״̬
		_card_journey_data()
		{
			dwTravelStartStation = 0;
			nPaid = 0;
			dwLastTransStation = 0;
			byTravelStatus = TICKET_INIT;
		}
	}CARD_JOURNEY_DATA;

	// ��ʷ��Ϣ�ṹ��
	typedef struct _card_history_data {
		_DATE_TIME TransTime;			// ����ʱ��
		BYTE serviceProviderId;			// �����ṩ��
		DWORD TransOperator;			// ����Ա
		BYTE productIssuerId;			// ������
		BYTE category;					// ��Ʒ����  0:�� 1:Ǯ�� 2:�������� 3:�ƴ����� 4-7:Ԥ��
		BYTE productType;				// ��Ʒ����
		BYTE TransType;					// ��������  02 ��ʾ��ֵ��06 ��ʾ���ѣ�09 ��ʾ��������
		WORD paymentMethod;				// ���׷�ʽ
		BYTE transStation[6];				// ���׵ص�
		DWORD TransValue;				// ���׽��
		DWORD remaingValue;				// ���׺���
		_card_history_data(){
			serviceProviderId = 0;
			TransOperator = 0;
			productIssuerId = 0;
			category = 0;
			productType = 0;
			TransType = 0;
			paymentMethod = 0;
			memset(transStation,0,6);
			TransValue = 0;
			remaingValue = 0;
		}
	}CARD_HISTORY_DATA;

	// Ʊ��������Ϣ�ṹ��
	typedef struct _str_card_application_data {
		short sCityCode;								// ���д���,���д��룬UL����Ĭ��ֵ0x00, BCD
		short sTradeCode;								// ��ҵ����.��ҵ���룬UL����Ĭ��ֵ0x00, BCD
		BYTE  CardPhysicalNumber[CARD_PH_ID_LEN];		// ��Ʊ�����ţ�����7�ֽ�ǰ��0
		//TicketLogicalID_t CardLogicalNumber;			// ��Ʊ�߼����
		BYTE CardLogicalNumber[8];						// ��Ʊ�߼����
		short sLifeCycleCount;							// ���ط��м���
		ACC_CARD_TEST_STATUS  testFlag;					// ��Ƭ���Ա�־.0x00����ʽ��Ʊ, 0x01�������Ի���ѵʹ��
		BYTE  CardIssuerCode;							// �������̴���
		_DATE initialDate;								// ��ʼ������, 4Bytes, BCD
		short sInitialNum;								// ��ʼ������
		BYTE  byPassengerType;							// �˿�����:���ˣ���ͯ��
		DWORD  Deposit;									// Ѻ��
		BYTE  TicketStatus;								// M1��Ƭ״̬. 1��δ������2������ԭ��������3��Ʊ�����ջ��˿�������5������������6��δ���п�������11������������12������������13��������������������14������δʹ��������15��δ�趨��
		// UL��Ƭ״̬. 0:���� 1:û������ 2:���� 3:����-����/���տ�
		ProductCategory_t  byLastUsedProductCategory;	// ���ʹ�õĲ�Ʒ����.0x01 = Ǯ����0x02 = ���ڣ�0x03 = �ƴ�
		WORD byLastUsedProductType;						// ���ʹ�õĲ�Ʒ����
		BYTE  byLastUsedProductSubType;					// ���ʹ�õĲ�Ʒ������
		BYTE  byProductsNum;							// ��Ʒ����
		BYTE  byTransNum;								// �ѻ��˴���		
		BYTE  byPersonalizeStatus;						// ���Ի���ʶ
		_str_card_application_data(){
			sCityCode = 0;
			sTradeCode = 0;
			memset(CardPhysicalNumber,0,CARD_PH_ID_LEN);
			memset(CardLogicalNumber, 0, sizeof(CardLogicalNumber));
			sLifeCycleCount = 0;
			testFlag = TEST_CARD;
			sInitialNum = 0;
			byPassengerType = 0;
			Deposit = 0;
			TicketStatus = 0;
			byLastUsedProductCategory = UNDEFINE;
			byLastUsedProductType = UNKNOWPRODUCT;
			byLastUsedProductSubType= 0;
			byTransNum = 0;                                                                                                                                                                             
		}
	}CARD_APPLICATION_DATA;

	CACCCard(void);
	~CACCCard(void);
	bool GetCardInfo(BYTE*);
	bool GetHistoryInfo(int count,BYTE* historyInfo);
	bool GetProductInfo(WORD productType, WORD productSubType, ACCCARD_PRODUCT_INFO& productInfo);	// ��ȡָ���Ŀ��ڲ�Ʒ��Ϣ

	CARD_APPLICATION_DATA		applicationData;	// Ӧ������
	CARD_JOURNEY_DATA			journeyData;		// �ó�����
	CARD_HISTORY_DATA			rechargeData;		// ��ֵ��¼
	vector<CARD_HISTORY_DATA>   historyData;		// ��ʷ��¼
	ACC_CARD_PERSONALIZE_INFO   userInfo;			// ������Ϣ
	vector<ACCCARD_PRODUCT_INFO> v_ProductInfo;		// ���ڲ�Ʒ��Ϣ

private:
	void Initialize();
};