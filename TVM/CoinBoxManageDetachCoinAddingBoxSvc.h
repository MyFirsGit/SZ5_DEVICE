#pragma once
#include "TVMForeService.h"

/**
@brief �ӱ���ж��
*/
class CCoinBoxManageDetachCoinAddingBoxSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageDetachCoinAddingBoxSvc();
	virtual ~CCoinBoxManageDetachCoinAddingBoxSvc();

	// ������
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
		CString CoinAddingBoxAStatus;				// Ӳ�Ҳ�����A��װ״̬
		CString CoinAddingBoxBStatus;				// Ӳ�Ҳ�����B��װ״̬
		CString CoinBoxABeforeAddingStatus;			// Ӳ�Ҳ�����A��װǰ״̬
		CString CoinBoxBBeforeAddingStatus;			// Ӳ�Ҳ�����B��װǰ״̬
		CString CoinAddingBoxAID;					// Ӳ�Ҳ�����AID
		CString CoinAddingBoxBID;					// Ӳ�Ҳ�����BID
		CString CoinAddingBoxAfterDetachAID;		// Ӳ�Ҳ�����Aж�غ�ID
		CString CoinAddingBoxAfterDetachBID;		// Ӳ�Ҳ�����Bж�غ�ID
		CString CoinAddingBoxAfterDetachStatus;		// Ӳ�Ҳ�����Aж�غ�״̬
		CString CoinAddingBoxBfterDetachStatus;		// Ӳ�Ҳ�����Bж�غ�״̬
		int DetachCoinAddingBoxFlowTime;
		BOOL IsHasException;
		bool bIsRemoveAddingBoxA;					// �Ƿ��Ƴ�Ӳ�Ҽӱ���A
		bool bIsRemoveAddingBoxB;					// �Ƿ��Ƴ�Ӳ�Ҽӱ���B
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
	CString GetHopperCoinType(int hopetype);									// Ӳ������ֵת��
	void DoDetachCoinAddingBoxHandleData();										// ����ж��Ӳ�Ҳ���������
	void Sendcables(int boxnumber);												// �ϴ�����
	void PrintCoinBoxExchangeReceipt();											// Ӳ�Ҳ�����ж��ƾ֤��ӡ
protected:
	void OnStart();																// Service��������
private:

	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
	LRESULT OnKeyboardF2(WPARAM,LPARAM);										// ���� [F2/Alt] ��������Ϣ

	Model m_DataModel;

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;
};