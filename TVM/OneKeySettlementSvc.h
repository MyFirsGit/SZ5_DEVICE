#pragma once
#include "tvmforeservice.h"

class COneKeySettlementSvc :
	public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	COneKeySettlementSvc(void);
	~COneKeySettlementSvc(void);

	typedef enum _dialog_group {
		DIALOG_GROUP_REMOVE_EXCEPTION = 1
	} DIALOG_GROUP;
	
	// ÿ���ջ������Ӧ��״̬
	typedef enum _settlement_state{
		UNSELECTED		=0,				// ��ִ��
		READY			,				// ��ִ��
		PROCESSING		,				// ִ����
		DONE			,				// ��ִ��
		FAIL			,				// ִ��ʧ��
	}PROC_STATE;

	typedef enum _switch				// ����/�ر�
	{
		SWT_ON		= 1,				// ����
		SWT_OFF		= 2,				// �ر�
	}PROC_SWITCH;

	typedef enum _process						// һ���ջ�ִ�в���
	{
		PROC_REMOVE_CARDBOX			= 0x01,		// ж��token�䣨���ջ�����token��
		PROC_COLLECT_COIN			= 0x02,		// ����Ӳ��
		PROC_REMOVE_CN_CHGBOX		= 0x04,		// ж��Ӳ��������
		PROC_REMOVE_CN_COLBOX		= 0x08,		// ж��Ӳ�һ�����
		PROC_COLLECT_CASH			= 0x10,		// ����ֽ��
		PROC_REMOVE_BN_COLBOX		= 0x20,		// ж��ֽ�һ�����
		PROC_REMOVE_BN_CHGBOX		= 0x40,		// ж��ֽ��������
		PROC_BALANCE				= 0x80,		// ����

	
		PROC_NO_REMOVE_CARDBOX		= 0x7E,		// ��жƱ��
		PROC_NO_COLLECT_COIN		= 0x7D,		// ������Ӳ��
		PROC_NO_REMOVE_CN_CHGBOX	= 0x7B,		// ��жӲ��������
		PROC_NO_REMOVE_CN_COLBOX	= 0x77,		// ��ж��Ӳ�һ�����
		PROC_NO_COLLECT_CASH		= 0xEF,		// ������ֽ��
		PROC_NO_REMOVE_BN_COLBOX	= 0xDF,		// ��ж��ֽ�һ�����
		PROC_NO_REMOVE_BN_CHGBOX	= 0xBF,		// ��ж��ֽ��������
		PROC_NO_BALANCE				= 0x7F,		// ����ҵ��

	}PROCESS;

	// ������ջ�����
	typedef struct _settlement_process
	{
		CString			procName;		// ��������
		PROC_STATE		procState;		// �ջ������ĸ������赱ǰ״̬
		PROC_SWITCH		procSwitch;		// ����״̬
		_settlement_process()
		{
			procName	= _T("");
			procState	= UNSELECTED;
			procSwitch	= SWT_OFF;
		}

		COLORREF GetStateColor()				// ���ݲ���ĵ�ǰ״̬��ʾ��ͬ��ɫ
		{
			switch(procState)
			{
			case UNSELECTED:
				return BLACK;
			case DONE:		
				return GREEN;
			case FAIL:		
				return RED;
			case PROCESSING:
			case READY:
				return SOFTGREEN;
			default:
				return BLACK;
			}
		}

		COLORREF GetSwitchColor()			// ȡ�ÿ��ص���ɫ
		{
			if (procSwitch == SWT_ON){
				return GREEN;
			}else{
				return RED;
			}
		}

		CString GetSwitchString()
		{
			if (procSwitch == SWT_ON){
				return _opl(DEVICESTATUS_YES);
			}else{
				return _opl(DEVICESTATUS_NO);
			}
		}

		CString GetStateString()
		{
			switch(procState)
			{
			case UNSELECTED:	return _opl(DEVICESTATUS_UNSELECTED);
			case DONE:			return _opl(DEVICESTATUS_DONE);
			case FAIL:			return _opl(DEVICESTATUS_FAIL);
			case PROCESSING:	return _opl(DEVICESTATUS_PROCESSING);
			case READY:			return _opl(DEVICESTATUS_READY);
			default:
				return _T("");
			}
		}

	}PROCESS_INFO;
	// ����ģ��
	typedef struct _tag_data_modol
	{
		map<PROCESS,PROCESS_INFO> mapSettlement;
		
		bool bPreStatus;						// �Ƿ��ǳ�ʼ��״̬�أ�
		_tag_data_modol()
		{
			mapSettlement.empty();
			bPreStatus = false;
		}

		void InitializeMap()
		{
			PROCESS_INFO proc;
			proc.procName = _opl(TXT_SMARTCARDMGR_DETACH);
			mapSettlement[PROC_REMOVE_CARDBOX] = proc;
												 
			proc.procName = _opl(TXT_COINMGR_COLLECT_COIN);
			mapSettlement[PROC_COLLECT_COIN] = proc;
												 
			proc.procName = _opl(TXT_COINMGR_MENU_UNINSTALL_CHANGE_RECOVERY_BOX);
			mapSettlement[PROC_REMOVE_CN_CHGBOX] = proc;

			//proc.procName = _T("ж��Ӳ�һ�����");
			//mapSettlement[PROC_REMOVE_CN_COLBOX] = proc;
			
			proc.procName = _opl(TXT_BANKNOTEMGR_COLLECT_BANKNOTES);
			mapSettlement[PROC_COLLECT_CASH] = proc;

			proc.procName = _opl(TXT_ID_TVM_BANKNOTE_COLLECTION_BOX_UNINSTALL);
			mapSettlement[PROC_REMOVE_BN_COLBOX] = proc;
												 
			proc.procName = _T("ж��ֽ��������");
			mapSettlement[PROC_REMOVE_BN_CHGBOX] = proc;
											
			proc.procName = _T("����");
			mapSettlement[PROC_BALANCE] = proc;
		}
		
	}Model;

	Model& GetDataModol();            // ��ȡ����ģ��
	void NotifyDataModelChanged();    // ���ݸı��֪ͨ�����������

private:
	Model m_modle;
	int	m_settings;									// һ���ջ��������
	void OnStart();

	bool CollectToken();								// ����token
	bool RemoveCardBox();								// ж��Ʊ��
	bool CollectCoin();									// ����Ӳ��
	bool CoinCollection();
	bool RemoveCoinChangeAndCollectionBox();
	bool RemoveCoinCollectBox();						// ж��Ӳ�һ�����
	bool CollectBanknote();								// ����ֽ��
	bool RemoveBanknoteCollectBox();					// ж��ֽ�һ�����
	bool RemoveBanknoteChangeBox();						// ж��ֽ��������
	bool Balance();										// ����

	bool BalanceOrNot();								// �Ƿ�ɽ��н���
	bool GetState(PROCESS,CString);						// Ϊ�ƶ��Ĳ�����������״̬
	void GetStates();

	CString m_strCardRemoveTime;						// ��ȡ�������ʱ��
	CString m_strCoinColletTime;
	bool m_isCardRemovalAttemped;
	bool m_isCoinCollectAttemped;
	
	void PrintCoinBoxExchangeReceipt();					// Ӳ�������ƾ��
	void PrintBanknoteChangeReceiption();				// ֽ�һ��������ƾ��
	void PrintBanknoteChangeBoxReceiption();			// ֽ��������ƾ֤
	void LoadSetting(const int nSettings);				// �ջ�����
	void PrintBanknoteCollectionReception();
	CString FormatAmount(long amount);

	LRESULT OnKeyboardF3(WPARAM , LPARAM );
	LRESULT OnKeyboardF2(WPARAM , LPARAM );
	LRESULT OnKeyboardEnter(WPARAM,LPARAM);
	
	CTCoinCountInfo::COIN_BOX_INFO m_ex_coin_hopperA;	
	CTCoinCountInfo::COIN_BOX_INFO m_ex_coin_hopperB;
	
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO m_ex_coin_changeA;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO m_ex_coin_changeB;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO m_ex_coin_collectA;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO m_ex_coin_collectB;

	CTBanknoteCountInfo::BANKNOTE_BOX_INFO m_ex_note_collect;

	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO m_ex_note_change;
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO m_ex_note_recycleA;
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO m_ex_note_recycleB;
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO m_ex_note_recycleC;
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO m_ex_note_recycleD;

	/*CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO m_ex_note_changeB;
	CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO m_ex_note_invalid;*/

	// ʵ�ʻ�������
	long ChangeAColCount;
	long ChangeBColCount;
	long CircleAColCount;
	long CircleBColCount ;

	void InsertOperaterToDatabase(int index,CString strBoxID,int nCount);
};
