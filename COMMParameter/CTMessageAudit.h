#pragma once
#include "CParameterCommandExp.h"
#include "Xfile.h"
#include <afxmt.h>
#include "dxfile.h"


#define thePEKG_AUDIT    CTMessageAudit::GetInstance()       // CTMessageAudit��Ψһ�ӿ�

/**
@brief ���ݰ����Master
*/
class PARAMETERCOMMAND_DECLSPEC  CTMessageAudit
{
	
	public:

		// �����ݽṹ��
		typedef struct _Pekg_info{
			BYTE sPekgId[14];   
			WORD iRecordCnt;
			_Pekg_info()
			{
				memset(sPekgId,0x00,14);
				iRecordCnt = 0;
			}   
		} PEKG_INFO;

		bool GetPekgAuditMessage(VARIABLE_DATA& binfileData);		// ��ȡ��������Ƶ���
		bool Internalize(CString dataPath, CString backupPath);     // ���ڻ�
		void AddWriteFile(PEKG_INFO);								// ���°�����ļ�
		void SetStartDateTime(_DATE_TIME);							// ������ƿ�ʼʱ��
		void SetEndDateTime(_DATE_TIME);							// ������ƽ���ʱ��
		void ClearAuditData();										// �������ļ�����
		bool HasStartDateTime();									// �Ƿ����ÿ�ʼʱ��
		bool HasEndDateTime();										// �Ƿ����ÿ�ʼʱ��

		static CTMessageAudit &GetInstance()
		{
			return theInstance;
		}

	private:

		enum {
			MIN_FILE_LENGTH = 18,						// ���ݰ�����ļ���С����
		};

		static  CTMessageAudit theInstance;    // �����Ψһ�ӿ�

		// ˽�г�Ա����
		CCriticalSection m_criticalSection;                             // �ؼ�������
		CDXFile  *m_pekgAuditFile;        // ������ļ�
		vector<PEKG_INFO>  m_pekgList;   // ���ݰ���Ϣ
		_DATE_TIME   m_StartDateTime;    // ��ƿ�ʼʱ��
		_DATE_TIME   m_EndDateTime;      // ��ƽ���ʱ��
		DWORD m_PekgCnt;				 // ���ݰ�����

		CTMessageAudit(void);
		~CTMessageAudit();
		CTMessageAudit(const CTMessageAudit&);
		CTMessageAudit& operator=(const CTMessageAudit&);
	};

