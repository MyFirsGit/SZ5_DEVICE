#pragma once
#include "OperationDlg.h"

class CBanknoteCollectingBoxRemovalSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CBanknoteCollectingBoxRemovalSvc(void);
	~CBanknoteCollectingBoxRemovalSvc(void);
	typedef enum _dialog_group
	{
		DIALOG_GROUP_OPERATION = 1,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR,
	} DIALOG_GROUP;

	// ��������
	enum OperationType
	{
		OPERATION_ATTACH_BOX,				// ��װֽ����
		OPERATION_DETACH_BOX,				// ж��ֽ����
		OPERATION_UNKNOWN,					// δ֪
	};

	// ��ʾҳ��ֻ��һҳ��ʱ�����ã�һ�����ڼӱң�
	enum ShowPage
	{
		SHOW_PAGE1,		// ��һҳ
		SHOW_PAGE2,		// �ڶ�ҳ
		SHOW_PAGE3,		// ����ҳ
	};

	typedef enum _operation
	{
		NORMAL_OPERATION,	//��������
		VIOLATE_INSTALL,	//�Ƿ���װ
		VIOLATE_UNINSTALL	//�Ƿ�ж��
	}OPERATION;

	// ֽ������Ϣ
	typedef struct _tag_banknote_box_info
	{
		CString raw_id;			// ԭʼ���
		CString now_id;			// ���ڱ��
		DWORD face1;
		DWORD face5;
		DWORD face10;			// 10Ԫ����
		DWORD face20;			// 20Ԫ����
		DWORD face50;			// 50Ԫ����
		DWORD face100;			// 100Ԫ����
		//DWORD face500;			// 500Ԫ����
		//DWORD face1000;			// 1000Ԫ����
		//bool raw_attached;		// �Ƿ�װ��ԭʼ״̬��
		//bool now_attached;		// �Ƿ�װ������״̬��
		//OPERATION opt;			// ��������
		_tag_banknote_box_info()
		{
			raw_id = _T("");
			now_id = _T("");
			face1 = 0;
			face5 = 0;
			face10 = 0;
			face20 = 0;
			face50 = 0;
			face100 = 0;
			//face500 = 999;
			//face1000 = 999;
			/*raw_attached = false;
			now_attached = false;
			opt = NORMAL_OPERATION;*/
		}

		void SetBanknoteCount(CTBanknoteCountInfo::BANKNOTE_BOX_INFO &info)
		{
			face1 = info.ulCount_CNY1;
			face5 = info.ulCount_CNY5;
			face10 = info.ulCount_CNY10;
			face20 = info.ulCount_CNY20;
			face50 = info.ulCount_CNY50;
			face100 = info.ulCount_CNY100;
		}

	} BanknoteBoxInfo;

	// SVC����ģ��
	typedef struct _tagModel
	{
		OperationType operation_type;			// ��ǰ��������
		BanknoteBoxInfo box_data;				// ֽ������Ϣ

		CString			strState;					// ֽ��������״̬
		COLORREF		clrState;					// ֽ����״̬��Ӧ����ɫ
		bool			bIsPrintFailed;				// �Ƿ��ӡʧ�ܣ�
		_tagModel()
		{
			strState = _opl(TXT_BANKNOTEMGR_BOX_DETACHED);
			clrState = RED;
			operation_type = OPERATION_UNKNOWN;
			bIsPrintFailed = false;
		}
	} Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();
	void OnStart(); 

	typedef struct	_BANKNOTE_BOX_DATA			//Ǯ�ҽ�������
	{
		BankNoteAndCoinType_t	value;  //��ֵ
		int						nCount; //����
		long					lAmount;//���
	}BANKNOTE_BOX_DATA;

	OPERATION GetOperationType();
	bool IsBoxConnected();				//�ⲿ�ӿڣ���ȡֽ��������״̬

	CString GetShowErrorMsg();
private:
	Model m_DataModel;
	vector<BANKNOTE_BOX_DATA> vecBanknote_uninstall;

	bool CheckBoxID(LPBYTE boxID);						//�ж�ֽ����ID�Ƿ�Ϸ�

	void InitDetachBoxData();							//��ʼ��ж��ҵ������
	void DoHandleDetachBox();							//ж��ҵ����
	void LoadBanknoteBoxData();
	void InitializeDataModel();
	//void LoadBankNoteBoxUninstallReception();

	CString m_strShowMsg;
	CString CashValueToString(BankNoteAndCoinType_t& value);

	LRESULT OnKeyboardF3(WPARAM, LPARAM);
	LRESULT OnKeyboardF2(WPARAM , LPARAM );			// ����F2������Ϣ
	LRESULT OnKeyboardEnter(WPARAM, LPARAM);		// ���� [Enter] ��������Ϣ

	void LoadBankNoteBoxUninstallReception();
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO m_Ex_Col;
};

