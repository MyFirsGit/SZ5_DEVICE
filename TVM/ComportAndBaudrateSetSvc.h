#pragma once
#include "TVMForeService.h"

/**
@brief    �豸���ں��벨�����趨
*/
class CComportAndBaudrateSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CComportAndBaudrateSetSvc();
	~CComportAndBaudrateSetSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_COMPORT_BAUD_SET = 1,
	} DIALOG_GROUP;

	/*
		�˵�����
	*/
	typedef enum _em_menu_def_{
		MENU_KEY_RW	=1,
		//MENU_KEY_BH,
		MENU_KEY_TH,
		MENU_KEY_CH,
		//MENU_KEY_PRINT,
		MENU_KEY_DISPLAY,
		//MENU_KEY_BHCHANGE,
		MENU_KEY_IO,
	};

	/*
		�˵���ǰ״̬
	*/
	typedef enum _tag_menu_status_{
		MENU_STATUS_NORMAL,			// ����״̬
		MENU_STATUS_SELECT,			// ѡ��״̬
		MENU_STATUS_UPDATED,		// ����״̬
	}MENU_STATUS;

	/*
		���ݽṹ����
	*/
	typedef struct _tag_menu_detail_{
		CString strMenuName;			// ģ������
		CString strComport;				// ���ں�
		CString strBaudRate;			// ������

		MENU_STATUS	emComportStatus;	// ���ںŲ˵�״̬
		MENU_STATUS emBaudRateStatus;	// �����ʲ˵�״̬

		_tag_menu_detail_(){
			strMenuName = _T("");
			strComport  = _T("");
			strBaudRate = _T("");

			emComportStatus = MENU_STATUS_NORMAL;
			emBaudRateStatus= MENU_STATUS_NORMAL;
		}

	}Menu_Detail;


	/*
		����ģ��
	*/
	typedef struct _tag_data_model_{
		Menu_Detail stMenuDetail[7];	// ��ǰ�豸����7��ģ��
		int iCurSelectMenu;				// ��ǰѡ�в˵�
		int iCurSelSonMenu;				// ��ǰѡ���Ӳ˵�
		bool bAlreadySaved;				// �Ƿ��Ѿ�����

		// Init
		_tag_data_model_(){
			for(int i = 0;i < 7;i++){
				stMenuDetail[i] = _tag_menu_detail_();
			}

			iCurSelectMenu = 0;
			iCurSelSonMenu = 0;
			bAlreadySaved = true;
		}
	}Model;

	/*
		��ȡ����ģ��
	*/
	inline Model& GetDataModel(){
		return m_data_model;
	}

	virtual void	NotifyDataModelChanged();
protected:
	void OnStart();								// Service��������
private:
	Model	m_data_model;						// ����ģ�Ͷ���
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F2/Alt] ��������Ϣ�����棩
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�

	void ReadFromSettingIni();													// �������ļ��л�ȡ����������Ϣ
};