#pragma once
#include "TVMForeService.h"
#include <stack>

class CInstallCoinChangeBoxSvc : public CTVMForeService
{

	DECLARE_MESSAGE_MAP()

public:
	CInstallCoinChangeBoxSvc(void);
	~CInstallCoinChangeBoxSvc(void);
	// 画面组
	typedef enum _dialog_group
	{
		DIALOG_GROUP_ATTACH_COLLECTION_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _change_box_operation_type_{
		INSTALL_CHANGE_BOX		= 1,// 安装操作
		UNINSTALL_CHANGE_BOX	= 2,// 卸载操作
	}CHANGE_BOX_OPERATION_TYPE;

	typedef enum _current_phase						// 当前所处操作的状态
	{
		PHASE_CHOOSE_BOX,							// 选择箱子
		PHASE_SET_ID,								// 输入ID
		PHASE_SET_COUNT,							// 设置数量
	}CURRENT_PHASE;

	typedef enum _box_select
	{
		UNSELECTED	=  0,							// 未选择
		CHANGE_BOX_A,								// 硬币找零箱A
		CHANGE_BOX_B,								// 硬币找零箱B
		CHANGE_ALL_BOX,								// 所有箱子
	}BOX_SELECT;


	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// 硬币回收箱安装基本界面					
		SecondFrame									// 硬币回收箱安装完成界面	
	}FrameFlow;

	typedef enum _coin_collection_box
	{
		coin_collection_boxA = 1,
		coin_collection_boxB
	};
	typedef enum _box_state
	{
		STATE_UNKNOW	=	0,
		STATE_INSTALLED,							// 已安装
		STATE_NEED_TO_INSTALL,						// 待安装
		STATE_DETATCH								// 已卸载

	}BOX_STATE;

	typedef struct _tagModel
	{
		CString CoinCollectionBoxAStatus;			// 硬币回收箱A安装状态
		CString CoinCollectionBoxBStatus;			// 硬币回收箱B安装状态
		BOX_STATE	stateA;							// 找零箱A的状态
		BOX_STATE	stateB;							// 找零箱B的状态

		CString	strAChangeBoxExID;					// 安装前的ID
		CString	strBChangeBoxExID;

		CString nChangeABoxExCount;					// 安装前数量
		CString nChangeBBoxExCount;

		CString CoinBoxABeforeInstall;				// 硬币回收箱A安装之前的状态
		CString CoinBoxBBeforeInstall;				// 硬币回收箱B安装之前的状态
		CString CoinChangeBoxAID;					// 硬币回收箱AID
		CString CoinChangeBoxBID;					// 硬币回收箱BID
		CString CoinCollctionBoxAfterAttachAID;		// 硬币回收箱A安装后ID
		CString CoinCollctionBoxAfterAttachBID;		// 硬币回收箱B安装后ID
		CString CoinCollectionAfterBoxAStatus;		// 硬币回收箱A安装后状态
		CString CoinCollectionAfterBoxBStatus;		// 硬币回收箱B安装后状态
		CString strCoinACount;						// 硬币找零箱A的硬币数量
		CString strCoinBCount;						// 硬币找零箱B的硬币数量
		CString strCoinACountAfter;					// 硬币找零箱A安装后的数量
		CString strCoinBCountAfter;					// 硬币找零箱B安装后的数量
		int AttachCoinCollectionBoxFlowTime;
		BOOL IsHasException;
		bool bIsInstallCollectionA;					
		bool bIsInstallCollectionB;
		BOOL IsPrintFalied;
		CString		strPreID;						// 箱子ID固定部分
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
			case STATE_INSTALLED: strState = _T("已安装");break;
			case STATE_NEED_TO_INSTALL: strState = _T("待安装"); break;
			case STATE_DETATCH: strState = _T("已卸载"); break;
			default: break;
			}
			return strState;
		}

	}Model;

	Model& GetDataModel();

	void NotifyDataModelChanged();

	void InitializeDataModel();													// 初始化数据模型
	void DoAttachCollectionHandleData();										// 处理安装硬币回收箱数据
	CString GetHopperCoinType(int hopetype);									// 硬币箱面值转换
	void Sendcables(int boxnumber);												// 上传电文
	void PrintCoinBoxExchangeReceipt();											// 硬币回收箱安装凭证打印

protected:
	void OnStart();																// Service启动后处理
private:
	BOOL IsBoxAInstalled;														// 硬币回收箱A是否已安装标志
	BOOL IsBoxBInstalled;														// 硬币回收箱B是否已安装标志
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );									// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF3(WPARAM , LPARAM );										// 处理 [F3/Ctrl] 键按下消息（返回）
	LRESULT OnKeyboardF2(WPARAM,LPARAM);										// 处理 [F2/Alt] 键按下的消息


	Model m_DataModel;
	CURRENT_PHASE m_phase;														// 当前所处的操作状态
	BOX_SELECT	m_selected;														// 正在被操作的箱子
	int			m_times;														// 第几次设置箱子数量
	BOX_SELECT	m_selectedBox[2];
	void SetCountsAndState(BOX_SELECT forBox, CString strCounts);						// 设置数量
	bool isBoxCouldBeInstalled(BOX_SELECT box);									// 箱子是否可安装
	bool SetCoinChangeBoxCountInfo(BOX_SELECT box);
	stack<BOX_SELECT> m_opBox;													// 存储待安装的硬币箱
	inline void InitStack(){ while (!m_opBox.empty()){m_opBox.pop();}}			// 初始化待安装硬币箱容器
	inline char GetBoxLetter(BOX_SELECT& box){									// 取箱子编号
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

	void InsertCoinBoxToDatabase(int nBoxNumber);								// 安装找零箱入库
	BankNoteAndCoinType_t TransforCoinType(int nCoinType);						// 转换硬币对应代码

	bool CheckInputBoxID(BOX_SELECT boxType,int strBoxId);		// 检查输入的ID是否重复
};

