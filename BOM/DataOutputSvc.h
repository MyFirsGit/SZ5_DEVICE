#pragma once
#include "BOMForeService.h"

/**
@brief    ���ݵ���
*/
class CDataOutputSvc: public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	typedef struct _file_struct{
		int iParamID;									// ����ID
		CString sParamPath;								// ����Ŀ¼/�ļ���
		bool isSucceed;									// �����Ƿ�ɹ�
		_file_struct(){
			iParamID = 0;
			isSucceed = false;
		}
	}FILE_STRUCT;

	CDataOutputSvc();
	~CDataOutputSvc();

	bool ExportParameter(DATA_TYPE_CODE);				// ���ⲿý�嵼������
	CString GetFileKeyName(int ifileNum);				// ��ô洢�ļ��豸ID���ļ���key��
	bool AuthUSB();										// ��֤USB�豸
	long PrepareUploadLogFile(CString& fileName);		// ������־�ϴ��ļ�

	CString m_DeviceID;									// �豸����

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_DATAOUTPUT_SELECT = 1,           
	} DIALOG_GROUP;
};