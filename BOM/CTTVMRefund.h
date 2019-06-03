#pragma once

#include "Xfile.h"


static const TCHAR* const TVM_REFUND_FILE_NAME				    = _T("master\\TVMRefundInfo.mst");      // BOM��TVM�����˿�

#define theTVMRefund CCTVMRefund::GetInstance()       ///< CCTLOGPresellTicketInfo��Ψһ�ӿ�

/**
@brief Ԥ��Ʊ��ϢMaster
��Master�ļ���Ҫ�洢����Ԥ��ƱID
*/
class  CCTVMRefund
{
public:

	// Ԥ��Ʊ�ṹ��
	typedef struct _tvm_error_ticket_id {
		CString csTVMErrorSierialNo;
		_tvm_error_ticket_id()
		{
			csTVMErrorSierialNo = "";
		}   
	}TVM_ERROR_TICKET_ID;

	bool Internalize(CString dataPath, CString backupPath);				// ���ڻ�
	bool UpdateTVMTicketInfo(vector<TVM_ERROR_TICKET_ID>);				// ����TVM�ļ�
	bool CheckIsTVMTicket(CString);										// �ж��Ƿ�ΪTVMƱ

	static CCTVMRefund &GetInstance()
	{
		return theInstance;
	}

private:

	enum {
		MIN_FILE_LENGTH = 4,						///< Ԥ��Ʊ��Ϣ�ļ���С����
	};

	static  CCTVMRefund theInstance;    // �����Ψһ�ӿ�

	// ˽�г�Ա����
	void AddWriteFile(vector<TVM_ERROR_TICKET_ID>);   // ׷���ļ�
	void SetFileDate();                             // ����Ԥ��Ʊ�ļ�������

	// ˽�г�Ա����
	vector<TVM_ERROR_TICKET_ID>  m_preTicketNoList;   // Ԥ��Ʊ�ڿ����
	CXFile  *m_presellFile;                          // Ԥ��Ʊ�ļ�
	_DATE   m_curDate;                               // Ԥ��Ʊ�ļ��д������

	CCTVMRefund(void);
	~CCTVMRefund();
	CCTVMRefund(const CCTVMRefund&);
	CCTVMRefund& operator=(const CCTVMRefund&);
};
