#pragma once
#include "TVMForeService.h"

#define  _MAX_ROW  15     //�����������
#define  _MAX_COL   2     //�����������

/**
@brief    ���ݵ���
*/
class CDataOutputSvc: public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CDataOutputSvc();
	~CDataOutputSvc();
	//�����������
	typedef enum _long_error_code
	{
		ERROR_CODE_NORMAL = 1,
		ERROR_CODE_CREATE_FILE_FAILED,
		ERROR_CODE_OTHER_ERROR,
	}ERROR_CODE;
	//����
	typedef enum _dialog_group {
		DIALOG_GROUP_DATAOUTPUT_SELECT = 1,           
	} DIALOG_GROUP;

protected:
	bool AuthUSB();										// ��֤USB�豸
	long PrepareUploadLogFile(CString& fileName);		// ������־�ϴ��ļ�
	long ExportParameter(DATA_TYPE_CODE);				// ���ⲿý�嵼������
	CString GetFileKeyName(int ifileNum);				// ��ô洢�ļ��豸ID���ļ���key��
	CString m_DeviceID;									// �豸����
	bool IsUSBAuthed();									// USB�Ƿ���֤ͨ����
private:
	DATA_TYPE_CODE  m_DataType;
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);	// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF3(WPARAM , LPARAM );							// ���� [F3/Ctrl] ��������Ϣ
	void OnStart();
	bool m_bIsAuthPass;									// U����֤�Ƿ�ͨ����
};