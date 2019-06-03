#pragma once
#include "TVMForeService.h"

/**
@brief Ӳ�һ�����ж��
*/
class CCoinBoxManageDetachCoinCollectionBoxSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageDetachCoinCollectionBoxSvc();
	virtual ~CCoinBoxManageDetachCoinCollectionBoxSvc();

	// ������
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
		FirstFrame = 1,								// Ӳ�һ�����ж�ػ�������					
		SecondFrame									// Ӳ�һ�����ж����ɽ���	
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
		CString CoinCollectionBoxAID;				// Ӳ�һ�����AID
		CString CoinCollectionBoxA50AVOSData;		// Ӳ�һ�����A50AVOSData
		CString CoinCollectionBoxA1MOPData;			// Ӳ�һ�����A1MOPData
		CString CoinCollectionBoxA2MOPData;			// Ӳ�һ�����A2MOPData
		CString CoinCollectionBoxA5MOPData;			// Ӳ�һ�����A5MOPData
		CString CoinCollectionBoxATotalData;		// Ӳ�һ�����A�ϼ�Data
		CString CoinCollectionBoxAStatus;			// Ӳ�һ�����A��װ״̬
		CString CoinBoxABeforeRemove;				// Ӳ�һ�����Aж��ǰ״̬
		CString CoinCollectionAfterBoxAStatus;		// Ӳ�һ�����Aж�غ�״̬
		CString CoinCollectionBoxAfterDetachAID;	// Ӳ�һ�����Aж�غ�ID
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
		CString CoinCollectionBoxBID;				// Ӳ�һ�����BID
		CString CoinCollectionBoxB50AVOSData;		// Ӳ�һ�����B50AVOSData
		CString CoinCollectionBoxB1MOPData;			// Ӳ�һ�����B1MOPData
		CString CoinCollectionBoxB2MOPData;			// Ӳ�һ�����B2MOPData
		CString CoinCollectionBoxB5MOPData;			// Ӳ�һ�����B5MOPData
		CString CoinCollectionBoxBTotalData;		// Ӳ�һ�����B�ϼ�Data
		CString CoinCollectionBoxBStatus;			// Ӳ�һ�����B��װ״̬
		CString CoinBoxBBeforeRemove;				// Ӳ�һ�����Bж��ǰ״̬
		CString CoinCollectionAfterBoxBStatus;		// Ӳ�һ�����Bж�غ�״̬
		CString CoinCollectionBoxAfterDetachBID;	// Ӳ�һ�����Bж�غ�ID
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
	typedef struct _collection_box_data			// ����������
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
					str = _T("�Ѱ�װ");
					break;
				case ST_REMOVED:
					str = _T("��ж��");
					break;
				case ST_PRE_REMOVED:
					str = _T("��ж��");
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
												// ����ģ��
	typedef struct _tagModel
	{
		COLLECTION_BOX_DATA BoxA;
		COLLECTION_BOX_DATA BoxB;

		CString strColBoxAExID;						// ��װ֮ǰ��ID
		CString strColBoxBExID;

		int		nColBoxAExCount;					// ��װ֮ǰ������
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
	void InitializeDataModel();													// ��ʼ������ģ��
	void HandleDataModel(int index);										
	CString GetHopperCoinType(int hopetype);									// Ӳ������ֵת��
	void HandleCoinCollectionBoxData();											// ����Ӳ�һ���������
	void Sendcables(int boxnumber);												// �ϴ�����
	vector<CASH_TYPE_COUNT> PackageCoinTypeAndCount(int FAVOScount,int OMOPcount,int SMOPcount,int FMOPcount);	// ������ִ��롢����
	void PrintCoinBoxExchangeReceipt();								// Ӳ��ѭ��������ӱ�ƾ֤��ӡ
	void Dataintodatabase(int collectionboxnumber);								// ���ݵ������ݿ�
protected:
	void OnStart();																// Service��������
private:
	BOOL IsBoxARemoved;															// Ӳ�һ�����A�Ƿ��Ѱ�װ��־
	BOOL IsBoxBRemoved;;														// Ӳ�һ�����B�Ƿ��Ѱ�װ��־
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfoA;				// Ӳ�һ�����A������Ϣ
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfoB;				// Ӳ�һ�����B������Ϣ
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
	LRESULT OnKeyboardF2(WPARAM , LPARAM );										// ���� [F2/Alt] ��������Ϣ

	Model m_DataModel;

	void InitializeData();
	bool IsBoxCouldBeRemoved(COIN_COLLECTION_BOX box);													// �ж������Ƿ�ɱ�ж��

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;

};