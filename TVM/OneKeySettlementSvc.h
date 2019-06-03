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
	
	// 每个收机步骤对应的状态
	typedef enum _settlement_state{
		UNSELECTED		=0,				// 不执行
		READY			,				// 待执行
		PROCESSING		,				// 执行中
		DONE			,				// 已执行
		FAIL			,				// 执行失败
	}PROC_STATE;

	typedef enum _switch				// 开启/关闭
	{
		SWT_ON		= 1,				// 开启
		SWT_OFF		= 2,				// 关闭
	}PROC_SWITCH;

	typedef enum _process						// 一键收机执行步骤
	{
		PROC_REMOVE_CARDBOX			= 0x01,		// 卸载token箱（回收缓冲区token）
		PROC_COLLECT_COIN			= 0x02,		// 回收硬币
		PROC_REMOVE_CN_CHGBOX		= 0x04,		// 卸载硬币找零箱
		PROC_REMOVE_CN_COLBOX		= 0x08,		// 卸载硬币回收箱
		PROC_COLLECT_CASH			= 0x10,		// 回收纸币
		PROC_REMOVE_BN_COLBOX		= 0x20,		// 卸载纸币回收箱
		PROC_REMOVE_BN_CHGBOX		= 0x40,		// 卸载纸币找零箱
		PROC_BALANCE				= 0x80,		// 结算

	
		PROC_NO_REMOVE_CARDBOX		= 0x7E,		// 不卸票箱
		PROC_NO_COLLECT_COIN		= 0x7D,		// 不回收硬币
		PROC_NO_REMOVE_CN_CHGBOX	= 0x7B,		// 不卸硬币找零箱
		PROC_NO_REMOVE_CN_COLBOX	= 0x77,		// 不卸载硬币回收箱
		PROC_NO_COLLECT_CASH		= 0xEF,		// 不回收纸币
		PROC_NO_REMOVE_BN_COLBOX	= 0xDF,		// 不卸载纸币回收箱
		PROC_NO_REMOVE_BN_CHGBOX	= 0xBF,		// 不卸载纸币找零箱
		PROC_NO_BALANCE				= 0x7F,		// 不做业结

	}PROCESS;

	// 具体的收机步骤
	typedef struct _settlement_process
	{
		CString			procName;		// 步骤名称
		PROC_STATE		procState;		// 收机操作的各个步骤当前状态
		PROC_SWITCH		procSwitch;		// 开关状态
		_settlement_process()
		{
			procName	= _T("");
			procState	= UNSELECTED;
			procSwitch	= SWT_OFF;
		}

		COLORREF GetStateColor()				// 根据步骤的当前状态显示不同颜色
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

		COLORREF GetSwitchColor()			// 取得开关的颜色
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
	// 数据模型
	typedef struct _tag_data_modol
	{
		map<PROCESS,PROCESS_INFO> mapSettlement;
		
		bool bPreStatus;						// 是否是初始化状态呢？
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

			//proc.procName = _T("卸载硬币回收箱");
			//mapSettlement[PROC_REMOVE_CN_COLBOX] = proc;
			
			proc.procName = _opl(TXT_BANKNOTEMGR_COLLECT_BANKNOTES);
			mapSettlement[PROC_COLLECT_CASH] = proc;

			proc.procName = _opl(TXT_ID_TVM_BANKNOTE_COLLECTION_BOX_UNINSTALL);
			mapSettlement[PROC_REMOVE_BN_COLBOX] = proc;
												 
			proc.procName = _T("卸载纸币找零箱");
			mapSettlement[PROC_REMOVE_BN_CHGBOX] = proc;
											
			proc.procName = _T("结算");
			mapSettlement[PROC_BALANCE] = proc;
		}
		
	}Model;

	Model& GetDataModol();            // 获取数据模型
	void NotifyDataModelChanged();    // 数据改变后，通知窗体更新数据

private:
	Model m_modle;
	int	m_settings;									// 一键收机处理过程
	void OnStart();

	bool CollectToken();								// 回收token
	bool RemoveCardBox();								// 卸载票箱
	bool CollectCoin();									// 回收硬币
	bool CoinCollection();
	bool RemoveCoinChangeAndCollectionBox();
	bool RemoveCoinCollectBox();						// 卸载硬币回收箱
	bool CollectBanknote();								// 回收纸币
	bool RemoveBanknoteCollectBox();					// 卸载纸币回收箱
	bool RemoveBanknoteChangeBox();						// 卸载纸币找零箱
	bool Balance();										// 结算

	bool BalanceOrNot();								// 是否可进行结算
	bool GetState(PROCESS,CString);						// 为制定的操作步骤设置状态
	void GetStates();

	CString m_strCardRemoveTime;						// 读取卡箱操作时间
	CString m_strCoinColletTime;
	bool m_isCardRemovalAttemped;
	bool m_isCoinCollectAttemped;
	
	void PrintCoinBoxExchangeReceipt();					// 硬币箱操作凭条
	void PrintBanknoteChangeReceiption();				// 纸币回收箱操作凭条
	void PrintBanknoteChangeBoxReceiption();			// 纸币找零箱凭证
	void LoadSetting(const int nSettings);				// 收机配置
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

	// 实际回收数量
	long ChangeAColCount;
	long ChangeBColCount;
	long CircleAColCount;
	long CircleBColCount ;

	void InsertOperaterToDatabase(int index,CString strBoxID,int nCount);
};
