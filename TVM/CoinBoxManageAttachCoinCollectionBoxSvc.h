#pragma once
#include "TVMForeService.h"
#include <stack>
using namespace std;
/**
@brief 硬币回收箱安装业务
*/
class CCoinBoxManageAttachCoinCollectionBoxSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageAttachCoinCollectionBoxSvc();
	virtual ~CCoinBoxManageAttachCoinCollectionBoxSvc();

	// 画面组
	typedef enum _dialog_group
	{
		DIALOG_GROUP_ATTACH_COLLECTION_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// 硬币回收箱安装基本界面					
		SecondFrame									// 硬币回收箱安装完成界面	
	}FrameFlow;

	typedef enum _coin_collection_box
	{
		coin_collection_boxA = 1,			// 回收箱A
		coin_collection_boxB,				// 回收箱B
		coin_collection_all,				// 所有回收箱
	}COLLECTION_BOX_INDEX;
	typedef enum _current_phase
	{
		PHASE_SELECT_BOX	= 1,			// 选择箱子阶段
		PHASE_SET_BOXID		= 2,			// 输入ID阶段
	}CURRENT_PHASE;
	
	typedef enum _box_state
	{
		ST_REMOVED		=0,					// 已卸载
		ST_INSTALLED,						// 已安装
		ST_PRE_INSTALL,						// 待安装
	}BOX_STATE;

	typedef struct _tagModel
	{
		CString CoinCollectionBoxAID;				// 硬币回收箱AID
		CString CoinCollectionBoxBID;				// 硬币回收箱BID

		CString strColBoxAExID;						// 安装之前的ID
		CString strColBoxBExID;

		int		nColBoxAExCount;					// 安装之前的数量
		int		nColBoxBExCount;

		BOX_STATE stateA;							// 硬币回收箱A的状态
		BOX_STATE stateB;							// 硬币回收箱B的状态

		CString CoinCollectionBoxAStatus;			// 硬币回收箱A安装状态
		CString CoinCollectionBoxBStatus;			// 硬币回收箱B安装状态
		CString CoinBoxABeforeInstall;				// 硬币回收箱A安装之前的状态
		CString CoinBoxBBeforeInstall;				// 硬币回收箱B安装之前的状态
		CString CoinCollctionBoxAfterAttachAID;		// 硬币回收箱A安装后ID
		CString CoinCollctionBoxAfterAttachBID;		// 硬币回收箱B安装后ID
		CString CoinCollectionAfterBoxAStatus;		// 硬币回收箱A安装后状态
		CString CoinCollectionAfterBoxBStatus;		// 硬币回收箱B安装后状态
		int AttachCoinCollectionBoxFlowTime;
		BOOL IsHasException;
		bool bIsInstallCollectionA;					// 安装回收箱A
		bool bIsInstallCollectionB;					// 安装回收箱B
		BOOL IsPrintFalied;

		CString strPreID;							// 箱子固定ID部分
		_tagModel()
		{
			strColBoxAExID = _T("");
			strColBoxBExID = _T("");
			nColBoxAExCount = 0;
			nColBoxBExCount = 0;

			stateA = ST_REMOVED;
			stateB = ST_REMOVED;
			CoinCollectionBoxAStatus = _T("");
			CoinCollectionBoxBStatus = _T("");
			CoinBoxABeforeInstall = _T("");
			CoinBoxBBeforeInstall = _T("");
			CoinCollectionBoxAID = _T("");
			CoinCollectionBoxBID = _T("");
			CoinCollctionBoxAfterAttachAID = _T("");
			CoinCollctionBoxAfterAttachBID = _T("");
			CoinCollectionAfterBoxAStatus = _T("");
			CoinCollectionAfterBoxBStatus = _T("");
			AttachCoinCollectionBoxFlowTime = 1;
			IsHasException = FALSE;
			IsPrintFalied  = FALSE;
			bIsInstallCollectionA = false;
			bIsInstallCollectionB = false;
			strPreID.Format(_T("%02d51"),theMAINTENANCE_INFO.GetLineCode());
		}

		CString strGetState(const BOX_STATE& state)		// 取得状态名
		{
			CString strState = _T("--");
			switch(state){
			case ST_INSTALLED: strState = _T("已安装");break;
			case ST_PRE_INSTALL: strState = _T("待安装"); break;
			case ST_REMOVED: strState = _T("已卸载"); break;
			default: break;
			}
			return strState;
		}

		COLORREF GetColor(const BOX_STATE& state)		// 取状态对应颜色
		{
			COLORREF color = BLACK;
			switch(state)
			{
			case ST_INSTALLED:		color = GREEN;		break;
			case ST_REMOVED:		color = RED;		break;
			case ST_PRE_INSTALL:	color = RGB(0,200,0);	break;
			default:				break;
			}
			return color;
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
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
	LRESULT OnKeyboardF2(WPARAM,LPARAM);										// 处理 [F2/Alt] 键按下的消息

	Model m_DataModel;
	CURRENT_PHASE m_phase;														// 当前所处的状态
	bool  IsBoxCouldbeInstalled(COLLECTION_BOX_INDEX index);					// 判断箱子是否可被执行安装操作
	void  SetIdAndStateForBox(COLLECTION_BOX_INDEX index);						// 设置箱子ID 更新状态
	void  InitializeData();														// 数据初始化


	stack<COLLECTION_BOX_INDEX> m_stkOpBox;										// 存储待操作的箱子
	inline void Clear_Stk(){													// 清空栈容器
		while(!m_stkOpBox.empty()){ m_stkOpBox.pop();}
	}

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;

	bool CheckInputBoxID(COLLECTION_BOX_INDEX boxType,int nBoxId);		// 检查输入的ID是否重复
};