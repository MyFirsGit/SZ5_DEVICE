#pragma once
#include "stdafx.h"
#include "TVMForeService.h"
#include "GStatic.h"
#include <stack>
using namespace std;

class CTicketBoxExchangeSvc:public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CTicketBoxExchangeSvc();
	virtual ~CTicketBoxExchangeSvc();
public:
	typedef enum _UE_DLG_GROUP
	{		
		UE_NOTICEINFO=1,//��ʾ��Ϣ�Ի�����
	}UE_DLG_GROUP;		//ö�������õ��ĶԻ�����

	typedef enum _dialog_group {
		DLG_BASE     = 1, //��Ӫ����ѡ��
		DLG_INSTALL  = 2, //ƾ֤ѡ��
		DLG_REMOVE   = 3, //���ڴ�ӡ
	} DIALOG_GROUP;

	typedef enum _box_state
	{
		UNKNOWN_STATE,
		INSTALLED,		// �Ѱ�װ
		REMOVED,		// ��ж��
		PRE_INSTALL,	// ����װ
		PRE_REMOVE,		// ��ж��
		INVALID_AT,		// �Ƿ���װ
		INVALID_DT		// �Ƿ�ж��
	}BOX_STATE;

	typedef struct _US_INSTALL_INFO
	{
		CString szID;			//��װƱ���ID
		CString szQuantity;		//Ʊ������
		CString szSetCount;		//���õ�Ʊ��������δ��ʽ��װ��
		CString szWastedID;		//��װ�ķ�Ʊ���ID
		BOX_STATE state;		//����״̬


		_US_INSTALL_INFO()
		{
			szID=_T("FFFFFFFF");
			szQuantity=_T("0");
			szSetCount = _T("0");
			szWastedID=_T("FFFFFFFF");
		}

		CString GetStateToString()
		{
			CString strState = _T("--");
			switch(state)
			{
			case UNKNOWN_STATE: strState = _T("--");
				break;
			case INSTALLED: strState = _T("�Ѱ�װ");
				break;
			case PRE_INSTALL: strState = _T("����װ");
				break;
			case REMOVED: strState = _T("��ж��");
				break;
			case PRE_REMOVE: strState = _T("��ж��");
				break;
			}
			return strState;
		}
		// ��ͬ״̬��Ӧ����ɫ
		COLORREF GetColorForState()
		{
			COLORREF color = BLACK;
			switch(state)
			{
			case UNKNOWN_STATE: color = BLACK;
				break;
			case INSTALLED: color = GREEN;
				break;
			case PRE_INSTALL: color = SOFTGREEN;
				break;
			case REMOVED: color = RED;
				break;
			case PRE_REMOVE: color = RED;
				break;
			}
			return color;
		}
		// ͬ���������������ɹ��ǣ�����һ�����õ�������Ϊʵ������
		void SyncCount()
		{
			szQuantity = szSetCount;
		}

	}US_INSTALL_INFO;			//��װ��Ϣ

	typedef enum _UE_BOX_POS
	{
		UE_POS_CARD1=1,
		UE_POS_CARD2,
		UE_POS_WASTED
	}UE_BOX_POS;

	typedef enum box_moval			// Ʊ��ģ���ƶ�
	{
		BOX_UP	 = 1,
		BOX_DOWN,
	}BOX_MOVAL;

	typedef enum _UE_CHECK_RET
	{
		UE_VALID=0,
		UE_INVALID_CARDBOXID,
		UE_INVALID_CARDQUANTITY,
		UE_INVALID_WASTEDID,
		UE_INVALID
	}UE_CHECK_RET;

	typedef enum _current_phase		// ��ǰ�����Ĳ���״̬
	{
		PHASE_CHOOSE_BOX	=1,
		PHASE_SET_COUNT,
	}CURRENT_PHASE;

	typedef enum _operate_box		// ������������
	{
		TICKET_BOX_A		=1,
		TICKET_BOX_B,
		INVALID_BOX,
		//BUFFER,
		ALL_BOX,
	}OPERATE_BOX;

	// ����ģ�� ������ĿΪ��Ʊ�������û�õ���
	typedef struct _DATA_MODEL
	{
		US_INSTALL_INFO		BoxAinfo;		// Ʊ��A����
		US_INSTALL_INFO		BoxBinfo;		// Ʊ��B����
		US_INSTALL_INFO		InvalidBox;		// ��Ʊ������

		bool				m_bBoxACanOperator;
		bool				m_bBoxBCanOperator;
		bool				m_bInvalidBoxCanOperator;

		bool				bIsException;

		_DATA_MODEL()
		{
			m_bBoxACanOperator = false;
			m_bBoxBCanOperator = false;
			m_bInvalidBoxCanOperator = false;
			bIsException = false;
		}
	}DATA_MODEL;

private:
	DATA_MODEL m_dataModel;
	US_INSTALL_INFO m_usinstallinfo;	//���氲װ��Ϣ,ִ��F2��ʱ�����Ϸ���

	BOOL	m_bSelectBoxA;
	BOOL	m_bSelectWasted;

	BOOL	m_bSelectUninstallBoxA;
	BOOL	m_bSelectUninstallWasted;
protected:
	virtual void OnStart();                                                         // Service��������
public:
	DATA_MODEL& getModel();															// ��ȡ����ģ���е�����
	void PrintReceipt(int nType);

private:
	long checkInstallInfo(US_INSTALL_INFO& usinstallinfo);	//У�鰲װ��Ϣ�Ƿ�Ϸ�
	long InstallDevices();			//��װ�豸 Ʊ�� ��Ʊ��
	long UpdateCardBoxDataForInstall(OPERATE_BOX opBox);			//ֻ��װƱ��
	bool installWastedCardBox();	//ֻ��װ��Ʊ��
	long UninstallDevices();		//ж���豸 Ʊ�� ��Ʊ��
	long UpdateCardBoxDataForRemove(OPERATE_BOX opBox);		//ֻж��Ʊ��
	bool uninstallWastedCardBox();	//ֻж�ط�Ʊ��

	long checkInputCardBoxID(CString szID);
	long checkInputCardBoxQuantity(CString szQuantity);
	long checkInputWastedID(CString szID);

	bool isBoxCouldBeOperated(OPERATE_BOX opBox);	// �ж������Ƿ�ɲ���
	void toNextPhase();// �л�����һ��״̬
	void setCountAndState(OPERATE_BOX box, CString strCount);
	void DoRemoval(int nInput);
	void InitCardBoxData();
	bool HandleBox(TH_MOVE_BOX_CMD_INFO cmd);

	//implement base class's virtual function
public:
	virtual void		NotifyDataModelChanged();
	int  m_buffer_result;	// ���������ս�� 0 δ֪  1 �ɹ� 2 ʧ��

private:		
	virtual LRESULT OnKeyboardEnter(WPARAM , LPARAM);						// ���� [Enter] ��������Ϣ��ִ�У�
	virtual LRESULT OnKeyboardF2(WPARAM , LPARAM);							 // ���� [F2/Alt] ��������Ϣ�����棩
	virtual LRESULT OnKeyboardF3(WPARAM , LPARAM);							 //  ���� [F3/Ctrl] ��������Ϣ�����أ�
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );
	
	void ShowTips(OPERATE_BOX);												// ��ʾ��ʾ����
	bool MoveTicketBox(OPERATE_BOX, BOX_MOVAL);								// �ƶ�Ʊ��ģ����
	bool m_bHasException;
	bool boxAUnset;
	bool boxBUnset;

	void InitSvcData();														// ��ʼ��ҵ������
	long ReadBoxDataAndShow(int index);										// ��ȡƱ��(Ӳ��)���ݲ���ʾ
	bool CheckBoxStatus(OPERATE_BOX);										// ���Ʊ��״̬

	CURRENT_PHASE m_CurrentPhase;												 // ��ǰ�����Ĳ����׶�
	stack<OPERATE_BOX>  m_stkOpBox;												// �洢��ѡ�������

	inline void ClearStk(){	
		while (!m_stkOpBox.empty())	{ m_stkOpBox.pop();} }						// ���ջ

	CTCardCountInfo::CARD_BOX_INFO m_Ex_BoxA;									// ����ǰƱ������
	CTCardCountInfo::CARD_BOX_INFO m_Ex_BoxB;
	CTCardCountInfo::CARD_BOX_INFO m_Ex_INLD;
	bool isBufferSelected;
};
