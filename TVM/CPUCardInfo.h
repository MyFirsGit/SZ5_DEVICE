#pragma once
#include "StdAfx.h"
/**
@brief CPUƱ�����ݴ洢��                            
*/
//class CCPUCard
//{
//public:
//	typedef enum _cpucard_test_status
//	{
//		FORMAL_CARD = 0x00,					//��ʽ��
//		INVALID_CARD = 0x01,					//��Ч��
//		TEST_CARD = 0x02,							//���Կ�
//	} CPUCARD_TEST_STATUS;
//	typedef enum _cpucard_journey_status
//	{
//		UNENTER_STATION =  0X00,		//δ��վ
//		//ENTERED_STATION = 0X01,			//�ѽ�վ
//		//EXITED_STATION = 0X00,				//�ѳ�վ
//	} CPUCARD_JOURNEY_STATUS;
//	typedef enum _cpucard_enable_status
//	{
//		DISABLED = 0x00,							  //δ����
//	}CPUCARD_ENABLE_STATUS;
//	typedef enum _cpucard_ticket_status
//	{
//		UNISSUE = 0x00,								//δ��
//		PRECHARGED = 0x01,					//Ԥ��ֵ
//		ISSUED = 0x03,									//����
//		RECYCLED =  0x07,						//�ѻ���
//		BLACK_LOCKED = 0x09,			//������������
//	}CPUCARD_TICKET_STATUS;
//	typedef enum _cpucard_lock_status
//	{
//		UNLOCKED = 0x00,								//δ����
//		LOCKED = 0x01,										//������
//	} CPUCARD_LOCK_STATUS;
//	typedef enum _cpucard_active_status
//	{
//		ACTIVED = 0x00,										//�Ѽ���
//		UNACTIVED  = 0x01,							//δ����
//	} CPUCARD_ACTIVE_STATUS;
//	typedef enum _cpucard_exchange_status
//	{
//		LOCAL_CARD = 0x00,							//���ؿ�
//		OTHER_PLACE_CARD = 0x01,							//��ؿ�
//	}CPUCARD_EXCHANGE_STATUS;
//	CCPUCard(void);
//	~CCPUCard(void);
//	bool GetCardInfo(WORD,BYTE*);
//	bool GetHistoryInfo(int count,BYTE* historyInfo);
//
//private:
//	// Ʊ��������Ϣ�ṹ��
//	typedef struct _str_card_application_data {
//		WORD			PublisherCode;				// ����������
//		WORD			CityCode;					// ���д���
//		WORD			IndustryCode;				// ��ҵ����
//		BYTE			CardPhysicalType;			// Ʊ����������
//		BYTE			CardPhysicalNumber[8];		// Ʊ��������
//		DWORD			CardLogicalNumber;			// Ʊ���߼����
//		BYTE			CardAppMainType;			// ��Ӧ��������
//		BYTE			CardAppSubType;				// ��Ӧ��������
//		BYTE			TestCard;					// ����Ʊ��־
//		BYTE			CardEnabled;				// ���ñ�־
//		BYTE			TicketStatus;				// ��Ʊ״̬
//		BYTE			CardLocked;					// ������־
//		BYTE			CardActived;				// �����־
//		BYTE			DeviceID[6];				// �����豸��Ϣ
//		_DATE			PublishDate;				// ��������
//		_DATE			EnabledDate;				// ��������
//		_DATE			ValidDate;					// ��Ч����
//		WORD			AppVersion;					// Ӧ�ð汾��
//		WORD			ExchangeCard;				// ��ͨ����ʶ
//		DWORD			Deposit;					// Ѻ��
//		DWORD			Cost;						// �ɱ�
//		DWORD           dwOverdraftValue;			// ͸֧���
//		DWORD			Balance;					// Ʊ�����
//	}STR_CARD_APPLICATION_DATA;
//
//	// �ֿ�����Ϣ�ṹ��
//	typedef struct _str_card_personal_data {
//		BYTE			CardHoldType;				// �ֿ������ͱ�ʶ
//		BYTE			StaffStatus;				// �ֿ���ְ����ʶ
//		CString			Name;						// �ֿ�������
//		BYTE			Sex;						// �ֿ����Ա�
//		BYTE			CredentialsType;			// �ֿ���֤������
//		CString			CredentialsNumber;			// �ֿ���֤������
//	}STR_CARD_PERSONAL_DATA;
//
//	// �ó���Ϣ�ṹ��
//	typedef struct _str_card_journey_data {
//		BYTE			JourneyStatus;					// �ó�״̬
//		_DATE_TIME		EnterTime;						// ��վʱ��
//		WORD			EnterStation;					// ��վվ��
//		BYTE			EnterDeviceID[6];				// ��վ�豸 
//		_DATE_TIME		ExitTime;						// ��վʱ��
//		WORD			ExitStation;					// ��վվ��
//		BYTE			ExitDeviceID[6];				// ��վ�豸
//	}STR_CARD_JOURNEY_DATA;
//
//	// ������Ϣ�ṹ��
//	typedef struct _str_card_sale_data {
//		WORD			SaleStation;					// ���۳�վ
//		WORD			StarSection;					// �������
//		WORD			EndSection;						// �����յ�
//		DWORD			SalePrice;						// ��Ʊ��� 
//		_DATE_TIME		SaleTime;						// ��Ʊʱ��
//	}STR_CARD_SALE_DATA;
//
//public:
//	// ��ʷ��Ϣ�ṹ��
//	typedef struct _str_card_history_data {
//		WORD			TransNumber;				// ����Ǯ�����ѡ���ֵ�������	2HEX
//		DWORD			TransValue;					// ���׽��						4HEX
//		BYTE			TransType;					// �������ͱ�ʶ					1BCD
//		WORD			TransStation;				// ���׳�վ
//		_DATE_TIME		TransTime;					// ����������ʱ��				7BCD	YYYYMMDD HHmmSS
//		_str_card_history_data(){
//			TransNumber = 0;
//			TransValue = 0;
//			TransType = 0;
//		}
//	}STR_CARD_HISTORY_DATA;
//
//	STR_CARD_APPLICATION_DATA		applicationData;		// Ӧ������
//	STR_CARD_PERSONAL_DATA			personalData;			// ��������
//	STR_CARD_JOURNEY_DATA			journeyData;			// �ó�����
//	STR_CARD_SALE_DATA				saleData;				// ��������
//	vector<STR_CARD_HISTORY_DATA>	historyData;			// ��ʷ��¼
//
//private:
//	void Initialize();
//};
