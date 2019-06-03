#pragma once
#include "StdAfx.h"
/**
@brief CPU票卡数据存储类                            
*/
//class CCPUCard
//{
//public:
//	typedef enum _cpucard_test_status
//	{
//		FORMAL_CARD = 0x00,					//正式卡
//		INVALID_CARD = 0x01,					//无效卡
//		TEST_CARD = 0x02,							//测试卡
//	} CPUCARD_TEST_STATUS;
//	typedef enum _cpucard_journey_status
//	{
//		UNENTER_STATION =  0X00,		//未进站
//		//ENTERED_STATION = 0X01,			//已进站
//		//EXITED_STATION = 0X00,				//已出站
//	} CPUCARD_JOURNEY_STATUS;
//	typedef enum _cpucard_enable_status
//	{
//		DISABLED = 0x00,							  //未启用
//	}CPUCARD_ENABLE_STATUS;
//	typedef enum _cpucard_ticket_status
//	{
//		UNISSUE = 0x00,								//未售
//		PRECHARGED = 0x01,					//预充值
//		ISSUED = 0x03,									//已售
//		RECYCLED =  0x07,						//已回收
//		BLACK_LOCKED = 0x09,			//黑名单卡锁定
//	}CPUCARD_TICKET_STATUS;
//	typedef enum _cpucard_lock_status
//	{
//		UNLOCKED = 0x00,								//未锁定
//		LOCKED = 0x01,										//已锁定
//	} CPUCARD_LOCK_STATUS;
//	typedef enum _cpucard_active_status
//	{
//		ACTIVED = 0x00,										//已激活
//		UNACTIVED  = 0x01,							//未激活
//	} CPUCARD_ACTIVE_STATUS;
//	typedef enum _cpucard_exchange_status
//	{
//		LOCAL_CARD = 0x00,							//本地卡
//		OTHER_PLACE_CARD = 0x01,							//异地卡
//	}CPUCARD_EXCHANGE_STATUS;
//	CCPUCard(void);
//	~CCPUCard(void);
//	bool GetCardInfo(WORD,BYTE*);
//	bool GetHistoryInfo(int count,BYTE* historyInfo);
//
//private:
//	// 票卡基本信息结构体
//	typedef struct _str_card_application_data {
//		WORD			PublisherCode;				// 发卡方代码
//		WORD			CityCode;					// 城市代码
//		WORD			IndustryCode;				// 行业代码
//		BYTE			CardPhysicalType;			// 票卡物理类型
//		BYTE			CardPhysicalNumber[8];		// 票卡物理编号
//		DWORD			CardLogicalNumber;			// 票卡逻辑编号
//		BYTE			CardAppMainType;			// 卡应用主类型
//		BYTE			CardAppSubType;				// 卡应用子类型
//		BYTE			TestCard;					// 测试票标志
//		BYTE			CardEnabled;				// 启用标志
//		BYTE			TicketStatus;				// 车票状态
//		BYTE			CardLocked;					// 锁卡标志
//		BYTE			CardActived;				// 激活标志
//		BYTE			DeviceID[6];				// 发行设备信息
//		_DATE			PublishDate;				// 发行日期
//		_DATE			EnabledDate;				// 启用日期
//		_DATE			ValidDate;					// 有效日期
//		WORD			AppVersion;					// 应用版本号
//		WORD			ExchangeCard;				// 互通卡标识
//		DWORD			Deposit;					// 押金
//		DWORD			Cost;						// 成本
//		DWORD           dwOverdraftValue;			// 透支金额
//		DWORD			Balance;					// 票内余额
//	}STR_CARD_APPLICATION_DATA;
//
//	// 持卡人信息结构体
//	typedef struct _str_card_personal_data {
//		BYTE			CardHoldType;				// 持卡人类型标识
//		BYTE			StaffStatus;				// 持卡人职工标识
//		CString			Name;						// 持卡人姓名
//		BYTE			Sex;						// 持卡人性别
//		BYTE			CredentialsType;			// 持卡人证件类型
//		CString			CredentialsNumber;			// 持卡人证件号码
//	}STR_CARD_PERSONAL_DATA;
//
//	// 旅程信息结构体
//	typedef struct _str_card_journey_data {
//		BYTE			JourneyStatus;					// 旅程状态
//		_DATE_TIME		EnterTime;						// 进站时间
//		WORD			EnterStation;					// 进站站点
//		BYTE			EnterDeviceID[6];				// 进站设备 
//		_DATE_TIME		ExitTime;						// 出站时间
//		WORD			ExitStation;					// 出站站点
//		BYTE			ExitDeviceID[6];				// 出站设备
//	}STR_CARD_JOURNEY_DATA;
//
//	// 发售信息结构体
//	typedef struct _str_card_sale_data {
//		WORD			SaleStation;					// 发售车站
//		WORD			StarSection;					// 区段起点
//		WORD			EndSection;						// 区段终点
//		DWORD			SalePrice;						// 售票金额 
//		_DATE_TIME		SaleTime;						// 售票时间
//	}STR_CARD_SALE_DATA;
//
//public:
//	// 历史信息结构体
//	typedef struct _str_card_history_data {
//		WORD			TransNumber;				// 电子钱包消费、充值交易序号	2HEX
//		DWORD			TransValue;					// 交易金额						4HEX
//		BYTE			TransType;					// 交易类型标识					1BCD
//		WORD			TransStation;				// 交易车站
//		_DATE_TIME		TransTime;					// 交易日期与时间				7BCD	YYYYMMDD HHmmSS
//		_str_card_history_data(){
//			TransNumber = 0;
//			TransValue = 0;
//			TransType = 0;
//		}
//	}STR_CARD_HISTORY_DATA;
//
//	STR_CARD_APPLICATION_DATA		applicationData;		// 应用数据
//	STR_CARD_PERSONAL_DATA			personalData;			// 个人数据
//	STR_CARD_JOURNEY_DATA			journeyData;			// 旅程数据
//	STR_CARD_SALE_DATA				saleData;				// 发售数据
//	vector<STR_CARD_HISTORY_DATA>	historyData;			// 历史记录
//
//private:
//	void Initialize();
//};
