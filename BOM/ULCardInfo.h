#pragma once
#include "StdAfx.h"
/**
@brief ULƱ�����ݴ洢��                            
*/
class CULCard
{
public:
	typedef enum _ulcard_test_status
	{
		FORMAL_CARD = 0x00,					//��ʽ��
		INVALID_CARD = 0x01,					//��Ч��
		TEST_CARD = 0x02,							//���Կ�
	} ULCARD_TEST_STATUS;
	typedef enum _ulcard_ticket_status
	{
		UNISSUE = 0x00,								//δ����
		ISSUED = 0x03,									//�ѷ���
		RECYCLED = 0x07,							//�ѻ���
	}ULCARD_TICKET_STATUS;
	typedef enum _ulcard_active_status
	{
		ACTIVED = 0x00,							//�Ѽ���
		UNACTIVED  = 0x01,					//δ����
	}ULCARD_ACTIVE_STATUS;
	typedef enum _ulcard_journey_status
	{
		UNENTER_STATION =  0X00,		//δ��վ
		ENTERED_STATION = 0X01,			//�ѽ�վ
		EXITED_STATION = 0X02,				//�ѳ�վ
	} ULCARD_JOURNEY_STATUS;

	// Ʊ��������Ϣ�ṹ��
	typedef struct _str_card_application_data {
		WORD			PublisherCode;				// ����������
		BYTE			CardPhysicalType;			// Ʊ����������
		BYTE			CardPhysicalNumber[8];		// Ʊ��������
		DWORD			CardLogicalNumber;			// Ʊ���߼����
		BYTE			CardAppMainType;			// ��Ӧ��������
		BYTE			CardAppSubType;				// ��Ӧ��������(�ݲ�ʹ��)
		BYTE			TestCard;					// ����Ʊ��ʶ
		WORD			BatchNumber;				// Ʊ�����κ�
		_DATE			PublishDate;				// ��������
		_DATE_TIME		ValidStartTime;				// ��Ч��ʼʱ��
		_DATE_TIME		ValidEndTime;				// ��Ч����ʱ��
		BYTE			AppVersion;					// Ӧ�ð汾��
		BYTE			TicketStatus;				// ��Ʊ״̬
		BYTE			CardActived;				// �����־
		WORD			SaleStation;				// ���۳�վ
		_DATE_TIME		SaleTime;					// ����ʱ��
		DWORD			Deposit;					// Ѻ��
		DWORD			Cost;						// �ɱ�
		DWORD			SalePrice;					// ��Ʊ���
		DWORD			Balance;					// Ʊ�����
	}STR_CARD_APPLICATION_DATA;

	// �ó���Ϣ�ṹ��
	typedef struct _str_card_journey_data {
		BYTE			JourneyStatus;					// �ó�״̬
		_DATE_TIME		EnterTime;						// ��վʱ��
		WORD			EnterStation;					// ��վվ��
		BYTE			EnterDeviceID[6];				// ��վ�豸 
		_DATE_TIME		ExitTime;						// ��վʱ��
		WORD			ExitStation;					// ��վվ��
		BYTE			ExitDeviceID[6];				// ��վ�豸
	}STR_CARD_JOURNEY_DATA;

	// ��ʷ��Ϣ�ṹ��
	typedef struct _str_card_history_data {
		WORD			TransNumber;				// ����Ǯ�����ѡ���ֵ�������	2HEX
		DWORD			TransValue;					// ���׽��						4HEX
		BYTE			TransType;					// �������ͱ�ʶ					1BCD
		DWORD			TransStation;				// ���׳�վ
		_DATE_TIME		TransTime;					// ����������ʱ��				7BCD	YYYYMMDD HHmmSS
		_str_card_history_data(){
			TransNumber = 0;
			TransValue = 0;
			TransType = 0;
		}
	}STR_CARD_HISTORY_DATA;

	CULCard(void);
	~CULCard(void);
	bool GetCardInfo(BYTE*);
	bool GetHistoryInfo(int count,BYTE* historyInfo);

	STR_CARD_APPLICATION_DATA		applicationData;	// Ӧ������
	STR_CARD_JOURNEY_DATA			journeyData;		// �ó�����
	vector<STR_CARD_HISTORY_DATA>   historyData;		// ��ʷ��¼
private:
	void Initialize();
};