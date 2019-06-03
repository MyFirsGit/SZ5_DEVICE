#pragma once

#include "CParameterCommandExp.h"
#include "dxfile.h"


#define thePRE_TK    CCTLOGPresellTicketInfo::GetInstance()       ///< CCTLOGPresellTicketInfo��Ψһ�ӿ�

/**
@brief Ԥ��Ʊ��ϢMaster ��Master�ļ���Ҫ�洢����Ԥ��ƱID
*/
class PARAMETERCOMMAND_DECLSPEC  CCTLOGPresellTicketInfo
{
public:

	// Ԥ��Ʊ�ṹ��
	typedef struct _presell_ticket_id {
		CardSerialNumber_t presellId;       // Ԥ��ƱID
		_presell_ticket_id()
		{
			presellId = 0;
		}   
	} PRESELL_TICKET_ID;

	bool Internalize(CString dataPath, CString backupPath);          // ���ڻ�
	bool UpdatePresellTicketInfo(vector<PRESELL_TICKET_ID>);         // ����Ԥ��Ʊ�ļ�
	bool CheckIsPreTicket(CardSerialNumber_t);                       // �ж��Ƿ�ΪԤ��Ʊ

	static CCTLOGPresellTicketInfo &GetInstance()
	{
		return theInstance;
	}

private:

	enum {
		MIN_FILE_LENGTH = 4,						///< Ԥ��Ʊ��Ϣ�ļ���С����
	};

	static  CCTLOGPresellTicketInfo theInstance;    // �����Ψһ�ӿ�

	// ˽�г�Ա����
	void AddWriteFile(vector<PRESELL_TICKET_ID>);   // ׷���ļ�
	void SetFileDate();                             // ����Ԥ��Ʊ�ļ�������

	// ˽�г�Ա����
	vector<PRESELL_TICKET_ID>  m_preTicketNoList;   // Ԥ��Ʊ�ڿ����
	CDXFile  *m_presellFile;                          // Ԥ��Ʊ�ļ�
	_DATE   m_curDate;                               // Ԥ��Ʊ�ļ��д������

	CCTLOGPresellTicketInfo(void);
	~CCTLOGPresellTicketInfo();
	CCTLOGPresellTicketInfo(const CCTLOGPresellTicketInfo&);
	CCTLOGPresellTicketInfo& operator=(const CCTLOGPresellTicketInfo&);
};
