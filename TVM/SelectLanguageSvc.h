#pragma once
#include "TVMForeService.h"

/**
@brief ά��ҵ��Service
*/
class CSelectLanguageSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CSelectLanguageSvc();					// ���캯��
	virtual ~CSelectLanguageSvc();			// ��������

	typedef enum _dialog_group {
		DIALOG_GROUP_BASE	= 1
	} DIALOG_GROUP;

	// SVC����ģ��
	typedef struct _tagModel
	{
		_tagModel()
		{
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();


private:
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ

	Model m_DataModel;
};