#pragma once
#include "StdAfx.h"
/**
  @brief ECTƱ�����ݴ洢��(������ʹ��)                            
*/
class CECTCard
{
public:
    CECTCard(void);
    ~CECTCard(void);
    bool GetCardInfo(BYTE*);
	bool GetHistoryInfo(BYTE TicketPhyType,int count,BYTE* historyInfo);
private:
    // ��Ƭ������Ϣ�ṹ��
    typedef struct _str_card_application_data {
        BYTE				TicketPhyID[7];				// ������	    7HEX	��Ʊ������
        BYTE				TicketLogicalID[10];        // �߼�����	    10HEX	��Ʊ�߼����
        WORD				CityCode;					// ���д���		2BCD	
		WORD				IndustryCode;				// ��ҵ����		2BCD			
        BYTE				TicketFamily;               // ��Ʊ�����	1HEX	
        TicketType_t        TicketType;					// ��Ӧ������
		BYTE				TicketStatus;				// ���ñ�־
		WORD				Deposit;					// Ѻ��			2BCD    ��λ:��
		_DATE				InitDate;					// ��������	    4BCD	YYYYMMDD
		_DATE				ValiditDate;				// ��Ч����		4BCD	YYYYMMDD
		_DATE				StartDate;					// ��������	    4BCD	YYYYMMDD        
    }STR_CARD_APPLICATION_DATA;

	// Ǯ����Ϣ�ṹ��
	typedef struct _str_card_purse_data {
		long				Balance;					// ���/���
		DWORD               TransactionTimes;           // Ǯ���ۼƽ��״���
		_DATE			chargeDate;			//��ֵʱ��
		long				chargeAmout;		//��ֵԭ��
		int				currentChargeAmout;//���γ�ֵ
	}STR_CARD_PURSE_DATA;

	// ��ֵ��Ϣ�ṹ��
	typedef struct _str_card_charge_data {
		DWORD			ChargeTotalValue;               // �ۼƳ�ֵ�� ��λ:��
		DWORD			MaxChargeValue;					// ����������,CPUƱ�����ֶ�Ϊ0
		DWORD			ChargeDeviceID;					// ��ֵ�豸���(��·����վ���豸���͡��豸��Ÿ�1�ֽ�) 
	}STR_CARD_CHARGE_DATA;

    // �����ó����ݵĽṹ��
    typedef struct _str_card_journey_data {
		BYTE			JourneyStatus;					// �ó�״̬		41:��ʾ��վ   81:��ʾ��վ
		_DATE_TIME		EnterTime;						// ��վ����ʱ��	5BCD	YYMMDDHHMM   ��Ϊ0
		StationID_t		EnterStation;					// ��վվ��				��·��1HEX��+��վ��1HEX��
		WORD			EnterDeviceID;					// ��վ�豸             �豸���ͣ�1HEX��+ վ�ڱ�ţ�1HEX��
		WORD			EnterTransValue;				// ��վ���׽��	2HEX	��λ����		
		_DATE_TIME		ExitTime;						// ��վ����ʱ��	5BCD	YYMMDDHHMM   ��Ϊ0
		StationID_t		ExitStation;					// ��վվ��				��·��1HEX��+��վ��1HEX��
		WORD			ExitDeviceID;					// ��վ�豸				�豸���ͣ�1HEX��+ վ�ڱ�ţ�1HEX��
		WORD			ExitTransValue;					// ��վ���׽��	2HEX	��λ����
		BYTE			BackupFlag;						// ���ݱ�ʶ	1	HEX	55����ʾ�����վ�� 66����ʾ�����վ��	71����ʾ���������£�72����ʾ��ʱ����
    }STR_CARD_JOURNEY_DATA;

    // �Ż���Ϣ�ṹ��
    typedef struct _str_card_discount_data {        
		WORD				IndustryCode;				// ��ҵ��ʶ
        TicketType_t        TicketType;					// ��Ӧ������
		_DATE				StartDate;					// ��������	    4BCD	YYYYMMDD  
		_DATE				EndDate;			    	// ��������		4BCD	YYYYMMDD
	}STR_CARD_DISCOUNT_DATA;

public:
	// ��ʷ��Ϣ�ṹ��
	typedef struct _str_card_history_data {
		_DATE_TIME		TransTime;						// ����ʱ��	          	YYMMDDHHMM   ��Ϊ0
		StationID_t		TransStation;                   // ����վ��				��·��1HEX��+��վ��1HEX��
		CString         TransType;                      // ��������
		DWORD           TransValue;                     // ���׽��
		_str_card_history_data(){
			TransStation =0;
			TransType ="";
			TransValue =0;
		}
	}STR_CARD_HISTORY_DATA;

    STR_CARD_APPLICATION_DATA   cardData;				// ��Ƭ����
	STR_CARD_PURSE_DATA			purseData;				// Ǯ������
	STR_CARD_CHARGE_DATA        chargeData;				// ��ֵ����
    STR_CARD_JOURNEY_DATA       journeyData;			// �ó�����
    STR_CARD_DISCOUNT_DATA		discountData;			// �Ż���Ϣ
	vector<STR_CARD_HISTORY_DATA>      historyData;     // ��ʷ��¼

private:
	void Initialize();
};
