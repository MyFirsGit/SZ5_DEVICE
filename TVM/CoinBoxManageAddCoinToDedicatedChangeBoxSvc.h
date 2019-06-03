#pragma once
#include "TVMForeService.h"
#include <map>
using namespace std;

/**
@brief Ӳ�Ҳ���
*/
class CCoinChangeBoxAddingSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinChangeBoxAddingSvc();
	virtual ~CCoinChangeBoxAddingSvc();

	// �����׶ζ���
	typedef enum _operation_status_{
		OP_STATUS_UNKNOW,
		OP_STATUS_READY_ADDING,
		OP_STATUS_RUNING_ADDING,
		OP_STATUS_FINISH_ADDING,
	}OP_STATUS;

	// ������
	typedef enum _dialog_group
	{
		DIALOG_GROUP_DEDICATEDCHANGE_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// Ӳ�Ҳ����������					
		SecondFrame,								// Ӳ�Ҳ����н���
		ThridTFrame									// Ӳ�Ҳ�����ɽ���
	}FrameFlow;

	typedef enum _coin_adding_box_number
	{
		Coin_Adding_Box1 = 1,
		Coin_Adding_Box2 = 2,
		Coin_Adding_Box12 = 12
	}coin_adding_box_number;

	typedef enum _dedicated_change_box_number
	{
		Dedicated_Change_BoxA = 3,
		Dedicated_Change_BoxB = 4
	}dedicated_change_box_number;

	typedef enum _coin_type
	{
		coin_50avos = 1,
		coin_1mop,
		coin_2mop,
		coin_5mop
	}coin_type;

	typedef struct _DedicatedChangeBoxData
	{
		CString DedicatedChangeCoinAID;				// Ӳ��ר��������AID
		CString DedicatedChangeCoinBID;				// Ӳ��ר��������BID
		CString QuantityA;							// Ӳ��ר��������A����
		CString QuantityB;							// Ӳ��ר��������B����
		_DedicatedChangeBoxData()
		{
			DedicatedChangeCoinAID = _T("");
			DedicatedChangeCoinBID = _T("");
			QuantityA = _T("0");
			QuantityB = _T("0");
		}
	}DedicatedChangeBoxData;

	typedef struct _AddingBoxData
	{
		CString AddingBoxAID;						// ������AID
		CString AddingBoxBID;						// ������BID
		CString AddingBoxATypeData;					// ������A��ֵ
		CString AddingBoxBTypeData;					// ������B��ֵ
		CString AddingBoxQuantityA;					// ������A����
		CString AddingBoxQuantityB;					// ������B����
		int AddingBoxFrameFlowTime;					// ������ת����
		_AddingBoxData()
		{
			AddingBoxAID = _T("");
			AddingBoxBID = _T("");
			AddingBoxATypeData = _T("");
			AddingBoxBTypeData = _T("");
			AddingBoxQuantityA = _T("0");
			AddingBoxQuantityB = _T("0");
			AddingBoxFrameFlowTime = -1;
		}
	}AddingBoxData;

	

	typedef struct _DoAddingCoin
	{
		int DoAddingQuantityA;						// ������A�ӱ�����
		int DoAddingQuantityB;						// ������B�ӱ�����
		_DoAddingCoin()
		{
			DoAddingQuantityA = -1;
			DoAddingQuantityB = -1;
		}

	}DoAddingCoin;

	/////////////////////////////////////////////////////////////////////////////
	typedef enum _CHANGE_BOX_INDEX				// Ӳ��������λ��
	{
		CHANGE_BOX_A	= 1,		// A �� 1Ԫ
		CHANGE_BOX_B,				// B �� 5��
	}CHANGE_BOX_INDEX;

	typedef struct change_box_info			   //  Ӳ����������Ϣ�ṹ
	{
		CString strID;			// ���
		int		CurrentCount;   // ��ǰӲ������
		int		AddingCount;	// Ҫ��ӵ�Ӳ������
		int		ExCount;		// ����ǰ������
		BankNoteAndCoinType_t value;	// ���

		change_box_info()
		{
			strID = _T("FFFFFFFF");
			CurrentCount = 0;
			AddingCount = 0;
			ExCount = 0;
			value = VALUE_UNKNOWN;
		}
		// ȡ�������Ϣ
		void GetCoinChangeCountInfo(CTCoinCountInfo::COIN_BOX_INFO/*COIN_CHANGE_BOX_INFO*/& info)
		{
			strID = (info.coinBoxID).ToString();
			CurrentCount = info.ulCurCount;
		}
		// ����Ӳ�Ҳ�����������
		void AddCoin(const int& count)
		{
			AddingCount = count;
		}
	}CHANGE_BOX_INFO;

	typedef enum _current_phase				// �û������׶�
	{
		PHASE_SELECT_BOX	=0,	 // ѡ��Ҫ�����Ӳ����׶�
		PHASE_ADDING_COIN,		 // ���벹�������׶�
	}CURRENT_PHASE;

	typedef struct _tagModel
	{
		map<CHANGE_BOX_INDEX,CHANGE_BOX_INFO> mapBoxInfo;		// �洢����<λ�ã�����>���͵�����
		/////////////////////////////////////////////////
		DedicatedChangeBoxData dedicatedchangeboxdata;
		//AddingBoxInstalledStatus addingboxinstalledstatus;
		AddingBoxData addingboxdata;
		DoAddingCoin doaddingcoin;
		int BoxNumber;
		BOOL IsHasException;
		BOOL IsPrintFailed;
		OP_STATUS	opStatus;
		_tagModel()
		{
			mapBoxInfo.clear();
			///////////////////////////////////////////
			BoxNumber = 12;
			IsHasException = FALSE;
			IsPrintFailed  = FALSE;
			opStatus = OP_STATUS_UNKNOW;
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();
	bool IsShowTips();
													
	CString GetHopperCoinType(int hopetype);									// ��ȡӲ�Ҳ��������
	BYTE GetHopperType(int hopetype);											// ��ȡӲ��ר��������Ӳ������
	//void GetCoinAddingBoxID(int BoxNumber);										// ��ȡӲ�Ҳ�����ID
	void DedicatedChangeHandleData();											// ר�������䴦��ID��������Ϣ
	void DedicatedChangeAddingCoinFrame();										// ר��������ӱҽ��洦��
	void DoAddingCoinToDedicatedChangeBox();									// ר��������ӱҴ���
	void Sendcables(CHANGE_BOX_INDEX boxnumber);												// �ϴ�����
	vector<CASH_TYPE_COUNT> PackageCoinTypeAndCount(int hopetype,int count);	// ������ִ��롢����
	void FinishAddingCoinToDedicatedChangeBox();								// ר��������ӱ���ɽ��洦��
	void PrintCoinBoxExchangeReceipt(bool bBoxA);											// Ӳ��ר��������ӱ�ƾ֤��ӡ
	void InsertDataToDB(CHANGE_BOX_INDEX boxnumber);										// ���ݵ������ݿ�
protected:
	void OnStart();																// Service��������
private:

	int EnterTime;																// Enter�����´���
	int SecondaryMenuNumber;													// ��������˵����
	int AddingBoxNumber;														// �ӱҲ�����Ӳ�����	

	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Clear] ��������Ϣ��ɾ����
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/ );				// ���� [F2/Alt] ��������Ϣ�����棩
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
	LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [ESC] ��������Ϣ�����أ�

	Model m_DataModel;

	CURRENT_PHASE m_phase;
	CHANGE_BOX_INDEX m_index;													// ��¼��ѡ�����������

	void	ToSelectBoxPhase();													// ������һ�׶Σ� ѡ������
	void	ToAddingCoinPhase();												// ������һ�׶Σ� ����Ӳ��
	bool	IsBoxCouldBeAdded(const CHANGE_BOX_INDEX box);						// �ж���ѡֽ�����Ƿ��ܹ�ִ�мӱҲ���
	void	InitializeDataModel();												// ��ʼ������ģ��
	bool	IsTipsShow;
	bool	IsShow;

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;
};


