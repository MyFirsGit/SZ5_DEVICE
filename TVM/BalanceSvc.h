#pragma once
#include "tvmforeservice.h"
class CBalanceSvc :
	public CTVMForeService
{
	 DECLARE_MESSAGE_MAP()
public:
	CBalanceSvc(void);
	~CBalanceSvc(void);

	typedef enum _dialog_group
	{
		DIALOG_GROUP_BASE,
	} DIALOG_GROUP;

	void OnStart();
	void NotifyDataModelChanged();
	typedef enum _current
	{
		PROCESSING_DATA,	// ҵ�����ݴ�����
		PRINT_RECEPTION,	// ҵ�ᵥ�ݴ�ӡ��
		SHUTTING_DOWN		// �ػ�������
	}current_state;

	typedef struct data_model
	{
		current_state	state;
	}DATA_MODEL;

	DATA_MODEL GetDataModel();
private:
	DATA_MODEL model;
	void ShutDown();
	virtual LRESULT OnKeyboardF2(WPARAM , LPARAM);							 // ���� [F2/Alt] ��������Ϣ�����棩
	virtual LRESULT OnKeyboardF3(WPARAM , LPARAM);							 // ���� [F3/Ctrl] ��������Ϣ�����أ�
};

