#pragma once
#include "TVMForeService.h"
#include <stack>

class CInstallCoinChangeBoxSvc : public CTVMForeService
{

	DECLARE_MESSAGE_MAP()

public:
	CInstallCoinChangeBoxSvc(void);
	~CInstallCoinChangeBoxSvc(void);
	// ������
	typedef enum _dialog_group
	{
		DIALOG_GROUP_ATTACH_COLLECTION_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _change_box_operation_type_{
		INSTALL_CHANGE_BOX		= 1,// ��װ����
		UNINSTALL_CHANGE_BOX	= 2,// ж�ز���
	}CHANGE_BOX_OPERATION_TYPE;

	typedef enum _current_phase						// ��ǰ����������״̬
	{
		PHASE_CHOOSE_BOX,							// ѡ������
		PHASE_SET_ID,								// ����ID
		PHASE_SET_COUNT,							// ��������
	}CURRENT_PHASE;

	typedef enum _box_select
	{
		UNSELECTED	=  0,							// δѡ��
		CHANGE_BOX_A,								// Ӳ��������A
		CHANGE_BOX_B,								// Ӳ��������B
		CHANGE_ALL_BOX,								// ��������
	}BOX_SELECT;


	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// Ӳ�һ����䰲װ��������					
		SecondFrame									// Ӳ�һ����䰲װ��ɽ���	
	}FrameFlow;

	typedef enum _coin_collection_box
	{
		coin_collection_boxA = 1,
		coin_collection_boxB
	};
	typedef enum _box_state
	{
		STATE_UNKNOW	=	0,
		STATE_INSTALLED,							// �Ѱ�װ
		STATE_NEED_TO_INSTALL,						// ����װ
		STATE_DETATCH								// ��ж��

	}BOX_STATE;

	typedef struct _tagModel
	{
		CString CoinCollectionBoxAStatus;			// Ӳ�һ�����A��װ״̬
		CString CoinCollectionBoxBStatus;			// Ӳ�һ�����B��װ״̬
		BOX_STATE	stateA;							// ������A��״̬
		BOX_STATE	stateB;							// ������B��״̬

		CString	strAChangeBoxExID;					// ��װǰ��ID
		CString	strBChangeBoxExID;

		CString nChangeABoxExCount;					// ��װǰ����
		CString nChangeBBoxExCount;

		CString CoinBoxABeforeInstall;				// Ӳ�һ�����A��װ֮ǰ��״̬
		CString CoinBoxBBeforeInstall;				// Ӳ�һ�����B��װ֮ǰ��״̬
		CString CoinChangeBoxAID;					// Ӳ�һ�����AID
		CString CoinChangeBoxBID;					// Ӳ�һ�����BID
		CString CoinCollctionBoxAfterAttachAID;		// Ӳ�һ�����A��װ��ID
		CString CoinCollctionBoxAfterAttachBID;		// Ӳ�һ�����B��װ��ID
		CString CoinCollectionAfterBoxAStatus;		// Ӳ�һ�����A��װ��״̬
		CString CoinCollectionAfterBoxBStatus;		// Ӳ�һ�����B��װ��״̬
		CString strCoinACount;						// Ӳ��������A��Ӳ������
		CString strCoinBCount;						// Ӳ��������B��Ӳ������
		CString strCoinACountAfter;					// Ӳ��������A��װ�������
		CString strCoinBCountAfter;					// Ӳ��������B��װ�������
		int AttachCoinCollectionBoxFlowTime;
		BOOL IsHasException;
		bool bIsInstallCollectionA;					
		bool bIsInstallCollectionB;
		BOOL IsPrintFalied;
		CString		strPreID;						// ����ID�̶�����
		_tagModel()
		{
			strAChangeBoxExID = _T("");
			strBChangeBoxExID = _T("");
			nChangeABoxExCount = _T("");
			nChangeBBoxExCount = _T("");

			stateA = STATE_UNKNOW;
			stateB = STATE_UNKNOW;
			CoinCollectionBoxAStatus = _T("");
			CoinCollectionBoxBStatus = _T("");
			CoinBoxABeforeInstall = _T("");
			CoinBoxBBeforeInstall = _T("");
			CoinChangeBoxAID = _T("");
			CoinChangeBoxBID = _T("");
			CoinCollctionBoxAfterAttachAID = _T("");
			CoinCollctionBoxAfterAttachBID = _T("");
			CoinCollectionAfterBoxAStatus = _T("");
			CoinCollectionAfterBoxBStatus = _T("");
			AttachCoinCollectionBoxFlowTime = 1;
			IsHasException = FALSE;
			IsPrintFalied  = FALSE;
			bIsInstallCollectionA = false;
			bIsInstallCollectionB = false;
			strCoinACount = _T("");
			strCoinACountAfter = _T("");
			strCoinBCount = _T("");
			strCoinBCountAfter = _T("");
			strPreID.Format(_T("%02d50"),theMAINTENANCE_INFO.GetLineCode());
		}

		CString strGetState(const BOX_STATE& state){
			CString strState = _T("--");
			switch(state){
			case STATE_INSTALLED: strState = _T("�Ѱ�װ");break;
			case STATE_NEED_TO_INSTALL: strState = _T("����װ"); break;
			case STATE_DETATCH: strState = _T("��ж��"); break;
			default: break;
			}
			return strState;
		}

	}Model;

	Model& GetDataModel();

	void NotifyDataModelChanged();

	void InitializeDataModel();													// ��ʼ������ģ��
	void DoAttachCollectionHandleData();										// ����װӲ�һ���������
	CString GetHopperCoinType(int hopetype);									// Ӳ������ֵת��
	void Sendcables(int boxnumber);												// �ϴ�����
	void PrintCoinBoxExchangeReceipt();											// Ӳ�һ����䰲װƾ֤��ӡ

protected:
	void OnStart();																// Service��������
private:
	BOOL IsBoxAInstalled;														// Ӳ�һ�����A�Ƿ��Ѱ�װ��־
	BOOL IsBoxBInstalled;														// Ӳ�һ�����B�Ƿ��Ѱ�װ��־
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );									// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF3(WPARAM , LPARAM );										// ���� [F3/Ctrl] ��������Ϣ�����أ�
	LRESULT OnKeyboardF2(WPARAM,LPARAM);										// ���� [F2/Alt] �����µ���Ϣ


	Model m_DataModel;
	CURRENT_PHASE m_phase;														// ��ǰ�����Ĳ���״̬
	BOX_SELECT	m_selected;														// ���ڱ�����������
	int			m_times;														// �ڼ���������������
	BOX_SELECT	m_selectedBox[2];
	void SetCountsAndState(BOX_SELECT forBox, CString strCounts);						// ��������
	bool isBoxCouldBeInstalled(BOX_SELECT box);									// �����Ƿ�ɰ�װ
	bool SetCoinChangeBoxCountInfo(BOX_SELECT box);
	stack<BOX_SELECT> m_opBox;													// �洢����װ��Ӳ����
	inline void InitStack(){ while (!m_opBox.empty()){m_opBox.pop();}}			// ��ʼ������װӲ��������
	inline char GetBoxLetter(BOX_SELECT& box){									// ȡ���ӱ��
		if (box == CHANGE_BOX_A){
			return 'A'; 
		}else if (box == CHANGE_BOX_B){
			return 'B'; 
		}else
			return ' ';
	}

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;

	void InsertCoinBoxToDatabase(int nBoxNumber);								// ��װ���������
	BankNoteAndCoinType_t TransforCoinType(int nCoinType);						// ת��Ӳ�Ҷ�Ӧ����

	bool CheckInputBoxID(BOX_SELECT boxType,int strBoxId);		// ��������ID�Ƿ��ظ�
};

