#pragma once
#include "tvmforeservice.h"
#include "tpuerrorinfo.h"
#include "SCAuditManager.h"
#include "AudioHelper.h"
#include "DataHelper.h"
#include "TransactionInfo.h" 


/**
@brief      �˿�ҵ��Service
*/
class CPassengerForeSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:

	typedef enum _timers_id{
		TIMER_COUNTING_TO_RETURN				= 1,	// �����е������еļ�ʱ������������Ͷ�ҳ�ʱ��
		TIMERS_OUT_OF_REPLAY					= 2,	// �ظ�������һ��������ʱ��ID(ä��ҵ��)
		TIMERS_OPERATION_OUT_OF_TIME			= 3,	// ä�˲�����ʱʱ���ʱ��ID(ä��ҵ��)
		TIMERS_COUNTING_BEFORE_ISSUE			= 4,	// ��ʼ��Ʊ����ʼ��Ʊ֮��ļ�ʱ��������人ҵ��������
		TIMERS_WEB_QUERY_ORDER					= 5,	// ������ѯ��ʱ��
		TIMERS_WAIT_CARD_INSERT					= 6,	// �ȴ���ֵ������
	}TIMERS;

	typedef enum _finish_type {
		DEFAULT = 0,
		PASSAGE_CANCEL,						// �˿�ȡ��
		OVER_MONEY_AMOUNT_LIMIT,			// Ͷ�����
		OVER_MONEY_COUNT_LIMIT,				// Ͷ����������
		OVER_CHANGE_COUNT_LIMIT,			// ������������
		OVER_CHANGE_AMOUNT_LIMIT,			// ���㲻��
		SERVICE_ERROR_CANCEL,				// ҵ���߼�����ȡ��
		EXCEPTION_ERROR,					// �쳣ȡ��
		INSERT_OUT_OF_TIME,					// Ͷ�볬ʱȡ��
		NOT_ENOUGH_TICKETS_FOR_BOOKING,     // Ʊ�����㣬�޷���Ʊ
		TRANSACTION_STATE_FAILED,			// ����ʧ�ܣ�
		RUNNING_OUT_OF_TIME,				// ���׳�ʱ
		CHANGE_FALIED_RETURN,				// ����ʧ�ܽ���
		QR_ORDER_GENERATE_FAILED,			// ��ά�붩������ʧ��
	} FINISH_TYPE;

	// Ӳ�ҽ���״̬
	typedef enum CoinAcceptState{
		COIN_ACCEPT_STATE_NONE 					= 0,			// �޽���
		COIN_ACCEPT_STATE_INSERTED 				= 1,			// ��Ͷ��
		COIN_ACCEPT_STATE_COLLECTED 			= 2,			// ������
		COIN_ACCEPT_STATE_RETURNED 				= 3,			// ���˳�
		COIN_ACCEPT_STATE_RETURN_FAILD			= 4,			// �˱�ʧ��
		COIN_ACCEPT_STATE_ERROR					= 5,			// ����ʧ��
		COIN_ACCEPT_STATE_COLLECTED_FAILD		= 6,			// ����ʧ��
	}COIN_ACCEPT_STATE;

	// ֽ�ҽ���״̬
	typedef enum BanknoteAcceptState{
		BANKNOTE_ACCEPT_STATE_NONE 				= 0,			// �޽���
		BANKNOTE_ACCEPT_STATE_INSERTED 			= 1,			// ��Ͷ��
		BANKNOTE_ACCEPT_STATE_COLLECTED 		= 2,			// ������
		BANKNOTE_ACCEPT_STATE_RETURNED 			= 3,			// ���˳�
		BANKNOTE_ACCEPT_STATE_RETURN_FAILED		= 4,			// �˱�ʧ��
		BANKNOTE_ACCEPT_STATE_ERROR				= 5,			// ����ʧ��
		BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED	= 6,			// ����ʧ��
	}BANKNOTE_ACCEPT_STATE;

	// Ӳ������״̬
	typedef enum CoinChangeState{
		COIN_CHANGE_STATE_NONE 					= 0,			// �������ǰ
		COIN_CHANGE_STATE_SUCCESS 				= 1,			// ����ɹ�
		COIN_CHANGE_STATE_FAIL 					= 2,			// ����ʧ��
		COIN_CHANGE_STATE_NA					= 3,			// ��������
	}COIN_CHANGE_STATE;

	// ֽ������״̬
	typedef enum BanknoteChangeState{
		BANKNOTE_CHANGE_STATE_NONE 				= 0,			// ������
		BANKNOTE_CHANGE_STATE_SUCCESS 			= 1,			// ����ɹ�
		BANKNOTE_CHANGE_STATE_FAIL 				= 2,			// ����ʧ��
	}BANKNOTE_CHANGE_STATE;

	// ֧��״̬�����п���EP֧��ʱ�ã�
	typedef enum PaymentState{
		PAYMENT_STATE_NONE 						= 0,			// ��֧��
		PAYMENT_STATE_SUCCESS 					= 1,			// ֧���ɹ�
		PAYMENT_STATE_FAIL 						= 2,			// ֧��ʧ��
	}PAYMENT_STATE;

	// ��ӡ״̬
	typedef enum FailurePrintState{
		FAILURE_PRINT_STATE_NONE 				= 0,			// �޴�ӡ
		FAILURE_PRINT_STATE_SUCCESS 			= 1,			// ��ӡ�ɹ�
		FAILURE_PRINT_STATE_FAIL 				= 2,			// ��ӡʧ��
	}FAILURE_PRINT_STATE;

	// ����״̬ 
	typedef enum TransactionState{
		TRANSACTION_STATE_NONE 					= 0,			// �޽���
		TRANSACTION_STATE_SUCCESS 				= 1,			// ������������
		TRANSACTION_STATE_FAIL 					= 2,			// ���׹��Ͻ���
		TRANSACTION_STATE_CANCEL_SUCCESS		= 3,			// ȡ������������Ԥ����
		TRANSACTION_STATE_CANCEL_FAIL			= 4,			// ȡ���쳣������Ԥ����
	}TRANSACTION_STATE;

	// �����쳣
	typedef enum TransactionException{
		//TRANSACTION_EXCEPTION_TOKEN_MOUDOL	  = 1,			// Token�쳣
		TRANSACTION_EXCEPTION_CARD_MOUDOL	  = 1,			// Card �쳣
		TRANSACTION_EXCEPTION_RELEASE_COIN	  = 3,			// Ӳ�������쳣
		TRANSACTION_EXCEPTION_RELEASE_BANKNOTE= 4,			// ֽ�������쳣
		TRANSACTION_EXCEPTION_CREDIT		  = 5,			// ���ÿ��쳣
		TRANSACTION_EXCEPTION_BANKCARD		  = 6,			// ����쳣
		TRANSACTION_EXCEPTION_EP		      = 7,			// EP֧���쳣
		TRANSACTION_EXCEPTION_RETURN_COIN     = 8,			// Ӳ�ҷų��쳣
		TRANSACTION_EXCEPTION_RETURN_BANKNOTE = 9,			// ֽ�ҷų��쳣
		TRANSACTION_EXCEPTION_WRITE_CARD_FAILED = 10,		// Ʊ��������󣨳�ֵ����Ӳ�Ʒ�ã�
		TRANSACTION_EXCEPTION_COIN_INSERT		= 11,		// Ӳ��Ͷ���쳣
		TRANSACTION_EXCEPTION_COIN_COLLECTION	= 12,		// Ӳ�������쳣
		TRANSACTION_EXCEPTION_BANKNOTE_INSERT	= 13,		// ֽ��Ͷ���쳣
		TRANSACTION_EXCEPTION_BANKNOTE_COLLECTION = 14,		// ֽ�������쳣
	}TRANSACTION_EXCEPTION;

	// ģ��æµ״̬
	typedef enum _device_status_{
		DEVICE_ALL_OK				= 0x00,					// �����豸Ϊ����״̬
		DEVICE_COIN_BUSY			= 0x01,					// Ӳ��ģ�����ڴ���
		DEVICE_BANKNOTE_BUSY		= 0x02,					// ֽ��ģ�����ڴ���
		DEVICE_BANKNOTE_CHANGE_BUSY = 0x04,					// ֽ������ģ�����ڴ���
		DEVICE_CARD_BUSY			= 0x08,					// ����ģ�����ڴ�����
	};
	// Ʊ����Ϣ��¼��
	typedef struct _tagMediumInfo
	{
		CString cardLogicalNumber;							// Ʊ���߼���ţ�CACCCard��
		CARD_MEDIUM_TYPE mediumType;						// ������������
		BYTE cardIssuer;									// ��Ƭ������
		CardType_t cardPHType;								// Ʊ���������ͣ�0x11��һƱͨM1����0x12��һƱͨUL����
		long lCardDeposit;									// Ʊ��Ѻ��
		long lCardFee;										// Ʊ���ɱ���
		PassengerType_t passengerType;						// Ʊ���˿�����

		_tagMediumInfo () {
			cardLogicalNumber = _T("");
			mediumType = MIFARE1K;
			cardPHType = CARD_TYPE_ACC_M1;
			lCardDeposit = 0;
			lCardFee = 0;
			passengerType = 0x01; // ���� // should use "PASSENGER_TYPE_UNSPECIFIED"
		}
	} MEDIUM_INFO;

	// ��Ʒ��Ϣ��¼��
	typedef struct _tagProductInfo
	{
		ProductCategory_t productCategory;					// ��Ʒ���ࣨǮ�������ڡ��ƴΣ�
		ProductType_t productType;							// ��Ʒ���ͣ�SVT��PST��MST��
		ProductTypeVariant_t productSubType;				// ��Ʒ�����ͣ���ƴ�Ʊ��30�Ρ�50�Ρ�100�Σ�
		LocationNumber_t beginLocation;						// ��ʼվ�������
		LocationNumber_t endLocation;						// Ŀ��վ�������
		WORD useArea;										// ʹ������
		long lBalanceAmount;								// SVT��PST��ʹ��
		long lBalanceRides;									// MST������PST��ʹ��
		Duration_t lValidatePeriod;							// PSTʱ����Ϣ
		_DATE_TIME dtValidateDateTime;						// ��Ʒ��Ч�ڽ���ʱ��
		long lProductDeposit;								// ��ƷѺ��
		long lProductSaleAmount;							// ��Ʒ���۵���
		int  nProductPiece;									// ��Ʒ��������Ʊ��
		int  nProductIssuedPiece;							// ���۲�Ʒ��������Ʊ��
		_tagProductInfo () {
			productCategory = UNDEFINE;
			productType = static_cast<ProductType_t>(UNKNOWPRODUCT);
			productSubType = 0;
			beginLocation = LOCATION_NUMBER_UNSPECIFIED;
			endLocation = LOCATION_NUMBER_UNSPECIFIED;
			lBalanceAmount = 0;
			lBalanceRides = 0;
			useArea = 0;
			lValidatePeriod = static_cast<Duration_t>(0);
			dtValidateDateTime = _date_time();
			lProductDeposit = 0;
			lProductSaleAmount = 0;
			nProductPiece = 0;
			nProductIssuedPiece = 0;
		}

		// PRODUCT_INFO��lBalanceAmount��lBalanceRides��lValidatePeriodΪ��Ʒ��������Ϣ��

		// ������Ʊ����Ӳ�Ʒ������ԭ��ʣ�����Ϊ0��lBalanceAmount��lBalanceRides��lValidatePeriod��ͬ��
		// �������۲�Ʒ����Ϣ����Ϊ��Ӧ��Ʒ���͵Ĳ�����Ϣ��

		// ���ڳ�ֵҵ��lBalanceAmount��lBalanceRidesΪ��Ʒ��ֵǰ/��ľ�����Ϣ����ֵ��������ñ���������
		// lValidatePeriod��Ϊ�޷���Ʊ���ж������ʲ���ʹ�á�

	} PRODUCT_INFO;

	// ֧����Ϣ��¼��
	typedef struct _tagPaymentInfo
	{
		PaymentMethod_t paymentMethod;						// ֧����ʽ
		long lPriceAmount;									// ˰ǰ���
		long lTaxAmount;									// ˰�ѽ��
		long lDueAmount;									// Ӧ�ս��
		long lPaidAmount;									// ʵ�ս��
		long lChangeAmount;									// ������
		long lReturnAmount;									// �˿���
		long lNotChangeAmount;								// δ������

		// Ӳ��֧����Ϣ
		long lCHReceiveTotalAmount;							// Ӳ��֧���ܽ��
		long lCHReceiveTotalCount;							// Ӳ��֧���ܸ���
		map<BankNoteAndCoinType_t,long> PayCoinInfo;		// Ӳ��֧������

		// ֽ��֧����Ϣ
		long lBHReceiveTotalAmount;							// ֽ��֧���ܽ��
		long lBHReceiveTotalCount;							// ֽ��֧���ܸ���
		map<BankNoteAndCoinType_t,long> PayBankNoteInfo;	// ֽ��֧������

		// ���п�֧��
		BYTE BankCardNum[10];								// ���п���	
		long lBankReceiveTotalAmount;						// ���п�֧���ܽ��

		// EP֧��
		BYTE EPCardNum[8];									// EP����
		long lEPReceiveTotalAmount;							// EP֧���ܽ��

		// ������Ϣ
		//DWORD dwChangeStockCount;							// ѭ������������ö��
		//DWORD dwChangeHopper1Count;						// ������1����ö��
		//DWORD dwChangeHopper2Count;						// ������2����ö��

		DWORD dwChangeCoinOneYuanCount;						// Ӳ������һԪ����
		DWORD dwChangeCoinHalfYuanCount;					// Ӳ��������Ǹ���
		DWORD dwRecycleBNRBoxACount;						// ֽ��ѭ������A��������������������ȷ����
		DWORD dwRecycleBNRBoxBCount;						// ֽ��ѭ������B��������������������ȷ����
		DWORD dwRecycleBNRBoxCCount;						// ֽ��ѭ������C��������������������ȷ����
		DWORD dwRecycleBNRBoxDCount;						// ֽ��ѭ������D��������������������ȷ����

		DWORD dwChangeBNRBoxCount;							// ֽ��������������������������ȷ����

		DWORD dwChangeBHAmout;								// ֽ��������
		map<MONEY_BOX_TYPE,long> ChargeCoinBanknoteInfo;	// ����֧������

		DWORD dwCoinChangeErrorAmount;						// Ӳ������ʧ�ܽ��
		DWORD dwBanknoteChangeErrorAmount;					// ֽ������ʧ�ܽ��

		map<MONEY_BOX_TYPE,long> CoinSaveDetail;			// Ӳ��������Ϣ

		CH_COMMON_RSP rspCoinModule;						// Ӳ����Ϣ

		DWORD dwBanknoteChangeRetryCount;					// ֽ���������Դ���
		_tagPaymentInfo () {
			paymentMethod = PAYMENT_METHOD_CASH;			// Ĭ���ֽ�֧��
			lPriceAmount = 0;
			lTaxAmount = 0;
			lDueAmount = 0;
			lPaidAmount = 0;
			lChangeAmount = 0;
			lReturnAmount = 0;
			lNotChangeAmount = 0;

			lCHReceiveTotalAmount = 0;
			lCHReceiveTotalCount = 0;
			PayCoinInfo.clear();

			lBHReceiveTotalAmount = 0;
			lBHReceiveTotalCount = 0;
			PayBankNoteInfo.clear();

			memset(BankCardNum, 0, 10);
			lBankReceiveTotalAmount = 0;

			memset(EPCardNum, 0, 8);
			lEPReceiveTotalAmount = 0;

			lEPReceiveTotalAmount = 0;

			/*dwChangeStockCount = 0;
			dwChangeHopper1Count = 0;
			dwChangeHopper2Count = 0;*/
			dwChangeCoinHalfYuanCount = 0;
			dwChangeCoinOneYuanCount = 0;
			/*dwChangeBHBoxACount = 0;
			dwChangeBHBoxBCount = 0;*/
			dwRecycleBNRBoxACount = 0;
			dwRecycleBNRBoxBCount = 0;
			dwRecycleBNRBoxCCount = 0;
			dwRecycleBNRBoxDCount = 0;
			dwChangeBNRBoxCount = 0;

			dwChangeBHAmout = 0;
			ChargeCoinBanknoteInfo.clear();
			CoinSaveDetail.clear();
			dwCoinChangeErrorAmount = 0;
			dwBanknoteChangeErrorAmount = 0;
			dwBanknoteChangeRetryCount = 0;
			//rspCoinModule.coin_info = CH_COMMON_RSP_COININFO();

		}

		// ֽ�����������ܺ�
		DWORD BNRChangeCount(){
			return dwRecycleBNRBoxACount + dwRecycleBNRBoxBCount + dwRecycleBNRBoxCCount + dwRecycleBNRBoxDCount + dwChangeBNRBoxCount;
		}
	} PAYMENT_INFO;

	// ������Ϣ��¼��
	typedef struct _tagOperationInfo
	{
		FINISH_TYPE finishType;         					// ҵ�����ԭ��
		int readCardCount;									// ������������
		bool isCoinSlotOpen;								// Ӳ�ҿ��Ƿ��
		bool isBankNoteSlotOpen;							// ֽ�ҿ��Ƿ��
		bool isBankCardSlotOpen;							// ���п����Ƿ��
		bool isWaitingEPCard;								// �Ƿ��ڶ�EP��
		bool isFinishedReceveMoney;							// �Ƿ���ɽ���Ǯ
		//bool isFailedIssueCard;								// �Ƿ��ۿ�ʧ��
		//bool isSuccessIssueCard;							    // �ۿ��Ƿ�ɹ�
		bool isBeginBuyProduct;								// �Ƿ�ʼ���Ʒ
		bool isBanknotesMaxCount;							// �Ƿ�ֽ�ҽ��������ﵽ���ֵ
		bool isCoinMaxCount;								// �Ƿ�Ӳ�ҽ��������ﵽ���ֵ
		//bool isRechargeSuccess;								// ��ֵ�Ƿ�ɹ�
		bool isFinishedJob;									// �Ƿ����ǰ̨ҵ��
		COIN_ACCEPT_STATE coinAcceptState; 					// Ӳ������״̬
		BANKNOTE_ACCEPT_STATE banknoteAcceptState; 			// ֽ������״̬
		COIN_CHANGE_STATE coinChangeState; 					// Ӳ������״̬
		BANKNOTE_CHANGE_STATE banknoteChangeState; 			// ֽ������״̬
		PAYMENT_STATE  paymentState;						// ֧��״̬�����п���EP֧��ʱ�ã� 
		FAILURE_PRINT_STATE failurePrintState;				// ���ϵ���ӡ״̬
		map<TRANSACTION_EXCEPTION,long> mapException;		// ���Ͻ�������
		vector<CSysException>	m_vecException;				// �쳣��¼

		_tagOperationInfo () {
			finishType				= DEFAULT;
			readCardCount			= 0;
			isCoinSlotOpen			= false;
			isBankNoteSlotOpen		= false;
			isBankCardSlotOpen		= false;
			isWaitingEPCard			= false;
			isFinishedReceveMoney	= false;
			//isFailedIssueCard		= false;
			//isSuccessIssueCard		= false;
			isBeginBuyProduct		= false;
			isCoinMaxCount			= false;
			isBanknotesMaxCount		= false;
			//isRechargeSuccess		= false;
			isFinishedJob			= false;
			coinAcceptState = COIN_ACCEPT_STATE_NONE;
			banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;
			coinChangeState = COIN_CHANGE_STATE_NONE;
			banknoteChangeState = BANKNOTE_CHANGE_STATE_NONE;
			paymentState = PAYMENT_STATE_NONE;
			failurePrintState = FAILURE_PRINT_STATE_NONE;
			mapException.clear();
			m_vecException.clear();
		}
	} OPERATION_INFO;

	// �������ȼ��ṹ����
	typedef struct _change_type_value_{
		MONEY_BOX_TYPE	BoxType;			// ��������
		DWORD			dwValue;			// ���
		long			nCurCount;			// �����ӵĵ�ǰ����
		long			nMoneyCount;		// ���������������
		_change_type_value_(){
			BoxType = UNDEFINE_BOX;
			dwValue = 0;
			nCurCount = 0;
			nMoneyCount = 0;
		}

		_change_type_value_(MONEY_BOX_TYPE BoxType, DWORD dwValue, long nCurCount, long nMoneyCount){
			this->BoxType = BoxType;
			this->dwValue = dwValue;
			this->nCurCount = nCurCount;
			this->nMoneyCount = nMoneyCount;
		}

		bool operator<(const _change_type_value_& other){
			return BoxType<=other.BoxType;
		}

	}CHANGE_VALUE;

	struct CommonModel
	{
		_DATE_TIME transTime;							// ����ʱ��
		DWORD dwUDSN;         							// �ױʽ���UDSN
		DWORD dwTransactionID;							// AFC����
		MEDIUM_INFO mediumInfo;							// Ʊ����Ϣ
		MEDIUM_INFO mediumCardInfo;						// Ʊ����Ϣ(���ڴ洢��ֵ����Ʊҵ���д�ֵ����Ϣ��
		PRODUCT_INFO productCardInfo;					// ��Ʒ��Ϣ(���ڴ洢��ֵ����Ʊҵ���д�ֵ����Ʒ��Ϣ��
		TicketInfo* actrualTicketInfo;					// ��Ʊ��Ϣ�����ڴ洢��ά�빺Ʊҵ���з��۵�Ʊ����Ϣ��
		PRODUCT_INFO productInfo;						// ��Ʒ��Ϣ
		PAYMENT_INFO paymentInfo;						// ֧����Ϣ
		OPERATION_INFO operationInfo;					// ������Ϣ
		bool			isInsertToDatabase;				// �Ƿ��Ѿ������
		long lDeviceStatus;								// Ӳ��ģ��״̬

		// ��ʼ��
		CommonModel(){
			transTime = _date_time();
			dwUDSN = 0;
			dwTransactionID = 0;
			mediumInfo = MEDIUM_INFO();
			productInfo = PRODUCT_INFO();
			mediumCardInfo = MEDIUM_INFO();
			productCardInfo = PRODUCT_INFO();
			actrualTicketInfo = NULL;
			paymentInfo = PAYMENT_INFO();
			operationInfo = OPERATION_INFO();
			isInsertToDatabase = false;
			lDeviceStatus = 0;
		}
	};

	CPassengerForeSvc(SERVICE_ID serviceID, SERVICE_ID parentServiceID = MAIN_SVC);
	virtual ~CPassengerForeSvc();

	CommonModel& GetCommonModel();

	virtual void NotifyDataModelChanged();

	virtual long CalculatePrice();			// ��ȡ��ƷƱ�ۣ�����ֻ������TPU����Ĳ��֡��ر��������ھ���Svc�о���д����
	virtual void StartReceiveMoney();				// ��ʼ��Ǯ
	void AcceptMoney();						// �����ֽ�

	void OnCancel();
	void OnServiceErrorCancel();

protected:

	CommonModel *m_pCommonModel;
	int  m_nReadedCardCount;											// ��ǰ�����������
	bool bIsConfirmReadCard;											// �ж��ǵ�һ�ζ��������ǵڶ���ȷ�϶���
	bool m_bIsPassengerCanceled;										// �жϳ˿��Ƿ���ȡ����ֻ��Ӧһ��
	bool m_bIsFinishedWriteData;										// �ж��Ƿ�д��������ݣ�
	bool m_bNeedClickOkButton;											// �Ƿ���Ҫ�����ť������Ʊ��		
	virtual void OnReadCard(UINT,CRWReadCard*);
	virtual void OnCoinInserted(UINT,CCHReadReceivs*);
	virtual void OnCoinChanged(UINT,CCHChange*);
	virtual void OnCoinReceived(UINT,CCHAccept*);
	virtual void OnCoinReturned(UINT,CCHRefund*);
	virtual void OnBanknoteInserted(UINT,CBRHOpenAndValidate*);
	virtual void OnBanknoteCollect(UINT,CBRHCashInEnd*);
	virtual void OnBanknoteReturned(UINT,CBRHCashInRollback*);
	virtual void OnBanknoteChangeSendOut(UINT,CBRHDispenseRequest*);		// ֽ������ص�����
	//virtual void OnChargeInsertCard(UINT,CIOCardPosMonitor*);				// ��ֵ�˿Ͳ��뿨�ص�����

	// ҵ�������˳������
	virtual long SaveCardInfo(LPRW_READ_CARD_RESPONSE);						// ���泵Ʊ���ݵ�DataModel
	virtual bool ConfirmCard(LPRW_READ_CARD_RESPONSE);						// ȷ�Ͽ���Ϣ
	//void OnSuccessFinish();
	virtual void SaveSCData(); // �о���ҵ����ص�SaveSCData�ھ���ҵ����д����
	virtual void WriteDBData(Data::TransactionRecord &dbInfo); // �о���ҵ����ص�WriteDBData�ھ���ҵ����д����
	virtual void FlowToFinishDlg(); // ��OnSuccessFinishʹ�õ�һ��������װ
	virtual long FinishEPPayment(LPRW_READ_CARD_RESPONSE);
	virtual void FlowToDefaultDlg();					// ת��Ĭ��ҵ��
	// ҵ��������쳣����
	void OnError(CSysException e);											// ������Ӳ��/ϵͳ�׳�
	void OnError(CString errMsg, DWORD dwReceptMsg=GUIDE_CHARGE_FAILURE);	// ������Svc�����׳�
	//void OnErrorFinish();
	bool StartReturnCash();	// �˱�
	void CloseAllDevice();	// �ر�����Ӳ��
	//void OnReturnCashOK();  // �˱ҳɹ�
	void OnReturnFinish();	// ���
	virtual void FlowToErrorDlg(); // ��OnErrorFinish��OnCancel�͸��ֳ���ʹ�õ�һ��������װ

	virtual void PrinterPrintReceipt();
	virtual void PrinterPrintErrorReceipt();

	bool IsReceiveEnoughMoney();								// �Ƿ����֧��

	// ֽ�ҽ�����غ�����֮���Է��ھ���Svc�㣬��Ϊ��theAFC_TVM_OPERATIONȡ����ֽ�����ƣ�����Ʊ�����㲻ͬ
	virtual bool IsBothCoinBanknoteMaxCount();
	virtual bool IsBanknoteMaxCount();
	virtual bool IsBanknoteMaxAmount();
	virtual bool IsCoinMaxCount();
	virtual bool IsCoinChageMaxCount();									// ���������Ƿ��ޣ�
	virtual bool IsBHChangeMaxCount();									// ֽ�����������Ƿ��ޣ�
	virtual bool IsBHChangeMaxAmout();									// ֽ���������Ƿ��ޣ�

	virtual COIN_ACCEPTABLE GetAcceptCoinFaceTable() = 0;				// ��ȡ�ɽ��յ�Ӳ�ҵ����
	virtual BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList* table = NULL) = 0;			// ��ȡ�ɽ��յ�ֽ�ҵ����

	// �˵���֧����ť
	// ��ʱ����������Ҫ�õ�
	virtual void OnTimer(UINT nEventID);								// ������,�����е������еļ�ʱ����Ӧ����
	virtual void UpdateReceptDlg(CString strTxt){}						// ���³˿ͽ��浹��ʱ��ʾ
public:
	void ResetMoneyNeedTimer();											// ���ý���ֽ�һ�Ӳ�Ҷ�ʱ��

	void EndRecvTimer();													// ֹͣ�����еļ�ʱ��������人ҵ��������
	void InsertRecvStartTimer();											// ������Ʊǰ���ؼ�ʱ��������人ҵ��������
private:

	//?? ������ЩӲ������������δ����÷���ֵ����void��Ŀǰ���У���Ϊ��TVMForeServiceì�ܣ���
	//���Ӳ����Щ���ش�����룬��Щû�С���ø��ݴ���������onError���ɡ�
	long BlockCard();					// ����������
	void StartReceiveCoin();			// ��ʼ�����ֽ�ֽ�ң�Ӳ�ң�
	void StartReceiveBanknote();
	void StartPayUsingEPCard();			// ��ʼ�ô�ֵ��֧��
	void OnAcceptMoneyOK();				// Ͷ�ҽ���㹻��OnCoinInserted��OnBanknoteInserted��OnBankcardInserted����EP��֧���ɹ��ĺ������á�
	void StartAcceptCash();				// �ؿں�ѹ��
	void OnReceiveMoneyOK();			// ѹ���󣬱�OnCoinReceived��OnBanknoteCollect����
	//bool StartChange();					// ���� //!! �����Ҫ��������ֵ����Ϊ�ɹ���ҪȥOnSuccessfulFinish

	// ֣���¼ӽӿڣ�ֽ�����㼰Ӳ������ֿ�
	void StartCHChange();				// ��ʼӲ�����㣬��Ҫ�ȴ�Ӳ��ѹ�����������ܹ�����
	void StartBHChange();				// ��ʼֽ�����㣬������������Ӳ�������������Լ�����������


	void RenewCoinReceivedAmount(tCHACoinValueInfo chAmountInfo);				// ����Ӳ�ҽ��ܽ����Ϣ
	void RenewBanknoteReceivedAmount(tBncCashInfo& bh_status);			// ����ֽ�ҽ��ܽ����Ϣ
	void RenewCoinReceivedCount(tCHACoinValueInfo chAmountInfo);					// ����Ӳ�ҽ��ܸ�����Ϣ
	void RenewBanknoteReceivedCount(tBncCashInfo& bh_status);			// ����ֽ�ҽ��ܸ�����Ϣ
	bool CalculateChangeCount();											// ��changeAmount����changeCount
	void EndInsertTimer();													// ֹͣ�����еļ�ʱ��
	void InsertMoneyNeedStartTimer();										// ����ͶǮ���������ؼ�ʱ��
	void ReturnWhenCoinInserted();											// ��Ӳ��Ͷ���㹻����Ϊ�ﵽҪ��ʱ���˻����յ�Ӳ�Ҽ�ֽ�ҡ�
	void ReturnWhenBanknotesInserted();										// ��ֽ��Ͷ���������ܶ��㹻��ʱ���˻�����ֽ�Ҽ�Ӳ�ҡ�

	string GetFailedReason();

private:
	int m_timerInterval;


// �人11���ߣ��޸���ӣ�
public:
	// �������ļ�ȡƱ���б�
	vector<int> GetTicketAmountTable();
	vector<int> m_vecTicketAmount;
	// �������ļ�ȡ��Ʊ�����б�
	vector<int> GetTicketCountTable();
	vector<int> m_vecTicketCount;
	// ѭ���������
	CString m_CardLogicalNumber;// ��ֵҵ����ζ����ĺϷ�����
	bool m_bReaderBad;			// �Ƿ��д�������쳣
	CString m_strReadCardRsp;	// ����������ʾ��Ϣ
	static DWORD WINAPI DoCircleReadCard(LPVOID lpParameter);			//����ѭ�������߳�  
	long StartCircleReadCard();
	long StopCircleReadCard();
	virtual long EmptyCardInfo();
	long StopReceiveMoney();

	// ��ά�빺Ʊ���
	void EndQueryOrderTimer();											// ֹͣ������ѯ��ʱ��
	void InsertQueryOrderTimer();										// ����������ѯ��ʱ��

	long StartWebThread();
	static DWORD WINAPI WebCommunicateFunction(LPVOID lpParameter);
	void RequestOrder();
	void QueryOrder();
	void ConfirmOrder();
	void CancelOrder();

	// Ӳ�ҽ������
	bool m_bCoinHopperAccept;

	void OnErrorFinish();
	void OnSuccessFinish();

	volatile bool m_bStopCircleRead;
	volatile bool m_bCircleReading;
	RW_READ_CARD_RESPONSE m_readResponse;
	volatile bool m_bRechargeConfirm;
	volatile bool m_bRechargeOverlimit;
private:
	bool m_bExitWebThread;
	HANDLE m_hWebEvent;
	int m_nCurWebOper;
	bool m_bWebPaySuccess;
public:
	typedef enum enum_pay_type{
		PAY_WITH_NORMAL		= 1,	// ����֧����ʽ���ֽ�
		PAY_WITH_SVT		= 2,	// ��ֵƱ֧����Ʊ
		PAY_WITH_WEIXIN		= 3,	// ΢��֧��
		PAY_WITH_ZHIFUBAO	= 4,	// ֧����֧��
	}PAY_TYPE;

	int m_ePayType;											// ��Ʊ��ʽ
};
