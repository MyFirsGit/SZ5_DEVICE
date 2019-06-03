#pragma once
#include "TVMForeService.h"
#include "DeviceStatusDlg.h"
#include "CTCardCountInfo.h"
#include "CTCoinCountInfo.h"
#include "CTBanknoteCountInfo.h"
#include "CAfcTVMOperationParam.h"

/**
@brief �豸״̬ҵ��Service
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
		CString CardBoxAID;							// Ʊ��ID
		CString CardBoxBID;							// Ʊ��ID	
		CString INVALIDCardID;						// ��Ʊ��ID
		//CString TokenBoxAID;							// TOKEN��A��ID
		//CString TokenBoxBID;							// TOKEN��B��ID
		//CString TokenHopperAID;						// Token Hopper A
		//CString TokenHopperBID;						// Token Hopper B
		//CString InvalidTokenBoxID;					// TOKEN��Ʊ��ID
		//CString CollectedTokenBoxID;				// TOKEN������ID
		CString DedicatedChangeCoinAID;				// Ӳ��ר��������AID
		CString DedicatedChangeCoinBID;				// Ӳ��ר��������BID
		CString CycleChangeCoinAID;					// Ӳ��ѭ��������A ID
		CString CycleChangeCoinBID;					// Ӳ��ѭ��������B ID 
		CString CoinCollectionAID;					// Ӳ�һ�����AID
		CString CoinCollectionBID;					// Ӳ�һ�����BID
		CString BanknoteCollectionID;				// ֽ�һ�����ID
		CString BanknoteChangeBoxID;				// ֽ��������AID
		CString BNR_RECYCLER_BOX_A_ID;				// ѭ����A ID
		CString BNR_RECYCLER_BOX_B_ID;				// ѭ����B ID
		CString BNR_RECYCLER_BOX_C_ID;				// ѭ����C ID
		CString BNR_RECYCLER_BOX_D_ID;				// ѭ����D ID

		CString CardBoxAData;						// Ʊ��A������
		CString CardBoxBData;						// Ʊ��B����
		CString InvalidCardData;					// ��Ʊ��ʣ������
		//CString TokenBoxAData;						// Token��Aʣ��ö��
		//CString TokenBoxBData;						// Token��Bʣ��ö��
		//CString TokenHopperAData;					// Token Hopper Aʣ��ö��
		//CString TokenHopperBData;					// Token Hopper Bʣ��ö��
		//CString InvalidTokenData;					// Token��Ʊ��ʣ��ö��
		CString CollectedTokenData;					// Token������ʣ��ö��
		CString DedicatedChangeCoinAData;			// Ӳ��ר��������Aʣ��ö��
		CString DedicatedChangeCoinBData;			// Ӳ��ר��������Bʣ��ö��
		CString CycleChangeCoinAData;				// Ӳ��ѭ��������Aʣ��ö��
		CString CycleChangeCoinBData;				// Ӳ��ѭ��������Bʣ��ö��
		CString CoinCollectionAData;				// Ӳ�һ�����Aʣ��ö��
		CString CoinCollectionBData;				// Ӳ�һ�����Bʣ��ö��
		CString BanknoteCollectionData;				// ֽ�һ�����ʣ������
		CString BanknoteChangeData;					// ֽ������������
		CString BNR_RecyclerBoxA;					// ѭ����Aʣ������
		CString BNR_RecyclerBoxB;					// ѭ����Bʣ������
		CString BNR_RecyclerBoxC;					// ѭ����Cʣ������
		CString BNR_RecyclerBoxD;					// ѭ����Dʣ������

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

			//CString CardBoxAData;						// Ʊ��A������
			//CString CardBoxBData;						// Ʊ��B����
			//CString InvalidCardData;					// ��Ʊ��ʣ������
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
		CString CardMNearlyEmpty;					// Smart-card�佫������
		CString CardMEmpty;							// Smart-card���ѿ�����
		CString INVALIDCardMNearlyFull;				// Smart-card��Ʊ�佫������
		CString INVALIDCardMFull;					// Smart-card��Ʊ���ѿ�����

		CString DedicatedChangeCoinANearlyEmpty;	// Ӳ��ר��������A����ö��
		CString DedicatedChangeCoinAEmpty;			// Ӳ��ר��������A�ѿ�ö��
		CString DedicatedChangeCoinBNearlyEmpty;	// Ӳ��ר��������B����ö��
		CString DedicatedChangeCoinBEmpty;			// Ӳ��ר��������B�ѿ�ö��

		//��
		CString CycleChangeCoinANearlyEmpty;		// Ӳ��ѭ�������佫��ö��
		CString CycleChangeCoinAEmpty;				// Ӳ��ѭ���������ѿ�ö��
		CString CycleChangeCoinBNearlyEmpty;		// Ӳ��ѭ�������佫��ö��
		CString CycleChangeCoinBEmpty;				// Ӳ��ѭ���������ѿ�ö��

		CString CoinCollectionANearlyFull;			// Ӳ�һ�����A����ö��
		CString CoinCollectionAFull;				// Ӳ�һ�����A����ö��
		CString CoinCollectionBNearlyFull;			// Ӳ�һ�����B����ö��
		CString CoinCollectionBFull;				// Ӳ�һ�����B����ö��

		CString BanknoteCollectionNearlyFulll;		// ֽ�һ����佫������
		CString BanknoteCollectionFull;				// ֽ�һ�������������

		CString BanknoteChangeEmpty;				// ֽ���������ѿ�����
		CString BanknoteChangeNearlyEmpty;			// ֽ�������佫������

		CString BNR_RECYCLER_BOX_A_Full;			// ֽ��ѭ����A��������
		CString BNR_RECYCLER_BOX_A_NearlyFull;		// ֽ��ѭ����A��������

		CString BNR_RECYCLER_BOX_B_Full;			// ֽ��ѭ����B��������
		CString BNR_RECYCLER_BOX_B_NearlyFull;		// ֽ��ѭ����B��������

		CString BNR_RECYCLER_BOX_C_Full;			// ֽ��ѭ����C��������
		CString BNR_RECYCLER_BOX_C_NearlyFull;		// ֽ��ѭ����C��������

		CString BNR_RECYCLER_BOX_D_Full;			// ֽ��ѭ����D��������
		CString BNR_RECYCLER_BOX_D_NearlyFull;		// ֽ��ѭ����D��������

		//CString BanknoteInvalidFull;				// ֽ�ҷϱһ�������������
		//CString BanknoteInvalidNearlyFull;			// ֽ�ҷϱһ����佫������
		//CString TokenANearlyEmpty;					// Token��A����ö��
		//CString TokenAEmpty;						// Token��A�ѿ�ö��
		//CString TokenBNearlyEmpty;					// Token��B����ö��
		//CString TokenBEmpty;						// Token��B�ѿ�ö��
		//CString InvalidTokenNearlyFull;				// Token��Ʊ�佫��ö��
		//CString InvalidTokenFull;					// Token��Ʊ���ѿ�ö��
		//CString CollectedTokenNearlyFull;			// Token�����佫��ö��
		//CString CollectedTokenFull;					// Token�������ѿ�ö��
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
		CString CCyleChangeAStatus;					// Ӳ��ѭ������A��
		CString CCyleChangeBStatus;					// Ӳ��ѭ������B��
		CString CDChangeAStatus;					// Ӳ��ר��������A
		CString CDChangeBStatus;					// Ӳ��ר��������B
		CString CCollectionAStatus;					// Ӳ�һ�����A
		CString CCollectionBStatus;					// Ӳ�һ�����B

		CString BanknoteStatus;						// ֽ�һ�����
		CString BanknoteChangeStatus;				// ֽ��������
		CString BNR_RECYCLER_BOX_A_STATUS;			// ѭ����A״̬
		CString BNR_RECYCLER_BOX_B_STATUS;			// ѭ����B״̬
		CString BNR_RECYCLER_BOX_C_STATUS;			// ѭ����C״̬
		CString BNR_RECYCLER_BOX_D_STATUS;			// ѭ����D״̬


		CString CardBoxAStatus;						// Ʊ��A
		CString CardBoxBStatus;						// Ʊ��B
		CString WastedSmartcardStatus;				// Smart-card��Ʊ��
		//CString TokenAStatus;						// Token��A
		//CString TokenBStatus;						// Token��B
		//CString TokenHopperAStatus;					// Token Hopper A
		//CString TokenHopperBStatus;					// Token Hopper B 
		//CString WastedTokenStatus;					// Token��Ʊ��
		//CString CollectedTokenStatus;				// Token������

		int timesstatus;							// ��ҳ�еĵ�ǰҳ��
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
	void ReadCashBoxID();																				//	��ȡǮ�䡢Ʊ��ID
	void ReadCashBoxData();																				//	��ȡǮ�䡢Ʊ��ʣ��ö��/����
	void ReadEmptyFull();																				//	��ȡǮ�䡢Ʊ�佫�ա��������ѿա�����ö��/����
	void ReadStatus();																					//  �ж�Ǯ�䡢Ʊ��״̬
	CString StatusChangeEmpty(CString remainboxdata,CString nearlyemptyboxdata,CString emptyboxdata,int BoxID = 2);	//  ת��Ǯ�䡢Ʊ�佫�ա��ѿ�����
	CString StatusChangeFull(CString remaindata,CString nearlyfullboxdata,CString fullboxdata);			//  ת��Ǯ�䡢Ʊ�佫������������
	virtual void NotifyDataModelChanged();
protected:
	void OnStart();																						// Service��������
private:
	Model m_DataModel;
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);											//  ���� [F3/Ctrl] ��������Ϣ�����أ�
	bool m_bIsShowNearlyFullStatus;
	bool m_bIsAutoInputRFID;
};
