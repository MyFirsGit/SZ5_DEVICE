#pragma once
#include "TVMForeService.h"

#define _MAX_ROW  15        //���������
#define _MAX_COL   4        //���������

/**
@brief      �쳣���ҵ����
*/
class CRemoveExceptionSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CRemoveExceptionSvc();
	~CRemoveExceptionSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_REMOVE_EXCEPTION = 1
	} DIALOG_GROUP;

	typedef struct _tag_data_modol
	{
		vector<CSysException> m_ListException;
		vector<CSysException>::size_type m_ExceptionNum;
		_tag_data_modol(){
			m_ListException.clear();
			m_ExceptionNum = 0;
		}
	}Modol;

	Modol& GetDataModol();            // ��ȡ����ģ��
	void NotifyDataModelChanged();    // ���ݸı��֪ͨ�����������
	void InitExceptionData();         // ��ʼ���쳣����
private:
	Modol m_modol;

	//��Ϣ
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );			                //ȷ�ϼ�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );								//����
	void OnStart();
};
