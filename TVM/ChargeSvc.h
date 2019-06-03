#pragma once

#include "PassengerForeSvc.h"
#include "ReceptionStaticInfo.h"

static const TCHAR* const WM_WAIT_TO_INIT = _T("WM_WAIT_TO_INIT");
static const TCHAR* const WM_WAIT_TO_ERROR = _T("WM_WAIT_TO_ERROR");
static const TCHAR* const WM_INIT_TO_WAIT = _T("WM_INIT_TO_WAIT");
static const TCHAR* const WM_INIT_TO_FINISH = _T("WM_INIT_TO_FINISH");
static const TCHAR* const WM_INIT_TO_ERROR = _T("WM_INIT_TO_ERROR");

/**
@brief      ��ֵҵ��Service
*/
class CChargeSvc : public CPassengerForeSvc
{
     DECLARE_MESSAGE_MAP()

public:

	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT = 1,
		DIALOG_GROUP_INIT,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR
	} DIALOG_GROUP;

	//����״̬
	typedef enum _readcard_info
	{
		NONE_READ = 0,
		NOT_COMPARE,
		COMPARE,
		ABORD,			// ��ֹ����
	}READCARD;

	// �ɳ�ֵ�Ĳ�Ʒ����
	typedef enum _available_recharge_type{
		TYPE_NULL = 0,
		TYPE_SVT  = 1,
		TYPE_MST  = 2,
		TYPE_PST  = 4,
	}AVAILABLE_RECHARGE; 

	// SVC����ģ��
	struct DataModel : public CommonModel
	{
		// �ڳ�ֵģ���У���Ʒ���Ͷ�Ӧ�Ĳ�Ʒ��Ϣ���ڴˡ�Ʊ���е�ʣ��������PRODUCT_INFO��lBalanceAmount��lAmount��
		// PRODUCT_INFO��lValidatePeriod����û�ж�Ӧ��Ϣ��Ʊ����������ʹ�á�
		long lAddAmount;					// SVT��ֵ�����Ӷ�ȣ�PST��ʹ��
		long lAddRides;						// MST��ֵ�����Ӵ�����PST��ʹ��
		Duration_t lValidatePeriod;			// PSTʱ��
		

		vector<PRODUCT_INFO> vecTicketProduct;	// ��Ʊ�ڿɳ�ֵ�Ӳ�Ʒ��¼��
		PRODUCT_INFO oldProductInfo;			// ����ֵ��Ʒ��ֵǰ״̬
		unsigned long lAvailableRechargeProduct;// ���ڿɳ�ֵ������
		map<CString,bool> mapAvailableProduct;// ���ڿɳ�ֵ�������Ӳ�Ʒmap<����Ʒ���ͳ��Բ�Ʒ������,true>�����Ӳ�Ʒ���Ͳ�����10
	};

	CChargeSvc();
	virtual ~CChargeSvc();

	DataModel& GetDataModel();

	bool GetCurrentProductInfo(WORD productType, WORD productSubType, PRODUCT_INFO& ticketProduct); // ��ȡ���ڲ�Ʒ��Ϣ

	// ҵ�����˳������
	long CalculatePrice();							// ��ȡ��ƷƱ��	
	long DoCardCharge();							// TPU��ֵ����(��ֵ/����/�ƴ�)
	bool ConfirmCard(LPRW_READ_CARD_RESPONSE);		// ȷ�ϵڶ���ˢ���Ƿ��ǸղŵĿ�
	void PrinterPrintReceipt();
	CString GetAddProductTime(Duration_t date);     // ��Ӧת����ֵ������30��
	
//	bool POSPayment();												// ���п�֧��
//	bool POSCancle();												// ���п�����
//	bool ChargeCard();												// ��ֵ����
	bool IsBanknoteMaxCount();
	bool IsCoinMaxCount();
	void PlayAudioInDlg(DWORD eventID);								// �ڽ����в�������
	COIN_ACCEPTABLE GetAcceptCoinFaceTable();						// ��ȡ�������Ӳ�������Ϣ
	BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList* table = NULL);					// ��ȡ�������ֽ�������Ϣ
	void ReturnFinishDlg();											// ������ɽ���
	CString GetCurrentUnitOfProduct();

	void NotifyDataModelChanged();
	void UpdateReceptDlg(CString strTxt);						// ���³˿ͽ��浹��ʱ��ʾ

	void FlowToFinishDlg();								
	void FlowToErrorDlg();								
	void FlowToDefaultDlg();

	virtual void OnError(CSysException e);
protected:

	// �������Ĺ��캯���У���*m_pDataModel��PassengerForeSvc���*m_pCommonModelָ��ͬһ��struct��
	// ���������������methodʱ��ʹ��*m_pDataModel���̳еĸ���struct������������ӡ�
	// ��ʹ��PassengerForeSvc���methodʱ��ͨ��*m_pCommonModel���Կɲ���DataModel��CommonModel�Ĳ��֡�
	DataModel *m_pDataModel;

	// ҵ�����˳������
	long SaveCardInfo(LPRW_READ_CARD_RESPONSE); // ���泵Ʊ���ݵ�DataModel
	void SaveSCData();
	void WriteDBData(Data::TransactionRecord &dbInfo);
	//void FlowToFinishDlg(); // ��OnSuccessFinishʹ�õ�һ��������װ
	//void FlowToErrorDlg(); // ��OnErrorFinish��OnCancel�͸��ֳ���ʹ�õ�һ��������װ

	//void OnChargeInsertCard(UINT,CIOCardPosMonitor*);					// �˿Ͳ��뿨��Ļص�����

	// ҵ��������쳣����
	void PrinterPrintErrorReceipt();

protected:
	virtual void OnStart();
	
private:

	// ֽ�ҽ�����غ�����֮���Է��ھ���Svc�㣬��Ϊ��theAFC_TVM_OPERATIONȡ����ֽ�����ƣ�����Ʊ�����㲻ͬ
	bool IsBothCoinBanknoteMaxCount();	
	bool IsBanknoteMaxAmount();
	bool IsCoinChageMaxCount();										// �ж������Ƿ�ﵽ���������
	// ֽ�����������ж�
	bool IsBHChangeMaxCount();
	bool IsBHChangeMaxAmout();

	READCARD  m_enumIsReadedCard;									// �ж��Ƿ������
	
	// ä�˹�Ʊ����
	void OnTimer(UINT timerID);										// ��ʱ��
	CChargeAudioHelper* m_pChargeAudioHelper;						// ä�˹�Ʊ���Բ���ָ��
	void	PlayEventAudio(DWORD eventID,bool bIsStopRepeat = false);	// ��װ���Բ����ӿڣ�������ʱ����
	//void	OnPinPadPressed(UINT,CTVMPinPadStartInput*);				// ������̻ص�����
//	void    TurnOnPinPadAndTimer(bool bIsStartPinPad = true,bool bIsStartOutOfTimer = true);	// ����������̼�ؼ�������ʱ��ʱ��
								// �ڽ����в�������
	DWORD m_dwLastAudio;											// ��һ�䲥�ŵ�����ID
	BLIND_OPERATION_STATUS m_operation_status;
	bool    m_bIsPinPadStoped;										// ��������Ƿ�ر�
	vector<int> m_vPassengerInput;									// �˿͵���������
	map<int,DWORD> m_KeyValue;										// ��ǰ��Ӧ���ļ�ֵ��Ӧֵ
	int m_VoicePlayTime;

	void InitDataModel();

public:
	long EmptyCardInfo();
};
