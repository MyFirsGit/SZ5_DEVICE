#pragma once
#include "TVMForeService.h"

#define ALL_COIN_COUNT 0xFFFF

/**
@brief 硬币箱管理业务--硬币回收Service
*/
class CCoinBoxManageCollectCoinSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageCollectCoinSvc();
	virtual ~CCoinBoxManageCollectCoinSvc();

	// 画面组
	typedef enum _dialog_group
	{
		DIALOG_GROUP_COLLECT_COIN = 1,

	} DIALOG_GROUP;

	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// 硬币回收基本界面					
		SecondFrame,								// 硬币回收中界面
		ThridTFrame,								// 硬币回收完成界面
	}FrameFlow;

	typedef enum _coinboxnumber
	{
		coin_dedicated_change_boxA = 1,				// 硬币专用找零箱A
		coin_dedicated_change_boxB,					// 硬币专用找零箱B
		coin_cycle_change_box						// 硬币循环找零箱
	}coinboxnumber;

	typedef enum _coin_type
	{
		coin_50avos = 1,
		coin_1mop,
		coin_2mop,
		coin_5mop
	}coin_type;

	typedef struct _boxID
	{
		CString CoinDedicatedChangeBoxAID;			// 硬币专用找零箱AID 右
		CString CoinDedicatedChangeBoxBID;			// 硬币专用找零箱BID 左
		CString CoinCycleChangeBoxAID;				// 硬币循环找零箱AID 右
		CString CoinCycleChangeBoxBID;				// 硬币循环找零箱BID 左
		_boxID()
		{
			CoinDedicatedChangeBoxAID = _T("");
			CoinDedicatedChangeBoxBID = _T("");
			CoinCycleChangeBoxAID	  = _T("");
			CoinCycleChangeBoxBID	  = _T("");
		}
	}boxID;

	typedef struct _boxType
	{
		CString CoinDedicatedChangeBoxAType;			// 硬币专用找零箱A面值
		CString CoinDedicatedChangeBoxBType;			// 硬币专用找零箱B面值
		CString CoinCycleChangeBoxAType;				// 硬币循环找零箱A面值
		CString CoinCycleChangeBoxBType;				// 硬币循环找零箱B面值
		_boxType()
		{
			CoinDedicatedChangeBoxAType = _T("");
			CoinDedicatedChangeBoxBType = _T("");
			CoinCycleChangeBoxAType = _T("");
			CoinCycleChangeBoxBType = _T("");
		}
	}boxType;

	typedef struct _boxQuantity
	{
		CString CoinDedicatedChangeBoxAQuantity;			// 硬币专用找零箱A数量
		CString CoinDedicatedChangeBoxBQuantity;			// 硬币专用找零箱B数量
		CString CoinCycleChangeBoxAQuantity;				// 硬币循环找零箱A数量
		CString CoinCycleChangeBoxBQuantity;				// 硬币循环找零箱B数量
		CString DeBoxAAfterColQuantity;						// 硬币专用找零箱A回收后数量
		CString DeBoxBAfterColQuantity;						// 硬币专用找零箱B回收后数量
		CString CyBoxAfterColAQuantity;						// 硬币循环找零箱A回收后数量
		CString CyBoxAfterColBQuantity;						// 硬币循环找零箱B回收后数量
		_boxQuantity()
		{
			CoinDedicatedChangeBoxAQuantity = _T("");
			CoinDedicatedChangeBoxBQuantity = _T("");
			CoinCycleChangeBoxAQuantity = _T("");
			CoinCycleChangeBoxBQuantity = _T("");
			DeBoxAAfterColQuantity = _T("");
			DeBoxBAfterColQuantity = _T("");
			CyBoxAfterColAQuantity = _T("");
			CyBoxAfterColBQuantity = _T("");
		}
	}boxQuantity;

	typedef struct _tagModel
	{
		boxID boxid;
		boxType boxtype;
		boxQuantity boxquantity;
		int CollectCoinFlowTime;
		BOOL IsHasException;
		BOOL IsAlreadyFull;
		bool bIsSelectColl;
		BOOL IsPrintFailed;

		int  ChangeACount;
		int  ChangeBCount;
		int	 CircleACount;
		int	 CircleBCount;

		int  ChangeAColCount;			// 找零箱A实际回收数量
		int  ChangeBColCount;			// 找零箱B实际回收数量
		int  CircleAColCount;			// 循环找零箱A实际回收数量
		int  CircleBColCount;			// 循环找零箱A实际回收数量
		_tagModel()
		{
			ChangeACount = 0;
			ChangeBCount = 0;
			CircleACount = 0;
			CircleBCount = 0;

			ChangeAColCount = 0;
			ChangeBColCount = 0;
			CircleAColCount = 0;
			CircleBColCount = 0;

			CollectCoinFlowTime = -1;
			IsHasException = FALSE;
			IsAlreadyFull = FALSE;
			bIsSelectColl = false;
			IsPrintFailed = FALSE;
		}
	}Model;

	Model& GetDataModel();

	void NotifyDataModelChanged();
	void InitializeDataModel();													// 初始化数据模型
	void PrintCoinCollectReceipt();												// 硬币回收凭证打印
	void CollectCoinHandleData();												// 处理面值、ID、数量数据
	CString GetHopperCoinType(int hopetype);									// 获取硬币补充箱币种
	bool DoCheckInput();														// 对输入数据是否合理做出相应项的实际判断
	void DoCollectCoin();														// 硬币回收
	bool DoCollectStockBox();													// 循环找零箱硬币回收
	bool DoCollectHopper1Box();													// 硬币专用找零箱A硬币回收
	bool DoCollectHopper2Box();													// 硬币专用找零箱B硬币回收
	void Sendcables();															// 上传电文
	CASH_TYPE_COUNT PackageCoinTypeAndCount(int hopetype,int count);			// 打包币种代码、数量
	void Dataintodatabase();													// 数据导入数据库
protected:
	void OnStart();																// Service启动后处理
private:
	int Menu_No;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfoA;				// 硬币回收箱A计数信息
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfoB;				// 硬币回收箱B计数信息
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
	LRESULT OnKeyboardF2(WPARAM , LPARAM );										// 处理 [F2/Alt] 键按下消息

	Model m_DataModel;
	bool ExecuteCollection();

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;
};
