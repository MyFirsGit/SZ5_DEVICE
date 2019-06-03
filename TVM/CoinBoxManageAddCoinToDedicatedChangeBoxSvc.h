#pragma once
#include "TVMForeService.h"
#include <map>
using namespace std;

/**
@brief 硬币补充
*/
class CCoinChangeBoxAddingSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinChangeBoxAddingSvc();
	virtual ~CCoinChangeBoxAddingSvc();

	// 操作阶段定义
	typedef enum _operation_status_{
		OP_STATUS_UNKNOW,
		OP_STATUS_READY_ADDING,
		OP_STATUS_RUNING_ADDING,
		OP_STATUS_FINISH_ADDING,
	}OP_STATUS;

	// 画面组
	typedef enum _dialog_group
	{
		DIALOG_GROUP_DEDICATEDCHANGE_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// 硬币补充基本界面					
		SecondFrame,								// 硬币补充中界面
		ThridTFrame									// 硬币补充完成界面
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
		CString DedicatedChangeCoinAID;				// 硬币专用找零箱AID
		CString DedicatedChangeCoinBID;				// 硬币专用找零箱BID
		CString QuantityA;							// 硬币专用找零箱A数量
		CString QuantityB;							// 硬币专用找零箱B数量
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
		CString AddingBoxAID;						// 补充箱AID
		CString AddingBoxBID;						// 补充箱BID
		CString AddingBoxATypeData;					// 补充箱A面值
		CString AddingBoxBTypeData;					// 补充箱B面值
		CString AddingBoxQuantityA;					// 补充箱A数量
		CString AddingBoxQuantityB;					// 补充箱B数量
		int AddingBoxFrameFlowTime;					// 界面流转次数
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
		int DoAddingQuantityA;						// 补充箱A加币数量
		int DoAddingQuantityB;						// 补充箱B加币数量
		_DoAddingCoin()
		{
			DoAddingQuantityA = -1;
			DoAddingQuantityB = -1;
		}

	}DoAddingCoin;

	/////////////////////////////////////////////////////////////////////////////
	typedef enum _CHANGE_BOX_INDEX				// 硬币找零箱位置
	{
		CHANGE_BOX_A	= 1,		// A 箱 1元
		CHANGE_BOX_B,				// B 箱 5角
	}CHANGE_BOX_INDEX;

	typedef struct change_box_info			   //  硬币找零箱信息结构
	{
		CString strID;			// 编号
		int		CurrentCount;   // 当前硬币数量
		int		AddingCount;	// 要添加的硬币数量
		int		ExCount;		// 补充前的数量
		BankNoteAndCoinType_t value;	// 面额

		change_box_info()
		{
			strID = _T("FFFFFFFF");
			CurrentCount = 0;
			AddingCount = 0;
			ExCount = 0;
			value = VALUE_UNKNOWN;
		}
		// 取软计数信息
		void GetCoinChangeCountInfo(CTCoinCountInfo::COIN_BOX_INFO/*COIN_CHANGE_BOX_INFO*/& info)
		{
			strID = (info.coinBoxID).ToString();
			CurrentCount = info.ulCurCount;
		}
		// 补币硬币操作更新数据
		void AddCoin(const int& count)
		{
			AddingCount = count;
		}
	}CHANGE_BOX_INFO;

	typedef enum _current_phase				// 用户操作阶段
	{
		PHASE_SELECT_BOX	=0,	 // 选择要补充的硬币箱阶段
		PHASE_ADDING_COIN,		 // 输入补充数量阶段
	}CURRENT_PHASE;

	typedef struct _tagModel
	{
		map<CHANGE_BOX_INDEX,CHANGE_BOX_INFO> mapBoxInfo;		// 存储箱子<位置，数据>类型的容器
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
													
	CString GetHopperCoinType(int hopetype);									// 获取硬币补充箱币种
	BYTE GetHopperType(int hopetype);											// 获取硬币专用找零箱硬币类型
	//void GetCoinAddingBoxID(int BoxNumber);										// 获取硬币补充箱ID
	void DedicatedChangeHandleData();											// 专用找零箱处理ID、数量信息
	void DedicatedChangeAddingCoinFrame();										// 专用找零箱加币界面处理
	void DoAddingCoinToDedicatedChangeBox();									// 专用找零箱加币处理
	void Sendcables(CHANGE_BOX_INDEX boxnumber);												// 上传电文
	vector<CASH_TYPE_COUNT> PackageCoinTypeAndCount(int hopetype,int count);	// 打包币种代码、数量
	void FinishAddingCoinToDedicatedChangeBox();								// 专用找零箱加币完成界面处理
	void PrintCoinBoxExchangeReceipt(bool bBoxA);											// 硬币专用找零箱加币凭证打印
	void InsertDataToDB(CHANGE_BOX_INDEX boxnumber);										// 数据导入数据库
protected:
	void OnStart();																// Service启动后处理
private:

	int EnterTime;																// Enter键按下次数
	int SecondaryMenuNumber;													// 二级界面菜单编号
	int AddingBoxNumber;														// 加币操作的硬币箱号	

	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Clear] 键按下消息（删除）
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/ );				// 处理 [F2/Alt] 键按下消息（保存）
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
	LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [ESC] 键按下消息（返回）

	Model m_DataModel;

	CURRENT_PHASE m_phase;
	CHANGE_BOX_INDEX m_index;													// 记录已选定的箱子序号

	void	ToSelectBoxPhase();													// 设置下一阶段： 选择箱子
	void	ToAddingCoinPhase();												// 设置下一阶段： 补充硬币
	bool	IsBoxCouldBeAdded(const CHANGE_BOX_INDEX box);						// 判断所选纸币箱是否能够执行加币操作
	void	InitializeDataModel();												// 初始化数据模型
	bool	IsTipsShow;
	bool	IsShow;

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;
};


