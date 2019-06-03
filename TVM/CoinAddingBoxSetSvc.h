#pragma once
#include "TVMForeService.h"

#define MAX_COINADDMENU_NUM 2		//������������ý���˵��������

/**
@brief    �������������
*/
class CCoinAddingBoxSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CCoinAddingBoxSetSvc();
	~CCoinAddingBoxSetSvc();
	
	typedef enum _dialog_group{
		DIALOG_GROUP_COINADDINGBOX_SET = 1,
	}DIALOG_GROUP;

	typedef struct _tagModel
	{
		int hopone ;
		int hoptwo ;
		CString hoponecurrency;
		CString hoptwocurrency;
		int MenuNumber;			//�˵���
		BOOL MenuChange;		//�˵��仯��־
		BOOL CodeChange;		//����仯��־
		BOOL CodeContent;		//���������Ƿ�Ƿ���־
		_tagModel()
		{
			hopone = -1;
			hoptwo = -1;
			hoponecurrency = _T("");
			hoptwocurrency = _T("");
			MenuNumber = 0;
			MenuChange = FALSE;
			CodeChange = FALSE;
			CodeContent = TRUE;

		}
	}Model;

	typedef struct _CurrIndex
	{
		CString numberone;
		CString numbertwo;
		_CurrIndex()
		{
			numberone = _T("");
			numbertwo = _T("");
		}
	}CurrIndex;

	Model& GetDataModel();
	void ReadTvmIni();															//��ȡTVM.INI�ļ�
	CString ReadCurrency(int hopetype);											//ʶ�����
	void CheckMenuNumber();
	void InputGetGuide();														//��GetGuide�ֱ�ֵ
	void DoCheckInput();														//�����������Ƿ����������Ӧ���ʵ���ж�
	CString CurrGet(int currmenunumber);										//�Բ˵���ת���󱣴�
	virtual void NotifyDataModelChanged();
private:
	int EnterTime;
	CString GetGuide;															//������ʾ
	
	Model m_DataModel;
	CurrIndex CurrIndexNumber;
	
	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Clear] ��������Ϣ��ɾ����
	//LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/);					// ���� [0~9] ��������Ϣ
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
};