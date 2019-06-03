#pragma once
class CBanknoteCollectingBoxInstallationSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public: 
	CBanknoteCollectingBoxInstallationSvc(void);
	~CBanknoteCollectingBoxInstallationSvc(void);
	// ������
	typedef enum _dialog_group
	{
		DIALOG_GROUP_OPERATION= 1,
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
		DWORD face500;			// 500Ԫ����
		DWORD face1000;			// 1000Ԫ����
		bool raw_attached;		// �Ƿ�װ��ԭʼ״̬��
		bool now_attached;		// �Ƿ�װ������״̬��
		OPERATION opt;			// ��������
		_tag_banknote_box_info()
		{
			raw_id = _T("");
			now_id = _T("");
			face1 = 999;
			face5 = 999;
			face10 = 999;
			face20 = 999;
			face50 = 999;
			face100 = 999;
			face500 = 999;
			face1000 = 999;
			raw_attached = false;
			now_attached = false;
			opt = NORMAL_OPERATION;
		}
	} BanknoteBoxInfo;

	typedef enum _CURRENT_PHASE
	{
		PHASE_SET_COMMOND	= 0,
		PHASE_SET_ID,
	}CURRENT_PHASE;


	// SVC����ģ��
	typedef struct _tagModel
	{
		OperationType operation_type;			// ��ǰ��������
		ShowPage show_page;						// ��ǰ��ʾҳ
		BanknoteBoxInfo banknote_box;			// ֽ������Ϣ
		bool bIsSelectInstall;					// �Ƿ�ѡ���˰�װ

		CURRENT_PHASE	m_phase;					// ��ǰ���������׶�
		CString			strBoxIDSt;					// �û����õ�ID
		CString			strState;					// ֽ��������״̬
		COLORREF		clrState;					// ֽ����״̬��Ӧ����ɫ
		CString			strBoxIDEx;

		CString			strPreID;					// ֽ���䰲װIDǰ׺
		bool			bIsPrintFailed;				// ��ӡ�Ƿ�ʧ�ܣ�
		_tagModel()
		{
			operation_type = OPERATION_UNKNOWN;
			show_page = SHOW_PAGE1;
			bIsSelectInstall = false;
			m_phase	 = PHASE_SET_COMMOND;
			strBoxIDSt = _T("FFFFFFFF");
			strBoxIDEx = _T("FFFFFFFF");
			//bIsSelectUninstall = false;
			strPreID.Format(_T("%02d61"),theMAINTENANCE_INFO.GetLineCode());
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
	bool IsBoxConnected();//�ⲿ�ӿڣ���ȡֽ��������״̬

	CString GetShowErrorMsg();
private:

	LRESULT OnKeyboardF3(WPARAM, LPARAM);
	LRESULT OnKeyboardF2(WPARAM , LPARAM );		// ����F2������Ϣ
	LRESULT OnKeyboardEnter(WPARAM, LPARAM);	// ���� [Enter] ��������Ϣ

	void InitAttachBoxData();		// ��ʼ����װҵ������
	bool CheckBoxID(LPBYTE boxID);  //�ж�ֽ����ID�Ƿ�Ϸ�
	Model m_DataModel;
	vector<BANKNOTE_BOX_DATA> vecBanknote_uninstall;

	void LoadBankNoteBoxInstallReception();
	CString  CashValueToString(BankNoteAndCoinType_t& value);
	void LoadBanknoteBoxData();
	CString m_strShowMsg;
	void InitializeDataModel();

	CTBanknoteCountInfo::BANKNOTE_BOX_INFO m_Ex_Col;

	//void PrintBanknoteChangeReceiption();
};

