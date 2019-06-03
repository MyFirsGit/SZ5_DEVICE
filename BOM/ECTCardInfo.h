#pragma once
#include "StdAfx.h"
/**
  @brief ECT票卡数据存储类(沈阳不使用)                            
*/
class CECTCard
{
public:
    CECTCard(void);
    ~CECTCard(void);
    bool GetCardInfo(BYTE*);
	bool GetHistoryInfo(BYTE TicketPhyType,int count,BYTE* historyInfo);
private:
    // 卡片基本信息结构体
    typedef struct _str_card_application_data {
        BYTE				TicketPhyID[7];				// 物理卡号	    7HEX	车票物理卡号
        BYTE				TicketLogicalID[10];        // 逻辑卡号	    10HEX	车票逻辑编号
        WORD				CityCode;					// 城市代码		2BCD	
		WORD				IndustryCode;				// 行业代码		2BCD			
        BYTE				TicketFamily;               // 车票大分类	1HEX	
        TicketType_t        TicketType;					// 卡应用类型
		BYTE				TicketStatus;				// 启用标志
		WORD				Deposit;					// 押金			2BCD    单位:分
		_DATE				InitDate;					// 发行日期	    4BCD	YYYYMMDD
		_DATE				ValiditDate;				// 有效日期		4BCD	YYYYMMDD
		_DATE				StartDate;					// 启用日期	    4BCD	YYYYMMDD        
    }STR_CARD_APPLICATION_DATA;

	// 钱包信息结构体
	typedef struct _str_card_purse_data {
		long				Balance;					// 余额/余次
		DWORD               TransactionTimes;           // 钱包累计交易次数
		_DATE			chargeDate;			//充值时间
		long				chargeAmout;		//充值原额
		int				currentChargeAmout;//本次充值
	}STR_CARD_PURSE_DATA;

	// 充值信息结构体
	typedef struct _str_card_charge_data {
		DWORD			ChargeTotalValue;               // 累计充值额 单位:分
		DWORD			MaxChargeValue;					// 允许最大余额,CPU票卡该字段为0
		DWORD			ChargeDeviceID;					// 充值设备编号(线路、车站、设备类型、设备编号各1字节) 
	}STR_CARD_CHARGE_DATA;

    // 卡内旅程数据的结构体
    typedef struct _str_card_journey_data {
		BYTE			JourneyStatus;					// 旅程状态		41:表示进站   81:表示出站
		_DATE_TIME		EnterTime;						// 进站交易时间	5BCD	YYMMDDHHMM   秒为0
		StationID_t		EnterStation;					// 进站站点				线路（1HEX）+车站（1HEX）
		WORD			EnterDeviceID;					// 进站设备             设备类型（1HEX）+ 站内编号（1HEX）
		WORD			EnterTransValue;				// 进站交易金额	2HEX	单位：分		
		_DATE_TIME		ExitTime;						// 出站交易时间	5BCD	YYMMDDHHMM   秒为0
		StationID_t		ExitStation;					// 出站站点				线路（1HEX）+车站（1HEX）
		WORD			ExitDeviceID;					// 出站设备				设备类型（1HEX）+ 站内编号（1HEX）
		WORD			ExitTransValue;					// 出站交易金额	2HEX	单位：分
		BYTE			BackupFlag;						// 备份标识	1	HEX	55：表示允许出站； 66：表示允许进站；	71：表示付费区更新；72：表示超时更新
    }STR_CARD_JOURNEY_DATA;

    // 优惠信息结构体
    typedef struct _str_card_discount_data {        
		WORD				IndustryCode;				// 行业标识
        TicketType_t        TicketType;					// 卡应用类型
		_DATE				StartDate;					// 启用日期	    4BCD	YYYYMMDD  
		_DATE				EndDate;			    	// 结束日期		4BCD	YYYYMMDD
	}STR_CARD_DISCOUNT_DATA;

public:
	// 历史信息结构体
	typedef struct _str_card_history_data {
		_DATE_TIME		TransTime;						// 交易时间	          	YYMMDDHHMM   秒为0
		StationID_t		TransStation;                   // 交易站点				线路（1HEX）+车站（1HEX）
		CString         TransType;                      // 交易类型
		DWORD           TransValue;                     // 交易金额
		_str_card_history_data(){
			TransStation =0;
			TransType ="";
			TransValue =0;
		}
	}STR_CARD_HISTORY_DATA;

    STR_CARD_APPLICATION_DATA   cardData;				// 卡片数据
	STR_CARD_PURSE_DATA			purseData;				// 钱包数据
	STR_CARD_CHARGE_DATA        chargeData;				// 充值数据
    STR_CARD_JOURNEY_DATA       journeyData;			// 旅程数据
    STR_CARD_DISCOUNT_DATA		discountData;			// 优惠信息
	vector<STR_CARD_HISTORY_DATA>      historyData;     // 历史记录

private:
	void Initialize();
};
