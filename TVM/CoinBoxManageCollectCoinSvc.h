#pragma once
#include "TVMForeService.h"

#define ALL_COIN_COUNT 0xFFFF

/**
@brief Ӳ�������ҵ��--Ӳ�һ���Service
*/
class CCoinBoxManageCollectCoinSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageCollectCoinSvc();
	virtual ~CCoinBoxManageCollectCoinSvc();

	// ������
	typedef enum _dialog_group
	{
		DIALOG_GROUP_COLLECT_COIN = 1,

	} DIALOG_GROUP;

	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// Ӳ�һ��ջ�������					
		SecondFrame,								// Ӳ�һ����н���
		ThridTFrame,								// Ӳ�һ�����ɽ���
	}FrameFlow;

	typedef enum _coinboxnumber
	{
		coin_dedicated_change_boxA = 1,				// Ӳ��ר��������A
		coin_dedicated_change_boxB,					// Ӳ��ר��������B
		coin_cycle_change_box						// Ӳ��ѭ��������
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
		CString CoinDedicatedChangeBoxAID;			// Ӳ��ר��������AID ��
		CString CoinDedicatedChangeBoxBID;			// Ӳ��ר��������BID ��
		CString CoinCycleChangeBoxAID;				// Ӳ��ѭ��������AID ��
		CString CoinCycleChangeBoxBID;				// Ӳ��ѭ��������BID ��
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
		CString CoinDedicatedChangeBoxAType;			// Ӳ��ר��������A��ֵ
		CString CoinDedicatedChangeBoxBType;			// Ӳ��ר��������B��ֵ
		CString CoinCycleChangeBoxAType;				// Ӳ��ѭ��������A��ֵ
		CString CoinCycleChangeBoxBType;				// Ӳ��ѭ��������B��ֵ
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
		CString CoinDedicatedChangeBoxAQuantity;			// Ӳ��ר��������A����
		CString CoinDedicatedChangeBoxBQuantity;			// Ӳ��ר��������B����
		CString CoinCycleChangeBoxAQuantity;				// Ӳ��ѭ��������A����
		CString CoinCycleChangeBoxBQuantity;				// Ӳ��ѭ��������B����
		CString DeBoxAAfterColQuantity;						// Ӳ��ר��������A���պ�����
		CString DeBoxBAfterColQuantity;						// Ӳ��ר��������B���պ�����
		CString CyBoxAfterColAQuantity;						// Ӳ��ѭ��������A���պ�����
		CString CyBoxAfterColBQuantity;						// Ӳ��ѭ��������B���պ�����
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

		int  ChangeAColCount;			// ������Aʵ�ʻ�������
		int  ChangeBColCount;			// ������Bʵ�ʻ�������
		int  CircleAColCount;			// ѭ��������Aʵ�ʻ�������
		int  CircleBColCount;			// ѭ��������Aʵ�ʻ�������
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
	void InitializeDataModel();													// ��ʼ������ģ��
	void PrintCoinCollectReceipt();												// Ӳ�һ���ƾ֤��ӡ
	void CollectCoinHandleData();												// ������ֵ��ID����������
	CString GetHopperCoinType(int hopetype);									// ��ȡӲ�Ҳ��������
	bool DoCheckInput();														// �����������Ƿ����������Ӧ���ʵ���ж�
	void DoCollectCoin();														// Ӳ�һ���
	bool DoCollectStockBox();													// ѭ��������Ӳ�һ���
	bool DoCollectHopper1Box();													// Ӳ��ר��������AӲ�һ���
	bool DoCollectHopper2Box();													// Ӳ��ר��������BӲ�һ���
	void Sendcables();															// �ϴ�����
	CASH_TYPE_COUNT PackageCoinTypeAndCount(int hopetype,int count);			// ������ִ��롢����
	void Dataintodatabase();													// ���ݵ������ݿ�
protected:
	void OnStart();																// Service��������
private:
	int Menu_No;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfoA;				// Ӳ�һ�����A������Ϣ
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfoB;				// Ӳ�һ�����B������Ϣ
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
	LRESULT OnKeyboardF2(WPARAM , LPARAM );										// ���� [F2/Alt] ��������Ϣ

	Model m_DataModel;
	bool ExecuteCollection();

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;
};
