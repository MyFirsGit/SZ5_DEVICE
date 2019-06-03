#pragma once
#include "TVMForeService.h"

/**
@brief 加币箱卸载
*/
class CCoinBoxManageDetachCoinAddingBoxSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageDetachCoinAddingBoxSvc();
	virtual ~CCoinBoxManageDetachCoinAddingBoxSvc();

	// 画面组
	typedef enum _dialog_group
	{
		DIALOG_GROUP_DETACH_ADDING_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _coin_adding_box_number
	{
		Coin_Adding_Box1 = 1,
		Coin_Adding_Box2 = 2
	}coin_adding_box_number;

	typedef struct _tagModel
	{
		CString CoinAddingBoxAStatus;				// 硬币补充箱A安装状态
		CString CoinAddingBoxBStatus;				// 硬币补充箱B安装状态
		CString CoinBoxABeforeAddingStatus;			// 硬币补充箱A安装前状态
		CString CoinBoxBBeforeAddingStatus;			// 硬币补充箱B安装前状态
		CString CoinAddingBoxAID;					// 硬币补充箱AID
		CString CoinAddingBoxBID;					// 硬币补充箱BID
		CString CoinAddingBoxAfterDetachAID;		// 硬币补充箱A卸载后ID
		CString CoinAddingBoxAfterDetachBID;		// 硬币补充箱B卸载后ID
		CString CoinAddingBoxAfterDetachStatus;		// 硬币补充箱A卸载后状态
		CString CoinAddingBoxBfterDetachStatus;		// 硬币补充箱B卸载后状态
		int DetachCoinAddingBoxFlowTime;
		BOOL IsHasException;
		bool bIsRemoveAddingBoxA;					// 是否移除硬币加币箱A
		bool bIsRemoveAddingBoxB;					// 是否移除硬币加币箱B
		bool IsPrintFailed;
		_tagModel()
		{
			CoinAddingBoxAStatus = _T("");
			CoinAddingBoxBStatus = _T("");
			CoinBoxABeforeAddingStatus = _T("");
			CoinBoxBBeforeAddingStatus = _T("");
			CoinAddingBoxAID = _T("");
			CoinAddingBoxBID = _T("");
			CoinAddingBoxAfterDetachAID = _T("");
			CoinAddingBoxAfterDetachBID = _T("");
			CoinAddingBoxAfterDetachStatus = _T("");
			CoinAddingBoxBfterDetachStatus = _T("");
			DetachCoinAddingBoxFlowTime = 1;
			IsHasException = FALSE;
			bIsRemoveAddingBoxA = false;
			bIsRemoveAddingBoxB = false;
			IsPrintFailed		= false;
		}
	}Model;

	Model& GetDataModel();

	void NotifyDataModelChanged();
	CString GetHopperCoinType(int hopetype);									// 硬币箱面值转换
	void DoDetachCoinAddingBoxHandleData();										// 处理卸载硬币补充箱数据
	void Sendcables(int boxnumber);												// 上传电文
	void PrintCoinBoxExchangeReceipt();											// 硬币补充箱卸载凭证打印
protected:
	void OnStart();																// Service启动后处理
private:

	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
	LRESULT OnKeyboardF2(WPARAM,LPARAM);										// 处理 [F2/Alt] 键按下消息

	Model m_DataModel;

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;
};