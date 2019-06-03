#pragma once
#include "BOMForeService.h"

/**
@brief    ���ݵ���
*/
class CDataInputSvc: public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	typedef struct _tagParameterInputStruct
	{
		CString	name;					//��������
		DWORD localCurrentVersion;		//���ص�ǰ�汾
		DWORD localFutureVersion;		//���ص�ǰ������
		DWORD importCurrentVersion;		//����ĵ�ǰ�汾
		DWORD importFutureVersion;		//����Ľ����汾
		CString importCurrentTo;		//���뵱ǰ�浽��λ��
		CString importCurrentFrom;		//�����ﵼ�뵱ǰ��
		CString importFutureTo;			//���뽫���浽��λ��
		CString importFutureFrom;		//�����ﵼ�뽫����
		_tagParameterInputStruct()
		{
			name = "";
			localCurrentVersion = -1;
			localFutureVersion = -1;
			importCurrentVersion = -1;
			importFutureVersion = -1;
			importCurrentTo = "";
			importCurrentFrom = "";
			importFutureTo = "";
			importFutureFrom = "";
		}
	} ParameterInputStruct;

	CDataInputSvc();
	~CDataInputSvc();
	bool AuthUSB();										// ��֤USB
	map<WORD,ParameterInputStruct>& GetDataModel();
	bool ImportParameter();								// �������

	int m_ErrorCode;

protected:
	void OnStart();

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_DATAINPUT_SELECT = 1,           
	} DIALOG_GROUP;
	map<WORD,ParameterInputStruct> m_DataModel;
	void InitDataModel();
	void GetImportInfo();
};