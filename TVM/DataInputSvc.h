#pragma once
#include "TVMForeService.h"

#define  _MAX_ROW  15	//�����������
#define  _MAX_COL   3	//�����������
/**
@brief    ���ݵ���
*/
class CDataInputSvc: public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CDataInputSvc();
	~CDataInputSvc();

	//����
	typedef enum _dialog_group {
		DIALOG_GROUP_DATAINPUT_SELECT = 1,           
	} DIALOG_GROUP;

	//����ģ��
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
			name = _T("");
			localCurrentVersion = -1;
			localFutureVersion = -1;
			importCurrentVersion = -1;
			importFutureVersion = -1;
			importCurrentTo =  _T("");
			importCurrentFrom =  _T("");
			importFutureTo = _T("");
			importFutureFrom = _T("");
		}
	} ParameterInputStruct;

	map<WORD,ParameterInputStruct>& GetDataModel();     // ��ȡ����ģ��
	bool IsIndexFileExist();							// �ж������ļ��Ƿ����
	bool IsUSBAuthed();									// USB�Ƿ���֤ͨ����
protected:
	void OnStart();
	bool ImportParameter();								// �������
	bool AuthUSB();										// ��֤USB
	void NotifyDataModelChanged();						// ���½�����ʾ
private:	
	map<WORD,ParameterInputStruct> m_DataModel;
	void InitDataModel();
	void GetImportInfo();
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );	// ���̴�����[ȷ��]
	LRESULT OnKeyboardF3(WPARAM , LPARAM );     // ���̴�����[F3/Ctrl]
	bool m_bIsIndexFileExist;
	bool m_bIsAuthPass;							// U����֤�Ƿ�ͨ����
};