#pragma once
#include "CParameterCommandExp.h"
#include "dxfile.h"


#define thePWD_ERR    CPwdInputError::GetInstance()       ///< CPwdInputError��Ψһ�ӿ�

class PARAMETERCOMMAND_DECLSPEC   CPwdInputError
{
public:

	bool Internalize(CString, CString);                             // ���ڻ�
	void AddPWDErrorStuffID(ULONG);                                 // ���Ա����Ϣ
	bool IsStuffIDExist(ULONG);                                     // �ж�Ա����Ϣ�Ƿ����
	void ClearStuffInfo();                                          // �������Ա����Ϣ
	bool ReadFile();                                                // ��ȡ�ļ�

	void ClearTheStaffNoInfo(ULONG);                                // �˻�����
	static CPwdInputError&  GetInstance()
	{
		return theInstance;
	}

private:

	static  CPwdInputError theInstance;        // �����Ψһ�ӿ�

	enum {
		FILE_BASE_SIZE = 5,					        // ���������¼�� 4Byte(operatorID) + 1byte(error_Count)
		FILE_DATE_SIZE = 4,					        // ���ڼ�¼����
	};

	typedef struct _staff_err_info
	{
		ULONG    stuffID;             // Ա��ID
		BYTE     errCnt;              // ������
		_staff_err_info(){
			stuffID = 0;
			errCnt = 0;
		}
	} STAFF_ERR_INFO;

	vector<STAFF_ERR_INFO>   m_stuffErrList;        // ���������Ա����Ϣ

	CDXFile* m_StuffIDFile;                          // ������������Ա��ID�ļ�
	CString m_dataPath;                             // ������Ŀ¼
	CString m_backupPath;                           // ������Ŀ¼
	_DATE   m_curDate;                              // ����������

	void SetFileDate();								// д��
	void WriteFile();                               // д�ļ�

	CPwdInputError(void);
	~CPwdInputError();
	CPwdInputError(const CPwdInputError&);
	CPwdInputError& operator=(const CPwdInputError&);
};
