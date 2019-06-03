#pragma once

#include "CParameterCommandExp.h"
#include "dxfile.h"


#define theREGI_INFO    CCTLOGRegister::GetInstance()              ///< CCTLOGCountInfo��Ψһ�ӿ�

/**
@brief Register��Ϣ

---------------------------------------------------------------
������ά�����ݵ���ϸ����

Module ID		�C���ڲ�����(MC)
0x10	        Ԥ��
0x11	        Ԥ��
0x12	        ��Ʊ����(TH)
0x13	        Ԥ��
0x14	        Ԥ��
0x15	        Ԥ��
0x16	        Ԥ��
0x17	        Ԥ��
0x18	        Ԥ��
0x19	        Ԥ��
0x1A~0x1F	    Ԥ��

Condition:      TH
Condition	    헡�Ŀ	�ڡ���
000	            Ԥ��
��	            Ԥ��
029	            Ԥ��
030             Ʊ��1����Ʊ����
031             Ʊ��2����Ʊ����
032             ��Ʊ������
033             Ԥ��
034	            Ԥ��
035	            Ԥ��
��	            Ԥ��
255	            Ԥ��

---------------------------------------------------------------
�������쳣������Ϣ

Condition	    �쳣����
000	            Total Count
��	            Total Count
255	            Total Count
---------------------------------------------------------------

�����Ǹ�������master�еĴ洢���
-------------------------------------------------------

��Ŀ����             ��ʼλ��     BYTE��    ����       

��¼��ʼʱ��             0          7       BCD        

ά������                 7       4��256     BIN

�쳣���ݴ洢����       1031         1       BIN

�쳣����               1032      6��256     BIN   
-------------------------------------------------------
*/

class PARAMETERCOMMAND_DECLSPEC  CCTLOGRegister
{
public:
	// ά������ Module ID
	typedef enum _maintain_module
	{
		MAINTAIN_UNUSED     = 0x00,                 ///< δʹ��
		MAINTAIN_TH         = 0x12,                 ///< ��Ʊ����(TH)
	} MAINTAIN_MODULE;

	// ά������ Condition
	typedef enum _maintain_condition
	{
		MAGAZINE_ONE_CNT           = 30,            ///<  Ʊ��1����Ʊ����
		MAGAZINE_TWO_CNT   	       = 31,            ///<  Ʊ��2����Ʊ����
		INVALIDATED_TICKET_CNT     = 32,	        ///<  ��Ʊ������
	} MAINTAIN_CONDITION;

	// �쳣���� �ṹ
	typedef struct _error_info
	{
		WORD EC_code;                               // ������Ϣ��ģ��ţ�λ��
		ULONG count;                                // ������Ϣ�ĸ���
		_error_info(){
			EC_code = 0;
			count = 0;
		}

	} ERROR_INFO;

	// �쳣���� Module ID
	typedef enum _error_module
	{
		ERROR_UNUSED        = 0x00,                 ///< δʹ��
		ERROR_APLI          = 0x11,                 ///< �����ڲ�����
		ERROR_TH            = 0x12,                 ///< TH����
		ERROR_ICRW          = 0x13,                 ///< IC RW����
	} ERROR_MODULE;

	bool Internalize(CString dataPath, CString backupPath);            // ���ڻ�

	void GetRecordTime(_DATE_TIME&);                                   // ȡ�ü�¼ʱ��

	ULONG GetMaintDataCount(MAINTAIN_CONDITION,MAINTAIN_MODULE&);      // �õ�ά�����ݵļ���
	void UpdateMaintDataCount(MAINTAIN_CONDITION,ULONG count=1);       // ����ά�����ݵļ���

	bool ClearMaintData(MAINTAIN_CONDITION);                           // ���ָ����ά������
	bool ClearAllMaintData();                                          // ������е�ά������

	void GetErrDataCount(LPBYTE);                                      // �õ��쳣��Ϣ
	void UpdateErrDataCount(WORD);                                     // �����쳣��Ŀ

	static CCTLOGRegister&  GetInstance()
	{
		return theInstance;
	}

private:

	enum {
		REGISTER_LEN           = 3+1024+1+256*6,		     			///< �ļ�����
		REGISTER_BLOCK_SIZE    = 4,										///< �������ݵ�ÿһ��Ĵ�С
		ERROR_BLOCK_SIZE       = 6,										///< maintenance���ݵ�ÿһ��Ĵ�С
		ERROR_INFO_SIZE        = 256*6,                                 ///< ������Ϣ��ռ��BYTE��
	};

	// �ֶ�����λ��
	enum {
		START_TIME_POS         = 0,                                     // ��¼��ʼʱ��λ��
		MAINTAIN_POS           = 7,                                     // ά������λ��
		ERROR_KINDS_POS        = 1031,                                  // ������������λ��
		ERROR_INFO_POS         = 1032                                   // ������Ϣλ��
	};    

	static  CCTLOGRegister theInstance;

	CDXFile* m_file;
	char m_fileData[REGISTER_LEN];                                  // ��ǰ�ļ�����
	CString m_dataPath;                                             // ������Ŀ¼
	CString m_backupPath;                                           // ������Ŀ¼

	_DATE_TIME m_registerTime;                                      // ��¼ʱ��

	ULONG m_maintCntTotal[256];                                     // ά�����ݵļ���
	ERROR_INFO m_errorInfo[256];                                    // �쳣��Ϣ
	BYTE m_errorKindCnt;                                            // ���������ܼ�

	bool SetRecordTime(_DATE_TIME);                                 // ���ü�¼ʱ��

	bool ReadFile();                                                // ��ȡ�ļ�
	bool OverWriteFile(const char* , ULONG );                       // �����ļ�

	CCTLOGRegister(void);
	~CCTLOGRegister();
	CCTLOGRegister(const CCTLOGRegister&);
	CCTLOGRegister& operator=(const CCTLOGRegister&);
};
