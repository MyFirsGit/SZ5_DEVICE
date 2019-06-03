#pragma once
#include "TVMForeService.h"

/**
@brief ά��ҵ��Service
*/
class CMaintenanceSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CMaintenanceSvc();					// ���캯��
	virtual ~CMaintenanceSvc();			// ��������

	typedef enum _dialog_group {
		DIALOG_GROUP_BASE	= 1
	} DIALOG_GROUP;

	// SVC����ģ��
	typedef struct _tagModel
	{
		vector<Menu_Item*> firstlist;
		Menu_Item* secondlist;
		_tagModel()
		{
			secondlist = NULL;
			firstlist.clear();
		}
	}Model;

	Model& GetDataModel();

	void InitData();
	void ExecuteService();
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ
	LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/ );				// ���� [Esc] ��������Ϣ
	void SetReturn(){m_isReturn = true;};
private:
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	void OnStart();
	Model m_DataModel;
	bool m_isReturn;
};