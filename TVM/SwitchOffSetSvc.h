#pragma once
#include "TVMForeService.h"
#include "SwitchOffSetMaintainDlg.h"

#define MAX_SWITCHOFF_NUM  6		//�������ý���˵��������

/**
@brief    ͨ�Ų����趨
*/
class CSwitchOffSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CSwitchOffSetSvc();
	~CSwitchOffSetSvc();

	typedef enum _dialog_group{
		DIALOG_GROUP_SWITCHOFF_SET = 1,
	}DIALOG_GROUP;

	typedef struct _tagModel
	{
		BOOL UsedUnUsed;						// ����/ͣ��
		CString MenuRechargeRW;					// ��ֵ��д��
		CString MenuSmartcardRW;				// Smart-card��д��
		//CString MenuTokenRW;					// Token��д��
		CString MenuBanknoteModule;				// ֽ��ģ��
		CString MenuBanknoteChangeModule;		// ֽ������ģ��
		CString MenuSmartcardModule;			// Smart-cardģ��
		//CString MenuTokenModule;				// Tokenģ��
		CString MenuCoinModule;					// Ӳ��ģ��
		CString MenuReceiptPrint;				// ƾ֤��ӡ��
		//CString MenuPinPadModule;				// �������
		//CString MenuBankCardModule;			// ���п�ģ��
		int MenuNumber;							// �˵���
		BOOL MenuChange;						// �˵��仯��־(�˵�����1�䵽2�����ֱ仯��MenuChangeΪTRUE)
		BOOL CodeChange;						// ����仯��־(�û��ı�����/ͣ��״̬����CodeChangeΪTRUE)
		BOOL CodeLegal;						    // ���������Ƿ�Ƿ���־
		_tagModel()
		{
			UsedUnUsed = FALSE;
			MenuRechargeRW = _T("");			
			MenuSmartcardRW = _T("");			
			//MenuTokenRW = _T("");				
			MenuBanknoteModule = _T("");
			MenuBanknoteChangeModule = _T("");
			MenuSmartcardModule = _T("");		
			//MenuTokenModule = _T("");			
			MenuCoinModule = _T("");			
			MenuReceiptPrint = _T("");			
			//MenuPinPadModule = _T("");			
			//MenuBankCardModule = _T("");		
			MenuNumber = 0;
			MenuChange = FALSE;
			CodeChange = FALSE;
			CodeLegal = TRUE;
		}
	}Model;

	typedef struct _USSAVE						// ���á�ͣ��״̬����
	{
		CString saveRechargeRW;					// ��ֵ��д��
		CString saveSmartcardRW;				// Smart-card��д��
		//CString saveTokenRW;					// Token��д��
		CString saveBanknoteModule;				// ֽ��ģ��
		CString saveBanknoteChangeModule;		// ֽ������ģ��
		CString saveSmartcardModule;			// Smart-cardģ��
		//CString saveTokenModule;				// Tokenģ��
		CString saveCoinModule;					// Ӳ��ģ��
		CString saveReceiptPrint;				// ƾ֤��ӡ��
		//CString savePinPadModule;				// �������
		//CString saveBankCardModule;			// ���п�ģ��
		_USSAVE()
		{
			saveRechargeRW = _T("");
			saveSmartcardRW = _T("");
			//saveTokenRW = _T("");
			saveBanknoteModule = _T("");
			saveBanknoteChangeModule = _T("");
			saveSmartcardModule = _T("");
			//saveTokenModule = _T("");
			saveCoinModule = _T("");
			saveReceiptPrint = _T("");
			//savePinPadModule = _T("");
			//saveBankCardModule = _T("");
		}
	}USSAVE;

	Model& GetDataModel();
	void ReadTvmIni();	    // ��ȡTVM.INI�ļ�
	CString ReadUSEING(BOOL UsedUnUsed);
	void CheckMenuNumber();
	void InputGuideMsg();	// ��GuideMsg�ֱ�ֵ
	void DoCheckInput();	// �����������Ƿ����������Ӧ���ʵ���ж�
	CString ChangeToStr(int UsedUnUsed);
	CString SaveDataChange(BOOL SaveData);												
	virtual void NotifyDataModelChanged();
protected:
	void OnStart();		// Service��������
private:
	int EnterTime;
	CString GuideMsg;	// ������ʾ
	Model m_DataModel;
	USSAVE UsSave;
	int DeviceType;		// �豸����

	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Clear] ��������Ϣ��ɾ����
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F2/Alt] ��������Ϣ�����棩
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
};