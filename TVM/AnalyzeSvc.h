#pragma once
//#include "TVMForeService.h"
#include "ACCCardInfo.h"
#include "PassengerForeSvc.h"

#define UD_MAX_BASEINFO		256
#define UD_MAX_USERINFO		256
#define UD_MAX_ANAROWS		15
#define UD_MAX_WIDEROWS		22
/**
  @brief 分析业务Service                                                                  
*/
static const TCHAR* const WM_ANALYZE_WAIT_TO_BASEUL = _T("WM_ANALYZE_WAIT_TO_BASEUL");
static const TCHAR* const WM_ANALYZE_WAIT_TO_BASEM1 = _T("WM_ANALYZE_WAIT_TO_BASEM1");
static const TCHAR* const WM_ANALYZE_BASEM1_TO_BASEINFO=_T("WM_ANALYZE_BASEM1_TO_BASEINFO");
static const TCHAR* const WM_ANALYZE_BASEM1_TO_HISTORY = _T("WM_ANALYZE_BASEM1_TO_HISTORY");
static const TCHAR* const WM_ANALYZE_BASEM1_TO_USERINFO = _T("WM_ANALYZE_BASEM1_TO_USERINFO");
static const TCHAR* const WM_ANALYZE_BASEINFO_TO_BASEM1=_T("WM_ANALYZE_BASEINFO_TO_BASEM1");
static const TCHAR* const WM_ANALYZE_HISTORY_TO_BASEM1 = _T("WM_ANALYZE_HISTORY_TO_BASEM1");
static const TCHAR* const WM_ANALYZE_USERINFO_TO_BASEM1 = _T("WM_ANALYZE_USERINFO_TO_BASEM1");

class CAnalyzeSvc : public CPassengerForeSvc
{
    DECLARE_MESSAGE_MAP()

public:

    // 画面分组
    typedef enum _dialog_group {
        DIALOG_GROUP_WAIT	= 1,
        DIALOG_GROUP_BASE_UL,
		DIALOG_GROUP_BASE_M1,
		DIALOG_GROUP_BASEINFO,
		DIALOG_GROUP_HISTORY,
		DIALOG_GROUP_USERINFO,
		DIALOG_GROUP_ERROR,
    } DIALOG_GROUP;

	// 显示信息分组
	typedef enum _showInfo_group {
		SHOW_PURSE	= PURSE,
		SHOW_MULITY = MULTIRIDE,
		SHOW_PERIOD = PERIOD,
		SHOW_CARDINFO,
		SHOW_HISTORY,
		SHOW_USERINFO
	} SHOWINFO_GROUP;

	typedef struct _tagProductType
	{
		WORD ProductType;           // 产品类型 
		WORD ProductSubType;           // 子产品类型 

	}ModeProductType;

	typedef enum _UE_ANALYZENOTICE
	{
		UE_ANA_BASEULTITLE=0x05030040,
		UE_ANA_BASEM1TITLE,
		UE_ANA_BASEIINFOTITLE,
		UE_ANA_HISTORYTITLE,
		UE_ANA_USERINFOTITLE,
		UE_ANA_TICKID,
		UE_ANA_TICKTYPE,
		UE_ANA_EXPIREDATA,
		UE_ANA_PASSAGETYPE,
		UE_ANA_ORIGINAL,

		UE_ANA_DESTINATION=0x5030050,
		UE_ANA_REMAINVALUE,
		UE_ANA_TICKETSTATUS,
		UE_ANA_JOUNERYSTATUS,
		UE_ANA_LASTTRANSTIME,
		UE_ANA_LASTTRANSLOCATION,
		UE_ANA_ACTIVESTATUS,
		UE_ANA_DEPOSIT,
		UE_ANA_PRODUCTSTATUS,
		UE_ANA_NAME,

		UE_ANA_IDENTITYCARDTYPE=0x5030060,
		UE_ANA_IDENTITYCARDNUM
	}UE_ANALYZENOTICE;
	typedef struct _tagBaseInfo
	{
		CString strTicketID;//ＩＤ
		CString strTicketType;//卡类型
		CString strExpireData;//截止日期
		CString strPassageType;//乘客类型
		CString strOriginal; //始发站
		CString strDestination;//终点站
		CString strRemainValue;//余额
		CString strTicketStatus;//票卡状态
		CString strJouneryStatus;//旅程状态
		CString strLastTransTime;//上次交易时间	
		CString strLastTransLocation;//上次交易地点

		_tagBaseInfo()
		{
			strTicketID = _T("");
			strTicketType = _T("");
			strExpireData = _T("");
			strPassageType = _T("");
			strOriginal = _T("");
			strDestination = _T("");
			strRemainValue = _T("");
			strTicketStatus = _T("");
			strJouneryStatus = _T("");
			strLastTransTime = _T("");
			strLastTransLocation = _T("");
		}
	}BaseInfo;

	typedef struct _tagUlBaseInfo
	{
		BaseInfo baseInfo;
		CString strActiveSatus;//激活状态

		_tagUlBaseInfo()
		{
			strActiveSatus = _T("");
		}
	}UlBaseInfo; //UL基本信息

	typedef struct _tagM1BaseInfo
	{
		BaseInfo baseInfo;
		CString strDeposit;//押金
		CString strProductStatus;//产品状态

		_tagM1BaseInfo()
		{
			/*
			memset(tcDeposit,0,UD_MAX_BASEINFO);
			memset(tcProductStatus,0,UD_MAX_BASEINFO);
			*/
			strDeposit = _T("");
			strProductStatus = _T("");
		}
	}M1BaseInfo; //M1基本信息

	typedef struct _tagUserInfo
	{
		CString strName;
		CString strIdentityCardType;
		CString strIdentityCardNum;
		//TCHAR tcName[UD_MAX_USERINFO]; //姓名		
		//TCHAR tcIdentityCardType[UD_MAX_USERINFO];//证件类型
		//TCHAR tcIdentityCardNum[UD_MAX_USERINFO];//证件号
		_tagUserInfo()
		{
			strName = _T("");
			strIdentityCardType = _T("");
			strIdentityCardNum = _T("");
			/*memset(tcName,0,UD_MAX_USERINFO);
			memset(tcIdentityCardType,0,UD_MAX_USERINFO);
			memset(tcIdentityCardNum,0,UD_MAX_USERINFO);*/
		}
	}UserInfo; //用户信息
	
	typedef struct _tagCardinfo
	{
		BaseInfo baseInfo;
		CString strDeposit;
		CString strProductStatus;
		CString strActiveSatus;
		_tagCardinfo()
		{
			strDeposit = _T("");
			strProductStatus = _T("");
			strActiveSatus = _T("");
		}

	}CARD_INFO;

	typedef struct _tagDataModel
	{
		UlBaseInfo ulBaseInfo;
		M1BaseInfo m1BaseInfo;
		UserInfo   userInfo;
		CARD_INFO  cardInfo;

		RW_READ_CARD_RESPONSE rwReadResponse;
		CACCCard* accCard;
		_DATE_TIME printTime;	// 当前打印时间	
		BYTE currentShow;

		_DATE_TIME StartDate;		// 7 BCD 交易起始日期
		_DATE_TIME EndDate;			// 7 BCD 交易结束日期，	EndDate – Start <=31天
		vector<ModeProductType> productType; // 产品类型、子类型
		long commandExecuteResult;          // 查询是否成功的返回值
		// 基本信息
		DWORD ticketTypeName;
		DWORD StartStation;
		DWORD EndStation;
		_DATE_TIME strValidEndTime;
		DWORD strBalance;
		ProductCategory_t ticketCategory;
		// 历史信息	
		DWORD TransactionCount;			// 查询返回交易记录数
		// 个人信息		
		CString passengerName;			// 姓名
		BYTE	passengerGender;		// 性别
		CString passengerBirthday;		// 出生日期
		CString passengerPhone;			// 电话
		CString passengerEmail;			// 邮件
		CString passengerAddress;		// 地址
		vector<HistoryProductInfo> result;// 查询结果

		vector<pair<int,CString>> vec_baseinfo;//基本信息数据 int文言 CString数据
		vector<HistoryTranceInfo> vec_trans_history;//交易历史记录数据
		_tagDataModel(){
			ticketTypeName = 0;
			StartStation = 0;
			EndStation = 0;
			strBalance = 0;

			passengerName = _T("");
			passengerGender = SEX_UNSPECIFIED;
			passengerBirthday = _T("");
			passengerPhone = _T("");
			passengerEmail = _T("");
			passengerAddress = _T("");

			accCard = NULL;
			printTime = ComGetCurTime();
			currentShow = SHOW_PURSE;
		}
	}DataModel;

	CAnalyzeSvc();
	virtual ~CAnalyzeSvc();
	DataModel& GetDataModel();
	void PrinterPrint();
	void PrintQuerySuccessHistory();
	void PrintQuerFailHistory();
	void NotifyDataModelChanged();
	bool RequestHistoryData();			// 请求历史记录数据
	bool RequestUserInfoData();
	CString ComPrintLineFormatForTemplate(CString& PrintLine);
	void GetCardBaseInfo(CardType_t cardType/*, WORD productType, WORD productSubType*/, CACCCard::ACCCARD_PRODUCT_INFO& productInfo);//获取票卡基本信息数据
	void GetM1PrivateInfo();//获取M1卡个人信息数据
	static void Abort(){m_IsAbort = true;}
protected:
	void OnReadCard(UINT,CRWReadCard*);
	void GetBaseInfoData(WORD productType, WORD productSubType, CACCCard::ACCCARD_PRODUCT_INFO& productInfo); //获取M1卡信息
	virtual COIN_ACCEPTABLE GetAcceptCoinFaceTable();				// 获取可接收的硬币的面额
	virtual BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList *table = NULL);			// 获取可接收的纸币的面额

	void OnStart();

private:
    long BlockCard();
	void OnError(CString strMessage);
	DataModel m_DataModel;
	DWORD m_dwTime;
	static bool m_IsAbort;

};


