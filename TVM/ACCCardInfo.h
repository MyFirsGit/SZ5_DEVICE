#pragma once
#include "StdAfx.h"
/**
@brief ACC票卡数据存储类                            
*/
#define CARD_PH_ID_LEN 7
#define CARD_CERTIFICATE_ID_LEN 9

class CACCCard
{
public:
	// 产品信息结构体
	typedef struct _acccard_product_info
	{
		ProductCategory_t productCategory;		// 产品种类
		WORD productType;						// 产品类型
		WORD productSubType;					// 产品子类型
		BYTE productIssuerID;					// 产品发行商ID
		BYTE byProductStatus;					// 产品状态  卡片状态. 1：未锁定；2：不明原因锁定；3：票卡回收或退卡锁定；5：坏卡锁定；6：未发行卡锁定；11：丢卡锁定；12：批次锁定；13：到达生命周期锁定；14：过期未使用锁定；15：未设定。
		ACC_CARD_ACTIVE_STATUS CardActived;		// 产品激活标志 产品是否激活，0：未激活，1：已激活
		_DATE_TIME ValidStartTime;				// 产品有效期开始时间, 7B, BCD
		_DATE_TIME ValidEndTime;				// 产品有效期结束时间 , 7B, BCD
		DWORD dwStartStation;					// 产品有效乘车起点, 车站位置信息
		DWORD dwEndStation;						// 产品有效乘车终点, 车站位置信息
		WORD wArea;								// 产品有效使用区间
		DWORD Balance;							// 产品余额，如为定期票，则此字段无意义。
		short sSamID;							// 卡内保存的上次对产品发售/加值的SAM ID
		DWORD nRemainRides;						// 余次（当前仅对应老人卡的余次）
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

	//个人信息结构体
	typedef struct _acc_Card_personalize_info
	{
		_DATE birthday;                                        // 生日
		BYTE certificateCardNumber[CARD_CERTIFICATE_ID_LEN];   // 证件号
		BYTE certificateCardType;                              // 证件类型
		TCHAR name[20];                                        // 姓名
		WORD companyID;                                        // 公司
		ClassificationLevel_t userStatus;                      // 乘客级别 0:普通 1:贵宾 2:-7:保留0:普通 1:贵宾 2:-7:保留	
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

	// 旅程信息结构体
	typedef struct _card_journey_data {
		_DATE_TIME travelStartTime;				// 本次旅程开始时间, 7B, BCD
		DWORD dwTravelStartStation;				// 本次旅程的起始站点.本次旅程的起始站点的车站位置信息
		int nPaid;								// 已付车费
		_DATE_TIME lastTransTime;				// 上次交易时间
		DWORD dwLastTransStation;				// 上次交易车站的车站位置信息
		ACC_CARD_JOURNEY_STATUS byTravelStatus;	// 上次交易旅程区状态
		_card_journey_data()
		{
			dwTravelStartStation = 0;
			nPaid = 0;
			dwLastTransStation = 0;
			byTravelStatus = TICKET_INIT;
		}
	}CARD_JOURNEY_DATA;

	// 历史信息结构体
	typedef struct _card_history_data {
		_DATE_TIME TransTime;			// 交易时间
		BYTE serviceProviderId;			// 服务提供者
		DWORD TransOperator;			// 操作员
		BYTE productIssuerId;			// 发行商
		BYTE category;					// 产品类型  0:无 1:钱包 2:定期类型 3:计次类型 4-7:预留
		BYTE productType;				// 产品类型
		BYTE TransType;					// 交易类型  02 表示充值，06 表示消费，09 表示复合消费
		WORD paymentMethod;				// 交易方式
		BYTE transStation[6];				// 交易地点
		DWORD TransValue;				// 交易金额
		DWORD remaingValue;				// 交易后金额
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

	// 票卡基本信息结构体
	typedef struct _str_card_application_data {
		short sCityCode;								// 城市代码,城市代码，UL卡填默认值0x00, BCD
		short sTradeCode;								// 行业代码.行业代码，UL卡填默认值0x00, BCD
		BYTE  CardPhysicalNumber[CARD_PH_ID_LEN];		// 车票物理卡号，不够7字节前补0
		//TicketLogicalID_t CardLogicalNumber;			// 车票逻辑编号
		BYTE CardLogicalNumber[8];						// 车票逻辑编号
		short sLifeCycleCount;							// 卡重发行计数
		ACC_CARD_TEST_STATUS  testFlag;					// 卡片测试标志.0x00：正式车票, 0x01：供测试或培训使用
		BYTE  CardIssuerCode;							// 卡发行商代码
		_DATE initialDate;								// 初始化日期, 4Bytes, BCD
		short sInitialNum;								// 初始化批次
		BYTE  byPassengerType;							// 乘客类型:成人，儿童等
		DWORD  Deposit;									// 押金
		BYTE  TicketStatus;								// M1卡片状态. 1：未锁定；2：不明原因锁定；3：票卡回收或退卡锁定；5：坏卡锁定；6：未发行卡锁定；11：丢卡锁定；12：批次锁定；13：到达生命周期锁定；14：过期未使用锁定；15：未设定。
		// UL卡片状态. 0:保留 1:没有锁定 2:锁定 3:锁定-废弃/回收卡
		ProductCategory_t  byLastUsedProductCategory;	// 最后使用的产品种类.0x01 = 钱包；0x02 = 定期；0x03 = 计次
		WORD byLastUsedProductType;						// 最后使用的产品类型
		BYTE  byLastUsedProductSubType;					// 最后使用的产品子类型
		BYTE  byProductsNum;							// 产品数量
		BYTE  byTransNum;								// 已换乘次数		
		BYTE  byPersonalizeStatus;						// 个性化标识
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
	bool GetProductInfo(WORD productType, WORD productSubType, ACCCARD_PRODUCT_INFO& productInfo);	// 获取指定的卡内产品信息

	CARD_APPLICATION_DATA		applicationData;	// 应用数据
	CARD_JOURNEY_DATA			journeyData;		// 旅程数据
	CARD_HISTORY_DATA			rechargeData;		// 充值记录
	vector<CARD_HISTORY_DATA>   historyData;		// 历史记录
	ACC_CARD_PERSONALIZE_INFO   userInfo;			// 个人信息
	vector<ACCCARD_PRODUCT_INFO> v_ProductInfo;		// 卡内产品信息

private:
	void Initialize();
};