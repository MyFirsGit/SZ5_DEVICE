#pragma once
#include "TVMForeService.h"
#include "DeviceStatusDlg.h"
#include "CTCardCountInfo.h"
#include "CTCoinCountInfo.h"
#include "CTBanknoteCountInfo.h"
#include "CAfcTVMOperationParam.h"

/**
@brief 设备状态业务Service
*/
class CDeviceStatusSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CDeviceStatusSvc();
	~CDeviceStatusSvc();
	
	typedef enum _dialog_group{
		DIALOG_GROUP_DEVICESTATUS_SET = 1,
	}DIALOG_GROUP;

	typedef struct _CashBoxData
	{
		CString CardBoxAID;							// 票箱ID
		CString CardBoxBID;							// 票箱ID	
		CString INVALIDCardID;						// 废票箱ID
		//CString TokenBoxAID;							// TOKEN箱A的ID
		//CString TokenBoxBID;							// TOKEN箱B的ID
		//CString TokenHopperAID;						// Token Hopper A
		//CString TokenHopperBID;						// Token Hopper B
		//CString InvalidTokenBoxID;					// TOKEN废票箱ID
		//CString CollectedTokenBoxID;				// TOKEN回收箱ID
		CString DedicatedChangeCoinAID;				// 硬币专用找零箱AID
		CString DedicatedChangeCoinBID;				// 硬币专用找零箱BID
		CString CycleChangeCoinAID;					// 硬币循环找零箱A ID
		CString CycleChangeCoinBID;					// 硬币循环找零箱B ID 
		CString CoinCollectionAID;					// 硬币回收箱AID
		CString CoinCollectionBID;					// 硬币回收箱BID
		CString BanknoteCollectionID;				// 纸币回收箱ID
		CString BanknoteChangeBoxID;				// 纸币找零箱AID
		CString BNR_RECYCLER_BOX_A_ID;				// 循环鼓A ID
		CString BNR_RECYCLER_BOX_B_ID;				// 循环鼓B ID
		CString BNR_RECYCLER_BOX_C_ID;				// 循环鼓C ID
		CString BNR_RECYCLER_BOX_D_ID;				// 循环鼓D ID

		CString CardBoxAData;						// 票箱A的张数
		CString CardBoxBData;						// 票箱B张数
		CString InvalidCardData;					// 废票箱剩余张数
		//CString TokenBoxAData;						// Token箱A剩余枚数
		//CString TokenBoxBData;						// Token箱B剩余枚数
		//CString TokenHopperAData;					// Token Hopper A剩余枚数
		//CString TokenHopperBData;					// Token Hopper B剩余枚数
		//CString InvalidTokenData;					// Token废票箱剩余枚数
		CString CollectedTokenData;					// Token回收箱剩余枚数
		CString DedicatedChangeCoinAData;			// 硬币专用找零箱A剩余枚数
		CString DedicatedChangeCoinBData;			// 硬币专用找零箱B剩余枚数
		CString CycleChangeCoinAData;				// 硬币循环找零箱A剩余枚数
		CString CycleChangeCoinBData;				// 硬币循环找零箱B剩余枚数
		CString CoinCollectionAData;				// 硬币回收箱A剩余枚数
		CString CoinCollectionBData;				// 硬币回收箱B剩余枚数
		CString BanknoteCollectionData;				// 纸币回收箱剩余张数
		CString BanknoteChangeData;					// 纸币找零箱张数
		CString BNR_RecyclerBoxA;					// 循环鼓A剩余张数
		CString BNR_RecyclerBoxB;					// 循环鼓B剩余张数
		CString BNR_RecyclerBoxC;					// 循环鼓C剩余张数
		CString BNR_RecyclerBoxD;					// 循环鼓D剩余张数

		_CashBoxData()
		{
			CardBoxAID = _T("");
			CardBoxBID = _T("");
			INVALIDCardID = _T("");
			//TokenBoxAID = _T("");
			//TokenBoxBID = _T("");
			//TokenHopperAID = _T("");
			//TokenHopperBID = _T("");
			//InvalidTokenBoxID = _T("");
			//CollectedTokenBoxID = _T("");
			DedicatedChangeCoinAID = _T("");
			DedicatedChangeCoinBID = _T("");
			CycleChangeCoinAID = _T("");
			CycleChangeCoinBID = _T("");
			CoinCollectionAID = _T("");
			BanknoteChangeBoxID = _T("");
			CoinCollectionBID = _T("");
			BanknoteCollectionID = _T("");
			BNR_RECYCLER_BOX_A_ID = _T("");
			BNR_RECYCLER_BOX_B_ID = _T("");
			BNR_RECYCLER_BOX_C_ID = _T("");
			BNR_RECYCLER_BOX_D_ID = _T("");

			//CString CardBoxAData;						// 票箱A的张数
			//CString CardBoxBData;						// 票箱B张数
			//CString InvalidCardData;					// 废票箱剩余张数
			//TokenBoxAData = _T("");
			//TokenBoxBData = _T("");
			//TokenHopperAData = _T("");
			//TokenHopperBData = _T("");
			//InvalidTokenData = _T("");
			CollectedTokenData = _T("");	
			DedicatedChangeCoinAData = _T("");
			DedicatedChangeCoinBData = _T("");
			CycleChangeCoinAData = _T("");
			CycleChangeCoinBData = _T("");
			CoinCollectionAData = _T("");
			CoinCollectionBData = _T("");
			BanknoteCollectionData = _T("");
			BanknoteChangeData = _T("");
			BNR_RecyclerBoxA = _T("");
			BNR_RecyclerBoxB = _T("");
			BNR_RecyclerBoxC = _T("");
			BNR_RecyclerBoxD = _T("");
		}

	}CashBoxData;

	typedef struct _EmptyFull
	{
		CString CardMNearlyEmpty;					// Smart-card箱将空张数
		CString CardMEmpty;							// Smart-card箱已空张数
		CString INVALIDCardMNearlyFull;				// Smart-card废票箱将空张数
		CString INVALIDCardMFull;					// Smart-card废票箱已空张数

		CString DedicatedChangeCoinANearlyEmpty;	// 硬币专用找零箱A将空枚数
		CString DedicatedChangeCoinAEmpty;			// 硬币专用找零箱A已空枚数
		CString DedicatedChangeCoinBNearlyEmpty;	// 硬币专用找零箱B将空枚数
		CString DedicatedChangeCoinBEmpty;			// 硬币专用找零箱B已空枚数

		//？
		CString CycleChangeCoinANearlyEmpty;		// 硬币循环找零箱将空枚数
		CString CycleChangeCoinAEmpty;				// 硬币循环找零箱已空枚数
		CString CycleChangeCoinBNearlyEmpty;		// 硬币循环找零箱将空枚数
		CString CycleChangeCoinBEmpty;				// 硬币循环找零箱已空枚数

		CString CoinCollectionANearlyFull;			// 硬币回收箱A将满枚数
		CString CoinCollectionAFull;				// 硬币回收箱A已满枚数
		CString CoinCollectionBNearlyFull;			// 硬币回收箱B将满枚数
		CString CoinCollectionBFull;				// 硬币回收箱B已满枚数

		CString BanknoteCollectionNearlyFulll;		// 纸币回收箱将满张数
		CString BanknoteCollectionFull;				// 纸币回收箱已满张数

		CString BanknoteChangeEmpty;				// 纸币找零箱已空张数
		CString BanknoteChangeNearlyEmpty;			// 纸币找零箱将空张数

		CString BNR_RECYCLER_BOX_A_Full;			// 纸币循环鼓A已满张数
		CString BNR_RECYCLER_BOX_A_NearlyFull;		// 纸币循环鼓A将满张数

		CString BNR_RECYCLER_BOX_B_Full;			// 纸币循环鼓B已满张数
		CString BNR_RECYCLER_BOX_B_NearlyFull;		// 纸币循环鼓B将满张数

		CString BNR_RECYCLER_BOX_C_Full;			// 纸币循环鼓C已满张数
		CString BNR_RECYCLER_BOX_C_NearlyFull;		// 纸币循环鼓C将满张数

		CString BNR_RECYCLER_BOX_D_Full;			// 纸币循环鼓D已满张数
		CString BNR_RECYCLER_BOX_D_NearlyFull;		// 纸币循环鼓D将满张数

		//CString BanknoteInvalidFull;				// 纸币废币回收箱已满张数
		//CString BanknoteInvalidNearlyFull;			// 纸币废币回收箱将满张数
		//CString TokenANearlyEmpty;					// Token箱A将空枚数
		//CString TokenAEmpty;						// Token箱A已空枚数
		//CString TokenBNearlyEmpty;					// Token箱B将空枚数
		//CString TokenBEmpty;						// Token箱B已空枚数
		//CString InvalidTokenNearlyFull;				// Token废票箱将空枚数
		//CString InvalidTokenFull;					// Token废票箱已空枚数
		//CString CollectedTokenNearlyFull;			// Token回收箱将空枚数
		//CString CollectedTokenFull;					// Token回收箱已空枚数
		_EmptyFull()
		{
			//CardMNearlyEmpty = _T("");
			//CardMEmpty = _T("");
			//INVALIDCardMNearlyFull = _T("");
			//INVALIDCardMFull = _T("");
			DedicatedChangeCoinANearlyEmpty = _T("");
			DedicatedChangeCoinAEmpty = _T("");
			DedicatedChangeCoinBNearlyEmpty = _T("");
			DedicatedChangeCoinBEmpty = _T("");
			CycleChangeCoinANearlyEmpty = _T("");
			CycleChangeCoinAEmpty = _T("");
			CoinCollectionANearlyFull = _T("");
			CoinCollectionAFull = _T("");
			CoinCollectionBNearlyFull = _T("");
			CoinCollectionBFull = _T("");
			BanknoteCollectionNearlyFulll = _T("");
			BanknoteCollectionFull = _T("");

			BNR_RECYCLER_BOX_A_Full = _T("");		
			BNR_RECYCLER_BOX_A_NearlyFull = _T("");

			BNR_RECYCLER_BOX_B_Full = _T("");		
			BNR_RECYCLER_BOX_B_NearlyFull = _T("");

			BNR_RECYCLER_BOX_C_Full = _T("");		
			BNR_RECYCLER_BOX_C_NearlyFull = _T("");

			BNR_RECYCLER_BOX_D_Full = _T("");		
			BNR_RECYCLER_BOX_D_NearlyFull = _T("");

			//TokenANearlyEmpty = _T("");
			//TokenAEmpty = _T("");
			//TokenBNearlyEmpty = _T("");
			//TokenBEmpty = _T("");
			//InvalidTokenNearlyFull = _T("");
			//InvalidTokenFull = _T("");
			//CollectedTokenNearlyFull = _T("");
			//CollectedTokenFull = _T("");
		}
	}EmptyFull;

	typedef struct _CashBoxStatus 
	{
		CString CCyleChangeAStatus;					// 硬币循环找零A箱
		CString CCyleChangeBStatus;					// 硬币循环找零B箱
		CString CDChangeAStatus;					// 硬币专用找零箱A
		CString CDChangeBStatus;					// 硬币专用找零箱B
		CString CCollectionAStatus;					// 硬币回收箱A
		CString CCollectionBStatus;					// 硬币回收箱B

		CString BanknoteStatus;						// 纸币回收箱
		CString BanknoteChangeStatus;				// 纸币找零箱
		CString BNR_RECYCLER_BOX_A_STATUS;			// 循环鼓A状态
		CString BNR_RECYCLER_BOX_B_STATUS;			// 循环鼓B状态
		CString BNR_RECYCLER_BOX_C_STATUS;			// 循环鼓C状态
		CString BNR_RECYCLER_BOX_D_STATUS;			// 循环鼓D状态


		CString CardBoxAStatus;						// 票箱A
		CString CardBoxBStatus;						// 票箱B
		CString WastedSmartcardStatus;				// Smart-card废票箱
		//CString TokenAStatus;						// Token箱A
		//CString TokenBStatus;						// Token箱B
		//CString TokenHopperAStatus;					// Token Hopper A
		//CString TokenHopperBStatus;					// Token Hopper B 
		//CString WastedTokenStatus;					// Token废票箱
		//CString CollectedTokenStatus;				// Token回收箱

		int timesstatus;							// 翻页中的当前页数
		_CashBoxStatus()
		{
			CCyleChangeAStatus = _T("");
			CCyleChangeBStatus = _T("");
			CDChangeAStatus = _T("");
			CDChangeBStatus = _T("");
			CCollectionAStatus = _T("");
			CCollectionBStatus = _T("");
			BanknoteStatus = _T("");
			BNR_RECYCLER_BOX_A_STATUS = _T("");
			BNR_RECYCLER_BOX_B_STATUS = _T("");
			BNR_RECYCLER_BOX_C_STATUS = _T("");
			BNR_RECYCLER_BOX_D_STATUS = _T("");

			CardBoxAStatus = _T("");
			CardBoxBStatus = _T("");
			WastedSmartcardStatus = _T("");
			//TokenAStatus = _T("");
			//TokenBStatus = _T("");
			//TokenHopperAStatus = _T("");
			//TokenHopperBStatus = _T("");
			//WastedTokenStatus = _T("");	
			//CollectedTokenStatus = _T("");	

			timesstatus = -1;
		}
	}CashBoxStatus;

	typedef struct _tagModel
	{
		CashBoxData cashboxdata;
		EmptyFull emptyfull;
		CashBoxStatus cashboxstatus;
	}Model;

	Model& GetDataModel();
	void ReadCashBoxID();																				//	读取钱箱、票箱ID
	void ReadCashBoxData();																				//	读取钱箱、票箱剩余枚数/张数
	void ReadEmptyFull();																				//	读取钱箱、票箱将空、将满、已空、已满枚数/张数
	void ReadStatus();																					//  判断钱箱、票箱状态
	CString StatusChangeEmpty(CString remainboxdata,CString nearlyemptyboxdata,CString emptyboxdata,int BoxID = 2);	//  转换钱箱、票箱将空、已空数据
	CString StatusChangeFull(CString remaindata,CString nearlyfullboxdata,CString fullboxdata);			//  转换钱箱、票箱将满、已满数据
	virtual void NotifyDataModelChanged();
protected:
	void OnStart();																						// Service启动后处理
private:
	Model m_DataModel;
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);											//  处理 [F3/Ctrl] 键按下消息（返回）
	bool m_bIsShowNearlyFullStatus;
	bool m_bIsAutoInputRFID;
};
