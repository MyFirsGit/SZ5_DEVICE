#pragma once
#include "TVMForeService.h"
#include <stack>
using namespace std;
/**
@brief Ӳ�һ����䰲װҵ��
*/
class CCoinBoxManageAttachCoinCollectionBoxSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageAttachCoinCollectionBoxSvc();
	virtual ~CCoinBoxManageAttachCoinCollectionBoxSvc();

	// ������
	typedef enum _dialog_group
	{
		DIALOG_GROUP_ATTACH_COLLECTION_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// Ӳ�һ����䰲װ��������					
		SecondFrame									// Ӳ�һ����䰲װ��ɽ���	
	}FrameFlow;

	typedef enum _coin_collection_box
	{
		coin_collection_boxA = 1,			// ������A
		coin_collection_boxB,				// ������B
		coin_collection_all,				// ���л�����
	}COLLECTION_BOX_INDEX;
	typedef enum _current_phase
	{
		PHASE_SELECT_BOX	= 1,			// ѡ�����ӽ׶�
		PHASE_SET_BOXID		= 2,			// ����ID�׶�
	}CURRENT_PHASE;
	
	typedef enum _box_state
	{
		ST_REMOVED		=0,					// ��ж��
		ST_INSTALLED,						// �Ѱ�װ
		ST_PRE_INSTALL,						// ����װ
	}BOX_STATE;

	typedef struct _tagModel
	{
		CString CoinCollectionBoxAID;				// Ӳ�һ�����AID
		CString CoinCollectionBoxBID;				// Ӳ�һ�����BID

		CString strColBoxAExID;						// ��װ֮ǰ��ID
		CString strColBoxBExID;

		int		nColBoxAExCount;					// ��װ֮ǰ������
		int		nColBoxBExCount;

		BOX_STATE stateA;							// Ӳ�һ�����A��״̬
		BOX_STATE stateB;							// Ӳ�һ�����B��״̬

		CString CoinCollectionBoxAStatus;			// Ӳ�һ�����A��װ״̬
		CString CoinCollectionBoxBStatus;			// Ӳ�һ�����B��װ״̬
		CString CoinBoxABeforeInstall;				// Ӳ�һ�����A��װ֮ǰ��״̬
		CString CoinBoxBBeforeInstall;				// Ӳ�һ�����B��װ֮ǰ��״̬
		CString CoinCollctionBoxAfterAttachAID;		// Ӳ�һ�����A��װ��ID
		CString CoinCollctionBoxAfterAttachBID;		// Ӳ�һ�����B��װ��ID
		CString CoinCollectionAfterBoxAStatus;		// Ӳ�һ�����A��װ��״̬
		CString CoinCollectionAfterBoxBStatus;		// Ӳ�һ�����B��װ��״̬
		int AttachCoinCollectionBoxFlowTime;
		BOOL IsHasException;
		bool bIsInstallCollectionA;					// ��װ������A
		bool bIsInstallCollectionB;					// ��װ������B
		BOOL IsPrintFalied;

		CString strPreID;							// ���ӹ̶�ID����
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

		CString strGetState(const BOX_STATE& state)		// ȡ��״̬��
		{
			CString strState = _T("--");
			switch(state){
			case ST_INSTALLED: strState = _T("�Ѱ�װ");break;
			case ST_PRE_INSTALL: strState = _T("����װ"); break;
			case ST_REMOVED: strState = _T("��ж��"); break;
			default: break;
			}
			return strState;
		}

		COLORREF GetColor(const BOX_STATE& state)		// ȡ״̬��Ӧ��ɫ
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

	void InitializeDataModel();													// ��ʼ������ģ��
	void DoAttachCollectionHandleData();										// ����װӲ�һ���������
	CString GetHopperCoinType(int hopetype);									// Ӳ������ֵת��
	void Sendcables(int boxnumber);												// �ϴ�����
	void PrintCoinBoxExchangeReceipt();											// Ӳ�һ����䰲װƾ֤��ӡ
protected:
	void OnStart();																// Service��������
private:
	BOOL IsBoxAInstalled;														// Ӳ�һ�����A�Ƿ��Ѱ�װ��־
	BOOL IsBoxBInstalled;														// Ӳ�һ�����B�Ƿ��Ѱ�װ��־
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
	LRESULT OnKeyboardF2(WPARAM,LPARAM);										// ���� [F2/Alt] �����µ���Ϣ

	Model m_DataModel;
	CURRENT_PHASE m_phase;														// ��ǰ������״̬
	bool  IsBoxCouldbeInstalled(COLLECTION_BOX_INDEX index);					// �ж������Ƿ�ɱ�ִ�а�װ����
	void  SetIdAndStateForBox(COLLECTION_BOX_INDEX index);						// ��������ID ����״̬
	void  InitializeData();														// ���ݳ�ʼ��


	stack<COLLECTION_BOX_INDEX> m_stkOpBox;										// �洢������������
	inline void Clear_Stk(){													// ���ջ����
		while(!m_stkOpBox.empty()){ m_stkOpBox.pop();}
	}

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;

	bool CheckInputBoxID(COLLECTION_BOX_INDEX boxType,int nBoxId);		// ��������ID�Ƿ��ظ�
};