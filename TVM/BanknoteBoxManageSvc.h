#pragma once
#include "TVMForeService.h"

/**
@brief ֽ�������ҵ��Service
*/
class CBanknoteBoxManageSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteBoxManageSvc();
	virtual ~CBanknoteBoxManageSvc();

	// ������
	typedef enum _dialog_group
	{
		DIALOG_GROUP_BASE = 1,
		DIALOG_GROUP_OPERATION,
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
		bool raw_attached;		// �Ƿ�װ��ԭʼ״̬��
		bool now_attached;		// �Ƿ�װ������״̬��
		OPERATION opt;			// ��������
		_tag_banknote_box_info()
		{
			raw_id = _T("");
			now_id = _T("");
			face10 = 0;
			face20 = 0;
			face50 = 0;
			face100 = 0;
			face5 = 0;
			face1 = 0;
			raw_attached = false;
			now_attached = false;
			opt = NORMAL_OPERATION;
		}
	} BanknoteBoxInfo;

	// SVC����ģ��
	typedef struct _tagModel
	{
		OperationType operation_type;			// ��ǰ��������
		ShowPage show_page;						// ��ǰ��ʾҳ
		BanknoteBoxInfo banknote_box;			// ֽ������Ϣ
		bool bIsSelectInstall;					// �Ƿ�ѡ���˰�װ
		bool bIsSelectUninstall;				// �Ƿ�ѡ����ж��
		bool bIsBanknoteModuleException;
		_tagModel()
		{
			operation_type = OPERATION_UNKNOWN;
			show_page = SHOW_PAGE1;
			bIsSelectInstall = false;
			bIsSelectUninstall = false;
			bIsBanknoteModuleException = false;
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

	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);	// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF2(WPARAM , LPARAM );							// ����F2������Ϣ

	void InitAttachBoxData();		// ��ʼ����װҵ������
	void InitDetachBoxData();		// ��ʼ��ж��ҵ������

	void DoHandleBase();			// ���������Ϣ����
	void DoHandleAttachBox();		// ��װҵ����
	void DoHandleDetachBox();		// ж��ҵ����

	bool CheckBoxID(LPBYTE boxID);  //�ж�ֽ����ID�Ƿ�Ϸ�
	Model m_DataModel;
	vector<BANKNOTE_BOX_DATA> vecBanknote_uninstall;

	void LoadBankNoteBoxInstallReception();
	void LoadBankNoteBoxUninstallReception();
	CString  CashValueToString(BankNoteAndCoinType_t& value);
	void LoadBanknoteBoxData();
	CString m_strShowMsg;
};
