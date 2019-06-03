#pragma once
#include "CParameterCommandExp.h"
#include "dxfile.h"

/**
@brief	�Ǽ�ʱ�˿������¼Master��
*/

#define theREFUND_APPLY_RRSULT    CRefundApplyResult::GetInstance()       // Ψһ�ӿ�

class PARAMETERCOMMAND_DECLSPEC  CRefundApplyResult
{

public:
	typedef struct _refund_apply_result_info
	{
		CERTIFICATE_TYPE certificateType;	//֤������
		BYTE certificateID[20];					//ASC֤������
		BYTE ticketApplicationSerialNo[8];		//8HEXƱ��Ӧ�����к�
		BYTE    ReceiptNum[10];           // �վݱ��
		long	Balance;				  // �������
		long	Deposit;				  // ��Ѻ����
		BYTE    Status;					  // ���״̬
		_refund_apply_result_info(){
			certificateType = CERTIFICATE_TYPE::CERTIFICATE_UNSPECIFIED;
			memset(certificateID,0,sizeof(certificateID));
			memset(ticketApplicationSerialNo,0,sizeof(ticketApplicationSerialNo));
			memset(ReceiptNum,0,sizeof(ReceiptNum));
			Balance = 0;
			Deposit = 0;
			Status = 0;
		}
	} REFUND_APP_RESULT;
	bool Internalize(CString, CString);                             // ���ڻ�
	void UpdateTheApplyResult(REFUND_APP_RESULT&);                // ����ָ���˿��������˼�¼(����¼�����������)
	bool GetTheApplyResult(REFUND_APP_RESULT&);                // ��ȡָ���˿��������˽��

	static CRefundApplyResult&  GetInstance()
	{
		return theInstance;
	}

private:

	static  CRefundApplyResult theInstance;             // �����Ψһ�ӿ�

	enum {
		FILE_BASE_SIZE = 51,					        // ������¼���� 4 Byte + 20 Byte + 8 Byte  + 4 byte + 4 byte + 1 byte
		FILE_DATE_SIZE = 4,					            // ���ڼ�¼����
	};
	

	vector<REFUND_APP_RESULT>   m_ResultList;       // ������ݼ�¼��

	CDXFile* m_File;                                 // �����ļ�
	CString m_dataPath;                             // ������Ŀ¼
	CString m_backupPath;                           // ������Ŀ¼
	_DATE   m_saveDate;                             // �����ѯ����
    CCriticalSection m_lock;                        // �ؼ�������

	void AddRefundApplyRecord(BYTE*,long,long,BYTE status = 0);     // ����˿������¼
	bool IsRecordExist(BYTE* );                                     // �ж��˿������¼�Ƿ����
	void ClearAllRecord();                                          // ����˿������¼
	bool ReadFile();                                                // ��ȡ�ļ�

	void SetFileDate();								// д���ļ��洢����
	void WriteFile();                               // д�ļ�
	void SaveToFile();
	CRefundApplyResult(void);
	~CRefundApplyResult();
	CRefundApplyResult(const CRefundApplyResult&);
	CRefundApplyResult& operator=(const CRefundApplyResult&);
};
