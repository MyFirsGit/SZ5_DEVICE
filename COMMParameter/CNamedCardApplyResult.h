#pragma once
#include "CParameterCommandExp.h"
#include "dxfile.h"

/**
@brief	�����������¼Master��
*/

#define theNAMED_CARD_APPLY_RRSULT    CNamedCardApplyResult::GetInstance()       // Ψһ�ӿ�

class PARAMETERCOMMAND_DECLSPEC  CNamedCardApplyResult
{

public:
	bool Internalize(CString, CString);                             // ���ڻ�
	void UpdateTheApplyResult(BYTE*,BYTE status = 0);               // ����ָ���������������˼�¼(����¼�����������)
	bool GetTheApplyResult(BYTE*,BYTE&);                            // ��ȡָ���������������˽��

	static CNamedCardApplyResult&  GetInstance()
	{
		return theInstance;
	}

private:

	static  CNamedCardApplyResult theInstance;             // �����Ψһ�ӿ�

	enum {
		FILE_BASE_SIZE = 22,					        // ������¼���� 21 Byte  + 1byte
		FILE_DATE_SIZE = 4,					            // ���ڼ�¼����
	};

	typedef struct _named_card_apply_result_info
	{
		BYTE    CertificatesNum[21];      // ֤������ + ֤������(1Byte + 20Byte)
		BYTE    Status;					  // ���״̬
		_named_card_apply_result_info(){
			memset(CertificatesNum,0,sizeof(CertificatesNum));
			Status = 0;
		}
	} NAMED_CARD_APP_RESULT;

	vector<NAMED_CARD_APP_RESULT>   m_ResultList;       // ������ݼ�¼��

	CDXFile* m_File;                                 // �����ļ�
	CString m_dataPath;                             // ������Ŀ¼
	CString m_backupPath;                           // ������Ŀ¼
	_DATE   m_saveDate;                             // �����ѯ����
    CCriticalSection m_lock;                        // �ؼ�������

	void AddNamedCardApplyRecord(BYTE*,BYTE status = 0);            // ��Ӽ����������¼
	bool IsRecordExist(BYTE* );                                     // �жϼ����������¼�Ƿ����
	void ClearAllRecord();                                          // ��������������¼
	bool ReadFile();                                                // ��ȡ�ļ�

	void SetFileDate();								// д���ļ��洢����
	void WriteFile();                               // д�ļ�

	CNamedCardApplyResult(void);
	~CNamedCardApplyResult();
	CNamedCardApplyResult(const CNamedCardApplyResult&);
	CNamedCardApplyResult& operator=(const CNamedCardApplyResult&);
};
