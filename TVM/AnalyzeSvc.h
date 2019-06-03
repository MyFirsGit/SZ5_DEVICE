#pragma once
//#include "TVMForeService.h"
#include "ACCCardInfo.h"
#include "PassengerForeSvc.h"

#define UD_MAX_BASEINFO		256
#define UD_MAX_USERINFO		256
#define UD_MAX_ANAROWS		15
#define UD_MAX_WIDEROWS		22
/**
  @brief ����ҵ��Service                                                                  
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

    // �������
    typedef enum _dialog_group {
        DIALOG_GROUP_WAIT	= 1,
        DIALOG_GROUP_BASE_UL,
		DIALOG_GROUP_BASE_M1,
		DIALOG_GROUP_BASEINFO,
		DIALOG_GROUP_HISTORY,
		DIALOG_GROUP_USERINFO,
		DIALOG_GROUP_ERROR,
    } DIALOG_GROUP;

	// ��ʾ��Ϣ����
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
		WORD ProductType;           // ��Ʒ���� 
		WORD ProductSubType;           // �Ӳ�Ʒ���� 

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
		CString strTicketID;//�ɣ�
		CString strTicketType;//������
		CString strExpireData;//��ֹ����
		CString strPassageType;//�˿�����
		CString strOriginal; //ʼ��վ
		CString strDestination;//�յ�վ
		CString strRemainValue;//���
		CString strTicketStatus;//Ʊ��״̬
		CString strJouneryStatus;//�ó�״̬
		CString strLastTransTime;//�ϴν���ʱ��	
		CString strLastTransLocation;//�ϴν��׵ص�

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
		CString strActiveSatus;//����״̬

		_tagUlBaseInfo()
		{
			strActiveSatus = _T("");
		}
	}UlBaseInfo; //UL������Ϣ

	typedef struct _tagM1BaseInfo
	{
		BaseInfo baseInfo;
		CString strDeposit;//Ѻ��
		CString strProductStatus;//��Ʒ״̬

		_tagM1BaseInfo()
		{
			/*
			memset(tcDeposit,0,UD_MAX_BASEINFO);
			memset(tcProductStatus,0,UD_MAX_BASEINFO);
			*/
			strDeposit = _T("");
			strProductStatus = _T("");
		}
	}M1BaseInfo; //M1������Ϣ

	typedef struct _tagUserInfo
	{
		CString strName;
		CString strIdentityCardType;
		CString strIdentityCardNum;
		//TCHAR tcName[UD_MAX_USERINFO]; //����		
		//TCHAR tcIdentityCardType[UD_MAX_USERINFO];//֤������
		//TCHAR tcIdentityCardNum[UD_MAX_USERINFO];//֤����
		_tagUserInfo()
		{
			strName = _T("");
			strIdentityCardType = _T("");
			strIdentityCardNum = _T("");
			/*memset(tcName,0,UD_MAX_USERINFO);
			memset(tcIdentityCardType,0,UD_MAX_USERINFO);
			memset(tcIdentityCardNum,0,UD_MAX_USERINFO);*/
		}
	}UserInfo; //�û���Ϣ
	
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
		_DATE_TIME printTime;	// ��ǰ��ӡʱ��	
		BYTE currentShow;

		_DATE_TIME StartDate;		// 7 BCD ������ʼ����
		_DATE_TIME EndDate;			// 7 BCD ���׽������ڣ�	EndDate �C Start <=31��
		vector<ModeProductType> productType; // ��Ʒ���͡�������
		long commandExecuteResult;          // ��ѯ�Ƿ�ɹ��ķ���ֵ
		// ������Ϣ
		DWORD ticketTypeName;
		DWORD StartStation;
		DWORD EndStation;
		_DATE_TIME strValidEndTime;
		DWORD strBalance;
		ProductCategory_t ticketCategory;
		// ��ʷ��Ϣ	
		DWORD TransactionCount;			// ��ѯ���ؽ��׼�¼��
		// ������Ϣ		
		CString passengerName;			// ����
		BYTE	passengerGender;		// �Ա�
		CString passengerBirthday;		// ��������
		CString passengerPhone;			// �绰
		CString passengerEmail;			// �ʼ�
		CString passengerAddress;		// ��ַ
		vector<HistoryProductInfo> result;// ��ѯ���

		vector<pair<int,CString>> vec_baseinfo;//������Ϣ���� int���� CString����
		vector<HistoryTranceInfo> vec_trans_history;//������ʷ��¼����
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
	bool RequestHistoryData();			// ������ʷ��¼����
	bool RequestUserInfoData();
	CString ComPrintLineFormatForTemplate(CString& PrintLine);
	void GetCardBaseInfo(CardType_t cardType/*, WORD productType, WORD productSubType*/, CACCCard::ACCCARD_PRODUCT_INFO& productInfo);//��ȡƱ��������Ϣ����
	void GetM1PrivateInfo();//��ȡM1��������Ϣ����
	static void Abort(){m_IsAbort = true;}
protected:
	void OnReadCard(UINT,CRWReadCard*);
	void GetBaseInfoData(WORD productType, WORD productSubType, CACCCard::ACCCARD_PRODUCT_INFO& productInfo); //��ȡM1����Ϣ
	virtual COIN_ACCEPTABLE GetAcceptCoinFaceTable();				// ��ȡ�ɽ��յ�Ӳ�ҵ����
	virtual BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList *table = NULL);			// ��ȡ�ɽ��յ�ֽ�ҵ����

	void OnStart();

private:
    long BlockCard();
	void OnError(CString strMessage);
	DataModel m_DataModel;
	DWORD m_dwTime;
	static bool m_IsAbort;

};


