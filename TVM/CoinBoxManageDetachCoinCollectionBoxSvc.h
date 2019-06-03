#pragma once
#include "TVMForeService.h"

/**
@brief 硬币回收箱卸载
*/
class CCoinBoxManageDetachCoinCollectionBoxSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageDetachCoinCollectionBoxSvc();
	virtual ~CCoinBoxManageDetachCoinCollectionBoxSvc();

	// 画面组
	typedef enum _dialog_group
	{
		DIALOG_GROUP_DETACH_COLLECTION_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _coin_collection_box
	{
		coin_collection_boxA = 1,
		coin_collection_boxB,
		coin_collection_All,
	}COIN_COLLECTION_BOX;

	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// 硬币回收箱卸载基本界面					
		SecondFrame									// 硬币回收箱卸载完成界面	
	}FrameFlow;

	typedef enum _coin_type
	{
		coin_50avos = 1,
		coin_1mop,
		coin_2mop,
		coin_5mop
	}coin_type;

	typedef struct _CoinCollectionBoxAData
	{
		CString CoinCollectionBoxAID;				// 硬币回收箱AID
		CString CoinCollectionBoxA50AVOSData;		// 硬币回收箱A50AVOSData
		CString CoinCollectionBoxA1MOPData;			// 硬币回收箱A1MOPData
		CString CoinCollectionBoxA2MOPData;			// 硬币回收箱A2MOPData
		CString CoinCollectionBoxA5MOPData;			// 硬币回收箱A5MOPData
		CString CoinCollectionBoxATotalData;		// 硬币回收箱A合计Data
		CString CoinCollectionBoxAStatus;			// 硬币回收箱A安装状态
		CString CoinBoxABeforeRemove;				// 硬币回收箱A卸载前状态
		CString CoinCollectionAfterBoxAStatus;		// 硬币回收箱A卸载后状态
		CString CoinCollectionBoxAfterDetachAID;	// 硬币回收箱A卸载后ID
		_CoinCollectionBoxAData()
		{
			CoinCollectionBoxAID = _T("");
			CoinCollectionBoxA50AVOSData = _T("");
			CoinCollectionBoxA1MOPData = _T("");
			CoinCollectionBoxA2MOPData = _T("");
			CoinCollectionBoxA5MOPData = _T("");
			CoinCollectionBoxATotalData = _T("");
			CoinCollectionBoxAStatus = _T("");
			CoinCollectionAfterBoxAStatus = _T("");
			CoinCollectionBoxAfterDetachAID = _T("");
		}
	}CoinCollectionBoxAData;

	typedef struct _CoinCollectionBoxBData
	{
		CString CoinCollectionBoxBID;				// 硬币回收箱BID
		CString CoinCollectionBoxB50AVOSData;		// 硬币回收箱B50AVOSData
		CString CoinCollectionBoxB1MOPData;			// 硬币回收箱B1MOPData
		CString CoinCollectionBoxB2MOPData;			// 硬币回收箱B2MOPData
		CString CoinCollectionBoxB5MOPData;			// 硬币回收箱B5MOPData
		CString CoinCollectionBoxBTotalData;		// 硬币回收箱B合计Data
		CString CoinCollectionBoxBStatus;			// 硬币回收箱B安装状态
		CString CoinBoxBBeforeRemove;				// 硬币回收箱B卸载前状态
		CString CoinCollectionAfterBoxBStatus;		// 硬币回收箱B卸载后状态
		CString CoinCollectionBoxAfterDetachBID;	// 硬币回收箱B卸载后ID
		_CoinCollectionBoxBData()
		{
			CoinCollectionBoxBID = _T("");
			CoinCollectionBoxB50AVOSData = _T("");
			CoinCollectionBoxB1MOPData = _T("");
			CoinCollectionBoxB2MOPData = _T("");
			CoinCollectionBoxB5MOPData = _T("");
			CoinCollectionBoxBTotalData = _T("");
			CoinCollectionBoxBStatus = _T("");
			CoinCollectionAfterBoxBStatus = _T("");
			CoinCollectionBoxAfterDetachBID = _T("");
		}
	}CoinCollectionBoxBData;
	typedef enum _box_status
	{
		ST_INSTALLED,
		ST_REMOVED,
		ST_PRE_REMOVED
	}COLLECTION_BOX_STATUS;
	typedef struct _collection_box_data			// 回收箱数据
	{
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO countInfo;
		COLLECTION_BOX_STATUS  status;
		_collection_box_data()
		{
			status = ST_REMOVED;
		}
		CString GetStatusString()
		{
			CString str = _T("");
			switch(status)
			{
			case ST_INSTALLED:
					str = _T("已安装");
					break;
				case ST_REMOVED:
					str = _T("已卸载");
					break;
				case ST_PRE_REMOVED:
					str = _T("待卸载");
					break;
				default:
					break;
			}
			return str;
		}

		COLORREF GetStatusColor()
		{
			COLORREF color = BLACK;
			switch(status)
			{
			case ST_INSTALLED:
					color = GREEN;
					break;
				case ST_REMOVED:
					color = RED;
					break;
				case ST_PRE_REMOVED:
					color = RGB(200,0,0);
					break;
				default:
					break;
			}
			return color;
		}
	}COLLECTION_BOX_DATA;
												// 数据模型
	typedef struct _tagModel
	{
		COLLECTION_BOX_DATA BoxA;
		COLLECTION_BOX_DATA BoxB;

		CString strColBoxAExID;						// 安装之前的ID
		CString strColBoxBExID;

		int		nColBoxAExCount;					// 安装之前的数量
		int		nColBoxBExCount;

		CoinCollectionBoxAData coincollectionboxAdata;
		CoinCollectionBoxBData coincollectionboxBdata;
		int DetachCoinCollectionBoxFlowTime;
		BOOL IsHasException;
		bool bIsPrintFalied;
		bool bIsRemoveBoxA;
		bool bIsRemoveBoxB;

		_tagModel()
		{
			DetachCoinCollectionBoxFlowTime = 1;
			IsHasException = FALSE;
			bIsRemoveBoxA  = false;
			bIsRemoveBoxB  = false;
			bIsPrintFalied = false;
		}

	}Model;

	Model& GetDataModel();

	void NotifyDataModelChanged();
	void InitializeDataModel();													// 初始化数据模型
	void HandleDataModel(int index);										
	CString GetHopperCoinType(int hopetype);									// 硬币箱面值转换
	void HandleCoinCollectionBoxData();											// 处理硬币回收箱数据
	void Sendcables(int boxnumber);												// 上传电文
	vector<CASH_TYPE_COUNT> PackageCoinTypeAndCount(int FAVOScount,int OMOPcount,int SMOPcount,int FMOPcount);	// 打包币种代码、数量
	void PrintCoinBoxExchangeReceipt();								// 硬币循环找零箱加币凭证打印
	void Dataintodatabase(int collectionboxnumber);								// 数据导入数据库
protected:
	void OnStart();																// Service启动后处理
private:
	BOOL IsBoxARemoved;															// 硬币回收箱A是否已安装标志
	BOOL IsBoxBRemoved;;														// 硬币回收箱B是否已安装标志
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfoA;				// 硬币回收箱A计数信息
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfoB;				// 硬币回收箱B计数信息
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
	LRESULT OnKeyboardF2(WPARAM , LPARAM );										// 处理 [F2/Alt] 键按下消息

	Model m_DataModel;

	void InitializeData();
	bool IsBoxCouldBeRemoved(COIN_COLLECTION_BOX box);													// 判断箱子是否可被卸载

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;

};