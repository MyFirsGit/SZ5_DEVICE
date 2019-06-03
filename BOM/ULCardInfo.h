#pragma once
#include "StdAfx.h"
/**
@brief UL票卡数据存储类                            
*/
class CULCard
{
public:
	typedef enum _ulcard_test_status
	{
		FORMAL_CARD = 0x00,					//正式卡
		INVALID_CARD = 0x01,					//无效卡
		TEST_CARD = 0x02,							//测试卡
	} ULCARD_TEST_STATUS;
	typedef enum _ulcard_ticket_status
	{
		UNISSUE = 0x00,								//未发售
		ISSUED = 0x03,									//已发售
		RECYCLED = 0x07,							//已回收
	}ULCARD_TICKET_STATUS;
	typedef enum _ulcard_active_status
	{
		ACTIVED = 0x00,							//已激活
		UNACTIVED  = 0x01,					//未激活
	}ULCARD_ACTIVE_STATUS;
	typedef enum _ulcard_journey_status
	{
		UNENTER_STATION =  0X00,		//未进站
		ENTERED_STATION = 0X01,			//已进站
		EXITED_STATION = 0X02,				//已出站
	} ULCARD_JOURNEY_STATUS;

	// 票卡基本信息结构体
	typedef struct _str_card_application_data {
		WORD			PublisherCode;				// 发卡方代码
		BYTE			CardPhysicalType;			// 票卡物理类型
		BYTE			CardPhysicalNumber[8];		// 票卡物理编号
		DWORD			CardLogicalNumber;			// 票卡逻辑编号
		BYTE			CardAppMainType;			// 卡应用主类型
		BYTE			CardAppSubType;				// 卡应用子类型(暂不使用)
		BYTE			TestCard;					// 测试票标识
		WORD			BatchNumber;				// 票卡批次号
		_DATE			PublishDate;				// 发行日期
		_DATE_TIME		ValidStartTime;				// 有效开始时间
		_DATE_TIME		ValidEndTime;				// 有效结束时间
		BYTE			AppVersion;					// 应用版本号
		BYTE			TicketStatus;				// 车票状态
		BYTE			CardActived;				// 激活标志
		WORD			SaleStation;				// 发售车站
		_DATE_TIME		SaleTime;					// 发售时间
		DWORD			Deposit;					// 押金
		DWORD			Cost;						// 成本
		DWORD			SalePrice;					// 售票金额
		DWORD			Balance;					// 票内余额
	}STR_CARD_APPLICATION_DATA;

	// 旅程信息结构体
	typedef struct _str_card_journey_data {
		BYTE			JourneyStatus;					// 旅程状态
		_DATE_TIME		EnterTime;						// 进站时间
		WORD			EnterStation;					// 进站站点
		BYTE			EnterDeviceID[6];				// 进站设备 
		_DATE_TIME		ExitTime;						// 出站时间
		WORD			ExitStation;					// 出站站点
		BYTE			ExitDeviceID[6];				// 出站设备
	}STR_CARD_JOURNEY_DATA;

	// 历史信息结构体
	typedef struct _str_card_history_data {
		WORD			TransNumber;				// 电子钱包消费、充值交易序号	2HEX
		DWORD			TransValue;					// 交易金额						4HEX
		BYTE			TransType;					// 交易类型标识					1BCD
		DWORD			TransStation;				// 交易车站
		_DATE_TIME		TransTime;					// 交易日期与时间				7BCD	YYYYMMDD HHmmSS
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

	STR_CARD_APPLICATION_DATA		applicationData;	// 应用数据
	STR_CARD_JOURNEY_DATA			journeyData;		// 旅程数据
	vector<STR_CARD_HISTORY_DATA>   historyData;		// 历史记录
private:
	void Initialize();
};